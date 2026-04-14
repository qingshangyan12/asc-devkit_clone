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
 * \file asc_debug_utils.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_DEBUG_UTILS_H
#define IMPL_UTILS_DEBUG_ASC_DEBUG_UTILS_H

inline __gm__ uint8_t* __gm__ g_sysPrintFifoSpace = nullptr;

#ifndef ASCENDC_CPU_DEBUG
#include <type_traits>

#include "impl/utils/sys_macros.h"
#include "impl/utils/debug/asc_utils_types.h"
#include "impl/utils/debug/asc_debug_types.h"

#if __NPU_ARCH__ == 2002
#include "impl/utils/debug/npu_arch_2002/asc_debug_utils_impl.h"
#elif __NPU_ARCH__ == 2201
#include "impl/utils/debug/npu_arch_2201/asc_debug_utils_impl.h"
#elif __NPU_ARCH__ == 3510
#include "impl/utils/debug/npu_arch_3510/asc_debug_utils_impl.h"
#endif

namespace __asc_simd_vf {
template <typename T, typename U, typename... Args>
__simd_callee__ constexpr bool is_same_in_list()
{
    if constexpr (sizeof...(Args) > 0) {
        return std::is_same<T, U>::value || is_same_in_list<T, Args...>();
    }
    return std::is_same<T, U>::value;
}

__simd_callee__ inline uint32_t get_cstring_len_simd(__ubuf__ const char* s)
{
    uint32_t i = 0;
    while (*(s + i) != '\0') {
        i++;
    }
    return i + 1;
}

__simd_callee__ constexpr inline uint32_t div_ceil(uint32_t a, uint32_t b)
{
    if (b == 0) {
        return 0;
    }
    return (a + b - 1) / b;
}

__simd_callee__ constexpr inline uint32_t align_up(uint32_t a, uint32_t b)
{
    return div_ceil(a, b) * b;
}

template <typename T>
__simd_callee__ constexpr inline DumpTensorDataType get_dump_datatype_impl();

template <typename T>
__simd_callee__ constexpr inline DumpTensorDataType get_dump_datatype()
{
    return get_dump_datatype_impl<T>();
}

__simd_callee__ inline void enable_asc_diagnostics()
{
#if defined(__ENABLE_ASCENDC_PRINTF__)
#if (!defined(ASCENDC_DUMP) || (ASCENDC_DUMP != 0)) || defined(ASCENDC_TIME_STAMP_ON)
    static const struct AscTlv __asc_debug_meta_section__ __attribute__ ((used, section (".ascend.meta"))) =
    {4, 4, 1};
#endif // defined(ASCENDC_DUMP) || defined(ASCENDC_TIME_STAMP_ON)
#endif // __ENABLE_ASCENDC_PRINTF__
}

__simd_callee__ __ubuf__ inline BlockVFBufInfo* get_printf_ubuf_addr(uint64_t addr)
{
    constexpr uint32_t safeStaticLen = 32;
    static __ubuf__ uint64_t info[safeStaticLen];
    if (addr != 0) {
        info[0] = addr;
        ((__ubuf__BlockVFBufInfo*)addr)->writeLen = 0;
        ((__ubuf__BlockVFBufInfo*)addr)->pidx = 0;
    }
    return (__ubuf__ BlockVFBufInfo*)(info[0]);
}

__simd_callee__ inline void asc_copy_ub2gm_align(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    __cce_scalar::copy_ubuf_to_gm_align_v2(dst, src, 1, size, 0, 0, 0, 0);
}

__simd_callee__ inline void asc_copy_ub2ub(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    __cce_scalar::copy_ubuf_to_ubuf(dst, src, 1, size / 32, 0, 0);
}
} // namespace __asc_simd_vf

namespace __asc_aicore {
__aicore__ inline void asc_copy_ub2gm_align(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    copy_ubuf_to_gm_align_v2(dst, src, 1, size, 0, 0, 0, 0);
}

__aicore__ inline void enable_asc_diagnostics()
{
#if (!defined(ASCENDC_DUMP) || (ASCENDC_DUMP != 0)) || defined(ASCENDC_TIME_STAMP_ON)
    static const struct AscTlv __asc_debug_meta_section__ __attribute__ ((used, section (".ascend.meta"))) =
    {4, 4, 1};
#endif // defined(ASCENDC_DUMP) || defined(ASCENDC_TIME_STAMP_ON)
}

__aicore__ inline void enable_asc_assert()
{
#if (!defined(ASCENDC_DUMP) || (ASCENDC_DUMP != 0)) || defined(ASCENDC_TIME_STAMP_ON)
    static const struct AscTlv __asc_assert_meta_section__ __attribute__ ((used, section (".ascend.meta"))) =
    {4, 4, 5};
#endif // defined(ASCENDC_DUMP) || defined(ASCENDC_TIME_STAMP_ON)
}

template <typename T, typename U, typename... Args>
__aicore__ constexpr bool is_same_in_list()
{
    if constexpr (sizeof...(Args) > 0) {
        return std::is_same<T, U>::value || is_same_in_list<T, Args...>();
    }
    return std::is_same<T, U>::value;
}

__aicore__ inline void asc_entire_dcci(__gm__ uint64_t* ptr)
{
    asc_entire_dcci_impl(ptr);
}

__aicore__ inline uint64_t asc_debug_get_system_cycle()
{
    return asc_debug_get_system_cycle_impl();
}

__aicore__ inline uint32_t asc_debug_get_core_idx()
{
    return asc_debug_get_core_idx_impl();
}

__aicore__ inline uint64_t asc_debug_get_block_idx()
{
    return asc_debug_get_block_idx_impl();
}

__aicore__ inline uint64_t asc_debug_get_block_num()
{
    return get_block_num();
}

__aicore__ inline uint64_t asc_debug_get_block_total_num()
{
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510)
    return asc_debug_get_block_num() * get_task_ration();
#else
    return asc_debug_get_block_num();
#endif
}

__aicore__ inline int64_t asc_debug_get_program_counter()
{
    return get_pc();
}

template <uint64_t timeoutCycle = 15 * 1000 * 1000> // 20ms * 15
__aicore__ inline void ringbuf_wait_rts_sync()
{
    return ringbuf_wait_rts_sync_impl<timeoutCycle>();
}

__aicore__ inline uint32_t get_cstring_len(__gm__ const char* s)
{
    uint32_t i = 0;
    while (*(s + i) != '\0') {
        i++;
    }
    return i + 1;
}

__aicore__ constexpr inline uint32_t div_ceil(uint32_t a, uint32_t b)
{
    if (b == 0) {
        return 0;
    }
    return (a + b - 1) / b;
}

__aicore__ constexpr inline uint32_t align_up(uint32_t a, uint32_t b)
{
    return div_ceil(a, b) * b;
}

__aicore__ inline void sync_all()
{
    sync_all_impl();
}

__aicore__ inline void mem_copy_gm_to_gm(__gm__ uint8_t* dst, __gm__ const uint8_t* src, const uint32_t& len)
{
    if (dst == nullptr || src == nullptr)
    {
        return;
    }
    for (uint32_t i = 0; i < len; i++) {
        *(dst + i) = *(src + i);
    }
    asc_entire_dcci((__gm__ uint64_t*)dst);
}

__aicore__ __gm__ inline DebugBlockHeadInfo* get_block_info()
{
    uint32_t blockIdx = asc_debug_get_core_idx();
    uint32_t blockLength = reinterpret_cast<__gm__ DebugBlockHeadInfo*>(g_sysPrintFifoSpace)->length;
    __gm__ DebugBlockHeadInfo* blockInfo =
        reinterpret_cast<__gm__ DebugBlockHeadInfo*>(g_sysPrintFifoSpace + blockLength * blockIdx);
    return blockInfo->magic == 0xAE86 ? blockInfo : nullptr;
}

__aicore__ __gm__ inline DebugBlockReadInfo* get_block_read_info(__gm__ DebugBlockHeadInfo* blockInfo)
{
    __gm__ uint8_t* blockHeadAddr = reinterpret_cast<__gm__ uint8_t*>(blockInfo);

    return reinterpret_cast<__gm__ DebugBlockReadInfo*>(blockHeadAddr + sizeof(DebugBlockHeadInfo));
}

__aicore__ __gm__ inline DebugBlockWriteInfo* get_block_write_info(__gm__ DebugBlockHeadInfo* blockInfo)
{
    __gm__ uint8_t* ringBufAddr = reinterpret_cast<__gm__ uint8_t*>(blockInfo->ringBufAddr);

    return reinterpret_cast<__gm__ DebugBlockWriteInfo*>(ringBufAddr + blockInfo->ringBufLen);
}

__aicore__ __gm__ inline uint8_t* get_ringbuf_tlv_addr(__gm__ DebugBlockHeadInfo* blockInfo)
{
    __gm__ DebugBlockWriteInfo* writeInfo = get_block_write_info(blockInfo);
    __gm__ uint8_t* ringBufAddr = reinterpret_cast<__gm__ uint8_t*>(blockInfo->ringBufAddr);
    return ringBufAddr + writeInfo->bufOffset;
}

__aicore__ inline void update_write_info(__gm__ DebugBlockWriteInfo* writeInfo, const uint32_t& tlvLen)
{
    writeInfo->bufOffset += tlvLen;
    writeInfo->packIdx += 1;
    asc_entire_dcci((__gm__ uint64_t*)writeInfo);
}

__aicore__ inline bool ringbuf_read_wait(__gm__ DebugBlockReadInfo* readInfo)
{
    constexpr uint32_t maxCounter = 15;
    uint32_t counter = 0;
    while (readInfo->bufOffset == 0) {
        if (counter >= maxCounter) { // max wait 15 * 300ms, rts read gm per 200ms
            return false;
        }
        ringbuf_wait_rts_sync(); // wait 20 * 15 ms
        ++counter;
        asc_entire_dcci((__gm__ uint64_t*)readInfo);
    }
    return true;
}

__aicore__ inline void ringbuf_skip_directly(__gm__ DebugBlockWriteInfo* writeInfo)
{
    writeInfo->bufOffset = 0;
    asc_entire_dcci((__gm__ uint64_t*)writeInfo);
}

__aicore__ inline void ringbuf_skip_with_info(
    __gm__ DebugBlockWriteInfo* writeInfo, __gm__ uint8_t* ringBufAddr, const uint32_t& ringBufLen)
{
    __gm__ SkipTlv* skipInfo = reinterpret_cast<__gm__ SkipTlv*>(ringBufAddr + writeInfo->bufOffset);
    skipInfo->type = static_cast<uint32_t>(DumpType::DUMP_SKIP);
    skipInfo->length = ringBufLen - writeInfo->bufOffset - sizeof(SkipTlv);
    writeInfo->bufOffset = 0;
    writeInfo->packIdx += 1;
    asc_entire_dcci((__gm__ uint64_t*)skipInfo);
    asc_entire_dcci((__gm__ uint64_t*)writeInfo);
    return;
}

__aicore__ inline bool ringbuf_overflow_wait(__gm__ DebugBlockReadInfo* readInfo, __gm__ DebugBlockWriteInfo* writeInfo,
                                             const uint32_t& tlvLen)
{
    constexpr uint32_t maxCounter = 15;
    uint32_t counter = 0;
    while (writeInfo->bufOffset < readInfo->bufOffset && writeInfo->bufOffset + tlvLen >= readInfo->bufOffset) {
        if (counter >= maxCounter) { // max wait 15 * 300ms, rts read gm per 200ms
            return false;
        }
        ringbuf_wait_rts_sync(); // wait 20 * 15 ms
        ++counter;
        asc_entire_dcci((__gm__ uint64_t*)readInfo);
    }
    return true;
}

__aicore__ inline bool check_ringbuf_space(__gm__ DebugBlockHeadInfo* blockInfo, const uint32_t& tlvLen)
{
    constexpr uint32_t minTlvLen = sizeof(SkipTlv);

    __gm__ uint8_t* ringBufAddr = reinterpret_cast<__gm__ uint8_t*>(blockInfo->ringBufAddr);
    uint32_t ringBufLen = blockInfo->ringBufLen;

    __gm__ DebugBlockReadInfo* readInfo = get_block_read_info(blockInfo);
    __gm__ DebugBlockWriteInfo* writeInfo = get_block_write_info(blockInfo);

    if (minTlvLen >= ringBufLen || tlvLen > ringBufLen) {
        return false;
    } else if (writeInfo->bufOffset + minTlvLen >= ringBufLen) {
        if (!ringbuf_read_wait(readInfo)) { // check read is begin
            return false;
        }
        ringbuf_skip_directly(writeInfo);
    } else if (writeInfo->bufOffset + tlvLen > ringBufLen) {
        if (!ringbuf_read_wait(readInfo)) { // check read is begin
            return false;
        }
        ringbuf_skip_with_info(writeInfo, ringBufAddr, ringBufLen);
    }
    if (writeInfo->packIdx > 0 &&
        writeInfo->bufOffset < readInfo->bufOffset &&
        writeInfo->bufOffset + tlvLen >= readInfo->bufOffset) {
        return ringbuf_overflow_wait(readInfo, writeInfo, tlvLen);
    }
    return true;
}

template <typename T>
__aicore__ constexpr inline DumpTensorDataType get_dump_datatype_impl();

template <typename T>
__aicore__ constexpr inline DumpTensorDataType get_dump_datatype()
{
    return get_dump_datatype_impl<T>();
}

__aicore__ inline void asc_debug_get_cann_vserion(__gm__ char*& versionStr, uint64_t& version, uint64_t& timeStamp)
{
#ifdef ASC_DEVKIT_VERSION_STR
    versionStr = const_cast<__gm__ char*>(ASC_DEVKIT_VERSION_STR);
#else
    versionStr = const_cast<__gm__ char*>("Unknown ASC_DEVKIT version");
#endif

#ifdef ASC_DEVKIT_TIMESTAMP
    timeStamp = static_cast<uint64_t>(ASC_DEVKIT_TIMESTAMP);
#else
    timeStamp = 0;
#endif

#ifdef ASC_DEVKIT_VERSION
    version = static_cast<uint64_t>(ASC_DEVKIT_VERSION);
#else
    version = 0;
#endif
}

__aicore__ __ubuf__ inline BlockVFBufInfo* get_printf_ubuf_addr_aicore(uint64_t addr)
{
    return __asc_simd_vf::get_printf_ubuf_addr(addr);
}

__aicore__ __gm__ inline BlockRingBufInfo* get_block_ring_buf_info()
{
    uint32_t blockIdx = asc_debug_get_core_idx();
    uint32_t blockLength = reinterpret_cast<__gm__ BlockRingBufInfo*>(g_sysPrintFifoSpace)->length;
    __gm__ BlockRingBufInfo* blockInfo =
        reinterpret_cast<__gm__ BlockRingBufInfo*>(g_sysPrintFifoSpace + blockLength * blockIdx);
    // magic check to be considered
    return blockInfo;
}

__aicore__ inline __gm__ RingBufReadInfo* get_ring_buf_read_info(
    __gm__ BlockRingBufInfo* block_ring_buf_info)
{
    __gm__ uint8_t* block_head = reinterpret_cast<__gm__ uint8_t*>(block_ring_buf_info);
    return reinterpret_cast<__gm__ RingBufReadInfo*>(block_head + sizeof(BlockRingBufInfo));
}

__aicore__ inline __gm__ RingBufWriteInfo* get_ring_buf_write_info(
    __gm__ BlockRingBufInfo* block_ring_buf_info)
{
    __gm__ uint8_t* ring_buf_addr = reinterpret_cast<__gm__ uint8_t*>(block_ring_buf_info->ringBufAddr);
    return reinterpret_cast<__gm__ RingBufWriteInfo*>(ring_buf_addr + block_ring_buf_info->ringBufLen);
}

__aicore__ __gm__ inline uint8_t* call_get_ring_buf_tlv(__gm__ BlockRingBufInfo* block_ring_buf_info)
{
    __gm__ RingBufWriteInfo* writeInfo = get_ring_buf_write_info(block_ring_buf_info);
    __gm__ uint8_t* ringBufAddr = reinterpret_cast<__gm__ uint8_t*>(block_ring_buf_info->ringBufAddr);
    return ringBufAddr + writeInfo->bufOffset;
}

__aicore__ inline void update_write_info(__gm__ RingBufWriteInfo* writeInfo, const uint32_t& tlvLen)
{
    writeInfo->bufOffset += tlvLen;
    writeInfo->packIdx += 1;
    __asc_aicore::asc_entire_dcci((__gm__ uint64_t*)writeInfo);
}

__aicore__ inline void asc_vf_debug_ub2gm()
{
    __ubuf__ BlockVFBufInfo *blockInfo = get_printf_ubuf_addr_aicore(0);
    __ubuf__ uint8_t *tlv = (__ubuf__ uint8_t*)blockInfo->buffer;
    
    __gm__ BlockRingBufInfo *block_ring_buf_info = get_block_ring_buf_info();
    __gm__ uint8_t *dstTlv = reinterpret_cast<__gm__ uint8_t*>(call_get_ring_buf_tlv(block_ring_buf_info));
    const uint32_t tlvLen = blockInfo->writeLen;

    // asc_copy_ub2gm_align(dstTlv, blockInfo->buffer, tlvLen);
    sync_all();
    constexpr uint32_t sizeU32 = 4;
    for (uint32_t i = 0; i < tlvLen / sizeU32; ++i) {
        ((__gm__ uint32_t*)(dstTlv + 1))[i] = ((__ubuf__ uint32_t*)(tlv + 1))[i];
    }
    sync_all();

    asc_entire_dcci(reinterpret_cast<__gm__ uint64_t*>(dstTlv));

    __gm__ RingBufWriteInfo *write_info = get_ring_buf_write_info(block_ring_buf_info);
    update_write_info(write_info, tlvLen);
}
} // namespace __asc_aicore
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_DEBUG_UTILS__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_DEBUG_UTILS__
#endif

#endif // IMPL_UTILS_DEBUG_ASC_DEBUG_UTILS_H