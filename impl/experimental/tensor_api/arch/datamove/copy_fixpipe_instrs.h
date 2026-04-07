/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/arch/datamove/copy_fixpipe_instrs.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy_fixpipe_instrs.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_DATAMOVE_COPY_FIXPIPE_INSTRS_H
#define IMPL_TENSOR_API_ARCH_DATAMOVE_COPY_FIXPIPE_INSTRS_H

#include "include/experimental/tensor_api/utils/utils.h"
#include "impl/experimental/tensor_api/arch/utils/copy_fixpipe_utils.h"

namespace AscendC {
namespace Te {

class SetRegister3510 {
public:
    // 带量化前置 + 3参数版本
    __aicore__ inline static void SetRegister(uint64_t quant, uint32_t ndNum, uint32_t dstNDStride,
                                              uint32_t srcNDStride)
    {
        SetQuantPre(quant);
        SetLoop3Para(ndNum, dstNDStride, srcNDStride);
    }

    // 带量化前置 + 4参数版本
    __aicore__ inline static void SetRegister(uint64_t quant, uint32_t dnNum, uint32_t dstDNStride,
                                              uint32_t srcNZMatrixStride, uint32_t srcNZC0Stride)
    {
        SetQuantPre(quant);
        SetLoop3Para(dnNum, dstDNStride, srcNZMatrixStride);
        SetChannelPara(srcNZC0Stride);
    }

    // 不带量化前置 + 3参数版本
    __aicore__ inline static void SetRegister(uint32_t ndNum, uint32_t dstNDStride, uint32_t srcNDStride)
    { SetLoop3Para(ndNum, dstNDStride, srcNDStride); }

    // 不带量化前置 + 4参数版本
    __aicore__ inline static void SetRegister(uint32_t dnNum, uint32_t dstDNStride, uint32_t srcNZMatrixStride,
                                              uint32_t srcNZC0Stride)
    {
        SetLoop3Para(dnNum, dstDNStride, srcNZMatrixStride);
        SetChannelPara(srcNZC0Stride);
    }

private:
    // 位域定义，消除魔法数字
    static constexpr uint32_t SHIFT_LOOP3_DST_STRIDE = 32;
    static constexpr uint32_t SHIFT_LOOP3_SRC_MATRIX = 16;
    static constexpr uint32_t SHIFT_CHANNEL_C0_STRIDE = 48;

    __aicore__ inline static void SetQuantPre(uint64_t quant)
    {
        if (ASCEND_IS_AIV) {
            return;
        }
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            set_quant_pre(quant);
        }
    }

    __aicore__ inline static void SetLoop3Para(uint32_t num, uint32_t dstStride, uint32_t srcStride)
    {
        if (ASCEND_IS_AIV) {
            return;
        }
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            uint64_t loop3Para = 0;
            loop3Para |= static_cast<uint64_t>(dstStride) << SHIFT_LOOP3_DST_STRIDE;
            loop3Para |= static_cast<uint64_t>(srcStride) << SHIFT_LOOP3_SRC_MATRIX;
            loop3Para |= static_cast<uint64_t>(num);
            set_loop3_para(loop3Para);
        }
    }

    __aicore__ inline static void SetChannelPara(uint32_t srcNZC0Stride)
    {
        if (ASCEND_IS_AIV) {
            return;
        }
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            uint64_t channelPara = 0;
            channelPara |= static_cast<uint64_t>(srcNZC0Stride) << SHIFT_CHANNEL_C0_STRIDE;
            set_channel_para(channelPara);
        }
    }

    // 禁止拷贝与构造
    SetRegister3510() = default;
    SetRegister3510(const SetRegister3510&) = delete;
    SetRegister3510& operator=(const SetRegister3510&) = delete;
};

class CopyMatrixCcToGm3510 {
public:
    template <QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void DataCopy(const T& dst, const U& src, uint32_t nSize, uint32_t mSize,
                                           uint32_t srcStride, uint32_t dstStride, uint8_t cacheMode, bool reluEn,
                                           uint8_t unitFlag, bool isChannelSplit, bool nz2ndEn, bool nz2dnEn)
    {
        if (ASCEND_IS_AIV) {
            return;
        }
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            copy_matrix_cc_to_gm(dst.Data().Get(), src.Data().Get(), 0, nSize, mSize, dstStride, srcStride, cacheMode,
                                 0, unitFlag, static_cast<uint64_t>(quantPre), reluEn, isChannelSplit, nz2ndEn,
                                 static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false,
                                 false, false, nz2dnEn);
        }
    }
};

class CopyMatrixCcToUb3510 {
public:
    template <QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void DataCopy(const T& dst, const U& src, uint32_t nSize, uint32_t mSize,
                                           uint32_t srcStride, uint32_t dstStride, uint8_t dualDstCtl, bool reluEn,
                                           uint8_t unitFlag, bool subBlockId, bool isChannelSplit, bool nz2ndEn, bool nz2dnEn)
    {
        if (ASCEND_IS_AIV) {
            return;
        }
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            copy_matrix_cc_to_ub(dst.Data().Get(), src.Data().Get(), 0, nSize, mSize, dstStride, srcStride, dualDstCtl,
                                 subBlockId, 0, unitFlag, static_cast<uint64_t>(quantPre), reluEn, isChannelSplit, nz2ndEn,
                                 static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false,
                                 false, false, nz2dnEn);
        }
    }
};

__aicore__ inline auto AllocFbTempBuf(const uint16_t& /* calNSize */)
{
    uint64_t deqTensorTempBuf = 0;
    if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
        deqTensorTempBuf = reinterpret_cast<uint64_t>(get_imm(0));
    }
    return deqTensorTempBuf;
}

template <typename T>
__aicore__ inline void SetFpc(const __fbuf__ T* deqTensorTempBuf)
{
    if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
        uint64_t deqTensorAddr = (reinterpret_cast<uint64_t>(deqTensorTempBuf) >> 7) << 8;
        set_fpc(deqTensorAddr);
    }
}

__aicore__ inline void InsertSync()
{
    if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
        pipe_barrier(PIPE_FIX);
    }
}

class CopyDeqTensorToFbuf3510 {
public:
    template <typename T>
    __aicore__ inline static void CopyDeqTensorToFbufImpl(const T& src, uint16_t calNSize, uint16_t nIterIndex)
    {
        auto dstAddr = reinterpret_cast<__fbuf__ uint64_t*>(AllocFbTempBuf(calNSize));
        auto dst = MakeTensor(MakeFixbufmemPtr(dstAddr), src.Layout());
        auto coord = MakeCoord(MakeCoord(Std::Int<0>{}, Std::Int<0>{}),
                               MakeCoord(Std::Int<0>{}, nIterIndex * MAIN_LOOP_N_SIZE_3510));
        auto shape = MakeShape(MakeShape(Std::Int<1>{}, Std::Int<1>{}), MakeShape(Std::Int<1>{}, calNSize));
        auto tileSrc = src(coord, shape);
        DataCopyL12FB3510::Run<DEFAULT_DATA_COPY_TRAIT>(dst, tileSrc);
        SetFpc(dstAddr);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_DATAMOVE_COPY_FIXPIPE_INSTRS_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif