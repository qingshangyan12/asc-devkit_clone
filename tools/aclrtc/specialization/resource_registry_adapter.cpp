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
#include "resource_registry.h"

#include <mutex>

namespace ascendc {
namespace aclrtc {
namespace {
using specialization_compile::ResourceData;
using specialization_compile::ResourceRegistry;
using specialization_compile::ResourceStatus;

const char* GetResourceStatusName(ResourceStatus resourceStatus) noexcept
{
    switch (resourceStatus) {
        case ResourceStatus::Success:
            return "success";
        case ResourceStatus::NotFound:
            return "not_found";
        case ResourceStatus::Conflict:
            return "conflict";
        case ResourceStatus::InvalidResource:
            return "invalid_resource";
        case ResourceStatus::IoError:
            return "io_error";
        case ResourceStatus::LoadError:
            return "load_error";
        case ResourceStatus::InternalError:
            return "internal_error";
        default:
            return "unknown";
    }
}

aclError LoadAndCopyFromProcessResourceRegistry(
    const std::string& resourceId, RegisteredJitResource& registeredResource)
{
    ResourceRegistry& resourceRegistry = ResourceRegistry::Instance();
    const ResourceStatus loadStatus = resourceRegistry.Load(nullptr);
    if (loadStatus != ResourceStatus::Success) {
        ACLRTC_LOG_ERROR(
            "Failed to load JIT resources before specializing resourceId '%s': status=%s(%d); check the preceding "
            "ResourceRegistry logs and configured OPP paths",
            resourceId.c_str(), GetResourceStatusName(loadStatus), static_cast<int32_t>(loadStatus));
        return ACLRTC_ERROR_FAILURE;
    }

    // ResourceRegistry returns shared entry data and refreshes resourceDir on every lookup. Copy all fields before
    // another specialization request can refresh that entry.
    static std::mutex lookupAndCopyMutex;
    std::lock_guard<std::mutex> lookupAndCopyLock(lookupAndCopyMutex);
    const ResourceData* resourceData = nullptr;
    const ResourceStatus lookupStatus = resourceRegistry.Lookup(resourceId.c_str(), resourceData);
    if (lookupStatus != ResourceStatus::Success) {
        ACLRTC_LOG_ERROR(
            "Failed to find or materialize JIT resource '%s': status=%s(%d); check resourceId and the preceding "
            "ResourceRegistry logs",
            resourceId.c_str(), GetResourceStatusName(lookupStatus), static_cast<int32_t>(lookupStatus));
        return ACLRTC_ERROR_FAILURE;
    }
    if (resourceData == nullptr) {
        ACLRTC_LOG_ERROR(
            "ResourceRegistry returned success with null resource data for resourceId '%s'; fix the Registry "
            "implementation",
            resourceId.c_str());
        return ACLRTC_ERROR_FAILURE;
    }

    RegisteredJitResource loadedResource;
    loadedResource.borrowedManifest = &resourceData->json;
    loadedResource.ownedWorktreePath = resourceData->resourceDir;
    loadedResource.externalSourceDirectoryPath = resourceData->sourceFilePath;
    registeredResource = std::move(loadedResource);
    return ACLRTC_SUCCESS;
}
} // namespace

JitResourceRegistryAdapter::JitResourceRegistryAdapter(
    LoadAndLookupFunction loadAndLookupFunction, void* callbackContext) noexcept
    : loadAndLookupFunction_(loadAndLookupFunction), callbackContext_(callbackContext)
{}

aclError JitResourceRegistryAdapter::LoadAndLookupJitResource(
    const std::string& resourceId, RegisteredJitResource& registeredResource) const
{
    if (loadAndLookupFunction_ == nullptr) {
        return LoadAndCopyFromProcessResourceRegistry(resourceId, registeredResource);
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
