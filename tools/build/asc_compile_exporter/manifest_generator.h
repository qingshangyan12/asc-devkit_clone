/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_MANIFEST_GENERATOR_H
#define ASCENDC_MANIFEST_GENERATOR_H

#include <cstdint>
#include <string>

namespace ascendc {
namespace manifest_generator {

struct BuildCollectedBundleRequest {
    std::string packageId;
    std::string manifestSearchRoot;
    std::string outputSo;
    std::string workDir;
    bool keepTemp = false;
    uint32_t jobs = 0U;
};

class ManifestBundleCompiler final {
public:
    explicit ManifestBundleCompiler(BuildCollectedBundleRequest request) noexcept;

    void Compile() const;

private:
    BuildCollectedBundleRequest request_;
};

} // namespace manifest_generator
} // namespace ascendc

#endif // ASCENDC_MANIFEST_GENERATOR_H
