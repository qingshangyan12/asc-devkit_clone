/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_MS_ACCUM_H
#define HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_MS_ACCUM_H

#include <ios>
#include "utils.h"
#include "ccu_kernel.h"
#include "ccu_kernel_utils.h"
#include "ccu_kernel_alg_base.h"

namespace mc2_ops_hccl {
using namespace hcomm;

// SoleMeshChunkTwoShot 的 KFC server 子内核，移植自 hccl rs-sched-perf 分支的
// CcuReduceScatterMesh1DMsAccumKernel：本地份 + 各 peer 份一次性 GroupReduce（LoopGroup 多 Op
// 归并），无需 scratch。数据参数由 KFC dispatch 从 HBM 单帧逐槽转发（布局见
// KfcReduceScatterMeshChunkTwoShotParamIndex）；token 走 dispatch 的 ctx.token 不占槽；
// goSize 四元组对应宿主 CalcGoSize(sliceSize, MS_ACCUM_REDUCE_SCATTER_LOOP_COUNT, 4096) 口径。
CcuResult CcuReduceScatterMesh1DMsAccumKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable localInputOffset,
    ccu::Variable goAddrOffset, ccu::Variable goLoopParam, ccu::Variable goParallelParam, ccu::Variable goResidual,
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    const HcclDataType& dataType, const HcclDataType& outputType, const HcclReduceOp& reduceType);

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_MS_ACCUM_H
