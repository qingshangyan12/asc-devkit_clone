/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <vector>
#include <gtest/gtest.h>
#include "mockcpp/mockcpp.hpp"
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"

using namespace AscendC::Te;
using namespace AscendC::Std;

// Struct to hold captured call information
struct LoadCbufToCbCapture {
    void* dst = nullptr;
    void* src = nullptr;
    uint16_t mStartPosition = 0;
    uint16_t kStartPosition = 0;
    uint8_t mStep = 0;
    uint8_t kStep = 0;
    int16_t srcStride = 0;
    uint16_t dstStride = 0;
    bool transpose = false;
};

// Global capture object
std::vector<LoadCbufToCbCapture> gL12L0BCapture;

// Reset capture data
void ResetCapture()
{
    gL12L0BCapture.clear();
}

#define CAPTURE_LOAD_L1_TO_L0B(type)                                                                                   \
    void CaptureLoadCbufToCbCall_##type(__cb__ type* dst, __cbuf__ type* src, uint16_t mStartPos, uint16_t kStartPos,  \
                                        uint8_t mStepVal, uint8_t kStepVal, int16_t srcStrideVal,                      \
                                        uint16_t dstStrideVal, bool transposeVal)                                      \
    {                                                                                                                  \
        LoadCbufToCbCapture capture;                                                                                   \
        capture.dst = reinterpret_cast<void*>(dst);                                                                    \
        capture.src = reinterpret_cast<void*>(src);                                                                    \
        capture.mStartPosition = mStartPos;                                                                            \
        capture.kStartPosition = kStartPos;                                                                            \
        capture.mStep = mStepVal;                                                                                      \
        capture.kStep = kStepVal;                                                                                      \
        capture.srcStride = srcStrideVal;                                                                              \
        capture.dstStride = dstStrideVal;                                                                              \
        capture.transpose = transposeVal;                                                                              \
        gL12L0BCapture.push_back(capture);                                                                             \
    }

CAPTURE_LOAD_L1_TO_L0B(half);
CAPTURE_LOAD_L1_TO_L0B(float);

#define MOCKER_LOAD_CBUF_TO_CB(type)                                                                                   \
    MOCKER(load_cbuf_to_cb,                                                                                            \
           void (*)(__cb__ type*, __cbuf__ type*, uint16_t, uint16_t, uint8_t, uint8_t, int16_t, uint16_t, bool))      \
        .stubs()                                                                                                       \
        .will(invoke(CaptureLoadCbufToCbCall_##type))

class Tensor_Api_LoadData_L12L0B : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}

    virtual void SetUp()
    {
        ResetCapture();
        MOCKER_LOAD_CBUF_TO_CB(half);
        MOCKER_LOAD_CBUF_TO_CB(float);
    }

    virtual void TearDown()
    {
        GlobalMockObject::verify();
    }
};

struct LoadData2BTrait {
    using TraitType = LoadDataTrait;
    static constexpr const TraitType value{true};
};

// Test function that validates load_cbuf_to_cb parameters for L1->L0B copy
TEST_F(Tensor_Api_LoadData_L12L0B, CopyL12L0BOperation)
{
    constexpr int32_t n = 128;
    constexpr int32_t k = 128;
    constexpr uint32_t TILE_LENGTH = k * n;

    __cbuf__ half l1BBuf[TILE_LENGTH] = {0};
    __cb__ half l0BBuf[TILE_LENGTH] = {0};

    auto l1BLayout = MakeNZLayout<half>(k, n);
    auto l0BLayout = MakeZNLayout<half>(k, n);

    auto l1BTensor = MakeTensor(MakeL1memPtr(l1BBuf), l1BLayout);
    auto l0BTensor = MakeTensor(MakeL0BmemPtr(l0BBuf), l0BLayout);

    Copy(CopyAtom<CopyTraits<CopyL12L0, LoadData2BTrait>>{}, l0BTensor, l1BTensor);

    ASSERT_EQ(gL12L0BCapture.size(), 1);
    EXPECT_EQ(gL12L0BCapture[0].src, reinterpret_cast<void*>(l1BBuf));
    EXPECT_EQ(gL12L0BCapture[0].dst, reinterpret_cast<void*>(l0BBuf));
    EXPECT_EQ(gL12L0BCapture[0].mStartPosition, 0);
    EXPECT_EQ(gL12L0BCapture[0].kStartPosition, 0);
    EXPECT_EQ(gL12L0BCapture[0].mStep, 8);
    EXPECT_EQ(gL12L0BCapture[0].kStep, 8);
    EXPECT_EQ(gL12L0BCapture[0].srcStride, 8);
    EXPECT_EQ(gL12L0BCapture[0].dstStride, 8);
    EXPECT_EQ(gL12L0BCapture[0].transpose, true);
}

// Test with coordinate parameter
TEST_F(Tensor_Api_LoadData_L12L0B, CopyL12L0BOperationWithCoord)
{
    constexpr int32_t n = 128;
    constexpr int32_t k = 128;
    constexpr uint32_t TILE_LENGTH = k * n;

    __cbuf__ half l1BBuf[TILE_LENGTH] = {0};
    __cb__ half l0BBuf[TILE_LENGTH] = {0};

    auto l1BLayout = MakeNZLayout<half>(k, n);
    auto l0BLayout = MakeZNLayout<half>(k, n);

    auto l1BTensor = MakeTensor(MakeL1memPtr(l1BBuf), l1BLayout);
    auto l0BTensor = MakeTensor(MakeL0BmemPtr(l0BBuf), l0BLayout);

    auto coord = MakeCoord(Int<32>{}, Int<32>{});

    Copy(CopyAtom<CopyTraits<CopyL12L0, LoadData2BTrait>>{}, l0BTensor, l1BTensor, coord);

    ASSERT_EQ(gL12L0BCapture.size(), 1);
    EXPECT_EQ(gL12L0BCapture[0].src, reinterpret_cast<void*>(l1BBuf));
    EXPECT_EQ(gL12L0BCapture[0].dst, reinterpret_cast<void*>(l0BBuf));
    EXPECT_EQ(gL12L0BCapture[0].mStartPosition, 2);
    EXPECT_EQ(gL12L0BCapture[0].kStartPosition, 2);
    EXPECT_EQ(gL12L0BCapture[0].mStep, 8);
    EXPECT_EQ(gL12L0BCapture[0].kStep, 8);
    EXPECT_EQ(gL12L0BCapture[0].srcStride, 8);
    EXPECT_EQ(gL12L0BCapture[0].dstStride, 8);
    EXPECT_EQ(gL12L0BCapture[0].transpose, true);
}
