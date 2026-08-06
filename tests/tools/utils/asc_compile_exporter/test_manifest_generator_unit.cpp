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

namespace ascendc::manifest_generator {
namespace {

void WriteTestFile(const fs::path &path, const std::string &content)
{
    std::error_code error;
    fs::create_directories(path.parent_path(), error);
    if (error) {
        throw std::runtime_error("failed to create unit test directory: " + error.message());
    }
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        throw std::runtime_error("failed to create unit test file: " + path.string());
    }
    output.write(content.data(), static_cast<std::streamsize>(content.size()));
    output.close();
    if (!output) {
        throw std::runtime_error("failed to write unit test file: " + path.string());
    }
}

std::string ReadTestFile(const fs::path &path)
{
    std::ifstream input(path, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
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
        std::string pathTemplate =
            (fs::temp_directory_path() / "asc_compile_exporter_module_ut_XXXXXX").string();
        std::vector<char> buffer(pathTemplate.begin(), pathTemplate.end());
        buffer.push_back('\0');
        char *created = mkdtemp(buffer.data());
        ASSERT_NE(created, nullptr);
        root_ = created;
    }

    void TearDown() override
    {
        std::error_code ignored;
        fs::remove_all(root_, ignored);
    }

    void CreateManifest(const fs::path &unit, const std::string &name) const
    {
        WriteTestFile(unit / (name + "_manifest.json"),
                      "{\"base_dir\":\"resources\",\"name\":\"" + name + "\"}");
    }

    fs::path root_;
};

TEST_F(ManifestGeneratorModuleTest, FileSystemReadsFilesAndProtectsGeneratedOutputs)
{
    const fs::path input = root_ / "input/data.bin";
    WriteTestFile(input, std::string("A\0B", 3U));

    EXPECT_EQ(FileSystem::FileSize(input, "input"), 3U);
    EXPECT_EQ(FileSystem::ReadText(input, 3U, "input"), std::string("A\0B", 3U));
    EXPECT_THROW(FileSystem::ReadBytes(input, 2U, "input"), std::runtime_error);
    EXPECT_THROW(FileSystem::FileSize(root_ / "missing", "missing"), std::runtime_error);

    const fs::path generated = root_ / "generated.cpp";
    std::ofstream output = FileSystem::OpenOutput(generated);
    output << "generated";
    FileSystem::FinishOutput(output, generated);
    EXPECT_EQ(ReadTestFile(generated), "generated");
    EXPECT_THROW(FileSystem::OpenOutput(generated), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, FileSystemChecksDirectoryContainmentByPathComponent)
{
    const fs::path resourceRoot = root_ / "manifest/resources";
    const fs::path nested = resourceRoot / "nested";
    const fs::path outside = root_ / "outside";
    fs::create_directories(nested);
    fs::create_directories(outside);

    EXPECT_TRUE(FileSystem::IsWithin(nested, resourceRoot));
    EXPECT_TRUE(FileSystem::IsWithin(resourceRoot, resourceRoot));
    EXPECT_FALSE(FileSystem::IsWithin(outside, resourceRoot));
    EXPECT_EQ(FileSystem::CanonicalDirectoryWithin(nested, resourceRoot, "resource"),
              fs::canonical(nested));
    EXPECT_THROW(FileSystem::CanonicalDirectoryWithin(outside, resourceRoot, "resource"),
                 std::runtime_error);
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
    const fs::path first = root_ / "collection/z/First";
    const fs::path second = root_ / "collection/a/Second";
    WriteTestFile(first / "resources/z.cpp", "z");
    WriteTestFile(first / "resources/a.cpp", "a");
    WriteTestFile(first / "resources/empty.cpp", "");
    WriteTestFile(second / "resources/kernel.cpp", "second");
    CreateManifest(first, "First");
    CreateManifest(second, "Second");
    WriteTestFile(root_ / "collection/ignored.json", "not a manifest");

    const std::vector<ManifestUnit> units =
        CollectedManifestRepository(root_ / "collection").Load();

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
    const fs::path work = root_ / "work";
    fs::create_directories(work);
    const std::vector<ManifestUnit> units = {
        {"{\"base_dir\":\"resources\"}",
         {{"kernel.bin", "resources/kernel.bin", {0U, 127U, 255U}}}}
    };

    BundleSourceGenerator(work, "unit_package", units).Generate();

    const std::string bundleSource = ReadTestFile(work / "compile_resource_bundle.cpp");
    const std::string manifestSource = ReadTestFile(work / "manifest_0.cpp");
    const std::string makefile = ReadTestFile(work / "Makefile");
    EXPECT_NE(bundleSource.find("GetManifest0()"), std::string::npos);
    EXPECT_NE(bundleSource.find("1ULL, nullptr, 0ULL"), std::string::npos);
    EXPECT_NE(manifestSource.find("0, 127, 255"), std::string::npos);
    EXPECT_NE(manifestSource.find("g_manifest_0_file_0_path_bytes"), std::string::npos);
    EXPECT_NE(manifestSource.find("g_manifest_0_file_0_path ="), std::string::npos);
    EXPECT_NE(manifestSource.find("20ULL"), std::string::npos);
    EXPECT_NE(makefile.find("manifest_0.cpp"), std::string::npos);
    EXPECT_NE(makefile.find("ASCENDC_BUNDLE_INCLUDE_DIR"), std::string::npos);
    EXPECT_THROW(BundleSourceGenerator(work, "unit_package", units).Generate(), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, BuildWorkspaceHonorsCleanupPolicy)
{
    const fs::path parent = root_ / "sessions";
    fs::path removedSession;
    {
        BuildWorkspace workspace = BuildWorkspace::Create(parent, false);
        removedSession = workspace.Session();
        EXPECT_TRUE(fs::is_directory(removedSession));
    }
    EXPECT_FALSE(fs::exists(removedSession));

    fs::path keptSession;
    {
        BuildWorkspace workspace = BuildWorkspace::Create(parent, true);
        keptSession = workspace.Session();
    }
    EXPECT_TRUE(fs::is_directory(keptSession));

    const fs::path regularFile = root_ / "not-a-directory";
    WriteTestFile(regularFile, "file");
    EXPECT_THROW(BuildWorkspace::Create(regularFile, false), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, ExecutableLocatorValidatesConfiguredCompiler)
{
    EXPECT_TRUE(fs::is_regular_file(ExecutableLocator::FindRequired("sh")));
    EXPECT_THROW(ExecutableLocator::FindRequired("asc_compile_exporter_missing_executable"),
                 std::runtime_error);

    ScopedEnvironment validCompiler("CXX", "/bin/sh");
    EXPECT_TRUE(fs::is_regular_file(ExecutableLocator::FindCompiler()));
}

TEST_F(ManifestGeneratorModuleTest, ProcessRunnerPropagatesEnvironmentAndExitStatus)
{
    EXPECT_THROW(ProcessRunner::Run({}, {}), std::invalid_argument);
    EXPECT_THROW(ProcessRunner::Run({"/bin/sh", "-c", "exit 7"}, {}), std::runtime_error);

    const fs::path output = root_ / "environment.txt";
    ProcessRunner::Run(
        {"/bin/sh", "-c", "printf '%s' \"$ASC_COMPILE_EXPORTER_UNIT_VALUE\" > \"$ASC_COMPILE_EXPORTER_UNIT_OUTPUT\""},
        {{"ASC_COMPILE_EXPORTER_UNIT_VALUE", "visible"}, {"ASC_COMPILE_EXPORTER_UNIT_OUTPUT", output.string()}});
    EXPECT_EQ(ReadTestFile(output), "visible");
}

TEST_F(ManifestGeneratorModuleTest, BundlePublisherPublishesFilesAndRejectsSymlinks)
{
    const fs::path output = root_ / "output/bundle.so";
    BundlePublisher::Prepare(output);
    EXPECT_TRUE(fs::is_directory(output.parent_path()));

    const fs::path staged = root_ / "staged.so";
    WriteTestFile(staged, "new bundle");
    WriteTestFile(output, "old bundle");
    BundlePublisher::Publish(staged, output);
    EXPECT_EQ(ReadTestFile(output), "new bundle");
    EXPECT_FALSE(fs::exists(staged));

    const fs::path victim = root_ / "victim.so";
    const fs::path symlink = root_ / "output/symlink.so";
    WriteTestFile(victim, "victim");
    fs::create_symlink(victim, symlink);
    EXPECT_THROW(BundlePublisher::Prepare(symlink), std::runtime_error);
}

TEST(ManifestGeneratorStandaloneUnitTest, ResolveJobCountHonorsExplicitValue)
{
    EXPECT_EQ(ResolveJobCount(7U), 7U);
    EXPECT_GE(ResolveJobCount(0U), 1U);
}

} // namespace
} // namespace ascendc::manifest_generator
