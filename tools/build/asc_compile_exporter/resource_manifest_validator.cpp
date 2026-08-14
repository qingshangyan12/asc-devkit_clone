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

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>

#include "ascendc_tool_log.h"
#include "file_utils.h"
#include "nlohmann/json.hpp"

namespace ascendc {
namespace manifest_generator {
namespace {

using Json = nlohmann::json;

constexpr const char* RESOURCE_ROOT_MARKER = "${resource}";
constexpr char OUTPUT_REFERENCE_PREFIX[] = "${output}/";
constexpr std::size_t OUTPUT_REFERENCE_PREFIX_SIZE = sizeof(OUTPUT_REFERENCE_PREFIX) - 1U;
constexpr const char* SOURCE_FILE_PATH_MARKER = "${source_file_path}";

bool CheckAndLog(bool condition, int lineNumber, const std::string& message) noexcept
{
    if (condition) {
        return true;
    }
    ASCENDLOGE("Manifest validation failed, linenumber: %d, %s", lineNumber, message.c_str());
    return false;
}

} // namespace

ResourceManifestValidator::ResourceManifestValidator(const Json& manifest, const std::string& manifestPath)
    : manifest_(manifest),
      manifestPath_(manifestPath),
      manifestRoot_(FileUtils::ParentPath(manifestPath)),
      options_(nullptr),
      hasSourceFile_(false)
{}

bool ResourceManifestValidator::IsIdentifier(const std::string& value)
{
    if (value.empty() ||
        !((value[0] >= 'A' && value[0] <= 'Z') || (value[0] >= 'a' && value[0] <= 'z') || value[0] == '_')) {
        return false;
    }
    for (size_t index = 1U; index < value.size(); ++index) {
        const char character = value[index];
        if (!((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z') ||
              (character >= '0' && character <= '9') || character == '_')) {
            return false;
        }
    }
    return true;
}

bool ResourceManifestValidator::ParseNamedMarker(
    const std::string& marker, const std::string& prefix, std::string& name)
{
    if (marker.size() <= prefix.size() + 1U || marker.compare(0U, prefix.size(), prefix) != 0 || marker.back() != '}') {
        return false;
    }
    name = marker.substr(prefix.size(), marker.size() - prefix.size() - 1U);
    return IsIdentifier(name);
}

bool ResourceManifestValidator::ValidateManifestBaseFields()
{
    return CheckAndLog(manifest_.is_object(), __LINE__, "manifest must be an object") &&
           CheckAndLog(
               manifest_.contains("schema_version") && manifest_.at("schema_version").is_string(), __LINE__,
               "invalid schema_version") &&
           CheckAndLog(
               manifest_.contains("resource_id") && manifest_.at("resource_id").is_string() &&
                   !manifest_.at("resource_id").get_ref<const std::string&>().empty(),
               __LINE__, "invalid resource_id") &&
           CheckAndLog(
               manifest_.contains("soc_version") && manifest_.at("soc_version").is_string() &&
                   !manifest_.at("soc_version").get_ref<const std::string&>().empty(),
               __LINE__, "invalid soc_version") &&
           CheckAndLog(
               manifest_.contains("resource_path") && manifest_.at("resource_path").is_string(), __LINE__,
               "invalid resource_path") &&
           CheckAndLog(
               manifest_.contains("kernels") && manifest_.at("kernels").is_array() && !manifest_.at("kernels").empty(),
               __LINE__, "kernels must be a nonempty array");
}

bool ResourceManifestValidator::ValidateManifestFields()
{
    std::string sourceFile;
    if (manifest_.contains("source_file")) {
        if (!CheckAndLog(manifest_.at("source_file").is_string(), __LINE__, "invalid source_file")) {
            return false;
        }
        sourceFile = manifest_.at("source_file").get_ref<const std::string&>();
        if (!CheckAndLog(
                FileUtils::IsSafeRelativePath(sourceFile) && FileUtils::FileName(sourceFile) == sourceFile, __LINE__,
                "source_file must be a plain file name")) {
            return false;
        }
        hasSourceFile_ = true;
    }
    if (!resourcePath_.empty() && !CheckAndLog(
                                      FileUtils::IsSafeRelativePath(resourcePath_), __LINE__,
                                      "resource_path is not a safe relative POSIX path: " + resourcePath_)) {
        return false;
    }
    if (manifest_.contains("options")) {
        if (!CheckAndLog(manifest_.at("options").is_object(), __LINE__, "options must be an object")) {
            return false;
        }
        options_ = &manifest_.at("options");
    }

    ASCENDLOGD(
        "Validated manifest fields: resource_id=%s, soc_version=%s, source_file=%s, resource_path=%s",
        manifest_.at("resource_id").get_ref<const std::string&>().c_str(),
        manifest_.at("soc_version").get_ref<const std::string&>().c_str(), sourceFile.c_str(), resourcePath_.c_str());
    return true;
}

bool ResourceManifestValidator::ValidateResourceTarget(const std::string& relativePath)
{
    const std::string target = FileUtils::JoinPath(manifestRoot_, relativePath);
    boost::system::error_code error;
    const boost::filesystem::file_status status =
        boost::filesystem::symlink_status(boost::filesystem::path(target), error);
    return CheckAndLog(!error, __LINE__, "failed to inspect resource target: " + target) &&
           CheckAndLog(boost::filesystem::exists(status), __LINE__, "resource target does not exist: " + target) &&
           CheckAndLog(!boost::filesystem::is_symlink(status), __LINE__, "resource target is a symlink: " + target) &&
           CheckAndLog(
               boost::filesystem::is_regular_file(status) || boost::filesystem::is_directory(status), __LINE__,
               "resource target must be a regular file or directory: " + target);
}

bool ResourceManifestValidator::ValidateResourceReference(const std::string& value)
{
    const size_t marker = value.find(RESOURCE_ROOT_MARKER);
    if (marker == std::string::npos) {
        return true;
    }
    const size_t pathBegin = marker + std::char_traits<char>::length(RESOURCE_ROOT_MARKER);
    if (!CheckAndLog(
            value.find(RESOURCE_ROOT_MARKER, pathBegin) == std::string::npos, __LINE__,
            "resource value contains multiple resource markers") ||
        !CheckAndLog(
            pathBegin < value.size() && value[pathBegin] == '/', __LINE__,
            "resource marker must be followed by a relative path")) {
        return false;
    }
    const std::string path = value.substr(pathBegin + 1U);
    return CheckAndLog(
               FileUtils::IsSafeRelativePath(path), __LINE__,
               "resource path is not a safe relative POSIX path: " + path) &&
           CheckAndLog(!resourcePath_.empty(), __LINE__, "resource marker requires a nonempty resource_path") &&
           CheckAndLog(
               FileUtils::IsPathWithin(path, resourcePath_), __LINE__,
               "resource path is outside resource_path: " + path) &&
           ValidateResourceTarget(path);
}

bool ResourceManifestValidator::ValidateOutputMarker(const std::string& value)
{
    if (!CheckAndLog(
            value.rfind(OUTPUT_REFERENCE_PREFIX, 0U) == 0U && value.size() > OUTPUT_REFERENCE_PREFIX_SIZE, __LINE__,
            "output must use ${output}/ followed by a relative path")) {
        return false;
    }
    const std::string path = value.substr(OUTPUT_REFERENCE_PREFIX_SIZE);
    return CheckAndLog(path.find("${") == std::string::npos, __LINE__, "output path must not contain another marker") &&
           CheckAndLog(
               FileUtils::IsSafeRelativePath(path), __LINE__, "output path is not a safe relative POSIX path: " + path);
}

bool ResourceManifestValidator::ValidateMarkerSyntax(const std::string& value)
{
    if (!CheckAndLog(value.find('\0') == std::string::npos, __LINE__, "command value contains an embedded NUL")) {
        return false;
    }
    size_t begin = 0U;
    while ((begin = value.find("${", begin)) != std::string::npos) {
        const size_t end = value.find('}', begin + 2U);
        if (!CheckAndLog(end != std::string::npos, __LINE__, "unterminated command marker")) {
            return false;
        }
        const std::string marker = value.substr(begin, end - begin + 1U);
        std::string name;
        if (marker == SOURCE_FILE_PATH_MARKER &&
            !CheckAndLog(hasSourceFile_, __LINE__, "source_file_path marker requires source_file")) {
            return false;
        }
        const bool knownMarker = marker == RESOURCE_ROOT_MARKER || marker == SOURCE_FILE_PATH_MARKER;
        const bool environmentMarker = ParseNamedMarker(marker, "${env:", name);
        if (!CheckAndLog(knownMarker || environmentMarker, __LINE__, "unsupported command marker " + marker)) {
            return false;
        }
        begin = end + 1U;
    }
    return ValidateResourceReference(value);
}

bool ResourceManifestValidator::ValidateOptionValue(const std::string& value)
{
    const size_t outputBegin = value.find(OUTPUT_REFERENCE_PREFIX);
    if (outputBegin == std::string::npos) {
        return ValidateMarkerSyntax(value);
    }
    return ValidateMarkerSyntax(value.substr(0U, outputBegin)) && ValidateOutputMarker(value.substr(outputBegin));
}

bool ResourceManifestValidator::ValidateOptionValue(const std::string& value, std::set<std::string>& referencedOutputs)
{
    if (!ValidateOptionValue(value)) {
        return false;
    }
    const size_t outputBegin = value.find(OUTPUT_REFERENCE_PREFIX);
    if (outputBegin != std::string::npos) {
        referencedOutputs.insert(value.substr(outputBegin));
    }
    return true;
}

bool ResourceManifestValidator::ValidateOptionReference(const std::string& marker)
{
    std::string name;
    return CheckAndLog(ParseNamedMarker(marker, "${options:", name), __LINE__, "invalid options marker " + marker) &&
           ValidateOptionArray(name);
}

bool ResourceManifestValidator::ValidateOptionArray(const std::string& name)
{
    if (validatedOptions_.count(name) != 0U) {
        return true;
    }
    if (!CheckAndLog(
            options_ != nullptr && options_->contains(name), __LINE__,
            "options marker references a missing field: " + name)) {
        return false;
    }
    const Json& values = options_->at(name);
    if (!CheckAndLog(
            values.is_array() && !values.empty(), __LINE__,
            "referenced option field must be a nonempty array: " + name)) {
        return false;
    }
    for (const Json& value : values) {
        if (!CheckAndLog(value.is_string(), __LINE__, "referenced option item must be a string: " + name) ||
            !ValidateOptionValue(value.get_ref<const std::string&>())) {
            return false;
        }
    }
    validatedOptions_.insert(name);
    return true;
}

bool ResourceManifestValidator::ValidateConstant(const Json& constant, std::set<std::string>& constantNames)
{
    if (!CheckAndLog(constant.is_object() && constant.size() == 5U, __LINE__, "constant must contain five fields") ||
        !CheckAndLog(
            constant.contains("name") && constant.at("name").is_string() &&
                !constant.at("name").get_ref<const std::string&>().empty(),
            __LINE__, "invalid constant name") ||
        !CheckAndLog(
            constant.contains("parameter_index") && constant.at("parameter_index").is_number_unsigned(), __LINE__,
            "invalid parameter_index") ||
        !CheckAndLog(
            constant.contains("byte_size") && constant.at("byte_size").is_number_unsigned() &&
                constant.at("byte_size").get<Json::number_unsigned_t>() != 0U,
            __LINE__, "invalid byte_size") ||
        !CheckAndLog(constant.contains("file") && constant.at("file").is_string(), __LINE__, "invalid constant file") ||
        !CheckAndLog(
            constant.contains("template") && constant.at("template").is_string() &&
                !constant.at("template").get_ref<const std::string&>().empty(),
            __LINE__, "invalid constant template")) {
        return false;
    }

    const std::string name = constant.at("name").get_ref<const std::string&>();
    const std::string file = constant.at("file").get_ref<const std::string&>();
    if (!CheckAndLog(
            file.rfind(std::string(RESOURCE_ROOT_MARKER) + "/", 0U) == 0U, __LINE__,
            "constant file must start with ${resource}/") ||
        !ValidateResourceReference(file) ||
        !CheckAndLog(constantNames.insert(name).second, __LINE__, "duplicate constant name")) {
        return false;
    }

    ASCENDLOGD(
        "Validated constant: name=%s, parameter_index=%llu, byte_size=%llu", name.c_str(),
        static_cast<unsigned long long>(constant.at("parameter_index").get<Json::number_unsigned_t>()),
        static_cast<unsigned long long>(constant.at("byte_size").get<Json::number_unsigned_t>()));
    return true;
}

bool ResourceManifestValidator::ValidateConstants(const Json& constantInfos)
{
    std::set<std::string> constantNames;
    for (const Json& constant : constantInfos) {
        if (!ValidateConstant(constant, constantNames)) {
            return false;
        }
    }
    return true;
}

bool ResourceManifestValidator::ValidateObjectOutputs(const Json& outputs)
{
    for (const Json& output : outputs) {
        if (!CheckAndLog(output.is_string(), __LINE__, "object output must be a string") ||
            !ValidateOutputMarker(output.get_ref<const std::string&>())) {
            return false;
        }
    }
    return true;
}

bool ResourceManifestValidator::ValidateObjectOutputReferences(
    const Json& outputs, const std::set<std::string>& referencedOutputs)
{
    for (const Json& output : outputs) {
        const std::string value = output.get_ref<const std::string&>();
        if (!CheckAndLog(
                referencedOutputs.count(value) != 0U, __LINE__,
                "object output is not referenced by its commands: " + value)) {
            return false;
        }
    }
    ASCENDLOGD(
        "Validated object output references: declared=%zu, referenced=%zu", outputs.size(), referencedOutputs.size());
    return true;
}

bool ResourceManifestValidator::ValidateCommandOptions(
    const Json& commandValues, std::set<std::string>& referencedOutputs)
{
    for (const Json& option : commandValues) {
        if (!CheckAndLog(option.is_string(), __LINE__, "command option must be a string")) {
            return false;
        }
        const std::string value = option.get_ref<const std::string&>();
        if (value.find("${options:") != std::string::npos) {
            if (!ValidateOptionReference(value)) {
                return false;
            }
            continue;
        }
        if (!ValidateOptionValue(value, referencedOutputs)) {
            return false;
        }
    }
    return true;
}

bool ResourceManifestValidator::ValidateCommand(const Json& command, std::set<std::string>& referencedOutputs)
{
    if (!CheckAndLog(command.is_object(), __LINE__, "command must be an object") ||
        !CheckAndLog(command.contains("type") && command.at("type").is_string(), __LINE__, "invalid command type") ||
        !CheckAndLog(
            command.contains("stage") && command.at("stage").is_number_unsigned(), __LINE__, "invalid command stage") ||
        !CheckAndLog(command.contains("cmd") && command.at("cmd").is_array(), __LINE__, "invalid command cmd")) {
        return false;
    }

    const std::string type = command.at("type").get_ref<const std::string&>();
    if (!ValidateCommandOptions(command.at("cmd"), referencedOutputs)) {
        return false;
    }
    ASCENDLOGD(
        "Validated command: type=%s, stage=%llu, cmd=%zu", type.c_str(),
        static_cast<unsigned long long>(command.at("stage").get<Json::number_unsigned_t>()), command.at("cmd").size());
    return true;
}

bool ResourceManifestValidator::ValidateCommands(const Json& commands, std::set<std::string>& referencedOutputs)
{
    for (const Json& command : commands) {
        if (!ValidateCommand(command, referencedOutputs)) {
            return false;
        }
    }
    return true;
}

bool ResourceManifestValidator::ValidateObject(const Json& object, std::set<std::string>& objectNames)
{
    if (!CheckAndLog(object.is_object(), __LINE__, "object must be an object") ||
        !CheckAndLog(
            object.contains("object_name") && object.at("object_name").is_string() &&
                !object.at("object_name").get_ref<const std::string&>().empty(),
            __LINE__, "invalid object_name") ||
        !CheckAndLog(
            object.contains("object_type") && object.at("object_type").is_string(), __LINE__, "invalid object_type") ||
        !CheckAndLog(
            object.contains("commands") && object.at("commands").is_array() && !object.at("commands").empty(), __LINE__,
            "object commands must be nonempty") ||
        !CheckAndLog(
            object.contains("outputs") && object.at("outputs").is_array() && !object.at("outputs").empty(), __LINE__,
            "object outputs must be nonempty")) {
        return false;
    }

    const std::string objectName = object.at("object_name").get_ref<const std::string&>();
    const std::string objectType = object.at("object_type").get_ref<const std::string&>();
    if (!CheckAndLog(
            FileUtils::IsSafeRelativePath(objectName) && FileUtils::FileName(objectName) == objectName, __LINE__,
            "object_name must be a plain name") ||
        !CheckAndLog(objectNames.insert(objectName).second, __LINE__, "duplicate object_name") ||
        !CheckAndLog(objectType == "basic" || objectType == "sk", __LINE__, "unsupported object_type") ||
        !ValidateObjectOutputs(object.at("outputs"))) {
        return false;
    }

    std::set<std::string> referencedOutputs;
    if (!ValidateCommands(object.at("commands"), referencedOutputs) ||
        !ValidateObjectOutputReferences(object.at("outputs"), referencedOutputs)) {
        return false;
    }
    ASCENDLOGD(
        "Validated object: name=%s, type=%s, commands=%zu, outputs=%zu", objectName.c_str(), objectType.c_str(),
        object.at("commands").size(), object.at("outputs").size());
    return true;
}

bool ResourceManifestValidator::ValidateObjects(const Json& objects)
{
    std::set<std::string> objectNames;
    for (const Json& object : objects) {
        if (!ValidateObject(object, objectNames)) {
            return false;
        }
    }
    return true;
}

bool ResourceManifestValidator::ValidateKernel(const Json& kernel, std::set<std::string>& kernelNames)
{
    if (!CheckAndLog(kernel.is_object(), __LINE__, "kernel must be an object") ||
        !CheckAndLog(
            kernel.contains("kernel_name") && kernel.at("kernel_name").is_string() &&
                !kernel.at("kernel_name").get_ref<const std::string&>().empty(),
            __LINE__, "invalid kernel_name") ||
        !CheckAndLog(
            !kernel.contains("constant_infos") || kernel.at("constant_infos").is_array(), __LINE__,
            "constant_infos must be an array") ||
        !CheckAndLog(
            kernel.contains("objects") && kernel.at("objects").is_array() && !kernel.at("objects").empty(), __LINE__,
            "objects must be nonempty") ||
        !CheckAndLog(
            !kernel.contains("link_options") || kernel.at("link_options").is_array(), __LINE__,
            "link_options must be an array")) {
        return false;
    }

    const std::string kernelName = kernel.at("kernel_name").get_ref<const std::string&>();
    if (!CheckAndLog(kernelNames.insert(kernelName).second, __LINE__, "duplicate kernel_name") ||
        (kernel.contains("constant_infos") && !ValidateConstants(kernel.at("constant_infos"))) ||
        !ValidateObjects(kernel.at("objects"))) {
        return false;
    }
    ASCENDLOGD(
        "Validated kernel: name=%s, constants=%zu, objects=%zu, link_options=%zu", kernelName.c_str(),
        kernel.contains("constant_infos") ? kernel.at("constant_infos").size() : 0U, kernel.at("objects").size(),
        kernel.contains("link_options") ? kernel.at("link_options").size() : 0U);
    return true;
}

bool ResourceManifestValidator::Validate()
{
    options_ = nullptr;
    validatedOptions_.clear();
    hasSourceFile_ = false;

    if (!ValidateManifestBaseFields()) {
        return false;
    }
    ASCENDLOGD("Manifest JSON and base fields are valid: path=%s", manifestPath_.c_str());

    resourcePath_ = manifest_.at("resource_path").get_ref<const std::string&>();
    if (!ValidateManifestFields()) {
        return false;
    }

    std::set<std::string> kernelNames;
    for (const Json& kernel : manifest_.at("kernels")) {
        if (!ValidateKernel(kernel, kernelNames)) {
            return false;
        }
    }
    ASCENDLOGI(
        "Manifest validated successfully: resource_id=%s, soc_version=%s, kernels=%zu",
        manifest_.at("resource_id").get_ref<const std::string&>().c_str(),
        manifest_.at("soc_version").get_ref<const std::string&>().c_str(), manifest_.at("kernels").size());
    return true;
}

bool ValidateResourceManifest(const Json& manifest, const std::string& manifestPath)
{
    ASCENDLOGI("Validating resource manifest: path=%s", manifestPath.c_str());
    const bool valid = ResourceManifestValidator(manifest, manifestPath).Validate();
    if (!valid) {
        ASCENDLOGE("Resource manifest validation failed: path=%s", manifestPath.c_str());
    }
    return valid;
}

} // namespace manifest_generator
} // namespace ascendc
