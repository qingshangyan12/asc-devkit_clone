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
 * \file check_not_null_impl.h
 * \brief Internal implementation of the Host-side null pointer check.
 */
#ifndef IMPL_UTILS_DEBUG_CHECK_NOT_NULL_IMPL_H
#define IMPL_UTILS_DEBUG_CHECK_NOT_NULL_IMPL_H

#include <cstdio>
#include <cstdlib>

namespace AscendC {

inline void CheckNotNull(const void* ptr, const char* ptrName, const char* file, int line)
{
    if (ptr != nullptr) {
        return;
    }
    std::fprintf(stderr, "CHECK_NOT_NULL failed: pointer '%s' is null, file=%s, line=%d\n", ptrName, file, line);
    std::exit(EXIT_FAILURE);
}

} // namespace AscendC

#endif // IMPL_UTILS_DEBUG_CHECK_NOT_NULL_IMPL_H
