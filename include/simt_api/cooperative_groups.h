/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef INCLUDE_SIMT_API_COOPERATIVE_GROUPS_H
#define INCLUDE_SIMT_API_COOPERATIVE_GROUPS_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_COOPERATIVE_GROUPS_H__
#endif

#include "simt_api/asc_simt.h"
#include "simt_api/device_types.h"

template <typename T, typename... Args>
constexpr bool SupportTypeSimtInternel = (std::is_same_v<T, Args> || ...);

__SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int __fns(unsigned int mask, unsigned base, int offset)
{
    unsigned int temp_mask = mask;
    unsigned int temp_offset = offset;
    if (offset == 0) {
        temp_mask &= (1 << base);
        temp_offset = 1;
    } else if (offset < 0) {
        temp_mask = __brev(mask);
        base = 31 - base;
        temp_offset = -offset;
    }

    temp_mask = temp_mask & ((~0U) << base);
    if (__popc(temp_mask) < temp_offset) {
        return -1;
    }
    int total = 0;
    for (int i = 0x10; i > 0; i >>= 1) {
        unsigned int temp_mask_lo = temp_mask & ((1U << i) - 1);
        int pcnt = __popc(temp_mask_lo);
        if (pcnt < temp_offset) {
            temp_mask = temp_mask >> i;
            temp_offset -= pcnt;
            total += i;
        } else {
            temp_mask = temp_mask_lo;
        }
    }
    if (offset < 0) {
        return 31 - total;
    } else {
        return total;
    }
}

enum class group_type : unsigned int {
    thread_block_type,
    coalesced_group_type,
};

class thread_group {
protected:
    group_type _type;

    struct tiled_info {
        bool is_tiled;
        unsigned int mask;
        unsigned int num_threads;
        unsigned int meta_group_rank;
        unsigned int meta_group_size;
    } _tiled_info;

    friend class thread_block;

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline thread_group(group_type type) : _type(type) {}

public:
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long size() const;
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long num_threads() const;
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long thread_rank() const;
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline void sync() const;
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline group_type get_type() const { return _type; }
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int get_mask() const { return _tiled_info.mask; }
};

class thread_block : public thread_group {
    friend __SIMT_DEVICE_FUNCTIONS_DECL__ inline thread_block this_thread_block();

private:
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline thread_block() : thread_group(group_type::thread_block_type) {}
    __SIMT_DEVICE_FUNCTIONS_DECL__ thread_group create_tiled_group(unsigned int tile_size) const
    {
        const bool pow2 = ((tile_size & (tile_size - 1)) == 0);
        // tile size check
        if (tile_size == 0 || tile_size > warpSize || !pow2) {
            __trap();
        }

        unsigned int block_size = num_threads();
        unsigned int rank = thread_rank();
        unsigned int partition = (block_size + tile_size - 1) / tile_size;
        unsigned int tail = (partition * tile_size) - block_size;
        unsigned int partition_size = tile_size;
        if (rank >= (partition - 1) * tile_size) {
            partition_size -= tail;
        }

        unsigned int mask = static_cast<unsigned int>(-1) >> (warpSize - partition_size);
        mask <<= laneid() & ~(tile_size - 1);
        thread_group tiled_group = thread_group(group_type::coalesced_group_type);
        tiled_group._tiled_info.is_tiled = true;
        tiled_group._tiled_info.mask = mask;
        tiled_group._tiled_info.num_threads = partition_size;
        tiled_group._tiled_info.meta_group_rank = rank / tile_size;
        tiled_group._tiled_info.meta_group_size = partition;
        return tiled_group;
    }

public:
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline static void sync() { asc_syncthreads(); }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline static unsigned int thread_rank()
    {
        return static_cast<unsigned int>(
            threadIdx.x + threadIdx.y * blockDim.x + threadIdx.z * blockDim.x * blockDim.y);
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline static dim3 group_index() { return dim3(blockIdx.x, blockIdx.y, blockIdx.z); }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline static dim3 thread_index()
    {
        return dim3(threadIdx.x, threadIdx.y, threadIdx.z);
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline static dim3 dim_threads() { return dim3(blockDim.x, blockDim.y, blockDim.z); }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline static unsigned int num_threads()
    {
        return static_cast<unsigned int>(blockDim.x * blockDim.y * blockDim.z);
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline static unsigned int size() { return num_threads(); }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline static dim3 group_dim() { return dim_threads(); }
};

__SIMT_DEVICE_FUNCTIONS_DECL__ inline thread_block this_thread_block() { return thread_block(); }

class coalesced_group : public thread_group {
    friend __SIMT_DEVICE_FUNCTIONS_DECL__ coalesced_group coalesced_threads();

private:
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int _packLanes(unsigned int laneMask) const
    {
        unsigned int member_pack = 0;
        unsigned int member_rank = 0;
        for (int bit_idx = 0; bit_idx < 32; bit_idx++) {
            unsigned int lane_bit = _tiled_info.mask & (1U << bit_idx);
            if (lane_bit) {
                if (laneMask & lane_bit) {
                    member_pack |= 1U << member_rank;
                }
                member_rank++;
            }
        }
        return member_pack;
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline coalesced_group create_tiled_group(unsigned int tile_size) const
    {
        const bool pow2 = ((tile_size & (tile_size - 1)) == 0);
        // Invalid, immediately fail
        if (tile_size == 0 || (tile_size > warpSize) || !pow2) {
            __trap();
            return (coalesced_group(0));
        }
        if (size() <= tile_size) {
            return (*this);
        }

        if (_tiled_info.is_tiled) {
            unsigned int base_offset = (thread_rank() & (~(tile_size - 1)));
            unsigned int mask_length = min(static_cast<unsigned int>(num_threads()) - base_offset, tile_size);
            unsigned int mask = static_cast<unsigned int>(-1) >> (32 - mask_length);
            mask <<= (laneid() & ~(tile_size - 1));
            coalesced_group coalesced_tile = coalesced_group(mask);
            coalesced_tile._tiled_info.is_tiled = true;
            coalesced_tile._tiled_info.meta_group_rank = thread_rank() / tile_size;
            coalesced_tile._tiled_info.meta_group_size = num_threads() / tile_size;
            return coalesced_tile;
        } else {
            unsigned int mask = 0;
            unsigned int member_rank = 0;
            int seen_lanes = (thread_rank() / tile_size) * tile_size;

            for (unsigned int bit_idx = 0; bit_idx < 32; bit_idx++) {
                unsigned int lane_bit = _tiled_info.mask & (1U << bit_idx);
                if (lane_bit) {
                    if (seen_lanes <= 0 && member_rank < tile_size) {
                        mask |= lane_bit;
                        member_rank++;
                    }
                    seen_lanes--;
                }
            }
            coalesced_group coalesced_tile = coalesced_group(mask);
            coalesced_tile._tiled_info.is_tiled = true;
            coalesced_tile._tiled_info.meta_group_rank = thread_rank() / tile_size;
            coalesced_tile._tiled_info.meta_group_size = (num_threads() + tile_size - 1) / tile_size;
            return coalesced_tile;
        }
    }

protected:
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline coalesced_group(unsigned int mask)
        : thread_group(group_type::coalesced_group_type)
    {
        _tiled_info.is_tiled = false;
        _tiled_info.mask = mask;
        _tiled_info.num_threads = __popc(mask);
        _tiled_info.meta_group_rank = 0;
        _tiled_info.meta_group_size = 1;
    }

public:
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline void sync() const { asc_threadfence_block(); }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long num_threads() const
    {
        return static_cast<unsigned long long>(_tiled_info.num_threads);
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long size() const { return num_threads(); }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long thread_rank() const
    {
        return static_cast<unsigned long long>(__popc(_tiled_info.mask & lanemask_lt()));
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long meta_group_size() const
    {
        return static_cast<unsigned long long>(_tiled_info.meta_group_size);
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long meta_group_rank() const
    {
        return static_cast<unsigned long long>(_tiled_info.meta_group_rank);
    }

    template <typename T>
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline T shfl(T var, unsigned int src_rank) const
    {
        static_assert(
            SupportTypeSimtInternel<T, int32_t, uint32_t, int64_t, uint64_t, float, half, half2>,
            "Input type T only supports int32_t, uint32_t, int64_t, uint64_t, float, half, half2.");
        unsigned int lane = src_rank % static_cast<unsigned int>(num_threads());
        if (num_threads() != warpSize) {
            lane = __fns(_tiled_info.mask, 0, static_cast<int>(src_rank + 1));
        }
        return asc_shfl(var, static_cast<int32_t>(lane), warpSize);
    }

    template <typename T>
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline T shfl_up(T var, int delta) const
    {
        static_assert(
            SupportTypeSimtInternel<T, int32_t, uint32_t, int64_t, uint64_t, float, half, half2>,
            "Input type T only supports int32_t, uint32_t, int64_t, uint64_t, float, half, half2.");
        if (num_threads() == warpSize) {
            return asc_shfl_up(var, static_cast<uint32_t>(delta), warpSize);
        }

        int lane = static_cast<int>(__fns(_tiled_info.mask, laneid(), -(delta + 1)));
        if (lane == -1) {
            lane = static_cast<int>(laneid());
        }

        return asc_shfl(var, lane, warpSize);
    }

    template <typename T>
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline T shfl_down(T var, int delta) const
    {
        static_assert(
            SupportTypeSimtInternel<T, int32_t, uint32_t, int64_t, uint64_t, float, half, half2>,
            "Input type T only supports int32_t, uint32_t, int64_t, uint64_t, float, half, half2.");
        if (num_threads() == warpSize) {
            return asc_shfl_down(var, static_cast<uint32_t>(delta), warpSize);
        }

        int lane = static_cast<int>(__fns(_tiled_info.mask, laneid(), delta + 1));
        if (lane == -1) {
            lane = static_cast<int>(laneid());
        }

        return asc_shfl(var, lane, warpSize);
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline int any(int predicate) const
    {
        return (asc_ballot(predicate) & _tiled_info.mask) != 0;
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline int all(int predicate) const
    {
        return (asc_ballot(predicate) & _tiled_info.mask) == _tiled_info.mask;
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int ballot(int predicate) const
    {
        if (num_threads() == warpSize) {
            return asc_ballot(predicate) & 0xFFFFFFFF;
        }
        unsigned int lane_ballot = asc_ballot(predicate) & _tiled_info.mask;
        return _packLanes(lane_ballot);
    }
};

__SIMT_DEVICE_FUNCTIONS_DECL__ inline coalesced_group coalesced_threads() { return coalesced_group(asc_activemask()); }

template <unsigned int TileCount, unsigned int TileMask, unsigned int LaneMask, unsigned int ShiftCount>
struct _tile_helpers {
    static constexpr unsigned int tile_count = TileCount;
    static constexpr unsigned int tile_mask = TileMask;
    static constexpr unsigned int lane_mask = LaneMask;
    static constexpr unsigned int shift_count = ShiftCount;
};
template <unsigned int> struct tile_helpers;
template <> struct tile_helpers<32> : public _tile_helpers<1,  0xFFFFFFFF, 0x1F, 5> {};
template <> struct tile_helpers<16> : public _tile_helpers<2,  0x0000FFFF, 0x0F, 4> {};
template <> struct tile_helpers<8>  : public _tile_helpers<4,  0x000000FF, 0x07, 3> {};
template <> struct tile_helpers<4>  : public _tile_helpers<8,  0x0000000F, 0x03, 2> {};
template <> struct tile_helpers<2>  : public _tile_helpers<16, 0x00000003, 0x01, 1> {};
template <> struct tile_helpers<1>  : public _tile_helpers<32, 0x00000001, 0x00, 0> {};

template <unsigned int Size>
class thread_block_tile_base {
    using th = tile_helpers<Size>;
    static constexpr unsigned int numThreads = Size;

    __SIMT_DEVICE_FUNCTIONS_DECL__ static inline unsigned int build_mask()
    {
        unsigned int mask = static_cast<unsigned int>(-1);
        if (numThreads != warpSize) {
            unsigned int lane_id = laneid();
            mask = th::tile_mask << (lane_id & ~(th::lane_mask));
        }
        return mask;
    }

public:
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline void sync() const { asc_threadfence_block(); }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long num_threads() const { return numThreads; }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long thread_rank() const { return laneid() & (numThreads - 1); }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned long long size() const { return num_threads(); }

    template <typename T>
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline T shfl(T var, int src_rank) const
    {
        static_assert(
            SupportTypeSimtInternel<T, int32_t, uint32_t, int64_t, uint64_t, float, half, half2>,
            "Input type T only supports int32_t, uint32_t, int64_t, uint64_t, float, half, half2.");
        return asc_shfl(var, src_rank, static_cast<int32_t>(numThreads));
    }

    template <typename T>
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline T shfl_up(T var, unsigned int delta) const
    {
        static_assert(
            SupportTypeSimtInternel<T, int32_t, uint32_t, int64_t, uint64_t, float, half, half2>,
            "Input type T only supports int32_t, uint32_t, int64_t, uint64_t, float, half, half2.");
        return asc_shfl_up(var, delta, static_cast<int32_t>(numThreads));
    }

    template <typename T>
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline T shfl_down(T var, unsigned int delta) const
    {
        static_assert(
            SupportTypeSimtInternel<T, int32_t, uint32_t, int64_t, uint64_t, float, half, half2>,
            "Input type T only supports int32_t, uint32_t, int64_t, uint64_t, float, half, half2.");
        return asc_shfl_down(var, delta, static_cast<int32_t>(numThreads));
    }

    template <typename T>
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline T shfl_xor(T var, unsigned int laneMask) const
    {
        static_assert(
            SupportTypeSimtInternel<T, int32_t, uint32_t, int64_t, uint64_t, float, half, half2>,
            "Input type T only supports int32_t, uint32_t, int64_t, uint64_t, float, half, half2.");
        return asc_shfl_xor(var, laneMask, static_cast<int32_t>(numThreads));
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline int any(int predicate) const
    {
        uint32_t lane_ballot = asc_ballot(predicate) & build_mask();
        return (lane_ballot != 0);
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline int all(int predicate) const
    {
        uint32_t lane_ballot = asc_ballot(predicate) & build_mask();
        return (lane_ballot == build_mask());
    }

    __SIMT_DEVICE_FUNCTIONS_DECL__ inline unsigned int ballot(int predicate) const
    {
        uint32_t lane_ballot = asc_ballot(predicate) & build_mask();
        unsigned int shift = laneid() & (~(numThreads - 1));
        return lane_ballot >> shift;
    }
};

template <unsigned int Size, typename ParentT = void>
class thread_block_tile_impl : public thread_block_tile_base<Size> {
protected:
    __SIMT_DEVICE_FUNCTIONS_DECL__ thread_block_tile_impl() {}
};

template <unsigned int Size>
class thread_block_tile_impl<Size, void> : public thread_block_tile_base<Size>, public thread_group {
    static constexpr unsigned int numThreads = Size;

protected:
    __SIMT_DEVICE_FUNCTIONS_DECL__ thread_block_tile_impl(
        unsigned int meta_group_rank = 0, unsigned int meta_group_size = 1)
    {
        _tiled_info.is_tiled = true;
        _tiled_info.num_threads = Size;
        _tiled_info.mask = thread_block_tile_base<Size>::build_mask();
        _tiled_info.meta_group_rank = meta_group_rank;
        _tiled_info.meta_group_size = meta_group_size;
    }

public:
    __SIMT_DEVICE_FUNCTIONS_DECL__ unsigned int meta_group_rank() const { return _tiled_info.meta_group_rank; }

    __SIMT_DEVICE_FUNCTIONS_DECL__ unsigned int meta_group_size() const { return _tiled_info.meta_group_size; }
};

template <unsigned int Size, typename ParentT>
class thread_block_tile : public thread_block_tile_impl<Size, ParentT> {
public:
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline thread_block_tile(const ParentT& g) : thread_block_tile_impl<Size, ParentT>()
    {}
};

template <unsigned int Size>
class thread_block_tile<Size, void> : public thread_block_tile_impl<Size, void> {
public:
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline thread_block_tile() : thread_block_tile_impl<Size, void>() {}
};

template <unsigned int Size, typename ParentT>
struct tiled_partition_impl;

template <unsigned int Size>
struct tiled_partition_impl<Size, thread_block> : public thread_block_tile<Size, thread_block> {
    __SIMT_DEVICE_FUNCTIONS_DECL__ tiled_partition_impl(const thread_block& g)
        : thread_block_tile<Size, thread_block>(g)
    {}
};

template <unsigned int Size, unsigned int ParentSize, typename GrandParent>
struct tiled_partition_impl<Size, thread_block_tile<ParentSize, GrandParent>>
    : public thread_block_tile<Size, thread_block_tile<ParentSize, GrandParent>> {
    static_assert(Size < ParentSize, "Tile size bigger or equal to the parent group size");
    __SIMT_DEVICE_FUNCTIONS_DECL__ inline tiled_partition_impl(const thread_block_tile<ParentSize, GrandParent>& g)
        : thread_block_tile<Size, thread_block_tile<ParentSize, GrandParent>>(g)
    {}
};

template <unsigned int Size, typename ParentT>
__SIMT_DEVICE_FUNCTIONS_DECL__ inline thread_block_tile<Size, ParentT> tiled_partition(const ParentT& g)
{
    return tiled_partition_impl<Size, ParentT>(g);
}

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_COOPERATIVE_GROUPS_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_COOPERATIVE_GROUPS_H__
#endif

#endif