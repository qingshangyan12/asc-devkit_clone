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
 * \file asc_time.h
 * \brief
 */

#ifndef INCLUDE_UTILS_DEBUG_ASC_TIME_H
#define INCLUDE_UTILS_DEBUG_ASC_TIME_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_TIME_H__
#endif

#ifndef __CHECK_FEATURE_AT_PRECOMPILE

namespace __asc_aicore {
__aicore__ inline void asc_time_stamp(uint32_t desc_id);

__aicore__ inline void asc_prof_start();

__aicore__ inline void asc_prof_stop();

#if __NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3501
template<pipe_t pipe = PIPE_S>
__aicore__ inline void asc_mark_stamp(uint16_t idx);

template<pipe_t pipe = PIPE_S, uint16_t idx>
__aicore__ inline void asc_mark_stamp();
#endif
}   // namespase __asc_aicore

#define COUNT_ARGS_IMPL(_1, _2, N, ...) N
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__, 2, 1)
#define GET_ARG_COUNT(...) COUNT_ARGS(__VA_ARGS__)
#define TRACE_START_IMPL(N, ...) TRACE_START_FUNC(N, __VA_ARGS__)
#define TRACE_START_FUNC(N, ...) TRACE_START_##N(__VA_ARGS__)
#define TRACE_START(...) TRACE_START_IMPL(GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)
#define TRACE_START_1(idx) asc_mark_stamp<PIPE_S>(idx)
#define TRACE_START_2(pipe, idx) asc_mark_stamp<pipe>(idx)

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5102)
__aicore__ inline void TRACE_START(TraceId apid)
{}
__aicore__ inline void TRACE_STOP(TraceId apid)
{}

#elif defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || \
      (__NPU_ARCH__ == 3113)) || (__NPU_ARCH__ == 3101)
    #define TRACE_START(apid)                                          \
    do {                                                           \
        uint32_t v = (ASC_PROF_START_EVENT | static_cast<uint32_t>(apid));                               \
        __asm__ __volatile__("");                                  \
        asm volatile("MOV COND, %0\n" : "+l"(v));                  \
        __asm__ __volatile__("");                                  \
    } while (0)

#define TRACE_STOP(apid)                                          \
    do {                                                          \
        uint32_t v = (ASC_PROF_STOP_EVENT | static_cast<uint32_t>(apid));                              \
        __asm__ __volatile__("");                                 \
        asm volatile("MOV COND, %0\n" : "+l"(v));                 \
        __asm__ __volatile__("");                                 \
    } while (0)
#else
#define TRACE_START(apid)                                          \
    do {                                                           \
        set_lpcnt(ASC_PROF_START_EVENT | static_cast<uint32_t>(apid)); \
        prof_mark_event();                                           \
    } while (0)

#define TRACE_STOP(apid)                                          \
    do {                                                          \
        set_lpcnt(ASC_PROF_STOP_EVENT | static_cast<uint32_t>(apid)); \
        prof_mark_event();                                          \
    } while (0)
#endif

#endif

#include "impl/utils/debug/asc_aicore_time_impl.h"

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_TIME_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_TIME_H__
#endif

#endif