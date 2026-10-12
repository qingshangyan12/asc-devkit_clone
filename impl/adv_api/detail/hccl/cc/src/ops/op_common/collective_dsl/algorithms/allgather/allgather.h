// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#pragma once
#include "exchange.h"
namespace asc { namespace ir {
using Groups = std::vector<Ranks>; // groups[p] 是 p 所在的有序 rank 域，不是 Channel
// input 是本 rank 的输入视图，bytes 自带长度；不再重复传 bytesPerRank。
// 它可以直接指向 output 的自己的槽位，故 in-place 不需要另一套算法。
struct AllGather { Slice input; Buffer output; };
// 算法参数仅包含数据和拓扑。split/chunkBytes 改变数据分割，留在算法层；
// link/jetty/unroll 等不改变集合语义的选项交给 backend，不放进这里。
Event allGatherSoleMesh(Program&, Rank, const AllGather&, const Groups&, Op = Op::Write);
Event allGatherSoleNhr(Program&, Rank, const AllGather&, const Groups&, Op = Op::Write);
Event allGatherSequence(Program&, Rank, const AllGather&, const Groups& mesh, const Groups& nhr, Op = Op::Write);
Event allGatherConcurrent(Program&, Rank, const AllGather&, const Groups& full, uint64_t split, Op = Op::Write);
Event allGatherParallel(Program&, Rank, const AllGather&, const Groups& mesh, const Groups& nhr,
                        uint64_t split, Op = Op::Write);
Event allGatherPipeline(Program&, Rank, const AllGather&, const Groups& mesh, const Groups& nhr,
                        uint64_t chunkBytes, Op = Op::Write);
} }
