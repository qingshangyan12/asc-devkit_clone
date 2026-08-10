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
#include <cstdint>
#include <cstring>
#include <dirent.h>
#include <memory>
#include <utility>

#include <sys/stat.h>

#include "ascendc_tool_log.h"
#include "file_utils.h"
#include "nlohmann/json.hpp"

using Json = nlohmann::json;

namespace ascendc {
namespace manifest_generator {
namespace {

constexpr uint32_t MAX_MANIFEST_BYTES = 8U * 1024U * 1024U;
constexpr uint32_t MAX_RESOURCE_FILE_BYTES = 1U * 1024U * 1024U;
constexpr uint32_t MAX_BUNDLE_BYTES = 256U * 1024U * 1024U;
constexpr size_t MAX_MANIFEST_COUNT = 4096U;
constexpr size_t MAX_FILES_PER_MANIFEST = 4096U;
constexpr size_t MAX_FILES_PER_BUNDLE = 65536U;
constexpr size_t MAX_DIRECTORY_DEPTH = 64U;

struct DirectoryCloser {
    void operator()(DIR* directory) const noexcept
    {
        if (directory != nullptr) {
            (void)closedir(directory);
        }
    }
};

using DirectoryHandle = std::unique_ptr<DIR, DirectoryCloser>;

class ResourceBudget final {
public:
    bool AddResource(uint32_t size, const std::string& field)
    {
        if (fileCount_ >= MAX_FILES_PER_BUNDLE) {
            ASCENDLOGE("Bundle exceeds resource file count limit of %zu", MAX_FILES_PER_BUNDLE);
            return false;
        }
        if (!AddPayload(size, field)) {
            return false;
        }
        ++fileCount_;
        return true;
    }

    bool AddPayload(uint32_t size, const std::string& field)
    {
        if (size > MAX_BUNDLE_BYTES - totalBytes_) {
            ASCENDLOGE("Bundle exceeds raw payload limit while adding %s", field.c_str());
            return false;
        }
        totalBytes_ += size;
        return true;
    }

private:
    uint32_t totalBytes_{0U};
    size_t fileCount_{0U};
};

bool ReadBaseDir(const std::string& text, const std::string& manifestPath, std::string& baseDir)
{
    const Json manifest = Json::parse(text, nullptr, false);
    if (manifest.is_discarded()) {
        ASCENDLOGE("Manifest is not valid JSON: %s", manifestPath.c_str());
        return false;
    }

    const Json::const_iterator baseDirValue = manifest.find("base_dir");
    if (baseDirValue == manifest.end() || !baseDirValue->is_string()) {
        ASCENDLOGE("Manifest base_dir is unavailable: %s", manifestPath.c_str());
        return false;
    }
    baseDir = baseDirValue->get<std::string>();
    return true;
}

bool IsManifestFile(const std::string& path)
{
    constexpr const char* suffix = "_manifest.json";
    const std::string name = FileUtils::FileName(path);
    return name.size() > std::strlen(suffix) &&
           name.compare(name.size() - std::strlen(suffix), std::strlen(suffix), suffix) == 0;
}

bool DiscoverManifestFilesBelow(const std::string& directory, std::vector<std::string>& manifests, size_t depth)
{
    DirectoryHandle stream(opendir(directory.c_str()));
    if (!stream) {
        const int openError = errno;
        ASCENDLOGE("Failed to enumerate %s: %s", directory.c_str(), std::strerror(openError));
        return false;
    }
    while (dirent* entry = readdir(stream.get())) {
        const std::string name(entry->d_name);
        if (name == "." || name == "..") {
            continue;
        }
        const std::string path = FileUtils::JoinPath(directory, name);
        struct stat info {};
        if (lstat(path.c_str(), &info) != 0) {
            const int inspectError = errno;
            ASCENDLOGE("Failed to inspect collection entry %s: %s", path.c_str(), std::strerror(inspectError));
            return false;
        }
        if (S_ISLNK(info.st_mode)) {
            continue;
        }
        if (S_ISDIR(info.st_mode)) {
            if (depth >= MAX_DIRECTORY_DEPTH) {
                ASCENDLOGE("Collection exceeds directory depth limit of %zu: %s", MAX_DIRECTORY_DEPTH, path.c_str());
                return false;
            }
            if (!DiscoverManifestFilesBelow(path, manifests, depth + 1U)) {
                return false;
            }
            continue;
        }
        if (!S_ISREG(info.st_mode) || !IsManifestFile(path)) {
            continue;
        }
        if (manifests.size() >= MAX_MANIFEST_COUNT) {
            ASCENDLOGE("Collection exceeds manifest count limit of %zu", MAX_MANIFEST_COUNT);
            return false;
        }
        manifests.push_back(path);
    }
    return true;
}

bool DiscoverManifestFiles(const std::string& manifestSearchRoot, std::vector<std::string>& manifests)
{
    if (!DiscoverManifestFilesBelow(manifestSearchRoot, manifests, 0U)) {
        return false;
    }
    std::sort(manifests.begin(), manifests.end());
    if (manifests.empty()) {
        ASCENDLOGE("Collected resources contain no manifest units");
        return false;
    }
    return true;
}

bool LoadResourceFiles(
    const std::string& directory, const std::string& manifestRoot, const std::string& manifestPath,
    ResourceBudget& budget, ManifestUnit& unit, size_t depth)
{
    DirectoryHandle stream(opendir(directory.c_str()));
    if (!stream) {
        const int openError = errno;
        ASCENDLOGE("Failed to enumerate manifest base_dir %s: %s", directory.c_str(), std::strerror(openError));
        return false;
    }
    while (dirent* entry = readdir(stream.get())) {
        const std::string name(entry->d_name);
        if (name == "." || name == "..") {
            continue;
        }
        const std::string source = FileUtils::JoinPath(directory, name);
        struct stat info {};
        if (lstat(source.c_str(), &info) != 0) {
            const int inspectError = errno;
            ASCENDLOGE("Failed to inspect manifest resource %s: %s", source.c_str(), std::strerror(inspectError));
            return false;
        }
        if (S_ISLNK(info.st_mode)) {
            ASCENDLOGE("Manifest base_dir contains a symlink: %s", source.c_str());
            return false;
        }
        if (S_ISDIR(info.st_mode)) {
            if (depth >= MAX_DIRECTORY_DEPTH) {
                ASCENDLOGE(
                    "Manifest resources exceed directory depth limit of %zu: %s", MAX_DIRECTORY_DEPTH, source.c_str());
                return false;
            }
            if (!LoadResourceFiles(source, manifestRoot, manifestPath, budget, unit, depth + 1U)) {
                return false;
            }
            continue;
        }
        if (!S_ISREG(info.st_mode)) {
            ASCENDLOGE("Manifest base_dir contains a non-regular file: %s", source.c_str());
            return false;
        }
        std::vector<uint8_t> data;
        if (!FileUtils::ReadRegularFile(source, MAX_RESOURCE_FILE_BYTES, data)) {
            ASCENDLOGE("Failed to read permitted manifest resource: %s", source.c_str());
            return false;
        }
        if (data.empty()) {
            ASCENDLOGD("Skipping empty manifest resource %s", source.c_str());
            continue;
        }
        if (unit.files.size() >= MAX_FILES_PER_MANIFEST) {
            ASCENDLOGE("Manifest exceeds resource file count limit: %s", manifestPath.c_str());
            return false;
        }
        if (!budget.AddResource(static_cast<uint32_t>(data.size()), "manifest resource " + source)) {
            return false;
        }
        ResourceFile file;
        file.fileName = name;
        file.filePath = source.substr(manifestRoot.size() + 1U);
        file.data = std::move(data);
        unit.files.push_back(std::move(file));
    }
    return true;
}

bool LoadUnit(const std::string& manifestPath, ResourceBudget& budget, ManifestUnit& unit)
{
    ASCENDLOGD("Loading manifest %s", manifestPath.c_str());
    std::vector<uint8_t> manifestBytes;
    if (!FileUtils::ReadRegularFile(manifestPath, MAX_MANIFEST_BYTES, manifestBytes)) {
        ASCENDLOGE("Failed to read permitted manifest: %s", manifestPath.c_str());
        return false;
    }
    if (!budget.AddPayload(static_cast<uint32_t>(manifestBytes.size()), "manifest " + manifestPath)) {
        return false;
    }
    unit.json.assign(manifestBytes.begin(), manifestBytes.end());
    std::string baseDir;
    if (!ReadBaseDir(unit.json, manifestPath, baseDir)) {
        return false;
    }

    const std::string manifestRoot = FileUtils::ParentPath(manifestPath);
    const std::string resourceInput =
        !baseDir.empty() && baseDir[0] == '/' ? baseDir : FileUtils::JoinPath(manifestRoot, baseDir);
    std::string resourceRoot;
    if (!FileUtils::ResolveSubdirectory(resourceInput, manifestRoot, resourceRoot)) {
        ASCENDLOGE(
            "Manifest base_dir must be a directory below the manifest root and must not be a symlink: %s",
            resourceInput.c_str());
        return false;
    }
    if (!LoadResourceFiles(resourceRoot, manifestRoot, manifestPath, budget, unit, 0U)) {
        return false;
    }
    std::sort(unit.files.begin(), unit.files.end(), [](const ResourceFile& left, const ResourceFile& right) {
        return left.filePath < right.filePath;
    });
    ASCENDLOGD("Loaded manifest %s with %zu resource file(s)", manifestPath.c_str(), unit.files.size());
    return true;
}

} // namespace

CollectedManifestRepository::CollectedManifestRepository(std::string manifestSearchRoot) noexcept
    : manifestSearchRoot_(std::move(manifestSearchRoot))
{}

bool CollectedManifestRepository::Load(std::vector<ManifestUnit>& units) const
{
    ASCENDLOGD("Discovering manifests under %s", manifestSearchRoot_.c_str());
    ResourceBudget budget;
    std::vector<std::string> manifests;
    if (!DiscoverManifestFiles(manifestSearchRoot_, manifests)) {
        return false;
    }
    ASCENDLOGI("Discovered %zu manifest(s)", manifests.size());
    units.clear();
    units.reserve(manifests.size());
    for (const std::string& manifest : manifests) {
        ManifestUnit unit;
        if (!LoadUnit(manifest, budget, unit)) {
            return false;
        }
        units.push_back(std::move(unit));
    }
    return true;
}

} // namespace manifest_generator
} // namespace ascendc
