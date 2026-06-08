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
    "impl/tensor_api/utils/constant_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file constant_impl.h
* \brief
*/
#ifndef IMPL_TENSOR_API_UTILS_CONSTANT_IMPL_H
#define IMPL_TENSOR_API_UTILS_CONSTANT_IMPL_H

#include "impl/tensor_api/utils/extra_impl.h"
#include "impl/tensor_api/utils/map_impl.h"

namespace AscendC {
namespace Te {
using Std::Int;
#define ASCENDC_TE_DEFINE_INT_ALIAS(VALUE) using _##VALUE = Std::Int<VALUE>;

ASCENDC_TE_DEFINE_INT_ALIAS(0)
ASCENDC_TE_DEFINE_INT_ALIAS(1)
ASCENDC_TE_DEFINE_INT_ALIAS(2)
ASCENDC_TE_DEFINE_INT_ALIAS(3)
ASCENDC_TE_DEFINE_INT_ALIAS(4)
ASCENDC_TE_DEFINE_INT_ALIAS(5)
ASCENDC_TE_DEFINE_INT_ALIAS(6)
ASCENDC_TE_DEFINE_INT_ALIAS(7)
ASCENDC_TE_DEFINE_INT_ALIAS(8)
ASCENDC_TE_DEFINE_INT_ALIAS(9)
ASCENDC_TE_DEFINE_INT_ALIAS(10)
ASCENDC_TE_DEFINE_INT_ALIAS(16)
ASCENDC_TE_DEFINE_INT_ALIAS(24)
ASCENDC_TE_DEFINE_INT_ALIAS(32)
ASCENDC_TE_DEFINE_INT_ALIAS(64)
ASCENDC_TE_DEFINE_INT_ALIAS(128)
ASCENDC_TE_DEFINE_INT_ALIAS(256)
ASCENDC_TE_DEFINE_INT_ALIAS(512)
ASCENDC_TE_DEFINE_INT_ALIAS(1024)
ASCENDC_TE_DEFINE_INT_ALIAS(2048)
ASCENDC_TE_DEFINE_INT_ALIAS(4096)

#undef ASCENDC_TE_DEFINE_INT_ALIAS

constexpr size_t TWO_DIM_DATA = 2;
constexpr size_t THREE_DIM_DATA = 3;
constexpr size_t FOUR_DIM_DATA = 4;
constexpr size_t FIVE_DIM_DATA = 5;
constexpr size_t FRACTAL_FIXED = 16;
constexpr size_t MX_SCALE_K0 = 2;
constexpr uint32_t BLOCK_CUBE = 16;
constexpr uint64_t HIFLOAT8_MMAD_CTRL_MASK = 0x200000000000ULL;

struct ArchVersion {
    static constexpr uint32_t V3510 = 3510;
    static constexpr uint32_t V2201 = 2201;
};

struct GetArchVersion {
    __aicore__ inline constexpr uint32_t operator()() const {
#ifdef __NPU_ARCH__
        return __NPU_ARCH__;
#else
        return 0;
#endif
    }
};

constexpr uint32_t CURRENT_ARCH_VERSION = GetArchVersion{}();

namespace CopyMode {
    struct NORMAL {};
    struct NORMAL_COORD {};
    struct TRANS {};
    struct TRANS_COORD {};
    struct TRANS_B8B4 {};
    struct TRANS_B8B4_COORD {};
};

namespace Location {
    struct INVALID {};
    struct GM {};
    struct UB {};
    struct L1 {};
    struct L0A {};
    struct L0B {};
    struct L0ScaleA {};
    struct L0ScaleB {};
    struct L0C {};
    struct BIAS {};
    struct FIXBUF {};
    struct SSBUF {};
}

template <typename T>
struct IsHardware {
private:
    template <typename Tp, typename... Tps>
    __aicore__ inline static constexpr bool IsUnqualifiedAnyOf() {
        return (... || Std::is_same_v<Std::remove_cvref_t<Tp>, Tps>);
    }

public:
    static constexpr bool value = IsUnqualifiedAnyOf<T,
        Location::INVALID, Location::GM, Location::UB, Location::L1,
        Location::L0A, Location::L0B, Location::L0ScaleA, Location::L0ScaleB,
        Location::L0C, Location::BIAS, Location::FIXBUF, Location::SSBUF>();
};

template <typename T>
constexpr bool IsHardwareV = IsHardware<T>::value;

template <typename TupleType>
using tuple_sequence = Std::make_index_sequence<Std::tuple_size_v<Std::remove_cvref_t<TupleType>>>;

template <typename T>
struct locationAttr {
    using gmAttr =          __gm__      T*;
    using cbufAttr =        __cbuf__    T*;
    using caAttr =          __ca__      T*;
    using cbAttr =          __cb__      T*;
    using ccAttr =          __cc__      T*;
    using ubufAttr =        __ubuf__    T*;
    using fbufAttr =        __fbuf__    T*;
    using ssbufAttr =       __ssbuf__   T*;
    using biasbufAttr =     __biasbuf__ T*;
    using noneAttr =                    T*;

    using type = Std::tuple<gmAttr, cbufAttr, caAttr, cbAttr, ccAttr, ubufAttr, fbufAttr, ssbufAttr, biasbufAttr>;

    using locationMap = TupleMap<Std::tuple<Location::GM, gmAttr>, Std::tuple<Location::L1, cbufAttr>, Std::tuple<Location::L0A, caAttr>,
    Std::tuple<Location::L0B, cbAttr>, Std::tuple<Location::L0ScaleA, noneAttr>,
    Std::tuple<Location::L0ScaleB, noneAttr>, Std::tuple<Location::L0C, ccAttr>, Std::tuple<Location::UB, ubufAttr>, 
    Std::tuple<Location::BIAS, biasbufAttr>, Std::tuple<Location::FIXBUF, fbufAttr>>;
};

template <typename A, typename ProcessedTuple>
struct AllElementsSameAsA;

template <typename A, typename First, typename... Rest>
struct AllElementsSameAsA<A, Std::tuple<First, Rest...>> {
    static constexpr bool value = Std::is_same_v<A, typename IterEle<First>::type> || AllElementsSameAsA<A, Std::tuple<Rest...>>::value;
};

template <typename A>
struct AllElementsSameAsA<A, Std::tuple<>> { static constexpr bool value = false; };

template <typename A, typename... BList>
struct CheckAllSame;

template <typename A, typename B, typename... RestB>
struct CheckAllSame<A, B, RestB...> {
    static constexpr bool value =  Std::is_same_v<A, B> || AllElementsSameAsA<A, typename locationAttr<B>::type>::value || CheckAllSame<A, RestB...>::value;
};

template <typename A>
struct CheckAllSame<A> { static constexpr bool value = false; };

template <typename A, typename... BList>
constexpr bool IsOneOfAttrV = CheckAllSame<A, BList...>::value;

template <typename DataType>
inline constexpr bool IsDataType = IsOneOfAttrV<Std::remove_cvref_t<DataType>, hifloat8_t, bfloat16_t, fp4x2_e1m2_t, fp4x2_e2m1_t, 
                                                fp8_e5m2_t, fp8_e4m3fn_t, fp8_e8m0_t> || Std::is_integral_v<Std::remove_cvref_t<DataType>> 
                                                || Std::is_floating_point_v<Std::remove_cvref_t<DataType>>;

template <typename DataType>
inline constexpr bool IsB4Type = IsOneOfAttrV<DataType, fp4x2_e1m2_t, fp4x2_e2m1_t>;

template<typename T = Std::ignore_t>
__aicore__ inline constexpr size_t GetC0Size() {
    constexpr size_t c0Size = 32;
    if constexpr (IsB4Type<T>) {
        return c0Size * 2;
    } else {
        return c0Size;
    }
}

template<typename T = Std::ignore_t>
constexpr size_t C0_SIZE = GetC0Size<T>();

template<typename T>
constexpr size_t C0_ELEMENT = C0_SIZE<T> / sizeof(T);

// IsIntegralConstant
template <typename T>
struct IsIntegralConstant : Std::false_type {};

template <size_t Value>
struct IsIntegralConstant<Std::Int<Value>> : Std::true_type {};

template <typename T>
constexpr bool IsIntegralConstantV = IsIntegralConstant<T>::value;

#if defined(__NPU_ARCH__)
using VectorTypeTransform = TupleMap<
    Std::tuple<uint8_t,        vector_uint8_t>,
    Std::tuple<uint16_t,       vector_uint16_t>,
    Std::tuple<uint32_t,       vector_uint32_t>,
    Std::tuple<uint64_t,       vector_uint64_t>,
    Std::tuple<int8_t,         vector_int8_t>,
    Std::tuple<int16_t,        vector_int16_t>,
    Std::tuple<int32_t,        vector_int32_t>,
    Std::tuple<int64_t,        vector_int64_t>,
    Std::tuple<bfloat16_t,     vector_bfloat16_t>,
    Std::tuple<half,           vector_half>,
    Std::tuple<float,          vector_float>,
#if  __NPU_ARCH__ == 3510
    Std::tuple<hifloat8_t,     vector_hifloat8_t>,
    Std::tuple<fp8_e4m3fn_t,   vector_fp8_e4m3fn_t>,
    Std::tuple<fp8_e5m2_t,     vector_fp8_e5m2_t>,
    Std::tuple<fp8_e8m0_t,     vector_fp8_e8m0_t>,
    Std::tuple<int4x2_t,       vector_int4x2_t>,
    Std::tuple<fp4x2_e2m1_t,   vector_fp4x2_e2m1_t>,
    Std::tuple<fp4x2_e1m2_t,   vector_fp4x2_e1m2_t>
#endif
    >;
#endif

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_UTILS_CONSTANT_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
