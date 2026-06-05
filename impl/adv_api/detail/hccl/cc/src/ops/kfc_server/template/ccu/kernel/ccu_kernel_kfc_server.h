/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_CCU_KERNEL_KFC_SERVER_H
#define HCCL_CCU_KERNEL_KFC_SERVER_H

#include <vector>
#include <ios>
#include "utils.h"
// #include "ccu_kernel.h"
#include "ccu_kernel_utils.h"
#include "ccu_kernel_alg_base.h"
#include "../../../all_to_all_v/template/ccu/kernel/ccu_kernel_all_to_all_mesh1d.h"

using namespace AscendC;

namespace mc2_ops_hccl {
constexpr uint64_t INVALID_U64 = UINT64_MAX;
constexpr uint32_t CCU_PARAM_NUM_PER_DIE = 32;
constexpr uint32_t CCU_PARAM_NUM_MAX     = CCU_PARAM_NUM_PER_DIE * 2;
constexpr uint32_t CCU_ONE_PARAM_SIZE    = 8;
constexpr uint32_t CCU_TASK_NUM_MAX      = 64;


// new---20260603
struct CcuKernelArgKfcServer: CcuKernelArgBase{
    uint64_t                                rankSize;
    uint32_t                                rankId;
    bool                                    loadFromMem;
    OpParam                                 opParam;
    std::vector<std::vector<uint32_t>>      subCommRanks;
};

// 第一层同步：missionId=0 与missionId>0之间的
// 第二层同步：在missionId=0中，dieId=0与dieId=1之间的同步
struct KfcServerContext: CcuKernelCtxBase {
    const CcuKernelArgKfcServer *arg;
    
    ccu::Variable xnAddr;              // HBM上的算子执行参数首地址
    ccu::Variable ckeAddr;
    ccu::Variable waitAddr;            // HBM上的每轮开始信号的首地址
    ccu::Variable recordAddr;          // HBM上的每轮完成信号的首地址

    ccu::Variable token;
    ccu::Variable dieNum;              // Die数量，用于判断单双Die，默认为1即单Die
    
    ccu::Variable missionNum;
    ccu::Variable missionIndex;

    // 用于Die间同步信号[missionId=0]
    const char *exportDieSig;
    const char *importDieSig;

    // TODO：当前missionNum_=1，missionIndex_=0---
    // 用于Mission间同步信号
    std::vector<const char *> exportMissoinSig;
    std::vector<const char *> importMissionSig;
    // 用于Mission间同步变量
    std::vector<ccu::Variable> exportMissionVar;
    std::vector<ccu::Variable> importMissionVar;
}    


//new---20260603
}//mc2_ops_hccl

#endif