/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "hcomm_desc_cache_dl.h"

#include <dlfcn.h>
#include "log.h"

namespace {
using BeginFn = HcclResult (*)(uint32_t);
using BeginRoundFn = HcclResult (*)(uint32_t);
using NoArgFn = HcclResult (*)(void);

struct AicpuDescCacheDispatch {
    BeginFn begin{nullptr};
    BeginRoundFn beginRound{nullptr};
    NoArgFn endRound{nullptr};
    NoArgFn end{nullptr};
    NoArgFn abort{nullptr};

    bool IsReady() const
    {
        return begin != nullptr && beginRound != nullptr && endRound != nullptr && end != nullptr && abort != nullptr;
    }

    void Clear()
    {
        begin = nullptr;
        beginRound = nullptr;
        endRound = nullptr;
        end = nullptr;
        abort = nullptr;
    }
};

AicpuDescCacheDispatch g_dispatch;

template <typename Function>
Function Resolve(void* handle, const char* symbol)
{
    dlerror();
    void* address = dlsym(handle, symbol);
    const char* error = dlerror();
    if (address == nullptr || error != nullptr) {
        HCCL_DEBUG("[HcclWrapper] %s not supported, %s", symbol, error == nullptr ? "(no error)" : error);
        return nullptr;
    }
    return reinterpret_cast<Function>(address);
}

HcclResult Unsupported(const char* function)
{
    HCCL_DEBUG("[HcclWrapper] %s not supported", function);
    return HCCL_E_NOT_SUPPORT;
}
} // namespace

extern "C" {

void HcommAicpuDescCacheDlInit(void* libHcommHandle)
{
    g_dispatch.begin = Resolve<BeginFn>(libHcommHandle, "HcommAicpuDescCacheBegin");
    g_dispatch.beginRound = Resolve<BeginRoundFn>(libHcommHandle, "HcommAicpuDescCacheBeginRound");
    g_dispatch.endRound = Resolve<NoArgFn>(libHcommHandle, "HcommAicpuDescCacheEndRound");
    g_dispatch.end = Resolve<NoArgFn>(libHcommHandle, "HcommAicpuDescCacheEnd");
    g_dispatch.abort = Resolve<NoArgFn>(libHcommHandle, "HcommAicpuDescCacheAbort");
    if (!g_dispatch.IsReady()) {
        g_dispatch.Clear();
    }
}

bool HcommAicpuDescCacheDlSupported(void) { return g_dispatch.IsReady(); }

HcclResult HcommAicpuDescCacheDlBegin(uint32_t repeatCount)
{
    return g_dispatch.begin == nullptr ? Unsupported(__func__) : g_dispatch.begin(repeatCount);
}

HcclResult HcommAicpuDescCacheDlBeginRound(uint32_t roundIndex)
{
    return g_dispatch.beginRound == nullptr ? Unsupported(__func__) : g_dispatch.beginRound(roundIndex);
}

HcclResult HcommAicpuDescCacheDlEndRound(void)
{
    return g_dispatch.endRound == nullptr ? Unsupported(__func__) : g_dispatch.endRound();
}

HcclResult HcommAicpuDescCacheDlEnd(void)
{
    return g_dispatch.end == nullptr ? Unsupported(__func__) : g_dispatch.end();
}

HcclResult HcommAicpuDescCacheDlAbort(void)
{
    return g_dispatch.abort == nullptr ? Unsupported(__func__) : g_dispatch.abort();
}

} // extern "C"
