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


class TEST_TENSOR_API_FIX_PIPE : public testing::Test {
protected:
    void SetUp() override {
        AscendC::SetGCoreType(1);
    }

    void TearDown() override {
        AscendC::SetGCoreType(0);
    }
};


using namespace AscendC::Te;

// FixPipeIgnore
TEST_F(TEST_TENSOR_API_FIX_PIPE, TestFixPipeRouting_FixPipeIgnoreDoesNothing) {
    static bool callFlag = false;
    __gm__ float dst[16 * 16];
    auto gmIterator = MakeGMmemPtr(dst);
    auto gmMatrixLayout = MakeNZLayout<float>(16, 16);
    auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

    __gm__ float src[16 * 16];
    auto l0cIterator = MakeL0CmemPtr(src);
    auto l0cMatrixLayout = MakeNZLayout<float>(16, 16);
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);

    MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(0);

    // gm -> l0c  Ignore
    Fixpipe<DEFAULT_FIXPIPE_TRAIT>(l0cTensor, gmTensor);

    mockcpp::GlobalMockObject::verify();
}


TEST_F(TEST_TENSOR_API_FIX_PIPE, TestFixPipe_NoQuant_NZ2ND_float_NoCoord) {
        static __gm__ float dstFloat[16 * 16] = {0};
        static __cc__ float srcFloat[16 * 16] = {0};
        auto gmIterator = MakeGMmemPtr(dstFloat);
        auto gmMatrixLayout = MakeRowMajorLayout<float>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        auto l0cIterator = MakeL0CmemPtr(srcFloat);
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ float*, __cc__ float*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, dstFloat);
                EXPECT_EQ(src, srcFloat);
            } 
        ))); 

        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor);

        mockcpp::GlobalMockObject::verify();
}

TEST_F(TEST_TENSOR_API_FIX_PIPE, TestFixPipe_NoQuant_NZ2ND_float_Coord) {
        static __gm__ float dstFloat[16 * 16] = {0};
        static __cc__ float srcFloat[16 * 16] = {0};
        // __gm__ float dst[16 * 16 * sizeof(float)] = {0};
        auto gmIterator = MakeGMmemPtr(dstFloat);
        auto gmMatrixLayout = MakeRowMajorLayout<float>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        // __cc__ float src[16 * 16 * sizeof(float)] = {0};
        auto l0cIterator = MakeL0CmemPtr(srcFloat);
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ float*, __cc__ float*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, dstFloat + 16 * 1 + 1);
                EXPECT_EQ(src, srcFloat);
            } 
        ))); 
        auto coord = MakeCoord(AscendC::Std::Int<1>{}, AscendC::Std::Int<1>{});
        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor, coord);

        mockcpp::GlobalMockObject::verify();
}


TEST_F(TEST_TENSOR_API_FIX_PIPE, TestFixPipe_NoQuant_NZ2ND_int32_t_NoCoord) {
        static __gm__ int32_t dstInt32_t[16 * 16] = {0};
        static __cc__ int32_t srcInt32_t[16 * 16] = {0};
        auto gmIterator = MakeGMmemPtr(dstInt32_t);
        auto gmMatrixLayout = MakeRowMajorLayout<int32_t>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        auto l0cIterator = MakeL0CmemPtr(srcInt32_t);
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ int32_t*, __cc__ int32_t*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, dstInt32_t);
                EXPECT_EQ(src, srcInt32_t);
            } 
        ))); 

        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor);

        mockcpp::GlobalMockObject::verify();
}

TEST_F(TEST_TENSOR_API_FIX_PIPE, TestFixPipe_NoQuant_NZ2ND_int32_t_Coord) {
        static __gm__ int32_t dstInt32_t[16 * 16] = {0};
        static __cc__ int32_t srcInt32_t[16 * 16] = {0};
        auto gmIterator = MakeGMmemPtr(dstInt32_t);
        auto gmMatrixLayout = MakeRowMajorLayout<int32_t>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        auto l0cIterator = MakeL0CmemPtr(srcInt32_t);
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ int32_t*, __cc__ int32_t*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, dstInt32_t + 16 * 1 + 1);
                EXPECT_EQ(src, srcInt32_t);
            } 
        ))); 
        auto coord = MakeCoord(AscendC::Std::Int<1>{}, AscendC::Std::Int<1>{});
        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor, coord);

        mockcpp::GlobalMockObject::verify();
}

TEST_F(TEST_TENSOR_API_FIX_PIPE, TestFixPipe_NoQuant_NZ2NZ_float_NoCoord) {
        static __gm__ float dstFloat[16 * 16] = {0};
        static __cc__ float srcFloat[16 * 16] = {0};
        auto gmIterator = MakeGMmemPtr(dstFloat);
        auto gmMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        auto l0cIterator = MakeL0CmemPtr(srcFloat);
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ float*, __cc__ float*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, dstFloat);
                EXPECT_EQ(src, srcFloat);
            } 
        ))); 

        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor);

        mockcpp::GlobalMockObject::verify();
}

TEST_F(TEST_TENSOR_API_FIX_PIPE, TestFixPipe_NoQuant_NZ2NZ_float_Coord) {
        static __gm__ float dstFloat[16 * 16] = {0};
        static __cc__ float srcFloat[16 * 16] = {0};
        auto gmIterator = MakeGMmemPtr(dstFloat);
        auto gmMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        auto l0cIterator = MakeL0CmemPtr(srcFloat);
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ float*, __cc__ float*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, dstFloat);
                EXPECT_EQ(src, srcFloat);
            } 
        ))); 
        auto coord = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor, coord);

        mockcpp::GlobalMockObject::verify();
}


TEST_F(TEST_TENSOR_API_FIX_PIPE, TestFixPipe_NoQuant_NZ2NZ_int32_t_NoCoord) {
        static __gm__ int32_t dstInt32_t[16 * 16] = {0};
        static __cc__ int32_t srcInt32_t[16 * 16] = {0};
        auto gmIterator = MakeGMmemPtr(dstInt32_t);
        auto gmMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        auto l0cIterator = MakeL0CmemPtr(srcInt32_t);
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ int32_t*, __cc__ int32_t*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, dstInt32_t);
                EXPECT_EQ(src, srcInt32_t);
            } 
        ))); 

        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor);

        mockcpp::GlobalMockObject::verify();
}

TEST_F(TEST_TENSOR_API_FIX_PIPE, TestFixPipe_NoQuant_NZ2NZ_int32_t_Coord) {
        static __gm__ int32_t dstInt32_t[16 * 16] = {0};
        static __cc__ int32_t srcInt32_t[16 * 16] = {0};
        auto gmIterator = MakeGMmemPtr(dstInt32_t);
        auto gmMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        auto l0cIterator = MakeL0CmemPtr(srcInt32_t);
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ int32_t*, __cc__ int32_t*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, dstInt32_t);
                EXPECT_EQ(src, srcInt32_t);
            } 
        ))); 
        auto coord = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor, coord);

        mockcpp::GlobalMockObject::verify();
}

