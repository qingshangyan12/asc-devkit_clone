/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "aicpu_kfc_interface.h"
#include <cstdlib>

extern "C" {
__attribute__((visibility("default"))) uint32_t Mc2ServerKernel(void *args[])
{
    // 通过解引用空指针触发SIGSEGV信号，操作系统收到该信号后会终止进程并生成core dump文件
    // 如果运行时产生了core dump，说明外部调用成功进入了此函数；如果没有core dump，说明此函数未被调用到
    volatile int *p = nullptr;
    *p = 0xDEAD;
    return 0;
}
}
