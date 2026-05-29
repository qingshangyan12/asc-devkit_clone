/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "dl_hccl_lib.h"
#include "log.h"

namespace mc2_ops_hccl {

namespace {
#if defined(AICPU_COMPILE) || defined(MC2_SERVER_ONLY)
constexpr const char *kHcclLibName = "libscatter_aicpu_kernel.so";
#else
constexpr const char *kHcclLibName = "libhccl.so";
#endif
}  // namespace

DlHcclLib &DlHcclLib::GetInstance()
{
    static DlHcclLib instance;
    return instance;
}

DlHcclLib::~DlHcclLib()
{
    void *h = nullptr;
    {
        std::lock_guard<std::mutex> lock(mu_);
        h = handle_;
        handle_ = nullptr;
    }
    if (h != nullptr) {
        (void)dlclose(h);
    }
}

void *DlHcclLib::GetHandle()
{
    std::lock_guard<std::mutex> lock(mu_);
    if (handle_ != nullptr) {
        return handle_;
    }
    if (tried_) {
        return nullptr;
    }
    tried_ = true;

    void *h = dlopen(kHcclLibName, RTLD_NOW | RTLD_GLOBAL);
    if (h == nullptr) {
        HCCL_WARNING("[DlHcclLib] dlopen %s failed: %s", kHcclLibName, dlerror());
        return nullptr;
    }
    handle_ = h;
    HCCL_INFO("[DlHcclLib] dlopen %s success", kHcclLibName);
    return handle_;
}

void *DlHcclLib::Resolve(const char *mangled)
{
    void *h = GetHandle();
    if (h == nullptr) {
        return nullptr;
    }
    void *sym = dlsym(h, mangled);
    if (sym == nullptr) {
        HCCL_WARNING("[DlHcclLib] dlsym %s failed: %s", mangled, dlerror());
    }
    return sym;
}

}  // namespace mc2_ops_hccl
