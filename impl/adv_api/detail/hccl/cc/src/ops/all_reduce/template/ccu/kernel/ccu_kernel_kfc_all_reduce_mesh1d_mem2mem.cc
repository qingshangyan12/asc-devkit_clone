/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.h"

#include "ccu_kernel_kfc_all_gather_mesh1d_mem2mem.h"
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.h"

namespace mc2_ops_hccl {

CcuResult CcuKfcAllReduceMesh1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable scratchAddr,
    ccu::Variable sliceOffset, ccu::Variable sliceSize, ccu::Variable goSize0, ccu::Variable goSize1,
    ccu::Variable goSize2, ccu::Variable goSize3, const ChannelHandle channels[], uint32_t channelCount,
    uint32_t rankSize, uint32_t rankId, const HcclDataType& dataType, const HcclDataType& outputType,
    const HcclReduceOp& reduceType)
{
    ccu::Variable reducedSliceAddr;
    reducedSliceAddr = outputAddr;
    reducedSliceAddr += sliceOffset;

    ccu::Variable repeatNum;
    repeatNum = UINT64_MAX - 1;
    CCU_CHK_RET(CcuReduceScatterMesh1DMem2MemKernel(
        inputAddr, reducedSliceAddr, tokenInfo, scratchAddr, sliceOffset, sliceSize, repeatNum, channels,
        channelCount, rankSize, rankId, dataType, outputType, reduceType));
    ccu::Variable isInputOutputEqual;
    isInputOutputEqual = 1;
    CCU_CHK_RET(CcuKfcAllGatherMesh1DMem2MemKernel(
        reducedSliceAddr, outputAddr, tokenInfo, sliceOffset, sliceSize, goSize0, goSize1, goSize2, goSize3,
        isInputOutputEqual, channels, channelCount, rankSize, rankId));
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl
