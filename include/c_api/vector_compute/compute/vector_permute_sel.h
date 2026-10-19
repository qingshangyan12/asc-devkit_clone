/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "vector_compute/compute/vector_permute_sel.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/vector_permute_sel_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/vector_permute_sel_intf_impl.h"
#endif
/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Sets the 16 transpose int8_t DataBlock addresses by associating the src_array address list with the given VA address register, first 8 and last 8 separately (void return, PIPE_V).
 *
 * @override_id asc_set_va_reg__uba8_PPUi8
 * @archs 2201 3510
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为： &bull; VA0 &bull; VA1 &bull; VA2 &bull; VA3 &bull; VA4 &bull; VA5 &bull; VA6 &bull; VA7 数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int8_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Sets the 16 transpose uint8_t DataBlock addresses by associating the src_array address list with the given VA address register, first 8 and last 8 separately (void return, PIPE_V).
 *
 * @override_id asc_set_va_reg__uba8_PPUu8
 * @archs 2201 3510
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为： &bull; VA0 &bull; VA1 &bull; VA2 &bull; VA3 &bull; VA4 &bull; VA5 &bull; VA6 &bull; VA7 数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint8_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Sets the 16 transpose int16_t DataBlock addresses by associating the src_array address list with the given VA address register, first 8 and last 8 separately (void return, PIPE_V).
 *
 * @override_id asc_set_va_reg__uba8_PPUi16
 * @archs 2201 3510
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为： &bull; VA0 &bull; VA1 &bull; VA2 &bull; VA3 &bull; VA4 &bull; VA5 &bull; VA6 &bull; VA7 数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int16_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Sets the 16 transpose uint16_t DataBlock addresses by associating the src_array address list with the given VA address register, first 8 and last 8 separately (void return, PIPE_V).
 *
 * @override_id asc_set_va_reg__uba8_PPUu16
 * @archs 2201 3510
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为： &bull; VA0 &bull; VA1 &bull; VA2 &bull; VA3 &bull; VA4 &bull; VA5 &bull; VA6 &bull; VA7 数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint16_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Sets the 16 transpose half DataBlock addresses by associating the src_array address list with the given VA address register, first 8 and last 8 separately (void return, PIPE_V).
 *
 * @override_id asc_set_va_reg__uba8_PPUf16
 * @archs 2201 3510
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为： &bull; VA0 &bull; VA1 &bull; VA2 &bull; VA3 &bull; VA4 &bull; VA5 &bull; VA6 &bull; VA7 数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ half** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Sets the 16 transpose int32_t DataBlock addresses by associating the src_array address list with the given VA address register, first 8 and last 8 separately (void return, PIPE_V).
 *
 * @override_id asc_set_va_reg__uba8_PPUi32
 * @archs 2201 3510
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为： &bull; VA0 &bull; VA1 &bull; VA2 &bull; VA3 &bull; VA4 &bull; VA5 &bull; VA6 &bull; VA7 数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int32_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Sets the 16 transpose uint32_t DataBlock addresses by associating the src_array address list with the given VA address register, first 8 and last 8 separately (void return, PIPE_V).
 *
 * @override_id asc_set_va_reg__uba8_PPUu32
 * @archs 2201 3510
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为： &bull; VA0 &bull; VA1 &bull; VA2 &bull; VA3 &bull; VA4 &bull; VA5 &bull; VA6 &bull; VA7 数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint32_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Sets the 16 transpose float DataBlock addresses by associating the src_array address list with the given VA address register, first 8 and last 8 separately (void return, PIPE_V).
 *
 * @override_id asc_set_va_reg__uba8_PPUf32
 * @archs 2201 3510
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为： &bull; VA0 &bull; VA1 &bull; VA2 &bull; VA3 &bull; VA4 &bull; VA5 &bull; VA6 &bull; VA7 数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ float** src_array);

__aicore__ inline void asc_transpose(__ubuf__ int16_t* dst, __ubuf__ int16_t* src);

__aicore__ inline void asc_transpose(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Converts data layout (typically NCHW to NC1HWC0) for 8-bit data, transposing 16 DataBlocks (512 bytes) per repeat into dst, with dst_high_half/src_high_half selecting the half DataBlock.
 *
 * @override_id asc_transto5hd_b8__uba8_uba8_u8_u16_u16_b_b
 * @archs 2201 3510
 * @param [out] dst 目的地址寄存器。
 * @param [in] src 源地址寄存器。
 * @param [in] repeat 重复迭代次数， repeat∈[0,255]。 注意事项： &bull;当repeat为1时，目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置加上dst_stride/src_stride。如果要让目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，需要将dst_stride/src_stride置为0。 &bull;当repeat大于1时，第一次repeat中目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，第二次会加上dst_stride/src_stride。以此类推。
 * @param [in] dst_stride 相邻迭代间，目的操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 * @param [in] src_stride 相邻迭代间，源操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 * @param [in] dst_high_half 指定每个dst地址中的数据存储到DataBlock的高半部还是低半部，该配置只配置int8_t/uint8_t的数据类型。 支持的数据类型为bool，有以下两种取值： &bull; true:表示存储于DataBlock的高半部。 &bull; false:表示存储于DataBlock的低半部。
 * @param [in] src_high_half 指定每个src地址中的数据存储到DataBlock的高半部还是低半部，该配置只配置int8_t/uint8_t的数据类型。 支持的数据类型为bool，有以下两种取值： &bull; true:表示存储于DataBlock的高半部。 &bull;false:表示存储于DataBlock的低半部。
 */
__aicore__ inline void asc_transto5hd_b8(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Converts data layout (typically NCHW to NC1HWC0) for 16-bit data, transposing 16 DataBlocks (512 bytes) per repeat into dst; dst/src are address registers VA0/VA2/VA4/VA6.
 *
 * @override_id asc_transto5hd_b16__uba8_uba8_u8_u16_u16
 * @archs 2201 3510
 * @param [out] dst 目的地址寄存器。
 * @param [in] src 源地址寄存器。
 * @param [in] repeat 重复迭代次数， repeat∈[0,255]。 注意事项： &bull;当repeat为1时，目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置加上dst_stride/src_stride。如果要让目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，需要将dst_stride/src_stride置为0。 &bull;当repeat大于1时，第一次repeat中目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，第二次会加上dst_stride/src_stride。以此类推。
 * @param [in] dst_stride 相邻迭代间，目的操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 * @param [in] src_stride 相邻迭代间，源操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 */
__aicore__ inline void asc_transto5hd_b16(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Converts data layout (typically NCHW to NC1HWC0) for 32-bit data, transposing 16 DataBlocks (512 bytes) per repeat into dst; dst/src are address registers VA0/VA2/VA4/VA6.
 *
 * @override_id asc_transto5hd_b32__uba8_uba8_u8_u16_u16
 * @archs 2201 3510
 * @param [out] dst 目的地址寄存器。
 * @param [in] src 源地址寄存器。
 * @param [in] repeat 重复迭代次数， repeat∈[0,255]。 注意事项： &bull;当repeat为1时，目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置加上dst_stride/src_stride。如果要让目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，需要将dst_stride/src_stride置为0。 &bull;当repeat大于1时，第一次repeat中目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，第二次会加上dst_stride/src_stride。以此类推。
 * @param [in] dst_stride 相邻迭代间，目的操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 * @param [in] src_stride 相邻迭代间，源操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 */
__aicore__ inline void asc_transto5hd_b32(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Performs vector selection over the first count half elements: elements whose sel_mask bit is 1 come from src0 and whose bit is 0 come from src1, written to dst.
 *
 * @override_id asc_select__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief High-dimensional split vector selection for half data: over repeat iterations, selects elements from src0/src1 per sel_mask bits into dst using block and repeat strides.
 *
 * @override_id asc_select__PUf16_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_select(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Performs vector selection over the first count float elements: elements whose sel_mask bit is 1 come from src0 and whose bit is 0 come from src1, written to dst.
 *
 * @override_id asc_select__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief High-dimensional split vector selection for float data: over repeat iterations, selects elements from src0/src1 per sel_mask bits into dst using block and repeat strides.
 *
 * @override_id asc_select__PUf32_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_select(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Compresses the uint16 source vector src0 into a shorter vector by comparing against mask src1 and stores the result into dst; operands need 32-byte aligned start addresses.
 *
 * @override_id asc_squeeze__PUu16_PUu16_PUu16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_squeeze(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @brief Compresses the uint32 source vector src0 into a shorter vector by comparing against mask src1 and stores the result into dst; operands need 32-byte aligned start addresses.
 *
 * @override_id asc_squeeze__PUu32_PUu32_PUu32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_squeeze(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_transto5hd_b32 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous data-format conversion for 32-bit data that transposes 16 DataBlocks (512 bytes) per repeat from NCHW to NC1HWC0 layout into dst; use asc_transto5hd plus asc_sync instead.
 *
 * @override_id asc_transto5hd_b32_sync__uba8_uba8_u8_u16_u16
 * @archs 2201 3510
 * @param [out] dst 目的地址寄存器。
 * @param [in] src 源地址寄存器。
 * @param [in] repeat 重复迭代次数， repeat∈[0,255]。 注意事项： &bull;当repeat为1时，目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置加上dst_stride/src_stride。如果要让目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，需要将dst_stride/src_stride置为0。 &bull;当repeat大于1时，第一次repeat中目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，第二次会加上dst_stride/src_stride。以此类推。
 * @param [in] dst_stride 相邻迭代间，目的操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../general_description_and_constraints.md#repeatStride)。
 * @param [in] src_stride 相邻迭代间，源操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../general_description_and_constraints.md#repeatStride)。
 */
__aicore__ inline void asc_transto5hd_b32_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_transto5hd_b16 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous data-format conversion for 16-bit data that transposes 16 DataBlocks (512 bytes) per repeat from NCHW to NC1HWC0 layout into dst; use asc_transto5hd plus asc_sync instead.
 *
 * @override_id asc_transto5hd_b16_sync__uba8_uba8_u8_u16_u16
 * @archs 2201 3510
 * @param [out] dst 目的地址寄存器。
 * @param [in] src 源地址寄存器。
 * @param [in] repeat 重复迭代次数， repeat∈[0,255]。 注意事项： &bull;当repeat为1时，目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置加上dst_stride/src_stride。如果要让目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，需要将dst_stride/src_stride置为0。 &bull;当repeat大于1时，第一次repeat中目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，第二次会加上dst_stride/src_stride。以此类推。
 * @param [in] dst_stride 相邻迭代间，目的操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../general_description_and_constraints.md#repeatStride)。
 * @param [in] src_stride 相邻迭代间，源操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../general_description_and_constraints.md#repeatStride)。
 */
__aicore__ inline void asc_transto5hd_b16_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_transto5hd_b8 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 8-bit data-format conversion transposing 16 DataBlocks per repeat into dst, with dst_high_half/src_high_half selecting the half DataBlock; use asc_transto5hd plus asc_sync.
 *
 * @override_id asc_transto5hd_b8_sync__uba8_uba8_u8_u16_u16_b_b
 * @archs 2201 3510
 * @param [out] dst 目的地址寄存器。
 * @param [in] src 源地址寄存器。
 * @param [in] repeat 重复迭代次数， repeat∈[0,255]。 注意事项： &bull;当repeat为1时，目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置加上dst_stride/src_stride。如果要让目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，需要将dst_stride/src_stride置为0。 &bull;当repeat大于1时，第一次repeat中目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，第二次会加上dst_stride/src_stride。以此类推。
 * @param [in] dst_stride 相邻迭代间，目的操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../general_description_and_constraints.md#repeatStride)。
 * @param [in] src_stride 相邻迭代间，源操作数相同DataBlock地址Stride，单位：DataBlock。 相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../general_description_and_constraints.md#repeatStride)。
 * @param [in] dst_high_half 指定每个dst地址中的数据存储到DataBlock的高半部还是低半部，该配置只配置int8_t/uint8_t的数据类型。 支持的数据类型为bool，有以下两种取值： &bull; true:表示存储于DataBlock的高半部。 &bull; false:表示存储于DataBlock的低半部。
 * @param [in] src_high_half 指定每个src地址中的数据存储到DataBlock的高半部还是低半部，该配置只配置int8_t/uint8_t的数据类型。 支持的数据类型为bool，有以下两种取值： &bull; true:表示存储于DataBlock的高半部。 &bull;false:表示存储于DataBlock的低半部。
 */
__aicore__ inline void asc_transto5hd_b8_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H
#endif
