/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file hcomm_inner_def.h
 * \brief
 */
#ifndef IMPL_HCOMM_HCOMM_INNER_DEF_H
#define IMPL_HCOMM_HCOMM_INNER_DEF_H

#include <cstdint>

namespace AscendC {

typedef struct {
    uint32_t type; // 0 RDMA   1 URMA
    uint64_t addr;
    uint64_t length;
    union {
        struct {
            uint32_t lkey;
            uint32_t rkey;
        } rdmaMemProtectionInfo;
        struct {
            uint32_t tokenID;
            uint32_t tokenValue;
        } urmaMemProtectionInfo;
        int8_t reserve[32];
    } pti;
} ProtectionInfo;

typedef struct {
    uint32_t type;
    union {
        struct {
            uint32_t jfsID;
            uint64_t sqVa;
            uint32_t wqeSize;
            uint32_t sqDepth;
            uint32_t tpID;
            uint64_t headAddr;
            uint64_t tailAddr;
            uint8_t remoteEID[16];
            uint64_t dbVa;
        } jfsContext; // 48+16=64Bytes
        struct {
            uint32_t qpn;
            uint64_t sqVa;     // sq的基地址，wqe后续往这个地址放
            uint32_t wqeSize;  // 单个wqe的size
            uint32_t depth;    // sq队列长度
            uint64_t headAddr; // sq_PI
            uint64_t tailAddr; // sq_CI
            uint8_t sl;
            uint64_t dbVa; // 敲doorbell的地址
            int8_t dbMode; // 0-hw/1-sw
        } rdmaSqContext;   // 46 Bytes
        int8_t reserve[128];
    } cxt;
} SqContext;

typedef struct {
    uint32_t type;
    union {
        struct {
            uint32_t jfcID;
            uint64_t scqVa;
            uint32_t cqeSize;
            uint32_t cqDepth;
            uint64_t headAddr;
            uint64_t tailAddr;
            uint64_t dbVa;
        } jfcContext;
        struct {
            uint32_t cqn;
            uint64_t cqVa;
            uint32_t cqeSize;
            uint32_t cqDepth;
            uint64_t headAddr;
            uint64_t tailAddr; // cq_CI
            uint64_t dbVa;
            int8_t dbMode; // 0-hw/1-sw
        } rdmaCqContext;
        int8_t reserve[128];
    } ctx;
} CqContext;

typedef struct {
    uint32_t type;
    union {
        struct {
            uint64_t address;
            int32_t notifyId;
            uint32_t size; // 默认4Byte
        } hccsNotify;
        struct {
            uint64_t address; // 创建notify的时候申请的内存的地址（寄存器的虚地址）
            int32_t notifyId; // remote 时不用
            uint32_t size;    // 默认4Byte
            ProtectionInfo protectionInfo;
        } rmaNotify;
        int8_t reserve[64];
    };
} Notify;

// AIV直驱RoCE，无需考虑notify
// write with notify的语义，地址是GM地址
typedef struct {   // channel 本身一片内存
    uint32_t type; // RDMA, SDMA, URMA
    // local notify
    uint32_t localNotifyNum; // 几个local notify
    Notify* localNotifyAddr; // local notify的数组，分别指向不同的内存片
    // remote notify
    uint32_t remoteNotifyNum;
    Notify* remoteNotifyAddr;
    // Local buffer
    uint32_t localBufferNum;
    ProtectionInfo* localBufferAddr;
    // Remote buffer
    uint32_t remoteBufferNum;
    ProtectionInfo* remoteBufferAddr;
    // SQ
    uint32_t sqNum;
    SqContext* sqContextAddr; // write/read/commit 操作这个字段
    // CQ
    uint32_t cqNum;
    CqContext* cqContextAddr; // wait 操作这个字段
    // AIV Status
    uint8_t aivStatus[64];
    // reserve
    uint8_t reserve[1024];
} Channel;

} // namespace AscendC
#endif // IMPL_HCOMM_HCOMM_INNER_DEF_H
