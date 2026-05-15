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
    "impl/tensor_api/utils/print_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file print_impl.h
 * \brief Tensor API debug print helpers implementation.
 */
#ifndef IMPL_TENSOR_API_UTILS_PRINT_IMPL_H
#define IMPL_TENSOR_API_UTILS_PRINT_IMPL_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/tensor/layout_impl.h"
#include "impl/tensor_api/tensor/local_tensor_impl.h"
#include "include/utils/debug/asc_printf.h"
#ifdef ASCENDC_CPU_DEBUG
#include <cstdio>
#endif

namespace AscendC {
namespace Te {
namespace detail {

template <typename T>
struct PrintElementType {
private:
    using RawType = Std::remove_cvref_t<T>;
    using AttrType = GetAttributeElementType<Std::add_pointer_t<RawType>>;

public:
    using type = Std::conditional_t<Std::is_same_v<AttrType, EmptyValue>, RawType, AttrType>;
};

template <typename T>
using PrintElementTypeT = typename PrintElementType<T>::type;

__aicore__ inline int NumDigits(long long x)
{
    unsigned long long value = x < 0 ? static_cast<unsigned long long>(-x) : static_cast<unsigned long long>(x);
    int digits = x < 0 ? 2 : 1;
    while (value >= 10) {
        value /= 10;
        ++digits;
    }
    return digits;
}

template <typename... Args>
__aicore__ inline void PrintFormat(__gm__ const char* fmt, Args&&... args)
{
#if defined(ASCENDC_DUMP) && (ASCENDC_DUMP != 0)
#ifdef ASCENDC_CPU_DEBUG
    std::printf(fmt, args...);
#else
    __asc_aicore::printf(fmt, args...);
#endif
#else
    (void)fmt;
    ((void)args, ...);
#endif
}

__aicore__ inline void EmitSpaces(int count)
{
    for (int i = 0; i < count; ++i) {
        PrintFormat((__gm__ const char*)" ");
    }
}

__aicore__ inline void EmitPaddedInt(long long value, int width)
{
    EmitSpaces(width - NumDigits(value));
    PrintFormat((__gm__ const char*)"%lld", value);
}

template <typename T>
__aicore__ inline void EmitTupleElement(const T& value);

template <typename Tuple, size_t I, size_t N>
struct TuplePrinter {
    __aicore__ inline static void Print(const Tuple& tuple)
    {
        EmitTupleElement(Std::get<I>(tuple));
        if constexpr (I + 1 < N) {
            PrintFormat((__gm__ const char*)", ");
            TuplePrinter<Tuple, I + 1, N>::Print(tuple);
        }
    }
};

template <typename Tuple, size_t N>
struct TuplePrinter<Tuple, N, N> {
    __aicore__ inline static void Print(const Tuple&) {}
};

template <typename T>
__aicore__ inline void EmitTupleElement(const T& value)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<T>>) {
        PrintFormat((__gm__ const char*)"(");
        TuplePrinter<Std::remove_cvref_t<T>, 0, Std::tuple_size_v<Std::remove_cvref_t<T>>>::Print(value);
        PrintFormat((__gm__ const char*)")");
    } else if constexpr (IsPrintStringV<T>) {
        PrintFormat((__gm__ const char*)"%s", value);
    } else if constexpr (Std::is_floating_point<Std::remove_cvref_t<T>>::value) {
        PrintFormat((__gm__ const char*)"%f", static_cast<double>(value));
    } else if constexpr (Std::is_integral<Std::remove_cvref_t<T>>::value) {
        PrintFormat((__gm__ const char*)"%lld", static_cast<long long>(value));
    } else {
        PrintFormat((__gm__ const char*)"%lld", static_cast<long long>(value));
    }
}

template <typename T>
__aicore__ inline void EmitValue(const T& value)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<T>>) {
        EmitTupleElement(value);
    } else if constexpr (IsLayoutV<T>) {
        PrintFormat((__gm__ const char*)"Layout(shape=");
        EmitTupleElement(value.Shape());
        PrintFormat((__gm__ const char*)", stride=");
        EmitTupleElement(value.Stride());
        PrintFormat((__gm__ const char*)")");
    } else if constexpr (IsAttrTensorV<T>) {
        PrintFormat((__gm__ const char*)"Tensor<");
        EmitValue(value.Layout());
        PrintFormat((__gm__ const char*)">");
    } else {
        EmitTupleElement(value);
    }
}

template <typename ValueType, typename T>
__aicore__ inline void EmitElementAs(const T& value)
{
    if constexpr (Std::is_same_v<ValueType, float>) {
        PrintFormat((__gm__ const char*)"%f  ", static_cast<float>(value));
    } else if constexpr (Std::is_same_v<ValueType, half>) {
        PrintFormat((__gm__ const char*)"%f  ", static_cast<half>(value));
    } else if constexpr (Std::is_same_v<ValueType, bfloat16_t>) {
        PrintFormat((__gm__ const char*)"%f  ", static_cast<bfloat16_t>(value));
    } else if constexpr (Std::is_same_v<ValueType, int8_t> || Std::is_same_v<ValueType, int16_t> ||
        Std::is_same_v<ValueType, int32_t>) {
        PrintFormat((__gm__ const char*)"%d  ", static_cast<int32_t>(value));
    } else if constexpr (Std::is_same_v<ValueType, int64_t>) {
        PrintFormat((__gm__ const char*)"%lld  ", static_cast<long long>(value));
    } else if constexpr (Std::is_same_v<ValueType, uint8_t> || Std::is_same_v<ValueType, uint16_t> ||
        Std::is_same_v<ValueType, uint32_t>) {
        PrintFormat((__gm__ const char*)"%u  ", static_cast<uint32_t>(value));
    } else if constexpr (Std::is_same_v<ValueType, uint64_t>) {
        PrintFormat((__gm__ const char*)"%llu  ", static_cast<unsigned long long>(value));
    } else {
        PrintFormat((__gm__ const char*)"  ");
        EmitValue(value);
    }
}

template <typename T>
__aicore__ inline void EmitElement(const T& value)
{
    EmitElementAs<PrintElementTypeT<T>>(value);
}

template <typename T>
__aicore__ inline void EmitLayoutTable(const T& layout)
{
    static_assert(IsLayoutV<T>, "Print expects a Tensor API Layout.");
    static_assert(T::rank == 2, "Print only supports rank-2 layout tables.");

    int idxWidth = NumDigits(static_cast<long long>(Cosize(layout))) + 2;
    EmitValue(layout);
    PrintFormat((__gm__ const char*)"\n");

    PrintFormat((__gm__ const char*)"    ");
    for (int n = 0; n < static_cast<int>(Size<1>(layout)); ++n) {
        PrintFormat((__gm__ const char*)"  ");
        EmitPaddedInt(n, idxWidth - 2);
        PrintFormat((__gm__ const char*)" ");
    }
    PrintFormat((__gm__ const char*)"\n");

    for (int m = 0; m < static_cast<int>(Size<0>(layout)); ++m) {
        PrintFormat((__gm__ const char*)"    ");
        for (int n = 0; n < static_cast<int>(Size<1>(layout)); ++n) {
            PrintFormat((__gm__ const char*)"+");
            for (int i = 0; i < idxWidth; ++i) {
                PrintFormat((__gm__ const char*)"-");
            }
        }
        PrintFormat((__gm__ const char*)"+\n");

        EmitPaddedInt(m, 2);
        PrintFormat((__gm__ const char*)"  ");
        for (int n = 0; n < static_cast<int>(Size<1>(layout)); ++n) {
            PrintFormat((__gm__ const char*)"| ");
            EmitPaddedInt(static_cast<long long>(layout(MakeCoord(m, n))), idxWidth - 2);
            PrintFormat((__gm__ const char*)" ");
        }
        PrintFormat((__gm__ const char*)"|\n");
    }

    PrintFormat((__gm__ const char*)"    ");
    for (int n = 0; n < static_cast<int>(Size<1>(layout)); ++n) {
        PrintFormat((__gm__ const char*)"+");
        for (int i = 0; i < idxWidth; ++i) {
            PrintFormat((__gm__ const char*)"-");
        }
    }
    PrintFormat((__gm__ const char*)"+\n");
}

template <typename T>
__aicore__ inline void EmitTensorContent(const T& tensor)
{
    static_assert(IsAttrTensorV<T>, "Print expects a Tensor API LocalTensor created by MakeTensor.");
    using LayoutType = typename Std::remove_cvref_t<T>::layoutType;
    using ElementType = typename Std::remove_cvref_t<T>::elementType;
    static_assert(LayoutType::rank >= 1 && LayoutType::rank <= 4, "Print only supports rank 1 to 4 tensors.");

    EmitValue(tensor);
    PrintFormat((__gm__ const char*)":\n");

    if constexpr (LayoutType::rank == 1) {
        for (int m = 0; m < static_cast<int>(Size<0>(tensor.Layout())); ++m) {
            EmitElementAs<ElementType>(tensor[MakeCoord(m)]);
            PrintFormat((__gm__ const char*)"\n");
        }
    } else if constexpr (LayoutType::rank == 2) {
        for (int m = 0; m < static_cast<int>(Size<0>(tensor.Layout())); ++m) {
            for (int n = 0; n < static_cast<int>(Size<1>(tensor.Layout())); ++n) {
                EmitElementAs<ElementType>(tensor[MakeCoord(m, n)]);
            }
            PrintFormat((__gm__ const char*)"\n");
        }
    } else if constexpr (LayoutType::rank == 3) {
        for (int k = 0; k < static_cast<int>(Size<2>(tensor.Layout())); ++k) {
            if (k != 0) {
                for (int i = 0; i < 5 * static_cast<int>(Size<1>(tensor.Layout())); ++i) {
                    PrintFormat((__gm__ const char*)"-");
                }
                PrintFormat((__gm__ const char*)"\n");
            }
            for (int m = 0; m < static_cast<int>(Size<0>(tensor.Layout())); ++m) {
                for (int n = 0; n < static_cast<int>(Size<1>(tensor.Layout())); ++n) {
                    EmitElementAs<ElementType>(tensor[MakeCoord(m, n, k)]);
                }
                PrintFormat((__gm__ const char*)"\n");
            }
        }
    } else if constexpr (LayoutType::rank == 4) {
        for (int p = 0; p < static_cast<int>(Size<3>(tensor.Layout())); ++p) {
            if (p != 0) {
                for (int i = 0; i < 5 * static_cast<int>(Size<1>(tensor.Layout())); ++i) {
                    PrintFormat((__gm__ const char*)"=");
                }
                PrintFormat((__gm__ const char*)"\n");
            }
            for (int k = 0; k < static_cast<int>(Size<2>(tensor.Layout())); ++k) {
                if (k != 0) {
                    for (int i = 0; i < 5 * static_cast<int>(Size<1>(tensor.Layout())); ++i) {
                        PrintFormat((__gm__ const char*)"-");
                    }
                    PrintFormat((__gm__ const char*)"\n");
                }
                for (int m = 0; m < static_cast<int>(Size<0>(tensor.Layout())); ++m) {
                    for (int n = 0; n < static_cast<int>(Size<1>(tensor.Layout())); ++n) {
                        EmitElementAs<ElementType>(tensor[MakeCoord(m, n, k, p)]);
                    }
                    PrintFormat((__gm__ const char*)"\n");
                }
            }
        }
    }
}

} // namespace detail

#ifdef ASCENDC_CPU_DEBUG
__aicore__ inline void Print(const char* fmt)
{
    detail::PrintFormat(fmt);
}

template <typename Arg0, typename... Args>
__aicore__ inline void Print(const char* fmt, Arg0&& arg0, Args&&... args)
{
    detail::PrintFormat(fmt, arg0, args...);
}

#else
__aicore__ inline void Print(__gm__ const char* fmt)
{
    detail::PrintFormat(fmt);
}

template <typename Arg0, typename... Args>
__aicore__ inline void Print(__gm__ const char* fmt, Arg0&& arg0, Args&&... args)
{
    detail::PrintFormat(fmt, arg0, args...);
}
#endif // ASCENDC_CPU_DEBUG

template <typename T, typename Std::enable_if_t<!detail::IsPrintStringV<T>, int>>
__aicore__ inline void Print(const T& value)
{
    if constexpr (IsAttrTensorV<T>) {
        detail::EmitTensorContent(value);
    } else if constexpr (IsLayoutV<T>) {
        if constexpr (Std::remove_cvref_t<T>::rank == 2) {
            detail::EmitLayoutTable(value);
        } else {
            detail::EmitValue(value);
        }
    } else {
        detail::EmitValue(value);
    }
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_UTILS_PRINT_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
