/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_BUNDLE_SOURCE_GENERATOR_H
#define ASCENDC_BUNDLE_SOURCE_GENERATOR_H

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

#include "manifest_bundle_types.h"

namespace ascendc {
namespace manifest_generator {

class BundleSourceGenerator final {
public:
    BundleSourceGenerator(std::string workDir, const std::vector<ManifestUnit>& units) noexcept;

    void Generate() const;

private:
    static void EmitAbiHeader(const std::string& path);
    static void EmitByteArray(
        std::ostream& output, const std::string& name, const uint8_t* data, size_t size, bool nulTerminate);
    static void EmitString(std::ostream& output, const std::string& name, const std::string& value);
    static void EmitManifestSource(const std::string& path, size_t unitIndex, const ManifestUnit& unit);
    void EmitBundleSource(const std::string& path) const;
    void EmitMakefile(const std::string& path) const;

    std::string workDir_;
    const std::vector<ManifestUnit>& units_;
};

} // namespace manifest_generator
} // namespace ascendc

#endif // ASCENDC_BUNDLE_SOURCE_GENERATOR_H
