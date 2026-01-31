/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file types_def.h
 * \brief used to store definitions of public macros
 */
#ifndef IMPL_UTILS_BASE_DEFS_MARCOS_DEF_H
#define IMPL_UTILS_BASE_DEFS_MARCOS_DEF_H

#include <cstdint>

#ifndef __aicore__
#define __aicore__ [aicore]
#endif // __aicore__

#ifndef __host_aicore__
#define __host_aicore__ [host, aicore]
#endif // __host_aicore__

#if (_ASCENDC_HAS_BISHENG_COMPILER)
#define ASCENDC_HOST __host__
#define ASCENDC_AICORE __aicore__
#define ASCENDC_HOST_AICORE __host_aicore__
#else
#define ASCENDC_HOST
#define ASCENDC_AICORE
#define ASCENDC_HOST_AICORE
#endif

#endif // IMPL_UTILS_COMMON_MARCOS_DEF_H