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
    __aicore__ inline static constexpr bool CheckAllEquals() {
        return true;
    }

    template <typename T0, typename T1, typename... Ts>
    __aicore__ inline static constexpr bool CheckAllEquals(T0&& t0, T1&& t1, Ts&&... ts) {
        if (!Std::is_constant<t0, T1>::value) {
            return false;
        }
        return CheckAllEquals(ts...);
    }

    static constexpr bool IsScaleType() {
        return Std::is_one_of_v<type, __cbuf__ fp8_e8m0_t, __ca__ fp8_e8m0_t, __cb__ fp8_e8m0_t>;
    }
};

template <typename T>
struct IsZZFormat : FormatChecker<T> {
private:
    __aicore__ inline static constexpr bool IsFractalZZFormatNormal() {
        using Base = FormatChecker<T>;
        return Base::template CheckAllEquals(Std::Int<FRACTAL_FIXED>{}, typename Base::ShapeRow0{},
                                             Std::Int<Base::c0Size>{}, typename Base::ShapeColumn0{},
                                             Std::Int<Base::c0Size>{}, typename Base::StrideRow0{},
                                             Std::Int<1>{}, typename Base::StrideColumn0{});
    }

    __aicore__ inline static constexpr bool IsFractalScaleZZFormat() {
        using Base = FormatChecker<T>;
        return Base::template CheckAllEquals(Std::Int<FRACTAL_FIXED>{}, typename Base::ShapeRow0{},
                                             Std::Int<MX_SCALE_K0>{}, typename Base::ShapeColumn0{},
                                             Std::Int<MX_SCALE_K0>{}, typename Base::StrideRow0{},
                                             Std::Int<1>{}, typename Base::StrideColumn0{});
    }

    __aicore__ inline static constexpr bool IsFractalZZFormat() {
        if constexpr (FormatChecker<T>::IsScaleType()) {
            return IsFractalScaleZZFormat();
        } else {
            return IsFractalZZFormatNormal();
        }
        // using ResultType = Std::conditional_t<,
        //                    Std::bool_constant<>,
        //                    Std::bool_constant<>>;
        // return ResultType::value;
    }
public:
    static constexpr bool value = IsFractalZZFormat();
};

template <typename T>
struct IsNNFormat : FormatChecker<T> {
private:
    __aicore__ inline static constexpr bool IsFractalNNFormat() {
        using Base = FormatChecker<T>;
        return Base::template CheckAllEquals(Std::Int<MX_SCALE_K0>{}, typename Base::ShapeRow0{},
                                             Std::Int<FRACTAL_FIXED>{}, typename Base::ShapeColumn0{},
                                             Std::Int<1>{}, typename Base::StrideRow0{},
                                             Std::Int<MX_SCALE_K0>{}, typename Base::StrideColumn0{});
    }
public:
    static constexpr bool value = IsFractalNNFormat();
};

template <typename T>
struct IsZNFormat : FormatChecker<T> {
private:
    __aicore__ inline static constexpr bool IsFractalZNFormat() {
        using Base = FormatChecker<T>;
        return Base::template CheckAllEquals(Std::Int<Base::c0Size>{}, typename Base::ShapeRow0{},
                                             Std::Int<FRACTAL_FIXED>{}, typename Base::ShapeColumn0{},
                                             Std::Int<1>{}, typename Base::StrideRow0{},
                                             Std::Int<Base::c0Size>{}, typename Base::StrideColumn0{});
    }
public:
    static constexpr bool value = IsFractalZNFormat();
};

template <typename T>
struct IsNZFormat : FormatChecker<T> {
private:
    __aicore__ inline static constexpr bool IsFractalNZFormat() {
        using Base = FormatChecker<T>;
        return Base::template CheckAllEquals(Std::Int<FRACTAL_FIXED>{}, typename Base::ShapeRow0{},
                                             Std::Int<Base::c0Size>{}, typename Base::ShapeColumn0{},
                                             Std::Int<Base::c0Size>{}, typename Base::StrideRow0{},
                                             Std::Int<1>{}, typename Base::StrideColumn0{});
    }
public:
    static constexpr bool value = IsFractalNZFormat();
};

template <typename T>
struct IsL0cNZFormat : FormatChecker<T> {
private:
    __aicore__ inline static constexpr bool IsFractalL0cNZFormat() {
        using Base = FormatChecker<T>;
        return Base::template CheckAllEquals(Std::Int<FRACTAL_FIXED>{}, typename Base::ShapeRow0{},
                                             Std::Int<FRACTAL_FIXED>{}, typename Base::ShapeColumn0{},
                                             Std::Int<FRACTAL_FIXED>{}, typename Base::StrideRow0{},
                                             Std::Int<1>{}, typename Base::StrideColumn0{});
    }
public:
    static constexpr bool value = IsFractalL0cNZFormat();
};

template <typename T>
struct IsNDFormat : FormatChecker<T>{
private:
    __aicore__ inline static constexpr bool IsFractalNDFormatNormal() {
        using Base = FormatChecker<T>;
        return Base::template CheckAllEquals(Std::Int<1>{}, typename Base::ShapeRow0{},
                                             Std::Int<1>{}, typename Base::ShapeColumn0{},
                                             Std::Int<0>{}, typename Base::StrideRow0{},
                                             Std::Int<0>{}, typename Base::StrideColumn0{},
                                             Std::Int<1>{}, typename Base::StrideColumn1{});
    }

    __aicore__ inline static constexpr bool IsFractalScaleNDFormat() {
        using Base = FormatChecker<T>;
        return Base::template CheckAllEquals(Std::Int<2>{}, typename Base::ShapeRow0{},
                                             Std::Int<1>{}, typename Base::ShapeColumn0{},
                                             Std::Int<1>{}, typename Base::StrideRow0{},
                                             Std::Int<0>{}, typename Base::StrideColumn0{},
                                             Std::Int<2>{}, typename Base::StrideColumn1{});
    }

    __aicore__ inline static constexpr bool IsFractalNDFormat() {
        if constexpr (FormatChecker<T>::IsScaleType()) {
            return IsFractalScaleNDFormat();
        } else {
            return IsFractalNDFormatNormal();
        }
        // using ResultType = Std::conditional_t<,
        //                    Std::bool_constant<>,
        //                    Std::bool_constant<>>;
        // return ResultType::value;
    }
public:
    static constexpr bool value = IsFractalNDFormat();
};

template <typename T>
struct IsDNFormat : FormatChecker<T> {
private:
    __aicore__ inline static constexpr bool IsFractalDNFormatNormal() {
        using Base = FormatChecker<T>;
        return Base::template CheckAllEquals(Std::Int<1>{}, typename Base::ShapeRow0{},
                                             Std::Int<1>{}, typename Base::ShapeColumn0{},
                                             Std::Int<0>{}, typename Base::StrideRow0{},
                                             Std::Int<1>{}, typename Base::StrideRow1{},
                                             Std::Int<0>{}, typename Base::StrideColumn0{});
    }

    __aicore__ inline static constexpr bool IsFractalScaleDNFormat() {
        using Base = FormatChecker<T>;
        return Base::template CheckAllEquals<>(Std::Int<2>{}, typename Base::ShapeColumn0{},
                                               Std::Int<2>{}, typename Base::StrideRow0{},
                                               Std::Int<1>{}, typename Base::StrideColumn0{});
    }

    __aicore__ inline static constexpr bool IsFractalDNFormat() {
        if constexpr (FormatChecker<T>::IsScaleType()) {
            return IsFractalScaleDNFormat();
        } else {
            return IsFractalDNFormatNormal();
        }
        // using ResultType = Std::conditional_t<,
        //                    Std::bool_constant<>,
        //                    Std::bool_constant<>>;
        // return ResultType::value;
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