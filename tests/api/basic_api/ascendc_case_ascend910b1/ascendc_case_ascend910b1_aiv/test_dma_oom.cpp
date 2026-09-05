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
#include "mockcpp/mockcpp.hpp"
#define ASCENDC_OOM 1
#include "kernel_utils.h"
#include "kernel_common.h"
#include "kernel_operator.h"
#undef ASCENDC_OOM

using namespace std;
using namespace AscendC;
class TestDMAOom : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(TestDMAOom, TestCheckGmMemOverflow)
{
    const uint64_t len = 1024;
    uint8_t* workspaceGm = (uint8_t*)AscendC::GmAlloc(len * sizeof(uint8_t));

    bool isSrc = true;
    uint64_t gmLen = 256;
    for (int i = 0; i < 8; i++) {
        g_oomAddrArange.addr[i] = reinterpret_cast<uintptr_t>(workspaceGm + 128);
        g_oomAddrArange.len[i] = 128;
    }
    g_oomAddrArange.count = 8;
    g_oomAddrArange.len[0] = 0;
    AscendCUtils::CheckGmMemOverflow(workspaceGm, isSrc, 0);
    AscendCUtils::CheckGmMemOverflow(workspaceGm, isSrc, gmLen);
    AscendCUtils::CheckGmMemOverflow(workspaceGm + 128, isSrc, 1024);
    AscendCUtils::CheckGmMemOverflow(workspaceGm + 128, isSrc, 128);
    AscendCUtils::CheckGmMemOverflow(workspaceGm + 1024, isSrc, gmLen);
    EXPECT_EQ(g_oomAddrArange.len[1], 128);
    g_oomAddrArange.count = 0;
    AscendCUtils::CheckGmMemOverflow(workspaceGm, isSrc, gmLen);
    AscendC::GmFree((void*)workspaceGm);
}

TEST_F(TestDMAOom, TestOOMCheckAddrRange)
{
    const uint64_t len = 1024;
    uint8_t* workspaceGm = (uint8_t*)AscendC::GmAlloc(len * sizeof(uint8_t));
    OOMInit();
    OOMCheckAddrRange(workspaceGm, len);
    EXPECT_EQ(g_oomAddrArange.len[0], len);
    AscendC::GmFree((void*)workspaceGm);
}

TEST_F(TestDMAOom, TestOOMCheckTensorListRange)
{
    const uint64_t len = 1024;
    uint8_t* argsGm = (uint8_t*)AscendC::GmAlloc(len * sizeof(uint8_t));
    uint64_t* dynamicPtr = (uint64_t*)argsGm;
    *(dynamicPtr) = 0x28;
    *(dynamicPtr + 1) = 0x0000000100000001;
    *(dynamicPtr + 2) = 2048;
    *(dynamicPtr + 3) = 0x0000000100000001;
    *(dynamicPtr + 4) = 2048;
    uint8_t* argsGm1 = (uint8_t*)AscendC::GmAlloc(len * sizeof(uint8_t));
    uint8_t* argsGm2 = (uint8_t*)AscendC::GmAlloc(len * sizeof(uint8_t));
    *(dynamicPtr + 5) = reinterpret_cast<uint64_t>(argsGm1);
    *(dynamicPtr + 6) = reinterpret_cast<uint64_t>(argsGm2);
    uint64_t tmpAddr = *(dynamicPtr + 5);

    OOMInit();
    OOMCheckTensorListRange(dynamicPtr, 2);
    EXPECT_EQ(g_oomAddrArange.len[0], 2);

    uintptr_t inputOutputAddr = 0;
    uint64_t inputOutputLen = 0;
    bool ret = OOMCheckAddrInTensorList(0, tmpAddr, inputOutputAddr, inputOutputLen);
    EXPECT_EQ(ret, true);
    OOMInit();
    AscendC::GmFree((void*)argsGm);
    AscendC::GmFree((void*)argsGm1);
    AscendC::GmFree((void*)argsGm2);
}

TEST_F(TestDMAOom, TestOOMStorageShapeTensorDescriptor)
{
    uint8_t* metadata = static_cast<uint8_t*>(AscendC::GmAlloc(64));
    uint8_t* tensor = static_cast<uint8_t*>(AscendC::GmAlloc(128));
    *reinterpret_cast<uint64_t*>(metadata) = g_oomStorageShapeMagic;
    *reinterpret_cast<uint32_t*>(metadata + 8) = g_oomStorageShapeVersion;
    uint8_t* cursor = metadata + g_oomStorageShapeHeaderSize;
    *reinterpret_cast<uint64_t*>(cursor) = 40;
    *reinterpret_cast<uint32_t*>(cursor + 8) = g_oomDescriptorKindTensor;
    *reinterpret_cast<uint32_t*>(cursor + 16) = g_oomTensorViewVersion;
    *reinterpret_cast<uint32_t*>(cursor + 20) = 2;
    *reinterpret_cast<uint64_t*>(cursor + 24) = 2;
    *reinterpret_cast<uint64_t*>(cursor + 32) = 8;

    OOMInit();
    EXPECT_TRUE(OOMHasStorageShapeHeader(metadata));
    EXPECT_TRUE(OOMTryRegisterTensorWithStorageShape(cursor, tensor, 4));
    EXPECT_EQ(cursor, metadata + 56);
    EXPECT_EQ(g_oomAddrArange.count, 1);
    EXPECT_EQ(g_oomAddrArange.len[0], 64);

    OOMInit();
    uint8_t* noMagicCursor = metadata;
    *reinterpret_cast<uint64_t*>(metadata) = 0;
    EXPECT_FALSE(OOMHasStorageShapeHeader(noMagicCursor));
    EXPECT_EQ(g_oomAddrArange.count, 0);
    AscendC::GmFree(metadata);
    AscendC::GmFree(tensor);
}

TEST_F(TestDMAOom, TestOOMStorageShapeTensorListDescriptor)
{
    uint8_t* tensorList = static_cast<uint8_t*>(AscendC::GmAlloc(128));
    uint8_t* tensor0 = static_cast<uint8_t*>(AscendC::GmAlloc(128));
    uint8_t* tensor1 = static_cast<uint8_t*>(AscendC::GmAlloc(128));
    auto dynamicPtr = reinterpret_cast<uint64_t*>(tensorList);
    dynamicPtr[0] = 40;
    dynamicPtr[5] = reinterpret_cast<uint64_t>(tensor0);
    dynamicPtr[6] = reinterpret_cast<uint64_t>(tensor1);

    uint8_t* metadata = static_cast<uint8_t*>(AscendC::GmAlloc(128));
    uint8_t* cursor = metadata;
    *reinterpret_cast<uint64_t*>(cursor) = 72;
    *reinterpret_cast<uint32_t*>(cursor + 8) = g_oomDescriptorKindTensorList;
    *reinterpret_cast<uint32_t*>(cursor + 12) = 0;  // U mode
    *reinterpret_cast<uint64_t*>(cursor + 16) = 2;
    *reinterpret_cast<uint32_t*>(cursor + 24) = g_oomTensorViewVersion;
    *reinterpret_cast<uint32_t*>(cursor + 28) = 2;
    *reinterpret_cast<uint64_t*>(cursor + 32) = 2;
    *reinterpret_cast<uint64_t*>(cursor + 40) = 8;
    *reinterpret_cast<uint32_t*>(cursor + 48) = g_oomTensorViewVersion;
    *reinterpret_cast<uint32_t*>(cursor + 52) = 2;
    *reinterpret_cast<uint64_t*>(cursor + 56) = 3;
    *reinterpret_cast<uint64_t*>(cursor + 64) = 4;

    OOMInit();
    EXPECT_TRUE(OOMTryRegisterTensorListWithStorageShape(cursor, tensorList, 4));
    EXPECT_EQ(g_oomAddrArange.count, 2);
    EXPECT_EQ(g_oomAddrArange.len[0], 64);
    EXPECT_EQ(g_oomAddrArange.len[1], 48);

    OOMInit();
    cursor = metadata;
    *reinterpret_cast<uint32_t*>(cursor + 12) = 1;  // F mode
    EXPECT_TRUE(OOMTryRegisterTensorListWithStorageShape(cursor, tensorList, 4));
    EXPECT_EQ(g_oomAddrArange.count, 2);
    EXPECT_EQ(g_oomAddrArange.len[0], 64);
    EXPECT_EQ(g_oomAddrArange.len[1], 48);

    AscendC::GmFree(metadata);
    AscendC::GmFree(tensorList);
    AscendC::GmFree(tensor0);
    AscendC::GmFree(tensor1);
}
