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
 * \file check_condition.h
 * \brief
 */
#ifndef INCLUDE_UTILS_DEBUG_CHECK_CONDITION_H
#define INCLUDE_UTILS_DEBUG_CHECK_CONDITION_H

namespace AscendC {

inline void check_condition(bool condition, const char* violation_message, const char* file, int line);

} // namespace AscendC

#include "impl/utils/debug/check_condition_impl.h"

#define ASC_HOST_CHECK_CONDITION(condition, violation_message) \
    AscendC::check_condition((condition), (violation_message), __FILE__, __LINE__)

#endif // INCLUDE_UTILS_DEBUG_CHECK_CONDITION_H
