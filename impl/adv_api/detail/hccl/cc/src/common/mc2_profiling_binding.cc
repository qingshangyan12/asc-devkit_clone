/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "hccl_alloc_ctx_res.h"
#include "mc2_profiling_binding.h"
#include "mc2_profiling_binding_protocol.h"
#include "acl/acl.h"
#include "log.h"
#include <algorithm>
#include <atomic>
#include <cstring>
#include <mutex>

namespace {
using namespace Mc2ProfilingBinding;
using namespace mc2_ops_hccl;
constexpr uint64_t SNAPSHOT_MAGIC = 0x4D4332534E415031ULL;
struct Snapshot {
    uint64_t magic = SNAPSHOT_MAGIC;
    uint64_t resourceId = 0;
    uint64_t generation = 0;
    uint64_t boundGeneration = 0;
    uint64_t boundStream = 0;
    uint32_t count = 0;
    uint32_t reserved = 0;
    uint64_t threads[MAX_THREADS] = {};
};
std::mutex snapshotMutex;
std::atomic<uint64_t> nextResourceId{1};
std::atomic<uint64_t> nextRequestId{1};

std::string SnapshotTag(const char* kind, const void* address)
{
    return std::string("__asc_mc2_prof_") + kind + "_" +
           std::to_string(reinterpret_cast<uintptr_t>(address));
}

HcclResult GetSnapshot(HcclComm comm, const std::string& tag, Snapshot*& snapshot, bool create)
{
    void* data = nullptr;
    uint64_t size = 0;
    HcclResult ret = HcclEngineCtxGet(comm, tag.c_str(), COMM_ENGINE_CPU, &data, &size);
    if (ret != HCCL_SUCCESS) {
        if (!create) {
            return ret;
        }
        CHK_RET(HcclEngineCtxCreate(comm, tag.c_str(), COMM_ENGINE_CPU, sizeof(Snapshot), &data));
        Snapshot empty;
        CHK_RET(HcclEngineCtxCopy(comm, COMM_ENGINE_CPU, tag.c_str(), &empty, sizeof(empty), 0));
        size = sizeof(Snapshot);
    }
    if (data == nullptr || size != sizeof(Snapshot)) {
        return HCCL_E_PARA;
    }
    snapshot = static_cast<Snapshot*>(data);
    return snapshot->magic == SNAPSHOT_MAGIC && snapshot->count <= MAX_THREADS ? HCCL_SUCCESS : HCCL_E_PARA;
}

HcclResult StoreThreads(Snapshot& snapshot, std::vector<uint64_t> threads, bool rebuilt)
{
    std::sort(threads.begin(), threads.end());
    threads.erase(std::unique(threads.begin(), threads.end()), threads.end());
    if (threads.empty() || threads.size() > MAX_THREADS || threads.front() == 0) {
        return HCCL_E_PARA;
    }
    if (snapshot.resourceId == 0) {
        snapshot.resourceId = nextResourceId.fetch_add(1);
    }
    if (rebuilt || snapshot.count != threads.size() ||
        !std::equal(threads.begin(), threads.end(), snapshot.threads)) {
        if (snapshot.generation == UINT64_MAX) {
            return HCCL_E_PARA;
        }
        ++snapshot.generation;
        snapshot.boundGeneration = 0;
        snapshot.boundStream = 0;
        snapshot.count = static_cast<uint32_t>(threads.size());
        std::copy(threads.begin(), threads.end(), snapshot.threads);
    }
    return HCCL_SUCCESS;
}

HcclResult ReadAlgorithmSnapshot(HcclComm comm, const OpParam& param, Snapshot*& snapshot)
{
    const auto tag = SnapshotTag("alg", param.resCtx);
    if (GetSnapshot(comm, tag, snapshot, false) == HCCL_SUCCESS && snapshot->count != 0) {
        return HCCL_SUCCESS;
    }
    // A cache hit may predate this library. Recover once from the actual serialized context.
    if (param.resCtx == nullptr || param.ctxSize == 0) {
        return HCCL_E_PARA;
    }
    std::vector<char> bytes(param.ctxSize);
    if (aclrtMemcpy(bytes.data(), bytes.size(), param.resCtx, bytes.size(), ACL_MEMCPY_DEVICE_TO_HOST) != ACL_SUCCESS) {
        return HCCL_E_RUNTIME;
    }
    AlgResourceCtxSerializable restored;
    restored.DeSerialize(bytes);
    CHK_RET(GetSnapshot(comm, tag, snapshot, true));
    return StoreThreads(*snapshot, std::vector<uint64_t>(restored.threads.begin(), restored.threads.end()), true);
}

HcclResult RemoveBinding(HcclComm comm, uint64_t resourceId)
{
    if (resourceId != 0) {
        const auto tag = Tag(resourceId);
        void* storage = nullptr;
        uint64_t size = 0;
        if (HcclEngineCtxGet(comm, tag.c_str(), COMM_ENGINE_CPU, &storage, &size) == HCCL_SUCCESS) {
            return HcclEngineCtxDestroy(comm, tag.c_str(), COMM_ENGINE_CPU);
        }
    }
    return HCCL_SUCCESS;
}

void InvalidateResource(HcclComm comm, Snapshot& resource) noexcept
{
    resource.boundGeneration = 0;
    resource.count = 0;
    try {
        const HcclResult ret = RemoveBinding(comm, resource.resourceId);
        if (ret != HCCL_SUCCESS) {
            HCCL_WARNING("[MC2_PROF_BIND] failed to invalidate stale relation, ret[%d]", ret);
        }
    } catch (...) {
        HCCL_WARNING("[MC2_PROF_BIND] failed to invalidate stale relation");
    }
}

HcclResult BindStream(HcclComm comm, const void* resource, void* stream)
{
    std::lock_guard<std::mutex> lock(snapshotMutex);
    Snapshot* snapshot = nullptr;
    CHK_RET(GetSnapshot(comm, SnapshotTag("res", resource), snapshot, false));
    if (snapshot->count == 0 || snapshot->resourceId == 0 || snapshot->generation == 0) {
        return HCCL_E_NOT_SUPPORT;
    }
    const uint64_t streamToken = reinterpret_cast<uintptr_t>(stream);
    if (snapshot->boundGeneration == snapshot->generation && snapshot->boundStream == streamToken) {
        return HCCL_SUCCESS;
    }
    // A failed replacement must not leave the previous KFC relation live for the next START.
    snapshot->boundGeneration = 0;
    CHK_RET(RemoveBinding(comm, snapshot->resourceId));
    int32_t streamId = -1;
    if (aclrtStreamGetId(stream, &streamId) != ACL_SUCCESS || streamId < 0) {
        return HCCL_E_RUNTIME;
    }
    Header request;
    request.resourceId = snapshot->resourceId;
    request.generation = snapshot->generation;
    request.kfcStreamId = static_cast<uint32_t>(streamId);
    request.threadCount = snapshot->count;
    request.requestId = nextRequestId.fetch_add(1);
    request.totalSize = sizeof(Header) + request.threadCount * sizeof(uint64_t);
    std::vector<uint8_t> wire(request.totalSize);
    std::memcpy(wire.data(), &request, sizeof(request));
    std::memcpy(wire.data() + sizeof(request), snapshot->threads, request.threadCount * sizeof(uint64_t));
    const auto tag = Tag(request.resourceId);
    void* storage = nullptr;
    uint64_t size = 0;
    if (HcclEngineCtxGet(comm, tag.c_str(), COMM_ENGINE_CPU, &storage, &size) != HCCL_SUCCESS) {
        // Reserve the protocol limit so the complete request always fits.
        CHK_RET(HcclEngineCtxCreate(comm, tag.c_str(), COMM_ENGINE_CPU,
                                   sizeof(Header) + MAX_THREADS * sizeof(uint64_t), &storage));
        size = sizeof(Header) + MAX_THREADS * sizeof(uint64_t);
    }
    if (storage == nullptr || size < request.totalSize) {
        return HCCL_E_PARA;
    }
    CHK_RET(HcclEngineCtxCopy(comm, COMM_ENGINE_CPU, tag.c_str(), wire.data(), wire.size(), 0));
    Header ack;
    std::memcpy(&ack, storage, sizeof(ack));
    if (ack.ackRequestId != request.requestId || ack.ackStatus == ACK_PENDING) {
        HCCL_WARNING("[MC2_PROF_BIND] HCOMM package does not acknowledge the binding protocol");
        return HCCL_E_NOT_SUPPORT;
    }
    if (ack.ackStatus != HCCL_SUCCESS) {
        return static_cast<HcclResult>(ack.ackStatus);
    }
    snapshot->boundGeneration = snapshot->generation;
    snapshot->boundStream = streamToken;
    return HCCL_SUCCESS;
}
} // namespace

HcclResult SaveMc2AlgorithmThreads(HcclComm comm, const OpParam& param, const std::vector<ThreadHandle>& threads)
{
    Snapshot* snapshot = nullptr;
    try {
        if (threads.empty()) {
            return HCCL_SUCCESS; // Reuse is recovered from the saved or serialized context, not this empty object.
        }
        std::lock_guard<std::mutex> lock(snapshotMutex);
        CHK_RET(GetSnapshot(comm, SnapshotTag("alg", param.resCtx), snapshot, true));
        const HcclResult ret = StoreThreads(*snapshot, std::vector<uint64_t>(threads.begin(), threads.end()), !param.cacheValid);
        if (ret != HCCL_SUCCESS) {
            snapshot->count = 0;
        }
        return ret;
    } catch (...) {
        if (snapshot != nullptr) {
            std::lock_guard<std::mutex> lock(snapshotMutex);
            snapshot->count = 0;
        }
        return HCCL_E_INTERNAL;
    }
}

HcclResult SaveMc2ProfilingResource(HcclComm comm, const void* opResCtx, const std::vector<OpParam>& params)
{
    Snapshot* resource = nullptr;
    try {
        std::lock_guard<std::mutex> lock(snapshotMutex);
        CHK_RET(GetSnapshot(comm, SnapshotTag("res", opResCtx), resource, true));
        if (resource->count != 0 && !params.empty() &&
            std::all_of(params.begin(), params.end(), [](const OpParam& param) { return param.cacheValid; })) {
            return HCCL_SUCCESS;
        }
        std::vector<uint64_t> threads;
        bool rebuilt = false;
        for (const auto& param : params) {
            Snapshot* algorithm = nullptr;
            const HcclResult ret = ReadAlgorithmSnapshot(comm, param, algorithm);
            if (ret != HCCL_SUCCESS) {
                InvalidateResource(comm, *resource);
                return ret;
            }
            threads.insert(threads.end(), algorithm->threads, algorithm->threads + algorithm->count);
            rebuilt = rebuilt || !param.cacheValid;
        }
        const uint64_t previousGeneration = resource->generation;
        HcclResult ret = StoreThreads(*resource, std::move(threads), rebuilt);
        if (ret == HCCL_SUCCESS && previousGeneration != 0 && previousGeneration != resource->generation) {
            ret = RemoveBinding(comm, resource->resourceId);
        }
        if (ret != HCCL_SUCCESS) {
            InvalidateResource(comm, *resource);
        }
        return ret;
    } catch (...) {
        if (resource != nullptr) {
            std::lock_guard<std::mutex> lock(snapshotMutex);
            InvalidateResource(comm, *resource);
        }
        return HCCL_E_INTERNAL;
    }
}

extern "C" __attribute__((visibility("default"))) int32_t HcclMc2BindProfilingStream(
    void* comm, const void* opResCtx, void* actualKfcStream)
{
    if (comm == nullptr || opResCtx == nullptr || actualKfcStream == nullptr) {
        return HCCL_E_PTR;
    }
    try {
        return BindStream(comm, opResCtx, actualKfcStream);
    } catch (...) {
        return HCCL_E_INTERNAL;
    }
}
