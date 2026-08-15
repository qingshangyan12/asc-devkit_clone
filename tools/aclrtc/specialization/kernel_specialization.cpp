/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "kernel_specialization.h"

#include "compilation_command_executor.h"
#include "kernel_compilation_plan_builder.h"
#include "kernel_compilation_workspace.h"
#include "kernel_specialization_diagnostics.h"
#include "kernel_specialization_log.h"
#include "resource_registry_adapter.h"

#include <atomic>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <unistd.h>

namespace ascendc {
namespace aclrtc {
namespace {
namespace fs = boost::filesystem;

bool IsKernelMetaSavingEnabled()
{
    const char* saveKernelMeta = std::getenv("ASCEND_OP_COMPILE_SAVE_KERNEL_META");
    return saveKernelMeta != nullptr && std::strcmp(saveKernelMeta, "1") == 0;
}

bool CopyFileContents(int sourceFileDescriptor, int destinationFileDescriptor)
{
    char copyBuffer[64U * 1024U];
    while (true) {
        const ssize_t readByteCount = read(sourceFileDescriptor, copyBuffer, sizeof(copyBuffer));
        if (readByteCount == 0) {
            return true;
        }
        if (readByteCount < 0) {
            if (errno == EINTR) {
                continue;
            }
            ACLRTC_LOG_ERROR("Failed to read linked kernel ELF while publishing: errno=%d", errno);
            return false;
        }
        ssize_t writtenByteCount = 0;
        while (writtenByteCount < readByteCount) {
            const ssize_t writeResult =
                write(destinationFileDescriptor, copyBuffer + writtenByteCount, readByteCount - writtenByteCount);
            if (writeResult < 0 && errno == EINTR) {
                continue;
            }
            if (writeResult <= 0) {
                ACLRTC_LOG_ERROR("Failed to write linked kernel ELF while publishing: errno=%d", errno);
                return false;
            }
            writtenByteCount += writeResult;
        }
    }
}

bool CreateTemporaryOutputFile(const fs::path& outputElfPath, fs::path& temporaryOutputPath,
                               int& temporaryOutputFileDescriptor)
{
    constexpr size_t MAX_TEMPORARY_FILE_ATTEMPTS = 32U;
    static std::atomic<uint64_t> temporaryOutputFileSequence {0};
    for (size_t attempt = 0U; attempt < MAX_TEMPORARY_FILE_ATTEMPTS; ++attempt) {
        temporaryOutputPath =
            outputElfPath.string() + ".aclrtc_tmp_" + std::to_string(getpid()) + "_" +
            std::to_string(temporaryOutputFileSequence.fetch_add(1U, std::memory_order_relaxed));
        temporaryOutputFileDescriptor =
            open(temporaryOutputPath.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0644);
        if (temporaryOutputFileDescriptor >= 0) {
            return true;
        }
        if (errno != EEXIST) {
            ACLRTC_LOG_ERROR("Failed to create temporary output ELF: path=%s errno=%d",
                             temporaryOutputPath.c_str(), errno);
            return false;
        }
    }
    ACLRTC_LOG_ERROR("Failed to create a unique temporary output ELF after %zu attempts: output=%s",
                     MAX_TEMPORARY_FILE_ATTEMPTS, outputElfPath.c_str());
    return false;
}

void RemoveTemporaryOutputFile(const fs::path& temporaryOutputPath)
{
    boost::system::error_code removeError;
    fs::remove(temporaryOutputPath, removeError);
    if (removeError) {
        ACLRTC_LOG_WARNING("Failed to remove temporary output ELF: path=%s error=%d",
                           temporaryOutputPath.c_str(), removeError.value());
    }
}
} // namespace

KernelSpecializationSession::KernelSpecializationSession(JitResourceRegistryAdapter& resourceRegistryAdapter) noexcept
    : resourceRegistryAdapter_(resourceRegistryAdapter)
{}

aclError KernelSpecializationSession::RunKernelSpecialization(
    const aclrtcKernelSpecRequest* apiRequest, const char* outputElfPath)
{
    if (apiRequest == nullptr) {
        ACLRTC_LOG_ERROR("aclrtcKernelSpecialization request must not be null");
        return ACLRTC_ERROR_INVALID_INPUT;
    }
    if (outputElfPath == nullptr) {
        ACLRTC_LOG_ERROR("aclrtcKernelSpecialization outPath must not be null");
        return ACLRTC_ERROR_INVALID_INPUT;
    }

    NormalizedKernelSpecializationRequest specializationRequest;
    aclError result = NormalizeKernelSpecializationRequest(*apiRequest, outputElfPath, specializationRequest);
    if (result != ACLRTC_SUCCESS) {
        return result;
    }

    const bool saveKernelMetaEnabled = IsKernelMetaSavingEnabled();
    const DiagnosticCollectionMode diagnosticCollectionMode = saveKernelMetaEnabled
        ? DiagnosticCollectionMode::Enabled
        : DiagnosticCollectionMode::Disabled;
    KernelSpecializationDiagnostics diagnostics(specializationRequest.requestId, diagnosticCollectionMode);
    auto finishRequest = [&](aclError specializationResult) {
        const OutputPublicationStatus publicationStatus = specializationResult == ACLRTC_SUCCESS
            ? OutputPublicationStatus::Published
            : OutputPublicationStatus::NotPublished;
        diagnostics.RecordSpecializationResult(specializationRequest.outputElfPath, publicationStatus);
        if (specializationResult == ACLRTC_SUCCESS) {
            ACLRTC_LOG_INFO("Kernel %s specialization completed", specializationRequest.kernelName.c_str());
        } else {
            ACLRTC_LOG_ERROR("Kernel %s specialization failed with aclError %d",
                             specializationRequest.kernelName.c_str(), specializationResult);
        }
        return specializationResult;
    };

    RegisteredJitResource registeredResource;
    result = resourceRegistryAdapter_.LoadAndLookupJitResource(specializationRequest.resourceId, registeredResource);
    if (result != ACLRTC_SUCCESS) {
        return finishRequest(result);
    }

    const WorktreeRetentionPolicy worktreeRetentionPolicy = saveKernelMetaEnabled
        ? WorktreeRetentionPolicy::RetainForDiagnostics
        : WorktreeRetentionPolicy::RemoveAfterCompilation;
    KernelCompilationWorkspace compilationWorkspace(
        std::move(registeredResource.ownedWorktreePath), worktreeRetentionPolicy);
    diagnostics.RecordResourceWorktreePath(compilationWorkspace.GetWorktreePath());
    if (registeredResource.borrowedManifest == nullptr) {
        ACLRTC_LOG_ERROR("ResourceRegistry returned a null resource Manifest");
        return finishRequest(ACLRTC_ERROR_FAILURE);
    }
    if (compilationWorkspace.GetWorktreePath().empty()) {
        ACLRTC_LOG_ERROR("ResourceRegistry returned an empty resource worktree path");
        return finishRequest(ACLRTC_ERROR_FAILURE);
    }
    if (registeredResource.externalSourceFilePath.empty()) {
        ACLRTC_LOG_ERROR("ResourceRegistry returned an empty sourceFilePath");
        return finishRequest(ACLRTC_ERROR_FAILURE);
    }
    diagnostics.SaveManifestForDiagnostics(*registeredResource.borrowedManifest);

    KernelCompilationPlan compilationPlan;
    result = KernelCompilationPlanBuilder(specializationRequest, *registeredResource.borrowedManifest,
                                          compilationWorkspace.GetWorktreePath(),
                                          registeredResource.externalSourceFilePath)
                 .BuildCompilationPlan(compilationPlan);
    if (result != ACLRTC_SUCCESS) {
        return finishRequest(result);
    }
    result = compilationWorkspace.MaterializeCompilationInputs(compilationPlan);
    if (result != ACLRTC_SUCCESS) {
        return finishRequest(result);
    }

    CompilationCommandExecutor commandExecutor(diagnostics, CompilationCommandExecutionLimits::ProductionDefaults());
    result = commandExecutor.ExecuteCompilationPlan(compilationPlan);
    if (result != ACLRTC_SUCCESS) {
        return finishRequest(result);
    }

    result = PublishKernelElf(compilationPlan.linkedKernelElfPath, specializationRequest.outputElfPath,
                              compilationWorkspace);
    return finishRequest(result);
}

aclError KernelSpecializationSession::PublishKernelElf(
    const fs::path& linkedKernelElfPath, const fs::path& outputElfPath,
    KernelCompilationWorkspace& compilationWorkspace) const
{
    const int linkedKernelFileDescriptor = open(linkedKernelElfPath.c_str(), O_RDONLY | O_CLOEXEC);
    if (linkedKernelFileDescriptor < 0) {
        ACLRTC_LOG_ERROR("Failed to open linked kernel ELF: path=%s errno=%d", linkedKernelElfPath.c_str(), errno);
        return ACLRTC_ERROR_FAILURE;
    }

    fs::path temporaryOutputPath;
    int temporaryOutputFileDescriptor = -1;
    if (!CreateTemporaryOutputFile(outputElfPath, temporaryOutputPath, temporaryOutputFileDescriptor)) {
        if (close(linkedKernelFileDescriptor) != 0) {
            ACLRTC_LOG_WARNING("Failed to close linked kernel ELF after publish setup failure: path=%s errno=%d",
                               linkedKernelElfPath.c_str(), errno);
        }
        return ACLRTC_ERROR_FAILURE;
    }

    const bool copySucceeded = CopyFileContents(linkedKernelFileDescriptor, temporaryOutputFileDescriptor);
    const bool sourceCloseSucceeded = close(linkedKernelFileDescriptor) == 0;
    const int sourceCloseError = sourceCloseSucceeded ? 0 : errno;
    const bool destinationCloseSucceeded = close(temporaryOutputFileDescriptor) == 0;
    const int destinationCloseError = destinationCloseSucceeded ? 0 : errno;
    if (!copySucceeded || !sourceCloseSucceeded || !destinationCloseSucceeded) {
        if (!sourceCloseSucceeded) {
            ACLRTC_LOG_ERROR("Failed to close linked kernel ELF: path=%s errno=%d",
                             linkedKernelElfPath.c_str(), sourceCloseError);
        }
        if (!destinationCloseSucceeded) {
            ACLRTC_LOG_ERROR("Failed to close temporary output ELF: path=%s errno=%d",
                             temporaryOutputPath.c_str(), destinationCloseError);
        }
        RemoveTemporaryOutputFile(temporaryOutputPath);
        return ACLRTC_ERROR_FAILURE;
    }

    const aclError finalizeResult = compilationWorkspace.FinalizeBeforePublishingKernelElf();
    if (finalizeResult != ACLRTC_SUCCESS) {
        RemoveTemporaryOutputFile(temporaryOutputPath);
        return finalizeResult;
    }

    boost::system::error_code renameError;
    fs::rename(temporaryOutputPath, outputElfPath, renameError);
    if (renameError) {
        ACLRTC_LOG_ERROR("Failed to publish kernel ELF: source=%s target=%s error=%d",
                         temporaryOutputPath.c_str(), outputElfPath.c_str(), renameError.value());
        RemoveTemporaryOutputFile(temporaryOutputPath);
        return ACLRTC_ERROR_FAILURE;
    }
    return ACLRTC_SUCCESS;
}

} // namespace aclrtc
} // namespace ascendc

extern "C" aclError aclrtcKernelSpecialization(const aclrtcKernelSpecRequest* request, const char* outPath)
{
    try {
        ascendc::aclrtc::JitResourceRegistryAdapter resourceRegistryAdapter;
        ascendc::aclrtc::KernelSpecializationSession specializationSession(resourceRegistryAdapter);
        return specializationSession.RunKernelSpecialization(request, outPath);
    } catch (const std::bad_alloc&) {
        ACLRTC_LOG_ERROR("Out of memory while specializing kernel");
        return ascendc::aclrtc::ACLRTC_ERROR_OUT_OF_MEMORY;
    } catch (const std::exception& exception) {
        ACLRTC_LOG_ERROR("Kernel specialization failed with exception: %s", exception.what());
        return ascendc::aclrtc::ACLRTC_ERROR_FAILURE;
    } catch (...) {
        ACLRTC_LOG_ERROR("Kernel specialization failed with an unknown exception");
        return ascendc::aclrtc::ACLRTC_ERROR_FAILURE;
    }
}
