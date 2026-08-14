/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "resource_registry_adapter.h"

#include "kernel_specialization_log.h"

namespace ascendc {
namespace aclrtc {

JitResourceRegistryAdapter::JitResourceRegistryAdapter(
    LoadAndLookupFunction loadAndLookupFunction, void* functionContext) noexcept
    : loadAndLookupFunction_(loadAndLookupFunction), functionContext_(functionContext)
{}

aclError JitResourceRegistryAdapter::LoadAndLookupJitResource(
    const std::string& resourceId, RegisteredJitResource& resource) const
{
    if (loadAndLookupFunction_ == nullptr) {
        ACLRTC_LOG_ERROR(
            "ResourceRegistry integration is unavailable; connect Load(nullptr) and Lookup(resourceId) in "
            "resource_registry_adapter.cpp");
        return ACLRTC_ERROR_FAILURE;
    }
    RegisteredJitResource loadedResource;
    const aclError result = loadAndLookupFunction_(resourceId, loadedResource, functionContext_);
    if (result != ACLRTC_SUCCESS) {
        ACLRTC_LOG_ERROR("ResourceRegistry Load/Lookup failed for resourceId %s with aclError %d",
                         resourceId.c_str(), result);
        return result;
    }
    resource = std::move(loadedResource);
    return ACLRTC_SUCCESS;
}

} // namespace aclrtc
} // namespace ascendc
