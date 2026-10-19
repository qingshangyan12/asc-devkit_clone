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
#error "cube_compute.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CUBE_COMPUTE_CUBE_COMPUTE_H
#endif

#ifndef INCLUDE_C_API_CUBE_COMPUTE_CUBE_COMPUTE_H
#define INCLUDE_C_API_CUBE_COMPUTE_CUBE_COMPUTE_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/cube_compute_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 9201)
#include "impl/c_api/reg_base_impl/cube_compute_intf_impl.h"
#endif

#include "c_api/composite/cube_compute_composite.h"

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source,
    bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Matrix multiply-accumulate C = A x B + C for int4b_t inputs (asc_mmad_s4) on the AIC cube unit; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad_s4__PCCi32_PCAi4x2_PCBi4x2_u16_u16_u16_unitflag_b_b_b
 * @archs 2201
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] k_direction_align K方向对齐的核心功能是通过`k_direction_align`参数控制在使用float数据类型时，L0A Buffer和L0B Buffer矩阵在K方向上的对齐方式。 取值说明如下： &nbsp;&nbsp;&bull; false：K方向对齐到`ceil(k / 8) * 8`。 &nbsp;&nbsp;&bull; true：K方向对齐到`ceil(k / 16) * 16`。 转置输入场景的说明见[K方向对齐约束](../mmad_compute_key_features/k_direction_alignment_constraint.md)。
 * @param [in] c_matrix_source 当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer。
 * @param [in] c_matrix_init_val 配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_s4(
    __cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Sparse matrix multiply-accumulate using asc_unit_flag_mode: sparse A is densified during compute, dense B must be pre-densified, and the result matrix C is written to output c; no return value.
 *
 * @override_id asc_mmad_sparse__PCCi32_PCAi8_PCBi8_u16_u16_u16_unitflag_b_b
 * @archs 2201
 * @param [out] c 目的操作数，结果矩阵C，起始地址需要1024字节对齐。
 * @param [in] a 源操作数，左矩阵A，起始地址需要512字节对齐。
 * @param [in] b 源操作数，右矩阵B，起始地址需要512字节对齐。
 * @param [in] m 左矩阵Height，取值范围：m∈[0,4095]。
 * @param [in] k 左矩阵Width、右矩阵Height，取值范围：k∈[0,4095]。
 * @param [in] n 右矩阵Width，取值范围：n∈[0,4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。
 * @param [in] c_matrix_source 配置C矩阵初始值是否来源于BiasTable Buffer（存放Bias的硬件缓存区）。取值说明如下：   &bull; true：来源于BiasTable Buffer。  &bull; false：来源于L0C Buffer。
 * @param [in] c_matrix_init_val 配置C矩阵初始值是否为0。取值说明如下：   &bull; true：C矩阵初始值为0。  &bull; false：C矩阵初始值通过c_matrix_source参数进行配置。
 */
__aicore__ inline void asc_mmad_sparse(
    __cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated matrix multiply-accumulate overload with k_direction_align: computes C = A x B + C for int4 A and int4 B and writes the int32 result to c_matrix; void return.
 *
 * @override_id asc_mmad_s4__PCCi32_PCAi4x2_PCBi4x2_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad_s4(
    __cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated matrix multiply-accumulate overload with feat_offset/smask_offset: computes C = A x B + C for int4 A and int4 B and writes the int32 result to c_matrix; void return.
 *
 * @override_id asc_mmad_s4__PCCi32_PCAi4x2_PCBi4x2_u16_u16_u16_u8_u8_u8_b_b_b_b
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
__aicore__ inline void asc_mmad_s4(
    __cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sparse matrix multiply-accumulate using uint8_t unit_flag: sparse A is densified during compute, dense B is used as-is, and the result matrix C is written to output c; no return value.
 *
 * @override_id asc_mmad_sparse__PCCi32_PCAi8_PCBi8_u16_u16_u16_u8_b_b
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
__aicore__ inline void asc_mmad_sparse(
    __cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m, uint16_t k, uint16_t n, uint8_t unit_flag,
    bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute_aux_config
 * @brief Configuration interface that makes Mmad generate result fractals column by column; only the L0C fractal generation order changes, not the values, MAC count or output format.
 *
 * @override_id asc_set_mmad_direction_m__z
 * @archs 2201 3510
 */
__aicore__ inline void asc_set_mmad_direction_m();
/**
 * @ingroup simd_c_api.cube_compute.mmad_compute_aux_config
 * @brief Configuration interface that makes Mmad generate result fractals row by row; only the L0C fractal generation order changes, not the values, MAC count or output format.
 *
 * @override_id asc_set_mmad_direction_n__z
 * @archs 2201 3510
 */
__aicore__ inline void asc_set_mmad_direction_n();
/**
 * @ingroup simd_c_api.cube_compute.mmad_compute_aux_config
 * @brief Configuration interface that sets the HF32 rounding mode (asc_hf32_round_mode, NEAREST_AWAY or NEAREST_EVEN) used when FP32 data is rounded to HF32; enable HF32 mode first.
 *
 * @override_id asc_set_hf32_round_mode__hfround
 * @archs 2201 3510
 * @param [in] hf32_round_mode HF32舍入模式控制入参，[asc_hf32_round_mode](../../defs/enum/asc_hf32_round_mode.md)枚举类型，支持如下2种枚举值： &nbsp;&nbsp;&bull; NEAREST_AWAY：FP32将以向最接近的值舍入，平局时远离零的方式舍入为HF32。 &nbsp;&nbsp;&bull; NEAREST_EVEN：FP32将以向最接近的值舍入，平局时向偶数舍入的方式舍入为HF32。
 */
__aicore__ inline void asc_set_hf32_round_mode(asc_hf32_round_mode hf32_round_mode);
/**
 * @ingroup simd_c_api.cube_compute.mmad_compute_aux_config
 * @brief Configuration interface that disables HiF8 mode for Mmad so fp8_e4m3fn_t data in L0A/L0B is used directly without conversion to hifloat8_t; call it before the mmad instruction.
 *
 * @override_id asc_disable_hif8__z
 * @archs 3510
 */
__aicore__ inline void asc_disable_hif8();
/**
 * @ingroup simd_c_api.cube_compute.mmad_compute_aux_config
 * @brief Configuration interface that enables HF32 mode for Mmad, improving FP32 performance at some precision loss because FP32 data is rounded to HF32 before compute; call it before mmad.
 *
 * @override_id asc_enable_hf32__z
 * @archs 2201 3510
 */
__aicore__ inline void asc_enable_hf32();
/**
 * @ingroup simd_c_api.cube_compute.mmad_compute_aux_config
 * @brief Configuration interface that disables HF32 mode for Mmad so float data in L0A/L0B is not rounded before compute; call it before the mmad instruction; no return value.
 *
 * @override_id asc_disable_hf32__z
 * @archs 2201 3510
 */
__aicore__ inline void asc_disable_hf32();
// ==========mmad_mx(e1m2/e1m2, e4m3/e5m2)==========
/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Scale-quantized matrix multiply-accumulate (MmadMx), applying ScaleA and ScaleB before the product, on the AIC unit for fp4x2_e1m2_t x fp4x2_e1m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e1m2x2_PCBf4e1m2x2_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Scale-quantized matrix multiply-accumulate (MmadMx), applying ScaleA and ScaleB before the product, on the AIC unit for fp4x2_e1m2_t x fp4x2_e2m1_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e1m2x2_PCBf4e2m1x2_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Scale-quantized matrix multiply-accumulate (MmadMx), applying ScaleA and ScaleB before the product, on the AIC unit for fp4x2_e2m1_t x fp4x2_e1m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e2m1x2_PCBf4e1m2x2_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Scale-quantized matrix multiply-accumulate (MmadMx), applying ScaleA and ScaleB before the product, on the AIC unit for fp4x2_e2m1_t x fp4x2_e2m1_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e2m1x2_PCBf4e2m1x2_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Scale-quantized matrix multiply-accumulate (MmadMx), applying ScaleA and ScaleB before the product, on the AIC unit for fp8_e4m3fn_t x fp8_e4m3fn_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e4m3_PCBf8e4m3_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Scale-quantized matrix multiply-accumulate (MmadMx), applying ScaleA and ScaleB before the product, on the AIC unit for fp8_e4m3fn_t x fp8_e5m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e4m3_PCBf8e5m2_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Scale-quantized matrix multiply-accumulate (MmadMx), applying ScaleA and ScaleB before the product, on the AIC unit for fp8_e5m2_t x fp8_e4m3fn_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e5m2_PCBf8e4m3_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Scale-quantized matrix multiply-accumulate (MmadMx), applying ScaleA and ScaleB before the product, on the AIC unit for fp8_e5m2_t x fp8_e5m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e5m2_PCBf8e5m2_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

// ==========mmad==========
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Matrix multiply-accumulate C = A x B + C on the AIC cube unit for fp8_e4m3fn_t x fp8_e4m3fn_t inputs with UnitFlag and C-init controls; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e4m3_PCBf8e4m3_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Matrix multiply-accumulate C = A x B + C on the AIC cube unit for fp8_e4m3fn_t x fp8_e5m2_t inputs with UnitFlag and C-init controls; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e4m3_PCBf8e5m2_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Matrix multiply-accumulate C = A x B + C on the AIC cube unit for fp8_e5m2_t x fp8_e4m3fn_t inputs with UnitFlag and C-init controls; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e5m2_PCBf8e4m3_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Matrix multiply-accumulate C = A x B + C on the AIC cube unit for fp8_e5m2_t x fp8_e5m2_t inputs with UnitFlag and C-init controls; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e5m2_PCBf8e5m2_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Matrix multiply-accumulate C = A x B + C on the AIC cube unit for hifloat8_t x hifloat8_t inputs with UnitFlag and C-init controls; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAhf8_PCBhf8_u16_u16_u16_unitflag_b_b_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 * @param [in] c_matrix_source 仅不显式传入Bias起始地址的重载包含该参数。当参数`c_matrix_init_val`为false时，配置矩阵C的初始值来源。 &nbsp;&nbsp;&bull; false：矩阵C的初始值来源于L0C Buffer。 &nbsp;&nbsp;&bull; true：矩阵C的初始值来源于BiasTable Buffer，并固定从0地址开始读取。
 * @param [in] c_matrix_init_val 仅不显式传入Bias起始地址的重载包含该参数。配置是否将矩阵C的初始值设置为0。 &nbsp;&nbsp;&bull; true：将矩阵C的初始值设置为0，参数`c_matrix_source`不生效。 &nbsp;&nbsp;&bull; false：不执行清零操作，矩阵C的初始值由参数`c_matrix_source`配置。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ hifloat8_t* a_matrix, __cb__ hifloat8_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

// ==========mmad with bias==========
/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized matrix multiply-accumulate C = A x B + C on the AIC cube unit for bfloat16_t x bfloat16_t inputs; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAbf16_PCBbf16_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias的数据类型需与C矩阵的数据类型保持一致，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(c_dtype)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized matrix multiply-accumulate C = A x B + C on the AIC cube unit for fp8_e4m3fn_t x fp8_e4m3fn_t inputs; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e4m3_PCBf8e4m3_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias的数据类型需与C矩阵的数据类型保持一致，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(c_dtype)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized matrix multiply-accumulate C = A x B + C on the AIC cube unit for fp8_e4m3fn_t x fp8_e5m2_t inputs; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e4m3_PCBf8e5m2_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias的数据类型需与C矩阵的数据类型保持一致，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(c_dtype)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized matrix multiply-accumulate C = A x B + C on the AIC cube unit for fp8_e5m2_t x fp8_e4m3fn_t inputs; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e5m2_PCBf8e4m3_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias的数据类型需与C矩阵的数据类型保持一致，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(c_dtype)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized matrix multiply-accumulate C = A x B + C on the AIC cube unit for fp8_e5m2_t x fp8_e5m2_t inputs; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e5m2_PCBf8e5m2_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias的数据类型需与C矩阵的数据类型保持一致，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(c_dtype)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized matrix multiply-accumulate C = A x B + C on the AIC cube unit for half x half inputs; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAf16_PCBf16_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias的数据类型需与C矩阵的数据类型保持一致，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(c_dtype)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint64_t bias, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized matrix multiply-accumulate C = A x B + C on the AIC cube unit for float x float inputs; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAf32_PCBf32_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias的数据类型需与C矩阵的数据类型保持一致，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(c_dtype)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint64_t bias, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized matrix multiply-accumulate C = A x B + C on the AIC cube unit for int8_t x int8_t inputs; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCi32_PCAi8_PCBi8_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias的数据类型需与C矩阵的数据类型保持一致，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(c_dtype)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint64_t bias, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized matrix multiply-accumulate C = A x B + C on the AIC cube unit for hifloat8_t x hifloat8_t inputs; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad__PCCf32_PCAhf8_PCBhf8_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，需按照1024字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址，需按照512字节对齐。数据类型由接口重载决定，具体请参见[支持的数据类型组合](#asc_mmad_data_type)。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias的数据类型需与C矩阵的数据类型保持一致，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(c_dtype)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ hifloat8_t* a_matrix, __cb__ hifloat8_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

// ==========mmad_mx with bias==========
/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized scale-quantized matrix multiply-accumulate (MmadMx) for fp4x2_e1m2_t x fp4x2_e1m2_t inputs on the AIC unit; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e1m2x2_PCBf4e1m2x2_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias支持的数据类型为`float`，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(float)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized scale-quantized matrix multiply-accumulate (MmadMx) for fp4x2_e1m2_t x fp4x2_e2m1_t inputs on the AIC unit; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e1m2x2_PCBf4e2m1x2_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias支持的数据类型为`float`，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(float)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized scale-quantized matrix multiply-accumulate (MmadMx) for fp4x2_e2m1_t x fp4x2_e1m2_t inputs on the AIC unit; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e2m1x2_PCBf4e1m2x2_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias支持的数据类型为`float`，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(float)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized scale-quantized matrix multiply-accumulate (MmadMx) for fp4x2_e2m1_t x fp4x2_e2m1_t inputs on the AIC unit; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e2m1x2_PCBf4e2m1x2_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias支持的数据类型为`float`，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(float)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized scale-quantized matrix multiply-accumulate (MmadMx) for fp8_e4m3fn_t x fp8_e4m3fn_t inputs on the AIC unit; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e4m3_PCBf8e4m3_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias支持的数据类型为`float`，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(float)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized scale-quantized matrix multiply-accumulate (MmadMx) for fp8_e4m3fn_t x fp8_e5m2_t inputs on the AIC unit; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e4m3_PCBf8e5m2_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias支持的数据类型为`float`，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(float)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized scale-quantized matrix multiply-accumulate (MmadMx) for fp8_e5m2_t x fp8_e4m3fn_t inputs on the AIC unit; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e5m2_PCBf8e4m3_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias支持的数据类型为`float`，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(float)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute
 * @brief Bias-initialized scale-quantized matrix multiply-accumulate (MmadMx) for fp8_e5m2_t x fp8_e5m2_t inputs on the AIC unit; result written to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e5m2_PCBf8e5m2_u64_u16_u16_u16_unitflag_b
 * @archs 3510
 * @param [out] c_matrix 目的操作数，结果矩阵C在L0C Buffer中的起始地址，数据类型为`float`，需按照1024字节对齐。
 * @param [in] a_matrix 源操作数，左矩阵A在L0A Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleA矩阵在L0A_MX Buffer中的起始地址为`a_matrix`起始地址除以16。
 * @param [in] b_matrix 源操作数，右矩阵B在L0B Buffer中的起始地址。数据类型由接口重载决定，具体请参见[表2](#asc_mmad_mx_data_type)。 矩阵数据类型为`fp4x2_e2m1_t`、`fp4x2_e1m2_t`时需按照512字节对齐，矩阵数据类型为`fp8_e5m2_t`、`fp8_e4m3fn_t`时需按照1024字节对齐。 ScaleB矩阵在L0B_MX Buffer中的起始地址为`b_matrix`起始地址除以16。
 * @param [in] bias Bias矩阵在BiasTable Buffer中的起始地址，需按照64字节对齐，可以为非零地址，仅显式传入Bias起始地址的接口包含该参数。 Bias支持的数据类型为`float`，调用本接口前，需将Bias数据通过[asc_copy_l12bt](../../cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)接口搬运到该地址，Bias占用长度为`n * sizeof(float)`向上补齐到64字节。
 * @param [in] m 左矩阵A和结果矩阵C的M维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] k 左矩阵A和右矩阵B的K维大小，单位为元素，取值范围为[0, 4095]，并且需为64的倍数。
 * @param [in] n 右矩阵B和结果矩阵C的N维大小，单位为元素，取值范围为[0, 4095]。
 * @param [in] unit_flag_mode 用于控制矩阵乘加指令与矩阵搬出指令的细粒度并行，开启UnitFlag后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令后不改变单元标志位。 &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令后改变单元标志位。 矩阵乘加指令与对应的矩阵搬出指令必须都开启或都不开启UnitFlag，开启后指令之间无需再插入同步指令。 同步机制和迭代场景见[UnitFlag](../mmad_compute_key_features/UnitFlag.md)。
 * @param [in] disable_gemv M为1时，配置是否关闭GEMV模式。 &nbsp;&nbsp;&bull; false：开启GEMV模式。 &nbsp;&nbsp;&bull; true：关闭GEMV模式。 M不为1时，该参数不生效。 向量布局与使用场景见[GEMV](../mmad_compute_key_features/GEMV.md)。
 */
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

#if (ASC_DEVKIT_VERSION_NUM >= 90200000)
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ int8_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ int4b_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
    asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint64_t bias, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

__aicore__ inline void asc_mmad(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int4b_t* b_matrix, uint64_t bias, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ int8_t* b_matrix, uint64_t bias, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ int8_t* b_matrix, uint64_t bias, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ int4b_t* b_matrix, uint64_t bias, uint16_t m, uint16_t k,
    uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ int4b_t* b_matrix, uint64_t bias, uint16_t m,
    uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e1m2_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e1m2_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e2m1_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e2m1_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e1m2_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e1m2_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e2m1_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e2m1_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp8_e4m3fn_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp8_e4m3fn_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp8_e5m2_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp8_e5m2_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp8_e4m3fn_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp8_e4m3fn_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ hif4x2_t* a_matrix, uint64_t scale_a_matrix, __cb__ hif4x2_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ hif4x2_t* a_matrix, uint64_t scale_a_matrix, __cb__ hif4x2_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, uint64_t scale_a_matrix, __cb__ hif4x2_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, uint64_t scale_a_matrix, __cb__ hif4x2_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e2m1_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e2m1_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ half* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e2m1_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ half* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e2m1_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ half* a_matrix, uint64_t scale_a_matrix, __cb__ hif4x2_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ half* a_matrix, uint64_t scale_a_matrix, __cb__ hif4x2_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e2m1_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp4x2_e2m1_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, uint64_t scale_a_matrix, __cb__ hif4x2_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, uint64_t scale_a_matrix, __cb__ hif4x2_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp8_e5m2_t* b_matrix,
    uint64_t scale_b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv,
    bool c_matrix_source, bool c_matrix_init_val);

__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, uint64_t scale_a_matrix, __cb__ fp8_e5m2_t* b_matrix,
    uint64_t scale_b_matrix, uint64_t bias, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode,
    bool disable_gemv);
#endif

/**
 * @ingroup simd_c_api.cube_compute.mmad_compute_aux_config
 * @brief Configuration interface that enables HiF8 mode for Mmad: when both input matrices are fp8_e4m3fn_t, L0A/L0B data is converted to hifloat8_t before multiplication.
 *
 * @override_id asc_enable_hif8__z
 * @archs 3510
 */
__aicore__ inline void asc_enable_hif8();

// ==========deprecated interfaces==========
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_hf32_round_mode)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated configuration interface that sets the HF32 rounding mode via uint32_t mode (0 nearest-even, 1 nearest-away) for Mmad after HF32 is enabled; replaced by asc_set_hf32_round_mode.
 *
 * @override_id asc_enable_hf32_trans__u32
 * @archs 2201 3510
 * @param [in] mode HF32舍入模式控制参数，uint32_t类型，支持如下2种取值： &nbsp;&nbsp;&bull; 0：FP32将以向最接近的值舍入，平局时向偶数舍入的方式舍入为HF32。 &nbsp;&nbsp;&bull; 1：FP32将以向最接近的值舍入，平局时远离零的方式舍入为HF32。
 */
__aicore__ inline void asc_enable_hf32_trans(uint32_t mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_disable_hf32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated configuration interface that disables HF32 mode for Mmad so float data in L0A/L0B is not rounded before compute; replaced by asc_disable_hf32.
 *
 * @override_id asc_set_fp32_mode__z
 * @archs 2201 3510
 */
__aicore__ inline void asc_set_fp32_mode();

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad_mx(
        __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx overload with uint8_t unit_flag: computes scale-quantized matrix multiply-accumulate (MmadMx) for fp4x2_e1m2_t x fp4x2_e1m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e1m2x2_PCBf4e1m2x2_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad_mx(
        __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx overload with uint8_t unit_flag: computes scale-quantized matrix multiply-accumulate (MmadMx) for fp4x2_e1m2_t x fp4x2_e2m1_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e1m2x2_PCBf4e2m1x2_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad_mx(
        __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx overload with uint8_t unit_flag: computes scale-quantized matrix multiply-accumulate (MmadMx) for fp4x2_e2m1_t x fp4x2_e1m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e2m1x2_PCBf4e1m2x2_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad_mx(
        __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx overload with uint8_t unit_flag: computes scale-quantized matrix multiply-accumulate (MmadMx) for fp4x2_e2m1_t x fp4x2_e2m1_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf4e2m1x2_PCBf4e2m1x2_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad_mx(
        __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx overload with uint8_t unit_flag: computes scale-quantized matrix multiply-accumulate (MmadMx) for fp8_e4m3fn_t x fp8_e4m3fn_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e4m3_PCBf8e4m3_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad_mx(
        __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx overload with uint8_t unit_flag: computes scale-quantized matrix multiply-accumulate (MmadMx) for fp8_e4m3fn_t x fp8_e5m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e4m3_PCBf8e5m2_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad_mx(
        __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx overload with uint8_t unit_flag: computes scale-quantized matrix multiply-accumulate (MmadMx) for fp8_e5m2_t x fp8_e4m3fn_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e5m2_PCBf8e4m3_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad_mx(
        __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated asc_mmad_mx overload with uint8_t unit_flag: computes scale-quantized matrix multiply-accumulate (MmadMx) for fp8_e5m2_t x fp8_e5m2_t inputs; result to c_matrix; returns void.
 *
 * @override_id asc_mmad_mx__PCCf32_PCAf8e5m2_PCBf8e5m2_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad_mx(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source,
        bool c_matrix_init_val))
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source,
        bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated matrix multiply-accumulate overload with feat_offset/smask_offset: computes C = A x B + C for bfloat16 A and bfloat16 B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad__PCCf32_PCAbf16_PCBbf16_u16_u16_u16_u8_u8_u8_b_b_b_b
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
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag,
    bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t m, uint16_t k, uint16_t n,
        asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val))
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t m, uint16_t k, uint16_t n,
        asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated matrix multiply-accumulate overload with feat_offset/smask_offset: computes C = A x B + C for half A and half B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad__PCCf32_PCAf16_PCBf16_u16_u16_u16_u8_u8_u8_b_b_b_b
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
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t m, uint16_t k, uint16_t n,
        asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val))
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t m, uint16_t k, uint16_t n,
        asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated matrix multiply-accumulate overload with feat_offset/smask_offset: computes C = A x B + C for float A and float B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad__PCCf32_PCAf32_PCBf32_u16_u16_u16_u8_u8_u8_b_b_b_b
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
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
        asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val))
__aicore__ inline void asc_mmad(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
        asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated matrix multiply-accumulate overload with feat_offset/smask_offset: computes C = A x B + C for int8 A and int8 B and writes the int32 result to c_matrix; void return.
 *
 * @override_id asc_mmad__PCCi32_PCAi8_PCBi8_u16_u16_u16_u8_u8_u8_b_b_b_b
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
__aicore__ inline void asc_mmad(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align,
    bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated matrix multiply-accumulate overload with disable_gemv: computes C = A x B + C for fp8_e4m3fn A and fp8_e4m3fn B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e4m3_PCBf8e4m3_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated matrix multiply-accumulate overload with disable_gemv: computes C = A x B + C for fp8_e4m3fn A and fp8_e5m2 B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e4m3_PCBf8e5m2_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated matrix multiply-accumulate overload with disable_gemv: computes C = A x B + C for fp8_e5m2 A and fp8_e4m3fn B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e5m2_PCBf8e4m3_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated matrix multiply-accumulate overload with disable_gemv: computes C = A x B + C for fp8_e5m2 A and fp8_e5m2 B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad__PCCf32_PCAf8e5m2_PCBf8e5m2_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t m, uint16_t k, uint16_t n,
        asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t m, uint16_t k, uint16_t n,
        asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t m, uint16_t k, uint16_t n,
        asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
__aicore__ inline void asc_mmad(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_mmad(
        __cc__ float* c_matrix, __ca__ hifloat8_t* a_matrix, __cb__ hifloat8_t* b_matrix, uint16_t m, uint16_t k,
        uint16_t n, asc_unit_flag_mode unit_flag_mode, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated matrix multiply-accumulate overload with disable_gemv: computes C = A x B + C for hifloat8 A and hifloat8 B and writes the float result to c_matrix; void return.
 *
 * @override_id asc_mmad__PCCf32_PCAhf8_PCBhf8_u16_u16_u16_u8_b_b_b
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
__aicore__ inline void asc_mmad(
    __cc__ float* c_matrix, __ca__ hifloat8_t* a_matrix, __cb__ hifloat8_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_disable_hif8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated configuration interface that enables FP8 mode for Mmad while turning HiF8 off, so FP8 data in L0A/L0B is not converted to hifloat8_t; use asc_disable_hif8 instead.
 *
 * @override_id asc_enable_fp8__z
 * @archs 3510
 */
__aicore__ inline void asc_enable_fp8();

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CUBE_COMPUTE_CUBE_COMPUTE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CUBE_COMPUTE_CUBE_COMPUTE_H

#endif
