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

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <pthread.h>
#include <utility>
#include <vector>

#include "log.h"

namespace AscendC {
namespace ccu {
namespace dfx {

namespace {

constexpr std::chrono::microseconds CCU_PRINT_IDLE_POLL_INTERVAL(1000);
constexpr std::chrono::microseconds CCU_PRINT_ACTIVE_POLL_INTERVAL(100);
constexpr uint32_t CCU_PRINT_POST_UPDATE_DRAIN_ROUNDS = 4;

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
    return argumentIndex == argc;
}

} // namespace detail

struct CcuPrintStream::Impl {
    struct PendingRecord {
        PrintInfoKey key;
        uint64_t sequence;
        std::vector<uint64_t> argv;
    };

    std::map<PrintInfoKey, PrintInfoHandle*, PrintInfoKeyLess> fmtMap;
    std::mutex mapMutex;
    std::atomic<bool> running{false};
    pthread_t consumer{};
    bool consumerStarted{false};
    std::mutex waitMutex;
    std::condition_variable wakeup;
    aclrtContext context{nullptr};
    uint64_t* sequence{nullptr};
    uint64_t* sequenceAddressCell{nullptr};

    bool InitSequenceLocked()
    {
        if (sequence != nullptr) {
            return true;
        }
        if (!CheckAcl(
                aclrtMalloc(reinterpret_cast<void**>(&sequence), sizeof(uint64_t), ACL_MEM_MALLOC_HUGE_FIRST),
                "aclrtMalloc print sequence")) {
            return false;
        }
        if (!CheckAcl(
                aclrtMalloc(
                    reinterpret_cast<void**>(&sequenceAddressCell), sizeof(uint64_t), ACL_MEM_MALLOC_HUGE_FIRST),
                "aclrtMalloc print sequence address cell")) {
            (void)aclrtFree(sequence);
            sequence = nullptr;
            return false;
        }
        if (!CheckAcl(aclrtMemset(sequence, sizeof(uint64_t), 0, sizeof(uint64_t)), "aclrtMemset print sequence")) {
            (void)aclrtFree(sequenceAddressCell);
            (void)aclrtFree(sequence);
            sequenceAddressCell = nullptr;
            sequence = nullptr;
            return false;
        }

        const uint64_t sequenceAddress = reinterpret_cast<uint64_t>(sequence);
        if (!CheckAcl(
                aclrtMemcpy(
                    sequenceAddressCell, sizeof(uint64_t), &sequenceAddress, sizeof(uint64_t),
                    ACL_MEMCPY_HOST_TO_DEVICE),
                "aclrtMemcpy print sequence address")) {
            (void)aclrtFree(sequenceAddressCell);
            (void)aclrtFree(sequence);
            sequenceAddressCell = nullptr;
            sequence = nullptr;
            return false;
        }
        return true;
    }

    bool JoinConsumer()
    {
        if (!consumerStarted) {
            return true;
        }
        const int ret = pthread_join(consumer, nullptr);
        if (ret != 0) {
            HCCL_RUN_WARNING("[CCU_PRINT] pthread_join failed, ret[%d]", ret);
            return false;
        }
        consumerStarted = false;
        return true;
    }

    bool StartLocked()
    {
        if (running.load()) {
            return true;
        }
        if (!CheckAcl(aclrtGetCurrentContext(&context), "aclrtGetCurrentContext")) {
            return false;
        }
        if (!JoinConsumer()) {
            return false;
        }

        running.store(true);
        // pthread_create reports failure directly; no exception is needed.
        const int ret = pthread_create(
            &consumer, nullptr,
            [](void* arg) -> void* {
                static_cast<Impl*>(arg)->ConsumeLoop();
                return nullptr;
            },
            this);
        if (ret != 0) {
            running.store(false);
            HCCL_RUN_WARNING("[CCU_PRINT] pthread_create failed, ret[%d]", ret);
            return false;
        }
        consumerStarted = true;
        return true;
    }

    void Stop()
    {
        running.store(false);
        wakeup.notify_all();
        (void)JoinConsumer();
    }

    void ConsumeLoop()
    {
        if (!CheckAcl(aclrtSetCurrentContext(context), "aclrtSetCurrentContext")) {
            running.store(false);
            return;
        }

        uint64_t observedSequence = 0;
        uint32_t postUpdateDrainRounds = 0;
        while (running.load()) {
            uint64_t currentSequence = observedSequence;
            const bool sequenceRead = CheckAcl(
                aclrtMemcpy(&currentSequence, sizeof(uint64_t), sequence, sizeof(uint64_t), ACL_MEMCPY_DEVICE_TO_HOST),
                "aclrtMemcpy print sequence device-to-host");
            if (sequenceRead && currentSequence != observedSequence) {
                if (currentSequence > observedSequence + 1) {
                    HCCL_RUN_WARNING(
                        "[CCU_PRINT] sequence advanced by %llu between scans; increase ring depth if records are "
                        "missing",
                        currentSequence - observedSequence);
                } else if (currentSequence < observedSequence) {
                    // The shared sequence is deliberately non-atomic. A late
                    // store from another mission can temporarily move it back.
                    HCCL_RUN_WARNING(
                        "[CCU_PRINT] non-atomic sequence moved backward from %llu to %llu", observedSequence,
                        currentSequence);
                }
                observedSequence = currentSequence;
                ConsumeOnce();
                postUpdateDrainRounds = CCU_PRINT_POST_UPDATE_DRAIN_ROUNDS;
            } else if (postUpdateDrainRounds != 0) {
                // CCU updates sequence before publishing the per-call-site
                // tail. Rescan briefly so the notification cannot race the
                // record publication.
                ConsumeOnce();
                --postUpdateDrainRounds;
            }

            const std::chrono::microseconds interval =
                postUpdateDrainRounds == 0 ? CCU_PRINT_IDLE_POLL_INTERVAL : CCU_PRINT_ACTIVE_POLL_INTERVAL;
            std::unique_lock<std::mutex> lock(waitMutex);
            wakeup.wait_for(lock, interval, [this] { return !running.load(); });
        }
        ConsumeOnce();
    }

    void ConsumeOnce()
    {
        std::lock_guard<std::mutex> lock(mapMutex);
        std::vector<PendingRecord> records;
        for (const auto& entry : fmtMap) {
            ConsumeOne(entry.first, *entry.second, records);
        }
        std::stable_sort(records.begin(), records.end(), [](const PendingRecord& lhs, const PendingRecord& rhs) {
            return lhs.sequence < rhs.sequence;
        });
        for (const PendingRecord& record : records) {
            std::string output;
            if (detail::Format(record.key.fmtstr, record.argv.data(), record.argv.size(), output)) {
                HCCL_RUN_INFO(
                    "[CCU_PRINT][%s:%d][seq=%llu] %s", record.key.filename, record.key.lineno, record.sequence,
                    output.c_str());
            } else {
                HCCL_RUN_WARNING("[CCU_PRINT] failed to format %s:%d", record.key.filename, record.key.lineno);
            }
        }
    }

    void ConsumeOne(const PrintInfoKey& key, const PrintInfoHandle& handle, std::vector<PendingRecord>& records)
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
            const uint64_t* record =
                reinterpret_cast<const uint64_t*>(reinterpret_cast<const uint8_t*>(value->argvArray) + head);
            try {
                records.push_back(
                    PendingRecord{key, record[0], std::vector<uint64_t>(record + 1, record + 1 + handle.argc)});
            } catch (...) {
                HCCL_RUN_WARNING("[CCU_PRINT] failed to collect record at %s:%d", key.filename, key.lineno);
                break;
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
    (void)aclrtFree(impl_->sequenceAddressCell);
    (void)aclrtFree(impl_->sequence);
}

PrintInfoHandle* CcuPrintStream::AllocPrintInfo(const char* filename, int lineno, const char* fmtstr, size_t argc)
{
    if (impl_ == nullptr || filename == nullptr || !detail::ValidateFormat(fmtstr, argc)) {
        HCCL_RUN_WARNING("[CCU_PRINT] invalid format at %s:%d", filename == nullptr ? "<null>" : filename, lineno);
        return nullptr;
    }

    if (argc == std::numeric_limits<size_t>::max() ||
        argc + 1 >
            (std::numeric_limits<size_t>::max() - sizeof(PrintInfoValue)) / sizeof(uint64_t) / CCU_PRINT_RING_DEPTH) {
        HCCL_RUN_WARNING("[CCU_PRINT] allocation size overflow at %s:%d", filename, lineno);
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(impl_->mapMutex);
    if (!impl_->InitSequenceLocked()) {
        return nullptr;
    }
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
    handle->recordBytes = sizeof(uint64_t) * (argc + 1);
    handle->ringBytes = handle->recordBytes * CCU_PRINT_RING_DEPTH;
    handle->allocationBytes = sizeof(PrintInfoValue) + handle->ringBytes;
    handle->sequenceAddressCell = impl_->sequenceAddressCell;

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
