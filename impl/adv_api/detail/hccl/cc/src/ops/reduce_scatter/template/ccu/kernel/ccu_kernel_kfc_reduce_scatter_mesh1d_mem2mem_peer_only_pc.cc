/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem_peer_only_pc.h"
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem_peer_only.h"
#include "ccu_func_dl.hpp"
#include "../ccu_temp_kfc_reduce_scatter_mesh_1D_mem2mem.h"

namespace mc2_ops_hccl {
namespace pc {
CcuResult CcuReduceScatterMeshMem2Mem1DPeerOnlyKernel(CcuKernelArg arg)
{
    const auto* kernelArg = static_cast<const CcuKernelArgKfcReduceScatterMesh1DMem2Mem*>(arg);
    ccu::Func func(
        [kernelArg](ccu::Array<ccu::Variable>& args) {
            mc2_ops_hccl::CcuReduceScatterMeshMem2Mem1DPeerOnlyKernel(
                args[1], args[2], args[KFC_FUNC_TOKEN_ARG_INDEX], args[5], args[6], args[8], args[9],
                kernelArg->channels, kernelArg->channelCount, static_cast<uint32_t>(kernelArg->rankSize),
                kernelArg->rankId, kernelArg->opParam.DataDes.dataType, kernelArg->opParam.reduceType);
        },
        CCU_FUNC_TASK_ARG_MAX, KFC_REDUCE_SCATTER_PEER_ONLY_KERNEL_NAME);
    return CCU_SUCCESS;
}
} // namespace pc
} // namespace mc2_ops_hccl
