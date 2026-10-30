/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the LICENSE.
 */

#include <gtest/gtest.h>

#include <string>
#include <fstream>
#include <vector>

#include <sys/stat.h>
#include <unistd.h>

#include "collected_manifest_repository.h"
#include "file_utils.h"
#include "nlohmann/json.hpp"
#include "test_support.h"

namespace ascendc {
namespace manifest_generator {
namespace {

constexpr size_t TEST_MAX_DIRECTORY_DEPTH = 64U;

using Json = nlohmann::json;
using asc_compile_exporter_test::ModuleTest;
using asc_compile_exporter_test::ReadTestFile;
using asc_compile_exporter_test::WriteTestFile;

TEST_F(ModuleTest, RepositorySortsManifestsAndResourcesAndSkipsEmptyFiles)
{
    const std::string first = FileUtils::JoinPath(root_, "collection/z/First");
    const std::string second = FileUtils::JoinPath(root_, "collection/a/Second");
    WriteTestFile(FileUtils::JoinPath(first, "resources/z.cpp"), "z");
    WriteTestFile(FileUtils::JoinPath(first, "resources/a.cpp"), "a");
    WriteTestFile(FileUtils::JoinPath(first, "resources/empty.cpp"), "");
    WriteTestFile(FileUtils::JoinPath(second, "resources/kernel.cpp"), "second");
    CreateManifest(first, "First");
    CreateManifest(second, "Second");
    WriteTestFile(FileUtils::JoinPath(root_, "collection/ignored.json"), "not a manifest");

    std::vector<ManifestUnit> units;
    ASSERT_TRUE(CollectedManifestRepository(FileUtils::JoinPath(root_, "collection")).Load(units));

    ASSERT_EQ(units.size(), 2U);
    EXPECT_NE(units[0].json.find("Second"), std::string::npos);
    EXPECT_NE(units[1].json.find("First"), std::string::npos);
    ASSERT_EQ(units[1].files.size(), 2U);
    EXPECT_EQ(units[1].files[0].fileName, "a.cpp");
    EXPECT_EQ(units[1].files[0].filePath, "resources/a.cpp");
    EXPECT_EQ(units[1].files[1].filePath, "resources/z.cpp");
    EXPECT_EQ(std::string(units[1].files[0].data.begin(), units[1].files[0].data.end()), "a");
}

TEST_F(ModuleTest, RepositoryAddsSchemaVersionToManifestJson)
{
    const std::string unit = FileUtils::JoinPath(root_, "collection-schema-version/Unit");
    WriteTestFile(FileUtils::JoinPath(unit, "resources/kernel.cpp"), "source");
    CreateManifest(unit, "Unit");

    std::vector<ManifestUnit> units;
    ASSERT_TRUE(CollectedManifestRepository(FileUtils::JoinPath(root_, "collection-schema-version")).Load(units));

    ASSERT_EQ(units.size(), 1U);
    const Json manifest = Json::parse(units[0].json);
    ASSERT_TRUE(manifest.contains("schema_version"));
    EXPECT_EQ(manifest.at("schema_version"), "1.0");
}

TEST_F(ModuleTest, RepositoryDeduplicatesEquivalentResourceIds)
{
    const std::string collection = FileUtils::JoinPath(root_, "duplicates");
    const std::string first = FileUtils::JoinPath(collection, "a");
    const std::string second = FileUtils::JoinPath(collection, "b");
    // Different creation order and JSON formatting do not change the resource.
    WriteTestFile(FileUtils::JoinPath(first, "resources/z.h"), "z");
    WriteTestFile(FileUtils::JoinPath(first, "resources/a.h"), "a");
    WriteTestFile(FileUtils::JoinPath(second, "resources/a.h"), "a");
    WriteTestFile(FileUtils::JoinPath(second, "resources/z.h"), "z");
    CreateManifest(first, "Same");
    CreateManifest(second, "Same");
    const std::string path = FileUtils::JoinPath(second, "Same_manifest.json");
    WriteTestFile(path, Json::parse(ReadTestFile(path)).dump(4));

    std::vector<ManifestUnit> units;
    ASSERT_TRUE(CollectedManifestRepository(collection).Load(units));
    ASSERT_EQ(units.size(), 1U);
    EXPECT_EQ(units[0].files.size(), 2U);
}

TEST_F(ModuleTest, RepositoryKeepsFirstDuplicateIdDespiteContentDifferences)
{
    for (const std::string difference : {"manifest", "bytes", "path", "empty-directory", "empty-file"}) {
        SCOPED_TRACE(difference);
        const std::string collection = FileUtils::JoinPath(root_, difference);
        const std::string first = FileUtils::JoinPath(collection, "a");
        const std::string second = FileUtils::JoinPath(collection, "b");
        WriteTestFile(FileUtils::JoinPath(first, "resources/kernel.cpp"), "source");
        WriteTestFile(FileUtils::JoinPath(second, "resources/kernel.cpp"), "source");
        CreateManifest(first, "Same");
        CreateManifest(second, "Same");
        if (difference == "manifest") {
            const std::string path = FileUtils::JoinPath(second, "Same_manifest.json");
            Json manifest = Json::parse(ReadTestFile(path));
            manifest["soc_version"] = "different";
            WriteTestFile(path, manifest.dump());
        } else if (difference == "bytes") {
            WriteTestFile(FileUtils::JoinPath(second, "resources/kernel.cpp"), "changed");
        } else if (difference == "path") {
            ASSERT_EQ(
                rename(
                    FileUtils::JoinPath(second, "resources/kernel.cpp").c_str(),
                    FileUtils::JoinPath(second, "resources/renamed.cpp").c_str()),
                0);
        } else if (difference == "empty-directory") {
            ASSERT_TRUE(FileUtils::CreateDirectories(FileUtils::JoinPath(second, "resources/include")));
        } else {
            WriteTestFile(FileUtils::JoinPath(second, "resources/empty.h"), "");
        }
        const std::string third = FileUtils::JoinPath(collection, "c");
        WriteTestFile(FileUtils::JoinPath(third, "resources/unique.cpp"), "unique");
        CreateManifest(third, "Unique");
        std::vector<ManifestUnit> units;
        ASSERT_TRUE(CollectedManifestRepository(collection).Load(units));
        ASSERT_EQ(units.size(), 2U);
        ASSERT_EQ(units[0].files.size(), 1U);
        EXPECT_EQ(units[0].files[0].filePath, "resources/kernel.cpp");
        EXPECT_EQ(std::string(units[0].files[0].data.begin(), units[0].files[0].data.end()), "source");
    }
}

TEST_F(ModuleTest, RepositoryRejectsManifestWithSchemaVersion)
{
    const std::string collection = FileUtils::JoinPath(root_, "collection-existing-schema-version");
    const std::string unit = FileUtils::JoinPath(collection, "Unit");
    WriteTestFile(FileUtils::JoinPath(unit, "resources/kernel.cpp"), "source");
    CreateManifest(unit, "Unit");
    const std::string manifestPath = FileUtils::JoinPath(unit, "Unit_manifest.json");
    Json manifest = Json::parse(ReadTestFile(manifestPath));
    manifest["schema_version"] = "2.0";
    WriteTestFile(manifestPath, manifest.dump());

    std::vector<ManifestUnit> units;
    EXPECT_FALSE(CollectedManifestRepository(collection).Load(units));
}

TEST_F(ModuleTest, RepositoryReportsManifestJsonAndResourcePathErrors)
{
    const std::string malformed = FileUtils::JoinPath(root_, "malformed/Unit");
    WriteTestFile(FileUtils::JoinPath(malformed, "resources/kernel.cpp"), "source");
    WriteTestFile(FileUtils::JoinPath(malformed, "Unit_manifest.json"), "{\"resource_path\":");
    std::vector<ManifestUnit> units;
    EXPECT_FALSE(CollectedManifestRepository(FileUtils::JoinPath(root_, "malformed")).Load(units));

    const std::string missing = FileUtils::JoinPath(root_, "missing-resource-path/Unit");
    WriteTestFile(FileUtils::JoinPath(missing, "resources/kernel.cpp"), "source");
    CreateManifest(missing, "Unit");
    std::string missingResourcePath = ReadTestFile(FileUtils::JoinPath(missing, "Unit_manifest.json"));
    const std::string field = "\"resource_path\":\"resources\",";
    missingResourcePath.erase(missingResourcePath.find(field), field.size());
    WriteTestFile(FileUtils::JoinPath(missing, "Unit_manifest.json"), missingResourcePath);
    EXPECT_FALSE(CollectedManifestRepository(FileUtils::JoinPath(root_, "missing-resource-path")).Load(units));

    const std::string wrongType = FileUtils::JoinPath(root_, "wrong-type/Unit");
    WriteTestFile(FileUtils::JoinPath(wrongType, "resources/kernel.cpp"), "source");
    CreateManifest(wrongType, "Unit");
    std::string wrongResourcePath = ReadTestFile(FileUtils::JoinPath(wrongType, "Unit_manifest.json"));
    const size_t value = wrongResourcePath.find("\"resources\"");
    wrongResourcePath.replace(value, std::string("\"resources\"").size(), "7");
    WriteTestFile(FileUtils::JoinPath(wrongType, "Unit_manifest.json"), wrongResourcePath);
    EXPECT_FALSE(CollectedManifestRepository(FileUtils::JoinPath(root_, "wrong-type")).Load(units));
}

TEST_F(ModuleTest, RepositorySkipsSymlinkedManifests)
{
    const std::string collection = FileUtils::JoinPath(root_, "collection-symlink");
    const std::string unit = FileUtils::JoinPath(collection, "Unit");
    WriteTestFile(FileUtils::JoinPath(unit, "resources/kernel.cpp"), "source");
    CreateManifest(unit, "Unit");

    const std::string ignoredManifest = FileUtils::JoinPath(root_, "ignored_manifest.json");
    WriteTestFile(ignoredManifest, "ignored");
    ASSERT_EQ(symlink(ignoredManifest.c_str(), FileUtils::JoinPath(collection, "Link_manifest.json").c_str()), 0);

    std::vector<ManifestUnit> units;
    ASSERT_TRUE(CollectedManifestRepository(collection).Load(units));
    ASSERT_EQ(units.size(), 1U);
    EXPECT_NE(units[0].json.find("Unit"), std::string::npos);
}

TEST_F(ModuleTest, RepositoryLoadsManifestWithNoResourceFiles)
{
    const std::string collection = FileUtils::JoinPath(root_, "collection-empty-resource-path");
    const std::string unit = FileUtils::JoinPath(collection, "Unit");
    CreateManifest(unit, "Unit");
    std::string manifest = ReadTestFile(FileUtils::JoinPath(unit, "Unit_manifest.json"));
    const std::string resourcePath = "\"resource_path\":\"resources\"";
    manifest.replace(manifest.find(resourcePath), resourcePath.size(), "\"resource_path\":\"\"");
    WriteTestFile(FileUtils::JoinPath(unit, "Unit_manifest.json"), manifest);

    std::vector<ManifestUnit> units;
    ASSERT_TRUE(CollectedManifestRepository(collection).Load(units));
    ASSERT_EQ(units.size(), 1U);
    EXPECT_TRUE(units[0].files.empty());
}

TEST_F(ModuleTest, RepositoryRejectsAbsoluteResourcePath)
{
    const std::string collection = FileUtils::JoinPath(root_, "collection-absolute");
    const std::string unit = FileUtils::JoinPath(collection, "Unit");
    const std::string resources = FileUtils::JoinPath(unit, "resources");
    WriteTestFile(FileUtils::JoinPath(resources, "kernel.cpp"), "source");
    CreateManifest(unit, "Unit");
    std::string absoluteResourcePath = ReadTestFile(FileUtils::JoinPath(unit, "Unit_manifest.json"));
    const size_t value = absoluteResourcePath.find("resources");
    absoluteResourcePath.replace(value, std::string("resources").size(), resources);
    WriteTestFile(FileUtils::JoinPath(unit, "Unit_manifest.json"), absoluteResourcePath);

    std::vector<ManifestUnit> units;
    EXPECT_FALSE(CollectedManifestRepository(collection).Load(units));
}

TEST_F(ModuleTest, RepositoryRejectsNonRegularResource)
{
    const std::string unit = FileUtils::JoinPath(root_, "collection-special/Unit");
    const std::string resources = FileUtils::JoinPath(unit, "resources");
    ASSERT_TRUE(FileUtils::CreateDirectories(resources));
    ASSERT_EQ(mkfifo(FileUtils::JoinPath(resources, "resource.pipe").c_str(), 0600), 0);
    CreateManifest(unit, "Unit");

    std::vector<ManifestUnit> units;
    EXPECT_FALSE(CollectedManifestRepository(FileUtils::JoinPath(root_, "collection-special")).Load(units));
}

TEST_F(ModuleTest, RepositoryEnforcesDirectoryDepthLimit)
{
    const std::string allowedCollection = FileUtils::JoinPath(root_, "collection-depth-allowed");
    std::string allowedDirectory = allowedCollection;
    for (size_t depth = 0U; depth + 1U < TEST_MAX_DIRECTORY_DEPTH; ++depth) {
        allowedDirectory = FileUtils::JoinPath(allowedDirectory, "level-" + std::to_string(depth));
    }
    WriteTestFile(FileUtils::JoinPath(allowedDirectory, "resources/kernel.cpp"), "source");
    CreateManifest(allowedDirectory, "Deep");
    std::vector<ManifestUnit> units;
    EXPECT_TRUE(CollectedManifestRepository(allowedCollection).Load(units));

    const std::string rejectedCollection = FileUtils::JoinPath(root_, "collection-depth-rejected");
    std::string rejectedDirectory = rejectedCollection;
    for (size_t depth = 0U; depth <= TEST_MAX_DIRECTORY_DEPTH; ++depth) {
        rejectedDirectory = FileUtils::JoinPath(rejectedDirectory, "level-" + std::to_string(depth));
    }
    ASSERT_TRUE(FileUtils::CreateDirectories(rejectedDirectory));
    EXPECT_FALSE(CollectedManifestRepository(rejectedCollection).Load(units));
}

TEST_F(ModuleTest, RepositoryReportsDirectoryOpenFailure)
{
    std::vector<ManifestUnit> units;
    EXPECT_FALSE(CollectedManifestRepository("/dev/null").Load(units));
}

TEST_F(ModuleTest, RepositoryAcceptsResourceAboveLegacyOneMiBLimit)
{
    const std::string unit = FileUtils::JoinPath(root_, "large/Unit");
    WriteTestFile(FileUtils::JoinPath(unit, "resources/large.h"), std::string(1024U * 1024U + 1U, 'x'));
    CreateManifest(unit, "Unit");
    std::vector<ManifestUnit> units;
    ASSERT_TRUE(CollectedManifestRepository(FileUtils::JoinPath(root_, "large")).Load(units));
    ASSERT_EQ(units.size(), 1U);
    ASSERT_EQ(units[0].files.size(), 1U);
    EXPECT_EQ(units[0].files[0].data.size(), 1024U * 1024U + 1U);
}

TEST_F(ModuleTest, RepositoryRejectsResourceAboveConsumerFileLimit)
{
    const std::string unit = FileUtils::JoinPath(root_, "oversized/Unit");
    ASSERT_TRUE(FileUtils::CreateDirectories(FileUtils::JoinPath(unit, "resources")));
    {
        std::ofstream resource(FileUtils::JoinPath(unit, "resources/large.h"), std::ios::binary);
        ASSERT_TRUE(resource.is_open());
        resource.seekp(256U * 1024U * 1024U);
        resource.put('x');
        ASSERT_TRUE(resource.good());
    }
    CreateManifest(unit, "Unit");
    std::vector<ManifestUnit> units;
    EXPECT_FALSE(CollectedManifestRepository(FileUtils::JoinPath(root_, "oversized")).Load(units));
}

} // namespace
} // namespace manifest_generator
} // namespace ascendc
