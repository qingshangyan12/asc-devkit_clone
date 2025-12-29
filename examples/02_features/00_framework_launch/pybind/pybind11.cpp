/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file pybind11.cpp
 * \brief
 */

#include <pybind11/pybind11.h>
#include <torch/extension.h>

#include "aclrtlaunch_add_custom.h"
#include "torch_npu/csrc/core/npu/NPUStream.h"

namespace my_add {
at::Tensor run_add_custom(const at::Tensor &x, const at::Tensor &y)
{
    auto acl_stream = c10_npu::getCurrentNPUStream().stream(false);
    at::Tensor z = at::empty_like(x);
    uint32_t blockDim = 8;
    uint32_t totalLength = 1;
    for (uint32_t size : x.sizes()) {
        totalLength *= size;
    }
    ACLRT_LAUNCH_KERNEL(add_custom)
    (blockDim, acl_stream, const_cast<void *>(x.storage().data()), const_cast<void *>(y.storage().data()),
     const_cast<void *>(z.storage().data()), totalLength);
    return z;
}
} // namespace my_add

PYBIND11_MODULE(add_custom, m)
{
    m.doc() = "add_custom pybind11 interfaces"; // optional module docstring
    m.def("run_add_custom", &my_add::run_add_custom, "");
}
