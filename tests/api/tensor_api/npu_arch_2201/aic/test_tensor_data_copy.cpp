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
#include <mockcpp/mockcpp.hpp>
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"

using namespace AscendC::Te;
using namespace AscendC::Std;

enum class MemoryLayout {
    RowMajor,
    NZ,
    ColumnMajor,
    ZN,
};

enum class Location {
    GM,
    L1,
    Bias,
    Fixbuf,
};

enum class Prefix {
    gm,
    cbuf,
    biasbuf,
    fbuf,
};


class TEST_DATA_COPY : public testing::Test {
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

// ==================== Constants ====================
constexpr int TEST_FRACTAL_FIXED = 16;
constexpr int TEST_C0_SIZE = 32;
constexpr int TEST_L12BT_UNIT = TEST_C0_SIZE * 2;          // 64
constexpr int TEST_C2PIPE2GM_UNIT = TEST_C0_SIZE * 4;      // 128

constexpr int TestCeilDivision(int value, int divisor) {
    return (value + divisor - 1) / divisor;
}

// ND2ND: CopyGmToCbufNDBase
template<typename DTYPE, int SRC_SIZE1, int SRC_SIZE2, int DST_SIZE1, int DST_SIZE2>
__aicore__ inline void copy_gm_to_cbuf_nd2nd_stub(__cbuf__ void* dst, __gm__ void* src, uint8_t sid, \
    uint16_t blockCount, uint16_t blockLen, uint16_t srcStride, uint16_t dstStride, pad_t padValue) {
    EXPECT_EQ(sid, 0);
    EXPECT_EQ(blockCount, DST_SIZE1);
    EXPECT_EQ(blockLen, DST_SIZE2 * sizeof(DTYPE));
    EXPECT_EQ(srcStride, SRC_SIZE2 * sizeof(DTYPE));
    EXPECT_EQ(dstStride, DST_SIZE2 * sizeof(DTYPE));
}

// NZ2NZ: CopyGmToCbufNZBase
template<typename DTYPE, int SRC_SIZE1, int SRC_SIZE2, int DST_SIZE1, int DST_SIZE2>
__aicore__ inline void copy_gm_to_cbuf_nz2nz_stub(__cbuf__ void* dst, __gm__ void* src, uint8_t sid, \
    uint16_t blockCount, uint16_t blockLen, uint16_t srcStride, uint16_t dstStride, pad_t padValue) {
    EXPECT_EQ(sid, 0);
    EXPECT_EQ(blockCount, DST_SIZE2 * sizeof(DTYPE) / TEST_C0_SIZE);
    EXPECT_EQ(blockLen, DST_SIZE1 * TEST_C0_SIZE);
    EXPECT_EQ(srcStride, SRC_SIZE1 * TEST_C0_SIZE);
    EXPECT_EQ(dstStride, DST_SIZE1 * TEST_C0_SIZE);
}

// ND2NZ: CopyGmToCbufMultiND2NZBase
template<typename DTYPE, int SRC_SIZE1, int SRC_SIZE2, int DST_SIZE1, int DST_SIZE2>
__aicore__ inline void copy_gm_to_cbuf_multi_nd2nz_stub(__cbuf__ DTYPE* dst, __gm__ DTYPE* src, uint8_t sid,
            uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue,
            uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride) {
    EXPECT_EQ(sid, 0);
    EXPECT_EQ(ndNum, 1);
    EXPECT_EQ(srcNdMatrixStride, 0);
    EXPECT_EQ(dstNzMatrixStride, 0);
    EXPECT_EQ(nValue, DST_SIZE1);
    EXPECT_EQ(dValue, DST_SIZE2);
    EXPECT_EQ(srcDValue, SRC_SIZE2);
    EXPECT_EQ(dstNzC0Stride, DST_SIZE1);
    EXPECT_EQ(dstNzNStride, 1);
}

// DN2ZN: CopyGmToCbufMultiDN2ZNBase
template<typename DTYPE, int SRC_SIZE1, int SRC_SIZE2, int DST_SIZE1, int DST_SIZE2>
__aicore__ inline void copy_gm_to_cbuf_multi_dn2zn_stub(__cbuf__ DTYPE* dst, __gm__ DTYPE* src, uint8_t sid,
            uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue,
            uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride) {
    EXPECT_EQ(sid, 0);
    EXPECT_EQ(ndNum, 1);
    EXPECT_EQ(srcNdMatrixStride, 0);
    EXPECT_EQ(dstNzMatrixStride, 0);
    EXPECT_EQ(nValue, DST_SIZE2);
    EXPECT_EQ(dValue, DST_SIZE1);
    EXPECT_EQ(srcDValue, SRC_SIZE1);
    EXPECT_EQ(dstNzC0Stride, DST_SIZE2);
    EXPECT_EQ(dstNzNStride, 1);
}

// L1 -> BIAS: CopyCbufToBT2201
template<typename DTYPE, int SRC_SIZE1, int SRC_SIZE2, int DST_SIZE1, int DST_SIZE2>
__aicore__ inline void copy_cbuf_to_bt_stub(uint64_t dst, __cbuf__ DTYPE* src, uint16_t convControl, uint16_t blockCount, uint16_t blockLen,
                                uint16_t srcStride, uint16_t dstStride) {
    EXPECT_EQ(convControl, 0);
    EXPECT_EQ(blockCount, DST_SIZE1);
    EXPECT_EQ(blockLen, DST_SIZE2 * sizeof(DTYPE) / TEST_L12BT_UNIT);
    EXPECT_EQ(srcStride, (SRC_SIZE2 - DST_SIZE2) * sizeof(DTYPE) / TEST_C0_SIZE);
    EXPECT_EQ(dstStride, (DST_SIZE2 - DST_SIZE2) * sizeof(DTYPE) / TEST_L12BT_UNIT);
}

// L1 -> FIXBUF: CopyCbufToFB2201
template<typename DTYPE, int SRC_SIZE1, int SRC_SIZE2, int DST_SIZE1, int DST_SIZE2>
__aicore__ inline void copy_cbuf_to_fbuf_stub(__fbuf__ void* dst, __cbuf__ void* src, uint16_t blockCount, uint16_t blockLen,
                                uint16_t srcStride, uint16_t dstStride) {
    EXPECT_EQ(blockCount, DST_SIZE1);
    EXPECT_EQ(blockLen, TestCeilDivision(DST_SIZE2 * sizeof(DTYPE), TEST_C2PIPE2GM_UNIT));
    EXPECT_EQ(srcStride, TestCeilDivision(SRC_SIZE2 * sizeof(DTYPE), TEST_C0_SIZE));
    EXPECT_EQ(dstStride, TestCeilDivision(DST_SIZE2 * sizeof(DTYPE), TEST_C2PIPE2GM_UNIT));
}

// datacopy move
#define TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, SRC_PREFIX, SRC_LOCATION, SRC_LAYOUT, DST_PREFIX, DST_LOCATION, DST_LAYOUT) \
    uint8_t srcData[SRC_SIZE1 * SRC_SIZE2 * sizeof(DTYPE)]; \
    uint8_t dstData[DST_SIZE1 * DST_SIZE2 * sizeof(DTYPE)]; \
    \
    auto src = reinterpret_cast<__##SRC_PREFIX##__ DTYPE*>(srcData); \
    auto srcIterator = Make##SRC_LOCATION##memPtr(src); \
    auto srcLayout = Make##SRC_LAYOUT##Layout<DTYPE>(SRC_SIZE1, SRC_SIZE2); \
    auto srcTensor = MakeTensor(srcIterator, srcLayout); \
    \
    auto dst = reinterpret_cast<__##DST_PREFIX##__ DTYPE*>(dstData); \
    auto dstIterator = Make##DST_LOCATION##memPtr(dst); \
    auto dstLayout = Make##DST_LAYOUT##Layout<DTYPE>(DST_SIZE1, DST_SIZE2); \
    auto dstTensor = MakeTensor(dstIterator, dstLayout); \
    \
    DataCopy<DEFAULT_DATA_COPY_TRAIT>(dstTensor, srcTensor);

// datacopy move with coord
#define TEST_DATACOPY_WITH_COORD(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, SRC_PREFIX, SRC_LOCATION, SRC_LAYOUT, DST_PREFIX, DST_LOCATION, DST_LAYOUT) \
    uint8_t srcData[SRC_SIZE1 * SRC_SIZE2 * sizeof(DTYPE)]; \
    uint8_t dstData[DST_SIZE1 * DST_SIZE2 * sizeof(DTYPE)]; \
    \
    auto src = reinterpret_cast<__##SRC_PREFIX##__ DTYPE*>(srcData); \
    auto srcIterator = Make##SRC_LOCATION##memPtr(src); \
    auto srcLayout = Make##SRC_LAYOUT##Layout<DTYPE>(SRC_SIZE1, SRC_SIZE2); \
    auto srcTensor = MakeTensor(srcIterator, srcLayout); \
    \
    auto dst = reinterpret_cast<__##DST_PREFIX##__ DTYPE*>(dstData); \
    auto dstIterator = Make##DST_LOCATION##memPtr(dst); \
    auto dstLayout = Make##DST_LAYOUT##Layout<DTYPE>(DST_SIZE1, DST_SIZE2); \
    auto dstTensor = MakeTensor(dstIterator, dstLayout); \
    \
    DataCopy<DEFAULT_DATA_COPY_TRAIT>(dstTensor, srcTensor,  \
                MakeCoord(MakeCoord(Std::Int<2>{}, Std::Int<2>{}), MakeCoord(Std::Int<2>{}, Std::Int<2>{})));

// ignore branch
#define DATA_COPY_TEST_IGNORE(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2) \ 
    TEST_F(TEST_DATA_COPY, TEST_DATACOPY_IGNORE_##DTYPE##_SRC_SIZE##_##SRC_SIZE1##x##SRC_SIZE2##_DST_SIZE##_##DST_SIZE1##x##DST_SIZE2) \
    { \
        TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, cbuf, L1, RowMajor, gm, GM, RowMajor) \
    }

DATA_COPY_TEST_IGNORE(float, 32, 64, 32, 64)

// GM to l1 ND2ND test case
#define DATA_COPY_TEST_GM2L1_ND2ND(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2) \
    TEST_F(TEST_DATA_COPY, TEST_DATACOPY_GM2L1_ND2ND_##DTYPE##_SRC_SIZE##_##SRC_SIZE1##x##SRC_SIZE2##_DST_SIZE##_##DST_SIZE1##x##DST_SIZE2) \
    { \
        MOCKER_CPP(copy_gm_to_cbuf, void(__cbuf__ void*, __gm__ void*, uint8_t, uint16_t, uint16_t, uint16_t, uint16_t, pad_t)) \
            .times(1) \
            .will(invoke(&copy_gm_to_cbuf_nd2nd_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
        TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, gm, GM, RowMajor, cbuf, L1, RowMajor) \
        GlobalMockObject::verify(); \
    }

DATA_COPY_TEST_GM2L1_ND2ND(float, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_ND2ND(half, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_ND2ND(bfloat16_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_ND2ND(int32_t, 32, 64, 32, 64)

// GM to l1 NZ2NZ test case
#define DATA_COPY_TEST_GM2L1_NZ2NZ(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2) \
    TEST_F(TEST_DATA_COPY, TEST_DATACOPY_GM2L1_NZ2NZ_##DTYPE##_SRC_SIZE##_##SRC_SIZE1##x##SRC_SIZE2##_DST_SIZE##_##DST_SIZE1##x##DST_SIZE2) \
    { \
        MOCKER_CPP(copy_gm_to_cbuf, void(__cbuf__ void*, __gm__ void*, uint8_t, uint16_t, uint16_t, uint16_t, uint16_t, pad_t)) \
            .times(1) \
            .will(invoke(&copy_gm_to_cbuf_nz2nz_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
        TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, gm, GM, NZ, cbuf, L1, NZ) \
        GlobalMockObject::verify(); \
    }

// NZ2NZ - 4 byte types
DATA_COPY_TEST_GM2L1_NZ2NZ(float, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_NZ2NZ(int32_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_NZ2NZ(uint32_t, 32, 64, 32, 64)
// NZ2NZ - 2 byte types
DATA_COPY_TEST_GM2L1_NZ2NZ(half, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_NZ2NZ(bfloat16_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_NZ2NZ(int16_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_NZ2NZ(uint16_t, 32, 64, 32, 64)
// NZ2NZ - 1 byte types
DATA_COPY_TEST_GM2L1_NZ2NZ(int8_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_NZ2NZ(uint8_t, 32, 64, 32, 64)

// GM to L1 ND2NZ test case - B8 (1 byte types: int8_t, uint8_t)
#define DATA_COPY_TEST_GM2L1_ND2NZ_B8(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2) \
    TEST_F(TEST_DATA_COPY, TEST_DATACOPY_GM2L1_ND2NZ_##DTYPE##_##SRC_SIZE1##x##SRC_SIZE2##_##DST_SIZE1##x##DST_SIZE2) \
    { \
        MOCKER_CPP(copy_gm_to_cbuf_multi_nd2nz_b8, void(__cbuf__ DTYPE*, __gm__ DTYPE*, uint8_t, \
            uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t)) \
            .times(1) \
            .will(invoke(&copy_gm_to_cbuf_multi_nd2nz_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
        TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, gm, GM, RowMajor, cbuf, L1, NZ) \
        GlobalMockObject::verify(); \
    }

DATA_COPY_TEST_GM2L1_ND2NZ_B8(int8_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_ND2NZ_B8(uint8_t, 32, 64, 32, 64)

// GM to L1 ND2NZ test case - B16 (2 byte types: half, bfloat16_t, int16_t, uint16_t)
#define DATA_COPY_TEST_GM2L1_ND2NZ_B16(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2) \
    TEST_F(TEST_DATA_COPY, TEST_DATACOPY_GM2L1_ND2NZ_##DTYPE##_##SRC_SIZE1##x##SRC_SIZE2##_##DST_SIZE1##x##DST_SIZE2) \
    { \
        MOCKER_CPP(copy_gm_to_cbuf_multi_nd2nz_b16, void(__cbuf__ DTYPE*, __gm__ DTYPE*, uint8_t, \
            uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t)) \
            .times(1) \
            .will(invoke(&copy_gm_to_cbuf_multi_nd2nz_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
        TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, gm, GM, RowMajor, cbuf, L1, NZ) \
        GlobalMockObject::verify(); \
    }

DATA_COPY_TEST_GM2L1_ND2NZ_B16(bfloat16_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_ND2NZ_B16(half, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_ND2NZ_B16(int16_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_ND2NZ_B16(uint16_t, 32, 64, 32, 64)

// GM to L1 ND2NZ test case - B32S (4 byte types: float, int32_t, uint32_t)
#define DATA_COPY_TEST_GM2L1_ND2NZ_B32S(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2) \
    TEST_F(TEST_DATA_COPY, TEST_DATACOPY_GM2L1_ND2NZ_##DTYPE##_##SRC_SIZE1##x##SRC_SIZE2##_##DST_SIZE1##x##DST_SIZE2) \
    { \
        MOCKER_CPP(copy_gm_to_cbuf_multi_nd2nz_b32s, void(__cbuf__ DTYPE*, __gm__ DTYPE*, uint8_t, \
            uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t)) \
            .times(1) \
            .will(invoke(&copy_gm_to_cbuf_multi_nd2nz_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
        TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, gm, GM, RowMajor, cbuf, L1, NZ) \
        GlobalMockObject::verify(); \
    }

DATA_COPY_TEST_GM2L1_ND2NZ_B32S(float, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_ND2NZ_B32S(int32_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_ND2NZ_B32S(uint32_t, 32, 64, 32, 64)

// GM to L1 DN2ZN test case - B8 (1 byte types: int8_t, uint8_t)
#define DATA_COPY_TEST_GM2L1_DN2ZN_B8(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2) \
    TEST_F(TEST_DATA_COPY, TEST_DATACOPY_GM2L1_DN2ZN_##DTYPE##_##SRC_SIZE1##x##SRC_SIZE2##_##DST_SIZE1##x##DST_SIZE2) \
    { \
        if constexpr (sizeof(DTYPE) == 1) { \
            MOCKER_CPP(copy_gm_to_cbuf_multi_nd2nz_b8, void(__cbuf__ DTYPE*, __gm__ DTYPE*, uint8_t, \
                uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t)) \
                .times(1) \
                .will(invoke(&copy_gm_to_cbuf_multi_dn2zn_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
        } else if constexpr (sizeof(DTYPE) == 2) { \
            MOCKER_CPP(copy_gm_to_cbuf_multi_nd2nz_b16, void(__cbuf__ DTYPE*, __gm__ DTYPE*, uint8_t, \
                uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t)) \
                .times(1) \
                .will(invoke(&copy_gm_to_cbuf_multi_dn2zn_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
        } else if constexpr (sizeof(DTYPE) == 4) { \
            MOCKER_CPP(copy_gm_to_cbuf_multi_nd2nz_b32s, void(__cbuf__ DTYPE*, __gm__ DTYPE*, uint8_t, \
                uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t)) \
                .times(1) \
                .will(invoke(&copy_gm_to_cbuf_multi_dn2zn_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
        } \
        TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, gm, GM, ColumnMajor, cbuf, L1, ZN) \
        GlobalMockObject::verify(); \
    }

DATA_COPY_TEST_GM2L1_DN2ZN_B8(int8_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_DN2ZN_B8(uint8_t, 32, 64, 32, 64)

// // GM to L1 DN2ZN test case - B16 (2 byte types: half, bfloat16_t, int16_t, uint16_t)
// #define DATA_COPY_TEST_GM2L1_DN2ZN_B16(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2) \
//     TEST_F(TEST_DATA_COPY, TEST_DATACOPY_GM2L1_DN2ZN_##DTYPE##_##SRC_SIZE1##x##SRC_SIZE2##_##DST_SIZE1##x##DST_SIZE2) \
//     { \
//         MOCKER_CPP(copy_gm_to_cbuf_multi_nd2nz_b16, void(__cbuf__ DTYPE*, __gm__ DTYPE*, uint8_t, \
//             uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t)) \
//             .times(1) \
//             .will(invoke(&copy_gm_to_cbuf_multi_dn2zn_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
//         TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, gm, GM, ColumnMajor, cbuf, L1, ZN) \
//         GlobalMockObject::verify(); \
//     }

DATA_COPY_TEST_GM2L1_DN2ZN_B8(bfloat16_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_DN2ZN_B8(half, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_DN2ZN_B8(int16_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_DN2ZN_B8(uint16_t, 32, 64, 32, 64)

// // GM to L1 DN2ZN test case - B32S (4 byte types: float, int32_t, uint32_t)
// #define DATA_COPY_TEST_GM2L1_DN2ZN_B32S(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2) \
//     TEST_F(TEST_DATA_COPY, TEST_DATACOPY_GM2L1_DN2ZN_##DTYPE##_##SRC_SIZE1##x##SRC_SIZE2##_##DST_SIZE1##x##DST_SIZE2) \
//     { \
//         MOCKER_CPP(copy_gm_to_cbuf_multi_nd2nz_b32s, void(__cbuf__ DTYPE*, __gm__ DTYPE*, uint8_t, \
//             uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t)) \
//             .times(1) \
//             .will(invoke(&copy_gm_to_cbuf_multi_dn2zn_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
//         TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, gm, GM, ColumnMajor, cbuf, L1, ZN) \
//         GlobalMockObject::verify(); \
//     }

DATA_COPY_TEST_GM2L1_DN2ZN_B8(float, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_DN2ZN_B8(int32_t, 32, 64, 32, 64)
DATA_COPY_TEST_GM2L1_DN2ZN_B8(uint32_t, 32, 64, 32, 64)

// L1 to BIAS ND2ND test case
#define DATA_COPY_TEST_L12BIAS_ND2ND(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2) \
    TEST_F(TEST_DATA_COPY, TEST_DATACOPY_L12BIAS_ND2ND_##DTYPE##_SRC_SIZE##_##SRC_SIZE1##x##SRC_SIZE2##_DST_SIZE##_##DST_SIZE1##x##DST_SIZE2) \
    { \
        MOCKER_CPP(copy_cbuf_to_bt, void(uint64_t, __cbuf__ DTYPE*, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t)) \
            .times(1) \
            .will(invoke(&copy_cbuf_to_bt_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
        TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, cbuf, L1, RowMajor, biasbuf, Bias, RowMajor) \
        GlobalMockObject::verify(); \
    }

DATA_COPY_TEST_L12BIAS_ND2ND(float, 32, 64, 32, 64)
DATA_COPY_TEST_L12BIAS_ND2ND(int32_t, 32, 64, 32, 64)

// L1 to FIXBUF ND2ND test case
#define DATA_COPY_TEST_L12FIXBUF_ND2ND(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2) \
    TEST_F(TEST_DATA_COPY, TEST_DATACOPY_L12FIXBUF_ND2ND_##DTYPE##_SRC_SIZE##_##SRC_SIZE1##x##SRC_SIZE2##_DST_SIZE##_##DST_SIZE1##x##DST_SIZE2) \
    { \
        MOCKER_CPP(copy_cbuf_to_fbuf, void(__fbuf__ void*, __cbuf__ void*, uint16_t, uint16_t, uint16_t, uint16_t)) \
            .times(1) \
            .will(invoke(&copy_cbuf_to_fbuf_stub<DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2>)); \
        TEST_DATACOPY(DTYPE, SRC_SIZE1, SRC_SIZE2, DST_SIZE1, DST_SIZE2, cbuf, L1, RowMajor, fbuf, Fixbuf, RowMajor) \
        GlobalMockObject::verify(); \
    }

// FIXBUF - 8 byte types
DATA_COPY_TEST_L12FIXBUF_ND2ND(double, 32, 64, 32, 64)
DATA_COPY_TEST_L12FIXBUF_ND2ND(int64_t, 32, 64, 32, 64)
DATA_COPY_TEST_L12FIXBUF_ND2ND(uint64_t, 32, 64, 32, 64)
// FIXBUF - 4 byte types
DATA_COPY_TEST_L12FIXBUF_ND2ND(float, 32, 64, 32, 64)
DATA_COPY_TEST_L12FIXBUF_ND2ND(int32_t, 32, 64, 32, 64)
DATA_COPY_TEST_L12FIXBUF_ND2ND(uint32_t, 32, 64, 32, 64)
// FIXBUF - 2 byte types
DATA_COPY_TEST_L12FIXBUF_ND2ND(half, 32, 64, 32, 64)
DATA_COPY_TEST_L12FIXBUF_ND2ND(bfloat16_t, 32, 64, 32, 64)
DATA_COPY_TEST_L12FIXBUF_ND2ND(int16_t, 32, 64, 32, 64)
DATA_COPY_TEST_L12FIXBUF_ND2ND(uint16_t, 32, 64, 32, 64)
// FIXBUF - 1 byte types
DATA_COPY_TEST_L12FIXBUF_ND2ND(int8_t, 32, 64, 32, 64)
DATA_COPY_TEST_L12FIXBUF_ND2ND(uint8_t, 32, 64, 32, 64)
