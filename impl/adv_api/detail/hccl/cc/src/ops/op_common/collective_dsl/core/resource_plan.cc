// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#include "resource_plan.h"

#include <algorithm>
#include <set>
#include <utility>

namespace mc2_ops_hccl {
namespace dsl {

ResourcePlan CalcResources(const Plan& plan)
{
    ResourcePlan result;
    std::set<std::pair<Rank, uint32_t>> channels;
    std::set<Queue> queues;
    uint32_t maxEvent = 0;
    for (const Instruction& ins : plan.instructions) {
        if (ins.op != Op::COMPLETE) {
            queues.insert(ins.queue);
        }
        if (ins.op == Op::EXCHANGE) {
            channels.insert({ins.send.peer, ins.send.path});
            channels.insert({ins.recv.peer, ins.recv.path});
        }
        maxEvent = std::max(maxEvent, ins.completion.id);
        for (const Event& event : ins.dependencies) {
            maxEvent = std::max(maxEvent, event.id);
        }
    }
    for (const auto& channel : channels) {
        result.channels.push_back(ChannelNeed{channel.first, channel.second});
    }
    result.queueCount = static_cast<uint32_t>(queues.size());
    result.eventCount = maxEvent;
    return result;
}

} // namespace dsl
} // namespace mc2_ops_hccl
