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

bool ReadFile(const fs::path& filePath, std::string& contents)
{
    std::ifstream inputStream(filePath, std::ios::in | std::ios::binary);
    if (!inputStream) {
        const int openError = errno;
        ACLRTC_LOG_ERROR("Failed to open source patch file: path=%s errno=%d message=%s",
                         filePath.c_str(), openError, std::strerror(openError));
        return false;
    }
    std::ostringstream contentsStream;
    contentsStream << inputStream.rdbuf();
    if (inputStream.bad()) {
        const int readError = errno;
        ACLRTC_LOG_ERROR("Failed to read source patch file: path=%s errno=%d message=%s",
                         filePath.c_str(), readError, std::strerror(readError));
        return false;
    }
    contents = contentsStream.str();
    return true;
}

bool ReplaceTemplateOnce(std::string& sourceText, const std::string& templateText,
                         const std::string& replacementText)
{
    const size_t templatePosition = sourceText.find(templateText);
    if (templatePosition == std::string::npos) {
        return false;
    }
    sourceText.replace(templatePosition, templateText.size(), replacementText);
    return true;
}

bool ReplaceFileAtomically(const fs::path& targetFilePath, const std::string& contents)
{
    static std::atomic<uint64_t> temporaryPatchFileSequence {0};
    const fs::path temporaryFilePath =
        targetFilePath.string() + ".aclrtc_patch_" + std::to_string(getpid()) + "_" +
        std::to_string(temporaryPatchFileSequence.fetch_add(1U, std::memory_order_relaxed));
    {
        std::ofstream outputStream(temporaryFilePath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!outputStream) {
            const int createError = errno;
            ACLRTC_LOG_ERROR("Failed to create temporary source patch file: path=%s errno=%d message=%s",
                             temporaryFilePath.c_str(), createError, std::strerror(createError));
            return false;
        }
        outputStream.write(contents.data(), static_cast<std::streamsize>(contents.size()));
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
    fs::rename(temporaryFilePath, targetFilePath, renameError);
    if (renameError) {
        ACLRTC_LOG_ERROR("Failed to replace patched source file: source=%s target=%s error=%d message=%s",
                         temporaryFilePath.c_str(), targetFilePath.c_str(), renameError.value(),
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
    if (worktreeDispositionFinalized_ || retentionPolicy_ == WorktreeRetentionPolicy::RetainForDiagnostics ||
        worktreePath_.empty()) {
        return;
    }
    boost::system::error_code cleanupError;
    fs::remove_all(worktreePath_, cleanupError);
    if (cleanupError) {
        ACLRTC_LOG_WARNING("Failed to clean resource worktree: path=%s error=%d message=%s",
                           worktreePath_.c_str(), cleanupError.value(), cleanupError.message().c_str());
    }
}

const fs::path& KernelCompilationWorkspace::GetWorktreePath() const noexcept
{
    return worktreePath_;
}

aclError KernelCompilationWorkspace::MaterializeCompilationInputs(const KernelCompilationPlan& compilationPlan)
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
        if (!ReadFile(filePatches.first, patchedSourceText)) {
            return ACLRTC_ERROR_FAILURE;
        }
        for (size_t patchIndex = 0U; patchIndex < filePatches.second.size(); ++patchIndex) {
            const KernelSourcePatch* sourcePatch = filePatches.second[patchIndex];
            if (!ReplaceTemplateOnce(patchedSourceText, sourcePatch->templateText, sourcePatch->replacementText)) {
                ACLRTC_LOG_ERROR(
                    "Source patch template was not found: path=%s patch_index=%zu; regenerate the JIT resource "
                    "from matching source files",
                    filePatches.first.c_str(), patchIndex);
                return ACLRTC_ERROR_FAILURE;
            }
        }
        if (!ReplaceFileAtomically(filePatches.first, patchedSourceText)) {
            return ACLRTC_ERROR_FAILURE;
        }
    }
    return ACLRTC_SUCCESS;
}

aclError KernelCompilationWorkspace::FinalizeBeforePublishingKernelElf()
{
    if (worktreeDispositionFinalized_) {
        return ACLRTC_SUCCESS;
    }
    if (retentionPolicy_ == WorktreeRetentionPolicy::RetainForDiagnostics) {
        worktreeDispositionFinalized_ = true;
        return ACLRTC_SUCCESS;
    }
    boost::system::error_code cleanupError;
    fs::remove_all(worktreePath_, cleanupError);
    if (cleanupError) {
        ACLRTC_LOG_ERROR(
            "Failed to clean resource worktree before publishing ELF: path=%s error=%d message=%s",
            worktreePath_.c_str(), cleanupError.value(), cleanupError.message().c_str());
        return ACLRTC_ERROR_FAILURE;
    }
    worktreeDispositionFinalized_ = true;
    return ACLRTC_SUCCESS;
}

} // namespace aclrtc
} // namespace ascendc
