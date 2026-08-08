/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <type_traits>

#include <gtest/gtest.h>

#include "c_api/stub/cce_stub.h"
#include "tensor_api/tensor.h"

namespace AscendC {
namespace Te {

template <typename T, typename = void>
struct has_mask_member : std::false_type {};

template <typename T>
struct has_mask_member<T, std::void_t<decltype(&T::mask)>> : std::true_type {};

static_assert(std::is_same_v<typename asc::te::type_get<uint8_t>::T, vector_uint8_t>);
static_assert(std::is_same_v<typename asc::te::type_get<uint16_t>::T, vector_uint16_t>);
static_assert(std::is_same_v<typename asc::te::type_get<uint32_t>::T, vector_uint32_t>);
static_assert(std::is_same_v<typename asc::te::type_get<uint64_t>::T, vector_uint64_t>);
static_assert(std::is_same_v<typename asc::te::type_get<int8_t>::T, vector_int8_t>);
static_assert(std::is_same_v<typename asc::te::type_get<int16_t>::T, vector_int16_t>);
static_assert(std::is_same_v<typename asc::te::type_get<int32_t>::T, vector_int32_t>);
static_assert(std::is_same_v<typename asc::te::type_get<int64_t>::T, vector_int64_t>);
static_assert(std::is_same_v<typename asc::te::type_get<half>::T, vector_half>);
static_assert(std::is_same_v<typename asc::te::type_get<bfloat16_t>::T, vector_bfloat16_t>);
static_assert(std::is_same_v<typename asc::te::type_get<float>::T, vector_float>);
static_assert(std::is_same_v<typename asc::te::type_get<hifloat8_t>::T, vector_hifloat8_t>);
static_assert(std::is_same_v<typename asc::te::type_get<fp8_e4m3fn_t>::T, vector_fp8_e4m3fn_t>);
static_assert(std::is_same_v<typename asc::te::type_get<fp8_e5m2_t>::T, vector_fp8_e5m2_t>);
static_assert(std::is_same_v<typename asc::te::type_get<fp8_e8m0_t>::T, vector_fp8_e8m0_t>);
static_assert(std::is_same_v<typename asc::te::type_get<int4x2_t>::T, vector_int4x2_t>);
static_assert(std::is_same_v<typename asc::te::type_get<fp4x2_e2m1_t>::T, vector_fp4x2_e2m1_t>);
static_assert(std::is_same_v<typename asc::te::type_get<fp4x2_e1m2_t>::T, vector_fp4x2_e1m2_t>);
static_assert(std::is_same_v<typename asc::te::type_get<bool>::T, vector_bool>);
static_assert(std::is_same_v<typename asc::te::reg_tensor<float>::type, float>);
static_assert(std::is_same_v<typename asc::te::reg_tensor<float>::reg_type, vector_float>);
static_assert(std::is_same_v<typename asc::te::reg_tensor<bool>::reg_type, vector_bool>);
static_assert(has_mask_member<asc::te::reg_tensor<float>>::value);
static_assert(!has_mask_member<asc::te::reg_tensor<bool>>::value);
static_assert(std::is_same_v<decltype(asc::te::ones<float>()), asc::te::reg_tensor<bool>>);
static_assert(std::is_same_v<decltype(asc::te::zeros<float>()), asc::te::reg_tensor<bool>>);
static_assert(std::is_same_v<
    decltype(asc::te::make_mask<asc::te::mask_pattern::vl8, float>()), asc::te::reg_tensor<bool>>);
static_assert(std::is_same_v<
    decltype(asc::te::interleave<float>({}, {})), asc::te::reg_pair<bool>>);
static_assert(std::is_same_v<
    decltype(asc::te::deinterleave<float>({}, {})), asc::te::reg_pair<bool>>);
static_assert(std::is_same_v<
    decltype(asc::te::select({}, {}, {})), asc::te::reg_tensor<bool>>);
static_assert(std::is_same_v<
    decltype(asc::te::pack({})), asc::te::reg_tensor<bool>>);
static_assert(std::is_same_v<
    decltype(asc::te::unpack({})), asc::te::reg_tensor<bool>>);

template <typename T>
__aicore__ inline void CompileMaskApis(uint32_t& remain)
{
    auto all = asc::te::make_mask<asc::te::mask_pattern::all, T>();
    auto vl1 = asc::te::make_mask<asc::te::mask_pattern::vl1, T>();
    auto vl2 = asc::te::make_mask<asc::te::mask_pattern::vl2, T>();
    auto vl3 = asc::te::make_mask<asc::te::mask_pattern::vl3, T>();
    auto vl4 = asc::te::make_mask<asc::te::mask_pattern::vl4, T>();
    auto vl8 = asc::te::make_mask<asc::te::mask_pattern::vl8, T>();
    auto vl16 = asc::te::make_mask<asc::te::mask_pattern::vl16, T>();
    auto vl32 = asc::te::make_mask<asc::te::mask_pattern::vl32, T>();
    auto vl64 = asc::te::make_mask<asc::te::mask_pattern::vl64, T>();
    auto vl128 = asc::te::make_mask<asc::te::mask_pattern::vl128, T>();
    auto every3 = asc::te::make_mask<asc::te::mask_pattern::every3, T>();
    auto every4 = asc::te::make_mask<asc::te::mask_pattern::every4, T>();
    auto half = asc::te::make_mask<asc::te::mask_pattern::half, T>();
    auto quarter = asc::te::make_mask<asc::te::mask_pattern::quarter, T>();
    auto none = asc::te::make_mask<asc::te::mask_pattern::none, T>();
    auto updated = asc::te::update_mask<T>(remain);
    auto interleaved = asc::te::interleave<T>(all, none);
    auto deinterleaved = asc::te::deinterleave<T>(interleaved.first, interleaved.second);
    auto selected = asc::te::select(updated, deinterleaved.first, deinterleaved.second);
    auto packedLow = asc::te::pack(selected);
    auto packedHigh = asc::te::pack<asc::te::mask_half::high>(selected);
    auto unpackedLow = asc::te::unpack(packedLow);
    auto unpackedHigh = asc::te::unpack<asc::te::mask_half::high>(packedHigh);

    (void)vl1;
    (void)vl2;
    (void)vl3;
    (void)vl4;
    (void)vl8;
    (void)vl16;
    (void)vl32;
    (void)vl64;
    (void)vl128;
    (void)every3;
    (void)every4;
    (void)half;
    (void)quarter;
    (void)unpackedLow;
    (void)unpackedHigh;
}

template <typename T, asc::te::load_sideband_mode sidebandMode = asc::te::load_sideband_mode::direct>
__aicore__ inline void CompileTensorRegLoadStore(
    __ubuf__ T* dst, __ubuf__ T* src, vector_bool mask)
{
    auto layout = MakeLayout(MakeShape(64));
    auto dstTensor = MakeTensor(MakeMemPtr<Location::UB>(dst), layout);
    auto srcTensor = MakeTensor(MakeMemPtr<Location::UB>(src), layout);
    asc::te::reg_tensor<bool> maskReg {mask};
    auto value = srcTensor.template load<sidebandMode>(MakeCoord(0)).with_mask(maskReg);
    static_assert(std::is_same_v<typename decltype(value)::type, T>);
    dstTensor.store(MakeCoord(0), value);

    auto fullValue = srcTensor.load(MakeCoord(0)).with_mask(asc::te::ones<T>());
    static_assert(std::is_same_v<typename decltype(fullValue)::type, T>);
}

TEST(TestTensorApiRegTensor, HoldsRegisterAndMask)
{
    asc::te::reg_tensor<float> value {};
    (void)value.reg;
    (void)value.mask;

    auto compileLoadStore = &CompileTensorRegLoadStore<float>;
    auto compileMask8 = &CompileMaskApis<uint8_t>;
    auto compileMask16 = &CompileMaskApis<half>;
    auto compileMask32 = &CompileMaskApis<float>;
    (void)compileLoadStore;
    (void)compileMask8;
    (void)compileMask16;
    (void)compileMask32;
}

} // namespace Te
} // namespace AscendC
