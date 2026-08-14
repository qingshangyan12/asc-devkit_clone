/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_ACLRTC_KERNEL_SPECIALIZATION_DIAGNOSTICS_H
#define ASCENDC_ACLRTC_KERNEL_SPECIALIZATION_DIAGNOSTICS_H

#include "kernel_specialization_types.h"

#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace ascendc {
namespace aclrtc {

class KernelSpecializationDiagnostics final {
public:
    KernelSpecializationDiagnostics(std::string requestId, bool saveKernelMetaEnabled);

    void SaveManifestForDiagnostics(const nlohmann::json& resourceManifest) noexcept;
    void RecordResourceWorktreePath(const boost::filesystem::path& resourceWorktreePath) noexcept;
    void RecordCompilationCommand(const CompilationCommand& compilationCommand) noexcept;
    void RecordCompilationProcessResult(const CompilationCommand& compilationCommand,
                                        const CompilationProcessResult& processResult) noexcept;
    void RecordSpecializationResult(const boost::filesystem::path& outputElfPath,
                                    bool outputElfPublished) noexcept;

private:
    void AppendDiagnosticText(const boost::filesystem::path& filePath, const std::string& text) noexcept;

    std::string requestId_;
    bool saveKernelMetaEnabled_ {false};
    boost::filesystem::path diagnosticDirectoryPath_;
};

} // namespace aclrtc
} // namespace ascendc

#endif // ASCENDC_ACLRTC_KERNEL_SPECIALIZATION_DIAGNOSTICS_H
