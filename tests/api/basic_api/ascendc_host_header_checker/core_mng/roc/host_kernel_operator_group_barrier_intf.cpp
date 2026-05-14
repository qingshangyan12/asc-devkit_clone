/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "core_mng/roc/kernel_operator_group_barrier_intf.h"

//template <PipeMode pipeMode>
//class GroupBarrier {
//    __aicore__ inline GroupBarrier(GM_ADDR groupWorkspace, uint32_t arriveSizeIn, uint32_t waitSizeIn);
//    __aicore__ inline void Arrive(uint32_t arriveIndex);
//    __aicore__ inline void Wait(uint32_t waitIndex);
//    __aicore__ inline uint64_t GetWorkspaceLen();
//};
void HostTestGroupBarrier() {
    GM_ADDR groupWorkspace = nullptr;
    uint32_t arriveSizeIn = 8;
    uint32_t waitSizeIn = 8;
    AscendC::GroupBarrier<AscendC::PipeMode::SCALAR_MODE> groupBarrier(groupWorkspace, arriveSizeIn, waitSizeIn);
    groupBarrier.Arrive(0);
    groupBarrier.Wait(0);
    groupBarrier.GetWorkspaceLen();
}