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

#include "kernel_specialization_log.h"

#include <atomic>
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
    std::ifstream inputStream(sourceFilePath, std::ios::in | std::ios::binary);
    if (!inputStream) {
        const int openError = errno;
        ACLRTC_LOG_ERROR("Failed to open source patch file: path=%s errno=%d message=%s",
                         sourceFilePath.c_str(), openError, std::strerror(openError));
        return false;
    }
    std::ostringstream sourceFileContentsStream;
    sourceFileContentsStream << inputStream.rdbuf();
    if (inputStream.bad()) {
        const int readError = errno;
        ACLRTC_LOG_ERROR("Failed to read source patch file: path=%s errno=%d message=%s",
                         sourceFilePath.c_str(), readError, std::strerror(readError));
        return false;
    }
    sourceFileContents = sourceFileContentsStream.str();
    return true;
}

bool ReplaceFirstSourceTemplateOccurrence(std::string& sourceFileContents, const std::string& sourceTemplateText,
                                          const std::string& replacementText)
{
    const size_t templatePosition = sourceFileContents.find(sourceTemplateText);
    if (templatePosition == std::string::npos) {
        return false;
    }
    sourceFileContents.replace(templatePosition, sourceTemplateText.size(), replacementText);
    return true;
}

bool WritePatchedSourceFileAtomically(const fs::path& sourceFilePath, const std::string& patchedSourceFileContents)
{
    static std::atomic<uint64_t> temporaryPatchFileSequence {0};
    const fs::path temporaryFilePath =
        sourceFilePath.string() + ".aclrtc_patch_" + std::to_string(getpid()) + "_" +
        std::to_string(temporaryPatchFileSequence.fetch_add(1U, std::memory_order_relaxed));
    {
        std::ofstream outputStream(temporaryFilePath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!outputStream) {
            const int createError = errno;
            ACLRTC_LOG_ERROR("Failed to create temporary source patch file: path=%s errno=%d message=%s",
                             temporaryFilePath.c_str(), createError, std::strerror(createError));
            return false;
        }
        outputStream.write(patchedSourceFileContents.data(),
                           static_cast<std::streamsize>(patchedSourceFileContents.size()));
        if (!outputStream) {
            const int writeError = errno;
            ACLRTC_LOG_ERROR("Failed to write temporary source patch file: path=%s errno=%d message=%s",
                             temporaryFilePath.c_str(), writeError, std::strerror(writeError));
            boost::system::error_code ignoredError;
            fs::remove(temporaryFilePath, ignoredError);
            return false;
        }
    }
    boost::system::error_code renameError;
    fs::rename(temporaryFilePath, sourceFilePath, renameError);
    if (renameError) {
        ACLRTC_LOG_ERROR("Failed to replace patched source file: source=%s target=%s error=%d message=%s",
                         temporaryFilePath.c_str(), sourceFilePath.c_str(), renameError.value(),
                         renameError.message().c_str());
        boost::system::error_code ignoredError;
        fs::remove(temporaryFilePath, ignoredError);
        return false;
    }
    return true;
}
} // namespace

KernelCompilationWorkspace::KernelCompilationWorkspace(
    fs::path worktreePath, WorktreeRetentionPolicy retentionPolicy)
    : worktreePath_(std::move(worktreePath)), retentionPolicy_(retentionPolicy)
{}

KernelCompilationWorkspace::~KernelCompilationWorkspace()
{
    if (retentionPolicy_ == WorktreeRetentionPolicy::RetainForDiagnostics || worktreePath_.empty()) {
        return;
    }
    boost::system::error_code cleanupError;
    fs::remove_all(worktreePath_, cleanupError);
    if (cleanupError) {
        ACLRTC_LOG_WARNING(
            "Failed to clean resource worktree: path=%s error=%d message=%s; remove the worktree manually if it "
            "is no longer needed",
            worktreePath_.c_str(), cleanupError.value(), cleanupError.message().c_str());
    }
}

const fs::path& KernelCompilationWorkspace::GetWorktreePath() const noexcept
{
    return worktreePath_;
}

bool KernelCompilationWorkspace::TryMoveWorktreeToDiagnosticsDirectory(
    const fs::path& diagnosticWorktreePath) noexcept
{
    if (worktreePath_.empty() || diagnosticWorktreePath.empty()) {
        ACLRTC_LOG_WARNING(
            "Cannot retain resource worktree because its source or diagnostic path is empty: source=%s target=%s",
            worktreePath_.c_str(), diagnosticWorktreePath.c_str());
        return false;
    }

    boost::system::error_code pathError;
    const bool diagnosticPathExists = fs::exists(diagnosticWorktreePath, pathError);
    const bool diagnosticPathInspectionFailed =
        pathError && pathError.value() != ENOENT && pathError.value() != ENOTDIR;
    if (diagnosticPathInspectionFailed || diagnosticPathExists) {
        ACLRTC_LOG_WARNING(
            "Cannot retain resource worktree because the diagnostic target is unavailable: source=%s target=%s "
            "error=%d message=%s",
            worktreePath_.c_str(), diagnosticWorktreePath.c_str(), pathError.value(),
            pathError ? pathError.message().c_str() : "target already exists");
        return false;
    }

    pathError.clear();
    fs::rename(worktreePath_, diagnosticWorktreePath, pathError);
    if (!pathError) {
        worktreePath_ = diagnosticWorktreePath;
        return true;
    }

    boost::system::error_code copyError;
    fs::copy(worktreePath_, diagnosticWorktreePath, fs::copy_options::recursive, copyError);
    if (copyError) {
        boost::system::error_code ignoredError;
        fs::remove_all(diagnosticWorktreePath, ignoredError);
        ACLRTC_LOG_WARNING(
            "Failed to retain resource worktree in the diagnostic directory: source=%s target=%s rename_error=%s "
            "copy_error=%s; compilation will continue using the original worktree",
            worktreePath_.c_str(), diagnosticWorktreePath.c_str(), pathError.message().c_str(),
            copyError.message().c_str());
        return false;
    }

    const fs::path originalWorktreePath = worktreePath_;
    worktreePath_ = diagnosticWorktreePath;
    boost::system::error_code cleanupError;
    fs::remove_all(originalWorktreePath, cleanupError);
    if (cleanupError) {
        ACLRTC_LOG_WARNING(
            "Retained resource worktree but failed to remove its original copy: source=%s retained=%s error=%d "
            "message=%s",
            originalWorktreePath.c_str(), worktreePath_.c_str(), cleanupError.value(),
            cleanupError.message().c_str());
    }
    return true;
}

aclError KernelCompilationWorkspace::CreateOutputDirectoriesAndApplySourcePatches(
    const KernelCompilationPlan& compilationPlan)
{
    for (const fs::path& outputDirectoryPath : compilationPlan.requiredOutputDirectoryPaths) {
        boost::system::error_code directoryError;
        fs::create_directories(outputDirectoryPath, directoryError);
        if (directoryError) {
            ACLRTC_LOG_ERROR("Failed to create compilation output directory: path=%s error=%d message=%s",
                             outputDirectoryPath.c_str(), directoryError.value(),
                             directoryError.message().c_str());
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
            if (!ReplaceFirstSourceTemplateOccurrence(
                    patchedSourceText, sourcePatch->templateText, sourcePatch->replacementText)) {
                ACLRTC_LOG_ERROR(
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

aclError KernelCompilationWorkspace::RemoveWorktreeBeforePublishingElfIfNeeded()
{
    if (worktreePath_.empty()) {
        return ACLRTC_SUCCESS;
    }
    if (retentionPolicy_ == WorktreeRetentionPolicy::RetainForDiagnostics) {
        return ACLRTC_SUCCESS;
    }
    boost::system::error_code cleanupError;
    fs::remove_all(worktreePath_, cleanupError);
    if (cleanupError) {
        ACLRTC_LOG_ERROR(
            "Failed to clean resource worktree before publishing ELF: path=%s error=%d message=%s; ensure the "
            "path is removable and retry",
            worktreePath_.c_str(), cleanupError.value(), cleanupError.message().c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    worktreePath_.clear();
    return ACLRTC_SUCCESS;
}

} // namespace aclrtc
} // namespace ascendc
