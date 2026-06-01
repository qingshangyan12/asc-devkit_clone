/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef CCU_FUNC_DL_H
#define CCU_FUNC_DL_H

#include "dlsym_common.h"
#include "ccu/ccu_types.h"

#ifdef __cplusplus
extern "C" {
#endif

DECL_WEAK_FUNC(CcuResult, CcuVariableAlloc, CcuVariableHandle *varHandle);
DECL_WEAK_FUNC(CcuResult, CcuFuncBlockLookup, const void *funcPtr, uint64_t *outHandle);
DECL_WEAK_FUNC(CcuResult, CcuFuncBlockBegin, const void *funcPtr, uint64_t *outHandle);
DECL_WEAK_FUNC(CcuResult, CcuFuncBlockEnd, uint64_t handle);
DECL_WEAK_FUNC(CcuResult, CcuFuncDefineInArg, uint64_t handle, CcuVariableHandle formal);
DECL_WEAK_FUNC(CcuResult, CcuFuncCall, uint64_t handle, const CcuVariableHandle *inArgs, uint32_t numIn);

void CcuFuncDlInit(void* libHcommHandle);

#ifdef __cplusplus
}
#endif

#endif // CCU_FUNC_DL_H
