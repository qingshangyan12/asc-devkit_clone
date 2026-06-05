/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef EXAMPLES_01_SIMD_CPP_API_02_FEATURES_02_FRAMEWORK_03_GE_TRIPLE_CHEVRON_NOTATION_ADD_CUSTOM_PROTO_H_
#define EXAMPLES_01_SIMD_CPP_API_02_FEATURES_02_FRAMEWORK_03_GE_TRIPLE_CHEVRON_NOTATION_ADD_CUSTOM_PROTO_H_

#include "graph/operator_reg.h"

namespace ge {
REG_OP(AddCustom)
    .INPUT(x, TensorType({DT_FLOAT16}))
    .INPUT(y, TensorType({DT_FLOAT16}))
    .OUTPUT(z, TensorType({DT_FLOAT16}))
    .OP_END_FACTORY_REG(AddCustom);
}  // namespace ge

#endif  // EXAMPLES_01_SIMD_CPP_API_02_FEATURES_02_FRAMEWORK_03_GE_TRIPLE_CHEVRON_NOTATION_ADD_CUSTOM_PROTO_H_
