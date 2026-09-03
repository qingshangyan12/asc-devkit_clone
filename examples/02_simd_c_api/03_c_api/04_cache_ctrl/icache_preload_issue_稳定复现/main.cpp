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
 * \file main.cpp
 * \brief Pure ACL host program for the ICache preload issue reproducer.
 */

#include <cstdint>
#include <cstring>
#include <dlfcn.h>
#include <iostream>

#include "acl/acl.h"

namespace {
constexpr int32_t DEVICE_ID = 0;
constexpr int64_t ELEMENT_COUNT = 4;
constexpr int64_t PROBE_ELEMENT_COUNT = 128;

using RunIcacheProbe = void (*)(float*, int64_t, aclrtStream);
using RunUnrelatedProbe = void (*)(uint8_t*, int64_t, aclrtStream);

bool CheckAcl(aclError status, const char* operation)
{
    if (status == ACL_SUCCESS) {
        return true;
    }
    std::cerr << operation << " failed, acl error: " << status << std::endl;
    return false;
}

void* OpenLibrary(const char* path)
{
    void* handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (handle == nullptr) {
        std::cerr << "dlopen(" << path << ") failed: " << dlerror() << std::endl;
    }
    return handle;
}

void* LoadSymbol(void* handle, const char* name)
{
    (void)dlerror();
    void* symbol = dlsym(handle, name);
    const char* error = dlerror();
    if (error != nullptr) {
        std::cerr << "dlsym(" << name << ") failed: " << error << std::endl;
        return nullptr;
    }
    return symbol;
}

void Cleanup(aclrtStream stream, float* xDevice, uint8_t* probeDevice, void* firstLibrary, void* secondLibrary)
{
    if (probeDevice != nullptr) {
        (void)aclrtFree(probeDevice);
    }
    if (xDevice != nullptr) {
        (void)aclrtFree(xDevice);
    }
    if (secondLibrary != nullptr) {
        (void)dlclose(secondLibrary);
    }
    if (firstLibrary != nullptr) {
        (void)dlclose(firstLibrary);
    }
    if (stream != nullptr) {
        (void)aclrtDestroyStream(stream);
    }
    (void)aclrtResetDevice(DEVICE_ID);
    (void)aclFinalize();
}
} // namespace

int32_t main(int32_t argc, char* argv[])
{
    if (argc != 2 || (std::strcmp(argv[1], "--preload") != 0 && std::strcmp(argv[1], "--no-preload") != 0)) {
        std::cerr << "Usage: " << argv[0] << " --preload|--no-preload" << std::endl;
        return 2;
    }
    const bool usePreload = std::strcmp(argv[1], "--preload") == 0;

    aclrtStream stream = nullptr;
    float* xDevice = nullptr;
    uint8_t* probeDevice = nullptr;
    void* firstLibrary = nullptr;
    void* secondLibrary = nullptr;
    float xHost[ELEMENT_COUNT] = {0.0F, 0.0F, 0.0F, 0.0F};
    uint8_t probeHost[PROBE_ELEMENT_COUNT] = {};
    const size_t xBytes = sizeof(xHost);
    const size_t probeBytes = sizeof(probeHost);

    if (!CheckAcl(aclInit(nullptr), "aclInit") || !CheckAcl(aclrtSetDevice(DEVICE_ID), "aclrtSetDevice") ||
        !CheckAcl(aclrtCreateStream(&stream), "aclrtCreateStream") ||
        !CheckAcl(aclrtMalloc(reinterpret_cast<void**>(&xDevice), xBytes, ACL_MEM_MALLOC_HUGE_FIRST), "aclrtMalloc(x)") ||
        !CheckAcl(aclrtMalloc(reinterpret_cast<void**>(&probeDevice), probeBytes, ACL_MEM_MALLOC_HUGE_FIRST),
            "aclrtMalloc(probe)") ||
        !CheckAcl(aclrtMemcpy(xDevice, xBytes, xHost, xBytes, ACL_MEMCPY_HOST_TO_DEVICE), "aclrtMemcpy(x H2D)") ||
        !CheckAcl(aclrtMemcpy(probeDevice, probeBytes, probeHost, probeBytes, ACL_MEMCPY_HOST_TO_DEVICE),
            "aclrtMemcpy(probe H2D)")) {
        Cleanup(stream, xDevice, probeDevice, firstLibrary, secondLibrary);
        return 1;
    }

    firstLibrary = OpenLibrary("./libicache_probe_kernel.so");
    const char* firstSymbol =
        usePreload ? "run_icache_probe_with_preload" : "run_icache_probe_without_preload";
    auto runFirst = firstLibrary == nullptr
        ? nullptr
        : reinterpret_cast<RunIcacheProbe>(LoadSymbol(firstLibrary, firstSymbol));
    if (runFirst == nullptr) {
        Cleanup(stream, xDevice, probeDevice, firstLibrary, secondLibrary);
        return 1;
    }

    runFirst(xDevice, ELEMENT_COUNT, stream);
    aclError firstSyncStatus = aclrtSynchronizeStream(stream);
    if (firstSyncStatus != ACL_SUCCESS) {
        std::cout << "FIRST_SYNC_FAIL acl_error=" << firstSyncStatus << std::endl;
        Cleanup(stream, xDevice, probeDevice, firstLibrary, secondLibrary);
        return 1;
    }
    std::cout << "FIRST_SYNC_OK" << std::endl;

    secondLibrary = OpenLibrary("./libunrelated_probe_kernel.so");
    auto runSecond = secondLibrary == nullptr
        ? nullptr
        : reinterpret_cast<RunUnrelatedProbe>(LoadSymbol(secondLibrary, "run_unrelated_probe"));
    if (runSecond == nullptr) {
        Cleanup(stream, xDevice, probeDevice, firstLibrary, secondLibrary);
        return 1;
    }

    runSecond(probeDevice, PROBE_ELEMENT_COUNT, stream);
    aclError secondSyncStatus = aclrtSynchronizeStream(stream);
    if (secondSyncStatus != ACL_SUCCESS) {
        std::cout << "PROBE_FAIL acl_error=" << secondSyncStatus << std::endl;
        Cleanup(stream, xDevice, probeDevice, firstLibrary, secondLibrary);
        return 1;
    }

    bool copySucceeded = CheckAcl(
        aclrtMemcpy(xHost, xBytes, xDevice, xBytes, ACL_MEMCPY_DEVICE_TO_HOST), "aclrtMemcpy(x D2H)") &&
        CheckAcl(aclrtMemcpy(probeHost, probeBytes, probeDevice, probeBytes, ACL_MEMCPY_DEVICE_TO_HOST),
            "aclrtMemcpy(probe D2H)");
    bool outputCorrect = copySucceeded;
    for (float value : xHost) {
        outputCorrect = outputCorrect && value == 1.0F;
    }
    for (uint8_t value : probeHost) {
        outputCorrect = outputCorrect && value == 1;
    }

    std::cout << (outputCorrect ? "PROBE_OK" : "OUTPUT_MISMATCH") << std::endl;
    Cleanup(stream, xDevice, probeDevice, firstLibrary, secondLibrary);
    return outputCorrect ? 0 : 1;
}
