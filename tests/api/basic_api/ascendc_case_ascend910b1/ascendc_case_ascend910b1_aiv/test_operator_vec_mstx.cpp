/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#define __MSTX_DFX_REPORT__
#include <cstdint>
extern "C" void __mstx_dfx_report_stub(int type, uint8_t size, void* desc);
#include <gtest/gtest.h>
#include <string.h>
 
#include "kernel_log.h"
static uint8_t g_testRes = 1;      // 全局变量记录运行结果, 如果进入ASCENDC_ASSERT报错，会被置为0
// 重定义ASCENDC_ASSERT，不Abort，仅修改全局变量通知进入报错分支
#undef ASCENDC_ASSERT
#define ASCENDC_ASSERT(cond, behavior) \
    do {                               \
        if (!(cond)) {                 \
            g_testRes = 0;              \
            behavior;                  \
        }                              \
    } while (0)

#undef ASCENDC_REPORT_CHECK_ERROR
#define ASCENDC_REPORT_CHECK_ERROR(apiMsg, funcType)   \
    do {                                                \
        g_testRes = 0;                                   \
    } while (0)

#include "kernel_utils.h"
#include "kernel_operator.h"

using namespace std;
using namespace AscendC;


MstxTensor::MstxVecUnaryDesc       g_vec_unary;
MstxTensor::MstxVecBinaryDesc      g_vec_binary;
MstxTensor::MstxVecBilinearInterpolation g_vec_bilinear;
MstxTensor::MstxVecCastDeqDesc     g_vec_cast_deq;
MstxTensor::MstxVecSelDesc         g_vec_sel;
MstxTensor::MstxVecGatherMaskDesc  g_vec_gather_mask;
MstxTensor::MstxVecTranspose       g_vec_transpose;
MstxTensor::MstxVecReduceDesc      g_vec_reduce;
MstxTensor::MstxVecComplexReduceDesc g_vec_cpx_reduce;
MstxTensor::MstxVecDupDesc         g_vec_dup;
MstxTensor::MstxVecBrcbDesc        g_vec_brcb;
MstxTensor::MstxVecCopy            g_vec_copy;
MstxTensor::MstxDataCopyDesc       g_data_copy;
MstxTensor::MstxDataCopyPadDesc    g_data_copy_pad;

extern "C" void __mstx_dfx_report_stub(int type, uint8_t size, void* desc) {
    switch (type) {
        case static_cast<int>(MstxReportType::MSTX_VEC_UNARY):
        case static_cast<int>(MstxReportType::MSTX_VEC_BINARY_SCALAR):
        case static_cast<int>(MstxReportType::MSTX_VEC_TENARY):
        case static_cast<int>(MstxReportType::MSTX_VEC_CAST):
        case static_cast<int>(MstxReportType::MSTX_VEC_CMPS): {
            if (desc && size == sizeof(MstxTensor::MstxVecUnaryDesc)) {
                g_vec_unary = *static_cast<MstxTensor::MstxVecUnaryDesc*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_VEC_BINARY):
        case static_cast<int>(MstxReportType::MSTX_VEC_CMP): {
            if (desc && size == sizeof(MstxTensor::MstxVecBinaryDesc)) {
                g_vec_binary = *static_cast<MstxTensor::MstxVecBinaryDesc*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_VEC_BILINEAR_INTERPOLATION): {
            if (desc && size == sizeof(MstxTensor::MstxVecBilinearInterpolation)) {
                g_vec_bilinear = *static_cast<MstxTensor::MstxVecBilinearInterpolation*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_VEC_CASTDEQ): {
            if (desc && size == sizeof(MstxTensor::MstxVecCastDeqDesc)) {
                g_vec_cast_deq = *static_cast<MstxTensor::MstxVecCastDeqDesc*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_VEC_SEL): {
            if (desc && size == sizeof(MstxTensor::MstxVecSelDesc)) {
                g_vec_sel = *static_cast<MstxTensor::MstxVecSelDesc*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_VEC_GATHER_MASK): {
            if (desc && size == sizeof(MstxTensor::MstxVecGatherMaskDesc)) {
                g_vec_gather_mask = *static_cast<MstxTensor::MstxVecGatherMaskDesc*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_VEC_TRANSDATA):
        case static_cast<int>(MstxReportType::MSTX_VEC_TRANSPOSE): {
            if (desc && size == sizeof(MstxTensor::MstxVecTranspose)) {
                g_vec_transpose = *static_cast<MstxTensor::MstxVecTranspose*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_VEC_WHOLE_REDUCE):
        case static_cast<int>(MstxReportType::MSTX_VEC_BLK_REDUCE):
        case static_cast<int>(MstxReportType::MSTX_VEC_PAIR_REDUCE):
        case static_cast<int>(MstxReportType::MSTX_VEC_REPEAT_REDUCE): {
            if (desc && size == sizeof(MstxTensor::MstxVecReduceDesc)) {
                g_vec_reduce = *static_cast<MstxTensor::MstxVecReduceDesc*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_VEC_REDUCE): {
            if (desc && size == sizeof(MstxTensor::MstxVecComplexReduceDesc)) {
                g_vec_complex_reduce = *static_cast<MstxTensor::MstxVecComplexReduceDesc*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_VEC_DUP):
        case static_cast<int>(MstxReportType::MSTX_VEC_VCI): {
            if (desc && size == sizeof(MstxTensor::MstxVecDupDesc)) {
                g_vec_dup = *static_cast<MstxTensor::MstxVecDupDesc*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_VEC_BROADCAST): {
            if (desc && size == sizeof(MstxTensor::MstxVecBrcbDesc)) {
                g_vec_brcb = *static_cast<MstxTensor::MstxVecBrcbDesc*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_VEC_COPY): {
            if (desc && size == sizeof(MstxTensor::MstxVecCopy)) {
                g_vec_copy = *static_cast<MstxTensor::MstxVecCopy*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_DATA_COPY): {
            if (desc && size == sizeof(MstxTensor::MstxDataCopyDesc)) {
                g_data_copy = *static_cast<MstxTensor::MstxDataCopyDesc*>(desc);
            }
            break;
        }

        case static_cast<int>(MstxReportType::MSTX_DATA_COPY_PAD): {
            if (desc && size == sizeof(MstxTensor::MstxDataCopyPadDesc)) {
                g_data_copy_pad = *static_cast<MstxTensor::MstxDataCopyPadDesc*>(desc);
            }
            break;
        }

        default:
            break;
    }
}


void CopyName(char b[64], const char* a)
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

 template <typename T>
MstxTensor::MstxTensorAddressSpace GetTensorSpace(const LocalTensor<T>& dst)
{
    Hardware dstHWPos = GetPhyType((TPosition)dst.GetPosition());
    if (dstHWPos == Hardware::GM) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_GM;
    } else if (dstHWPos == Hardware::UB) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_UB;
    } else if (dstHWPos == Hardware::L1) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_L1;
    } else if (dstHWPos == Hardware::L0A) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_L0A;
    } else if (dstHWPos == Hardware::L0B) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_L0B;
    } else if (dstHWPos == Hardware::L0C) {
        return AscendC::MstxTensor::MSTX_TENSOR_AT_L0C;
    }
}

void CheckResult(MstxTensor::MstxVecUnaryDesc& expect_desc, MstxTensor::MstxVecUnaryDesc& g_vec_unary)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_unary.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_unary.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_unary.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_unary.dst.dataBits);
    EXPECT_EQ(expect_desc.src.space, g_vec_unary.src.space);
    EXPECT_EQ(expect_desc.src.addr, g_vec_unary.src.addr);
    EXPECT_EQ(expect_desc.src.size, g_vec_unary.src.size);
    EXPECT_EQ(expect_desc.src.dataBits, g_vec_unary.src.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_unary.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_unary.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_unary.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_unary.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_unary.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.blockNum, g_vec_unary.blockNum);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_unary.repeatTimes);
    EXPECT_EQ(expect_desc.dstBlockStride, g_vec_unary.dstBlockStride);
    EXPECT_EQ(expect_desc.srcBlockStride, g_vec_unary.srcBlockStride);
    EXPECT_EQ(expect_desc.dstRepeatStride, g_vec_unary.dstRepeatStride);
    EXPECT_EQ(expect_desc.srcRepeatStride, g_vec_unary.srcRepeatStride);
    EXPECT_EQ(expect_desc.name[0], g_vec_unary.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_unary.name[1]);
}

void CheckResult(MstxTensor::MstxVecBinaryDesc& expect_desc, MstxTensor::MstxVecBinaryDesc& g_vec_binary)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_binary.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_binary.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_binary.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_binary.dst.dataBits);
    EXPECT_EQ(expect_desc.src0.space, g_vec_binary.src0.space);
    EXPECT_EQ(expect_desc.src0.addr, g_vec_binary.src0.addr);
    EXPECT_EQ(expect_desc.src0.size, g_vec_binary.src0.size);
    EXPECT_EQ(expect_desc.src0.dataBits, g_vec_binary.src0.dataBits);
    EXPECT_EQ(expect_desc.src1.space, g_vec_binary.src1.space);
    EXPECT_EQ(expect_desc.src1.addr, g_vec_binary.src1.addr);
    EXPECT_EQ(expect_desc.src1.size, g_vec_binary.src1.size);
    EXPECT_EQ(expect_desc.src1.dataBits, g_vec_binary.src1.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_binary.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_binary.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_binary.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_binary.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_binary.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.blockNum, g_vec_binary.blockNum);
    EXPECT_EQ(expect_desc.dstBlockStride, g_vec_binary.dstBlockStride);
    EXPECT_EQ(expect_desc.src0BlockStride, g_vec_binary.src0BlockStride);
    EXPECT_EQ(expect_desc.src1BlockStride, g_vec_binary.src1BlockStride);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_binary.repeatTimes);
    EXPECT_EQ(expect_desc.dstRepeatStride, g_vec_binary.dstRepeatStride);
    EXPECT_EQ(expect_desc.src0RepeatStride, g_vec_binary.src0RepeatStride);
    EXPECT_EQ(expect_desc.src1RepeatStride, g_vec_binary.src1RepeatStride);
    EXPECT_EQ(expect_desc.name[0], g_vec_binary.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_binary.name[1]);
}

void CheckResult(MstxTensor::MstxVecReduceDesc& expect_desc, MstxTensor::MstxVecReduceDesc& g_vec_reduce)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_reduce.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_reduce.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_reduce.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_reduce.dst.dataBits);
    EXPECT_EQ(expect_desc.src.space, g_vec_reduce.src.space);
    EXPECT_EQ(expect_desc.src.addr, g_vec_reduce.src.addr);
    EXPECT_EQ(expect_desc.src.size, g_vec_reduce.src.size);
    EXPECT_EQ(expect_desc.src.dataBits, g_vec_reduce.src.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_reduce.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_reduce.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_reduce.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_reduce.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_reduce.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.srcBlockStride, g_vec_reduce.srcBlockStride);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_reduce.repeatTimes);
    EXPECT_EQ(expect_desc.dstRepeatStride, g_vec_reduce.dstRepeatStride);
    EXPECT_EQ(expect_desc.srcRepeatStride, g_vec_reduce.srcRepeatStride);
    EXPECT_EQ(expect_desc.name[0], g_vec_reduce.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_reduce.name[1]);
}

void CheckResult(MstxTensor::MstxVecBrcbDesc& expect_desc, MstxTensor::MstxVecBrcbDesc& g_vec_brcb)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_brcb.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_brcb.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_brcb.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_brcb.dst.dataBits);
    EXPECT_EQ(expect_desc.src.space, g_vec_brcb.src.space);
    EXPECT_EQ(expect_desc.src.addr, g_vec_brcb.src.addr);
    EXPECT_EQ(expect_desc.src.size, g_vec_brcb.src.size);
    EXPECT_EQ(expect_desc.src.dataBits, g_vec_brcb.src.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_brcb.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_brcb.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_brcb.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_brcb.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_brcb.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.dstBlockStride, g_vec_brcb.dstBlockStride);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_brcb.repeatTimes);
    EXPECT_EQ(expect_desc.dstRepeatStride, g_vec_brcb.dstRepeatStride);
    EXPECT_EQ(expect_desc.name[0], g_vec_brcb.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_brcb.name[1]);
}

void CheckResult(MstxTensor::MstxVecComplexReduceDesc& expect_desc, MstxTensor::MstxVecComplexReduceDesc& g_vec_cpx_reduce)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_cpx_reduce.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_cpx_reduce.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_cpx_reduce.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_cpx_reduce.dst.dataBits);
    EXPECT_EQ(expect_desc.src.space, g_vec_cpx_reduce.src.space);
    EXPECT_EQ(expect_desc.src.addr, g_vec_cpx_reduce.src.addr);
    EXPECT_EQ(expect_desc.src.size, g_vec_cpx_reduce.src.size);
    EXPECT_EQ(expect_desc.src.dataBits, g_vec_cpx_reduce.src.dataBits);
    EXPECT_EQ(expect_desc.tmp.space, g_vec_cpx_reduce.tmp.space);
    EXPECT_EQ(expect_desc.tmp.addr, g_vec_cpx_reduce.tmp.addr);
    EXPECT_EQ(expect_desc.tmp.size, g_vec_cpx_reduce.tmp.size);
    EXPECT_EQ(expect_desc.tmp.dataBits, g_vec_cpx_reduce.tmp.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_cpx_reduce.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_cpx_reduce.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_cpx_reduce.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_cpx_reduce.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_cpx_reduce.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_cpx_reduce.repeatTimes);
    EXPECT_EQ(expect_desc.srcRepeatStride, g_vec_cpx_reduce.srcRepeatStride);
    EXPECT_EQ(expect_desc.name[0], g_vec_cpx_reduce.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_cpx_reduce.name[1]);
}

void CheckResult(MstxTensor::MstxVecDupDesc& expect_desc, MstxTensor::MstxVecDupDesc& g_vec_dup)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_dup.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_dup.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_dup.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_dup.dst.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_dup.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_dup.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_dup.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_dup.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_dup.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.dstBlockStride, g_vec_dup.dstBlockStride);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_dup.repeatTimes);
    EXPECT_EQ(expect_desc.dstRepeatStride, g_vec_dup.dstRepeatStride);
    EXPECT_EQ(expect_desc.name[0], g_vec_dup.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_dup.name[1]);
}

void CheckResult(MstxTensor::MstxVecCopy& expect_desc, MstxTensor::MstxVecCopy& g_vec_copy)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_copy.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_copy.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_copy.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_copy.dst.dataBits);
    EXPECT_EQ(expect_desc.src.space, g_vec_copy.src.space);
    EXPECT_EQ(expect_desc.src.addr, g_vec_copy.src.addr);
    EXPECT_EQ(expect_desc.src.size, g_vec_copy.src.size);
    EXPECT_EQ(expect_desc.src.dataBits, g_vec_copy.src.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_copy.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_copy.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_copy.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_copy.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_copy.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_copy.repeatTimes);
    EXPECT_EQ(expect_desc.dstStride, g_vec_copy.dstStride);
    EXPECT_EQ(expect_desc.srcStride, g_vec_copy.srcStride);
    EXPECT_EQ(expect_desc.dstRepeatSize, g_vec_copy.dstRepeatSize);
    EXPECT_EQ(expect_desc.srcRepeatSize, g_vec_copy.srcRepeatSize);
    EXPECT_EQ(expect_desc.name[0], g_vec_copy.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_copy.name[1]);
}

void CheckResult(MstxTensor::MstxVecCastDeqDesc& expect_desc, MstxTensor::MstxVecCastDeqDesc& g_vec_cast_deq)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_cast_deq.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_cast_deq.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_cast_deq.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_cast_deq.dst.dataBits);
    EXPECT_EQ(expect_desc.src.space, g_vec_cast_deq.src.space);
    EXPECT_EQ(expect_desc.src.addr, g_vec_cast_deq.src.addr);
    EXPECT_EQ(expect_desc.src.size, g_vec_cast_deq.src.size);
    EXPECT_EQ(expect_desc.src.dataBits, g_vec_cast_deq.src.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_cast_deq.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_cast_deq.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_cast_deq.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_cast_deq.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_cast_deq.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.blockNum, g_vec_cast_deq.blockNum);
    EXPECT_EQ(expect_desc.dstBlockStride, g_vec_cast_deq.dstBlockStride);
    EXPECT_EQ(expect_desc.srcBlockStride, g_vec_cast_deq.srcBlockStride);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_cast_deq.repeatTimes);
    EXPECT_EQ(expect_desc.dstRepeatStride, g_vec_cast_deq.dstRepeatStride);
    EXPECT_EQ(expect_desc.srcRepeatStride, g_vec_cast_deq.srcRepeatStride);
    EXPECT_EQ(expect_desc.halfBlock, g_vec_cast_deq.halfBlock);
    EXPECT_EQ(expect_desc.name[0], g_vec_cast_deq.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_cast_deq.name[1]);
}

void CheckResult(MstxTensor::MstxDataCopyDesc& expect_desc, MstxTensor::MstxDataCopyDesc& g_data_copy)
{
    EXPECT_EQ(expect_desc.dst.space, g_data_copy.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_data_copy.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_data_copy.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_data_copy.dst.dataBits);
    EXPECT_EQ(expect_desc.src.space, g_data_copy.src.space);
    EXPECT_EQ(expect_desc.src.addr, g_data_copy.src.addr);
    EXPECT_EQ(expect_desc.src.size, g_data_copy.src.size);
    EXPECT_EQ(expect_desc.src.dataBits, g_data_copy.src.dataBits);
    EXPECT_EQ(expect_desc.lenBurst, g_data_copy.lenBurst);
    EXPECT_EQ(expect_desc.nBurst, g_data_copy.nBurst);
    EXPECT_EQ(expect_desc.srcGap, g_data_copy.srcGap);
    EXPECT_EQ(expect_desc.dstGap, g_data_copy.dstGap);
    EXPECT_EQ(expect_desc.name[0], g_data_copy.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_data_copy.name[1]);
}

void CheckResult(MstxTensor::MstxDataCopyPadDesc& expect_desc, MstxTensor::MstxDataCopyPadDesc& g_data_copy_pad)
{
    EXPECT_EQ(expect_desc.dst.space, g_data_copy_pad.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_data_copy_pad.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_data_copy_pad.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_data_copy_pad.dst.dataBits);
    EXPECT_EQ(expect_desc.src.space, g_data_copy_pad.src.space);
    EXPECT_EQ(expect_desc.src.addr, g_data_copy_pad.src.addr);
    EXPECT_EQ(expect_desc.src.size, g_data_copy_pad.src.size);
    EXPECT_EQ(expect_desc.src.dataBits, g_data_copy_pad.src.dataBits);
    EXPECT_EQ(expect_desc.lenBurst, g_data_copy_pad.lenBurst);
    EXPECT_EQ(expect_desc.nBurst, g_data_copy_pad.nBurst);
    EXPECT_EQ(expect_desc.srcGap, g_data_copy_pad.srcGap);
    EXPECT_EQ(expect_desc.dstGap, g_data_copy_pad.dstGap);
    EXPECT_EQ(expect_desc.leftPad, g_data_copy_pad.leftPad);
    EXPECT_EQ(expect_desc.rightPad, g_data_copy_pad.rightPad);
    EXPECT_EQ(expect_desc.name[0], g_data_copy_pad.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_data_copy_pad.name[1]);
}

void CheckResult(MstxTensor::MstxVecBilinearInterpolation& expect_desc, MstxTensor::MstxVecBilinearInterpolation& g_vec_bilinear)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_bilinear.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_bilinear.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_bilinear.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_bilinear.dst.dataBits);
    EXPECT_EQ(expect_desc.src0.space, g_vec_bilinear.src0.space);
    EXPECT_EQ(expect_desc.src0.addr, g_vec_bilinear.src0.addr);
    EXPECT_EQ(expect_desc.src0.size, g_vec_bilinear.src0.size);
    EXPECT_EQ(expect_desc.src0.dataBits, g_vec_bilinear.src0.dataBits);
    EXPECT_EQ(expect_desc.src1.space, g_vec_bilinear.src1.space);
    EXPECT_EQ(expect_desc.src1.addr, g_vec_bilinear.src1.addr);
    EXPECT_EQ(expect_desc.src1.size, g_vec_bilinear.src1.size);
    EXPECT_EQ(expect_desc.src1.dataBits, g_vec_bilinear.src1.dataBits);
    EXPECT_EQ(expect_desc.src0Offset.space, g_vec_bilinear.src0Offset.space);
    EXPECT_EQ(expect_desc.src0Offset.addr, g_vec_bilinear.src0Offset.addr);
    EXPECT_EQ(expect_desc.src0Offset.size, g_vec_bilinear.src0Offset.size);
    EXPECT_EQ(expect_desc.src0Offset.dataBits, g_vec_bilinear.src0Offset.dataBits);
    EXPECT_EQ(expect_desc.shared.space, g_vec_bilinear.shared.space);
    EXPECT_EQ(expect_desc.shared.addr, g_vec_bilinear.shared.addr);
    EXPECT_EQ(expect_desc.shared.size, g_vec_bilinear.shared.size);
    EXPECT_EQ(expect_desc.shared.dataBits, g_vec_bilinear.shared.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_bilinear.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_bilinear.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_bilinear.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_bilinear.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_bilinear.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.hRepeat, g_vec_bilinear.hRepeat);
    EXPECT_EQ(expect_desc.repeatMode, g_vec_bilinear.repeatMode);
    EXPECT_EQ(expect_desc.dstBlockStride, g_vec_bilinear.dstBlockStride);
    EXPECT_EQ(expect_desc.vROffset, g_vec_bilinear.vROffset);
    EXPECT_EQ(expect_desc.vRepeat, g_vec_bilinear.vRepeat);
    EXPECT_EQ(expect_desc.name[0], g_vec_bilinear.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_bilinear.name[1]);
}

void CheckResult(MstxTensor::MstxVecTranspose& expect_desc, MstxTensor::MstxVecTranspose& g_vec_transpose)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_transpose.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_transpose.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_transpose.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_transpose.dst.dataBits);
    EXPECT_EQ(expect_desc.src.space, g_vec_transpose.src.space);
    EXPECT_EQ(expect_desc.src.addr, g_vec_transpose.src.addr);
    EXPECT_EQ(expect_desc.src.size, g_vec_transpose.src.size);
    EXPECT_EQ(expect_desc.src.dataBits, g_vec_transpose.src.dataBits);
    EXPECT_EQ(expect_desc.shared.space, g_vec_transpose.shared.space);
    EXPECT_EQ(expect_desc.shared.addr, g_vec_transpose.shared.addr);
    EXPECT_EQ(expect_desc.shared.size, g_vec_transpose.shared.size);
    EXPECT_EQ(expect_desc.shared.dataBits, g_vec_transpose.shared.dataBits);
    EXPECT_EQ(expect_desc.temp, g_vec_transpose.temp);
    EXPECT_EQ(expect_desc.name[0], g_vec_transpose.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_transpose.name[1]);
}

void CheckResult(MstxTensor::MstxVecSelDesc& expect_desc, MstxTensor::MstxVecSelDesc& g_vec_sel)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_sel.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_sel.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_sel.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_sel.dst.dataBits);
    EXPECT_EQ(expect_desc.src0.space, g_vec_sel.src0.space);
    EXPECT_EQ(expect_desc.src0.addr, g_vec_sel.src0.addr);
    EXPECT_EQ(expect_desc.src0.size, g_vec_sel.src0.size);
    EXPECT_EQ(expect_desc.src0.dataBits, g_vec_sel.src0.dataBits);
    EXPECT_EQ(expect_desc.src1.space, g_vec_sel.src1.space);
    EXPECT_EQ(expect_desc.src1.addr, g_vec_sel.src1.addr);
    EXPECT_EQ(expect_desc.src1.size, g_vec_sel.src1.size);
    EXPECT_EQ(expect_desc.src1.dataBits, g_vec_sel.src1.dataBits);
    EXPECT_EQ(expect_desc.mask.space, g_vec_sel.mask.space);
    EXPECT_EQ(expect_desc.mask.addr, g_vec_sel.mask.addr);
    EXPECT_EQ(expect_desc.mask.size, g_vec_sel.mask.size);
    EXPECT_EQ(expect_desc.mask.dataBits, g_vec_sel.mask.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_sel.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_sel.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_sel.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_sel.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_sel.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.scalarMode, g_vec_sel.scalarMode);
    EXPECT_EQ(expect_desc.blockNum, g_vec_sel.blockNum);
    EXPECT_EQ(expect_desc.dstBlockStride, g_vec_sel.dstBlockStride);
    EXPECT_EQ(expect_desc.src0BlockStride, g_vec_sel.src0BlockStride);
    EXPECT_EQ(expect_desc.src1BlockStride, g_vec_sel.src1BlockStride);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_sel.repeatTimes);
    EXPECT_EQ(expect_desc.dstRepeatStride, g_vec_sel.dstRepeatStride);
    EXPECT_EQ(expect_desc.src0RepeatStride, g_vec_sel.src0RepeatStride);
    EXPECT_EQ(expect_desc.src1RepeatStride, g_vec_sel.src1RepeatStride);
    EXPECT_EQ(expect_desc.name[0], g_vec_sel.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_sel.name[1]);
}

void CheckResult(MstxTensor::MstxVecGatherMaskDesc& expect_desc, MstxTensor::MstxVecGatherMaskDesc& g_vec_gather_mask)
{
    EXPECT_EQ(expect_desc.dst.space, g_vec_gather_mask.dst.space);
    EXPECT_EQ(expect_desc.dst.addr, g_vec_gather_mask.dst.addr);
    EXPECT_EQ(expect_desc.dst.size, g_vec_gather_mask.dst.size);
    EXPECT_EQ(expect_desc.dst.dataBits, g_vec_gather_mask.dst.dataBits);
    EXPECT_EQ(expect_desc.src.space, g_vec_gather_mask.src.space);
    EXPECT_EQ(expect_desc.src.addr, g_vec_gather_mask.src.addr);
    EXPECT_EQ(expect_desc.src.size, g_vec_gather_mask.src.size);
    EXPECT_EQ(expect_desc.src.dataBits, g_vec_gather_mask.src.dataBits);
    EXPECT_EQ(expect_desc.wapper.maskMode, g_vec_gather_mask.wapper.maskMode);
    EXPECT_EQ(expect_desc.wapper.mask.mask0, g_vec_gather_mask.wapper.mask.mask0);
    EXPECT_EQ(expect_desc.wapper.mask.mask1, g_vec_gather_mask.wapper.mask.mask1);
    EXPECT_EQ(expect_desc.wapper.useMask, g_vec_gather_mask.wapper.useMask);
    EXPECT_EQ(expect_desc.wapper.reserveBufSize, g_vec_gather_mask.wapper.reserveBufSize);
    EXPECT_EQ(expect_desc.mode, g_vec_gather_mask.mode);
    EXPECT_EQ(expect_desc.repeatTimes, g_vec_gather_mask.repeatTimes);
    EXPECT_EQ(expect_desc.src0BlockStride, g_vec_gather_mask.src0BlockStride);
    EXPECT_EQ(expect_desc.src0RepeatStride, g_vec_gather_mask.src0RepeatStride);
    EXPECT_EQ(expect_desc.src1RepeatStride, g_vec_gather_mask.src1RepeatStride);
    EXPECT_EQ(expect_desc.name[0], g_vec_gather_mask.name[0]);
    EXPECT_EQ(expect_desc.name[1], g_vec_gather_mask.name[1]);
}


template <typename T, bool USE_CAST_DEQ>
void MainVecUnaryMstx01(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, __gm__ int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::VECCALC> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(T));
    LocalTensor<T> inputLocal = tbuf.Get<T>();

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, dataSize * sizeof(int16_t));
    LocalTensor<int16_t> inputLocal2 = tbuf1.Get<int16_t>();

    TBuf<TPosition::VECCALC> tbuf2;
    tpipe.InitBuffer(tbuf2, dataSize * sizeof(T));
    LocalTensor<T> outputLocal = tbuf2.Get<T>();

    TBuf<TPosition::VECCALC> tbuf3;
    tpipe.InitBuffer(tbuf3, dataSize * 2 * sizeof(int8_t));
    LocalTensor<int8_t> outputLocal2 = tbuf3.Get<int8_t>();

    TBuf<TPosition::VECCALC> tbuf4;
    tpipe.InitBuffer(tbuf4, dataSize * 2 * sizeof(uint8_t));
    LocalTensor<uint8_t> outputLocal3 = tbuf4.Get<uint8_t>();

    AscendCUtils::SetMask<uint8_t>(256);

    DataCopy(inputLocal, inputGlobal, dataSize);

    set_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);
    AscendCUtils::SetMask<uint8_t>(128);
    wait_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);

    uint64_t maskBit[2] = {0x77, 0x88};
    uint64_t maskCounter = 123;//mask相等时，mask0是18446744073709551615，mask1是576460752303423487
    uint8_t repeatTimes = 5;
    UnaryRepeatParams repeatParams = {2, 2, 6, 6};
    int32_t count = 456;
    T scalar = 2;
    RoundMode roundMode = RoundMode::CAST_ROUND;
    MstxTensor::MstxVecUnaryDesc expect_desc;

    expect_desc.dst.space = GetTensorSpace(outputLocal);
    expect_desc.dst.addr = reinterpret_cast<uint64_t>(outputLocal.GetPhyAddr());
    expect_desc.dst.size = outputLocal.GetSize();
    expect_desc.dst.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));
    expect_desc.src.space = GetTensorSpace(inputLocal);
    expect_desc.src.addr = reinterpret_cast<uint64_t>(inputLocal.GetPhyAddr());
    expect_desc.src.size = inputLocal.GetSize();
    expect_desc.src.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));
    expect_desc.wapper.maskMode = MstxTensor::MSTX_MASK_FROM_REG;
    expect_desc.wapper.useMask = true;
    expect_desc.wapper.reserveBufSize = 0;
    expect_desc.blockNum = repeatParams.blockNumber;
    expect_desc.repeatTimes = repeatTimes;
    expect_desc.dstBlockStride = repeatParams.dstBlkStride;
    expect_desc.srcBlockStride = repeatParams.srcBlkStride;
    expect_desc.dstRepeatStride = repeatParams.dstRepStride;
    expect_desc.srcRepeatStride = repeatParams.srcRepStride;
    
    //mask是uint64
    expect_desc.wapper.mask.mask0 = 18446744073709551615;
    expect_desc.wapper.mask.mask1 = 576460752303423487;
    CopyName(expect_desc.name, "Relu");
    Relu(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Exp");
    Exp(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Ln");
    Ln(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Abs");
    Abs(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Reciprocal");
    Reciprocal(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Sqrt");
    Sqrt(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Rsqrt");
    Rsqrt(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Not");
    Not(outputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Adds");
    Adds(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Muls");
    Muls(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Maxs");
    Maxs(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Mins");
    Mins(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftLeft");
    ShiftLeft(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftRight");
    ShiftRight(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "LeakyRelu");
    LeakyRelu(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Axpy");
    Axpy(outputLocal, inputLocal, scalar, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Cast");
    Cast(outputLocal, inputLocal, roundMode, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);


    //mask是[]
    expect_desc.wapper.mask.mask0 = maskBit[0];
    expect_desc.wapper.mask.mask1 = maskBit[1];
    CopyName(expect_desc.name, "Relu");
    Relu(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Exp");
    Exp(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Ln");
    Ln(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Abs");
    Abs(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Reciprocal");
    Reciprocal(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Sqrt");
    Sqrt(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Rsqrt");
    Rsqrt(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Not");
    Not(outputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Adds");
    Adds(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Muls");
    Muls(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Maxs");
    Maxs(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Mins");
    Mins(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftLeft");
    ShiftLeft(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftRight");
    ShiftRight(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "LeakyRelu");
    LeakyRelu(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Axpy");
    Axpy(outputLocal, inputLocal, scalar, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Cast");
    Cast(outputLocal, inputLocal, roundMode,  maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_unary);

    //count
    expect_desc.wapper.mask.mask0 = count;
    expect_desc.wapper.mask.mask1 = 0;
    expect_desc.blockNum = 8;
    expect_desc.repeatTimes = 1;
    expect_desc.dstBlockStride = 1;
    expect_desc.srcBlockStride = 1;
    expect_desc.dstRepeatStride = 8;
    expect_desc.srcRepeatStride = 8;
    expect_desc.wapper.maskMode = MstxTensor::MSTX_MASK_COUNT;
    CopyName(expect_desc.name, "Relu");
    Relu(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Exp");
    Exp(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Ln");
    Ln(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Abs");
    Abs(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Reciprocal");
    Reciprocal(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Sqrt");
    Sqrt(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Rsqrt");
    Rsqrt(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Not");
    Not(outputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Adds");
    Adds(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Muls");
    Muls(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Maxs");
    Maxs(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Mins");
    Mins(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftLeft");
    ShiftLeft(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "ShiftRight");
    ShiftRight(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "LeakyRelu");
    LeakyRelu(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);

    CopyName(expect_desc.name, "Axpy");
    Axpy(outputLocal, inputLocal, scalar, dataSize);
    CheckResult(expect_desc, g_vec_unary);
    CopyName(expect_desc.name, "Cast");
    Cast(outputLocal, inputLocal, roundMode, dataSize);
    CheckResult(expect_desc, g_vec_unary);



    Exp(outputLocal, inputLocal, dataSize);


    set_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);
    wait_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);

    DataCopy(outputGlobal, outputLocal, dataSize);
    pipe_barrier(PIPE_ALL);
}

template <typename T>
void MainVecBinaryMstx02(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, __gm__ int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::VECCALC> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(T));
    LocalTensor<T> inputLocal = tbuf.Get<T>();

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, dataSize * sizeof(T));
    LocalTensor<T> outputLocal = tbuf1.Get<T>();

    AscendCUtils::SetMask<uint8_t>(256);

    DataCopy(inputLocal, inputGlobal, dataSize);

    set_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);
    AscendCUtils::SetMask<uint8_t>(128);
    wait_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);

    MstxVecBinaryDesc repeatParams = { 2, 2, 2, 6, 6, 6 };
    uint64_t maskBit[2] = {0x77, 0x88};
    uint64_t maskCounter = 123;//mask相等时，mask0是18446744073709551615，mask1是576460752303423487
    uint8_t repeatTimes = 5;
    int32_t count = 456;
    MstxTensor::MstxVecBinaryDesc expect_desc;

    expect_desc.dst.space = GetTensorSpace(outputLocal);
    expect_desc.dst.addr = reinterpret_cast<uint64_t>(outputLocal.GetPhyAddr());
    expect_desc.dst.size = outputLocal.GetSize();
    expect_desc.dst.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));
    expect_desc.src0.space = GetTensorSpace(inputLocal);
    expect_desc.src0.addr = reinterpret_cast<uint64_t>(inputLocal.GetPhyAddr());
    expect_desc.src0.size = inputLocal.GetSize();
    expect_desc.src0.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));
    expect_desc.src1 = expect_desc.src0;
    expect_desc.wapper.maskMode = MstxTensor::MSTX_MASK_FROM_REG;
    expect_desc.wapper.useMask = true;
    expect_desc.wapper.reserveBufSize = 0;
    expect_desc.blockNum = repeatParams.blockNumber;
    expect_desc.repeatTimes = repeatTimes;
    expect_desc.dstBlockStride = repeatParams.dstBlkStride;
    expect_desc.src0BlockStride = repeatParams.src0BlkStride;
    expect_desc.src1BlockStride = repeatParams.src1BlkStride;
    expect_desc.dstRepeatStride = repeatParams.dstRepStride;
    expect_desc.src0RepeatStride = repeatParams.src0RepStride;
    expect_desc.src1RepeatStride = repeatParams.src1RepStride;

    
    //mask是uint64
    expect_desc.wapper.mask.mask0 = 18446744073709551615;
    expect_desc.wapper.mask.mask1 = 576460752303423487;
    CopyName(expect_desc.name, "And");
    And(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Or");
    Or(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Add");
    Add(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Sub");
    Sub(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Mul");
    Mul(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Max");
    Max(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Min");
    Min(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Div");
    Div(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "AddRelu");
    AddRelu(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "SubRelu");
    SubRelu(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "AddReluCast");
    AddReluCast(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "SubReluCast");
    SubReluCast(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "MulAddDst");
    MulAddDst(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "MulCast");
    MulCast(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "FusedMulAdd");
    FusedMulAdd(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "FusedMulAddRelu");
    FusedMulAddRelu(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "AddDeqRelu");
    expect_desc.wapper.reserveBufSize = 8192;
    AddDeqRelu(outputLocal, inputLocal, inputLocal, maskCounter, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    

 

    //mask是[]
    expect_desc.wapper.mask.mask0 = maskBit[0];
    expect_desc.wapper.mask.mask1 = maskBit[1];
    CopyName(expect_desc.name, "And");
    And(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Or");
    Or(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Add");
    Add(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Sub");
    Sub(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Mul");
    Mul(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Max");
    Max(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Min");
    Min(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Div");
    Div(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "AddRelu");
    AddRelu(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "SubRelu");
    SubRelu(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "AddReluCast");
    AddReluCast(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "SubReluCast");
    SubReluCast(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "MulAddDst");
    MulAddDst(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "MulCast");
    MulCast(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "FusedMulAdd");
    FusedMulAdd(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "FusedMulAddRelu");
    FusedMulAddRelu(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "AddDeqRelu");
    expect_desc.wapper.reserveBufSize = 8192;
    AddDeqRelu(outputLocal, inputLocal, inputLocal, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Compare");
    expect_desc.wapper.reserveBufSize = 0;
    Compare(outputLocal, inputLocal, inputLocal, 0, maskBit, repeatTimes, repeatParams);
    CheckResult(expect_desc, g_vec_binary);


    //count
    expect_desc.wapper.mask.mask0 = count;
    expect_desc.wapper.mask.mask1 = 0;
    expect_desc.blockNum = 8;
    expect_desc.repeatTimes = 1;
    expect_desc.dstBlockStride = 1;
    expect_desc.srcBlockStride = 1;
    expect_desc.dstRepeatStride = 8;
    expect_desc.srcRepeatStride = 8;
    expect_desc.wapper.maskMode = MstxTensor::MSTX_MASK_COUNT;
    CopyName(expect_desc.name, "And");
    And(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Or");
    Or(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Add");
    Add(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Sub");
    Sub(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Mul");
    Mul(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Max");
    Max(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Min");
    Min(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "Div");
    Div(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "AddRelu");
    AddRelu(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "SubRelu");
    SubRelu(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "AddReluCast");
    AddReluCast(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "SubReluCast");
    SubReluCast(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "MulAddDst");
    MulAddDst(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "MulCast");
    MulCast(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "FusedMulAdd");
    FusedMulAdd(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "FusedMulAddRelu");
    FusedMulAddRelu(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    CopyName(expect_desc.name, "AddDeqRelu");
    expect_desc.wapper.reserveBufSize = 8192;
    AddDeqRelu(outputLocal, inputLocal, inputLocal, count);
    CheckResult(expect_desc, g_vec_binary);
    
  

    
  

    set_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);
    wait_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);

    DataCopy(outputGlobal, outputLocal, dataSize);
    pipe_barrier(PIPE_ALL);
}

template <typename T>
void MainVecUnaryMstx03(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, __gm__ int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::VECCALC> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(T));
    LocalTensor<T> inputLocal = tbuf.Get<T>();

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, dataSize * sizeof(T));
    LocalTensor<T> outputLocal = tbuf1.Get<T>();

    AscendCUtils::SetMask<uint8_t>(256);

    DataCopy(inputLocal, inputGlobal, dataSize);

    set_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);
    AscendCUtils::SetMask<uint8_t>(128);
    wait_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);

    uint64_t maskBit[2] = {0x77, 0x88};
uint64_t maskCounter = 123; // mask相等时，mask0是18446744073709551615，mask1是576460752303423487
uint8_t repeatTimes = 5;
UnaryRepeatParams repeatParams = {2, 2, 6, 6};  // 假设 blockNum, dstBlkStride, srcBlkStride, repStride
int32_t count = 456;
T scalar = 2;
RoundMode roundMode = RoundMode::CAST_ROUND;

MstxTensor::MstxVecBilinearInterpolation expect_desc;

expect_desc.dst.space = GetTensorSpace(outputLocal);
expect_desc.dst.addr = reinterpret_cast<uint64_t>(outputLocal.GetPhyAddr());
expect_desc.dst.size = outputLocal.GetSize();
expect_desc.dst.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));

expect_desc.src0.space = GetTensorSpace(inputLocal0);  // 假设有 inputLocal0
expect_desc.src0.addr = reinterpret_cast<uint64_t>(inputLocal0.GetPhyAddr());
expect_desc.src0.size = inputLocal0.GetSize();
expect_desc.src0.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));

expect_desc.src1.space = GetTensorSpace(inputLocal1);  // 假设有 inputLocal1
expect_desc.src1.addr = reinterpret_cast<uint64_t>(inputLocal1.GetPhyAddr());
expect_desc.src1.size = inputLocal1.GetSize();
expect_desc.src1.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));

expect_desc.src0Offset.space = GetTensorSpace(offsetLocal);  // 偏移量张量，假设名为 offsetLocal
expect_desc.src0Offset.addr = reinterpret_cast<uint64_t>(offsetLocal.GetPhyAddr());
expect_desc.src0Offset.size = offsetLocal.GetSize();
expect_desc.src0Offset.dataBits = static_cast<uint8_t>(8 * sizeof(uint32_t));  // 通常 offset 是 int32/uint32

expect_desc.shared.space = GetTensorSpace(sharedLocal);  // 共享内存区域，如临时 buffer
expect_desc.shared.addr = reinterpret_cast<uint64_t>(sharedLocal.GetPhyAddr());
expect_desc.shared.size = sharedLocal.GetSize();
expect_desc.shared.dataBits = static_cast<uint8_t>(8 * sizeof(PrimT<T>));  // 和数据类型一致

expect_desc.wapper.maskMode = MstxTensor::MSTX_MASK_FROM_REG;
expect_desc.wapper.useMask = true;
expect_desc.wapper.reserveBufSize = 0;
expect_desc.wapper.mask.mask0 = maskBit[0];
expect_desc.wapper.mask.mask1 = maskBit[1];

expect_desc.hRepeat = repeatTimes;  // 横向重复次数，对应 repeatTimes
expect_desc.repeatMode = false;     // 可配置：是否启用复杂重复模式？默认 false 即可
expect_desc.dstBlockStride = repeatParams.dstBlkStride;  // 来自 repeatParams
expect_desc.vROffset = 0;           // 纵向偏移步长（Vertical Repeat Offset），具体值需根据算法确定
expect_desc.vRepeat = 1;            // 纵向重复次数（Vertical Repeat），常为 1 或动态计算




    Not(outputLocal, inputLocal, dataSize);





    set_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);
    wait_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);

    DataCopy(outputGlobal, outputLocal, dataSize);
    pipe_barrier(PIPE_ALL);
}

template <typename T>
void MainVecUnaryMstx04(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, __gm__ int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::VECCALC> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(T));
    LocalTensor<T> inputLocal = tbuf.Get<T>();

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, dataSize * sizeof(T));
    LocalTensor<T> outputLocal = tbuf1.Get<T>();

    AscendCUtils::SetMask<uint8_t>(256);

    DataCopy(inputLocal, inputGlobal, dataSize);

    set_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);
    AscendCUtils::SetMask<uint8_t>(128);
    wait_flag(PIPE_MTE2, PIPE_V, EVENT_ID0);

    Abs(outputLocal, inputLocal, dataSize);

    set_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);
    wait_flag(PIPE_V, PIPE_MTE3, EVENT_ID0);

    DataCopy(outputGlobal, outputLocal, dataSize);
    pipe_barrier(PIPE_ALL);
}

struct UnaryTestParamsMstx {
    int32_t dataSize;
    int32_t dataBitSize;
    void (*CalFunc)(uint8_t*, uint8_t*, int32_t);
};

class UnarySimpleMstxTestsuite : public testing::Test, public testing::WithParamInterface<UnaryTestParamsMstx> {
protected:
    void SetUp()
    {
        AscendC::SetGCoreType(2);
    }
    void TearDown()
    {
        AscendC::SetGCoreType(0);
    }
};

INSTANTIATE_TEST_CASE_P(TEST_UNARY_SIMPLE_MSTX, UnarySimpleMstxTestsuite,
    ::testing::Values(UnaryTestParamsMstx { 208, 2, MainVecUnaryMstx01<half, true> },
    UnaryTestParamsMstx { 208, 4, MainVecUnaryMstx01<float, true> }, UnaryTestParamsMstx { 208, 2, MainVecUnaryMstx01<half, false> }, 
    UnaryTestParamsMstx { 208, 4, MainVecUnaryMstx01<float, false> }, UnaryTestParamsMstx { 208, 4, MainVecBinaryMstx02<int32_t> }, 
    UnaryTestParamsMstx { 208, 4, MainVecBinaryMstx02<half> }, UnaryTestParamsMstx { 208, 4, MainVecBinaryMstx02<float> },
    UnaryTestParamsMstx { 208, 2, MainVecUnaryMstx03<int16_t> }, UnaryTestParamsMstx { 208, 2, MainVecUnaryMstx03<uint16_t> },
    UnaryTestParamsMstx { 208, 2, MainVecUnaryMstx04<int16_t> }));

TEST_P(UnarySimpleMstxTestsuite, UnarySimpleMstxTestCase)
{
    auto param = GetParam();
    uint8_t srcGm[param.dataSize * param.dataBitSize];
    uint8_t dstGm[param.dataSize * param.dataBitSize];

    param.CalFunc(srcGm, dstGm, param.dataSize);
    for (int32_t i = 0; i < param.dataSize; i++) {
        EXPECT_EQ(dstGm[i], 0x00);
    }
}
