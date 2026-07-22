/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 */
#ifndef CCU_LAUNCH_DL_H
#define CCU_LAUNCH_DL_H

#include <cstdint>

enum class CcuResult : int32_t { CCU_SUCCESS = 0, CCU_E_INTERNAL = 1 };

using ThreadHandle = uint64_t;
using CcuKernelHandle = uint64_t;

inline CcuResult HcommCcuKernelLaunch(
    ThreadHandle threadHandle, CcuKernelHandle kernelHandle,
    const void* taskArgs, uint32_t argSize)
{
    (void)threadHandle;
    (void)kernelHandle;
    (void)taskArgs;
    (void)argSize;
    return CcuResult::CCU_SUCCESS;
}

#endif // CCU_LAUNCH_DL_H
