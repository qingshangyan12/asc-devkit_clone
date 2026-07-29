/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file l2_cache_runner.h
 * \brief Host runtime shared by the negative and positive examples.
 */

#ifndef ADD_L2_CACHE_SPLIT_L2_CACHE_RUNNER_H
#define ADD_L2_CACHE_SPLIT_L2_CACHE_RUNNER_H

#include "acl/acl.h"
#include "data_utils.h"
#include "l2_cache_kernel.h"
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>

namespace AddL2Cache {

using KernelLauncher = void (*)(aclrtStream, uint8_t*, uint8_t*);

inline bool RunKernel(
    const char* caseName, const char* outputPath, KernelLauncher launchKernel, aclrtStream stream)
{
    constexpr size_t dataSizeBytes = static_cast<size_t>(totalElementCount) * sizeof(uint16_t);
    constexpr size_t syncSizeBytes =
        static_cast<size_t>(vectorCoreCount) * syncInt32ElementsPerCore * sizeof(int32_t);

    uint8_t* inputHost = nullptr;
    uint8_t* outputHost = nullptr;
    uint8_t* dataDevice = nullptr;
    uint8_t* syncDevice = nullptr;

    aclrtMallocHost(reinterpret_cast<void**>(&inputHost), dataSizeBytes);
    aclrtMallocHost(reinterpret_cast<void**>(&outputHost), dataSizeBytes);
    aclrtMalloc(reinterpret_cast<void**>(&dataDevice), dataSizeBytes, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&syncDevice), syncSizeBytes, ACL_MEM_MALLOC_NORMAL_ONLY);

    size_t fileSize = 0;
    bool success = ReadFile("./input/input.bin", fileSize, inputHost, dataSizeBytes) && fileSize == dataSizeBytes;
    if (!success) {
        std::fprintf(
            stderr, "[ERROR] invalid input file (expected %zu bytes, got %zu)\n", dataSizeBytes, fileSize);
    }
    if (success && mkdir("./output", 0755) != 0 && errno != EEXIST) {
        std::fprintf(stderr, "[ERROR] failed to create output directory: %s\n", std::strerror(errno));
        success = false;
    }
    if (success) {
        aclrtMemcpy(dataDevice, dataSizeBytes, inputHost, dataSizeBytes, ACL_MEMCPY_HOST_TO_DEVICE);
        aclrtMemset(syncDevice, syncSizeBytes, 0, syncSizeBytes);
        launchKernel(stream, dataDevice, syncDevice);
        aclrtSynchronizeStream(stream);
        aclrtMemcpy(outputHost, dataSizeBytes, dataDevice, dataSizeBytes, ACL_MEMCPY_DEVICE_TO_HOST);
        success = WriteFile(outputPath, outputHost, dataSizeBytes);
    }
    if (success) {
        std::printf("%s output: %s\n", caseName, outputPath);
    }

    aclrtFree(syncDevice);
    aclrtFree(dataDevice);
    aclrtFreeHost(outputHost);
    aclrtFreeHost(inputHost);
    return success;
}

inline int32_t RunSample(
    int32_t argc, char* argv[], const char* caseName, const char* outputPath, KernelLauncher launchKernel,
    bool enableL2Split)
{
    constexpr uint32_t bytesPerMiB = 1024 * 1024;
    if (argc != 1) {
        std::fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }
    const uint32_t activeWorkingSetBytes = enableL2Split ? l2BatchSizeBytes : totalByteSize;
    const uint32_t activeBatchCount = enableL2Split ? l2BatchCount : 1U;
    std::printf(
        "case=%s, total_data_elements=%u, total_data=%u MB, "
        "l2_split=%s, l2_working_set_threshold=%u MB, active_working_set=%u MB, "
        "logical_gm_to_ub=%u MB, vector_cores=%u, l2_batches=%u, compute_rounds=%u\n",
        caseName, totalElementCount, totalByteSize / bytesPerMiB,
        enableL2Split ? "enabled" : "disabled", L2_TILE_THRESHOLD / bytesPerMiB,
        activeWorkingSetBytes / bytesPerMiB, logicalGmToUbBytesPerKernel / bytesPerMiB,
        vectorCoreCount, activeBatchCount, computeRoundCount);

    aclInit(nullptr);
    constexpr int32_t deviceId = 0;
    aclrtSetDevice(deviceId);
    aclrtStream stream = nullptr;
    aclrtCreateStream(&stream);

    bool success = RunKernel(caseName, outputPath, launchKernel, stream);
    aclrtDestroyStream(stream);
    aclrtResetDevice(deviceId);
    aclFinalize();
    return success ? 0 : 1;
}

} // namespace AddL2Cache

#endif
