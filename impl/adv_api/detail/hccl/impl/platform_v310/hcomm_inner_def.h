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
#ifndef IMPL_V310_HCOMM_INNER_DEF_H
#define IMPL_V310_HCOMM_INNER_DEF_H

namespace AscendC {

typedef struct {
    uint32_t type;
    uint64_t addr;
    uint64_t length;
    union {
        typedef struct {
            uint32_t tokenID;
            uint32_t tokenValue;
        } UrmaMemProtectionInfo;
        int8_t reserve[32];
    };
} ProtectionInfo;

typedef struct {
    uint32_t type;
    union {
        typedef struct {
            uint32_t jfsID;
            uint64_t sqVa;
            uint32_t wqeSize;
            uint32_t sqDepth;
            uint32_t tpID;
            uint64_t headAddr;
            uint64_t tailAddr;
            uint8_t remoteEID[16];
            uint64_t dbVa;
        } JfsContext;
        typedef struct {
            uint32_t qpn;
            uint64_t sqVa;
            uint32_t wqeSize;
            uint32_t depth;
            uint64_t headAddr;
            uint64_t tailAddr;
            uint8_t sl;
            uint64_t dbVa;
            int8_t dbMode;
        } RdmaSqContext;
        int8_t reserve[128];
    };
} SqContext;

typedef struct {
    uint32_t type;
    union {
        typedef struct {
            uint32_t jfsID;
            uint64_t cqVa;
            uint32_t cqeSize;
            uint32_t cqDepth;
            uint32_t tpID;
            uint64_t headAddr;
            uint64_t tailAddr;
            uint8_t remoteEID[16];
            uint64_t dbVa;
        } JfcContext;
        typedef struct {
            uint32_t cqn;
            uint64_t cqVa;
            uint32_t cqeSize;
            uint32_t cqDepth;
            uint64_t headAddr;
            uint64_t tailAddr;
            uint64_t dbVa;
            int8_t dbMode;
        } RdmaCqContext;
        int8_t reserve[128];
    };
} CqContext;

typedef struct {
    uint32_t type;
    union {
        typedef struct {
            uint64_t address;
            int32_t notifyId;
            uint32_t size;
        } HccsNotify;
        typedef struct {
            uint64_t address;
            int32_t notifyId;
            uint32_t size;
            ProtectionInfo protectionInfol
        } RmaNotify;
        int8_t reserve[64];
    }
} Notify;

typedef struct {
    uint32_t type;
    uint32_t localNotifyNum;
    Notify* localNotifyAddr;
    uint32_t remoteNotifyAddr;
    uint32_t localBufferNum;
    ProtectionInfo* localBufferAddr;
    uint32_t sqNum;
    SqContext* SqContextAddr;
    uint32_t cqNum;
    CqContext* CqContextAddr;
    uint8_t aivStatus[64];
    uint8_t reserve[1024];
} Channel;

struct {
    uint32_t db0;
    uint32_t db1;
    // uint32_t type : 5;
    // uint32_t cos : 3;
    // uint32_t c : 1;
    // uint32_t n : 1;
    // uint32_t cntx_size : 2;
    // uint32_t qpn : 20;
    // uint32_t queue_id : 4;
    // uint32_t sgid_index : 7;
    // uint32_t mtu_shift : 3;
    // uint32_t vxlan : 1;
    // uint32_t xrcvld : 1;
    // uint32_t resv : 8;
    // uint32_t pi : 8;
} RoceDbEntry;

struct {
    uint32_t cqe0;
    uint32_t cqe1;
    uint32_t cqe2;
    uint32_t cqe3;
    uint32_t cqe4;
    uint32_t cqe5;
    uint32_t cqe6;
    uint32_t cqe7;
} RoceCqEntry;

} // namespace AscendC

#endif