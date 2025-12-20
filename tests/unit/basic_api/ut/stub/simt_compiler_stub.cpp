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

}
}
}
