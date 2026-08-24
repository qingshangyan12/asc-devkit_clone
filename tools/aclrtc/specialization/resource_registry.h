/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_ACLRTC_SPECIALIZATION_RESOURCE_REGISTRY_H
#define ASCENDC_ACLRTC_SPECIALIZATION_RESOURCE_REGISTRY_H

#include <cstdint>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "ascendc_manifest_abi.h"
#include "nlohmann/json.hpp"
#include "directory_cleanup_guard.h"

namespace boost {
namespace filesystem {
class path;
} // namespace filesystem
} // namespace boost

namespace ascendc {
namespace specialization_compile {

enum class ResourceStatus : uint32_t {
    Success,
    NotFound,
    Conflict,
    InvalidResource,
    IoError,
    LoadError,
    InternalError,
};

enum class ResourceSourceType : uint32_t {
    External,
    Custom,
    BuiltIn,
};

/**
 * @brief Returns whether kernel metadata and diagnostic artifacts are retained.
 * @return true only when ASCEND_OP_COMPILE_SAVE_KERNEL_META, after surrounding
 * whitespace is ignored, is exactly "1".
 */
bool IsKernelMetaSavingEnabled() noexcept;

struct ResourceData {
    nlohmann::json json;
    std::string resourceDir;
    std::string sourceFilePath;
};

struct BundledResourceFile {
    std::string fileName;
    std::string relativePath;
    std::vector<uint8_t> contents;
};

struct ResourceEntry {
    ResourceData resourceData;
    std::vector<BundledResourceFile> bundledFiles;
    std::string sourceLibraryPath;
    ResourceSourceType sourceType = ResourceSourceType::External;
    std::mutex materializationMutex;
};

using ResourceStore = std::unordered_map<std::string, std::unique_ptr<ResourceEntry>>;

struct StagedResourceSet {
    ResourceStore resources;
    uint64_t payloadByteCount = 0U;
    uint64_t fileCount = 0U;
    bool libraryDiscovered = false;
    bool conflictDetected = false;
};

struct ResourceLoadTransaction {
    StagedResourceSet external;
    StagedResourceSet custom;
    StagedResourceSet builtIn;
};

struct ResourceLibraryDescriptor {
    std::string libraryPath;
    ResourceSourceType sourceType;
};

struct ResourceSearchRoots {
    std::vector<std::string> customSearchRoots;
    std::vector<std::string> builtInSearchRoots;
};

struct SharedLibraryDeleter {
    void operator()(void* handle) const noexcept;
};

using SharedLibraryHandle = std::unique_ptr<void, SharedLibraryDeleter>;

class ResourceRegistry {
public:
    /**
     * @brief Returns the process-wide compile resource registry instance.
     * @return ResourceRegistry& The process-wide registry instance.
     */
    static ResourceRegistry& Instance();

    /**
     * @brief Loads compile resources from an explicit shared object or the configured OPP search paths.
     * @param[in] resourceLibraryPath Resource shared object file to load. Pass nullptr or an empty string to use
     * automatic OPP discovery. Directory input is not supported for explicit loading.
     * @return ResourceStatus::Success on success; otherwise, a status describing the discovery, validation, loading,
     * I/O, or conflict failure.
     */
    ResourceStatus Load(const char* resourceLibraryPath);

    /**
     * @brief Looks up a registered compile resource and materializes its bundled files.
     * @param[in] resourceId Non-empty identifier of the resource to look up.
     * @param[out] resource Receives the resource data on success. It is reset when the lookup fails.
     * @return ResourceStatus::Success on success; otherwise, ResourceStatus::NotFound, ResourceStatus::InvalidResource,
     * or a status describing the materialization failure.
     */
    ResourceStatus Lookup(const char* resourceId, ResourceData& resource);

    ResourceRegistry(const ResourceRegistry&) = delete;
    ResourceRegistry& operator=(const ResourceRegistry&) = delete;

private:
    ResourceRegistry();
    ~ResourceRegistry();

    static ResourceSearchRoots AutomaticSearchRoots();
    static bool IsLibraryName(const std::string& path);
    static void AddLibrary(const std::string& path, std::set<std::string>& found);
    static bool CollectLibraries(
        const std::string& root, ResourceSourceType sourceType, bool recursive, std::set<std::string>& found);
    static bool CollectAutomaticLibraries(
        const std::vector<std::string>& roots, ResourceSourceType sourceType, std::set<std::string>& found);
    static ResourceStatus DiscoverLibraries(
        const char* resourceLibraryPath, std::vector<ResourceLibraryDescriptor>& libraries);
    static ResourceStatus ResolveAndValidateBundle(
        const SharedLibraryHandle& library, const ResourceLibraryDescriptor& libraryDescriptor,
        const AcCompileResourceBundle*& bundle);
    static ResourceStatus ResolveSourceRoot(
        const boost::filesystem::path& searchRoot, const std::string& resourceId, std::string& canonicalRoot);
    static ResourceStatus AddSourceMatch(
        const boost::filesystem::path& candidate, const std::string& sourceFile, const std::string& canonicalRoot,
        const std::string& resourceId, std::set<std::string>& matches);
    static ResourceStatus LocateExternalSourceDirectory(
        const ResourceLibraryDescriptor& libraryDescriptor, const std::string& resourceId,
        const std::string& sourceFile, std::string& sourceDirectoryPath);
    static bool FindPathConflict(const std::set<std::string>& paths);
    static ResourceStatus AppendFile(
        const AcCompileResourceFile& file, const std::string& resourceId, ResourceEntry& entry,
        std::set<std::string>& paths, uint64_t& manifestBytes, StagedResourceSet& stagedResources);
    static ResourceStatus CopyManifestFiles(
        const AcCompileResourceManifest& unit, const std::string& resourceId, ResourceEntry& entry,
        StagedResourceSet& stagedResources);
    static StagedResourceSet& SelectStagedResources(
        ResourceLoadTransaction& loadTransaction, ResourceSourceType sourceType);
    static ResourceStatus LoadManifest(
        const AcCompileResourceManifest& unit, const ResourceLibraryDescriptor& libraryDescriptor,
        uint64_t manifestIndex, ResourceLoadTransaction& loadTransaction);
    static ResourceStatus LoadLibrary(
        const ResourceLibraryDescriptor& libraryDescriptor, ResourceLoadTransaction& loadTransaction);
    static ResourceStatus LoadLibraries(
        const std::vector<ResourceLibraryDescriptor>& libraries, ResourceLoadTransaction& loadTransaction);
    static ResourceStatus WriteMaterializedFiles(
        const std::vector<BundledResourceFile>& bundledFiles, const std::string& materializationRootPath);

    ResourceEntry* FindResource(const std::string& resourceId) noexcept;
    ResourceStatus MaterializeResource(
        const std::string& resourceId, const ResourceEntry& entry, ResourceData& resource);
    ResourceStatus CommitStagedResources(ResourceLoadTransaction& loadTransaction);
    bool HasCommitConflict(const ResourceStore& incomingResources, const ResourceStore& committedResources) const;

    DirectoryCleanupGuard materializationRootDirectory_;
    ResourceStore externalResources_;
    ResourceStore customResources_;
    ResourceStore builtInResources_;
    uint64_t resourceBytes_ = 0U;
    uint64_t resourceFileCount_ = 0U;
    bool automaticLoadAttempted_ = false;
    bool retainMaterializedDirectories_ = false;
    ResourceStatus automaticLoadStatus_ = ResourceStatus::Success;
    std::mutex loadMutex_;
    std::mutex registryMutex_;
};

} // namespace specialization_compile
} // namespace ascendc

#endif // ASCENDC_ACLRTC_SPECIALIZATION_RESOURCE_REGISTRY_H
