/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_KERNEL_KFC_ALL_REDUCE_MESH_1D_MEM2MEM_H
#define HCCL_CCU_KERNEL_KFC_ALL_REDUCE_MESH_1D_MEM2MEM_H

#include <ios>
#include "utils.h"
#include "ccu_kernel.h"
#include "ccu_kernel_utils.h"
#include "ccu_kernel_alg_base.h"

namespace mc2_ops_hccl {
using namespace hcomm;

struct CcuKernelArgAllReduceKfc : CcuKernelArgBase {
    uint64_t rankSize;
    uint32_t rankId;
    OpParam opParam;
    std::vector<std::vector<uint32_t>> subCommRanks;
};

struct KfcAllReduceMesh1DMem2MemContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};

    HcclDataType dataType;
    HcclDataType outputDataType;
    HcclReduceOp reduceOp;

    std::vector<ccu::Variable> input;
    std::vector<ccu::Variable> output;
    std::vector<ccu::Variable> token;
    ccu::Variable myScratch;
    ccu::Variable currentRankSliceInputOffset;
    ccu::Variable currentRankSliceOutputOffset;
    ccu::Variable normalSliceSize;
    ccu::Variable lastSliceSize;
    ccu::Variable mySliceSize;
    ccu::Variable sliceOffset;
    ccu::Variable isInputOutputEqual;
    ccu::Variable sliceSize;
    GroupOpSizeVars goSize;
    ccu::Variable chunkSize;
    ccu::Variable currentSliceSize;
    ccu::Variable tailSize;
    ccu::Variable chunkLoopNum;
    GroupOpSizeVars fullGoSize;
    GroupOpSizeVars tailGoSize;

    std::vector<ccu::Event> events;

    ccu::LocalAddr srcMem;
    ccu::LocalAddr localDstMem;
    ccu::RemoteAddr remoteDstMem;
    std::vector<ccu::RemoteAddr> reduceScatterSrc;
    std::vector<ccu::LocalAddr> reduceScatterDst;
};

CcuResult CcuKfcAllReduceMesh1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable scratch,
    ccu::Variable currentRankSliceInputOffset, ccu::Variable currentRankSliceOutputOffset,
    ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable mySliceSize, ccu::Variable sliceOffset,
    ccu::Variable isInputOutputEqual, ccu::Variable goSize0, ccu::Variable goSize1, ccu::Variable goSize2,
    ccu::Variable goSize3, ccu::Variable chunkSize, ccu::Variable tailSize, ccu::Variable chunkLoopNum,
    ccu::Variable fullGoSize0, ccu::Variable fullGoSize1, ccu::Variable fullGoSize2, ccu::Variable fullGoSize3,
    ccu::Variable tailGoSize0, ccu::Variable tailGoSize1, ccu::Variable tailGoSize2, ccu::Variable tailGoSize3,
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    const HcclDataType& dataType, const HcclDataType& outputType, const HcclReduceOp& reduceType);

CcuResult ReduceLoopGroupV1(KfcAllReduceMesh1DMem2MemContext& ctx, ccu::LocalAddr srcOrg);
CcuResult ReduceLoopGroupV2(KfcAllReduceMesh1DMem2MemContext& ctx, ccu::LocalAddr srcOrg);
} // namespace mc2_ops_hccl
#endif // HCCL_CCU_KERNEL_KFC_ALL_REDUCE_MESH_1D_MEM2MEM_H
