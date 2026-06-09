/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
  * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
  * CANN Open Software License Agreement Version 2.0 (the "License").
  * Please refer to the License for details. You may not use this file except in compliance with the License.
  * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
  * See LICENSE in the root of the software repository for the full text of the License.
  */

#include "hcomm_dlsym.h"
#include "hccl_res_dl.h"
#include "ccu_res_dl.h"
#include "hccl_ccu_res_dl.h"
#include "ccu_launch_dl.h"
#include "ccu_primitives_impl_dl.h"
#include "hccl_rank_graph_dl.h"
#include "hcomm_primitives_dl.h"
#include "hccl_inner_dl.h"
#include "hcomm_host_profiling_dl.h"
#include "hccl_host_comm_dl.h"
#include "hccl_res_expt_dl.h"
#include "log.h"
#include <pthread.h>
#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#include <acl/acl.h>

#ifdef __cplusplus
extern "C" {
#endif

static void* gLibHandle = nullptr;
static int gHcommVersion = 0;

int GetHcommVersion(void) {
    if (gHcommVersion == 0) {
        char hcommPkgName[] = "hcomm";
        if (aclsysGetVersionNum(hcommPkgName, &gHcommVersion) != ACL_SUCCESS) {
            gHcommVersion = 0;
        }
    }

    return gHcommVersion;
}

bool HcommIsProfilingSupported()
{
    if (GetHcommVersion() >= 90000000) {
        return true;
    } else {
        return false;
    }
}

bool HcommIsExportThreadSupported()
{
    if (GetHcommVersion() >= 90000000 && HcommIsSupportHcclThreadExportToCommEngine()) {
        return true;
    } else {
        return false;
    }
}

// 初始化
void HcommDlInit(void) {
    fprintf(stderr, "[HcclWrapper-FPRINTF] ===== HcommDlInit ENTRY =====\n");
    fprintf(stderr, "[HcclWrapper-FPRINTF] HcommDlInit called, gLibHandle=%p\n", gLibHandle);
    fflush(stderr);
    
    if (gLibHandle != nullptr) {
        fprintf(stderr, "[HcclWrapper-FPRINTF] HcommDlInit already initialized, gLibHandle=%p, skipping\n", gLibHandle);
        fprintf(stderr, "[HcclWrapper-FPRINTF] ===== HcommDlInit EARLY RETURN =====\n");
        fflush(stderr);
        return;
    }

    fprintf(stderr, "[HcclWrapper-FPRINTF] gLibHandle is nullptr, proceeding with initialization\n");
    fprintf(stderr, "[HcclWrapper-FPRINTF] Calling dlopen(\"libhcomm.so\", RTLD_NOW)\n");
    fflush(stderr);
    
    gLibHandle = dlopen("libhcomm.so", RTLD_NOW);
    
    if (!gLibHandle) {
        const char* dlErr = dlerror();
        fprintf(stderr, "[HcclWrapper-FPRINTF] dlopen FAILED, error: %s\n", dlErr ? dlErr : "(null)");
        fprintf(stderr, "[HcclWrapper-FPRINTF] ===== HcommDlInit FAILED RETURN =====\n");
        fflush(stderr);
        HCCL_ERROR("[HcclWrapper] Failed to open libhcomm: %s", dlErr ? dlErr : "(null)");
        return;
    }
    
    fprintf(stderr, "[HcclWrapper-FPRINTF] dlopen SUCCESS, handle=%p\n", gLibHandle);
    fflush(stderr);

    dlerror();

    fprintf(stderr, "[HcclWrapper-FPRINTF] Initializing DL modules...\n");
    fflush(stderr);
    
    HcclResDlInit(gLibHandle);
    HcclRankGraphDlInit(gLibHandle);
    HcommPrimitivesDlInit(gLibHandle);
    HcclInnerDlInit(gLibHandle);
    HcommProfilingDlInit(gLibHandle);
    HcclCommDlInit(gLibHandle);
    HcclResExptDlInit(gLibHandle);
    CcuResDlInit(gLibHandle);
    HcclCcuResDlInit(gLibHandle);
    
    fprintf(stderr, "[HcclWrapper-FPRINTF] Calling CcuLaunchDlInit...\n");
    fflush(stderr);
    
    CcuLaunchDlInit(gLibHandle);
    CcuPrimitivesImplDlInit(gLibHandle);
    
    fprintf(stderr, "[HcclWrapper-FPRINTF] HcommDlInit completed successfully, final gLibHandle=%p\n", gLibHandle);
    fprintf(stderr, "[HcclWrapper-FPRINTF] ===== HcommDlInit NORMAL RETURN =====\n");
    fflush(stderr);
}

#ifdef __cplusplus
}
#endif