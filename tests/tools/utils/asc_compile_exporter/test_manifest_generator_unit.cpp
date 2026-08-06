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

void WriteTestFile(const std::string& path, const std::string& content)
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

std::string ReadTestFile(const std::string& path)
{
    std::ifstream input(path.c_str(), std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}

bool PathExists(const std::string& path)
{
    struct stat info {};
    return lstat(path.c_str(), &info) == 0;
}

bool IsRegularPath(const std::string& path)
{
    struct stat info {};
    return stat(path.c_str(), &info) == 0 && S_ISREG(info.st_mode);
}

class ScopedEnvironment final {
public:
    ScopedEnvironment(const char* name, const char* value) : name_(name)
    {
        const char* current = std::getenv(name);
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

    ScopedEnvironment(const ScopedEnvironment&) = delete;
    ScopedEnvironment& operator=(const ScopedEnvironment&) = delete;

private:
    std::string name_;
    std::string oldValue_;
    bool existed_ = false;
};

class ManifestGeneratorModuleTest : public testing::Test {
protected:
    void SetUp() override
    {
        const char* temporary = std::getenv("TMPDIR");
        std::string pathTemplate = JoinPath(
            temporary == nullptr || temporary[0] == '\0' ? "/tmp" : temporary, "asc_compile_exporter_module_ut_XXXXXX");
        std::vector<char> buffer(pathTemplate.begin(), pathTemplate.end());
        buffer.push_back('\0');
        char* created = mkdtemp(buffer.data());
        ASSERT_NE(created, nullptr);
        root_ = created;
    }

    void TearDown() override { RemoveTree(root_); }

    void CreateManifest(const std::string& unit, const std::string& name) const
    {
        WriteTestFile(
            JoinPath(unit, name + "_manifest.json"), "{\"base_dir\":\"resources\",\"name\":\"" + name + "\"}");
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
    EXPECT_THROW(ReadRegularFile(root_, 3U, "directory"), std::runtime_error);

    const std::string generated = JoinPath(root_, "generated.cpp");
    std::ofstream output = OpenGeneratedFile(generated);
    output << "generated";
    FinishOutput(output, generated);
    EXPECT_EQ(ReadTestFile(generated), "generated");
    EXPECT_THROW(OpenGeneratedFile(generated), std::runtime_error);

    std::ofstream failedOutput;
    failedOutput.setstate(std::ios::badbit);
    EXPECT_THROW(FinishOutput(failedOutput, "failed-output"), std::runtime_error);
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
    EXPECT_EQ(CanonicalDirectoryWithin(nested, resourceRoot, "resource"), CanonicalDirectory(nested, "resource"));
    EXPECT_THROW(CanonicalDirectoryWithin(outside, resourceRoot, "resource"), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, PathFunctionsHandleEmptyRelativeAndRootInputs)
{
    EXPECT_EQ(JoinPath("", "child"), "child");
    EXPECT_EQ(JoinPath("parent", ""), "parent");
    EXPECT_EQ(JoinPath("parent/", "child"), "parent/child");
    EXPECT_EQ(ParentPath("file"), "");
    EXPECT_EQ(ParentPath("/file"), "/");
    EXPECT_EQ(FileName(""), "");
    EXPECT_EQ(FileName("/directory/"), "");
    EXPECT_EQ(FileName("file"), "file");

    const std::string current = CurrentDirectory();
    EXPECT_FALSE(current.empty());
    EXPECT_EQ(AbsolutePath("/absolute"), "/absolute");
    EXPECT_EQ(AbsolutePath("relative"), JoinPath(current, "relative"));
    EXPECT_TRUE(IsWithin("/child", "/"));
    EXPECT_FALSE(IsWithin("relative", "/"));

    std::string resolved;
    EXPECT_TRUE(ResolveCanonicalPath(root_, resolved));
    EXPECT_EQ(resolved, CanonicalDirectory(root_, "test root"));
    EXPECT_EQ(resolved.c_str()[resolved.size()], '\0');
    EXPECT_FALSE(ResolveCanonicalPath(JoinPath(root_, "missing"), resolved));
}

TEST_F(ManifestGeneratorModuleTest, DirectoryFunctionsCreateValidateAndRemovePaths)
{
    EnsureDirectory("");
    EnsureDirectory("/");
    const std::string directory = JoinPath(root_, "directory");
    EnsureDirectory(directory);
    EnsureDirectory(directory);
    EXPECT_TRUE(IsDirectory(directory));

    const std::string regularFile = JoinPath(root_, "regular-file");
    WriteTestFile(regularFile, "data");
    EXPECT_THROW(EnsureDirectory(regularFile), std::runtime_error);
    EXPECT_THROW(CanonicalDirectory(regularFile, "regular file"), std::runtime_error);
    EXPECT_THROW(CanonicalDirectory(JoinPath(root_, "missing"), "missing"), std::runtime_error);

    const std::string symlinkPath = JoinPath(root_, "directory-link");
    ASSERT_EQ(symlink(directory.c_str(), symlinkPath.c_str()), 0);
    EXPECT_THROW(CanonicalDirectory(symlinkPath, "directory link"), std::runtime_error);
    EXPECT_TRUE(RemoveTree(symlinkPath));
    EXPECT_FALSE(PathExists(symlinkPath));
    EXPECT_TRUE(IsDirectory(directory));

    EXPECT_TRUE(RemoveTree(regularFile));
    EXPECT_FALSE(PathExists(regularFile));
    EXPECT_TRUE(RemoveTree(JoinPath(root_, "does-not-exist")));
    EXPECT_FALSE(RemoveTree("/dev/null/child"));
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

    const std::vector<ManifestUnit> units = CollectedManifestRepository(JoinPath(root_, "collection")).Load();

    ASSERT_EQ(units.size(), 2U);
    EXPECT_NE(units[0].json.find("Second"), std::string::npos);
    EXPECT_NE(units[1].json.find("First"), std::string::npos);
    ASSERT_EQ(units[1].files.size(), 2U);
    EXPECT_EQ(units[1].files[0].filePath, "resources/a.cpp");
    EXPECT_EQ(units[1].files[1].filePath, "resources/z.cpp");
    EXPECT_EQ(std::string(units[1].files[0].data.begin(), units[1].files[0].data.end()), "a");
}

TEST_F(ManifestGeneratorModuleTest, RepositoryReportsManifestJsonAndBaseDirErrors)
{
    const std::string malformed = JoinPath(root_, "malformed/Unit");
    WriteTestFile(JoinPath(malformed, "resources/kernel.cpp"), "source");
    WriteTestFile(JoinPath(malformed, "Unit_manifest.json"), "{\"base_dir\":");
    EXPECT_THROW(CollectedManifestRepository(JoinPath(root_, "malformed")).Load(), std::runtime_error);

    const std::string missing = JoinPath(root_, "missing-base/Unit");
    WriteTestFile(JoinPath(missing, "resources/kernel.cpp"), "source");
    WriteTestFile(JoinPath(missing, "Unit_manifest.json"), "{}");
    EXPECT_THROW(CollectedManifestRepository(JoinPath(root_, "missing-base")).Load(), std::runtime_error);

    const std::string wrongType = JoinPath(root_, "wrong-type/Unit");
    WriteTestFile(JoinPath(wrongType, "resources/kernel.cpp"), "source");
    WriteTestFile(JoinPath(wrongType, "Unit_manifest.json"), "{\"base_dir\":7}");
    EXPECT_THROW(CollectedManifestRepository(JoinPath(root_, "wrong-type")).Load(), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, RepositorySkipsSymlinkedManifestsAndAcceptsAbsoluteBaseDir)
{
    const std::string collection = JoinPath(root_, "collection-absolute");
    const std::string unit = JoinPath(collection, "Unit");
    const std::string resources = JoinPath(unit, "resources");
    WriteTestFile(JoinPath(resources, "kernel.cpp"), "source");
    WriteTestFile(JoinPath(unit, "Unit_manifest.json"), "{\"base_dir\":\"" + resources + "\"}");

    const std::string ignoredManifest = JoinPath(root_, "ignored_manifest.json");
    WriteTestFile(ignoredManifest, "{\"base_dir\":\"resources\"}");
    ASSERT_EQ(symlink(ignoredManifest.c_str(), JoinPath(collection, "Link_manifest.json").c_str()), 0);

    const std::vector<ManifestUnit> units = CollectedManifestRepository(collection).Load();
    ASSERT_EQ(units.size(), 1U);
    ASSERT_EQ(units[0].files.size(), 1U);
    EXPECT_EQ(units[0].files[0].filePath, "resources/kernel.cpp");
}

TEST_F(ManifestGeneratorModuleTest, RepositoryRejectsNonRegularResource)
{
    const std::string unit = JoinPath(root_, "collection-special/Unit");
    const std::string resources = JoinPath(unit, "resources");
    CreateDirectories(resources);
    ASSERT_EQ(mkfifo(JoinPath(resources, "resource.pipe").c_str(), 0600), 0);
    CreateManifest(unit, "Unit");

    EXPECT_THROW(CollectedManifestRepository(JoinPath(root_, "collection-special")).Load(), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, RepositoryEnforcesDirectoryDepthLimit)
{
    const std::string allowedCollection = JoinPath(root_, "collection-depth-allowed");
    std::string allowedDirectory = allowedCollection;
    for (size_t depth = 0U; depth < kMaxDirectoryDepth; ++depth) {
        allowedDirectory = JoinPath(allowedDirectory, "level-" + std::to_string(depth));
    }
    WriteTestFile(JoinPath(allowedDirectory, "Deep_manifest.json"), "{\"base_dir\":\".\"}");
    EXPECT_NO_THROW(CollectedManifestRepository(allowedCollection).Load());

    const std::string rejectedCollection = JoinPath(root_, "collection-depth-rejected");
    std::string rejectedDirectory = rejectedCollection;
    for (size_t depth = 0U; depth <= kMaxDirectoryDepth; ++depth) {
        rejectedDirectory = JoinPath(rejectedDirectory, "level-" + std::to_string(depth));
    }
    CreateDirectories(rejectedDirectory);
    EXPECT_THROW(CollectedManifestRepository(rejectedCollection).Load(), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, SourceGeneratorEmitsBundleManifestAndMakefile)
{
    const std::string work = JoinPath(root_, "work");
    CreateDirectories(work);
    const std::vector<ManifestUnit> units = {
        {"{\"base_dir\":\"resources\"}", {{"kernel.bin", "resources/kernel.bin", {0U, 127U, 255U}}}}};

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

TEST_F(ManifestGeneratorModuleTest, SourceGeneratorEmitsManifestWithoutResourceFiles)
{
    const std::string work = JoinPath(root_, "empty-resource-work");
    CreateDirectories(work);
    const std::vector<ManifestUnit> units = {{"{\"base_dir\":\"resources\"}", {}}};

    BundleSourceGenerator(work, "empty_package", units).Generate();

    const std::string manifestSource = ReadTestFile(JoinPath(work, "manifest_0.cpp"));
    EXPECT_NE(manifestSource.find("nullptr, 0ULL"), std::string::npos);
    EXPECT_EQ(manifestSource.find("AcCompileResourceFile g_manifest_0_files"), std::string::npos);
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

TEST_F(ManifestGeneratorModuleTest, BuildWorkspaceUsesTmpDirAndSupportsMoveConstruction)
{
    ScopedEnvironment temporary("TMPDIR", root_.c_str());
    std::string session;
    {
        BuildWorkspace workspace = BuildWorkspace::Create("", false);
        session = workspace.Session();
        EXPECT_TRUE(IsWithin(session, root_));
        BuildWorkspace moved(std::move(workspace));
        EXPECT_TRUE(workspace.Session().empty());
        EXPECT_EQ(moved.Session(), session);
    }
    EXPECT_FALSE(PathExists(session));
}

TEST_F(ManifestGeneratorModuleTest, ExecutableLocatorValidatesRequiredExecutable)
{
    EXPECT_TRUE(IsRegularPath(ExecutableLocator::FindRequired("sh")));
    EXPECT_THROW(ExecutableLocator::FindRequired("asc_compile_exporter_missing_executable"), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, ExecutableLocatorHandlesSearchPath)
{
    const std::string bin = JoinPath(root_, "bin");
    const std::string fakeCompiler = JoinPath(bin, "g++");
    WriteTestFile(fakeCompiler, "#!/bin/sh\nexit 0\n");
    ASSERT_EQ(chmod(fakeCompiler.c_str(), 0700), 0);
    ScopedEnvironment compiler("CXX", JoinPath(root_, "missing-compiler").c_str());
    ScopedEnvironment path("PATH", bin.c_str());
    EXPECT_EQ(ExecutableLocator::FindCompiler(), CanonicalDirectory(bin, "compiler directory") + "/g++");
    EXPECT_TRUE(IsRegularPath(ExecutableLocator::FindRequired(fakeCompiler)));
}

TEST_F(ManifestGeneratorModuleTest, ExecutableLocatorUsesDefaultPathAndReportsMissingCompiler)
{
    {
        ScopedEnvironment path("PATH", nullptr);
        EXPECT_TRUE(IsRegularPath(ExecutableLocator::FindRequired("sh")));
    }
    {
        ScopedEnvironment path("PATH", root_.c_str());
        EXPECT_THROW(ExecutableLocator::FindCompiler(), std::runtime_error);
    }
}

TEST_F(ManifestGeneratorModuleTest, ProcessRunnerPropagatesEnvironmentAndExitStatus)
{
    EXPECT_THROW(ProcessRunner::Run({}, {}), std::invalid_argument);
    EXPECT_THROW(ProcessRunner::Run({"/bin/sh", "-c", "exit 7"}, {}), std::runtime_error);

    const std::string output = JoinPath(root_, "environment.txt");
    ScopedEnvironment makeFlags("MAKEFLAGS", "must-not-leak");
    ProcessRunner::Run(
        {"/bin/sh", "-c",
         "printf '%s|%s' \"$ASC_COMPILE_EXPORTER_UNIT_VALUE\" \"${MAKEFLAGS-unset}\" > "
         "\"$ASC_COMPILE_EXPORTER_UNIT_OUTPUT\""},
        {{"ASC_COMPILE_EXPORTER_UNIT_VALUE", "visible"}, {"ASC_COMPILE_EXPORTER_UNIT_OUTPUT", output}});
    EXPECT_EQ(ReadTestFile(output), "visible|unset");
}

TEST_F(ManifestGeneratorModuleTest, ProcessRunnerReportsExecAndSignalFailures)
{
    EXPECT_THROW(ProcessRunner::Run({JoinPath(root_, "missing-command")}, {}), std::runtime_error);
    EXPECT_THROW(ProcessRunner::Run({"/bin/sh", "-c", "kill -TERM $$"}, {}), std::runtime_error);
}

TEST_F(ManifestGeneratorModuleTest, ProcessRunnerTimesOutAndReapsProcessGroup)
{
    const std::string pidFile = JoinPath(root_, "timed-out.pid");
    const std::string terminationFile = JoinPath(root_, "termination.txt");
    EXPECT_THROW(
        ProcessRunner::Run(
            {"/bin/sh", "-c",
             "trap 'printf terminated > \"$ASC_COMPILE_EXPORTER_TERMINATION_FILE\"; exit 0' TERM; "
             "printf '%s' \"$$\" > \"$ASC_COMPILE_EXPORTER_PID_FILE\"; sleep 10"},
            {{"ASC_COMPILE_EXPORTER_PID_FILE", pidFile}, {"ASC_COMPILE_EXPORTER_TERMINATION_FILE", terminationFile}},
            std::chrono::milliseconds(100U)),
        std::runtime_error);

    EXPECT_EQ(ReadTestFile(terminationFile), "terminated");
    const pid_t child = static_cast<pid_t>(std::strtol(ReadTestFile(pidFile).c_str(), nullptr, 10));
    ASSERT_GT(child, 0);
    errno = 0;
    EXPECT_EQ(kill(child, 0), -1);
    EXPECT_EQ(errno, ESRCH);
    EXPECT_THROW(ProcessRunner::Run({"/bin/true"}, {}, std::chrono::milliseconds(0U)), std::invalid_argument);
}

TEST_F(ManifestGeneratorModuleTest, MakeBundleBuilderValidatesCannHomeAndAbiHeader)
{
    {
        ScopedEnvironment cannHome("ASCEND_HOME_PATH", nullptr);
        EXPECT_THROW(MakeBundleBuilder(root_, JoinPath(root_, "bundle.so"), 1U).Build(), std::runtime_error);
    }
    {
        const std::string cann = JoinPath(root_, "incomplete-cann");
        CreateDirectories(JoinPath(cann, "include"));
        ScopedEnvironment cannHome("ASCEND_HOME_PATH", cann.c_str());
        EXPECT_THROW(MakeBundleBuilder(root_, JoinPath(root_, "bundle.so"), 1U).Build(), std::runtime_error);
    }
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

TEST_F(ManifestGeneratorModuleTest, BundlePublisherReportsRenameFailure)
{
    const std::string output = JoinPath(root_, "output/missing.so");
    CreateDirectories(ParentPath(output));
    EXPECT_THROW(BundlePublisher::Publish(JoinPath(root_, "missing-staged.so"), output), std::runtime_error);
    EXPECT_FALSE(PathExists(output));
}

TEST_F(ManifestGeneratorModuleTest, BundlePublisherCopiesAcrossFileSystems)
{
    struct stat rootInfo {};
    struct stat sharedMemoryInfo {};
    if (stat(root_.c_str(), &rootInfo) != 0 || stat("/dev/shm", &sharedMemoryInfo) != 0 ||
        rootInfo.st_dev == sharedMemoryInfo.st_dev || access("/dev/shm", W_OK) != 0) {
        GTEST_SKIP() << "cross-filesystem writable directory is unavailable";
    }

    BuildWorkspace destinationWorkspace = BuildWorkspace::Create("/dev/shm", false);
    const std::string staged = JoinPath(root_, "large-staged.so");
    const std::string output = JoinPath(destinationWorkspace.Session(), "bundle.so");
    const std::string content(128U * 1024U + 17U, 'x');
    WriteTestFile(staged, content);

    BundlePublisher::Publish(staged, output);

    EXPECT_EQ(ReadTestFile(output), content);
    EXPECT_TRUE(PathExists(staged));

    const std::string stagedDirectory = JoinPath(root_, "staged-directory");
    CreateDirectories(stagedDirectory);
    EXPECT_THROW(
        BundlePublisher::Publish(stagedDirectory, JoinPath(destinationWorkspace.Session(), "directory.so")),
        std::runtime_error);

    const std::string stagedSymlink = JoinPath(root_, "staged-symlink.so");
    ASSERT_EQ(symlink(staged.c_str(), stagedSymlink.c_str()), 0);
    EXPECT_THROW(
        BundlePublisher::Publish(stagedSymlink, JoinPath(destinationWorkspace.Session(), "symlink.so")),
        std::runtime_error);

    const std::string outputDirectory = JoinPath(destinationWorkspace.Session(), "existing-directory");
    CreateDirectories(outputDirectory);
    EXPECT_THROW(BundlePublisher::Publish(staged, outputDirectory), std::runtime_error);
}

TEST(ManifestGeneratorStandaloneUnitTest, ResolveJobCountClampsExplicitValueToHardwareConcurrency)
{
    const uint32_t reportedCpuCount = static_cast<uint32_t>(std::thread::hardware_concurrency());
    const uint32_t cpuCount = reportedCpuCount == 0U ? 1U : reportedCpuCount;
    EXPECT_EQ(ResolveJobCount(1U), 1U);
    EXPECT_EQ(ResolveJobCount(cpuCount), cpuCount);
    EXPECT_EQ(ResolveJobCount(std::numeric_limits<uint32_t>::max()), cpuCount);
    EXPECT_EQ(ResolveJobCount(0U), cpuCount < 2U ? 1U : cpuCount / 2U);
}

} // namespace
} // namespace manifest_generator
} // namespace ascendc
