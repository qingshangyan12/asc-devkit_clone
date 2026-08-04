/**
<<<<<<< HEAD
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
=======
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
>>>>>>> upstream/master
    "impl/tensor_api/algorithm/transform_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
<<<<<<< HEAD
* \file transform_impl.h
* \brief
*/
=======
 * \file transform_impl.h
 * \brief
 */
>>>>>>> upstream/master
#ifndef IMPL_TENSOR_API_ALGORITHM_TRANSFORM_IMPL_H
#define IMPL_TENSOR_API_ALGORITHM_TRANSFORM_IMPL_H

#include "impl/tensor_api/arch/vector/transform/transform_vf.h"

namespace AscendC {
namespace Te {

<<<<<<< HEAD
template<typename CalcFunc, typename TraitType = Std::ignore_t, typename... Args>
=======
template <typename CalcFunc, typename TraitType = Std::ignore_t, typename... Args>
>>>>>>> upstream/master
__aicore__ inline void Transform(const Args&... args)
{
    Transform2VF<CalcFunc, TraitType>::template Run(args...);
}

<<<<<<< HEAD
}
}
=======
} // namespace Te
} // namespace AscendC
>>>>>>> upstream/master

#endif // IMPL_TENSOR_API_ALGORITHM_TRANSFORM_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
