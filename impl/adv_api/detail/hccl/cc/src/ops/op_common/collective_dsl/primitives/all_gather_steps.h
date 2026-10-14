// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_DSL_ALL_GATHER_STEPS_H
#define MC2_DSL_ALL_GATHER_STEPS_H
#include "collective_plan.h"
namespace mc2_ops_hccl { namespace dsl {
// 一次双向交换；txOwners 是发送方已经持有的槽，rxOwners 是本轮新接收的槽。
// channel、地址和传输模式由 engine 绑定。step 边界表示数据完成依赖。
struct ExchangeStep {
    Rank sendPeer;
    Rank recvPeer;
    Ranks txOwners;
    Ranks rxOwners;
    Interval bytes;
};
using TransferRound = std::vector<ExchangeStep>;
std::vector<TransferRound> ExpandAllGather(const Stage& stage, Rank rank);
} }
#endif
