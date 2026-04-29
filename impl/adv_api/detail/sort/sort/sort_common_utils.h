/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file sort_common_utils.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/sort/sort/sort_common_utils.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/sort/sort.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SORT_SORT_SORT_COMMON_UTILS_H__
#endif

#ifndef IMPL_SORT_SORT_SORT_COMMON_UTILS_H
#define IMPL_SORT_SORT_SORT_COMMON_UTILS_H

#include "include/adv_api/sort/sort_utils_constants.h"

namespace AscendC {
#ifndef ASCC_STRUCT_SORTCONFIG
#define ASCC_STRUCT_SORTCONFIG
struct SortConfig {
    SortType type = SortType::RADIX_SORT;
    bool isDescend = false;
};
#endif

constexpr SortConfig DEFAULT_SORT_CONFIG = {SortType::RADIX_SORT, false};
}

#endif // IMPL_SORT_SORT_SORT_COMMON_UTILS_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SORT_SORT_SORT_COMMON_UTILS_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SORT_SORT_SORT_COMMON_UTILS_H__
#endif
