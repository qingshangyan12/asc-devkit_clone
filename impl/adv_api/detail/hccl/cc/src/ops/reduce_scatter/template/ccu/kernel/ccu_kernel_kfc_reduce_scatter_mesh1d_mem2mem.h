/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_MEM2MEM_H
#define HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_MEM2MEM_H

#include <ios>
#include "utils.h"
#include "ccu_kernel.h"
#include "ccu_kernel_utils.h"
#include "ccu_kernel_alg_base.h"

namespace mc2_ops_hccl {
using namespace hcomm;

struct CcuKernelArgKfcReduceScatterMesh1DMem2Mem : CcuKernelArgBase {
    uint64_t rankSize = 0;
    uint32_t rankId = 0;
    OpParam opParam;
    std::vector<std::vector<uint32_t>> subCommRanks;
};

struct KfcReduceScatterMesh1DMem2MemContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    HcclDataType dataType;
    HcclDataType outputDataType;
    HcclReduceOp reduceOp;

    std::vector<ccu::Variable> input;
    ccu::Variable output;
    std::vector<ccu::Variable> scratch;
    std::vector<ccu::Variable> token;
    ccu::Variable currentRankSliceInputOffset;
    ccu::Variable currentRankSliceOutputOffset;
    ccu::Variable chunkSize;
    ccu::Variable chunkLoopNum;
    ccu::Variable currentSliceSize;
    ccu::Variable tailSize;
    GroupOpSizeVars fullGoSize;
    GroupOpSizeVars tailGoSize;
    GroupOpSizeVars goSize; // 当前 chunk 选中的 goSize(full 或 tail)

    // 三阶段设计字段(从 hccl ReduceScatterMesh1DMem2MemContext 移植)
    ccu::Variable normalSliceSize;
    ccu::Variable lastSliceSize;
    ccu::Variable inputRepeatStride;
    ccu::Variable outputRepeatStride;
    ccu::Variable scratchRepeatStride;
    ccu::Variable repeatNum;
    ccu::Variable flag;
    ccu::Variable constVar1;
    ccu::Variable sliceSize;
    ccu::Variable readRepeatNum;
    ccu::Variable waitRepeatNum;
    std::vector<ccu::Event> events;

    ccu::LocalAddr myInput;
    std::vector<ccu::RemoteAddr> remoteInput;
    std::vector<ccu::LocalAddr> scratchMem;
    ccu::Event event;
};

CcuResult CcuReduceScatterMesh1DMem2MemKernel(CcuKernelArg arg);

} // namespace mc2_ops_hccl
#endif // HCCL_CCU_KERNEL_KFC_REDUCE_SCATTER_MESH_1D_MEM2MEM_H
