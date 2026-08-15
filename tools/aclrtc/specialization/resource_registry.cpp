/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "resource_registry.h"

#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <set>
#include <utility>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>

#include "ascendc_manifest_abi.h"
#include "ascendc_tool_log.h"
#include "file_utils.h"
#include "nlohmann/json.hpp"

namespace ascendc {
namespace specialization_compile {

void LibraryDeleter::operator()(void* handle) const noexcept
{
    if (handle != nullptr && dlclose(handle) != 0) {
        ASCENDLOGW("Failed to close compile resource shared object");
    }
}

namespace {

namespace fs = boost::filesystem;
using Json = nlohmann::json;

constexpr char RELATIVE_JIT_ROOT[] = "op_impl/ai_core/tbe/kernel/jit";
constexpr char LIBRARY_NAME_SUFFIX[] = "_compile_database.so";
constexpr uint64_t MAX_MANIFEST_SIZE = 8U * 1024U * 1024U;
constexpr uint64_t MAX_PATH_SIZE = 4096U;
constexpr uint64_t MAX_RESOURCE_FILE_SIZE = 256U * 1024U * 1024U;
constexpr uint64_t MAX_MANIFEST_RESOURCE_SIZE = 512U * 1024U * 1024U;
constexpr uint64_t MAX_REGISTRY_RESOURCE_SIZE = 1024U * 1024U * 1024U;
constexpr uint64_t MAX_MANIFEST_COUNT = 4096U;
constexpr uint64_t MAX_FILE_COUNT = 65536U;
constexpr uint64_t MAX_REGISTRY_FILE_COUNT = 131072U;

const char* SourceTypeName(ResourceSourceType sourceType) noexcept
{
    switch (sourceType) {
        case ResourceSourceType::External:
            return "external";
        case ResourceSourceType::Custom:
            return "custom";
        case ResourceSourceType::BuiltIn:
            return "built-in";
    }
    return "unknown";
}

void LogResourceConflict(const std::string& resourceId, const ResourceEntry& incoming, const ResourceEntry& existing)
{
    ASCENDLOGE(
        "Duplicate resource_id in load transaction: resource_id=%s incoming_source_type=%s incoming_so=%s "
        "existing_source_type=%s existing_so=%s",
        resourceId.c_str(), SourceTypeName(incoming.sourceType), incoming.sourceSoPath.c_str(),
        SourceTypeName(existing.sourceType), existing.sourceSoPath.c_str());
}

void CleanupPath(const std::string& path) noexcept
{
    if (path.empty()) {
        return;
    }
    if (!FileUtils::RemoveAll(path)) {
        ASCENDLOGW("Failed to remove compile resource path: path=%s", path.c_str());
    }
}

ResourceStatus CreateTemporaryRoot(std::string& temporaryRoot)
{
    const char* environment = std::getenv("TMPDIR");
    const fs::path configured = environment == nullptr || *environment == '\0' ? "/tmp" : environment;
    boost::system::error_code error;
    const fs::path parent = fs::canonical(configured, error);
    if (error || !fs::is_directory(parent, error) || error) {
        ASCENDLOGE(
            "Compile resource temporary parent is unavailable: configured=%s error=%s", configured.c_str(),
            error.message().c_str());
        return ResourceStatus::IoError;
    }
    const std::string pattern = (parent / "aclrtc-resource-XXXXXX").string();
    std::vector<char> writable(pattern.begin(), pattern.end());
    writable.push_back('\0');
    char* created = mkdtemp(writable.data());
    if (created == nullptr) {
        ASCENDLOGE(
            "Failed to create compile resource temporary root: parent=%s error=%s", parent.c_str(),
            std::strerror(errno));
        return ResourceStatus::IoError;
    }
    const fs::path canonical = fs::canonical(created, error);
    if (error || canonical != fs::path(created).lexically_normal()) {
        ASCENDLOGE(
            "Compile resource temporary root identity is invalid: path=%s canonical=%s error=%s", created,
            canonical.c_str(), error.message().c_str());
        CleanupPath(created);
        return ResourceStatus::IoError;
    }
    temporaryRoot = canonical.string();
    return ResourceStatus::Success;
}

bool TryAddWithinLimit(uint64_t value, uint64_t limit, uint64_t& total) noexcept
{
    if (total > limit || value > limit - total) {
        return false;
    }
    total += value;
    return true;
}

ResourceStatus CopyString(const AcString& value, const char* field, uint64_t limit, std::string& output)
{
    if ((value.size != 0U && value.data == nullptr) || value.size > limit) {
        ASCENDLOGE("Invalid compile resource field: field=%s size=%" PRIu64, field, value.size);
        return ResourceStatus::InvalidResource;
    }
    output.assign(value.size == 0U ? "" : value.data, value.size);
    return ResourceStatus::Success;
}

bool IsPlainName(const std::string& value)
{
    return !value.empty() && FileUtils::IsSafeRelativePath(value) && FileUtils::FileName(value) == value;
}

std::string Trim(const std::string& value)
{
    const size_t begin = value.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) {
        return {};
    }
    return value.substr(begin, value.find_last_not_of(" \t\r\n") - begin + 1U);
}

std::vector<std::string> SplitList(const char* value, char delimiter)
{
    std::vector<std::string> result;
    const std::string text(value == nullptr ? "" : value);
    size_t begin = 0U;
    while (begin <= text.size()) {
        const size_t end = text.find(delimiter, begin);
        const std::string item = Trim(text.substr(begin, end - begin));
        if (!item.empty() && std::find(result.begin(), result.end(), item) == result.end()) {
            result.push_back(item);
        }
        if (end == std::string::npos) {
            break;
        }
        begin = end + 1U;
    }
    return result;
}

} // namespace

AutomaticRoots ResourceRegistry::AutomaticSearchRoots()
{
    AutomaticRoots roots;
    for (const std::string& path : SplitList(std::getenv("ASCEND_CUSTOM_OPP_PATH"), ':')) {
        roots.custom.push_back(FileUtils::JoinPath(path, RELATIVE_JIT_ROOT));
    }
    const char* environment = std::getenv("ASCEND_OPP_PATH");
    if (environment == nullptr || *environment == '\0') {
        return roots;
    }
    const std::string oppRoot(environment);
    std::ifstream input(FileUtils::JoinPath(oppRoot, "vendors/config.ini"));
    std::string line;
    while (std::getline(input, line)) {
        const size_t separator = line.find('=');
        if (separator == std::string::npos || Trim(line.substr(0U, separator)) != "load_priority") {
            continue;
        }
        for (const std::string& vendor : SplitList(line.substr(separator + 1U).c_str(), ',')) {
            if (IsPlainName(vendor)) {
                roots.custom.push_back(
                    FileUtils::JoinPath(FileUtils::JoinPath(oppRoot, "vendors/" + vendor), RELATIVE_JIT_ROOT));
            }
        }
        break;
    }
    roots.builtIn.push_back(FileUtils::JoinPath(FileUtils::JoinPath(oppRoot, "built-in"), RELATIVE_JIT_ROOT));
    return roots;
}

bool ResourceRegistry::IsLibraryName(const std::string& path)
{
    const std::string name = FileUtils::FileName(path);
    return name.size() > std::strlen("lib") + std::strlen(LIBRARY_NAME_SUFFIX) && name.rfind("lib", 0U) == 0U &&
           name.compare(
               name.size() - std::strlen(LIBRARY_NAME_SUFFIX), std::strlen(LIBRARY_NAME_SUFFIX), LIBRARY_NAME_SUFFIX) ==
               0;
}

void ResourceRegistry::AddLibrary(const std::string& path, std::set<std::string>& found)
{
    if (!IsLibraryName(path) || !FileUtils::IsRegularFile(path) || FileUtils::IsSymlink(path)) {
        return;
    }
    std::string canonical;
    if (FileUtils::ResolveCanonicalPath(path, canonical)) {
        found.insert(canonical);
    }
}

bool ResourceRegistry::CollectLibraries(
    const std::string& root, ResourceSourceType sourceType, bool recursive, std::set<std::string>& found)
{
    std::vector<std::string> pending = {root};
    boost::system::error_code error;
    while (!pending.empty()) {
        const std::string currentRoot = std::move(pending.back());
        pending.pop_back();
        fs::directory_iterator current(currentRoot, fs::directory_options::none, error);
        const fs::directory_iterator end;
        if (error) {
            ASCENDLOGE(
                "Failed to scan compile resource directory: source_type=%s path=%s error=%s",
                SourceTypeName(sourceType), currentRoot.c_str(), error.message().c_str());
            return false;
        }
        while (current != end) {
            const std::string path = current->path().string();
            AddLibrary(path, found);
            const fs::file_status status = recursive ? current->symlink_status(error) : fs::file_status();
            if (error) {
                ASCENDLOGE(
                    "Failed to inspect compile resource path: source_type=%s path=%s error=%s",
                    SourceTypeName(sourceType), path.c_str(), error.message().c_str());
                return false;
            }
            if (recursive && fs::is_directory(status)) {
                pending.push_back(path);
            }
            current.increment(error);
            if (error) {
                ASCENDLOGE(
                    "Failed to scan compile resource directory: source_type=%s path=%s error=%s",
                    SourceTypeName(sourceType), currentRoot.c_str(), error.message().c_str());
                return false;
            }
        }
    }
    return true;
}

bool ResourceRegistry::CollectAutomaticLibraries(
    const std::vector<std::string>& roots, ResourceSourceType sourceType, std::set<std::string>& found)
{
    for (const std::string& root : roots) {
        if (!FileUtils::IsDirectory(root)) {
            ASCENDLOGD(
                "Skipping missing automatic compile resource root: source_type=%s path=%s", SourceTypeName(sourceType),
                root.c_str());
            continue;
        }
        if (!CollectLibraries(root, sourceType, true, found)) {
            return false;
        }
    }
    return true;
}

ResourceStatus ResourceRegistry::DiscoverLibraries(const char* directory, std::vector<LibrarySpec>& libraries)
{
    const bool automatic = directory == nullptr || *directory == '\0';
    if (!automatic) {
        if (FileUtils::IsRegularFile(directory)) {
            std::string canonical;
            if (FileUtils::IsSymlink(directory) || !FileUtils::ResolveCanonicalPath(directory, canonical)) {
                ASCENDLOGE("Invalid explicit compile resource SO: path=%s", directory);
                return ResourceStatus::InvalidResource;
            }
            libraries.push_back({canonical, ResourceSourceType::External});
            return ResourceStatus::Success;
        }
        if (!FileUtils::IsDirectory(directory) || FileUtils::IsSymlink(directory)) {
            ASCENDLOGE("Invalid explicit compile resource path: path=%s", directory);
            return ResourceStatus::InvalidResource;
        }
        std::set<std::string> found;
        if (!CollectLibraries(directory, ResourceSourceType::External, false, found)) {
            return ResourceStatus::IoError;
        }
        for (const std::string& path : found) {
            libraries.push_back({path, ResourceSourceType::External});
        }
        return libraries.empty() ? ResourceStatus::NotFound : ResourceStatus::Success;
    }
    const AutomaticRoots roots = AutomaticSearchRoots();
    std::set<std::string> custom;
    std::set<std::string> builtIn;
    if (!CollectAutomaticLibraries(roots.custom, ResourceSourceType::Custom, custom) ||
        !CollectAutomaticLibraries(roots.builtIn, ResourceSourceType::BuiltIn, builtIn)) {
        return ResourceStatus::IoError;
    }
    for (const std::string& path : custom) {
        libraries.push_back({path, ResourceSourceType::Custom});
    }
    for (const std::string& path : builtIn) {
        libraries.push_back({path, ResourceSourceType::BuiltIn});
    }
    return libraries.empty() ? ResourceStatus::NotFound : ResourceStatus::Success;
}

ResourceStatus ResourceRegistry::GetBundle(
    const LibraryHandle& library, const LibrarySpec& spec, const AcCompileResourceBundle*& bundle)
{
    dlerror();
    const auto getter =
        reinterpret_cast<AscendcGetCompileResourceBundleFn>(dlsym(library.get(), "AscendcGetCompileResourceBundle"));
    if (dlerror() != nullptr || getter == nullptr) {
        ASCENDLOGE(
            "Compile resource SO has no bundle entry point: source_type=%s so=%s", SourceTypeName(spec.sourceType),
            spec.path.c_str());
        return ResourceStatus::LoadError;
    }
    const AcCompileResourceBundleHeader* header = getter();
    if (header == nullptr || header->magic != AC_COMPILE_RESOURCE_MAGIC ||
        header->abiVersion != AC_COMPILE_RESOURCE_ABI_VERSION ||
        header->structSize != sizeof(AcCompileResourceBundle) || header->flags != 0U) {
        ASCENDLOGE(
            "Invalid compile resource bundle header: source_type=%s so=%s", SourceTypeName(spec.sourceType),
            spec.path.c_str());
        return ResourceStatus::InvalidResource;
    }
    bundle = reinterpret_cast<const AcCompileResourceBundle*>(header);
    if (bundle->manifestCount == 0U || bundle->manifestCount > MAX_MANIFEST_COUNT || bundle->manifests == nullptr) {
        ASCENDLOGE(
            "Invalid compile resource manifest table: so=%s count=%" PRIu64, spec.path.c_str(), bundle->manifestCount);
        return ResourceStatus::InvalidResource;
    }
    return ResourceStatus::Success;
}

ResourceStatus ResourceRegistry::ResolveSourceRoot(
    const fs::path& searchRoot, const std::string& resourceId, std::string& canonicalRoot)
{
    boost::system::error_code error;
    const fs::file_status status = fs::symlink_status(searchRoot, error);
    if (error) {
        ASCENDLOGE(
            "Failed to inspect compile source root: resource_id=%s root=%s error=%s", resourceId.c_str(),
            searchRoot.c_str(), error.message().c_str());
        return error == boost::system::errc::no_such_file_or_directory ? ResourceStatus::InvalidResource :
                                                                         ResourceStatus::IoError;
    }
    if (fs::is_symlink(status) || !fs::is_directory(status)) {
        ASCENDLOGE(
            "Compile source root must be a non-symlink directory: resource_id=%s root=%s", resourceId.c_str(),
            searchRoot.c_str());
        return ResourceStatus::InvalidResource;
    }
    const fs::path canonical = fs::canonical(searchRoot, error);
    if (error) {
        ASCENDLOGE(
            "Failed to resolve compile source root: resource_id=%s root=%s error=%s", resourceId.c_str(),
            searchRoot.c_str(), error.message().c_str());
        return ResourceStatus::IoError;
    }
    canonicalRoot = canonical.string();
    return ResourceStatus::Success;
}

ResourceStatus ResourceRegistry::AddSourceMatch(
    const fs::path& candidate, const std::string& sourceFile, const std::string& canonicalRoot,
    const std::string& resourceId, std::set<std::string>& matches)
{
    if (candidate.filename() != sourceFile) {
        return ResourceStatus::Success;
    }
    boost::system::error_code error;
    const fs::file_status status = fs::symlink_status(candidate, error);
    if (error) {
        ASCENDLOGE(
            "Failed to inspect compile source candidate: resource_id=%s path=%s error=%s", resourceId.c_str(),
            candidate.c_str(), error.message().c_str());
        return ResourceStatus::IoError;
    }
    if (!fs::is_regular_file(status)) {
        return ResourceStatus::Success;
    }
    const fs::path canonical = fs::canonical(candidate, error);
    if (error) {
        ASCENDLOGE(
            "Failed to resolve compile source candidate: resource_id=%s path=%s error=%s", resourceId.c_str(),
            candidate.c_str(), error.message().c_str());
        return ResourceStatus::IoError;
    }
    if (!FileUtils::IsPathWithin(canonical.string(), canonicalRoot)) {
        ASCENDLOGE(
            "Compile source candidate escaped root: resource_id=%s candidate=%s canonical=%s root=%s",
            resourceId.c_str(), candidate.c_str(), canonical.c_str(), canonicalRoot.c_str());
        return ResourceStatus::InvalidResource;
    }
    matches.insert(canonical.parent_path().string());
    return ResourceStatus::Success;
}

ResourceStatus ResourceRegistry::LocateSourceFile(
    const LibrarySpec& spec, const std::string& resourceId, const std::string& sourceFile, std::string& located)
{
    // Relative path example:
    //   kernel/jit/ascend910b/libfoo_compile_database.so  <- resource SO
    //   kernel/ascend910b/impl/foo.cpp                    <- source file
    // Derive the SoC and kernel root from the SO path, then recursively search the sibling kernel/<soc> tree.
    // Exactly one regular-file match is required; located is set to that file's parent directory.
    const fs::path soPath(spec.path);
    const fs::path soc = soPath.parent_path().filename();
    const fs::path jitRoot = soPath.parent_path().parent_path();
    const fs::path kernelRoot = jitRoot.parent_path();
    if (soc.empty() || jitRoot.filename() != "jit" || kernelRoot.filename() != "kernel") {
        ASCENDLOGE(
            "Compile resource SO is outside kernel/jit/<soc>: resource_id=%s source_type=%s so=%s", resourceId.c_str(),
            SourceTypeName(spec.sourceType), spec.path.c_str());
        return ResourceStatus::InvalidResource;
    }
    const fs::path searchRoot = kernelRoot / soc;
    std::string canonicalRoot;
    ResourceStatus status = ResolveSourceRoot(searchRoot, resourceId, canonicalRoot);
    if (status != ResourceStatus::Success) {
        return status;
    }
    std::set<std::string> matches;
    boost::system::error_code error;
    fs::recursive_directory_iterator current(canonicalRoot, fs::directory_options::none, error);
    const fs::recursive_directory_iterator end;
    while (!error && current != end) {
        status = AddSourceMatch(current->path(), sourceFile, canonicalRoot, resourceId, matches);
        if (status != ResourceStatus::Success) {
            return status;
        }
        current.increment(error);
    }
    if (error) {
        ASCENDLOGE(
            "Failed to search compile source: resource_id=%s root=%s error=%s", resourceId.c_str(), searchRoot.c_str(),
            error.message().c_str());
        return ResourceStatus::IoError;
    }
    if (matches.size() != 1U) {
        ASCENDLOGE(
            "Compile source match count is invalid: resource_id=%s root=%s source_file=%s matches=%zu",
            resourceId.c_str(), searchRoot.c_str(), sourceFile.c_str(), matches.size());
        return ResourceStatus::InvalidResource;
    }
    located = *matches.begin();
    return ResourceStatus::Success;
}

bool ResourceRegistry::FindPathConflict(const std::set<std::string>& paths)
{
    for (const std::string& path : paths) {
        size_t separator = path.find('/');
        while (separator != std::string::npos) {
            if (paths.count(path.substr(0U, separator)) != 0U) {
                ASCENDLOGE("Resource file path conflicts with child: child=%s", path.c_str());
                return true;
            }
            separator = path.find('/', separator + 1U);
        }
    }
    return false;
}

ResourceStatus ResourceRegistry::AppendFile(
    const AcCompileResourceFile& file, const std::string& resourceId, ResourceEntry& entry,
    std::set<std::string>& paths, uint64_t& manifestBytes, StagedResources& staged)
{
    std::string fileName;
    std::string relativePath;
    ResourceStatus status = CopyString(file.fileName, "resource.fileName", MAX_PATH_SIZE, fileName);
    if (status == ResourceStatus::Success) {
        status = CopyString(file.filePath, "resource.filePath", MAX_PATH_SIZE, relativePath);
    }
    if (status != ResourceStatus::Success || !IsPlainName(fileName) || !FileUtils::IsSafeRelativePath(relativePath) ||
        FileUtils::FileName(relativePath) != fileName || !paths.insert(relativePath).second ||
        file.size > MAX_RESOURCE_FILE_SIZE || (file.size != 0U && file.data == nullptr) ||
        !TryAddWithinLimit(file.size, MAX_MANIFEST_RESOURCE_SIZE, manifestBytes) ||
        !TryAddWithinLimit(file.size, MAX_REGISTRY_RESOURCE_SIZE, staged.bytes)) {
        ASCENDLOGE(
            "Invalid bundled resource file: resource_id=%s path=%s size=%" PRIu64, resourceId.c_str(),
            relativePath.c_str(), file.size);
        return ResourceStatus::InvalidResource;
    }
    std::vector<uint8_t> bytes;
    if (file.size != 0U) {
        bytes.assign(file.data, file.data + file.size);
    }
    entry.files.push_back({std::move(fileName), std::move(relativePath), std::move(bytes)});
    return ResourceStatus::Success;
}

ResourceStatus ResourceRegistry::CopyManifestFiles(
    const AcCompileResourceManifest& unit, const std::string& resourceId, ResourceEntry& entry, StagedResources& staged)
{
    if (unit.fileCount > MAX_FILE_COUNT || (unit.fileCount == 0U) != (unit.files == nullptr) ||
        staged.files > MAX_REGISTRY_FILE_COUNT || unit.fileCount > MAX_REGISTRY_FILE_COUNT - staged.files) {
        ASCENDLOGE(
            "Invalid bundle resource file table: resource_id=%s files=%" PRIu64, resourceId.c_str(), unit.fileCount);
        return ResourceStatus::InvalidResource;
    }
    entry.files.reserve(static_cast<size_t>(unit.fileCount));
    std::set<std::string> paths;
    uint64_t manifestBytes = 0U;
    for (uint64_t index = 0U; index < unit.fileCount; ++index) {
        const ResourceStatus status = AppendFile(unit.files[index], resourceId, entry, paths, manifestBytes, staged);
        if (status != ResourceStatus::Success) {
            return status;
        }
    }
    if (FindPathConflict(paths)) {
        return ResourceStatus::InvalidResource;
    }
    staged.files += unit.fileCount;
    std::sort(entry.files.begin(), entry.files.end(), [](const ResourceFileData& left, const ResourceFileData& right) {
        return left.relativePath < right.relativePath;
    });
    return ResourceStatus::Success;
}

StagedResources& ResourceRegistry::SelectStagedResources(StageState& stage, ResourceSourceType sourceType)
{
    if (sourceType == ResourceSourceType::Custom) {
        return stage.custom;
    }
    return sourceType == ResourceSourceType::BuiltIn ? stage.builtIn : stage.external;
}

ResourceStatus ResourceRegistry::LoadManifest(
    const AcCompileResourceManifest& unit, const LibrarySpec& spec, uint64_t manifestIndex, StageState& stage)
{
    ASCENDLOGI(
        "Loading compile resource manifest: source_type=%s so=%s manifest=%" PRIu64, SourceTypeName(spec.sourceType),
        spec.path.c_str(), manifestIndex);
    std::string manifestText;
    ResourceStatus status = CopyString(unit.json, "manifest.json", MAX_MANIFEST_SIZE, manifestText);
    Json document;
    std::string resourceId;
    std::string sourceFile;
    bool hasSourceFile = false;
    if (status == ResourceStatus::Success) {
        document = Json::parse(manifestText, nullptr, false);
        if (document.is_discarded() || !document.is_object() || !document.contains("resource_id") ||
            !document.at("resource_id").is_string()) {
            ASCENDLOGE("Compile resource manifest has invalid ownership fields");
            status = ResourceStatus::InvalidResource;
        }
    }
    if (status == ResourceStatus::Success) {
        resourceId = document.at("resource_id").get<std::string>();
        hasSourceFile = document.contains("source_file");
        if (hasSourceFile && !document.at("source_file").is_string()) {
            ASCENDLOGE(
                "Compile resource manifest field has invalid type: field=source_file expected=string actual=%s",
                document.at("source_file").type_name());
            status = ResourceStatus::InvalidResource;
        } else if (hasSourceFile) {
            sourceFile = document.at("source_file").get<std::string>();
        }
        if (status == ResourceStatus::Success &&
            (!IsPlainName(resourceId) || (hasSourceFile && !IsPlainName(sourceFile)))) {
            ASCENDLOGE(
                "Compile resource manifest has unsafe ownership fields: resource_id=%s source_file=%s",
                resourceId.c_str(), sourceFile.c_str());
            status = ResourceStatus::InvalidResource;
        }
    }
    std::unique_ptr<ResourceEntry> entry(new ResourceEntry());
    if (status == ResourceStatus::Success && hasSourceFile) {
        status = LocateSourceFile(spec, resourceId, sourceFile, entry->data.sourceFilePath);
    }
    StagedResources& staged = SelectStagedResources(stage, spec.sourceType);
    if (status == ResourceStatus::Success) {
        status = CopyManifestFiles(unit, resourceId, *entry, staged);
    }
    if (status != ResourceStatus::Success) {
        return status;
    }
    entry->data.json = std::move(document);
    entry->sourceSoPath = spec.path;
    entry->sourceType = spec.sourceType;
    const auto existing = staged.resources.find(resourceId);
    if (existing != staged.resources.end()) {
        LogResourceConflict(resourceId, *entry, *existing->second);
        return ResourceStatus::Conflict;
    }
    staged.resources.emplace(resourceId, std::move(entry));
    return ResourceStatus::Success;
}

ResourceStatus ResourceRegistry::LoadLibrary(const LibrarySpec& spec, StageState& stage)
{
    ASCENDLOGI("Loading compile resource SO: source_type=%s so=%s", SourceTypeName(spec.sourceType), spec.path.c_str());
    LibraryHandle library(dlopen(spec.path.c_str(), RTLD_NOW | RTLD_LOCAL));
    if (!library) {
        const char* error = dlerror();
        ASCENDLOGE(
            "Failed to open compile resource SO: source_type=%s so=%s error=%s", SourceTypeName(spec.sourceType),
            spec.path.c_str(), error == nullptr ? "unknown" : error);
        return ResourceStatus::LoadError;
    }
    const AcCompileResourceBundle* bundle = nullptr;
    ResourceStatus status = GetBundle(library, spec, bundle);
    if (status != ResourceStatus::Success) {
        return status;
    }
    for (uint64_t manifestIndex = 0U; manifestIndex < bundle->manifestCount; ++manifestIndex) {
        status = LoadManifest(bundle->manifests[manifestIndex], spec, manifestIndex, stage);
        if (status != ResourceStatus::Success) {
            ASCENDLOGE(
                "Failed to load compile resource manifest: source_type=%s so=%s manifest=%" PRIu64 " status=%d",
                SourceTypeName(spec.sourceType), spec.path.c_str(), manifestIndex, static_cast<int>(status));
            return status;
        }
    }
    ASCENDLOGI(
        "Loaded compile resource SO: source_type=%s so=%s manifests=%" PRIu64, SourceTypeName(spec.sourceType),
        spec.path.c_str(), bundle->manifestCount);
    return ResourceStatus::Success;
}

ResourceStatus ResourceRegistry::LoadLibraries(const std::vector<LibrarySpec>& libraries, StageState& stage)
{
    ASCENDLOGI("Loading compile resource SO list: count=%zu", libraries.size());
    for (const LibrarySpec& library : libraries) {
        SelectStagedResources(stage, library.sourceType).discovered = true;
        const ResourceStatus status = LoadLibrary(library, stage);
        if (status != ResourceStatus::Success) {
            ASCENDLOGE(
                "Stopped loading compile resource SO list: source_type=%s so=%s status=%d",
                SourceTypeName(library.sourceType), library.path.c_str(), static_cast<int>(status));
            return status;
        }
    }
    ASCENDLOGI("Loaded compile resource SO list: count=%zu", libraries.size());
    return ResourceStatus::Success;
}

ResourceStatus ResourceRegistry::WriteMaterializedFiles(
    const std::vector<ResourceFileData>& files, const std::string& root)
{
    for (const ResourceFileData& file : files) {
        const std::string path = FileUtils::JoinPath(root, file.relativePath);
        if (!FileUtils::CreateDirectories(FileUtils::ParentPath(path))) {
            return ResourceStatus::IoError;
        }
        std::ofstream output(path.c_str(), std::ios::binary | std::ios::trunc);
        if (!output.is_open()) {
            return ResourceStatus::IoError;
        }
        if (!file.bytes.empty()) {
            output.write(
                reinterpret_cast<const char*>(file.bytes.data()), static_cast<std::streamsize>(file.bytes.size()));
        }
        if (!FileUtils::FinalizeOutput(output)) {
            return ResourceStatus::IoError;
        }
    }
    return ResourceStatus::Success;
}

ResourceRegistry& ResourceRegistry::Instance()
{
    static ResourceRegistry registry;
    return registry;
}

ResourceRegistry::ResourceRegistry() = default;

ResourceRegistry::~ResourceRegistry() { CleanupPath(temporaryRoot_); }

ResourceEntry* ResourceRegistry::FindResource(const std::string& resourceId) noexcept
{
    const auto external = externalResources_.find(resourceId);
    if (external != externalResources_.end()) {
        return external->second.get();
    }
    const auto custom = customResources_.find(resourceId);
    if (custom != customResources_.end()) {
        return custom->second.get();
    }
    const auto builtIn = builtInResources_.find(resourceId);
    return builtIn == builtInResources_.end() ? nullptr : builtIn->second.get();
}

bool ResourceRegistry::HasCommitConflict(const ResourceStore& incoming, const ResourceStore& committed)
{
    for (const auto& item : incoming) {
        const auto existing = committed.find(item.first);
        if (existing != committed.end()) {
            LogResourceConflict(item.first, *item.second, *existing->second);
            return true;
        }
    }
    return false;
}

ResourceStatus ResourceRegistry::Commit(StageState& stage)
{
    std::lock_guard<std::mutex> lock(registryMutex_);
    ResourceStatus status = ResourceStatus::Success;
    auto checkCategory = [&](StagedResources& staged, ResourceStore& committed, ResourceSourceType sourceType) {
        if (!staged.discovered && !staged.conflict) {
            return;
        }
        if (!staged.conflict && HasCommitConflict(staged.resources, committed)) {
            staged.conflict = true;
        }
        if (staged.conflict) {
            ASCENDLOGW(
                "Skipping compile resource category because conflicts were detected: source_type=%s resource_count=%zu",
                SourceTypeName(sourceType), staged.resources.size());
            status = ResourceStatus::Conflict;
            return;
        }
        committed.reserve(committed.size() + staged.resources.size());
    };
    checkCategory(stage.external, externalResources_, ResourceSourceType::External);
    checkCategory(stage.custom, customResources_, ResourceSourceType::Custom);
    checkCategory(stage.builtIn, builtInResources_, ResourceSourceType::BuiltIn);
    uint64_t bytes = resourceBytes_;
    uint64_t files = resourceFileCount_;
    for (const StagedResources* staged : {&stage.external, &stage.custom, &stage.builtIn}) {
        if (staged->discovered && !staged->conflict &&
            (!TryAddWithinLimit(staged->bytes, MAX_REGISTRY_RESOURCE_SIZE, bytes) ||
             !TryAddWithinLimit(staged->files, MAX_REGISTRY_FILE_COUNT, files))) {
            ASCENDLOGE(
                "Compile resource registry limit exceeded: bytes=%" PRIu64 " files=%" PRIu64, staged->bytes,
                staged->files);
            return ResourceStatus::InvalidResource;
        }
    }
    auto commitCategory = [&](StagedResources& staged, ResourceStore& committed) {
        if (!staged.discovered || staged.conflict) {
            return;
        }
        committed.merge(staged.resources);
    };
    commitCategory(stage.external, externalResources_);
    commitCategory(stage.custom, customResources_);
    commitCategory(stage.builtIn, builtInResources_);
    resourceBytes_ = bytes;
    resourceFileCount_ = files;
    return status;
}

ResourceStatus ResourceRegistry::Load(const char* directory)
{
    const bool automatic = directory == nullptr || *directory == '\0';
    std::lock_guard<std::mutex> loadLock(loadMutex_);
    if (automatic && automaticLoadAttempted_) {
        ASCENDLOGI(
            "Returning cached automatic resource load status: status=%d", static_cast<int>(automaticLoadStatus_));
        return automaticLoadStatus_;
    }
    ASCENDLOGI(
        "Loading compile resources: mode=%s directory=%s", automatic ? "automatic" : "explicit",
        automatic ? "<environment>" : directory);
    std::vector<LibrarySpec> libraries;
    ResourceStatus status = DiscoverLibraries(directory, libraries);
    if (status != ResourceStatus::Success) {
        ASCENDLOGE(
            "Failed to discover compile resource libraries: mode=%s status=%d", automatic ? "automatic" : "explicit",
            static_cast<int>(status));
    } else {
        StageState stage;
        status = LoadLibraries(libraries, stage);
        if (status == ResourceStatus::Success) {
            status = Commit(stage);
        }
    }
    if (automatic) {
        automaticLoadAttempted_ = true;
        automaticLoadStatus_ = status;
    }
    if (status == ResourceStatus::Success) {
        ASCENDLOGI(
            "Finished loading compile resources: mode=%s libraries=%zu status=%d", automatic ? "automatic" : "explicit",
            libraries.size(), static_cast<int>(status));
    }
    return status;
}

ResourceStatus ResourceRegistry::Materialize(const std::string& resourceId, ResourceEntry& entry)
{
    std::string temporaryRoot;
    {
        std::lock_guard<std::mutex> lock(registryMutex_);
        if (temporaryRoot_.empty()) {
            const ResourceStatus status = CreateTemporaryRoot(temporaryRoot_);
            if (status != ResourceStatus::Success) {
                return status;
            }
        }
        temporaryRoot = temporaryRoot_;
    }
    const std::string categoryParent = FileUtils::JoinPath(temporaryRoot, SourceTypeName(entry.sourceType));
    boost::system::error_code error;
    const fs::path randomComponent = fs::unique_path("materialize-%%%%%%%%%%%%%%%%", error);
    if (error || randomComponent.empty() || randomComponent.has_parent_path()) {
        ASCENDLOGE(
            "Failed to generate compile resource materialization path: source_type=%s error=%s",
            SourceTypeName(entry.sourceType), error.message().c_str());
        entry.data.resourceDir.clear();
        return ResourceStatus::IoError;
    }
    const std::string categoryRoot = FileUtils::JoinPath(categoryParent, randomComponent.string());
    const std::string resourceRoot = FileUtils::JoinPath(categoryRoot, resourceId);
    fs::path canonicalRoot;
    ResourceStatus status = ResourceStatus::IoError;
    if (FileUtils::CreateDirectories(resourceRoot)) {
        canonicalRoot = fs::canonical(resourceRoot, error);
        if (!error && canonicalRoot == fs::path(resourceRoot).lexically_normal()) {
            status = WriteMaterializedFiles(entry.files, canonicalRoot.string());
        }
    }
    if (status != ResourceStatus::Success) {
        CleanupPath(categoryRoot);
        entry.data.resourceDir.clear();
        return status;
    }
    entry.data.resourceDir = canonicalRoot.string();
    ASCENDLOGI(
        "Compile resource materialized: resource_id=%s source_type=%s so=%s files=%zu", resourceId.c_str(),
        SourceTypeName(entry.sourceType), entry.sourceSoPath.c_str(), entry.files.size());
    return ResourceStatus::Success;
}

ResourceStatus ResourceRegistry::Lookup(const char* resourceId, const ResourceData*& resource)
{
    resource = nullptr;
    if (resourceId == nullptr || *resourceId == '\0') {
        return ResourceStatus::InvalidResource;
    }
    ResourceEntry* entry = nullptr;
    {
        std::lock_guard<std::mutex> lock(registryMutex_);
        entry = FindResource(resourceId);
    }
    if (entry == nullptr) {
        ASCENDLOGD("Compile resource lookup missed: resource_id=%s", resourceId);
        return ResourceStatus::NotFound;
    }
    std::lock_guard<std::mutex> materializeLock(entry->materializeMutex);
    const ResourceStatus status = Materialize(resourceId, *entry);
    if (status == ResourceStatus::Success) {
        resource = &entry->data;
    }
    return status;
}

} // namespace specialization_compile
} // namespace ascendc
