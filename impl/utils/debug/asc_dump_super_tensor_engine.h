// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef IMPL_UTILS_DEBUG_ASC_DUMP_SUPER_TENSOR_ENGINE_H
#define IMPL_UTILS_DEBUG_ASC_DUMP_SUPER_TENSOR_ENGINE_H

#include "impl/utils/debug/asc_debug_types.h"

namespace __asc_aicore {

__aicore__ inline void store_super_tensor_u64(__gm__ uint8_t* address, uint64_t value)
{
    __gm__ uint32_t* words = reinterpret_cast<__gm__ uint32_t*>(address);
    words[0] = static_cast<uint32_t>(value);
    words[1] = static_cast<uint32_t>(value >> 32);
}

// Transport owns FIFO capacity, source addressing, staging and publication.
// The protocol engine only advances the source offset after Commit succeeds.
template <typename Transport>
__aicore__ inline void dump_super_tensor(Transport& transport, uint64_t tensorLength,
    uint32_t addr, uint32_t dataType, uint16_t position, uint16_t blockIdx,
    uint32_t desc, const uint32_t* shape, uint32_t shapeDim)
{
    constexpr uint32_t maxShapeDim = 8;
    constexpr uint32_t alignment = 32;
    const uint32_t firstCapacity = transport.Capacity(sizeof(DumpTensorInfo));
    const uint32_t bodyCapacity = transport.Capacity(sizeof(DumpTensorBodyInfo));
    if (tensorLength == 0 || firstCapacity == 0 || bodyCapacity == 0 || shapeDim > maxShapeDim ||
        (shapeDim != 0 && shape == nullptr)) {
        return;
    }
    for (uint32_t i = 0; i < shapeDim; ++i) {
        if (shape[i] == 0) {
            return;
        }
    }

    uint64_t offset = 0;
    while (offset < tensorLength) {
        const bool first = offset == 0;
        const uint32_t headerSize = first ? sizeof(DumpTensorInfo) : sizeof(DumpTensorBodyInfo);
        const uint32_t capacity = first ? firstCapacity : bodyCapacity;
        const uint64_t remaining = tensorLength - offset;
        const uint32_t payload = static_cast<uint32_t>(remaining < capacity ? remaining : capacity);
        if (payload == 0 || payload > UINT32_MAX - headerSize - (alignment - 1) ||
            (remaining > payload && payload % alignment != 0)) {
            return;
        }
        const uint32_t recordLength = (headerSize + payload + alignment - 1) / alignment * alignment;
        __gm__ uint8_t* record = transport.Reserve(headerSize, payload);
        if (record == nullptr || !transport.Copy(record, headerSize, offset, payload)) {
            return;
        }
        for (uint32_t i = headerSize + payload; i < recordLength; ++i) {
            record[i] = 0;
        }
        if (first) {
            auto info = reinterpret_cast<__gm__ DumpTensorInfo*>(record);
            info->dumpType = static_cast<uint32_t>(DumpType::DUMP_SUPER_TENSOR);
            info->dumpLength = recordLength - 8;
            info->addr = addr;
            info->dataType = dataType;
            info->desc = desc;
            info->offset = 0;
            info->position = position;
            info->blockIdx = blockIdx;
            info->dim = shapeDim;
            for (uint32_t i = 0; i < maxShapeDim; ++i) {
                info->shape[i] = i < shapeDim ? shape[i] : 0;
            }
            info->resv = 0;
            store_super_tensor_u64(record + 68, tensorLength);
            store_super_tensor_u64(record + 76, offset);
            info->dumpSize = payload;
        } else {
            auto info = reinterpret_cast<__gm__ DumpTensorBodyInfo*>(record);
            info->dumpType = static_cast<uint32_t>(DumpType::DUMP_SUPER_TENSOR_BODY);
            info->dumpLength = recordLength - 8;
            info->resv1 = 0;
            info->resv2 = 0;
            store_super_tensor_u64(record + 16, tensorLength);
            store_super_tensor_u64(record + 24, offset);
            info->dumpSize = payload;
        }
        transport.Commit(record, recordLength);
        offset += payload;
    }
}

} // namespace __asc_aicore
#endif // IMPL_UTILS_DEBUG_ASC_DUMP_SUPER_TENSOR_ENGINE_H
