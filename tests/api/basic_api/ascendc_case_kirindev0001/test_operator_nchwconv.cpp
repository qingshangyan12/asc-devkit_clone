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
#define private public
#define protect public
#include "kernel_tensor.h"
#include "kernel_struct_transpose.h"
#if ASCENDC_CPU_DEBUG
namespace AscendC {
template <typename T>
__aicore__ inline void TransDataTo5HDVldVaRegImpl(
    __ubuf__ uint64_t* dst, __ubuf__ uint64_t* src, const TransDataTo5HDParams& transDataTo5HDParams) = delete;
}
#endif
#include "kernel_operator_vec_transpose_intf.h"

using namespace std;
using namespace AscendC;

template <typename T>
__global__ __aicore__ void MainNchwconvVaReg(
    __gm__ uint16_t* __restrict__ dstGm, __gm__ uint16_t* __restrict__ srcGm, int32_t dataSize,
    const TransDataTo5HDParams& nchwconvParams, const int32_t width)
{
    TPipe tpipe;
    GlobalTensor<T> input_global;
    GlobalTensor<T> output_global;
    input_global.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    output_global.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::VECCALC> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(T));
    LocalTensor<T> input_local = tbuf.Get<T>();

    TBuf<TPosition::VECCALC> tbuf1;
    tpipe.InitBuffer(tbuf1, dataSize * sizeof(T));
    LocalTensor<T> output_local = tbuf1.Get<T>();

    DataCopy(input_local, input_global, dataSize);

    auto eventIdMTE2ToS = GetTPipePtr()->FetchEventID(HardEvent::MTE2_S);
    SetFlag<HardEvent::MTE2_S>(eventIdMTE2ToS);
    WaitFlag<HardEvent::MTE2_S>(eventIdMTE2ToS);

    uint64_t vaRegArray1[NCHW_CONV_ADDR_LIST_SIZE] = {0};
    uint64_t vaRegArray3[NCHW_CONV_ADDR_LIST_SIZE] = {0};
    for (int32_t i = 0; i < NCHW_CONV_ADDR_LIST_SIZE; i++) {
        vaRegArray1[i] = (uint64_t)output_local[width * i].GetPhyAddr();
        vaRegArray3[i] = (uint64_t)input_local[width * i].GetPhyAddr();
    }
    TransDataTo5HD<T>(vaRegArray1, vaRegArray3, nchwconvParams);
    if constexpr (sizeof(T) == 1) {
        uint64_t vaRegArray5[NCHW_CONV_ADDR_LIST_SIZE] = {0};
        uint64_t vaRegArray7[NCHW_CONV_ADDR_LIST_SIZE] = {0};
        for (int32_t i = 0; i < NCHW_CONV_ADDR_LIST_SIZE; i++) {
            vaRegArray5[i] = (uint64_t)output_local[(i + NCHW_CONV_ADDR_LIST_SIZE) * width].GetPhyAddr();
            vaRegArray7[i] = (uint64_t)input_local[(i + NCHW_CONV_ADDR_LIST_SIZE) * width].GetPhyAddr();
        }

        TransDataTo5HDParams b8Params = nchwconvParams;
        b8Params.dstHighHalf = true;
        b8Params.srcHighHalf = false;
        TransDataTo5HD<T>(vaRegArray1, vaRegArray7, b8Params);

        b8Params.dstHighHalf = false;
        b8Params.srcHighHalf = true;
        TransDataTo5HD<T>(vaRegArray5, vaRegArray3, b8Params);

        b8Params.dstHighHalf = true;
        b8Params.srcHighHalf = true;
        TransDataTo5HD<T>(vaRegArray5, vaRegArray7, b8Params);
    }

    auto eventIdVToMTE3 = GetTPipePtr()->FetchEventID(HardEvent::V_MTE3);
    SetFlag<HardEvent::V_MTE3>(eventIdVToMTE3);
    WaitFlag<HardEvent::V_MTE3>(eventIdVToMTE3);

    DataCopy(output_global, output_local, dataSize * 2);

    pipe_barrier(PIPE_ALL);
}

struct NchwconvVaRegTestParams {
    int32_t dataSize;
    int32_t typeBytes;
    const uint8_t repTimes;
    const uint16_t dstRep;
    const uint16_t srcRep;
    void (*cal_func)(uint16_t*, uint16_t*, int32_t, const TransDataTo5HDParams&, const int32_t);
    const int32_t width;
};

namespace {
class NchwconvVaRegTestsuite : public testing::Test, public testing::WithParamInterface<NchwconvVaRegTestParams> {
protected:
    void SetUp() { SetGCoreType(2); }
    void TearDown() { SetGCoreType(0); }
};

INSTANTIATE_TEST_CASE_P(
    TEST_OPEARATION_NCHWCONV_VAREG, NchwconvVaRegTestsuite,
    ::testing::Values(
        NchwconvVaRegTestParams{256, 2, 1, 16, 16, MainNchwconvVaReg<half>, 16},
        NchwconvVaRegTestParams{512, 2, 2, 16, 16, MainNchwconvVaReg<uint16_t>, 16},
        NchwconvVaRegTestParams{512, 2, 2, 1, 1, MainNchwconvVaReg<int16_t>, 32},
        NchwconvVaRegTestParams{1024, 1, 1, 16, 16, MainNchwconvVaReg<int8_t>, 32},
        NchwconvVaRegTestParams{1024, 1, 2, 16, 16, MainNchwconvVaReg<uint8_t>, 32}));

TEST_P(NchwconvVaRegTestsuite, NchwconvVaRegTestCase)
{
    auto param = GetParam();
    TransDataTo5HDParams nchwconvParams;
    nchwconvParams.dstHighHalf = false;
    nchwconvParams.srcHighHalf = false;
    nchwconvParams.repeatTimes = param.repTimes;
    nchwconvParams.dstRepStride = param.dstRep;
    nchwconvParams.srcRepStride = param.srcRep;

    uint16_t dstGm[param.dataSize * param.typeBytes] = {0};
    uint16_t srcGm[param.dataSize * param.typeBytes] = {0};

    param.cal_func(dstGm, srcGm, param.dataSize, nchwconvParams, param.width);

    for (int i = 0; i < param.dataSize; i++) {
        EXPECT_EQ(dstGm[i], 0x00);
    }
}
} // namespace
