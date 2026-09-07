/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_ACLRTC_KERNEL_MANIFEST_PARSER_H
#define ASCENDC_ACLRTC_KERNEL_MANIFEST_PARSER_H

#include "kernel_specialization_types.h"
#include "manifest_constant.h"

#include <nlohmann/json_fwd.hpp>

namespace ascendc {
namespace aclrtc {

enum class ManifestObjectKind { Basic, SuperKernel };

struct ManifestTextSpec {
    std::string text;
    std::string location;
};

struct CommandSpec {
    CompilationCommandKind commandKind{CompilationCommandKind::Compile};
    ManifestObjectKind objectKind{ManifestObjectKind::Basic};
    std::optional<uint32_t> parallelStage;
    std::string diagnosticLabel;
    ManifestTextSpec executable;
    std::vector<ManifestTextSpec> arguments;
};

// Owns the selected manifest fields; no JSON nodes or request buffers are borrowed.
struct KernelManifestSpec {
    std::vector<ManifestConstant> constants;
    std::vector<CommandSpec> commands;
    std::vector<ManifestTextSpec> linkOptions;
    std::vector<ManifestTextSpec> linkInputs;
};

aclError ParseSelectedKernelManifest(
    const nlohmann::json& manifest, const std::string& kernelName, KernelCompilationVariant variant,
    KernelManifestSpec& parsedManifest);

} // namespace aclrtc
} // namespace ascendc

#endif // ASCENDC_ACLRTC_KERNEL_MANIFEST_PARSER_H
