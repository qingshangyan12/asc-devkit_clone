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


class TEST_FIX_PIPE : public testing::Test {
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
TEST_F(TEST_FIX_PIPE, TestFixPipeRouting_FixPipeIgnoreDoesNothing) {
    static bool callFlag = false;
    float dst[16 * 16 * sizeof(float)];
    auto dstAddr = reinterpret_cast<__gm__ float*>(dst);
    auto gmIterator = MakeGMmemPtr(dstAddr);
    auto gmMatrixLayout = MakeNZLayout<float>(16, 16);
    auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

    float src[16 * 16 * sizeof(float)];
    auto srcAddr = reinterpret_cast<__cc__ float*>(src);
    auto l0cIterator = MakeL0CmemPtr(srcAddr);
    auto l0cMatrixLayout = MakeNZLayout<float>(16, 16);
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);

    MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ float*, __cc__ float*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                callFlag = true;
            } 
        ))); 

    // gm -> l0c  Ignore
    Fixpipe<DEFAULT_FIXPIPE_TRAIT>(l0cTensor, gmTensor);

    EXPECT_FALSE(callFlag);
    mockcpp::GlobalMockObject::reset(); 
}


TEST_F(TEST_FIX_PIPE, TestFixPipe_NoQuant_NZ2ND_float_NoCoord) {
        static __gm__ float dstFloat[16 * 16 * sizeof(float)] = {0};
        static __cc__ float srcFloat[16 * 16 * sizeof(float)] = {0};
        auto dstAddr = reinterpret_cast<__gm__ float*>(dstFloat);
        auto gmIterator = MakeGMmemPtr(dstAddr); 
        auto gmMatrixLayout = MakeLayoutForND<float>(16, 16); 
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        auto srcAddr = reinterpret_cast<__cc__ float*>(srcFloat);  
        auto l0cIterator = MakeL0CmemPtr(srcAddr); 
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 



        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ float*, __cc__ float*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, reinterpret_cast<__gm__ float*>(dstFloat)); 
                EXPECT_EQ(src, reinterpret_cast<__gm__ float*>(srcFloat)); 
            } 
        ))); 

        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor);

        mockcpp::GlobalMockObject::reset(); 
}

TEST_F(TEST_FIX_PIPE, TestFixPipe_NoQuant_NZ2ND_float_Coord) {
        static __gm__ float dstFloat[16 * 16 * sizeof(float)] = {0};
        static __cc__ float srcFloat[16 * 16 * sizeof(float)] = {0};
        // __gm__ float dst[16 * 16 * sizeof(float)] = {0};
        auto dstAddr = reinterpret_cast<__gm__ float*>(dstFloat);
        auto gmIterator = MakeGMmemPtr(dstAddr); 
        auto gmMatrixLayout = MakeLayoutForND<float>(16, 16); 
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        // __cc__ float src[16 * 16 * sizeof(float)] = {0};
        auto srcAddr = reinterpret_cast<__cc__ float*>(srcFloat);  
        auto l0cIterator = MakeL0CmemPtr(srcAddr); 
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ float*, __cc__ float*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, reinterpret_cast<__gm__ float*>(dstFloat) + 17); 
                EXPECT_EQ(src, reinterpret_cast<__gm__ float*>(srcFloat)); 
            } 
        ))); 
        auto coordZero = MakeCoord(AscendC::Std::Int<1>{}, AscendC::Std::Int<1>{});
        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor, coordZero);

        mockcpp::GlobalMockObject::reset(); 
}


TEST_F(TEST_FIX_PIPE, TestFixPipe_NoQuant_NZ2ND_int32_t_NoCoord) {
        static __gm__ int32_t dstInt32_t[16 * 16 * sizeof(int32_t)] = {0};
        static __cc__ int32_t srcInt32_t[16 * 16 * sizeof(int32_t)] = {0};
        auto dstAddr = reinterpret_cast<__gm__ int32_t*>(dstInt32_t);
        auto gmIterator = MakeGMmemPtr(dstAddr); 
        auto gmMatrixLayout = MakeLayoutForND<int32_t>(16, 16); 
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        auto srcAddr = reinterpret_cast<__cc__ int32_t*>(srcInt32_t);  
        auto l0cIterator = MakeL0CmemPtr(srcAddr); 
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 



        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ int32_t*, __cc__ int32_t*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, reinterpret_cast<__gm__ int32_t*>(dstInt32_t)); 
                EXPECT_EQ(src, reinterpret_cast<__gm__ int32_t*>(srcInt32_t)); 
            } 
        ))); 

        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor);

        mockcpp::GlobalMockObject::reset(); 
}

TEST_F(TEST_FIX_PIPE, TestFixPipe_NoQuant_NZ2ND_int32_t_Coord) {
        static __gm__ int32_t dstInt32_t[16 * 16 * sizeof(int32_t)] = {0};
        static __cc__ int32_t srcInt32_t[16 * 16 * sizeof(int32_t)] = {0};
        // __gm__ int32_t dst[16 * 16 * sizeof(int32_t)] = {0};
        auto dstAddr = reinterpret_cast<__gm__ int32_t*>(dstInt32_t);
        auto gmIterator = MakeGMmemPtr(dstAddr); 
        auto gmMatrixLayout = MakeLayoutForND<int32_t>(16, 16); 
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        // __cc__ int32_t src[16 * 16 * sizeof(int32_t)] = {0};
        auto srcAddr = reinterpret_cast<__cc__ int32_t*>(srcInt32_t);  
        auto l0cIterator = MakeL0CmemPtr(srcAddr); 
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ int32_t*, __cc__ int32_t*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, reinterpret_cast<__gm__ int32_t*>(dstInt32_t) + 17); 
                EXPECT_EQ(src, reinterpret_cast<__gm__ int32_t*>(srcInt32_t)); 
            } 
        ))); 
        auto coordZero = MakeCoord(AscendC::Std::Int<1>{}, AscendC::Std::Int<1>{});
        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor, coordZero);

        mockcpp::GlobalMockObject::reset(); 
}


// ****************TODO******************
TEST_F(TEST_FIX_PIPE, TestFixPipe_NoQuant_NZ2NZ_float_NoCoord) {
        static __gm__ float dstFloat[16 * 16 * sizeof(float)] = {0};
        static __cc__ float srcFloat[16 * 16 * sizeof(float)] = {0};
        auto dstAddr = reinterpret_cast<__gm__ float*>(dstFloat);
        auto gmIterator = MakeGMmemPtr(dstAddr); 
        auto gmMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        auto srcAddr = reinterpret_cast<__cc__ float*>(srcFloat);  
        auto l0cIterator = MakeL0CmemPtr(srcAddr); 
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 



        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ float*, __cc__ float*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, reinterpret_cast<__gm__ float*>(dstFloat)); 
                EXPECT_EQ(src, reinterpret_cast<__gm__ float*>(srcFloat)); 
            } 
        ))); 

        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor);

        mockcpp::GlobalMockObject::reset(); 
}

TEST_F(TEST_FIX_PIPE, TestFixPipe_NoQuant_NZ2NZ_float_Coord) {
        static __gm__ float dstFloat[16 * 16 * sizeof(float)] = {0};
        static __cc__ float srcFloat[16 * 16 * sizeof(float)] = {0};
        // __gm__ float dst[16 * 16 * sizeof(float)] = {0};
        auto dstAddr = reinterpret_cast<__gm__ float*>(dstFloat);
        auto gmIterator = MakeGMmemPtr(dstAddr); 
        auto gmMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        // __cc__ float src[16 * 16 * sizeof(float)] = {0};
        auto srcAddr = reinterpret_cast<__cc__ float*>(srcFloat);  
        auto l0cIterator = MakeL0CmemPtr(srcAddr); 
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ float*, __cc__ float*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ float* dst, __cc__ float* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, reinterpret_cast<__gm__ float*>(dstFloat)); 
                EXPECT_EQ(src, reinterpret_cast<__gm__ float*>(srcFloat)); 
            } 
        ))); 
        auto coordZero = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor, coordZero);

        mockcpp::GlobalMockObject::reset(); 
}


TEST_F(TEST_FIX_PIPE, TestFixPipe_NoQuant_NZ2NZ_int32_t_NoCoord) {
        static __gm__ int32_t dstInt32_t[16 * 16 * sizeof(int32_t)] = {0};
        static __cc__ int32_t srcInt32_t[16 * 16 * sizeof(int32_t)] = {0};
        auto dstAddr = reinterpret_cast<__gm__ int32_t*>(dstInt32_t);
        auto gmIterator = MakeGMmemPtr(dstAddr); 
        auto gmMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        auto srcAddr = reinterpret_cast<__cc__ int32_t*>(srcInt32_t);  
        auto l0cIterator = MakeL0CmemPtr(srcAddr); 
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 



        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ int32_t*, __cc__ int32_t*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, reinterpret_cast<__gm__ int32_t*>(dstInt32_t)); 
                EXPECT_EQ(src, reinterpret_cast<__gm__ int32_t*>(srcInt32_t)); 
            } 
        ))); 

        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor);

        mockcpp::GlobalMockObject::reset(); 
}

TEST_F(TEST_FIX_PIPE, TestFixPipe_NoQuant_NZ2NZ_int32_t_Coord) {
        static __gm__ int32_t dstInt32_t[16 * 16 * sizeof(int32_t)] = {0};
        static __cc__ int32_t srcInt32_t[16 * 16 * sizeof(int32_t)] = {0};
        // __gm__ int32_t dst[16 * 16 * sizeof(int32_t)] = {0};
        auto dstAddr = reinterpret_cast<__gm__ int32_t*>(dstInt32_t);
        auto gmIterator = MakeGMmemPtr(dstAddr); 
        auto gmMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(16, 16);
        auto gmTensor = MakeTensor(gmIterator, gmMatrixLayout);

        // __cc__ int32_t src[16 * 16 * sizeof(int32_t)] = {0};
        auto srcAddr = reinterpret_cast<__cc__ int32_t*>(srcInt32_t);  
        auto l0cIterator = MakeL0CmemPtr(srcAddr); 
        auto l0cMatrixLayout = MakeL0CLayout(16, 16); 
        auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); 

        MOCKER_CPP(copy_matrix_cc_to_gm, void(__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN)) 
        .times(1) 
        .will(invoke(static_cast<void(*)(__gm__ int32_t*, __cc__ int32_t*, uint8_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, QuantMode_t, uint8_t, bool, bool)>( 
            [](__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, uint16_t NSize, uint16_t MSize, uint32_t dstStride_dst_D, uint16_t srcStride, uint8_t UnitFlagMode, QuantMode_t QuantPRE, uint8_t ReLUPRE, bool channelSplit, bool NZ2ND_EN) { 
                EXPECT_EQ(dst, reinterpret_cast<__gm__ int32_t*>(dstInt32_t)); 
                EXPECT_EQ(src, reinterpret_cast<__gm__ int32_t*>(srcInt32_t)); 
            } 
        ))); 
        auto coordZero = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{});
        Fixpipe<DEFAULT_FIXPIPE_TRAIT>(gmTensor, l0cTensor, coordZero);

        mockcpp::GlobalMockObject::reset(); 
}

