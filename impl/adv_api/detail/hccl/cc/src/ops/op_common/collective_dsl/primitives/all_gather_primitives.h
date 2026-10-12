// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#ifndef MC2_COLLECTIVE_DSL_ALL_GATHER_PRIMITIVES_H
#define MC2_COLLECTIVE_DSL_ALL_GATHER_PRIMITIVES_H

#include "collective_plan.h"

namespace mc2_ops_hccl {
namespace dsl {

using Ranks = std::vector<Rank>;

// groups[p] 是 TopoMatch 为 rank p 生成的这一维有序通信域。
struct Dimension {
    std::vector<Ranks> groups;
    const Ranks& At(Rank rank) const;
};

struct AllGatherArgs {
    Rank rank = 0;
    uint32_t rankSize = 0;
    uint64_t sendBytes = 0;
};

// Channel 属于通信资源布局，不属于 AllGather 的算子参数。
struct ChannelLayout {
    std::vector<std::vector<Channel>> byPeer;
    const std::vector<Channel>& To(Rank peer) const;
};

struct Interval {
    uint64_t begin = 0;
    uint64_t end = 0;
};

// 原语只展开一次 Mesh 或 NHR 通信，不决定 Sole/Parallel 等算法组合。
std::vector<Event> Mesh(
    Builder& ir, Queue queue, const AllGatherArgs& args, const ChannelLayout& channels, const Ranks& ranks,
    const std::vector<Ranks>& owners, Interval data, const std::vector<Event>& dependsOn);
std::vector<Event> Nhr(
    Builder& ir, Queue queue, const AllGatherArgs& args, const ChannelLayout& channels, const Ranks& ranks,
    const std::vector<Ranks>& owners, Interval data, const std::vector<Event>& dependsOn);

} // namespace dsl
} // namespace mc2_ops_hccl

#endif
