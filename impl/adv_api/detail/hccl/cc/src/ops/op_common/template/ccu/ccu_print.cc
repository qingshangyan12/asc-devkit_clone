/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_print.h"

#include <acl/acl_rt.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <thread>
#include <utility>

#include "log.h"

namespace AscendC {
namespace ccu {
namespace dfx {

namespace {

struct PrintInfoKeyLess {
    bool operator()(const PrintInfoKey& lhs, const PrintInfoKey& rhs) const
    {
        int result = std::strcmp(lhs.filename, rhs.filename);
        if (result != 0) {
            return result < 0;
        }
        if (lhs.lineno != rhs.lineno) {
            return lhs.lineno < rhs.lineno;
        }
        return std::strcmp(lhs.fmtstr, rhs.fmtstr) < 0;
    }
};

bool AppendUnsigned(std::string& output, uint64_t value, uint32_t base)
{
    static const char digits[] = "0123456789abcdef";
    char buffer[sizeof(uint64_t) * 8];
    char* current = buffer + sizeof(buffer);
    do {
        *--current = digits[value % base];
        value /= base;
    } while (value != 0);

    try {
        output.append(current, buffer + sizeof(buffer));
    } catch (...) {
        return false;
    }
    return true;
}

bool CheckAcl(aclError result, const char* operation)
{
    if (result == ACL_SUCCESS) {
        return true;
    }
    HCCL_RUN_WARNING("[CCU_PRINT] %s failed, aclError[%d]", operation, result);
    return false;
}

} // namespace

namespace detail {

bool ValidateFormat(const char* fmtstr, size_t argc)
{
    if (fmtstr == nullptr) {
        return false;
    }

    size_t argumentCount = 0;
    for (const char* current = fmtstr; *current != '\0'; ++current) {
        if (*current != '%') {
            continue;
        }

        ++current;
        if (*current == '\0') {
            return false;
        }
        if (*current == '%') {
            continue;
        }
        if (*current != 'u' && *current != 'x' && *current != 'p' && *current != 'v') {
            return false;
        }
        ++argumentCount;
    }
    return argumentCount == argc;
}

bool Format(const char* fmtstr, const uint64_t* argv, size_t argc, std::string& output)
{
    if (!ValidateFormat(fmtstr, argc) || (argc != 0 && argv == nullptr)) {
        return false;
    }

    size_t argumentIndex = 0;
    try {
        for (const char* current = fmtstr; *current != '\0'; ++current) {
            if (*current != '%') {
                output.push_back(*current);
                continue;
            }

            const char conversion = *++current;
            if (conversion == '%') {
                output.push_back('%');
                continue;
            }

            const uint64_t value = argv[argumentIndex++];
            switch (conversion) {
                case 'u':
                case 'v':
                    if (!AppendUnsigned(output, value, 10)) {
                        return false;
                    }
                    break;
                case 'x':
                    if (!AppendUnsigned(output, value, 16)) {
                        return false;
                    }
                    break;
                case 'p':
                    output.append("0x");
                    if (!AppendUnsigned(output, value, 16)) {
                        return false;
                    }
                    break;
                default:
                    return false;
            }
        }
    } catch (...) {
        return false;
    }
    return argumentIndex == argc;
}

} // namespace detail

struct CcuPrintStream::Impl {
    std::map<PrintInfoKey, PrintInfoHandle*, PrintInfoKeyLess> fmtMap;
    std::mutex mapMutex;
    std::atomic<bool> running{false};
    std::thread consumer;
    std::mutex waitMutex;
    std::condition_variable wakeup;
    aclrtContext context{nullptr};

    bool StartLocked()
    {
        if (running.load()) {
            return true;
        }
        if (!CheckAcl(aclrtGetCurrentContext(&context), "aclrtGetCurrentContext")) {
            return false;
        }
        if (consumer.joinable()) {
            consumer.join();
        }

        running.store(true);
        try {
            consumer = std::thread([this] { ConsumeLoop(); });
        } catch (...) {
            running.store(false);
            HCCL_RUN_WARNING("[CCU_PRINT] failed to create consumer thread");
            return false;
        }
        return true;
    }

    void Stop()
    {
        running.store(false);
        wakeup.notify_all();
        if (consumer.joinable()) {
            consumer.join();
        }
    }

    void ConsumeLoop()
    {
        if (!CheckAcl(aclrtSetCurrentContext(context), "aclrtSetCurrentContext")) {
            running.store(false);
            return;
        }

        while (running.load()) {
            ConsumeOnce();
            std::unique_lock<std::mutex> lock(waitMutex);
            wakeup.wait_for(lock, std::chrono::milliseconds(20), [this] { return !running.load(); });
        }
        ConsumeOnce();
    }

    void ConsumeOnce()
    {
        std::lock_guard<std::mutex> lock(mapMutex);
        for (const auto& entry : fmtMap) {
            ConsumeOne(entry.first, *entry.second);
        }
    }

    void ConsumeOne(const PrintInfoKey& key, const PrintInfoHandle& handle)
    {
        std::unique_ptr<uint8_t[]> snapshot(new (std::nothrow) uint8_t[handle.allocationBytes]);
        if (snapshot == nullptr) {
            HCCL_RUN_WARNING("[CCU_PRINT] failed to allocate Host snapshot for %s:%d", key.filename, key.lineno);
            return;
        }
        if (!CheckAcl(
                aclrtMemcpy(
                    snapshot.get(), handle.allocationBytes, handle.value, handle.allocationBytes,
                    ACL_MEMCPY_DEVICE_TO_HOST),
                "aclrtMemcpy device-to-host")) {
            return;
        }

        PrintInfoValue* value = reinterpret_cast<PrintInfoValue*>(snapshot.get());
        uint64_t head = value->head;
        const uint64_t tail = value->tail;
        if (head >= handle.ringBytes || tail >= handle.ringBytes || head % handle.recordBytes != 0 ||
            tail % handle.recordBytes != 0) {
            HCCL_RUN_WARNING(
                "[CCU_PRINT] invalid ring cursor at %s:%d, head[%llu], tail[%llu]", key.filename, key.lineno, head,
                tail);
            return;
        }

        while (head != tail) {
            const uint64_t* argv =
                reinterpret_cast<const uint64_t*>(reinterpret_cast<const uint8_t*>(value->argvArray) + head);
            std::string output;
            if (detail::Format(key.fmtstr, argv, handle.argc, output)) {
                HCCL_RUN_INFO("[CCU_PRINT][%s:%d] %s", key.filename, key.lineno, output.c_str());
            } else {
                HCCL_RUN_WARNING("[CCU_PRINT] failed to format %s:%d", key.filename, key.lineno);
            }

            head += handle.recordBytes;
            if (head == handle.ringBytes) {
                head = 0;
            }
        }

        if (head == value->head) {
            return;
        }
        (void)CheckAcl(
            aclrtMemcpy(
                reinterpret_cast<uint8_t*>(handle.value) + offsetof(PrintInfoValue, head), sizeof(uint64_t), &head,
                sizeof(uint64_t), ACL_MEMCPY_HOST_TO_DEVICE),
            "aclrtMemcpy print head host-to-device");
    }
};

CcuPrintStream* CcuPrintStream::GetInstance()
{
    static CcuPrintStream instance;
    return &instance;
}

CcuPrintStream::CcuPrintStream() : impl_(new(std::nothrow) Impl) {}

CcuPrintStream::~CcuPrintStream()
{
    if (impl_ == nullptr) {
        return;
    }

    impl_->Stop();
    for (auto& entry : impl_->fmtMap) {
        (void)aclrtFree(entry.second->valueAddressCell);
        (void)aclrtFree(entry.second->value);
        delete entry.second;
    }
}

PrintInfoHandle* CcuPrintStream::AllocPrintInfo(const char* filename, int lineno, const char* fmtstr, size_t argc)
{
    if (impl_ == nullptr || filename == nullptr || !detail::ValidateFormat(fmtstr, argc)) {
        HCCL_RUN_WARNING("[CCU_PRINT] invalid format at %s:%d", filename == nullptr ? "<null>" : filename, lineno);
        return nullptr;
    }

    const size_t minArgc = argc == 0 ? 1 : argc;
    if (minArgc >
        (std::numeric_limits<size_t>::max() - sizeof(PrintInfoValue)) / sizeof(uint64_t) / CCU_PRINT_RING_DEPTH) {
        HCCL_RUN_WARNING("[CCU_PRINT] allocation size overflow at %s:%d", filename, lineno);
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(impl_->mapMutex);
    PrintInfoKey key{filename, lineno, fmtstr};
    auto found = impl_->fmtMap.find(key);
    if (found != impl_->fmtMap.end()) {
        if (!impl_->StartLocked()) {
            return nullptr;
        }
        return found->second;
    }

    std::unique_ptr<PrintInfoHandle> handle(new (std::nothrow) PrintInfoHandle{});
    if (handle == nullptr) {
        HCCL_RUN_WARNING("[CCU_PRINT] failed to allocate handle for %s:%d", filename, lineno);
        return nullptr;
    }
    handle->argc = argc;
    handle->recordBytes = sizeof(uint64_t) * minArgc;
    handle->ringBytes = handle->recordBytes * CCU_PRINT_RING_DEPTH;
    handle->allocationBytes = sizeof(PrintInfoValue) + handle->ringBytes;

    if (!CheckAcl(
            aclrtMalloc(reinterpret_cast<void**>(&handle->value), handle->allocationBytes, ACL_MEM_MALLOC_HUGE_FIRST),
            "aclrtMalloc PrintInfoValue")) {
        return nullptr;
    }
    if (!CheckAcl(
            aclrtMalloc(
                reinterpret_cast<void**>(&handle->valueAddressCell), sizeof(uint64_t), ACL_MEM_MALLOC_HUGE_FIRST),
            "aclrtMalloc PrintInfoValue address cell")) {
        (void)aclrtFree(handle->value);
        return nullptr;
    }
    if (!CheckAcl(
            aclrtMemset(handle->value, handle->allocationBytes, 0, handle->allocationBytes),
            "aclrtMemset PrintInfoValue")) {
        (void)aclrtFree(handle->valueAddressCell);
        (void)aclrtFree(handle->value);
        return nullptr;
    }

    const uint64_t valueAddress = reinterpret_cast<uint64_t>(handle->value);
    if (!CheckAcl(
            aclrtMemcpy(
                handle->valueAddressCell, sizeof(uint64_t), &valueAddress, sizeof(uint64_t), ACL_MEMCPY_HOST_TO_DEVICE),
            "aclrtMemcpy PrintInfoValue address")) {
        (void)aclrtFree(handle->valueAddressCell);
        (void)aclrtFree(handle->value);
        return nullptr;
    }

    PrintInfoHandle* result = handle.release();
    try {
        impl_->fmtMap.emplace(key, result);
    } catch (...) {
        (void)aclrtFree(result->valueAddressCell);
        (void)aclrtFree(result->value);
        delete result;
        HCCL_RUN_WARNING("[CCU_PRINT] failed to register %s:%d", filename, lineno);
        return nullptr;
    }

    if (!impl_->StartLocked()) {
        return nullptr;
    }
    return result;
}

void CcuPrintStream::Stop()
{
    if (impl_ != nullptr) {
        impl_->Stop();
    }
}

} // namespace dfx
} // namespace ccu
} // namespace AscendC
