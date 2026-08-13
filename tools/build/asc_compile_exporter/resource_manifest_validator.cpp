/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "resource_manifest_validator.h"

#include <set>

#include "ascendc_tool_log.h"
#include "file_utils.h"
#include "nlohmann/json.hpp"

namespace ascendc {
namespace {

using Json = nlohmann::json;

constexpr const char* COMMON_COMPILE_MARKER = "${options:common_compile}";
constexpr const char* RESOURCE_ROOT_MARKER = "${resource}";
constexpr char OUTPUT_MARKER[] = "${output}";
constexpr std::size_t OUTPUT_MARKER_SIZE = sizeof(OUTPUT_MARKER) - 1U;
constexpr char OUTPUT_REFERENCE_PREFIX[] = "${output}/";
constexpr std::size_t OUTPUT_REFERENCE_PREFIX_SIZE = sizeof(OUTPUT_REFERENCE_PREFIX) - 1U;
constexpr const char* SOURCE_DIR_MARKER = "${source_dir}";
constexpr const char* ASCEND_HOME_MARKER = "${env:ASCEND_HOME_PATH}";
constexpr const char* SOURCE_FILE_PATH_MARKER = "${source_file_path}";

bool CheckAndLog(bool condition, const std::string& message) noexcept
{
    if (condition) {
        return true;
    }
    ASCENDLOGE("Manifest validation failed: %s", message.c_str());
    return false;
}

bool ValidateResourceReference(const std::string& value, const std::string& resourcePath)
{
    const size_t marker = value.find(RESOURCE_ROOT_MARKER);
    if (marker == std::string::npos) {
        return true;
    }
    const size_t pathBegin = marker + std::char_traits<char>::length(RESOURCE_ROOT_MARKER);
    if (!CheckAndLog(
            value.find(RESOURCE_ROOT_MARKER, pathBegin) == std::string::npos,
            "resource value contains multiple resource markers") ||
        !CheckAndLog(
            pathBegin < value.size() && value[pathBegin] == '/',
            "resource marker must be followed by a relative path")) {
        return false;
    }
    const std::string path = value.substr(pathBegin + 1U);
    return CheckAndLog(
               FileUtils::IsSafeRelativePath(path), "resource path is not a safe relative POSIX path: " + path) &&
           (resourcePath.empty() ||
            CheckAndLog(
                FileUtils::IsPathWithin(path, resourcePath), "resource path is outside resource_path: " + path));
}

bool ValidateOutputMarker(const std::string& value)
{
    if (!CheckAndLog(
            value.rfind(OUTPUT_REFERENCE_PREFIX, 0U) == 0U && value.size() > OUTPUT_REFERENCE_PREFIX_SIZE,
            "output must use ${output}/ followed by a relative path")) {
        return false;
    }
    const std::string path = value.substr(OUTPUT_REFERENCE_PREFIX_SIZE);
    return CheckAndLog(path.find("${") == std::string::npos, "output path must not contain another marker") &&
           CheckAndLog(FileUtils::IsSafeRelativePath(path), "output path is not a safe relative POSIX path: " + path);
}

bool ValidateMarkerSyntax(const std::string& value, const std::string& resourcePath)
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
                marker == SOURCE_DIR_MARKER || marker == RESOURCE_ROOT_MARKER || marker == ASCEND_HOME_MARKER ||
                    marker == SOURCE_FILE_PATH_MARKER,
                "unsupported command marker " + marker)) {
            return false;
        }
        begin = end + 1U;
    }
    return ValidateResourceReference(value, resourcePath);
}

bool ValidateOptionValue(const std::string& value, const std::string& resourcePath)
{
    if (!(value.size() >= OUTPUT_MARKER_SIZE && value.compare(0U, OUTPUT_MARKER_SIZE, OUTPUT_MARKER) == 0)) {
        return ValidateMarkerSyntax(value, resourcePath);
    }
    if (!ValidateOutputMarker(value)) {
        return false;
    }
    return true;
}

bool ValidateOptionValue(
    const std::string& value, const std::string& resourcePath, std::set<std::string>& referencedOutputs)
{
    if (!ValidateOptionValue(value, resourcePath)) {
        return false;
    }
    if (value.size() >= OUTPUT_MARKER_SIZE && value.compare(0U, OUTPUT_MARKER_SIZE, OUTPUT_MARKER) == 0) {
        referencedOutputs.insert(value);
    }
    return true;
}

bool ValidateCommonCompileOptions(const Json& manifest, const std::string& resourcePath, bool& hasCommonCompileOptions)
{
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
        if (!ValidateOptionValue(option.get_ref<const std::string&>(), resourcePath)) {
            return false;
        }
    }
    hasCommonCompileOptions = !compileOptions.empty();
    if (compileOptions.empty()) {
        ASCENDLOGW("Manifest common_compile is empty");
    } else {
        ASCENDLOGD("Validated common compile options: count=%zu", compileOptions.size());
    }
    return true;
}

bool ValidateManifestFields(const Json& manifest, std::string& resourcePath, bool& hasCommonCompileOptions)
{
    std::string sourceFile;
    if (manifest.contains("source_file")) {
        if (!CheckAndLog(manifest.at("source_file").is_string(), "invalid source_file")) {
            return false;
        }
        sourceFile = manifest.at("source_file").get_ref<const std::string&>();
        if (!CheckAndLog(
                FileUtils::IsSafeRelativePath(sourceFile) && FileUtils::FileName(sourceFile) == sourceFile,
                "source_file must be a plain file name")) {
            return false;
        }
    }
    if (manifest.contains("resource_path")) {
        if (!CheckAndLog(manifest.at("resource_path").is_string(), "invalid resource_path")) {
            return false;
        }
        resourcePath = manifest.at("resource_path").get_ref<const std::string&>();
        if (!CheckAndLog(
                FileUtils::IsSafeRelativePath(resourcePath),
                "resource_path is not a safe relative POSIX path: " + resourcePath)) {
            return false;
        }
    }

    ASCENDLOGD(
        "Validated manifest fields: resource_id=%s, soc_version=%s, source_file=%s, resource_path=%s",
        manifest.at("resource_id").get_ref<const std::string&>().c_str(),
        manifest.at("soc_version").get_ref<const std::string&>().c_str(), sourceFile.c_str(), resourcePath.c_str());
    return ValidateCommonCompileOptions(manifest, resourcePath, hasCommonCompileOptions);
}

bool ValidateConstant(const Json& constant, const std::string& resourcePath, std::set<std::string>& constantNames)
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
                constant.at("byte_size").get<Json::number_unsigned_t>() != 0U,
            "invalid byte_size") ||
        !CheckAndLog(constant.contains("file") && constant.at("file").is_string(), "invalid constant file") ||
        !CheckAndLog(
            constant.contains("template") && constant.at("template").is_string() &&
                !constant.at("template").get_ref<const std::string&>().empty(),
            "invalid constant template")) {
        return false;
    }

    const std::string name = constant.at("name").get_ref<const std::string&>();
    const std::string file = constant.at("file").get_ref<const std::string&>();
    if (!CheckAndLog(
            file.rfind(std::string(RESOURCE_ROOT_MARKER) + "/", 0U) == 0U,
            "constant file must start with ${resource}/") ||
        !ValidateResourceReference(file, resourcePath) ||
        !CheckAndLog(constantNames.insert(name).second, "duplicate constant name")) {
        return false;
    }

    ASCENDLOGD(
        "Validated constant: name=%s, parameter_index=%llu, byte_size=%llu", name.c_str(),
        static_cast<unsigned long long>(constant.at("parameter_index").get<Json::number_unsigned_t>()),
        static_cast<unsigned long long>(constant.at("byte_size").get<Json::number_unsigned_t>()));
    return true;
}

bool ValidateConstants(const Json& constantInfos, const std::string& resourcePath)
{
    std::set<std::string> constantNames;
    for (const Json& constant : constantInfos) {
        if (!ValidateConstant(constant, resourcePath, constantNames)) {
            return false;
        }
    }
    return true;
}

bool ValidateObjectOutputs(const Json& outputs)
{
    for (const Json& output : outputs) {
        if (!CheckAndLog(output.is_string(), "object output must be a string")) {
            return false;
        }
        if (!ValidateOutputMarker(output.get_ref<const std::string&>())) {
            return false;
        }
    }
    return true;
}

bool ValidateObjectOutputReferences(const Json& outputs, const std::set<std::string>& referencedOutputs)
{
    for (const Json& output : outputs) {
        const std::string value = output.get_ref<const std::string&>();
        if (!CheckAndLog(
                referencedOutputs.count(value) != 0U, "object output is not referenced by its commands: " + value)) {
            return false;
        }
    }
    ASCENDLOGD(
        "Validated object output references: declared=%zu, referenced=%zu", outputs.size(), referencedOutputs.size());
    return true;
}

bool ValidateCommandOptions(
    const Json& commandValues, const std::string& resourcePath, bool hasCommonCompileOptions,
    std::set<std::string>& referencedOutputs)
{
    for (const Json& option : commandValues) {
        if (!CheckAndLog(option.is_string(), "command option must be a string")) {
            return false;
        }
        const std::string value = option.get_ref<const std::string&>();
        if (value == COMMON_COMPILE_MARKER) {
            if (!CheckAndLog(hasCommonCompileOptions, "common compile marker references an empty option")) {
                return false;
            }
            continue;
        }
        if (!ValidateOptionValue(value, resourcePath, referencedOutputs)) {
            return false;
        }
    }
    return true;
}

bool ValidateCommand(
    const Json& command, const std::string& resourcePath, bool hasCommonCompileOptions,
    std::set<std::string>& referencedOutputs)
{
    if (!CheckAndLog(command.is_object(), "command must be an object") ||
        !CheckAndLog(command.contains("type") && command.at("type").is_string(), "invalid command type") ||
        !CheckAndLog(command.contains("stage") && command.at("stage").is_number_unsigned(), "invalid command stage") ||
        !CheckAndLog(command.contains("cmd") && command.at("cmd").is_array(), "invalid command cmd")) {
        return false;
    }

    const std::string type = command.at("type").get_ref<const std::string&>();
    if (!ValidateCommandOptions(command.at("cmd"), resourcePath, hasCommonCompileOptions, referencedOutputs)) {
        return false;
    }

    ASCENDLOGD(
        "Validated command: type=%s, stage=%llu, cmd=%zu", type.c_str(),
        static_cast<unsigned long long>(command.at("stage").get<Json::number_unsigned_t>()), command.at("cmd").size());
    return true;
}

bool ValidateCommands(
    const Json& commands, const std::string& resourcePath, bool hasCommonCompileOptions,
    std::set<std::string>& referencedOutputs)
{
    for (const Json& command : commands) {
        if (!ValidateCommand(command, resourcePath, hasCommonCompileOptions, referencedOutputs)) {
            return false;
        }
    }
    return true;
}

bool ValidateObject(
    const Json& object, const std::string& resourcePath, bool hasCommonCompileOptions,
    std::set<std::string>& objectNames)
{
    if (!CheckAndLog(object.is_object(), "object must be an object") ||
        !CheckAndLog(
            object.contains("object_name") && object.at("object_name").is_string() &&
                !object.at("object_name").get_ref<const std::string&>().empty(),
            "invalid object_name") ||
        !CheckAndLog(object.contains("object_type") && object.at("object_type").is_string(), "invalid object_type") ||
        !CheckAndLog(
            object.contains("commands") && object.at("commands").is_array() && !object.at("commands").empty(),
            "object commands must be nonempty") ||
        !CheckAndLog(
            object.contains("outputs") && object.at("outputs").is_array() && !object.at("outputs").empty(),
            "object outputs must be nonempty")) {
        return false;
    }

    const std::string objectName = object.at("object_name").get_ref<const std::string&>();
    const std::string objectType = object.at("object_type").get_ref<const std::string&>();
    if (!CheckAndLog(
            FileUtils::IsSafeRelativePath(objectName) && FileUtils::FileName(objectName) == objectName,
            "object_name must be a plain name") ||
        !CheckAndLog(objectNames.insert(objectName).second, "duplicate object_name") ||
        !CheckAndLog(objectType == "basic" || objectType == "sk", "unsupported object_type") ||
        !ValidateObjectOutputs(object.at("outputs"))) {
        return false;
    }

    std::set<std::string> referencedOutputs;
    if (!ValidateCommands(object.at("commands"), resourcePath, hasCommonCompileOptions, referencedOutputs) ||
        !ValidateObjectOutputReferences(object.at("outputs"), referencedOutputs)) {
        return false;
    }

    ASCENDLOGD(
        "Validated object: name=%s, type=%s, commands=%zu, outputs=%zu", objectName.c_str(), objectType.c_str(),
        object.at("commands").size(), object.at("outputs").size());
    return true;
}

bool ValidateObjects(const Json& objects, const std::string& resourcePath, bool hasCommonCompileOptions)
{
    std::set<std::string> objectNames;
    for (const Json& object : objects) {
        if (!ValidateObject(object, resourcePath, hasCommonCompileOptions, objectNames)) {
            return false;
        }
    }
    return true;
}

bool ValidateKernel(
    const Json& kernel, const std::string& resourcePath, bool hasCommonCompileOptions,
    std::set<std::string>& kernelNames)
{
    if (!CheckAndLog(kernel.is_object(), "kernel must be an object") ||
        !CheckAndLog(
            kernel.contains("kernel_name") && kernel.at("kernel_name").is_string() &&
                !kernel.at("kernel_name").get_ref<const std::string&>().empty(),
            "invalid kernel_name") ||
        !CheckAndLog(
            !kernel.contains("constant_infos") || kernel.at("constant_infos").is_array(),
            "constant_infos must be an array") ||
        !CheckAndLog(
            kernel.contains("objects") && kernel.at("objects").is_array() && !kernel.at("objects").empty(),
            "objects must be nonempty") ||
        !CheckAndLog(
            !kernel.contains("link_options") || kernel.at("link_options").is_array(),
            "link_options must be an array")) {
        return false;
    }

    const std::string kernelName = kernel.at("kernel_name").get_ref<const std::string&>();
    if (!CheckAndLog(kernelNames.insert(kernelName).second, "duplicate kernel_name")) {
        return false;
    }
    if (kernel.contains("constant_infos") && !ValidateConstants(kernel.at("constant_infos"), resourcePath)) {
        return false;
    }
    if (!ValidateObjects(kernel.at("objects"), resourcePath, hasCommonCompileOptions)) {
        return false;
    }

    ASCENDLOGD(
        "Validated kernel: name=%s, constants=%zu, objects=%zu, link_options=%zu", kernelName.c_str(),
        kernel.contains("constant_infos") ? kernel.at("constant_infos").size() : 0U, kernel.at("objects").size(),
        kernel.contains("link_options") ? kernel.at("link_options").size() : 0U);
    return true;
}

bool ValidateManifest(const Json& manifest)
{
    if (!CheckAndLog(manifest.is_object(), "manifest must be an object") ||
        !CheckAndLog(
            manifest.contains("schema_version") && manifest.at("schema_version").is_string(),
            "invalid schema_version") ||
        !CheckAndLog(
            manifest.contains("resource_id") && manifest.at("resource_id").is_string() &&
                !manifest.at("resource_id").get_ref<const std::string&>().empty(),
            "invalid resource_id") ||
        !CheckAndLog(
            manifest.contains("soc_version") && manifest.at("soc_version").is_string() &&
                !manifest.at("soc_version").get_ref<const std::string&>().empty(),
            "invalid soc_version") ||
        !CheckAndLog(
            manifest.contains("kernels") && manifest.at("kernels").is_array() && !manifest.at("kernels").empty(),
            "kernels must be a nonempty array")) {
        return false;
    }
    ASCENDLOGD("Manifest JSON and base fields are valid");

    std::string resourcePath;
    bool hasCommonCompileOptions = false;
    if (!ValidateManifestFields(manifest, resourcePath, hasCommonCompileOptions)) {
        return false;
    }

    std::set<std::string> kernelNames;
    for (const Json& kernel : manifest.at("kernels")) {
        if (!ValidateKernel(kernel, resourcePath, hasCommonCompileOptions, kernelNames)) {
            return false;
        }
    }

    ASCENDLOGI(
        "Manifest validated successfully: resource_id=%s, soc_version=%s, kernels=%zu",
        manifest.at("resource_id").get_ref<const std::string&>().c_str(),
        manifest.at("soc_version").get_ref<const std::string&>().c_str(), manifest.at("kernels").size());
    return true;
}

} // namespace

bool ValidateResourceManifest(const Json& manifest, const std::string& origin)
{
    ASCENDLOGI("Validating resource manifest: origin=%s", origin.empty() ? "<unknown>" : origin.c_str());
    const bool valid = ValidateManifest(manifest);
    if (!valid) {
        ASCENDLOGE("Resource manifest validation failed: origin=%s", origin.empty() ? "<unknown>" : origin.c_str());
    }
    return valid;
}

} // namespace ascendc
