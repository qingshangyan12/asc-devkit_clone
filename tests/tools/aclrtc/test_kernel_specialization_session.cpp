/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "specialization/kernel_specialization.h"
#include "specialization/resource_registry_adapter.h"
#include "stub/kernel_specialization_log.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

namespace {
namespace fs = boost::filesystem;
using Json = nlohmann::json;
using ascendc::aclrtc::JitResourceRegistryAdapter;
using ascendc::aclrtc::KernelSpecializationSession;
using ascendc::aclrtc::RegisteredJitResource;

enum class FakeCompilerOutcome : uint32_t {
    Success,
    Failure,
};

constexpr char RESOURCE_ID[] = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";

class ScopedEnvironmentVariable final {
public:
    ScopedEnvironmentVariable(const char* name, const char* value) : name_(name)
    {
        const char* previousValue = std::getenv(name);
        if (previousValue != nullptr) {
            previousValue_ = previousValue;
            wasSet_ = true;
        }
        if (value == nullptr) {
            unsetenv(name);
        } else {
            setenv(name, value, 1);
        }
    }

    ~ScopedEnvironmentVariable()
    {
        if (wasSet_) {
            setenv(name_.c_str(), previousValue_.c_str(), 1);
        } else {
            unsetenv(name_.c_str());
        }
    }

private:
    std::string name_;
    std::string previousValue_;
    bool wasSet_ {false};
};

class ScopedCurrentDirectory final {
public:
    explicit ScopedCurrentDirectory(const fs::path& currentDirectory)
        : previousDirectory_(fs::current_path())
    {
        fs::current_path(currentDirectory);
    }

    ~ScopedCurrentDirectory()
    {
        boost::system::error_code ignoredError;
        fs::current_path(previousDirectory_, ignoredError);
    }

private:
    fs::path previousDirectory_;
};

class EndToEndTestResource final {
public:
    explicit EndToEndTestResource(FakeCompilerOutcome compilerOutcome)
    {
        static uint64_t sequence = 0;
        rootPath_ = fs::temp_directory_path() /
                    ("aclrtc_session_test_" + std::to_string(getpid()) + "_" + std::to_string(sequence++));
        worktreePath_ = rootPath_ / "worktree";
        sourceDirectoryPath_ = rootPath_ / "user";
        toolchainPath_ = rootPath_ / "toolchain";
        outputElfPath_ = rootPath_ / "output/kernel.elf";
        fs::create_directories(worktreePath_ / "resources/include");
        fs::create_directories(worktreePath_ / "resources/src");
        fs::create_directories(sourceDirectoryPath_);
        fs::create_directories(toolchainPath_ / "bin");
        fs::create_directories(outputElfPath_.parent_path());
        WriteFile(sourceDirectoryPath_ / "add_custom.cpp", "// user source\n");
        WriteFile(worktreePath_ / "resources/include/constants.h", "TILING=@@STATIC_TILING@@;\n");
        WriteFile(worktreePath_ / "resources/src/basic.cpp", "// basic source\n");
        WriteExecutable(toolchainPath_ / "bin/bisheng",
                        compilerOutcome == FakeCompilerOutcome::Success ? CreateOutputScript("object")
                                                                        : "#!/bin/sh\nexit 2\n");
        WriteExecutable(toolchainPath_ / "bin/ld.lld", CreateOutputScript("linked-elf"));
        manifest_ = CreateManifest();
    }

    ~EndToEndTestResource()
    {
        boost::system::error_code ignoredError;
        fs::remove_all(rootPath_, ignoredError);
    }

    static aclError Lookup(const std::string& resourceId, RegisteredJitResource& resource, void* context)
    {
        auto& testResource = *static_cast<EndToEndTestResource*>(context);
        testResource.lookupCalled_ = true;
        if (resourceId != RESOURCE_ID) {
            return ascendc::aclrtc::ACLRTC_ERROR_FAILURE;
        }
        resource.borrowedManifest = &testResource.manifest_;
        resource.ownedWorktreePath = testResource.worktreePath_;
        resource.externalSourceDirectoryPath = testResource.sourceDirectoryPath_;
        return 0;
    }

    const fs::path& ToolchainPath() const { return toolchainPath_; }
    const fs::path& WorktreePath() const { return worktreePath_; }
    const fs::path& OutputElfPath() const { return outputElfPath_; }
    bool LookupCalled() const { return lookupCalled_; }

private:
    static void WriteFile(const fs::path& path, const std::string& contents)
    {
        std::ofstream stream(path);
        stream << contents;
    }

    static void WriteExecutable(const fs::path& path, const std::string& contents)
    {
        WriteFile(path, contents);
        chmod(path.c_str(), 0755);
    }

    static std::string CreateOutputScript(const char* contents)
    {
        return std::string("#!/bin/sh\n") +
               "while [ \"$#\" -gt 0 ]; do\n"
               "  if [ \"$1\" = \"-o\" ]; then\n"
               "    shift\n"
               "    printf '" + contents + "' > \"$1\"\n"
               "    exit 0\n"
               "  fi\n"
               "  shift\n"
               "done\n"
               "exit 3\n";
    }

    static Json CreateManifest()
    {
        return Json {
            {"schema_version", "1.0"},
            {"source_file", "add_custom.cpp"},
            {"kernels",
             Json::array({{{"kernel_name", "add_custom_100000"},
                           {"constant_infos",
                            Json::array({{{"parameter_index", 0},
                                          {"byte_size", 2},
                                          {"file", "${resource}/resources/include/constants.h"},
                                          {"template", "@@STATIC_TILING@@"}}})},
                           {"link_options", Json::array({"-m", "aicorelinux", "-Ttext=0"})},
                           {"objects",
                            Json::array({{{"object_name", "basic"},
                                          {"object_type", "basic"},
                                          {"commands",
                                           Json::array({{{"type", "compile"},
                                                         {"stage", 0},
                                                         {"cmd",
                                                          Json::array({"${env:ASCEND_HOME_PATH}/bin/bisheng",
                                                                       "${resource}/resources/src/basic.cpp", "-o",
                                                                       "${output}/basic.o"})}}})},
                                          {"outputs", Json::array({"${output}/basic.o"})}}})}}})}};
    }

    fs::path rootPath_;
    fs::path worktreePath_;
    fs::path sourceDirectoryPath_;
    fs::path toolchainPath_;
    fs::path outputElfPath_;
    Json manifest_;
    bool lookupCalled_ {false};
};

aclrtcKernelSpecRequest CreateRequest(const uint64_t* constantByteCounts, const void* const* constantAddresses)
{
    aclrtcKernelSpecRequest request {};
    request.resourceId = RESOURCE_ID;
    request.kernelEntry = "add_custom_100000";
    request.argsCount = 1;
    request.argsAddr = constantAddresses;
    request.argsBytes = constantByteCounts;
    return request;
}

aclError ReturnInjectedRegistryFailure(
    const std::string&, RegisteredJitResource&, void*)
{
    return ascendc::aclrtc::ACLRTC_ERROR_FAILURE;
}

std::string ReadFile(const fs::path& path)
{
    std::ifstream stream(path);
    return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}

TEST(KernelSpecializationSessionTest, RunsLookupPatchCompileLinkPublishAndCleanup)
{
    EndToEndTestResource testResource(FakeCompilerOutcome::Success);
    ScopedEnvironmentVariable ascendHomeEnvironment("ASCEND_HOME_PATH", testResource.ToolchainPath().c_str());
    ScopedEnvironmentVariable saveMetaEnvironment("ASCEND_OP_COMPILE_SAVE_KERNEL_META", nullptr);
    JitResourceRegistryAdapter registryAdapter(EndToEndTestResource::Lookup, &testResource);
    const uint8_t constantBytes[] = {0x01, 0xfe};
    const void* constantAddresses[] = {constantBytes};
    const uint64_t constantByteCounts[] = {sizeof(constantBytes)};
    aclrtcKernelSpecRequest request = CreateRequest(constantByteCounts, constantAddresses);

    KernelSpecializationSession session(registryAdapter);
    EXPECT_EQ(session.RunKernelSpecialization(&request, testResource.OutputElfPath().c_str()), 0);
    EXPECT_TRUE(testResource.LookupCalled());
    EXPECT_EQ(ReadFile(testResource.OutputElfPath()), "linked-elf");
    EXPECT_FALSE(fs::exists(testResource.WorktreePath()));
}

TEST(KernelSpecializationSessionTest, DoesNotReplaceExistingOutputWhenCompilationFails)
{
    EndToEndTestResource testResource(FakeCompilerOutcome::Failure);
    ScopedEnvironmentVariable ascendHomeEnvironment("ASCEND_HOME_PATH", testResource.ToolchainPath().c_str());
    ScopedEnvironmentVariable saveMetaEnvironment("ASCEND_OP_COMPILE_SAVE_KERNEL_META", nullptr);
    std::ofstream(testResource.OutputElfPath()) << "previous-elf";
    JitResourceRegistryAdapter registryAdapter(EndToEndTestResource::Lookup, &testResource);
    const uint8_t constantBytes[] = {0x01, 0xfe};
    const void* constantAddresses[] = {constantBytes};
    const uint64_t constantByteCounts[] = {sizeof(constantBytes)};
    aclrtcKernelSpecRequest request = CreateRequest(constantByteCounts, constantAddresses);

    ascendc::aclrtc::ClearCapturedKernelSpecializationLogs();
    KernelSpecializationSession session(registryAdapter);
    EXPECT_EQ(session.RunKernelSpecialization(&request, testResource.OutputElfPath().c_str()),
              ascendc::aclrtc::ACLRTC_ERROR_COMPILATION);
    EXPECT_EQ(ReadFile(testResource.OutputElfPath()), "previous-elf");
    EXPECT_FALSE(fs::exists(testResource.WorktreePath()));
    EXPECT_FALSE(ascendc::aclrtc::CapturedKernelSpecializationLogsContain(
        "specialization failed with aclError"));
}

TEST(KernelSpecializationSessionTest, ReportsInjectedRegistryFailure)
{
    JitResourceRegistryAdapter registryAdapter(ReturnInjectedRegistryFailure, nullptr);
    RegisteredJitResource resource;
    ascendc::aclrtc::ClearCapturedKernelSpecializationLogs();

    EXPECT_EQ(registryAdapter.LoadAndLookupJitResource("resource-id", resource),
              ascendc::aclrtc::ACLRTC_ERROR_FAILURE);
    EXPECT_TRUE(ascendc::aclrtc::CapturedKernelSpecializationLogsContain(
        "JIT ResourceRegistry Load/Lookup failed for resourceId 'resource-id'"));
    EXPECT_FALSE(ascendc::aclrtc::CapturedKernelSpecializationLogsContain(
        "resource_registry_adapter.cpp"));
}

TEST(KernelSpecializationSessionTest, ReportsHowToCorrectMissingOutputDirectory)
{
    EndToEndTestResource testResource(FakeCompilerOutcome::Success);
    ScopedEnvironmentVariable ascendHomeEnvironment("ASCEND_HOME_PATH", testResource.ToolchainPath().c_str());
    ScopedEnvironmentVariable saveMetaEnvironment("ASCEND_OP_COMPILE_SAVE_KERNEL_META", nullptr);
    boost::system::error_code removeError;
    fs::remove_all(testResource.OutputElfPath().parent_path(), removeError);
    ASSERT_FALSE(removeError);
    JitResourceRegistryAdapter registryAdapter(EndToEndTestResource::Lookup, &testResource);
    const uint8_t constantBytes[] = {0x01, 0xfe};
    const void* constantAddresses[] = {constantBytes};
    const uint64_t constantByteCounts[] = {sizeof(constantBytes)};
    aclrtcKernelSpecRequest request = CreateRequest(constantByteCounts, constantAddresses);

    ascendc::aclrtc::ClearCapturedKernelSpecializationLogs();
    KernelSpecializationSession session(registryAdapter);
    EXPECT_EQ(session.RunKernelSpecialization(&request, testResource.OutputElfPath().c_str()),
              ascendc::aclrtc::ACLRTC_ERROR_FAILURE);
    EXPECT_TRUE(ascendc::aclrtc::CapturedKernelSpecializationLogsContain(
        "create the output parent directory and ensure it is writable"));
    EXPECT_TRUE(ascendc::aclrtc::CapturedKernelSpecializationLogsContain(
        testResource.OutputElfPath().parent_path().string()));
}

TEST(KernelSpecializationSessionTest, RetainsWorktreeWhenKernelMetaSavingIsEnabled)
{
    EndToEndTestResource testResource(FakeCompilerOutcome::Success);
    static uint64_t diagnosticSequence = 0U;
    const fs::path diagnosticRoot =
        fs::temp_directory_path() /
        ("aclrtc_session_diagnostics_" + std::to_string(getpid()) + "_" +
         std::to_string(diagnosticSequence++));
    fs::create_directories(diagnosticRoot);
    ScopedCurrentDirectory currentDirectory(diagnosticRoot);
    ScopedEnvironmentVariable ascendHomeEnvironment("ASCEND_HOME_PATH", testResource.ToolchainPath().c_str());
    ScopedEnvironmentVariable saveMetaEnvironment("ASCEND_OP_COMPILE_SAVE_KERNEL_META", "1");
    JitResourceRegistryAdapter registryAdapter(EndToEndTestResource::Lookup, &testResource);
    const uint8_t constantBytes[] = {0x01, 0xfe};
    const void* constantAddresses[] = {constantBytes};
    const uint64_t constantByteCounts[] = {sizeof(constantBytes)};
    aclrtcKernelSpecRequest request = CreateRequest(constantByteCounts, constantAddresses);

    KernelSpecializationSession session(registryAdapter);
    EXPECT_EQ(session.RunKernelSpecialization(&request, testResource.OutputElfPath().c_str()), 0);
    EXPECT_FALSE(fs::exists(testResource.WorktreePath()));

    const fs::path processDiagnosticDirectory = diagnosticRoot / "kernel_meta" / std::to_string(getpid());
    const fs::directory_iterator sessionDirectory = fs::directory_iterator(processDiagnosticDirectory);
    ASSERT_NE(sessionDirectory, fs::directory_iterator());
    const fs::path retainedWorktreePath = sessionDirectory->path() / "resource_worktree";
    EXPECT_TRUE(fs::is_directory(retainedWorktreePath));
    const std::string replayScript = ReadFile(sessionDirectory->path() / "replay.sh");
    EXPECT_NE(replayScript.find(retainedWorktreePath.string()), std::string::npos);
    EXPECT_EQ(replayScript.find(testResource.WorktreePath().string()), std::string::npos);

    boost::system::error_code ignoredError;
    fs::remove_all(diagnosticRoot, ignoredError);
}

TEST(KernelSpecializationSessionTest, CleansWorktreeWhenRegistryReturnsIncompleteData)
{
    static uint64_t sequence = 0U;
    const fs::path testRoot = fs::temp_directory_path() /
                              ("aclrtc_incomplete_resource_" + std::to_string(getpid()) + "_" +
                               std::to_string(sequence++));
    fs::path worktreePath = testRoot / "worktree";
    fs::create_directories(worktreePath);
    ScopedEnvironmentVariable saveMetaEnvironment("ASCEND_OP_COMPILE_SAVE_KERNEL_META", nullptr);
    auto returnIncompleteResource = [](const std::string&, RegisteredJitResource& resource,
                                       void* context) -> aclError {
        resource.ownedWorktreePath = *static_cast<const fs::path*>(context);
        return 0;
    };
    JitResourceRegistryAdapter registryAdapter(returnIncompleteResource, &worktreePath);
    aclrtcKernelSpecRequest request = CreateRequest(nullptr, nullptr);

    KernelSpecializationSession session(registryAdapter);
    EXPECT_EQ(session.RunKernelSpecialization(&request, (testRoot / "kernel.elf").c_str()),
              ascendc::aclrtc::ACLRTC_ERROR_FAILURE);
    EXPECT_FALSE(fs::exists(worktreePath));
    boost::system::error_code ignoredError;
    fs::remove_all(testRoot, ignoredError);
}

TEST(KernelSpecializationApiTest, RejectsNullPointersBeforeResourceLookup)
{
    aclrtcKernelSpecRequest request {};
    ascendc::aclrtc::ClearCapturedKernelSpecializationLogs();
    EXPECT_EQ(aclrtcKernelSpecialization(nullptr, "/tmp/kernel.elf"),
              ascendc::aclrtc::ACLRTC_ERROR_INVALID_INPUT);
    EXPECT_TRUE(ascendc::aclrtc::CapturedKernelSpecializationLogsContain("request must not be null"));

    ascendc::aclrtc::ClearCapturedKernelSpecializationLogs();
    EXPECT_EQ(aclrtcKernelSpecialization(&request, nullptr), ascendc::aclrtc::ACLRTC_ERROR_INVALID_INPUT);
    EXPECT_TRUE(ascendc::aclrtc::CapturedKernelSpecializationLogsContain("outPath must not be null"));
}
} // namespace
