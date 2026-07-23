/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_KERNEL_ALL_TO_ALL_V_MESH_1D_H
#define HCCL_CCU_KERNEL_ALL_TO_ALL_V_MESH_1D_H

#include <vector>
#include <ios>
#include "utils.h"
#include "ccu_kernel_utils.h"
#include "ccu_kernel_alg_base.h"

namespace mc2_ops_hccl {

struct CcuKernelArgAlltoAllVMesh1D : CcuKernelArgBase {
    uint64_t                            rankSize;
    uint32_t                            rankId;
    bool                                loadFromMem;
    OpParam                             opParam;
    std::vector<std::vector<uint32_t>>  subCommRanks;
};

struct A2AsingleSendRecvInfo {
    ccu::Variable sendSize;   // 本卡发往该 rank 的字节数（单发即整块大小）
    ccu::Variable sendOffset; // 本卡 input 中发往该 rank 的偏移
    ccu::Variable recvOffset; // 该 rank 在“本卡 output”中的接收偏移
};

struct AlltoAllVMesh1DContext {
    // ---- 由 kfc_server 入参填充 ----
    uint32_t rankSize{0};
    uint32_t rankId{0};
    const ChannelHandle *channels{nullptr};
    uint32_t channelCount{0};
    ccu::Variable input;                  // 本卡 input 基址（0305 只用本卡的，不取对端 INPUT 窗）
    ccu::Variable srcOffset;
    ccu::Variable dstOffset;
    ccu::Variable extAddr;                // ext 参数(CCUMsgExt 数组)在 HBM 的地址

    // ---- InitResource / LoadArgs 填充 ----
    std::vector<ccu::Variable> output;    // 本卡=入参，对端=远端 OUTPUT 窗
    std::vector<ccu::Variable> token;     // 本卡=入参，对端=远端 TOKEN 窗
    std::vector<A2AsingleSendRecvInfo> sendRecvInfo;

    // ---- CalcGroupSrcDst 填充 ----
    std::vector<ccu::LocalAddr>  src;
    std::vector<ccu::RemoteAddr> dst;
    ccu::LocalAddr               myDst;   // 本端接收
    ccu::Event                   event;
};

CcuResult CcuAlltoAllVMesh1DKernel(ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                                   ccu::Variable srcOffset, ccu::Variable dstOffset, ccu::Variable extAddr,
                                   ccu::Variable goSize1, ccu::Variable goSize2, ccu::Variable goSize3,
                                   ccu::Variable goSize4, const ChannelHandle channels[], uint32_t channelCount,
                                   uint32_t rankSize, uint32_t rankId);

} // namespace mc2_ops_hccl
#endif // HCCL_CCU_KERNEL_ALL_TO_ALL_V_MESH_1D_H
