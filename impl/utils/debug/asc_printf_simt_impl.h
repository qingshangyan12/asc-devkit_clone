/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file asc_printf_simt_impl.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_PRINTF_SIMT_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_PRINTF_SIMT_IMPL_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_PRINTF_SIMT_IMPL__
#warning "asc_printf_simt_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "asc_printf.h" and use public functions or variables defined in interface header files."
#endif

#include "kernel_scalar_convert.h"
#include "utils/kernel_utils_dump_constants.h"
#include "utils/kernel_utils_ceil_oom_que.h"

#ifdef ASCENDC_DUMP
namespace {
constexpr uint32_t SIMT_DUMP_BLOCK_NUM = 72;
constexpr uint32_t SIMT_DUMP_SIZE = 2048;
constexpr uint32_t SIMT_MAX_THREAD_NUM = 2048;
}

namespace AscendC {
namespace Simt {
static __gm__ uint8_t* __gm__ g_simtDumpWorkspaceReserved;

__aicore__ inline void SetSimtDumpWorkspace(GM_ADDR workspace)
{
    g_simtDumpWorkspaceReserved = workspace - SIMT_DUMP_BLOCK_NUM * SIMT_MAX_THREAD_NUM * SIMT_DUMP_SIZE;
    dsb(DSB_DDR);
}
}  // namespace Simt
}  // namespace AscendC

namespace __asc_simt_vf {

__simt_callee__ inline uint32_t GetArgsNum()
{
    return 0;
}

template <typename T, typename... Args>
__simt_callee__ inline uint32_t GetArgsNum(T scalar, Args... args)
{
    return 1 + __asc_simt_vf::GetArgsNum(args...);
}

__simt_callee__ inline uint32_t GetStringLength(const __gm__ char *s)
{
    uint32_t i = 0;
    while (*(s + i) != '\0') {
        i++;
    }
    return i + 1;
}

__simt_callee__ inline uint32_t GetArgsSize()
{
    return 0;
}

template <typename... Args>
__simt_callee__ inline uint32_t GetArgsSize(Args &&...args);

template <typename... Args>
__simt_callee__ inline uint32_t GetArgsSizeImpl(const __gm__ char *s, Args &&...args)
{
    uint32_t strLen = __asc_simt_vf::GetStringLength(s);
    uint32_t strParamSize = AscendC::ONE_PARAM_SIZE + strLen;
    return strParamSize + __asc_simt_vf::GetArgsSize(args...);
}

template <typename T, typename... Args>
__simt_callee__ inline uint32_t GetArgsSizeImpl(T scalar, Args &&...args)
{
    return AscendC::ONE_PARAM_SIZE + __asc_simt_vf::GetArgsSize(args...);
}

template <typename... Args>
__simt_callee__ inline uint32_t GetArgsSize(Args &&...args)
{
    return __asc_simt_vf::GetArgsSizeImpl(args...);
}

template <typename... Args>
__simt_callee__ inline uint32_t GetParamSize(const __gm__ char *fmt, Args &&...args)
{
    uint32_t fmtSize = __asc_simt_vf::GetStringLength(fmt);
    uint32_t argsSize = __asc_simt_vf::GetArgsSize(args...);
    return fmtSize + argsSize + AscendC::ONE_PARAM_SIZE;
}

__simt_callee__ inline void WriteTLHead(AscendC::DumpType printType, __gm__ uint8_t *tlv, uint32_t valueSize)
{
    *((__gm__ uint32_t *)tlv) = static_cast<uint32_t>(printType);
    *((__gm__ uint32_t *)tlv + 1) = valueSize;
}

__simt_callee__ inline void WriteString(__gm__ uint8_t *paramAddr, uint32_t paramIdx, const __gm__ char *s, uint32_t &offset)
{
    __gm__ uint64_t *stringAddr = reinterpret_cast<__gm__ uint64_t *>(paramAddr) + paramIdx;
    __gm__ uint64_t *dstStrAddr = reinterpret_cast<__gm__ uint64_t *>(paramAddr + offset);

    // write string value offset
    *((__gm__ uint64_t *)stringAddr) = static_cast<uint64_t>(offset - AscendC::ONE_PARAM_SIZE * paramIdx);

    // write string content
    __gm__ char *d = (__gm__ char *)(dstStrAddr);
    uint32_t strLen = __asc_simt_vf::GetStringLength(s);

    for (uint32_t i = 0; i < strLen; i++) {
        *(d + i) = *(s + i);
    }
    offset += strLen;
}

template <typename T>
__simt_callee__ inline void WriteScalar(__gm__ uint8_t *paramAddr, uint32_t paramIdx, T scalar)
{
    __gm__ uint64_t *scalarAddr = (__gm__ uint64_t *)paramAddr + paramIdx;
    *scalarAddr = 0;

    if constexpr (AscendC::SupportType<T, half, float>()) {
        *((__gm__ float *)scalarAddr) = static_cast<float>(scalar);
    } else if constexpr (AscendC::SupportType<T, double>()) {
        *((__gm__ double *)scalarAddr) = static_cast<double>(scalar);
    } else if constexpr (std::is_signed<T>::value) {
        *((__gm__ int64_t *)scalarAddr) = static_cast<int64_t>(scalar);
    } else if constexpr (std::is_unsigned<T>::value) {
        *((__gm__ uint64_t *)scalarAddr) = static_cast<uint64_t>(scalar);
    } else if constexpr (AscendC::SupportType<T, bfloat16_t, fp8_e5m2_t, AscendC::fp8_e8m0_t, fp8_e4m3fn_t, hifloat8_t>()) {
        *((__gm__ float *)scalarAddr) = AscendC::ToFloat(scalar);
    } else if constexpr (std::is_pointer<T>::value) {
        *((__gm__ uint64_t *)scalarAddr) = (uintptr_t)scalar;
    } else if constexpr (std::is_enum<T>::value) {
        *((__gm__ uint64_t *)scalarAddr) = static_cast<uint64_t>(scalar);
    }
}

__simt_callee__ inline void SetParam(__gm__ uint8_t *paramAddr, uint32_t paramIdx, uint32_t &offset)
{
    return;
}

template <typename... Args>
__simt_callee__ inline void SetParam(__gm__ uint8_t *paramAddr, uint32_t paramIdx, uint32_t &offset, Args &&...args);

template <typename... Args>
__simt_callee__ inline void SetParamImpl(__gm__ uint8_t *paramAddr, uint32_t paramIdx, uint32_t &offset,
                                    const __gm__ char *s, Args &&...args)
{
    __asc_simt_vf::WriteString(paramAddr, paramIdx, s, offset);
    __asc_simt_vf::SetParam(paramAddr, paramIdx + 1, offset, args...);
}

template <typename T, typename... Args>
__simt_callee__ inline void SetParamImpl(__gm__ uint8_t *paramAddr, uint32_t paramIdx, uint32_t &offset, T scalar,
                                    Args &&...args)
{
    __asc_simt_vf::WriteScalar(paramAddr, paramIdx, scalar);
    __asc_simt_vf::SetParam(paramAddr, paramIdx + 1, offset, args...);
}

template <typename... Args>
__simt_callee__ inline void SetParam(__gm__ uint8_t *paramAddr, uint32_t paramIdx, uint32_t &offset, Args &&...args)
{
    __asc_simt_vf::SetParamImpl(paramAddr, paramIdx, offset, args...);
}

__simt_callee__ inline uint32_t GetDumpBlockIdx()
{
    return bisheng::cce::simt::get_block_idx();
}

__simt_callee__ inline __gm__ AscendC::BlockInfo *GetBlockInfo()
{
    uint64_t dumpWorkspaceStart = reinterpret_cast<uint64_t>(AscendC::Simt::g_simtDumpWorkspaceReserved);

    uint32_t blockIdx = __asc_simt_vf::GetDumpBlockIdx();
    uint64_t blockDumpWorkspaceStart =
        dumpWorkspaceStart + blockIdx * SIMT_MAX_THREAD_NUM * SIMT_DUMP_SIZE;

    uint32_t threadId = threadIdx.x * blockDim.y * blockDim.z + threadIdx.y * blockDim.z + threadIdx.z;
    return (__gm__ AscendC::BlockInfo *)(blockDumpWorkspaceStart + threadId * SIMT_DUMP_SIZE);
}

__simt_callee__ inline void UpdateBlockInfo(uint32_t tlvSize)
{
    __gm__ AscendC::BlockInfo *blockInfo = __asc_simt_vf::GetBlockInfo();
    uint32_t remainSize = blockInfo->dumpOffset;
    uint64_t lastDumpAddr = blockInfo->dumpAddr;

    __gm__ uint8_t *blockInfoStart = (__gm__ uint8_t *)blockInfo;
    *((__gm__ uint32_t *)blockInfoStart + AscendC::BLOCK_INFO_DUMPOFFSET_POS) = remainSize - tlvSize;
    *((__gm__ uint64_t *)((__gm__ uint32_t *)blockInfoStart + AscendC::BLOCK_INFO_DUMP_ADDR)) = lastDumpAddr + tlvSize;
}

__simt_callee__ inline void InitDumpForThread()
{
    if (AscendC::Simt::g_simtDumpWorkspaceReserved == nullptr) {
        return;
    }

    uint32_t blockIdx = __asc_simt_vf::GetDumpBlockIdx();
    if (blockIdx >= SIMT_DUMP_BLOCK_NUM) {
        return;
    }

    __gm__ AscendC::BlockInfo *blockInfo = __asc_simt_vf::GetBlockInfo();
    uint32_t blockInfoLen = sizeof(AscendC::BlockInfo) + sizeof(AscendC::SimtDumpMeta);
    uint64_t blockInfoStart = reinterpret_cast<uint64_t>(blockInfo);

    uint32_t threadId = threadIdx.x * blockDim.y * blockDim.z + threadIdx.y * blockDim.z + threadIdx.z;

    *((__gm__ uint32_t *)blockInfoStart + AscendC::BLOCK_INFO_LEN_POS) = SIMT_DUMP_SIZE;
    *((__gm__ uint32_t *)blockInfoStart + AscendC::BLOCK_INFO_CORE_POS) = blockIdx;
    *((__gm__ uint32_t *)blockInfoStart + AscendC::BLOCK_INFO_BLOCKNUM_POS) = bisheng::cce::simt::get_block_num();
    *((__gm__ uint32_t *)blockInfoStart + AscendC::BLOCK_INFO_DUMPOFFSET_POS) = SIMT_DUMP_SIZE - blockInfoLen;
    *((__gm__ uint32_t *)blockInfoStart + AscendC::BLOCK_INFO_MAGIC_POS) = AscendC::BLOCK_INFO_MAGIC_NUM;
    *((__gm__ uint32_t *)blockInfoStart + AscendC::BLOCK_INFO_RSV_POS) = 0;
    *((__gm__ uint64_t *)((__gm__ uint32_t *)blockInfoStart + AscendC::BLOCK_INFO_DUMP_ADDR)) = blockInfoStart + blockInfoLen;

    blockInfoStart = blockInfoStart + sizeof(AscendC::BlockInfo);
    *(__gm__ uint32_t *)((__gm__ uint8_t *)blockInfoStart + AscendC::DUMP_META_TYPE_POS) =
        static_cast<uint32_t>(AscendC::DumpType::DUMP_META);
    *(__gm__ uint32_t *)((__gm__ uint8_t *)blockInfoStart + AscendC::DUMP_META_LEN_POS) = 8; // 8: simt thread(uint32_t) id and rsv(uint32_t)
    *(__gm__ uint32_t *)((__gm__ uint8_t *)blockInfoStart + AscendC::DUMP_META_SIMT_THREAD_ID_POS) = threadId;
    *(__gm__ uint32_t *)((__gm__ uint8_t *)blockInfoStart + AscendC::DUMP_META_RSV_POS) = 0;
}

template <class... Args>
__simt_callee__ inline void PrintfImpl(const __gm__ char *fmt, Args &&...args)
{
    if (AscendC::Simt::g_simtDumpWorkspaceReserved == nullptr) {
        return;
    }

    uint32_t blockIdx = __asc_simt_vf::GetDumpBlockIdx();
    if (blockIdx >= SIMT_DUMP_BLOCK_NUM) {
        return;
    }

    __gm__ AscendC::BlockInfo *blockInfo = __asc_simt_vf::GetBlockInfo();
    if (blockInfo->magic != AscendC::BLOCK_INFO_MAGIC_NUM) {
        __asc_simt_vf::InitDumpForThread();
    }
    uint32_t remainSize = blockInfo->dumpOffset;
    uint64_t dumpAddr = blockInfo->dumpAddr;

    uint32_t paramSize = __asc_simt_vf::GetParamSize(fmt, args...);
    uint32_t paramNum = __asc_simt_vf::GetArgsNum(args...) + 1;
    // AscendC::ONE_PARAM_SIZE(8 byte) algin
    paramSize = (paramSize + AscendC::ONE_PARAM_SIZE - 1) & (~(AscendC::ONE_PARAM_SIZE - 1));

    // sizeof(AscendC::DumpType + DumpLength) = AscendC::ONE_PARAM_SIZE(8 byte)
    uint32_t tlvSize = paramSize + AscendC::ONE_PARAM_SIZE;
    if (tlvSize > remainSize) {
        *((__gm__ uint32_t *)blockInfo + AscendC::BLOCK_INFO_RSV_POS) = AscendC::DUMP_EXC_FLAG;
        return;
    }

    __gm__ uint8_t *tlvAddr = (__gm__ uint8_t *)dumpAddr;
    __asc_simt_vf::WriteTLHead(AscendC::DumpType::DUMP_SIMT, tlvAddr, paramSize);

    // sizeof(AscendC::DumpType + DumpLength) = AscendC::ONE_PARAM_SIZE(8 byte)
    __gm__ uint8_t *paramAddr = tlvAddr + AscendC::ONE_PARAM_SIZE;
    uint32_t offset = paramNum * AscendC::ONE_PARAM_SIZE;
    __asc_simt_vf::WriteString(paramAddr, 0, fmt, offset);
    uint32_t paramIdx = 1;
    __asc_simt_vf::SetParam(paramAddr, paramIdx, offset, args...);

    __asc_simt_vf::UpdateBlockInfo(tlvSize);
}
}   // namespace __asc_simt_vf
#endif

#ifndef __CHECK_FEATURE_AT_PRECOMPILE

namespace __asc_simt_vf {
template <class... Args>
__attribute__((noinline)) __simt_callee__ void printf(const __gm__ char* fmt, Args&&... args)
{
#ifdef ASCENDC_DUMP
    __asc_simt_vf::PrintfImpl(fmt, args...);
#endif
}

}   // namespace __asc_simt_vf

#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_PRINTF_SIMT_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_PRINTF_SIMT_IMPL__
#endif

#endif