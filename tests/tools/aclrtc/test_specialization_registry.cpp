/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "specialization_compile_types.h"
#define private public
#include "resource_registry.h"
#undef private

#include "ascendc_manifest_abi.h"
#include "file_utils.h"

#include <cstdlib>
#include <fstream>
#include <functional>
#include <iterator>
#include <utility>

#include <gtest/gtest.h>

#include <sys/stat.h>
#include <unistd.h>

namespace ascendc {
namespace resource {
namespace {

constexpr const char* kPluginPath = ACLRTC_UT_PLUGIN_PATH;
constexpr const char* kPluginDirectory = ACLRTC_UT_PLUGIN_DIR;
constexpr const char* kNoEntryPluginPath = ACLRTC_UT_NO_ENTRY_PLUGIN_PATH;

std::string SimpleManifest(const std::string& resourceId = "direct_resource", const std::string& baseDir = "assets")
{
    const std::string baseField = baseDir.empty() ? "" : ",\"base_dir\":\"" + baseDir + "\"";
    const std::string output = baseDir.empty() ? "kernel.o" : baseDir + "/kernel.o";
    return "{\"schema_version\":\"1.0\",\"resource_id\":\"" + resourceId + "\",\"soc_version\":\"Ascend910B\"" +
           baseField + ",\"kernels\":[{\"kernel_name\":\"kernel\",\"objects\":[{\"objects_name\":\"kernel.o\"," +
           "\"objects_type\":\"basic\",\"commands\":[{\"type\":\"compile\",\"stage\":0,\"options\":[" +
           "\"${output:" + output + "}\"]}],\"output\":[\"${output:" + output + "}\"]}]}]}";
}

struct ManifestUnitBuilder {
    explicit ManifestUnitBuilder(std::string text = SimpleManifest()) : json(std::move(text)) {}

    void AddFile(const std::string& name, const std::string& path, std::vector<uint8_t> bytes)
    {
        names.push_back(name);
        paths.push_back(path);
        payloads.push_back(std::move(bytes));
    }

    AcCompileResourceManifest Build()
    {
        files.clear();
        for (size_t index = 0U; index < names.size(); ++index) {
            files.push_back({
                {names[index].data(), names[index].size()},
                {paths[index].data(), paths[index].size()},
                payloads[index].empty() ? nullptr : payloads[index].data(),
                payloads[index].size(),
            });
        }
        return {
            {json.data(), json.size()},
            files.empty() ? nullptr : files.data(),
            files.size(),
        };
    }

    std::string json;
    std::vector<std::string> names;
    std::vector<std::string> paths;
    std::vector<std::vector<uint8_t>> payloads;
    std::vector<AcCompileResourceFile> files;
};

std::unique_ptr<ResourceRecord> MakeRecord(const std::string& socVersion = "Ascend910B")
{
    std::unique_ptr<ResourceRecord> record(new ResourceRecord());
    record->resourceId = "materialized_resource";
    record->socVersion = socVersion;
    record->sourceFile = "kernel.cpp";
    record->resourceSoPath = "resource.so";
    record->kernel.kernelName = "kernel";
    ManifestConstant constant;
    constant.name = "CONST_A";
    constant.byteSize = 4U;
    constant.file = "${resource}/assets/const.bin";
    record->kernel.constants.push_back(std::move(constant));
    ManifestObject object;
    object.objectsName = "kernel.o";
    object.objectsType = "basic";
    ManifestCommand command;
    command.type = "compile";
    command.options = {"plain", "-I${resource}/assets/include.bin", "${output:assets/kernel.o}"};
    object.commands.push_back(std::move(command));
    object.outputs = {"${output:assets/kernel.o}"};
    record->kernel.objects.push_back(std::move(object));
    record->kernel.linkOptions = {"plain-link", "${resource}/assets/link.ld"};
    return record;
}

ResourceRecordPack MakePack()
{
    ResourceRecordPack pack;
    pack.socVersion = "Ascend910B";
    pack.resourceSoPath = "resource.so";
    pack.files = {
        {"const.bin", "assets/const.bin", "", {1U, 2U, 3U, 4U}},
        {"include.bin", "assets/include.bin", "", {'i', 'n', 'c'}},
        {"link.ld", "assets/link.ld", "", {'l', 'i', 'n', 'k'}},
    };
    pack.records.emplace("kernel", MakeRecord());
    return pack;
}

bool CopyFile(const std::string& source, const std::string& destination)
{
    std::ifstream input(source, std::ios::binary);
    if (!input.is_open()) {
        return false;
    }
    const std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    return !input.bad() && FileUtils::WriteFile(destination, bytes) && chmod(destination.c_str(), 0700) == 0;
}

class ResourceRegistryTest : public testing::Test {
protected:
    void SetUp() override
    {
        ASSERT_TRUE(FileUtils::MakeTempRoot(root_));
        unsetenv("ACLRTC_UT_BUNDLE_MODE");
        unsetenv("ASCEND_CUSTOM_OPP_PATH");
        unsetenv("ASCEND_OPP_PATH");
    }

    void TearDown() override
    {
        unsetenv("ACLRTC_UT_BUNDLE_MODE");
        unsetenv("ASCEND_CUSTOM_OPP_PATH");
        unsetenv("ASCEND_OPP_PATH");
        FileUtils::RemoveTree(root_);
    }

    std::string root_;
};

TEST_F(ResourceRegistryTest, LoadManifestAcceptsValidFilesAndRejectsDuplicates)
{
    ResourceRegistry registry;
    ManifestUnitBuilder builder;
    builder.AddFile("z.bin", "assets/z.bin", {1U});
    builder.AddFile("a.bin", "assets/a.bin", {});
    AcCompileResourceManifest unit = builder.Build();
    EXPECT_EQ(registry.LoadManifest(unit, "direct.so", 0U), ResourceStatus::Success);
    EXPECT_EQ(registry.resourceBytes_, 1U);
    EXPECT_EQ(registry.resourceFileCount_, 2U);
    ASSERT_EQ(registry.recordPacks_.at("direct_resource").files.size(), 2U);
    EXPECT_EQ(registry.recordPacks_.at("direct_resource").files.front().fileName, "a.bin");
    EXPECT_EQ(registry.LoadManifest(unit, "direct.so", 1U), ResourceStatus::InvalidResource);

    ResourceRegistry duplicateFilesRegistry;
    ManifestUnitBuilder duplicateFiles;
    duplicateFiles.AddFile("a.bin", "assets/a.bin", {1U});
    duplicateFiles.AddFile("a.bin", "assets/a.bin", {2U});
    unit = duplicateFiles.Build();
    EXPECT_EQ(duplicateFilesRegistry.LoadManifest(unit, "direct.so", 0U), ResourceStatus::InvalidResource);
}

TEST_F(ResourceRegistryTest, LoadManifestRejectsInvalidManifestAndFileTables)
{
    ResourceRegistry registry;
    AcCompileResourceManifest unit = {{nullptr, 1U}, nullptr, 0U};
    EXPECT_EQ(registry.LoadManifest(unit, "direct.so", 0U), ResourceStatus::InvalidResource);

    ManifestUnitBuilder largeJson;
    unit = largeJson.Build();
    unit.json.size = 8U * 1024U * 1024U + 1U;
    EXPECT_EQ(registry.LoadManifest(unit, "direct.so", 0U), ResourceStatus::InvalidResource);

    ManifestUnitBuilder invalidJson("{");
    unit = invalidJson.Build();
    EXPECT_EQ(registry.LoadManifest(unit, "direct.so", 0U), ResourceStatus::InvalidResource);

    ManifestUnitBuilder builder;
    unit = builder.Build();
    unit.fileCount = 1U;
    EXPECT_EQ(registry.LoadManifest(unit, "direct.so", 0U), ResourceStatus::InvalidResource);
    unit.fileCount = 65537U;
    unit.files = reinterpret_cast<const AcCompileResourceFile*>(1U);
    EXPECT_EQ(registry.LoadManifest(unit, "direct.so", 0U), ResourceStatus::InvalidResource);

    registry.resourceFileCount_ = 131073U;
    unit = builder.Build();
    EXPECT_EQ(registry.LoadManifest(unit, "direct.so", 0U), ResourceStatus::InvalidResource);

    ResourceRegistry unsafeRegistry;
    ManifestUnitBuilder unsafe(SimpleManifest("../escape"));
    unit = unsafe.Build();
    EXPECT_EQ(unsafeRegistry.LoadManifest(unit, "direct.so", 0U), ResourceStatus::InvalidResource);
}

TEST_F(ResourceRegistryTest, LoadManifestRejectsInvalidFileMetadataAndLimits)
{
    auto expectInvalid = [this](
                             ManifestUnitBuilder& builder,
                             const std::function<void(AcCompileResourceManifest&, AcCompileResourceFile&)>& edit) {
        ResourceRegistry registry;
        AcCompileResourceManifest unit = builder.Build();
        AcCompileResourceFile& file = const_cast<AcCompileResourceFile&>(unit.files[0]);
        edit(unit, file);
        EXPECT_EQ(registry.LoadManifest(unit, "direct.so", 0U), ResourceStatus::InvalidResource);
    };

    ManifestUnitBuilder builder;
    builder.AddFile("file.bin", "assets/file.bin", {1U});
    expectInvalid(
        builder, [](AcCompileResourceManifest&, AcCompileResourceFile& file) { file.fileName = {nullptr, 1U}; });
    expectInvalid(builder, [](AcCompileResourceManifest&, AcCompileResourceFile& file) { file.fileName.size = 4097U; });
    expectInvalid(
        builder, [](AcCompileResourceManifest&, AcCompileResourceFile& file) { file.filePath = {nullptr, 1U}; });
    expectInvalid(builder, [](AcCompileResourceManifest&, AcCompileResourceFile& file) { file.filePath.size = 4097U; });
    expectInvalid(
        builder, [](AcCompileResourceManifest&, AcCompileResourceFile& file) { file.fileName = {"../file.bin", 11U}; });
    expectInvalid(builder, [](AcCompileResourceManifest&, AcCompileResourceFile& file) {
        file.filePath = {"outside/file.bin", 16U};
    });
    expectInvalid(
        builder, [](AcCompileResourceManifest&, AcCompileResourceFile& file) { file.filePath = {"assets", 6U}; });
    expectInvalid(builder, [](AcCompileResourceManifest&, AcCompileResourceFile& file) {
        file.filePath = {"assets/other.bin", 16U};
    });
    expectInvalid(builder, [](AcCompileResourceManifest&, AcCompileResourceFile& file) {
        file.size = 256U * 1024U * 1024U + 1U;
    });
    expectInvalid(builder, [](AcCompileResourceManifest&, AcCompileResourceFile& file) {
        file.data = nullptr;
        file.size = 1U;
    });

    ResourceRegistry aggregateRegistry;
    aggregateRegistry.resourceBytes_ = 1024ULL * 1024ULL * 1024ULL;
    AcCompileResourceManifest unit = builder.Build();
    EXPECT_EQ(aggregateRegistry.LoadManifest(unit, "direct.so", 0U), ResourceStatus::InvalidResource);
}

TEST_F(ResourceRegistryTest, MaterializeResolvesReferencesAndHandlesMissingResources)
{
    ResourceRegistry registry;
    ResourceRecordPack pack = MakePack();
    ASSERT_EQ(registry.Materialize("materialized_resource", pack), ResourceStatus::Success);
    EXPECT_TRUE(pack.filesMaterialized);
    ASSERT_NE(pack.records.at("kernel"), nullptr);
    const ResourceRecord& record = *pack.records.at("kernel");
    EXPECT_TRUE(FileUtils::IsRegularFile(record.kernel.constants.front().file));
    EXPECT_EQ(record.kernel.objects.front().commands.front().options.front(), "plain");
    EXPECT_EQ(record.kernel.linkOptions.front(), "plain-link");
    EXPECT_EQ(registry.Materialize("materialized_resource", pack), ResourceStatus::Success);

    ResourceRecordPack missingConstant = MakePack();
    missingConstant.files.erase(missingConstant.files.begin());
    EXPECT_EQ(registry.Materialize("missing_constant", missingConstant), ResourceStatus::InvalidResource);

    ResourceRecordPack missingCommand = MakePack();
    missingCommand.files.erase(missingCommand.files.begin() + 1);
    EXPECT_EQ(registry.Materialize("missing_command", missingCommand), ResourceStatus::InvalidResource);

    ResourceRecordPack missingLink = MakePack();
    missingLink.files.pop_back();
    EXPECT_EQ(registry.Materialize("missing_link", missingLink), ResourceStatus::InvalidResource);
}

TEST_F(ResourceRegistryTest, MaterializeReportsWriteFailure)
{
    const std::string blocker = FileUtils::JoinPath(root_, "blocker");
    ASSERT_TRUE(FileUtils::WriteFile(blocker, {1U}));
    ResourceRegistry registry;
    registry.temporaryRoot_ = blocker;
    ResourceRecordPack pack = MakePack();
    EXPECT_EQ(registry.Materialize("resource", pack), ResourceStatus::IoError);
    registry.temporaryRoot_.clear();
}

TEST_F(ResourceRegistryTest, LookupCoversNotFoundSocMismatchAndSuccess)
{
    ResourceRegistry registry;
    const ResourceRecord* record = reinterpret_cast<const ResourceRecord*>(1U);
    EXPECT_EQ(registry.Lookup("missing", "kernel", record), ResourceStatus::NotFound);
    EXPECT_EQ(record, nullptr);

    registry.recordPacks_.emplace("resource", MakePack());
    EXPECT_EQ(registry.Lookup("resource", "missing", record), ResourceStatus::NotFound);
    registry.recordPacks_.at("resource").records.at("kernel")->socVersion = "OtherSoc";
    EXPECT_EQ(registry.Lookup("resource", "kernel", record), ResourceStatus::NotFound);
    registry.recordPacks_.at("resource").records.at("kernel")->socVersion = "Ascend910B";
    ASSERT_EQ(registry.Lookup("resource", "kernel", record), ResourceStatus::Success);
    ASSERT_NE(record, nullptr);
    EXPECT_EQ(record->kernel.kernelName, "kernel");
}

TEST_F(ResourceRegistryTest, LoadLibraryValidatesHeadersTablesExtensionsAndSymbols)
{
    const std::vector<std::string> invalidModes = {
        "null_header",
        "bad_magic",
        "bad_version",
        "bad_size",
        "bad_flags",
        "no_manifests",
        "too_many_manifests",
        "null_manifests",
        "too_many_extensions",
        "null_extensions",
        "invalid_extension_flags",
        "invalid_extension_data",
        "required_extension",
        "null_package",
        "large_package",
    };
    for (const std::string& mode : invalidModes) {
        ASSERT_EQ(setenv("ACLRTC_UT_BUNDLE_MODE", mode.c_str(), 1), 0);
        ResourceRegistry registry;
        EXPECT_EQ(registry.LoadLibrary(kPluginPath), ResourceStatus::InvalidResource) << mode;
    }

    unsetenv("ACLRTC_UT_BUNDLE_MODE");
    ResourceRegistry noEntryRegistry;
    EXPECT_EQ(noEntryRegistry.LoadLibrary(kNoEntryPluginPath), ResourceStatus::InvalidResource);
    ResourceRegistry notElfRegistry;
    const std::string notElf = FileUtils::JoinPath(root_, "lib_bad_compile_database.so");
    ASSERT_TRUE(FileUtils::WriteFile(notElf, {'n', 'o', 't', '-', 'e', 'l', 'f'}));
    EXPECT_EQ(notElfRegistry.LoadLibrary(notElf), ResourceStatus::LoadError);

    ASSERT_EQ(setenv("ACLRTC_UT_BUNDLE_MODE", "optional_extension", 1), 0);
    ResourceRegistry optionalExtensionRegistry;
    EXPECT_EQ(optionalExtensionRegistry.LoadLibrary(kPluginPath), ResourceStatus::Success);
}

TEST_F(ResourceRegistryTest, ExplicitLoadDiscoversPluginAndPerformsLookup)
{
    ResourceRegistry registry;
    EXPECT_EQ(registry.Load(kPluginDirectory), ResourceStatus::Success);
    const ResourceRecord* record = nullptr;
    ASSERT_EQ(registry.Lookup("ut_resource", "ut_kernel", record), ResourceStatus::Success);
    ASSERT_NE(record, nullptr);
    EXPECT_TRUE(FileUtils::IsRegularFile(record->kernel.constants.front().file));
    EXPECT_NE(record->kernel.linkOptions.front().find("${resource}"), 0U);
    EXPECT_EQ(registry.Load(kPluginDirectory), ResourceStatus::InvalidResource);
}

TEST_F(ResourceRegistryTest, LoadRejectsInvalidAndEmptyExplicitDirectories)
{
    ResourceRegistry registry;
    EXPECT_EQ(registry.Load(FileUtils::JoinPath(root_, "missing")), ResourceStatus::InvalidResource);
    EXPECT_EQ(registry.Load(root_), ResourceStatus::NotFound);

    const std::string irrelevant = FileUtils::JoinPath(root_, "ordinary.so");
    ASSERT_TRUE(CopyFile(kPluginPath, irrelevant));
    EXPECT_EQ(registry.Load(root_), ResourceStatus::NotFound);
}

TEST_F(ResourceRegistryTest, AutomaticLoadReadsCustomAndVendorSearchRoots)
{
    const std::string customRoot = FileUtils::JoinPath(root_, "custom");
    const std::string customJit = FileUtils::JoinPath(customRoot, "op_impl/ai_core/tbe/kernel/jit/nested");
    const std::string customPlugin = FileUtils::JoinPath(customJit, "lib_custom_compile_database.so");
    ASSERT_TRUE(CopyFile(kPluginPath, customPlugin));

    const std::string oppRoot = FileUtils::JoinPath(root_, "opp");
    ASSERT_TRUE(
        FileUtils::WriteFile(
            FileUtils::JoinPath(oppRoot, "vendors/config.ini"),
            std::vector<uint8_t>{'o', 't', 'h', 'e', 'r', '=', 'x', '\n', ' ', 'l', 'o', 'a', 'd', '_',
                                 'p', 'r', 'i', 'o', 'r', 'i', 't', 'y',  ' ', '=', ' ', 'v', 'e', 'n',
                                 'd', 'o', 'r', 'A', ',', ' ', 'v', 'e',  'n', 'd', 'o', 'r', 'A', '\n'}));
    ASSERT_EQ(setenv("ASCEND_CUSTOM_OPP_PATH", ("  " + customRoot + ":" + customRoot).c_str(), 1), 0);
    ASSERT_EQ(setenv("ASCEND_OPP_PATH", oppRoot.c_str(), 1), 0);

    ResourceRegistry registry;
    EXPECT_EQ(registry.Load(""), ResourceStatus::Success);
    const ResourceRecord* record = nullptr;
    EXPECT_EQ(registry.Lookup("ut_resource", "ut_kernel", record), ResourceStatus::Success);
}

TEST_F(ResourceRegistryTest, AutomaticLoadSkipsUnavailableAndSymlinkRoots)
{
    ResourceRegistry noEnvironmentRegistry;
    EXPECT_EQ(noEnvironmentRegistry.Load(""), ResourceStatus::NotFound);

    const std::string actualRoot = FileUtils::JoinPath(root_, "actual");
    ASSERT_TRUE(FileUtils::WriteFile(FileUtils::JoinPath(actualRoot, "marker"), {}));
    const std::string customRoot = FileUtils::JoinPath(root_, "symlink_custom");
    const std::string jitParent = FileUtils::JoinPath(customRoot, "op_impl/ai_core/tbe/kernel");
    ASSERT_TRUE(FileUtils::WriteFile(FileUtils::JoinPath(jitParent, "marker"), {}));
    const std::string rootLink = FileUtils::JoinPath(jitParent, "jit");
    ASSERT_EQ(symlink(actualRoot.c_str(), rootLink.c_str()), 0);
    ASSERT_EQ(setenv("ASCEND_CUSTOM_OPP_PATH", customRoot.c_str(), 1), 0);
    ResourceRegistry symlinkRegistry;
    EXPECT_EQ(symlinkRegistry.Load(""), ResourceStatus::NotFound);
}

} // namespace
} // namespace resource
} // namespace ascendc
