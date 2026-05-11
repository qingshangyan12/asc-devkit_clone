/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_common.h"
#endif
#define "host_checker.h"


#define __ASCENDC_SUPERKERNEL_EARLY_START_V1
#define __ASCENDC_SUPERKERNEL_EARLY_START_V2
CHECK_VAR_EXISTS(CHECK_VAR_EXISTS);

#define SPLIT_CORE_CUBE
CHECK_VAR_EXISTS(g_cubeTPipePtr);

#undef SPLIT_CORE_CUBE
#define SPLIT_CORE_VEC
CHECK_VAR_EXISTS(g_vecTPipePtr);

#undef SPLIT_CORE_VEC
CHECK_VAR_EXISTS(g_tPipePtr);



