/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "mc2_aicpu_task_cache.h"
#include "log.h"
#include <cstring>
#include <vector>
#include <atomic>
#include <cstdlib>
#include <dlfcn.h>
#include <utility>
#include "asccomm_primitives.h"
#include "asccomm_command_replay.h"
#include "mc2_server_stage_timing.h"

namespace mc2_ops_hccl {
namespace {
using ReplayFn = int32_t (*)(uint32_t, void**, AsccommReplayInfoV1*);
ReplayFn CommandBackend()
{
    static ReplayFn fn = reinterpret_cast<ReplayFn>(dlsym(RTLD_DEFAULT, "asccomm_command_replay_v1"));
    return fn;
}
constexpr uint64_t MAX_COMMAND_BYTES = 64U * 1024U * 1024U;
constexpr uint64_t MAX_DOMAIN_BYTES = 10U * 1024U * 1024U;
constexpr size_t MAX_DOMAINS = 256U;
constexpr size_t MAX_ENTRIES_PER_DOMAIN = 1024U;
std::atomic<uint64_t> commandBytes{0};
thread_local std::shared_ptr<const AlgResourceCtxSerializable> currentHolder;

struct CommandEntry {
    void* handle = nullptr;
    uint64_t bytes = 0;
    ~CommandEntry()
    {
        if (handle != nullptr) {
            (void)CommandBackend()(ASCCOMM_REPLAY_DESTROY, &handle, nullptr);
        }
        commandBytes.fetch_sub(bytes, std::memory_order_relaxed);
    }
};
struct CommandDomain {
    void* comm;
    std::shared_ptr<const AlgResourceCtxSerializable> holder;
    std::mutex mutex;
    uint64_t bytes = 0;
    std::unordered_map<std::string, std::unique_ptr<CommandEntry>> entries;
};
std::mutex domainsMutex;
std::unordered_map<const AlgResourceCtxSerializable*, std::shared_ptr<CommandDomain>> domains;

std::shared_ptr<CommandDomain> GetDomain(void* comm, const AlgResourceCtxSerializable& resCtx)
{
    if (currentHolder.get() != &resCtx) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(domainsMutex);
    auto found = domains.find(&resCtx);
    if (found != domains.end()) {
        return found->second->comm == comm ? found->second : nullptr;
    }
    if (domains.size() >= MAX_DOMAINS || !ValidateMc2CommandResource(resCtx)) {
        return nullptr;
    }
    auto domain = std::make_shared<CommandDomain>();
    domain->comm = comm;
    domain->holder = currentHolder;
    domains.emplace(&resCtx, domain);
    return domain;
}
bool ReserveCommandBytes(uint64_t bytes)
{
    uint64_t previous = commandBytes.load(std::memory_order_relaxed);
    do {
        if (previous > MAX_COMMAND_BYTES || bytes > MAX_COMMAND_BYTES - previous) {
            return false;
        }
    } while (!commandBytes.compare_exchange_weak(previous, previous + bytes, std::memory_order_relaxed));
    return true;
}

HcclResult ExecuteBatch(const OpParam& param, const std::function<HcclResult()>& execute)
{
    CHK_RET(static_cast<HcclResult>(asccomm_batch_mode_start(param.algTag)));
    HcclResult ret = HCCL_SUCCESS;
    try {
        ret = execute();
    } catch (...) {
        ret = HCCL_E_INTERNAL;
    }
    const auto end = static_cast<HcclResult>(asccomm_batch_mode_end(param.algTag));
    return ret != HCCL_SUCCESS ? ret : end;
}
} // namespace

Mc2CommandResourceScope::Mc2CommandResourceScope(std::shared_ptr<const AlgResourceCtxSerializable> holder)
    : previous_(std::move(currentHolder))
{
    currentHolder = std::move(holder);
}
Mc2CommandResourceScope::~Mc2CommandResourceScope() { currentHolder = std::move(previous_); }

bool Mc2AsccommCommandCacheSupported() { return CommandBackend() != nullptr; }

HcclResult RunWithAsccommCommandCache(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx, const std::function<HcclResult()>& execute)
{
    Mc2AicpuTaskCachePlan plan;
    std::shared_ptr<CommandDomain> domain;
    try {
        if (param.aicpuCacheEnable && Mc2AsccommCommandCacheSupported()) {
            domain = GetDomain(param.hcclComm, resCtx);
            if (domain != nullptr &&
                (BuildMc2AicpuTaskCachePlan(param, resCtx, plan, true, true) != HCCL_SUCCESS || !plan.enabled)) {
                domain.reset();
            }
        }
    } catch (...) { /* No task has been submitted: execute without caching. */
    }
    if (domain == nullptr) {
        return ExecuteBatch(param, execute);
    }
    std::unique_lock<std::mutex> lock(domain->mutex);
    const auto found = domain->entries.find(plan.exactKey);
    if (found != domain->entries.end()) {
        Mc2ServerStageTiming::Cache(true);
        CHK_RET(static_cast<HcclResult>(asccomm_batch_mode_start(param.algTag)));
        auto ret = static_cast<HcclResult>(CommandBackend()(ASCCOMM_REPLAY_EXECUTE, &found->second->handle, nullptr));
        const auto end = static_cast<HcclResult>(asccomm_batch_mode_end(param.algTag));
        if (ret == HCCL_SUCCESS) {
            ret = end;
        }
        if (ret != HCCL_SUCCESS) {
            domain->bytes -= found->second->bytes;
            domain->entries.erase(found);
        }
        return ret; // A replay may have partially submitted. Never execute again.
    }
    if (domain->entries.size() >= MAX_ENTRIES_PER_DOMAIN || domain->bytes >= MAX_DOMAIN_BYTES) {
        return ExecuteBatch(param, execute);
    }
    std::unique_ptr<CommandEntry> entry;
    try {
        entry.reset(new CommandEntry);
    } catch (...) {
        return ExecuteBatch(param, execute);
    }
    if (CommandBackend()(ASCCOMM_REPLAY_CREATE, &entry->handle, nullptr) != 0) {
        return ExecuteBatch(param, execute);
    }
    // Start batch before recording. Its final flush is captured; replay has the same boundaries.
    CHK_RET(static_cast<HcclResult>(asccomm_batch_mode_start(param.algTag)));
    const bool capturing = CommandBackend()(ASCCOMM_REPLAY_BEGIN, &entry->handle, nullptr) == 0;
    if (capturing) {
        Mc2ServerStageTiming::Cache(false);
    }
    HcclResult ret = HCCL_SUCCESS;
    try {
        ret = execute();
    } catch (...) {
        ret = HCCL_E_INTERNAL;
    }
    const auto end = static_cast<HcclResult>(asccomm_batch_mode_end(param.algTag));
    if (ret == HCCL_SUCCESS) {
        ret = end;
    }
    if (!capturing || ret != HCCL_SUCCESS) {
        return ret;
    }
    AsccommReplayInfoV1 info{};
    const auto cacheRet = CommandBackend()(ASCCOMM_REPLAY_END, &entry->handle, &info);
    if (cacheRet != 0 || info.ready == 0 ||
        info.storageBytes + plan.exactKey.size() > MAX_DOMAIN_BYTES - domain->bytes) {
        return HCCL_SUCCESS; // Communication completed, but its capture is not admitted.
    }
    info.storageBytes += plan.exactKey.size();
    if (!ReserveCommandBytes(info.storageBytes)) {
        return HCCL_SUCCESS;
    }
    entry->bytes = info.storageBytes;
    try {
        domain->entries.emplace(plan.exactKey, std::move(entry));
        domain->bytes += info.storageBytes;
    } catch (...) { /* Drop metadata; the completed communication is never rerun. */
    }
    return HCCL_SUCCESS;
}

HcclResult ClearAsccommCommandCache(void* comm)
{
    std::vector<std::shared_ptr<CommandDomain>> matching;
    {
        std::lock_guard<std::mutex> lock(domainsMutex);
        for (const auto& item : domains) {
            if (item.second->comm == comm) {
                matching.push_back(item.second);
            }
        }
    }
    for (auto& domain : matching) {
        std::lock_guard<std::mutex> lock(domain->mutex);
        domain->entries.clear();
        domain->bytes = 0;
    }
    return HCCL_SUCCESS;
}
} // namespace mc2_ops_hccl
