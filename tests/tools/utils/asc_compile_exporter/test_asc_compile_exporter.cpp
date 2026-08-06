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
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "ascendc_compile_resource/ascendc_manifest_abi.h"
#include "manifest_generator.h"

namespace {

using ascendc::manifest_generator::BuildCollectedBundleRequest;
using ascendc::manifest_generator::ManifestBundleCompiler;

struct SavedEnvironment {
    SavedEnvironment() : exists(false) {}

    SavedEnvironment(bool existsValue, const std::string& valueValue) : exists(existsValue), value(valueValue) {}

    bool exists;
    std::string value;
};

SavedEnvironment SaveEnvironment(const char* name)
{
    const char* value = std::getenv(name);
    return SavedEnvironment(value != nullptr, value == nullptr ? std::string() : std::string(value));
}

void RestoreEnvironment(const char* name, const SavedEnvironment& saved) noexcept
{
    if (saved.exists) {
        (void)setenv(name, saved.value.c_str(), 1);
    } else {
        (void)unsetenv(name);
    }
}

std::string JoinTestPath(const std::string& left, const std::string& right)
{
    if (left.empty()) {
        return right;
    }
    return left[left.size() - 1U] == '/' ? left + right : left + "/" + right;
}

std::string ParentTestPath(const std::string& path)
{
    const size_t separator = path.find_last_of('/');
    if (separator == std::string::npos) {
        return std::string();
    }
    return separator == 0U ? "/" : path.substr(0U, separator);
}

bool IsTestDirectory(const std::string& path)
{
    struct stat info {};
    return stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode);
}

bool IsTestRegularFile(const std::string& path)
{
    struct stat info {};
    return stat(path.c_str(), &info) == 0 && S_ISREG(info.st_mode);
}

void CreateTestDirectories(const std::string& path)
{
    if (path.empty() || path == "/" || IsTestDirectory(path)) {
        return;
    }
    CreateTestDirectories(ParentTestPath(path));
    if (mkdir(path.c_str(), 0700) != 0 && !(errno == EEXIST && IsTestDirectory(path))) {
        throw std::runtime_error("failed to create test directory " + path + ": " + std::strerror(errno));
    }
}

void RemoveTestTree(const std::string& path) noexcept
{
    struct stat info {};
    if (lstat(path.c_str(), &info) != 0) {
        return;
    }
    if (!S_ISDIR(info.st_mode) || S_ISLNK(info.st_mode)) {
        (void)unlink(path.c_str());
        return;
    }
    DIR* directory = opendir(path.c_str());
    if (directory != nullptr) {
        while (dirent* entry = readdir(directory)) {
            const std::string name(entry->d_name);
            if (name != "." && name != "..") {
                RemoveTestTree(JoinTestPath(path, name));
            }
        }
        (void)closedir(directory);
    }
    (void)rmdir(path.c_str());
}

std::vector<std::string> ListTestDirectory(const std::string& path)
{
    DIR* directory = opendir(path.c_str());
    if (directory == nullptr) {
        throw std::runtime_error("failed to enumerate test directory: " + path);
    }
    std::vector<std::string> names;
    while (dirent* entry = readdir(directory)) {
        const std::string name(entry->d_name);
        if (name != "." && name != "..") {
            names.push_back(name);
        }
    }
    (void)closedir(directory);
    return names;
}

void WriteFile(const std::string& path, const std::string& content)
{
    CreateTestDirectories(ParentTestPath(path));
    std::ofstream output(path.c_str(), std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        throw std::runtime_error("failed to create test file: " + path);
    }
    output.write(content.data(), static_cast<std::streamsize>(content.size()));
    output.close();
    if (!output) {
        throw std::runtime_error("failed to write test file: " + path);
    }
}

std::string ReadFile(const std::string& path)
{
    std::ifstream input(path.c_str(), std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}

std::string ToString(const AcString& value) { return std::string(value.data, value.size); }

class AscCompileExporterTest : public testing::Test {
protected:
    void SetUp() override
    {
        ascendHome_ = SaveEnvironment("ASCEND_HOME_PATH");

        const char* temporary = std::getenv("TMPDIR");
        std::string pathTemplate = JoinTestPath(
            temporary == nullptr || temporary[0] == '\0' ? "/tmp" : temporary, "asc_compile_exporter_ut_XXXXXX");
        std::vector<char> buffer(pathTemplate.begin(), pathTemplate.end());
        buffer.push_back('\0');
        char* created = mkdtemp(buffer.data());
        ASSERT_NE(created, nullptr);
        root_ = created;

        ASSERT_EQ(setenv("ASCEND_HOME_PATH", ASC_COMPILE_EXPORTER_TEST_CANN_HOME, 1), 0);
    }

    void TearDown() override
    {
        RestoreEnvironment("ASCEND_HOME_PATH", ascendHome_);
        RemoveTestTree(root_);
    }

    BuildCollectedBundleRequest Request(const std::string& packageId = "test_package") const
    {
        BuildCollectedBundleRequest request;
        request.packageId = packageId;
        request.manifestSearchRoot = JoinTestPath(root_, "collection");
        request.outputSo = JoinTestPath(root_, "output/bundle.so");
        request.workDir = JoinTestPath(root_, "work");
        request.keepTemp = false;
        request.jobs = 1U;
        return request;
    }

    void CreateManifest(
        const std::string& unit, const std::string& name, const std::string& baseDir = "resources") const
    {
        WriteFile(
            JoinTestPath(unit, name + "_manifest.json"),
            "{\"base_dir\":\"" + baseDir + "\",\"name\":\"" + name + "\"}");
    }

    static void ExpectCompileError(const BuildCollectedBundleRequest& request, const std::string& message)
    {
        try {
            ManifestBundleCompiler(request).Compile();
            FAIL() << "expected manifest bundle compilation to fail";
        } catch (const std::runtime_error& error) {
            EXPECT_NE(std::string(error.what()).find(message), std::string::npos) << error.what();
        }
    }

    std::string root_;

private:
    SavedEnvironment ascendHome_;
};

TEST_F(AscCompileExporterTest, RejectsEmptyPackageId)
{
    CreateTestDirectories(JoinTestPath(root_, "collection"));
    ExpectCompileError(Request(""), "package ID must not be empty");
}

TEST_F(AscCompileExporterTest, RejectsInvalidManifestSearchRoot)
{
    ExpectCompileError(Request(), "manifest search root must exist");
}

TEST_F(AscCompileExporterTest, RejectsOutputWithoutParentDirectory)
{
    CreateTestDirectories(JoinTestPath(root_, "collection"));
    BuildCollectedBundleRequest request = Request();
    request.outputSo = "bundle.so";
    ExpectCompileError(request, "output path must include a parent directory");
}

TEST_F(AscCompileExporterTest, RejectsEmptyAndInvalidOutputPaths)
{
    CreateTestDirectories(JoinTestPath(root_, "collection"));
    BuildCollectedBundleRequest emptyOutput = Request();
    emptyOutput.outputSo.clear();
    ExpectCompileError(emptyOutput, "output path must not be empty");

    BuildCollectedBundleRequest invalidOutput = Request();
    invalidOutput.outputSo = JoinTestPath(root_, "output/");
    ExpectCompileError(invalidOutput, "output path has an invalid file name");
}

TEST_F(AscCompileExporterTest, RejectsMissingManifestWithoutReplacingOutput)
{
    CreateTestDirectories(JoinTestPath(root_, "collection"));
    const BuildCollectedBundleRequest request = Request();
    WriteFile(request.outputSo, "previous bundle");

    ExpectCompileError(request, "contain no manifest units");
    EXPECT_EQ(ReadFile(request.outputSo), "previous bundle");
}

TEST_F(AscCompileExporterTest, RejectsMalformedJsonWithoutReplacingOutput)
{
    const std::string unit = JoinTestPath(root_, "collection/Malformed");
    WriteFile(JoinTestPath(unit, "resources/kernel.cpp"), "source");
    WriteFile(JoinTestPath(unit, "Malformed_manifest.json"), "{\"base_dir\":\"resources\"");
    const BuildCollectedBundleRequest request = Request();
    WriteFile(request.outputSo, "previous bundle");

    ExpectCompileError(request, "manifest is not valid JSON");
    EXPECT_EQ(ReadFile(request.outputSo), "previous bundle");
}

TEST_F(AscCompileExporterTest, RejectsBaseDirectoryOutsideManifestDirectory)
{
    const std::string unit = JoinTestPath(root_, "collection/Traversal");
    WriteFile(JoinTestPath(root_, "collection/outside/kernel.cpp"), "source");
    CreateManifest(unit, "Traversal", "../outside");

    ExpectCompileError(Request(), "is not a directory below the manifest");
}

TEST_F(AscCompileExporterTest, RejectsSymlinkInResourceDirectory)
{
    const std::string unit = JoinTestPath(root_, "collection/Symlink");
    WriteFile(JoinTestPath(unit, "resources/kernel.cpp"), "source");
    const std::string outside = JoinTestPath(root_, "outside.cpp");
    WriteFile(outside, "outside");
    ASSERT_EQ(symlink(outside.c_str(), JoinTestPath(unit, "resources/link.cpp").c_str()), 0);
    CreateManifest(unit, "Symlink");

    ExpectCompileError(Request(), "base_dir contains a symlink");
}

TEST_F(AscCompileExporterTest, RejectsOversizedResourceWithoutReplacingOutput)
{
    const std::string unit = JoinTestPath(root_, "collection/Large");
    const std::string resource = JoinTestPath(unit, "resources/kernel.cpp");
    WriteFile(resource, "");
    ASSERT_EQ(truncate(resource.c_str(), 1U * 1024U * 1024U + 1U), 0);
    CreateManifest(unit, "Large");
    const BuildCollectedBundleRequest request = Request();
    WriteFile(request.outputSo, "previous bundle");

    ExpectCompileError(request, "not a permitted regular file");
    EXPECT_EQ(ReadFile(request.outputSo), "previous bundle");
}

TEST_F(AscCompileExporterTest, BuildsLoadableBundleAndCleansTemporaryWorkspace)
{
    const std::string first = JoinTestPath(root_, "collection/nested/First");
    const std::string second = JoinTestPath(root_, "collection/Second");
    WriteFile(JoinTestPath(first, "resources/kernel.cpp"), "first source");
    WriteFile(JoinTestPath(first, "resources/include/nested.h"), "nested header");
    WriteFile(JoinTestPath(first, "resources/empty.bin"), "");
    WriteFile(JoinTestPath(second, "resources/kernel.cpp"), "second source");
    CreateManifest(first, "First");
    CreateManifest(second, "Second");

    const BuildCollectedBundleRequest request = Request("multi_manifest");
    ManifestBundleCompiler(request).Compile();

    ASSERT_TRUE(IsTestRegularFile(request.outputSo));
    ASSERT_TRUE(IsTestDirectory(request.workDir));
    const std::vector<std::string> workEntries = ListTestDirectory(request.workDir);
    for (size_t index = 0U; index < workEntries.size(); ++index) {
        EXPECT_NE(workEntries[index].find(".manifest-generator-"), 0U);
    }

    void* handle = dlopen(request.outputSo.c_str(), RTLD_NOW | RTLD_LOCAL);
    const char* loadError = handle == nullptr ? dlerror() : nullptr;
    ASSERT_NE(handle, nullptr) << (loadError == nullptr ? "dlopen failed" : loadError);
    auto getter = reinterpret_cast<AscendcGetCompileResourceBundleFn>(dlsym(handle, "AscendcGetCompileResourceBundle"));
    ASSERT_NE(getter, nullptr);
    const AcCompileResourceBundleHeader* header = getter();
    ASSERT_NE(header, nullptr);
    EXPECT_EQ(header->magic, AC_COMPILE_RESOURCE_MAGIC);
    EXPECT_EQ(header->abi_version, AC_COMPILE_RESOURCE_ABI_VERSION);
    EXPECT_EQ(header->struct_size, sizeof(AcCompileResourceBundle));
    EXPECT_EQ(header->flags, 0U);

    const auto* bundle = reinterpret_cast<const AcCompileResourceBundle*>(header);
    EXPECT_EQ(ToString(bundle->package_id), "multi_manifest");
    ASSERT_EQ(bundle->manifest_count, 2U);
    EXPECT_EQ(bundle->extensions, nullptr);
    EXPECT_EQ(bundle->extension_count, 0U);

    uint64_t totalFiles = 0U;
    bool foundNestedHeader = false;
    bool foundEmptyFile = false;
    bool foundFirstSource = false;
    for (uint64_t manifestIndex = 0U; manifestIndex < bundle->manifest_count; ++manifestIndex) {
        const AcCompileResourceManifest& manifest = bundle->manifests[manifestIndex];
        totalFiles += manifest.file_count;
        for (uint64_t fileIndex = 0U; fileIndex < manifest.file_count; ++fileIndex) {
            const AcCompileResourceFile& file = manifest.files[fileIndex];
            const std::string path = ToString(file.file_path);
            foundNestedHeader = foundNestedHeader || path == "resources/include/nested.h";
            foundEmptyFile = foundEmptyFile || path == "resources/empty.bin";
            foundFirstSource = foundFirstSource ||
                               (path == "resources/kernel.cpp" &&
                                std::string(reinterpret_cast<const char*>(file.data), file.size) == "first source");
        }
    }
    EXPECT_EQ(totalFiles, 3U);
    EXPECT_TRUE(foundNestedHeader);
    EXPECT_FALSE(foundEmptyFile);
    EXPECT_TRUE(foundFirstSource);
    EXPECT_EQ(dlclose(handle), 0);
}

TEST_F(AscCompileExporterTest, KeepsTemporaryWorkspaceWhenRequested)
{
    const std::string unit = JoinTestPath(root_, "collection/Keep");
    WriteFile(JoinTestPath(unit, "resources/kernel.cpp"), "source");
    CreateManifest(unit, "Keep");

    BuildCollectedBundleRequest request = Request("keep_workspace");
    request.keepTemp = true;
    ManifestBundleCompiler(request).Compile();

    const std::vector<std::string> workEntries = ListTestDirectory(request.workDir);
    ASSERT_EQ(workEntries.size(), 1U);
    EXPECT_EQ(workEntries[0].find(".manifest-generator-"), 0U);
    EXPECT_TRUE(IsTestRegularFile(request.outputSo));
}

} // namespace
