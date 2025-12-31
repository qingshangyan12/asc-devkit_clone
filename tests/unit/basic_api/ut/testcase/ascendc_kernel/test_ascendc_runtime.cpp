/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <unordered_map>
#include <fcntl.h>

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "ascendc_runtime.h"
#include "securec.h"
#include "runtime/kernel.h"
#include "acl_rt.h"
#include "ascendc_tool_log.h"

#include <iostream>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
aclError aclrtBinaryGetFunction(const aclrtBinHandle binHandle, const char *kernelName,
                                                    aclrtFuncHandle *funcHandle) { return 0; }
aclError aclrtLaunchKernelWithHostArgs(aclrtFuncHandle funcHandle, uint32_t blockDim,
                                                           aclrtStream stream, aclrtLaunchKernelCfg *cfg,
                                                           void *hostArgs, size_t argsSize,
                                                           aclrtPlaceHolderInfo *placeHolderArray,
                                                           size_t placeHolderNum) { return 0; }

#ifdef __cplusplus
}
#endif

class TEST_ASCENDC_RUNTIME : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {
        GlobalMockObject::verify();
    }
};

TEST_F(TEST_ASCENDC_RUNTIME, ascendcRuntimeProfTest){
    const char *name = "xxx";
    uint32_t blockDim = 1;
    uint32_t taskType = 0;
    const uint64_t startTime = 0;
    MOCKER(ReportAscendProf).expects(once());
    ReportAscendProf(name, blockDim, taskType, startTime);
    EXPECT_NO_THROW(GlobalMockObject::verify());
}

TEST_F(TEST_ASCENDC_RUNTIME, ascendcRuntimeProfFixTest){
    const char *name = "xxx";
    uint32_t blockDim = 1;
    uint32_t taskType = 10;
    const uint64_t startTime = 0;
    ReportAscendProf(name, blockDim, taskType, startTime);
    EXPECT_NO_THROW(GlobalMockObject::verify());
}

#define ASCENDC_DUMP_SIZE 75 * 1024 * 1024
TEST_F(TEST_ASCENDC_RUNTIME, ascendcRuntimeRegisterTest){
    const char *name = "sss";
    void *handle;
    size_t fileSize = 3;
    uint32_t type = 0;
    uint32_t ret;
    ret = RegisterAscendBinary(name, fileSize, type, &handle);
    EXPECT_EQ(ret, 0);
    type = 1;
    ret = RegisterAscendBinary(name, fileSize, type, &handle);
    EXPECT_EQ(ret, 0);
    type = 2;
    ret = RegisterAscendBinary(name, fileSize, type, &handle);
    EXPECT_EQ(ret, 0);
    GetAscendCoreSyncAddr(&handle);
    AllocAscendMemDevice(&handle, fileSize);
    FreeAscendMemDevice(handle);
    UnregisterAscendBinary(handle);
}

TEST_F(TEST_ASCENDC_RUNTIME, ascendcRuntimeDevBinaryRegisterTest) {
    uint8_t fileBuf[32];
    size_t fileSize = 32;
    void *handle;
    int32_t ret;
    ret = AscendDevBinaryRegister(fileBuf, fileSize, &handle);
    EXPECT_EQ(ret, 0);
    const char* stubFunc = "hello_world";
    ret = AscendFunctionRegister(handle, stubFunc);
    EXPECT_EQ(ret, 0);
    MOCKER(rtKernelLaunchWithFlagV2).stubs().will(returnValue(0));
    uint32_t blockDim;
    void **args = nullptr;
    uint32_t size;
    rtStream_t stream = nullptr;
    ret = AscendKernelLaunchWithFlagV2(stubFunc, blockDim, args, size, stream, 0);
    EXPECT_EQ(ret, 0);
    ret = AscendKernelLaunchWithFlagV2(stubFunc, blockDim, args, size, stream, 2048);
    EXPECT_EQ(ret, 0);
}

TEST_F(TEST_ASCENDC_RUNTIME, ascendcRuntimeMemoryFailedTest){
    size_t bufsize = 16;
    uint32_t ret;
    MOCKER(aclrtMalloc).expects(once()).will(returnValue(1));
    ret = AllocAscendMemDevice(nullptr, bufsize);
    EXPECT_NE(ret, 0);
    ret = FreeAscendMemDevice(nullptr);
    EXPECT_NE(ret, 0);
}

TEST_F(TEST_ASCENDC_RUNTIME, ascendcRuntimeGetProfStatusTest){
    MOCKER(GetAscendProfStatus).expects(once()).will(returnValue(true));
    MOCKER(AscendProfRegister).expects(once());
    GetAscendProfStatus();
    AscendProfRegister();
    EXPECT_NO_THROW(GlobalMockObject::verify());
}

TEST_F(TEST_ASCENDC_RUNTIME, GetCoreNumForMixVectorCore){
    uint32_t aiCoreNum = 0;
    uint32_t vectorCoreNum = 0;
    GetCoreNumForMixVectorCore(&aiCoreNum, &vectorCoreNum);
    EXPECT_EQ(aiCoreNum, 0);
    EXPECT_EQ(vectorCoreNum, 0);
}

typedef void *rtEvent_t;
typedef void *rtStream_t;
typedef void *rtContext_t;

typedef struct {
    rtStream_t stream;
    rtEvent_t eventA;
    rtEvent_t eventB;
} AscendCStreamForVectorCore;

extern "C" uint32_t LaunchAscendKernelForVectorCore(const char *opType, void *handle, const uint64_t key, void **args, uint32_t size,
    const void *stream, bool enbaleProf, uint32_t aicBlockDim, uint32_t aivBlockDim, uint32_t aivBlockDimOffset);
extern "C" void AscendCDestroyStreamCallBack(rtStream_t stream, const bool isCreate);
extern std::unordered_map<const void *, AscendCStreamForVectorCore> g_ascStreamMap;
extern "C" uint32_t LaunchAscendKernel(void *handle, const uint64_t key, const uint32_t blockDim, void **args, uint32_t size,
    const rtStream_t stream);


TEST_F(TEST_ASCENDC_RUNTIME, LaunchVectorCore){
    const char* opType = "AddCustom";
    void * handle = nullptr;
    void **args = nullptr;
    void *stream = nullptr;
    AscendCStreamForVectorCore streamForVectorCore;
    AscendCDestroyStreamCallBack(stream, true);
    g_ascStreamMap[stream] = streamForVectorCore;
    AscendCDestroyStreamCallBack(stream, false);
    uint32_t ret = LaunchAscendKernelForVectorCore(opType, handle, 0, args, 0, stream, false, 1, 1, 0);
    EXPECT_EQ(ret, 0);
    ret = LaunchAscendKernelForVectorCore(opType, handle, 0, args, 0, stream, true, 1, 1, 0);
    EXPECT_EQ(ret, 0);
}

const char *fake_rtGetSocVersion()
{
    return "ascend910b2";
}

const char *fake_rtGetSocVersion1()
{
    return "ascend910b22";
}

TEST_F(TEST_ASCENDC_RUNTIME, TestAscendCheckSoCVersion)
{
    char errMsg[1024]; // max err msg length is 1024
    const char* socVersion = "ascend910b1";
    MOCKER(aclrtGetSocName).stubs().will(invoke(fake_rtGetSocVersion));
    bool ret = AscendCheckSoCVersion(socVersion, errMsg);
    EXPECT_EQ(ret, true);
    const char* socVersion1 = "ascend310p1";
    ret = AscendCheckSoCVersion(socVersion1, errMsg);
    EXPECT_EQ(ret, false);
    GlobalMockObject::verify();
    MOCKER(aclrtGetSocName).stubs().will(invoke(fake_rtGetSocVersion1));
    ret = AscendCheckSoCVersion(socVersion1, errMsg);
    EXPECT_EQ(ret, false);
    GlobalMockObject::verify();
    const char* socVersion2 = "ascend310p1xx";
    ret = AscendCheckSoCVersion(socVersion2, errMsg);
    EXPECT_EQ(ret, false);
}

TEST_F(TEST_ASCENDC_RUNTIME, LaunchAscendKernelA){
    void * handle = nullptr;
    void *stream = nullptr;
    void **args = nullptr;
    const uint32_t blockDim = 0;
    uint32_t size = 0;
    const uint64_t key = 0;
    aclrtLaunchKernelCfg *cfg = nullptr;
    aclrtLaunchKernelWithConfig(handle, blockDim, stream, cfg, nullptr, nullptr);
    uint32_t ret = LaunchAscendKernel(handle, key, blockDim, args, size, stream);
    EXPECT_EQ(ret, 0);
}

TEST_F(TEST_ASCENDC_RUNTIME, AscendDevBinaryLazyRegister) {
    const char* binBuf = "test_binary";
    size_t binSize = strlen(binBuf);
    void** handle = nullptr;
    int32_t ret = AscendDevBinaryLazyRegister(binBuf, binSize, handle);
    EXPECT_EQ(ret, 0);
}

TEST_F(TEST_ASCENDC_RUNTIME, AscendGetFuncFromBinary) {
    void* const binHandle = nullptr;
    const char* kernelName = "test_kernel";
    void** funcHandle = nullptr;
    int32_t ret = AscendGetFuncFromBinary(binHandle, kernelName, funcHandle);
    EXPECT_EQ(ret, 0);
}

TEST_F(TEST_ASCENDC_RUNTIME, AscendLaunchKernelWithHostArgs) {
    void* funcHandle = nullptr;
    uint32_t blockDim = 1;
    void* stream = nullptr;
    void* hostArgs = nullptr;
    size_t argsSize = 0;
    int32_t ret = AscendLaunchKernelWithHostArgs(funcHandle, blockDim, stream, hostArgs, argsSize, 0);
    EXPECT_EQ(ret, 0);
    uint32_t ubufDynamicSize = 512;
    ret = AscendLaunchKernelWithHostArgs(funcHandle, blockDim, stream, hostArgs, argsSize, ubufDynamicSize);
    EXPECT_EQ(ret, 0);
}

TEST_F(TEST_ASCENDC_RUNTIME, AscendCFunctionGetMetaInfoKtype) {
    void* funcHandle = nullptr;
    unsigned int kernelType = 5;
    uint32_t ret = AscendCFunctionGetMetaInfoKtype(funcHandle, &kernelType);
    EXPECT_EQ(ret, 0);
}

TEST_F(TEST_ASCENDC_RUNTIME, AscendCFunctionGetMetaInfoKCoreRation) {
    void* funcHandle = nullptr;
    unsigned short aicRation;
    unsigned short aivRation;
    uint32_t ret = AscendCFunctionGetMetaInfoKCoreRation(funcHandle, &aicRation, &aivRation);
    EXPECT_EQ(ret, 0);
}

TEST_F(TEST_ASCENDC_RUNTIME, AscendCFunctionGetMetaInfoKtypeFailed) {
    void* funcHandle = nullptr;
    unsigned int kernelType = 5;
    MOCKER(rtFunctionGetMetaInfo).stubs().will(returnValue(1));
    uint32_t ret = AscendCFunctionGetMetaInfoKtype(funcHandle, &kernelType);
    EXPECT_EQ(ret, 1);
}

TEST_F(TEST_ASCENDC_RUNTIME, AscendCFunctionGetMetaInfoKCoreRationFailed) {
    void* funcHandle = nullptr;
    unsigned short aicRation;
    unsigned short aivRation;
    MOCKER(rtFunctionGetMetaInfo).stubs().will(returnValue(1));
    uint32_t ret = AscendCFunctionGetMetaInfoKCoreRation(funcHandle, &aicRation, &aivRation);
    EXPECT_EQ(ret, 1);
}