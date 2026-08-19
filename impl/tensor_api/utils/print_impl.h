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

#include "include/tensor_api/utils/utils.h"
#include "impl/tensor_api/tensor/tensor_impl.h"
#include "utils/debug/asc_printf.h"

namespace asc {
namespace te {
namespace detail {

template <typename T>
struct is_print_char_pointer : Std::false_type {};

template <>
struct is_print_char_pointer<char*> : Std::true_type {};

template <>
struct is_print_char_pointer<const char*> : Std::true_type {};

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
    __asc_aicore::printf(fmt, static_cast<Args&&>(args)...);
#else
    (void)fmt;
    ((void)args, ...);
#endif
}

struct print_session {
    bool first_fragment = true;
};

__aicore__ static __attribute__((noinline)) void print_fragment(print_session& session, __gm__ const char* fmt)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    if (session.first_fragment) {
        session.first_fragment = false;
        __asc_aicore::printf(fmt);
    } else {
        __asc_aicore::scalar_printf_impl(__asc_aicore::DumpType::DUMP_SCALAR, fmt, (__gm__ const char*)"");
    }
#else
    (void)session;
    (void)fmt;
#endif
}

template <typename ArgType>
__aicore__ static __attribute__((noinline)) void print_fragment(print_session& session, __gm__ const char* fmt,
                                                                ArgType arg)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    if (session.first_fragment) {
        session.first_fragment = false;
        __asc_aicore::printf(fmt, arg);
    } else {
        __asc_aicore::scalar_printf_impl(__asc_aicore::DumpType::DUMP_SCALAR, fmt, (__gm__ const char*)"", arg);
    }
#else
    (void)session;
    (void)fmt;
    (void)arg;
#endif
}

__aicore__ inline void emit_spaces(print_session& session, int count)
{
    for (int i = 0; i < count; ++i) {
        print_fragment(session, (__gm__ const char*)" ");
    }
}

__aicore__ inline void emit_padded_int(print_session& session, long long value, int width)
{
    emit_spaces(session, width - num_digits(value));
    print_fragment(session, (__gm__ const char*)"%lld", value);
}

template <typename T>
__aicore__ inline void emit_tuple_element(print_session& session, const T& value);

template <typename Tuple, size_t I, size_t N>
struct tuple_printer {
    __aicore__ inline static void print(print_session& session, const Tuple& tuple)
    {
        emit_tuple_element(session, Std::get<I>(tuple));
        if constexpr (I + 1 < N) {
            print_fragment(session, (__gm__ const char*)", ");
            tuple_printer<Tuple, I + 1, N>::print(session, tuple);
        }
    }
};

template <typename Tuple, size_t N>
struct tuple_printer<Tuple, N, N> {
    __aicore__ inline static void print(print_session&, const Tuple&) {}
};

template <typename T>
__aicore__ inline void emit_tuple_element(print_session& session, const T& value)
{
    using ValueType = Std::remove_cvref_t<T>;
    if constexpr (Std::is_tuple_v<ValueType>) {
        print_fragment(session, (__gm__ const char*)"(");
        tuple_printer<ValueType, 0, Std::tuple_size_v<ValueType>>::print(session, value);
        print_fragment(session, (__gm__ const char*)")");
    } else if constexpr (is_print_string_v<T>) {
        print_fragment(session, (__gm__ const char*)"%s", value);
    } else if constexpr (is_one_of_attr_v<ValueType, half, bfloat16_t>) {
        print_fragment(session, (__gm__ const char*)"%f", static_cast<float>(value));
    } else if constexpr (is_one_of_attr_v<ValueType, float>) {
        print_fragment(session, (__gm__ const char*)"%f", static_cast<float>(value));
    } else if constexpr (Std::is_floating_point_v<ValueType>) {
        print_fragment(session, (__gm__ const char*)"%f", static_cast<double>(value));
    } else if constexpr (is_one_of_attr_v<ValueType, uint8_t, uint16_t, uint32_t, uint64_t>) {
        print_fragment(session, (__gm__ const char*)"%llu", static_cast<unsigned long long>(value));
    } else if constexpr (Std::is_integral_v<ValueType>) {
        print_fragment(session, (__gm__ const char*)"%lld", static_cast<long long>(value));
    } else {
        print_fragment(session, (__gm__ const char*)"%lld", static_cast<long long>(value));
    }
}

template <typename T>
__aicore__ inline void emit_value(print_session& session, const T& value)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<T>>) {
        emit_tuple_element(session, value);
    } else if constexpr (is_layout_v<T>) {
        print_fragment(session, (__gm__ const char*)"layout(shape=");
        emit_tuple_element(session, value.shape());
        print_fragment(session, (__gm__ const char*)", stride=");
        emit_tuple_element(session, value.stride());
        print_fragment(session, (__gm__ const char*)")");
    } else if constexpr (is_attr_tensor_v<T>) {
        using ElementType = typename Std::remove_cvref_t<T>::element_type;
        using DataType = get_attribute_element_type<ElementType*>;
        constexpr uint32_t ELEMENT_BITS = is_b4_type<DataType> ? 4U : sizeof(DataType) * 8U;
        print_fragment(session, (__gm__ const char*)"ptr[%ub](", ELEMENT_BITS);
        print_fragment(session, (__gm__ const char*)"%p", value.data().get());
        print_fragment(session, (__gm__ const char*)") o ");
        emit_value(session, value.layout());
    } else {
        emit_tuple_element(session, value);
    }
}

template <typename ValueType, typename T>
__aicore__ inline void emit_element_as(print_session& session, const T& value)
{
    if constexpr (Std::is_same_v<ValueType, float> || Std::is_same_v<ValueType, half>
                  || Std::is_same_v<ValueType, bfloat16_t>) {
        print_fragment(session, (__gm__ const char*)"%f  ", static_cast<float>(value));
    } else if constexpr (Std::is_same_v<ValueType, int8_t> || Std::is_same_v<ValueType, int16_t>
                         || Std::is_same_v<ValueType, int32_t>) {
        print_fragment(session, (__gm__ const char*)"%d  ", static_cast<int32_t>(value));
    } else if constexpr (Std::is_same_v<ValueType, int64_t>) {
        print_fragment(session, (__gm__ const char*)"%lld  ", static_cast<long long>(value));
    } else if constexpr (Std::is_same_v<ValueType, uint8_t> || Std::is_same_v<ValueType, uint16_t>
                         || Std::is_same_v<ValueType, uint32_t>) {
        print_fragment(session, (__gm__ const char*)"%u  ", static_cast<uint32_t>(value));
    } else if constexpr (Std::is_same_v<ValueType, uint64_t>) {
        print_fragment(session, (__gm__ const char*)"%llu  ", static_cast<unsigned long long>(value));
    } else {
        print_fragment(session, (__gm__ const char*)"  ");
        emit_value(session, value);
    }
}

template <typename T>
__aicore__ inline void emit_layout_table(print_session& session, const T& layout)
{
    static_assert(is_layout_v<T>, "print_layout expects a Tensor API layout.");
    static_assert(Std::remove_cvref_t<T>::rank_size == 2, "print_layout only supports rank-2 layouts.");

    int idx_width = num_digits(static_cast<long long>(cosize(layout))) + 2;
    emit_value(session, layout);
    print_fragment(session, (__gm__ const char*)"\n");

    print_fragment(session, (__gm__ const char*)"    ");
    for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
        print_fragment(session, (__gm__ const char*)"  ");
        emit_padded_int(session, n, idx_width - 2);
        print_fragment(session, (__gm__ const char*)" ");
    }
    print_fragment(session, (__gm__ const char*)"\n");

    for (int m = 0; m < static_cast<int>(size<0>(layout)); ++m) {
        print_fragment(session, (__gm__ const char*)"    ");
        for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
            print_fragment(session, (__gm__ const char*)"+");
            for (int i = 0; i < idx_width; ++i) {
                print_fragment(session, (__gm__ const char*)"-");
            }
        }
        print_fragment(session, (__gm__ const char*)"+\n");

        emit_padded_int(session, m, 2);
        print_fragment(session, (__gm__ const char*)"  ");
        for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
            print_fragment(session, (__gm__ const char*)"| ");
            emit_padded_int(session, static_cast<long long>(layout(make_coord(m, n))), idx_width - 2);
            print_fragment(session, (__gm__ const char*)" ");
        }
        print_fragment(session, (__gm__ const char*)"|\n");
    }

    print_fragment(session, (__gm__ const char*)"    ");
    for (int n = 0; n < static_cast<int>(size<1>(layout)); ++n) {
        print_fragment(session, (__gm__ const char*)"+");
        for (int i = 0; i < idx_width; ++i) {
            print_fragment(session, (__gm__ const char*)"-");
        }
    }
    print_fragment(session, (__gm__ const char*)"+\n");
}

__aicore__ inline void emit_tensor_separator(print_session& session, int width, __gm__ const char* separator)
{
    for (int i = 0; i < width; ++i) {
        print_fragment(session, separator);
    }
    print_fragment(session, (__gm__ const char*)"\n");
}

template <typename DataType, typename T>
__aicore__ inline void emit_rank_one_tensor(print_session& session, const T& tensor)
{
    for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
        emit_element_as<DataType>(session, tensor[make_coord(m)]);
        print_fragment(session, (__gm__ const char*)"\n");
    }
}

template <typename DataType, typename T>
__aicore__ inline void emit_rank_two_tensor(print_session& session, const T& tensor)
{
    for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
        for (int n = 0; n < static_cast<int>(size<1>(tensor.layout())); ++n) {
            emit_element_as<DataType>(session, tensor[make_coord(m, n)]);
        }
        print_fragment(session, (__gm__ const char*)"\n");
    }
}

template <typename DataType, typename T>
__aicore__ inline void emit_rank_three_plane(print_session& session, const T& tensor, int k)
{
    for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
        for (int n = 0; n < static_cast<int>(size<1>(tensor.layout())); ++n) {
            emit_element_as<DataType>(session, tensor[make_coord(m, n, k)]);
        }
        print_fragment(session, (__gm__ const char*)"\n");
    }
}

template <typename DataType, typename T>
__aicore__ inline void emit_rank_three_tensor(print_session& session, const T& tensor)
{
    int separator_width = 5 * static_cast<int>(size<1>(tensor.layout()));
    for (int k = 0; k < static_cast<int>(size<2>(tensor.layout())); ++k) {
        if (k != 0) {
            emit_tensor_separator(session, separator_width, (__gm__ const char*)"-");
        }
        emit_rank_three_plane<DataType>(session, tensor, k);
    }
}

template <typename DataType, typename T>
__aicore__ inline void emit_rank_four_plane(print_session& session, const T& tensor, int k, int p)
{
    for (int m = 0; m < static_cast<int>(size<0>(tensor.layout())); ++m) {
        for (int n = 0; n < static_cast<int>(size<1>(tensor.layout())); ++n) {
            emit_element_as<DataType>(session, tensor[make_coord(m, n, k, p)]);
        }
        print_fragment(session, (__gm__ const char*)"\n");
    }
}

template <typename DataType, typename T>
__aicore__ inline void emit_rank_four_tensor(print_session& session, const T& tensor)
{
    int separator_width = 5 * static_cast<int>(size<1>(tensor.layout()));
    for (int p = 0; p < static_cast<int>(size<3>(tensor.layout())); ++p) {
        if (p != 0) {
            emit_tensor_separator(session, separator_width, (__gm__ const char*)"=");
        }
        for (int k = 0; k < static_cast<int>(size<2>(tensor.layout())); ++k) {
            if (k != 0) {
                emit_tensor_separator(session, separator_width, (__gm__ const char*)"-");
            }
            emit_rank_four_plane<DataType>(session, tensor, k, p);
        }
    }
}

template <typename T>
__aicore__ inline void emit_tensor_content(print_session& session, const T& tensor, bool print_type)
{
    static_assert(is_attr_tensor_v<T>, "print_tensor expects a Tensor API tensor created by make_tensor.");
    using LayoutType = typename Std::remove_cvref_t<T>::layout_type;
    using ElementType = typename Std::remove_cvref_t<T>::element_type;
    using DataType = get_attribute_element_type<ElementType*>;
    static_assert(LayoutType::rank_size >= 1 && LayoutType::rank_size <= 4,
                  "print_tensor only supports rank 1 to 4 tensors.");

    if (print_type) {
        emit_value(session, tensor);
        print_fragment(session, (__gm__ const char*)":\n");
    }
    if constexpr (LayoutType::rank_size == 1) {
        emit_rank_one_tensor<DataType>(session, tensor);
    } else if constexpr (LayoutType::rank_size == 2) {
        emit_rank_two_tensor<DataType>(session, tensor);
    } else if constexpr (LayoutType::rank_size == 3) {
        emit_rank_three_tensor<DataType>(session, tensor);
    } else if constexpr (LayoutType::rank_size == 4) {
        emit_rank_four_tensor<DataType>(session, tensor);
    }
}

} // namespace detail

__aicore__ inline void print(__gm__ const char* fmt)
{
    detail::print_format(fmt);
}

template <typename Arg0, typename... Args>
__aicore__ inline void print(__gm__ const char* fmt, Arg0&& arg0, Args&&... args)
{
    detail::print_format(fmt, static_cast<Arg0&&>(arg0), static_cast<Args&&>(args)...);
}

template <typename T, typename Std::enable_if_t<!detail::is_print_string_v<T>, int> = 0>
__aicore__ inline void print(const T& value)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    detail::print_session session;
    detail::emit_value(session, value);
    detail::print_fragment(session, (__gm__ const char*)"\n");
#else
    (void)value;
#endif
}

template <typename LayoutType>
__aicore__ inline void print_layout(const LayoutType& layout)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    detail::print_session session;
    detail::emit_layout_table(session, layout);
#else
    (void)layout;
#endif
}

template <typename TensorType>
__aicore__ inline void print_tensor(const TensorType& tensor, bool print_type = true)
{
#if !(defined(ASCENDC_DUMP) && (ASCENDC_DUMP == 0))
    detail::print_session session;
    detail::emit_tensor_content(session, tensor, print_type);
#else
    (void)tensor;
    (void)print_type;
#endif
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_UTILS_PRINT_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
