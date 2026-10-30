/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "kernel_compilation_plan_builder.h"

#include "ascendc_tool_log.h"

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <cstdlib>
#include <iomanip>
#include <set>
#include <sstream>
#include <unistd.h>

namespace ascendc {
namespace aclrtc {
namespace {
namespace fs = boost::filesystem;
constexpr char ENVIRONMENT_REFERENCE_PREFIX[] = "${env:";
constexpr char SPECIALIZATION_FLAG_SOURCE_FILE_NAME[] = "specialization_flag.cpp";
constexpr char SPECIALIZATION_FLAG_OBJECT_FILE_NAME[] = "specialization_flag.o";
constexpr char SPECIALIZATION_FLAG_SOURCE[] =
    "#include \"basic_api/kernel_tensor.h\"\n"
    "__attribute__((used, section(\".ascend.meta\")))\n"
    "static const BinaryMetaSpecializationFlag g_ascend_specialization_flag = "
    "{{B_TYPE_SPECIALIZATION_FLAG, sizeof(uint32_t)}, 0};\n";

void ReplaceAllLiteralOccurrences(std::string& text, const std::string& literalText, const std::string& replacementText)
{
    size_t occurrencePosition = 0U;
    while ((occurrencePosition = text.find(literalText, occurrencePosition)) != std::string::npos) {
        text.replace(occurrencePosition, literalText.size(), replacementText);
        occurrencePosition += replacementText.size();
    }
}

bool ExpandEnvironmentVariablePlaceholders(
    const std::string& text, const std::string& manifestValueLocation, std::string& resolvedText)
{
    resolvedText.clear();
    size_t sourcePosition = 0U;
    while (sourcePosition < text.size()) {
        const size_t referencePosition = text.find(ENVIRONMENT_REFERENCE_PREFIX, sourcePosition);
        if (referencePosition == std::string::npos) {
            resolvedText.append(text, sourcePosition, std::string::npos);
            break;
        }
        resolvedText.append(text, sourcePosition, referencePosition - sourcePosition);
        const size_t namePosition = referencePosition + sizeof(ENVIRONMENT_REFERENCE_PREFIX) - 1U;
        const size_t closingBracePosition = text.find('}', namePosition);
        if (closingBracePosition == std::string::npos) {
            ASCENDLOGE(
                "Unterminated environment variable placeholder at %s byte %zu; regenerate the JIT resource "
                "using the form ${env:NAME}",
                manifestValueLocation.c_str(), referencePosition);
            return false;
        }
        if (closingBracePosition == namePosition) {
            ASCENDLOGE(
                "Environment variable placeholder at %s byte %zu has an empty name; regenerate the JIT resource "
                "using the form ${env:NAME}",
                manifestValueLocation.c_str(), referencePosition);
            return false;
        }
        const std::string environmentName = text.substr(namePosition, closingBracePosition - namePosition);
        const char* environmentValue = std::getenv(environmentName.c_str());
        if (environmentValue == nullptr) {
            ASCENDLOGE(
                "Environment variable %s referenced at %s byte %zu is not set; "
                "set it with: export %s=<value>",
                environmentName.c_str(), manifestValueLocation.c_str(), referencePosition, environmentName.c_str());
            return false;
        }
        resolvedText.append(environmentValue);
        sourcePosition = closingBracePosition + 1U;
    }
    return true;
}

bool IsAbsoluteExecutableRegularFile(const fs::path& executablePath)
{
    boost::system::error_code fileError;
    return executablePath.is_absolute() && fs::is_regular_file(executablePath, fileError) && !fileError &&
           access(executablePath.c_str(), X_OK) == 0;
}

bool IsMissingPathError(const boost::system::error_code& pathError) noexcept
{
    return pathError.value() == ENOENT || pathError.value() == ENOTDIR;
}

bool FindSeparatedCompilerOutputOptionPosition(const std::vector<std::string>& arguments, size_t& outputOptionPosition)
{
    for (size_t argumentIndex = 0; argumentIndex < arguments.size(); ++argumentIndex) {
        const std::string& argument = arguments[argumentIndex];
        if (argument != "-o") {
            continue;
        }
        if (argumentIndex + 1U < arguments.size() && !arguments[argumentIndex + 1U].empty()) {
            outputOptionPosition = argumentIndex;
            return true;
        }
    }
    return false;
}

std::string FormatBytesAsCppInitializerList(const void* constantAddress, uint64_t constantByteCount)
{
    constexpr int32_t hexadecimalByteWidth = 2;
    const auto* constantBytes = static_cast<const uint8_t*>(constantAddress);
    std::ostringstream encodedBytes;
    encodedBytes << '{';
    for (uint64_t byteIndex = 0; byteIndex < constantByteCount; ++byteIndex) {
        if (byteIndex != 0U) {
            encodedBytes << ", ";
        }
        encodedBytes << "0x" << std::hex << std::nouppercase << std::setw(hexadecimalByteWidth) << std::setfill('0')
                     << static_cast<unsigned int>(constantBytes[byteIndex]);
    }
    encodedBytes << '}';
    return encodedBytes.str();
}

bool AppendCompilerTargetOptions(const std::vector<std::string>& arguments, std::vector<std::string>& targetArguments)
{
    bool foundTarget = false;
    for (const std::string& argument : arguments) {
        if (argument.compare(0, sizeof("--cce-aicore-arch=") - 1U, "--cce-aicore-arch=") == 0 ||
            argument.compare(0, sizeof("--npu-arch=") - 1U, "--npu-arch=") == 0) {
            if (argument.back() == '=') {
                return false;
            }
            targetArguments.emplace_back(argument);
            foundTarget = true;
        }
    }
    return foundTarget;
}

} // namespace

bool KernelCompilationPlanBuilder::ExpandPathAndEnvironmentReferences(
    const std::string& text, const std::string& location, std::string& resolvedText) const
{
    std::string pathResolvedText = text;
    ReplaceAllLiteralOccurrences(pathResolvedText, "${resource}", resourceWorktreePath_.string());
    ReplaceAllLiteralOccurrences(pathResolvedText, "${output}", outputDirectoryPath_.string());
    ReplaceAllLiteralOccurrences(pathResolvedText, "${source_file_path}", externalSourceDirectoryPath_.string());
    return ExpandEnvironmentVariablePlaceholders(pathResolvedText, location, resolvedText);
}

bool KernelCompilationPlanBuilder::CheckSpecializationMetadataPath(
    const std::string& path, const std::string& location) const
{
    const fs::path normalizedPath = fs::absolute(fs::path(path)).lexically_normal();
    if (normalizedPath == (outputDirectoryPath_ / SPECIALIZATION_FLAG_SOURCE_FILE_NAME).lexically_normal() ||
        normalizedPath == (outputDirectoryPath_ / SPECIALIZATION_FLAG_OBJECT_FILE_NAME).lexically_normal()) {
        ASCENDLOGE("Manifest %s uses a reserved specialization metadata path: %s", location.c_str(), path.c_str());
        return false;
    }
    return true;
}

aclError KernelCompilationPlanBuilder::BuildConstantSourcePatches(KernelCompilationPlan& plan) const
{
    auto constants = borrowedManifest_.constants;
    std::vector<KernelSourcePatch> patches;
    for (ManifestConstant& constant : constants) {
        if (!constant.BindArgument(
                specializationRequest_.kernelArgumentCount, specializationRequest_.borrowedKernelArgumentDataPointers,
                specializationRequest_.borrowedKernelArgumentByteCounts)) {
            return ACLRTC_ERROR_INVALID_INPUT;
        }
        std::string targetFile;
        if (!ExpandPathAndEnvironmentReferences(constant.GetTargetFile(), constant.GetName(), targetFile)) {
            return ACLRTC_ERROR_FAILURE;
        }
        if (!CheckSpecializationMetadataPath(targetFile, constant.GetName())) {
            return ACLRTC_ERROR_FAILURE;
        }
        patches.push_back({targetFile, constant.GetTemplateText(), {}});
    }
    // Avoid reading caller-owned buffers if any constant binding or target expansion fails.
    for (size_t index = 0; index < constants.size(); ++index) {
        patches[index].replacementText =
            FormatBytesAsCppInitializerList(constants[index].GetBoundData(), constants[index].GetBoundByteSize());
    }
    plan.sourcePatches = std::move(patches);
    return ACLRTC_SUCCESS;
}

aclError KernelCompilationPlanBuilder::BindManifestCommand(
    const ManifestCommand& spec, CompilationCommand& compilationCommand) const
{
    compilationCommand.commandKind = spec.commandKind;
    compilationCommand.parallelStage = spec.parallelStage;
    compilationCommand.diagnosticLabel = spec.diagnosticLabel;
    std::string executable;
    if (!ExpandPathAndEnvironmentReferences(spec.executable, spec.diagnosticLabel, executable)) {
        return ACLRTC_ERROR_FAILURE;
    }
    compilationCommand.executablePath = std::move(executable);
    if (!IsAbsoluteExecutableRegularFile(compilationCommand.executablePath)) {
        ASCENDLOGE(
            "Manifest command %s requires an absolute path to an existing executable regular file: path=%s; "
            "check the referenced environment variable and tool installation",
            compilationCommand.diagnosticLabel.c_str(), compilationCommand.executablePath.c_str());
        return ACLRTC_ERROR_FAILURE;
    }

    for (const std::string& argument : spec.arguments) {
        std::string resolvedArgument;
        if (!ExpandPathAndEnvironmentReferences(argument, spec.diagnosticLabel, resolvedArgument)) {
            return ACLRTC_ERROR_FAILURE;
        }
        if (!CheckSpecializationMetadataPath(resolvedArgument, spec.diagnosticLabel)) {
            return ACLRTC_ERROR_FAILURE;
        }
        compilationCommand.arguments.emplace_back(std::move(resolvedArgument));
    }

    if (compilationCommand.commandKind == CompilationCommandKind::Compile) {
        size_t outputOptionPosition = 0U;
        if (!FindSeparatedCompilerOutputOptionPosition(compilationCommand.arguments, outputOptionPosition)) {
            ASCENDLOGE(
                "Manifest compile command %s must contain a valid separated '-o' output option and a non-empty "
                "output path; regenerate the JIT resource with '-o' followed by its output path",
                compilationCommand.diagnosticLabel.c_str());
            return ACLRTC_ERROR_FAILURE;
        }
        const auto& options = spec.objectKind == ManifestObjectKind::Basic ?
                                  specializationRequest_.compilerOptions.basicOptions :
                                  specializationRequest_.compilerOptions.superKernelOptions;
        compilationCommand.arguments.insert(
            compilationCommand.arguments.begin() + static_cast<std::ptrdiff_t>(outputOptionPosition), options.begin(),
            options.end());
    }
    return ACLRTC_SUCCESS;
}

aclError KernelCompilationPlanBuilder::BuildSpecializationMetadata(
    const CompilationCommand& basicCompileCommand, KernelCompilationPlan& plan) const
{
    std::string ascendHome;
    if (!ExpandPathAndEnvironmentReferences("${env:ASCEND_HOME_PATH}", "specialization flag compiler", ascendHome)) {
        return ACLRTC_ERROR_FAILURE;
    }
    CompilationCommand command;
    command.commandKind = CompilationCommandKind::Compile;
    command.diagnosticLabel = "specialization_flag/compile";
    command.executablePath = fs::path(ascendHome) / "bin/bisheng";
    if (!IsAbsoluteExecutableRegularFile(command.executablePath)) {
        ASCENDLOGE(
            "Specialization flag compiler is not an executable regular file: %s", command.executablePath.c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    const fs::path sourcePath = outputDirectoryPath_ / SPECIALIZATION_FLAG_SOURCE_FILE_NAME;
    const fs::path objectPath = outputDirectoryPath_ / SPECIALIZATION_FLAG_OBJECT_FILE_NAME;
    const fs::path ascPath = fs::path(ascendHome) / "asc";
    command.arguments = {
        "-c", "-O3", "-xcce", sourcePath.string(), "-I" + ascPath.string(), "-I" + (ascPath / "include").string()};
    // SuperKernel auxiliary commands can precede basic commands and use a different target.
    if (!AppendCompilerTargetOptions(basicCompileCommand.arguments, command.arguments)) {
        ASCENDLOGE(
            "Basic compile command %s requires a nonempty explicit --cce-aicore-arch=<arch> or --npu-arch=<arch> "
            "option "
            "for specialization metadata",
            basicCompileCommand.diagnosticLabel.c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    // This metadata-only object must not define the kernel's g_tilingKey variable.
    command.arguments.insert(
        command.arguments.end(), {"--cce-aicore-only", "-std=c++17", "-DTILING_KEY_VAR=0", "-o", objectPath.string()});
    plan.generatedSources.push_back({sourcePath, SPECIALIZATION_FLAG_SOURCE});
    plan.compilationCommands.emplace_back(std::move(command));
    return ACLRTC_SUCCESS;
}

aclError KernelCompilationPlanBuilder::BindLinkCommand(
    const CompilationManifest& manifest, KernelCompilationPlan& plan) const
{
    CompilationCommand linkCommand;
    std::set<fs::path> requiredOutputDirectories{outputDirectoryPath_};
    linkCommand.commandKind = CompilationCommandKind::Link;
    linkCommand.diagnosticLabel = "link";
    std::string linkerPath;
    if (!ExpandPathAndEnvironmentReferences(
            "${env:ASCEND_HOME_PATH}/bin/ld.lld", "internal linker executable", linkerPath)) {
        return ACLRTC_ERROR_FAILURE;
    }
    linkCommand.executablePath = linkerPath;

    for (const std::string& option : manifest.linkOptions) {
        std::string resolvedOption;
        if (!ExpandPathAndEnvironmentReferences(option, "link_options", resolvedOption)) {
            return ACLRTC_ERROR_FAILURE;
        }
        linkCommand.arguments.emplace_back(std::move(resolvedOption));
    }
    for (const std::string& input : manifest.linkInputs) {
        std::string resolvedInput;
        if (!ExpandPathAndEnvironmentReferences(input, "link_inputs", resolvedInput)) {
            return ACLRTC_ERROR_FAILURE;
        }
        if (!CheckSpecializationMetadataPath(resolvedInput, "link_inputs")) {
            return ACLRTC_ERROR_FAILURE;
        }
        requiredOutputDirectories.emplace(fs::path(resolvedInput).parent_path());
        linkCommand.arguments.emplace_back(std::move(resolvedInput));
    }
    linkCommand.arguments.emplace_back((outputDirectoryPath_ / SPECIALIZATION_FLAG_OBJECT_FILE_NAME).string());
    plan.linkedKernelElfPath = outputDirectoryPath_ / "linked_kernel.elf";
    linkCommand.arguments.emplace_back("-o");
    linkCommand.arguments.emplace_back(plan.linkedKernelElfPath.string());
    if (!IsAbsoluteExecutableRegularFile(linkCommand.executablePath)) {
        ASCENDLOGE(
            "Linker requires an absolute path to an existing executable regular file: path=%s; verify "
            "ASCEND_HOME_PATH points to a complete CANN installation",
            linkCommand.executablePath.c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    plan.compilationCommands.emplace_back(std::move(linkCommand));
    plan.requiredOutputDirectoryPaths.assign(requiredOutputDirectories.begin(), requiredOutputDirectories.end());
    return ACLRTC_SUCCESS;
}

aclError KernelCompilationPlanBuilder::CheckResourceWorktreeDirectory() const
{
    if (!resourceWorktreePath_.is_absolute()) {
        ASCENDLOGE(
            "ResourceRegistry returned a non-absolute worktree path: %s; fix the registry integration",
            resourceWorktreePath_.c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    boost::system::error_code worktreeError;
    const bool worktreeIsDirectory = fs::is_directory(resourceWorktreePath_, worktreeError);
    if (worktreeError && !IsMissingPathError(worktreeError)) {
        ASCENDLOGE(
            "Failed to inspect resource worktree: path=%s error=%d message=%s; fix the registry "
            "integration",
            resourceWorktreePath_.c_str(), worktreeError.value(), worktreeError.message().c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    if (!worktreeIsDirectory) {
        ASCENDLOGE(
            "Resource worktree must be an existing directory: path=%s; fix the registry integration",
            resourceWorktreePath_.c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    return ACLRTC_SUCCESS;
}

KernelCompilationPlanBuilder::KernelCompilationPlanBuilder(
    const NormalizedKernelSpecializationRequest& specializationRequest, const CompilationManifest& manifest,
    fs::path resourceWorktreePath, fs::path externalSourceDirectoryPath)
    : specializationRequest_(specializationRequest),
      borrowedManifest_(manifest),
      resourceWorktreePath_(std::move(resourceWorktreePath)),
      externalSourceDirectoryPath_(std::move(externalSourceDirectoryPath)),
      outputDirectoryPath_(
          resourceWorktreePath_ / (".aclrtc_" + specializationRequest_.specializationSessionId) / "outputs")
{}

aclError KernelCompilationPlanBuilder::BuildCompilationPlan(KernelCompilationPlan& compilationPlan) const
{
    aclError result = CheckResourceWorktreeDirectory();
    if (result != ACLRTC_SUCCESS) {
        return result;
    }
    KernelCompilationPlan plan;
    std::optional<size_t> basicCompileCommandIndex;
    for (const ManifestCommand& spec : borrowedManifest_.commands) {
        CompilationCommand command;
        result = BindManifestCommand(spec, command);
        if (result != ACLRTC_SUCCESS) {
            return result;
        }
        if (!basicCompileCommandIndex.has_value() && spec.objectKind == ManifestObjectKind::Basic &&
            spec.commandKind == CompilationCommandKind::Compile) {
            basicCompileCommandIndex = plan.compilationCommands.size();
        }
        plan.compilationCommands.emplace_back(std::move(command));
    }
    if (!basicCompileCommandIndex.has_value()) {
        ASCENDLOGE("Specialization metadata requires a basic compile command in the selected manifest");
        return ACLRTC_ERROR_FAILURE;
    }
    result = BuildSpecializationMetadata(plan.compilationCommands[*basicCompileCommandIndex], plan);
    if (result != ACLRTC_SUCCESS) {
        return result;
    }
    result = BindLinkCommand(borrowedManifest_, plan);
    if (result != ACLRTC_SUCCESS) {
        return result;
    }
    result = BuildConstantSourcePatches(plan);
    if (result != ACLRTC_SUCCESS) {
        return result;
    }
    compilationPlan = std::move(plan);
    return ACLRTC_SUCCESS;
}

} // namespace aclrtc
} // namespace ascendc
