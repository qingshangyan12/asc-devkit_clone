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

#include "ccu_kernel_alg_base.h"

namespace mc2_ops_hccl {

// MC2 变长 AllToAll。sendSizes/sendOffsets 由 AIC 运行时写入设备内存，故此处只收地址，
// 由 kernel 用 ccu::Load 批量读入，与 hcomm 源实现的 LoadArgsFromMem 语义一致。
//
// 参数顺序必须与设备侧 CcuPrepareForAllToAllVWrite 写入 xnData_ 的顺序严格一致
// （hccl_ccu_v0_prepare.h:64）：
//   xn[0]=opId(签名)  xn[1]=sendBuf  xn[2]=sendSizes  xn[3]=sendOffsets
//   xn[4..7]=goSize[0..3]  xn[8]=remoteWinOffset
// 对应 AIC 侧 API：AlltoAllvWrite(usrIn, sendOffsets, sendSizes, remoteWinOffset, localDataSize)
// 且 hccl_ccu_v0.h:269 的 ccuUsedXnNum_ = 9 与此处 9 个 xn 参数吻合。
//
// 阶段一：单 mission 承担全部数据（missionId 固化为 0），只读 A 片槽位，
// 语义等同常规 1D Fullmesh AllToAllV。A/B 双片内存布局保持不变，阶段二恢复双 mission
// 时无需改动 host 侧参数打包。
// localWinAddr 为建图期常量（本端 window 基址），非运行时 xn 参数。
CcuResult CcuKfcHalfAlltoAllVMesh1DKernel(
    ccu::Variable userInAddr, ccu::Variable sendSizeAddr, ccu::Variable sendOffsetAddr, ccu::Variable goSize0,
    ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable recvOffset,
    ccu::Variable tokenInfo, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    uint64_t localWinAddr);

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_KERNEL_KFC_HALF_ALL_TO_ALL_V_MESH_1D_H
