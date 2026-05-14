/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "core_mng/roc/kernel_operator_cube_group_intf.h"

//class KfcWorkspace {
//    __aicore__ inline KfcWorkspace(GM_ADDR workspace);
//    __aicore__ inline void UpdateKfcWorkspace(uint32_t offset);
//    __aicore__ inline GM_ADDR GetKfcWorkspace();
//    __aicore__ inline ~KfcWorkspace();
//};
void HostTestKfcWorkspace() {
    GM_ADDR workspace = nullptr;
    AscendC::KfcWorkspace kfcWorkspace(workspace);
    kfcWorkspace.UpdateKfcWorkspace(0);
    kfcWorkspace.GetKfcWorkspace();
}

//template <typename CubeMsgType>
//class CubeResGroupHandle {
//    __aicore__ inline CubeResGroupHandle() = default;
//    __aicore__ inline CubeResGroupHandle(
//        GM_ADDR workspace, uint8_t blockStart, uint8_t blockSize, uint8_t msgQueueSize, uint8_t evtIDIn);
//    __aicore__ inline void AssignQueue(uint8_t queueIdIn);
//    template <PipeMode pipeMode = PipeMode::SCALAR_MODE>
//    __aicore__ inline __gm__ CubeMsgType *AllocMessage();
//    template <PipeMode pipeMode = PipeMode::SCALAR_MODE>
//    __aicore__ inline uint16_t PostMessage(__gm__ CubeMsgType *msg, CubeMsgType &msgInput);
//    __aicore__ inline uint16_t FreeMessage(__gm__ CubeMsgType *msg);
//    __aicore__ inline uint16_t FreeMessage(__gm__ CubeMsgType *msg, CubeMsgState waitState);
//    __aicore__ inline uint16_t PostFakeMsg(__gm__ CubeMsgType *msg);
//    __aicore__ inline void SetQuit(__gm__ CubeMsgType *msg);
//    __aicore__ inline void SetSkipMsg(uint8_t skipCnt);
//    template <bool sync = true>
//    __aicore__ inline bool Wait(uint16_t offset);
//};
void HostTestCubeResGroupHandle() {
    GM_ADDR workspace = nullptr;
    uint8_t blockStart = 0;
    uint8_t blockSize = 8;
    uint8_t msgQueueSize = 4;
    uint8_t evtIDIn = 0;
    AscendC::CubeResGroupHandle<int> cubeResGroupHandle(workspace, blockStart, blockSize, msgQueueSize, evtIDIn);
    cubeResGroupHandle.AssignQueue(0);
}

//template <int groupID, class MatmulApiType, template <class, class> class CallBack, typename CubeMsgType>
//__aicore__ inline CubeResGroupHandle<CubeMsgType> CreateCubeResGroup(
//    KfcWorkspace &desc, uint8_t blockStart, uint8_t blockSize, uint8_t msgQueueSize, GM_ADDR tiling);
void HostTestCreateCubeResGroup() {
    GM_ADDR workspace = nullptr;
    AscendC::KfcWorkspace kfcWorkspace(workspace);
    uint8_t blockStart = 0;
    uint8_t blockSize = 8;
    uint8_t msgQueueSize = 4;
    GM_ADDR tiling = nullptr;
}