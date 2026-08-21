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
 * \file ssbuffer.h
 * \brief SSBuffer核间通信样例的Kernel实现
 */

#include "kernel_operator.h"

namespace {

// 消息定义：32字节对齐的SSBuffer通信消息
struct alignas(32) SsbufferMessage {
    uint64_t value[4];
};
static_assert(sizeof(SsbufferMessage) == 32);

// AIV发布到AIC的flagId，需避开当前算子中Matmul高阶API或SyncAll等接口占用的flagId
constexpr uint16_t AIV_TO_AIC_FLAG = 14;
// 消息在SSBuffer中的起始偏移（须32字节对齐，且不越过SSBuffer末端）
constexpr uint32_t VERIFY_MESSAGE_OFFSET = 256;
// 期望的验证值
constexpr uint64_t EXPECTED_VALUE[4] = {
    0x1122334455667788ULL,
    0x8877665544332211ULL,
    0x0123456789abcdefULL,
    0xfedcba9876543210ULL,
};

} // namespace