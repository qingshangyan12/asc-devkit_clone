/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_KERNEL_KFC_ALL_GATHER_NHR_1D_MEM2MEM_H
#define HCCL_CCU_KERNEL_KFC_ALL_GATHER_NHR_1D_MEM2MEM_H

#include <map>
#include <vector>
#include "ccu_kernel_alg_base.h"
#include "kfc_server_protocol.h"

namespace mc2_ops_hccl {

// CcuSchedAllGatherSoleNHR 的 KFC kernel。与 hccl CcuAllGatherNHR1DMem2MemKernel 逐行对照移植；
// 双 die 语义（对齐原版 dieNum=1/2 动态划分）：axisId/axisSize 来自 kernelArg 区分实例，
// die0/die1 份额与各自 goSize 同帧读取，axisId==1 首迭代跳过 die0 份额（与原版一致）。
// xnData 槽位见 KfcAllGatherSoleNhrParamIndex（kfc_server_message.h），三处索引必须一致。
CcuResult CcuKernelKfcAllGatherNHR1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable die0Size,
    ccu::Variable die0LastSize, ccu::Variable die1Size, ccu::Variable die1LastSize, ccu::Variable repeatNumInv,
    ccu::Variable inputSliceStride, ccu::Variable outputSliceStride, ccu::Variable inputRepeatStride,
    ccu::Variable outputRepeatStride, ccu::Variable isInputOutputEqual, ccu::Variable goSize0, ccu::Variable goSize1,
    ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable goSizeDie1_0, ccu::Variable goSizeDie1_1,
    ccu::Variable goSizeDie1_2, ccu::Variable goSizeDie1_3, const ChannelHandle channels[], uint32_t channelCount,
    uint32_t rankSize, uint32_t rankId, uint32_t axisId, uint32_t axisSize,
    const std::vector<KfcNhrStepInfo>& stepInfoVector, const std::map<uint32_t, uint32_t>& rank2ChannelIdx);

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_KERNEL_KFC_ALL_GATHER_NHR_1D_MEM2MEM_H
