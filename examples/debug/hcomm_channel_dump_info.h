/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCOMM_CHANNEL_DUMP_INFO_H
#define HCOMM_CHANNEL_DUMP_INFO_H

#include <cstdint>

constexpr uint32_t HCOMM_CHANNEL_DUMP_MAGIC = 0x48434449U;
constexpr uint32_t HCOMM_CHANNEL_DUMP_VERSION = 1U;
constexpr uint32_t HCOMM_CHANNEL_DUMP_MAX_CHANNELS = 8U;
constexpr uint32_t HCOMM_CHANNEL_DUMP_MAX_BUFFERS = 8U;
constexpr uint32_t HCOMM_CHANNEL_DUMP_MAX_QUEUE_CONTEXTS = 8U;
constexpr uint32_t HCOMM_CHANNEL_DUMP_MAX_QUEUE_ENTRIES = 16U;
constexpr uint32_t HCOMM_CHANNEL_DUMP_QUEUE_ENTRY_BYTES = 256U;
constexpr uint32_t HCOMM_CHANNEL_DUMP_CHANNEL_ENTITY_BYTES = 256U;
constexpr uint32_t HCOMM_CHANNEL_DUMP_CONTEXT_BYTES = 128U;

enum HcommChannelDumpEntryType : uint32_t {
    HCOMM_CHANNEL_DUMP_ENTRY_URMA_WQE = 0,
    HCOMM_CHANNEL_DUMP_ENTRY_URMA_CQE = 1,
};

struct HcommChannelDumpBufferInfo {
    int32_t type;
    uint64_t addr;
    uint64_t size;
};

struct HcommChannelDumpQueueContext {
    int32_t type;
    uint64_t va;
    uint64_t headAddr;
    uint64_t tailAddr;
    uint64_t dbVa;
    uint32_t depth;
    uint32_t entrySize;
    uint32_t head;
    uint32_t tail;
    uint32_t headValid;
    uint32_t tailValid;
    uint32_t rawSize;
    uint8_t raw[HCOMM_CHANNEL_DUMP_CONTEXT_BYTES];
};

struct HcommChannelDumpQueueEntry {
    uint32_t entryType;
    uint32_t queueIndex;
    uint32_t entryIndex;
    uint32_t cursor;
    uint64_t addr;
    uint32_t size;
    uint32_t valid;
    uint8_t data[HCOMM_CHANNEL_DUMP_QUEUE_ENTRY_BYTES];
};

struct HcommChannelDumpChannelInfo {
    uint64_t handle;
    uint32_t valid;
    uint32_t channelEntitySize;
    uint8_t channelEntity[HCOMM_CHANNEL_DUMP_CHANNEL_ENTITY_BYTES];
    int32_t engine;
    int32_t protocol;
    uint32_t localBufferNum;
    uint32_t remoteBufferNum;
    uint32_t sqNum;
    uint32_t cqNum;
    uint32_t wqeCnt;
    uint32_t localBufferDumpNum;
    uint32_t remoteBufferDumpNum;
    uint32_t sqContextDumpNum;
    uint32_t cqContextDumpNum;
    uint32_t queueEntryDumpNum;
    HcommChannelDumpBufferInfo localBuffers[HCOMM_CHANNEL_DUMP_MAX_BUFFERS];
    HcommChannelDumpBufferInfo remoteBuffers[HCOMM_CHANNEL_DUMP_MAX_BUFFERS];
    HcommChannelDumpQueueContext sqContexts[HCOMM_CHANNEL_DUMP_MAX_QUEUE_CONTEXTS];
    HcommChannelDumpQueueContext cqContexts[HCOMM_CHANNEL_DUMP_MAX_QUEUE_CONTEXTS];
    HcommChannelDumpQueueEntry queueEntries[HCOMM_CHANNEL_DUMP_MAX_QUEUE_ENTRIES];
};

struct HcommChannelDumpData {
    uint32_t magic;
    uint32_t version;
    uint32_t size;
    uint32_t channelNum;
    HcommChannelDumpChannelInfo channels[HCOMM_CHANNEL_DUMP_MAX_CHANNELS];
};

#endif // HCOMM_CHANNEL_DUMP_INFO_H
