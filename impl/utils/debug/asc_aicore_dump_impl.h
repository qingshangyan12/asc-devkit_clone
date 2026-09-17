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
 * \file asc_aicore_dump_impl.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_IMPL_H

#ifndef ASCENDC_CPU_DEBUG
#include "impl/utils/sys_macros.h"
#include "impl/utils/common_types.h"
#include "impl/utils/debug/asc_debug_types.h"
#include "impl/utils/debug/asc_debug_utils.h"
#include "impl/utils/debug/asc_dump_super_tensor_capability.h"
#include "impl/utils/debug/asc_dump_super_tensor_engine.h"

#if __NPU_ARCH__ == 2201
#include "impl/utils/debug/npu_arch_2201/asc_aicore_dump_utils.h"
#elif __NPU_ARCH__ == 3510
#include "impl/utils/debug/npu_arch_3510/asc_aicore_dump_utils.h"
#elif __NPU_ARCH__ == 5102
#include "impl/utils/debug/npu_arch_5102/asc_aicore_dump_utils.h"
#endif

namespace __asc_aicore {

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline void set_dump_tlv_info(
    U src, __gm__ DumpTensorTlv* dumpTlv, uint32_t alignDumpLen, uint32_t desc, uint32_t dump_size)
{
    dumpTlv->type = static_cast<uint32_t>(DumpType::DUMP_TENSOR);
    dumpTlv->length = sizeof(DumpTensorTlv) - sizeof(uint32_t[2]) + alignDumpLen;
    dumpTlv->tensorAddr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(src));
    dumpTlv->dataType = static_cast<uint32_t>(get_dump_datatype<T>());
    dumpTlv->desc = desc;
    dumpTlv->blockIdx = static_cast<uint32_t>(asc_debug_get_block_idx());
    dumpTlv->bufferId = static_cast<uint32_t>(0U);
    dumpTlv->position = static_cast<uint16_t>(hardware);
    dumpTlv->dim = static_cast<uint32_t>(0U);
    for (uint32_t i = 0; i < K_MAX_SHAPE_DIM; ++i) {
        dumpTlv->shape[i] = static_cast<uint32_t>(0U);
    }
    dumpTlv->resv1 = static_cast<uint32_t>(0U);
    dumpTlv->dumpSize = dump_size * sizeof(T);
    asc_entire_dcci(reinterpret_cast<__gm__ uint64_t*>(dumpTlv));
}

__aicore__ inline void set_dump_shape_info(
    __gm__ DumpTensorTlv* dumpTlv, const uint32_t shapeDim, const uint32_t* shape)
{
    if (shapeDim <= 0 || shapeDim > K_MAX_SHAPE_DIM || shape == nullptr) {
        return;
    }
    dumpTlv->dim = static_cast<uint32_t>(shapeDim);
    for (uint32_t i = 0; i < K_MAX_SHAPE_DIM; ++i) {
        dumpTlv->shape[i] = i < shapeDim ? static_cast<uint32_t>(shape[i]) : static_cast<uint32_t>(1U);
    }
    asc_entire_dcci(reinterpret_cast<__gm__ uint64_t*>(dumpTlv));
}

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline uint32_t set_dump_tlv_data(
    U src, __gm__ DumpTensorTlv* dumpTlv, uint32_t alignDumpLen, uint32_t dump_size)
{
    __gm__ T* dumpDstAddr = reinterpret_cast<__gm__ T*>(dumpTlv + 1);

    if (dumpDstAddr == nullptr) {
        return 1;
    }
    if (hardware == AscendC::Hardware::GM && src == nullptr) {
        return 1;
    }

    sync_all();
    uint32_t ret = 0;
    uint32_t dumpLen = 0;
    if constexpr (hardware == AscendC::Hardware::GM) {
        dumpLen = dump_size * sizeof(T);
        ret = mem_copy_gm_to_gm(
            reinterpret_cast<__gm__ uint8_t*>(dumpDstAddr), reinterpret_cast<__gm__ const uint8_t*>(src), dumpLen);
    } else if constexpr (hardware == AscendC::Hardware::UB) {
        dumpLen = alignDumpLen / ASC_ONE_DATABLOCK_SIZE;
        ret = mem_copy_ub_to_gm_impl(dumpDstAddr, src, static_cast<uint16_t>(dumpLen));
    } else if constexpr (hardware == AscendC::Hardware::L1) {
        ret = mem_copy_l1buf_to_gm_impl(dumpDstAddr, src, alignDumpLen);
    } else if constexpr (hardware == AscendC::Hardware::L0A) {
        ret = mem_copy_abuf_to_gm_impl(dumpDstAddr, src, alignDumpLen);
    } else if constexpr (hardware == AscendC::Hardware::L0B) {
        ret = mem_copy_bbuf_to_gm_impl(dumpDstAddr, src, alignDumpLen);
    } else if constexpr (hardware == AscendC::Hardware::L0C) {
        ret = mem_copy_cbuf_to_gm_impl(dumpDstAddr, src, alignDumpLen);
    } else if constexpr (hardware == AscendC::Hardware::BIAS) {
        ret = mem_copy_biasbuf_to_gm_impl(dumpDstAddr, src, alignDumpLen);
    } else if constexpr (hardware == AscendC::Hardware::FIXBUF) {
        ret = mem_copy_fbuf_to_gm_impl(dumpDstAddr, src, alignDumpLen);
    }
    sync_all();
    return ret;
}

#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102
template <AscendC::Hardware hardware>
__aicore__ inline bool is_super_tensor_position_supported()
{
    if constexpr (hardware == AscendC::Hardware::GM) {
        return true;
    }
#if defined(__DAV_VEC__) || __NPU_ARCH__ == 5102
    if constexpr (hardware == AscendC::Hardware::UB) {
        return true;
    }
#endif
#if defined(__DAV_CUBE__) || __NPU_ARCH__ == 5102
    if constexpr (hardware == AscendC::Hardware::L0C) {
        return true;
    }
#endif
#if defined(__DAV_CUBE__) && __NPU_ARCH__ == 2201
    if constexpr (hardware == AscendC::Hardware::L1) {
        return true;
    }
#elif defined(__DAV_CUBE__) && __NPU_ARCH__ == 3510
    if constexpr (hardware == AscendC::Hardware::L1 || hardware == AscendC::Hardware::L0A ||
                  hardware == AscendC::Hardware::L0B || hardware == AscendC::Hardware::BIAS ||
                  hardware == AscendC::Hardware::FIXBUF) {
        return get_debug_bus_addr_impl() != 0;
    }
#endif
    return false;
}

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline uint32_t copy_super_tensor_data(
    U src, __gm__ T* dst, uint32_t alignedBytes, uint32_t payloadBytes)
{
    if (dst == nullptr) {
        return 1;
    }
    sync_all();
    uint32_t ret = 1;
    if constexpr (hardware == AscendC::Hardware::GM) {
        ret = mem_copy_gm_to_gm(dst, reinterpret_cast<__gm__ const uint8_t*>(src), payloadBytes);
    } else if constexpr (hardware == AscendC::Hardware::UB) {
        ret = mem_copy_ub_to_gm_impl(dst, src, alignedBytes / ASC_ONE_DATABLOCK_SIZE);
    } else if constexpr (hardware == AscendC::Hardware::L0C) {
#if __NPU_ARCH__ == 3510
        ret = mem_copy_cbuf_to_gm_impl(dst, src, 1024);
#else
        ret = mem_copy_cbuf_to_gm_impl(dst, src, 256);
#endif
    } else if constexpr (hardware == AscendC::Hardware::L1 && __NPU_ARCH__ == 2201) {
        ret = mem_copy_l1buf_to_gm_impl(dst, src, alignedBytes / ASC_ONE_DATABLOCK_SIZE);
    } else if constexpr (hardware == AscendC::Hardware::L1) {
        ret = mem_copy_l1buf_to_gm_impl(dst, src, alignedBytes);
    } else if constexpr (hardware == AscendC::Hardware::L0A) {
        ret = mem_copy_abuf_to_gm_impl(dst, src, alignedBytes);
    } else if constexpr (hardware == AscendC::Hardware::L0B) {
        ret = mem_copy_bbuf_to_gm_impl(dst, src, alignedBytes);
    } else if constexpr (hardware == AscendC::Hardware::BIAS) {
        ret = mem_copy_biasbuf_to_gm_impl(dst, src, alignedBytes);
    } else if constexpr (hardware == AscendC::Hardware::FIXBUF) {
        ret = mem_copy_fbuf_to_gm_impl(dst, src, alignedBytes);
    }
    sync_all();
    return ret;
}

template <AscendC::Hardware hardware, typename T>
__aicore__ inline uint32_t get_super_tensor_capacity(
    __gm__ DebugBlockHeadInfo* blockInfo, uint32_t headerSize)
{
    const uint32_t alignedHeader = align_up(headerSize, ASC_ONE_DATABLOCK_SIZE);
    uint32_t scratch = hardware == AscendC::Hardware::GM ? 0 : 31;
    if constexpr (hardware == AscendC::Hardware::L0C) {
        if (sizeof(T) != sizeof(uint32_t)) {
            return 0;
        }
        scratch += 1024;
    }
    uint32_t capacity = get_max_tlv_payload(blockInfo, alignedHeader, ASC_ONE_DATABLOCK_SIZE, scratch);
    if constexpr (hardware != AscendC::Hardware::GM && hardware != AscendC::Hardware::L0C) {
        capacity = capacity / (2 * ASC_ONE_DATABLOCK_SIZE) * ASC_ONE_DATABLOCK_SIZE;
    }
    if constexpr (hardware == AscendC::Hardware::L0C) {
        return capacity >= 1024 ? 1024 : 0;
    }
    constexpr uint32_t maxDmaBytes = 65535U * ASC_ONE_DATABLOCK_SIZE;
    return capacity < maxDmaBytes ? capacity : maxDmaBytes;
}

template <AscendC::Hardware hardware, typename T, typename U>
struct SharedFifoTensorTransport {
    U src;
    __gm__ DebugBlockHeadInfo* blockInfo;

    __aicore__ inline uint32_t Capacity(uint32_t headerSize)
    {
        return get_super_tensor_capacity<hardware, T>(blockInfo, headerSize);
    }

    __aicore__ inline __gm__ uint8_t* Reserve(uint32_t headerSize, uint32_t payload)
    {
        const uint32_t alignedBytes = align_up(payload, ASC_ONE_DATABLOCK_SIZE);
        const uint32_t recordLength = align_up(headerSize + payload, ASC_ONE_DATABLOCK_SIZE);
        uint32_t reserveLength = recordLength;
        if constexpr (hardware != AscendC::Hardware::GM) {
            reserveLength += 31 + (hardware == AscendC::Hardware::L0C ? 1024 : alignedBytes);
        }
        if (!can_write_single_tlv(blockInfo, reserveLength) || !check_ringbuf_space(blockInfo, reserveLength)) {
            return nullptr;
        }
        return get_ringbuf_tlv_addr(blockInfo);
    }

    __aicore__ inline bool Copy(__gm__ uint8_t* record, uint32_t headerSize, uint64_t offset, uint32_t payload)
    {
        const uint32_t alignedBytes = align_up(payload, ASC_ONE_DATABLOCK_SIZE);
        const uint32_t recordLength = align_up(headerSize + payload, ASC_ONE_DATABLOCK_SIZE);
        sync_all();
        if constexpr (hardware == AscendC::Hardware::GM) {
            const uintptr_t address = reinterpret_cast<uintptr_t>(src);
            if (address == 0 || offset > UINTPTR_MAX - address || payload > UINTPTR_MAX - address - offset) {
                return false;
            }
            return mem_copy_gm_to_gm(
                       record + headerSize,
                       reinterpret_cast<__gm__ const uint8_t*>(src) + offset,
                       payload) == 0;
        } else {
            if (offset % sizeof(T) != 0 || offset / sizeof(T) > UINT32_MAX) {
                return false;
            }
            const uintptr_t stagingAddr =
                (reinterpret_cast<uintptr_t>(record + recordLength) + 31U) & ~static_cast<uintptr_t>(31U);
            __gm__ T* staging = reinterpret_cast<__gm__ T*>(stagingAddr);
            if (copy_super_tensor_data<hardware, T>(src + offset / sizeof(T), staging, alignedBytes, payload) != 0) {
                return false;
            }
            asc_entire_dcci(reinterpret_cast<__gm__ uint64_t*>(staging));
            return mem_copy_gm_to_gm(
                       record + headerSize, reinterpret_cast<__gm__ const uint8_t*>(staging), payload) == 0;
        }
    }

    __aicore__ inline void Commit(__gm__ uint8_t*, uint32_t recordLength)
    {
        publish_dump_tlv(blockInfo, recordLength);
    }
};

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline void asc_dump_super_tensor(U src, __gm__ DebugBlockHeadInfo* blockInfo, uint32_t desc,
    uint64_t tensorLength, const uint32_t* shape, uint32_t shapeDim)
{
    if (tensorLength == 0 || blockInfo == nullptr || !is_super_tensor_position_supported<hardware>() ||
        get_dump_datatype<T>() == DumpTensorDataType::ACL_MAX || shapeDim > K_MAX_SHAPE_DIM ||
        (shapeDim != 0 && shape == nullptr)) {
        return;
    }
    for (uint32_t i = 0; i < shapeDim; ++i) {
        if (shape[i] == 0) {
            return;
        }
    }
    if constexpr (hardware == AscendC::Hardware::GM) {
        const uintptr_t address = reinterpret_cast<uintptr_t>(src);
        if (address == 0 || tensorLength > UINTPTR_MAX - address) {
            return;
        }
    }
    SharedFifoTensorTransport<hardware, T, U> transport{src, blockInfo};
    dump_super_tensor(transport, tensorLength, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(src)),
        static_cast<uint32_t>(get_dump_datatype<T>()), static_cast<uint16_t>(hardware),
        static_cast<uint16_t>(asc_debug_get_block_idx()), desc, shape, shapeDim);
}
#endif

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline void asc_dump_impl(
    U src, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    __gm__ DebugBlockHeadInfo* blockInfo = get_block_info();
    if (dump_size <= 0 || blockInfo == nullptr) {
        return;
    }
    constexpr uint16_t dataBlockSize = 32;
    const uint64_t tensorLength = static_cast<uint64_t>(dump_size) * sizeof(T);
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102
    if (has_super_tensor_protocol() && is_super_tensor_position_supported<hardware>()) {
        asc_dump_super_tensor<hardware, T>(src, blockInfo, desc, tensorLength, shape, shapeDim);
        return;
    }
#endif
    if (tensorLength > UINT32_MAX - (dataBlockSize - 1) ||
        tensorLength + dataBlockSize - 1 > UINT32_MAX - sizeof(DumpTensorTlv)) {
        return;
    }
    uint32_t alignDumpLen = align_up(static_cast<uint32_t>(tensorLength), dataBlockSize);
    uint32_t tlvLen = sizeof(DumpTensorTlv) + alignDumpLen;
    if (!check_ringbuf_space(blockInfo, tlvLen)) {
        return;
    }
    __gm__ DumpTensorTlv* dumpTlv = reinterpret_cast<__gm__ DumpTensorTlv*>(get_ringbuf_tlv_addr(blockInfo));

    set_dump_tlv_info<hardware, T>(src, dumpTlv, alignDumpLen, desc, dump_size);
    set_dump_shape_info(dumpTlv, shapeDim, shape);
    if (set_dump_tlv_data<hardware, T>(src, dumpTlv, alignDumpLen, dump_size) != 0) {
        return;
    }

    __gm__ DebugBlockWriteInfo* writeInfo = get_block_write_info(blockInfo);
    update_write_info(writeInfo, tlvLen);
#endif
}

__aicore__ inline void asc_dump_shape_impl(const uint32_t shapeDim, const uint32_t* shape)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    __gm__ DebugBlockHeadInfo* blockInfo = get_block_info();
    if (blockInfo == nullptr) {
        return;
    }
    uint32_t tlvLen = sizeof(DumpShapeTlv);
    if (!check_ringbuf_space(blockInfo, tlvLen)) {
        return;
    }
    __gm__ DumpShapeTlv* shapeTlv = reinterpret_cast<__gm__ DumpShapeTlv*>(get_ringbuf_tlv_addr(blockInfo));
    shapeTlv->type = static_cast<uint32_t>(DumpType::DUMP_SHAPE);
    shapeTlv->length = tlvLen - sizeof(uint32_t[2]);
    shapeTlv->dim = shapeDim;
    for (uint32_t i = 0; i < 8; ++i) {
        shapeTlv->shape[i] = i < shapeDim ? shape[i] : 1;
    }
    shapeTlv->resv = static_cast<uint32_t>(0U);
    asc_entire_dcci(reinterpret_cast<__gm__ uint64_t*>(shapeTlv));

    __gm__ DebugBlockWriteInfo* writeInfo = get_block_write_info(blockInfo);
    update_write_info(writeInfo, tlvLen);
#endif
}

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline void asc_dump_local_impl(
    U input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    enable_asc_diagnostics();
    if (g_sysPrintFifoSpace != nullptr) {
        asc_dump_impl<hardware, T>(input, desc, dump_size, shape, shapeDim);
    }
    set_ctrl(ctrlValue);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_gm(
    __gm__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    asc_dump_local_impl<AscendC::Hardware::GM, T>(input, desc, dump_size, shape, shapeDim);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_ubuf(
    __ubuf__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    asc_dump_local_impl<AscendC::Hardware::UB, T>(input, desc, dump_size, shape, shapeDim);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_abuf(
    __ca__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    asc_dump_local_impl<AscendC::Hardware::L0A, T>(input, desc, dump_size, shape, shapeDim);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_bbuf(
    __cb__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    asc_dump_local_impl<AscendC::Hardware::L0B, T>(input, desc, dump_size, shape, shapeDim);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_cbuf(
    __cc__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    asc_dump_local_impl<AscendC::Hardware::L0C, T>(input, desc, dump_size, shape, shapeDim);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_l1buf(
    __cbuf__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    asc_dump_local_impl<AscendC::Hardware::L1, T>(input, desc, dump_size, shape, shapeDim);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump(
    __biasbuf__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    asc_dump_local_impl<AscendC::Hardware::BIAS, T>(input, desc, dump_size, shape, shapeDim);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump(
    __fbuf__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    asc_dump_local_impl<AscendC::Hardware::FIXBUF, T>(input, desc, dump_size, shape, shapeDim);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_gm(__gm__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump_local_impl<AscendC::Hardware::GM, T>(input, desc, dump_size, nullptr, 0);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_ubuf(__ubuf__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump_local_impl<AscendC::Hardware::UB, T>(input, desc, dump_size, nullptr, 0);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_abuf(__ca__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump_local_impl<AscendC::Hardware::L0A, T>(input, desc, dump_size, nullptr, 0);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_bbuf(__cb__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump_local_impl<AscendC::Hardware::L0B, T>(input, desc, dump_size, nullptr, 0);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_cbuf(__cc__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump_local_impl<AscendC::Hardware::L0C, T>(input, desc, dump_size, nullptr, 0);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_l1buf(__cbuf__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump_local_impl<AscendC::Hardware::L1, T>(input, desc, dump_size, nullptr, 0);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump(__gm__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump_gm(input, desc, dump_size);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump(__ubuf__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump_ubuf(input, desc, dump_size);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump(__cc__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump_cbuf(input, desc, dump_size);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump(__cbuf__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump_l1buf(input, desc, dump_size);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump(__biasbuf__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump(input, desc, dump_size, nullptr, 0);
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump(__fbuf__ T* input, uint32_t desc, uint32_t dump_size)
{
    asc_dump(input, desc, dump_size, nullptr, 0);
}
} // namespace __asc_aicore
#else
#include <cstdio>

namespace __asc_aicore {
template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_gm(__gm__ T* input, uint32_t desc, uint32_t dump_size)
{
    assert(false && "asc_dump_gm is not supported in cpu mode.");
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_ubuf(__ubuf__ T* input, uint32_t desc, uint32_t dump_size)
{
    assert(false && "asc_dump_ubuf is not supported in cpu mode.");
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_cbuf(__cc__ T* input, uint32_t desc, uint32_t dump_size)
{
    assert(false && "asc_dump_cbuf is not supported in cpu mode.");
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_l1buf(__cbuf__ T* input, uint32_t desc, uint32_t dump_size)
{
    assert(false && "asc_dump_l1buf is not supported in cpu mode.");
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_gm(
    __gm__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    assert(false && "asc_dump_gm is not supported in cpu mode.");
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_ubuf(
    __ubuf__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    assert(false && "asc_dump_ubuf is not supported in cpu mode.");
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_cbuf(
    __cc__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    assert(false && "asc_dump_cbuf is not supported in cpu mode.");
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump_l1buf(
    __cbuf__ T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    assert(false && "asc_dump_l1buf is not supported in cpu mode.");
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump(T* input, uint32_t desc, uint32_t dump_size)
{
    assert(false && "asc_dump is not supported in cpu mode.");
}

template <typename T>
__aicore__ static __attribute__((noinline)) void asc_dump(
    T* input, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
    assert(false && "asc_dump is not supported in cpu mode.");
}

__aicore__ inline void asc_dump_shape_impl(const uint32_t shapeDim, const uint32_t* shape)
{
    assert(false && "asc_dump_shape_impl is not supported in cpu mode.");
}
} // namespace __asc_aicore

using namespace __asc_aicore;
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_IMPL__
#endif

#endif // IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_IMPL_H
