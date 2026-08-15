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

#include "kernel_specialization_log.h"

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
using Json = nlohmann::json;

constexpr char SUPPORTED_MANIFEST_SCHEMA_VERSION[] = "1.0";
constexpr char OPTIONS_REFERENCE_PREFIX[] = "${options:";
constexpr char ENVIRONMENT_REFERENCE_PREFIX[] = "${env:";

enum class SelectedObjectKind : uint32_t {
    Basic,
    SuperKernel,
};

struct SelectedManifestObject {
    const Json* object {nullptr};
    SelectedObjectKind kind {SelectedObjectKind::Basic};
};

struct ManifestExpansionContext {
    const Json& resourceManifest;
    const fs::path& resourceWorktreePath;
    const fs::path& outputDirectoryPath;
    const fs::path& sourceFilePath;
};

const char* GetSelectedObjectKindName(SelectedObjectKind objectKind) noexcept
{
    return objectKind == SelectedObjectKind::Basic ? "basic" : "sk";
}

void ReplaceAllLiteralReferences(std::string& text, const std::string& reference, const std::string& value)
{
    size_t referencePosition = 0U;
    while ((referencePosition = text.find(reference, referencePosition)) != std::string::npos) {
        text.replace(referencePosition, reference.size(), value);
        referencePosition += value.size();
    }
}

bool ResolveEnvironmentReferences(const std::string& text, std::string& resolvedText)
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
        if (closingBracePosition == std::string::npos || closingBracePosition == namePosition) {
            ACLRTC_LOG_ERROR("Manifest contains a malformed environment variable placeholder");
            return false;
        }
        const std::string environmentName = text.substr(namePosition, closingBracePosition - namePosition);
        const char* environmentValue = std::getenv(environmentName.c_str());
        if (environmentValue == nullptr) {
            ACLRTC_LOG_ERROR("Environment variable %s is not set; set it with: export %s=<value>",
                             environmentName.c_str(), environmentName.c_str());
            return false;
        }
        resolvedText.append(environmentValue);
        sourcePosition = closingBracePosition + 1U;
    }
    return true;
}

bool ResolveManifestText(const std::string& manifestText, const ManifestExpansionContext& expansionContext,
                         std::string& resolvedText)
{
    std::string pathResolvedText = manifestText;
    ReplaceAllLiteralReferences(pathResolvedText, "${resource}", expansionContext.resourceWorktreePath.string());
    ReplaceAllLiteralReferences(pathResolvedText, "${output}", expansionContext.outputDirectoryPath.string());
    ReplaceAllLiteralReferences(pathResolvedText, "${source_file_path}",
                                expansionContext.sourceFilePath.parent_path().string());
    return ResolveEnvironmentReferences(pathResolvedText, resolvedText);
}

bool ParseOptionReference(const std::string& argument, std::string& optionName)
{
    if (argument.compare(0, sizeof(OPTIONS_REFERENCE_PREFIX) - 1U, OPTIONS_REFERENCE_PREFIX) != 0 ||
        argument.size() <= sizeof(OPTIONS_REFERENCE_PREFIX) || argument.back() != '}') {
        return false;
    }
    optionName = argument.substr(sizeof(OPTIONS_REFERENCE_PREFIX) - 1U,
                                 argument.size() - sizeof(OPTIONS_REFERENCE_PREFIX));
    return !optionName.empty();
}

aclError ExpandManifestArgument(const std::string& manifestArgument,
                                const ManifestExpansionContext& expansionContext,
                                std::vector<std::string>& expandedArguments)
{
    std::string optionName;
    if (!ParseOptionReference(manifestArgument, optionName)) {
        std::string resolvedArgument;
        if (!ResolveManifestText(manifestArgument, expansionContext, resolvedArgument)) {
            return ACLRTC_ERROR_FAILURE;
        }
        expandedArguments.emplace_back(std::move(resolvedArgument));
        return ACLRTC_SUCCESS;
    }

    const Json& optionTable = expansionContext.resourceManifest.at("options");
    const std::vector<std::string> optionArguments = optionTable.at(optionName).get<std::vector<std::string>>();
    for (const std::string& optionArgument : optionArguments) {
        std::string resolvedArgument;
        if (!ResolveManifestText(optionArgument, expansionContext, resolvedArgument)) {
            return ACLRTC_ERROR_FAILURE;
        }
        expandedArguments.emplace_back(std::move(resolvedArgument));
    }
    return ACLRTC_SUCCESS;
}

bool IsExecutableRegularFile(const fs::path& executablePath)
{
    boost::system::error_code fileError;
    return executablePath.is_absolute() && fs::is_regular_file(executablePath, fileError) && !fileError &&
           access(executablePath.c_str(), X_OK) == 0;
}

bool FindCompilerOutputOption(const std::vector<std::string>& commandArguments, size_t& outputOptionPosition)
{
    for (size_t argumentIndex = 0; argumentIndex < commandArguments.size(); ++argumentIndex) {
        const std::string& argument = commandArguments[argumentIndex];
        if (argument == "-o" || (argument.size() > 2U && argument.compare(0, 2, "-o") == 0)) {
            outputOptionPosition = argumentIndex;
            return true;
        }
    }
    return false;
}

std::string EncodeConstantBytes(const void* constantAddress, uint64_t constantByteCount)
{
    const auto* constantBytes = static_cast<const uint8_t*>(constantAddress);
    std::ostringstream encodedBytes;
    encodedBytes << '{';
    for (uint64_t byteIndex = 0; byteIndex < constantByteCount; ++byteIndex) {
        if (byteIndex != 0U) {
            encodedBytes << ", ";
        }
        encodedBytes << "0x" << std::hex << std::nouppercase << std::setw(2) << std::setfill('0')
                     << static_cast<unsigned int>(constantBytes[byteIndex]);
    }
    encodedBytes << '}';
    return encodedBytes.str();
}

aclError BuildSourcePatches(const Json& selectedKernel,
                            const NormalizedKernelSpecializationRequest& specializationRequest,
                            const ManifestExpansionContext& expansionContext,
                            std::vector<KernelSourcePatch>& sourcePatches)
{
    const auto constantInfos = selectedKernel.find("constant_infos");
    if (constantInfos == selectedKernel.end() || constantInfos->empty()) {
        ACLRTC_LOG_ERROR("Selected kernel must contain at least one constant_info for static compilation");
        return ACLRTC_ERROR_FAILURE;
    }
    for (const Json& constantInfo : *constantInfos) {
        const uint64_t parameterIndex = constantInfo.at("parameter_index").get<uint64_t>();
        const uint64_t constantByteCount = constantInfo.at("byte_size").get<uint64_t>();
        if (specializationRequest.borrowedKernelArgumentDataPointers == nullptr ||
            specializationRequest.borrowedKernelArgumentByteCounts == nullptr) {
            ACLRTC_LOG_ERROR("constant parameter_index %llu requires argsAddr and argsBytes",
                             static_cast<unsigned long long>(parameterIndex));
            return ACLRTC_ERROR_INVALID_INPUT;
        }
        if (parameterIndex >= specializationRequest.kernelArgumentCount) {
            ACLRTC_LOG_ERROR("constant parameter_index %llu is outside argsCount %llu",
                             static_cast<unsigned long long>(parameterIndex),
                             static_cast<unsigned long long>(specializationRequest.kernelArgumentCount));
            return ACLRTC_ERROR_INVALID_INPUT;
        }
        if (specializationRequest.borrowedKernelArgumentDataPointers[parameterIndex] == nullptr) {
            ACLRTC_LOG_ERROR("argsAddr[%llu] must not be null for a static constant",
                             static_cast<unsigned long long>(parameterIndex));
            return ACLRTC_ERROR_INVALID_INPUT;
        }
        const uint64_t suppliedByteCount =
            specializationRequest.borrowedKernelArgumentByteCounts[parameterIndex];
        if (suppliedByteCount != constantByteCount) {
            ACLRTC_LOG_ERROR("argsBytes[%llu]=%llu does not match required byte_size %llu",
                             static_cast<unsigned long long>(parameterIndex),
                             static_cast<unsigned long long>(suppliedByteCount),
                             static_cast<unsigned long long>(constantByteCount));
            return ACLRTC_ERROR_INVALID_INPUT;
        }

        std::string resolvedTargetFilePath;
        if (!ResolveManifestText(constantInfo.at("file").get<std::string>(), expansionContext,
                                 resolvedTargetFilePath)) {
            return ACLRTC_ERROR_FAILURE;
        }
        sourcePatches.push_back(
            {resolvedTargetFilePath, constantInfo.at("template").get<std::string>(),
             EncodeConstantBytes(specializationRequest.borrowedKernelArgumentDataPointers[parameterIndex],
                                 constantByteCount)});
    }
    return ACLRTC_SUCCESS;
}

aclError BuildManifestCommand(const Json& commandJson, const SelectedManifestObject& selectedObject,
                              size_t commandIndex,
                              const NormalizedKernelSpecializationRequest& specializationRequest,
                              const ManifestExpansionContext& expansionContext,
                              CompilationCommand& compilationCommand)
{
    const char* objectKindName = GetSelectedObjectKindName(selectedObject.kind);
    const std::string commandType = commandJson.at("type").get<std::string>();
    if (commandType == "compile") {
        compilationCommand.commandKind = CompilationCommandKind::Compile;
    } else if (commandType == "objcopy") {
        compilationCommand.commandKind = CompilationCommandKind::ObjectCopy;
    } else {
        ACLRTC_LOG_ERROR("Manifest object %s command[%zu] has unsupported type %s",
                         objectKindName, commandIndex, commandType.c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    const auto parallelStage = commandJson.find("stage");
    if (parallelStage != commandJson.end()) {
        compilationCommand.parallelStage = parallelStage->get<uint32_t>();
    }
    compilationCommand.diagnosticLabel = std::string(objectKindName) + "/" + commandType + "[" +
                                         std::to_string(commandIndex) + "]";

    const std::vector<std::string> manifestArguments = commandJson.at("cmd").get<std::vector<std::string>>();
    if (manifestArguments.empty()) {
        ACLRTC_LOG_ERROR("Manifest command %s has an empty cmd array",
                         compilationCommand.diagnosticLabel.c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    std::vector<std::string> expandedExecutable;
    aclError result = ExpandManifestArgument(manifestArguments.front(), expansionContext, expandedExecutable);
    if (result != ACLRTC_SUCCESS) {
        return result;
    }
    if (expandedExecutable.size() != 1U) {
        ACLRTC_LOG_ERROR("Manifest command %s executable must expand to exactly one argv item",
                         compilationCommand.diagnosticLabel.c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    compilationCommand.executablePath = std::move(expandedExecutable.front());

    for (size_t argumentIndex = 1U; argumentIndex < manifestArguments.size(); ++argumentIndex) {
        result = ExpandManifestArgument(manifestArguments[argumentIndex], expansionContext,
                                        compilationCommand.commandArguments);
        if (result != ACLRTC_SUCCESS) {
            return result;
        }
    }

    if (compilationCommand.commandKind == CompilationCommandKind::Compile) {
        size_t outputOptionPosition = 0U;
        if (!FindCompilerOutputOption(compilationCommand.commandArguments, outputOptionPosition)) {
            ACLRTC_LOG_ERROR("Manifest compile command %s must contain an output option",
                             compilationCommand.diagnosticLabel.c_str());
            return ACLRTC_ERROR_FAILURE;
        }
        const std::vector<std::string>& userOptions = selectedObject.kind == SelectedObjectKind::Basic
            ? specializationRequest.compilerOptions.basicOptions
            : specializationRequest.compilerOptions.superKernelOptions;
        compilationCommand.commandArguments.insert(
            compilationCommand.commandArguments.begin() + static_cast<std::ptrdiff_t>(outputOptionPosition),
            userOptions.begin(), userOptions.end());
    }
    if (!IsExecutableRegularFile(compilationCommand.executablePath)) {
        ACLRTC_LOG_ERROR("Manifest command %s executable is unavailable or not executable: %s",
                         compilationCommand.diagnosticLabel.c_str(), compilationCommand.executablePath.c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    return ACLRTC_SUCCESS;
}

aclError SelectManifestObjects(const Json& selectedKernel, CompilationVariant compilationVariant,
                               std::vector<SelectedManifestObject>& selectedObjects)
{
    const Json* basicObject = nullptr;
    const Json* superKernelObject = nullptr;
    for (const Json& object : selectedKernel.at("objects")) {
        const std::string objectType = object.at("object_type").get<std::string>();
        if (objectType == "basic" && basicObject == nullptr) {
            basicObject = &object;
        } else if (objectType == "sk" && superKernelObject == nullptr) {
            superKernelObject = &object;
        }
    }
    if (basicObject == nullptr) {
        ACLRTC_LOG_ERROR("Selected kernel does not contain a basic object");
        return ACLRTC_ERROR_FAILURE;
    }
    selectedObjects.push_back({basicObject, SelectedObjectKind::Basic});
    if (compilationVariant == CompilationVariant::BasicWithSuperKernel) {
        if (superKernelObject == nullptr) {
            ACLRTC_LOG_ERROR("Super-kernel compilation was requested but the selected kernel has no sk object");
            return ACLRTC_ERROR_FAILURE;
        }
        selectedObjects.push_back({superKernelObject, SelectedObjectKind::SuperKernel});
    }
    return ACLRTC_SUCCESS;
}

aclError BuildLinkCommand(const Json& selectedKernel, const std::vector<SelectedManifestObject>& selectedObjects,
                          const ManifestExpansionContext& expansionContext, CompilationCommand& linkCommand,
                          std::set<fs::path>& requiredOutputDirectories, fs::path& linkedKernelElfPath)
{
    linkCommand.commandKind = CompilationCommandKind::Link;
    linkCommand.diagnosticLabel = "link";
    std::string linkerPath;
    if (!ResolveManifestText("${env:ASCEND_HOME_PATH}/bin/ld.lld", expansionContext, linkerPath)) {
        return ACLRTC_ERROR_FAILURE;
    }
    linkCommand.executablePath = linkerPath;

    const auto linkOptions = selectedKernel.find("link_options");
    if (linkOptions != selectedKernel.end()) {
        for (const std::string& linkOption : linkOptions->get<std::vector<std::string>>()) {
            std::string resolvedLinkOption;
            if (!ResolveManifestText(linkOption, expansionContext, resolvedLinkOption)) {
                return ACLRTC_ERROR_FAILURE;
            }
            linkCommand.commandArguments.emplace_back(std::move(resolvedLinkOption));
        }
    }
    for (const SelectedManifestObject& selectedObject : selectedObjects) {
        for (const std::string& output : selectedObject.object->at("outputs").get<std::vector<std::string>>()) {
            std::string resolvedOutput;
            if (!ResolveManifestText(output, expansionContext, resolvedOutput)) {
                return ACLRTC_ERROR_FAILURE;
            }
            linkCommand.commandArguments.emplace_back(resolvedOutput);
            requiredOutputDirectories.emplace(fs::path(resolvedOutput).parent_path());
        }
    }
    linkedKernelElfPath = expansionContext.outputDirectoryPath / "linked_kernel.elf";
    requiredOutputDirectories.emplace(linkedKernelElfPath.parent_path());
    linkCommand.commandArguments.emplace_back("-o");
    linkCommand.commandArguments.emplace_back(linkedKernelElfPath.string());
    if (!IsExecutableRegularFile(linkCommand.executablePath)) {
        ACLRTC_LOG_ERROR("Linker is unavailable or not executable: %s", linkCommand.executablePath.c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    return ACLRTC_SUCCESS;
}
} // namespace

KernelCompilationPlanBuilder::KernelCompilationPlanBuilder(
    const NormalizedKernelSpecializationRequest& specializationRequest, const nlohmann::json& resourceManifest,
    fs::path resourceWorktreePath, fs::path sourceFilePath)
    : specializationRequest_(specializationRequest), resourceManifest_(resourceManifest),
      resourceWorktreePath_(std::move(resourceWorktreePath)), sourceFilePath_(std::move(sourceFilePath))
{}

aclError KernelCompilationPlanBuilder::BuildCompilationPlan(KernelCompilationPlan& compilationPlan) const
{
    try {
        if (!resourceWorktreePath_.is_absolute()) {
            ACLRTC_LOG_ERROR("Resource worktree path must be absolute: %s", resourceWorktreePath_.c_str());
            return ACLRTC_ERROR_FAILURE;
        }
        boost::system::error_code worktreeError;
        if (!fs::is_directory(resourceWorktreePath_, worktreeError) || worktreeError) {
            ACLRTC_LOG_ERROR("Resource worktree is unavailable: path=%s error=%d",
                             resourceWorktreePath_.c_str(), worktreeError.value());
            return ACLRTC_ERROR_FAILURE;
        }
        if (!sourceFilePath_.is_absolute()) {
            ACLRTC_LOG_ERROR("sourceFilePath must be absolute: %s", sourceFilePath_.c_str());
            return ACLRTC_ERROR_FAILURE;
        }
        boost::system::error_code sourceFileError;
        if (!fs::is_regular_file(sourceFilePath_, sourceFileError) || sourceFileError) {
            ACLRTC_LOG_ERROR("sourceFilePath is unavailable: path=%s error=%d",
                             sourceFilePath_.c_str(), sourceFileError.value());
            return ACLRTC_ERROR_FAILURE;
        }
        const std::string schemaVersion = resourceManifest_.at("schema_version").get<std::string>();
        if (schemaVersion != SUPPORTED_MANIFEST_SCHEMA_VERSION) {
            ACLRTC_LOG_ERROR("Unsupported Manifest schema_version %s; expected %s", schemaVersion.c_str(),
                             SUPPORTED_MANIFEST_SCHEMA_VERSION);
            return ACLRTC_ERROR_FAILURE;
        }

        const Json* selectedKernel = nullptr;
        for (const Json& kernel : resourceManifest_.at("kernels")) {
            if (kernel.at("kernel_name").get<std::string>() == specializationRequest_.kernelName) {
                selectedKernel = &kernel;
                break;
            }
        }
        if (selectedKernel == nullptr) {
            ACLRTC_LOG_ERROR("Kernel entry %s is not present in the selected JIT resource",
                             specializationRequest_.kernelName.c_str());
            return ACLRTC_ERROR_FAILURE;
        }

        KernelCompilationPlan builtPlan;
        const fs::path outputDirectoryPath =
            resourceWorktreePath_ / (".aclrtc_" + specializationRequest_.specializationSessionId) / "outputs";
        const ManifestExpansionContext expansionContext {
            resourceManifest_, resourceWorktreePath_, outputDirectoryPath, sourceFilePath_};
        aclError result = BuildSourcePatches(*selectedKernel, specializationRequest_, expansionContext,
                                             builtPlan.sourcePatches);
        if (result != ACLRTC_SUCCESS) {
            return result;
        }

        std::vector<SelectedManifestObject> selectedObjects;
        result = SelectManifestObjects(*selectedKernel, specializationRequest_.compilationVariant, selectedObjects);
        if (result != ACLRTC_SUCCESS) {
            return result;
        }

        for (const SelectedManifestObject& selectedObject : selectedObjects) {
            const Json& commands = selectedObject.object->at("commands");
            for (size_t commandIndex = 0U; commandIndex < commands.size(); ++commandIndex) {
                CompilationCommand command;
                result = BuildManifestCommand(commands.at(commandIndex), selectedObject, commandIndex,
                                              specializationRequest_, expansionContext, command);
                if (result != ACLRTC_SUCCESS) {
                    return result;
                }
                builtPlan.compilationCommands.emplace_back(std::move(command));
            }
        }

        std::set<fs::path> requiredOutputDirectories {outputDirectoryPath};
        CompilationCommand linkCommand;
        result = BuildLinkCommand(*selectedKernel, selectedObjects, expansionContext, linkCommand,
                                  requiredOutputDirectories,
                                  builtPlan.linkedKernelElfPath);
        if (result != ACLRTC_SUCCESS) {
            return result;
        }
        builtPlan.compilationCommands.emplace_back(std::move(linkCommand));
        builtPlan.requiredOutputDirectoryPaths.assign(requiredOutputDirectories.begin(),
                                                      requiredOutputDirectories.end());
        compilationPlan = std::move(builtPlan);
        return ACLRTC_SUCCESS;
    } catch (const nlohmann::json::exception& exception) {
        ACLRTC_LOG_ERROR("Selected manifest data is incompatible: %s", exception.what());
        return ACLRTC_ERROR_FAILURE;
    }
}

} // namespace aclrtc
} // namespace ascendc
