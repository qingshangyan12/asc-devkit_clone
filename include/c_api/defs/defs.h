/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "defs.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DEFS_H
#endif

#ifndef INCLUDE_C_API_DEFS_H
#define INCLUDE_C_API_DEFS_H

#include "constant.h"
#include "enum.h"
#include "impl/utils/common_types.h"
#include "macro.h"
#include "struct.h"
#include "type.h"
#include "union.h"

// The CPU debug test stub exposes uint8_t sync overloads while the CANN
// cpudebug implementation exposes uint64_t overloads.  This exact-match
// fallback keeps integer sync IDs unambiguous for 3510 host-side builds.
#if defined(ASCENDC_CPU_DEBUG) && defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
template <typename Pipe, typename SyncId>
inline void set_intra_block(Pipe, SyncId) {}

template <typename Pipe, typename SyncId>
inline void wait_intra_block(Pipe, SyncId) {}
#endif

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DEFS_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DEFS_H
#endif
