/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "host_checker.h"

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510))
#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "core_mng/roc/kernel_operator_cube_group_intf.h"
#endif

struct HostCubeGroupMsg {
    AscendC::CubeGroupMsgHead head;
    uint8_t reserved[62];
};
struct HostCubeGroupMatmulApiType {
    static constexpr int32_t GetSubBlockDim()
    {
        return 1;
    }
};
template <class MatmulApiType, class CubeMsgType> struct HostCubeGroupCallBack {
    static constexpr int32_t GetActualFuncId()
    {
        return 0;
    }
    __aicore__ inline void Init(HostCubeGroupCallBack&, MatmulApiType&, GM_ADDR) {}
    __aicore__ inline void Call(MatmulApiType&, __gm__ CubeMsgType*, AscendC::CubeResGroupHandle<CubeMsgType>&) {}
};

static_assert(sizeof(AscendC::KfcWorkspace) > 0, "AscendC::KfcWorkspace is not defined");
static_assert(sizeof(AscendC::CubeResGroupHandle<HostCubeGroupMsg>) > 0,
    "AscendC::CubeResGroupHandle is not defined");
CHECK_FUNC_EXISTS(
    (AscendC::CreateCubeResGroup<0, HostCubeGroupMatmulApiType, HostCubeGroupCallBack, HostCubeGroupMsg>));
#endif
