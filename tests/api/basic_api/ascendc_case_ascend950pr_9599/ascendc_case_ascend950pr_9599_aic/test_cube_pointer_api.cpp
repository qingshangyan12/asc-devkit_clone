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
#include "kernel_operator.h"

using namespace AscendC;

// ============================================================================
// GetUnderlyingPtr 单元测试
// ============================================================================

TEST(CubePointerApiTest, GetUnderlyingPtr_LocalTensor)
{
    TPipe pipe;
    LocalTensor<half> localBuf;
    TBuffAddr tbuf;
    tbuf.logicPos = static_cast<uint8_t>(TPosition::A1);
    localBuf.SetAddr(tbuf);
    localBuf.InitBuffer(nullptr, 1024);
    auto ptr = GetUnderlyingPtr(localBuf);
    EXPECT_NE(ptr, nullptr);
}

TEST(CubePointerApiTest, GetUnderlyingPtr_RawPointer)
{
    half arr[1024];
    auto ptr = GetUnderlyingPtr(arr);
    EXPECT_EQ(ptr, arr);
}

TEST(CubePointerApiTest, GetUnderlyingPtr_ConstRawPointer)
{
    const half arr[1024];
    auto ptr = GetUnderlyingPtr(arr);
    EXPECT_EQ(ptr, arr);
}

// ============================================================================
// LoadData 指针重载编译验证
// ============================================================================

TEST(CubePointerApiTest, LoadData_2D_PtrOverload_Compile)
{
    half l0Buf[1024];
    half l1Buf[1024];
    LoadData2DParams params = {0, 0, 0, 0, 0, 0, true, 0};
    LoadData(l0Buf, l1Buf, params);
}

TEST(CubePointerApiTest, LoadData_2DV2_PtrOverload_Compile)
{
    half l0Buf[1024];
    half l1Buf[1024];
    LoadData2DParamsV2 params = {0, 0, 0, 0, 0, 0, true, 0};
    LoadData(l0Buf, l1Buf, params);
}

TEST(CubePointerApiTest, LoadData_3DV2_PtrOverload_Compile)
{
    half l0Buf[1024];
    half l1Buf[1024];
    LoadData3DParamsV2<half> params;
    params.padList[0] = params.padList[1] = params.padList[2] = params.padList[3] = 0;
    params.l1H = 32; params.l1W = 32; params.channelSize = 32;
    params.kExtension = 32; params.mExtension = 32;
    params.kStartPt = 0; params.mStartPt = 0;
    params.strideW = 32; params.strideH = 1;
    params.filterW = 1; params.filterH = 1;
    params.dilationFilterW = 1; params.dilationFilterH = 1;
    params.enTranspose = false; params.enSmallK = false;
    params.padValue = 0;
    LoadData(l0Buf, l1Buf, params);
}

TEST(CubePointerApiTest, LoadData_3DV2Pro_PtrOverload_Compile)
{
    half l0Buf[1024];
    half l1Buf[1024];
    LoadData3DParamsV2Pro params;
    params.padList[0] = params.padList[1] = params.padList[2] = params.padList[3] = 0;
    params.l1H = 32; params.l1W = 32; params.channelSize = 32;
    params.kExtension = 32; params.mExtension = 32;
    params.kStartPt = 0; params.mStartPt = 0;
    params.strideW = 32; params.strideH = 1;
    params.filterW = 1; params.filterH = 1;
    params.dilationFilterW = 1; params.dilationFilterH = 1;
    params.enTranspose = false; params.enSmallK = false;
    params.padValue = 0;
    LoadData(l0Buf, l1Buf, params);
}

// ============================================================================
// Mmad 指针重载编译验证
// ============================================================================

TEST(CubePointerApiTest, Mmad_PtrOverload_Compile)
{
    float l0cBuf[1024];
    half l0aBuf[1024];
    half l0bBuf[1024];
    MmadParams mmadParams;
    mmadParams.m = 32; mmadParams.n = 32; mmadParams.k = 32;
    mmadParams.unitFlag = false; mmadParams.cmatrixInitVal = true;
    mmadParams.cmatrixSource = false;
    Mmad(l0cBuf, l0aBuf, l0bBuf, mmadParams);
}

TEST(CubePointerApiTest, Mmad_WithBias_PtrOverload_Compile)
{
    float l0cBuf[1024];
    half l0aBuf[1024];
    half l0bBuf[1024];
    float biasBuf[32];
    MmadParams mmadParams;
    mmadParams.m = 32; mmadParams.n = 32; mmadParams.k = 32;
    mmadParams.unitFlag = false; mmadParams.cmatrixInitVal = false;
    mmadParams.cmatrixSource = true;
    Mmad(l0cBuf, l0aBuf, l0bBuf, biasBuf, mmadParams);
}

// ============================================================================
// Fill 指针重载编译验证
// ============================================================================

TEST(CubePointerApiTest, Fill_PtrOverload_Compile)
{
    half l1Buf[1024];
    InitConstValueParams<uint16_t> params;
    params.repeatTimes = 1; params.blockNum = 1;
    params.dstGap = 0; params.initValue = 0;
    Fill(l1Buf, params);
}

// ============================================================================
// Fixpipe 指针重载编译验证
// ============================================================================

TEST(CubePointerApiTest, Fixpipe_PtrOverload_Compile)
{
    float gmBuf[1024];
    float l0cBuf[1024];
    FixpipeParamsV220 params;
    params.cburstNum = 1; params.burstLen = 1;
    params.srcStride = 32; params.dstStride = 32;
    Fixpipe(gmBuf, l0cBuf, params);
}

TEST(CubePointerApiTest, Fixpipe_Dequant_PtrOverload_Compile)
{
    float gmBuf[1024];
    float l0cBuf[1024];
    uint64_t workspace[16];
    FixpipeParamsV220 params;
    params.cburstNum = 1; params.burstLen = 1;
    params.srcStride = 32; params.dstStride = 32;
    Fixpipe(gmBuf, l0cBuf, workspace, params);
}

// ============================================================================
// Tensor vs Pointer 混合调用编译验证
// ============================================================================

TEST(CubePointerApiTest, MixedCall_LoadData_Compile)
{
    TPipe pipe;
    LocalTensor<half> l1Tensor;
    TBuffAddr tbuf;
    tbuf.logicPos = static_cast<uint8_t>(TPosition::A1);
    l1Tensor.SetAddr(tbuf);
    l1Tensor.InitBuffer(nullptr, 1024);
    half l0Buf[1024];
    LoadData2DParamsV2 params = {0, 0, 0, 0, 0, 0, true, 0};
    LoadData(l0Buf, l1Tensor, params);
}

TEST(CubePointerApiTest, MixedCall_Mmad_Compile)
{
    TPipe pipe;
    LocalTensor<half> fmTensor;
    TBuffAddr tbufA;
    tbufA.logicPos = static_cast<uint8_t>(TPosition::A2);
    fmTensor.SetAddr(tbufA);
    fmTensor.InitBuffer(nullptr, 1024);
    float l0cBuf[1024];
    half l0bBuf[1024];
    MmadParams mmadParams;
    mmadParams.m = 32; mmadParams.n = 32; mmadParams.k = 32;
    mmadParams.unitFlag = false; mmadParams.cmatrixInitVal = true;
    mmadParams.cmatrixSource = false;
    Mmad(l0cBuf, fmTensor, l0bBuf, mmadParams);
}
