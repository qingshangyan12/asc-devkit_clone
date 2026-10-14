// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_DSL_AIV_PROGRAM_H
#define MC2_DSL_AIV_PROGRAM_H
#include <stdint.h>
namespace mc2_ops_hccl { namespace dsl {
enum class AivOp:uint32_t { COPY_INPUT, READ_PEER, BARRIER };
struct AivCommand { AivOp op;uint32_t peer;uint64_t srcOffset;uint64_t dstOffset;uint64_t bytes;uint64_t signal; };
// 每个 peer 由调用方提供经 HComm/对称窗口映射的 GM 地址，绝不是直接传 Host 指针。
struct AivPeer {uint64_t output;uint64_t signals;};
struct AivProgramHeader {uint32_t rank;uint32_t rankSize;uint32_t commandCount;uint32_t reserved;uint64_t epoch;};
// signals 至少 commandBarrierCount * rankSize * 32 字节，并在首轮启动前清零。
// 运行中不能重用正在读的 output；barrier 在每轮前后保证发布和远端读取完成。
} }
#endif
