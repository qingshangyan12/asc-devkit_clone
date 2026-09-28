/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// Standalone host-only test: compile with production include paths, log stub,
// -ffunction-sections -fdata-sections and link with --gc-sections -pthread.
// Link ccu_timestamp.cc and ccu_print.cc; do not link the real ACL runtime.
#include "ccu_timestamp.h"
#include "load_kernel.h"
#include "hccl_host_comm_dl.h"
#include "hcomm_dlsym.h"
#include <acl/acl_rt.h>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace {
int launches = 0, aborts = 0, destroys = 0, frees = 0;
bool failLaunch = true, failAbort = true;
uint64_t argument = 0;
aclrtContext context = reinterpret_cast<void*>(1);
Mc2HcclCommStateCallback callback = nullptr;
} // namespace
void HcommDlInit() {}
bool HcclMc2IsSupportCommStateCallback() { return true; }
HcclResult HcclMc2CommRegStateCallback(const char*, Mc2HcclCommStateCallback cb, void*)
{
    callback = cb;
    return HCCL_SUCCESS;
}
namespace mc2_ops_hccl {
aclrtBinHandle g_binKernelHandle = reinterpret_cast<void*>(1);
HcclResult LoadAICPUKernel() { return HCCL_SUCCESS; }
} // namespace mc2_ops_hccl
extern "C" {
aclError aclrtGetCurrentContext(aclrtContext* out)
{
    *out = context;
    return 0;
}
aclError aclrtSetCurrentContext(aclrtContext in)
{
    context = in;
    return 0;
}
aclError aclrtMalloc(void** out, size_t size, aclrtMemMallocPolicy policy)
{
    assert(policy == ACL_MEM_MALLOC_NORMAL_ONLY);
    *out = std::malloc(size);
    return *out ? 0 : 1;
}
aclError aclrtFree(void* ptr)
{
    ++frees;
    std::free(ptr);
    return 0;
}
aclError aclrtMemset(void* ptr, size_t max, int32_t value, size_t size)
{
    assert(size <= max);
    std::memset(ptr, value, size);
    return 0;
}
aclError aclrtBinaryGetFunction(aclrtBinHandle, const char* name, aclrtFuncHandle* fn)
{
    assert(std::strcmp(name, "CcuTimestampKernel") == 0);
    *fn = reinterpret_cast<void*>(2);
    return 0;
}
aclError aclrtKernelArgsInit(aclrtFuncHandle, aclrtArgsHandle* out)
{
    *out = reinterpret_cast<void*>(3);
    return 0;
}
aclError aclrtKernelArgsAppend(aclrtArgsHandle, void* value, size_t size, aclrtParamHandle* out)
{
    assert(size == sizeof(argument));
    std::memcpy(&argument, value, size);
    *out = reinterpret_cast<void*>(4);
    return 0;
}
aclError aclrtKernelArgsFinalize(aclrtArgsHandle) { return 0; }
aclError aclrtCreateStreamWithConfig(aclrtStream* out, uint32_t, uint32_t)
{
    *out = reinterpret_cast<void*>(5);
    return 0;
}
aclError aclrtLaunchKernelWithConfig(
    aclrtFuncHandle, uint32_t blocks, aclrtStream, aclrtLaunchKernelCfg*, aclrtArgsHandle, void*)
{
    assert(blocks == 1 && argument != 0);
    ++launches;
    return failLaunch ? 1 : 0;
}
aclError aclrtStreamAbort(aclrtStream)
{
    ++aborts;
    return failAbort ? 1 : 0;
}
aclError aclrtDestroyStream(aclrtStream)
{
    ++destroys;
    return 0;
}
}
int main()
{
    using namespace AscendC::ccu::dfx;
    assert(StartAicpuTimestamp() == 0);
    assert(launches == 1 && destroys == 1 && frees == 0);
    failLaunch = false;
    const uint64_t slot = StartAicpuTimestamp();
    assert(slot == argument && slot != 0 && *reinterpret_cast<uint64_t*>(slot) == 0);
    assert(StartAicpuTimestamp() == slot && launches == 2);
    // No stream/device synchronization mock is provided: accidentally waiting
    // on the infinite AICPU kernel makes this executable fail to link.
    std::string output;
    const uint64_t samples[]{100, 145, 145, 201};
    assert(detail::Format("a=%t b=%t", samples, 4, output) && output == "a=45 b=56");
    assert(!detail::ValidateFormat("%t", 1));
    assert(detail::ValidateFormat("%% %u %x %p %v", 4));
    const uint64_t stale[]{100, 100};
    output.clear();
    assert(detail::Format("%t", stale, 2, output) && output == "unresolved");
    const uint64_t unstarted[]{0, 100};
    output.clear();
    assert(detail::Format("%t", unstarted, 2, output) && output == "unresolved");
    assert(callback != nullptr);
    assert(callback(nullptr, 1, nullptr) == HCCL_SUCCESS);
    assert(aborts == 1 && frees == 0);
    assert(StartAicpuTimestamp() == 0); // Do not restart a possibly live worker.
    failAbort = false;
    StopAicpuTimestamps();
    assert(aborts == 2 && destroys == 2 && frees == 0);
    StopAicpuTimestamps();
    assert(aborts == 2);
    std::cout << "CCU timestamp host mocks passed (not a hardware/cache/abort validation)\n";
}
