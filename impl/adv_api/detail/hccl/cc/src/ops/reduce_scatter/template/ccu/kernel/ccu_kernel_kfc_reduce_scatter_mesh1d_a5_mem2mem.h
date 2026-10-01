/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_A5_MEM2MEM_H
#define HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_A5_MEM2MEM_H

#include <ios>
#include "utils.h"
#include "ccu_kernel.h"
#include "ccu_kernel_utils.h"
#include "ccu_kernel_alg_base.h"

namespace mc2_ops_hccl {
using namespace hcomm;

// A5(Ascend950) 单server CCU_SCHED 2~8卡专用 ReduceScatter kernel（大消息双缓冲流水线版）。
// rankSize <= 8 保证：1) 走 GroupLocalReduce 聚合规约路径；2) 所有 rank 信号可容纳在单个 CKE（16 bit）内。
constexpr uint32_t RS_A5_MIN_RANK_SIZE = 2;
constexpr uint32_t RS_A5_MAX_RANK_SIZE = 8;

// 21 个 ccu::Variable 形参（与 KfcReduceScatterSoleMeshA5ParamIndex 的 [1..21] 一一对应）。
CcuResult CcuKfcReduceScatterMesh1DA5Mem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable scratch,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable currentRankSliceOutputOffset,
    ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride, ccu::Variable normalSliceSize,
    ccu::Variable lastSliceSize, ccu::Variable scratchRepeatStride, ccu::Variable repeatNum, ccu::Variable goAddrOffset,
    ccu::Variable goLoopParam, ccu::Variable goParallelParam, ccu::Variable goResidual, ccu::Variable goBAddrOffset,
    ccu::Variable goBLoopParam, ccu::Variable goBParallelParam, ccu::Variable goBResidual,
    ccu::Variable scratchSlotStride, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize,
    uint32_t rankId, const HcclDataType& dataType, const HcclDataType& outputType, const HcclReduceOp& reduceType);

} // namespace mc2_ops_hccl
#endif // HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_A5_MEM2MEM_H
