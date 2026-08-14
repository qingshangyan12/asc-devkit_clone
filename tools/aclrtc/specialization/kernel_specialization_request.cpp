/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "kernel_specialization_types.h"

#include "kernel_specialization_log.h"

#include <algorithm>
#include <atomic>
#include <boost/filesystem/path.hpp>
#include <cctype>
#include <cstring>
#include <unistd.h>

namespace ascendc {
namespace aclrtc {
namespace {
constexpr size_t MAX_REQUEST_STRING_BYTES = 1024U * 1024U;
constexpr size_t MAX_USER_OPTION_COUNT = 4096U;
constexpr size_t MAX_REQUEST_TEXT_BYTES = 4U * 1024U * 1024U;
constexpr char SUPER_KERNEL_ENABLE_OPTION[] = "--enable-super-kernel";

std::atomic<uint64_t> gRequestSequence {0};

bool ReadBoundedString(const char* input, std::string& output, size_t& totalTextBytes)
{
    if (input == nullptr) {
        return false;
    }
    const size_t inputLength = strnlen(input, MAX_REQUEST_STRING_BYTES + 1U);
    if (inputLength == 0U || inputLength > MAX_REQUEST_STRING_BYTES ||
        inputLength > MAX_REQUEST_TEXT_BYTES - totalTextBytes) {
        return false;
    }
    output.assign(input, inputLength);
    totalTextBytes += inputLength;
    return true;
}

bool IsLowercaseSha256(const std::string& resourceId)
{
    return resourceId.size() == 64U && std::all_of(resourceId.begin(), resourceId.end(), [](char character) {
        return (character >= '0' && character <= '9') || (character >= 'a' && character <= 'f');
    });
}

bool ContainsAsciiControlCharacter(const std::string& option)
{
    return std::any_of(option.begin(), option.end(), [](unsigned char character) {
        return character < 0x20U || character == 0x7fU;
    });
}

bool RewritesCompilerOutput(const std::string& option)
{
    return option == "-o" || option == "--output" ||
           (option.size() > 2U && option.compare(0, 2, "-o") == 0) ||
           option.compare(0, sizeof("--output=") - 1U, "--output=") == 0;
}

bool IsStructurallySafeCompilerOption(const std::string& option)
{
    return option.front() != '@' && !ContainsAsciiControlCharacter(option) && !RewritesCompilerOutput(option);
}

aclError CopyUserOptions(const char* const* apiOptions, uint64_t apiOptionCount,
                         bool validateStructuralCompilerBoundaries,
                         std::vector<std::string>& copiedOptions, size_t& totalTextBytes)
{
    if (apiOptionCount == 0U) {
        return ACLRTC_SUCCESS;
    }
    if (apiOptions == nullptr) {
        return ACLRTC_ERROR_INVALID_INPUT;
    }

    copiedOptions.reserve(static_cast<size_t>(apiOptionCount));
    for (uint64_t optionIndex = 0; optionIndex < apiOptionCount; ++optionIndex) {
        std::string copiedOption;
        if (!ReadBoundedString(apiOptions[optionIndex], copiedOption, totalTextBytes)) {
            return ACLRTC_ERROR_INVALID_OPTION;
        }
        if (ContainsAsciiControlCharacter(copiedOption) ||
            (validateStructuralCompilerBoundaries && !IsStructurallySafeCompilerOption(copiedOption))) {
            return ACLRTC_ERROR_INVALID_OPTION;
        }
        copiedOptions.emplace_back(std::move(copiedOption));
    }
    return ACLRTC_SUCCESS;
}

std::string CreateRequestId()
{
    return std::to_string(getpid()) + "_" + std::to_string(gRequestSequence.fetch_add(1U, std::memory_order_relaxed));
}
} // namespace

aclError NormalizeKernelSpecializationRequest(const aclrtcKernelSpecRequest& apiRequest, const char* outputElfPath,
                                              NormalizedKernelSpecializationRequest& normalizedRequest)
{
    if (std::any_of(std::begin(apiRequest.reserved), std::end(apiRequest.reserved),
                    [](uint8_t value) { return value != 0U; })) {
        return ACLRTC_ERROR_INVALID_INPUT;
    }

    size_t totalTextBytes = 0U;
    NormalizedKernelSpecializationRequest request;
    if (!ReadBoundedString(apiRequest.resourceId, request.resourceId, totalTextBytes) ||
        !IsLowercaseSha256(request.resourceId) ||
        !ReadBoundedString(apiRequest.kernelEntry, request.kernelName, totalTextBytes)) {
        return ACLRTC_ERROR_INVALID_INPUT;
    }

    std::string copiedOutputElfPath;
    if (!ReadBoundedString(outputElfPath, copiedOutputElfPath, totalTextBytes) ||
        !boost::filesystem::path(copiedOutputElfPath).is_absolute()) {
        return ACLRTC_ERROR_INVALID_INPUT;
    }
    request.outputElfPath = std::move(copiedOutputElfPath);

    if (apiRequest.optionCount > MAX_USER_OPTION_COUNT ||
        apiRequest.skOptionCount > MAX_USER_OPTION_COUNT - apiRequest.optionCount) {
        return ACLRTC_ERROR_INVALID_INPUT;
    }
    aclError result = CopyUserOptions(apiRequest.options, apiRequest.optionCount, true,
                                      request.compilerOptions.basicOptions, totalTextBytes);
    if (result != ACLRTC_SUCCESS) {
        return result;
    }

    std::vector<std::string> copiedSuperKernelOptions;
    result = CopyUserOptions(apiRequest.skOptions, apiRequest.skOptionCount, false,
                             copiedSuperKernelOptions, totalTextBytes);
    if (result != ACLRTC_SUCCESS) {
        return result;
    }

    const size_t enableOptionCount = static_cast<size_t>(std::count(
        copiedSuperKernelOptions.begin(), copiedSuperKernelOptions.end(), SUPER_KERNEL_ENABLE_OPTION));
    if (enableOptionCount > 1U) {
        return ACLRTC_ERROR_INVALID_OPTION;
    }
    if (enableOptionCount == 1U) {
        request.compilationVariant = CompilationVariant::BasicWithSuperKernel;
        for (std::string& superKernelOption : copiedSuperKernelOptions) {
            if (superKernelOption == SUPER_KERNEL_ENABLE_OPTION) {
                continue;
            }
            if (!IsStructurallySafeCompilerOption(superKernelOption)) {
                return ACLRTC_ERROR_INVALID_OPTION;
            }
            request.compilerOptions.superKernelOptions.emplace_back(std::move(superKernelOption));
        }
    } else if (!copiedSuperKernelOptions.empty()) {
        ACLRTC_LOG_WARNING(
            "skOptions were provided without --enable-super-kernel; ignoring skOptions and compiling basic only");
    }

    request.requestId = CreateRequestId();
    request.kernelArgumentCount = apiRequest.argsCount;
    request.borrowedKernelArgumentAddresses = apiRequest.argsAddr;
    request.borrowedKernelArgumentByteCounts = apiRequest.argsBytes;
    normalizedRequest = std::move(request);
    return ACLRTC_SUCCESS;
}

} // namespace aclrtc
} // namespace ascendc
