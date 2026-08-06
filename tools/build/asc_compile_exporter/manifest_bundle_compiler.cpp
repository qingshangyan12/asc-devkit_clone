/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "manifest_bundle_compiler.h"

#include <algorithm>
#include <thread>
#include <utility>
#include <vector>

#include "ascendc_tool_log.h"
#include "build_workspace.h"
#include "bundle_publisher.h"
#include "bundle_source_generator.h"
#include "collected_manifest_repository.h"
#include "file_utils.h"
#include "make_bundle_builder.h"
#include "manifest_bundle_types.h"

namespace ascendc {
namespace manifest_generator {
namespace {

uint32_t ResolveJobCount(uint32_t requested) noexcept
{
    const uint32_t reportedCpuCount = static_cast<uint32_t>(std::thread::hardware_concurrency());
    const uint32_t cpuCount = reportedCpuCount == 0U ? 1U : reportedCpuCount;
    if (requested != 0U) {
        return std::min(requested, cpuCount);
    }
    return cpuCount < 2U ? 1U : cpuCount / 2U;
}

} // namespace

ManifestBundleCompiler::ManifestBundleCompiler(BuildCollectedBundleRequest request) noexcept
    : request_(std::move(request))
{}

bool ManifestBundleCompiler::Compile() const
{
    ASCENDLOGI(
        "Starting bundle generation: manifest_root=%s output=%s keep_temp=%s", request_.manifestSearchRoot.c_str(),
        request_.outputSo.c_str(), request_.keepTemp ? "true" : "false");

    // Validate required inputs before creating output or temporary directories.
    if (request_.makeExecutable.empty()) {
        ASCENDLOGE("Bundle make executable must not be empty");
        return false;
    }
    if (request_.cxxCompiler.empty()) {
        ASCENDLOGE("Bundle C++ compiler must not be empty");
        return false;
    }
    std::string manifestSearchRoot;
    if (!FileUtils::ResolveDirectory(request_.manifestSearchRoot, manifestSearchRoot)) {
        ASCENDLOGE(
            "Manifest search root must exist, must be a directory, and must not be a symlink: %s",
            request_.manifestSearchRoot.c_str());
        return false;
    }

    // Normalize the output path and reject unsafe targets before starting the expensive build stages.
    if (request_.outputSo.empty()) {
        ASCENDLOGE("Bundle output path must not be empty");
        return false;
    }
    const std::string outputParent = FileUtils::ParentPath(request_.outputSo);
    const std::string outputName = FileUtils::FileName(request_.outputSo);
    if (outputParent.empty()) {
        ASCENDLOGE("Bundle output path must include a parent directory");
        return false;
    }
    if (outputName.empty() || outputName == "." || outputName == "..") {
        ASCENDLOGE("Bundle output path has an invalid file name");
        return false;
    }
    std::string absoluteOutputParent;
    if (!FileUtils::MakeAbsolutePath(outputParent, absoluteOutputParent)) {
        ASCENDLOGE("Failed to resolve bundle output directory: %s", outputParent.c_str());
        return false;
    }
    if (!FileUtils::CreateDirectories(absoluteOutputParent)) {
        ASCENDLOGE("Failed to create bundle output directory: %s", absoluteOutputParent.c_str());
        return false;
    }
    std::string resolvedOutputParent;
    if (!FileUtils::ResolveDirectory(absoluteOutputParent, resolvedOutputParent)) {
        ASCENDLOGE("Bundle output directory is unavailable: %s", absoluteOutputParent.c_str());
        return false;
    }
    const std::string output = FileUtils::JoinPath(resolvedOutputParent, outputName);
    const uint32_t jobs = ResolveJobCount(request_.jobs);
    ASCENDLOGI("Using %u parallel bundle build job(s)", jobs);

    // Load and validate the complete collection before allocating a private build workspace.
    std::vector<ManifestUnit> units;
    if (!CollectedManifestRepository(manifestSearchRoot).Load(units)) {
        return false;
    }
    BuildWorkspace workspace;
    if (!workspace.Create(std::string(), request_.keepTemp)) {
        return false;
    }
    if (request_.keepTemp) {
        ASCENDLOGI("Keeping temporary bundle directory %s", workspace.Session().c_str());
    }

    // Generate isolated translation units and compile them into a staged shared library.
    if (!BundleSourceGenerator(workspace.Session(), units).Generate()) {
        return false;
    }
    const std::string stagedOutput = FileUtils::JoinPath(workspace.Session(), "compile_resource_bundle.so");
    if (!MakeBundleBuilder(workspace.Session(), stagedOutput, request_.makeExecutable, request_.cxxCompiler, jobs)
             .Build()) {
        return false;
    }

    // Publish only after a successful build so failures never replace an existing output bundle.
    if (!BundlePublisher::Publish(stagedOutput, output)) {
        return false;
    }
    ASCENDLOGI("Bundle generation completed successfully: %s", output.c_str());
    return true;
}

} // namespace manifest_generator
} // namespace ascendc
