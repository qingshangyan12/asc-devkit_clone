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
#include "kernel_operator.h"
// Ndtri lives under experimental/ and is therefore NOT pulled in by kernel_api.h. Include it
// directly; tests/api/adv_api/CMakeLists.txt adds ${ASCENDC_DIR}/experimental/include to the
// header search path. On promotion into the main tree this include can be dropped in favour of
// registering "math/ndtri.h" in include/adv_api/kernel_api.h.
#include "adv_api/math/ndtri.h"

using namespace std;
using namespace AscendC;

enum TestMode {
    LEVEL0_COUNT_MODE,   // Ndtri(dst, src, calCount)
    FIXED_BLOCK_MODE,    // Ndtri(dst, src)                 -> whole tensor
    EXTRA_BUFFER_MODE,   // Ndtri(dst, src, sharedTmpBuffer)
    EXTRA_BUFFER_COUNT_MODE, // Ndtri(dst, src, sharedTmpBuffer, calCount)
};

class TEST_NDTRI : public testing::Test {
protected:
    void SetUp() { AscendC::SetGCoreType(2); }
    void TearDown() { AscendC::SetGCoreType(0); }
};

template <typename T>
void MainVecNdtriDemo(
    __gm__ uint8_t* __restrict__ dstGm, __gm__ uint8_t* __restrict__ srcGm, uint32_t dataSize, TestMode testMode)
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, dataSize * sizeof(T));
    LocalTensor<T> inputLocal = tbuf1.Get<T>();

    TBuf<TPosition::VECCALC> tbuf2;
    tpipe.InitBuffer(tbuf2, dataSize * sizeof(T));
    LocalTensor<T> outputLocal = tbuf2.Get<T>();

    DataCopy(inputLocal, inputGlobal, dataSize);

    SetFlag<HardEvent::MTE2_V>(EVENT_ID0);
    WaitFlag<HardEvent::MTE2_V>(EVENT_ID0);

    if (testMode == LEVEL0_COUNT_MODE) {
        Ndtri<T>(outputLocal, inputLocal, dataSize);
    } else if (testMode == FIXED_BLOCK_MODE) {
        Ndtri<T, false>(outputLocal, inputLocal);
    } else {
        // Ndtri needs no temporary space on 3510 (GetNdtriMaxMinTmpSize returns 0/0); the buffer is
        // accepted for interface parity only. Pass a small one to exercise both overloads.
        TBuf<TPosition::VECCALC> tbuf3;
        tpipe.InitBuffer(tbuf3, dataSize * sizeof(T));
        LocalTensor<uint8_t> tmpLocal = tbuf3.Get<uint8_t>();
        if (testMode == EXTRA_BUFFER_MODE) {
            Ndtri<T, false>(outputLocal, inputLocal, tmpLocal);
        } else {
            Ndtri<T, false>(outputLocal, inputLocal, tmpLocal, dataSize);
        }
    }

    SetFlag<HardEvent::V_MTE3>(EVENT_ID0);
    WaitFlag<HardEvent::V_MTE3>(EVENT_ID0);

    DataCopy(outputGlobal, outputLocal, dataSize);

    PipeBarrier<PIPE_ALL>();
}

// Kernel-side UT: drives every overload through the CPU-emulated core to cover the API surface and
// the parameter checks. Numerical accuracy is covered by the functional test in
// experimental/tests/functional/ndtri (compared against scipy.special.ndtri on device/simulator),
// because this harness has no golden.
#define VEC_NDTRI_LEVEL2_TESTCASE(DATA_TYPE, testMode)                      \
    TEST_F(TEST_NDTRI, Ndtri##DATA_TYPE##testMode##Case)                    \
    {                                                                       \
        uint32_t dataSize = 512;                                            \
        uint8_t inputGm[dataSize * sizeof(DATA_TYPE)] = {0};                \
        uint8_t outputGm[dataSize * sizeof(DATA_TYPE)] = {0};               \
                                                                            \
        MainVecNdtriDemo<DATA_TYPE>(outputGm, inputGm, dataSize, testMode); \
                                                                            \
        for (uint32_t i = 0; i < dataSize; i++) {                           \
            EXPECT_EQ(outputGm[i], 0x00);                                   \
        }                                                                   \
    }

// float only — the task and the PyTorch prims.ndtri reference are float32.
VEC_NDTRI_LEVEL2_TESTCASE(float, LEVEL0_COUNT_MODE);
VEC_NDTRI_LEVEL2_TESTCASE(float, FIXED_BLOCK_MODE);
VEC_NDTRI_LEVEL2_TESTCASE(float, EXTRA_BUFFER_MODE);
VEC_NDTRI_LEVEL2_TESTCASE(float, EXTRA_BUFFER_COUNT_MODE);

// Unaligned / boundary element counts: exercises Reg::UpdateMask's ragged final repeat
// (calCount % 64 != 0) and the single-element case.
TEST_F(TEST_NDTRI, NdtriUnalignedCountCase)
{
    for (uint32_t dataSize : {1u, 32u, 63u, 65u, 1023u}) {
        uint8_t inputGm[1023 * sizeof(float)] = {0};
        uint8_t outputGm[1023 * sizeof(float)] = {0};
        MainVecNdtriDemo<float>(outputGm, inputGm, dataSize, LEVEL0_COUNT_MODE);
    }
}
