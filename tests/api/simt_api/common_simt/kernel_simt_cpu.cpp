/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifdef ASCENDC_CPU_DEBUG
#include "kernel_simt_cpu.h"

#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "kernel_log.h"

namespace AscendC {
namespace Simt {
Warp::~Warp()
{
    Done();
}

void Warp::Done()
{
    for (uint32_t i = 0; i < THREAD_PER_WARP; i++) {
        if (threads[i].joinable()) {
            threads[i].join();
        }
    }
}

ThreadBlock &ThreadBlock::GetBlockInstance()
{
    static ThreadBlock threadBlock;
    return threadBlock;
}

void ThreadBlock::Init(uint32_t num)
{
    ASCENDC_ASSERT((num % THREAD_PER_WARP == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "thread num %u must be multiple of %u", num, THREAD_PER_WARP); });
    ASCENDC_ASSERT((num > 0 && num <= 2048),
                   { KERNEL_LOG(KERNEL_ERROR, "thread num %u should be within (0, 2048]", num); });
    FinishJobs();
    activeThreads = num;
    threadThreshold = num;
    
    uint32_t warpCount = (num + THREAD_PER_WARP - 1) / THREAD_PER_WARP;
    warps_ = std::vector<Warp>(warpCount);
    warpNum_ = warpCount;
}

void ThreadBlock::FinishJobs()
{
    for (Warp &warp : warps_) {
        warp.Done();
    }
}

void ThreadBlock::SyncAllThreads()
{
    std::unique_lock<std::mutex> lck(mtx_);

    auto currGeneration = syncGeneration;
    activeThreads--;
    if (activeThreads == 0) {
        syncGeneration++;
        activeThreads = threadThreshold;
        cv_.notify_all();
    } else {
        cv_.wait(lck, [this, currGeneration] { return currGeneration != syncGeneration; });
    }
}

void ThreadBlock::ThreadFinished()
{
    std::unique_lock<std::mutex> lck(mtx_);
    threadThreshold--;
    activeThreads--;
    if (activeThreads == 0) {
        syncGeneration++;
        activeThreads = threadThreshold;
        cv_.notify_all();
    }
}

uint32_t GetThreadIdx()
{
    return g_threadIdxX + g_threadIdxY * g_threadDimX + g_threadIdxZ * g_threadDimX * g_threadDimY;
}

uint32_t GetLaneId()
{
    return GetThreadIdx() % THREAD_PER_WARP;
}

uint32_t GetWarpId()
{
    return GetThreadIdx() / THREAD_PER_WARP;
}

void Sync()
{
    ThreadBlock::GetBlockInstance().SyncAllThreads();
}
}  // namespace Simt
}  // namespace AscendC
#endif
