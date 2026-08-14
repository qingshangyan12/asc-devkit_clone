/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file aicpu_dump.cpp
 * \brief
 */

#include <stdarg.h>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <time.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "securec.h"
#include "aicpu_api/aicpu_api.h"

struct DumpConfig {
    void* dumpBufAddr;
    size_t dumpBufSize;
    size_t dumpOffset;
};

std::mutex dump_mutex;
extern "C" {
DumpConfig g_aicpuDumpConfig = {reinterpret_cast<void*>(0x8), 0x100000, 0x0};
}

/* ── rts interface (weak symbol provided by rts) ────────────────────── */

extern "C" {
int32_t __attribute__((weak)) AicpuGetDfxInfo(uint64_t *infoAddr);
}

namespace AscendC {

/* ── Constants ──────────────────────────────────────────────────────── */

constexpr uint32_t ATTR_ID_MEM_INFO = 1U;
constexpr uint32_t PRINT_TYPE_WRITING = 0xF0A55A0FU;
constexpr uint32_t PRINT_TYPE_READY = 11U; /* DUMP_AICPU */
constexpr uint32_t FIFO_ALIGN = 8U;
constexpr uint64_t FIFO_WAIT_INTERVAL_NS = 300000000ULL; /* 300ms */
constexpr uint32_t FIFO_MAX_WAIT_ITERS = 15U;

/* ── FIFO protocol structures ───────────────────────────────────────── */

struct RingBufCookie {
    uint8_t* ringBase;      /* base address of the ring buffer data area */
    uint32_t ringBufLen;    /* total size of the ring buffer data area    */
    uint32_t writeOffset;   /* current write offset (wraps mod ringBufLen) */
    uint32_t bytesWritten;  /* total bytes written so far                 */
};

struct AicpuPrintfMemInfo {
    uint64_t printfMemAddr = 0U;
    uint32_t printfMemSize = 0U;
    uint32_t resv0 = 0U;
};

union AicpuDfxAttrValue {
    uint8_t resv[64U] = {0U};
    AicpuPrintfMemInfo printfMemInfo;
};

struct AicpuDfxAttrInfo {
    uint32_t attrId = 0U;
    AicpuDfxAttrValue value;
};

struct AicpuDfxInfo {
    uint64_t attrs = 0U;
    uint64_t numAttrs = 0U;
};

struct BlockRingBufInfo {
    uint32_t length = 0U;       // total size per block (include head and r/w info)
    uint32_t resv0 = 0U;        // resv
    uint32_t resv1 = 0U;        // resv
    uint32_t ringBufLen = 0U;   // fifo buff size (print tlv storage)
    uint16_t magic = 0U;        // magic number for info verify, expected to be 0xAE86
    uint16_t flag = 0U;         // 0: simd, 1: simt, 3: aicpu
    uint32_t resv2 = 0U;          // reserve
    uint64_t ringBufAddr = 0U;  // start addr of fifo buff
    uint32_t resvMem[6];        // reserved
};

struct RingBufWriteInfo {
    uint32_t type = static_cast<uint32_t>(DumpType::DUMP_BUFI); // DumpType = DUMP_BUFI
    uint32_t length = 0U;                                       // u64 + u64
    uint64_t bufOffset = 0U;                                    // the offset of write addr relative to ringBufAddr
    uint64_t packIdx = 0U;                                      // print pack counter
};

struct RingBufReadInfo {
    uint32_t type = static_cast<uint32_t>(DumpType::DUMP_BUFO); // DumpType = DUMP_BUFO
    uint32_t length = 0U;                                       // u64 + u64
    uint64_t bufOffset = 0U;                                    // the offset of read addr relative to ringBufAddr
    uint64_t resv = 0U;
};

struct PrintfInfo {
    uint32_t printfType;             // DumpType = DUMP_AICPU
    uint32_t printfLength;           // length of print info
    uint32_t blockIdx = 0U;           // threadIdx
    uint32_t resv = 0U;               // reserved
    uint64_t fmtOffset = 0U;          // offset of fmt string from the start of fmtOffset addr 
};

/* ── Mutex for multi-thread safety ──────────────────────────────────── */

std::mutex g_aicpuFifoMutex;

/* ── set debug flag in .ascend.meta ──────────────────────────────────── */

struct AscAicpuTlvHead { // TLV Head include type and length
    uint16_t type;
    uint16_t len;
};

struct AscAicpuTlv {
    AscAicpuTlvHead head;
    uint32_t value; // feature: PRINT = 1, FFTS = 2, L2CACHE = 3
};

inline void enable_asc_aicpu_fifo_debug()
{
#if (!defined(ASCENDC_DUMP) || (ASCENDC_DUMP != 0))
    static const struct AscAicpuTlv __asc_aicpu_meta_section__ __attribute__((used, section(".ascend.meta"))) = {4, 4, 6};
#endif // (!defined(ASCENDC_DUMP) || (ASCENDC_DUMP != 0))
}

/* ── FIFO memory retrieval ──────────────────────────────────────────── */

inline int32_t GetAscAicpuPrintfMemInfo(AicpuPrintfMemInfo& info)
{
    if (AicpuGetDfxInfo == nullptr) {
        return -1;
    }

    uint64_t dfxInfoAddr = 0U;
    if (AicpuGetDfxInfo(&dfxInfoAddr) != 0) {
        return -1;
    }

    auto* dfxInfo = reinterpret_cast<AicpuDfxInfo*>(dfxInfoAddr);
    if (dfxInfo == nullptr || dfxInfo->attrs == 0U || dfxInfo->numAttrs == 0U) {
        return -1;
    }

    auto* attrBase = reinterpret_cast<AicpuDfxAttrInfo*>(dfxInfo->attrs);
    for (uint64_t i = 0U; i < dfxInfo->numAttrs; ++i) {
        if (attrBase[i].attrId == ATTR_ID_MEM_INFO) {
            info = attrBase[i].value.printfMemInfo;
            if (info.printfMemAddr != 0U) {
                return 0;
            }
        }
    }
    return -1;
}

inline BlockRingBufInfo* GetAicpuDebugBlockInfo(AicpuPrintfMemInfo& memInfo)
{
    if (memInfo.printfMemAddr == 0U) {
        return nullptr;
    }
    auto* blockInfo = reinterpret_cast<BlockRingBufInfo*>(memInfo.printfMemAddr);
    if (blockInfo->magic != 0xAE86U) {
        return nullptr;
    }
    return blockInfo;
}

inline RingBufReadInfo* GetRingBufReadInfo(BlockRingBufInfo* blockInfo)
{
    auto* base = reinterpret_cast<uint8_t*>(blockInfo);
    return reinterpret_cast<RingBufReadInfo*>(base + sizeof(BlockRingBufInfo));
}

inline RingBufWriteInfo* GetRingBufWriteInfo(BlockRingBufInfo* blockInfo)
{
    auto* ringBase = reinterpret_cast<uint8_t*>(blockInfo->ringBufAddr);
    return reinterpret_cast<RingBufWriteInfo*>(ringBase + blockInfo->ringBufLen);
}

inline uint32_t CalcFormattedLen(const char* fmt, va_list args)
{
    va_list argsCopy;
    va_copy(argsCopy, args);
    int32_t len = vsnprintf(nullptr, 0, fmt, argsCopy);
    va_end(argsCopy);
    return (len > 0) ? static_cast<uint32_t>(len) : 0U;
}

inline uint32_t CalcTlvLen(uint32_t strLen)
{
    uint32_t total = sizeof(PrintfInfo) + strLen + 1U; /* +1 for NUL */
    total = (total + FIFO_ALIGN - 1U) & ~(FIFO_ALIGN - 1U);
    return total;
}

inline uint64_t GetFreeRingBufLen(uint64_t rOff, uint64_t wOff, uint32_t ringBufLen)
{
    if (wOff == rOff) {
        return ringBufLen;
    } else if (wOff > rOff) {
        return (ringBufLen - wOff) + rOff;
    } else {
        return rOff - wOff;
    }
}

inline bool AicpuRingbufWait(RingBufReadInfo* readInfo, RingBufWriteInfo* writeInfo,
                             uint32_t tlvLen, uint32_t ringBufLen)
{
    struct timespec ts;
    ts.tv_sec  = static_cast<time_t>(FIFO_WAIT_INTERVAL_NS / 1000000000ULL);
    ts.tv_nsec = static_cast<long>(FIFO_WAIT_INTERVAL_NS % 1000000000ULL);

    for (uint32_t i = 0U; i < FIFO_MAX_WAIT_ITERS; ++i) {
        if (GetFreeRingBufLen(readInfo->bufOffset, writeInfo->bufOffset, ringBufLen) > tlvLen) {
            return true;
        }
        /* Sleep 300ms to let rts reader make progress. */
        (void)nanosleep(&ts, nullptr);
        __atomic_thread_fence(__ATOMIC_SEQ_CST); // TODO 刷新缓存，实际写法应该是什么
    }
    return false;
}

inline bool CheckAicpuRingBuf(BlockRingBufInfo* blockInfo, uint32_t tlvLen)
{
    const uint32_t ringBufLen = blockInfo->ringBufLen;
    RingBufReadInfo* readInfo = GetRingBufReadInfo(blockInfo);
    RingBufWriteInfo* writeInfo = GetRingBufWriteInfo(blockInfo);

    if (tlvLen > ringBufLen) {
        return false;
    }
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    if (!AicpuRingbufWait(readInfo, writeInfo, tlvLen, ringBufLen)) {
        return false;
    }

    return true;
}

static ssize_t RingBufCookieWrite(void* cookie, const char* buf, size_t size)
{
    auto* c = static_cast<RingBufCookie*>(cookie);
    uint32_t off = c->writeOffset;
    uint32_t tailSpace = c->ringBufLen - off;

    if (size <= tailSpace) {
        /* Fits in the tail — single memcpy. */
        (void)memcpy(c->ringBase + off, buf, size);
        c->writeOffset = off + static_cast<uint32_t>(size);
    } else {
        /* Spans tail and head — two memcpys. */
        uint32_t part1 = tailSpace;
        uint32_t part2 = static_cast<uint32_t>(size) - part1;
        (void)memcpy(c->ringBase + off, buf, part1);
        (void)memcpy(c->ringBase, buf + part1, part2);
        c->writeOffset = part2;
    }
    c->bytesWritten += static_cast<uint32_t>(size);
    return static_cast<ssize_t>(size);
}

inline FILE* RingBufOpenStream(RingBufCookie* cookie)
{
    cookie_io_functions_t hooks = {};
    hooks.write = RingBufCookieWrite;
    return fopencookie(cookie, "w", hooks);
}

/* ── Legacy printf (linear buffer) ──────────────────────────────────── */

static void PrintfLegacy(const char* fmt, va_list otherArgs)
{
    std::lock_guard<std::mutex> lock(dump_mutex);
    void* aicpuDumpBuffer = g_aicpuDumpConfig.dumpBufAddr;
    size_t aicpuDumpBufSize = g_aicpuDumpConfig.dumpBufSize;
    size_t aicpuDumpOffSet = 0;
    if (aicpuDumpBuffer != nullptr) {
        aicpuDumpOffSet = *reinterpret_cast<size_t*>(aicpuDumpBuffer);
    } else {
        return;
    }
    if (aicpuDumpOffSet >= aicpuDumpBufSize) {
        return;
    }
    aicpuDumpOffSet += static_cast<size_t>(vsnprintf_s(
        reinterpret_cast<char*>(aicpuDumpBuffer) + aicpuDumpOffSet,
        aicpuDumpBufSize - aicpuDumpOffSet,
        aicpuDumpBufSize - aicpuDumpOffSet, fmt, otherArgs));
    *reinterpret_cast<size_t*>(g_aicpuDumpConfig.dumpBufAddr) = aicpuDumpOffSet;
}

/* ── FIFO printf ────────────────────────────────────────────────────── */

static void PrintfFifo(const char* fmt, va_list otherArgs)
{
    AicpuPrintfMemInfo memInfo;
    if (GetAscAicpuPrintfMemInfo(memInfo) != 0) {
        PrintfLegacy(fmt, otherArgs);
        return;
    }

    enable_asc_aicpu_fifo_debug();
    BlockRingBufInfo* blockInfo = GetAicpuDebugBlockInfo(memInfo);
    if (blockInfo == nullptr) {
        return;
    }

    const uint32_t ringBufLen = blockInfo->ringBufLen;
    uint8_t* ringBase = reinterpret_cast<uint8_t*>(blockInfo->ringBufAddr);
    if (ringBase == nullptr || ringBufLen == 0U) {
        return;
    }

    // 1. calculate write info length and check whether there is enough free space
    uint32_t strLen = CalcFormattedLen(fmt, otherArgs);
    uint32_t tlvLen = CalcTlvLen(strLen);
    /* Lock the whole reservation + write to guarantee atomicity. */
    std::lock_guard<std::mutex> lock(g_aicpuFifoMutex);

    if (!CheckAicpuRingBuf(blockInfo, tlvLen)) {
        // TODO 是否涉及解锁
        return; /* drop on overflow / timeout */
    }
    // 2. atomically reserve space.
    RingBufWriteInfo* writeInfo = GetRingBufWriteInfo(blockInfo);
    uint64_t wOff = writeInfo->bufOffset;
    uint32_t tailSpace = ringBufLen - static_cast<uint32_t>(wOff);
    bool needSplit = (tlvLen > tailSpace);
    uint64_t newBufOff;
    if (needSplit) {
        newBufOff = static_cast<uint64_t>(tlvLen) - tailSpace;
    } else {
        newBufOff = wOff + tlvLen;
    }
    __atomic_store_n(reinterpret_cast<uint64_t*>(&writeInfo->bufOffset),
                     newBufOff, __ATOMIC_SEQ_CST);
    // 3. prepare PrintInfo header
    PrintfInfo infoHead;
    infoHead.printfType   = PRINT_TYPE_WRITING;
    infoHead.printfLength = tlvLen - sizeof(uint32_t[2]);
    infoHead.blockIdx     = 0U;
    infoHead.resv         = 0U;
    infoHead.fmtOffset    = sizeof(PrintfInfo);
    // 4. write print info
    if (!needSplit) {
        uint8_t* tlvWriteAddr = ringBase + wOff;
        // write header
        (void)memcpy(tlvWriteAddr, &infoHead, sizeof(PrintfInfo));
        // write formatted string directly into the ring buffer
        char* strDest = reinterpret_cast<char*>(tlvWriteAddr + sizeof(PrintfInfo));
        size_t destMax = ringBufLen - static_cast<size_t>(wOff) - sizeof(PrintfInfo);
        size_t count = (strLen < destMax) ? strLen : (destMax - 1U);
        (void)vsnprintf_s(strDest, destMax, count, fmt, otherArgs);
    } else {
        // copy printTLV to ringbuf by bytes
        const uint8_t* headBytes = reinterpret_cast<const uint8_t*>(&infoHead);
        uint32_t off = static_cast<uint32_t>(wOff);
        for (uint32_t i = 0U; i < sizeof(PrintfInfo); ++i) {
            ringBase[off] = headBytes[i];
            off = (off + 1U) % ringBufLen;
        }
        // set up cookie for string write
        RingBufCookie cookie;
        cookie.ringBase = ringBase;
        cookie.ringBufLen = ringBufLen;
        cookie.writeOffset = off;
        cookie.bytesWritten = 0U;
        // Format directly into the ringbuf via fopencookie + vfprintf.
        FILE* fp = RingBufOpenStream(&cookie);
        if (fp != nullptr) {
            (void)vfprintf(fp, fmt, otherArgs);
            (void)fclose(fp);
            ringBase[cookie.writeOffset] = '\0';
        }
    }
/* ── Step 7: Release the entry to the rts reader ─────────────────── */
    /* Ensure all data writes are visible before flipping the type. */
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    uint32_t readyType = PRINT_TYPE_READY;
    if (!needSplit) {
        /* Non-split: entry is contiguous, typeField is aligned. */
        uint32_t* typeField = reinterpret_cast<uint32_t*>(ringBase + wOff);
        __atomic_store_n(typeField, readyType, __ATOMIC_SEQ_CST);
    } else {
        /* Split: wOff may be unaligned or the type field may span
         * the tail/head boundary.  Use byte-by-byte copy with
         * wrap-around to handle both cases. */
        const uint8_t* src = reinterpret_cast<const uint8_t*>(&readyType);
        for (uint32_t i = 0U; i < sizeof(uint32_t); ++i) {
            ringBase[(static_cast<uint32_t>(wOff) + i) % ringBufLen] = src[i];
        }
        __atomic_thread_fence(__ATOMIC_SEQ_CST);
    }

    __atomic_fetch_add(reinterpret_cast<uint64_t*>(&writeInfo->packIdx), 1U,
                       __ATOMIC_SEQ_CST);
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
}

/* ── Public API: unified printf entry point ─────────────────────────── */

void printf(const char* fmt, ...)
{
    va_list otherArgs;
    va_start(otherArgs, fmt);

    if (AicpuGetDfxInfo == nullptr) {
        /* rts does not provide FIFO — use legacy linear buffer. */
        PrintfLegacy(fmt, otherArgs);
    } else {
        PrintfFifo(fmt, otherArgs);
    }

    va_end(otherArgs);
}

} // namespace AscendC
