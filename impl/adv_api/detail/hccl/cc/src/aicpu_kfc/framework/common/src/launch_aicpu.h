/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
 
#ifndef LAUNCH_AICPU_H
#define LAUNCH_AICPU_H

#include "hccl/base.h"
#include "hccl_types.h"
#include "acl/acl_rt.h"

namespace hccl {
    constexpr const char *kAicpuIndOpCommInitKernelName = "RunAicpuIndOpCommInit";
    constexpr const char *kMc2AicpuIndOpCommInitKernelName = "Mc2RunAicpuIndOpCommInit";
    constexpr const char *kAicpuIndOpThreadInitKernelName = "RunAicpuIndOpThreadInit";
    constexpr const char *kMc2AicpuIndOpThreadInitKernelName = "Mc2RunAicpuIndOpThreadInit";
    constexpr const char *kAicpuIndOpChannelInitV2KernelName = "RunAicpuIndOpChannelInitV2";
    constexpr const char *kMc2AicpuIndOpChannelInitV2KernelName = "Mc2RunAicpuIndOpChannelInitV2";
    constexpr const char *kAicpuIndOpChannelUpdateV2KernelName = "RunAicpuIndOpChannelUpdateV2";
    constexpr const char *kMc2AicpuIndOpChannelUpdateV2KernelName = "Mc2RunAicpuIndOpChannelUpdateV2";
    constexpr const char *kAicpuIndOpNotifyKernelName = "RunAicpuIndOpNotify";
    constexpr const char *kMc2AicpuIndOpNotifyKernelName = "Mc2RunAicpuIndOpNotify";
    constexpr const char *kAicpuThreadSupplementNotifyKernelName = "RunAicpuThreadSupplementNotify";
    constexpr const char *kMc2AicpuThreadSupplementNotifyKernelName = "Mc2RunAicpuThreadSupplementNotify";

    inline const char *GetAicpuIndOpCommInitKernelName(bool useMc2ServerKernelNames)
    {
        return useMc2ServerKernelNames ? kMc2AicpuIndOpCommInitKernelName : kAicpuIndOpCommInitKernelName;
    }

    inline const char *GetAicpuIndOpThreadInitKernelName(bool useMc2ServerKernelNames)
    {
        return useMc2ServerKernelNames ? kMc2AicpuIndOpThreadInitKernelName : kAicpuIndOpThreadInitKernelName;
    }

    inline const char *GetAicpuIndOpChannelInitV2KernelName(bool useMc2ServerKernelNames)
    {
        return useMc2ServerKernelNames ? kMc2AicpuIndOpChannelInitV2KernelName : kAicpuIndOpChannelInitV2KernelName;
    }

    inline const char *GetAicpuIndOpChannelUpdateV2KernelName(bool useMc2ServerKernelNames)
    {
        return useMc2ServerKernelNames ? kMc2AicpuIndOpChannelUpdateV2KernelName
                                       : kAicpuIndOpChannelUpdateV2KernelName;
    }

    inline const char *GetAicpuIndOpNotifyKernelName(bool useMc2ServerKernelNames)
    {
        return useMc2ServerKernelNames ? kMc2AicpuIndOpNotifyKernelName : kAicpuIndOpNotifyKernelName;
    }

    inline const char *GetAicpuThreadSupplementNotifyKernelName(bool useMc2ServerKernelNames)
    {
        return useMc2ServerKernelNames ? kMc2AicpuThreadSupplementNotifyKernelName
                                       : kAicpuThreadSupplementNotifyKernelName;
    }

    inline bool IsMc2ServerKernelBinHandle(aclrtBinHandle binHandle)
    {
        if (binHandle == nullptr) {
            return false;
        }
        aclrtFuncHandle funcHandle = nullptr;
        return aclrtBinaryGetFunction(binHandle, kMc2AicpuIndOpCommInitKernelName, &funcHandle) == ACL_SUCCESS;
    }

    HcclResult AicpuAclKernelLaunch(const rtStream_t stm, void* addr, u32 size, aclrtBinHandle binHandle,
        const std::string &kernelName, bool isInitTask, u16 timeOut,
        void* tilingDataPtr = nullptr, u32 tilingDataSize = 0);
	HcclResult AicpuAclKernelLaunchV2(const rtStream_t stm, void *addr, u32 size,
		aclrtBinHandle binHandle, const std::string &kernelName, bool isInitTask, u16 timeOut,
		void *tilingDataPtr, u32 tilingDataSize);
    HcclResult GetKernelFilePath(std::string &binaryPath);
    HcclResult GetCustomKernelFilePath(std::string &binaryPath);
}

#endif // LAUNCH_AICPU_H
