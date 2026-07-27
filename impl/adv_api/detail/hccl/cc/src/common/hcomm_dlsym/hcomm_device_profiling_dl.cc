/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "hcomm_device_profiling_dl.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

DEFINE_WEAK_FUNC(HcclResult, HcommProfilingReportMainStreamAndFirstTask, ThreadHandle thread);
DEFINE_WEAK_FUNC(HcclResult, HcommProfilingReportMainStreamAndLastTask, ThreadHandle thread);
DEFINE_WEAK_FUNC(HcclResult, HcommProfilingReportDeviceHcclOpInfo, HcomProInfoTmp profInfo);
DEFINE_WEAK_FUNC(HcclResult, HcommProfilingInit, ThreadHandle* threads, uint32_t threadNum);
DEFINE_WEAK_FUNC(HcclResult, HcommProfilingEnd, ThreadHandle* threads, uint32_t threadNum);
DEFINE_WEAK_FUNC(HcclResult, HcommProfilingReportDeviceOp, const char* groupname);
DEFINE_WEAK_FUNC(HcclResult, HcommProfilingReportKernelStartTask, uint64_t thread, const char* groupname);
DEFINE_WEAK_FUNC(HcclResult, HcommProfilingReportKernelEndTask, uint64_t thread, const char* groupname);

using HcclDfxRegOpInfoByCommIdFunc = HcclResult (*)(char*, void*);

static bool g_HcclDfxRegOpInfoByCommIdSupported = false;
static HcclDfxRegOpInfoByCommIdFunc g_HcclDfxRegOpInfoByCommId = nullptr;

extern "C" bool HcommIsSupportHcclDfxRegOpInfoByCommId(void)
{
    return g_HcclDfxRegOpInfoByCommIdSupported;
}

extern "C" HcclResult HcclHcommDfxRegOpInfoByCommId(char* commId, void* hcclDfxOpInfo)
{
    if (g_HcclDfxRegOpInfoByCommId == nullptr) {
        HCCL_ERROR("[HcclWrapper] HcclDfxRegOpInfoByCommId not supported");
        return HCCL_E_NOT_SUPPORT;
    }
    return g_HcclDfxRegOpInfoByCommId(commId, hcclDfxOpInfo);
}

// 初始化
void HcommDeviceProfilingDlInit(void* libHcommHandle)
{
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingReportMainStreamAndFirstTask);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingReportMainStreamAndLastTask);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingReportDeviceHcclOpInfo);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingInit);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingEnd);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingReportDeviceOp);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingReportKernelStartTask);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingReportKernelEndTask);

    dlerror();
    g_HcclDfxRegOpInfoByCommId =
        reinterpret_cast<HcclDfxRegOpInfoByCommIdFunc>(dlsym(libHcommHandle, "HcclDfxRegOpInfoByCommId"));
    const char* dfxRegError = dlerror();
    g_HcclDfxRegOpInfoByCommIdSupported =
        (g_HcclDfxRegOpInfoByCommId != nullptr && dfxRegError == nullptr);
    if (!g_HcclDfxRegOpInfoByCommIdSupported) {
        g_HcclDfxRegOpInfoByCommId = nullptr;
    }
    HCCL_DEBUG(
        "[MC2_DFX_REPORT][RuntimeSupport] HcclDfxRegOpInfoByCommId supported[%d], func[%p], error[%s].",
        static_cast<int>(g_HcclDfxRegOpInfoByCommIdSupported),
        reinterpret_cast<void*>(g_HcclDfxRegOpInfoByCommId),
        dfxRegError == nullptr ? "none" : dfxRegError);
}
