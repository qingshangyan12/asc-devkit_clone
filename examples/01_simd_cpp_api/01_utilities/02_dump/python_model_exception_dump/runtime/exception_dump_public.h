/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file exception_dump_public.h
 * \brief Exception Dump 外挂回调库的 C ABI 声明（Python 经 ctypes 加载无需本头文件，
 * 供 C/C++ 宿主程序接入使用；实现依赖的 CANN 头文件仅在编译回调库时使用）
 */

#ifndef EXCEPTION_DUMP_PUBLIC_H
#define EXCEPTION_DUMP_PUBLIC_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 登记一个输入：name 落盘文件名，ptrIndex 为该输入指针在 kernel args 区的槽位
 * （0-based，8 字节/槽，按 launch 形参序校准），size/shapeStr/dtypeStr 描述该输入。
 * 返回 0 成功，超出容量返回负值。 */
int AddInputInfo(const char *name, size_t ptrIndex, size_t size,
                 const char *shapeStr, const char *dtypeStr);

/* 清空全部输入登记（新一轮 model 调用前调用） */
void ClearInputInfo(void);

/* 注册异常回调并按 kernelName 过滤异常任务（mangled 名子串匹配）。首次匹配异常时
 * 回调内自动挂载 per-binary 回调。算子零改动。返回 0 成功，失败返回 CANN 错误码。 */
int RegisterPublicExceptionDump(const char *kernelName);

#ifdef __cplusplus
}
#endif

#endif /* EXCEPTION_DUMP_PUBLIC_H */
