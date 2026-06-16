/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "utils/debug/asc_printf.h"

#if defined(ASCENDC_SIMD_VF_DEBUG)
#error "utils/debug/asc_printf.h must not enable ASCENDC_SIMD_VF_DEBUG by default"
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_PRINTF_TEST__
#endif

#include "impl/utils/debug/asc_printf_config.h"

#if (__NPU_ARCH__ == 3510) && !defined(ASCENDC_SIMD_VF_DEBUG)
#error "impl/utils/debug/asc_printf_config.h must enable ASCENDC_SIMD_VF_DEBUG for the kernel_operator path"
#endif

#include "impl/utils/debug/asc_printf_impl.h"

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_PRINTF_TEST__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_PRINTF_TEST__
#endif
