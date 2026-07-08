/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "hccl_inner_dl.h"
#include "hcom.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

DEFINE_DLSYM_FUNC(
    HcclResult, HcclAllGatherInner,
    (void* sendBuf, void* recvBuf, uint64_t sendCount, HcclDataType dataType, HcclComm comm, aclrtStream stream),
    (sendBuf, recvBuf, sendCount, dataType, comm, stream));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclReduceScatterInner,
    (void* sendBuf, void* recvBuf, uint64_t recvCount, HcclDataType dataType, HcclReduceOp op, HcclComm comm,
        aclrtStream stream),
    (sendBuf, recvBuf, recvCount, dataType, op, comm, stream));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclCreateOpResCtxInner,
    (HcclComm comm, uint8_t opType, HcclDataType srcDataType, HcclDataType dstDataType, HcclReduceOp reduceType,
        uint64_t count, char* algConfig, uint32_t commEngine, void** opResCtx),
    (comm, opType, srcDataType, dstDataType, reduceType, count, algConfig, commEngine, opResCtx));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclConfigGetInfo, (HcclComm comm, HcclConfigType cfgType, uint32_t infoLen, void* info),
    (comm, cfgType, infoLen, info));
DEFINE_DLSYM_FUNC(HcclResult, HcomGetCommHandleByGroup, (const char* group, HcclComm* commHandle),
    (group, commHandle));

// 初始化
void HcclInnerDlInit(void* libHcommHandle)
{
    INIT_SUPPORT_FLAG(libHcommHandle, HcclAllGatherInner);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclReduceScatterInner);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclCreateOpResCtxInner);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclConfigGetInfo);
    INIT_SUPPORT_FLAG(libHcommHandle, HcomGetCommHandleByGroup);
}
