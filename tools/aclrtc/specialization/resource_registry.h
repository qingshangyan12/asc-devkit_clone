/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef JIT_DEMO_COMMON_COMPILE_RESOURCE_REGISTRY_H
#define JIT_DEMO_COMMON_COMPILE_RESOURCE_REGISTRY_H

#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "specialization_compile_types.h"

struct AcCompileResourceManifest;

namespace ascendc {
namespace resource {

enum class ResourceStatus { Success, NotFound, InvalidResource, IoError, LoadError, NoMemory, InternalError };

class ResourceRegistry final {
public:
    ResourceRegistry();
    ~ResourceRegistry();
    ResourceRegistry(const ResourceRegistry&) = delete;
    ResourceRegistry& operator=(const ResourceRegistry&) = delete;

    ResourceStatus Load(const std::string& directory) noexcept;
    ResourceStatus Lookup(
        const std::string& resourceId, const std::string& kernelName, const ResourceRecord*& record) noexcept;

private:
    ResourceStatus Materialize(const std::string& resourceId, ResourceRecordPack& pack);
    ResourceStatus LoadLibrary(const std::string& libraryPath);
    ResourceStatus LoadManifest(
        const ::AcCompileResourceManifest& unit, const std::string& libraryPath, uint64_t manifestIndex);

    std::string temporaryRoot_;
    std::vector<void*> libraries_;
    std::unordered_map<std::string, ResourceRecordPack> recordPacks_;
    std::mutex lookupMutex_;
};

} // namespace resource
} // namespace ascendc

#endif // JIT_DEMO_COMMON_COMPILE_RESOURCE_REGISTRY_H
