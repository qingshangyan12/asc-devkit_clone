/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "log.h"
#include "hcomm_host_profiling_dl.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

DEFINE_DLSYM_FUNC(HcclResult, HcommProfilingRegThread, (HcomProInfoTmp profInfo, ThreadHandle* threads),
    (profInfo, threads));
DEFINE_DLSYM_FUNC(HcclResult, HcommProfilingUnRegThread, (HcomProInfoTmp profInfo, ThreadHandle* threads),
    (profInfo, threads));
DEFINE_DLSYM_FUNC(HcclResult, HcommProfilingReportKernel, (uint64_t beginTime, const char* profName),
    (beginTime, profName));
DEFINE_DLSYM_FUNC(HcclResult, HcommProfilingReportOp, (HcomProInfoTmp profInfo), (profInfo));
DEFINE_DLSYM_FUNC(uint64_t, HcommGetProfilingSysCycleTime, (void), ());
DEFINE_DLSYM_FUNC(HcclResult, HcclDfxRegOpInfo, (HcclComm comm, void* dfxOpInfo), (comm, dfxOpInfo));
DEFINE_DLSYM_FUNC(HcclResult, HcclProfilingReportOp, (HcclComm comm, uint64_t beginTime), (comm, beginTime));
DEFINE_DLSYM_FUNC(HcclResult, HcclReportAicpuKernel, (HcclComm comm, uint64_t beginTime, char* kernelName),
    (comm, beginTime, kernelName));

// 初始化
void HcommProfilingDlInit(void* libHcommHandle)
{
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingRegThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingUnRegThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingReportKernel);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommProfilingReportOp);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommGetProfilingSysCycleTime);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclDfxRegOpInfo);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclProfilingReportOp);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclReportAicpuKernel);
}
