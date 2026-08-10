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
#include "tensor_api/experimental/reg/load_mode.h"
#include "tensor_api/experimental/reg/mask.h"
#include "tensor_api/tensor.h"

namespace AscendC {
namespace Te {

template <typename T, typename = void>
struct has_mask_member : std::false_type {};

template <typename T>
struct has_mask_member<T, std::void_t<decltype(&T::mask)>> : std::true_type {};

template <typename T, asc::te::load_sideband_mode sidebandMode = asc::te::load_sideband_mode::direct>
__simd_vf__ inline void CompileTensorRegLoadStore(
    __ubuf__ T* dst, __ubuf__ T* src, vector_bool mask)
{
    auto layout = MakeLayout(MakeShape(64));
    auto dstTensor = MakeTensor(MakeMemPtr<Location::UB>(dst), layout);
    auto srcTensor = MakeTensor(MakeMemPtr<Location::UB>(src), layout);
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

    auto compileLoadStore = &CompileTensorRegLoadStore<float>;
    (void)compileLoadStore;

    auto compileRegMaskAndFillB8 = &CompileRegMaskAndFill<uint8_t>;
    auto compileRegMaskAndFillB16 = &CompileRegMaskAndFill<half>;
    auto compileRegMaskAndFillB32 = &CompileRegMaskAndFill<float>;
    (void)compileRegMaskAndFillB8;
    (void)compileRegMaskAndFillB16;
    (void)compileRegMaskAndFillB32;
}

} // namespace Te
} // namespace AscendC
