/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_timestamp.h"
#include "load_kernel.h"
#include "log.h"
#include "hccl_host_comm_dl.h"
#include "hcomm_dlsym.h"
#include <acl/acl_rt.h>
#include <map>
#include <mutex>

namespace AscendC {
namespace ccu {
namespace dfx {
namespace {
struct ClockState {
    aclrtStream stream = nullptr;
    uint64_t* slot = nullptr;
    bool retired = false;
};
struct Clocks {
    std::mutex mutex;
    std::map<aclrtContext, ClockState> states;
    bool callbackRegistered = false;
};
Clocks& GetClocks()
{
    // Addresses are embedded in registered JIT kernels. Until hcomm provides
    // a kernel-unregister lifetime hook, retain slots until device reset.
    static auto* clocks = new Clocks;
    return *clocks;
}
// Test scope: one communicator per process. Stop the publisher after
// communicator teardown; never synchronize its infinite task.
HcclResult ClockCommStateCallback(const HcclComm, int32_t state, void*)
{
    if (state == 1) { // DESTROY_POST: CCU work has been retired by hcomm.
        StopAicpuTimestamps();
    }
    return HCCL_SUCCESS;
}
bool Check(aclError ret, const char* operation)
{
    if (ret == ACL_SUCCESS) {
        return true;
    }
    HCCL_RUN_WARNING("[CCU_CLOCK] %s failed, ret[%d]", operation, ret);
    return false;
}
} // namespace

uint64_t StartAicpuTimestamp()
{
    auto& clocks = GetClocks();
    std::lock_guard<std::mutex> lock(clocks.mutex);
    if (!clocks.callbackRegistered) {
        ::HcommDlInit();
        if (::HcclMc2IsSupportCommStateCallback()) {
            clocks.callbackRegistered =
                ::HcclMc2CommRegStateCallback("ccu_timestamp", ClockCommStateCallback, nullptr) == HCCL_SUCCESS;
        }
        if (!clocks.callbackRegistered) {
            HCCL_RUN_WARNING("[CCU_CLOCK] no teardown callback; call CcuPrintStream::Stop before device reset");
        }
    }
    aclrtContext context = nullptr;
    if (!Check(aclrtGetCurrentContext(&context), "get context")) {
        return 0;
    }
    auto& state = clocks.states[context];
    if (state.retired) {
        HCCL_RUN_WARNING("[CCU_CLOCK] clock retired; create a fresh device context before another test");
        return 0;
    }
    if (state.stream != nullptr) {
        return reinterpret_cast<uint64_t>(state.slot);
    }
    if (mc2_ops_hccl::LoadAICPUKernel() != HCCL_SUCCESS) {
        HCCL_RUN_WARNING("[CCU_CLOCK] load AICPU binary failed");
        return 0;
    }
    aclrtFuncHandle function = nullptr;
    if (!Check(
            aclrtBinaryGetFunction(mc2_ops_hccl::g_binKernelHandle, "CcuTimestampKernel", &function), "get function")) {
        return 0;
    }
    // aclrtMallocCached would request MEM_CACHEABLE_TYPE from the driver.
    // Ordinary aclrtMalloc uses the uncached device mapping for AICPU.
    if (state.slot == nullptr && !Check(
                                     aclrtMalloc(reinterpret_cast<void**>(&state.slot), 64, ACL_MEM_MALLOC_NORMAL_ONLY),
                                     "allocate uncached clock slot")) {
        return 0;
    }
    if (!Check(aclrtMemset(state.slot, 64, 0, 64), "initialize clock")) {
        return 0;
    }
    aclrtArgsHandle args = nullptr;
    aclrtParamHandle param = nullptr;
    uint64_t address = reinterpret_cast<uint64_t>(state.slot);
    if (!Check(aclrtKernelArgsInit(function, &args), "args init") ||
        !Check(aclrtKernelArgsAppend(args, &address, sizeof(address), &param), "args append") ||
        !Check(aclrtKernelArgsFinalize(args), "args finalize")) {
        return 0;
    }
    aclrtStream stream = nullptr;
    if (!Check(aclrtCreateStreamWithConfig(&stream, 0, ACL_STREAM_FAST_SYNC), "create clock stream")) {
        return 0;
    }
    // Dedicated stream: never synchronize this infinite kernel, or put CCU
    // work on its stream. This test publisher has an explicit one-hour limit.
    aclrtLaunchKernelAttr attr{};
    attr.id = ACL_RT_LAUNCH_KERNEL_ATTR_TIMEOUT;
    attr.value.timeout = 3600;
    aclrtLaunchKernelCfg config{};
    config.numAttrs = 1;
    config.attrs = &attr;
    if (!Check(aclrtLaunchKernelWithConfig(function, 1, stream, &config, args, nullptr), "launch clock kernel")) {
        (void)aclrtDestroyStream(stream);
        return 0;
    }
    state.stream = stream;
    HCCL_RUN_INFO(
        "[CCU_CLOCK] launched context[%p] stream[%p] slot[%p] unit[ns] timeout[3600s]", context, stream, state.slot);
    return address;
}

void StopAicpuTimestamps()
{
    auto& clocks = GetClocks();
    std::lock_guard<std::mutex> lock(clocks.mutex);
    aclrtContext saved = nullptr;
    if (!Check(aclrtGetCurrentContext(&saved), "get teardown context")) {
        return;
    }
    for (auto& entry : clocks.states) {
        auto& state = entry.second;
        if (state.stream == nullptr || !Check(aclrtSetCurrentContext(entry.first), "set teardown context")) {
            continue;
        }
        // Abort the dedicated stream, not all device tasks or the shared
        // AICPU process. Do NOT free the slot: JIT code still references it,
        // and stream-abort completion alone is not proof of worker exit on
        // every firmware version. Device reset ultimately reclaims memory.
        state.retired = true;
        HCCL_RUN_INFO("[CCU_CLOCK] abort begin stream[%p] slot[%p]", state.stream, state.slot);
        if (Check(aclrtStreamAbort(state.stream), "abort clock stream")) {
            if (Check(aclrtDestroyStream(state.stream), "destroy aborted clock stream")) {
                state.stream = nullptr;
            }
        }
        HCCL_RUN_INFO("[CCU_CLOCK] abort end; retain slot[%p] until device reset", state.slot);
    }
    (void)Check(aclrtSetCurrentContext(saved), "restore context");
}
} // namespace dfx
} // namespace ccu
} // namespace AscendC
