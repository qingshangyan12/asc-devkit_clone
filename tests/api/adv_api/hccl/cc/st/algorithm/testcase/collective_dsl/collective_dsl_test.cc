// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#include "all_gather_algorithms.h"
#include "resource_plan.h"
#include "trace_backend.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace mc2_ops_hccl::dsl;

namespace {

struct RankMemory {
    std::vector<uint8_t> input;
    std::vector<uint8_t> output;
};

uint64_t EventKey(const Event& event) { return (static_cast<uint64_t>(event.rank) << 32U) | event.id; }

class PlanSimulator {
public:
    PlanSimulator(uint32_t rankSize, uint64_t bytesPerRank)
        : rankSize_(rankSize), bytesPerRank_(bytesPerRank), memory_(rankSize)
    {
        for (Rank rank = 0; rank < rankSize_; ++rank) {
            memory_[rank].input.resize(bytesPerRank_);
            memory_[rank].output.assign(rankSize_ * bytesPerRank_, 0xff);
            for (uint64_t i = 0; i < bytesPerRank_; ++i) {
                memory_[rank].input[i] = static_cast<uint8_t>(rank * 31U + i);
            }
        }
    }

    void Run(const std::vector<Plan>& plans)
    {
        using QueueKey = std::pair<Rank, Queue>;
        std::map<QueueKey, std::vector<const Instruction*>> queues;
        for (const Plan& plan : plans) {
            for (const Instruction& ins : plan.instructions) {
                queues[{plan.rank, ins.queue}].push_back(&ins);
            }
        }

        std::map<QueueKey, size_t> next;
        size_t remaining = 0;
        for (const auto& entry : queues) {
            next[entry.first] = 0;
            remaining += entry.second.size();
        }

        while (remaining != 0) {
            bool progressed = false;
            const std::set<uint64_t> completedAtTickStart = completed_;
            for (const auto& entry : queues) {
                size_t& index = next[entry.first];
                if (index == entry.second.size()) {
                    continue;
                }
                const Instruction& ins = *entry.second[index];
                if ((ins.op == Op::WAIT || ins.op == Op::COMPLETE) &&
                    !AllCompleted(ins.dependencies, completedAtTickStart)) {
                    continue;
                }
                Execute(ins);
                ++index;
                --remaining;
                progressed = true;
            }
            if (!progressed) {
                throw std::runtime_error("plan is deadlocked");
            }
        }

        for (const Plan& plan : plans) {
            if (!AllCompleted(plan.finalEvents, completed_)) {
                throw std::runtime_error("plan finished before its final events");
            }
        }
    }

    void CheckAllGather() const
    {
        for (Rank dst = 0; dst < rankSize_; ++dst) {
            for (Rank owner = 0; owner < rankSize_; ++owner) {
                const auto begin = memory_[dst].output.begin() + owner * bytesPerRank_;
                if (!std::equal(begin, begin + bytesPerRank_, memory_[owner].input.begin())) {
                    throw std::runtime_error("AllGather result mismatch");
                }
            }
        }
    }

private:
    bool AllCompleted(const std::vector<Event>& events, const std::set<uint64_t>& completed) const
    {
        for (const Event& event : events) {
            if (completed.count(EventKey(event)) == 0) {
                return false;
            }
        }
        return true;
    }

    uint8_t* Address(const Slice& slice)
    {
        std::vector<uint8_t>& buffer =
            slice.buffer == Buffer::INPUT ? memory_.at(slice.rank).input : memory_.at(slice.rank).output;
        const uint64_t base = slice.buffer == Buffer::INPUT ? 0 : slice.owner * bytesPerRank_;
        if (base + slice.offset + slice.bytes > buffer.size()) {
            throw std::out_of_range("simulated slice is out of bounds");
        }
        return buffer.data() + base + slice.offset;
    }

    void Execute(const Instruction& ins)
    {
        if (ins.op == Op::WAIT || ins.op == Op::COMPLETE) {
            return;
        }
        if (ins.op == Op::LOCAL_COPY) {
            std::copy(Address(ins.src), Address(ins.src) + ins.src.bytes, Address(ins.dst));
        } else {
            for (size_t i = 0; i < ins.txSrc.size(); ++i) {
                std::copy(Address(ins.txSrc[i]), Address(ins.txSrc[i]) + ins.txSrc[i].bytes, Address(ins.txDst[i]));
            }
        }
        completed_.insert(EventKey(ins.completion));
    }

    uint32_t rankSize_;
    uint64_t bytesPerRank_;
    std::vector<RankMemory> memory_;
    std::set<uint64_t> completed_;
};

Dimension FullDimension(uint32_t rankSize)
{
    Dimension result;
    Ranks all;
    for (Rank rank = 0; rank < rankSize; ++rank) {
        all.push_back(rank);
    }
    result.groups.assign(rankSize, all);
    return result;
}

Dimension ContiguousDimension(uint32_t rankSize, uint32_t groupSize)
{
    Dimension result;
    result.groups.resize(rankSize);
    for (Rank rank = 0; rank < rankSize; ++rank) {
        const Rank first = rank / groupSize * groupSize;
        for (Rank member = first; member < first + groupSize; ++member) {
            result.groups[rank].push_back(member);
        }
    }
    return result;
}

Dimension StridedDimension(uint32_t rankSize, uint32_t stride)
{
    Dimension result;
    result.groups.resize(rankSize);
    for (Rank rank = 0; rank < rankSize; ++rank) {
        const Rank column = rank % stride;
        for (Rank member = column; member < rankSize; member += stride) {
            result.groups[rank].push_back(member);
        }
    }
    return result;
}

AllGatherArgs ArgsFor(Rank rank, uint32_t rankSize, uint64_t bytes)
{
    AllGatherArgs args;
    args.rank = rank;
    args.rankSize = rankSize;
    args.sendBytes = bytes;
    return args;
}

ChannelLayout ChannelsFor(Rank rank, uint32_t rankSize)
{
    ChannelLayout channels;
    channels.byPeer.resize(rankSize);
    for (Rank peer = 0; peer < rankSize; ++peer) {
        if (peer != rank) {
            channels.byPeer[peer] = {{peer, 0, 1}, {peer, 1, 1}};
        }
    }
    return channels;
}

template <typename Build>
void CheckAlgorithm(uint32_t rankSize, uint64_t bytes, Build build)
{
    std::vector<Plan> plans;
    for (Rank rank = 0; rank < rankSize; ++rank) {
        plans.push_back(build(ArgsFor(rank, rankSize, bytes), ChannelsFor(rank, rankSize)));
    }
    PlanSimulator simulator(rankSize, bytes);
    simulator.Run(plans);
    simulator.CheckAllGather();
}

void TestTrace()
{
    const Dimension full = FullDimension(4);
    const Plan plan = SoleMesh(ArgsFor(0, 4, 16), ChannelsFor(0, 4), full);
    std::ostringstream first;
    std::ostringstream second;
    WriteTrace(plan, first);
    WriteTrace(plan, second);

    std::istringstream expected(first.str());
    std::istringstream actual(second.str());
    std::string difference;
    if (!CompareTrace(expected, actual, difference)) {
        throw std::runtime_error(difference);
    }
    if (first.str().find("op=exchange send=1/0 recv=1/0") == std::string::npos) {
        throw std::runtime_error("trace omitted channel information");
    }
}

void TestResourcePlan()
{
    const Plan plan = SoleMesh(ArgsFor(0, 4, 16), ChannelsFor(0, 4), FullDimension(4));
    const ResourcePlan resources = CalcResources(plan);
    if (resources.channels.size() != 6U) {
        throw std::runtime_error("resource plan omitted a MultiLink channel");
    }
    if (resources.queueCount == 0U || resources.eventCount == 0U) {
        throw std::runtime_error("resource plan omitted queue or event resources");
    }
}

} // namespace

int main()
{
    const Dimension full4 = FullDimension(4);
    CheckAlgorithm(4, 32, [&](const AllGatherArgs& args, const ChannelLayout& channels) {
        return SoleMesh(args, channels, full4);
    });
    CheckAlgorithm(4, 32, [&](const AllGatherArgs& args, const ChannelLayout& channels) {
        return SoleNhr(args, channels, full4);
    });
    const Dimension full5 = FullDimension(5);
    CheckAlgorithm(5, 32, [&](const AllGatherArgs& args, const ChannelLayout& channels) {
        return SoleNhr(args, channels, full5);
    });
    CheckAlgorithm(4, 32, [&](const AllGatherArgs& args, const ChannelLayout& channels) {
        return ConcurrentMeshNhr(args, channels, full4, 12);
    });

    const Dimension mesh8 = ContiguousDimension(8, 4);
    const Dimension nhr8 = StridedDimension(8, 4);
    CheckAlgorithm(8, 32, [&](const AllGatherArgs& args, const ChannelLayout& channels) {
        return ParallelMeshNhr(args, channels, mesh8, nhr8, 12);
    });
    CheckAlgorithm(8, 32, [&](const AllGatherArgs& args, const ChannelLayout& channels) {
        return PipelineMeshNhr(args, channels, mesh8, nhr8, 8);
    });

    TestTrace();
    TestResourcePlan();
    std::cout << "collective_dsl_test: PASS\n";
    return 0;
}
