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

#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_tensor.h"
#include "kernel_common.h"
#endif

CHECK_VAR_EXISTS(g_maskCount);
CHECK_VAR_EXISTS(g_deqValue);
CHECK_VAR_EXISTS(g_dumpWorkspaceReserved);
CHECK_VAR_EXISTS(g_hcclContextReserved);
CHECK_FUNC_EXISTS(GetTPipePtr);
CHECK_FUNC_EXISTS(AscendC::AllocMutexID);
CHECK_FUNC_EXISTS(AscendC::ReleaseMutexID);
CHECK_FUNC_EXISTS(AscendC::SetMaskCount);
CHECK_FUNC_EXISTS(AscendC::SetMaskNorm);
CHECK_FUNC_EXISTS(GetDumpWorkSpacePtr);
CHECK_FUNC_EXISTS(SetDumpWorkSpacePtr);

#ifdef SPLIT_CORE_CUBE
CHECK_VAR_EXISTS(g_cubeTPipePtr);
#elif defined(SPLIT_CORE_VEC)
CHECK_VAR_EXISTS(g_vecTPipePtr);
#else
CHECK_VAR_EXISTS(g_tPipePtr);
#endif

#if defined(__ASCENDC_SUPERKERNEL_EARLY_START_V1) || defined(__ASCENDC_SUPERKERNEL_EARLY_START_V2)
CHECK_VAR_EXISTS(g_super_kernel_early_start_config);
#endif

template <uint32_t index>
using SetHcclContextFunc = void (*)(__gm__ uint8_t*);
template <uint32_t index>
using GetHcclContextFunc = __gm__ uint8_t* __gm__ (*)();
template <typename T, typename U> using SetAippFunctionsSingleFunc =
    void (*)(const AscendC::GlobalTensor<T>&, AscendC::AippInputFormat, AscendC::AippParams<U>);
template <typename T, typename U> using SetAippFunctionsDualFunc =
    void (*)(const AscendC::GlobalTensor<T>&, const AscendC::GlobalTensor<T>&, AscendC::AippInputFormat,
        AscendC::AippParams<U>);

CHECK_FUNC_EXISTS((*static_cast<SetHcclContextFunc<0>>(&AscendC::SetHcclContext<0>)));
CHECK_FUNC_EXISTS((*static_cast<GetHcclContextFunc<0>>(&AscendC::GetHcclContext<0>)));
CHECK_FUNC_EXISTS((*static_cast<SetAippFunctionsSingleFunc<uint8_t, uint8_t>>(
    &AscendC::SetAippFunctions<uint8_t, uint8_t>)));
CHECK_FUNC_EXISTS((*static_cast<SetAippFunctionsDualFunc<uint8_t, uint8_t>>(
    &AscendC::SetAippFunctions<uint8_t, uint8_t>)));
