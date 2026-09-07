/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_COMMON_MANIFEST_CONSTANT_H
#define ASCENDC_COMMON_MANIFEST_CONSTANT_H

#include <cstdint>
#include <string>
#include <nlohmann/json_fwd.hpp>

namespace ascendc {

struct ManifestConstant;

class ConstantSizeRule final {
public:
    ConstantSizeRule() = default;
    bool UsesArgumentSize() const { return fixedByteCount_ == 0U; }
    uint64_t ResolveByteCount(uint64_t argumentByteCount) const
    {
        return UsesArgumentSize() ? argumentByteCount : fixedByteCount_;
    }

private:
    friend bool ParseManifestConstant(const nlohmann::json& constant, ManifestConstant& parsedConstant);
    explicit ConstantSizeRule(uint64_t fixedByteCount) : fixedByteCount_(fixedByteCount) {}
    // Zero denotes argument-derived size; only the parser can set a positive fixed size.
    uint64_t fixedByteCount_{0U};
};

struct ManifestConstant {
    std::string name;
    uint64_t parameterIndex{0U};
    ConstantSizeRule sizeRule;
    std::string targetFile;
    std::string templateText;
};

// Parses field contracts only; resource existence and argument binding belong to callers.
bool ParseManifestConstant(const nlohmann::json& constant, ManifestConstant& parsedConstant);

} // namespace ascendc

#endif // ASCENDC_COMMON_MANIFEST_CONSTANT_H
