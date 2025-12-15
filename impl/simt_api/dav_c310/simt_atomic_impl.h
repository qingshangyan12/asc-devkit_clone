/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef ASCENDC_MODULE_SIMT_C_ATOMIC__IMPL_H
#define ASCENDC_MODULE_SIMT_C_ATOMIC__IMPL_H

#if defined(ASCENDC_CPU_DEBUG)
#include "kernel_process_lock.h"
#include "kernel_utils.h"
#include "kernel_simt_cpu.h"
#include "stub_def.h"
#endif

#ifndef ASCENDC_CPU_DEBUG
__simt_callee__ inline int32_t asc_atomic_add_impl(__ubuf__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicAdd(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_add_impl(__ubuf__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicAdd(address, val);
}

__simt_callee__ inline float asc_atomic_add_impl(__ubuf__ float *address, float val)
{
    return bisheng::cce::simt::atomicAdd(address, val);
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
template <typename T>
T asc_atomic_add_cpu(__gm__ T *address, T val)
{
    T ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address += val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

__simt_callee__ inline int32_t asc_atomic_add_impl(__gm__ int32_t *address, int32_t val)
{
    return asc_atomic_add_cpu(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_add_impl(__gm__ uint32_t *address, uint32_t val)
{
    return asc_atomic_add_cpu(address, val);
}

__simt_callee__ inline float asc_atomic_add_impl(__gm__ float *address, float val)
{
    return asc_atomic_add_cpu(address, val);
}

__simt_callee__ inline int64_t asc_atomic_add_impl(__gm__ int64_t *address, int64_t val)
{
    return asc_atomic_add_cpu(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_add_impl(__gm__ uint64_t *address, uint64_t val)
{
    return asc_atomic_add_cpu(address, val);
}
#else
__simt_callee__ inline int32_t asc_atomic_add_impl(__gm__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicAdd(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_add_impl(__gm__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicAdd(address, val);
}

__simt_callee__ inline float asc_atomic_add_impl(__gm__ float *address, float val)
{
    return bisheng::cce::simt::atomicAdd(address, val);
}

__simt_callee__ inline int64_t asc_atomic_add_impl(__gm__ int64_t *address, int64_t val)
{
    return bisheng::cce::simt::atomicAdd(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_add_impl(__gm__ uint64_t *address, uint64_t val)
{
    return bisheng::cce::simt::atomicAdd(address, val);
}
#endif

#ifndef ASCENDC_CPU_DEBUG
__simt_callee__ inline int32_t asc_atomic_sub_impl(__ubuf__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicSub(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_sub_impl(__ubuf__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicSub(address, val);
}

__simt_callee__ inline float asc_atomic_sub_impl(__ubuf__ float *address, float val)
{
    return bisheng::cce::simt::atomicSub(address, val);
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
template <typename T>
T asc_atomic_sub_cpu(__gm__ T *address, T val)
{
    T ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address -= val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

__simt_callee__ inline int32_t asc_atomic_sub_impl(__gm__ int32_t *address, int32_t val)
{
    return asc_atomic_sub_cpu(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_sub_impl(__gm__ uint32_t *address, uint32_t val)
{
    return asc_atomic_sub_cpu(address, val);
}

__simt_callee__ inline float asc_atomic_sub_impl(__gm__ float *address, float val)
{
    return asc_atomic_sub_cpu(address, val);
}

__simt_callee__ inline int64_t asc_atomic_sub_impl(__gm__ int64_t *address, int64_t val)
{
    return asc_atomic_sub_cpu(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_sub_impl(__gm__ uint64_t *address, uint64_t val)
{
    return asc_atomic_sub_cpu(address, val);
}
#else
__simt_callee__ inline int32_t asc_atomic_sub_impl(__gm__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicSub(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_sub_impl(__gm__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicSub(address, val);
}

__simt_callee__ inline float asc_atomic_sub_impl(__gm__ float *address, float val)
{
    return bisheng::cce::simt::atomicSub(address, val);
}

__simt_callee__ inline int64_t asc_atomic_sub_impl(__gm__ int64_t *address, int64_t val)
{
    return bisheng::cce::simt::atomicSub(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_sub_impl(__gm__ uint64_t *address, uint64_t val)
{
    return bisheng::cce::simt::atomicSub(address, val);
}
#endif

#ifndef ASCENDC_CPU_DEBUG
__simt_callee__ inline int32_t asc_atomic_exch_impl(__ubuf__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicExch(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_exch_impl(__ubuf__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicExch(address, val);
}

__simt_callee__ inline float asc_atomic_exch_impl(__ubuf__ float *address, float val)
{
    return bisheng::cce::simt::atomicExch(address, val);
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
template <typename T>
T asc_atomic_exch_cpu(__gm__ T *address, T val)
{
    T ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() { *address = val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

__simt_callee__ inline int32_t asc_atomic_exch_impl(__gm__ int32_t *address, int32_t val)
{
    return asc_atomic_exch_cpu(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_exch_impl(__gm__ uint32_t *address, uint32_t val)
{
    return asc_atomic_exch_cpu(address, val);
}

__simt_callee__ inline int64_t asc_atomic_exch_impl(__gm__ int64_t *address, int64_t val)
{
    return asc_atomic_exch_cpu(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_exch_impl(__gm__ uint64_t *address, uint64_t val)
{
    return asc_atomic_exch_cpu(address, val);
}

__simt_callee__ inline float asc_atomic_exch_impl(__gm__ float *address, float val)
{
    return asc_atomic_exch_cpu(address, val);
}
#else
__simt_callee__ inline int32_t asc_atomic_exch_impl(__gm__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicExch(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_exch_impl(__gm__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicExch(address, val);
}

__simt_callee__ inline int64_t asc_atomic_exch_impl(__gm__ int64_t *address, int64_t val)
{
    return bisheng::cce::simt::atomicExch(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_exch_impl(__gm__ uint64_t *address, uint64_t val)
{
    return bisheng::cce::simt::atomicExch(address, val);
}

__simt_callee__ inline float asc_atomic_exch_impl(__gm__ float *address, float val)
{
    return bisheng::cce::simt::atomicExch(address, val);
}
#endif

#ifndef ASCENDC_CPU_DEBUG
__simt_callee__ inline int32_t asc_atomic_max_impl(__ubuf__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicMax(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_max_impl(__ubuf__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicMax(address, val);
}

__simt_callee__ inline float asc_atomic_max_impl(__ubuf__ float *address, float val)
{
    return bisheng::cce::simt::atomicMax(address, val);
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
template <typename T>
T asc_atomic_max_cpu(__gm__ T *address, T val)
{
    T ret;
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

__simt_callee__ inline int32_t asc_atomic_max_impl(__gm__ int32_t *address, int32_t val)
{
    return asc_atomic_max_cpu(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_max_impl(__gm__ uint32_t *address, uint32_t val)
{
    return asc_atomic_max_cpu(address, val);
}

__simt_callee__ inline float asc_atomic_max_impl(__gm__ float *address, float val)
{
    return asc_atomic_max_cpu(address, val);
}

__simt_callee__ inline int64_t asc_atomic_max_impl(__gm__ int64_t *address, int64_t val)
{
    return asc_atomic_max_cpu(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_max_impl(__gm__ uint64_t *address, uint64_t val)
{
    return asc_atomic_max_cpu(address, val);
}
#else
__simt_callee__ inline int32_t asc_atomic_max_impl(__gm__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicMax(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_max_impl(__gm__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicMax(address, val);
}

__simt_callee__ inline float asc_atomic_max_impl(__gm__ float *address, float val)
{
    return bisheng::cce::simt::atomicMax(address, val);
}

__simt_callee__ inline int64_t asc_atomic_max_impl(__gm__ int64_t *address, int64_t val)
{
    return bisheng::cce::simt::atomicMax(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_max_impl(__gm__ uint64_t *address, uint64_t val)
{
    return bisheng::cce::simt::atomicMax(address, val);
}
#endif

#ifndef ASCENDC_CPU_DEBUG
__simt_callee__ inline int32_t asc_atomic_min_impl(__ubuf__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicMin(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_min_impl(__ubuf__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicMin(address, val);
}

__simt_callee__ inline float asc_atomic_min_impl(__ubuf__ float *address, float val)
{
    return bisheng::cce::simt::atomicMin(address, val);
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
template <typename T>
T asc_atomic_min_cpu(__gm__ T *address, T val)
{
    T ret;
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

__simt_callee__ inline int32_t asc_atomic_min_impl(__gm__ int32_t *address, int32_t val)
{
    return asc_atomic_min_cpu(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_min_impl(__gm__ uint32_t *address, uint32_t val)
{
    return asc_atomic_min_cpu(address, val);
}

__simt_callee__ inline float asc_atomic_min_impl(__gm__ float *address, float val)
{
    return asc_atomic_min_cpu(address, val);
}

__simt_callee__ inline int64_t asc_atomic_min_impl(__gm__ int64_t *address, int64_t val)
{
    return asc_atomic_min_cpu(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_min_impl(__gm__ uint64_t *address, uint64_t val)
{
    return asc_atomic_min_cpu(address, val);
}
#else
__simt_callee__ inline int32_t asc_atomic_min_impl(__gm__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicMin(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_min_impl(__gm__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicMin(address, val);
}

__simt_callee__ inline float asc_atomic_min_impl(__gm__ float *address, float val)
{
    return bisheng::cce::simt::atomicMin(address, val);
}

__simt_callee__ inline int64_t asc_atomic_min_impl(__gm__ int64_t *address, int64_t val)
{
    return bisheng::cce::simt::atomicMin(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_min_impl(__gm__ uint64_t *address, uint64_t val)
{
    return bisheng::cce::simt::atomicMin(address, val);
}
#endif

#ifndef ASCENDC_CPU_DEBUG
__simt_callee__ inline uint32_t asc_atomic_inc_impl(__ubuf__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicInc(address, val);
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
template <typename T>
T asc_atomic_inc_cpu(__gm__ T *address, T val)
{
    T ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address >= val) {
            *address = (T)0;
        } else {
            *address += (T)1;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

__simt_callee__ inline uint32_t asc_atomic_inc_impl(__gm__ uint32_t *address, uint32_t val)
{
    return asc_atomic_inc_cpu(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_inc_impl(__gm__ uint64_t *address, uint64_t val)
{
    return asc_atomic_inc_cpu(address, val);
}
#else
__simt_callee__ inline uint32_t asc_atomic_inc_impl(__gm__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicInc(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_inc_impl(__gm__ uint64_t *address, uint64_t val)
{
    return bisheng::cce::simt::atomicInc(address, val);
}
#endif

#ifndef ASCENDC_CPU_DEBUG
__simt_callee__ inline uint32_t asc_atomic_dec_impl(__ubuf__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicDec(address, val);
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
template <typename T>
T asc_atomic_dec_cpu(__gm__ T *address, T val)
{
    T ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val]() {
        if (*address == (T)0 || *address > val) {
            *address = val;
        } else {
            *address -= (T)1;
        }
    });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

__simt_callee__ inline uint32_t asc_atomic_dec_impl(__gm__ uint32_t *address, uint32_t val)
{
    return asc_atomic_dec_cpu(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_dec_impl(__gm__ uint64_t *address, uint64_t val)
{
    return asc_atomic_dec_cpu(address, val);
}
#else
__simt_callee__ inline uint32_t asc_atomic_dec_impl(__gm__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicDec(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_dec_impl(__gm__ uint64_t *address, uint64_t val)
{
    return bisheng::cce::simt::atomicDec(address, val);
}
#endif

#ifndef ASCENDC_CPU_DEBUG
__simt_callee__ inline int32_t asc_atomic_cas_impl(__ubuf__ int32_t *address, int32_t compare, int32_t val)
{
    return bisheng::cce::simt::atomicCAS(address, compare, val);
}

__simt_callee__ inline uint32_t asc_atomic_cas_impl(__ubuf__ uint32_t *address, uint32_t compare, uint32_t val)
{
    return bisheng::cce::simt::atomicCAS(address, compare, val);
}

__simt_callee__ inline float asc_atomic_cas_impl(__ubuf__ float *address, float compare, float val)
{
    return bisheng::cce::simt::atomicCAS(address, compare, val);
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
template <typename T>
T asc_atomic_cas_cpu(__gm__ T *address, T compare, T val)
{
    T ret;
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

__simt_callee__ inline int32_t asc_atomic_cas_impl(__gm__ int32_t *address, int32_t compare, int32_t val)
{
    return asc_atomic_cas_cpu(address, compare, val);
}

__simt_callee__ inline uint32_t asc_atomic_cas_impl(__gm__ uint32_t *address, uint32_t compare, uint32_t val)
{
    return asc_atomic_cas_cpu(address, compare, val);
}

__simt_callee__ inline int64_t asc_atomic_cas_impl(__gm__ int64_t *address, int64_t compare, int64_t val)
{
    return asc_atomic_cas_cpu(address, compare, val);
}

__simt_callee__ inline uint64_t asc_atomic_cas_impl(__gm__ uint64_t *address, uint64_t compare, uint64_t val)
{
    return asc_atomic_cas_cpu(address, compare, val);
}

__simt_callee__ inline float asc_atomic_cas_impl(__gm__ float *address, float compare, float val)
{
    return asc_atomic_cas_cpu(address, compare, val);
}
#else
__simt_callee__ inline int32_t asc_atomic_cas_impl(__gm__ int32_t *address, int32_t compare, int32_t val)
{
    return bisheng::cce::simt::atomicCAS(address, compare, val);
}

__simt_callee__ inline uint32_t asc_atomic_cas_impl(__gm__ uint32_t *address, uint32_t compare, uint32_t val)
{
    return bisheng::cce::simt::atomicCAS(address, compare, val);
}

__simt_callee__ inline int64_t asc_atomic_cas_impl(__gm__ int64_t *address, int64_t compare, int64_t val)
{
    return bisheng::cce::simt::atomicCAS(address, compare, val);
}

__simt_callee__ inline uint64_t asc_atomic_cas_impl(__gm__ uint64_t *address, uint64_t compare, uint64_t val)
{
    return bisheng::cce::simt::atomicCAS(address, compare, val);
}

__simt_callee__ inline float asc_atomic_cas_impl(__gm__ float *address, float compare, float val)
{
    return bisheng::cce::simt::atomicCAS(address, compare, val);
}
#endif

#ifndef ASCENDC_CPU_DEBUG
__simt_callee__ inline int32_t asc_atomic_and_impl(__ubuf__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicAnd(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_and_impl(__ubuf__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicAnd(address, val);
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
template <typename T>
T asc_atomic_and_cpu(__gm__ T *address, T val)
{
    T ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret & val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

__simt_callee__ inline int32_t asc_atomic_and_impl(__gm__ int32_t *address, int32_t val)
{
    return asc_atomic_and_cpu(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_and_impl(__gm__ uint32_t *address, uint32_t val)
{
    return asc_atomic_and_cpu(address, val);
}

__simt_callee__ inline int64_t asc_atomic_and_impl(__gm__ int64_t *address, int64_t val)
{
    return asc_atomic_and_cpu(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_and_impl(__gm__ uint64_t *address, uint64_t val)
{
    return asc_atomic_and_cpu(address, val);
}
#else
__simt_callee__ inline int32_t asc_atomic_and_impl(__gm__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicAnd(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_and_impl(__gm__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicAnd(address, val);
}

__simt_callee__ inline int64_t asc_atomic_and_impl(__gm__ int64_t *address, int64_t val)
{
    return bisheng::cce::simt::atomicAnd(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_and_impl(__gm__ uint64_t *address, uint64_t val)
{
    return bisheng::cce::simt::atomicAnd(address, val);
}
#endif

#ifndef ASCENDC_CPU_DEBUG
__simt_callee__ inline int32_t asc_atomic_or_impl(__ubuf__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicOr(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_or_impl(__ubuf__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicOr(address, val);
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
template <typename T>
T asc_atomic_or_cpu(__gm__ T *address, T val)
{
    T ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret | val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

__simt_callee__ inline int32_t asc_atomic_or_impl(__gm__ int32_t *address, int32_t val)
{
    return asc_atomic_or_cpu(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_or_impl(__gm__ uint32_t *address, uint32_t val)
{
    return asc_atomic_or_cpu(address, val);
}

__simt_callee__ inline int64_t asc_atomic_or_impl(__gm__ int64_t *address, int64_t val)
{
    return asc_atomic_or_cpu(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_or_impl(__gm__ uint64_t *address, uint64_t val)
{
    return asc_atomic_or_cpu(address, val);
}
#else
__simt_callee__ inline int32_t asc_atomic_or_impl(__gm__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicOr(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_or_impl(__gm__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicOr(address, val);
}

__simt_callee__ inline int64_t asc_atomic_or_impl(__gm__ int64_t *address, int64_t val)
{
    return bisheng::cce::simt::atomicOr(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_or_impl(__gm__ uint64_t *address, uint64_t val)
{
    return bisheng::cce::simt::atomicOr(address, val);
}
#endif

#ifndef ASCENDC_CPU_DEBUG
__simt_callee__ inline int32_t asc_atomic_xor_impl(__ubuf__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicXOr(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_xor_impl(__ubuf__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicXOr(address, val);
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
template <typename T>
T asc_atomic_xor_cpu(__gm__ T *address, T val)
{
    T ret;
    AscendC::ProcessLock::GetProcessLock()->Write();
    ret = *address;
    AscendC::Simt::ThreadBlock::GetBlockInstance().AtomicOp([address, val, ret]() { *address = ret ^ val; });
    AscendC::ProcessLock::GetProcessLock()->Unlock();
    return ret;
}

__simt_callee__ inline int32_t asc_atomic_xor_impl(__gm__ int32_t *address, int32_t val)
{
    return asc_atomic_xor_cpu(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_xor_impl(__gm__ uint32_t *address, uint32_t val)
{
    return asc_atomic_xor_cpu(address, val);
}

__simt_callee__ inline int64_t asc_atomic_xor_impl(__gm__ int64_t *address, int64_t val)
{
    return asc_atomic_xor_cpu(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_xor_impl(__gm__ uint64_t *address, uint64_t val)
{
    return asc_atomic_xor_cpu(address, val);
}
#else
__simt_callee__ inline int32_t asc_atomic_xor_impl(__gm__ int32_t *address, int32_t val)
{
    return bisheng::cce::simt::atomicXOr(address, val);
}

__simt_callee__ inline uint32_t asc_atomic_xor_impl(__gm__ uint32_t *address, uint32_t val)
{
    return bisheng::cce::simt::atomicXOr(address, val);
}

__simt_callee__ inline int64_t asc_atomic_xor_impl(__gm__ int64_t *address, int64_t val)
{
    return bisheng::cce::simt::atomicXOr(address, val);
}

__simt_callee__ inline uint64_t asc_atomic_xor_impl(__gm__ uint64_t *address, uint64_t val)
{
    return bisheng::cce::simt::atomicXOr(address, val);
}
#endif

#endif  // ASCENDC_MODULE_SIMT_C_ATOMIC_IMPL_H
