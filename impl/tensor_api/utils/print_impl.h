/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/utils/print_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/utils/print.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file print_impl.h
 * \brief Tensor API debug print helpers implementation.
 */
#ifndef IMPL_TENSOR_API_UTILS_PRINT_IMPL_H
#define IMPL_TENSOR_API_UTILS_PRINT_IMPL_H

#include "impl/tensor_api/tensor/tensor_impl.h"
#include "utils/debug/asc_printf.h"
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdio>
#endif

namespace asc {
namespace te {
namespace detail {

template <typename T>
struct is_print_char_pointer : Std::false_type {};

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
template <>
struct is_print_char_pointer<char*> : Std::true_type {};

template <>
struct is_print_char_pointer<const char*> : Std::true_type {};
#else
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"
template <>
struct is_print_char_pointer<__gm__ char*> : Std::true_type {};

template <>
struct is_print_char_pointer<__gm__ const char*> : Std::true_type {};
#pragma clang diagnostic pop
#endif

template <typename T>
constexpr bool is_print_string_v = is_print_char_pointer<Std::decay_t<T>>::value;

__aicore__ inline int num_digits(long long x)
{
    unsigned long long value = x < 0 ? 0ULL - static_cast<unsigned long long>(x) : static_cast<unsigned long long>(x);
    int digits = x < 0 ? 2 : 1;
    while (value >= 10) {
        value /= 10;
        ++digits;
    }
    return digits;
}

template <typename... Args>
__aicore__ inline void print_format(__gm__ const char* fmt, Args&&... args)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    std::printf(fmt, static_cast<Args&&>(args)...);
#else
    __asc_aicore::printf(fmt, static_cast<Args&&>(args)...);
#endif
#else
    (void)fmt;
    ((void)args, ...);
#endif
}

__aicore__ inline void emit_spaces(int count)
{
    for (int i = 0; i < count; ++i) {
        print_format((__gm__ const char*)" ");
    }
}

__aicore__ inline void emit_padded_int(long long value, int width)
{
    emit_spaces(width - num_digits(value));
    print_format((__gm__ const char*)"%lld", value);
}

template <typename T>
__aicore__ inline void emit_tuple_element(const T& value);

template <typename Tuple, size_t I, size_t N>
struct tuple_printer {
    __aicore__ inline static void print(const Tuple& tuple)
    {
        emit_tuple_element(Std::get<I>(tuple));
        if constexpr (I + 1 < N) {
            print_format((__gm__ const char*)", ");
            tuple_printer<Tuple, I + 1, N>::print(tuple);
        }
    }
};

template <typename Tuple, size_t N>
struct tuple_printer<Tuple, N, N> {
    __aicore__ inline static void print(const Tuple&) {}
};

template <typename T>
__aicore__ inline void emit_tuple_element(const T& value)
{
    using value_type = Std::remove_cvref_t<T>;
    if constexpr (Std::is_tuple_v<value_type>) {
        print_format((__gm__ const char*)"(");
        tuple_printer<value_type, 0, Std::tuple_size_v<value_type>>::print(value);
        print_format((__gm__ const char*)")");
    } else if constexpr (is_print_string_v<T>) {
        print_format((__gm__ const char*)"%s", value);
    } else if constexpr (is_one_of_attr_v<value_type, half, bfloat16_t>) {
        print_format((__gm__ const char*)"%f", static_cast<float>(value));
    } else if constexpr (is_one_of_attr_v<value_type, float>) {
        print_format((__gm__ const char*)"%f", static_cast<float>(value));
    } else if constexpr (Std::is_floating_point_v<value_type>) {
        print_format((__gm__ const char*)"%f", static_cast<double>(value));
    } else if constexpr (is_one_of_attr_v<value_type, uint8_t, uint16_t, uint32_t, uint64_t>) {
        print_format((__gm__ const char*)"%llu", static_cast<unsigned long long>(value));
    } else if constexpr (Std::is_integral_v<value_type>) {
        print_format((__gm__ const char*)"%lld", static_cast<long long>(value));
    } else {
        print_format((__gm__ const char*)"%lld", static_cast<long long>(value));
    }
}

template <typename T>
__aicore__ inline void emit_value(const T& value)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<T>>) {
        emit_tuple_element(value);
    } else if constexpr (is_layout_v<T>) {
        print_format((__gm__ const char*)"layout(shape=");
        emit_tuple_element(value.shape());
        print_format((__gm__ const char*)", stride=");
        emit_tuple_element(value.stride());
        print_format((__gm__ const char*)")");
    } else if constexpr (is_attr_tensor_v<T>) {
        print_format((__gm__ const char*)"tensor<");
        emit_value(value.layout());
        print_format((__gm__ const char*)">");
    } else {
        emit_tuple_element(value);
    }
}

template <typename ValueType, typename T>
__aicore__ inline void emit_element_as(const T& value)
{
    if constexpr (Std::is_same_v<ValueType, float> || Std::is_same_v<ValueType, half>
                  || Std::is_same_v<ValueType, bfloat16_t>) {
        print_format((__gm__ const char*)"%f  ", static_cast<float>(value));
    } else if constexpr (Std::is_same_v<ValueType, int8_t> || Std::is_same_v<ValueType, int16_t>
                         || Std::is_same_v<ValueType, int32_t>) {
        print_format((__gm__ const char*)"%d  ", static_cast<int32_t>(value));
    } else if constexpr (Std::is_same_v<ValueType, int64_t>) {
        print_format((__gm__ const char*)"%lld  ", static_cast<long long>(value));
    } else if constexpr (Std::is_same_v<ValueType, uint8_t> || Std::is_same_v<ValueType, uint16_t>
                         || Std::is_same_v<ValueType, uint32_t>) {
        print_format((__gm__ const char*)"%u  ", static_cast<uint32_t>(value));
    } else if constexpr (Std::is_same_v<ValueType, uint64_t>) {
        print_format((__gm__ const char*)"%llu  ", static_cast<unsigned long long>(value));
    } else {
        print_format((__gm__ const char*)"  ");
        emit_value(value);
    }
}

template <typename T>
__aicore__ inline void emit_layout_table(const T& layout)
{
    static_assert(is_layout_v<T>, "print expects a Tensor API layout.");
    static_assert(T::rank == 2, "print only supports rank-2 layout tables.");

    int idx_width = num_digits(static_cast<long long>(cosize(layout))) + 2;
    emit_value(layout);
    print_format((__gm__ const char*)"\n");

    print_format((__gm__ const char*)"    ");
    for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
        print_format((__gm__ const char*)"  ");
        emit_padded_int(n, idx_width - 2);
        print_format((__gm__ const char*)" ");
    }
    print_format((__gm__ const char*)"\n");

    for (int m = 0; m < static_cast<int>(size<0>(layout)); ++m) {
        print_format((__gm__ const char*)"    ");
        for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
            print_format((__gm__ const char*)"+");
            for (int i = 0; i < idx_width; ++i) {
                print_format((__gm__ const char*)"-");
            }
        }
        print_format((__gm__ const char*)"+\n");

        emit_padded_int(m, 2);
        print_format((__gm__ const char*)"  ");
        for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
            print_format((__gm__ const char*)"| ");
            emit_padded_int(static_cast<long long>(layout(make_coord(m, n))), idx_width - 2);
            print_format((__gm__ const char*)" ");
        }
        print_format((__gm__ const char*)"|\n");
    }

    print_format((__gm__ const char*)"    ");
    for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
        print_format((__gm__ const char*)"+");
        for (int i = 0; i < idx_width; ++i) {
            print_format((__gm__ const char*)"-");
        }
    }
    print_format((__gm__ const char*)"+\n");
}

template <typename T>
__aicore__ inline void emit_tensor_content(const T& tensor)
{
    static_assert(is_attr_tensor_v<T>, "print expects a Tensor API tensor created by make_tensor.");
    using layout_type = typename Std::remove_cvref_t<T>::layout_type;
    using element_type = typename Std::remove_cvref_t<T>::element_type;
    using data_type = get_attribute_element_type<element_type*>;
    static_assert(layout_type::rank >= 1 && layout_type::rank <= 4, "print only supports rank 1 to 4 tensors.");

    emit_value(tensor);
    print_format((__gm__ const char*)":\n");

    if constexpr (layout_type::rank == 1) {
        for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
            emit_element_as<data_type>(tensor[make_coord(m)]);
            print_format((__gm__ const char*)"\n");
        }
    } else if constexpr (layout_type::rank == 2) {
        for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
            for (int n = 0; n < static_cast<int>(size<1>(tensor.layout())); ++n) {
                emit_element_as<data_type>(tensor[make_coord(m, n)]);
            }
            print_format((__gm__ const char*)"\n");
        }
    } else if constexpr (layout_type::rank == 3) {
        for (int k = 0; k < static_cast<int>(size<2>(tensor.layout())); ++k) {
            if (k != 0) {
                for (int i = 0; i < 5 * static_cast<int>(size<1>(tensor.layout())); ++i) {
                    print_format((__gm__ const char*)"-");
                }
                print_format((__gm__ const char*)"\n");
            }
            for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
                for (int n = 0; n < static_cast<int>(size<1>(tensor.layout())); ++n) {
                    emit_element_as<data_type>(tensor[make_coord(m, n, k)]);
                }
                print_format((__gm__ const char*)"\n");
            }
        }
    } else if constexpr (layout_type::rank == 4) {
        for (int p = 0; p < static_cast<int>(size<3>(tensor.layout())); ++p) {
            if (p != 0) {
                for (int i = 0; i < 5 * static_cast<int>(size<1>(tensor.layout())); ++i) {
                    print_format((__gm__ const char*)"=");
                }
                print_format((__gm__ const char*)"\n");
            }
            for (int k = 0; k < static_cast<int>(size<2>(tensor.layout())); ++k) {
                if (k != 0) {
                    for (int i = 0; i < 5 * static_cast<int>(size<1>(tensor.layout())); ++i) {
                        print_format((__gm__ const char*)"-");
                    }
                    print_format((__gm__ const char*)"\n");
                }
                for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
                    for (int n = 0; n < static_cast<int>(size<1>(tensor.layout())); ++n) {
                        emit_element_as<data_type>(tensor[make_coord(m, n, k, p)]);
                    }
                    print_format((__gm__ const char*)"\n");
                }
            }
        }
    }
}

} // namespace detail

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
__aicore__ inline void print(const char* fmt)
{
    detail::print_format(fmt);
}

template <typename Arg0, typename... Args>
__aicore__ inline void print(const char* fmt, Arg0&& arg0, Args&&... args)
{
    detail::print_format(fmt, static_cast<Arg0&&>(arg0), static_cast<Args&&>(args)...);
}

#else
__aicore__ inline void print(__gm__ const char* fmt)
{
    detail::print_format(fmt);
}

template <typename Arg0, typename... Args>
__aicore__ inline void print(__gm__ const char* fmt, Arg0&& arg0, Args&&... args)
{
    detail::print_format(fmt, static_cast<Arg0&&>(arg0), static_cast<Args&&>(args)...);
}
#endif // ASCENDC_CPU_DEBUG

template <typename T, typename Std::enable_if_t<!detail::is_print_string_v<T>, int> = 0>
__aicore__ inline void print(const T& value)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    if constexpr (is_attr_tensor_v<T>) {
        detail::emit_tensor_content(value);
    } else if constexpr (is_layout_v<T>) {
        if constexpr (Std::remove_cvref_t<T>::rank == 2) {
            detail::emit_layout_table(value);
        } else {
            detail::emit_value(value);
        }
    } else {
        detail::emit_value(value);
    }
#else
    (void)value;
#endif
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_UTILS_PRINT_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
