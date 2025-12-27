/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file device_shlf_functions_impl.h
 * \brief
 */
#ifndef IMPL_SIMT_API_DEVICE_SHLF_FUNCTIONS_IMPL_H
#define IMPL_SIMT_API_DEVICE_SHLF_FUNCTIONS_IMPL_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_DEVICE_SHLF_FUNCTIONS_IMPL__
#warning "device_shlf_functions_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "device_shlf_functions.h" and use public functions or variables defined in interface header files."
#endif

// clamp the max source lane, except up mode
constexpr int32_t MAX_OFFSET_OF_MODE = 0x1f;

// clamp the max source lane for up mode
constexpr int32_t MAX_OFFSET_OF_UP_MODE = 0;

// the start position of max offset and lane mask
constexpr int32_t MAX_OFFSET_START_POS = 8;
constexpr int32_t LANE_MASK_START_POS = 16;

__simt_callee__ inline int32_t asc_shfl(int32_t var, int32_t src_lane, int32_t width)
{
    return bisheng::cce::simt::__shfl(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (src_lane));
}

__simt_callee__ inline uint32_t asc_shfl(uint32_t var, int32_t src_lane, int32_t width)
{
    return bisheng::cce::simt::__shfl(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (src_lane));
}

__simt_callee__ inline float asc_shfl(float var, int32_t src_lane, int32_t width)
{
    return bisheng::cce::simt::__shfl(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (src_lane));
}

__simt_callee__ inline int64_t asc_shfl(int64_t var, int32_t src_lane, int32_t width)
{
    return bisheng::cce::simt::__shfl(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (src_lane));
}

__simt_callee__ inline uint64_t asc_shfl(uint64_t var, int32_t src_lane, int32_t width)
{
    return bisheng::cce::simt::__shfl(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (src_lane));
}

__simt_callee__ inline half asc_shfl(half var, int32_t src_lane, int32_t width)
{
    return bisheng::cce::simt::__shfl(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (src_lane));
}

__simt_callee__ inline half2 asc_shfl(half2 var, int32_t src_lane, int32_t width)
{
    return bisheng::cce::simt::__shfl(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (src_lane));
}

__simt_callee__ inline int32_t asc_shfl_up(int32_t var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_up(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_UP_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline uint32_t asc_shfl_up(uint32_t var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_up(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_UP_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline float asc_shfl_up(float var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_up(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_UP_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline int64_t asc_shfl_up(int64_t var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_up(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_UP_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline uint64_t asc_shfl_up(uint64_t var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_up(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_UP_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline half asc_shfl_up(half var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_up(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_UP_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline half2 asc_shfl_up(half2 var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_up(var,
        ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_UP_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline int32_t asc_shfl_down(int32_t var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_down(var,
                       ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline uint32_t asc_shfl_down(uint32_t var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_down(var,
                       ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline float asc_shfl_down(float var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_down(var,
                       ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline int64_t asc_shfl_down(int64_t var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_down(var,
                       ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline uint64_t asc_shfl_down(uint64_t var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_down(var,
                       ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline half asc_shfl_down(half var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_down(var,
                       ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline half2 asc_shfl_down(half2 var, uint32_t delta, int32_t width)
{
    return bisheng::cce::simt::__shfl_down(var,
                       ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (delta));
}

__simt_callee__ inline int32_t asc_shfl_xor(int32_t var, int32_t lane_mask, int32_t width)
{
    return bisheng::cce::simt::__shfl_xor(var,
                      ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (lane_mask));
}

__simt_callee__ inline uint32_t asc_shfl_xor(uint32_t var, int32_t lane_mask, int32_t width)
{
    return bisheng::cce::simt::__shfl_xor(var,
                      ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (lane_mask));
}

__simt_callee__ inline float asc_shfl_xor(float var, int32_t lane_mask, int32_t width)
{
    return bisheng::cce::simt::__shfl_xor(var,
                      ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (lane_mask));
}

__simt_callee__ inline int64_t asc_shfl_xor(int64_t var, int32_t lane_mask, int32_t width)
{
    return bisheng::cce::simt::__shfl_xor(var,
                      ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (lane_mask));
}

__simt_callee__ inline uint64_t asc_shfl_xor(uint64_t var, int32_t lane_mask, int32_t width)
{
    return bisheng::cce::simt::__shfl_xor(var,
                      ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (lane_mask));
}

__simt_callee__ inline half asc_shfl_xor(half var, int32_t lane_mask, int32_t width)
{
    return bisheng::cce::simt::__shfl_xor(var,
                      ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (lane_mask));
}

__simt_callee__ inline half2 asc_shfl_xor(half2 var, int32_t lane_mask, int32_t width)
{
    return bisheng::cce::simt::__shfl_xor(var,
                      ((warpSize - width) << LANE_MASK_START_POS) | (MAX_OFFSET_OF_MODE << MAX_OFFSET_START_POS) | (lane_mask));
}

__simt_callee__ inline int32_t asc_reduce_add(int32_t val)
{
    return bisheng::cce::simt::__reduce_add(val);
}

__simt_callee__ inline uint32_t asc_reduce_add(uint32_t val)
{
    return bisheng::cce::simt::__reduce_add(val);
}

__simt_callee__ inline float asc_reduce_add(float val)
{
    return bisheng::cce::simt::__reduce_add(val);
}

__simt_callee__ inline half asc_reduce_add(half val)
{
    return bisheng::cce::simt::__reduce_add(val);
}

__simt_callee__ inline int32_t asc_reduce_max(int32_t val)
{
    return bisheng::cce::simt::__reduce_max(val);
}

__simt_callee__ inline uint32_t asc_reduce_max(uint32_t val)
{
    return bisheng::cce::simt::__reduce_max(val);
}

__simt_callee__ inline float asc_reduce_max(float val)
{
    return bisheng::cce::simt::__reduce_max(val);
}

__simt_callee__ inline half asc_reduce_max(half val)
{
    return bisheng::cce::simt::__reduce_max(val);
}

__simt_callee__ inline int32_t asc_reduce_min(int32_t val)
{
    return bisheng::cce::simt::__reduce_min(val);
}

__simt_callee__ inline uint32_t asc_reduce_min(uint32_t val)
{
    return bisheng::cce::simt::__reduce_min(val);
}

__simt_callee__ inline float asc_reduce_min(float val)
{
    return bisheng::cce::simt::__reduce_min(val);
}

__simt_callee__ inline half asc_reduce_min(half val)
{
    return bisheng::cce::simt::__reduce_min(val);
}

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_DEVICE_SHLF_FUNCTIONS_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_DEVICE_SHLF_FUNCTIONS_IMPL__
#endif

#endif  // IMPL_SIMT_API_DEVICE_SHLF_FUNCTIONS_IMPL_H