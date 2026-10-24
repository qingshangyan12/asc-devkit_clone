/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_API_CHECK_BINARY_ELEMENTWISE_CHECK_H
#define IMPL_API_CHECK_BINARY_ELEMENTWISE_CHECK_H

#include "calcount_check.h"
#include "reuse_source_check.h"
#include "single_tensor_check.h"
#include "multiple_tensor_check.h"

namespace AscendC {
namespace HighLevelApiCheck {
template <bool isReuseSource, typename T, typename U>
__aicore__ inline void VerifyBinaryElementwiseParameters(
    ReuseSourceCheckFuncBasicClass& reuseCheck, CalCountCheckFuncBasicClass& countCheck,
    SingleTensorCheckFuncBasicClass& singleCheck, MultipleTensorCheckFuncBasicClass& multipleCheck,
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<U>& src1, const uint32_t count)
{
    reuseCheck.IsReuseSourceVerifyingParameters<false>(ARG_AND_STRING(isReuseSource));
    countCheck.CalCountVerifyingParameters(ARG_AND_STRING(count), VA_ARGS_TO_MAKE_TUPLE(dst, src0, src1));
    singleCheck.TensorVerifyingParameters(
        VA_ARGS_TO_MAKE_TUPLE(dst, src0, src1),
        MakeParameters2Tuple(TPosition::VECIN, TPosition::VECOUT, TPosition::VECCALC), "UB(VECIN/VECOUT/VECCALC)");
    multipleCheck.TensorReuseVerifyingParameters(VA_ARGS_TO_MAKE_TUPLE(dst, src0));
    multipleCheck.TensorReuseVerifyingParameters(VA_ARGS_TO_MAKE_TUPLE(dst, src1));
}
} // namespace HighLevelApiCheck
} // namespace AscendC
#endif
