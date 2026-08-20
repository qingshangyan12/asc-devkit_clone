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
 * \brief 多进程注册执行 AICPU 算子样例的参数结构体定义
 */

#pragma once

#include <cstdint>

namespace KernelInfo {
struct ProcArgs {
    uint32_t procId;        // 进程标识
    uint32_t printCount;    // 内核内 printf 次数
};
} // namespace KernelInfo
