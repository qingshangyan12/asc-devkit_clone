/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "simt_compiler_stub.h"
#include "kernel_process_lock.h"
#include "kernel_utils.h"
#include "dav_c310/kernel_simt_cpu.h"
#include "stub_def.h"
#include <cmath>

namespace bisheng {
namespace cce {
namespace simt {

int32_t atomicAdd(__gm__ int32_t *address, int32_t val)
{
    int32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address += val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint32_t atomicAdd(__gm__ uint32_t *address, uint32_t val)
{
    uint32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address += val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

float atomicAdd(__gm__ float *address, float val)
{
    float ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address += val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int64_t atomicAdd(__gm__ int64_t *address, int64_t val)
{
    int64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address += val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint64_t atomicAdd(__gm__ uint64_t *address, uint64_t val)
{
    uint64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address += val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int32_t atomicSub(__gm__ int32_t *address, int32_t val)
{
    int32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address -= val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint32_t atomicSub(__gm__ uint32_t *address, uint32_t val)
{
    uint32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address -= val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

float atomicSub(__gm__ float *address, float val)
{
    float ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address -= val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int64_t atomicSub(__gm__ int64_t *address, int64_t val)
{
    int64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address -= val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint64_t atomicSub(__gm__ uint64_t *address, uint64_t val)
{
    uint64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address -= val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int32_t atomicExch(__gm__ int32_t *address, int32_t val)
{
    int32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address = val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint32_t atomicExch(__gm__ uint32_t *address, uint32_t val)
{
    uint32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address = val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int64_t atomicExch(__gm__ int64_t *address, int64_t val)
{
    int64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address = val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint64_t atomicExch(__gm__ uint64_t *address, uint64_t val)
{
    uint64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address = val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

float atomicExch(__gm__ float *address, float val)
{
    float ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address = val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int32_t atomicMax(__gm__ int32_t *address, int32_t val)
{
    int32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address < val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint32_t atomicMax(__gm__ uint32_t *address, uint32_t val)
{
    uint32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address < val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

float atomicMax(__gm__ float *address, float val)
{
    float ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address < val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int64_t atomicMax(__gm__ int64_t *address, int64_t val)
{
    int64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address < val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint64_t atomicMax(__gm__ uint64_t *address, uint64_t val)
{
    uint64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address < val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int32_t atomicMin(__gm__ int32_t *address, int32_t val)
{
    int32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address > val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint32_t atomicMin(__gm__ uint32_t *address, uint32_t val)
{
    uint32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address > val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

float atomicMin(__gm__ float *address, float val)
{
    float ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address > val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int64_t atomicMin(__gm__ int64_t *address, int64_t val)
{
    int64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address > val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint64_t atomicMin(__gm__ uint64_t *address, uint64_t val)
{
    uint64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address > val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint32_t atomicInc(__gm__ uint32_t *address, uint32_t val)
{
    uint32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address >= val) {
            *address = 0;
        } else {
            *address += 1;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint64_t atomicInc(__gm__ uint64_t *address, uint64_t val)
{
    uint64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address >= val) {
            *address = 0;
        } else {
            *address += 1;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint32_t atomicDec(__gm__ uint32_t *address, uint32_t val)
{
    uint32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address == 0 || *address > val) {
            *address = val;
        } else {
            *address -= 1;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint64_t atomicDec(__gm__ uint64_t *address, uint64_t val)
{
    uint64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address == 0 || *address > val) {
            *address = val;
        } else {
            *address -= 1;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int32_t atomicCAS(__gm__ int32_t *address, int32_t compare, int32_t val)
{
    int32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, compare, val]() {
        if (*address == compare) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint32_t atomicCAS(__gm__ uint32_t *address, uint32_t compare, uint32_t val)
{
    uint32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, compare, val]() {
        if (*address == compare) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int64_t atomicCAS(__gm__ int64_t *address, int64_t compare, int64_t val)
{
    int64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, compare, val]() {
        if (*address == compare) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint64_t atomicCAS(__gm__ uint64_t *address, uint64_t compare, uint64_t val)
{
    uint64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, compare, val]() {
        if (*address == compare) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

float atomicCAS(__gm__ float *address, float compare, float val)
{
    float ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, compare, val]() {
        if (*address == compare) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int32_t atomicAnd(__gm__ int32_t *address, int32_t val)
{
    int32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret & val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint32_t atomicAnd(__gm__ uint32_t *address, uint32_t val)
{
    uint32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret & val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int64_t atomicAnd(__gm__ int64_t *address, int64_t val)
{
    int64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret & val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint64_t atomicAnd(__gm__ uint64_t *address, uint64_t val)
{
    uint64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret & val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int32_t atomicOr(__gm__ int32_t *address, int32_t val)
{
    int32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret | val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint32_t atomicOr(__gm__ uint32_t *address, uint32_t val)
{
    uint32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret | val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int64_t atomicOr(__gm__ int64_t *address, int64_t val)
{
    int64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret | val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint64_t atomicOr(__gm__ uint64_t *address, uint64_t val)
{
    uint64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret | val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int32_t atomicXOr(__gm__ int32_t *address, int32_t val)
{
    int32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret ^ val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint32_t atomicXOr(__gm__ uint32_t *address, uint32_t val)
{
    uint32_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret ^ val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int64_t atomicXOr(__gm__ int64_t *address, int64_t val)
{
    int64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret ^ val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

uint64_t atomicXOr(__gm__ uint64_t *address, uint64_t val)
{
    uint64_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret ^ val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

int32_t __all(int32_t predicate)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](int32_t a, int32_t b) -> int32_t { return (a != 0) && (b != 0); };
    return warp.WarpOp(predicate, customFunc);
}

int32_t __any(int32_t predicate)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](int32_t a, int32_t b) -> int32_t { return (a != 0) || (b != 0); };
    return warp.WarpOp(predicate, customFunc);
}

uint32_t __ballot(int32_t predicate)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    uint32_t one = 1;
    uint32_t bitNum = 0;
    if (predicate != 0) {
        bitNum = one << laneId;
    }
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](uint32_t a, uint32_t b) -> uint32_t { return a + b; };
    return warp.WarpOp(bitNum, customFunc);
}

uint32_t __activemask()
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    uint32_t one = 1;
    uint32_t bitNum = one << laneId;
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](uint32_t a, uint32_t b) -> uint32_t { return a + b; };
    return warp.WarpOp(bitNum, customFunc);
}

int32_t __shfl(int32_t var, int32_t num)
{
    int32_t srcLane = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((srcLane < WARP_SIZE && srcLane >= 0), { KERNEL_LOG(KERNEL_ERROR, "srcLane must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    srcLane = minLane + srcLane % width;

    return warp.WarpShuffleOp(var, laneId, srcLane);

}

uint32_t __shfl(uint32_t var, int32_t num)
{
    int32_t srcLane = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((srcLane < WARP_SIZE && srcLane >= 0), { KERNEL_LOG(KERNEL_ERROR, "srcLane must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    srcLane = minLane + srcLane % width;

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

float __shfl(float var, int32_t num)
{
    int32_t srcLane = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((srcLane < WARP_SIZE && srcLane >= 0), { KERNEL_LOG(KERNEL_ERROR, "srcLane must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    srcLane = minLane + srcLane % width;

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

int64_t __shfl(int64_t var, int32_t num)
{
    int32_t srcLane = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((srcLane < WARP_SIZE && srcLane >= 0), { KERNEL_LOG(KERNEL_ERROR, "srcLane must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    srcLane = minLane + srcLane % width;

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

uint64_t __shfl(uint64_t var, int32_t num)
{
    int32_t srcLane = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((srcLane < WARP_SIZE && srcLane >= 0), { KERNEL_LOG(KERNEL_ERROR, "srcLane must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    srcLane = minLane + srcLane % width;

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

half __shfl(half var, int32_t num)
{
    int32_t srcLane = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((srcLane < WARP_SIZE && srcLane >= 0), { KERNEL_LOG(KERNEL_ERROR, "srcLane must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    srcLane = minLane + srcLane % width;

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

half2 __shfl(half2 var, int32_t num)
{
    int32_t srcLane = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((srcLane < WARP_SIZE && srcLane >= 0), { KERNEL_LOG(KERNEL_ERROR, "srcLane must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    srcLane = minLane + srcLane % width;

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

int32_t __shfl_up(int32_t var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t srcLane = laneId - delta;

    if (srcLane < minLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

uint32_t __shfl_up(uint32_t var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t srcLane = laneId - delta;

    if (srcLane < minLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

float __shfl_up(float var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t srcLane = laneId - delta;

    if (srcLane < minLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

int64_t __shfl_up(int64_t var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t srcLane = laneId - delta;

    if (srcLane < minLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

uint64_t __shfl_up(uint64_t var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t srcLane = laneId - delta;

    if (srcLane < minLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

half __shfl_up(half var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t srcLane = laneId - delta;

    if (srcLane < minLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

half2 __shfl_up(half2 var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t srcLane = laneId - delta;

    if (srcLane < minLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

int32_t __shfl_down(int32_t var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((delta < WARP_SIZE && delta >= 0), { KERNEL_LOG(KERNEL_ERROR, "delta must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId + delta;

    if (srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

uint32_t __shfl_down(uint32_t var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((delta < WARP_SIZE && delta >= 0), { KERNEL_LOG(KERNEL_ERROR, "delta must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId + delta;

    if (srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

float __shfl_down(float var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((delta < WARP_SIZE && delta >= 0), { KERNEL_LOG(KERNEL_ERROR, "delta must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId + delta;

    if (srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

int64_t __shfl_down(int64_t var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((delta < WARP_SIZE && delta >= 0), { KERNEL_LOG(KERNEL_ERROR, "delta must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId + delta;

    if (srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

uint64_t __shfl_down(uint64_t var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((delta < WARP_SIZE && delta >= 0), { KERNEL_LOG(KERNEL_ERROR, "delta must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId + delta;

    if (srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

half __shfl_down(half var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((delta < WARP_SIZE && delta >= 0), { KERNEL_LOG(KERNEL_ERROR, "delta must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId + delta;

    if (srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

half2 __shfl_down(half2 var, int32_t num)
{
    int32_t delta = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((delta < WARP_SIZE && delta >= 0), { KERNEL_LOG(KERNEL_ERROR, "delta must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId + delta;

    if (srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

int32_t __shfl_xor(int32_t var, int32_t num)
{
    int32_t laneMask = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((laneMask < WARP_SIZE && laneMask >= 0),
                   { KERNEL_LOG(KERNEL_ERROR, "laneMask must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId ^ laneMask;

    if (srcLane < 0 || srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

uint32_t __shfl_xor(uint32_t var, int32_t num)
{
    int32_t laneMask = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((laneMask < WARP_SIZE && laneMask >= 0),
                   { KERNEL_LOG(KERNEL_ERROR, "laneMask must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId ^ laneMask;

    if (srcLane < 0 || srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

float __shfl_xor(float var, int32_t num)
{
    int32_t laneMask = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((laneMask < WARP_SIZE && laneMask >= 0),
                   { KERNEL_LOG(KERNEL_ERROR, "laneMask must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId ^ laneMask;

    if (srcLane < 0 || srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

int64_t __shfl_xor(int64_t var, int32_t num)
{
    int32_t laneMask = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((laneMask < WARP_SIZE && laneMask >= 0),
                   { KERNEL_LOG(KERNEL_ERROR, "laneMask must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId ^ laneMask;

    if (srcLane < 0 || srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

uint64_t __shfl_xor(uint64_t var, int32_t num)
{
    int32_t laneMask = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((laneMask < WARP_SIZE && laneMask >= 0),
                   { KERNEL_LOG(KERNEL_ERROR, "laneMask must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId ^ laneMask;

    if (srcLane < 0 || srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

half __shfl_xor(half var, int32_t num)
{
    int32_t laneMask = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((laneMask < WARP_SIZE && laneMask >= 0),
                   { KERNEL_LOG(KERNEL_ERROR, "laneMask must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId ^ laneMask;

    if (srcLane < 0 || srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

half2 __shfl_xor(half2 var, int32_t num)
{
    int32_t laneMask = num & 0xff;
    int32_t width = 32 - ((num >> 16) & 0xff);
    ASCENDC_ASSERT((width <= WARP_SIZE && width > 0 && WARP_SIZE % width == 0),
                   { KERNEL_LOG(KERNEL_ERROR, "width must be a power of 2 and in (0, 32]"); });
    ASCENDC_ASSERT((laneMask < WARP_SIZE && laneMask >= 0),
                   { KERNEL_LOG(KERNEL_ERROR, "laneMask must be in [0, 32)"); });

    uint32_t warpId = AscendC::Simt::GetWarpId();
    uint32_t laneId = AscendC::Simt::GetLaneId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    int32_t minLane = laneId / width * width;
    int32_t maxLane = minLane + MAX_SHLF_OFFSET % width;
    int32_t srcLane = laneId ^ laneMask;

    if (srcLane < 0 || srcLane > maxLane) {
        srcLane = laneId;
    }

    return warp.WarpShuffleOp(var, laneId, srcLane);
}

int32_t __reduce_add(int32_t val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](int32_t a, int32_t b) -> int32_t { return a + b; };
    return warp.WarpOp(val, customFunc);
}

uint32_t __reduce_add(uint32_t val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](uint32_t a, uint32_t b) -> uint32_t { return a + b; };
    return warp.WarpOp(val, customFunc);
}

float __reduce_add(float val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](float a, float b) -> float { return a + b; };
    return warp.WarpOp(val, customFunc);
}

half __reduce_add(half val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](half a, half b) -> half { return a + b; };
    return warp.WarpOp(val, customFunc);
}

int32_t __reduce_max(int32_t val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](int32_t a, int32_t b) -> int32_t { return a > b ? a : b; };
    return warp.WarpOp(val, customFunc);
}

uint32_t __reduce_max(uint32_t val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](uint32_t a, uint32_t b) -> uint32_t { return a > b ? a : b; };
    return warp.WarpOp(val, customFunc);
}

float __reduce_max(float val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](float a, float b) -> float { return a > b ? a : b; };
    return warp.WarpOp(val, customFunc);
}

half __reduce_max(half val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](half a, half b) -> half { return a > b ? a : b; };
    return warp.WarpOp(val, customFunc);
}

int32_t __reduce_min(int32_t val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](int32_t a, int32_t b) -> int32_t { return a < b ? a : b; };
    return warp.WarpOp(val, customFunc);
}

uint32_t __reduce_min(uint32_t val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](uint32_t a, uint32_t b) -> uint32_t { return a < b ? a : b; };
    return warp.WarpOp(val, customFunc);
}

float __reduce_min(float val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](float a, float b) -> float { return a < b ? a : b; };
    return warp.WarpOp(val, customFunc);
}

half __reduce_min(half val)
{
    uint32_t warpId = AscendC::Simt::GetWarpId();
    AscendC::Simt::Warp &warp = AscendC::Simt::ThreadBlock::GetBlockInstance().warps_[warpId];
    auto customFunc = [](half a, half b) -> half { return a < b ? a : b; };
    return warp.WarpOp(val, customFunc);
}

void __threadfence()
{    
}

float roundf(float x) {
    return round(x);
}

float rintf(float x) {
    return rint(x);
}

float floorf(float x) {
    return floor(x);
}

float ceilf(float x) {
    return ceil(x);
}

float __fma(float x, float y, float z) {
    return fma(x, y, z);
}

template<ROUND rnd, RoundingSaturation rst>
int32_t __cvt_int32_t(float x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
int32_t __cvt_int32_t(half x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
int32_t __cvt_int32_t(bfloat16_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
uint32_t __cvt_uint32_t(float x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
uint32_t __cvt_uint32_t(half x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
uint32_t __cvt_uint32_t(bfloat16_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
int64_t __cvt_int64_t(float x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
uint64_t __cvt_uint64_t(float x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
half __cvt_half(int32_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
half __cvt_half(uint32_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(int32_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(uint32_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(int64_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(uint64_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
bfloat16_t __cvt_bfloat16_t(int32_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
bfloat16_t __cvt_bfloat16_t(uint32_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
half __cvt_half(half x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
half __cvt_half(float x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
half __cvt_half(bfloat16_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
bfloat16_t __cvt_bfloat16_t(bfloat16_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
bfloat16_t __cvt_bfloat16_t(half x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
bfloat16_t __cvt_bfloat16_t(float x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(bfloat16_t x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(half x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

template<ROUND rnd, RoundingSaturation rst>
float __cvt_float(float x) {
    if (rnd == ROUND::R) {
        return rint(x);
    } else if (rnd == ROUND::Z) {
        return trunc(x);
    } else if (rnd == ROUND::F) {
        return floor(x);
    } else if (rnd == ROUND::C) {
        return ceil(x);
    } else if (rnd == ROUND::A) {
        return round(x);
    }
}

int32_t get_block_idx()
{
    return block_idx;
}

int32_t get_block_num()
{
    return block_num;
}

half atomicAdd(__gm__ half *address, half val)
{
    half ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address += val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

bfloat16_t atomicAdd(__gm__ bfloat16_t *address, bfloat16_t val)
{
    bfloat16_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address += val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

half2 atomicAdd(__gm__ half2 *address, half2 val)
{
    return val;
}

bfloat16x2_t atomicAdd(__gm__ bfloat16x2_t *address, bfloat16x2_t val)
{
    return val;
}

half atomicSub(__gm__ half *address, half val)
{
    half ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address -= val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

bfloat16_t atomicSub(__gm__ bfloat16_t *address, bfloat16_t val)
{
    bfloat16_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address -= val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

half2 atomicSub(__gm__ half2 *address, half2 val)
{
    return val;
}

bfloat16x2_t atomicSub(__gm__ bfloat16x2_t *address, bfloat16x2_t val)
{
    return val;
}

half atomicMax(__gm__ half *address, half val)
{
    half ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address < val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

bfloat16_t atomicMax(__gm__ bfloat16_t *address, bfloat16_t val)
{
    bfloat16_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address < val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

half2 atomicMax(__gm__ half2 *address, half2 val)
{
    return val;
}

bfloat16x2_t atomicMax(__gm__ bfloat16x2_t *address, bfloat16x2_t val)
{
    return val;
}

half atomicMin(__gm__ half *address, half val)
{
    half ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address > val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

bfloat16_t atomicMin(__gm__ bfloat16_t *address, bfloat16_t val)
{
    bfloat16_t ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address > val) {
            *address = val;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

half2 atomicMin(__gm__ half2 *address, half2 val)
{
    return val;
}

bfloat16x2_t atomicMin(__gm__ bfloat16x2_t *address, bfloat16x2_t val)
{
    return val;
}

half2 atomicExch(__gm__ half2 *address, half2 val)
{
    return val;
}

bfloat16x2_t atomicExch(__gm__ bfloat16x2_t *address, bfloat16x2_t val)
{
    return val;
}

half2 atomicCAS(__gm__ half2 *address, half2 compare, half2 val)
{
    return val;
}

bfloat16x2_t atomicCAS(__gm__ bfloat16x2_t *address, bfloat16x2_t compare, bfloat16x2_t val)
{
    return val;
}

}
}
}

void __sync_workitems()
{
    AscendC::Simt::Sync();
}

bool __isnan(half x)
{
    uint16_t* intX = (uint16_t*)&x;
    return (*intX > half_inf && *intX <= half_max_nan) ||
                (*intX > half_neg_inf);
}

bool __isnan(bfloat16_t x)
{
    uint16_t* intX = (uint16_t*)&x;
    return (*intX > b_half_inf && *intX <= half_max_nan) ||
                (*intX > b_half_neg_inf);
}

bool __isinf(half x)
{
    uint16_t* intX = (uint16_t*)&x;
    return (*intX == half_inf) || (*intX == half_neg_inf);
}

bool __isinf(bfloat16_t x)
{
    uint16_t* intX = (uint16_t*)&x;
    return (*intX == b_half_inf || *intX == b_half_neg_inf);
}

float __floorf(float x)
{
    return floor(x);
}

half __floorf(half x)
{
    if (__isinf(x)) {
        return x;
    }
    return floor(x);
}

template <typename T>
__aicore__ inline T FindNear2ndPow(T x)
{
    if ((T)2 >= x) {
        return (T)1;
    } else {
        return (T)pow(2, static_cast<uint32_t>(log2(x)));
    }
}

bfloat16_t __floorf(bfloat16_t x)
{
    if (__isinf(x)) {
        return x;
    }
    uint16_t *uintX = (uint16_t *)(&x);
    uint16_t bf16Sign15Bit = 0xf;
    uint16_t bf16Frac7Bit = 0x7;
    int8_t sign = ((((*uintX) >> bf16Sign15Bit) & 0x1) == 0) ? 1 : -1;
    uint16_t exp = ((*uintX) >> bf16Frac7Bit) & 0xff;
    bfloat16_t frac = (*uintX) & 0x7f;
    bfloat16_t two = 2.0;
    uint16_t bfl16Exp126Bit = 0x7e;
    uint16_t bfl16Exp127Bit = 0x7f;
    if (exp == 0xff && frac == 0) {
        return (bfloat16_t)(sign * INFINITY);
    } else if (exp == 0xff && frac != 0) {
        return (bfloat16_t)(sign * NAN);
    } else {
        bfloat16_t base;
        bfloat16_t minVal;
        if (exp == 0) {
            base = sign * (pow(two, (exp - bfl16Exp126Bit)) * (frac / (pow(two, bf16Frac7Bit))));
            if (exp - bfl16Exp126Bit > 0) {
                minVal = sign * (pow(two, (exp - bfl16Exp126Bit)));
            } else {
                minVal = 0;
            }
        } else {
            base = sign * (pow(two, (exp - bfl16Exp127Bit)) * (1 + (frac / (pow(two, bf16Frac7Bit)))));
            if (exp - bfl16Exp127Bit > 0) {
                minVal = sign * (pow(two, (exp - bfl16Exp127Bit)));
            } else {
                minVal = 0;
            }
        }
        if (base > minVal) {
            while (base - minVal >= 1) {
                minVal += FindNear2ndPow(base - minVal);
            }
            return minVal;
        } else if (base < minVal) {
            while (minVal - base > 0) {
                minVal -= FindNear2ndPow(minVal - base);
            }
            return minVal;
        } else {
            return base;
        }
    }
}

float __roundf(float x)
{
    return round(x);
}

float __rintf(float x)
{
    return rint(x);
}

half __rintf(half x)
{
    if (__isinf(x)) {
        return x;
    }
    return rint(x);
}

bfloat16_t __rintf(bfloat16_t x)
{
    if (__isinf(x)) {
        return x;
    }
    bfloat16_t floorX = __floorf(x);
    if (x == floorX) {
        return x;
    }
    bfloat16_t ceilX = floorX + (bfloat16_t)1;
    bfloat16_t midVal = 0.5;
    if (x - floorX > midVal) {
        return ceilX;
    } else if (x - floorX < midVal) {
        return floorX;
    } else {
        // Same Distance Round to Even
        uint16_t two = 2;
        if (((int32_t)ceilX) % two == 0) {
            return ceilX;
        } else {
            return floorX;
        }
    }
}

float __ceilf(float x)
{
    return ceil(x);
}

half __ceilf(half x)
{
    if (__isinf(x)) {
        return x;
    }
    return ceil(x);
}

bfloat16_t __ceilf(bfloat16_t x)
{
    bfloat16_t floorX = __floorf(x);
    if (floorX == x) {
        return floorX;
    }
    return floorX + (bfloat16_t)1;
}

float __fma(float x, float y, float z)
{
    if (__isnan(z) || __isnan(x) || __isnan(y)) {
        return NAN;
    }
    return std::fmaf(x, y, z);
}

half __fma(half x, half y, half z)
{
    if (__isnan(z) || __isnan(x) || __isnan(y)) {
        return NAN;
    }
    return (static_cast<float>(x) * static_cast<float>(y)) + static_cast<float>(z);
}

bfloat16_t __fma(bfloat16_t x, bfloat16_t y, bfloat16_t z)
{
    if (__isnan(z) || __isnan(x) || __isnan(y)) {
        return NAN;
    }
    return x * y + z;
}

float __fabsf(float x)
{
    return fabs(x);
}

float __fmaxf(float x, float y)
{
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

float __fminf(float x, float y)
{
    if (x < y) {
        return x;
    } else {
        return y;
    }
}
float __expf(float x)
{
    return exp(x);
}

half __expf(half x)
{
    float tmp = x.ToFloat();
    tmp = exp(tmp);
    return half(tmp);
}

half2 __expf(half2 x)
{
    return x;
}

float __powf(float base, float x)
{
    return powf(base, x);
}

half __powf(half base, half x)
{
    float base_f = base.ToFloat();
    float exponent = x.ToFloat();
    return half(powf(base, exponent));
}

float __logf(float x)
{
    return logf(x);
}

half __logf(half x)
{
    float tmp = x.ToFloat();
    tmp = logf(tmp);
    return half(tmp);
}

float __sqrtf(float x)
{
    return sqrtf(x);
}

half __sqrtf(half x)
{
    float tmp = x.ToFloat();
    tmp = sqrtf(tmp);
    return half(tmp);
}

bfloat16_t max(bfloat16_t x, bfloat16_t y)
{
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

bfloat16_t min(bfloat16_t x, bfloat16_t y)
{
    if (x < y) {
        return x;
    } else {
        return y;
    }
}