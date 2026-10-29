/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_KERNEL_KFC_ALL_REDUCE_NHR_1D_MEM2MEM_H
#define HCCL_CCU_KERNEL_KFC_ALL_REDUCE_NHR_1D_MEM2MEM_H

#include <map>
#include <vector>
#include "ccu_kernel_alg_base.h"
#include "kfc_server_protocol.h"

namespace mc2_ops_hccl {

// CcuSchedAllReduceSoleNHR 的 KFC kernel。与 hccl CcuAllReduceNHR1DKernel
// （ccu_kernel_all_reduce_nhr1d_mem2mem.cc）逐行对照移植：
// LocalCopySlices → PreSync → DoReduceScatterNHR（前半 step，WriteReduce）→
// DoAllGatherNHR（后半 step，Write）→ PostSync。
// 双 die（D4，RS 2Die/AG SoleNHR 先例）：axisId=0 处理 [0, die0Size)、axisId=1 处理
// [die0Size, +die1Size)，双 mission 共享同一参数帧、kernelArg.axisId 区分实例；
// 单 die 时 axisId 恒 0、die1Size=0。
// xnData 槽位见 KfcAllReduceSoleNhrParamIndex（kfc_server_message.h），AIV prepare
// （CcuPrepareForAllReduceSoleNhrM2M）写入、KFC dispatch 逐槽转发、kernel 形参三处索引
// 必须一致；token 走 dispatch 的 ctx.token，不占槽。
CcuResult CcuKernelKfcAllReduceNHR1DMem2MemKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo, ccu::Variable isInputOutputEqual,
    ccu::Variable die0Size, ccu::Variable die1Size, ccu::Variable die0SliceSize, ccu::Variable die1SliceSize,
    ccu::Variable die0LastSliceSize, ccu::Variable die1LastSliceSize, const ChannelHandle channels[],
    uint32_t channelCount, uint32_t rankSize, uint32_t rankId, uint32_t axisId, uint32_t axisSize,
    const HcclDataType& dataType, const HcclReduceOp& reduceType, const std::vector<KfcNhrStepInfo>& stepInfoVector,
    const std::map<uint32_t, uint32_t>& rank2ChannelIdx);

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_KERNEL_KFC_ALL_REDUCE_NHR_1D_MEM2MEM_H
