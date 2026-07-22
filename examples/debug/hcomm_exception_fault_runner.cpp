/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <unistd.h>

#include "acl/acl.h"
#include "acl/acl_rt.h"
#include "hccl/hccl.h"
#include "hccl/hccl_rank_graph.h"
#include "hccl/hccl_res.h"
#include "hcomm_exception_fault_cases.h"

extern void run_hcomm_urma_timeout_fault(CommContext *ctx, aclrtStream stream);
extern void run_hcomm_cqe_error_fault(CommContext *ctx, aclrtStream stream);
extern "C" void HcommUrmaTimeoutFaultKernel(CommContext *ctx);
extern "C" void HcommCqeErrorFaultKernel(CommContext *ctx);

namespace {
using HcommChannelInfoDumpFunc = void (*)(uint32_t, ChannelHandle *);

enum class FaultCase {
    URMA_TIMEOUT,
    CQE_ERROR,
};

class ThreadBarrier {
public:
    explicit ThreadBarrier(uint32_t total) : total_(total) {}

    void Wait()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        uint32_t generation = generation_;
        if (++arrived_ == total_) {
            arrived_ = 0;
            generation_++;
            cv_.notify_all();
            return;
        }
        cv_.wait(lock, [this, generation] { return generation != generation_; });
    }

private:
    uint32_t total_;
    uint32_t arrived_{0};
    uint32_t generation_{0};
    std::mutex mutex_;
    std::condition_variable cv_;
};

std::mutex g_dumpMutex;
std::vector<ChannelHandle> g_channels;
std::vector<CommContext> g_contexts;

struct ExceptionSummary {
    uint32_t taskId = 0xFFFFFFFFU;
    uint32_t streamId = 0xFFFFFFFFU;
    uint32_t threadId = 0xFFFFFFFFU;
    uint32_t deviceId = 0xFFFFFFFFU;
    uint32_t errorCode = 0xFFFFFFFFU;
    void *devArgs = nullptr;
    uint32_t devArgsLen = 0;
};

std::mutex g_exceptionMutex;
std::atomic<bool> g_exceptionPending{false};
std::atomic<uint64_t> g_exceptionCount{0};
ExceptionSummary g_lastException;

using AclrtGetFuncBySymbolFunc = aclError (*)(const void *, aclrtFuncHandle *);
using AclrtFunctionGetBinaryFunc = aclError (*)(aclrtFuncHandle, aclrtBinHandle *);
using AclrtExceptionInfoCallbackFunc = void (*)(aclrtExceptionInfo *);
using AclrtBinarySetExceptionCallbackFunc = aclError (*)(aclrtBinHandle, AclrtExceptionInfoCallbackFunc, void *);

#define ACL_CHECK(expr)                                                                                  \
    do {                                                                                                 \
        aclError ret = (expr);                                                                           \
        if (ret != ACL_SUCCESS) {                                                                        \
            std::printf("[hcomm_fault_runner] ACL failed %s:%d ret=%d\n", __FILE__, __LINE__, ret);       \
            return static_cast<int>(ret);                                                                \
        }                                                                                                \
    } while (0)

#define HCCL_CHECK(expr)                                                                                 \
    do {                                                                                                 \
        HcclResult ret = (expr);                                                                         \
        if (ret != HCCL_SUCCESS) {                                                                       \
            std::printf("[hcomm_fault_runner] HCCL failed %s:%d ret=%d\n", __FILE__, __LINE__, ret);      \
            return static_cast<int>(ret);                                                                \
        }                                                                                                \
    } while (0)

HcommChannelInfoDumpFunc ResolveChannelDumpFunc()
{
    static HcommChannelInfoDumpFunc dumpFunc = []() -> HcommChannelInfoDumpFunc {
        const char *soPath = std::getenv("HCOMM_CHANNEL_DUMP_SO");
        if (soPath == nullptr || soPath[0] == '\0') {
            soPath = "libhcomm_channel_dump.so";
        }

        void *handle = dlopen(soPath, RTLD_NOW | RTLD_LOCAL);
        if (handle == nullptr) {
            std::printf("[hcomm_fault_runner] dlopen %s failed: %s\n", soPath, dlerror());
            return nullptr;
        }

        dlerror();
        void *symbol = dlsym(handle, "HcommChannelInfoDump");
        const char *error = dlerror();
        if (error != nullptr) {
            std::printf("[hcomm_fault_runner] dlsym HcommChannelInfoDump failed: %s\n", error);
            return nullptr;
        }
        std::printf("[hcomm_fault_runner] resolved HcommChannelInfoDump from %s\n", soPath);
        std::fflush(stdout);
        return reinterpret_cast<HcommChannelInfoDumpFunc>(symbol);
    }();
    return dumpFunc;
}

void DumpChannels(const std::vector<ChannelHandle> &channels)
{
    auto dumpFunc = ResolveChannelDumpFunc();
    if (dumpFunc == nullptr || channels.empty()) {
        std::printf("[hcomm_fault_runner] skip channel dump, dumpFunc=%p channelNum=%zu\n",
            reinterpret_cast<void *>(dumpFunc), channels.size());
        std::fflush(stdout);
        return;
    }
    std::vector<ChannelHandle> channelCopy = channels;
    std::printf("[hcomm_fault_runner] call HcommChannelInfoDump channelNum=%zu first=0x%llx\n",
        channelCopy.size(), static_cast<unsigned long long>(channelCopy[0]));
    std::fflush(stdout);
    dumpFunc(static_cast<uint32_t>(channelCopy.size()), channelCopy.data());
    std::printf("[hcomm_fault_runner] HcommChannelInfoDump returned\n");
    std::fflush(stdout);
}

bool TryCopyCommContext(void *devArgs, uint32_t devArgsLen, CommContext &hostCtx)
{
    if (devArgs == nullptr || devArgsLen < sizeof(CommContext)) {
        std::printf("[hcomm_fault_runner] exception dev args are unavailable, len=%u\n", devArgsLen);
        return false;
    }
    aclError ret = aclrtMemcpy(&hostCtx, sizeof(hostCtx), devArgs, sizeof(CommContext), ACL_MEMCPY_DEVICE_TO_HOST);
    if (ret != ACL_SUCCESS) {
        std::printf("[hcomm_fault_runner] copy CommContext failed at report point, ret=%d\n", ret);
        return false;
    }
    return true;
}

void CustomCallback(aclrtExceptionInfo *exceptionInfo)
{
    if (exceptionInfo == nullptr) {
        return;
    }

    ExceptionSummary summary;
    summary.taskId = aclrtGetTaskIdFromExceptionInfo(exceptionInfo);
    summary.streamId = aclrtGetStreamIdFromExceptionInfo(exceptionInfo);
    summary.threadId = aclrtGetThreadIdFromExceptionInfo(exceptionInfo);
    summary.deviceId = aclrtGetDeviceIdFromExceptionInfo(exceptionInfo);
    summary.errorCode = aclrtGetErrorCodeFromExceptionInfo(exceptionInfo);
    (void)aclrtGetArgsFromExceptionInfo(exceptionInfo, &summary.devArgs, &summary.devArgsLen);

    {
        std::lock_guard<std::mutex> lock(g_exceptionMutex);
        g_lastException = summary;
    }
    uint64_t count = g_exceptionCount.fetch_add(1, std::memory_order_relaxed) + 1;
    g_exceptionPending.store(true, std::memory_order_release);
    std::printf("[hcomm_fault_runner] CustomCallback entered count=%llu task=%u stream=%u thread=%u device=%u "
                "error=%u devArgs=%p devArgsLen=%u\n",
        static_cast<unsigned long long>(count),
        summary.taskId,
        summary.streamId,
        summary.threadId,
        summary.deviceId,
        summary.errorCode,
        summary.devArgs,
        summary.devArgsLen);
    std::fflush(stdout);
}

template <typename T>
T ResolveAclRtSymbol(const char *symbol)
{
    dlerror();
    void *func = dlsym(RTLD_DEFAULT, symbol);
    const char *error = dlerror();
    if (error != nullptr || func == nullptr) {
        std::printf("[hcomm_fault_runner] optional runtime symbol %s unavailable: %s\n",
            symbol, error == nullptr ? "null" : error);
        std::fflush(stdout);
        return nullptr;
    }
    return reinterpret_cast<T>(func);
}

bool BindOneKernelExceptionArgs(const char *kernelName, const void *kernelSymbol, CommContext *devContext,
    AclrtGetFuncBySymbolFunc getFuncBySymbol, AclrtFunctionGetBinaryFunc functionGetBinary,
    AclrtBinarySetExceptionCallbackFunc binarySetExceptionCallback)
{
    aclrtFuncHandle funcHandle = nullptr;
    aclrtBinHandle binHandle = nullptr;
    aclError ret = getFuncBySymbol(kernelSymbol, &funcHandle);
    if (ret != ACL_SUCCESS || funcHandle == nullptr) {
        std::printf("[hcomm_fault_runner] aclrtGetFuncBySymbol %s failed, ret=%d func=%p\n",
            kernelName, ret, reinterpret_cast<void *>(funcHandle));
        std::fflush(stdout);
        return false;
    }

    ret = functionGetBinary(funcHandle, &binHandle);
    if (ret != ACL_SUCCESS || binHandle == nullptr) {
        std::printf("[hcomm_fault_runner] aclrtFunctionGetBinary %s failed, ret=%d bin=%p\n",
            kernelName, ret, reinterpret_cast<void *>(binHandle));
        std::fflush(stdout);
        return false;
    }

    ret = binarySetExceptionCallback(binHandle, CustomCallback, devContext);
    if (ret != ACL_SUCCESS) {
        std::printf("[hcomm_fault_runner] aclrtBinarySetExceptionCallback %s failed, ret=%d devContext=%p\n",
            kernelName, ret, devContext);
        std::fflush(stdout);
        return false;
    }

    std::printf("[hcomm_fault_runner] exception args bound for %s devContext=%p\n", kernelName, devContext);
    std::fflush(stdout);
    return true;
}

void TryBindFaultKernelExceptionArgs(CommContext *devContext)
{
    auto getFuncBySymbol = ResolveAclRtSymbol<AclrtGetFuncBySymbolFunc>("aclrtGetFuncBySymbol");
    auto functionGetBinary = ResolveAclRtSymbol<AclrtFunctionGetBinaryFunc>("aclrtFunctionGetBinary");
    auto binarySetExceptionCallback =
        ResolveAclRtSymbol<AclrtBinarySetExceptionCallbackFunc>("aclrtBinarySetExceptionCallback");
    if (getFuncBySymbol == nullptr || functionGetBinary == nullptr || binarySetExceptionCallback == nullptr) {
        std::printf("[hcomm_fault_runner] skip binary exception args binding; runtime binding API unavailable\n");
        std::fflush(stdout);
        return;
    }

    (void)BindOneKernelExceptionArgs("HcommUrmaTimeoutFaultKernel",
        reinterpret_cast<const void *>(HcommUrmaTimeoutFaultKernel), devContext, getFuncBySymbol, functionGetBinary,
        binarySetExceptionCallback);
    (void)BindOneKernelExceptionArgs("HcommCqeErrorFaultKernel",
        reinterpret_cast<const void *>(HcommCqeErrorFaultKernel), devContext, getFuncBySymbol, functionGetBinary,
        binarySetExceptionCallback);
}

bool TakePendingException(ExceptionSummary &summary, uint64_t &count)
{
    if (!g_exceptionPending.exchange(false, std::memory_order_acq_rel)) {
        return false;
    }
    std::lock_guard<std::mutex> lock(g_exceptionMutex);
    summary = g_lastException;
    count = g_exceptionCount.load(std::memory_order_relaxed);
    return true;
}

void PrintCommContext(const char *prefix, const CommContext &ctx)
{
    std::printf("%s rank=%u channel=0x%llx local=0x%llx remote=0x%llx len=%llu\n",
        prefix,
        ctx.rankId,
        static_cast<unsigned long long>(ctx.channelHandle),
        static_cast<unsigned long long>(ctx.localBufferAddr),
        static_cast<unsigned long long>(ctx.remoteBufferAddr),
        static_cast<unsigned long long>(ctx.len));
}

void CollectCachedContexts(uint32_t rank, std::vector<ChannelHandle> &channels, std::vector<CommContext> &contexts)
{
    std::lock_guard<std::mutex> lock(g_dumpMutex);
    for (const auto &ctx : g_contexts) {
        if (ctx.rankId != rank || ctx.channelHandle == 0) {
            continue;
        }
        contexts.push_back(ctx);
        channels.push_back(static_cast<ChannelHandle>(ctx.channelHandle));
    }
}

bool ReportPendingException(uint32_t rank)
{
    ExceptionSummary summary;
    uint64_t count = 0;
    if (!TakePendingException(summary, count)) {
        return false;
    }

    std::printf("[hcomm_fault_runner] runtime exception count=%llu task=%u stream=%u thread=%u device=%u error=%u\n",
        static_cast<unsigned long long>(count),
        summary.taskId,
        summary.streamId,
        summary.threadId,
        summary.deviceId,
        summary.errorCode);

    CommContext ctx = {};
    if (TryCopyCommContext(summary.devArgs, summary.devArgsLen, ctx) && ctx.channelHandle != 0) {
        PrintCommContext("[hcomm_fault_runner] exception ctx", ctx);
        ChannelHandle channel = static_cast<ChannelHandle>(ctx.channelHandle);
        DumpChannels({channel});
        return true;
    }

    std::vector<ChannelHandle> channels;
    std::vector<CommContext> contexts;
    CollectCachedContexts(rank, channels, contexts);
    for (const auto &cachedCtx : contexts) {
        PrintCommContext("[hcomm_fault_runner] cached ctx", cachedCtx);
    }
    std::printf("[hcomm_fault_runner] dump rank %u cached channels because exception dev args are unavailable.\n", rank);
    DumpChannels(channels);
    return true;
}

void DumpCachedChannelsForDiagnosis(uint32_t rank)
{
    std::vector<ChannelHandle> channels;
    std::vector<CommContext> contexts;
    CollectCachedContexts(rank, channels, contexts);
    std::printf("[hcomm_fault_runner] no pending exception after polling, dump rank %u cached channels for diagnosis. "
                "callbackCount=%llu channelNum=%zu\n",
        rank,
        static_cast<unsigned long long>(g_exceptionCount.load(std::memory_order_relaxed)),
        channels.size());
    for (const auto &cachedCtx : contexts) {
        PrintCommContext("[hcomm_fault_runner] cached ctx", cachedCtx);
    }
    DumpChannels(channels);
}

bool ParseFaultCase(int argc, char **argv, FaultCase &faultCase)
{
    faultCase = FaultCase::URMA_TIMEOUT;
    for (int idx = 1; idx < argc; ++idx) {
        std::string arg(argv[idx]);
        if (arg == "--case" && idx + 1 < argc) {
            std::string value(argv[++idx]);
            if (value == "urma" || value == "urma_timeout") {
                faultCase = FaultCase::URMA_TIMEOUT;
                continue;
            }
            if (value == "cqe" || value == "cqe_error") {
                faultCase = FaultCase::CQE_ERROR;
                continue;
            }
            std::printf("[hcomm_fault_runner] unsupported case: %s\n", value.c_str());
            return false;
        }
    }
    return true;
}

uint32_t ParseRankSize(int argc, char **argv, uint32_t devCount)
{
    uint32_t rankSize = std::min<uint32_t>(devCount, 2);
    for (int idx = 1; idx < argc; ++idx) {
        std::string arg(argv[idx]);
        if (arg == "--rank-size" && idx + 1 < argc) {
            rankSize = static_cast<uint32_t>(std::strtoul(argv[++idx], nullptr, 10));
        }
    }
    return rankSize;
}

HcclResult BuildUbcChannelDesc(HcclComm comm, uint32_t rank, uint32_t remoteRank, HcclMemHandle *memHandle,
    HcclChannelDesc &desc)
{
    uint32_t *netLayers = nullptr;
    uint32_t netLayerNum = 0;
    HcclResult ret = HcclRankGraphGetLayers(comm, &netLayers, &netLayerNum);
    if (ret != HCCL_SUCCESS) {
        return ret;
    }

    bool hasFallback = false;
    HcclChannelDesc fallbackDesc;
    for (uint32_t layerIdx = 0; layerIdx < netLayerNum; ++layerIdx) {
        CommLink *linkList = nullptr;
        uint32_t linkNum = 0;
        ret = HcclRankGraphGetLinks(comm, netLayers[layerIdx], rank, remoteRank, &linkList, &linkNum);
        if (ret != HCCL_SUCCESS) {
            return ret;
        }
        for (uint32_t linkIdx = 0; linkIdx < linkNum; ++linkIdx) {
            const CommLink &link = linkList[linkIdx];
            std::printf("[hcomm_fault_runner] rank=%u remote=%u layer=%u link=%u protocol=%d\n",
                rank,
                remoteRank,
                netLayers[layerIdx],
                linkIdx,
                static_cast<int>(link.linkAttr.linkProtocol));
            if (!hasFallback) {
                HcclChannelDescInit(&fallbackDesc, 1);
                fallbackDesc.remoteRank = remoteRank;
                fallbackDesc.channelProtocol = link.linkAttr.linkProtocol;
                fallbackDesc.localEndpoint = link.srcEndpointDesc;
                fallbackDesc.remoteEndpoint = link.dstEndpointDesc;
                fallbackDesc.notifyNum = 2;
                fallbackDesc.memHandles = memHandle;
                fallbackDesc.memHandleNum = 1;
                hasFallback = true;
            }
            if (link.linkAttr.linkProtocol != COMM_PROTOCOL_UBC_CTP) {
                continue;
            }
            HcclChannelDescInit(&desc, 1);
            desc.remoteRank = remoteRank;
            desc.channelProtocol = link.linkAttr.linkProtocol;
            desc.localEndpoint = link.srcEndpointDesc;
            desc.remoteEndpoint = link.dstEndpointDesc;
            desc.notifyNum = 2;
            desc.memHandles = memHandle;
            desc.memHandleNum = 1;
            return HCCL_SUCCESS;
        }
    }
    if (hasFallback) {
        desc = fallbackDesc;
        std::printf("[hcomm_fault_runner] fallback to first graph link, rank=%u remote=%u protocol=%d\n",
            rank, remoteRank, static_cast<int>(desc.channelProtocol));
        return HCCL_SUCCESS;
    }
    return HCCL_E_NOT_FOUND;
}

int AcquireLegalChannel(HcclComm comm, uint32_t rank, uint32_t remoteRank, HcclMemHandle *memHandle,
    ChannelHandle &channel, void *&remoteBuffer)
{
    HcclChannelDesc desc;
    HCCL_CHECK(BuildUbcChannelDesc(comm, rank, remoteRank, memHandle, desc));
    HCCL_CHECK(HcclChannelAcquire(comm, COMM_ENGINE_AIV, &desc, 1, &channel));

    uint32_t memNum = 0;
    CommMem *remoteMems = nullptr;
    char **memTags = nullptr;
    HCCL_CHECK(HcclChannelGetRemoteMems(comm, channel, &memNum, &remoteMems, &memTags));
    if (memNum == 0 || remoteMems == nullptr) {
        std::printf("[hcomm_fault_runner] no remote registered memory on channel 0x%llx\n",
            static_cast<unsigned long long>(channel));
        return static_cast<int>(HCCL_E_NOT_FOUND);
    }
    remoteBuffer = remoteMems[memNum - 1].addr;
    return 0;
}

struct RankContext {
    uint32_t rank;
    uint32_t rankSize;
    HcclRootInfo *rootInfo;
    FaultCase faultCase;
    ThreadBarrier *barrier;
};

int RunRank(RankContext *ctx)
{
    const uint32_t rank = ctx->rank;
    const uint32_t remoteRank = (rank + 1) % ctx->rankSize;
    const uint64_t bufferSize =
        ctx->faultCase == FaultCase::URMA_TIMEOUT ? HCOMM_URMA_TIMEOUT_TEST_LEN : HCOMM_CQE_ERROR_TEST_LEN;

    void *localBuffer = nullptr;
    CommContext *devContext = nullptr;
    HcclComm comm = nullptr;
    aclrtStream stream = nullptr;
    HcclMemHandle memHandle = nullptr;
    ChannelHandle channel = 0;
    void *remoteBuffer = nullptr;

    ACL_CHECK(aclrtSetDevice(static_cast<int32_t>(rank)));
    HCCL_CHECK(HcclCommInitRootInfo(ctx->rankSize, ctx->rootInfo, rank, &comm));
    ACL_CHECK(aclrtCreateStream(&stream));
    std::string memTag = "hcomm_exception_fault_rank_" + std::to_string(rank);
    HCCL_CHECK(HcclEngineCtxCreate(comm, memTag.c_str(), COMM_ENGINE_AIV, bufferSize, &localBuffer));
    ACL_CHECK(aclrtMemset(localBuffer, bufferSize, 0, bufferSize));

    CommMem regMem{COMM_MEM_TYPE_DEVICE, localBuffer, bufferSize};
    HCCL_CHECK(HcclCommMemReg(comm, memTag.c_str(), &regMem, &memHandle));

    ctx->barrier->Wait();
    int ret = AcquireLegalChannel(comm, rank, remoteRank, &memHandle, channel, remoteBuffer);
    if (ret != 0) {
        return ret;
    }
    {
        std::lock_guard<std::mutex> lock(g_dumpMutex);
        g_channels.push_back(channel);
    }
    std::printf("[hcomm_fault_runner] rank=%u channel=0x%llx local=%p remote=%p size=%llu\n",
        rank, static_cast<unsigned long long>(channel), localBuffer, remoteBuffer,
        static_cast<unsigned long long>(bufferSize));

    CommContext hostCtx = {};
    hostCtx.channelHandle = static_cast<uint64_t>(channel);
    hostCtx.localBufferAddr = reinterpret_cast<uint64_t>(localBuffer);
    hostCtx.remoteBufferAddr = reinterpret_cast<uint64_t>(remoteBuffer);
    hostCtx.len = bufferSize;
    hostCtx.invalidBufferAddr = HCOMM_INVALID_BUFFER_ADDR;
    hostCtx.rankId = rank;
    {
        std::lock_guard<std::mutex> lock(g_dumpMutex);
        g_contexts.push_back(hostCtx);
    }
    ACL_CHECK(aclrtMalloc(reinterpret_cast<void **>(&devContext), sizeof(CommContext), ACL_MEM_MALLOC_HUGE_ONLY));
    ACL_CHECK(aclrtMemcpy(devContext, sizeof(CommContext), &hostCtx, sizeof(hostCtx), ACL_MEMCPY_HOST_TO_DEVICE));
    if (rank == 0) {
        TryBindFaultKernelExceptionArgs(devContext);
    }

    ctx->barrier->Wait();
    if (rank == 0) {
        if (ctx->faultCase == FaultCase::URMA_TIMEOUT) {
            std::printf("[hcomm_fault_runner] launch URMA timeout fault kernel.\n");
            run_hcomm_urma_timeout_fault(devContext, stream);
        } else {
            std::printf("[hcomm_fault_runner] launch CQE error fault kernel.\n");
            run_hcomm_cqe_error_fault(devContext, stream);
        }
        aclError syncRet = aclrtSynchronizeStream(stream);
        std::printf("[hcomm_fault_runner] rank=0 aclrtSynchronizeStream ret=%d\n", syncRet);
        bool reported = false;
        for (uint32_t retry = 0; retry < 20; ++retry) {
            if (ReportPendingException(rank)) {
                reported = true;
                break;
            }
            usleep(100000);
        }
        if (!reported) {
            DumpCachedChannelsForDiagnosis(rank);
        }
    }
    ctx->barrier->Wait();

    if (devContext != nullptr) {
        ACL_CHECK(aclrtFree(devContext));
    }
    if (stream != nullptr) {
        ACL_CHECK(aclrtDestroyStream(stream));
    }
    if (comm != nullptr) {
        HCCL_CHECK(HcclCommDestroy(comm));
    }
    ACL_CHECK(aclrtResetDevice(static_cast<int32_t>(rank)));
    return 0;
}
} // namespace

int main(int argc, char **argv)
{
    FaultCase faultCase;
    if (!ParseFaultCase(argc, argv, faultCase)) {
        return 1;
    }

    ACL_CHECK(aclInit(nullptr));
    ACL_CHECK(aclrtSetExceptionInfoCallback(CustomCallback));
    std::printf("[hcomm_fault_runner] exception callback registered\n");
    std::fflush(stdout);

    uint32_t devCount = 0;
    ACL_CHECK(aclrtGetDeviceCount(&devCount));
    uint32_t rankSize = ParseRankSize(argc, argv, devCount);
    if (rankSize < 2 || rankSize > devCount) {
        std::printf("[hcomm_fault_runner] need at least 2 devices, devCount=%u rankSize=%u\n", devCount, rankSize);
        return 1;
    }

    ACL_CHECK(aclrtSetDevice(0));
    void *rootInfoBuf = nullptr;
    ACL_CHECK(aclrtMallocHost(&rootInfoBuf, sizeof(HcclRootInfo)));
    auto *rootInfo = reinterpret_cast<HcclRootInfo *>(rootInfoBuf);
    HCCL_CHECK(HcclGetRootInfo(rootInfo));

    ThreadBarrier barrier(rankSize);
    std::vector<RankContext> contexts(rankSize);
    std::vector<std::thread> threads;
    threads.reserve(rankSize);
    std::vector<int> results(rankSize, 0);
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        contexts[rank] = RankContext{rank, rankSize, rootInfo, faultCase, &barrier};
        threads.emplace_back([&, rank] { results[rank] = RunRank(&contexts[rank]); });
    }
    for (auto &thread : threads) {
        thread.join();
    }

    ACL_CHECK(aclrtFreeHost(rootInfoBuf));
    ACL_CHECK(aclFinalize());
    for (int result : results) {
        if (result != 0) {
            return result;
        }
    }
    return 0;
}
