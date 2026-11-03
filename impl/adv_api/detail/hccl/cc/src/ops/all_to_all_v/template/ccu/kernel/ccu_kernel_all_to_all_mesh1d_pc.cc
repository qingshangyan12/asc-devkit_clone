/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kernel_all_to_all_mesh1d_pc.h"
#include "ccu_kernel_all_to_all_mesh1d.h"
#include "ccu_func_dl.hpp"

namespace mc2_ops_hccl {
namespace pc {
CcuResult CcuAlltoAllMesh1DKernel(CcuKernelArg arg)
{
    const auto* kernelArg = static_cast<const CcuKernelArgAlltoAllMesh1D*>(arg);
    ccu::Func func(
        [kernelArg](ccu::Array<ccu::Variable>& args) {
            mc2_ops_hccl::CcuAlltoAllMesh1DKernel(
                args[1], args[2], args[KFC_FUNC_TOKEN_ARG_INDEX], args[4], args[5], args[6], args[7], args[8], args[9],
                args[10], args[11], kernelArg->channels, kernelArg->channelCount,
                static_cast<uint32_t>(kernelArg->rankSize), kernelArg->rankId, kernelArg->loadFromMem);
        },
        CCU_FUNC_TASK_ARG_MAX, KFC_FUNC_ALL_TO_ALL_MESH1D_NAME);
    return CCU_SUCCESS;
}
} // namespace pc
} // namespace mc2_ops_hccl
