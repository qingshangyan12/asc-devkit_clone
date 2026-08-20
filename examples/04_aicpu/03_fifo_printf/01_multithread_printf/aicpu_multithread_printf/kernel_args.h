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
 * \brief 多线程 printf 样例的参数结构体定义
 */

#pragma once

#include <cstdint>

namespace KernelInfo {
struct ThreadArgs {
    uint32_t threadId;       // 线程 ID
    uint32_t printCount;     // 每个线程的 printf 次数
};

struct KernelArgs {
    ThreadArgs* args;       // 线程参数数组
    uint32_t threadNum;      // 线程总数
};
} // namespace KernelInfo
