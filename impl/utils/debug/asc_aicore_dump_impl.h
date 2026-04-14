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
#endif

using vector_uint8_t = vector_u8;

namespace __asc_aicore {

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline void set_dump_tlv_info(U src, __gm__ DumpTensorTlv* dumpTlv, uint32_t alignDumpLen, uint32_t desc, uint32_t dumpSize)
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
    dumpTlv->dumpSize = dumpSize * sizeof(T);
    asc_entire_dcci(reinterpret_cast<__gm__ uint64_t*>(dumpTlv));
}

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline void set_dump_tlv_data(U src, __gm__ DumpTensorTlv* dumpTlv, uint32_t alignDumpLen, uint32_t dumpSize)
{
    __gm__ T* dumpDstAddr = reinterpret_cast<__gm__ T*>(dumpTlv + 1);
    
    if (dumpDstAddr == nullptr) {
        return;
    }
    if (hardware == AscendC::Hardware::GM && src == nullptr) {
        return;
    }
    
    sync_all();
    uint32_t dumpLen = 0;
    if constexpr (hardware == AscendC::Hardware::GM) {
        dumpLen = dumpSize * sizeof(T);
        mem_copy_gm_to_gm(reinterpret_cast<__gm__ uint8_t*>(dumpDstAddr), reinterpret_cast<__gm__ const uint8_t*>(src), dumpLen);
    } else if constexpr (hardware == AscendC::Hardware::UB) {
        dumpLen = alignDumpLen / ASC_ONE_DATABLOCK_SIZE;
        mem_copy_ub_to_gm_impl(dumpDstAddr, src, static_cast<uint16_t>(dumpLen));
    } else if constexpr (hardware == AscendC::Hardware::L1) {
        mem_copy_l1buf_to_gm_impl(dumpDstAddr, src, alignDumpLen);
    } else if constexpr (hardware == AscendC::Hardware::L0C) {
        mem_copy_cbuf_to_gm_impl(dumpDstAddr, src, alignDumpLen);
    }
    sync_all();
}

template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline void asc_dump_impl(U src, uint32_t desc, uint32_t dumpSize)
{
    __gm__ DebugBlockHeadInfo* blockInfo = get_block_info();
    if (dumpSize <= 0 || blockInfo == nullptr) {
        return;
    }
    constexpr uint16_t dataBlockSize = 32;
    uint32_t alignDumpLen = align_up(dumpSize * sizeof(T), dataBlockSize);
    uint32_t tlvLen = sizeof(DumpTensorTlv) + alignDumpLen;
    if (!check_ringbuf_space(blockInfo, tlvLen)) {
        return;
    }
    __gm__ DumpTensorTlv* dumpTlv = reinterpret_cast<__gm__ DumpTensorTlv*>(get_ringbuf_tlv_addr(blockInfo));

    set_dump_tlv_info<hardware, T>(src, dumpTlv, alignDumpLen, desc, dumpSize);
    set_dump_tlv_data<hardware, T>(src, dumpTlv, alignDumpLen, dumpSize);

    __gm__ DebugBlockWriteInfo* writeInfo = get_block_write_info(blockInfo);
    update_write_info(writeInfo, tlvLen);
}

template<typename T>
__aicore__ inline void asc_dump_gm(__gm__ T* input, uint32_t desc, uint32_t dumpSize)
{
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    enable_asc_diagnostics();
    if (g_sysPrintFifoSpace != nullptr) {
        asc_dump_impl<AscendC::Hardware::GM, T>(input, desc, dumpSize);
    }
    set_ctrl(ctrlValue);
}

template<typename T>
__aicore__ inline void asc_dump_ubuf(__ubuf__ T* input, uint32_t desc, uint32_t dumpSize)
{
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    enable_asc_diagnostics();
    if (g_sysPrintFifoSpace != nullptr) {
        asc_dump_impl<AscendC::Hardware::UB, T>(input, desc, dumpSize);
    }
    set_ctrl(ctrlValue);
}

template<typename T>
__aicore__ inline void asc_dump_cbuf(__cc__ T* input, uint32_t desc, uint32_t dumpSize)
{
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    enable_asc_diagnostics();
    if (g_sysPrintFifoSpace != nullptr) {
        asc_dump_impl<AscendC::Hardware::L0C, T>(input, desc, dumpSize);
    }
    set_ctrl(ctrlValue);
}

template<typename T>
__aicore__ inline void asc_dump_l1buf(__cbuf__ T* input, uint32_t desc, uint32_t dumpSize)
{
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    enable_asc_diagnostics();
    if (g_sysPrintFifoSpace != nullptr) {
        asc_dump_impl<AscendC::Hardware::L1, T>(input, desc, dumpSize);
    }
    set_ctrl(ctrlValue);
}
} // namespace __asc_aicore
#else
#include "kernel_log.h"

namespace __asc_aicore {
template<typename T>
__aicore__ inline void asc_dump_gm(__gm__ T* input, uint32_t desc, uint32_t dumpSize) {
    ASCENDC_ASSERT((false), "asc_dump_gm is not supported in cpu mode.");
}

template<typename T>
__aicore__ inline void asc_dump_ubuf(__ubuf__ T* input, uint32_t desc, uint32_t dumpSize) {
    ASCENDC_ASSERT((false), "asc_dump_ubuf is not supported in cpu mode.");
}

template<typename T>
__aicore__ inline void asc_dump_cbuf(__cc__ T* input, uint32_t desc, uint32_t dumpSize) {
    ASCENDC_ASSERT((false), "asc_dump_cbuf is not supported in cpu mode.");
}

template<typename T>
__aicore__ inline void asc_dump_l1buf(__cbuf__ T* input, uint32_t desc, uint32_t dumpSize) {
    ASCENDC_ASSERT((false), "asc_dump_l1buf is not supported in cpu mode.");
}
} // namespace __asc_aicore

using namespace __asc_aicore;
#endif

namespace __asc_simd_vf {
// 辅助函数：在 UBuf 的 buffer 中构造 DumpTensorTlv 头部
template <AscendC::Hardware hardware, typename T, typename U>
__simd_callee__ inline void set_dump_tlv_info_ubuf(U src, __ubuf__ DumpTensorTlv* dumpTlv,
    uint32_t alignDumpLen, uint32_t desc, uint32_t dumpSize)
{
    dumpTlv->type = static_cast<uint32_t>(DumpType::DUMP_TENSOR);
    dumpTlv->length = sizeof(DumpTensorTlv) - sizeof(uint32_t[2]) + alignDumpLen;
    dumpTlv->tensorAddr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(src));
    dumpTlv->dataType = static_cast<uint32_t>(get_dump_datatype<T>());
    dumpTlv->desc = desc;
    dumpTlv->blockIdx = static_cast<uint16_t>(0U);  // set in aicore
    dumpTlv->bufferId = static_cast<uint32_t>(0U);
    dumpTlv->position = static_cast<uint16_t>(hardware);
    dumpTlv->dim = static_cast<uint32_t>(0U);
    for (uint32_t i = 0; i < 8; ++i) {
        dumpTlv->shape[i] = static_cast<uint32_t>(0U);
    }
    dumpTlv->resv1 = static_cast<uint32_t>(0U);
    dumpTlv->dumpSize = dumpSize * sizeof(T);
}

// 辅助函数：将源数据填入 buffer 中 DumpTensorTlv 之后的 dumpData 区域
template <AscendC::Hardware hardware, typename T, typename U>
__simd_callee__ inline void set_dump_tlv_data_ubuf(U src, __ubuf__ DumpTensorTlv* dumpTlv,
    uint32_t alignDumpLen, uint32_t dumpSize)
{
    __ubuf__ T* dumpDstAddr = reinterpret_cast<__ubuf__ T*>(dumpTlv + 1);

    // 使用scalar方式进行搬运，暂不考虑搬运性能
    for (uint32_t i = 0; i < dumpSize; i++) {
        dumpDstAddr[i] = src[i];
    }
}

template <AscendC::Hardware hardware, typename T, typename U>
__simd_callee__ inline void asc_dump_impl(U src, uint32_t desc, uint32_t dumpSize)
{
    __ubuf__ BlockVFBufInfo *blockInfo = get_printf_ubuf_addr(0);
    
    constexpr uint16_t dataBlockSize = 32;
    uint32_t alignDumpLen = align_up(dumpSize * sizeof(T), dataBlockSize);
    uint32_t tlvLen = sizeof(DumpTensorTlv) + alignDumpLen;

    blockInfo->magic = 0xF0A00B0F;
    blockInfo->writeLen = tlvLen;

    __ubuf__ DumpTensorTlv* dumpTlv = (__ubuf__ DumpTensorTlv*)blockInfo->buffer;
    set_dump_tlv_info_ubuf<hardware, T>(src, dumpTlv, alignDumpLen, desc, dumpSize);
    set_dump_tlv_data_ubuf<hardware, T>(src, dumpTlv, alignDumpLen, dumpSize);
}

template <typename T>
__simd_callee__ inline void asc_dump_reg(vector_uint8_t *input, uint32_t desc, uint32_t dumpSize) {
    enable_asc_diagnostics();

    __ubuf__ BlockVFBufInfo *blockInfo = get_printf_ubuf_addr(0);
    constexpr uint16_t dataBlockSize = 32;
    uint32_t alignDumpLen = align_up(dumpSize * sizeof(T), dataBlockSize);
    uint32_t tlvLen = sizeof(DumpTensorTlv) + alignDumpLen;

    // 检查 buffer 容量
    uint32_t bufferCapacity = blockInfo->length * sizeof(uint16_t);
    if (tlvLen > bufferCapacity) {
        return;
    }

    // // Step 1: 构造头部
    // __ubuf__ uint8_t* tlvBase = reinterpret_cast<__ubuf__ uint8_t*>(blockInfo->buffer);
    // set_dump_tlv_info_ubuf<AscendC::Hardware::UB, T>(input, tlvBase, alignDumpLen, desc, dumpSize);

    // // Step 2: 用 asc_storealign 将 vector 寄存器数据存入 buffer 的 dumpData 区域
    // __ubuf__ uint8_t* dumpDataAddr = tlvBase + sizeof(DumpTensorTlv);
    // asc_storealign(dumpDataAddr, *input, static_cast<vector_bool>(true));

    // // Step 3: 设置 writeLen 并搬运到 GM
    // blockInfo->writeLen = tlvLen;
    // asc_vf_printf_ub2gm();
}

// 内存数据dump
template <typename T>
__simd_callee__ inline void asc_dump_ubuf(__ubuf__ T* input, uint32_t desc, uint32_t dumpSize) {
    // 如下指令无法使用，且在simd_callee中不涉及：get_ctrl()，set_atomic_none(), set_ctrl()
    enable_asc_diagnostics();
    asc_dump_impl<AscendC::Hardware::UB, T>(input, desc, dumpSize);
}
} // namespace __asc_simd_vf

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_IMPL__
#endif

#endif // IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_IMPL_H