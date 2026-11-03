/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_kfc_all_reduce_mesh1d_mem2mem_pc.h"
#include "ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.h"
#include "ccu_func_dl.hpp"

namespace mc2_ops_hccl {
namespace pc {
CcuResult CcuKfcAllReduceMesh1DMem2MemKernel(CcuKernelArg arg)
{
    const auto* kernelArg = static_cast<const CcuKernelArgAllReduceKfc*>(arg);
    ccu::Func func(
        [kernelArg](ccu::Array<ccu::Variable>& args) {
            mc2_ops_hccl::CcuKfcAllReduceMesh1DMem2MemKernel(
                args[1], args[2], args[KFC_FUNC_TOKEN_ARG_INDEX], args[4], args[5], args[6], args[7], args[8], args[9],
                args[10], args[11], args[12], args[13], args[14], args[15], args[16], args[17], args[18], args[19],
                args[20], args[21], args[22], args[23], args[24], args[25], args[26], kernelArg->channels,
                kernelArg->channelCount, static_cast<uint32_t>(kernelArg->rankSize), kernelArg->rankId,
                kernelArg->opParam.DataDes.dataType, kernelArg->opParam.DataDes.outputType,
                kernelArg->opParam.reduceType);
        },
        CCU_FUNC_TASK_ARG_MAX, KFC_FUNC_ALL_REDUCE_MESH1D_MEM2MEM_NAME);
    return CCU_SUCCESS;
}
} // namespace pc
} // namespace mc2_ops_hccl
