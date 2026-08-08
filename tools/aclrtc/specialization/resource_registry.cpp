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
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <exception>
#include <fstream>
#include <memory>
#include <mutex>
#include <new>
#include <set>
#include <string>
#include <vector>

#include "ascendc_manifest_abi.h"
#include "ascendc_tool_log.h"
#include "file_utils.h"
#include "resource_manifest.h"

namespace ascendc {
namespace resource {
namespace {

void CloseLibrary(void* library) noexcept
{
    if (library != nullptr && dlclose(library) != 0) {
        ASCENDLOGW("Failed to close compile resource shared object");
    }
}

struct LibraryDeleter {
    void operator()(void* library) const noexcept { CloseLibrary(library); }
};

using LibraryHandle = std::unique_ptr<void, LibraryDeleter>;

bool TryAddWithinLimit(uint64_t value, uint64_t limit, uint64_t& total) noexcept
{
    if (total > limit || value > limit - total) {
        return false;
    }
    total += value;
    return true;
}

bool FindResourcePathConflict(const std::set<std::string>& paths, std::string& parentPath, std::string& childPath)
{
    for (const std::string& path : paths) {
        size_t separator = path.find('/');
        while (separator != std::string::npos) {
            const auto parent = paths.find(path.substr(0U, separator));
            if (parent != paths.end()) {
                parentPath = *parent;
                childPath = path;
                return true;
            }
            separator = path.find('/', separator + 1U);
        }
    }
    return false;
}

ResourceStatus CopyString(const AcString& value, const char* field, uint64_t maxSize, std::string& output)
{
    if (value.size != 0U && value.data == nullptr) {
        ASCENDLOGE("Invalid compile resource field: field=%s, data is null, size=%" PRIu64, field, value.size);
        return ResourceStatus::InvalidResource;
    }
    if (value.size > maxSize) {
        ASCENDLOGE(
            "Compile resource field exceeds size limit: field=%s, size=%" PRIu64 ", limit=%" PRIu64, field, value.size,
            maxSize);
        return ResourceStatus::InvalidResource;
    }
    output.assign(value.size == 0U ? "" : value.data, value.size);
    return ResourceStatus::Success;
}

std::string ReplaceResourceRoot(std::string value, const std::string& materializedRoot)
{
    constexpr const char* marker = "${resource}";
    size_t begin = 0U;
    while ((begin = value.find(marker, begin)) != std::string::npos) {
        value.replace(begin, std::strlen(marker), materializedRoot);
        begin += materializedRoot.size();
    }
    return value;
}

std::string ReferencedResourcePath(const std::string& value, const std::string& materializedRoot)
{
    constexpr const char* marker = "${resource}";
    const size_t begin = value.find(marker);
    if (begin == std::string::npos) {
        return std::string();
    }
    const size_t pathBegin = begin + std::strlen(marker) + 1U;
    return FileUtils::JoinPath(materializedRoot, value.substr(pathBegin));
}

bool IsResourceSoName(const std::string& path)
{
    const std::string name = FileUtils::FileName(path);
    return name.size() > std::strlen("lib_compile_database.so") && name.rfind("lib", 0U) == 0U &&
           name.compare(
               name.size() - std::strlen("_compile_database.so"), std::strlen("_compile_database.so"),
               "_compile_database.so") == 0;
}

std::string Trim(const std::string& value)
{
    const size_t begin = value.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) {
        return {};
    }
    const size_t end = value.find_last_not_of(" \t\r\n");
    return value.substr(begin, end - begin + 1U);
}

std::vector<std::string> SplitList(const char* value, char delimiter)
{
    std::vector<std::string> result;
    if (value == nullptr) {
        return result;
    }
    const std::string text(value);
    size_t begin = 0U;
    while (begin <= text.size()) {
        const size_t end = text.find(delimiter, begin);
        const std::string item = Trim(text.substr(begin, end == std::string::npos ? std::string::npos : end - begin));
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

std::vector<std::string> ReadConfiguredVendors(const std::string& oppRoot)
{
    std::ifstream input(FileUtils::JoinPath(oppRoot, "vendors/config.ini"));
    std::string line;
    while (std::getline(input, line)) {
        const size_t separator = line.find('=');
        if (separator == std::string::npos || Trim(line.substr(0U, separator)) != "load_priority") {
            continue;
        }
        return SplitList(line.substr(separator + 1U).c_str(), ',');
    }
    return {};
}

std::vector<std::string> AutomaticSearchRoots()
{
    constexpr const char* relativeJitRoot = "op_impl/ai_core/tbe/kernel/jit";
    std::vector<std::string> roots;
    for (const std::string& customRoot : SplitList(std::getenv("ASCEND_CUSTOM_OPP_PATH"), ':')) {
        roots.push_back(FileUtils::JoinPath(customRoot, relativeJitRoot));
    }
    const char* oppEnvironment = std::getenv("ASCEND_OPP_PATH");
    if (oppEnvironment != nullptr && *oppEnvironment != '\0') {
        const std::string oppRoot(oppEnvironment);
        for (const std::string& vendor : ReadConfiguredVendors(oppRoot)) {
            if (FileUtils::FileName(vendor) == vendor && vendor != "." && vendor != "..") {
                roots.push_back(
                    FileUtils::JoinPath(
                        FileUtils::JoinPath(FileUtils::JoinPath(oppRoot, "vendors"), vendor), relativeJitRoot));
            }
        }
        roots.push_back(FileUtils::JoinPath(FileUtils::JoinPath(oppRoot, "built-in"), relativeJitRoot));
    }
    return roots;
}

void AddLibrary(const std::string& path, std::set<std::string>& libraries)
{
    if (!FileUtils::IsRegularFile(path) || FileUtils::IsSymlink(path) || !IsResourceSoName(path)) {
        return;
    }
    std::string canonical;
    if (FileUtils::CanonicalPath(path, canonical)) {
        libraries.insert(canonical);
    }
}

bool CollectLibraries(
    const std::string& directory, bool recursive, bool ignoreOpenFailure, std::set<std::string>& libraries)
{
    std::vector<std::string> paths;
    if (!FileUtils::WalkDirectory(directory, recursive, ignoreOpenFailure, paths)) {
        return false;
    }
    for (const std::string& path : paths) {
        AddLibrary(path, libraries);
    }
    return true;
}

ResourceStatus DiscoverDirectLibraries(const std::string& directory, std::set<std::string>& libraries)
{
    std::string root;
    if (!FileUtils::CanonicalPath(directory, root) || !FileUtils::IsDirectory(root)) {
        ASCENDLOGE("JIT_COMPILE_RESOURCE_DIR is not a directory: path=%s", directory.c_str());
        return ResourceStatus::InvalidResource;
    }
    ASCENDLOGD("Scanning explicit compile resource directory: path=%s", root.c_str());
    return CollectLibraries(root, false, false, libraries) ? ResourceStatus::Success : ResourceStatus::IoError;
}

ResourceStatus DiscoverRecursiveLibraries(const std::string& directory, std::set<std::string>& libraries)
{
    if (FileUtils::IsSymlink(directory)) {
        ASCENDLOGW("Skipping symlinked automatic compile resource root: path=%s", directory.c_str());
        return ResourceStatus::Success;
    }
    std::string root;
    if (!FileUtils::CanonicalPath(directory, root) || !FileUtils::IsDirectory(root)) {
        ASCENDLOGD("Automatic compile resource root is unavailable: path=%s", directory.c_str());
        return ResourceStatus::Success;
    }
    ASCENDLOGD("Scanning automatic compile resource root: path=%s", root.c_str());
    return CollectLibraries(root, true, true, libraries) ? ResourceStatus::Success : ResourceStatus::IoError;
}

ResourceStatus DiscoverLibraries(const std::string& explicitDirectory, std::vector<std::string>& libraries)
{
    std::set<std::string> found;
    if (!explicitDirectory.empty()) {
        const ResourceStatus status = DiscoverDirectLibraries(explicitDirectory, found);
        if (status != ResourceStatus::Success) {
            return status;
        }
    } else {
        for (const std::string& root : AutomaticSearchRoots()) {
            const ResourceStatus status = DiscoverRecursiveLibraries(root, found);
            if (status != ResourceStatus::Success) {
                return status;
            }
        }
    }
    if (found.empty()) {
        if (explicitDirectory.empty()) {
            ASCENDLOGW("No compile resource shared object found from ASCEND_CUSTOM_OPP_PATH or ASCEND_OPP_PATH");
        } else {
            ASCENDLOGE(
                "JIT_COMPILE_RESOURCE_DIR contains no compile resource shared object: path=%s",
                explicitDirectory.c_str());
        }
        return ResourceStatus::NotFound;
    }
    libraries.assign(found.begin(), found.end());
    ASCENDLOGI("Discovered compile resource shared objects: count=%zu", libraries.size());
    return ResourceStatus::Success;
}

constexpr uint64_t kMaxPackageIdSize = 4096U;
constexpr uint64_t kMaxManifestSize = 8U * 1024U * 1024U;
constexpr uint64_t kMaxPathSize = 4096U;
constexpr uint64_t kMaxResourceFileSize = 256U * 1024U * 1024U;
constexpr uint64_t kMaxManifestResourceSize = 512U * 1024U * 1024U;
constexpr uint64_t kMaxRegistryResourceSize = 1024U * 1024U * 1024U;
constexpr uint64_t kMaxManifestCount = 4096U;
constexpr uint64_t kMaxExtensionCount = 1024U;
constexpr uint64_t kMaxFileCount = 65536U;
constexpr uint64_t kMaxRegistryFileCount = 131072U;

ResourceStatus OpenResourceLibrary(const std::string& libraryPath, LibraryHandle& library)
{
    library.reset(dlopen(libraryPath.c_str(), RTLD_NOW | RTLD_LOCAL));
    if (library != nullptr) {
        return ResourceStatus::Success;
    }
    const char* error = dlerror();
    ASCENDLOGE(
        "Failed to load compile resource shared object: path=%s, error=%s", libraryPath.c_str(),
        error == nullptr ? "unknown" : error);
    return ResourceStatus::LoadError;
}

bool IsValidBundleHeader(const AcCompileResourceBundleHeader* header)
{
    return header != nullptr && header->magic == AC_COMPILE_RESOURCE_MAGIC &&
           header->abiVersion == AC_COMPILE_RESOURCE_ABI_VERSION &&
           header->structSize == sizeof(AcCompileResourceBundle) && header->flags == 0U;
}

bool IsValidBundleTable(const AcCompileResourceBundle& bundle)
{
    return bundle.manifestCount != 0U && bundle.manifestCount <= kMaxManifestCount && bundle.manifests != nullptr &&
           bundle.extensionCount <= kMaxExtensionCount &&
           ((bundle.extensionCount == 0U) == (bundle.extensions == nullptr));
}

ResourceStatus GetResourceBundle(
    const LibraryHandle& library, const std::string& libraryPath, const AcCompileResourceBundle*& bundle)
{
    dlerror();
    const auto getter =
        reinterpret_cast<AscendcGetCompileResourceBundleFn>(dlsym(library.get(), "AscendcGetCompileResourceBundle"));
    const char* symbolError = dlerror();
    if (symbolError != nullptr || getter == nullptr) {
        ASCENDLOGE("Compile resource shared object has no bundle entry point: path=%s", libraryPath.c_str());
        return ResourceStatus::InvalidResource;
    }
    const AcCompileResourceBundleHeader* header = getter();
    if (!IsValidBundleHeader(header)) {
        ASCENDLOGE("Invalid compile resource bundle header: path=%s", libraryPath.c_str());
        return ResourceStatus::InvalidResource;
    }
    bundle = reinterpret_cast<const AcCompileResourceBundle*>(header);
    if (!IsValidBundleTable(*bundle)) {
        ASCENDLOGE(
            "Invalid compile resource bundle table: path=%s, manifests=%" PRIu64 ", extensions=%" PRIu64,
            libraryPath.c_str(), bundle->manifestCount, bundle->extensionCount);
        return ResourceStatus::InvalidResource;
    }
    return ResourceStatus::Success;
}

ResourceStatus ValidateBundleExtensions(const AcCompileResourceBundle& bundle, const std::string& libraryPath)
{
    for (uint64_t index = 0U; index < bundle.extensionCount; ++index) {
        const AcCompileResourceExtension& extension = bundle.extensions[index];
        if ((extension.flags & ~AC_COMPILE_RESOURCE_EXTENSION_REQUIRED) != 0U ||
            ((extension.dataSize == 0U) != (extension.data == nullptr))) {
            ASCENDLOGE("Invalid compile resource extension: path=%s, index=%" PRIu64, libraryPath.c_str(), index);
            return ResourceStatus::InvalidResource;
        }
        if ((extension.flags & AC_COMPILE_RESOURCE_EXTENSION_REQUIRED) != 0U) {
            ASCENDLOGE(
                "Unsupported required compile resource extension: path=%s, index=%" PRIu64, libraryPath.c_str(), index);
            return ResourceStatus::InvalidResource;
        }
        ASCENDLOGW("Ignoring optional compile resource extension: path=%s, index=%" PRIu64, libraryPath.c_str(), index);
    }
    return ResourceStatus::Success;
}

ResourceStatus ParseManifestUnit(
    const AcCompileResourceManifest& unit, const std::string& origin, ResourceManifest& manifest)
{
    std::string manifestText;
    const ResourceStatus status = CopyString(unit.json, "manifest.json", kMaxManifestSize, manifestText);
    if (status != ResourceStatus::Success) {
        return status;
    }
    if (!ParseResourceManifest(manifestText, origin, manifest)) {
        ASCENDLOGE("Invalid compile resource manifest: origin=%s", origin.c_str());
        return ResourceStatus::InvalidResource;
    }
    return ResourceStatus::Success;
}

ResourceStatus ValidateManifestMetadata(
    const AcCompileResourceManifest& unit, const ResourceManifest& manifest, uint64_t registryFileCount,
    bool duplicateResourceId)
{
    if (unit.fileCount > kMaxFileCount || ((unit.fileCount == 0U) != (unit.files == nullptr)) ||
        registryFileCount > kMaxRegistryFileCount || unit.fileCount > kMaxRegistryFileCount - registryFileCount) {
        ASCENDLOGE(
            "Manifest has an invalid resource file table: resource_id=%s, files=%" PRIu64 ", registry_files=%" PRIu64,
            manifest.resourceId.c_str(), unit.fileCount, registryFileCount);
        return ResourceStatus::InvalidResource;
    }
    if (duplicateResourceId) {
        ASCENDLOGE("Duplicate resource_id in compile resource bundles: resource_id=%s", manifest.resourceId.c_str());
        return ResourceStatus::InvalidResource;
    }
    if (!ValidateRelativeResourcePath(manifest.resourceId) ||
        FileUtils::FileName(manifest.resourceId) != manifest.resourceId) {
        ASCENDLOGE("Compile resource manifest has an unsafe resource_id: resource_id=%s", manifest.resourceId.c_str());
        return ResourceStatus::InvalidResource;
    }
    return ResourceStatus::Success;
}

ResourceStatus CopyResourceFileIdentity(const AcCompileResourceFile& file, std::string& fileName, std::string& filePath)
{
    ResourceStatus status = CopyString(file.fileName, "resource.fileName", kMaxPathSize, fileName);
    if (status == ResourceStatus::Success) {
        status = CopyString(file.filePath, "resource.filePath", kMaxPathSize, filePath);
    }
    return status;
}

bool ValidateResourceFileIdentity(const std::string& fileName, const std::string& filePath, const std::string& baseDir)
{
    if (!ValidateRelativeResourcePath(fileName) || FileUtils::FileName(fileName) != fileName) {
        ASCENDLOGE("Bundle resource has an invalid file name: value=%s", fileName.c_str());
        return false;
    }
    const bool outsideBaseDir = !baseDir.empty() && !FileUtils::IsWithinPath(filePath, baseDir);
    const bool matchesBaseDir =
        !baseDir.empty() && FileUtils::PathComponents(filePath) == FileUtils::PathComponents(baseDir);
    if (!ValidateRelativeResourcePath(filePath) || outsideBaseDir || matchesBaseDir ||
        FileUtils::FileName(filePath) != fileName) {
        ASCENDLOGE("Bundle resource has an invalid file path: value=%s", filePath.c_str());
        return false;
    }
    return true;
}

ResourceStatus ValidateResourceFileData(
    const AcCompileResourceFile& file, const std::string& resourceId, const std::string& filePath,
    uint64_t& manifestBytes, uint64_t& registryBytes)
{
    if (file.size > kMaxResourceFileSize || (file.size != 0U && file.data == nullptr)) {
        ASCENDLOGE("Bundle resource has invalid data: path=%s, size=%" PRIu64, filePath.c_str(), file.size);
        return ResourceStatus::InvalidResource;
    }
    if (!TryAddWithinLimit(file.size, kMaxManifestResourceSize, manifestBytes) ||
        !TryAddWithinLimit(file.size, kMaxRegistryResourceSize, registryBytes)) {
        ASCENDLOGE(
            "Bundle resource exceeds aggregate size limit: resource_id=%s, path=%s, size=%" PRIu64, resourceId.c_str(),
            filePath.c_str(), file.size);
        return ResourceStatus::InvalidResource;
    }
    return ResourceStatus::Success;
}

ResourceStatus AppendResourceFile(
    const AcCompileResourceFile& file, const ResourceManifest& manifest, ResourceRecordPack& pack,
    std::set<std::string>& filePaths, uint64_t& manifestBytes, uint64_t& registryBytes)
{
    std::string fileName;
    std::string filePath;
    ResourceStatus status = CopyResourceFileIdentity(file, fileName, filePath);
    if (status != ResourceStatus::Success || !ValidateResourceFileIdentity(fileName, filePath, manifest.baseDir)) {
        return status == ResourceStatus::Success ? ResourceStatus::InvalidResource : status;
    }
    status = ValidateResourceFileData(file, manifest.resourceId, filePath, manifestBytes, registryBytes);
    if (status != ResourceStatus::Success) {
        return status;
    }
    if (!filePaths.insert(filePath).second) {
        ASCENDLOGE("Duplicate resource file path in bundle: path=%s", filePath.c_str());
        return ResourceStatus::InvalidResource;
    }
    std::vector<uint8_t> bytes;
    if (file.size != 0U) {
        bytes.assign(file.data, file.data + file.size);
    }
    pack.files.push_back({fileName, filePath, std::string(), std::move(bytes)});
    ASCENDLOGD(
        "Validated bundled resource file: resource_id=%s, path=%s, size=%" PRIu64, manifest.resourceId.c_str(),
        filePath.c_str(), file.size);
    return ResourceStatus::Success;
}

ResourceStatus LoadManifestFiles(
    const AcCompileResourceManifest& unit, const ResourceManifest& manifest, uint64_t currentRegistryBytes,
    ResourceRecordPack& pack, uint64_t& registryBytes)
{
    pack.files.reserve(static_cast<size_t>(unit.fileCount));
    std::set<std::string> filePaths;
    uint64_t manifestBytes = 0U;
    registryBytes = currentRegistryBytes;
    for (uint64_t index = 0U; index < unit.fileCount; ++index) {
        const ResourceStatus status =
            AppendResourceFile(unit.files[index], manifest, pack, filePaths, manifestBytes, registryBytes);
        if (status != ResourceStatus::Success) {
            return status;
        }
    }
    std::string parentPath;
    std::string childPath;
    if (FindResourcePathConflict(filePaths, parentPath, childPath)) {
        ASCENDLOGE(
            "Resource file path conflicts with a child path: parent=%s, child=%s", parentPath.c_str(),
            childPath.c_str());
        return ResourceStatus::InvalidResource;
    }
    std::sort(pack.files.begin(), pack.files.end(), [](const ResourceFileData& left, const ResourceFileData& right) {
        return left.filePath < right.filePath;
    });
    return ResourceStatus::Success;
}

void AddManifestRecords(const ResourceManifest& manifest, const std::string& libraryPath, ResourceRecordPack& pack)
{
    for (const ManifestKernel& kernel : manifest.kernels) {
        std::unique_ptr<ResourceRecord> record(new ResourceRecord());
        record->resourceId = manifest.resourceId;
        record->socVersion = manifest.socVersion;
        record->sourceFile = manifest.sourceFile;
        record->resourceSoPath = libraryPath;
        record->kernel = kernel;
        pack.records.emplace(kernel.kernelName, std::move(record));
    }
}

ResourceStatus WriteMaterializedFiles(
    const std::vector<ResourceFileData>& files, const std::string& materializedRoot,
    std::vector<std::string>& materializedPaths)
{
    materializedPaths.reserve(files.size());
    for (const ResourceFileData& file : files) {
        std::string path = FileUtils::JoinPath(materializedRoot, file.filePath);
        if (!FileUtils::WriteFile(path, file.bytes)) {
            return ResourceStatus::IoError;
        }
        materializedPaths.push_back(std::move(path));
    }
    return ResourceStatus::Success;
}

ResourceStatus ResolveOptions(
    std::vector<std::string>& options, const std::string& materializedRoot, const char* optionType)
{
    for (std::string& option : options) {
        const std::string referenced = ReferencedResourcePath(option, materializedRoot);
        if (!referenced.empty() && !FileUtils::Exists(referenced)) {
            ASCENDLOGE("%s references an unavailable resource: path=%s", optionType, referenced.c_str());
            return ResourceStatus::InvalidResource;
        }
        option = ReplaceResourceRoot(std::move(option), materializedRoot);
    }
    return ResourceStatus::Success;
}

ResourceStatus ResolveConstants(std::vector<ManifestConstant>& constants, const std::string& materializedRoot)
{
    for (ManifestConstant& constant : constants) {
        constant.file = ReplaceResourceRoot(std::move(constant.file), materializedRoot);
        if (!FileUtils::IsRegularFile(constant.file)) {
            ASCENDLOGE("Constant references an unavailable resource: path=%s", constant.file.c_str());
            return ResourceStatus::InvalidResource;
        }
    }
    return ResourceStatus::Success;
}

ResourceStatus ResolveCommands(std::vector<ManifestObject>& objects, const std::string& materializedRoot)
{
    for (ManifestObject& object : objects) {
        for (ManifestCommand& command : object.commands) {
            const ResourceStatus status = ResolveOptions(command.options, materializedRoot, "Command");
            if (status != ResourceStatus::Success) {
                return status;
            }
        }
    }
    return ResourceStatus::Success;
}

ResourceStatus ResolveRecordKernel(ManifestKernel& kernel, const std::string& materializedRoot)
{
    ResourceStatus status = ResolveConstants(kernel.constants, materializedRoot);
    if (status != ResourceStatus::Success) {
        return status;
    }
    status = ResolveCommands(kernel.objects, materializedRoot);
    return status == ResourceStatus::Success ? ResolveOptions(kernel.linkOptions, materializedRoot, "Link option") :
                                               status;
}

ResourceStatus ResolveResourceRecords(
    const ResourceRecordPack& pack, const std::string& materializedRoot,
    std::unordered_map<std::string, std::unique_ptr<ResourceRecord>>& resolvedRecords)
{
    resolvedRecords.reserve(pack.records.size());
    for (const auto& item : pack.records) {
        std::unique_ptr<ResourceRecord> record(new ResourceRecord(*item.second));
        record->materializedRoot = materializedRoot;
        record->resourceSoPath = pack.resourceSoPath;
        const ResourceStatus status = ResolveRecordKernel(record->kernel, materializedRoot);
        if (status != ResourceStatus::Success) {
            return status;
        }
        resolvedRecords.emplace(item.first, std::move(record));
    }
    return ResourceStatus::Success;
}

ResourceStatus MaterializePack(ResourceRecordPack& pack, const std::string& materializedRoot)
{
    std::vector<std::string> materializedPaths;
    ResourceStatus status = WriteMaterializedFiles(pack.files, materializedRoot, materializedPaths);
    if (status != ResourceStatus::Success) {
        return status;
    }
    std::unordered_map<std::string, std::unique_ptr<ResourceRecord>> resolvedRecords;
    status = ResolveResourceRecords(pack, materializedRoot, resolvedRecords);
    if (status != ResourceStatus::Success) {
        return status;
    }
    for (size_t index = 0U; index < pack.files.size(); ++index) {
        pack.files[index].materializedPath.swap(materializedPaths[index]);
    }
    pack.records.swap(resolvedRecords);
    pack.filesMaterialized = true;
    return ResourceStatus::Success;
}

} // namespace

ResourceStatus ResourceRegistry::Materialize(const std::string& resourceId, ResourceRecordPack& pack)
{
    if (pack.filesMaterialized) {
        return ResourceStatus::Success;
    }
    if (temporaryRoot_.empty()) {
        if (!FileUtils::MakeTempRoot(temporaryRoot_)) {
            return ResourceStatus::IoError;
        }
    }
    std::string materializedRoot;
    try {
        materializedRoot = FileUtils::JoinPath(temporaryRoot_, resourceId);
        ASCENDLOGI("Materializing compile resources: resource_id=%s, files=%zu", resourceId.c_str(), pack.files.size());
        const ResourceStatus status = MaterializePack(pack, materializedRoot);
        if (status != ResourceStatus::Success) {
            FileUtils::RemoveTree(materializedRoot);
            return status;
        }
        ASCENDLOGI(
            "Compile resources materialized successfully: resource_id=%s, kernels=%zu", resourceId.c_str(),
            pack.records.size());
        return ResourceStatus::Success;
    } catch (const std::bad_alloc&) {
        FileUtils::RemoveTree(materializedRoot);
        ASCENDLOGE("Compile resource materialization failed: out of memory, resource_id=%s", resourceId.c_str());
        return ResourceStatus::NoMemory;
    } catch (const std::exception& error) {
        FileUtils::RemoveTree(materializedRoot);
        ASCENDLOGE(
            "Compile resource materialization failed unexpectedly: resource_id=%s, error=%s", resourceId.c_str(),
            error.what());
        return ResourceStatus::InternalError;
    } catch (...) {
        FileUtils::RemoveTree(materializedRoot);
        ASCENDLOGE("Compile resource materialization failed unexpectedly: resource_id=%s", resourceId.c_str());
        return ResourceStatus::InternalError;
    }
}

ResourceStatus ResourceRegistry::LoadLibrary(const std::string& libraryPath)
{
    ASCENDLOGI("Loading compile resource shared object: path=%s", libraryPath.c_str());
    LibraryHandle library;
    ResourceStatus status = OpenResourceLibrary(libraryPath, library);
    if (status != ResourceStatus::Success) {
        return status;
    }
    const AcCompileResourceBundle* bundle = nullptr;
    status = GetResourceBundle(library, libraryPath, bundle);
    if (status != ResourceStatus::Success) {
        return status;
    }
    status = ValidateBundleExtensions(*bundle, libraryPath);
    if (status != ResourceStatus::Success) {
        return status;
    }
    std::string packageId;
    status = CopyString(bundle->packageId, "packageId", kMaxPackageIdSize, packageId);
    if (status != ResourceStatus::Success) {
        return status;
    }
    ASCENDLOGD(
        "Validated compile resource bundle: package_id=%s, manifests=%" PRIu64, packageId.c_str(),
        bundle->manifestCount);
    for (uint64_t manifestIndex = 0U; manifestIndex < bundle->manifestCount; ++manifestIndex) {
        status = LoadManifest(bundle->manifests[manifestIndex], libraryPath, manifestIndex);
        if (status != ResourceStatus::Success) {
            return status;
        }
    }
    libraries_.push_back(library.get());
    (void)library.release();
    ASCENDLOGI("Loaded compile resource shared object: path=%s, package_id=%s", libraryPath.c_str(), packageId.c_str());
    return ResourceStatus::Success;
}

ResourceStatus ResourceRegistry::LoadManifest(
    const AcCompileResourceManifest& unit, const std::string& libraryPath, uint64_t manifestIndex)
{
    const std::string manifestOrigin = libraryPath + ":manifest[" + std::to_string(manifestIndex) + "]";
    ResourceManifest manifest;
    ResourceStatus status = ParseManifestUnit(unit, manifestOrigin, manifest);
    if (status != ResourceStatus::Success) {
        return status;
    }
    status =
        ValidateManifestMetadata(unit, manifest, resourceFileCount_, recordPacks_.count(manifest.resourceId) != 0U);
    if (status != ResourceStatus::Success) {
        return status;
    }
    ResourceRecordPack pack;
    pack.socVersion = manifest.socVersion;
    pack.resourceSoPath = libraryPath;
    uint64_t registryResourceBytes = 0U;
    status = LoadManifestFiles(unit, manifest, resourceBytes_, pack, registryResourceBytes);
    if (status != ResourceStatus::Success) {
        return status;
    }
    AddManifestRecords(manifest, libraryPath, pack);
    ASCENDLOGI(
        "Validated compile resource manifest: resource_id=%s, kernels=%zu, files=%zu", manifest.resourceId.c_str(),
        pack.records.size(), pack.files.size());
    const auto inserted = recordPacks_.emplace(manifest.resourceId, std::move(pack));
    if (!inserted.second) {
        ASCENDLOGE("Duplicate resource_id in compile resource bundles: resource_id=%s", manifest.resourceId.c_str());
        return ResourceStatus::InvalidResource;
    }
    resourceBytes_ = registryResourceBytes;
    resourceFileCount_ += unit.fileCount;
    return ResourceStatus::Success;
}

ResourceRegistry::ResourceRegistry() = default;

ResourceRegistry::~ResourceRegistry()
{
    recordPacks_.clear();
    for (auto iterator = libraries_.rbegin(); iterator != libraries_.rend(); ++iterator) {
        CloseLibrary(*iterator);
    }
    if (!temporaryRoot_.empty()) {
        ASCENDLOGD("Removing compile resource temporary root: path=%s", temporaryRoot_.c_str());
        FileUtils::RemoveTree(temporaryRoot_);
    }
}

ResourceStatus ResourceRegistry::Load(const std::string& directory) noexcept
{
    try {
        std::lock_guard<std::mutex> lock(lookupMutex_);
        if (!libraries_.empty() || !recordPacks_.empty()) {
            ASCENDLOGE("Compile resource registry cannot be loaded more than once");
            return ResourceStatus::InvalidResource;
        }
        ASCENDLOGI(
            "Starting compile resource registry load: explicit_directory=%s",
            directory.empty() ? "<automatic>" : directory.c_str());
        std::vector<std::string> libraryPaths;
        ResourceStatus status = DiscoverLibraries(directory, libraryPaths);
        if (status != ResourceStatus::Success) {
            return status;
        }
        ResourceRegistry staged;
        staged.libraries_.reserve(libraryPaths.size());
        for (const std::string& libraryPath : libraryPaths) {
            status = staged.LoadLibrary(libraryPath);
            if (status != ResourceStatus::Success) {
                ASCENDLOGE("Compile resource registry load stopped: path=%s", libraryPath.c_str());
                return status;
            }
        }
        libraries_.swap(staged.libraries_);
        recordPacks_.swap(staged.recordPacks_);
        resourceBytes_ = staged.resourceBytes_;
        resourceFileCount_ = staged.resourceFileCount_;
        ASCENDLOGI(
            "Compile resource registry loaded successfully: libraries=%zu, manifests=%zu", libraryPaths.size(),
            recordPacks_.size());
        return ResourceStatus::Success;
    } catch (const std::bad_alloc&) {
        ASCENDLOGE("Compile resource registry load failed: out of memory");
        return ResourceStatus::NoMemory;
    } catch (const std::exception& error) {
        ASCENDLOGE("Compile resource registry load failed unexpectedly: error=%s", error.what());
        return ResourceStatus::InternalError;
    } catch (...) {
        ASCENDLOGE("Compile resource registry load failed unexpectedly: unknown error");
        return ResourceStatus::InternalError;
    }
}

ResourceStatus ResourceRegistry::Lookup(
    const std::string& resourceId, const std::string& kernelName, const ResourceRecord*& record) noexcept
{
    record = nullptr;
    try {
        ASCENDLOGD("Looking up compile resource: resource_id=%s, kernel=%s", resourceId.c_str(), kernelName.c_str());
        std::lock_guard<std::mutex> lock(lookupMutex_);
        const auto packIterator = recordPacks_.find(resourceId);
        if (packIterator == recordPacks_.end()) {
            ASCENDLOGD("Compile resource resource_id was not found: resource_id=%s", resourceId.c_str());
            return ResourceStatus::NotFound;
        }
        ResourceRecordPack& pack = packIterator->second;
        const auto recordIterator = pack.records.find(kernelName);
        if (recordIterator == pack.records.end() || recordIterator->second->socVersion != pack.socVersion) {
            ASCENDLOGD(
                "Compile resource kernel was not found: resource_id=%s, kernel=%s", resourceId.c_str(),
                kernelName.c_str());
            return ResourceStatus::NotFound;
        }
        const ResourceStatus status = Materialize(resourceId, pack);
        if (status != ResourceStatus::Success) {
            return status;
        }
        const auto resolvedIterator = pack.records.find(kernelName);
        if (resolvedIterator == pack.records.end()) {
            ASCENDLOGE(
                "Materialized compile resource record disappeared: resource_id=%s, kernel=%s", resourceId.c_str(),
                kernelName.c_str());
            return ResourceStatus::InternalError;
        }
        record = resolvedIterator->second.get();
        ASCENDLOGD(
            "Compile resource lookup succeeded: resource_id=%s, kernel=%s", resourceId.c_str(), kernelName.c_str());
        return ResourceStatus::Success;
    } catch (const std::bad_alloc&) {
        ASCENDLOGE(
            "Compile resource lookup failed: out of memory, resource_id=%s, kernel=%s", resourceId.c_str(),
            kernelName.c_str());
        return ResourceStatus::NoMemory;
    } catch (const std::exception& error) {
        ASCENDLOGE(
            "Compile resource lookup failed unexpectedly: resource_id=%s, kernel=%s, error=%s", resourceId.c_str(),
            kernelName.c_str(), error.what());
        return ResourceStatus::InternalError;
    } catch (...) {
        ASCENDLOGE(
            "Compile resource lookup failed unexpectedly: resource_id=%s, kernel=%s", resourceId.c_str(),
            kernelName.c_str());
        return ResourceStatus::InternalError;
    }
}

} // namespace resource
} // namespace ascendc
