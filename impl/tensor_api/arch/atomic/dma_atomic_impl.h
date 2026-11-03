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
    "impl/tensor_api/arch/atomic/dma_atomic_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use \"#include \"tensor_api/arch/atomic/dma_atomic.h\" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_ATOMIC_DMA_ATOMIC_IMPL_H
#endif

/*!
 * \file dma_atomic_impl.h
 * \brief DMA atomic implementation.
 */
#ifndef IMPL_TENSOR_API_ARCH_ATOMIC_DMA_ATOMIC_IMPL_H
#define IMPL_TENSOR_API_ARCH_ATOMIC_DMA_ATOMIC_IMPL_H

#include "c_api/asc_simd.h"

namespace asc {
namespace te {
namespace detail {

template <typename T>
struct dma_atomic_dispatch {
    static_assert(sizeof(T) == 0, "Unsupported DMA atomic element type");
};

template <>
struct dma_atomic_dispatch<int8_t> {
    static __aicore__ inline void add() { asc_set_atomic_add_int8(); }
    static __aicore__ inline void max() { asc_set_atomic_max_int8(); }
    static __aicore__ inline void min() { asc_set_atomic_min_int8(); }
};

template <>
struct dma_atomic_dispatch<int16_t> {
    static __aicore__ inline void add() { asc_set_atomic_add_int16(); }
    static __aicore__ inline void max() { asc_set_atomic_max_int16(); }
    static __aicore__ inline void min() { asc_set_atomic_min_int16(); }
};

template <>
struct dma_atomic_dispatch<int32_t> {
    static __aicore__ inline void add() { asc_set_atomic_add_int32(); }
    static __aicore__ inline void max() { asc_set_atomic_max_int32(); }
    static __aicore__ inline void min() { asc_set_atomic_min_int32(); }
};

template <>
struct dma_atomic_dispatch<half> {
    static __aicore__ inline void add() { asc_set_atomic_add_float16(); }
    static __aicore__ inline void max() { asc_set_atomic_max_float16(); }
    static __aicore__ inline void min() { asc_set_atomic_min_float16(); }
};

template <>
struct dma_atomic_dispatch<bfloat16_t> {
    static __aicore__ inline void add() { asc_set_atomic_add_bfloat(); }
    static __aicore__ inline void max() { asc_set_atomic_max_bfloat(); }
    static __aicore__ inline void min() { asc_set_atomic_min_bfloat(); }
};

template <>
struct dma_atomic_dispatch<float> {
    static __aicore__ inline void add() { asc_set_atomic_add_float(); }
    static __aicore__ inline void max() { asc_set_atomic_max_float(); }
    static __aicore__ inline void min() { asc_set_atomic_min_float(); }
};

} // namespace detail

template <typename T>
__aicore__ inline void set_atomic_add()
{
    detail::dma_atomic_dispatch<T>::add();
}

template <typename T>
__aicore__ inline void set_atomic_max()
{
    detail::dma_atomic_dispatch<T>::max();
}

template <typename T>
__aicore__ inline void set_atomic_min()
{
    detail::dma_atomic_dispatch<T>::min();
}

__aicore__ inline void disable_dma_atomic() { asc_disable_dma_atomic(); }

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_ATOMIC_DMA_ATOMIC_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_ATOMIC_DMA_ATOMIC_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_IMPL_TENSOR_API_ARCH_ATOMIC_DMA_ATOMIC_IMPL_H
#endif
