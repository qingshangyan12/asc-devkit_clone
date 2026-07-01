/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "acl_rt_compile.h"
#include "stub/asrtc.h"

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include <dlfcn.h>

class TEST_ACL_RT_COMPILE : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() { GlobalMockObject::verify(); }
};

using asrtcCreateProgramFuncPtr =
    asrtcResult (*)(asrtcProgram*, const char*, const char*, int, const char* const*, const char* const*);
using asrtcDestroyProgramFuncPtr = asrtcResult (*)(asrtcProgram*);
using asrtcCompileProgramFuncPtr = asrtcResult (*)(asrtcProgram, int, const char* const*);
using asrtcGetDeviceELFSizeFuncPtr = asrtcResult (*)(asrtcProgram, size_t*);
using asrtcGetDeviceELFFuncPtr = asrtcResult (*)(asrtcProgram, char*);
using asrtcAddNameExpressionFuncPtr = asrtcResult (*)(asrtcProgram, const char* const);
using asrtcGetLoweredNameFuncPtr = asrtcResult (*)(asrtcProgram, const char*, const char**);
using asrtcGetProgramLogSizeFuncPtr = asrtcResult (*)(asrtcProgram, size_t*);
using asrtcGetProgramLogFuncPtr = asrtcResult (*)(asrtcProgram, char*);

extern asrtcCreateProgramFuncPtr asrtcCreateProgramPtr;
extern asrtcDestroyProgramFuncPtr asrtcDestroyProgramPtr;
extern asrtcCompileProgramFuncPtr asrtcCompileProgramPtr;
extern asrtcGetDeviceELFSizeFuncPtr asrtcGetDeviceELFSizePtr;
extern asrtcGetDeviceELFFuncPtr asrtcGetDeviceELFPtr;
extern asrtcAddNameExpressionFuncPtr asrtcAddNameExpressionPtr;
extern asrtcGetLoweredNameFuncPtr asrtcGetLoweredNamePtr;
extern asrtcGetProgramLogSizeFuncPtr asrtcGetProgramLogSizePtr;
extern asrtcGetProgramLogFuncPtr asrtcGetProgramLogPtr;

extern aclError aclrtcAddNameExpr(aclrtcProg prog, const char* nameExpr);
extern aclError aclrtcGetLoweredName(aclrtcProg prog, const char* nameExpr, const char** manglingName);
extern inline bool EndsWith(std::string_view str, std::string_view suffix);
extern std::string ExtractCannPath(const std::string& pluginPath);
extern bool PathCheck(const char* path);
extern std::string GetCannPath();
extern std::string GetAsrtcLibPath(const std::string& cannPath);
extern void* handle;
extern aclError LoadExtraLib();
extern void UnloadExtraLib();
extern void ResetAsrtcSymbols();

extern const int ACL_ERROR_RTC_INVALID_PROG = 176000;
extern const int ACL_ERROR_RTC_INVALID_INPUT = 176001;
extern const int ACL_ERROR_RTC_INVALID_OPTION = 176002;
extern const int ACL_ERROR_RTC_COMPILATION = 176003;
extern const int ACL_ERROR_RTC_LINKING = 176004;
extern const int ACL_ERROR_RTC_NO_NAME_EXPR_AFTER_COMPILATION = 176005;
extern const int ACL_ERROR_RTC_NO_LOWERED_NAMES_BEFORE_COMPILATION = 176006;
extern const int ACL_ERROR_RTC_NAME_EXPR_NOT_VALID = 176007;
extern const int ACL_ERROR_RTC_PROG_CREATION_FAILURE = 276000;
extern const int ACL_ERROR_RTC_OUT_OF_MEMORY = 276001;
extern const int ACL_ERROR_RTC_FAILURE = 576000;
extern const int ACL_SUCCESS = 0;

namespace {
int gDlopenCount = 0;
int gDlsymCount = 0;

void ResetLoadStateForTest()
{
    handle = nullptr;
    ResetAsrtcSymbols();
    gDlopenCount = 0;
    gDlsymCount = 0;
}

void* DlopenStub(const char*, int)
{
    ++gDlopenCount;
    return reinterpret_cast<void*>(0x1234);
}

void* DlsymSuccessStub(void*, const char*)
{
    ++gDlsymCount;
    return reinterpret_cast<void*>(0x1);
}

void* DlsymMissingCompileProgramStub(void*, const char* symbol)
{
    ++gDlsymCount;
    if (std::string(symbol) == "asrtcCompileProgram") {
        return nullptr;
    }
    return reinterpret_cast<void*>(0x1);
}
} // namespace

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcGetCompileLogSize)
{
    MOCKER(PathCheck).stubs().will(returnValue(true));
    asrtcGetProgramLogSizeFuncPtr originalPtr = asrtcGetProgramLogSizePtr;
    auto mockFunc = [](asrtcProgram, size_t*) -> asrtcResult { return ASRTC_ERROR_NOT_IMPLEMENTED; };
    asrtcGetProgramLogSizePtr = mockFunc;
    aclrtcProg prog = nullptr;
    size_t actualLogSize = 1024;
    aclError result = aclrtcGetCompileLogSize(prog, &actualLogSize);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_PROG);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcGetCompileLog)
{
    asrtcGetProgramLogFuncPtr originalPtr = asrtcGetProgramLogPtr;
    auto mockFunc = [](asrtcProgram, char*) -> asrtcResult { return ASRTC_ERROR_NOT_IMPLEMENTED; };
    asrtcGetProgramLogPtr = mockFunc;
    aclrtcProg prog = nullptr;
    char log[32] = "some log info ...";
    aclError result = aclrtcGetCompileLog(prog, log);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_PROG);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcAddNameExpr_nullptr)
{
    aclrtcProg prog = nullptr;
    const char* nameExpr = "hello_world";
    aclError result = aclrtcAddNameExpr(prog, nameExpr);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_PROG);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcAddNameExpr)
{
    asrtcAddNameExpressionFuncPtr originalPtr = asrtcAddNameExpressionPtr;
    auto mockFunc = [](asrtcProgram, const char* const) -> asrtcResult {
        return ASRTC_ERROR_NAME_EXPRESSION_NOT_VALID;
    };
    asrtcAddNameExpressionPtr = mockFunc;

    alignas(void*) char fakeProgMem[64] = {0};
    aclrtcProg prog = reinterpret_cast<aclrtcProg>(fakeProgMem);
    const char* nameExpr = "hello_world";
    aclError result = aclrtcAddNameExpr(prog, nameExpr);
    EXPECT_EQ(result, ACL_ERROR_RTC_NAME_EXPR_NOT_VALID);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcGetLoweredName_nullptr)
{
    aclrtcProg prog = nullptr;
    const char* nameExpr = "hello_world";
    const char* loweredName = "hello_world";
    aclError result = aclrtcGetLoweredName(prog, nameExpr, &loweredName);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_PROG);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcGetLoweredName)
{
    asrtcGetLoweredNameFuncPtr originalPtr = asrtcGetLoweredNamePtr;
    auto mockFunc = [](asrtcProgram, const char*, const char**) -> asrtcResult {
        return ASRTC_ERROR_NO_NAME_EXPRESSION_AFTER_COMPILATION;
    };
    asrtcGetLoweredNamePtr = mockFunc;

    alignas(void*) char fakeProgMem[64] = {0};
    aclrtcProg prog = reinterpret_cast<aclrtcProg>(fakeProgMem);
    const char* nameExpr = "hello_world";
    const char* loweredName = "hello_world";
    aclError result = aclrtcGetLoweredName(prog, nameExpr, &loweredName);
    EXPECT_EQ(result, ACL_ERROR_RTC_NO_NAME_EXPR_AFTER_COMPILATION);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCompileProg)
{
    asrtcCompileProgramFuncPtr originalPtr = asrtcCompileProgramPtr;
    auto mockFunc = [](asrtcProgram, int, const char* const*) -> asrtcResult { return ASRTC_ERROR_COMPILE; };
    asrtcCompileProgramPtr = mockFunc;

    aclrtcProg prog = nullptr;
    const char* options[] = {"--npu-arch=dav-2201", "-O2"};
    int numOptions = sizeof(options) / sizeof(options[0]);
    MOCKER(PathCheck).stubs().will(returnValue(true));
    aclError result = aclrtcCompileProg(prog, numOptions, options);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_PROG);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCompileProg_unknown)
{
    asrtcCompileProgramFuncPtr originalPtr = asrtcCompileProgramPtr;
    auto mockFunc = [](asrtcProgram, int, const char* const*) -> asrtcResult { return ASRTC_ERROR_UNKNOWN; };
    asrtcCompileProgramPtr = mockFunc;

    aclrtcProg prog = nullptr;
    const char* options[] = {"--npu-arch=dav-2201", "-O2"};
    int numOptions = sizeof(options) / sizeof(options[0]);
    MOCKER(PathCheck).stubs().will(returnValue(true));
    aclError result = aclrtcCompileProg(prog, numOptions, options);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_PROG);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCompileProg_negative_num_options)
{
    auto mockFunc = [](asrtcProgram, int, const char* const*) -> asrtcResult { return ASRTC_SUCCESS; };
    asrtcCompileProgramPtr = mockFunc;
    MOCKER(PathCheck).stubs().will(returnValue(true));
    alignas(void*) char fakeProgMem[64] = {0};
    aclrtcProg prog = reinterpret_cast<aclrtcProg>(fakeProgMem);
    aclError result = aclrtcCompileProg(prog, -1, nullptr);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_INPUT);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCompileProg_null_options_with_positive_num_options)
{
    MOCKER(PathCheck).stubs().will(returnValue(true));
    alignas(void*) char fakeProgMem[64] = {0};
    aclrtcProg prog = reinterpret_cast<aclrtcProg>(fakeProgMem);
    aclError result = aclrtcCompileProg(prog, 1, nullptr);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_INPUT);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCompileProg_null_option_item)
{
    MOCKER(PathCheck).stubs().will(returnValue(true));
    alignas(void*) char fakeProgMem[64] = {0};
    aclrtcProg prog = reinterpret_cast<aclrtcProg>(fakeProgMem);
    const char* options[] = {nullptr};
    aclError result = aclrtcCompileProg(prog, 1, options);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_INPUT);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCompileProg_empty_option_item)
{
    MOCKER(PathCheck).stubs().will(returnValue(true));
    alignas(void*) char fakeProgMem[64] = {0};
    aclrtcProg prog = reinterpret_cast<aclrtcProg>(fakeProgMem);
    const char* options[] = {""};
    aclError result = aclrtcCompileProg(prog, 1, options);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_OPTION);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCompileProg_empty_option_before_valid_option)
{
    MOCKER(PathCheck).stubs().will(returnValue(true));
    alignas(void*) char fakeProgMem[64] = {0};
    aclrtcProg prog = reinterpret_cast<aclrtcProg>(fakeProgMem);
    const char* options[] = {"", "--npu-arch=dav-2201"};
    aclError result = aclrtcCompileProg(prog, 2, options);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_OPTION);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCompileProg_empty_option_after_valid_option)
{
    MOCKER(PathCheck).stubs().will(returnValue(true));
    alignas(void*) char fakeProgMem[64] = {0};
    aclrtcProg prog = reinterpret_cast<aclrtcProg>(fakeProgMem);
    const char* options[] = {"--npu-arch=dav-2201", ""};
    aclError result = aclrtcCompileProg(prog, 2, options);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_OPTION);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCompileProg_zero_num_options_allows_null_options)
{
    auto mockFunc = [](asrtcProgram, int, const char* const*) -> asrtcResult { return ASRTC_SUCCESS; };
    asrtcCompileProgramPtr = mockFunc;
    MOCKER(PathCheck).stubs().will(returnValue(true));

    alignas(void*) char fakeProgMem[64] = {0};
    aclrtcProg prog = reinterpret_cast<aclrtcProg>(fakeProgMem);
    aclError result = aclrtcCompileProg(prog, 0, nullptr);
    EXPECT_EQ(result, ACL_SUCCESS);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCompileProg_uses_only_num_options_entries)
{
    constexpr int kDefaultOptionCount = 6;
    constexpr int kUserOptionCount = 2;
    auto mockFunc = [](asrtcProgram, int numOptions, const char* const* options) -> asrtcResult {
        EXPECT_EQ(numOptions, kDefaultOptionCount + kUserOptionCount);
        EXPECT_STREQ(options[kDefaultOptionCount], "--npu-arch=dav-2201");
        EXPECT_STREQ(options[kDefaultOptionCount + 1], "-O2");
        return ASRTC_SUCCESS;
    };
    asrtcCompileProgramPtr = mockFunc;
    MOCKER(PathCheck).stubs().will(returnValue(true));

    alignas(void*) char fakeProgMem[64] = {0};
    aclrtcProg prog = reinterpret_cast<aclrtcProg>(fakeProgMem);
    const char* options[] = {"--npu-arch=dav-2201", "-O2", "--ignored"};
    aclError result = aclrtcCompileProg(prog, kUserOptionCount, options);
    EXPECT_EQ(result, ACL_SUCCESS);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCreateProg)
{
    asrtcCreateProgramFuncPtr originalPtr = asrtcCreateProgramPtr;
    auto mockFunc = [](asrtcProgram*, const char*, const char*, int, const char* const*,
                       const char* const*) -> asrtcResult { return ASRTC_SUCCESS; };
    asrtcCreateProgramPtr = mockFunc;

    aclrtcProg prog = nullptr;
    const char* src = R""""(
#include "const.h"
#include "kernel_operator.h"
#include "acl/acl.h"

__global__ __aicore__ void add_custom(GM_ADDR x) {*x = 3 + MY_CONST;}
// extern "C" __global__ __aicore__ void add_custom(GM_ADDR x) {*x = 3 + MY_CONST;}
)"""";
    MOCKER(LoadExtraLib).stubs().will(returnValue(ACL_SUCCESS));
    aclError result = aclrtcCreateProg(&prog, src, "test_kernel", 0, nullptr, nullptr);
    EXPECT_EQ(result, ACL_SUCCESS);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcCreateProg_fail)
{
    aclrtcProg prog = nullptr;
    const char* src = R""""(
#include "const.h"
#include "kernel_operator.h"
#include "acl/acl.h"

__global__ __aicore__ void add_custom(GM_ADDR x) {*x = 3 + MY_CONST;}
// extern "C" __global__ __aicore__ void add_custom(GM_ADDR x) {*x = 3 + MY_CONST;}
)"""";
    MOCKER(LoadExtraLib).stubs().will(returnValue(ACL_ERROR_RTC_FAILURE));
    aclError result = aclrtcCreateProg(&prog, src, "test_kernel", 0, nullptr, nullptr);
    EXPECT_EQ(result, ACL_ERROR_RTC_FAILURE);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcDestroyProg)
{
    asrtcDestroyProgramFuncPtr originalPtr = asrtcDestroyProgramPtr;
    auto mockFunc = [](asrtcProgram*) -> asrtcResult { return ASRTC_SUCCESS; };
    const char* src = R""""(
#include "const.h"
#include "kernel_operator.h"
#include "acl/acl.h"

__global__ __aicore__ void add_custom(GM_ADDR x) {*x = 3 + MY_CONST;}
// extern "C" __global__ __aicore__ void add_custom(GM_ADDR x) {*x = 3 + MY_CONST;}
)"""";
    asrtcDestroyProgramPtr = mockFunc;
    MOCKER(LoadExtraLib).stubs().will(returnValue(ACL_SUCCESS));
    aclrtcProg prog = nullptr;
    aclrtcCreateProg(&prog, src, "test_kernel", 0, nullptr, nullptr);
    aclError result = aclrtcDestroyProg(&prog);
    EXPECT_EQ(result, ACL_SUCCESS);
    EXPECT_EQ(prog, nullptr);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcDestroyProg_null_program_handle)
{
    aclrtcProg prog = nullptr;
    aclError result = aclrtcDestroyProg(&prog);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_PROG);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcGetBinData)
{
    asrtcGetDeviceELFFuncPtr originalPtr = asrtcGetDeviceELFPtr;
    auto mockFunc = [](asrtcProgram, char*) -> asrtcResult { return ASRTC_ERROR_COMPILE; };
    asrtcGetDeviceELFPtr = mockFunc;

    aclrtcProg prog = nullptr;
    char binData[32] = "some bin data ...";
    aclError result = aclrtcGetBinData(prog, binData);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_PROG);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_aclrtcGetBinDataSize)
{
    asrtcGetDeviceELFSizeFuncPtr originalPtr = asrtcGetDeviceELFSizePtr;
    auto mockFunc = [](asrtcProgram, size_t*) -> asrtcResult { return ASRTC_ERROR_COMPILE; };
    asrtcGetDeviceELFSizePtr = mockFunc;

    aclrtcProg prog = nullptr;
    size_t binDataSizeRet = 1024;
    aclError result = aclrtcGetBinDataSize(prog, &binDataSizeRet);
    EXPECT_EQ(result, ACL_ERROR_RTC_INVALID_PROG);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_EndsWith)
{
    std::string srcStr = "empty";
    std::string suffix = "not empty";
    EXPECT_EQ(EndsWith(srcStr, suffix), false);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_ExtractCannPath)
{
    std::string pluginPath = "/usr/local/Ascend/cann/x86_64-linux/lib64/libacl_rtc.so";
    std::string res = ExtractCannPath(pluginPath);
    EXPECT_EQ(res, std::string("/usr/local/Ascend/cann"));
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_ExtractCannPath_aarch64)
{
    std::string pluginPath = "/usr/local/Ascend/cann/aarch64-linux/lib64/libacl_rtc.so";
    std::string res = ExtractCannPath(pluginPath);
    EXPECT_EQ(res, std::string("/usr/local/Ascend/cann"));
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_GetAsrtcLibPath)
{
    std::string cannPath = "/usr/local/Ascend/cann-9.1.0";
    std::string res = GetAsrtcLibPath(cannPath);
    EXPECT_EQ(res, std::string("/usr/local/Ascend/cann-9.1.0/tools/bisheng_compiler/lib/libasrtc.so"));
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_PathCheck)
{
    const char* path = "../../../../tests/tools/aclrtc/stub/asrtc.h";
    bool res = PathCheck(path);
    EXPECT_EQ(res, true);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_UnloadExtraLib)
{
    handle = reinterpret_cast<void*>(0x1234);
    asrtcCreateProgramPtr = reinterpret_cast<asrtcCreateProgramFuncPtr>(0x1);
    asrtcDestroyProgramPtr = reinterpret_cast<asrtcDestroyProgramFuncPtr>(0x1);
    asrtcCompileProgramPtr = reinterpret_cast<asrtcCompileProgramFuncPtr>(0x1);
    asrtcGetDeviceELFSizePtr = reinterpret_cast<asrtcGetDeviceELFSizeFuncPtr>(0x1);
    asrtcGetDeviceELFPtr = reinterpret_cast<asrtcGetDeviceELFFuncPtr>(0x1);
    asrtcAddNameExpressionPtr = reinterpret_cast<asrtcAddNameExpressionFuncPtr>(0x1);
    asrtcGetLoweredNamePtr = reinterpret_cast<asrtcGetLoweredNameFuncPtr>(0x1);
    asrtcGetProgramLogSizePtr = reinterpret_cast<asrtcGetProgramLogSizeFuncPtr>(0x1);
    asrtcGetProgramLogPtr = reinterpret_cast<asrtcGetProgramLogFuncPtr>(0x1);
    MOCKER(dlclose).expects(exactly(1)).will(returnValue(0));
    UnloadExtraLib();
    ASSERT_EQ(handle, nullptr);
    ASSERT_EQ(asrtcCreateProgramPtr, nullptr);
    ASSERT_EQ(asrtcDestroyProgramPtr, nullptr);
    ASSERT_EQ(asrtcCompileProgramPtr, nullptr);
    ASSERT_EQ(asrtcGetDeviceELFSizePtr, nullptr);
    ASSERT_EQ(asrtcGetDeviceELFPtr, nullptr);
    ASSERT_EQ(asrtcAddNameExpressionPtr, nullptr);
    ASSERT_EQ(asrtcGetLoweredNamePtr, nullptr);
    ASSERT_EQ(asrtcGetProgramLogSizePtr, nullptr);
    ASSERT_EQ(asrtcGetProgramLogPtr, nullptr);
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_LoadExtraLib)
{
    ResetLoadStateForTest();
    MOCKER(GetCannPath).stubs().will(returnValue(std::string("")));
    MOCKER(PathCheck).stubs().will(returnValue(true));
    MOCKER(dlopen).stubs().will(returnValue((void*)("fake_handle")));
    MOCKER(dlsym).stubs().will(returnValue((void*)("fake_handle")));
    aclError ret = LoadExtraLib();
    ASSERT_EQ(ret, ACL_SUCCESS);
    ResetLoadStateForTest();
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_LoadExtraLib_fail)
{
    ResetLoadStateForTest();
    MOCKER(GetCannPath).stubs().will(returnValue(std::string("")));
    MOCKER(PathCheck).stubs().will(returnValue(true));
    MOCKER(dlopen).stubs().will(returnValue(static_cast<void*>(nullptr)));
    LoadExtraLib();
    ASSERT_EQ(handle, nullptr);
    ResetLoadStateForTest();
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_LoadExtraLib_dlsym_fail)
{
    ResetLoadStateForTest();
    MOCKER(GetCannPath).stubs().will(returnValue(std::string("")));
    MOCKER(PathCheck).stubs().will(returnValue(true));
    MOCKER(dlopen).expects(once()).will(invoke(DlopenStub));
    MOCKER(dlsym).stubs().will(invoke(DlsymMissingCompileProgramStub));
    MOCKER(dlclose).expects(once()).will(returnValue(0));
    aclError ret = LoadExtraLib();
    ASSERT_EQ(ret, ACL_ERROR_RTC_FAILURE);
    ASSERT_EQ(handle, nullptr);
    ASSERT_EQ(asrtcCompileProgramPtr, nullptr);
    ASSERT_EQ(gDlopenCount, 1);
    ResetLoadStateForTest();
}

TEST_F(TEST_ACL_RT_COMPILE, aclrtc_LoadExtraLib_idempotent)
{
    ResetLoadStateForTest();
    MOCKER(GetCannPath).stubs().will(returnValue(std::string("")));
    MOCKER(PathCheck).stubs().will(returnValue(true));
    MOCKER(dlopen).expects(once()).will(invoke(DlopenStub));
    MOCKER(dlsym).stubs().will(invoke(DlsymSuccessStub));
    aclError first = LoadExtraLib();
    aclError second = LoadExtraLib();
    ASSERT_EQ(first, ACL_SUCCESS);
    ASSERT_EQ(second, ACL_SUCCESS);
    ASSERT_EQ(gDlopenCount, 1);
    ASSERT_EQ(gDlsymCount, 9);
    ResetLoadStateForTest();
}
