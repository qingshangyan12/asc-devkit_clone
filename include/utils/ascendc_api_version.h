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
 * \file ascendc_api_version.h
 * \brief
 */

#ifndef INCLUDE_UTILS_ASCENDC_API_VERSION_H
#define INCLUDE_UTILS_ASCENDC_API_VERSION_H

#ifndef ASCENDC_API_VERSION
#define ASCENDC_API_VERSION 20250330
#endif

static uint64_t ascendcApiVersion __attribute__((section(".ascendc.api.version"))) = 20260330;

static volatile uint64_t ascendcUseApiVersion = ascendcApiVersion;

#endif // INCLUDE_UTILS_ASCENDC_API_VERSION_H