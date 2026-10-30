/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/tensor_api/arch/sync/mutex_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use \"#include \"tensor_api/arch/sync/mutex.h\" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file mutex_impl.h
 * \brief Object-style pipeline synchronization implementation.
 */
#ifndef IMPL_TENSOR_API_ARCH_SYNC_MUTEX_IMPL_H
#define IMPL_TENSOR_API_ARCH_SYNC_MUTEX_IMPL_H

#include "c_api/asc_simd.h"

namespace asc {
namespace te {

template <pipe_t Pipe>
__aicore__ inline mutex<Pipe>::mutex(mutex_id id) : id(id)
{}

template <pipe_t Pipe>
__aicore__ inline void mutex<Pipe>::lock()
{
    asc_lock(Pipe, id);
}

template <pipe_t Pipe>
__aicore__ inline void mutex<Pipe>::unlock()
{
    asc_unlock(Pipe, id);
}

template <typename Mutex>
__aicore__ inline lock_guard<Mutex>::lock_guard(Mutex& mutex) : mutex(mutex)
{
    mutex.lock();
}

template <typename Mutex>
__aicore__ inline lock_guard<Mutex>::~lock_guard()
{
    mutex.unlock();
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_SYNC_MUTEX_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
