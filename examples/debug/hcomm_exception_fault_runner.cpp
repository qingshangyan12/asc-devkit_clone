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
#include "hcomm_channel_dump_info.h"
#include "hcomm_exception_fault_cases.h"

extern void run_hcomm_urma_timeout_fault(CommContext *ctx, aclrtStream stream);
extern void run_hcomm_cqe_error_fault(CommContext *ctx, aclrtStream stream);
extern "C" void HcommUrmaTimeoutFaultKernel(CommContext *ctx);
extern "C" void HcommCqeErrorFaultKernel(CommContext *ctx);

namespace {
using HcommChannelGetDumpInfoSizeFunc = int32_t (*)(uint32_t, ChannelHandle *, uint32_t *);
using HcommChannelDumpInfoFunc = int32_t (*)(uint32_t, ChannelHandle *, void *, uint32_t);

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

void *ResolveChannelDumpSymbol(const char *symbolName)
{
    static void *handle = []() -> void * {
        const char *soPath = std::getenv("HCOMM_CHANNEL_DUMP_SO");
        if (soPath == nullptr || soPath[0] == '\0') {
            soPath = "libhcomm_channel_dump.so";
        }

        void *handle = dlopen(soPath, RTLD_NOW | RTLD_LOCAL);
        if (handle == nullptr) {
            std::printf("[hcomm_fault_runner] dlopen %s failed: %s\n", soPath, dlerror());
            return nullptr;
        }
        std::printf("[hcomm_fault_runner] loaded HCOMM dump library from %s\n", soPath);
        std::fflush(stdout);
        return handle;
    }();
    if (handle == nullptr) {
        return nullptr;
    }

    dlerror();
    void *symbol = dlsym(handle, symbolName);
    const char *error = dlerror();
    if (error != nullptr) {
        std::printf("[hcomm_fault_runner] dlsym %s failed: %s\n", symbolName, error);
        std::fflush(stdout);
        return nullptr;
    }
    return symbol;
}

HcommChannelGetDumpInfoSizeFunc ResolveChannelGetDumpInfoSizeFunc()
{
    static HcommChannelGetDumpInfoSizeFunc getSizeFunc =
        reinterpret_cast<HcommChannelGetDumpInfoSizeFunc>(ResolveChannelDumpSymbol("HcommChannelGetDumpInfoSize"));
    return getSizeFunc;
}

HcommChannelDumpInfoFunc ResolveChannelDumpInfoFunc()
{
    static HcommChannelDumpInfoFunc dumpInfoFunc =
        reinterpret_cast<HcommChannelDumpInfoFunc>(ResolveChannelDumpSymbol("HcommChannelDumpInfo"));
    return dumpInfoFunc;
}

struct UrmaSqeHeader {
    uint32_t dw0;
    uint32_t dw1;
    uint32_t dw2;
    uint32_t dw3;
    uint64_t rmtEidL;
    uint64_t rmtEidH;
    uint32_t rmtTokenValue;
    uint32_t dw4;
    uint32_t rmtAddrLOrTokenId;
    uint32_t rmtAddrHOrTokenValue;
};

struct UrmaSgeCtx {
    uint32_t len;
    uint32_t tokenId;
    uint64_t va;
};

struct UrmaNotifyCtx {
    uint32_t dw0;
    uint32_t notifyTokenValue;
    uint32_t notifyAddrL;
    uint32_t notifyAddrH;
    uint32_t notifyDataL;
    uint32_t notifyDataH;
    uint32_t rsv2[2];
};

struct UrmaCqeCtx {
    uint32_t dw0;
    uint32_t dw1;
    uint32_t dw2;
    uint32_t dw3;
    uint32_t byteCnt;
    uint32_t userDataL;
    uint32_t userDataH;
    uint32_t rmtEid[4];
    uint32_t dataL;
    uint32_t dataH;
    uint32_t inlineData[3];
};

uint32_t Bits(uint32_t value, uint32_t shift, uint32_t mask)
{
    return (value >> shift) & mask;
}

template <typename T>
bool ReadEntryObject(const HcommChannelDumpQueueEntry &entry, uint32_t offset, T &object)
{
    if (offset > entry.size || entry.size - offset < sizeof(T)) {
        return false;
    }
    std::memcpy(&object, entry.data + offset, sizeof(T));
    return true;
}

void PrintRawBytes(const char *name, const HcommChannelDumpQueueEntry &entry)
{
    constexpr uint32_t bytesPerLine = 16U;
    for (uint32_t offset = 0; offset < entry.size; offset += bytesPerLine) {
        uint32_t lineSize = std::min<uint32_t>(bytesPerLine, entry.size - offset);
        std::printf("[hcomm_fault_runner][parsed] %s raw[%03u]", name, offset);
        for (uint32_t idx = 0; idx < lineSize; ++idx) {
            std::printf(" %02x", entry.data[offset + idx]);
        }
        std::printf("\n");
    }
}

void PrintParsedNotify(const UrmaNotifyCtx &notify)
{
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA WQE: notifyTokenId:%x notifyTokenValue:%x "
                "notifyAddrL:%x notifyAddrH:%x notifyDataL:%x notifyDataH:%x\n",
        Bits(notify.dw0, 0, 0xFFFFFU), notify.notifyTokenValue, notify.notifyAddrL, notify.notifyAddrH,
        notify.notifyDataL, notify.notifyDataH);
}

void PrintParsedSge(const HcommChannelDumpQueueEntry &entry, uint32_t offset, uint32_t sgeNum)
{
    constexpr uint32_t maxPrintedSge = 8U;
    uint32_t printNum = std::min<uint32_t>(sgeNum, maxPrintedSge);
    for (uint32_t idx = 0; idx < printNum; ++idx) {
        UrmaSgeCtx sge = {};
        if (!ReadEntryObject(entry, offset + idx * sizeof(UrmaSgeCtx), sge)) {
            std::printf("[hcomm_fault_runner][parsed] Hcomm URMA SGE: truncated at idx:%u offset:%u size:%u\n",
                idx, offset + idx * static_cast<uint32_t>(sizeof(UrmaSgeCtx)), entry.size);
            return;
        }
        std::printf("[hcomm_fault_runner][parsed] Hcomm URMA SGE: sge idx:%u va:0x%llx sge_len:%u tokenId:%u\n",
            idx, static_cast<unsigned long long>(sge.va), sge.len, sge.tokenId);
    }
    if (sgeNum > maxPrintedSge) {
        std::printf("[hcomm_fault_runner][parsed] Hcomm URMA SGE: skipped %u entries after max printed entries\n",
            sgeNum - maxPrintedSge);
    }
}

void PrintParsedAmo(const HcommChannelDumpQueueEntry &entry, uint32_t offset, uint32_t opcode)
{
    constexpr uint32_t opcodeCas = 7U;
    constexpr uint32_t opcodeFaa = 0xBU;
    if (opcode == opcodeFaa) {
        uint64_t addValue = 0;
        if (ReadEntryObject(entry, offset, addValue)) {
            std::printf("[hcomm_fault_runner][parsed] Hcomm URMA SGE: addValue:0x%llx\n",
                static_cast<unsigned long long>(addValue));
        }
    } else if (opcode == opcodeCas) {
        uint64_t swapValue = 0;
        uint64_t condValue = 0;
        if (ReadEntryObject(entry, offset, swapValue) && ReadEntryObject(entry, offset + sizeof(uint64_t), condValue)) {
            std::printf("[hcomm_fault_runner][parsed] Hcomm URMA SGE: condValue:0x%llx swapValue:0x%llx\n",
                static_cast<unsigned long long>(condValue), static_cast<unsigned long long>(swapValue));
        }
    }
}

void PrintParsedWqe(const HcommChannelDumpQueueEntry &entry)
{
    if (entry.size < sizeof(UrmaSqeHeader)) {
        return;
    }
    UrmaSqeHeader sqe = {};
    std::memcpy(&sqe, entry.data, sizeof(sqe));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA WQE: queue=%u slot=%u cursor=%u addr=0x%llx\n",
        entry.queueIndex, entry.entryIndex, entry.cursor, static_cast<unsigned long long>(entry.addr));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA WQE: sqe_bb_idx:%x flag:%x rsv0:%x nf:%x "
                "token_en:%x rmt_jetty_type:%x\n",
        Bits(sqe.dw0, 0, 0xFFFFU), Bits(sqe.dw0, 16, 0xFFU), Bits(sqe.dw0, 24, 0x7U),
        Bits(sqe.dw0, 27, 0x1U), Bits(sqe.dw0, 28, 0x1U), Bits(sqe.dw0, 29, 0x3U));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA WQE: owner:%x target_hint:%x opcode:%x rsv1:%x "
                "inline_msg_len:%x tp_id:%x\n",
        Bits(sqe.dw0, 31, 0x1U), Bits(sqe.dw1, 0, 0xFFU), Bits(sqe.dw1, 8, 0xFFU),
        Bits(sqe.dw1, 16, 0x3FU), Bits(sqe.dw1, 22, 0x3FFU), Bits(sqe.dw2, 0, 0xFFFFFFU));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA WQE: sge_num:%x rmt_jetty_or_seg_id:%x rsv2:%x\n",
        Bits(sqe.dw2, 24, 0xFFU), Bits(sqe.dw3, 0, 0xFFFFFU), Bits(sqe.dw3, 20, 0xFFFU));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA WQE: rmt_eid:0x%llx, 0x%llx\n",
        static_cast<unsigned long long>(sqe.rmtEidL), static_cast<unsigned long long>(sqe.rmtEidH));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA WQE: rmt_token_value:%x udf_type:%x "
                "reduce_data_type:%x reduce_opcode:%x\n",
        sqe.rmtTokenValue, Bits(sqe.dw4, 0, 0xFFU), Bits(sqe.dw4, 8, 0xFU), Bits(sqe.dw4, 12, 0xFU));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA WQE: rmt_addr_l_or_token_id:%x "
                "rmt_addr_h_or_token_value:%x\n",
        sqe.rmtAddrLOrTokenId, sqe.rmtAddrHOrTokenValue);

    constexpr uint32_t opcodeWriteWithNotify = 5U;
    uint32_t opcode = Bits(sqe.dw1, 8, 0xFFU);
    uint32_t sgeNum = Bits(sqe.dw2, 24, 0xFFU);
    uint32_t sgeOffset = sizeof(UrmaSqeHeader);
    if (opcode == opcodeWriteWithNotify) {
        UrmaNotifyCtx notify = {};
        if (ReadEntryObject(entry, sgeOffset, notify)) {
            PrintParsedNotify(notify);
            sgeOffset += sizeof(UrmaNotifyCtx);
        } else {
            std::printf("[hcomm_fault_runner][parsed] Hcomm URMA WQE: notify truncated offset:%u size:%u\n",
                sgeOffset, entry.size);
            return;
        }
    }
    PrintParsedSge(entry, sgeOffset, sgeNum);
    PrintParsedAmo(entry, sgeOffset + sgeNum * static_cast<uint32_t>(sizeof(UrmaSgeCtx)), opcode);
    PrintRawBytes("Hcomm URMA WQE", entry);
}

void PrintParsedCqe(const HcommChannelDumpQueueEntry &entry)
{
    if (entry.size < sizeof(UrmaCqeCtx)) {
        return;
    }
    UrmaCqeCtx cqe = {};
    std::memcpy(&cqe, entry.data, sizeof(cqe));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA CQE: queue=%u index=%u cursor=%u addr=0x%llx\n",
        entry.queueIndex, entry.entryIndex, entry.cursor, static_cast<unsigned long long>(entry.addr));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA CQE: DW0 - sR:%u isJetty:%u owner:%u inlineEn:%u "
                "opcode:%u fd:%u substatus:%u status:%u\n",
        Bits(cqe.dw0, 0, 0x1U), Bits(cqe.dw0, 1, 0x1U), Bits(cqe.dw0, 2, 0x1U), Bits(cqe.dw0, 3, 0x1U),
        Bits(cqe.dw0, 4, 0x7U), Bits(cqe.dw0, 7, 0x1U), Bits(cqe.dw0, 16, 0xFFU), Bits(cqe.dw0, 24, 0xFFU));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA CQE: DW1 - entryIdx:%u localNumL:%u\n",
        Bits(cqe.dw1, 0, 0xFFFFU), Bits(cqe.dw1, 16, 0xFFFFU));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA CQE: DW2 - localNumH:%u rmtIdx:%u\n",
        Bits(cqe.dw2, 0, 0xFU), Bits(cqe.dw2, 4, 0xFFFFFU));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA CQE: DW3 - tpn:%u\n", Bits(cqe.dw3, 0, 0xFFFFFFU));
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA CQE: DW4 - byteCnt:%u\n", cqe.byteCnt);
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA CQE: DW5-DW6 - userData:0x%x%x\n",
        cqe.userDataH, cqe.userDataL);
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA CQE: DW7-DW10 - rmtEid:[0x%x, 0x%x, 0x%x, 0x%x]\n",
        cqe.rmtEid[0], cqe.rmtEid[1], cqe.rmtEid[2], cqe.rmtEid[3]);
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA CQE: DW11-DW12 - data:0x%x%x\n", cqe.dataH, cqe.dataL);
    std::printf("[hcomm_fault_runner][parsed] Hcomm URMA CQE: DW13-DW15 - inlineData:[0x%x, 0x%x, 0x%x]\n",
        cqe.inlineData[0], cqe.inlineData[1], cqe.inlineData[2]);
    PrintRawBytes("Hcomm URMA CQE", entry);
}

void PrintParsedDumpInfo(const HcommChannelDumpData &dumpInfo)
{
    if (dumpInfo.magic != HCOMM_CHANNEL_DUMP_MAGIC) {
        std::printf("[hcomm_fault_runner][parsed] invalid dump info magic=0x%x\n", dumpInfo.magic);
        return;
    }
    for (uint32_t ch = 0; ch < dumpInfo.channelNum && ch < HCOMM_CHANNEL_DUMP_MAX_CHANNELS; ++ch) {
        const auto &channel = dumpInfo.channels[ch];
        if (channel.valid == 0) {
            continue;
        }
        std::printf("[hcomm_fault_runner][parsed] Channel[%u] handle=0x%llx engine=%d protocol=%d "
                    "localBufferNum=%u remoteBufferNum=%u sqNum=%u cqNum=%u wqeCnt=%u\n",
            ch, static_cast<unsigned long long>(channel.handle), channel.engine, channel.protocol,
            channel.localBufferNum, channel.remoteBufferNum, channel.sqNum, channel.cqNum, channel.wqeCnt);
        for (uint32_t idx = 0; idx < channel.localBufferDumpNum; ++idx) {
            const auto &buffer = channel.localBuffers[idx];
            std::printf("[hcomm_fault_runner][parsed] localBuffer[%u] type=%d addr=0x%llx size=%llu\n",
                idx, buffer.type, static_cast<unsigned long long>(buffer.addr),
                static_cast<unsigned long long>(buffer.size));
        }
        for (uint32_t idx = 0; idx < channel.remoteBufferDumpNum; ++idx) {
            const auto &buffer = channel.remoteBuffers[idx];
            std::printf("[hcomm_fault_runner][parsed] remoteBuffer[%u] type=%d addr=0x%llx size=%llu\n",
                idx, buffer.type, static_cast<unsigned long long>(buffer.addr),
                static_cast<unsigned long long>(buffer.size));
        }
        for (uint32_t idx = 0; idx < channel.sqContextDumpNum; ++idx) {
            const auto &sq = channel.sqContexts[idx];
            std::printf("[hcomm_fault_runner][parsed] SqContext[%u] type=%d sqVa=0x%llx depth=%u entrySize=%u "
                        "head=%u%s tail=%u%s dbVa=0x%llx\n",
                idx, sq.type, static_cast<unsigned long long>(sq.va), sq.depth, sq.entrySize, sq.head,
                sq.headValid ? "" : "(invalid)", sq.tail, sq.tailValid ? "" : "(invalid)",
                static_cast<unsigned long long>(sq.dbVa));
        }
        for (uint32_t idx = 0; idx < channel.cqContextDumpNum; ++idx) {
            const auto &cq = channel.cqContexts[idx];
            std::printf("[hcomm_fault_runner][parsed] CqContext[%u] type=%d cqVa=0x%llx depth=%u entrySize=%u "
                        "head=%u%s tail=%u%s dbVa=0x%llx\n",
                idx, cq.type, static_cast<unsigned long long>(cq.va), cq.depth, cq.entrySize, cq.head,
                cq.headValid ? "" : "(invalid)", cq.tail, cq.tailValid ? "" : "(invalid)",
                static_cast<unsigned long long>(cq.dbVa));
        }
        for (uint32_t idx = 0; idx < channel.queueEntryDumpNum; ++idx) {
            const auto &entry = channel.queueEntries[idx];
            if (entry.valid == 0) {
                continue;
            }
            if (entry.entryType == HCOMM_CHANNEL_DUMP_ENTRY_URMA_WQE) {
                PrintParsedWqe(entry);
            } else if (entry.entryType == HCOMM_CHANNEL_DUMP_ENTRY_URMA_CQE) {
                PrintParsedCqe(entry);
            }
        }
    }
}

bool DumpChannelsByInfoApi(const std::vector<ChannelHandle> &channels)
{
    auto getSizeFunc = ResolveChannelGetDumpInfoSizeFunc();
    auto dumpInfoFunc = ResolveChannelDumpInfoFunc();
    if (getSizeFunc == nullptr || dumpInfoFunc == nullptr || channels.empty()) {
        return false;
    }
    std::vector<ChannelHandle> channelCopy = channels;
    uint32_t len = 0;
    int32_t ret = getSizeFunc(static_cast<uint32_t>(channelCopy.size()), channelCopy.data(), &len);
    if (ret != 0 || len < sizeof(HcommChannelDumpData)) {
        std::printf("[hcomm_fault_runner] HcommChannelGetDumpInfoSize failed ret=%d len=%u\n", ret, len);
        return false;
    }
    std::vector<uint8_t> buffer(len, 0);
    ret = dumpInfoFunc(static_cast<uint32_t>(channelCopy.size()), channelCopy.data(), buffer.data(), len);
    std::printf("[hcomm_fault_runner] HcommChannelDumpInfo ret=%d len=%u\n", ret, len);
    if (ret != 0) {
        return false;
    }
    PrintParsedDumpInfo(*reinterpret_cast<const HcommChannelDumpData *>(buffer.data()));
    std::fflush(stdout);
    return true;
}

void DumpChannels(const std::vector<ChannelHandle> &channels)
{
    (void)DumpChannelsByInfoApi(channels);
}

bool TryCopyCommContext(void *devArgs, uint32_t devArgsLen, CommContext &hostCtx)
{
    if (devArgs == nullptr) {
        std::printf("[hcomm_fault_runner] exception dev args are unavailable, len=%u\n", devArgsLen);
        return false;
    }

    void *ctxAddr = devArgs;
    if (devArgsLen == sizeof(uint64_t)) {
        uint64_t rawCtxAddr = 0;
        aclError ret = aclrtMemcpy(&rawCtxAddr, sizeof(rawCtxAddr), devArgs, sizeof(rawCtxAddr),
            ACL_MEMCPY_DEVICE_TO_HOST);
        if (ret != ACL_SUCCESS) {
            std::printf("[hcomm_fault_runner] copy exception dev args pointer failed, ret=%d\n", ret);
            return false;
        }
        ctxAddr = reinterpret_cast<void *>(rawCtxAddr);
        std::printf("[hcomm_fault_runner] exception dev args point to CommContext device addr=%p\n", ctxAddr);
    } else if (devArgsLen < sizeof(CommContext)) {
        std::printf("[hcomm_fault_runner] exception dev args len is too small, len=%u expect=%zu\n",
            devArgsLen, sizeof(CommContext));
        return false;
    }

    if (ctxAddr == nullptr) {
        std::printf("[hcomm_fault_runner] exception CommContext device addr is null\n");
        return false;
    }
    aclError ret = aclrtMemcpy(&hostCtx, sizeof(hostCtx), ctxAddr, sizeof(CommContext), ACL_MEMCPY_DEVICE_TO_HOST);
    if (ret != ACL_SUCCESS) {
        std::printf("[hcomm_fault_runner] copy CommContext failed at report point, ret=%d ctxAddr=%p\n",
            ret, ctxAddr);
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
    std::printf("%s rank=%u channel=0x%llx local=0x%llx remote=0x%llx len=%llu writeRet=%d drainRet=%d\n",
        prefix,
        ctx.rankId,
        static_cast<unsigned long long>(ctx.channelHandle),
        static_cast<unsigned long long>(ctx.localBufferAddr),
        static_cast<unsigned long long>(ctx.remoteBufferAddr),
        static_cast<unsigned long long>(ctx.len),
        ctx.writeRet,
        ctx.drainRet);
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
