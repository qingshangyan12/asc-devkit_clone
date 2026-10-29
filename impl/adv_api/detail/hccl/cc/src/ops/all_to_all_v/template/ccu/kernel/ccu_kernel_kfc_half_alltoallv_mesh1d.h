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
#include "kfc_server_protocol.h"

namespace mc2_ops_hccl {

// 变长 AllToAll：sendSizes/sendOffsets 由 AIC 运行时写入设备内存，kernel 只收
// 地址并用 ccu::Load 读入。xn 布局必须与 CcuPrepareForAllToAllVWrite 写入 xnData_ 的
// 顺序一致：xn[0]=opId(KFC 队列哨兵) xn[1]=sendBuf xn[2]=sendSizes xn[3]=sendOffsets
// xn[4..7]=goSize xn[8]=remoteWinOffset；token 走 kfcServerArgs[5]，不占 xn 槽位。
//
// 双 mission：mission0 搬前 rankSize 个 A 切片，mission1 搬后 rankSize 个 B 切片
// （接收地址加 sizeA 偏移）；missionNum==1 时合并 A+B 一次搬运。
// 双 mission 共享同一组 channel，PreSync 通知位必须按 missionIndex 错开：
// 同一位被两个 mission 各 post/wait 一次时，wait 消费位后第二个 wait 永远等不到。
CcuResult CcuKfcHalfAlltoAllVMesh1DKernel(
    ccu::Variable userInAddr, ccu::Variable sendSizeAddr, ccu::Variable sendOffsetAddr, ccu::Variable goSize0,
    ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable recvOffset,
    ccu::Variable tokenInfo, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    uint64_t localWinAddr, ccu::Variable missionNum, ccu::Variable missionIndex, uint32_t hostMissionIndex,
    uint32_t hostMissionNum);

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_KERNEL_KFC_HALF_ALL_TO_ALL_V_MESH_1D_H
