// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#ifndef MC2_COLLECTIVE_DSL_PLAN_H
#define MC2_COLLECTIVE_DSL_PLAN_H

#include <cstdint>
#include <string>
#include <vector>

namespace mc2_ops_hccl {
namespace dsl {

using Rank = uint32_t;
using Queue = uint32_t;

enum class Buffer { INPUT, OUTPUT };

// offset 是 owner 槽内偏移；OUTPUT 的实际偏移由 backend 按 owner 和槽跨度计算。
struct Slice {
    Rank rank = 0;
    Buffer buffer = Buffer::INPUT;
    Rank owner = 0;
    uint64_t offset = 0;
    uint64_t bytes = 0;
};

struct Event {
    Rank rank = 0;
    uint32_t id = 0;
};

inline bool operator==(const Event& lhs, const Event& rhs) { return lhs.rank == rhs.rank && lhs.id == rhs.id; }

// path 是 ChannelLayout::byPeer[peer] 的下标；weight 只负责 MultiLink 数据切分。
struct Channel {
    Rank peer = 0;
    uint32_t path = 0;
    uint32_t weight = 1;
};

enum class Op { LOCAL_COPY, EXCHANGE, WAIT, COMPLETE };

struct Instruction {
    Op op = Op::LOCAL_COPY;
    Queue queue = 0;
    Slice dst;
    Slice src;

    // EXCHANGE 同时描述本 rank 发出的 Write 和期望从对端收到的数据。
    Channel send;
    Channel recv;
    std::vector<Slice> txDst;
    std::vector<Slice> txSrc;
    std::vector<Slice> rxDst;
    std::vector<Slice> rxSrc;

    Event completion;
    std::vector<Event> dependencies;
};

struct Plan {
    std::string name;
    Rank rank = 0;
    std::vector<Instruction> instructions;
    std::vector<Event> finalEvents;
};

class Builder {
public:
    Builder(const std::string& name, Rank rank);

    Event LocalCopy(Queue queue, const Slice& dst, const Slice& src);
    Event Exchange(
        Queue queue, const Channel& send, const Channel& recv, const std::vector<Slice>& txDst,
        const std::vector<Slice>& txSrc, const std::vector<Slice>& rxDst, const std::vector<Slice>& rxSrc);
    void Wait(Queue queue, const std::vector<Event>& events);
    Plan Finish(const std::vector<Event>& events);

private:
    Event NewEvent();

    Plan plan_;
    uint32_t nextEvent_ = 1;
};

} // namespace dsl
} // namespace mc2_ops_hccl

#endif
