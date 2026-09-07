/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_PROFILING_BINDING_PROTOCOL_H
#define MC2_PROFILING_BINDING_PROTOCOL_H
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <type_traits>

namespace Mc2ProfilingBinding {
// Private host-only contract copied verbatim between asc-devkit and HCOMM.
// No C++ objects, device pointers to dereference, or public DFX layouts cross this boundary.
constexpr uint64_t MAGIC = 0x4D433250524F4631ULL;
constexpr uint32_t VERSION = 1;
constexpr uint32_t MAX_THREADS = 4096;
constexpr const char* TAG_PREFIX = "__asc_mc2_prof_bind_v1_";
constexpr int32_t ACK_PENDING = -1;
struct Header {
    uint64_t magic = MAGIC;
    uint32_t version = VERSION;
    uint32_t headerSize = 80;
    uint64_t totalSize = 0;
    uint64_t resourceId = 0;
    uint64_t generation = 0;
    uint32_t kfcStreamId = UINT32_MAX;
    uint32_t threadCount = 0;
    uint64_t requestId = 0;
    uint64_t ackRequestId = 0;
    int32_t ackStatus = ACK_PENDING;
    uint32_t reserved = 0;
    uint64_t reserved2 = 0;
};
static_assert(sizeof(Header) == 80, "MC2 binding header ABI");
static_assert(offsetof(Header, resourceId) == 24, "MC2 binding resource ABI");
static_assert(offsetof(Header, ackRequestId) == 56, "MC2 binding ack ABI");
static_assert(std::is_trivially_copyable<Header>::value, "MC2 binding must be POD");

inline bool IsBindingTag(const char* tag)
{
    return tag != nullptr && std::strncmp(tag, TAG_PREFIX, std::strlen(TAG_PREFIX)) == 0;
}

inline std::string Tag(uint64_t resourceId)
{
    return std::string(TAG_PREFIX) + std::to_string(resourceId);
}

inline bool Validate(const Header& header, uint64_t bytes, const char* tag)
{
    return header.magic == MAGIC && header.version == VERSION && header.headerSize == sizeof(Header) &&
        header.resourceId != 0 && header.generation != 0 && header.requestId != 0 &&
        header.kfcStreamId != UINT32_MAX && header.threadCount != 0 && header.threadCount <= MAX_THREADS &&
        header.totalSize == bytes && bytes == sizeof(Header) + uint64_t(header.threadCount) * sizeof(uint64_t) &&
        header.ackRequestId == 0 && header.ackStatus == ACK_PENDING && header.reserved == 0 &&
        header.reserved2 == 0 && tag != nullptr && Tag(header.resourceId) == tag;
}
} // namespace Mc2ProfilingBinding
#endif
