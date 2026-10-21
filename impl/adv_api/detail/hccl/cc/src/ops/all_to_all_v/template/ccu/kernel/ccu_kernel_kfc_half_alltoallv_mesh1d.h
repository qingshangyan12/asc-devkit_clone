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
// 参数顺序（必须与 CcuPrepareForAllToAllVWrite 写入 xnData_ 的顺序严格一致）：
//   xn[0]=opId(签名)  xn[1]=sendBuf  xn[2]=sendSizes  xn[3]=sendOffsets
//   xn[4..7]=goSize[0..3]  xn[8]=remoteWinOffset
// 对应 AIC 侧 API：AlltoAllvWrite(usrIn, sendOffsets, sendSizes, remoteWinOffset, localDataSize)
// 且 hccl_ccu_v0.h 的 ccuUsedXnNum_ = 9 与此处 9 个 xn 参数吻合。
//
// 双 mission（默认）：mission0 搬 A 切片、mission1 搬 B 切片，一次通信拆两半并发：
//   * missionIndex==0 使用 sendSize/sendOffset 的前 rankSize 个元素（A 切片）
//   * missionIndex==1 使用后 rankSize 个元素（B 切片），且接收地址需加 sizeA 偏移
// 退化为 missionNum==1 时合并 A+B 一次搬运（兼容 A 片全量 + B 片为 0 的单片填充）。
// Kernel 读取 2*rankSize 数组。localWinAddr 为建图期常量（本端 window 基址），非运行时 xn 参数。
//
// 硬约束（迁移过程中发现）：
//   - xn[0] 被 GetOpId 占用，且在 KFC 循环队列中作为哨兵（== INVALID_U64 退出循环）
//   - token 通过 kfcServerArgs[5] 传递，不占用 xn 槽位
//   - 轮次由 AIC 侧调用 AlltoAllvWrite 的次数决定，host 推 submitInfo 不影响轮次
//   - 两 mission 读同一份 xn 参数帧（AIC 只写一份），MissionPreSync 栅栏保证起跑时序
CcuResult CcuKfcHalfAlltoAllVMesh1DKernel(
    ccu::Variable userInAddr, ccu::Variable sendSizeAddr, ccu::Variable sendOffsetAddr, ccu::Variable goSize0,
    ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3, ccu::Variable recvOffset,
    ccu::Variable tokenInfo, const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    uint64_t localWinAddr, ccu::Variable missionNum, ccu::Variable missionIndex);

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_KERNEL_KFC_HALF_ALL_TO_ALL_V_MESH_1D_H
