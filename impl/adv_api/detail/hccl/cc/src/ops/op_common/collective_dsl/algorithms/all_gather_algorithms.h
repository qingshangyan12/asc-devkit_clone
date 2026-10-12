// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#ifndef MC2_COLLECTIVE_DSL_ALL_GATHER_ALGORITHMS_H
#define MC2_COLLECTIVE_DSL_ALL_GATHER_ALGORITHMS_H

#include "all_gather_primitives.h"

namespace mc2_ops_hccl {
namespace dsl {

Plan SoleMesh(const AllGatherArgs& args, const ChannelLayout& channels, const Dimension& mesh);
Plan SoleNhr(const AllGatherArgs& args, const ChannelLayout& channels, const Dimension& nhr);
Plan ConcurrentMeshNhr(
    const AllGatherArgs& args, const ChannelLayout& channels, const Dimension& whole, uint64_t split);
Plan ParallelMeshNhr(
    const AllGatherArgs& args, const ChannelLayout& channels, const Dimension& mesh, const Dimension& nhr,
    uint64_t split);
Plan PipelineMeshNhr(
    const AllGatherArgs& args, const ChannelLayout& channels, const Dimension& mesh, const Dimension& nhr,
    uint64_t chunk);

} // namespace dsl
} // namespace mc2_ops_hccl

#endif
