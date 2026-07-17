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
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "acl/acl.h"
#include "acl/acl_rt.h"
#include "hccl/hccl.h"
#include "hccl/hccl_rank_graph.h"
#include "hccl/hccl_res.h"
#include "hcomm_exception_fault_cases.h"

extern void run_hcomm_urma_timeout_fault(CommContext *ctx, aclrtStream stream);
extern void run_hcomm_cqe_error_fault(CommContext *ctx, aclrtStream stream);

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
        return reinterpret_cast<HcommChannelInfoDumpFunc>(symbol);
    }();
    return dumpFunc;
}

void DumpChannels(const std::vector<ChannelHandle> &channels)
{
    auto dumpFunc = ResolveChannelDumpFunc();
    if (dumpFunc == nullptr || channels.empty()) {
        return;
    }
    std::vector<ChannelHandle> channelCopy = channels;
    dumpFunc(static_cast<uint32_t>(channelCopy.size()), channelCopy.data());
}

bool TryGetCommContext(aclrtExceptionInfo *exceptionInfo, CommContext &hostCtx)
{
    if (exceptionInfo == nullptr) {
        return false;
    }
    auto argAddr = exceptionInfo->expandInfo.aicoreInfo.exceptionArgs.argAddr;
    if (argAddr == nullptr) {
        return false;
    }
    aclError ret = aclrtMemcpy(&hostCtx, sizeof(hostCtx), argAddr, sizeof(CommContext), ACL_MEMCPY_DEVICE_TO_HOST);
    return ret == ACL_SUCCESS;
}

void CustomCallback(aclrtExceptionInfo *exceptionInfo)
{
    std::lock_guard<std::mutex> lock(g_dumpMutex);
    CommContext ctx = {};
    if (TryGetCommContext(exceptionInfo, ctx) && ctx.channelHandle != 0) {
        std::printf(
            "[hcomm_fault_runner] exception ctx rank=%u channel=0x%llx local=0x%llx remote=0x%llx len=%llu\n",
            ctx.rankId,
            static_cast<unsigned long long>(ctx.channelHandle),
            static_cast<unsigned long long>(ctx.localBufferAddr),
            static_cast<unsigned long long>(ctx.remoteBufferAddr),
            static_cast<unsigned long long>(ctx.len));
        ChannelHandle channel = static_cast<ChannelHandle>(ctx.channelHandle);
        DumpChannels({channel});
        return;
    }

    std::printf("[hcomm_fault_runner] exception argAddr is unavailable, dump cached channels.\n");
    DumpChannels(g_channels);
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

    for (uint32_t layerIdx = 0; layerIdx < netLayerNum; ++layerIdx) {
        CommLink *linkList = nullptr;
        uint32_t linkNum = 0;
        ret = HcclRankGraphGetLinks(comm, netLayers[layerIdx], rank, remoteRank, &linkList, &linkNum);
        if (ret != HCCL_SUCCESS) {
            return ret;
        }
        for (uint32_t linkIdx = 0; linkIdx < linkNum; ++linkIdx) {
            const CommLink &link = linkList[linkIdx];
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
    ACL_CHECK(aclrtMalloc(&localBuffer, bufferSize, ACL_MEM_MALLOC_HUGE_ONLY));
    ACL_CHECK(aclrtMemset(localBuffer, bufferSize, 0, bufferSize));

    CommMem regMem{COMM_MEM_TYPE_DEVICE, localBuffer, bufferSize};
    std::string memTag = "hcomm_exception_fault_rank_" + std::to_string(rank);
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
    ACL_CHECK(aclrtMalloc(reinterpret_cast<void **>(&devContext), sizeof(CommContext), ACL_MEM_MALLOC_HUGE_ONLY));
    ACL_CHECK(aclrtMemcpy(devContext, sizeof(CommContext), &hostCtx, sizeof(hostCtx), ACL_MEMCPY_HOST_TO_DEVICE));

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
    }
    ctx->barrier->Wait();

    if (devContext != nullptr) {
        ACL_CHECK(aclrtFree(devContext));
    }
    if (localBuffer != nullptr) {
        ACL_CHECK(aclrtFree(localBuffer));
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
