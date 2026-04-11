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
    "impl/tensor_api/arch/datamove/l0c_to_out/common/utils.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file utils.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_DATAMOVE_L0C_TO_OUT_COMMON_UTILS_H
#define IMPL_TENSOR_API_ARCH_DATAMOVE_L0C_TO_OUT_COMMON_UTILS_H

#include "impl/experimental/tensor_api/utils/utils_impl.h"

namespace AscendC {
namespace Te {

constexpr uint32_t MAIN_LOOP_N_SIZE_3510 = 512;
constexpr uint32_t CBURST_NUM_3510 = MAIN_LOOP_N_SIZE_3510 / BLOCK_CUBE;

constexpr FixpipeTrait DEFAULT_FIXPIPE_TRAIT = FixpipeTrait{};

struct FixpipeTraitDefault {
    using TraitType = FixpipeTrait;
    static constexpr const TraitType value = DEFAULT_FIXPIPE_TRAIT;
};

template <const FixpipeTrait& trait, typename dstType, typename srcType>
__aicore__ inline constexpr QuantMode_t GetFixpipeVectorQuantPre()
{
    if constexpr (Std::is_same_v<srcType, __cc__ int32_t> && Std::is_one_of_v<dstType, __gm__ half, __ubuf__ half>) {
        return QuantMode_t::VDEQF16;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ uint8_t, __gm__ int8_t, __ubuf__ uint8_t,
                                             __ubuf__ int8_t>) {
        return QuantMode_t::VQF322B8_PRE;
    } else if constexpr (Std::is_same_v<srcType, __cc__ int32_t>
                         && Std::is_one_of_v<dstType, __gm__ uint8_t, __gm__ int8_t, __ubuf__ uint8_t,
                                             __ubuf__ int8_t>) {
        return QuantMode_t::VREQ8;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ fp8_e4m3fn_t, __ubuf__ fp8_e4m3fn_t>) {
        return QuantMode_t::VQF322FP8_PRE;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ hifloat8_t, __ubuf__ hifloat8_t>) {
        if constexpr (trait.roundMode == RoundMode::HYBRID) {
            return QuantMode_t::VQF322HIF8_PRE_HYBRID;
        } else {
            return QuantMode_t::VQF322HIF8_PRE;
        }
    } else if constexpr (Std::is_same_v<srcType, __cc__ int32_t>
                         && Std::is_one_of_v<dstType, __gm__ bfloat16_t, __ubuf__ bfloat16_t>) {
        return QuantMode_t::VQS322BF16_PRE;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ half, __ubuf__ half>) {
        return QuantMode_t::VQF322F16_PRE;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ bfloat16_t, __ubuf__ bfloat16_t>) {
        return QuantMode_t::VQF322BF16_PRE;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ float, __ubuf__ float>) {
        return QuantMode_t::VQF322F32_PRE;
    }
}

template <const FixpipeTrait& trait, typename dstType, typename srcType>
__aicore__ inline constexpr QuantMode_t GetFixpipeScalarQuantPre()
{
    if constexpr (Std::is_same_v<srcType, __cc__ int32_t> && Std::is_one_of_v<dstType, __gm__ half, __ubuf__ half>) {
        return QuantMode_t::DEQF16;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ uint8_t, __gm__ int8_t, __ubuf__ uint8_t,
                                             __ubuf__ int8_t>) {
        return QuantMode_t::QF322B8_PRE;
    } else if constexpr (Std::is_same_v<srcType, __cc__ int32_t>
                         && Std::is_one_of_v<dstType, __gm__ uint8_t, __gm__ int8_t, __ubuf__ uint8_t,
                                             __ubuf__ int8_t>) {
        return QuantMode_t::REQ8;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ fp8_e4m3fn_t, __ubuf__ fp8_e4m3fn_t>) {
        return QuantMode_t::QF322FP8_PRE;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ hifloat8_t, __ubuf__ hifloat8_t>) {
        if constexpr (trait.roundMode == RoundMode::HYBRID) {
            return QuantMode_t::QF322HIF8_PRE_HYBRID;
        } else {
            return QuantMode_t::QF322HIF8_PRE;
        }
    } else if constexpr (Std::is_same_v<srcType, __cc__ int32_t>
                         && Std::is_one_of_v<dstType, __gm__ bfloat16_t, __ubuf__ bfloat16_t>) {
        return QuantMode_t::QS322BF16_PRE;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ half, __ubuf__ half>) {
        return QuantMode_t::QF322F16_PRE;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ bfloat16_t, __ubuf__ bfloat16_t>) {
        return QuantMode_t::QF322BF16_PRE;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ float, __ubuf__ float>) {
        return QuantMode_t::QF322F32_PRE;
    }
}

template <const FixpipeTrait& trait, typename dstType, typename srcType>
__aicore__ inline constexpr QuantMode_t GetFixpipeCastQuantPre()
{
    if constexpr (Std::is_same_v<srcType, __cc__ float> && Std::is_one_of_v<dstType, __gm__ half, __ubuf__ half>) {
        return QuantMode_t::F322F16;
    } else if constexpr (Std::is_same_v<srcType, __cc__ float>
                         && Std::is_one_of_v<dstType, __gm__ bfloat16_t, __ubuf__ bfloat16_t>) {
        return QuantMode_t::F322BF16;
    } else {
        return QuantMode_t::NoQuant;
    }
}

template <const FixpipeTrait& trait, typename T, typename U, typename S = void>
__aicore__ inline constexpr QuantMode_t GetFixpipeQuantPre()
{
    using srcType = typename U::elementType;
    using dstType = typename T::elementType;
    constexpr bool isTensor = IsTileTensorV<S>;
    constexpr bool isScalar = Std::is_same_v<S, uint64_t>;

    if constexpr (trait.roundMode == RoundMode::HYBRID) {
        static_assert((Std::is_same_v<srcType, __cc__ float>
                       && Std::is_one_of_v<dstType, __gm__ hifloat8_t, __ubuf__ hifloat8_t>),
                      "Only when L0CType is float and output Type is hifloat8_t support RoundMode::HYBRID in Fixpipe");
    }
    if constexpr (isTensor) {
        return GetFixpipeVectorQuantPre<trait, dstType, srcType>();
    } else if constexpr (isScalar) {
        return GetFixpipeScalarQuantPre<trait, dstType, srcType>();
    } else {
        return GetFixpipeCastQuantPre<trait, dstType, srcType>();
    }
}

template <typename LayoutType>
__aicore__ inline static uint32_t GetColumnTotalSize(const LayoutType& layout)
{
    return GetEleFromLayout<LayoutType, AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(layout)
           * GetEleFromLayout<LayoutType, AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(layout);
}

template <typename LayoutType>
__aicore__ inline static uint32_t GetRowTotalSize(const LayoutType& layout)
{
    return GetEleFromLayout<LayoutType, AttrInfo::SHAPE, AttrInfo::ROW, 0>(layout)
           * GetEleFromLayout<LayoutType, AttrInfo::SHAPE, AttrInfo::ROW, 1>(layout);
}

template <typename LayoutType>
__aicore__ inline static uint32_t GetColumnStride(const LayoutType& layout, uint8_t index = 1)
{ return GetEleFromLayout<LayoutType, AttrInfo::STRIDE, AttrInfo::COLUMN, index>(layout); }

template <typename LayoutType>
__aicore__ inline static uint32_t GetRowStride(const LayoutType& layout, uint8_t index = 1)
{ return GetEleFromLayout<LayoutType, AttrInfo::STRIDE, AttrInfo::ROW, index>(layout); }

template <typename T, typename U>
static constexpr bool VerifyingFixpipeTemplate = (IsTileTensorV<T> && IsTileTensorV<U>);

template <typename T, typename U, typename S>
static constexpr bool VerifyingFixpipeQuantTemplate =
    (IsTileTensorV<T> && IsTileTensorV<U> && (IsTileTensorV<S> || Std::is_same_v<S, uint64_t>));

template <typename T, typename U, typename Coord>
constexpr bool VerifyingFixpipeTemplateWithCoord = Std::is_tuple_v<Coord> && VerifyingFixpipeTemplate<T, U>;

template <typename T, typename U, typename S, typename Coord>
constexpr bool VerifyingFixpipeQuantTemplateWithCoord =
    Std::is_tuple_v<Coord> && VerifyingFixpipeQuantTemplate<T, U, S>;

class FixpipeIgnore {
public:
    template <const FixpipeTrait& trait, typename... Args>
    __aicore__ inline static void Run(const Args&... args)
    {}
};

template <Hardware dstPos, Hardware srcpos, Hardware quantpos, uint32_t Version>
struct FixpipeTensor2Tensor {
    using type = FixpipeIgnore;
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_DATAMOVE_L0C_TO_OUT_COMMON_UTILS_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif