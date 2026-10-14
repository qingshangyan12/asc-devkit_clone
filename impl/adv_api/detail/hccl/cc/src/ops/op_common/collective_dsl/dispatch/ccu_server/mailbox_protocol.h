// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_DSL_CCU_MAILBOX_PROTOCOL_H
#define MC2_DSL_CCU_MAILBOX_PROTOCOL_H
#include <stdint.h>
namespace mc2_ops_hccl { namespace dsl {
// 单 producer、有界环。sequence 从 1 开始，slot=(sequence-1)%capacity。
// 同一槽位再次使用前等待 completed==sequence-capacity；参数必须先于 submitted 写入。
struct alignas(64) CcuRequest {
    uint64_t input;
    uint64_t output;
    uint64_t outputOffset;
    uint64_t bytes;
    uint64_t inplace;
    uint64_t stop;
    uint64_t reserved[2];
};
struct alignas(64) CcuMailbox {
    CcuRequest request;
    uint64_t submitted;
    uint64_t submitPadding[7];
    uint64_t completed;
    uint64_t status; // 0=成功，1=跨 rank 请求不一致；错误后 server 退出，调用方不可继续复用。
    uint64_t completionPadding[6];
};
static_assert(sizeof(CcuMailbox)==192,"mailbox ABI");
struct CcuQueueConfig {uint32_t capacity;uint32_t reserved;};
} }
#endif
