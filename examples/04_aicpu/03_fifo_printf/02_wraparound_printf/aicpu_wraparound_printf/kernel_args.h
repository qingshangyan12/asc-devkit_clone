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
 * \file kernel_args.h
 * \brief 回绕 printf 样例的参数结构体定义
 */

#pragma once

#include <cstdint>

namespace KernelInfo {
struct WrapArgs {
    uint32_t totalPrints;    // 总 printf 次数
    uint32_t strLen;          // 每条 printf 字符串的目标长度（不含参数部分）
    uint32_t batchSize;      // 每批输出数量（用于分批触发回绕）
};
} // namespace KernelInfo
