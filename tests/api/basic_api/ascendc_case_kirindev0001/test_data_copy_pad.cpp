/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <gtest/gtest.h>
#include "kernel_tensor.h"
#include "kernel_tpipe.h"
#include "kernel_operator_data_copy_intf.h"

using namespace std;
using namespace AscendC;

struct TestDataCopyPadGm2L1Params {
    uint32_t dataSize;   // element count
    uint32_t typeSize;   // sizeof(T)
    void (*cal_func)(uint8_t*, int32_t);   // (srcGm, dataSize)
};

struct TestDataCopyPadL12GmParams {
    uint32_t dataSize;   // element count
    uint32_t typeSize;   // sizeof(T)
    void (*cal_func)(uint8_t*, int32_t);   // (dstGm, dataSize)
};


class TestDataCopyPadGm2L1Suite : public testing::Test,
                                  public testing::WithParamInterface<TestDataCopyPadGm2L1Params> {};
class TestDataCopyPadL12GmSuite : public testing::Test,
                                  public testing::WithParamInterface<TestDataCopyPadL12GmParams> {};

template <typename T>
void main_data_copy_pad_gm2l1_kernel(__gm__ uint8_t* __restrict__ srcGm, int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);

    TBuf<TPosition::A1> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(T));
    LocalTensor<T> l1Local = tbuf.Get<T>();

    // 单行连续: blockCount == 1, blockLen == dataSize * sizeof(T)。
    DataCopyExtParams copyParams{1, static_cast<uint32_t>(dataSize * sizeof(T)), 0, 0, 0};
    DataCopyPadExtParams<T> padParams{false, 0, 0, 0};

    DataCopyPad<T, PaddingMode::Normal>(l1Local, srcGlobal, copyParams, padParams);

    PipeBarrier<PIPE_ALL>();
}

template <typename T>
void main_data_copy_pad_l12gm_kernel(__gm__ uint8_t* __restrict__ dstGm, int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> dstGlobal;
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::A1> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(T));
    LocalTensor<T> l1Local = tbuf.Get<T>();

    // 单行连续: blockCount == 1, blockLen == dataSize * sizeof(T)。
    DataCopyExtParams copyParams{1, static_cast<uint32_t>(dataSize * sizeof(T)), 0, 0, 0};

    DataCopyPad<T, PaddingMode::Normal>(dstGlobal, l1Local, copyParams);

    PipeBarrier<PIPE_ALL>();
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_PAD_GM_L1, TestDataCopyPadGm2L1Suite,
    ::testing::Values(
        TestDataCopyPadGm2L1Params{64, 2, main_data_copy_pad_gm2l1_kernel<half>},
        TestDataCopyPadGm2L1Params{64, 2, main_data_copy_pad_gm2l1_kernel<int16_t>},
        TestDataCopyPadGm2L1Params{64, 2, main_data_copy_pad_gm2l1_kernel<uint16_t>},
        TestDataCopyPadGm2L1Params{64, 1, main_data_copy_pad_gm2l1_kernel<int8_t>}));

TEST_P(TestDataCopyPadGm2L1Suite, DataCopyPadGm2L1Cases)
{
    auto param = GetParam();
    const size_t byteSize = static_cast<size_t>(param.dataSize) * param.typeSize;

    uint8_t srcGm[byteSize];
    for (size_t i = 0; i < byteSize; ++i) {
        srcGm[i] = static_cast<uint8_t>(i & 0xFF);
    }

    param.cal_func(srcGm, param.dataSize);

    for (size_t i = 0; i < byteSize; ++i) {
        EXPECT_EQ(srcGm[i], static_cast<uint8_t>(i & 0xFF))
            << "srcGm canary violated at offset " << i
            << " (GM->L1 leg wrote the read-only source buffer)";
    }
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_PAD_L1_GM, TestDataCopyPadL12GmSuite,
    ::testing::Values(
        TestDataCopyPadL12GmParams{64, 2, main_data_copy_pad_l12gm_kernel<half>},
        TestDataCopyPadL12GmParams{64, 2, main_data_copy_pad_l12gm_kernel<int16_t>},
        TestDataCopyPadL12GmParams{64, 2, main_data_copy_pad_l12gm_kernel<uint16_t>},
        TestDataCopyPadL12GmParams{64, 1, main_data_copy_pad_l12gm_kernel<int8_t>}));

TEST_P(TestDataCopyPadL12GmSuite, DataCopyPadL12GmCases)
{
    auto param = GetParam();
    const size_t byteSize = static_cast<size_t>(param.dataSize) * param.typeSize;

    uint8_t gm[2 * byteSize];
    for (size_t i = 0; i < byteSize; ++i) {
        gm[i] = 0xCA;
        gm[byteSize + i] = static_cast<uint8_t>(i & 0xFF);
    }

    param.cal_func(gm, param.dataSize);

    // 检查窗恰等于拷贝跨度, dst 须读回全 0x00。
    for (size_t i = 0; i < byteSize; ++i) {
        EXPECT_EQ(gm[i], 0x00)
            << "zeroing contract violated at offset " << i << " (dataSize=" << param.dataSize
            << ", typeSize=" << param.typeSize << ")";
    }

    // 修复5161 loop2Size=blockLen 字节当元素问题: 修复清零实跑 blockLen*sizeof(T) 字节
    // (2 字节类型为 2 倍真跨度) 抹掉 dstGm 后邻。canary 与 dst 同块紧邻, 回归复现即在此变红。
    for (size_t i = 0; i < byteSize; ++i) {
        EXPECT_EQ(gm[byteSize + i], static_cast<uint8_t>(i & 0xFF))
            << "dstGm canary violated at canary offset " << i
            << " (dst write overflow wiped the adjacent canary)";
    }
}
