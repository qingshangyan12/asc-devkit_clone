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

#include "compilation_plan_executor.h"
#include "kernel_compilation_plan_builder.h"
#include "kernel_compilation_workspace.h"
#include "kernel_specialization_diagnostics.h"
#include "kernel_specialization_log.h"
#include "resource_registry.h"

#include <atomic>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <mutex>
#include <unistd.h>

namespace ascendc {
namespace aclrtc {
namespace {
namespace fs = boost::filesystem;
using specialization_compile::ResourceData;
using specialization_compile::ResourceRegistry;
using specialization_compile::ResourceStatus;
using specialization_compile::ResourceStatusToString;

bool IsKernelMetaSavingEnabled()
{
    const char* saveKernelMeta = std::getenv("ASCEND_OP_COMPILE_SAVE_KERNEL_META");
    return saveKernelMeta != nullptr && std::strcmp(saveKernelMeta, "1") == 0;
}

bool CopyFileDescriptorContents(int sourceFileDescriptor, const fs::path& sourceFilePath,
                                int destinationFileDescriptor, const fs::path& destinationFilePath)
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
            const int readError = errno;
            ACLRTC_LOG_ERROR(
                "Failed to read linked kernel ELF while publishing: path=%s errno=%d message=%s",
                sourceFilePath.c_str(), readError, std::strerror(readError));
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
                const int writeError = errno;
                ACLRTC_LOG_ERROR(
                    "Failed to write linked kernel ELF while publishing: path=%s errno=%d message=%s",
                    destinationFilePath.c_str(), writeError, std::strerror(writeError));
                return false;
            }
            writtenByteCount += writeResult;
        }
    }
}

bool CreateUniqueTemporaryOutputFile(const fs::path& outputElfPath, fs::path& temporaryOutputPath,
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
            const int createError = errno;
            ACLRTC_LOG_ERROR(
                "Failed to create temporary output ELF: path=%s parent=%s errno=%d message=%s; create the "
                "output parent directory and ensure it is writable",
                temporaryOutputPath.c_str(), outputElfPath.parent_path().c_str(), createError,
                std::strerror(createError));
            return false;
        }
    }
    ACLRTC_LOG_ERROR("Failed to create a unique temporary output ELF after %zu attempts: output=%s",
                     MAX_TEMPORARY_FILE_ATTEMPTS, outputElfPath.c_str());
    return false;
}

void RemoveTemporaryOutputFileBestEffort(const fs::path& temporaryOutputPath)
{
    boost::system::error_code removeError;
    fs::remove(temporaryOutputPath, removeError);
    if (removeError) {
        ACLRTC_LOG_WARNING(
            "Failed to remove temporary output ELF: path=%s error=%d message=%s; remove the file manually if it "
            "remains",
            temporaryOutputPath.c_str(), removeError.value(), removeError.message().c_str());
    }
}
} // namespace

aclError KernelSpecializationSession::PrepareCompilationResource(
    const std::string& resourceId, MaterializedKernelCompilationResource& compilationResource)
{
    ResourceRegistry& resourceRegistry = ResourceRegistry::Instance();
    const ResourceStatus loadStatus = resourceRegistry.Load(nullptr);
    if (loadStatus != ResourceStatus::Success) {
        ACLRTC_LOG_ERROR(
            "Failed to load compilation resources before specializing resourceId '%s': status=%s(%d); check the "
            "preceding ResourceRegistry logs and configured OPP paths",
            resourceId.c_str(), ResourceStatusToString(loadStatus), static_cast<int32_t>(loadStatus));
        return ACLRTC_ERROR_FAILURE;
    }

    // ResourceRegistry refreshes resourceDir on every lookup. Serialize specialization lookups until the returned
    // paths are copied so another specialization request cannot replace them between Lookup and the copies.
    static std::mutex resourceLookupMutex;
    std::lock_guard<std::mutex> resourceLookupLock(resourceLookupMutex);
    const ResourceData* resourceData = nullptr;
    const ResourceStatus lookupStatus = resourceRegistry.Lookup(resourceId.c_str(), resourceData);
    if (lookupStatus != ResourceStatus::Success) {
        ACLRTC_LOG_ERROR(
            "Failed to find or materialize compilation resource '%s': status=%s(%d); check resourceId and the "
            "preceding ResourceRegistry logs",
            resourceId.c_str(), ResourceStatusToString(lookupStatus), static_cast<int32_t>(lookupStatus));
        return ACLRTC_ERROR_FAILURE;
    }
    if (resourceData == nullptr) {
        ACLRTC_LOG_ERROR(
            "ResourceRegistry returned success without resource data for resourceId '%s'; report this internal "
            "ResourceRegistry error",
            resourceId.c_str());
        return ACLRTC_ERROR_FAILURE;
    }

    compilationResource.borrowedManifest = &resourceData->json;
    compilationResource.ownedResourceDirectoryPath = resourceData->resourceDir;
    compilationResource.externalSourceDirectoryPath = resourceData->sourceFilePath;
    return ACLRTC_SUCCESS;
}

aclError KernelSpecializationSession::FinalizeSpecializationResult(
    const NormalizedKernelSpecializationRequest& specializationRequest,
    KernelSpecializationDiagnostics& diagnostics, aclError specializationResult) noexcept
{
    const OutputPublicationStatus publicationStatus = specializationResult == ACLRTC_SUCCESS
        ? OutputPublicationStatus::Published
        : OutputPublicationStatus::NotPublished;
    diagnostics.WriteSpecializationResult(specializationRequest.outputElfPath, publicationStatus);
    if (specializationResult == ACLRTC_SUCCESS) {
        ACLRTC_LOG_INFO("Kernel %s specialization completed: output=%s",
                        specializationRequest.kernelName.c_str(), specializationRequest.outputElfPath.c_str());
    }
    return specializationResult;
}

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
    aclError specializationResult =
        NormalizeKernelSpecializationRequest(*apiRequest, outputElfPath, specializationRequest);
    if (specializationResult != ACLRTC_SUCCESS) {
        return specializationResult;
    }

    MaterializedKernelCompilationResource compilationResource;
    specializationResult = PrepareCompilationResource(specializationRequest.resourceId, compilationResource);
    if (specializationResult != ACLRTC_SUCCESS) {
        const DiagnosticCollectionMode diagnosticCollectionMode = IsKernelMetaSavingEnabled()
            ? DiagnosticCollectionMode::Enabled
            : DiagnosticCollectionMode::Disabled;
        KernelSpecializationDiagnostics diagnostics(
            specializationRequest.specializationSessionId, diagnosticCollectionMode);
        return FinalizeSpecializationResult(specializationRequest, diagnostics, specializationResult);
    }
    return RunSpecializationWithMaterializedResource(specializationRequest, std::move(compilationResource));
}

aclError KernelSpecializationSession::RunSpecializationWithMaterializedResource(
    const NormalizedKernelSpecializationRequest& specializationRequest,
    MaterializedKernelCompilationResource compilationResource)
{
    const bool saveKernelMetaEnabled = IsKernelMetaSavingEnabled();
    KernelCompilationWorkspace compilationWorkspace(
        std::move(compilationResource.ownedResourceDirectoryPath), WorktreeRetentionPolicy::RemoveAfterCompilation);

    const DiagnosticCollectionMode diagnosticCollectionMode = saveKernelMetaEnabled
        ? DiagnosticCollectionMode::Enabled
        : DiagnosticCollectionMode::Disabled;
    KernelSpecializationDiagnostics diagnostics(
        specializationRequest.specializationSessionId, diagnosticCollectionMode);

    if (saveKernelMetaEnabled) {
        fs::path resourceWorktreeRetentionPath;
        if (diagnostics.GetResourceWorktreeRetentionPath(resourceWorktreeRetentionPath)) {
            compilationWorkspace.TryMoveWorktreeToDiagnosticsDirectory(resourceWorktreeRetentionPath);
        }
    }
    diagnostics.RecordWorktreePath(compilationWorkspace.GetWorktreePath());
    diagnostics.WriteManifestSnapshot(*compilationResource.borrowedManifest);

    KernelCompilationPlan compilationPlan;
    aclError specializationResult =
        KernelCompilationPlanBuilder(specializationRequest, *compilationResource.borrowedManifest,
                                     compilationWorkspace.GetWorktreePath(),
                                     compilationResource.externalSourceDirectoryPath)
            .BuildCompilationPlan(compilationPlan);
    if (specializationResult != ACLRTC_SUCCESS) {
        return FinalizeSpecializationResult(specializationRequest, diagnostics, specializationResult);
    }
    specializationResult = compilationWorkspace.CreateOutputDirectoriesAndApplySourcePatches(compilationPlan);
    if (specializationResult != ACLRTC_SUCCESS) {
        return FinalizeSpecializationResult(specializationRequest, diagnostics, specializationResult);
    }

    CompilationPlanExecutor planExecutor(diagnostics, CompilationCommandExecutionLimits::ProductionDefaults());
    specializationResult = planExecutor.ExecuteCompilationPlan(compilationPlan);
    if (specializationResult != ACLRTC_SUCCESS) {
        return FinalizeSpecializationResult(specializationRequest, diagnostics, specializationResult);
    }

    specializationResult = PublishKernelElf(compilationPlan.linkedKernelElfPath,
                                            specializationRequest.outputElfPath, compilationWorkspace);
    return FinalizeSpecializationResult(specializationRequest, diagnostics, specializationResult);
}

aclError KernelSpecializationSession::PublishKernelElf(
    const fs::path& linkedKernelElfPath, const fs::path& outputElfPath,
    KernelCompilationWorkspace& compilationWorkspace) const
{
    const int linkedKernelFileDescriptor = open(linkedKernelElfPath.c_str(), O_RDONLY | O_CLOEXEC);
    if (linkedKernelFileDescriptor < 0) {
        const int openError = errno;
        ACLRTC_LOG_ERROR("Failed to open linked kernel ELF: path=%s errno=%d message=%s",
                         linkedKernelElfPath.c_str(), openError, std::strerror(openError));
        return ACLRTC_ERROR_FAILURE;
    }

    fs::path temporaryOutputPath;
    int temporaryOutputFileDescriptor = -1;
    if (!CreateUniqueTemporaryOutputFile(outputElfPath, temporaryOutputPath, temporaryOutputFileDescriptor)) {
        if (close(linkedKernelFileDescriptor) != 0) {
            const int closeError = errno;
            ACLRTC_LOG_WARNING(
                "Failed to close linked kernel ELF after publish setup failure: path=%s errno=%d message=%s",
                linkedKernelElfPath.c_str(), closeError, std::strerror(closeError));
        }
        return ACLRTC_ERROR_FAILURE;
    }

    const bool copySucceeded = CopyFileDescriptorContents(
        linkedKernelFileDescriptor, linkedKernelElfPath, temporaryOutputFileDescriptor, temporaryOutputPath);
    const bool sourceCloseSucceeded = close(linkedKernelFileDescriptor) == 0;
    const int sourceCloseError = sourceCloseSucceeded ? 0 : errno;
    const bool destinationCloseSucceeded = close(temporaryOutputFileDescriptor) == 0;
    const int destinationCloseError = destinationCloseSucceeded ? 0 : errno;
    if (!copySucceeded || !sourceCloseSucceeded || !destinationCloseSucceeded) {
        if (!sourceCloseSucceeded) {
            ACLRTC_LOG_ERROR("Failed to close linked kernel ELF: path=%s errno=%d message=%s",
                             linkedKernelElfPath.c_str(), sourceCloseError, std::strerror(sourceCloseError));
        }
        if (!destinationCloseSucceeded) {
            ACLRTC_LOG_ERROR("Failed to close temporary output ELF: path=%s errno=%d message=%s",
                             temporaryOutputPath.c_str(), destinationCloseError,
                             std::strerror(destinationCloseError));
        }
        RemoveTemporaryOutputFileBestEffort(temporaryOutputPath);
        return ACLRTC_ERROR_FAILURE;
    }

    const aclError worktreeRemovalResult = compilationWorkspace.RemoveWorktreeBeforePublishingElfIfNeeded();
    if (worktreeRemovalResult != ACLRTC_SUCCESS) {
        RemoveTemporaryOutputFileBestEffort(temporaryOutputPath);
        return worktreeRemovalResult;
    }

    boost::system::error_code renameError;
    fs::rename(temporaryOutputPath, outputElfPath, renameError);
    if (renameError) {
        ACLRTC_LOG_ERROR("Failed to publish kernel ELF: source=%s target=%s error=%d message=%s",
                         temporaryOutputPath.c_str(), outputElfPath.c_str(), renameError.value(),
                         renameError.message().c_str());
        RemoveTemporaryOutputFileBestEffort(temporaryOutputPath);
        return ACLRTC_ERROR_FAILURE;
    }
    return ACLRTC_SUCCESS;
}

} // namespace aclrtc
} // namespace ascendc

extern "C" aclError aclrtcKernelSpecialization(const aclrtcKernelSpecRequest* request, const char* outPath)
{
    try {
        ascendc::aclrtc::KernelSpecializationSession specializationSession;
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
