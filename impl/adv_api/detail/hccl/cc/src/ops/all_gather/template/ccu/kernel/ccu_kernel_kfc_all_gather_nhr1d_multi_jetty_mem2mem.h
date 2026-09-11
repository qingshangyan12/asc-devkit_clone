/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_CCU_KERNEL_KFC_ALL_GATHER_NHR_1D_MULTI_JETTY_MEM2MEM_H
#define HCCL_CCU_KERNEL_KFC_ALL_GATHER_NHR_1D_MULTI_JETTY_MEM2MEM_H

#include <map>
#include <vector>
#include "ccu_kernel_alg_base.h"
#include "kfc_server_protocol.h"

namespace mc2_ops_hccl {

struct CcuKernelArgKfcAllGatherNHR1DMultiJettyMem2Mem : CcuKernelArgBase {
    uint64_t rankSize = 0;
    uint32_t rankId = 0;
    uint32_t jettyNum = 4;
    OpParam opParam;
    std::vector<KfcNhrStepInfo> stepInfoVector;
    std::map<uint32_t, uint32_t> rank2ChannelIdx;
    std::vector<std::vector<uint32_t>> subCommRanks;
};

struct KfcAllGatherNhrContext : CcuKernelCtxBase {
    const ChannelHandle* channels = nullptr;
    uint32_t channelCount = 0;
    uint32_t rankSize = 0;
    uint32_t rankId = 0;
    uint32_t jettyNum = 1;
    const std::vector<KfcNhrStepInfo>* stepInfoVector = nullptr;
    const std::map<uint32_t, uint32_t>* rank2ChannelIdx = nullptr;

    ccu::Variable input;
    std::vector<ccu::Variable> output;
    std::vector<ccu::Variable> token;
    ccu::Variable sliceSize;
    ccu::Variable sliceSizePerJetty;
    ccu::Variable lastSliceSizePerJetty;
    ccu::Variable repeatNumInv;
    ccu::Variable inputSliceStride;
    ccu::Variable outputSliceStride;
    ccu::Variable inputRepeatStride;
    ccu::Variable outputRepeatStride;
    ccu::Variable isInputOutputEqual;
    GroupOpSizeVars goSize;
    std::vector<ccu::Variable> outputSliceOffset;
    ccu::Variable myRankInputSliceOffset;
    ccu::Variable repeatFlag;
    ccu::Variable repeatNum;
    ccu::Variable constVar1;
    ccu::Event event;
};

CcuResult CcuKfcAllGatherNHR1DMultiJettyMem2MemKernel(CcuKernelArg arg);

} // namespace mc2_ops_hccl

#endif
