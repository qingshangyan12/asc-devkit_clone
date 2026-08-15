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
#include "specialization_compile_types.h"

namespace boost {
namespace filesystem {
class path;
} // namespace filesystem
} // namespace boost

namespace ascendc {
namespace specialization_compile {

enum class ResourceStatus { Success, NotFound, Conflict, InvalidResource, IoError, LoadError, InternalError };
enum class ResourceSourceType { External, Custom, BuiltIn };

struct ResourceFileData {
    std::string fileName;
    std::string relativePath;
    std::vector<uint8_t> bytes;
};

struct ResourceEntry {
    ResourceData data;
    std::vector<ResourceFileData> files;
    std::string sourceSoPath;
    ResourceSourceType sourceType = ResourceSourceType::External;
    std::mutex materializeMutex;
};

using ResourceStore = std::unordered_map<std::string, std::unique_ptr<ResourceEntry>>;

struct StagedResources {
    ResourceStore resources;
    uint64_t bytes = 0U;
    uint64_t files = 0U;
    bool discovered = false;
    bool conflict = false;
};

struct StageState {
    StagedResources external;
    StagedResources custom;
    StagedResources builtIn;
};

struct LibrarySpec {
    std::string path;
    ResourceSourceType sourceType;
};

struct AutomaticRoots {
    std::vector<std::string> custom;
    std::vector<std::string> builtIn;
};

struct LibraryDeleter {
    void operator()(void* handle) const noexcept;
};

using LibraryHandle = std::unique_ptr<void, LibraryDeleter>;

class ResourceRegistry final {
public:
    static ResourceRegistry& Instance();
    ResourceStatus Load(const char* directory);
    ResourceStatus Lookup(const char* resourceId, const ResourceData*& resource);

    ResourceRegistry(const ResourceRegistry&) = delete;
    ResourceRegistry& operator=(const ResourceRegistry&) = delete;

private:
    ResourceRegistry();
    ~ResourceRegistry();

    static AutomaticRoots AutomaticSearchRoots();
    static bool IsLibraryName(const std::string& path);
    static void AddLibrary(const std::string& path, std::set<std::string>& found);
    static bool CollectLibraries(
        const std::string& root, ResourceSourceType sourceType, bool recursive, std::set<std::string>& found);
    static bool CollectAutomaticLibraries(
        const std::vector<std::string>& roots, ResourceSourceType sourceType, std::set<std::string>& found);
    static ResourceStatus DiscoverLibraries(
        const char* directory, std::vector<LibrarySpec>& libraries);
    static ResourceStatus GetBundle(
        const LibraryHandle& library, const LibrarySpec& spec, const AcCompileResourceBundle*& bundle);
    static ResourceStatus ResolveSourceRoot(
        const boost::filesystem::path& searchRoot, const std::string& resourceId, std::string& canonicalRoot);
    static ResourceStatus AddSourceMatch(
        const boost::filesystem::path& candidate, const std::string& sourceFile, const std::string& canonicalRoot,
        const std::string& resourceId, std::set<std::string>& matches);
    static ResourceStatus LocateSourceFile(
        const LibrarySpec& spec, const std::string& resourceId, const std::string& sourceFile, std::string& located);
    static bool FindPathConflict(const std::set<std::string>& paths);
    static ResourceStatus AppendFile(
        const AcCompileResourceFile& file, const std::string& resourceId, ResourceEntry& entry,
        std::set<std::string>& paths, uint64_t& manifestBytes, StagedResources& staged);
    static ResourceStatus CopyManifestFiles(
        const AcCompileResourceManifest& unit, const std::string& resourceId, ResourceEntry& entry,
        StagedResources& staged);
    static StagedResources& SelectStagedResources(
        StageState& stage, ResourceSourceType sourceType);
    static ResourceStatus LoadManifest(
        const AcCompileResourceManifest& unit, const LibrarySpec& spec, uint64_t manifestIndex, StageState& stage);
    static ResourceStatus LoadLibrary(const LibrarySpec& spec,
                                                                      StageState& stage);
    static ResourceStatus LoadLibraries(
        const std::vector<LibrarySpec>& libraries, StageState& stage);
    static ResourceStatus WriteMaterializedFiles(
        const std::vector<ResourceFileData>& files, const std::string& root);

    ResourceEntry* FindResource(const std::string& resourceId) noexcept;
    ResourceStatus Materialize(const std::string& resourceId, ResourceEntry& entry);
    ResourceStatus Commit(StageState& stage);
    bool HasCommitConflict(const ResourceStore& incoming, const ResourceStore& committed);

    std::string temporaryRoot_;
    ResourceStore externalResources_;
    ResourceStore customResources_;
    ResourceStore builtInResources_;
    uint64_t resourceBytes_ = 0U;
    uint64_t resourceFileCount_ = 0U;
    bool automaticLoadAttempted_ = false;
    ResourceStatus automaticLoadStatus_ = ResourceStatus::Success;
    std::mutex loadMutex_;
    std::mutex registryMutex_;
};

} // namespace specialization_compile
} // namespace ascendc

#undef ASCENDC_RESOURCE_REGISTRY_HIDDEN

#endif // ASCENDC_ACLRTC_SPECIALIZATION_RESOURCE_REGISTRY_H
