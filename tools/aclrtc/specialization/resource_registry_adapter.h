/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_ACLRTC_RESOURCE_REGISTRY_ADAPTER_H
#define ASCENDC_ACLRTC_RESOURCE_REGISTRY_ADAPTER_H

#include "kernel_specialization_types.h"

#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace ascendc {
namespace aclrtc {

struct RegisteredJitResource {
    const nlohmann::json* borrowedManifest {nullptr};
    boost::filesystem::path ownedWorktreePath;
    boost::filesystem::path externalSourceDirectoryPath;
};

class JitResourceRegistryAdapter final {
public:
    using LoadAndLookupFunction = aclError (*)(const std::string& resourceId,
                                               RegisteredJitResource& registeredResource,
                                               void* callbackContext);

    JitResourceRegistryAdapter() = default;
    JitResourceRegistryAdapter(LoadAndLookupFunction loadAndLookupFunction, void* callbackContext) noexcept;

    aclError LoadAndLookupJitResource(
        const std::string& resourceId, RegisteredJitResource& registeredResource) const;

private:
    LoadAndLookupFunction loadAndLookupFunction_ {nullptr};
    void* callbackContext_ {nullptr};
};

} // namespace aclrtc
} // namespace ascendc

#endif // ASCENDC_ACLRTC_RESOURCE_REGISTRY_ADAPTER_H
