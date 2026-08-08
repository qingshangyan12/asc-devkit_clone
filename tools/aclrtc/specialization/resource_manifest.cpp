/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "resource_manifest.h"

#include <exception>
#include <set>
#include <utility>
#include <vector>

#include "ascendc_tool_log.h"
#include "nlohmann/json.hpp"
#include "file_utils.h"

using Json = nlohmann::json;

namespace ascendc {
namespace resource {
namespace {

constexpr const char* kCommonCompileMarker = "${options:common_compile}";
constexpr const char* kResourceRootMarker = "${resource}";

bool CheckAndLog(bool condition, const std::string& message) noexcept
{
    if (condition) {
        return true;
    }
    ASCENDLOGE("Manifest validation failed: %s", message.c_str());
    return false;
}

class ManifestReferenceValidator final {
public:
    bool ValidateRelativePath(const std::string& value) const
    {
        if (!CheckAndLog(
                !value.empty() && value[0] != '/' && value.find('\\') == std::string::npos &&
                    value.find('\0') == std::string::npos,
                "resource path is not a relative POSIX path: " + value)) {
            return false;
        }
        for (const std::string& component : FileUtils::PathComponents(value)) {
            if (!CheckAndLog(
                    component != "." && component != "..", "resource path contains an unsafe component: " + value)) {
                return false;
            }
        }
        return true;
    }

    bool ValidateResourceReference(const std::string& value, const std::string& baseDir) const
    {
        const size_t marker = value.find(kResourceRootMarker);
        if (marker == std::string::npos) {
            return true;
        }
        const size_t pathBegin = marker + std::char_traits<char>::length(kResourceRootMarker);
        if (!CheckAndLog(
                value.find(kResourceRootMarker, pathBegin) == std::string::npos,
                "resource value contains multiple resource markers") ||
            !CheckAndLog(
                pathBegin < value.size() && value[pathBegin] == '/',
                "resource marker must be followed by a relative path")) {
            return false;
        }
        const std::string path = value.substr(pathBegin + 1U);
        return ValidateRelativePath(path) &&
               (baseDir.empty() ||
                CheckAndLog(FileUtils::IsWithinPath(path, baseDir), "resource path is outside base_dir: " + path));
    }

    bool ValidateMarkerSyntax(const std::string& value, const std::string& baseDir) const
    {
        if (!CheckAndLog(value.find('\0') == std::string::npos, "command value contains an embedded NUL")) {
            return false;
        }
        size_t begin = 0U;
        while ((begin = value.find("${", begin)) != std::string::npos) {
            const size_t end = value.find('}', begin + 2U);
            if (!CheckAndLog(end != std::string::npos, "unterminated command marker")) {
                return false;
            }
            const std::string marker = value.substr(begin, end - begin + 1U);
            if (!CheckAndLog(
                    marker == "${source_dir}" || marker == kResourceRootMarker || marker.rfind("${output:", 0U) == 0U ||
                        marker == "${env:ASCEND_HOME_PATH}",
                    "unsupported command marker " + marker)) {
                return false;
            }
            if (marker.rfind("${output:", 0U) == 0U && !ValidateRelativePath(marker.substr(9U, marker.size() - 10U))) {
                return false;
            }
            begin = end + 1U;
        }
        return ValidateResourceReference(value, baseDir);
    }

    bool ValidateOutputMarker(const std::string& value, const std::string& baseDir) const
    {
        return CheckAndLog(
                   value.rfind("${output:", 0U) == 0U && value.size() > 10U && value.back() == '}' &&
                       value.find('}', 9U) == value.size() - 1U,
                   "output must be one output marker") &&
               ValidateMarkerSyntax(value, baseDir);
    }
};

class ManifestParser final {
public:
    bool Parse(const std::string& text, ResourceManifest& parsed)
    {
        baseDir_.clear();
        commonCompileOptions_.clear();

        const Json manifest = Json::parse(text, nullptr, false);
        if (!CheckAndLog(!manifest.is_discarded(), "invalid JSON syntax") || !ValidateManifestBaseInfo(manifest)) {
            return false;
        }
        ASCENDLOGD("Manifest JSON and base fields are valid");

        ResourceManifest result;
        if (!ParseManifestFields(manifest, result)) {
            return false;
        }

        std::set<std::string> kernelNames;
        for (const Json& kernel : manifest.at("kernels")) {
            ManifestKernel parsedKernel;
            if (!ParseKernel(kernel, kernelNames, parsedKernel)) {
                return false;
            }
            result.kernels.push_back(std::move(parsedKernel));
        }

        parsed = std::move(result);
        ASCENDLOGI(
            "Manifest parsed successfully: resource_id=%s, soc_version=%s, kernels=%zu", parsed.resourceId.c_str(),
            parsed.socVersion.c_str(), parsed.kernels.size());
        return true;
    }

private:
    bool ValidateManifestBaseInfo(const Json& manifest) const
    {
        return CheckAndLog(manifest.is_object(), "manifest must be an object") &&
               CheckAndLog(
                   manifest.contains("schema_version") && manifest.at("schema_version").is_string(),
                   "invalid schema_version") &&
               CheckAndLog(
                   manifest.contains("resource_id") && manifest.at("resource_id").is_string() &&
                       !manifest.at("resource_id").get_ref<const std::string&>().empty(),
                   "invalid resource_id") &&
               CheckAndLog(
                   manifest.contains("soc_version") && manifest.at("soc_version").is_string() &&
                       !manifest.at("soc_version").get_ref<const std::string&>().empty(),
                   "invalid soc_version") &&
               CheckAndLog(
                   manifest.contains("kernels") && manifest.at("kernels").is_array() && !manifest.at("kernels").empty(),
                   "kernels must be a nonempty array");
    }

    bool ParseManifestFields(const Json& manifest, ResourceManifest& result)
    {
        if (manifest.contains("source_file")) {
            if (!CheckAndLog(manifest.at("source_file").is_string(), "invalid source_file")) {
                return false;
            }
            result.sourceFile = manifest.at("source_file").get<std::string>();
            if (!CheckAndLog(IsPlainName(result.sourceFile), "source_file must be a plain file name")) {
                return false;
            }
        }
        if (manifest.contains("base_dir")) {
            if (!CheckAndLog(manifest.at("base_dir").is_string(), "invalid base_dir")) {
                return false;
            }
            result.baseDir = manifest.at("base_dir").get<std::string>();
            if (!references_.ValidateRelativePath(result.baseDir)) {
                return false;
            }
        }

        result.resourceId = manifest.at("resource_id").get<std::string>();
        result.socVersion = manifest.at("soc_version").get<std::string>();
        baseDir_ = result.baseDir;
        ASCENDLOGD(
            "Parsed manifest fields: resource_id=%s, soc_version=%s, source_file=%s, base_dir=%s",
            result.resourceId.c_str(), result.socVersion.c_str(), result.sourceFile.c_str(), result.baseDir.c_str());
        return ParseCommonCompileOptions(manifest);
    }

    bool ParseCommonCompileOptions(const Json& manifest)
    {
        commonCompileOptions_.clear();
        if (!manifest.contains("options")) {
            return true;
        }
        const Json& options = manifest.at("options");
        if (!CheckAndLog(options.is_object(), "options must be an object")) {
            return false;
        }
        if (!options.contains("common_compile")) {
            return true;
        }
        const Json& compileOptions = options.at("common_compile");
        if (!CheckAndLog(compileOptions.is_array(), "common_compile must be an array")) {
            return false;
        }
        for (const Json& option : compileOptions) {
            if (!CheckAndLog(option.is_string(), "common compile option must be a string")) {
                return false;
            }
            const std::string value = option.get<std::string>();
            if (!references_.ValidateMarkerSyntax(value, baseDir_)) {
                return false;
            }
            commonCompileOptions_.push_back(value);
        }
        if (commonCompileOptions_.empty()) {
            ASCENDLOGW("Manifest common_compile is empty");
        } else {
            ASCENDLOGD("Parsed common compile options: count=%zu", commonCompileOptions_.size());
        }
        return true;
    }

    bool ParseKernel(const Json& kernel, std::set<std::string>& kernelNames, ManifestKernel& parsed)
    {
        if (!ValidateKernelShape(kernel)) {
            return false;
        }

        ManifestKernel result;
        result.kernelName = kernel.at("kernel_name").get<std::string>();
        if (!CheckAndLog(kernelNames.insert(result.kernelName).second, "duplicate kernel_name")) {
            return false;
        }
        if (kernel.contains("constants_info") && !ParseConstants(kernel.at("constants_info"), result)) {
            return false;
        }
        if (!ParseObjects(kernel.at("objects"), result)) {
            return false;
        }
        if (kernel.contains("link_options") && !ParseLinkOptions(kernel.at("link_options"), result)) {
            return false;
        }

        parsed = std::move(result);
        ASCENDLOGD(
            "Parsed kernel: name=%s, constants=%zu, objects=%zu, link_options=%zu", parsed.kernelName.c_str(),
            parsed.constants.size(), parsed.objects.size(), parsed.linkOptions.size());
        return true;
    }

    bool ValidateKernelShape(const Json& kernel) const
    {
        return CheckAndLog(kernel.is_object(), "kernel must be an object") &&
               CheckAndLog(
                   kernel.contains("kernel_name") && kernel.at("kernel_name").is_string() &&
                       !kernel.at("kernel_name").get_ref<const std::string&>().empty(),
                   "invalid kernel_name") &&
               CheckAndLog(
                   !kernel.contains("constants_info") || kernel.at("constants_info").is_array(),
                   "constants_info must be an array") &&
               CheckAndLog(
                   kernel.contains("objects") && kernel.at("objects").is_array() && !kernel.at("objects").empty(),
                   "objects must be nonempty") &&
               CheckAndLog(
                   !kernel.contains("link_options") || kernel.at("link_options").is_array(),
                   "link_options must be an array");
    }

    bool ParseConstants(const Json& constants, ManifestKernel& kernel)
    {
        std::set<std::string> constantNames;
        for (const Json& constant : constants) {
            ManifestConstant parsed;
            if (!ParseConstant(constant, constantNames, parsed)) {
                return false;
            }
            kernel.constants.push_back(std::move(parsed));
        }
        return true;
    }

    bool ParseConstant(const Json& constant, std::set<std::string>& constantNames, ManifestConstant& parsed) const
    {
        if (!CheckAndLog(constant.is_object() && constant.size() == 5U, "constant must contain five fields") ||
            !CheckAndLog(
                constant.contains("name") && constant.at("name").is_string() &&
                    !constant.at("name").get_ref<const std::string&>().empty(),
                "invalid constant name") ||
            !CheckAndLog(
                constant.contains("parameter_index") && constant.at("parameter_index").is_number_unsigned(),
                "invalid parameter_index") ||
            !CheckAndLog(
                constant.contains("byte_size") && constant.at("byte_size").is_number_unsigned() &&
                    constant.at("byte_size").get<uint64_t>() != 0U,
                "invalid byte_size") ||
            !CheckAndLog(constant.contains("file") && constant.at("file").is_string(), "invalid constant file") ||
            !CheckAndLog(
                constant.contains("template") && constant.at("template").is_string() &&
                    !constant.at("template").get_ref<const std::string&>().empty(),
                "invalid constant template")) {
            return false;
        }

        ManifestConstant result;
        result.name = constant.at("name").get<std::string>();
        result.parameterIndex = constant.at("parameter_index").get<uint64_t>();
        result.byteSize = constant.at("byte_size").get<uint64_t>();
        result.file = constant.at("file").get<std::string>();
        result.templateValue = constant.at("template").get<std::string>();
        if (!CheckAndLog(
                result.file.rfind(std::string(kResourceRootMarker) + "/", 0U) == 0U,
                "constant file must start with ${resource}/") ||
            !references_.ValidateResourceReference(result.file, baseDir_) ||
            !CheckAndLog(constantNames.insert(result.name).second, "duplicate constant name")) {
            return false;
        }

        parsed = std::move(result);
        ASCENDLOGD(
            "Parsed constant: name=%s, parameter_index=%llu, byte_size=%llu", parsed.name.c_str(),
            static_cast<unsigned long long>(parsed.parameterIndex), static_cast<unsigned long long>(parsed.byteSize));
        return true;
    }

    bool ParseObjects(const Json& objects, ManifestKernel& kernel)
    {
        std::set<std::string> objectNames;
        for (const Json& object : objects) {
            ManifestObject parsed;
            if (!ParseObject(object, objectNames, parsed)) {
                return false;
            }
            kernel.objects.push_back(std::move(parsed));
        }
        return true;
    }

    bool ParseObject(const Json& object, std::set<std::string>& objectNames, ManifestObject& parsed)
    {
        if (!ValidateObjectShape(object)) {
            return false;
        }

        ManifestObject result;
        result.objectsName = object.at("objects_name").get<std::string>();
        result.objectsType = object.at("objects_type").get<std::string>();
        if (!CheckAndLog(IsPlainName(result.objectsName), "objects_name must be a plain name") ||
            !CheckAndLog(objectNames.insert(result.objectsName).second, "duplicate objects_name") ||
            !CheckAndLog(result.objectsType == "basic" || result.objectsType == "sk", "unsupported objects_type") ||
            !ParseObjectOutputs(object.at("output"), result) || !ParseCommands(object.at("commands"), result) ||
            !ValidateObjectOutputs(result)) {
            return false;
        }

        parsed = std::move(result);
        ASCENDLOGD(
            "Parsed object: name=%s, type=%s, commands=%zu, outputs=%zu", parsed.objectsName.c_str(),
            parsed.objectsType.c_str(), parsed.commands.size(), parsed.outputs.size());
        return true;
    }

    bool ValidateObjectShape(const Json& object) const
    {
        return CheckAndLog(object.is_object(), "object must be an object") &&
               CheckAndLog(
                   object.contains("objects_name") && object.at("objects_name").is_string() &&
                       !object.at("objects_name").get_ref<const std::string&>().empty(),
                   "invalid objects_name") &&
               CheckAndLog(
                   object.contains("objects_type") && object.at("objects_type").is_string(), "invalid objects_type") &&
               CheckAndLog(
                   object.contains("commands") && object.at("commands").is_array() && !object.at("commands").empty(),
                   "object commands must be nonempty") &&
               CheckAndLog(
                   object.contains("output") && object.at("output").is_array() && !object.at("output").empty(),
                   "object output must be nonempty");
    }

    bool ParseObjectOutputs(const Json& outputs, ManifestObject& object) const
    {
        for (const Json& output : outputs) {
            if (!CheckAndLog(output.is_string(), "object output must be a string")) {
                return false;
            }
            const std::string value = output.get<std::string>();
            if (!references_.ValidateOutputMarker(value, baseDir_)) {
                return false;
            }
            object.outputs.push_back(value);
        }
        return true;
    }

    bool ParseCommands(const Json& commands, ManifestObject& object)
    {
        for (const Json& command : commands) {
            ManifestCommand parsed;
            if (!ParseCommand(command, parsed)) {
                return false;
            }
            object.commands.push_back(std::move(parsed));
        }
        return true;
    }

    bool ParseCommand(const Json& command, ManifestCommand& parsed)
    {
        if (!CheckAndLog(command.is_object(), "command must be an object") ||
            !CheckAndLog(command.contains("type") && command.at("type").is_string(), "invalid command type") ||
            !CheckAndLog(
                command.contains("stage") && command.at("stage").is_number_unsigned(), "invalid command stage") ||
            !CheckAndLog(command.contains("options") && command.at("options").is_array(), "invalid command options")) {
            return false;
        }

        ManifestCommand result;
        result.type = command.at("type").get<std::string>();
        result.stage = command.at("stage").get<uint64_t>();
        if (!CheckAndLog(
                result.type == "compile" || result.type == "ld" || result.type == "objcopy",
                "unsupported command type") ||
            !ParseCommandOptions(command.at("options"), result)) {
            return false;
        }

        parsed = std::move(result);
        ASCENDLOGD(
            "Parsed command: type=%s, stage=%llu, options=%zu", parsed.type.c_str(),
            static_cast<unsigned long long>(parsed.stage), parsed.options.size());
        return true;
    }

    bool ParseCommandOptions(const Json& options, ManifestCommand& command)
    {
        for (const Json& option : options) {
            if (!CheckAndLog(option.is_string(), "command option must be a string")) {
                return false;
            }
            const std::string value = option.get<std::string>();
            if (value == kCommonCompileMarker) {
                if (!CheckAndLog(!commonCompileOptions_.empty(), "common compile marker references an empty option")) {
                    return false;
                }
                command.options.insert(
                    command.options.end(), commonCompileOptions_.begin(), commonCompileOptions_.end());
                continue;
            }

            command.options.push_back(value);
        }
        for (const std::string& option : command.options) {
            if (!references_.ValidateMarkerSyntax(option, baseDir_)) {
                return false;
            }
        }
        return true;
    }

    bool ValidateObjectOutputs(const ManifestObject& object) const
    {
        std::set<std::string> commandOutputs;
        for (const ManifestCommand& command : object.commands) {
            for (const std::string& option : command.options) {
                CollectOutputMarkers(option, commandOutputs);
            }
        }
        for (const std::string& output : object.outputs) {
            if (!CheckAndLog(
                    commandOutputs.count(output) != 0U, "object output is not referenced by its commands: " + output)) {
                return false;
            }
        }
        ASCENDLOGD(
            "Validated object output references: object=%s, declared=%zu, referenced=%zu", object.objectsName.c_str(),
            object.outputs.size(), commandOutputs.size());
        return true;
    }

    bool ParseLinkOptions(const Json& options, ManifestKernel& kernel) const
    {
        for (const Json& option : options) {
            if (!CheckAndLog(option.is_string(), "link option must be a string")) {
                return false;
            }
            const std::string value = option.get<std::string>();
            if (!references_.ValidateMarkerSyntax(value, baseDir_)) {
                return false;
            }
            kernel.linkOptions.push_back(value);
        }
        return true;
    }

    static bool IsPlainName(const std::string& value)
    {
        return !value.empty() && value.find('/') == std::string::npos && value.find('\\') == std::string::npos &&
               value != "." && value != "..";
    }

    static void CollectOutputMarkers(const std::string& value, std::set<std::string>& outputs)
    {
        size_t begin = 0U;
        while ((begin = value.find("${output:", begin)) != std::string::npos) {
            const size_t end = value.find('}', begin + 9U);
            if (end == std::string::npos) {
                return;
            }
            outputs.insert(value.substr(begin, end - begin + 1U));
            begin = end + 1U;
        }
    }

    ManifestReferenceValidator references_;
    std::string baseDir_;
    std::vector<std::string> commonCompileOptions_;
};

} // namespace

bool ValidateRelativeResourcePath(const std::string& value) noexcept
{
    try {
        return ManifestReferenceValidator().ValidateRelativePath(value);
    } catch (const std::exception& error) {
        ASCENDLOGE("Resource path validation failed: %s", error.what());
    } catch (...) {
        ASCENDLOGE("Unknown resource path validation failure");
    }
    return false;
}

bool ParseResourceManifest(const std::string& text, const std::string& origin, ResourceManifest& manifest) noexcept
{
    ASCENDLOGI("Parsing resource manifest: origin=%s", origin.empty() ? "<unknown>" : origin.c_str());
    try {
        ResourceManifest parsed;
        if (!ManifestParser().Parse(text, parsed)) {
            return false;
        }
        manifest = std::move(parsed);
        return true;
    } catch (const std::exception& error) {
        ASCENDLOGE("Manifest parsing failed with exception: %s", error.what());
    } catch (...) {
        ASCENDLOGE("Manifest parsing failed with unknown exception");
    }
    return false;
}

} // namespace resource
} // namespace ascendc
