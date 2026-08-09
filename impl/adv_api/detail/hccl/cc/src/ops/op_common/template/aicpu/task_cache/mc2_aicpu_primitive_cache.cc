/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "mc2_aicpu_primitive_cache.h"

#include <memory>
#include <mutex>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "log.h"

namespace mc2_ops_hccl {
namespace {

constexpr size_t MAX_PRIMITIVE_TASK_NUM = 4096U;

enum class AddressKind : uint8_t {
    INPUT = 0,
    OUTPUT,
    FIXED,
};

enum class PrimitiveKind : uint8_t {
    LOCAL_COPY = 0,
    LOCAL_REDUCE,
    THREAD_NOTIFY_RECORD,
    THREAD_NOTIFY_WAIT,
    WRITE,
    WRITE_REDUCE,
    READ,
    READ_REDUCE,
    CHANNEL_NOTIFY_RECORD,
    CHANNEL_NOTIFY_WAIT,
    BATCH_TRANSFER,
    BATCH_MODE_START,
    BATCH_MODE_END,
};

struct AddressRef {
    AddressKind kind = AddressKind::FIXED;
    uint64_t value = 0U;
};

struct CachedBatchTransfer {
    HcclHcommBatchTransferDesc desc{};
    AddressRef src;
    AddressRef dst;
    uint64_t bytes = 0U;
    bool hasAddress = false;
};

struct PrimitiveTask {
    PrimitiveKind kind = PrimitiveKind::LOCAL_COPY;
    ThreadHandle thread = 0U;
    ThreadHandle dstThread = 0U;
    ChannelHandle channel = 0U;
    AddressRef src;
    AddressRef dst;
    uint64_t countOrLength = 0U;
    uint32_t notifyIdx = 0U;
    uint32_t timeout = 0U;
    HcommDataType dataType = HCOMM_DATA_TYPE_RESERVED;
    HcommReduceOp reduceOp = HCOMM_REDUCE_RESERVED;
    std::vector<CachedBatchTransfer> batchTransfers;
    std::string batchTag;
};

struct PrimitiveCacheEntry {
    void* comm = nullptr;
    std::vector<PrimitiveTask> tasks;
};

struct CaptureContext {
    bool active = false;
    bool finished = false;
    bool valid = false;
    Mc2AicpuTaskCachePlan plan;
    std::vector<PrimitiveTask> tasks;
};

thread_local CaptureContext g_capture;
std::mutex g_cacheMutex;
std::unordered_map<std::string, std::shared_ptr<const PrimitiveCacheEntry>> g_cacheEntries;
std::unordered_map<void*, std::unordered_set<std::string>> g_commTags;

bool ContainsRange(void* basePtr, uint64_t size, const void* ptr, uint64_t bytes, uint64_t& offset)
{
    if (basePtr == nullptr || ptr == nullptr) {
        return false;
    }
    const uint64_t base = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(basePtr));
    const uint64_t addr = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(ptr));
    if (addr < base) {
        return false;
    }
    offset = addr - base;
    return offset <= size && bytes <= size - offset;
}

bool BuildAddressRef(const void* ptr, uint64_t bytes, AddressRef& address)
{
    if (bytes == 0U) {
        address.kind = AddressKind::FIXED;
        address.value = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(ptr));
        return true;
    }

    uint64_t offset = 0U;
    if (ContainsRange(g_capture.plan.addrs[0], g_capture.plan.sizes[0], ptr, bytes, offset)) {
        address.kind = AddressKind::INPUT;
        address.value = offset;
        return true;
    }
    if (ContainsRange(g_capture.plan.addrs[1], g_capture.plan.sizes[1], ptr, bytes, offset)) {
        address.kind = AddressKind::OUTPUT;
        address.value = offset;
        return true;
    }
    for (const Mc2TaskCacheMemoryRange& range : g_capture.plan.fixedRanges) {
        if (ContainsRange(range.addr, range.size, ptr, bytes, offset)) {
            address.kind = AddressKind::FIXED;
            address.value = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(ptr));
            return true;
        }
    }
    return false;
}

bool ResolveAddress(
    const Mc2AicpuTaskCachePlan& plan, const AddressRef& address, uint64_t bytes, void*& resolvedAddress)
{
    if (address.kind == AddressKind::INPUT || address.kind == AddressKind::OUTPUT) {
        const size_t index = address.kind == AddressKind::INPUT ? 0U : 1U;
        if (plan.addrs[index] == nullptr || address.value > plan.sizes[index] ||
            bytes > plan.sizes[index] - address.value) {
            return false;
        }
        resolvedAddress = static_cast<void*>(static_cast<uint8_t*>(plan.addrs[index]) + address.value);
        return true;
    }

    resolvedAddress = reinterpret_cast<void*>(static_cast<uintptr_t>(address.value));
    if (bytes == 0U) {
        return true;
    }
    uint64_t offset = 0U;
    for (const Mc2TaskCacheMemoryRange& range : plan.fixedRanges) {
        if (ContainsRange(range.addr, range.size, resolvedAddress, bytes, offset)) {
            return true;
        }
    }
    return false;
}

uint64_t GetDataTypeSize(HcommDataType dataType)
{
    switch (dataType) {
        case HCOMM_DATA_TYPE_INT8:
        case HCOMM_DATA_TYPE_UINT8:
        case HCOMM_DATA_TYPE_HIF8:
        case HCOMM_DATA_TYPE_FP8E4M3:
        case HCOMM_DATA_TYPE_FP8E5M2:
        case HCOMM_DATA_TYPE_FP8E8M0:
        case HCOMM_DATA_TYPE_MXFP8:
            return 1U;
        case HCOMM_DATA_TYPE_INT16:
        case HCOMM_DATA_TYPE_UINT16:
        case HCOMM_DATA_TYPE_FP16:
        case HCOMM_DATA_TYPE_BFP16:
            return 2U;
        case HCOMM_DATA_TYPE_INT32:
        case HCOMM_DATA_TYPE_UINT32:
        case HCOMM_DATA_TYPE_FP32:
            return 4U;
        case HCOMM_DATA_TYPE_INT64:
        case HCOMM_DATA_TYPE_UINT64:
        case HCOMM_DATA_TYPE_FP64:
            return 8U;
        case HCOMM_DATA_TYPE_INT128:
            return 16U;
        default:
            return 0U;
    }
}

bool GetReduceBytes(uint64_t count, HcommDataType dataType, uint64_t& bytes)
{
    const uint64_t elementSize = GetDataTypeSize(dataType);
    if (elementSize == 0U || count > std::numeric_limits<uint64_t>::max() / elementSize) {
        return false;
    }
    bytes = count * elementSize;
    return true;
}

void InvalidateCapture(const char* reason)
{
    if (g_capture.active && g_capture.valid) {
        HCCL_WARNING("[MC2_PRIMITIVE_CACHE][Capture] disabled, reason[%s].", reason);
        g_capture.valid = false;
    }
}

void RecordTask(PrimitiveTask&& task)
{
    if (!g_capture.active || !g_capture.valid) {
        return;
    }
    if (g_capture.tasks.size() >= MAX_PRIMITIVE_TASK_NUM) {
        InvalidateCapture("primitive task count exceeds limit");
        return;
    }
    g_capture.tasks.push_back(std::move(task));
}

bool BuildDataTaskAddresses(PrimitiveTask& task, const void* src, void* dst, uint64_t bytes)
{
    if (!BuildAddressRef(src, bytes, task.src) || !BuildAddressRef(dst, bytes, task.dst)) {
        InvalidateCapture("data address is outside replayable ranges");
        return false;
    }
    return true;
}

bool BuildBatchTransfer(const HcclHcommBatchTransferDesc& desc, CachedBatchTransfer& cached)
{
    cached.desc = desc;
    const void* src = nullptr;
    void* dst = nullptr;
    switch (desc.transType) {
        case HCCL_HCOMM_TRANSFER_TYPE_WRITE:
            cached.bytes = desc.transferInfo.write.len;
            src = desc.transferInfo.write.src;
            dst = desc.transferInfo.write.dst;
            break;
        case HCCL_HCOMM_TRANSFER_TYPE_READ:
            cached.bytes = desc.transferInfo.read.len;
            src = desc.transferInfo.read.src;
            dst = desc.transferInfo.read.dst;
            break;
        case HCCL_HCOMM_TRANSFER_TYPE_WRITE_REDUCE:
        case HCCL_HCOMM_TRANSFER_TYPE_READ_REDUCE:
            if (!GetReduceBytes(desc.transferInfo.reduce.count, desc.transferInfo.reduce.dataType, cached.bytes)) {
                return false;
            }
            src = desc.transferInfo.reduce.src;
            dst = desc.transferInfo.reduce.dst;
            break;
        default:
            return false;
    }
    cached.hasAddress = true;
    return BuildAddressRef(src, cached.bytes, cached.src) && BuildAddressRef(dst, cached.bytes, cached.dst);
}

bool ResolveBatchTransfer(
    const Mc2AicpuTaskCachePlan& plan, const CachedBatchTransfer& cached, HcclHcommBatchTransferDesc& desc)
{
    desc = cached.desc;
    if (!cached.hasAddress) {
        return true;
    }
    void* src = nullptr;
    void* dst = nullptr;
    if (!ResolveAddress(plan, cached.src, cached.bytes, src) ||
        !ResolveAddress(plan, cached.dst, cached.bytes, dst)) {
        return false;
    }
    switch (desc.transType) {
        case HCCL_HCOMM_TRANSFER_TYPE_WRITE:
            desc.transferInfo.write.src = src;
            desc.transferInfo.write.dst = dst;
            break;
        case HCCL_HCOMM_TRANSFER_TYPE_READ:
            desc.transferInfo.read.src = src;
            desc.transferInfo.read.dst = dst;
            break;
        case HCCL_HCOMM_TRANSFER_TYPE_WRITE_REDUCE:
        case HCCL_HCOMM_TRANSFER_TYPE_READ_REDUCE:
            desc.transferInfo.reduce.src = src;
            desc.transferInfo.reduce.dst = dst;
            break;
        default:
            return false;
    }
    return true;
}

bool ValidateTask(const Mc2AicpuTaskCachePlan& plan, const PrimitiveTask& task)
{
    uint64_t bytes = task.countOrLength;
    if (task.kind == PrimitiveKind::LOCAL_REDUCE || task.kind == PrimitiveKind::WRITE_REDUCE ||
        task.kind == PrimitiveKind::READ_REDUCE) {
        if (!GetReduceBytes(task.countOrLength, task.dataType, bytes)) {
            return false;
        }
    }
    if (task.kind == PrimitiveKind::LOCAL_COPY || task.kind == PrimitiveKind::LOCAL_REDUCE ||
        task.kind == PrimitiveKind::WRITE || task.kind == PrimitiveKind::WRITE_REDUCE ||
        task.kind == PrimitiveKind::READ || task.kind == PrimitiveKind::READ_REDUCE) {
        void* src = nullptr;
        void* dst = nullptr;
        return ResolveAddress(plan, task.src, bytes, src) && ResolveAddress(plan, task.dst, bytes, dst);
    }
    if (task.kind == PrimitiveKind::BATCH_TRANSFER) {
        for (const CachedBatchTransfer& transfer : task.batchTransfers) {
            HcclHcommBatchTransferDesc resolved{};
            if (!ResolveBatchTransfer(plan, transfer, resolved)) {
                return false;
            }
        }
    }
    return true;
}

HcclResult ReplayTask(const Mc2AicpuTaskCachePlan& plan, const PrimitiveTask& task)
{
    void* src = nullptr;
    void* dst = nullptr;
    uint64_t bytes = task.countOrLength;
    int32_t ret = HCCL_SUCCESS;
    switch (task.kind) {
        case PrimitiveKind::LOCAL_COPY:
            (void)ResolveAddress(plan, task.src, bytes, src);
            (void)ResolveAddress(plan, task.dst, bytes, dst);
            ret = ::HcommLocalCopyOnThread(task.thread, dst, src, bytes);
            break;
        case PrimitiveKind::LOCAL_REDUCE:
            (void)GetReduceBytes(task.countOrLength, task.dataType, bytes);
            (void)ResolveAddress(plan, task.src, bytes, src);
            (void)ResolveAddress(plan, task.dst, bytes, dst);
            ret = ::HcommLocalReduceOnThread(
                task.thread, dst, src, task.countOrLength, task.dataType, task.reduceOp);
            break;
        case PrimitiveKind::THREAD_NOTIFY_RECORD:
            ret = ::HcommThreadNotifyRecordOnThread(task.thread, task.dstThread, task.notifyIdx);
            break;
        case PrimitiveKind::THREAD_NOTIFY_WAIT:
            ret = ::HcommThreadNotifyWaitOnThread(task.thread, task.notifyIdx, task.timeout);
            break;
        case PrimitiveKind::WRITE:
        case PrimitiveKind::READ:
            (void)ResolveAddress(plan, task.src, bytes, src);
            (void)ResolveAddress(plan, task.dst, bytes, dst);
            ret = task.kind == PrimitiveKind::WRITE ?
                      ::HcommWriteOnThread(task.thread, task.channel, dst, src, bytes) :
                      ::HcommReadOnThread(task.thread, task.channel, dst, src, bytes);
            break;
        case PrimitiveKind::WRITE_REDUCE:
        case PrimitiveKind::READ_REDUCE:
            (void)GetReduceBytes(task.countOrLength, task.dataType, bytes);
            (void)ResolveAddress(plan, task.src, bytes, src);
            (void)ResolveAddress(plan, task.dst, bytes, dst);
            ret = task.kind == PrimitiveKind::WRITE_REDUCE ?
                      ::HcommWriteReduceOnThread(
                          task.thread, task.channel, dst, src, task.countOrLength, task.dataType, task.reduceOp) :
                      ::HcommReadReduceOnThread(
                          task.thread, task.channel, dst, src, task.countOrLength, task.dataType, task.reduceOp);
            break;
        case PrimitiveKind::CHANNEL_NOTIFY_RECORD:
            ret = ::HcommChannelNotifyRecordOnThread(task.thread, task.channel, task.notifyIdx);
            break;
        case PrimitiveKind::CHANNEL_NOTIFY_WAIT:
            ret = ::HcommChannelNotifyWaitOnThread(task.thread, task.channel, task.notifyIdx, task.timeout);
            break;
        case PrimitiveKind::BATCH_TRANSFER: {
            std::vector<HcclHcommBatchTransferDesc> descs(task.batchTransfers.size());
            for (size_t i = 0; i < task.batchTransfers.size(); ++i) {
                (void)ResolveBatchTransfer(plan, task.batchTransfers[i], descs[i]);
            }
            ret = ::HcclHcommBatchTransferOnThread(
                task.thread, task.channel, descs.data(), static_cast<uint32_t>(descs.size()));
            break;
        }
        case PrimitiveKind::BATCH_MODE_START:
            ret = ::HcommBatchModeStart(task.batchTag.c_str());
            break;
        case PrimitiveKind::BATCH_MODE_END:
            ret = ::HcommBatchModeEnd(task.batchTag.c_str());
            break;
        default:
            return HCCL_E_INTERNAL;
    }
    return static_cast<HcclResult>(ret);
}

} // namespace

HcclResult Mc2PrimitiveTaskCacheLookup(const Mc2AicpuTaskCachePlan& plan, bool& isHit)
{
    std::lock_guard<std::mutex> lock(g_cacheMutex);
    isHit = g_cacheEntries.find(plan.tag) != g_cacheEntries.end();
    return HCCL_SUCCESS;
}

HcclResult Mc2PrimitiveTaskCacheBeginCapture(const Mc2AicpuTaskCachePlan& plan)
{
    if (g_capture.active || g_capture.finished) {
        HCCL_ERROR("[MC2_PRIMITIVE_CACHE][Begin] capture context is busy, tag[%s].", plan.tag.c_str());
        return HCCL_E_INTERNAL;
    }
    g_capture = CaptureContext{};
    g_capture.active = true;
    g_capture.valid = true;
    g_capture.plan = plan;
    return HCCL_SUCCESS;
}

void Mc2PrimitiveTaskCacheFinishCapture()
{
    if (!g_capture.active) {
        return;
    }
    g_capture.active = false;
    g_capture.finished = true;
}

void Mc2PrimitiveTaskCacheAbortCapture()
{
    g_capture = CaptureContext{};
}

HcclResult Mc2PrimitiveTaskCachePublish(const Mc2AicpuTaskCachePlan& plan)
{
    if (!g_capture.finished || g_capture.plan.tag != plan.tag) {
        HCCL_ERROR("[MC2_PRIMITIVE_CACHE][Publish] invalid capture state, tag[%s].", plan.tag.c_str());
        Mc2PrimitiveTaskCacheAbortCapture();
        return HCCL_E_INTERNAL;
    }
    if (!g_capture.valid || g_capture.tasks.empty()) {
        HCCL_WARNING("[MC2_PRIMITIVE_CACHE][Publish] skip invalid or empty plan, tag[%s].", plan.tag.c_str());
        Mc2PrimitiveTaskCacheAbortCapture();
        return HCCL_SUCCESS;
    }

    auto entry = std::make_shared<PrimitiveCacheEntry>();
    entry->comm = plan.comm;
    entry->tasks = std::move(g_capture.tasks);
    {
        std::lock_guard<std::mutex> lock(g_cacheMutex);
        g_cacheEntries[plan.tag] = entry;
        g_commTags[plan.comm].insert(plan.tag);
    }
    HCCL_INFO(
        "[MC2_PRIMITIVE_CACHE][Publish] tag[%s], taskNum[%zu].", plan.tag.c_str(), entry->tasks.size());
    Mc2PrimitiveTaskCacheAbortCapture();
    return HCCL_SUCCESS;
}

HcclResult Mc2PrimitiveTaskCacheReplay(const Mc2AicpuTaskCachePlan& plan)
{
    std::shared_ptr<const PrimitiveCacheEntry> entry;
    {
        std::lock_guard<std::mutex> lock(g_cacheMutex);
        const auto iter = g_cacheEntries.find(plan.tag);
        if (iter == g_cacheEntries.end()) {
            return HCCL_E_NOT_FOUND;
        }
        entry = iter->second;
    }
    if (entry->comm != plan.comm) {
        return HCCL_E_INTERNAL;
    }
    for (const PrimitiveTask& task : entry->tasks) {
        if (!ValidateTask(plan, task)) {
            HCCL_ERROR("[MC2_PRIMITIVE_CACHE][Replay] preflight failed, tag[%s].", plan.tag.c_str());
            return HCCL_E_PARA;
        }
    }
    for (const PrimitiveTask& task : entry->tasks) {
        const HcclResult ret = ReplayTask(plan, task);
        if (ret != HCCL_SUCCESS) {
            HCCL_ERROR("[MC2_PRIMITIVE_CACHE][Replay] task failed, tag[%s], ret[%d].", plan.tag.c_str(), ret);
            return ret;
        }
    }
    HCCL_INFO(
        "[MC2_PRIMITIVE_CACHE][Hit] tag[%s], taskNum[%zu].", plan.tag.c_str(), entry->tasks.size());
    return HCCL_SUCCESS;
}

HcclResult Mc2PrimitiveTaskCacheClear(const Mc2AicpuTaskCachePlan& plan)
{
    std::lock_guard<std::mutex> lock(g_cacheMutex);
    g_cacheEntries.erase(plan.tag);
    const auto iter = g_commTags.find(plan.comm);
    if (iter == g_commTags.end()) {
        return HCCL_SUCCESS;
    }
    iter->second.erase(plan.tag);
    if (iter->second.empty()) {
        g_commTags.erase(iter);
    }
    return HCCL_SUCCESS;
}

HcclResult Mc2PrimitiveTaskCacheEvict(void* comm)
{
    std::lock_guard<std::mutex> lock(g_cacheMutex);
    const auto iter = g_commTags.find(comm);
    if (iter == g_commTags.end()) {
        return HCCL_SUCCESS;
    }
    for (const std::string& tag : iter->second) {
        g_cacheEntries.erase(tag);
    }
    g_commTags.erase(iter);
    return HCCL_SUCCESS;
}

int32_t Mc2CachedHcommLocalCopyOnThread(ThreadHandle thread, void* dst, const void* src, uint64_t len)
{
    const int32_t ret = ::HcommLocalCopyOnThread(thread, dst, src, len);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        PrimitiveTask task;
        task.kind = PrimitiveKind::LOCAL_COPY;
        task.thread = thread;
        task.countOrLength = len;
        if (BuildDataTaskAddresses(task, src, dst, len)) {
            RecordTask(std::move(task));
        }
    }
    return ret;
}

int32_t Mc2CachedHcommLocalReduceOnThread(
    ThreadHandle thread, void* dst, const void* src, uint64_t count, HcommDataType dataType, HcommReduceOp reduceOp)
{
    const int32_t ret = ::HcommLocalReduceOnThread(thread, dst, src, count, dataType, reduceOp);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        uint64_t bytes = 0U;
        PrimitiveTask task;
        task.kind = PrimitiveKind::LOCAL_REDUCE;
        task.thread = thread;
        task.countOrLength = count;
        task.dataType = dataType;
        task.reduceOp = reduceOp;
        if (!GetReduceBytes(count, dataType, bytes)) {
            InvalidateCapture("invalid local reduce data type or count");
        } else if (BuildDataTaskAddresses(task, src, dst, bytes)) {
            RecordTask(std::move(task));
        }
    }
    return ret;
}

int32_t Mc2CachedHcommThreadNotifyRecordOnThread(ThreadHandle thread, ThreadHandle dstThread, uint32_t dstNotifyIdx)
{
    const int32_t ret = ::HcommThreadNotifyRecordOnThread(thread, dstThread, dstNotifyIdx);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        PrimitiveTask task;
        task.kind = PrimitiveKind::THREAD_NOTIFY_RECORD;
        task.thread = thread;
        task.dstThread = dstThread;
        task.notifyIdx = dstNotifyIdx;
        RecordTask(std::move(task));
    }
    return ret;
}

int32_t Mc2CachedHcommThreadNotifyWaitOnThread(ThreadHandle thread, uint32_t notifyIdx, uint32_t timeOut)
{
    const int32_t ret = ::HcommThreadNotifyWaitOnThread(thread, notifyIdx, timeOut);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        PrimitiveTask task;
        task.kind = PrimitiveKind::THREAD_NOTIFY_WAIT;
        task.thread = thread;
        task.notifyIdx = notifyIdx;
        task.timeout = timeOut;
        RecordTask(std::move(task));
    }
    return ret;
}

int32_t Mc2CachedHcommWriteOnThread(
    ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len)
{
    const int32_t ret = ::HcommWriteOnThread(thread, channel, dst, src, len);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        PrimitiveTask task;
        task.kind = PrimitiveKind::WRITE;
        task.thread = thread;
        task.channel = channel;
        task.countOrLength = len;
        if (BuildDataTaskAddresses(task, src, dst, len)) {
            RecordTask(std::move(task));
        }
    }
    return ret;
}

int32_t Mc2CachedHcommWriteReduceOnThread(
    ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t count, HcommDataType dataType,
    HcommReduceOp reduceOp)
{
    const int32_t ret = ::HcommWriteReduceOnThread(thread, channel, dst, src, count, dataType, reduceOp);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        uint64_t bytes = 0U;
        PrimitiveTask task;
        task.kind = PrimitiveKind::WRITE_REDUCE;
        task.thread = thread;
        task.channel = channel;
        task.countOrLength = count;
        task.dataType = dataType;
        task.reduceOp = reduceOp;
        if (!GetReduceBytes(count, dataType, bytes)) {
            InvalidateCapture("invalid write reduce data type or count");
        } else if (BuildDataTaskAddresses(task, src, dst, bytes)) {
            RecordTask(std::move(task));
        }
    }
    return ret;
}

int32_t Mc2CachedHcommReadOnThread(
    ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len)
{
    const int32_t ret = ::HcommReadOnThread(thread, channel, dst, src, len);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        PrimitiveTask task;
        task.kind = PrimitiveKind::READ;
        task.thread = thread;
        task.channel = channel;
        task.countOrLength = len;
        if (BuildDataTaskAddresses(task, src, dst, len)) {
            RecordTask(std::move(task));
        }
    }
    return ret;
}

int32_t Mc2CachedHcommReadReduceOnThread(
    ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t count, HcommDataType dataType,
    HcommReduceOp reduceOp)
{
    const int32_t ret = ::HcommReadReduceOnThread(thread, channel, dst, src, count, dataType, reduceOp);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        uint64_t bytes = 0U;
        PrimitiveTask task;
        task.kind = PrimitiveKind::READ_REDUCE;
        task.thread = thread;
        task.channel = channel;
        task.countOrLength = count;
        task.dataType = dataType;
        task.reduceOp = reduceOp;
        if (!GetReduceBytes(count, dataType, bytes)) {
            InvalidateCapture("invalid read reduce data type or count");
        } else if (BuildDataTaskAddresses(task, src, dst, bytes)) {
            RecordTask(std::move(task));
        }
    }
    return ret;
}

int32_t Mc2CachedHcommChannelNotifyRecordOnThread(
    ThreadHandle thread, ChannelHandle channel, uint32_t remoteNotifyIdx)
{
    const int32_t ret = ::HcommChannelNotifyRecordOnThread(thread, channel, remoteNotifyIdx);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        PrimitiveTask task;
        task.kind = PrimitiveKind::CHANNEL_NOTIFY_RECORD;
        task.thread = thread;
        task.channel = channel;
        task.notifyIdx = remoteNotifyIdx;
        RecordTask(std::move(task));
    }
    return ret;
}

int32_t Mc2CachedHcommChannelNotifyWaitOnThread(
    ThreadHandle thread, ChannelHandle channel, uint32_t localNotifyIdx, uint32_t timeOut)
{
    const int32_t ret = ::HcommChannelNotifyWaitOnThread(thread, channel, localNotifyIdx, timeOut);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        PrimitiveTask task;
        task.kind = PrimitiveKind::CHANNEL_NOTIFY_WAIT;
        task.thread = thread;
        task.channel = channel;
        task.notifyIdx = localNotifyIdx;
        task.timeout = timeOut;
        RecordTask(std::move(task));
    }
    return ret;
}

int32_t Mc2CachedHcclHcommBatchTransferOnThread(
    ThreadHandle thread, ChannelHandle channel, const HcclHcommBatchTransferDesc* transferDescs,
    uint32_t transferDescNum)
{
    const int32_t ret = ::HcclHcommBatchTransferOnThread(thread, channel, transferDescs, transferDescNum);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        if (transferDescs == nullptr && transferDescNum > 0U) {
            InvalidateCapture("batch transfer descriptor is null");
            return ret;
        }
        PrimitiveTask task;
        task.kind = PrimitiveKind::BATCH_TRANSFER;
        task.thread = thread;
        task.channel = channel;
        task.batchTransfers.reserve(transferDescNum);
        for (uint32_t i = 0U; i < transferDescNum; ++i) {
            CachedBatchTransfer transfer;
            if (!BuildBatchTransfer(transferDescs[i], transfer)) {
                InvalidateCapture("unsupported batch transfer descriptor");
                return ret;
            }
            task.batchTransfers.push_back(std::move(transfer));
        }
        RecordTask(std::move(task));
    }
    return ret;
}

int32_t Mc2CachedHcommBatchModeStart(const char* batchTag)
{
    const int32_t ret = ::HcommBatchModeStart(batchTag);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        PrimitiveTask task;
        task.kind = PrimitiveKind::BATCH_MODE_START;
        task.batchTag = batchTag == nullptr ? "" : batchTag;
        RecordTask(std::move(task));
    }
    return ret;
}

int32_t Mc2CachedHcommBatchModeEnd(const char* batchTag)
{
    const int32_t ret = ::HcommBatchModeEnd(batchTag);
    if (ret == HCCL_SUCCESS && g_capture.active && g_capture.valid) {
        PrimitiveTask task;
        task.kind = PrimitiveKind::BATCH_MODE_END;
        task.batchTag = batchTag == nullptr ? "" : batchTag;
        RecordTask(std::move(task));
    }
    return ret;
}

} // namespace mc2_ops_hccl
