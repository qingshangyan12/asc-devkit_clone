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
#error "composite/cube_compute_composite.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_CUBE_COMPUTE_COMPOSITE_H
#endif

#ifndef INCLUDE_C_API_COMPOSITE_CUBE_COMPUTE_COMPOSITE_H
#define INCLUDE_C_API_COMPOSITE_CUBE_COMPUTE_COMPOSITE_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/cube_compute_composite_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/cube_compute_composite_intf_impl.h"
#endif

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous matrix multiply-accumulate overload with feat_offset/smask_offset: computes C = A x B + C for bfloat16 A and bfloat16 B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad_sync__PCCf32_PCAbf16_PCBbf16_u16_u16_u16_u8_u8_u8_b_b_b_b
 * @archs 2201
 * @param [out] c_matrix 结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。
 * @param [in] a_matrix 左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。
 * @param [in] b_matrix 右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，取值范围为[0, 4095]。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，取值范围为[0, 4095]。
 * @param [in] feat_offset 无效参数，用户无需关注，传入0即可。
 * @param [in] smask_offset 无效参数，用户无需关注，传入0即可。
 * @param [in] unit_flag UnitFlag控制参数。0表示关闭；2表示开启且执行后保持单元标志位；3表示开启且执行后更新单元标志位。
 * @param [in] k_direction_align 使用float数据类型时控制K方向的对齐方式。false表示对齐到`ceil(n_dim / 8) * 8`，true表示对齐到`ceil(n_dim / 16) * 16`。
 * @param [in] is_weight_offset 无效参数，用户无需关注，传入false即可。
 * @param [in] c_matrix_source 当`c_matrix_init_val`为false时，配置矩阵C的初始值是否来自BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。
 */
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag,
    bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous matrix multiply-accumulate overload with feat_offset/smask_offset: computes C = A x B + C for half A and half B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad_sync__PCCf32_PCAf16_PCBf16_u16_u16_u16_u8_u8_u8_b_b_b_b
 * @archs 2201
 * @param [out] c_matrix 结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。
 * @param [in] a_matrix 左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。
 * @param [in] b_matrix 右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，取值范围为[0, 4095]。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，取值范围为[0, 4095]。
 * @param [in] feat_offset 无效参数，用户无需关注，传入0即可。
 * @param [in] smask_offset 无效参数，用户无需关注，传入0即可。
 * @param [in] unit_flag UnitFlag控制参数。0表示关闭；2表示开启且执行后保持单元标志位；3表示开启且执行后更新单元标志位。
 * @param [in] k_direction_align 使用float数据类型时控制K方向的对齐方式。false表示对齐到`ceil(n_dim / 8) * 8`，true表示对齐到`ceil(n_dim / 16) * 16`。
 * @param [in] is_weight_offset 无效参数，用户无需关注，传入false即可。
 * @param [in] c_matrix_source 当`c_matrix_init_val`为false时，配置矩阵C的初始值是否来自BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。
 */
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous matrix multiply-accumulate overload with feat_offset/smask_offset: computes C = A x B + C for float A and float B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad_sync__PCCf32_PCAf32_PCBf32_u16_u16_u16_u8_u8_u8_b_b_b_b
 * @archs 2201
 * @param [out] c_matrix 结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。
 * @param [in] a_matrix 左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。
 * @param [in] b_matrix 右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，取值范围为[0, 4095]。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，取值范围为[0, 4095]。
 * @param [in] feat_offset 无效参数，用户无需关注，传入0即可。
 * @param [in] smask_offset 无效参数，用户无需关注，传入0即可。
 * @param [in] unit_flag UnitFlag控制参数。0表示关闭；2表示开启且执行后保持单元标志位；3表示开启且执行后更新单元标志位。
 * @param [in] k_direction_align 使用float数据类型时控制K方向的对齐方式。false表示对齐到`ceil(n_dim / 8) * 8`，true表示对齐到`ceil(n_dim / 16) * 16`。
 * @param [in] is_weight_offset 无效参数，用户无需关注，传入false即可。
 * @param [in] c_matrix_source 当`c_matrix_init_val`为false时，配置矩阵C的初始值是否来自BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。
 */
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous matrix multiply-accumulate overload with feat_offset/smask_offset: computes C = A x B + C for int8 A and int8 B and writes the int32 result to c_matrix; void return.
 *
 * @override_id asc_mmad_sync__PCCi32_PCAi8_PCBi8_u16_u16_u16_u8_u8_u8_b_b_b_b
 * @archs 2201
 * @param [out] c_matrix 结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。
 * @param [in] a_matrix 左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。
 * @param [in] b_matrix 右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，取值范围为[0, 4095]。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，取值范围为[0, 4095]。
 * @param [in] feat_offset 无效参数，用户无需关注，传入0即可。
 * @param [in] smask_offset 无效参数，用户无需关注，传入0即可。
 * @param [in] unit_flag UnitFlag控制参数。0表示关闭；2表示开启且执行后保持单元标志位；3表示开启且执行后更新单元标志位。
 * @param [in] k_direction_align 使用float数据类型时控制K方向的对齐方式。false表示对齐到`ceil(n_dim / 8) * 8`，true表示对齐到`ceil(n_dim / 16) * 16`。
 * @param [in] is_weight_offset 无效参数，用户无需关注，传入false即可。
 * @param [in] c_matrix_source 当`c_matrix_init_val`为false时，配置矩阵C的初始值是否来自BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。
 */
__aicore__ inline void asc_mmad_sync(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad_s4 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous matrix multiply-accumulate overload with k_direction_align: computes C = A x B + C for int4 A and int4 B and writes the int32 result to c_matrix; void return.
 *
 * @override_id asc_mmad_s4_sync__PCCi32_PCAi4x2_PCBi4x2_u16_u16_u16_u8_b_b_b
 * @archs 2201
 * @param [out] c_matrix 结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。
 * @param [in] a_matrix 左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。
 * @param [in] b_matrix 右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，取值范围为[0, 4095]。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，取值范围为[0, 4095]。
 * @param [in] unit_flag UnitFlag控制参数。0表示关闭；2表示开启且执行后保持单元标志位；3表示开启且执行后更新单元标志位。
 * @param [in] k_direction_align 使用float数据类型时控制K方向的对齐方式。false表示对齐到`ceil(n_dim / 8) * 8`，true表示对齐到`ceil(n_dim / 16) * 16`。
 * @param [in] c_matrix_source 当`c_matrix_init_val`为false时，配置矩阵C的初始值是否来自BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。
 */
__aicore__ inline void asc_mmad_s4_sync(
    __cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad_s4 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous matrix multiply-accumulate overload with feat_offset/smask_offset: computes C = A x B + C for int4 A and int4 B and writes the int32 result to c_matrix; void return.
 *
 * @override_id asc_mmad_s4_sync__PCCi32_PCAi4x2_PCBi4x2_u16_u16_u16_u8_u8_u8_b_b_b_b
 * @archs 2201
 * @param [out] c_matrix 结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。
 * @param [in] a_matrix 左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。
 * @param [in] b_matrix 右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，取值范围为[0, 4095]。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，取值范围为[0, 4095]。
 * @param [in] feat_offset 无效参数，用户无需关注，传入0即可。
 * @param [in] smask_offset 无效参数，用户无需关注，传入0即可。
 * @param [in] unit_flag UnitFlag控制参数。0表示关闭；2表示开启且执行后保持单元标志位；3表示开启且执行后更新单元标志位。
 * @param [in] k_direction_align 使用float数据类型时控制K方向的对齐方式。false表示对齐到`ceil(n_dim / 8) * 8`，true表示对齐到`ceil(n_dim / 16) * 16`。
 * @param [in] is_weight_offset 无效参数，用户无需关注，传入false即可。
 * @param [in] c_matrix_source 当`c_matrix_init_val`为false时，配置矩阵C的初始值是否来自BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。
 */
__aicore__ inline void asc_mmad_s4_sync(
    __cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad_sparse and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant of sparse matrix multiply-accumulate (uint8_t unit_flag) writing result C to output c; its synchronization duty is now covered by asc_mmad_sparse plus asc_sync.
 *
 * @override_id asc_mmad_sparse_sync__PCCi32_PCAi8_PCBi8_u16_u16_u16_u8_b_b
 * @archs 2201
 * @param [out] c 结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。
 * @param [in] a 左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。
 * @param [in] b 右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，取值范围为[0, 4095]。
 * @param [in] unit_flag UnitFlag控制参数。0表示关闭；2表示开启且执行后保持单元标志位；3表示开启且执行后更新单元标志位。
 * @param [in] c_matrix_source 当`c_matrix_init_val`为false时，配置矩阵C的初始值是否来自BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。
 */
__aicore__ inline void asc_mmad_sparse_sync(
    __cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad_mx and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx_sync overload with uint8_t unit_flag: synchronized scale-quantized MmadMx for fp4x2_e1m2_t x fp4x2_e1m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx_sync__PCCf32_PCAf4e1m2x2_PCBf4e1m2x2_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad_mx and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx_sync overload with uint8_t unit_flag: synchronized scale-quantized MmadMx for fp4x2_e1m2_t x fp4x2_e2m1_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx_sync__PCCf32_PCAf4e1m2x2_PCBf4e2m1x2_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad_mx and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx_sync overload with uint8_t unit_flag: synchronized scale-quantized MmadMx for fp4x2_e2m1_t x fp4x2_e1m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx_sync__PCCf32_PCAf4e2m1x2_PCBf4e1m2x2_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad_mx and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx_sync overload with uint8_t unit_flag: synchronized scale-quantized MmadMx for fp4x2_e2m1_t x fp4x2_e2m1_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx_sync__PCCf32_PCAf4e2m1x2_PCBf4e2m1x2_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad_mx and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx_sync overload with uint8_t unit_flag: synchronized scale-quantized MmadMx for fp8_e4m3fn_t x fp8_e4m3fn_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx_sync__PCCf32_PCAf8e4m3_PCBf8e4m3_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad_mx and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx_sync overload with uint8_t unit_flag: synchronized scale-quantized MmadMx for fp8_e4m3fn_t x fp8_e5m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx_sync__PCCf32_PCAf8e4m3_PCBf8e5m2_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad_mx and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx_sync overload with uint8_t unit_flag: synchronized scale-quantized MmadMx for fp8_e5m2_t x fp8_e4m3fn_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx_sync__PCCf32_PCAf8e5m2_PCBf8e4m3_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad_mx and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx_sync overload with uint8_t unit_flag: synchronized scale-quantized MmadMx for fp8_e5m2_t x fp8_e5m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx_sync__PCCf32_PCAf8e5m2_PCBf8e5m2_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous matrix multiply-accumulate overload with disable_gemv: computes C = A x B + C for fp8_e4m3fn A and fp8_e4m3fn B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad_sync__PCCf32_PCAf8e4m3_PCBf8e4m3_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous matrix multiply-accumulate overload with disable_gemv: computes C = A x B + C for fp8_e4m3fn A and fp8_e5m2 B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad_sync__PCCf32_PCAf8e4m3_PCBf8e5m2_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous matrix multiply-accumulate overload with disable_gemv: computes C = A x B + C for fp8_e5m2 A and fp8_e4m3fn B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad_sync__PCCf32_PCAf8e5m2_PCBf8e4m3_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous matrix multiply-accumulate overload with disable_gemv: computes C = A x B + C for fp8_e5m2 A and fp8_e5m2 B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad_sync__PCCf32_PCAf8e5m2_PCBf8e5m2_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mmad and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous matrix multiply-accumulate overload with disable_gemv: computes C = A x B + C for hifloat8 A and hifloat8 B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad_sync__PCCf32_PCAhf8_PCBhf8_u16_u16_u16_u8_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] left_height 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n_dim 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] right_width 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启UnitFlag。 &nbsp;&nbsp;&bull; 2：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; 3：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ hifloat8_t* a_matrix, __cb__ hifloat8_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_CUBE_COMPUTE_COMPOSITE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_CUBE_COMPUTE_COMPOSITE_H
#endif
