/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef INCLUDE_C_API_C_API_UTILS_INTF_H
#define INCLUDE_C_API_C_API_UTILS_INTF_H

#include "c_api/utils/union.h"

enum class asc_position { EVEN = 0, ODD };

constexpr std::integral_constant<asc_position, asc_position::EVEN> ASC_POSITION_EVEN;
constexpr std::integral_constant<asc_position, asc_position::ODD> ASC_POSITION_ODD;

enum class asc_disperse_quarter { FIRST = 0, SECOND, THIRD, FOURTH };

constexpr std::integral_constant<asc_disperse_quarter, asc_disperse_quarter::FIRST> ASC_DISPERSE_FIRST_QUARTER;
constexpr std::integral_constant<asc_disperse_quarter, asc_disperse_quarter::SECOND> ASC_DISPERSE_SECOND_QUARTER;
constexpr std::integral_constant<asc_disperse_quarter, asc_disperse_quarter::THIRD> ASC_DISPERSE_THIRD_QUARTER;
constexpr std::integral_constant<asc_disperse_quarter, asc_disperse_quarter::FOURTH> ASC_DISPERSE_FOURTH_QUARTER;

#endif
