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
 * \file mstx_local_tensor_info.h
 * \brief
 */
#ifndef MSTX_TENSOR_INFO_H
#define MSTX_TENSOR_INFO_H
#include "kernel_tensor.h"
#include "kernel_struct_unary.h"

namespace AscendC {
namespace MstxTensor {

#ifdef __MSTX_DFX_REPORT__

enum MstxTensorAddressSpace {
    MSTX_TENSOR_AT_PRIVATE = 0,
    MSTX_TENSOR_AT_GM,
    MSTX_TENSOR_AT_L1,
    MSTX_TENSOR_AT_L0A,
    MSTX_TENSOR_AT_L0B,
    MSTX_TENSOR_AT_L0C,
    MSTX_TENSOR_AT_UB,
};

struct MstxTensorDesc {
    MstxTensorAddressSpace space;
    uint64_t addr;
    uint64_t size;
    uint8_t dataBits;
};

template <typename T>
__aicore__ inline MstxTensorDesc From(const LocalTensor<T>& dst) {
    MstxTensorAddressSpace space;
    Hardware dstHWPos = GetPhyType((TPosition)dst.GetPosition());
    if (dstHWPos == Hardware::GM) {
        space = MSTX_TENSOR_AT_GM;
    } else if (dstHWPos == Hardware::UB) {
        space = MSTX_TENSOR_AT_UB;
    } else if (dstHWPos == Hardware::L1) {
        space = MSTX_TENSOR_AT_L1;
    } else if (dstHWPos == Hardware::L0A) {
        space = MSTX_TENSOR_AT_L0A;
    } else if (dstHWPos == Hardware::L0B) {
        space = MSTX_TENSOR_AT_L0B;
    } else if (dstHWPos == Hardware::L0C) {
        space = MSTX_TENSOR_AT_L0C;
    }

    return MstxTensorDesc{
        space,
        reinterpret_cast<uint64_t>(dst.GetPhyAddr()),
        dst.GetSize(),
        static_cast<uint8_t>(8 * sizeof(PrimT<T>))
    };
}

template <typename T>
__aicore__ inline MstxTensorDesc FromGm(const GlobalTensor<T>& dst) {
    return MstxTensorDesc{
        MSTX_TENSOR_AT_GM,
        reinterpret_cast<uint64_t>(dst.GetPhyAddr()),
        dst.GetSize(),
        static_cast<uint8_t>(sizeof(T))
    };
}

enum class MstxReportType : uint32_t {
    MSTX_VEC_UNARY = 3000,
    MSTX_VEC_BINARY = 3001,
    MSTX_VEC_GATHER = 3002,
    MSTX_DATA_COPY = 4001,
    MSTX_DATA_COPY_PAD = 4002,
};

enum MstxMaskMode: uint8_t {
    MSTX_MASK_NORM = 0,
    MSTX_MASK_COUNT,
    MSTX_MASK_FROM_REG = 0xff,
};

struct MstxVectorMask {
    uint64_t mask0;
    uint64_t mask1;
};

struct MstxVecWrapper {
    MstxMaskMode maskMode;
    MstxVectorMask mask;
    uint32_t reserveBufSize;
    bool useMask;
};

__aicore__ inline MstxVecWrapper WrapperFrom(MstxMaskMode maskMode, uint64_t mask0, uint64_t mask1, bool isSetMask, uint32_t reserveBufSize) {
    return MstxVecWrapper{
        maskMode,
        {mask0, mask1},
        reserveBufSize,
        isSetMask
    };
}

struct MstxVecUnaryDesc {
    MstxTensorDesc dst;
    MstxTensorDesc src;
    MstxVecWrapper wapper;
    uint32_t blockNum;
    uint8_t dstBlockStride;
    uint8_t srcBlockStride;
    uint8_t repeatTimes;
    uint8_t dstRepeatStride;
    uint8_t srcRepeatStride;
    char name[64];
};

struct MstxDataCopyDesc {
    MstxTensorDesc dst;
    MstxTensorDesc src;
    uint16_t lenBurst;
    uint16_t nBurst;
    uint16_t srcStride;
    uint16_t dstStride;
};

struct MstxDataCopyPadDesc {
    MstxTensorDesc dst;
    MstxTensorDesc src;
    uint32_t lenBurst;
    uint16_t nBurst;
    uint16_t srcGap;
    uint16_t dstGap;
    uint8_t leftPad;
    uint8_t rightPad;
};

struct MstxVecBinaryDesc {
    MstxTensorDesc dst;
    MstxTensorDesc src0;
    MstxTensorDesc src1;
    MstxVecWrapper wapper;
    uint32_t blockNum;
    uint8_t dstBlockStride;
    uint8_t src0BlockStride;
    uint8_t src1BlockStride;
    uint8_t repeatTimes;
    uint8_t dstRepeatStride;
    uint8_t src0RepeatStride;
    uint8_t src1RepeatStride;
    char name[64];
};

struct MstxVecGatherDesc {
    MstxTensorDesc dst;
    MstxTensorDesc src0;
    MstxTensorDesc src1;
    uint32_t blockNum;
    uint8_t dstBlockStride;
    uint8_t src0BlockStride;
    uint8_t src1BlockStride;
    uint8_t dstRepeatStride;
    uint8_t src0RepeatStride;
    uint8_t src1RepeatStride;
    bool useMask;
};

__aicore__ inline void CopyName(char b[64], const char* a)
{
    uint32_t i = 0;
    for (; i < 63; ++i){
        b[i] = a[i];
        if (a[i] == '\0') {
            break;
        }
    }
    b[i] = '\0';
}

template <typename T, bool isSetMask>
__aicore__ inline void GetMstxVecUnaryInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src, uint64_t mask0, uint64_t mask1,
    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams, const char* name)
{
    MstxVecUnaryDesc mstxVecUnaryDesc;
    mstxVecUnaryDesc.dst = From(dst);
    mstxVecUnaryDesc.src = From(src);
    mstxVecUnaryDesc.wapper = WrapperFrom(MSTX_MASK_FROM_REG, mask0, mask1, isSetMask, static_cast<uint32_t>(0));//0是非软仿
    mstxVecUnaryDesc.blockNum = repeatParams.blockNumber;
    mstxVecUnaryDesc.repeatTimes = repeatTime;
    mstxVecUnaryDesc.dstBlockStride = repeatParams.dstBlkStride;
    mstxVecUnaryDesc.srcBlockStride = repeatParams.srcBlkStride;
    mstxVecUnaryDesc.dstRepeatStride = repeatParams.dstRepStride;
    mstxVecUnaryDesc.srcRepeatStride = repeatParams.srcRepStride;
    CopyName(mstxVecUnaryDesc.name, name);

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_VEC_UNARY), sizeof(mstxVecUnaryDesc), &mstxVecUnaryDesc);
}

template <typename T, bool isSetMask>
__aicore__ inline void GetMstxVecUnaryInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src, const char* name, 
    const int32_t count)
{
    MstxVecUnaryDesc mstxVecUnaryDesc;
    mstxVecUnaryDesc.dst = From(dst);
    mstxVecUnaryDesc.src = From(src);
    mstxVecUnaryDesc.wapper = WrapperFrom(MSTX_MASK_COUNT, count, static_cast<uint64_t>(0), isSetMask, static_cast<uint32_t>(0));
    mstxVecUnaryDesc.blockNum = 8;
    mstxVecUnaryDesc.repeatTimes = 1;
    mstxVecUnaryDesc.dstBlockStride = 1;
    mstxVecUnaryDesc.srcBlockStride = 1;
    mstxVecUnaryDesc.dstRepeatStride = 8;
    mstxVecUnaryDesc.srcRepeatStride = 8;
    CopyName(mstxVecUnaryDesc.name, name);

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_VEC_UNARY), sizeof(mstxVecUnaryDesc), &mstxVecUnaryDesc);
}

template <typename T, typename U, typename V>
__aicore__ inline void GetMstxVecBinaryInfo(const LocalTensor<T>& dst, const LocalTensor<U>& src0,
    const LocalTensor<V>& src1, uint64_t mask0, uint64_t mask1, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams, bool isSetMask, const char* name)
{
    MstxVecBinaryDesc mstxVecBinaryDesc;
    mstxVecBinaryDesc.dst = From(dst);
    mstxVecBinaryDesc.src0 = From(src0);
    mstxVecBinaryDesc.src1 = From(src1);
    mstxVecBinaryDesc.wapper = WrapperFrom(MSTX_MASK_FROM_REG, mask0, mask1, isSetMask, static_cast<uint32_t>(0));
    mstxVecBinaryDesc.blockNum = repeatParams.blockNumber;
    mstxVecBinaryDesc.repeatTimes = repeatTime;
    mstxVecBinaryDesc.dstBlockStride = repeatParams.dstBlkStride;
    mstxVecBinaryDesc.src0BlockStride = repeatParams.src0BlkStride;
    mstxVecBinaryDesc.src1BlockStride = repeatParams.src1BlkStride;
    mstxVecBinaryDesc.dstRepeatStride = repeatParams.dstRepStride;
    mstxVecBinaryDesc.src0RepeatStride = repeatParams.src0RepStride;
    mstxVecBinaryDesc.src1RepeatStride = repeatParams.src1RepStride;
    CopyName(mstxVecBinaryDesc.name, name);

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_VEC_BINARY), sizeof(mstxVecBinaryDesc), &mstxVecBinaryDesc);
}

template <typename T, typename U, typename V>
__aicore__ inline void GetMstxVecBinaryInfo(const LocalTensor<T>& dst, const LocalTensor<U>& src0,
    const LocalTensor<V>& src1, const char* name, const int32_t count)
{
    MstxVecBinaryDesc mstxVecBinaryDesc;
    mstxVecBinaryDesc.dst = From(dst);
    mstxVecBinaryDesc.src0 = From(src0);
    mstxVecBinaryDesc.src1 = From(src1);
    mstxVecBinaryDesc.wapper = WrapperFrom(MSTX_MASK_COUNT, count, static_cast<uint64_t>(0), false, static_cast<uint32_t>(0));//没有ismask怎么办
    mstxVecBinaryDesc.blockNum = 8;
    mstxVecBinaryDesc.repeatTimes = 1;
    mstxVecBinaryDesc.dstBlockStride = 1;
    mstxVecBinaryDesc.src0BlockStride = 1;
    mstxVecBinaryDesc.src1BlockStride = 1;
    mstxVecBinaryDesc.dstRepeatStride = 8;
    mstxVecBinaryDesc.src0RepeatStride = 8;
    mstxVecBinaryDesc.src1RepeatStride = 8;
    CopyName(mstxVecBinaryDesc.name, name);

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_VEC_BINARY), sizeof(mstxVecBinaryDesc), &mstxVecBinaryDesc);
}


template <typename T>
__aicore__ inline void GetMstxVecBinaryScalarInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
    const LocalTensor<T>& src1, uint64_t mask0, uint64_t mask1, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool isSetMask, const char* name)
{
    MstxVecBinaryDesc mstxVecBinaryDesc;
    mstxVecBinaryDesc.dst = From(dst);
    mstxVecBinaryDesc.src0 = From(src0);
    mstxVecBinaryDesc.src1 = From(src1);
    mstxVecBinaryDesc.wapper = WrapperFrom(MSTX_MASK_FROM_REG, mask0, mask1, isSetMask, static_cast<uint32_t>(0));
    mstxVecBinaryDesc.blockNum = repeatParams.blockNumber;
    mstxVecBinaryDesc.repeatTimes = repeatTime;
    mstxVecBinaryDesc.dstBlockStride = repeatParams.dstBlkStride;
    mstxVecBinaryDesc.src0BlockStride = repeatParams.srcBlkStride;
    mstxVecBinaryDesc.src1BlockStride = repeatParams.srcBlkStride;
    mstxVecBinaryDesc.dstRepeatStride = repeatParams.dstRepStride;
    mstxVecBinaryDesc.src0RepeatStride = repeatParams.srcRepStride;
    mstxVecBinaryDesc.src1RepeatStride = repeatParams.srcRepStride;
    CopyName(mstxVecBinaryDesc.name, name);

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_VEC_BINARY), sizeof(mstxVecBinaryDesc), &mstxVecBinaryDesc);
}

template <typename T>
__aicore__ inline void GetMstxVecBinaryScalarInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
    const LocalTensor<T>& src1, bool isSetMask, const char* name, const int32_t count)
{
    MstxVecBinaryDesc mstxVecBinaryDesc;
    mstxVecBinaryDesc.dst = From(dst);
    mstxVecBinaryDesc.src0 = From(src0);
    mstxVecBinaryDesc.src1 = From(src1);
    mstxVecBinaryDesc.wapper = WrapperFrom(MSTX_MASK_COUNT, count, static_cast<uint64_t>(0), isSetMask, static_cast<uint32_t>(0));
    mstxVecBinaryDesc.blockNum = 8;
    mstxVecBinaryDesc.repeatTimes = 1;
    mstxVecBinaryDesc.dstBlockStride = 1;
    mstxVecBinaryDesc.src0BlockStride = 1;
    mstxVecBinaryDesc.src1BlockStride = 1;
    mstxVecBinaryDesc.dstRepeatStride = 8;
    mstxVecBinaryDesc.src0RepeatStride = 8;
    mstxVecBinaryDesc.src1RepeatStride = 8;
    CopyName(mstxVecBinaryDesc.name, name);

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_VEC_BINARY), sizeof(mstxVecBinaryDesc), &mstxVecBinaryDesc);
}

template <typename T>
__aicore__ inline void GetMstxVecGatherInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
    const LocalTensor<uint32_t>& src1, const GatherRepeatParams& repeatParams)
{
    MstxVecGatherDesc mstxVecGatherDesc;
    mstxVecGatherDesc.dst = From(dst);
    mstxVecGatherDesc.src0 = From(src0);
    mstxVecGatherDesc.src1 = From(src1);
    mstxVecGatherDesc.blockNum = repeatParams.blockNumber;
    mstxVecGatherDesc.dstBlockStride = repeatParams.dstBlkStride;
    mstxVecGatherDesc.src0BlockStride = repeatParams.src0BlkStride;
    mstxVecGatherDesc.src1BlockStride = repeatParams.src1BlkStride;
    mstxVecGatherDesc.dstRepeatStride = repeatParams.dstRepStride;
    mstxVecGatherDesc.src0RepeatStride = repeatParams.src0RepStride;
    mstxVecGatherDesc.src1RepeatStride = repeatParams.src1RepStride;

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_VEC_GATHER), sizeof(mstxVecGatherDesc), &mstxVecGatherDesc);
}

template <typename T>
__aicore__ inline void GetMstxVecGatherInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
    const LocalTensor<uint32_t>& src1, const uint16_t dstRepStride)
{
    MstxVecGatherDesc mstxVecGatherDesc;
    mstxVecGatherDesc.dst = From(dst);
    mstxVecGatherDesc.src0 = From(src0);
    mstxVecGatherDesc.src1 = From(src1);
    mstxVecGatherDesc.blockNum = 8;
    mstxVecGatherDesc.dstBlockStride = 1;
    mstxVecGatherDesc.src0BlockStride = 1;
    mstxVecGatherDesc.src1BlockStride = 1;
    mstxVecGatherDesc.dstRepeatStride = dstRepStride;
    mstxVecGatherDesc.src0RepeatStride = 8;
    mstxVecGatherDesc.src1RepeatStride = 8;

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_VEC_GATHER), sizeof(mstxVecGatherDesc), &mstxVecGatherDesc);
}

template <typename T>
__aicore__ inline void GetMstxVecGatherCountInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
    const LocalTensor<uint32_t>& src1, const int32_t count)
{
    MstxVecGatherDesc mstxVecGatherDesc;
    mstxVecGatherDesc.dst = From(dst);
    mstxVecGatherDesc.src0 = From(src0);
    mstxVecGatherDesc.src1 = From(src1);
    mstxVecGatherDesc.blockNum = 8;
    mstxVecGatherDesc.dstBlockStride = 1;
    mstxVecGatherDesc.src0BlockStride = 1;
    mstxVecGatherDesc.src1BlockStride = 1;
    mstxVecGatherDesc.dstRepeatStride = 8;
    mstxVecGatherDesc.src0RepeatStride = 8;
    mstxVecGatherDesc.src1RepeatStride = 8;

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_VEC_GATHER), sizeof(mstxVecGatherDesc), &mstxVecGatherDesc);
}

template <typename T, typename U>
__aicore__ inline void GetMstxDataCopyInfo(const LocalTensor<T>& dst, const GlobalTensor<U>& src,
    const DataCopyParams& repeatParams)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = From(dst);
    mstxDataCopyDesc.src = FromGm(src);
    mstxDataCopyDesc.nBurst = repeatParams.blockCount;
    mstxDataCopyDesc.lenBurst = repeatParams.blockLen;
    mstxDataCopyDesc.srcStride = repeatParams.srcStride;
    mstxDataCopyDesc.dstStride = repeatParams.dstStride;

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_DATA_COPY), sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}

template <typename T, typename U>
__aicore__ inline void GetMstxDataCopyInfo(const GlobalTensor<T>& dst, const LocalTensor<U>& src,
    const DataCopyParams& repeatParams)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = FromGm(dst);
    mstxDataCopyDesc.src = From(src);
    mstxDataCopyDesc.nBurst = repeatParams.blockCount;
    mstxDataCopyDesc.lenBurst = repeatParams.blockLen;
    mstxDataCopyDesc.srcStride = repeatParams.srcStride;
    mstxDataCopyDesc.dstStride = repeatParams.dstStride;

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_DATA_COPY), sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}

template <typename T, typename U>
__aicore__ inline void GetMstxDataCopyInfo(const LocalTensor<T>& dst, const LocalTensor<U>& src,
    const DataCopyParams& repeatParams)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = From(dst);
    mstxDataCopyDesc.src = From(src);
    mstxDataCopyDesc.nBurst = repeatParams.blockCount;
    mstxDataCopyDesc.lenBurst = repeatParams.blockLen;
    mstxDataCopyDesc.srcStride = repeatParams.srcStride;
    mstxDataCopyDesc.dstStride = repeatParams.dstStride;

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_DATA_COPY), sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}


template <typename T, typename U>
__aicore__ inline void GetMstxDataCopyInfo(const LocalTensor<T>& dst, const GlobalTensor<U>& src, const int32_t count)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = From(dst);
    mstxDataCopyDesc.src = FromGm(src);
    mstxDataCopyDesc.nBurst = 1;
    mstxDataCopyDesc.lenBurst = 0;

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_DATA_COPY), sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}

template <typename T, typename U>
__aicore__ inline void GetMstxDataCopyInfo(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const int32_t count)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = FromGm(dst);
    mstxDataCopyDesc.src = From(src);
    mstxDataCopyDesc.nBurst = 1;
    mstxDataCopyDesc.lenBurst = 0;

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_DATA_COPY), sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}

template <typename T, typename U>
__aicore__ inline void GetMstxDataCopyInfo(const LocalTensor<T>& dst, const LocalTensor<U>& src, const int32_t count)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = From(dst);
    mstxDataCopyDesc.src = From(src);
    mstxDataCopyDesc.nBurst = 1;
    mstxDataCopyDesc.lenBurst = 0;

    __mstx_dfx_report_stub(static_cast<int>(MstxReportType::MSTX_DATA_COPY), sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}








#endif
}
}
#endif //MSTX_TENSOR_INFO_H