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
#include "impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif
/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_set_va_reg__uba8_PPUi8
 * @archs 3510 2201
 * @brief 用于设置transpose的16个DataBlock地址，将操作数地址序列与地址寄存器关联，接口要求前8个和后8个地址序列与地址寄存器分别关联。
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为：&bull; VA0&bull; VA1&bull; VA2&bull; VA3&bull; VA4&bull; VA5&bull; VA6&bull; VA7数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int8_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_set_va_reg__uba8_PPUu8
 * @archs 3510 2201
 * @brief 用于设置transpose的16个DataBlock地址，将操作数地址序列与地址寄存器关联，接口要求前8个和后8个地址序列与地址寄存器分别关联。
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为：&bull; VA0&bull; VA1&bull; VA2&bull; VA3&bull; VA4&bull; VA5&bull; VA6&bull; VA7数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint8_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_set_va_reg__uba8_PPUi16
 * @archs 3510 2201
 * @brief 用于设置transpose的16个DataBlock地址，将操作数地址序列与地址寄存器关联，接口要求前8个和后8个地址序列与地址寄存器分别关联。
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为：&bull; VA0&bull; VA1&bull; VA2&bull; VA3&bull; VA4&bull; VA5&bull; VA6&bull; VA7数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int16_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_set_va_reg__uba8_PPUu16
 * @archs 3510 2201
 * @brief 用于设置transpose的16个DataBlock地址，将操作数地址序列与地址寄存器关联，接口要求前8个和后8个地址序列与地址寄存器分别关联。
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为：&bull; VA0&bull; VA1&bull; VA2&bull; VA3&bull; VA4&bull; VA5&bull; VA6&bull; VA7数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint16_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_set_va_reg__uba8_PPUf16
 * @archs 3510 2201
 * @brief 用于设置transpose的16个DataBlock地址，将操作数地址序列与地址寄存器关联，接口要求前8个和后8个地址序列与地址寄存器分别关联。
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为：&bull; VA0&bull; VA1&bull; VA2&bull; VA3&bull; VA4&bull; VA5&bull; VA6&bull; VA7数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ half** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_set_va_reg__uba8_PPUi32
 * @archs 3510 2201
 * @brief 用于设置transpose的16个DataBlock地址，将操作数地址序列与地址寄存器关联，接口要求前8个和后8个地址序列与地址寄存器分别关联。
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为：&bull; VA0&bull; VA1&bull; VA2&bull; VA3&bull; VA4&bull; VA5&bull; VA6&bull; VA7数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int32_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_set_va_reg__uba8_PPUu32
 * @archs 3510 2201
 * @brief 用于设置transpose的16个DataBlock地址，将操作数地址序列与地址寄存器关联，接口要求前8个和后8个地址序列与地址寄存器分别关联。
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为：&bull; VA0&bull; VA1&bull; VA2&bull; VA3&bull; VA4&bull; VA5&bull; VA6&bull; VA7数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint32_t** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_set_va_reg__uba8_PPUf32
 * @archs 3510 2201
 * @brief 用于设置transpose的16个DataBlock地址，将操作数地址序列与地址寄存器关联，接口要求前8个和后8个地址序列与地址寄存器分别关联。
 * @param [in] addr 地址寄存器，类型为ub_addr8_t，可取值为：&bull; VA0&bull; VA1&bull; VA2&bull; VA3&bull; VA4&bull; VA5&bull; VA6&bull; VA7数字代表寄存器顺序，每个地址寄存器只能关联8个地址，使用方法请参考调用示例
 * @param [in] src_array 操作数地址序列。
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ float** src_array);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_transpose__PUi16_PUi16
 * @archs 3510 2201
 * @brief 用于实现16*16的二维矩阵数据块转置。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__aicore__ inline void asc_transpose(__ubuf__ int16_t* dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_transpose__PUu16_PUu16
 * @archs 3510 2201
 * @brief 用于实现16*16的二维矩阵数据块转置。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__aicore__ inline void asc_transpose(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_select__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量选择操作，给定两个源操作数src0和src1，根据条件选择元素，得到目的操作数dst。此接口配合[asc_set_cmp_mask(sel_mask)](../vector_compare/asc_set_cmp_mask.md)设置，规则为：当sel_mask的比特位是1时，从src0选取；当比特位是0时，从src1选择。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_select__PUf16_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量选择操作，给定两个源操作数src0和src1，根据条件选择元素，得到目的操作数dst。此接口配合[asc_set_cmp_mask(sel_mask)](../vector_compare/asc_set_cmp_mask.md)设置，规则为：当sel_mask的比特位是1时，从src0选取；当比特位是0时，从src1选择。
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
 * @override_id asc_select__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量选择操作，给定两个源操作数src0和src1，根据条件选择元素，得到目的操作数dst。此接口配合[asc_set_cmp_mask(sel_mask)](../vector_compare/asc_set_cmp_mask.md)设置，规则为：当sel_mask的比特位是1时，从src0选取；当比特位是0时，从src1选择。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_select__PUf32_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量选择操作，给定两个源操作数src0和src1，根据条件选择元素，得到目的操作数dst。此接口配合[asc_set_cmp_mask(sel_mask)](../vector_compare/asc_set_cmp_mask.md)设置，规则为：当sel_mask的比特位是1时，从src0选取；当比特位是0时，从src1选择。
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
 * @override_id asc_squeeze__PUu16_PUu16_PUu16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 通过比较掩码src1，将src0中的向量压缩成更短的向量，存储到dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
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
 * @override_id asc_squeeze__PUu32_PUu32_PUu32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 通过比较掩码src1，将src0中的向量压缩成更短的向量，存储到dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
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

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H
#endif
