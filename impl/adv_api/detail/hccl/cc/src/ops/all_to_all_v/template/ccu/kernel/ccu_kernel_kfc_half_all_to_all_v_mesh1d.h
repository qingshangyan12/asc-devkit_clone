/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_KERNEL_KFC_HALF_ALL_TO_ALL_V_MESH_1D_H
#define HCCL_CCU_KERNEL_KFC_HALF_ALL_TO_ALL_V_MESH_1D_H

#include <vector>
#include <ios>
#include "utils.h"
#include "ccu_kernel.h"
#include "ccu_kernel_utils.h"
#include "ccu_kernel_alg_base.h"

namespace mc2_ops_hccl {
using namespace hcomm;

// HalfAllToAllV Mesh1D sub-kernel driven by CcuKernelKfcServer.
//
// Each rank writes a variable-length slice to every peer's CCL buffer. The per-peer slice sizes and
// source offsets live in HBM (written by CcuPrepareForAllToAllVWrite on the AI core side), so this
// kernel loads them through sendSizesAddr / sendOffsetsAddr instead of receiving them as SQE args.
//
// Argument order must stay in sync with DispatchKfcSubKernel in ccu_kernel_kfc_server.cc.
CcuResult CcuKfcHalfAllToAllVMesh1DKernel(
    ccu::Variable inputAddr, ccu::Variable cclBufferAddr, ccu::Variable tokenInfo, ccu::Variable sendSizesAddr,
    ccu::Variable sendOffsetsAddr, ccu::Variable recvOffset, ccu::Variable goSize0, ccu::Variable goSize1,
    ccu::Variable goSize2, ccu::Variable goSize3, const ChannelHandle channels[], uint32_t channelCount,
    uint32_t rankSize, uint32_t rankId);

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_KERNEL_KFC_HALF_ALL_TO_ALL_V_MESH_1D_H
