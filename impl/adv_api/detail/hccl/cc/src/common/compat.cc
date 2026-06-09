/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include <pthread.h>
#include <cstdio>
#include "log.h"
#include "hcomm_dlsym.h"

namespace mc2_ops_hccl {

void CompatSymInit(void)
{
    fprintf(stderr, "[HcclWrapper-FPRINTF] CompatSymInit called, invoking HcommDlInit\n");
    fflush(stderr);
    HcommDlInit();
    fprintf(stderr, "[HcclWrapper-FPRINTF] HcommDlInit completed\n");
    fflush(stderr);
}

__attribute__((constructor)) void InitCompat()
{
    fprintf(stderr, "[HcclWrapper-FPRINTF] InitCompat constructor called\n");
    fflush(stderr);
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once(&once, CompatSymInit);
    fprintf(stderr, "[HcclWrapper-FPRINTF] InitCompat constructor finished\n");
    fflush(stderr);
}

}
