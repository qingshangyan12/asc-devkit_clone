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
 * \file c_api_constants.h
 * \brief
 */


#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif     

#ifndef INCLUDE_C_API_UTILS_C_API_CONSTANTS_H
#define INCLUDE_C_API_UTILS_C_API_CONSTANTS_H

#include <cstdint>
#include <type_traits>

#ifndef __aicore__
#define __aicore__ [aicore]
#endif // __aicore__

/*
 * VALUE_INDEX = 0;
 * INDEX_VALUE = 1;
 * ONLY_VALUE = 2;
 * ONLY_INDEX = 3;
*/
using order_t = Order_t;

constexpr uint16_t ASC_DATABLOCK_NUM = 8;
constexpr uint16_t ASC_ONE_DATABLOCK_SIZE = 32;

constexpr uint8_t ASC_DEFAULT_REPEAT = 1;
constexpr uint8_t ASC_DEFAULT_BLOCK_STRIDE = 1;
constexpr uint8_t ASC_DEFAULT_REPEAT_STRIDE = 8;
constexpr uint8_t ASC_HALF_DEFAULT_REPEAT_STRIDE = 4;
constexpr uint8_t ASC_ONE_FOURTH_DEFAULT_REPEAT_STRIDE = 2;

constexpr uint8_t ASC_MRGSORT_ELEMENT_LEN = 4;
constexpr uint8_t ASC_REDUCE_REPEAT_STRIDE = 1;

#endif
#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif    

