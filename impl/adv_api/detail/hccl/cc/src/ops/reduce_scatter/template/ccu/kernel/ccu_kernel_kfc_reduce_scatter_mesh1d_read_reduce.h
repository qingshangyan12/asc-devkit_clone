/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_READ_REDUCE_H
#define HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_READ_REDUCE_H

#include <ios>
#include "utils.h"
#include "ccu_kernel.h"
#include "ccu_kernel_utils.h"
#include "ccu_kernel_alg_base.h"

namespace mc2_ops_hccl {
using namespace hcomm;

// SoleMeshChunkPipelined 的 KFC server 子内核，移植自 hccl rs-sched-perf 分支的
// CcuReduceScatterMesh1DReadReducePipelinedKernel（"Change to cycle" 终态：seed LocalCopy +
// wave×chunk 双层 ReadReduce，per-chunk 事件流水（复用前等待），peer 按 (chunk+wave)%peerCount+1
// 轮转以均衡各通道负载）。
// 数据参数由 KFC dispatch 从 HBM 单帧逐槽转发（布局见 KfcReduceScatterMeshChunkPipelinedParamIndex）；
// token 走 dispatch 的 ctx.token 不占槽。chunkCount 不占 HBM 槽（恒 4*(rankSize-1)，dispatch 按
// kernelArg.rankSize 计算后以标量转发）；chunk 尺寸以紧凑编码下发（base/align/extraUnits/tail），
// kernel 内按 sizes[i] = base + (i < extraUnits ? align : 0)、末块再加 tail 重建，与宿主
// CcuPrepareForReduceScatterChunkPipelinedM2M（= hccl 模板 4096→128→dataTypeSize 级联对齐切分）
// 逐值一致。
CcuResult CcuReduceScatterMesh1DReadReducePipelinedKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable localInputOffset,
    ccu::Variable sliceSize, ccu::Variable chunkBaseSize, ccu::Variable chunkAlign, ccu::Variable chunkExtraUnits,
    ccu::Variable chunkTail, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize,
    uint32_t rankId, uint32_t chunkCount, const HcclDataType& dataType, const HcclDataType& outputType,
    const HcclReduceOp& reduceType);

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_READ_REDUCE_H
