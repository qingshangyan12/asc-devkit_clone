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
    LoadAndLookupFunction loadAndLookupFunction, void* callbackContext) noexcept
    : loadAndLookupFunction_(loadAndLookupFunction), callbackContext_(callbackContext)
{}

aclError JitResourceRegistryAdapter::LoadAndLookupJitResource(
    const std::string& resourceId, RegisteredJitResource& registeredResource) const
{
    if (loadAndLookupFunction_ == nullptr) {
        ACLRTC_LOG_ERROR(
            "JIT ResourceRegistry integration is unavailable in this build; install or build ACLRTC with JIT "
            "resource registry support");
        return ACLRTC_ERROR_FAILURE;
    }
    RegisteredJitResource loadedResource;
    const aclError result = loadAndLookupFunction_(resourceId, loadedResource, callbackContext_);
    if (result != ACLRTC_SUCCESS) {
        ACLRTC_LOG_ERROR(
            "JIT ResourceRegistry Load/Lookup failed for resourceId '%s' with aclError %d; check resourceId and "
            "the preceding ResourceRegistry logs",
            resourceId.c_str(), result);
        return result;
    }
    registeredResource = std::move(loadedResource);
    return ACLRTC_SUCCESS;
}

} // namespace aclrtc
} // namespace ascendc
