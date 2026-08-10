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

namespace AscendC {
void printf(const char* fmt, ...)
{
    std::lock_guard<std::mutex> lock(dump_mutex);
    void* aicpuDumpBuffer = g_aicpuDumpConfig.dumpBufAddr;
    size_t aicpuDumpBufSize = g_aicpuDumpConfig.dumpBufSize;
    size_t aicpuDumpOffSet = 0;
    if (aicpuDumpBuffer) {
        aicpuDumpOffSet = *reinterpret_cast<size_t*>(aicpuDumpBuffer);
    } else {
        return;
    }
    if (aicpuDumpOffSet >= aicpuDumpBufSize) {
        return;
    }
    va_list otherArgs;
    va_start(otherArgs, fmt);
    aicpuDumpOffSet += vsnprintf_s(
        reinterpret_cast<char*>(aicpuDumpBuffer) + aicpuDumpOffSet, aicpuDumpBufSize - aicpuDumpOffSet,
        aicpuDumpBufSize - aicpuDumpOffSet, fmt, otherArgs);
    *reinterpret_cast<size_t*>(g_aicpuDumpConfig.dumpBufAddr) = aicpuDumpOffSet;
    va_end(otherArgs);
}
} // namespace AscendC

/**
 * FIFO PRINTF IMPL
 */

extern "C" {
int32_t __attribute__((weak)) AicpuGetDfxInfo(uint64_t *infoAddr);
}

namespace AscendC {
std::mutex g_aicpuFifoMutex;

constexpr uint32_t ATTR_ID_MEM_INFO = 1U;

constexpr uint32_t PRINT_TYPE_WRITING = 0xF0A55A0FU;

constexpr uint32_t PRINT_TYPE_READY = 11; // TODO: use ENUM DUMP_AICPU = 11U

/** Sleep duration per wait iteration (300ms = 300,000,000 ns). */
constexpr uint64_t FIFO_WAIT_INTERVAL_NS = 300000000ULL;

struct RingBufCookie {
    uint8_t* ringBase;      /* base address of the ring buffer data area */
    uint32_t ringBufLen;    /* total size of the ring buffer data area    */
    uint32_t writeOffset;   /* current write offset (wraps mod ringBufLen) */
    uint32_t bytesWritten;  /* total bytes written so far                 */
};

typedef struct AicpuPrintfMemInfo {
    uinit64_t printfMemAddr = 0U;
    uint32_t printfMemSize = 0U;
    unit32_t resv0 = 0U;
} AicpuPrintfMemInfo;

union AicpuDfxAttrValue {
    uint8_t resv[64U] = {0U};
    AicpuPrintfMemInfo printfMemInfo;
};

typedef struct AicpuDfxAttrInfo {
    uint32_t attrId = 0U;
    AicpuDfxAttrValue value;
} AicpuDfxAttrInfo;

struct AicpuDfxInfo {
    uint64_t attrs = 0U;
    uint64_t numAttrs = 0U;
}

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
}

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

__aicore__ inline void enable_asc_aicpu_fifo_debug()
{
#if (!defined(ASCENDC_DUMP) || (ASCENDC_DUMP != 0))
    static const struct AscTlv __asc_aicpu_meta_section__ __attribute__((used, section(".ascend.meta"))) = {4, 4, 6};
#endif // (!defined(ASCENDC_DUMP) || (ASCENDC_DUMP != 0))
}

int32_t GetAscAicpuPrintfMemInfo(AicpuPrintfMemInfo *info) {
    struct AicpuDfxInfo *dfxInfo;
    AicpuGetDfxInfo((uint64_t *)dfxInfo);
    
    AicpuDfxAttrInfo *dfxAttrInfo = reinterpret_cast<AicpuDfxAttrInfo*>(dfxInfo.attrs);
    for(num : dfxInfo->numAttrs) {
        if (dfxAttrInfo[num].attrId == ATTR_ID_MEM_INFO) {
            info = reinterpret_cast<AicpuPrintfMemInfo*>(&(dfxAttrInfo[num].value.printfMemInfo));
            return 0;
        }
    }
    return 1;
}

inline BlockRingBufInfo* GetAicpuDebugBlockInfo(AicpuPrintfMemInfo *memInfo)
{
    BlockRingBufInfo* blockInfo = reinterpret_cast<BlockRingBufInfo*>(memInfo->printfMemAddr);
    if (blockInfo == nullptr || blockInfo->magic != 0xAE86) {
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
    const uint32_t fifoAlign = 8U; // 8 bytes align
    uint32_t total = sizeof(PrintfInfo) + strLen + 1U; /* +1 for NUL */
    total = (total + fifoAlign - 1U) & ~(fifoAlign - 1U);
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
    constexpr uint32_t maxCounter = 15; // max wait time: 15 * 300ms
    uint32_t counter = 0;
    struct timespec ts;
    ts.tv_sec  = static_cast<time_t>(FIFO_WAIT_INTERVAL_NS / 1000000000ULL);
    ts.tv_nsec = static_cast<long>(FIFO_WAIT_INTERVAL_NS % 1000000000ULL);

    for (uint32_t counter = 0U; counter < maxCounter; ++counter) {
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
    RingBufReadInfo*  readInfo  = GetRingBufReadInfo(blockInfo);
    RingBufWriteInfo* writeInfo = GetRingBufWriteInfo(blockInfo);

    if (tlvLen > ringBufLen) {
        return false;
    }

    std::lock_guard<std::mutex> lock(g_aicpuFifoMutex);
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
    /* read, seek, close are left NULL (not needed for write-only). */
    return fopencookie(cookie, "w", hooks);
}

void printf_FIFO_AICPU(const char* fmt, ...)
{
    AicpuPrintfMemInfo *memInfo = nullptr;
    int32_t ret = GetAscAicpuPrintfMemInfo(memInfo);
    if (ret != 0 || memInfo == nullptr) {
        // TODO call origin aicpu printf
        // printfAicpuDump(fmt, ...);
        return;
    }

    enable_asc_aicpu_fifo_debug();
    BlockRingBufInfo* blockInfo = GetAicpuDebugBlockInfo(memInfo);
    if (blockInfo == nullptr) {
        return;
    }
    // 1. calculate write info length and check whether there is enough free space
    va_list otherArgs;
    va_start(otherArgs, fmt);
    uint32_t strLen = CalcFormattedLen(fmt, args);
    va_end(args);
    uint32_t tlvLen = CalcTlvLen(strLen);
    if (!CheckAicpuRingBuf(blockInfo, tlvLen)) {
        return;
    }
    // 2. atomically reserve space.
    RingBufWriteInfo* writeInfo = GetRingBufWriteInfo(blockInfo);
    uint32_t wOff = writeInfo->bufOffset;
    uint32_t ringBufLen = blockInfo->ringBufLen;
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
    auto* ringBase = reinterpret_cast<uint8_t*>(blockInfo->ringBufAddr);
    if (!needSplit) {
        uint8_t* tlvWriteAddr = ringBase + wOff;
        // write header
        (void)memcpy(tlvWriteAddr, &infoHead, sizeof(PrintfInfo));
        // write formatted string directly into the ring buffer
        va_list otherArgs2;
        va_start(otherArgs2, fmt);
        char* strDest = reinterpret_cast<char*>(tlvWriteAddr + sizeof(PrintfInfo));
        size_t destMax = ringBufLen - static_cast<size_t>(tlvWriteAddr) - sizeof(PrintfInfo);
        size_t count = (strLen < destMax) ? strLen : (destMax - 1U);
        (void)vsnprintf_s(strDest, destMax, strLen, fmt, otherArgs2);
        va_end(otherArgs2);
    } else {
        // copy printTLV to ringbuf by bytes
        const uint8_t* headBytes = reinterpret_cast<const uint8_t*>(&infoHead);
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
            va_list otherArgs2;
            va_start(otherArgs2, fmt);
            (void)vfprintf(fp, fmt, otherArgs2);
            va_end(otherArgs2);
            (void)fclose(fp);
            /* Ensure NUL terminator at the current write position. */
            ringBase[cookie.writeOffset] = '\0';
        }
    }
    /* ── Step 7: Release the entry to the rts reader ─────────────────── */
    /* Ensure all data writes are visible before flipping the type. */
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    // 5. update the printType (writting -> DUMP_AICPU)
    uint32_t readyType = 11; //DUMP_AICPU;
    uint32_t* typeField = reinterpret_cast<uint32_t*>(ringBase + wOff);
    __atomic_store_n(typeField, readyType, __ATOMIC_SEQ_CST);
    /* Bump pack counter. */
    __atomic_fetch_add(reinterpret_cast<uint64_t*>(&writeInfo->packIdx), 1U,
                       __ATOMIC_SEQ_CST);
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
}

} // namespace  AscendC
