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

float __expf(float x)
{
    return exp(x);
}

half2 __expf(half2 x)
{
    return x;
}

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

void __threadfence()
{    
}

}
}
}

void __sync_workitems()
{
    AscendC::Simt::Sync();
}
