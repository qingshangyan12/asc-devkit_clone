/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <dlfcn.h>
#include <unistd.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "ascendc_compile_resource/ascendc_manifest_abi.h"
#include "manifest_generator.h"

namespace {

namespace fs = std::filesystem;
using ascendc::manifest_generator::BuildCollectedBundleRequest;
using ascendc::manifest_generator::ManifestBundleCompiler;

struct SavedEnvironment {
    bool exists = false;
    std::string value;
};

SavedEnvironment SaveEnvironment(const char *name)
{
    const char *value = std::getenv(name);
    return {value != nullptr, value == nullptr ? std::string{} : std::string(value)};
}

void RestoreEnvironment(const char *name, const SavedEnvironment &saved) noexcept
{
    if (saved.exists) {
        (void)setenv(name, saved.value.c_str(), 1);
    } else {
        (void)unsetenv(name);
    }
}

void WriteFile(const fs::path &path, const std::string &content)
{
    std::error_code error;
    fs::create_directories(path.parent_path(), error);
    if (error) {
        throw std::runtime_error("failed to create test directory: " + error.message());
    }
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        throw std::runtime_error("failed to create test file: " + path.string());
    }
    output.write(content.data(), static_cast<std::streamsize>(content.size()));
    output.close();
    if (!output) {
        throw std::runtime_error("failed to write test file: " + path.string());
    }
}

std::string ReadFile(const fs::path &path)
{
    std::ifstream input(path, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}

std::string ToString(const AcString &value)
{
    return std::string(value.data, value.size);
}

class AscCompileExporterTest : public testing::Test {
protected:
    void SetUp() override
    {
        ascendHome_ = SaveEnvironment("ASCEND_HOME_PATH");
        compiler_ = SaveEnvironment("CXX");

        std::string pathTemplate = (fs::temp_directory_path() / "asc_compile_exporter_ut_XXXXXX").string();
        std::vector<char> buffer(pathTemplate.begin(), pathTemplate.end());
        buffer.push_back('\0');
        char *created = mkdtemp(buffer.data());
        ASSERT_NE(created, nullptr);
        root_ = created;

        ASSERT_EQ(setenv("ASCEND_HOME_PATH", ASC_COMPILE_EXPORTER_TEST_CANN_HOME, 1), 0);
        ASSERT_EQ(setenv("CXX", ASC_COMPILE_EXPORTER_TEST_CXX, 1), 0);
    }

    void TearDown() override
    {
        RestoreEnvironment("CXX", compiler_);
        RestoreEnvironment("ASCEND_HOME_PATH", ascendHome_);
        std::error_code ignored;
        fs::remove_all(root_, ignored);
    }

    BuildCollectedBundleRequest Request(const std::string &packageId = "test_package") const
    {
        return {packageId, root_ / "collection", root_ / "output/bundle.so",
                root_ / "work", false, 1U};
    }

    void CreateManifest(const fs::path &unit, const std::string &name,
                        const std::string &baseDir = "resources") const
    {
        WriteFile(unit / (name + "_manifest.json"),
                  "{\"base_dir\":\"" + baseDir + "\",\"name\":\"" + name + "\"}");
    }

    static void ExpectCompileError(const BuildCollectedBundleRequest &request,
                                   const std::string &message)
    {
        try {
            ManifestBundleCompiler(request).Compile();
            FAIL() << "expected manifest bundle compilation to fail";
        } catch (const std::runtime_error &error) {
            EXPECT_NE(std::string(error.what()).find(message), std::string::npos) << error.what();
        }
    }

    fs::path root_;

private:
    SavedEnvironment ascendHome_;
    SavedEnvironment compiler_;
};

TEST_F(AscCompileExporterTest, RejectsEmptyPackageId)
{
    fs::create_directories(root_ / "collection");
    ExpectCompileError(Request(""), "package ID must not be empty");
}

TEST_F(AscCompileExporterTest, RejectsInvalidManifestSearchRoot)
{
    ExpectCompileError(Request(), "manifest search root is not a directory");
}

TEST_F(AscCompileExporterTest, RejectsOutputWithoutParentDirectory)
{
    fs::create_directories(root_ / "collection");
    BuildCollectedBundleRequest request = Request();
    request.outputSo = "bundle.so";
    ExpectCompileError(request, "output path must include a parent directory");
}

TEST_F(AscCompileExporterTest, RejectsMissingManifestWithoutReplacingOutput)
{
    fs::create_directories(root_ / "collection");
    const BuildCollectedBundleRequest request = Request();
    WriteFile(request.outputSo, "previous bundle");

    ExpectCompileError(request, "contain no manifest units");
    EXPECT_EQ(ReadFile(request.outputSo), "previous bundle");
}

TEST_F(AscCompileExporterTest, RejectsMalformedJsonWithoutReplacingOutput)
{
    const fs::path unit = root_ / "collection/Malformed";
    WriteFile(unit / "resources/kernel.cpp", "source");
    WriteFile(unit / "Malformed_manifest.json", "{\"base_dir\":\"resources\"");
    const BuildCollectedBundleRequest request = Request();
    WriteFile(request.outputSo, "previous bundle");

    ExpectCompileError(request, "manifest is not valid JSON");
    EXPECT_EQ(ReadFile(request.outputSo), "previous bundle");
}

TEST_F(AscCompileExporterTest, RejectsBaseDirectoryOutsideManifestDirectory)
{
    const fs::path unit = root_ / "collection/Traversal";
    WriteFile(root_ / "collection/outside/kernel.cpp", "source");
    CreateManifest(unit, "Traversal", "../outside");

    ExpectCompileError(Request(), "is not a directory below the manifest");
}

TEST_F(AscCompileExporterTest, RejectsSymlinkInResourceDirectory)
{
    const fs::path unit = root_ / "collection/Symlink";
    WriteFile(unit / "resources/kernel.cpp", "source");
    WriteFile(root_ / "outside.cpp", "outside");
    fs::create_symlink(root_ / "outside.cpp", unit / "resources/link.cpp");
    CreateManifest(unit, "Symlink");

    ExpectCompileError(Request(), "base_dir contains a symlink");
}

TEST_F(AscCompileExporterTest, RejectsOversizedResourceWithoutReplacingOutput)
{
    const fs::path unit = root_ / "collection/Large";
    WriteFile(unit / "resources/kernel.cpp", "");
    fs::resize_file(unit / "resources/kernel.cpp", 1U * 1024U * 1024U + 1U);
    CreateManifest(unit, "Large");
    const BuildCollectedBundleRequest request = Request();
    WriteFile(request.outputSo, "previous bundle");

    ExpectCompileError(request, "resource exceeds size limit");
    EXPECT_EQ(ReadFile(request.outputSo), "previous bundle");
}

TEST_F(AscCompileExporterTest, BuildsLoadableBundleAndCleansTemporaryWorkspace)
{
    const fs::path first = root_ / "collection/nested/First";
    const fs::path second = root_ / "collection/Second";
    WriteFile(first / "resources/kernel.cpp", "first source");
    WriteFile(first / "resources/include/nested.h", "nested header");
    WriteFile(first / "resources/empty.bin", "");
    WriteFile(second / "resources/kernel.cpp", "second source");
    CreateManifest(first, "First");
    CreateManifest(second, "Second");

    const BuildCollectedBundleRequest request = Request("multi_manifest");
    ManifestBundleCompiler(request).Compile();

    ASSERT_TRUE(fs::is_regular_file(request.outputSo));
    ASSERT_TRUE(fs::is_directory(request.workDir));
    for (const fs::directory_entry &entry : fs::directory_iterator(request.workDir)) {
        EXPECT_NE(entry.path().filename().string().rfind(".manifest-generator-", 0U), 0U);
    }

    void *handle = dlopen(request.outputSo.c_str(), RTLD_NOW | RTLD_LOCAL);
    const char *loadError = handle == nullptr ? dlerror() : nullptr;
    ASSERT_NE(handle, nullptr) << (loadError == nullptr ? "dlopen failed" : loadError);
    auto getter = reinterpret_cast<AscendcGetCompileResourceBundleFn>(
        dlsym(handle, "AscendcGetCompileResourceBundle"));
    ASSERT_NE(getter, nullptr);
    const AcCompileResourceBundleHeader *header = getter();
    ASSERT_NE(header, nullptr);
    EXPECT_EQ(header->magic, AC_COMPILE_RESOURCE_MAGIC);
    EXPECT_EQ(header->abi_version, AC_COMPILE_RESOURCE_ABI_VERSION);
    EXPECT_EQ(header->struct_size, sizeof(AcCompileResourceBundle));
    EXPECT_EQ(header->flags, 0U);

    const auto *bundle = reinterpret_cast<const AcCompileResourceBundle *>(header);
    EXPECT_EQ(ToString(bundle->package_id), "multi_manifest");
    ASSERT_EQ(bundle->manifest_count, 2U);
    EXPECT_EQ(bundle->extensions, nullptr);
    EXPECT_EQ(bundle->extension_count, 0U);

    uint64_t totalFiles = 0U;
    bool foundNestedHeader = false;
    bool foundEmptyFile = false;
    bool foundFirstSource = false;
    for (uint64_t manifestIndex = 0U; manifestIndex < bundle->manifest_count; ++manifestIndex) {
        const AcCompileResourceManifest &manifest = bundle->manifests[manifestIndex];
        totalFiles += manifest.file_count;
        for (uint64_t fileIndex = 0U; fileIndex < manifest.file_count; ++fileIndex) {
            const AcCompileResourceFile &file = manifest.files[fileIndex];
            const std::string path = ToString(file.file_path);
            foundNestedHeader = foundNestedHeader || path == "resources/include/nested.h";
            foundEmptyFile = foundEmptyFile || path == "resources/empty.bin";
            foundFirstSource = foundFirstSource ||
                (path == "resources/kernel.cpp" &&
                 std::string(reinterpret_cast<const char *>(file.data), file.size) == "first source");
        }
    }
    EXPECT_EQ(totalFiles, 3U);
    EXPECT_TRUE(foundNestedHeader);
    EXPECT_FALSE(foundEmptyFile);
    EXPECT_TRUE(foundFirstSource);
    EXPECT_EQ(dlclose(handle), 0);
}

} // namespace
