/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_MATMUL_SCHEDULER_RUNTIME_ITERATE_ORDER_H
#define IMPL_MATMUL_SCHEDULER_RUNTIME_ITERATE_ORDER_H

#include "adv_api/matmul/matmul_config.h"
#include "utils/base/sys_macros.h"
#include "impl/basic_api/kernel_log.h"

namespace AscendC {
namespace Impl {
namespace Detail {

// Keep GetTiling() calls inside the branch, including DFX proxy calls and diagnostics.
template <typename ShapeTilingModule>
__aicore__ inline bool IsRuntimeMatmulOrderM(ShapeTilingModule&& shapeTiling)
{
    if (likely(shapeTiling->GetTiling().GetIterateOrder() == static_cast<int>(IterateOrder::ORDER_M))) {
        return true;
    }
    ASCENDC_ASSERT((shapeTiling->GetTiling().GetIterateOrder() == static_cast<int>(IterateOrder::ORDER_N)), {
        KERNEL_LOG(
            KERNEL_ERROR, "iterateOrder is %d , which should be ORDER_N", shapeTiling->GetTiling().GetIterateOrder());
    });
    return false;
}

} // namespace Detail
} // namespace Impl
} // namespace AscendC

#endif
