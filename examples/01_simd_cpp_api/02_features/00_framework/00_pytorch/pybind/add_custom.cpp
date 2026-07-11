/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdint>

#include <pybind11/pybind11.h>
#include <torch/extension.h>
#include "torch_npu/csrc/core/npu/NPUStream.h"

extern "C" void launch_add_custom(uint8_t* x, uint8_t* y, uint8_t* z, uint32_t totalLength, void* stream);

namespace ascendc_ops {
at::Tensor ascendc_add(const at::Tensor& x, const at::Tensor& y)
{
    at::Tensor z = at::empty_like(x);
    auto aclStream = c10_npu::getCurrentNPUStream().stream(false);
    uint32_t totalLength = 1;
    for (uint32_t size : x.sizes()) {
        totalLength *= size;
    }
    launch_add_custom(
        static_cast<uint8_t*>(x.mutable_data_ptr()), static_cast<uint8_t*>(y.mutable_data_ptr()),
        static_cast<uint8_t*>(z.mutable_data_ptr()), totalLength, aclStream);
    return z;
}
} // namespace ascendc_ops

PYBIND11_MODULE(ascendc_ops, m)
{
    m.doc() = "add_custom pybind11 interfaces";
    m.def("ascendc_add", &ascendc_ops::ascendc_add, "");
}
