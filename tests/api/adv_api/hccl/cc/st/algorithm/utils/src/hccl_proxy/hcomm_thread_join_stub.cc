/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// HcommThreadJoin 的 ST 强符号 stub（独立编译单元，先例见 hcomm_dlsym_stub.cc 的 HcommIsSupportCcuV2）。
// 为什么必须独立成文件：产品头 hcomm_primitives_dl.h 以 DECL_WEAK_FUNC 声明同名弱符号
// （__attribute__((weak))），任何 include 该头的编译单元内定义此函数都会继承 weak 属性；
// 与 hcomm_primitives_dl.cc 的弱兜底体并存时链接器取先链入者（kernel 库在前，选到返回 -1 的
// 兜底体）——PROD/64bit 归约路径因此中断并死锁。本文件只 include 原头 hcomm_primitives.h
// （无弱声明），产出唯一 extern "C" 强符号：链接期强符号全局胜出，与库顺序无关。
// sim 语义：顺序性已由任务图 notify 依赖保证（模板调 Join 仅为等 thread 队列排空的顺序防御），
// 成功返回即可。
#include "hcomm_primitives.h"
#include "hccl_types.h"

extern "C" HcclResult HcommThreadJoin(ThreadHandle thread, uint32_t timeout)
{
    (void)thread;
    (void)timeout;
    return HCCL_SUCCESS;
}
