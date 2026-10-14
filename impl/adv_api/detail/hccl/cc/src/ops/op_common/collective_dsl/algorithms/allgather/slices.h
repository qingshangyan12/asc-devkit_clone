// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#pragma once
#include "allgather.h"
namespace asc { namespace ir {
// AllGather 内部共用的地址构造，不隐藏 peer 公式或调度。
// owners[p] 列出 p 在当前 stage 开始时持有的原始 owner。
Groups initialOwners(uint32_t ranks);
Event placeInput(Queue&, Rank, const AllGather&);
MeshSlices meshSlices(Rank, const Ranks&, const Groups& owners, const AllGather&,
                      uint64_t begin, uint64_t bytes, Op);
NhrSlices nhrSlices(const Groups& owners, const AllGather&, uint64_t begin, uint64_t bytes, Op);
} }
