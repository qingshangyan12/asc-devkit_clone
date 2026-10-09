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
#include <cstdint>
#include "kernel_tensor.h"
#include "kernel_tpipe.h"
// impl 链引用的 bisheng::cce 编译器内建, 声明保证编译通过。后续修复可删除。
namespace bisheng {
namespace cce {
void enable_s4_expand_s8_datacopy();
void enable_s3_expand_s8_datacopy();
}
}
#include "kernel_operator_mm_intf.h"

using namespace AscendC;

struct TestLoadDataGm2B2Params {
    uint32_t dataSize;
    uint32_t typeSize;
    LoadData2DParamsV2 params;
    void (*calFunc)(__gm__ uint8_t*, int32_t, LoadData2DParamsV2&);
};

struct TestLoadDataGm2C1Params {
    uint32_t dataSize;
    uint32_t typeSize;
    LoadData2DParamsV2 params;
    void (*calFunc)(__gm__ uint8_t*, int32_t, LoadData2DParamsV2&);
};

struct TestLoadDataL1ToB2Params {
    uint32_t dataSize;
    LoadData2DParamsV2 params; // {mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride, ifTranspose, sid}
    void (*calFunc)(int32_t, LoadData2DParamsV2&);
};

class TestLoadDataGm2B2Suite : public testing::Test, public testing::WithParamInterface<TestLoadDataGm2B2Params> {};
class TestLoadDataGm2C1Suite : public testing::Test, public testing::WithParamInterface<TestLoadDataGm2C1Params> {};
class TestLoadDataL1ToB2Suite : public testing::Test, public testing::WithParamInterface<TestLoadDataL1ToB2Params> {};

// GM->L0B (load_gm_to_cb_2dv2): kStep != 1 时先触发 set_mte2_src_para 再搬运
template <typename T>
void main_load_data_gm2b2_kernel(__gm__ uint8_t* srcGm, int32_t dataSize, LoadData2DParamsV2& params)
{
    TPipe tpipe;
    TBuf<TPosition::B2> dstBuf;
    tpipe.InitBuffer(dstBuf, 512 * sizeof(T));
    LocalTensor<T> dst = dstBuf.Get<T>();

    GlobalTensor<T> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);

    LoadData(dst, srcGlobal, params);
    PipeBarrier<PIPE_ALL>();
}

// GM->L1 (load_gm_to_cbuf_2dv2)
template <typename T>
void main_load_data_gm2c1_kernel(__gm__ uint8_t* srcGm, int32_t dataSize, LoadData2DParamsV2& params)
{
    TPipe tpipe;
    TBuf<TPosition::C1> dstBuf;
    tpipe.InitBuffer(dstBuf, 512 * sizeof(T));
    LocalTensor<T> dst = dstBuf.Get<T>();

    GlobalTensor<T> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);

    LoadData(dst, srcGlobal, params);
    PipeBarrier<PIPE_ALL>();
}

// L1->L0B (load_cbuf_to_cb): kStep 直入 intrinsic 无分支, !=1 用例同臂无判别力不设面; ifTranspose 选转置臂。
// L1 源窗口无宿主可观测伙伴, 递增填充与只读 canary 均在核内。
template <typename T>
void main_load_data_l1tob2_kernel(int32_t dataSize, LoadData2DParamsV2& params)
{
    TPipe tpipe;

    const size_t winBytes = static_cast<size_t>(dataSize) * sizeof(T);
    TBuf<TPosition::C1> srcBuf;
    tpipe.InitBuffer(srcBuf, winBytes); // 源暂存递增值整驻 L1
    LocalTensor<T> src = srcBuf.Get<T>();

    uint8_t* srcP = reinterpret_cast<uint8_t*>(src.GetPhyAddr());
    for (size_t i = 0; i < winBytes; ++i) { // 递增值: 只读源 canary
        srcP[i] = static_cast<uint8_t>(i & 0xFF);
    }

    TBuf<TPosition::B2> dstBuf;
    tpipe.InitBuffer(dstBuf, 512 * sizeof(T));
    LocalTensor<T> dst = dstBuf.Get<T>();

    LoadData(dst, src, params);
    PipeBarrier<PIPE_ALL>();

    for (size_t i = 0; i < winBytes; ++i) {
        EXPECT_EQ(srcP[i], static_cast<uint8_t>(i & 0xFF))
            << "l1tob2 srcL1: source window modified at offset " << i << " -- rogue write";
    }
}

INSTANTIATE_TEST_CASE_P(
    TEST_LOAD_DATA_GM2B2, TestLoadDataGm2B2Suite,
    ::testing::Values(
        TestLoadDataGm2B2Params{1024, 1, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, false, 0},
                                main_load_data_gm2b2_kernel<int8_t>},
        TestLoadDataGm2B2Params{1024, 1, LoadData2DParamsV2{0, 0, 1, 2, 64, 0, false, 0},
                                main_load_data_gm2b2_kernel<int8_t>},
        TestLoadDataGm2B2Params{512, 2, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, false, 0},
                                main_load_data_gm2b2_kernel<half>},
        TestLoadDataGm2B2Params{512, 2, LoadData2DParamsV2{0, 0, 1, 2, 64, 0, false, 0},
                                main_load_data_gm2b2_kernel<half>},
        TestLoadDataGm2B2Params{256, 4, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, false, 0},
                                main_load_data_gm2b2_kernel<float>},
        TestLoadDataGm2B2Params{256, 4, LoadData2DParamsV2{0, 0, 1, 2, 64, 0, false, 0},
                                main_load_data_gm2b2_kernel<float>}));

TEST_P(TestLoadDataGm2B2Suite, LoadDataGm2B2Cases)
{
    auto param = GetParam();
    const size_t byteSize = static_cast<size_t>(param.dataSize) * param.typeSize;

    uint8_t srcGm[byteSize];
    for (size_t i = 0; i < byteSize; ++i) {
        srcGm[i] = static_cast<uint8_t>(i & 0xFF);
    }

    param.calFunc(srcGm, param.dataSize, param.params);

    for (size_t i = 0; i < byteSize; ++i) {
        EXPECT_EQ(srcGm[i], static_cast<uint8_t>(i & 0xFF))
            << "gm2b2 srcGm: source buffer modified at offset " << i << " -- rogue write";
    }
}

INSTANTIATE_TEST_CASE_P(
    TEST_LOAD_DATA_GM2C1, TestLoadDataGm2C1Suite,
    ::testing::Values(
        TestLoadDataGm2C1Params{1024, 1, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, false, 0},
                                main_load_data_gm2c1_kernel<int8_t>},
        TestLoadDataGm2C1Params{1024, 1, LoadData2DParamsV2{0, 0, 1, 2, 64, 0, false, 0},
                                main_load_data_gm2c1_kernel<int8_t>},
        TestLoadDataGm2C1Params{512, 2, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, false, 0},
                                main_load_data_gm2c1_kernel<half>},
        TestLoadDataGm2C1Params{512, 2, LoadData2DParamsV2{0, 0, 1, 2, 64, 0, false, 0},
                                main_load_data_gm2c1_kernel<half>},
        TestLoadDataGm2C1Params{256, 4, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, false, 0},
                                main_load_data_gm2c1_kernel<float>},
        TestLoadDataGm2C1Params{256, 4, LoadData2DParamsV2{0, 0, 1, 2, 64, 0, false, 0},
                                main_load_data_gm2c1_kernel<float>}));

TEST_P(TestLoadDataGm2C1Suite, LoadDataGm2C1Cases)
{
    auto param = GetParam();
    const size_t byteSize = static_cast<size_t>(param.dataSize) * param.typeSize;

    uint8_t srcGm[byteSize];
    for (size_t i = 0; i < byteSize; ++i) {
        srcGm[i] = static_cast<uint8_t>(i & 0xFF);
    }

    param.calFunc(srcGm, param.dataSize, param.params);

    for (size_t i = 0; i < byteSize; ++i) {
        EXPECT_EQ(srcGm[i], static_cast<uint8_t>(i & 0xFF))
            << "gm2c1 srcGm: source buffer modified at offset " << i << " -- rogue write";
    }
}

INSTANTIATE_TEST_CASE_P(
    TEST_LOAD_DATA_L1TOB2, TestLoadDataL1ToB2Suite,
    ::testing::Values(
        TestLoadDataL1ToB2Params{1024, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, false, 0},
                                 main_load_data_l1tob2_kernel<int8_t>},
        TestLoadDataL1ToB2Params{1024, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, true, 0},
                                 main_load_data_l1tob2_kernel<int8_t>},
        TestLoadDataL1ToB2Params{1024, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, false, 0},
                                 main_load_data_l1tob2_kernel<uint8_t>},
        TestLoadDataL1ToB2Params{1024, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, true, 0},
                                 main_load_data_l1tob2_kernel<uint8_t>},
        TestLoadDataL1ToB2Params{512, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, false, 0},
                                 main_load_data_l1tob2_kernel<half>},
        TestLoadDataL1ToB2Params{512, LoadData2DParamsV2{0, 0, 1, 1, 0, 0, true, 0},
                                 main_load_data_l1tob2_kernel<half>}));

TEST_P(TestLoadDataL1ToB2Suite, LoadDataL1ToB2Cases)
{
    auto param = GetParam();
    param.calFunc(param.dataSize, param.params);
}
