/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kernel_kfc_all_gather_mesh1d_mem2mem.h"
#include "ccu_kernel_kfc_all_gather_nhr1d_multi_jetty_mem2mem.h"
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.h"

namespace mc2_ops_hccl {

CcuResult CcuKfcAllGatherMesh1DMem2MemKernel(
    ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable,
    ccu::Variable, ccu::Variable, ccu::Variable, const ChannelHandle[], uint32_t, uint32_t, uint32_t)
{
    return CCU_E_NOT_SUPPORT;
}

CcuResult CcuKfcAllGatherNHR1DMultiJettyMem2MemKernel(
    ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable,
    ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable,
    ccu::Variable, ccu::Variable, const ChannelHandle[], uint32_t, uint32_t, uint32_t, uint32_t,
    const std::vector<KfcNhrStepInfo>&, const std::map<uint32_t, uint32_t>&)
{
    return CCU_E_NOT_SUPPORT;
}

CcuResult CcuReduceScatterMesh1DMem2MemKernel(
    ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable, ccu::Variable,
    const ChannelHandle[], uint32_t, uint32_t, uint32_t, const HcclDataType&, const HcclDataType&,
    const HcclReduceOp&)
{
    return CCU_E_NOT_SUPPORT;
}

} // namespace mc2_ops_hccl
