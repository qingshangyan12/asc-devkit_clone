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
 * \file kernel_utils_macros.h
 * \brief
 */
#ifndef INCLUDE_BASIC_API_KERNEL_UTILS_MACROS_H
#define INCLUDE_BASIC_API_KERNEL_UTILS_MACROS_H

#include <cstdint>
#include "utils/base/sys_macros.h"

#ifndef __PLUGIN__KERNEL_META_TYPE_ENUME_DEFINED__
#define __PLUGIN__KERNEL_META_TYPE_ENUME_DEFINED__
enum KernelMetaType : uint8_t {
    KERNEL_TYPE_AIV_ONLY,
    KERNEL_TYPE_AIC_ONLY,
    KERNEL_TYPE_MIX_AIV_1_0,
    KERNEL_TYPE_MIX_AIC_1_0,
    KERNEL_TYPE_MIX_AIC_1_1,
    KERNEL_TYPE_MIX_AIC_1_2,
    KERNEL_TYPE_AICORE,
    KERNEL_TYPE_VECTORCORE,
    KERNEL_TYPE_MIX_AICORE,
    KERNEL_TYPE_MIX_VECTOR_CORE,
    KERNEL_TYPE_MAX,
};
#endif

#ifdef ASCENDC_CPU_DEBUG
#define PRELOAD(len) \
    {}
#else
#define PRELOAD(len)                                  \
    do {                                              \
        uint64_t pc;                                  \
        asm volatile("mov %0, pc \n" : "=l"(pc) : :); \
        preload((void*)pc, len);                      \
    } while (0)
#endif

#define TILING_STRUCT_SECTION_INIT_BASE(counter, val)                                                                 \
    static const uint64_t __ascendc_tiling_struct_##counter __attribute__((used, section(".ascendc_tiling." #val))) = \
        sizeof(val)
#define TILING_STRUCT_SECTION_INIT(counter, val) TILING_STRUCT_SECTION_INIT_BASE(counter, val)

#ifdef __CHECK_FEATURE_AT_PRECOMPILE
#define ENABLE_FEATURE_FOR_COMPILE(f, val) auto __enable_feature_for_compile_##f = val
#define ENABLE_FEATURE_FOR_TILING(expression, val) auto __enable_custom_tiling val = expression
#define REGISTER_NONE_TILING auto __enable_no_register_custom_tiling ascendc_trigger_tiling_struct = default
#else
#define ENABLE_FEATURE_FOR_COMPILE(f, val)
#define ENABLE_FEATURE_FOR_TILING(expression, val) TILING_STRUCT_SECTION_INIT(__COUNTER__, val)
#define REGISTER_NONE_TILING
#endif

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
#define KERNEL_TASK_TYPE(key, value)
#define KERNEL_TASK_TYPE_DEFAULT(value)
#else
#define KERNEL_TASK_TYPE(key, value) ENABLE_FEATURE_FOR_COMPILE(key, value)
#ifndef __PLUGIN__KERNEL_TASK_TYPE_DEFAULT_DEFINED__
#define __PLUGIN__KERNEL_TASK_TYPE_DEFAULT_DEFINED__
#define KERNEL_TASK_TYPE_DEFAULT(value) ENABLE_FEATURE_FOR_COMPILE(default, value)
#endif
#endif

#define REGISTER_TILING_DEFAULT(tiling_struct) ENABLE_FEATURE_FOR_TILING(default, tiling_struct)
#define REGISTER_TILING_FOR_TILINGKEY(expression, tiling_struct) ENABLE_FEATURE_FOR_TILING(expression, tiling_struct)

#ifndef TILING_KEY_VAR
#if defined(ASCENDC_CPU_DEBUG)
extern uint64_t g_tilingKey;
#else
#if __NPU_ARCH__ == 2002
[[block_local]] uint64_t g_tilingKey;
#else
[[workgroup_local]] __gm__ uint64_t g_tilingKey;
#endif
#endif
#define TILING_KEY_VAR g_tilingKey
#endif

#define TILING_KEY_IS(k) (TILING_KEY_VAR == (k))

#define TILING_KEY_LIST_INOUT(...) TILING_KEY_LIST_INOUT_IMPL(__VA_ARGS__)
#define TILING_KEY_LIST_INOUT_IMPL(...) \
    TILING_KEY_ARGS_CONCAT(TILING_KEY_INDEX_INOUT_, TILING_KEY_ARG_COUNT(__VA_ARGS__)(__VA_ARGS__))

#define TILING_KEY_INDEX_INOUT_1(a) TILING_KEY_VAR == (a)
#define TILING_KEY_INDEX_INOUT_2(a, ...) TILING_KEY_INDEX_INOUT_1(a) || TILING_KEY_INDEX_INOUT_1(__VA_ARGS__)
#define TILING_KEY_INDEX_INOUT_3(a, ...) TILING_KEY_INDEX_INOUT_1(a) || TILING_KEY_INDEX_INOUT_2(__VA_ARGS__)
#define TILING_KEY_INDEX_INOUT_4(a, ...) TILING_KEY_INDEX_INOUT_1(a) || TILING_KEY_INDEX_INOUT_3(__VA_ARGS__)
#define TILING_KEY_INDEX_INOUT_5(a, ...) TILING_KEY_INDEX_INOUT_1(a) || TILING_KEY_INDEX_INOUT_4(__VA_ARGS__)
#define TILING_KEY_INDEX_INOUT_6(a, ...) TILING_KEY_INDEX_INOUT_1(a) || TILING_KEY_INDEX_INOUT_5(__VA_ARGS__)
#define TILING_KEY_INDEX_INOUT_7(a, ...) TILING_KEY_INDEX_INOUT_1(a) || TILING_KEY_INDEX_INOUT_6(__VA_ARGS__)
#define TILING_KEY_INDEX_INOUT_8(a, ...) TILING_KEY_INDEX_INOUT_1(a) || TILING_KEY_INDEX_INOUT_7(__VA_ARGS__)

#define TILING_KEY_ARG_COUNT(...) TILING_KEY_ARG_COUNT_IMPL(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define TILING_KEY_ARG_COUNT_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N

#define TILING_KEY_ARGS_CONCAT(a, b) TILING_KEY_ARGS_CONCAT_IMPL(a, b)
#define TILING_KEY_ARGS_CONCAT_IMPL(a, b) a##b

#ifdef __CHECK_FEATURE_AT_PRECOMPILE
#define TILING_KEY_LIST(...) (TILING_KEY_LIST_INOUT(__VA_ARGS__)) "TILING_KEY_LIST"
#else
#define TILING_KEY_LIST(...) (TILING_KEY_LIST_INOUT(__VA_ARGS__))
#endif

#endif // INCLUDE_BASIC_API_KERNEL_UTILS_MACROS_H
