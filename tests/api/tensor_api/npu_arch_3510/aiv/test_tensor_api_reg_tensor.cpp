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
#include <utility>

#include <gtest/gtest.h>

#include "c_api/stub/cce_stub.h"
#include "tensor_api/arch/vector/experimental/fill.h"
#include "tensor_api/arch/vector/experimental/mask.h"
#include "tensor_api/tensor/local_tensor.h"
#include "tensor_api/tensor.h"
#include "tensor_api/utils/utils.h"

namespace AscendC {
namespace Te {

template <typename T, typename = void>
struct has_mask_member : std::false_type {};

template <typename T>
struct has_mask_member<T, std::void_t<decltype(&T::mask)>> : std::true_type {};

template <typename TensorType, typename Coord, typename = void>
struct has_reg_load : std::false_type {};

template <typename TensorType, typename Coord>
struct has_reg_load<
    TensorType, Coord,
    std::void_t<decltype(std::declval<const TensorType&>().template load<asc::te::load_sideband_mode::direct>(
        std::declval<const Coord&>()))>> : std::true_type {};

template <typename T, asc::te::load_sideband_mode sidebandMode = asc::te::load_sideband_mode::direct>
__simd_vf__ inline void CompileRegLocalTensorLoadStore(
    __ubuf__ T* dst, __ubuf__ T* src, vector_bool mask)
{
    auto layout = MakeLayout(MakeShape(64));
    auto dstMem = MakeMemPtr<Location::UB>(dst);
    auto srcMem = MakeMemPtr<Location::UB>(src);
    using engine_type = asc::te::view_engine<decltype(srcMem)>;
    using layout_type = decltype(layout);
    asc::te::local_tensor<engine_type, layout_type> dstTensor(dstMem, layout);
    asc::te::local_tensor<engine_type, layout_type> srcTensor(srcMem, layout);
    using coord_type = decltype(MakeCoord(0));
    using tensor_api_base = typename decltype(srcTensor)::tensor_api_base;
    static_assert(!has_reg_load<tensor_api_base, coord_type>::value);
    static_assert(has_reg_load<decltype(srcTensor), coord_type>::value);
    static_assert(std::is_base_of_v<tensor_api_base, decltype(srcTensor)>);

    asc::te::reg_tensor<bool> maskReg {mask};
    auto value = srcTensor.template load<sidebandMode>(MakeCoord(0)).with_mask(maskReg);
    static_assert(std::is_same_v<typename decltype(value)::type, T>);
    dstTensor.store(MakeCoord(0), value);

    auto fullValue = srcTensor.load(MakeCoord(0)).with_mask(asc::te::all_mask<T>());
    static_assert(std::is_same_v<typename decltype(fullValue)::type, T>);

    asc::te::reg_tensor<T> assigned {mask};
    auto& assignedRef = (assigned = static_cast<T>(1));
    static_assert(std::is_same_v<decltype(assignedRef), asc::te::reg_tensor<T>&>);
}

template <asc::te::load_sideband_mode sidebandMode>
__simd_vf__ inline void CompileRegLocalTensorLoadMode(__ubuf__ uint8_t* src)
{
    auto layout = MakeLayout(MakeShape(256));
    auto srcTensor = MakeTensor(MakeMemPtr<Location::UB>(src), layout);
    if constexpr (sidebandMode == asc::te::load_sideband_mode::deintlv) {
        asc::te::reg_tensor<uint8_t> dst0;
        asc::te::reg_tensor<uint8_t> dst1;
        static_assert(std::is_same_v<decltype(srcTensor.load(MakeCoord(0), dst0, dst1)), void>);
        srcTensor.template load<sidebandMode>(MakeCoord(0), dst0, dst1);
        auto mask = asc::te::all_mask<uint8_t>();
        dst0.with_mask(mask);
        dst1.with_mask(mask);
    } else {
        auto value = srcTensor.template load<sidebandMode>(MakeCoord(0));
        static_assert(std::is_same_v<decltype(value), asc::te::reg_tensor<uint8_t>>);
        value.with_mask(asc::te::all_mask<uint8_t>());
    }
}

template <asc::te::store_sideband_mode sidebandMode>
__simd_vf__ inline void CompileRegLocalTensorStoreMode(__ubuf__ uint32_t* dst, vector_bool mask)
{
    auto layout = MakeLayout(MakeShape(64));
    auto dstTensor = MakeTensor(MakeMemPtr<Location::UB>(dst), layout);
    asc::te::reg_tensor<uint32_t> src0 {mask};
    if constexpr (sidebandMode == asc::te::store_sideband_mode::intlv) {
        asc::te::reg_tensor<uint32_t> src1 {mask};
        static_assert(std::is_same_v<decltype(dstTensor.store(MakeCoord(0), src0, src1)), void>);
        dstTensor.template store<sidebandMode>(MakeCoord(0), src0, src1);
    } else {
        static_assert(std::is_same_v<decltype(dstTensor.template store<sidebandMode>(MakeCoord(0), src0)), void>);
        dstTensor.template store<sidebandMode>(MakeCoord(0), src0);
    }
}

template <typename T>
__simd_vf__ inline void CompileRegMaskAndFill(T value, uint32_t remain)
{
    auto emptyMask = asc::te::none_mask<T>();
    auto tailMask = asc::te::update_mask<T>(remain);
    auto scalarFill = asc::te::fill(value, tailMask);
    auto registerFill = asc::te::fill(scalarFill, tailMask);
    (void)emptyMask;
    (void)registerFill;
}

TEST(TestTensorApiRegTensor, HoldsRegisterAndMask)
{
    asc::te::reg_tensor<float> value {};
    (void)value.reg;
    (void)value.mask;

    auto compileLoadStore = &CompileRegLocalTensorLoadStore<float>;
    (void)compileLoadStore;

    auto compileDirect = &CompileRegLocalTensorLoadMode<asc::te::load_sideband_mode::direct>;
    auto compileDeintlv = &CompileRegLocalTensorLoadMode<asc::te::load_sideband_mode::deintlv>;
    auto compileDownsample = &CompileRegLocalTensorLoadMode<asc::te::load_sideband_mode::downsample>;
    auto compileUnpack = &CompileRegLocalTensorLoadMode<asc::te::load_sideband_mode::unpack>;
    auto compileUnpack4 = &CompileRegLocalTensorLoadMode<asc::te::load_sideband_mode::unpack4>;
    auto compileUpsample = &CompileRegLocalTensorLoadMode<asc::te::load_sideband_mode::upsample>;
    (void)compileDirect;
    (void)compileDeintlv;
    (void)compileDownsample;
    (void)compileUnpack;
    (void)compileUnpack4;
    (void)compileUpsample;

    auto compileStoreDirect = &CompileRegLocalTensorStoreMode<asc::te::store_sideband_mode::direct>;
    auto compileStore1st = &CompileRegLocalTensorStoreMode<asc::te::store_sideband_mode::store_1st>;
    auto compileStoreIntlv = &CompileRegLocalTensorStoreMode<asc::te::store_sideband_mode::intlv>;
    auto compileStorePack = &CompileRegLocalTensorStoreMode<asc::te::store_sideband_mode::pack>;
    auto compileStorePackQuarter = &CompileRegLocalTensorStoreMode<asc::te::store_sideband_mode::pack_quarter>;
    (void)compileStoreDirect;
    (void)compileStore1st;
    (void)compileStoreIntlv;
    (void)compileStorePack;
    (void)compileStorePackQuarter;

    auto compileRegMaskAndFillB8 = &CompileRegMaskAndFill<uint8_t>;
    auto compileRegMaskAndFillB16 = &CompileRegMaskAndFill<half>;
    auto compileRegMaskAndFillB32 = &CompileRegMaskAndFill<float>;
    (void)compileRegMaskAndFillB8;
    (void)compileRegMaskAndFillB16;
    (void)compileRegMaskAndFillB32;
}

} // namespace Te
} // namespace AscendC
