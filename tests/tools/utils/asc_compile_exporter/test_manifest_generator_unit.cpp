/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>

// Internal components live in an unnamed namespace, so unit tests share their translation unit.
#include "manifest_generator.cpp"

namespace ascendc {
namespace manifest_generator {
namespace {

void WriteTestFile(const std::string &path, const std::string &content)
{
    CreateDirectories(ParentPath(path));
    std::ofstream output(path.c_str(), std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        throw std::runtime_error("failed to create unit test file: " + path);
    }
    output.write(content.data(), static_cast<std::streamsize>(content.size()));
    output.close();
    if (!output) {
        throw std::runtime_error("failed to write unit test file: " + path);
    }
}

std::string ReadTestFile(const std::string &path)
{
    std::ifstream input(path.c_str(), std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}

bool PathExists(const std::string &path)
{
    struct stat info {};
    return lstat(path.c_str(), &info) == 0;
}

bool IsRegularPath(const std::string &path)
{
    struct stat info {};
    return stat(path.c_str(), &info) == 0 && S_ISREG(info.st_mode);
}

class ScopedEnvironment final {
public:
    ScopedEnvironment(const char *name, const char *value) : name_(name)
    {
        const char *current = std::getenv(name);
        existed_ = current != nullptr;
        if (current != nullptr) {
            oldValue_ = current;
        }
        if (value == nullptr) {
            if (unsetenv(name_.c_str()) != 0) {
                throw SystemError("failed to unset test environment variable");
            }
        } else if (setenv(name_.c_str(), value, 1) != 0) {
            throw SystemError("failed to set test environment variable");
        }
    }

    ~ScopedEnvironment()
    {
        if (existed_) {
            (void)setenv(name_.c_str(), oldValue_.c_str(), 1);
        } else {
            (void)unsetenv(name_.c_str());
        }
    }

    ScopedEnvironment(const ScopedEnvironment &) = delete;
    ScopedEnvironment &operator=(const ScopedEnvironment &) = delete;

private:
    std::string name_;
    std::string oldValue_;
    bool existed_ = false;
};

class ManifestGeneratorModuleTest : public testing::Test {
protected:
    void SetUp() override
    {
        const char *temporary = std::getenv("TMPDIR");
        std::string pathTemplate = JoinPath(
            temporary == nullptr || temporary[0] == '\0' ? "/tmp" : temporary,
            "asc_compile_exporter_module_ut_XXXXXX");
        std::vector<char> buffer(pathTemplate.begin(), pathTemplate.end());
        buffer.push_back('\0');
        char *created = mkdtemp(buffer.data());
        ASSERT_NE(created, nullptr);
        root_ = created;
    }

    void TearDown() override
    {
        RemoveTree(root_);
    }

    void CreateManifest(const std::string &unit, const std::string &name) const
    {
        WriteTestFile(JoinPath(unit, name + "_manifest.json"),
                      "{\"base_dir\":\"resources\",\"name\":\"" + name + "\"}");
    }

    std::string root_;
};

TEST_F(ManifestGeneratorModuleTest, PosixFileHelpersReadFilesAndProtectGeneratedOutputs)
{
    const std::string input = JoinPath(root_, "input/data.bin");
    WriteTestFile(input, std::string("A\0B", 3U));

    const std::vector<uint8_t> data = ReadRegularFile(input, 3U, "input");
    EXPECT_EQ(std::string(data.begin(), data.end()), std::string("A\0B", 3U));
    EXPECT_THROW(ReadRegularFile(input, 2U, "input"), std::runtime_error);
    EXPECT_THROW(ReadRegularFile(JoinPath(root_, "missing"), 3U, "missing"), std::runtime_error);

    const std::string generated = JoinPath(root_, "generated.cpp");
    std::ofstream output = OpenGeneratedFile(generated);
    output << "generated";
    FinishOutput(output, generated);
    EXPECT_EQ(ReadTestFile(generated), "generated");
    EXPECT_THROW(OpenGeneratedFile(generated), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, PathHelpersCheckDirectoryContainmentByComponent)
{
    const std::string resourceRoot = JoinPath(root_, "manifest/resources");
    const std::string nested = JoinPath(resourceRoot, "nested");
    const std::string outside = JoinPath(root_, "outside");
    CreateDirectories(nested);
    CreateDirectories(outside);

    EXPECT_TRUE(IsWithin(nested, resourceRoot));
    EXPECT_TRUE(IsWithin(resourceRoot, resourceRoot));
    EXPECT_FALSE(IsWithin(outside, resourceRoot));
    EXPECT_EQ(CanonicalDirectoryWithin(nested, resourceRoot, "resource"),
              CanonicalDirectory(nested, "resource"));
    EXPECT_THROW(CanonicalDirectoryWithin(outside, resourceRoot, "resource"), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, ResourceBudgetEnforcesPayloadAndFileCountLimits)
{
    ResourceBudget payloadBudget;
    payloadBudget.AddPayload(kMaxBundleBytes, "full payload");
    EXPECT_THROW(payloadBudget.AddPayload(1U, "overflow"), std::runtime_error);

    ResourceBudget fileBudget;
    for (size_t index = 0U; index < kMaxFilesPerBundle; ++index) {
        fileBudget.AddResource(0U, "empty resource");
    }
    EXPECT_THROW(fileBudget.AddResource(0U, "one resource too many"), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, RepositorySortsManifestsAndResourcesAndSkipsEmptyFiles)
{
    const std::string first = JoinPath(root_, "collection/z/First");
    const std::string second = JoinPath(root_, "collection/a/Second");
    WriteTestFile(JoinPath(first, "resources/z.cpp"), "z");
    WriteTestFile(JoinPath(first, "resources/a.cpp"), "a");
    WriteTestFile(JoinPath(first, "resources/empty.cpp"), "");
    WriteTestFile(JoinPath(second, "resources/kernel.cpp"), "second");
    CreateManifest(first, "First");
    CreateManifest(second, "Second");
    WriteTestFile(JoinPath(root_, "collection/ignored.json"), "not a manifest");

    const std::vector<ManifestUnit> units =
        CollectedManifestRepository(JoinPath(root_, "collection")).Load();

    ASSERT_EQ(units.size(), 2U);
    EXPECT_NE(units[0].json.find("Second"), std::string::npos);
    EXPECT_NE(units[1].json.find("First"), std::string::npos);
    ASSERT_EQ(units[1].files.size(), 2U);
    EXPECT_EQ(units[1].files[0].filePath, "resources/a.cpp");
    EXPECT_EQ(units[1].files[1].filePath, "resources/z.cpp");
    EXPECT_EQ(std::string(units[1].files[0].data.begin(), units[1].files[0].data.end()), "a");
}

TEST_F(ManifestGeneratorModuleTest, SourceGeneratorEmitsBundleManifestAndMakefile)
{
    const std::string work = JoinPath(root_, "work");
    CreateDirectories(work);
    const std::vector<ManifestUnit> units = {
        {"{\"base_dir\":\"resources\"}",
         {{"kernel.bin", "resources/kernel.bin", {0U, 127U, 255U}}}}
    };

    BundleSourceGenerator(work, "unit_package", units).Generate();

    const std::string bundleSource = ReadTestFile(JoinPath(work, "compile_resource_bundle.cpp"));
    const std::string manifestSource = ReadTestFile(JoinPath(work, "manifest_0.cpp"));
    const std::string makefile = ReadTestFile(JoinPath(work, "Makefile"));
    EXPECT_NE(bundleSource.find("GetManifest0()"), std::string::npos);
    EXPECT_NE(bundleSource.find("1ULL, nullptr, 0ULL"), std::string::npos);
    EXPECT_NE(manifestSource.find("0, 127, 255"), std::string::npos);
    EXPECT_NE(manifestSource.find("g_manifest_0_file_0_path_bytes"), std::string::npos);
    EXPECT_NE(manifestSource.find("g_manifest_0_file_0_path ="), std::string::npos);
    EXPECT_NE(manifestSource.find("20ULL"), std::string::npos);
    EXPECT_NE(makefile.find("manifest_0.cpp"), std::string::npos);
    EXPECT_NE(makefile.find("ASCENDC_BUNDLE_INCLUDE_DIR"), std::string::npos);
    EXPECT_NE(makefile.find("-std=c++11"), std::string::npos);
    EXPECT_THROW(BundleSourceGenerator(work, "unit_package", units).Generate(), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, BuildWorkspaceHonorsCleanupPolicy)
{
    const std::string parent = JoinPath(root_, "sessions");
    std::string removedSession;
    {
        BuildWorkspace workspace = BuildWorkspace::Create(parent, false);
        removedSession = workspace.Session();
        EXPECT_TRUE(IsDirectory(removedSession));
    }
    EXPECT_FALSE(PathExists(removedSession));

    std::string keptSession;
    {
        BuildWorkspace workspace = BuildWorkspace::Create(parent, true);
        keptSession = workspace.Session();
    }
    EXPECT_TRUE(IsDirectory(keptSession));

    const std::string regularFile = JoinPath(root_, "not-a-directory");
    WriteTestFile(regularFile, "file");
    EXPECT_THROW(BuildWorkspace::Create(regularFile, false), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, ExecutableLocatorValidatesConfiguredCompiler)
{
    EXPECT_TRUE(IsRegularPath(ExecutableLocator::FindRequired("sh")));
    EXPECT_THROW(ExecutableLocator::FindRequired("asc_compile_exporter_missing_executable"),
                 std::runtime_error);

    ScopedEnvironment validCompiler("CXX", "/bin/sh");
    EXPECT_TRUE(IsRegularPath(ExecutableLocator::FindCompiler()));
}

TEST_F(ManifestGeneratorModuleTest, ProcessRunnerPropagatesEnvironmentAndExitStatus)
{
    EXPECT_THROW(ProcessRunner::Run({}, {}), std::invalid_argument);
    EXPECT_THROW(ProcessRunner::Run({"/bin/sh", "-c", "exit 7"}, {}), std::runtime_error);

    const std::string output = JoinPath(root_, "environment.txt");
    ProcessRunner::Run(
        {"/bin/sh", "-c", "printf '%s' \"$ASC_COMPILE_EXPORTER_UNIT_VALUE\" > \"$ASC_COMPILE_EXPORTER_UNIT_OUTPUT\""},
        {{"ASC_COMPILE_EXPORTER_UNIT_VALUE", "visible"}, {"ASC_COMPILE_EXPORTER_UNIT_OUTPUT", output}});
    EXPECT_EQ(ReadTestFile(output), "visible");
}

TEST_F(ManifestGeneratorModuleTest, BundlePublisherPublishesFilesAndRejectsSymlinks)
{
    const std::string output = JoinPath(root_, "output/bundle.so");
    CreateDirectories(ParentPath(output));
    BundlePublisher::Prepare(output);
    EXPECT_TRUE(IsDirectory(ParentPath(output)));

    const std::string staged = JoinPath(root_, "staged.so");
    WriteTestFile(staged, "new bundle");
    WriteTestFile(output, "old bundle");
    BundlePublisher::Publish(staged, output);
    EXPECT_EQ(ReadTestFile(output), "new bundle");
    EXPECT_FALSE(PathExists(staged));

    const std::string victim = JoinPath(root_, "victim.so");
    const std::string symlinkPath = JoinPath(root_, "output/symlink.so");
    WriteTestFile(victim, "victim");
    ASSERT_EQ(symlink(victim.c_str(), symlinkPath.c_str()), 0);
    EXPECT_THROW(BundlePublisher::Prepare(symlinkPath), std::runtime_error);
}

TEST(ManifestGeneratorStandaloneUnitTest, ResolveJobCountHonorsExplicitValue)
{
    EXPECT_EQ(ResolveJobCount(7U), 7U);
    EXPECT_GE(ResolveJobCount(0U), 1U);
}

} // namespace
} // namespace manifest_generator
} // namespace ascendc
