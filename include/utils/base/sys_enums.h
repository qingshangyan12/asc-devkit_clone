/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file sys_enums.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_UTILS_BASE_SYS_ENUMS_H__
#endif

#ifndef INCLUDE_UTILS_BASE_SYS_ENUMS_H
#define INCLUDE_UTILS_BASE_SYS_ENUMS_H

#include <cstdint>

enum class asc_saturation_mode : uint8_t { FLOAT, FLOAT8, INT, CAST };

enum class asc_override_strategy : uint8_t { USE_API = 0, USE_GLOBAL = 1 };

#endif // INCLUDE_UTILS_BASE_SYS_ENUMS_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_UTILS_BASE_SYS_ENUMS_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_UTILS_BASE_SYS_ENUMS_H__
#endif
