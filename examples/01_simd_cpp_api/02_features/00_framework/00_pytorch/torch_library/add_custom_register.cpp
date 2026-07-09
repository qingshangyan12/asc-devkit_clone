/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <torch/extension.h>

namespace ascendc_ops {
at::Tensor ascendc_add(const at::Tensor& x, const at::Tensor& y, const at::Tensor& z);
} // namespace ascendc_ops

TORCH_LIBRARY(ascendc_ops, m) { m.def("ascendc_add(Tensor x, Tensor y, Tensor z) -> Tensor"); }

TORCH_LIBRARY_IMPL(ascendc_ops, PrivateUse1, m) { m.impl("ascendc_add", TORCH_FN(ascendc_ops::ascendc_add)); }
