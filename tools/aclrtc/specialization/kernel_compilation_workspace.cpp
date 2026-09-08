/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "kernel_compilation_workspace.h"

#include "ascendc_tool_log.h"
#include "file_utils.h"

#include <cstdio>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <unistd.h>

namespace ascendc {
namespace aclrtc {
namespace {
namespace fs = boost::filesystem;

bool ReadSourceFileContents(const fs::path& sourceFilePath, std::string& sourceFileContents)
{
    std::string canonicalSourcePath;
    if (!FileUtils::ResolveCanonicalPath(sourceFilePath.string(), canonicalSourcePath) ||
        !FileUtils::IsRegularFile(canonicalSourcePath)) {
        return false;
    }
    std::ifstream inputStream(canonicalSourcePath, std::ios::in | std::ios::binary);
    if (!inputStream) {
        const int openError = errno;
        ASCENDLOGE(
            "Failed to open source patch file: path=%s errno=%d message=%s", sourceFilePath.c_str(), openError,
            std::strerror(openError));
        return false;
    }
    std::ostringstream sourceFileContentsStream;
    sourceFileContentsStream << inputStream.rdbuf();
    if (inputStream.bad()) {
        const int readError = errno;
        ASCENDLOGE(
            "Failed to read source patch file: path=%s errno=%d message=%s", sourceFilePath.c_str(), readError,
            std::strerror(readError));
        return false;
    }
    sourceFileContents = sourceFileContentsStream.str();
    return true;
}

bool ReplaceAllSourceTemplateOccurrences(
    std::string& sourceFileContents, const std::string& sourceTemplateText, const std::string& replacementText)
{
    bool replacedAtLeastOnce = false;
    size_t searchPosition = 0U;
    while ((searchPosition = sourceFileContents.find(sourceTemplateText, searchPosition)) != std::string::npos) {
        sourceFileContents.replace(searchPosition, sourceTemplateText.size(), replacementText);
        searchPosition += replacementText.size();
        replacedAtLeastOnce = true;
    }
    return replacedAtLeastOnce;
}

bool WritePatchedSourceFileAtomically(const fs::path& sourceFilePath, const std::string& patchedSourceFileContents)
{
    std::string canonicalSourcePath;
    if (!FileUtils::ResolveRegularFilePathForWrite(sourceFilePath.string(), canonicalSourcePath)) {
        return false;
    }
    // mkstemp creates a private file exclusively and never follows an existing symlink.
    std::string temporaryFileTemplate = canonicalSourcePath + ".aclrtc_patch_XXXXXX";
    const int temporaryFileDescriptor = mkstemp(temporaryFileTemplate.data());
    if (temporaryFileDescriptor < 0) {
        ASCENDLOGE(
            "Failed to create temporary source patch file: path=%s errno=%d", canonicalSourcePath.c_str(), errno);
        return false;
    }
    // Keep cleanup paths allocation-free after acquiring the descriptor.
    FILE* outputStream = fdopen(temporaryFileDescriptor, "wb");
    if (outputStream == nullptr) {
        (void)close(temporaryFileDescriptor);
        (void)unlink(temporaryFileTemplate.c_str());
        return false;
    }
    const size_t writtenBytes =
        fwrite(patchedSourceFileContents.data(), 1U, patchedSourceFileContents.size(), outputStream);
    const bool closeSucceeded = fclose(outputStream) == 0;
    if (writtenBytes != patchedSourceFileContents.size() || !closeSucceeded) {
        ASCENDLOGE("Failed to write or close temporary source patch file: path=%s", temporaryFileTemplate.c_str());
        (void)unlink(temporaryFileTemplate.c_str());
        return false;
    }
    const fs::path temporaryFilePath(temporaryFileTemplate);
    boost::system::error_code renameError;
    fs::rename(temporaryFilePath, fs::path(canonicalSourcePath), renameError);
    if (renameError) {
        ASCENDLOGE(
            "Failed to replace patched source file: source=%s target=%s error=%d message=%s", temporaryFilePath.c_str(),
            sourceFilePath.c_str(), renameError.value(), renameError.message().c_str());
        boost::system::error_code ignoredError;
        fs::remove(temporaryFilePath, ignoredError);
        return false;
    }
    return true;
}
} // namespace

KernelCompilationWorkspace::KernelCompilationWorkspace(fs::path worktreePath, WorktreeRetentionPolicy retentionPolicy)
    : retentionPolicy_(retentionPolicy)
{
    if (!worktreePath.empty()) {
        (void)worktreeCleanupGuard_.RegisterDirectoryForCleanup(worktreePath.string());
    }
    if (retentionPolicy_ == WorktreeRetentionPolicy::RetainAfterCompilation) {
        worktreeCleanupGuard_.PreserveDirectory();
    }
}

fs::path KernelCompilationWorkspace::GetWorktreePath() const
{
    return fs::path(worktreeCleanupGuard_.GetDirectoryPath());
}

aclError KernelCompilationWorkspace::CreateOutputDirectoriesAndApplySourcePatches(
    const KernelCompilationPlan& compilationPlan) const
{
    for (const fs::path& outputDirectoryPath : compilationPlan.requiredOutputDirectoryPaths) {
        boost::system::error_code directoryError;
        fs::create_directories(outputDirectoryPath, directoryError);
        if (directoryError) {
            ASCENDLOGE(
                "Failed to create compilation output directory: path=%s error=%d message=%s",
                outputDirectoryPath.c_str(), directoryError.value(), directoryError.message().c_str());
            return ACLRTC_ERROR_FAILURE;
        }
    }

    std::map<fs::path, std::vector<const KernelSourcePatch*>> patchesByFile;
    for (const KernelSourcePatch& sourcePatch : compilationPlan.sourcePatches) {
        patchesByFile[sourcePatch.targetFilePath].push_back(&sourcePatch);
    }
    for (const auto& filePatches : patchesByFile) {
        std::string patchedSourceText;
        if (!ReadSourceFileContents(filePatches.first, patchedSourceText)) {
            return ACLRTC_ERROR_FAILURE;
        }
        for (size_t patchIndex = 0U; patchIndex < filePatches.second.size(); ++patchIndex) {
            const KernelSourcePatch* sourcePatch = filePatches.second[patchIndex];
            if (!ReplaceAllSourceTemplateOccurrences(
                    patchedSourceText, sourcePatch->templateText, sourcePatch->replacementText)) {
                ASCENDLOGE(
                    "Source patch template was not found: path=%s patch_index=%zu; regenerate the JIT resource "
                    "from matching source files",
                    filePatches.first.c_str(), patchIndex);
                return ACLRTC_ERROR_FAILURE;
            }
        }
        if (!WritePatchedSourceFileAtomically(filePatches.first, patchedSourceText)) {
            return ACLRTC_ERROR_FAILURE;
        }
    }
    return ACLRTC_SUCCESS;
}

aclError KernelCompilationWorkspace::CleanupWorktreeBeforeElfPublication()
{
    if (worktreeCleanupGuard_.GetDirectoryPath().empty()) {
        return ACLRTC_SUCCESS;
    }
    if (retentionPolicy_ == WorktreeRetentionPolicy::RetainAfterCompilation) {
        return ACLRTC_SUCCESS;
    }
    const std::string worktreePath = worktreeCleanupGuard_.GetDirectoryPath();
    if (!worktreeCleanupGuard_.RemoveDirectory()) {
        ASCENDLOGE(
            "Failed to clean resource worktree before publishing ELF: path=%s; ensure the path is removable and retry",
            worktreePath.c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    return ACLRTC_SUCCESS;
}

} // namespace aclrtc
} // namespace ascendc
