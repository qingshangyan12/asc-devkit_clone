/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "collected_manifest_repository.h"

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <stdexcept>
#include <utility>

#include <sys/stat.h>

#include "ascendc_tool_log.h"
#include "file_utils.h"
#include "nlohmann/json.hpp"

using Json = nlohmann::json;

namespace ascendc {
namespace manifest_generator {
namespace {

constexpr uintmax_t kMaxManifestBytes = 8U * 1024U * 1024U;
constexpr uintmax_t kMaxResourceFileBytes = 1U * 1024U * 1024U;
constexpr uintmax_t kMaxBundleBytes = 256U * 1024U * 1024U;
constexpr size_t kMaxManifestCount = 4096U;
constexpr size_t kMaxFilesPerManifest = 4096U;
constexpr size_t kMaxFilesPerBundle = 65536U;
constexpr size_t kMaxDirectoryDepth = 64U;

std::runtime_error SystemError(const std::string& operation)
{
    return std::runtime_error(operation + ": " + std::strerror(errno));
}

class ResourceBudget final {
public:
    void AddResource(uintmax_t size, const std::string& field)
    {
        if (fileCount_ >= kMaxFilesPerBundle) {
            throw std::runtime_error(
                "bundle exceeds resource file count limit of " + std::to_string(kMaxFilesPerBundle));
        }
        ++fileCount_;
        AddPayload(size, field);
    }

    void AddPayload(uintmax_t size, const std::string& field)
    {
        if (size > kMaxBundleBytes - totalBytes_) {
            throw std::runtime_error("bundle exceeds raw payload limit while adding " + field);
        }
        totalBytes_ += size;
    }

private:
    uintmax_t totalBytes_{0U};
    size_t fileCount_{0U};
};

std::string ReadBaseDir(const std::string& text, const std::string& manifestPath)
{
    Json manifest;
    try {
        manifest = Json::parse(text);
    } catch (const Json::parse_error& error) {
        throw std::runtime_error("manifest is not valid JSON: " + manifestPath + ": " + error.what());
    }

    try {
        return manifest.at("base_dir").get<std::string>();
    } catch (const Json::exception& error) {
        throw std::runtime_error("manifest base_dir is unavailable: " + manifestPath + ": " + error.what());
    }
}

bool IsManifestFile(const std::string& path)
{
    constexpr const char* suffix = "_manifest.json";
    const std::string name = FileUtils::FileName(path);
    return name.size() > std::strlen(suffix) &&
           name.compare(name.size() - std::strlen(suffix), std::strlen(suffix), suffix) == 0;
}

void DiscoverManifestFilesBelow(const std::string& directory, std::vector<std::string>& manifests, size_t depth)
{
    DIR* stream = opendir(directory.c_str());
    if (stream == nullptr) {
        throw SystemError("failed to enumerate " + directory);
    }
    try {
        while (dirent* entry = readdir(stream)) {
            const std::string name(entry->d_name);
            if (name == "." || name == "..") {
                continue;
            }
            const std::string path = FileUtils::JoinPath(directory, name);
            struct stat info {};
            if (lstat(path.c_str(), &info) != 0) {
                throw SystemError("failed to inspect collection entry " + path);
            }
            if (S_ISLNK(info.st_mode)) {
                continue;
            }
            if (S_ISDIR(info.st_mode)) {
                if (depth >= kMaxDirectoryDepth) {
                    throw std::runtime_error(
                        "collection exceeds directory depth limit of " + std::to_string(kMaxDirectoryDepth) + ": " +
                        path);
                }
                DiscoverManifestFilesBelow(path, manifests, depth + 1U);
                continue;
            }
            if (!S_ISREG(info.st_mode) || !IsManifestFile(path)) {
                continue;
            }
            if (manifests.size() >= kMaxManifestCount) {
                throw std::runtime_error(
                    "collection exceeds manifest count limit of " + std::to_string(kMaxManifestCount));
            }
            manifests.push_back(path);
        }
    } catch (...) {
        (void)closedir(stream);
        throw;
    }
    (void)closedir(stream);
}

std::vector<std::string> DiscoverManifestFiles(const std::string& manifestSearchRoot)
{
    std::vector<std::string> manifests;
    DiscoverManifestFilesBelow(manifestSearchRoot, manifests, 0U);
    std::sort(manifests.begin(), manifests.end());
    if (manifests.empty()) {
        throw std::runtime_error("collected resources contain no manifest units");
    }
    return manifests;
}

void LoadResourceFiles(
    const std::string& directory, const std::string& manifestRoot, const std::string& manifestPath,
    ResourceBudget& budget, ManifestUnit& unit, size_t depth)
{
    DIR* stream = opendir(directory.c_str());
    if (stream == nullptr) {
        throw SystemError("failed to enumerate manifest base_dir " + directory);
    }
    try {
        while (dirent* entry = readdir(stream)) {
            const std::string name(entry->d_name);
            if (name == "." || name == "..") {
                continue;
            }
            const std::string source = FileUtils::JoinPath(directory, name);
            struct stat info {};
            if (lstat(source.c_str(), &info) != 0) {
                throw SystemError("failed to inspect manifest resource " + source);
            }
            if (S_ISLNK(info.st_mode)) {
                throw std::runtime_error("manifest base_dir contains a symlink: " + source);
            }
            if (S_ISDIR(info.st_mode)) {
                if (depth >= kMaxDirectoryDepth) {
                    throw std::runtime_error(
                        "manifest resources exceed directory depth limit of " + std::to_string(kMaxDirectoryDepth) +
                        ": " + source);
                }
                LoadResourceFiles(source, manifestRoot, manifestPath, budget, unit, depth + 1U);
                continue;
            }
            if (!S_ISREG(info.st_mode)) {
                throw std::runtime_error("manifest base_dir contains a non-regular file: " + source);
            }
            std::vector<uint8_t> data;
            if (!FileUtils::ReadRegularFile(source, kMaxResourceFileBytes, data)) {
                throw std::runtime_error("failed to read permitted manifest resource: " + source);
            }
            if (data.empty()) {
                ASCENDLOGD("Skipping empty manifest resource %s", source.c_str());
                continue;
            }
            if (unit.files.size() >= kMaxFilesPerManifest) {
                throw std::runtime_error("manifest exceeds resource file count limit: " + manifestPath);
            }
            budget.AddResource(data.size(), "manifest resource " + source);
            ResourceFile file;
            file.fileName = name;
            file.filePath = source.substr(manifestRoot.size() + 1U);
            file.data = data;
            unit.files.push_back(std::move(file));
        }
    } catch (...) {
        (void)closedir(stream);
        throw;
    }
    (void)closedir(stream);
}

ManifestUnit LoadUnit(const std::string& manifestPath, ResourceBudget& budget)
{
    ASCENDLOGD("Loading manifest %s", manifestPath.c_str());
    ManifestUnit unit;
    std::vector<uint8_t> manifestBytes;
    if (!FileUtils::ReadRegularFile(manifestPath, kMaxManifestBytes, manifestBytes)) {
        throw std::runtime_error("failed to read permitted manifest: " + manifestPath);
    }
    budget.AddPayload(manifestBytes.size(), "manifest " + manifestPath);
    unit.json.assign(manifestBytes.begin(), manifestBytes.end());
    const std::string baseDir = ReadBaseDir(unit.json, manifestPath);

    const std::string manifestRoot = FileUtils::ParentPath(manifestPath);
    const std::string resourceInput =
        !baseDir.empty() && baseDir[0] == '/' ? baseDir : FileUtils::JoinPath(manifestRoot, baseDir);
    std::string resourceRoot;
    if (!FileUtils::ResolveSubdirectory(resourceInput, manifestRoot, resourceRoot)) {
        throw std::runtime_error(
            "manifest base_dir must be a directory below the manifest root and must not be a symlink: " +
            resourceInput);
    }
    LoadResourceFiles(resourceRoot, manifestRoot, manifestPath, budget, unit, 0U);
    std::sort(unit.files.begin(), unit.files.end(), [](const ResourceFile& left, const ResourceFile& right) {
        return left.filePath < right.filePath;
    });
    ASCENDLOGD("Loaded manifest %s with %zu resource file(s)", manifestPath.c_str(), unit.files.size());
    return unit;
}

} // namespace

CollectedManifestRepository::CollectedManifestRepository(std::string manifestSearchRoot) noexcept
    : manifestSearchRoot_(std::move(manifestSearchRoot))
{}

std::vector<ManifestUnit> CollectedManifestRepository::Load() const
{
    ASCENDLOGD("Discovering manifests under %s", manifestSearchRoot_.c_str());
    ResourceBudget budget;
    const std::vector<std::string> manifests = DiscoverManifestFiles(manifestSearchRoot_);
    ASCENDLOGI("Discovered %zu manifest(s)", manifests.size());
    std::vector<ManifestUnit> units;
    units.reserve(manifests.size());
    for (const std::string& manifest : manifests) {
        units.push_back(LoadUnit(manifest, budget));
    }
    return units;
}

} // namespace manifest_generator
} // namespace ascendc
