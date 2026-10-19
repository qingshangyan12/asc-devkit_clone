/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "vector_datamove.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H
#endif

#ifndef INCLUDE_C_API_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H
#define INCLUDE_C_API_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H

#include "c_api/defs/defs.h"
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/vector_datamove_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 9201)
#include "impl/c_api/reg_base_impl/vector_datamove_intf_impl.h"
#endif

#include "c_api/composite/vector_compute_composite.h"

//===========asc_set_copy_pad_val(int8/uint8/int16/uint16/half/bfloat16/int32/uint32/float)===========
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the padding value consumed by subsequent GM-to-UB transfers when constant padding is selected.
 *
 * @override_id asc_set_copy_pad_val__i8
 * @archs 2201 3510
 * @param [in] pad_value 需要填补的数据值。
 */
__aicore__ inline void asc_set_copy_pad_val(int8_t pad_value);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the padding value consumed by subsequent GM-to-UB transfers when constant padding is selected.
 *
 * @override_id asc_set_copy_pad_val__u8
 * @archs 2201 3510
 * @param [in] pad_value 需要填补的数据值。
 */
__aicore__ inline void asc_set_copy_pad_val(uint8_t pad_value);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the padding value consumed by subsequent GM-to-UB transfers when constant padding is selected.
 *
 * @override_id asc_set_copy_pad_val__i16
 * @archs 2201 3510
 * @param [in] pad_value 需要填补的数据值。
 */
__aicore__ inline void asc_set_copy_pad_val(int16_t pad_value);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the padding value consumed by subsequent GM-to-UB transfers when constant padding is selected.
 *
 * @override_id asc_set_copy_pad_val__u16
 * @archs 2201 3510
 * @param [in] pad_value 需要填补的数据值。
 */
__aicore__ inline void asc_set_copy_pad_val(uint16_t pad_value);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the padding value consumed by subsequent GM-to-UB transfers when constant padding is selected.
 *
 * @override_id asc_set_copy_pad_val__f16
 * @archs 2201 3510
 * @param [in] pad_value 需要填补的数据值。
 */
__aicore__ inline void asc_set_copy_pad_val(half pad_value);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the padding value consumed by subsequent GM-to-UB transfers when constant padding is selected.
 *
 * @override_id asc_set_copy_pad_val__bf16
 * @archs 2201 3510
 * @param [in] pad_value 需要填补的数据值。
 */
__aicore__ inline void asc_set_copy_pad_val(bfloat16_t pad_value);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the padding value consumed by subsequent GM-to-UB transfers when constant padding is selected.
 *
 * @override_id asc_set_copy_pad_val__i32
 * @archs 2201 3510
 * @param [in] pad_value 需要填补的数据值。
 */
__aicore__ inline void asc_set_copy_pad_val(int32_t pad_value);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the padding value consumed by subsequent GM-to-UB transfers when constant padding is selected.
 *
 * @override_id asc_set_copy_pad_val__u32
 * @archs 2201 3510
 * @param [in] pad_value 需要填补的数据值。
 */
__aicore__ inline void asc_set_copy_pad_val(uint32_t pad_value);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the padding value consumed by subsequent GM-to-UB transfers when constant padding is selected.
 *
 * @override_id asc_set_copy_pad_val__f32
 * @archs 2201 3510
 * @param [in] pad_value 需要填补的数据值。
 */
__aicore__ inline void asc_set_copy_pad_val(float pad_value);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies data blocks from Unified Buffer (UB) to UB in high-dimensional split mode using burst_count, burst_len, src_gap and dst_gap; output goes to dst.
 *
 * @override_id asc_copy_ub2ub__PUv_PUv_u16_u16_u16_u16
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围：[1, 65535]。
 * @param [in] burst_len 每个连续数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。取值范围：[0, $2^{16}−1$]。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。取值范围：[0, $2^{16}−1$]。
 */
__aicore__ inline void asc_copy_ub2ub(
    __ubuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies size bytes contiguously from Unified Buffer (UB) to UB in the first-n form; the copied data is written to dst.
 *
 * @override_id asc_copy_ub2ub__PUv_PUv_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 待搬运数据大小，单位字节。取值范围：[32, 65535×32]，且需为32的整数倍。
 */
__aicore__ inline void asc_copy_ub2ub(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies the first count int16_t elements from UB src to UB dst; void return, destination holds the copied data.
 *
 * @override_id asc_copy__PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies int16_t elements from UB src to UB dst over repeat iterations with configurable block and repeat strides; void return.
 *
 * @override_id asc_copy__PUi16_PUi16_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_copy(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies the first count uint16_t elements from UB src to UB dst; void return, destination holds the copied data.
 *
 * @override_id asc_copy__PUu16_PUu16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies uint16_t elements from UB src to UB dst over repeat iterations with configurable block and repeat strides; void return.
 *
 * @override_id asc_copy__PUu16_PUu16_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_copy(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies the first count int32_t elements from UB src to UB dst; void return, destination holds the copied data.
 *
 * @override_id asc_copy__PUi32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies int32_t elements from UB src to UB dst over repeat iterations with configurable block and repeat strides; void return.
 *
 * @override_id asc_copy__PUi32_PUi32_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_copy(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies the first count uint32_t elements from UB src to UB dst; void return, destination holds the copied data.
 *
 * @override_id asc_copy__PUu32_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies uint32_t elements from UB src to UB dst over repeat iterations with configurable block and repeat strides; void return.
 *
 * @override_id asc_copy__PUu32_PUu32_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_copy(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub
 * @brief Copies data from Global Memory (GM) to Unified Buffer (UB) in high-dimensional split mode using burst_count, burst_len, src_gap and dst_gap; output goes to dst.
 *
 * @override_id asc_copy_gm2ub__PUv_PGv_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2ub(
    __ubuf__ void* dst, __gm__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub
 * @brief Copies data from GM to UB as a contiguous byte range.
 *
 * @override_id asc_copy_gm2ub__PUv_PGv_u32
 * @archs 2201
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub(__ubuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm
 * @brief Copies data from UB to GM as a contiguous byte range.
 *
 * @override_id asc_copy_ub2gm__PGv_PUv_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_ub2gm(__gm__ void* dst, __ubuf__ void* src, uint32_t size);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm
 * @brief Copies data from Unified Buffer (UB) to Global Memory (GM) in high-dimensional split mode using burst_count, burst_len, src_gap and dst_gap; output goes to dst.
 *
 * @override_id asc_copy_ub2gm__PGv_PUv_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的GM地址。
 * @param [in] src 源UB地址。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_ub2gm(
    __gm__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);

// asc_copy_gm2ub_align  int8_t / uint8_t / half / bfloat16_t / int16_t / uint16_t / float / int32_t / uint32_t
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies burst_count blocks of burst_len bytes of int8_t data from GM (src) to UB (dst) in Normal mode, padding each block with a manual or hardware-generated fill.
 *
 * @override_id asc_copy_gm2ub_align__PUi8_PGi8_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUi8_PGi8_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies burst_count blocks of burst_len bytes of uint8_t data from GM (src) to UB (dst) in Normal mode, padding each block with a manual or hardware-generated fill.
 *
 * @override_id asc_copy_gm2ub_align__PUu8_PGu8_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUu8_PGu8_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies burst_count blocks of burst_len bytes of half data from GM (src) to UB (dst) in Normal mode, padding each block with a manual or hardware-generated fill.
 *
 * @override_id asc_copy_gm2ub_align__PUf16_PGf16_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ half* dst, __gm__ half* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUf16_PGf16_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ half* dst, __gm__ half* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies burst_count blocks of burst_len bytes of bfloat16_t data from GM (src) to UB (dst) in Normal mode, padding each block with a manual or hardware-generated fill.
 *
 * @override_id asc_copy_gm2ub_align__PUbf16_PGbf16_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUbf16_PGbf16_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies burst_count blocks of burst_len bytes of int16_t data from GM (src) to UB (dst) in Normal mode, padding each block with a manual or hardware-generated fill.
 *
 * @override_id asc_copy_gm2ub_align__PUi16_PGi16_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUi16_PGi16_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies burst_count blocks of burst_len bytes of uint16_t data from GM (src) to UB (dst) in Normal mode, padding each block with a manual or hardware-generated fill.
 *
 * @override_id asc_copy_gm2ub_align__PUu16_PGu16_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUu16_PGu16_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies burst_count blocks of burst_len bytes of float data from GM (src) to UB (dst) in Normal mode, padding each block with a manual or hardware-generated fill.
 *
 * @override_id asc_copy_gm2ub_align__PUf32_PGf32_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ float* dst, __gm__ float* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUf32_PGf32_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ float* dst, __gm__ float* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies burst_count blocks of burst_len bytes of int32_t data from GM (src) to UB (dst) in Normal mode, padding each block with a manual or hardware-generated fill.
 *
 * @override_id asc_copy_gm2ub_align__PUi32_PGi32_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUi32_PGi32_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies burst_count blocks of burst_len bytes of uint32_t data from GM (src) to UB (dst) in Normal mode, padding each block with a manual or hardware-generated fill.
 *
 * @override_id asc_copy_gm2ub_align__PUu32_PGu32_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUu32_PGu32_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size);

// asc_copy_ub2gm_align
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of uint8_t data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGu8_PUu8_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of uint8_t data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGu8_PUu8_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of int8_t data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGi8_PUi8_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of int8_t data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGi8_PUi8_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of half data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGf16_PUf16_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ half* dst, __ubuf__ half* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of half data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGf16_PUf16_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ half* dst, __ubuf__ half* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of uint16_t data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGu16_PUu16_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of uint16_t data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGu16_PUu16_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of int16_t data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGi16_PUi16_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of int16_t data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGi16_PUi16_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of bfloat16_t data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGbf16_PUbf16_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint16_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of bfloat16_t data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGbf16_PUbf16_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of uint32_t data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGu32_PUu32_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of uint32_t data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGu32_PUu32_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of float data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGf32_PUf32_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ float* dst, __ubuf__ float* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of float data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGf32_PUf32_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ float* dst, __ubuf__ float* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of int32_t data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGi32_PUi32_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of int32_t data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGi32_PUi32_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of double data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGf64_PUf64_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ double* dst, __ubuf__ double* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of double data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGf64_PUf64_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ double* dst, __ubuf__ double* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of int64_t data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGi64_PUi64_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int64_t* dst, __ubuf__ int64_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of int64_t data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGi64_PUi64_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ int64_t* dst, __ubuf__ int64_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of uint64_t data from UB (src) to GM (dst) in Normal mode, padding each non-32-byte-aligned block with dummy data discarded on write.
 *
 * @override_id asc_copy_ub2gm_align__PGu64_PUu64_u16_u32_u8_u8_u32_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[0, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[0, 2097151]。
 * @param [in] left_padding_num 保留参数，必须设置为0。
 * @param [in] right_padding_num 保留参数，必须设置为0。
 * @param [in] src_gap 源操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的距离（前面一个数据块的尾与后面一个数据块的头的间隔），单位为字节。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of uint64_t data from UB (src) to GM (dst) in the first-n form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGu64_PUu64_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint32_t size);

// ==========asc_copy_gm2ub==========
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub
 * @brief Copies data from GM to UB in multidimensional blocks.
 *
 * @override_id asc_copy_gm2ub__PUv_PGv_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{16}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{16}−1$]。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{16}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{16}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub(
    __ubuf__ void* dst, __gm__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_stride,
    uint16_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm
 * @brief Copies data from UB to GM in multidimensional blocks.
 *
 * @override_id asc_copy_ub2gm__PGv_PUv_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{16}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{16}−1$]。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{16}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{16}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm(
    __gm__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t dst_stride,
    uint16_t src_stride);
// ==========asc_copy_gm2ub_align(int8_t/uint8_t/fp8_e5m2_t/fp8_e4m3fn_t/hifloat8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUu8_PGu8_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUi8_PGi8_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUu16_PGu16_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUi16_PGi16_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUu32_PGu32_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUi32_PGi32_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUf16_PGf16_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUf32_PGf32_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUhf8_PGhf8_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUbf16_PGbf16_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUf8e5m2_PGf8e5m2_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB in multidimensional blocks, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUf8e4m3_PGf8e4m3_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] left_padding_num 连续搬运数据块左侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] right_padding_num 连续搬运数据块右侧需要补充的元素个数。该参数对应的填充数据大小不能超过32字节。Compact模式下需要设置为0。
 * @param [in] enable_constant_pad 当`left_padding_num`和`right_padding_num`均为0时，配置非对齐场景的填充方式。取值说明如下：   &bull; `true`：手动填充，填充值为接口`asc_set_copy_pad_val`设置的值。  &bull; `false`：自动填充，由硬件填充dummy假数据，dummy假数据的值为数据块的第一个元素的值。 当`left_padding_num`或`right_padding_num`非0时，该参数不生效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，目的数据块在UB中紧密排列，`dst_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`dst_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUhf8_PGhf8_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUf8e5m2_PGf8e5m2_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @brief Copies data from GM to UB as a contiguous byte range, supporting non-aligned lengths and padding.
 *
 * @override_id asc_copy_gm2ub_align__PUf8e4m3_PGf8e4m3_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t size);

// ==========asc_copy_ub2gm_align(int8_t/uint8_t/fp8_e5m2_t/fp8_e4m3fn_t/hifloat8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of uint8_t data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGu8_PUu8_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of int8_t data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGi8_PUi8_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of uint16_t data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGu16_PUu16_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of int16_t data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGi16_PUi16_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of uint32_t data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGu32_PUu32_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of int32_t data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGi32_PUi32_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of half data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGf16_PUf16_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ half* dst, __ubuf__ half* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of float data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGf32_PUf32_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ float* dst, __ubuf__ float* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of hifloat8_t data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGhf8_PUhf8_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of bfloat16_t data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGbf16_PUbf16_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of fp8_e5m2_t data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGf8e5m2_PUf8e5m2_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies burst_count blocks of burst_len bytes of fp8_e4m3fn_t data from UB (src) to GM (dst) with l2_cache_mode and strides; src_stride selects Normal or Compact mode.
 *
 * @override_id asc_copy_ub2gm_align__PGf8e4m3_PUf8e4m3_u32_u32_storel2_u64_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, $2^{21}−1$]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, $2^{21}−1$]。
 * @param [in] l2_cache_mode [asc_store_l2_cache_mode](../../defs/enum/asc_store_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。取值范围：[0, $2^{40}−1$]。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 * @param [in] src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节，用于选择数据搬运模式。取值范围：[0, $2^{21}−1$]。 &bull; 等于`burst_len`：Compact模式，源数据块在UB中紧密排列，`src_stride`支持字节对齐。 &bull; 不等于`burst_len`：Normal模式，`src_stride`需要满足32字节对齐要求。 只搬运1个数据块，即`burst_count`设置为1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of hifloat8_t data from UB (src) to GM (dst) in the continuous form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGhf8_PUhf8_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of fp8_e5m2_t data from UB (src) to GM (dst) in the continuous form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGf8e5m2_PUf8e5m2_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @brief Copies size bytes of fp8_e4m3fn_t data from UB (src) to GM (dst) in the continuous form, padding non-32-byte-aligned reads with dummy data that is discarded when writing to GM.
 *
 * @override_id asc_copy_ub2gm_align__PGf8e4m3_PUf8e4m3_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[1, $2^{21}−1$]。
 */
__aicore__ inline void asc_copy_ub2gm_align(__gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the inner loop1 and outer loop2 iteration counts used by asc_copy_ub2gm_align in loop-copy mode; reset them to 1 after the copy.
 *
 * @override_id asc_set_ub2gm_loop_size__u32_u32
 * @archs 3510
 * @param [in] loop1_size 内层循环`loop1`的迭代次数。取值范围为[0, $2^{21}−1$]。
 * @param [in] loop2_size 外层循环`loop2`的迭代次数。取值范围为[0, $2^{21}−1$]。
 */
__aicore__ inline void asc_set_ub2gm_loop_size(uint32_t loop1_size, uint32_t loop2_size);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the source and destination data-block offsets between adjacent iterations of the inner loop1 used by asc_copy_ub2gm_align in loop-copy mode.
 *
 * @override_id asc_set_ub2gm_loop1_stride__u64_u64
 * @archs 3510
 * @param [in] loop1_src_stride 内层循环中，相邻两次迭代源操作数数据块起始地址之间的偏移量，单位为字节。取值范围为[0, $2^{21}−1$]，且必须32字节对齐。
 * @param [in] loop1_dst_stride 内层循环中，相邻两次迭代目的操作数数据块起始地址之间的偏移量，单位为字节。取值范围为[0, $2^{40}−1$]。
 */
__aicore__ inline void asc_set_ub2gm_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the source and destination data-block offsets between adjacent iterations of the outer loop2 used by asc_copy_ub2gm_align in loop-copy mode.
 *
 * @override_id asc_set_ub2gm_loop2_stride__u64_u64
 * @archs 3510
 * @param [in] loop2_src_stride 外层循环中，相邻两次迭代源操作数数据块起始地址之间的偏移量，单位为字节。取值范围为[0, $2^{21}−1$]，且必须32字节对齐。
 * @param [in] loop2_dst_stride 外层循环中，相邻两次迭代目的操作数数据块起始地址之间的偏移量，单位为字节。取值范围为[0, $2^{40}−1$]。
 */
__aicore__ inline void asc_set_ub2gm_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the inner loop1 and outer loop2 iteration counts used by asc_copy_gm2ub_align in loop-copy mode; reset them to 1 after the copy.
 *
 * @override_id asc_set_gm2ub_loop_size__u64_u64
 * @archs 3510
 * @param [in] loop1_size 内层循环的循环次数，取值范围为[0, $2^{21}−1$]。
 * @param [in] loop2_size 外层循环的循环次数，取值范围为[0, $2^{22}−1$]。
 */
__aicore__ inline void asc_set_gm2ub_loop_size(uint64_t loop1_size, uint64_t loop2_size);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the source and destination data-block offsets between adjacent iterations of the inner loop1 used by asc_copy_gm2ub_align in loop-copy mode.
 *
 * @override_id asc_set_gm2ub_loop1_stride__u64_u64
 * @archs 3510
 * @param [in] loop1_src_stride 内层循环中相邻迭代源操作数的数据块间隔，单位为字节，取值范围为[0, $2^{40}−1$]。
 * @param [in] loop1_dst_stride 内层循环中相邻迭代目的操作数的数据块间隔，单位为字节，取值范围为[0, $2^{21}−1$]，且必须32字节对齐。
 */
__aicore__ inline void asc_set_gm2ub_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the source and destination data-block offsets between adjacent iterations of the outer loop2 used by asc_copy_gm2ub_align in loop-copy mode.
 *
 * @override_id asc_set_gm2ub_loop2_stride__u64_u64
 * @archs 3510
 * @param [in] loop2_src_stride 外层循环中相邻迭代源操作数的数据块间隔，单位为字节，取值范围为[0, $2^{40}−1$]。
 * @param [in] loop2_dst_stride 外层循环中相邻迭代目的操作数的数据块间隔，单位为字节，取值范围为[0, $2^{21}−1$]，且必须32字节对齐。
 */
__aicore__ inline void asc_set_gm2ub_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride);
// ==========asc_set_ndim_loop_stride==========
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the innermost loop0 destination and source element strides used by NDDMA copies in asc_ndim_copy_gm2ub; set before the copy; effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_loop0_stride__u64_u64
 * @archs 3510
 * @param [in] dst_stride 同一循环维度内，相邻两次迭代所处理的目的操作数元素起始地址之间的偏移量，单位为元素个数。取值范围为[0, $2^{20}−1$]。
 * @param [in] src_stride 同一循环维度内，相邻两次迭代所处理的源操作数元素起始地址之间的偏移量，单位为元素个数。取值范围为[0, $2^{40}−1$]。
 */
__aicore__ inline void asc_set_ndim_loop0_stride(uint64_t dst_stride, uint64_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the loop1 destination and source element strides used by NDDMA copies in asc_ndim_copy_gm2ub; set before the copy; effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_loop1_stride__u64_u64
 * @archs 3510
 * @param [in] dst_stride 同一循环维度内，相邻两次迭代所处理的目的操作数元素起始地址之间的偏移量，单位为元素个数。取值范围为[0, $2^{20}−1$]。
 * @param [in] src_stride 同一循环维度内，相邻两次迭代所处理的源操作数元素起始地址之间的偏移量，单位为元素个数。取值范围为[0, $2^{40}−1$]。
 */
__aicore__ inline void asc_set_ndim_loop1_stride(uint64_t dst_stride, uint64_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the loop2 destination and source element strides used by NDDMA copies in asc_ndim_copy_gm2ub; set before the copy; effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_loop2_stride__u64_u64
 * @archs 3510
 * @param [in] dst_stride 同一循环维度内，相邻两次迭代所处理的目的操作数元素起始地址之间的偏移量，单位为元素个数。取值范围为[0, $2^{20}−1$]。
 * @param [in] src_stride 同一循环维度内，相邻两次迭代所处理的源操作数元素起始地址之间的偏移量，单位为元素个数。取值范围为[0, $2^{40}−1$]。
 */
__aicore__ inline void asc_set_ndim_loop2_stride(uint64_t dst_stride, uint64_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the loop3 destination and source element strides used by NDDMA copies in asc_ndim_copy_gm2ub; set before the copy; effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_loop3_stride__u64_u64
 * @archs 3510
 * @param [in] dst_stride 同一循环维度内，相邻两次迭代所处理的目的操作数元素起始地址之间的偏移量，单位为元素个数。取值范围为[0, $2^{20}−1$]。
 * @param [in] src_stride 同一循环维度内，相邻两次迭代所处理的源操作数元素起始地址之间的偏移量，单位为元素个数。取值范围为[0, $2^{40}−1$]。
 */
__aicore__ inline void asc_set_ndim_loop3_stride(uint64_t dst_stride, uint64_t src_stride);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the outermost loop4 destination and source element strides used by NDDMA copies in asc_ndim_copy_gm2ub; set before the copy; effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_loop4_stride__u64_u64
 * @archs 3510
 * @param [in] dst_stride 同一循环维度内，相邻两次迭代所处理的目的操作数元素起始地址之间的偏移量，单位为元素个数。取值范围为[0, $2^{20}−1$]。
 * @param [in] src_stride 同一循环维度内，相邻两次迭代所处理的源操作数元素起始地址之间的偏移量，单位为元素个数。取值范围为[0, $2^{40}−1$]。
 */
__aicore__ inline void asc_set_ndim_loop4_stride(uint64_t dst_stride, uint64_t src_stride);
//===========asc_set_ndim_pad_count===========
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Configures the left/right padding element counts of dimensions loop1 to loop4 for NDDMA high-dimensional copies consumed by asc_ndim_copy_gm2ub; AIV only.
 *
 * @override_id asc_set_ndim_pad_count__Rpadcountcfg
 * @archs 3510
 * @param [in] config NDDMA高维切分搬运的填充元素数配置，类型为`asc_ndim_pad_count_config&`。详细字段说明请参见[asc_ndim_pad_count_config](../defs/union/asc_ndim_pad_count_config.md)。
 */
__aicore__ inline void asc_set_ndim_pad_count(asc_ndim_pad_count_config& config);
//===========asc_set_ndim_pad_value(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float)===========
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the int8_t constant pad value used by later asc_ndim_copy_gm2ub calls in constant-fill mode (padding_mode true); effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_pad_value__i8
 * @archs 3510
 * @param [in] pad_value 常数填充值。当`padding_mode`设置为`true`时，后续调用`asc_ndim_copy_gm2ub`将使用该值填充搬运数据的边界区域。对于本接口直接支持的数据类型，`pad_value`的数据类型应与搬运数据类型一致。
 */
__aicore__ inline void asc_set_ndim_pad_value(int8_t pad_value);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the uint8_t constant pad value used by later asc_ndim_copy_gm2ub calls in constant-fill mode (padding_mode true); effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_pad_value__u8
 * @archs 3510
 * @param [in] pad_value 常数填充值。当`padding_mode`设置为`true`时，后续调用`asc_ndim_copy_gm2ub`将使用该值填充搬运数据的边界区域。对于本接口直接支持的数据类型，`pad_value`的数据类型应与搬运数据类型一致。
 */
__aicore__ inline void asc_set_ndim_pad_value(uint8_t pad_value);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the int16_t constant pad value used by later asc_ndim_copy_gm2ub calls in constant-fill mode (padding_mode true); effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_pad_value__i16
 * @archs 3510
 * @param [in] pad_value 常数填充值。当`padding_mode`设置为`true`时，后续调用`asc_ndim_copy_gm2ub`将使用该值填充搬运数据的边界区域。对于本接口直接支持的数据类型，`pad_value`的数据类型应与搬运数据类型一致。
 */
__aicore__ inline void asc_set_ndim_pad_value(int16_t pad_value);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the uint16_t constant pad value used by later asc_ndim_copy_gm2ub calls in constant-fill mode (padding_mode true); effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_pad_value__u16
 * @archs 3510
 * @param [in] pad_value 常数填充值。当`padding_mode`设置为`true`时，后续调用`asc_ndim_copy_gm2ub`将使用该值填充搬运数据的边界区域。对于本接口直接支持的数据类型，`pad_value`的数据类型应与搬运数据类型一致。
 */
__aicore__ inline void asc_set_ndim_pad_value(uint16_t pad_value);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the half constant pad value used by later asc_ndim_copy_gm2ub calls in constant-fill mode (padding_mode true); effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_pad_value__f16
 * @archs 3510
 * @param [in] pad_value 常数填充值。当`padding_mode`设置为`true`时，后续调用`asc_ndim_copy_gm2ub`将使用该值填充搬运数据的边界区域。对于本接口直接支持的数据类型，`pad_value`的数据类型应与搬运数据类型一致。
 */
__aicore__ inline void asc_set_ndim_pad_value(half pad_value);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the bfloat16_t constant pad value used by later asc_ndim_copy_gm2ub calls in constant-fill mode (padding_mode true); effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_pad_value__bf16
 * @archs 3510
 * @param [in] pad_value 常数填充值。当`padding_mode`设置为`true`时，后续调用`asc_ndim_copy_gm2ub`将使用该值填充搬运数据的边界区域。对于本接口直接支持的数据类型，`pad_value`的数据类型应与搬运数据类型一致。
 */
__aicore__ inline void asc_set_ndim_pad_value(bfloat16_t pad_value);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the int32_t constant pad value used by later asc_ndim_copy_gm2ub calls in constant-fill mode (padding_mode true); effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_pad_value__i32
 * @archs 3510
 * @param [in] pad_value 常数填充值。当`padding_mode`设置为`true`时，后续调用`asc_ndim_copy_gm2ub`将使用该值填充搬运数据的边界区域。对于本接口直接支持的数据类型，`pad_value`的数据类型应与搬运数据类型一致。
 */
__aicore__ inline void asc_set_ndim_pad_value(int32_t pad_value);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the uint32_t constant pad value used by later asc_ndim_copy_gm2ub calls in constant-fill mode (padding_mode true); effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_pad_value__u32
 * @archs 3510
 * @param [in] pad_value 常数填充值。当`padding_mode`设置为`true`时，后续调用`asc_ndim_copy_gm2ub`将使用该值填充搬运数据的边界区域。对于本接口直接支持的数据类型，`pad_value`的数据类型应与搬运数据类型一致。
 */
__aicore__ inline void asc_set_ndim_pad_value(uint32_t pad_value);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Sets the float constant pad value used by later asc_ndim_copy_gm2ub calls in constant-fill mode (padding_mode true); effective on AIV only, void return.
 *
 * @override_id asc_set_ndim_pad_value__f32
 * @archs 3510
 * @param [in] pad_value 常数填充值。当`padding_mode`设置为`true`时，后续调用`asc_ndim_copy_gm2ub`将使用该值填充搬运数据的边界区域。对于本接口直接支持的数据类型，`pad_value`的数据类型应与搬运数据类型一致。
 */
__aicore__ inline void asc_set_ndim_pad_value(float pad_value);
//===========asc_ndim_copy_gm2ub(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float)===========
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional int8_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUi8_PGi8_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional uint8_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUu8_PGu8_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional fp4x2_e2m1_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf4e2m1x2_PGf4e2m1x2_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional fp4x2_e1m2_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf4e1m2x2_PGf4e1m2x2_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional fp8_e8m0_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf8e8m0_PGf8e8m0_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e8m0_t* dst, __gm__ fp8_e8m0_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional fp8_e5m2_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf8e5m2_PGf8e5m2_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional fp8_e4m3fn_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf8e4m3_PGf8e4m3_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional int16_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUi16_PGi16_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional uint16_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUu16_PGu16_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional half data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf16_PGf16_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ half* dst, __gm__ half* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional bfloat16_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUbf16_PGbf16_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional int32_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUi32_PGi32_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional uint32_t data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUu32_PGu32_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies up to 5-dimensional float data from GM (src) to UB (dst) with configurable per-dimension strides and on-the-fly padding; companion interfaces set strides and padding.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf32_PGf32_u32_u32_u32_u32_u32_u8_u8_b_loadl2
 * @archs 3510
 * @param [out] dst 目的UB的起始地址，指向padding后数据的起始位置。需要1字节对齐。
 * @param [in] src 源GM的起始地址。需要按dtype字节对齐（b8类型要求1字节对齐、b16类型要求2字节对齐、b32类型要求4字节对齐）。
 * @param [in] loop0_size 0维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop1_size 1维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop2_size 2维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop3_size 3维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop4_size 4维处理的元素个数，不包含Padding元素。 单位为元素个数，取值范围为[1, $2^{20}−1$]。
 * @param [in] loop0_lp_count 0维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 0维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode Padding值的填充方式。 &bull; `true`：使用常数填充，所有Padding值均为固定值`pad_value`。 &bull; `false`：使用最近值填充，左右Padding值选取当前维度最左侧或最右侧的值。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，用于配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ float* dst, __gm__ float* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
//======asc_ndim_copy_dci=====
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Refreshes the NDDMA DataCache so multiple AI Cores reading the same GM address get fresh data; must be called before asc_ndim_copy_gm2ub.
 *
 * @override_id asc_ndim_copy_dci__z
 * @archs 3510
 */
__aicore__ inline void asc_ndim_copy_dci();
//======asc_copy_ub2l1=======
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies data blocks from Unified Buffer (UB) to L1 Buffer in high-dimensional split mode using burst_count, burst_len, src_gap and dst_gap.
 *
 * @override_id asc_copy_ub2l1__PL1v_PUv_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L1 Buffer的起始地址。需要32字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] burst_count 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] burst_len 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, $2^{16}−1$]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。取值范围：[0, $2^{16}−1$]。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔），单位为DataBlock（32字节）。取值范围：[0, $2^{16}−1$]。
 */
__aicore__ inline void asc_copy_ub2l1(
    __cbuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap);
/**
 * @ingroup simd_c_api.vector_datamove
 * @brief Copies a contiguous size-byte range from Unified Buffer (UB) to L1 Buffer; the copied data is written to the dst parameter.
 *
 * @override_id asc_copy_ub2l1__PL1v_PUv_u32
 * @archs 3510
 * @param [out] dst 目的L1 Buffer的起始地址。需要32字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[32, 4095×32]，且需为32的整数倍。
 */
__aicore__ inline void asc_copy_ub2l1(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ half* dst, __ubuf__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ float* dst, __ubuf__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional int8_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUi8_PGi8_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional uint8_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUu8_PGu8_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional fp4x2_e2m1_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf4e2m1x2_PGf4e2m1x2_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional fp4x2_e1m2_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf4e1m2x2_PGf4e1m2x2_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional fp8_e8m0_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf8e8m0_PGf8e8m0_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e8m0_t* dst, __gm__ fp8_e8m0_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional fp8_e5m2_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf8e5m2_PGf8e5m2_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional fp8_e4m3fn_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf8e4m3_PGf8e4m3_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional int16_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUi16_PGi16_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional uint16_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUu16_PGu16_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional half data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf16_PGf16_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ half* dst, __gm__ half* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional bfloat16_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUbf16_PGbf16_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional int32_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUi32_PGi32_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional uint32_t data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUu32_PGu32_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload: copies up to 5-dimensional float data from GM (src) to UB (dst) with configurable per-dimension strides and padding; use asc_load_l2_cache_mode instead.
 *
 * @override_id asc_ndim_copy_gm2ub__PUf32_PGf32_u32_u32_u32_u32_u32_u8_u8_b_u8
 * @archs 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] loop0_size 表示0维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop1_size 表示1维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop2_size 表示2维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop3_size 表示3维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop4_size 表示4维处理的元素个数（不包含Padding元素）。  单位为元素个数，取值范围为[1, 2^20-1]。
 * @param [in] loop0_lp_count 表示0维左侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] loop0_rp_count 表示0维右侧需要补齐的元素个数。  单位为元素个数，取值范围为[0, 255]。
 * @param [in] padding_mode 表示Padding值填取方式。 &bull; true：使用常数填充方式，即所有Padding值为固定值pad_value。 &bull; false：使用最近值填充方式，即左右Padding值会选取当前维度最左或最右的值进行填充。
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ float* dst, __gm__ float* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

//======asc_set_gm2ub_pad=======
ASC_DEPRECATED(9.1.0, "2027/09/07", asc_set_copy_pad_val)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated interface that sets the padding value filled on both sides of blocks copied non-aligned from Global Memory (GM) to Unified Buffer (UB); use asc_set_copy_pad_val.
 *
 * @override_id asc_set_gm2ub_pad__u32
 * @archs 3510
 * @param [in] pad_val 左右两侧需要填补的数据值。
 */
__aicore__ inline void asc_set_gm2ub_pad(uint32_t pad_val);

#if (ASC_DEVKIT_VERSION_NUM >= 90200000)
// ==========asc_scatter_ub2gm===========
__aicore__ inline void asc_scatter_ub2gm(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint8_t offset_type, __ubuf__ void* offset_src,
    asc_store_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num);

__aicore__ inline void asc_scatter_ub2gm(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t offset_type, __ubuf__ void* offset_src,
    asc_store_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num);

__aicore__ inline void asc_scatter_ub2gm(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint8_t offset_type, __ubuf__ void* offset_src,
    asc_store_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num);

__aicore__ inline void asc_scatter_ub2gm(
    __gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint8_t offset_type, __ubuf__ void* offset_src,
    asc_store_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t src_stride, uint32_t offset_num);

// ==========asc_gather_gm2ub===========
__aicore__ inline void asc_gather_gm2ub(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offset_src, asc_load_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t dst_stride,
    uint32_t offset_num);

__aicore__ inline void asc_gather_gm2ub(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offset_src, asc_load_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t dst_stride,
    uint32_t offset_num);

__aicore__ inline void asc_gather_gm2ub(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offset_src, asc_load_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t dst_stride,
    uint32_t offset_num);

__aicore__ inline void asc_gather_gm2ub(
    __ubuf__ uint64_t* dst, __gm__ uint64_t* src, uint8_t offset_type, bool constant_padding_ctl,
    __ubuf__ void* offset_src, asc_load_l2_cache_mode l2_cache_mode, uint32_t burst_len, uint32_t dst_stride,
    uint32_t offset_num);
#endif

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H

#endif
