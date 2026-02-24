/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"
#include <mockcpp/mockcpp.hpp>

class Tensor_Api_fixpipe_quant_nz2nd : public testing::Test {
protected:
    void SetUp()
    {
        AscendC::SetGCoreType(1);
    }
    void TearDown()
    {
        AscendC::SetGCoreType(0);
    }
};

using namespace AscendC::Te;

#define FIXPIPE_SIMPLE_QUANT_NZ2ND(DST_TYPE, SRC_TYPE, QUANTMODE)\
uint8_t dst##DST_TYPE##_##SRC_TYPE##_##QUANTMODE[256 * 256 * sizeof(DST_TYPE)] = {0};\
uint8_t src##DST_TYPE##_##SRC_TYPE##_##QUANTMODE[256 * 256 * sizeof(SRC_TYPE)] = {0};\
void FixpipeQuantNZ2ND##DST_TYPE##_##SRC_TYPE##_##QUANTMODE(__gm__ DST_TYPE* dst, __cc__ SRC_TYPE* src, uint8_t sid, uint16_t NSize, uint16_t MSize,\
    uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)\
{\
    EXPECT_EQ(dst, reinterpret_cast<__gm__ DST_TYPE*>(dst##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
    EXPECT_EQ(src, reinterpret_cast<__cc__ SRC_TYPE*>(src##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
    EXPECT_EQ(QuantPRE, QUANTMODE);\
    EXPECT_TRUE(NZ2ND_EN);\
}\
\
TEST_F(Tensor_Api_fixpipe_quant_nz2nd, SIMPLE_QUANT_NZ2ND##DST_TYPE##_##SRC_TYPE##_##QUANTMODE)\
{\
    auto dstAddr = reinterpret_cast<__gm__ DST_TYPE*>(dst##DST_TYPE##_##SRC_TYPE##_##QUANTMODE);\
    auto gmIterator = MakeGMmemPtr(dstAddr);\
    auto gmMatrixLayout = MakeLayoutForND<DST_TYPE>(16, 16);\
    auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);\
\
    auto srcAddr = reinterpret_cast<__cc__ SRC_TYPE*>(src##DST_TYPE##_##SRC_TYPE##_##QUANTMODE);\
    auto l0cIterator = MakeL0CmemPtr(srcAddr);\
    auto l0cMatrixLayout = MakeL0CLayout(16, 16);\
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);\
\
    MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ DST_TYPE* dst, __cc__ SRC_TYPE* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D,\
        uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN))\
        .times(1)\
        .will(invoke(FixpipeQuantNZ2ND##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
\
    static constexpr FixpipeTrait trait(static_cast<QuantMode_t>(QUANTMODE), false, false, 0, 0);\
    Fixpipe<trait>(gmTensor, l0cTensor, static_cast<uint64_t>(0));\
    GlobalMockObject::verify();\
}

FIXPIPE_SIMPLE_QUANT_NZ2ND(half, float, F322F16);
FIXPIPE_SIMPLE_QUANT_NZ2ND(bfloat16_t, float, F322BF16);
FIXPIPE_SIMPLE_QUANT_NZ2ND(uint8_t, float, QF322B8_PRE);
FIXPIPE_SIMPLE_QUANT_NZ2ND(int8_t, float, QF322B8_PRE);
FIXPIPE_SIMPLE_QUANT_NZ2ND(half, int32_t, DEQF16);
FIXPIPE_SIMPLE_QUANT_NZ2ND(uint8_t, int32_t, REQ8);
FIXPIPE_SIMPLE_QUANT_NZ2ND(int8_t, int32_t, REQ8);


#define FIXPIPE_VECTOR_QUANT_NZ2ND(DST_TYPE, SRC_TYPE, QUANTMODE)\
uint8_t dst_vector##DST_TYPE##_##SRC_TYPE##_##QUANTMODE[256 * 256 * sizeof(DST_TYPE)] = {0};\
uint8_t src_vector##DST_TYPE##_##SRC_TYPE##_##QUANTMODE[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t quant_vector##DST_TYPE##_##SRC_TYPE##_##QUANTMODE[16 * sizeof(uint64_t)] = {0};\
void FixpipeVectorQuantNZ2ND##DST_TYPE##_##SRC_TYPE##_##QUANTMODE(__gm__ DST_TYPE* dst, __cc__ SRC_TYPE* src, uint8_t sid, uint16_t NSize, uint16_t MSize,\
    uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)\
{\
    EXPECT_EQ(dst, reinterpret_cast<__cc__ DST_TYPE*>(dst_vector##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
    EXPECT_EQ(src, reinterpret_cast<__cc__ SRC_TYPE*>(src_vector##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
    EXPECT_EQ(QuantPRE, QUANTMODE);\
    EXPECT_TRUE(NZ2ND_EN);\
}\
\
TEST_F(Tensor_Api_fixpipe_quant_nz2nd, VECTOR_QUANT_NZ2ND##DST_TYPE##_##SRC_TYPE##_##QUANTMODE)\
{\
    auto dstAddr = reinterpret_cast<__gm__ DST_TYPE*>(dst_vector##DST_TYPE##_##SRC_TYPE##_##QUANTMODE);\
    auto gmIterator = MakeGMmemPtr(dstAddr);\
    auto gmMatrixLayout = MakeLayoutForND<DST_TYPE>(16, 16);\
    auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);\
\
    auto srcAddr = reinterpret_cast<__cc__ SRC_TYPE*>(src_vector##DST_TYPE##_##SRC_TYPE##_##QUANTMODE);\
    auto l0cIterator = MakeL0CmemPtr(srcAddr);\
    auto l0cMatrixLayout = MakeL0CLayout(16, 16);\
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);\
\
    auto quantAddr = reinterpret_cast<__cbuf__ uint64_t*>(quant_vector##DST_TYPE##_##SRC_TYPE##_##QUANTMODE);\
    auto qIterator = MakeL1memPtr(quantAddr);\
    auto qMatrixLayout = MakeRowMajorLayout<uint64_t>(1, 16);\
    auto qTensor = MakeTensor(qIterator, qMatrixLayout);\
\
    MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ DST_TYPE* dst, __cc__ SRC_TYPE* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D,\
        uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN))\
        .times(1)\
        .will(invoke(FixpipeVectorQuantNZ2ND##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
\
    static constexpr FixpipeTrait trait(static_cast<QuantMode_t>(QUANTMODE), false, false, 0, 0);\
    Fixpipe<trait>(gmTensor, l0cTensor, qTensor);\
    GlobalMockObject::verify();\
}

FIXPIPE_VECTOR_QUANT_NZ2ND(uint8_t, float, VQF322B8_PRE);
FIXPIPE_VECTOR_QUANT_NZ2ND(int8_t, float, VQF322B8_PRE);
FIXPIPE_VECTOR_QUANT_NZ2ND(half, int32_t, VDEQF16);
FIXPIPE_VECTOR_QUANT_NZ2ND(uint8_t, int32_t, VREQ8);
FIXPIPE_VECTOR_QUANT_NZ2ND(int8_t, int32_t, VREQ8);


#define FIXPIPE_SIMPLE_QUANT_NZ2ND_COORD(DST_TYPE, SRC_TYPE, QUANTMODE)\
uint8_t dst_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE[256 * 256 * sizeof(DST_TYPE)] = {0};\
uint8_t src_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE[256 * 256 * sizeof(SRC_TYPE)] = {0};\
void FixpipeQuantNZ2NDCoord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE(__gm__ DST_TYPE* dst, __cc__ SRC_TYPE* src, uint8_t sid, uint16_t NSize, uint16_t MSize,\
    uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)\
{\
    EXPECT_EQ(dst, reinterpret_cast<__gm__ DST_TYPE*>(dst_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
    EXPECT_EQ(src, reinterpret_cast<__cc__ SRC_TYPE*>(src_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
    EXPECT_EQ(QuantPRE, QUANTMODE);\
    EXPECT_TRUE(NZ2ND_EN);\
}\
\
TEST_F(Tensor_Api_fixpipe_quant_nz2nd, SIMPLE_QUANT_NZ2ND_COORD##DST_TYPE##_##SRC_TYPE##_##QUANTMODE)\
{\
    auto dstAddr = reinterpret_cast<__gm__ DST_TYPE*>(dst_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE);\
    auto gmIterator = MakeGMmemPtr(dstAddr);\
    auto gmMatrixLayout = MakeLayoutForND<DST_TYPE>(16, 16);\
    auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);\
\
    auto srcAddr = reinterpret_cast<__cc__ SRC_TYPE*>(src_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE);\
    auto l0cIterator = MakeL0CmemPtr(srcAddr);\
    auto l0cMatrixLayout = MakeL0CLayout(16, 16);\
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);\
\
    auto coordM = AscendC::Std::Int<0>{};\
    auto coordN = AscendC::Std::Int<0>{};\
    auto coord = MakeCoord(coordM, coordN);\
\
    MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ DST_TYPE* dst, __cc__ SRC_TYPE* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D,\
        uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN))\
        .times(1)\
        .will(invoke(FixpipeQuantNZ2NDCoord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
\
    static constexpr FixpipeTrait trait(static_cast<QuantMode_t>(QUANTMODE), false, false, 0, 0);\
    Fixpipe<trait>(gmTensor, l0cTensor, static_cast<uint64_t>(0), coord);\
    GlobalMockObject::verify();\
}

FIXPIPE_SIMPLE_QUANT_NZ2ND_COORD(half, float, F322F16);
FIXPIPE_SIMPLE_QUANT_NZ2ND_COORD(bfloat16_t, float, F322BF16);
FIXPIPE_SIMPLE_QUANT_NZ2ND_COORD(uint8_t, float, QF322B8_PRE);
FIXPIPE_SIMPLE_QUANT_NZ2ND_COORD(int8_t, float, QF322B8_PRE);
FIXPIPE_SIMPLE_QUANT_NZ2ND_COORD(half, int32_t, DEQF16);
FIXPIPE_SIMPLE_QUANT_NZ2ND_COORD(uint8_t, int32_t, REQ8);
FIXPIPE_SIMPLE_QUANT_NZ2ND_COORD(int8_t, int32_t, REQ8);


#define FIXPIPE_VECTOR_QUANT_NZ2ND_COORD(DST_TYPE, SRC_TYPE, QUANTMODE)\
uint8_t dst_vector_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE[256 * 256 * sizeof(DST_TYPE)] = {0};\
uint8_t src_vector_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t quant_vector_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE[16 * sizeof(uint64_t)] = {0};\
void FixpipeVectorQuantNZ2NDCoord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE(__gm__ DST_TYPE* dst, __cc__ SRC_TYPE* src, uint8_t sid, uint16_t NSize, uint16_t MSize,\
    uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)\
{\
    EXPECT_EQ(dst, reinterpret_cast<__cc__ DST_TYPE*>(dst_vector_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
    EXPECT_EQ(src, reinterpret_cast<__cc__ SRC_TYPE*>(src_vector_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
    EXPECT_EQ(QuantPRE, QUANTMODE);\
    EXPECT_TRUE(NZ2ND_EN);\
}\
\
TEST_F(Tensor_Api_fixpipe_quant_nz2nd, VECTOR_QUANT_NZ2ND_COORD##DST_TYPE##_##SRC_TYPE##_##QUANTMODE)\
{\
    auto dstAddr = reinterpret_cast<__gm__ DST_TYPE*>(dst_vector_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE);\
    auto gmIterator = MakeGMmemPtr(dstAddr);\
    auto gmMatrixLayout = MakeLayoutForND<DST_TYPE>(16, 16);\
    auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);\
\
    auto srcAddr = reinterpret_cast<__cc__ SRC_TYPE*>(src_vector_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE);\
    auto l0cIterator = MakeL0CmemPtr(srcAddr);\
    auto l0cMatrixLayout = MakeL0CLayout(16, 16);\
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);\
\
    auto quantAddr = reinterpret_cast<__cbuf__ uint64_t*>(quant_vector_coord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE);\
    auto qIterator = MakeL1memPtr(quantAddr);\
    auto qMatrixLayout = MakeRowMajorLayout<uint64_t>(1, 16);\
    auto qTensor = MakeTensor(qIterator, qMatrixLayout);\
\
    auto coordM = AscendC::Std::Int<0>{};\
    auto coordN = AscendC::Std::Int<0>{};\
    auto coord = MakeCoord(coordM, coordN);\
\
    MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ DST_TYPE* dst, __cc__ SRC_TYPE* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D,\
        uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN))\
        .times(1)\
        .will(invoke(FixpipeVectorQuantNZ2NDCoord##DST_TYPE##_##SRC_TYPE##_##QUANTMODE));\
\
    static constexpr FixpipeTrait trait(static_cast<QuantMode_t>(QUANTMODE), false, false, 0, 0);\
    Fixpipe<trait>(gmTensor, l0cTensor, qTensor, coord);\
    GlobalMockObject::verify();\
}

FIXPIPE_VECTOR_QUANT_NZ2ND_COORD(uint8_t, float, VQF322B8_PRE);
FIXPIPE_VECTOR_QUANT_NZ2ND_COORD(int8_t, float, VQF322B8_PRE);
FIXPIPE_VECTOR_QUANT_NZ2ND_COORD(half, int32_t, VDEQF16);
FIXPIPE_VECTOR_QUANT_NZ2ND_COORD(uint8_t, int32_t, VREQ8);
FIXPIPE_VECTOR_QUANT_NZ2ND_COORD(int8_t, int32_t, VREQ8);