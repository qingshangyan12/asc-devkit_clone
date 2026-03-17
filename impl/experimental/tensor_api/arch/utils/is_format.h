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
 * \file is_format.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_UTILS_IS_FORMAT_H
#define IMPL_TENSOR_API_ARCH_UTILS_IS_FORMAT_H

#include "impl/experimental/tensor_api/utils/utils_impl.h"
#include "impl/experimental/tensor_api/tensor/pointer_impl.h"
#include "impl/experimental/tensor_api/tensor/local_tensor_impl.h"

namespace AscendC {
namespace Te {

template <typename T>
struct GetTypeFromFourDimTrait;

template <Hardware hPos, typename Pointer, typename Shape1, typename Shape2, typename Stride1, typename Stride2>
struct GetTypeFromFourDimTrait<LocalTensor<TensorAttribute<ViewEngine<HardwareMemPtr<hPos, Pointer>>, Layout<Shape<Shape1, Shape2>, Stride<Stride1, Stride2>>>>> {
    using ShapeRowsZeroDim = typename Std::tuple_element<0, Shape1>::type;
    using ShapeRowsOneDim = typename Std::tuple_element<1, Shape1>::type;
    using ShapeColumnsZeroDim = typename Std::tuple_element<0, Shape2>::type;
    using ShapeColumnsOneDim = typename Std::tuple_element<1, Shape2>::type;

    using StrideRowsZeroDim = typename Std::tuple_element<0, Stride1>::type;
    using StrideRowsOneDim = typename Std::tuple_element<1, Stride1>::type;
    using StrideColumnsZeroDim = typename Std::tuple_element<0, Stride2>::type;
    using StrideColumnsOneDim = typename Std::tuple_element<1, Stride2>::type;
};

enum class AttrInfo : uint8_t {SHAPE, STRIDE, ROW, COLUMN};

template <typename T, AttrInfo info1, AttrInfo info2, size_t dim> 
struct GetFourDimType;

template <typename T>
struct GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::ROW, 0> {
    using type = Std::remove_cvref_t<typename GetTypeFromFourDimTrait<T>::ShapeRowsZeroDim>;
};
template <typename T>
struct GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::ROW, 1> {
    using type = Std::remove_cvref_t<typename GetTypeFromFourDimTrait<T>::ShapeRowsOneDim>;
};
template <typename T>
struct GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::COLUMN, 0> {
    using type = Std::remove_cvref_t<typename GetTypeFromFourDimTrait<T>::ShapeColumnsZeroDim>;
};
template <typename T>
struct GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::COLUMN, 1> {
    using type = Std::remove_cvref_t<typename GetTypeFromFourDimTrait<T>::ShapeColumnsOneDim>;
};
template <typename T>
struct GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::ROW, 0> {
    using type = Std::remove_cvref_t<typename GetTypeFromFourDimTrait<T>::StrideRowsZeroDim>;
};
template <typename T>
struct GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::ROW, 1> {
    using type = Std::remove_cvref_t<typename GetTypeFromFourDimTrait<T>::StrideRowsOneDim>;
};
template <typename T>
struct GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 0> {
    using type = Std::remove_cvref_t<typename GetTypeFromFourDimTrait<T>::StrideColumnsZeroDim>;
};
template <typename T>
struct GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 1> {
    using type = Std::remove_cvref_t<typename GetTypeFromFourDimTrait<T>::StrideColumnsOneDim>;
};

template <typename ValType, typename CheckType>
struct Pair {
    using ValueType = ValType;
    using TargetType = CheckType;
};

template <typename T>
struct FormatChecker {
protected:
    using type = typename T::elementType;
    static constexpr auto c0Size = is_b4_type<type> ? C0_SIZE * 2 : C0_SIZE / sizeof(type);
    using ShapeRow0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::ROW, 0>::type;
    using ShapeColumn0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::COLUMN, 0>::type;
    using StrideRow0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::ROW, 0>::type;
    using StrideRow1 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::ROW, 1>::type;
    using StrideColumn0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 0>::type;
    using StrideColumn1 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 1>::type;

public:
    template <typename ValType, typename CheckType>
    __aicore__ inline static constexpr bool CheckSingle() {
        return ValType::value == CheckType{};
    }

    template <typename... Pairs, size_t... Is>
    __aicore__ inline static constexpr bool CheckAllPairsImpl(Std::index_sequence<Is...>) {
        return (CheckSingle<
                typename std::tuple_element_t<Is, std::tuple<Pairs...>>::ValueType,
                typename std::tuple_element_t<Is, std::tuple<Pairs...>>::TargetType
                >() && ...);
    }

    template <typename... Pairs>
    __aicore__ inline static constexpr bool CheckAllPairs() {
        using Indices = Std::make_index_sequence<sizeof...(Pairs)>;
        return CheckAllPairsImpl<Pairs...>(Indices{});
    }

    __aicore__ inline static constexpr bool IsScaleType() {
 	    return is_one_of_attr_v<type, fp8_e8m0_t>;
 	}

    using ShapeRow0Type = ShapeRow0;
    using ShapeColumn0Type = ShapeColumn0;
    using StrideRow0Type = StrideRow0;
    using StrideRow1Type = StrideRow1;
    using StrideColumn0Type = StrideColumn0;
    using StrideColumn1Type = StrideColumn1;
};

template <typename T>
__aicore__ inline static constexpr bool Check() {
    return true;
}

template <typename T, typename ValType, typename CheckType, typename... Rest>
__aicore__ inline static constexpr bool Check(ValType, CheckType, Rest... rest) {
    return FormatChecker<T>::template CheckAllPairs<Pair<ValType, CheckType>>()
           && Check<T>(rest...);
}

template <typename T> using ShapeRow0_t = typename FormatChecker<T>::ShapeRow0Type;
template <typename T> using ShapeColumn0_t = typename FormatChecker<T>::ShapeColumn0Type;
template <typename T> using StrideRow0_t = typename FormatChecker<T>::StrideRow0Type;
template <typename T> using StrideRow1_t = typename FormatChecker<T>::StrideRow1Type;
template <typename T> using StrideColumn0_t = typename FormatChecker<T>::StrideColumn0Type;
template <typename T> using StrideColumn1_t = typename FormatChecker<T>::StrideColumn1Type;

template <typename T>
struct IsZZFormat {
private:
    __aicore__ inline static constexpr bool IsFractalZZFormatNormal() {
        return Check<T>(
               Std::Int<FRACTAL_FIXED>{}, ShapeRow0_t<T>{},
               Std::Int<FormatChecker<T>::c0Size>{}, ShapeColumn0_t<T>{},
               Std::Int<FormatChecker<T>::c0Size>{}, StrideRow0_t<T>{},
               Std::Int<1>{}, StrideColumn0_t<T>{});
    }

    __aicore__ inline static constexpr bool IsFractalScaleZZFormat() {
        return Check<T>(
               Std::Int<FRACTAL_FIXED>{}, ShapeRow0_t<T>{},
               Std::Int<MX_SCALE_K0>{}, ShapeColumn0_t<T>{},
               Std::Int<MX_SCALE_K0>{}, StrideRow0_t<T>{},
               Std::Int<1>{}, StrideColumn0_t<T>{});
    }

    __aicore__ inline static constexpr bool IsFractalZZFormat() {
        using ResultType = Std::conditional_t<FormatChecker<T>::IsScaleType(),
                           Std::bool_constant<IsFractalScaleZZFormat()>,
                           Std::bool_constant<IsFractalZZFormatNormal()>>;
        return ResultType::value;
    }
public:
    static constexpr bool value = IsFractalZZFormat();
};

template <typename T>
struct IsNNFormat {
private:
    __aicore__ inline static constexpr bool IsFractalNNFormat() {
        return Check<T>(
               Std::Int<MX_SCALE_K0>{}, ShapeRow0_t<T>{},
               Std::Int<FRACTAL_FIXED>{}, ShapeColumn0_t<T>{},
               Std::Int<1>{}, StrideRow0_t<T>{},
               Std::Int<MX_SCALE_K0>{}, StrideColumn0_t<T>{});
    }
public:
    static constexpr bool value = IsFractalNNFormat();
};

template <typename T>
struct IsZNFormat {
private:
    __aicore__ inline static constexpr bool IsFractalZNFormat() {
        return Check<T>(
               Std::Int<FormatChecker<T>::c0Size>{}, ShapeRow0_t<T>{},
               Std::Int<FRACTAL_FIXED>{}, ShapeColumn0_t<T>{},
               Std::Int<1>{}, StrideRow0_t<T>{},
               Std::Int<FormatChecker<T>::c0Size>{}, StrideColumn0_t<T>{});
    }
public:
    static constexpr bool value = IsFractalZNFormat();
};

template <typename T>
struct IsNZFormat {
private:
    __aicore__ inline static constexpr bool IsFractalNZFormat() {
        return Check<T>(
               Std::Int<FRACTAL_FIXED>{}, ShapeRow0_t<T>{},
               Std::Int<FormatChecker<T>::c0Size>{}, ShapeColumn0_t<T>{},
               Std::Int<FormatChecker<T>::c0Size>{}, StrideRow0_t<T>{},
               Std::Int<1>{}, StrideColumn0_t<T>{});
    }
public:
    static constexpr bool value = IsFractalNZFormat();
};

template <typename T>
struct IsL0cNZFormat {
private:
    __aicore__ inline static constexpr bool IsFractalL0cNZFormat() {
        return Check<T>(
               Std::Int<FRACTAL_FIXED>{}, ShapeRow0_t<T>{},
               Std::Int<FRACTAL_FIXED>{}, ShapeColumn0_t<T>{},
               Std::Int<FRACTAL_FIXED>{}, StrideRow0_t<T>{},
               Std::Int<1>{}, StrideColumn0_t<T>{});
    }
public:
    static constexpr bool value = IsFractalL0cNZFormat();
};

template <typename T>
struct IsNDFormat {
private:
    __aicore__ inline static constexpr bool IsFractalNDFormatNormal() {
        return Check<T>(
               Std::Int<1>{}, ShapeRow0_t<T>{},
               Std::Int<1>{}, ShapeColumn0_t<T>{},
               Std::Int<0>{}, StrideRow0_t<T>{},
               Std::Int<0>{}, StrideColumn0_t<T>{},
               Std::Int<1>{}, StrideColumn1_t<T>{});
    }

    __aicore__ inline static constexpr bool IsFractalScaleNDFormat() {
        return Check<T>(
               Std::Int<2>{}, ShapeRow0_t<T>{},
               Std::Int<1>{}, ShapeColumn0_t<T>{},
               Std::Int<1>{}, StrideRow0_t<T>{},
               Std::Int<0>{}, StrideColumn0_t<T>{},
               Std::Int<2>{}, StrideColumn1_t<T>{});
    }

    __aicore__ inline static constexpr bool IsFractalNDFormat() {
        using ResultType = Std::conditional_t<FormatChecker<T>::IsScaleType(),
                           Std::bool_constant<IsFractalScaleNDFormat()>,
                           Std::bool_constant<IsFractalNDFormatNormal()>>;
        return ResultType::value;
    }
public:
    static constexpr bool value = IsFractalNDFormat();
};

template <typename T>
struct IsDNFormat {
private:
    __aicore__ inline static constexpr bool IsFractalDNFormatNormal() {
        return Check<T>(
               Std::Int<1>{}, ShapeRow0_t<T>{},
               Std::Int<1>{}, ShapeColumn0_t<T>{},
               Std::Int<0>{}, StrideRow0_t<T>{},
               Std::Int<1>{}, StrideRow1_t<T>{},
               Std::Int<0>{}, StrideColumn0_t<T>{});
    }

    __aicore__ inline static constexpr bool IsFractalScaleDNFormat() {
        return Check<T>(
               Std::Int<1>{}, ShapeRow0_t<T>{},
               Std::Int<2>{}, ShapeColumn0_t<T>{},
               Std::Int<0>{}, StrideRow0_t<T>{},
               Std::Int<2>{}, StrideRow1_t<T>{},
               Std::Int<1>{}, StrideColumn0_t<T>{});
    }

    __aicore__ inline static constexpr bool IsFractalDNFormat() {
        using ResultType = Std::conditional_t<FormatChecker<T>::IsScaleType(),
                           Std::bool_constant<IsFractalScaleDNFormat()>,
                           Std::bool_constant<IsFractalDNFormatNormal()>>;
        return ResultType::value;
    }
public:
    static constexpr bool value = IsFractalDNFormat();
};

template <typename T>
struct IsScaleANDFormat { // shape = ((1, row),(1,col)) stride = ((0, col),(0, 1))
    static constexpr bool value = IsNDFormat<T>::normalValue;
};

template <typename T>
struct IsScaleADNFormat { // shape = ((1, row),(2,col/2)) stride = ((0, 2),(1, row*2))
    static constexpr bool value = IsDNFormat<T>::value;
};

template <typename T>
struct IsScaleBNDFormat { // shape = ((2, row/2),(1,col)) stride = ((1, 2*col),(0, 2))
    static constexpr bool value = IsNDFormat<T>::value;
};

template <typename T>
struct IsScaleBDNFormat { // shape = ((1, row),(1,col)) stride = ((0, 1),(0, row))
    static constexpr bool value = IsDNFormat<T>::normalValue;
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_UTILS_IS_FORMAT_H