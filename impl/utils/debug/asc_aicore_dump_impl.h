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

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline uint32_t copy_super_tensor_data(__gm__ uint8_t* dst, U src, uint32_t bytes)
{
    if constexpr (hardware == AscendC::Hardware::GM) {
        return mem_copy_gm_to_gm(dst, reinterpret_cast<__gm__ const uint8_t*>(src), bytes);
    } else if constexpr (hardware == AscendC::Hardware::UB) {
        const uint32_t aligned = bytes / 32 * 32;
        if (aligned != 0 && mem_copy_ub_to_gm_impl(reinterpret_cast<__gm__ T*>(dst), src, aligned / 32) != 0) {
            return 1;
        }
        sync_all();
        // Scalar tail loads avoid reading beyond a byte-sized LocalTensor view.
        auto source = reinterpret_cast<__ubuf__ uint8_t*>(src);
        for (uint32_t i = aligned; i < bytes; ++i) {
            dst[i] = source[i];
        }
        asc_entire_dcci(reinterpret_cast<__gm__ uint64_t*>(dst));
        return 0;
    } else if constexpr (hardware == AscendC::Hardware::L1) {
#if __NPU_ARCH__ == 2201
        // copy_cbuf_to_gm uses 32-byte blocks on this architecture.
        return mem_copy_l1buf_to_gm_impl(reinterpret_cast<__gm__ T*>(dst), src, bytes / 32);
#else
        return mem_copy_l1buf_to_gm_impl(reinterpret_cast<__gm__ T*>(dst), src, bytes);
#endif
    } else if constexpr (hardware == AscendC::Hardware::L0C) {
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 5102
        // These matrix helpers take a count of 32-bit accumulator elements.
        return mem_copy_cbuf_to_gm_impl(reinterpret_cast<__gm__ T*>(dst), src, bytes / sizeof(uint32_t));
#else
        return mem_copy_cbuf_to_gm_impl(reinterpret_cast<__gm__ T*>(dst), src, bytes);
#endif
    } else if constexpr (hardware == AscendC::Hardware::L0A) {
        return mem_copy_abuf_to_gm_impl(reinterpret_cast<__gm__ T*>(dst), src, bytes);
    } else if constexpr (hardware == AscendC::Hardware::L0B) {
        return mem_copy_bbuf_to_gm_impl(reinterpret_cast<__gm__ T*>(dst), src, bytes);
    } else if constexpr (hardware == AscendC::Hardware::BIAS) {
        return mem_copy_biasbuf_to_gm_impl(reinterpret_cast<__gm__ T*>(dst), src, bytes);
    } else if constexpr (hardware == AscendC::Hardware::FIXBUF) {
        return mem_copy_fbuf_to_gm_impl(reinterpret_cast<__gm__ T*>(dst), src, bytes);
    }
    return 1;
}

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline void asc_dump_super_tensor(
    __gm__ DebugBlockHeadInfo* blockInfo, U src, uint32_t desc, uint64_t total, const uint32_t* shape,
    uint32_t shapeDim)
{
    constexpr bool isGm = hardware == AscendC::Hardware::GM;
    constexpr uint32_t copyAlign = hardware == AscendC::Hardware::L0C ? 1024 : 32;
    constexpr uint32_t copies = isGm ? 1 : 2;
    for (uint64_t offset = 0; offset < total;) {
        const uint32_t header = offset == 0 ? sizeof(DumpSuperTensorTlv) : sizeof(DumpSuperTensorBodyTlv);
        // Extra 32 bytes allow absolute alignment of the private DMA destination.
        const uint32_t overhead = align_up(header, 32) + (isGm ? 0 : 32) + 1;
        if (blockInfo->ringBufLen <= overhead) {
            return;
        }
        uint32_t capacity = (blockInfo->ringBufLen - overhead) / copies / copyAlign * copyAlign;
        capacity = capacity > 16384 ? 16384 : capacity;
        if (capacity == 0) {
            return;
        }
        const uint32_t size = total - offset < capacity ? static_cast<uint32_t>(total - offset) : capacity;
        const uint32_t span = align_up(header + size, 32);
        const uint32_t copySize = align_up(size, copyAlign);
        const uint32_t reserve = span + (isGm ? 0 : 32 + copySize);
        auto writeInfo = get_block_write_info(blockInfo);
        if (writeInfo->bufOffset > blockInfo->ringBufLen || reserve > blockInfo->ringBufLen) {
            return;
        }
        if (!check_ringbuf_space(blockInfo, reserve)) {
            return;
        }
        auto record = get_ringbuf_tlv_addr(blockInfo);
        set_super_tensor_header(
            record, total, offset, size, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(src)),
            static_cast<uint32_t>(get_dump_datatype<T>()), desc, static_cast<uint16_t>(hardware),
            static_cast<uint16_t>(asc_debug_get_block_idx()), shape, shapeDim);
        sync_all();
        asc_entire_dcci(reinterpret_cast<__gm__ uint64_t*>(record));
        uint32_t ret;
        if constexpr (isGm) {
            ret = copy_super_tensor_data<hardware, T>(record + header, src + offset / sizeof(T), size);
        } else {
            // Do not DMA into the unaligned wire payload. Scratch is reserved but never published.
            auto scratch =
                reinterpret_cast<__gm__ uint8_t*>((reinterpret_cast<uintptr_t>(record + span) + 31) / 32 * 32);
            ret = copy_super_tensor_data<hardware, T>(
                scratch, src + offset / sizeof(T), hardware == AscendC::Hardware::UB ? size : copySize);
            sync_all();
            if (ret == 0) {
                asc_entire_dcci(reinterpret_cast<__gm__ uint64_t*>(scratch));
                ret = mem_copy_gm_to_gm(record + header, scratch, size);
            }
        }
        if (ret != 0) {
            return;
        }
        sync_all();
        asc_entire_dcci(reinterpret_cast<__gm__ uint64_t*>(record));
        update_write_info(get_block_write_info(blockInfo), span);
        offset += size;
    }
}

// Validate a LocalTensor view before its capacity information is lost at the raw-pointer boundary.
__aicore__ inline bool check_super_tensor_local_range(
    AscendC::Hardware position, uint64_t total, uint64_t available)
{
    auto blockInfo = get_block_info();
    const uint64_t ordinarySpan = sizeof(DumpTensorTlv) + ((total + 31) / 32) * 32;
    if (blockInfo == nullptr || (ordinarySpan <= blockInfo->ringBufLen && ordinarySpan <= UINT32_MAX)) {
        return true;
    }
    const uint32_t sourceAlign =
        position == AscendC::Hardware::L0C ? 1024 : (position == AscendC::Hardware::UB ? 1 : 32);
    const uint64_t required = (total + sourceAlign - 1) / sourceAlign * sourceAlign;
    return required <= available;
}

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline void asc_dump_impl(
    U src, uint32_t desc, uint32_t dump_size, const uint32_t* shape, const uint32_t shapeDim)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    __gm__ DebugBlockHeadInfo* blockInfo = get_block_info();
    if (dump_size <= 0 || blockInfo == nullptr) {
        return;
    }
    const uint64_t total = static_cast<uint64_t>(dump_size) * sizeof(T);
    const uint64_t ordinarySpan = sizeof(DumpTensorTlv) + ((total + 31) / 32) * 32;
    if (ordinarySpan > blockInfo->ringBufLen || ordinarySpan > UINT32_MAX) {
        asc_dump_super_tensor<hardware, T>(blockInfo, src, desc, total, shape, shapeDim);
        return;
    }
    constexpr uint16_t dataBlockSize = 32;
    uint32_t alignDumpLen = align_up(dump_size * sizeof(T), dataBlockSize);
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
