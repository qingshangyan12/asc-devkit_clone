#ifndef MSTX_TENSOR_INFO_H
#define MSTX_TENSOR_INFO_H
#include "kernel_tensor.h"
#include "kernel_struct_unary.h"

namespace AscendC {
namespace MstxTensor {

#ifdef __MSTX_DFX_REPORT__

enum MstxTensorAddressSpace {
    MSTX_TENSOR_AT_GM = 0,
    MSTX_TENSOR_AT_UB,
    MSTX_TENSOR_AT_L1,
    MSTX_TENSOR_AT_L0A,
    MSTX_TENSOR_AT_L0B,
    MSTX_TENSOR_AT_L0C,
};

struct MstxTensorDesc {
    MstxTensorAddressSpace space;
    uint64_t addr;
    uint64_t size;
    uint8_t databit;

    template <typename T>
    static MstxTensorAddressSpace GetTensorSpace(const LocalTensor<T>& dst)
    {
        Hardware dstHWPos = GetPhyType((TPosition)dst.GetPosition());
        if (dstHWPos == Hardware::GM) {
            return MSTX_TENSOR_AT_GM;
        } else if (dstHWPos == Hardware::UB) {
            return MSTX_TENSOR_AT_UB;
        } else if (dstHWPos == Hardware::L1) {
            return MSTX_TENSOR_AT_L1;
        } else if (dstHWPos == Hardware::L0A) {
            return MSTX_TENSOR_AT_L0A;
        } else if (dstHWPos == Hardware::L0B) {
            return MSTX_TENSOR_AT_L0B;
        } else if (dstHWPos == Hardware::L0C) {
            return MSTX_TENSOR_AT_L0C;
        }
    }

    template <typename T>
    static MstxTensorDesc From(const LocalTensor<T>& dst) {
        return MstxTensorDesc{
            MstxTensorDesc::GetTensorSpace(dst),
            reinterpret_cast<uint64_t>(dst.GetPhyAddr()),
            dst.GetSize(),
            static_cast<uint8_t>(sizeof(T))
        };
    }

    template <typename T>
    static MstxTensorDesc FromGm(const GlobalTensor<T>& dst) {
        return MstxTensorDesc{
            MSTX_TENSOR_AT_GM,
            reinterpret_cast<uint64_t>(dst.GetPhyAddr()),
            dst.GetSize(),
            static_cast<uint8_t>(sizeof(T))
        };
    }
};

enum class MstxReportType : uint32_t {
    MSTX_VEC_UNARY = 3000,
    MSTX_VEC_BINARY = 3001,
    MSTX_VEC_GATHER = 3002,
    MSTX_DATA_COPY = 4001,
    MSTX_DATA_COPY_PAD = 4002,
};

enum MstxMaskMode {
    MSTX_MASK_DEFAULT = 0,
    MSTX_MASK_NORM = 1,
    MSTX_MASK_COUNT = 2,
};

struct MstxVectorMask {
    uint64_t mask0;
    uint64_t mask1;
};

struct MstxVecWrapper {
    MstxMaskMode maskMode;
    MstxVectorMask mask;
    bool useMask;
    uint32_t reserveBufSize;
};

struct MstxVecUnaryDesc {
    MstxTensorDesc dst;
    MstxTensorDesc src;
    MstxVecWrapper wapper;
    uint32_t blockNum;
    uint8_t repeatTimes;
    uint8_t dstBlockStride;
    uint8_t srcBlockStride;
    uint8_t dstRepeatStride;
    uint8_t srcRepeatStride;
    bool useMask;
    char apiName[64];
};

struct MstxDataCopyDesc {
    MstxTensorDesc dst;
    MstxTensorDesc src;
    uint16_t nBurst;
    uint16_t lenBurst;
    uint16_t srcStride;
    uint16_t dstStride;
};

struct MstxDataCopyPadDesc {
    MstxTensorDesc dst;
    MstxTensorDesc src;
    uint16_t nBurst;
    uint32_t lenBurst;
    uint16_t srcGap;
    uint16_t dstGap;
    uint8_t leftPad;
    uint8_t rightPad;
};

struct MstxVecBinaryDesc {
    MstxTensorDesc dst;
    MstxTensorDesc src0;
    MstxTensorDesc src1;
    MstxMaskMode maskMode;
    MstxVectorMask mask;
    uint32_t blockNum;
    uint8_t repeatTimes;
    uint8_t dstBlockStride;
    uint8_t src0BlockStride;
    uint8_t src1BlockStride;
    uint8_t dstRepeatStride;
    uint8_t src0RepeatStride;
    uint8_t src1RepeatStride;
    bool useMask;
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

template <typename T, bool isSetMask>
void GetMstxVecUnaryInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src, uint64_t mask0, uint64_t mask1,
    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    MstxVecUnaryDesc mstxVecUnaryDesc;
    mstxVecUnaryDesc.dst = MstxTensorDesc::From(dst);
    mstxVecUnaryDesc.src = MstxTensorDesc::From(src);
    mstxVecUnaryDesc.wapper.maskMode = MSTX_MASK_DEFAULT;
    mstxVecUnaryDesc.wapper.mask.mask0 = mask0;
    mstxVecUnaryDesc.wapper.mask.mask1 = mask1;
    mstxVecUnaryDesc.wapper.useMask = isSetMask;
    mstxVecUnaryDesc.blockNum = repeatParams.blockNumber;
    mstxVecUnaryDesc.repeatTimes = repeatTime;
    mstxVecUnaryDesc.dstBlockStride = repeatParams.dstBlkStride;
    mstxVecUnaryDesc.srcBlockStride = repeatParams.srcBlkStride;
    mstxVecUnaryDesc.dstRepeatStride = repeatParams.dstRepStride;
    mstxVecUnaryDesc.srcRepeatStride = repeatParams.srcRepStride;

    __mstx_dfx_report_stub(MstxReportType::MSTX_VEC_UNARY, sizeof(mstxVecUnaryDesc), &mstxVecUnaryDesc);
}

template <typename T, bool isSetMask>
void GetMstxVecUnaryInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src)
{
    MstxVecUnaryDesc mstxVecUnaryDesc;
    mstxVecUnaryDesc.dst = MstxTensorDesc::From(dst);
    mstxVecUnaryDesc.src = MstxTensorDesc::From(src);
    mstxVecUnaryDesc.wapper.maskMode = MSTX_MASK_COUNT;
    mstxVecUnaryDesc.wapper.mask.mask0 = 0xffffffffffffffff;
    mstxVecUnaryDesc.wapper.mask.mask1 = 0xffffffffffffffff;
    mstxVecUnaryDesc.wapper.useMask = isSetMask;
    mstxVecUnaryDesc.blockNum = 8;
    mstxVecUnaryDesc.repeatTimes = 1;
    mstxVecUnaryDesc.dstBlockStride = 1;
    mstxVecUnaryDesc.srcBlockStride = 1;
    mstxVecUnaryDesc.dstRepeatStride = 8;
    mstxVecUnaryDesc.srcRepeatStride = 8;

    __mstx_dfx_report_stub(MstxReportType::MSTX_VEC_UNARY, sizeof(mstxVecUnaryDesc), &mstxVecUnaryDesc);
}

template <typename T, typename U, typename V>
void GetMstxVecBinaryInfo(const LocalTensor<T>& dst, const LocalTensor<U>& src0,
    const LocalTensor<V>& src1, uint64_t mask0, uint64_t mask1, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams, bool isSetMask)
{
    MstxVecBinaryDesc mstxVecBinaryDesc;
    mstxVecBinaryDesc.dst = MstxTensorDesc::From(dst);
    mstxVecBinaryDesc.src0 = MstxTensorDesc::From(src0);
    mstxVecBinaryDesc.src1 = MstxTensorDesc::From(src1);
    mstxVecBinaryDesc.maskMode = MSTX_MASK_DEFAULT;
    mstxVecBinaryDesc.mask.mask0 = mask0;
    mstxVecBinaryDesc.mask.mask1 = mask1;
    mstxVecBinaryDesc.blockNum = repeatParams.blockNumber;
    mstxVecBinaryDesc.repeatTimes = repeatTime;
    mstxVecBinaryDesc.dstBlockStride = repeatParams.dstBlkStride;
    mstxVecBinaryDesc.src0BlockStride = repeatParams.src0BlkStride;
    mstxVecBinaryDesc.src1BlockStride = repeatParams.src1BlkStride;
    mstxVecBinaryDesc.dstRepeatStride = repeatParams.dstRepStride;
    mstxVecBinaryDesc.src0RepeatStride = repeatParams.src0RepStride;
    mstxVecBinaryDesc.src1RepeatStride = repeatParams.src1RepStride;
    mstxVecBinaryDesc.useMask = isSetMask;

    __mstx_dfx_report_stub(MstxReportType::MSTX_VEC_BINARY, sizeof(mstxVecBinaryDesc), &mstxVecBinaryDesc);
}

template <typename T, typename U, typename V>
void GetMstxVecBinaryInfo(const LocalTensor<T>& dst, const LocalTensor<U>& src0,
    const LocalTensor<V>& src1)
{
    MstxVecBinaryDesc mstxVecBinaryDesc;
    mstxVecBinaryDesc.dst = MstxTensorDesc::From(dst);
    mstxVecBinaryDesc.src0 = MstxTensorDesc::From(src0);
    mstxVecBinaryDesc.src1 = MstxTensorDesc::From(src1);
    mstxVecBinaryDesc.maskMode = MSTX_MASK_COUNT;
    mstxVecBinaryDesc.mask.mask0 = 0xffffffffffffffff;
    mstxVecBinaryDesc.mask.mask1 = 0xffffffffffffffff;
    mstxVecBinaryDesc.blockNum = 8;
    mstxVecBinaryDesc.repeatTimes = 1;
    mstxVecBinaryDesc.dstBlockStride = 1;
    mstxVecBinaryDesc.src0BlockStride = 1;
    mstxVecBinaryDesc.src1BlockStride = 1;
    mstxVecBinaryDesc.dstRepeatStride = 8;
    mstxVecBinaryDesc.src0RepeatStride = 8;
    mstxVecBinaryDesc.src1RepeatStride = 8;

    __mstx_dfx_report_stub(MstxReportType::MSTX_VEC_BINARY, sizeof(mstxVecBinaryDesc), &mstxVecBinaryDesc);
}


template <typename T>
void GetMstxVecBinaryScalarInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
    const LocalTensor<T>& src1, uint64_t mask0, uint64_t mask1, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool isSetMask)
{
    MstxVecBinaryDesc mstxVecBinaryDesc;
    mstxVecBinaryDesc.dst = MstxTensorDesc::From(dst);
    mstxVecBinaryDesc.src0 = MstxTensorDesc::From(src0);
    mstxVecBinaryDesc.src1 = MstxTensorDesc::From(src1);
    mstxVecBinaryDesc.maskMode = MSTX_MASK_DEFAULT;
    mstxVecBinaryDesc.mask.mask0 = mask0;
    mstxVecBinaryDesc.mask.mask1 = mask1;
    mstxVecBinaryDesc.blockNum = repeatParams.blockNumber;
    mstxVecBinaryDesc.repeatTimes = repeatTime;
    mstxVecBinaryDesc.dstBlockStride = repeatParams.dstBlkStride;
    mstxVecBinaryDesc.src0BlockStride = repeatParams.srcBlkStride;
    mstxVecBinaryDesc.src1BlockStride = repeatParams.srcBlkStride;
    mstxVecBinaryDesc.dstRepeatStride = repeatParams.dstRepStride;
    mstxVecBinaryDesc.src0RepeatStride = repeatParams.srcRepStride;
    mstxVecBinaryDesc.src1RepeatStride = repeatParams.srcRepStride;
    mstxVecBinaryDesc.useMask = isSetMask;

    __mstx_dfx_report_stub(MstxReportType::MSTX_VEC_BINARY, sizeof(mstxVecBinaryDesc), &mstxVecBinaryDesc);
}

template <typename T>
void GetMstxVecBinaryScalarInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
    const LocalTensor<T>& src1, bool isSetMask)
{
    MstxVecBinaryDesc mstxVecBinaryDesc;
    mstxVecBinaryDesc.dst = MstxTensorDesc::From(dst);
    mstxVecBinaryDesc.src0 = MstxTensorDesc::From(src0);
    mstxVecBinaryDesc.src1 = MstxTensorDesc::From(src1);
    mstxVecBinaryDesc.maskMode = MSTX_MASK_COUNT;
    mstxVecBinaryDesc.mask.mask0 = 0xffffffffffffffff;
    mstxVecBinaryDesc.mask.mask1 = 0xffffffffffffffff;
    mstxVecBinaryDesc.blockNum = 8;
    mstxVecBinaryDesc.repeatTimes = 1;
    mstxVecBinaryDesc.dstBlockStride = 1;
    mstxVecBinaryDesc.src0BlockStride = 1;
    mstxVecBinaryDesc.src1BlockStride = 1;
    mstxVecBinaryDesc.dstRepeatStride = 8;
    mstxVecBinaryDesc.src0RepeatStride = 8;
    mstxVecBinaryDesc.src1RepeatStride = 8;
    mstxVecBinaryDesc.useMask = isSetMask;

    __mstx_dfx_report_stub(MstxReportType::MSTX_VEC_BINARY, sizeof(mstxVecBinaryDesc), &mstxVecBinaryDesc);
}

template <typename T>
void GetMstxVecGatherInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
    const LocalTensor<uint32_t>& src1, const GatherRepeatParams& repeatParams)
{
    MstxVecGatherDesc mstxVecGatherDesc;
    mstxVecGatherDesc.dst = MstxTensorDesc::From(dst);
    mstxVecGatherDesc.src0 = MstxTensorDesc::From(src0);
    mstxVecGatherDesc.src1 = MstxTensorDesc::From(src1);
    mstxVecGatherDesc.blockNum = repeatParams.blockNumber;
    mstxVecGatherDesc.dstBlockStride = repeatParams.dstBlkStride;
    mstxVecGatherDesc.src0BlockStride = repeatParams.src0BlkStride;
    mstxVecGatherDesc.src1BlockStride = repeatParams.src1BlkStride;
    mstxVecGatherDesc.dstRepeatStride = repeatParams.dstRepStride;
    mstxVecGatherDesc.src0RepeatStride = repeatParams.src0RepStride;
    mstxVecGatherDesc.src1RepeatStride = repeatParams.src1RepStride;

    __mstx_dfx_report_stub(MstxReportType::MSTX_VEC_GATHER, sizeof(mstxVecGatherDesc), &mstxVecGatherDesc);
}

template <typename T>
void GetMstxVecGatherInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
    const LocalTensor<uint32_t>& src1, const uint16_t dstRepStride)
{
    MstxVecGatherDesc mstxVecGatherDesc;
    mstxVecGatherDesc.dst = MstxTensorDesc::From(dst);
    mstxVecGatherDesc.src0 = MstxTensorDesc::From(src0);
    mstxVecGatherDesc.src1 = MstxTensorDesc::From(src1);
    mstxVecGatherDesc.blockNum = 8;
    mstxVecGatherDesc.dstBlockStride = 1;
    mstxVecGatherDesc.src0BlockStride = 1;
    mstxVecGatherDesc.src1BlockStride = 1;
    mstxVecGatherDesc.dstRepeatStride = dstRepStride;
    mstxVecGatherDesc.src0RepeatStride = 8;
    mstxVecGatherDesc.src1RepeatStride = 8;

    __mstx_dfx_report_stub(MstxReportType::MSTX_VEC_GATHER, sizeof(mstxVecGatherDesc), &mstxVecGatherDesc);
}

template <typename T>
void GetMstxVecGatherInfo(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
    const LocalTensor<uint32_t>& src1)
{
    MstxVecGatherDesc mstxVecGatherDesc;
    mstxVecGatherDesc.dst = MstxTensorDesc::From(dst);
    mstxVecGatherDesc.src0 = MstxTensorDesc::From(src0);
    mstxVecGatherDesc.src1 = MstxTensorDesc::From(src1);
    mstxVecGatherDesc.blockNum = 8;
    mstxVecGatherDesc.dstBlockStride = 1;
    mstxVecGatherDesc.src0BlockStride = 1;
    mstxVecGatherDesc.src1BlockStride = 1;
    mstxVecGatherDesc.dstRepeatStride = 8;
    mstxVecGatherDesc.src0RepeatStride = 8;
    mstxVecGatherDesc.src1RepeatStride = 8;

    __mstx_dfx_report_stub(MstxReportType::MSTX_VEC_GATHER, sizeof(mstxVecGatherDesc), &mstxVecGatherDesc);
}

template <typename T, typename U>
void GetMstxDataCopyInfo(const LocalTensor<T>& dst, const GlobalTensor<U>& src,
    const DataCopyParams& repeatParams)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = MstxTensorDesc::From(dst);
    mstxDataCopyDesc.src = MstxTensorDesc::FromGm(src);
    mstxDataCopyDesc.nBurst = repeatParams.blockCount;
    mstxDataCopyDesc.lenBurst = repeatParams.blockLen;
    mstxDataCopyDesc.srcStride = repeatParams.srcStride;
    mstxDataCopyDesc.dstStride = repeatParams.dstStride;

    __mstx_dfx_report_stub(MstxReportType::MSTX_DATA_COPY, sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}

template <typename T, typename U>
void GetMstxDataCopyInfo(const GlobalTensor<T>& dst, const LocalTensor<U>& src,
    const DataCopyParams& repeatParams)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = MstxTensorDesc::FromGm(dst);
    mstxDataCopyDesc.src = MstxTensorDesc::From(src);
    mstxDataCopyDesc.nBurst = repeatParams.blockCount;
    mstxDataCopyDesc.lenBurst = repeatParams.blockLen;
    mstxDataCopyDesc.srcStride = repeatParams.srcStride;
    mstxDataCopyDesc.dstStride = repeatParams.dstStride;

    __mstx_dfx_report_stub(MstxReportType::MSTX_DATA_COPY, sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}

template <typename T, typename U>
void GetMstxDataCopyInfo(const LocalTensor<T>& dst, const LocalTensor<U>& src,
    const DataCopyParams& repeatParams)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = MstxTensorDesc::From(dst);
    mstxDataCopyDesc.src = MstxTensorDesc::From(src);
    mstxDataCopyDesc.nBurst = repeatParams.blockCount;
    mstxDataCopyDesc.lenBurst = repeatParams.blockLen;
    mstxDataCopyDesc.srcStride = repeatParams.srcStride;
    mstxDataCopyDesc.dstStride = repeatParams.dstStride;

    __mstx_dfx_report_stub(MstxReportType::MSTX_DATA_COPY, sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}


template <typename T, typename U>
void GetMstxDataCopyInfo(const LocalTensor<T>& dst, const GlobalTensor<U>& src)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = MstxTensorDesc::From(dst);
    mstxDataCopyDesc.src = MstxTensorDesc::FromGm(src);
    mstxDataCopyDesc.nBurst = 1;
    mstxDataCopyDesc.lenBurst = 0;

    __mstx_dfx_report_stub(MstxReportType::MSTX_DATA_COPY, sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}

template <typename T, typename U>
void GetMstxDataCopyInfo(const GlobalTensor<T>& dst, const LocalTensor<U>& src)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = MstxTensorDesc::FromGm(dst);
    mstxDataCopyDesc.src = MstxTensorDesc::From(src);
    mstxDataCopyDesc.nBurst = 1;
    mstxDataCopyDesc.lenBurst = 0;

    __mstx_dfx_report_stub(MstxReportType::MSTX_DATA_COPY, sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}

template <typename T, typename U>
void GetMstxDataCopyInfo(const LocalTensor<T>& dst, const LocalTensor<U>& src)
{
    MstxDataCopyDesc mstxDataCopyDesc;
    mstxDataCopyDesc.dst = MstxTensorDesc::From(dst);
    mstxDataCopyDesc.src = MstxTensorDesc::From(src);
    mstxDataCopyDesc.nBurst = 1;
    mstxDataCopyDesc.lenBurst = 0;

    __mstx_dfx_report_stub(MstxReportType::MSTX_DATA_COPY, sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
}

// template <typename T, typename U>
// void GetMstxDataCopyPadInfo(const LocalTensor<T>& dst, const GlobalTensor<U>& src,
//     const DataCopyParams& repeatParams)
// {
//     MstxDataCopyPadDesc mstxDataCopyPadDesc;
//     mstxDataCopyPadDesc.dst = MstxTensorDesc::From(dst);
//     mstxDataCopyPadDesc.src = MstxTensorDesc::FromGm(src);
//     mstxDataCopyPadDesc.nBurst = repeatParams.blockCount;
//     mstxDataCopyPadDesc.lenBurst = repeatParams.blockLen;
//     mstxDataCopyPadDesc.srcStride = ;
//     mstxDataCopyPadDesc.dstStride = ;

//     __mstx_dfx_report_stub(MstxReportType::MSTX_DATA_COPY_PAD, sizeof(mstxDataCopyDesc), &mstxDataCopyDesc);
// }






#endif
}
}
#endif //MSTX_TENSOR_INFO_H