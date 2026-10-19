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
#error "cube_datamove.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CUBE_DATAMOVE_CUBE_DATAMOVE_H
#endif

#ifndef INCLUDE_C_API_CUBE_DATAMOVE_CUBE_DATAMOVE_H
#define INCLUDE_C_API_CUBE_DATAMOVE_CUBE_DATAMOVE_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/cube_datamove_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 9201)
#include "impl/c_api/reg_base_impl/cube_datamove_intf_impl.h"
#endif

#include "c_api/composite/cube_datamove_composite.h"

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Configures repeat-mode parameters for left-matrix 3D img2col asc_copy_l12l0a (f_matrix_ctrl false) from an asc_load3d_v2_config; sets repeat stride/times/direction and dst strides only.
 *
 * @override_id asc_set_l13d_rpt__Rload3dcfg
 * @archs 2201 3510
 * @param [in] config repeat配置，仅传入config位域类型参数的接口使用，位域结构体配置说明请参考[asc_load3d_v2_config](../../defs/union/asc_load3d_v2_config.md)。
 */
__aicore__ inline void asc_set_l13d_rpt(asc_load3d_v2_config& config);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Configures left-matrix repeat-mode 3D img2col asc_copy_l12l0a (f_matrix_ctrl false) via repeat_stride, repeat_times, repeat_direction, dst_stride_k and dst_start_pos_m; setup only.
 *
 * @override_id asc_set_l13d_rpt__u16_u8_l13ddir_u16_u16
 * @archs 2201 3510
 * @param [in] repeat_stride 相邻两次迭代起始位置的距离，取值范围：[0, 65535]。当`repeat_direction`为`M_DIRECTION`时，单位为16个元素；为`K_DIRECTION`时，单位为`32 / sizeof(data_type)`个元素，`data_type`为源操作数的数据类型。
 * @param [in] repeat_times M或K方向的迭代次数，取值范围：[0, 255]。`0`表示不执行搬运，3D搬运接口被视为NOP（空操作）。
 * @param [in] repeat_direction [asc_l13d_repeat_direction](../../defs/enum/asc_l13d_repeat_direction.md)类型枚举值，表示repeat方向。 &nbsp;&nbsp;&bull; `asc_l13d_repeat_direction::M_DIRECTION`：沿height（M）方向迭代； &nbsp;&nbsp;&bull; `asc_l13d_repeat_direction::K_DIRECTION`：沿width（K）方向迭代。
 * @param [in] dst_stride_k 目的矩阵K方向的步长，取值范围：[0, 65535]，单位为512字节的分形。
 * @param [in] dst_start_pos_m 目的矩阵M方向的起始位置，取值范围：[0, 65535]，单位为512字节的分形。
 */
__aicore__ inline void asc_set_l13d_rpt(
    uint16_t repeat_stride, uint8_t repeat_times, asc_l13d_repeat_direction repeat_direction, uint16_t dst_stride_k,
    uint16_t dst_start_pos_m);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Configures right-matrix repeat-mode parameters for 3D img2col asc_copy_l12l0b (f_matrix_ctrl true) via repeat_stride, repeat_times, direction, dst_stride_k, dst_start_pos_m; AIC only.
 *
 * @override_id asc_set_l13d_rpt_b__u16_u8_l13ddir_u16_u16
 * @archs 3510
 * @param [in] repeat_stride 相邻两次迭代起始位置的距离，取值范围：[0, 65535]。当`repeat_direction`为`asc_l13d_repeat_direction::M_DIRECTION`时，单位为16个元素；为`asc_l13d_repeat_direction::K_DIRECTION`时，单位为`32 / sizeof(data_type)`个元素，`data_type`为源操作数的数据类型。
 * @param [in] repeat_times M或K方向的迭代次数，取值范围：[0, 255]。`0`表示不执行搬运，3D搬运接口被视为NOP（空操作）。
 * @param [in] repeat_direction [asc_l13d_repeat_direction](../../defs/enum/asc_l13d_repeat_direction.md)类型枚举值，表示repeat方向。 &nbsp;&nbsp;&bull; `asc_l13d_repeat_direction::M_DIRECTION`：沿height（M）方向迭代； &nbsp;&nbsp;&bull; `asc_l13d_repeat_direction::K_DIRECTION`：沿width（K）方向迭代。
 * @param [in] dst_stride_k 目的矩阵K方向的步长，取值范围：[0, 65535]，单位为512字节的分形。
 * @param [in] dst_start_pos_m 目的矩阵M方向的起始位置，取值范围：[0, 65535]，单位为512字节的分形。
 */
__aicore__ inline void asc_set_l13d_rpt_b(
    uint16_t repeat_stride, uint8_t repeat_times, asc_l13d_repeat_direction repeat_direction, uint16_t dst_stride_k,
    uint16_t dst_start_pos_m);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Configures the left-matrix Feature Map descriptor (width/height/padding) for 3D img2col asc_copy_l12l0a when f_matrix_ctrl is false, taking a caller-built asc_l13d_fmatrix_config; setup only, AIC.
 *
 * @override_id asc_set_l13d_fmatrix__Rl13dcfg
 * @archs 2201 3510
 * @param [in] config Feature Map属性配置，仅传入config位域类型参数的接口使用，详细说明请参见[asc_l13d_fmatrix_config](../../defs/union/asc_l13d_fmatrix_config.md)。
 */
__aicore__ inline void asc_set_l13d_fmatrix(asc_l13d_fmatrix_config& config);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Configures the left-matrix Feature Map descriptor for 3D img2col asc_copy_l12l0a (f_matrix_ctrl false) from fmatrix_w, fmatrix_h and pad_left/right/top/bottom; setup only, AIC.
 *
 * @override_id asc_set_l13d_fmatrix__u16_u16_u8_u8_u8_u8
 * @archs 2201 3510
 * @param [in] fmatrix_w Feature Map的width，取值范围：[1, 32767]。
 * @param [in] fmatrix_h Feature Map的height，取值范围：[1, 32767]。
 * @param [in] pad_left 左侧padding大小，取值范围：[0, 255]。
 * @param [in] pad_right 右侧padding大小，取值范围：[0, 255]。
 * @param [in] pad_top 上侧padding大小，取值范围：[0, 255]。
 * @param [in] pad_bottom 下侧padding大小，取值范围：[0, 255]。
 */
__aicore__ inline void asc_set_l13d_fmatrix(
    uint16_t fmatrix_w, uint16_t fmatrix_h, uint8_t pad_left, uint8_t pad_right, uint8_t pad_top, uint8_t pad_bottom);

/**
 * @ingroup simd_c_api.cube_datamove.cube_store_aux_config
 * @brief Returns the start address of the vector before ReLU (FPC register field set by asc_set_l0c_copy_config's relu_pre_addr) as uint64_t; no inputs.
 *
 * @override_id asc_get_l0c2gm_relu__z
 * @archs 2201
 */
__aicore__ inline uint64_t asc_get_l0c2gm_relu();

/**
 * @ingroup simd_c_api.cube_datamove.cube_store_aux_config
 * @brief Returns the unit_flag setting (FPC register field set by asc_set_l0c_copy_config's is_clean_unit_flag) as uint64_t; gives fine-grained MMAD/copy parallelism, not for L0C accumulation.
 *
 * @override_id asc_get_l0c2gm_unitflag__z
 * @archs 2201
 */
__aicore__ inline uint64_t asc_get_l0c2gm_unitflag();

/**
 * @ingroup simd_c_api.cube_datamove.cube_store_aux_config
 * @brief Returns the start address of the vector before quantization (FPC register field set by asc_set_l0c_copy_config's quant_pre_addr) as uint64_t; no inputs.
 *
 * @override_id asc_get_l0c2gm_prequant__z
 * @archs 2201
 */
__aicore__ inline uint64_t asc_get_l0c2gm_prequant();

/**
 * @ingroup simd_c_api.cube_datamove.cube_store_aux_config
 * @brief Sets L0C copy-out tensor quantization/activation parameter address indexes (relu_pre_addr, quant_pre_addr) and initializes UnitFlag global state (is_clean_unit_flag); AIC only.
 *
 * @override_id asc_set_l0c_copy_config__u64_u64_b
 * @archs 2201 3510
 * @param [in] relu_pre_addr tensor激活参数在Fixpipe Buffer中的起始地址索引，地址索引的计算方式为：`tensor激活参数在Fixpipe Buffer中的起始地址 / 64`，单位为64字节，tensor激活参数在Fixpipe Buffer中的起始地址需64字节对齐。仅当后续L0C Buffer搬出接口开启tensor激活模式时，本参数生效。
 * @param [in] quant_pre_addr tensor量化参数在Fixpipe Buffer中的起始地址索引，地址索引的计算方式为：`tensor量化参数在Fixpipe Buffer中的起始地址 / 128`，单位为128字节，tensor量化参数在Fixpipe Buffer中的起始地址需128字节对齐。仅当后续L0C Buffer搬出接口开启tensor量化模式时，本参数生效。
 * @param [in] is_clean_unit_flag 是否初始化L0C Buffer所有存储块的UnitFlag标志位。 &nbsp;&nbsp;&bull; `false`：不初始化。 &nbsp;&nbsp;&bull; `true`：初始化。
 */
__aicore__ inline void asc_set_l0c_copy_config(
    uint64_t relu_pre_addr, uint64_t quant_pre_addr, bool is_clean_unit_flag);
/**
 * @ingroup simd_c_api.cube_datamove.cube_store_aux_config
 * @brief Sets matrix count and source/destination matrix strides for Nz-to-ND/DN conversion during L0C copy-out (enable_nz2nd/enable_nz2dn); AIC only.
 *
 * @override_id asc_set_l0c_copy_nz_para__u16_u16_u32
 * @archs 3510
 * @param [in] matrix_num ND/DN矩阵的数量，取值范围为[1, 65535]。
 * @param [in] src_nz_matrix_stride 源相邻Nz矩阵之间的偏移（相邻Nz矩阵头与头的间隔）。当nd_num配置为1时，src_nd_stride不生效，配置为0即可。取值范围为[0, 65535]，单位为64字节（`16 × sizeof(dtype)`，`dtype`为L0C Buffer上数据的类型，支持`int32_t`和`float`）。
 * @param [in] dst_matrix_stride 目的相邻ND/DN矩阵之间的偏移（相邻ND/DN矩阵头与头的间隔）。当matrix_num配置为1时，dst_matrix_stride不生效，配置为0即可。取值范围为[0, 2^32-1]，单位为元素。
 */
__aicore__ inline void asc_set_l0c_copy_nz_para(
    uint16_t matrix_num, uint16_t src_nz_matrix_stride, uint32_t dst_matrix_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_store_aux_config
 * @brief Sets the source Nz fractal adjacent-row address offset in 64-byte units for Nz2DN conversion during L0C Buffer copy-out; used by Nz2DN-enabled copy-out, AIC only.
 *
 * @override_id asc_set_l0c_copy_channel_para__u16
 * @archs 3510
 * @param [in] src_nz_fractal_stride Nz2DN格式转换配置源矩阵Nz分形中相邻行的地址偏移，取值范围为[1, 65535]，单位为64字节（`16 × sizeof(dtype)`，`dtype`为L0C Buffer上数据的类型，支持`int32_t`和`float`）。
 */
__aicore__ inline void asc_set_l0c_copy_channel_para(uint16_t src_nz_fractal_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets right-matrix repeat-mode parameters for 3D img2col asc_copy_l12l0b (f_matrix_ctrl true) from a caller-packed 64-bit config; AIC only.
 *
 * @override_id asc_set_l3d_rpt_b__u64
 * @archs 3510
 * @param [in] config repeat模式相关配置。接口不检查或转换字段值，需按照表2完成拼装。
 */
__aicore__ inline void asc_set_l3d_rpt_b(uint64_t config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the padding fill value through a uint64_t register encoding for asc_copy_l12l0a 3D im2col load when f_matrix_ctrl is false; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_l12l0a_3d_padding__u64
 * @archs 2201 3510
 * @param [in] config 搬运过程中使用的填充值配置寄存器。 &nbsp;&nbsp;&bull; 若填充值类型为b32，`[0:31]`表示的值作为填充值；若填充值类型为b16，`[0:15]`表示的值作为填充值；若填充值类型为b8，要求`[15:8]`与`[7:0]`取值相同，`[7:0]`表示的值作为填充值；若填充值类型为b4，要求`[15:12]`、`[11:8]`、`[7:4]`与`[3:0]`取值相同，`[3:0]`表示的值作为填充值； &nbsp;&nbsp;&bull; `[32:63]`未使用比特位设置为`0`即可。
 */
__aicore__ inline void asc_set_l12l0a_3d_padding(uint64_t config);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the int8_t padding fill value used by asc_copy_l12l0a 3D im2col load when f_matrix_ctrl is false; shares its register with asc_set_gm2l1_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_l12l0a_3d_padding__i8
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0a_3d_padding(int8_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the uint8_t padding fill value used by asc_copy_l12l0a 3D im2col load when f_matrix_ctrl is false; shares its register with asc_set_gm2l1_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_l12l0a_3d_padding__u8
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0a_3d_padding(uint8_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the int16_t padding fill value used by asc_copy_l12l0a 3D im2col load when f_matrix_ctrl is false; shares its register with asc_set_gm2l1_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_l12l0a_3d_padding__i16
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0a_3d_padding(int16_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the uint16_t padding fill value used by asc_copy_l12l0a 3D im2col load when f_matrix_ctrl is false; shares its register with asc_set_gm2l1_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_l12l0a_3d_padding__u16
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0a_3d_padding(uint16_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the half padding fill value used by asc_copy_l12l0a 3D im2col load when f_matrix_ctrl is false; shares its register with asc_set_gm2l1_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_l12l0a_3d_padding__f16
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0a_3d_padding(half padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the bfloat16_t padding fill value used by asc_copy_l12l0a 3D im2col load when f_matrix_ctrl is false; shares its register with asc_set_gm2l1_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_l12l0a_3d_padding__bf16
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0a_3d_padding(bfloat16_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the int32_t padding fill value used by asc_copy_l12l0a 3D im2col load when f_matrix_ctrl is false; shares its register with asc_set_gm2l1_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_l12l0a_3d_padding__i32
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0a_3d_padding(int32_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the uint32_t padding fill value used by asc_copy_l12l0a 3D im2col load when f_matrix_ctrl is false; shares its register with asc_set_gm2l1_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_l12l0a_3d_padding__u32
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0a_3d_padding(uint32_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the float padding fill value used by asc_copy_l12l0a 3D im2col load when f_matrix_ctrl is false; shares its register with asc_set_gm2l1_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_l12l0a_3d_padding__f32
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0a_3d_padding(float padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the padding fill value through a uint64_t register encoding for asc_copy_l12l0b 3D im2col load when f_matrix_ctrl is true; AIC only, arch 3510.
 *
 * @override_id asc_set_l12l0b_3d_padding__u64
 * @archs 3510
 * @param [in] config 搬运过程中使用的填充值配置寄存器。 &nbsp;&nbsp;&bull; 若填充值类型为b32，`[0:31]`表示的值作为填充值；若填充值类型为b16，`[0:15]`表示的值作为填充值；若填充值类型为b8，要求`[15:8]`与`[7:0]`取值相同，`[7:0]`表示的值作为填充值；若填充值类型为b4，要求`[15:12]`、`[11:8]`、`[7:4]`与`[3:0]`取值相同，`[3:0]`表示的值作为填充值； &nbsp;&nbsp;&bull; `[32:63]`未使用比特位设置为`0`即可。
 */
__aicore__ inline void asc_set_l12l0b_3d_padding(uint64_t config);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the int8_t padding fill value used by asc_copy_l12l0b 3D im2col load when f_matrix_ctrl is true; AIC only, arch 3510.
 *
 * @override_id asc_set_l12l0b_3d_padding__i8
 * @archs 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0b_3d_padding(int8_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the uint8_t padding fill value used by asc_copy_l12l0b 3D im2col load when f_matrix_ctrl is true; AIC only, arch 3510.
 *
 * @override_id asc_set_l12l0b_3d_padding__u8
 * @archs 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0b_3d_padding(uint8_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the int16_t padding fill value used by asc_copy_l12l0b 3D im2col load when f_matrix_ctrl is true; AIC only, arch 3510.
 *
 * @override_id asc_set_l12l0b_3d_padding__i16
 * @archs 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0b_3d_padding(int16_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the uint16_t padding fill value used by asc_copy_l12l0b 3D im2col load when f_matrix_ctrl is true; AIC only, arch 3510.
 *
 * @override_id asc_set_l12l0b_3d_padding__u16
 * @archs 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0b_3d_padding(uint16_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the half padding fill value used by asc_copy_l12l0b 3D im2col load when f_matrix_ctrl is true; AIC only, arch 3510.
 *
 * @override_id asc_set_l12l0b_3d_padding__f16
 * @archs 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0b_3d_padding(half padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the bfloat16_t padding fill value used by asc_copy_l12l0b 3D im2col load when f_matrix_ctrl is true; AIC only, arch 3510.
 *
 * @override_id asc_set_l12l0b_3d_padding__bf16
 * @archs 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0b_3d_padding(bfloat16_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the int32_t padding fill value used by asc_copy_l12l0b 3D im2col load when f_matrix_ctrl is true; AIC only, arch 3510.
 *
 * @override_id asc_set_l12l0b_3d_padding__i32
 * @archs 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0b_3d_padding(int32_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the uint32_t padding fill value used by asc_copy_l12l0b 3D im2col load when f_matrix_ctrl is true; AIC only, arch 3510.
 *
 * @override_id asc_set_l12l0b_3d_padding__u32
 * @archs 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0b_3d_padding(uint32_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the float padding fill value used by asc_copy_l12l0b 3D im2col load when f_matrix_ctrl is true; AIC only, arch 3510.
 *
 * @override_id asc_set_l12l0b_3d_padding__f32
 * @archs 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_l12l0b_3d_padding(float padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the padding fill value through a uint64_t register encoding for the asc_copy_gm2l1 high-dim-split copy padding mode; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_gm2l1_padding__u64
 * @archs 2201 3510
 * @param [in] config 搬运过程中使用的填充值配置寄存器。 &nbsp;&nbsp;&bull; 若填充值类型为b32，`[0:31]`表示的值作为填充值；若填充值类型为b16，`[0:15]`表示的值作为填充值；若填充值类型为b8，要求`[15:8]`与`[7:0]`取值相同，`[7:0]`表示的值作为填充值；若填充值类型为b4，要求`[15:12]`、`[11:8]`、`[7:4]`与`[3:0]`取值相同，`[3:0]`表示的值作为填充值； &nbsp;&nbsp;&bull; `[32:63]`未使用比特位设置为`0`即可。
 */
__aicore__ inline void asc_set_gm2l1_padding(uint64_t config);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the int8_t padding fill value used by the asc_copy_gm2l1 high-dim-split copy padding mode; shares its register with asc_set_l12l0a_3d_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_gm2l1_padding__i8
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_gm2l1_padding(int8_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the uint8_t padding fill value used by the asc_copy_gm2l1 high-dim-split copy padding mode; shares its register with asc_set_l12l0a_3d_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_gm2l1_padding__u8
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_gm2l1_padding(uint8_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the int16_t padding fill value used by the asc_copy_gm2l1 high-dim-split copy padding mode; shares its register with asc_set_l12l0a_3d_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_gm2l1_padding__i16
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_gm2l1_padding(int16_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the uint16_t padding fill value used by the asc_copy_gm2l1 high-dim-split copy padding mode; shares its register with asc_set_l12l0a_3d_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_gm2l1_padding__u16
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_gm2l1_padding(uint16_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the half padding fill value used by the asc_copy_gm2l1 high-dim-split copy padding mode; shares its register with asc_set_l12l0a_3d_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_gm2l1_padding__f16
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_gm2l1_padding(half padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the bfloat16_t padding fill value used by the asc_copy_gm2l1 high-dim-split copy padding mode; shares its register with asc_set_l12l0a_3d_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_gm2l1_padding__bf16
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_gm2l1_padding(bfloat16_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the int32_t padding fill value used by the asc_copy_gm2l1 high-dim-split copy padding mode; shares its register with asc_set_l12l0a_3d_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_gm2l1_padding__i32
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_gm2l1_padding(int32_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the uint32_t padding fill value used by the asc_copy_gm2l1 high-dim-split copy padding mode; shares its register with asc_set_l12l0a_3d_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_gm2l1_padding__u32
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_gm2l1_padding(uint32_t padding_value);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the float padding fill value used by the asc_copy_gm2l1 high-dim-split copy padding mode; shares its register with asc_set_l12l0a_3d_padding; AIC only, arch 2201/3510.
 *
 * @override_id asc_set_gm2l1_padding__f32
 * @archs 2201 3510
 * @param [in] padding_value 搬运过程中使用的填充值。
 */
__aicore__ inline void asc_set_gm2l1_padding(float padding_value);

/**
 * @ingroup simd_c_api.cube_datamove.cube_store_aux_config
 * @brief asc_set_l0c_copy_relu_alpha sets the Scalar activation scale coefficient (1,8,10 format, high 19 bits valid) used by Ascend 950 L0C copy-out with relu_pre_mode; AIC only.
 *
 * @override_id asc_set_l0c_copy_relu_alpha__f32
 * @archs 3510
 * @param [in] scalar_relu_pre_alpha Scalar激活缩放系数，仅高19位有效，为(1, 8, 10)浮点格式，即1bit符号位、8bit指数位和10bit尾数位，取值不能为inf或nan。
 */
__aicore__ inline void asc_set_l0c_copy_relu_alpha(float scalar_relu_pre_alpha);
/**
 * @ingroup simd_c_api.cube_datamove.cube_store_aux_config
 * @brief asc_set_l0c_copy_lrelu_alpha sets the Scalar activation scale coefficient (1,8,10 format, high 19 bits valid) used by L0C copy-out with relu_pre_mode; AIC only.
 *
 * @override_id asc_set_l0c_copy_lrelu_alpha__f32
 * @archs 2201 3510
 * @param [in] scalar_relu_pre_alpha Scalar激活缩放系数，仅高19位有效，为(1, 8, 10)浮点格式，即1bit符号位、8bit指数位和10bit尾数位，取值不能为inf或nan。
 */
__aicore__ inline void asc_set_l0c_copy_lrelu_alpha(float scalar_relu_pre_alpha);

// ==========asc_copy_l12fb==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies quantization parameters from L1 Buffer src to Fixpipe Buffer dst via high-dimensional slicing with n_burst, len_burst and 32-byte-unit src/dst gaps (void return, result in dst).
 *
 * @override_id asc_copy_l12fb__PFBv_PL1v_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap_size 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap_size 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_l12fb(
    __fbuf__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap_size,
    uint16_t dst_gap_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies size bytes of quantization parameters from L1 Buffer src to Fixpipe Buffer dst (void return, result in dst); first-n form requires 32-byte-aligned size.
 *
 * @override_id asc_copy_l12fb__PFBv_PL1v_u32
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size);

//=============asc_copy_l12bt===============
// void -> uint64_t
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies Bias data from L1 Buffer src to BiasTable Buffer dst via high-dimensional slicing with conv_control, n_burst, len_burst and 32-byte-unit source_gap/dst_gap (void return, result in dst).
 *
 * @override_id asc_copy_l12bt__u64_PL1v_u16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] conv_control 是否开启随路转化。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] source_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ void* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies size bytes of Bias data from L1 Buffer src to BiasTable Buffer dst (void return, result in dst); first-n form requires 32-byte-aligned size.
 *
 * @override_id asc_copy_l12bt__u64_PL1v_u32
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint32_t size);

// ==========asc_fill_l0a(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a half scalar into every half element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAf16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ half* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a uint32_t scalar into every half element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAf16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a half scalar into every float element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAf32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ float* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a uint32_t scalar into every float element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAf32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a half scalar into every int16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAi16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ int16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a uint32_t scalar into every int16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAi16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a half scalar into every int32_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAi32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ int32_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a uint32_t scalar into every int32_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAi32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a half scalar into every uint16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAu16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a uint32_t scalar into every uint16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAu16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a half scalar into every uint32_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAu32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a uint32_t scalar into every uint32_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAu32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0A Buffer local memory by writing the binary value of a bfloat16_t scalar into every bfloat16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0a__PCAbf16_bf16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

// ==========asc_fill_l1(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
__aicore__ inline void asc_fill_l1(__cbuf__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1(__cbuf__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Non-sync fill writing a bfloat16_t scalar value into L1 Buffer dst and broadcasting it to multiple 32-byte fractal blocks per asc_fill_value_config (void return); caller must add sync.
 *
 * @override_id asc_fill_l1__PL1bf16_bf16_RCfillcfg
 * @archs 2201 3510
 */
__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Non-sync fill writing a half scalar value into L1 Buffer dst and broadcasting it to multiple 32-byte fractal blocks per asc_fill_value_config (void return); caller must add sync.
 *
 * @override_id asc_fill_l1__PL1bf16_f16_RCfillcfg
 * @archs 2201 3510
 */
__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Non-sync fill writing a uint32_t scalar value into L1 Buffer dst and broadcasting it to multiple 32-byte fractal blocks per asc_fill_value_config (void return); caller must add sync.
 *
 * @override_id asc_fill_l1__PL1bf16_u32_RCfillcfg
 * @archs 2201 3510
 */
__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Configures the right-matrix Feature Map descriptor for 3D img2col asc_copy_l12l0b (f_matrix_ctrl true), taking a caller-built asc_l13d_fmatrix_config; setup only, AIC.
 *
 * @override_id asc_set_l13d_fmatrix_b__Rl13dcfg
 * @archs 2201 3510
 * @param [in] config Feature Map属性配置，仅传入config位域类型参数的接口使用，详细说明请参见[asc_l13d_fmatrix_config](../../defs/union/asc_l13d_fmatrix_config.md)。
 */
__aicore__ inline void asc_set_l13d_fmatrix_b(asc_l13d_fmatrix_config& config);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Configures the right-matrix Feature Map descriptor for 3D img2col asc_copy_l12l0b (f_matrix_ctrl true) from fmatrix_w, fmatrix_h and pad_left/right/top/bottom; setup only, AIC.
 *
 * @override_id asc_set_l13d_fmatrix_b__u16_u16_u8_u8_u8_u8
 * @archs 2201 3510
 * @param [in] fmatrix_w Feature Map的width，取值范围：[1, 32767]。
 * @param [in] fmatrix_h Feature Map的height，取值范围：[1, 32767]。
 * @param [in] pad_left 左侧padding大小，取值范围：[0, 255]。
 * @param [in] pad_right 右侧padding大小，取值范围：[0, 255]。
 * @param [in] pad_top 上侧padding大小，取值范围：[0, 255]。
 * @param [in] pad_bottom 下侧padding大小，取值范围：[0, 255]。
 */
__aicore__ inline void asc_set_l13d_fmatrix_b(
    uint16_t fmatrix_w, uint16_t fmatrix_h, uint8_t pad_left, uint8_t pad_right, uint8_t pad_top, uint8_t pad_bottom);

// ==========asc_fill_l0b(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a half scalar into every half element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBf16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ half* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a uint32_t scalar into every half element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBf16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a half scalar into every float element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBf32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ float* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a uint32_t scalar into every float element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBf32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a half scalar into every int16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBi16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ int16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a uint32_t scalar into every int16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBi16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a half scalar into every int32_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBi32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ int32_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a uint32_t scalar into every int32_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBi32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a half scalar into every uint16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBu16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a uint32_t scalar into every uint16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBu16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a half scalar into every uint32_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBu32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a uint32_t scalar into every uint32_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBu32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a bfloat16_t scalar into every bfloat16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBbf16_bf16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a half scalar into every bfloat16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBbf16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ bfloat16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Initializes L0B Buffer local memory by writing the binary value of a uint32_t scalar into every bfloat16_t element of dst, per asc_fill_value_config (void; result in dst).
 *
 * @override_id asc_fill_l0b__PCBbf16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b(__cb__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config);

// ==========asc_copy_gm2l1==========
// PAD_NONE
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief High-dimensional copy on arch 2201: copies n_burst blocks of len_burst DataBlocks from GM src to L1 Buffer dst using src_gap/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1__PL1v_PGv_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Ordinary copy on arch 2201: copies size bytes from GM src to L1 Buffer dst without padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

// PAD_MODE1
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding high-dimensional copy on arch 2201: copies n_burst blocks of len_burst DataBlocks from GM src to L1 Buffer dst with pad1 layout (31B pad per 1B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad1__PL1v_PGv_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad1(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding copy on arch 2201: copies size bytes from GM src to L1 Buffer dst with pad1 layout (31B pad per 1B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad1__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad1(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

// PAD_MODE2
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding high-dimensional copy on arch 2201: copies n_burst blocks of len_burst DataBlocks from GM src to L1 Buffer dst with pad2 layout (15B pad per 1B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad2__PL1v_PGv_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad2(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding copy on arch 2201: copies size bytes from GM src to L1 Buffer dst with pad2 layout (15B pad per 1B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad2__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad2(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

// PAD_MODE3
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding high-dimensional copy on arch 2201: copies n_burst blocks of len_burst DataBlocks from GM src to L1 Buffer dst with pad3 layout (14B pad per 2B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad3__PL1v_PGv_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad3(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding copy on arch 2201: copies size bytes from GM src to L1 Buffer dst with pad3 layout (14B pad per 2B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad3__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad3(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

// PAD_MODE4
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding high-dimensional copy on arch 2201: copies n_burst blocks of len_burst DataBlocks from GM src to L1 Buffer dst with pad4 layout (12B pad per 4B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad4__PL1v_PGv_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad4(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding copy on arch 2201: copies size bytes from GM src to L1 Buffer dst with pad4 layout (12B pad per 4B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad4__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad4(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

// PAD_MODE5
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding high-dimensional copy on arch 2201: copies n_burst blocks of len_burst DataBlocks from GM src to L1 Buffer dst with pad5 layout (8B pad per 8B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad5__PL1v_PGv_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad5(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding copy on arch 2201: copies size bytes from GM src to L1 Buffer dst with pad5 layout (8B pad per 8B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad5__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad5(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

// PAD_MODE6
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding high-dimensional copy on arch 2201: copies n_burst blocks of len_burst DataBlocks from GM src to L1 Buffer dst with pad6 layout (keep only lowest 4B of each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad6__PL1v_PGv_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad6(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding copy on arch 2201: copies size bytes from GM src to L1 Buffer dst with pad6 layout (keep only lowest 4B of each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad6__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad6(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

// PAD_MODE7
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding high-dimensional copy on arch 2201: copies n_burst blocks of len_burst DataBlocks from GM src to L1 Buffer dst with pad7 layout (keep only lowest 8B of each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad7__PL1v_PGv_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad7(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding copy on arch 2201: copies size bytes from GM src to L1 Buffer dst with pad7 layout (keep only lowest 8B of each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad7__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad7(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

// PAD_MODE8
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding high-dimensional copy on arch 2201: copies n_burst blocks of len_burst DataBlocks from GM src to L1 Buffer dst with pad8 layout (keep only lowest 16B of each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad8__PL1v_PGv_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。
 * @param [in] src_gap 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。 单位为DataBlock（32字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad8(
    __cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Padding copy on arch 2201: copies size bytes from GM src to L1 Buffer dst with pad8 layout (keep only lowest 16B of each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad8__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad8(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

// ==========asc_copy_gm2l1==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief 2D copy on arch 2201: copies bfloat16_t matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations using src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1__PL1bf16_PGbf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief 2D copy on arch 2201: copies half matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations using src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1__PL1f16_PGf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief 2D copy on arch 2201: copies float matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations using src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1__PL1f32_PGf32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief 2D copy on arch 2201: copies int32_t matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations using src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1__PL1i32_PGi32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief 2D copy on arch 2201: copies int8_t matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations using src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1__PL1i8_PGi8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief 2D copy on arch 2201: copies uint32_t matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations using src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1__PL1u32_PGu32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief 2D copy on arch 2201: copies uint8_t matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations using src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1__PL1u8_PGu8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

// ==========asc_copy_gm2l1_nd2nz b8(int8_t/uint8_t)==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int8_t data from Global Memory to an L1 Buffer destination while converting ND layout to NZ layout for nd_num matrices, using configurable source and destination strides.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1i8_PGi8_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint8_t data from Global Memory to an L1 Buffer destination while converting ND layout to NZ layout for nd_num matrices, using configurable source and destination strides.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1u8_PGu8_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

// ==========asc_copy_gm2l1_nd2nz b16(bfloat16_t/half/int16_t)==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies bfloat16_t data from Global Memory to an L1 Buffer destination while converting ND layout to NZ layout for nd_num matrices, using configurable source and destination strides.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1bf16_PGbf16_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies half data from Global Memory to an L1 Buffer destination while converting ND layout to NZ layout for nd_num matrices, using configurable source and destination strides.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1f16_PGf16_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int16_t data from Global Memory to an L1 Buffer destination while converting ND layout to NZ layout for nd_num matrices, using configurable source and destination strides.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1i16_PGi16_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

// ==========asc_copy_gm2l1_nd2nz b32s(float/int32_t/uint32_t)==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies float data from Global Memory to an L1 Buffer destination while converting ND layout to NZ layout for nd_num matrices, using configurable source and destination strides.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1f32_PGf32_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int32_t data from Global Memory to an L1 Buffer destination while converting ND layout to NZ layout for nd_num matrices, using configurable source and destination strides.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1i32_PGi32_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint32_t data from Global Memory to an L1 Buffer destination while converting ND layout to NZ layout for nd_num matrices, using configurable source and destination strides.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1u32_PGu32_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

// ==========asc_copy_l12l0b, 2D, int4b_t/uint8_t/int8_t/half/bfloat16_t/uint32_t/int32_t/float==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies 512-byte fractal matrices from an L1 Buffer int4b_t source to an L0B Buffer destination in 2D format, starting at fractal matrix id start_index.
 *
 * @override_id asc_copy_l12l0b__PCBi4x2_PL1i4x2_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies 512-byte fractal matrices from an L1 Buffer uint8_t source to an L0B Buffer destination in 2D format, starting at fractal matrix id start_index.
 *
 * @override_id asc_copy_l12l0b__PCBu8_PL1u8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies 512-byte fractal matrices from an L1 Buffer int8_t source to an L0B Buffer destination in 2D format, starting at fractal matrix id start_index.
 *
 * @override_id asc_copy_l12l0b__PCBi8_PL1i8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies 512-byte fractal matrices from an L1 Buffer half source to an L0B Buffer destination in 2D format, starting at fractal matrix id start_index.
 *
 * @override_id asc_copy_l12l0b__PCBf16_PL1f16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies 512-byte fractal matrices from an L1 Buffer bfloat16_t source to an L0B Buffer destination in 2D format, starting at fractal matrix id start_index.
 *
 * @override_id asc_copy_l12l0b__PCBbf16_PL1bf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies 512-byte fractal matrices from an L1 Buffer uint32_t source to an L0B Buffer destination in 2D format, starting at fractal matrix id start_index.
 *
 * @override_id asc_copy_l12l0b__PCBu32_PL1u32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies 512-byte fractal matrices from an L1 Buffer int32_t source to an L0B Buffer destination in 2D format, starting at fractal matrix id start_index.
 *
 * @override_id asc_copy_l12l0b__PCBi32_PL1i32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies 512-byte fractal matrices from an L1 Buffer float source to an L0B Buffer destination in 2D format, starting at fractal matrix id start_index.
 *
 * @override_id asc_copy_l12l0b__PCBf32_PL1f32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

// ==========asc_copy_l12l0b, 3D, half/bfloat16_t/uint32_t/int32_t/float==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a half NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBf16_PL1f16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a bfloat16_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBbf16_PL1bf16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a uint32_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBu32_PL1u32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a int32_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBi32_PL1i32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a float NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBf32_PL1f32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

// ==========asc_copy_l12l0a, 2D, int4b_t/uint8_t/int8_t/half/bfloat16_t/uint32_t/int32_t/float==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 2D high-dim-split copy: moves 512-byte int4b_t fractal blocks from L1 Buffer src to L0A Buffer dst starting at fractal start_index, repeating repeat times; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l12l0a__PCAi4x2_PL1i4x2_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 2D high-dim-split copy: moves 512-byte uint8_t fractal blocks from L1 Buffer src to L0A Buffer dst starting at fractal start_index, repeating repeat times; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l12l0a__PCAu8_PL1u8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 2D high-dim-split copy: moves 512-byte int8_t fractal blocks from L1 Buffer src to L0A Buffer dst starting at fractal start_index, repeating repeat times; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l12l0a__PCAi8_PL1i8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 2D high-dim-split copy: moves 512-byte half fractal blocks from L1 Buffer src to L0A Buffer dst starting at fractal start_index, repeating repeat times; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l12l0a__PCAf16_PL1f16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 2D high-dim-split copy: moves 512-byte bfloat16_t fractal blocks from L1 Buffer src to L0A Buffer dst starting at fractal start_index, repeating repeat times; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l12l0a__PCAbf16_PL1bf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 2D high-dim-split copy: moves 512-byte uint32_t fractal blocks from L1 Buffer src to L0A Buffer dst starting at fractal start_index, repeating repeat times; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l12l0a__PCAu32_PL1u32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 2D high-dim-split copy: moves 512-byte int32_t fractal blocks from L1 Buffer src to L0A Buffer dst starting at fractal start_index, repeating repeat times; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l12l0a__PCAi32_PL1i32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 2D high-dim-split copy: moves 512-byte float fractal blocks from L1 Buffer src to L0A Buffer dst starting at fractal start_index, repeating repeat times; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l12l0a__PCAf32_PL1f32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

// ==========asc_copy_l12l0a, 3D, int4b_t/uint8_t/int8_t/half/bfloat16_t/uint32_t/int32_t/float==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col copy: moves int4b_t NC1HWC0 blocks from L1 Buffer src to L0A Buffer dst using k/m extension, convolution stride, filter, dilation and channel_size; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l12l0a__PCAi4x2_PL1i4x2_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] k_extension 该指令在目的操作数width维度的传输长度。如果不覆盖最右侧的分形，对于half类型，应为16的倍数，对于int8_t/uint8_t类型，应为32的倍数；如果覆盖最右侧的分形，则无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 该指令在目的操作数height维度的传输长度。如果不覆盖最下侧的分形，对于half/int8_t/uint8_t类型，应为16的倍数；如果覆盖最下侧的分形，则无倍数要求。取值范围：[1, 65535]。
 * @param [in] k_start_pt 该指令在目的操作数width维度的起点。对于half类型，应为16的倍数，对于int8_t/uint8_t类型，应为32的倍数。取值范围：[0, 65535]
 * @param [in] m_start_pt 该指令在目的操作数height维度的起点，如果不覆盖最下侧的分形，对于half/int8_t/uint8_t，应为16的倍数；如果覆盖最下侧的分形，则无倍数要求。取值范围：[0, 65535]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在filter_w的基础上将卷积核width增加256个元素。true表示增加；false表示不增加。
 * @param [in] filter_size_h 是否在filter_h的基础上将卷积核height增加256个元素。true表示增加；false表示不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置，仅在源操作数为half类型时有效。true表示启用；false表示不启用。
 * @param [in] f_matrix_ctrl 表示asc_copy_l12l0a指令从左矩阵还是右矩阵获取FeatureMap的属性描述，当前只支持设置为false。
 * @param [in] channel_size 源操作数的通道数，取值范围：[1, 63]。对于uint32_t/int32_t/float，channelSize可取值为4，N * 8，N * 8 + 4；对于half/bfloat16，channelSize可取值为4，8，N * 16，N * 16 + 4，N * 16 + 8；对于int8_t/uint8_t，channelSize可取值为4，8，16， 32 * N，N * 32 + 4，N * 32 + 8，N * 32 + 16；对于int4b_t，ChannelSize可取值为8，16，32，N * 64，N * 64 + 8，N * 64 + 16，N * 64 + 32。N为正整数。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a uint8_t NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAu8_PL1u8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a int8_t NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAi8_PL1i8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a half NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAf16_PL1f16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a bfloat16_t NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAbf16_PL1bf16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a uint32_t NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAu32_PL1u32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a int32_t NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAi32_PL1i32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a float NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAf32_PL1f32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Moves a dense 512B weight matrix per repeat from L1 Buffer src to L0B Buffer dst while reading a 128B index matrix from L1 for sparsification; no transpose.
 *
 * @override_id asc_copy_l12l0b_sparse__PCBi8_PL1i8_PL1i8_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数在L0B Buffer的起始地址。
 * @param [in] src 源操作数在L1 Buffer的起始地址。
 * @param [in] index 索引矩阵在L1 Buffer的起始地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位：512B。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 */
__aicore__ inline void asc_copy_l12l0b_sparse(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat);

// ==========asc_copy_l12l0a_trans=========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of half data from L1 Buffer to L0A Buffer; merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0a_trans__PCAf16_PL1f16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of bfloat16_t data from L1 Buffer to L0A Buffer; merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0a_trans__PCAbf16_PL1bf16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of float data from L1 Buffer to L0A Buffer; merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0a_trans__PCAf32_PL1f32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of int32_t data from L1 Buffer to L0A Buffer; merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0a_trans__PCAi32_PL1i32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of int8_t data from L1 Buffer to L0A Buffer; merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0a_trans__PCAi8_PL1i8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of uint32_t data from L1 Buffer to L0A Buffer; merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0a_trans__PCAu32_PL1u32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of uint8_t data from L1 Buffer to L0A Buffer; merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0a_trans__PCAu8_PL1u8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

// ==========asc_copy_l12l0b_trans=========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of half data from L1 Buffer to L0B Buffer (arch 2201); merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0b_trans__PCBf16_PL1f16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of bfloat16_t data from L1 Buffer to L0B Buffer (arch 2201); merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0b_trans__PCBbf16_PL1bf16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of float data from L1 Buffer to L0B Buffer (arch 2201); merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0b_trans__PCBf32_PL1f32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of int32_t data from L1 Buffer to L0B Buffer (arch 2201); merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0b_trans__PCBi32_PL1i32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of int8_t data from L1 Buffer to L0B Buffer (arch 2201); merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0b_trans__PCBi8_PL1i8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of uint32_t data from L1 Buffer to L0B Buffer (arch 2201); merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0b_trans__PCBu32_PL1u32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Asynchronous transposed 2D-format load of uint8_t data from L1 Buffer to L0B Buffer (arch 2201); merges consecutive fractals into a square matrix, transposes, then splits them.
 *
 * @override_id asc_copy_l12l0b_trans__PCBu8_PL1u8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the L1 Buffer boundary in bytes for asc_copy_l12l0a/asc_copy_l12l0b 3D-format moves; non-zero values must be at least 1024, 0 means no boundary.
 *
 * @override_id asc_set_l13d_size__u64
 * @archs 2201
 * @param [in] value 用于设置asc_copy_l12l0a/asc_copy_l12l0b的3D格式搬运接口在L1 Buffer的边界值，单位是字节。非0场景的边界值不能小于1024。 如果value设置为0，则表示无边界，asc_copy_l12l0a/asc_copy_l12l0b接口可使用整个L1 Buffer。
 */
__aicore__ inline void asc_set_l13d_size(uint64_t value);
// ==========asc_copy_l0c2gm==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief C API L0C-to-GM store: quantizes float L0C src to half with F322F16 and copies to GM dst, with optional ReLU, channel split and NZ2ND; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l0c2gm__PGf16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief C API L0C-to-GM store: quantizes float L0C src to bfloat16_t with F322BF16 and copies to GM dst, with optional ReLU, channel split and NZ2ND; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l0c2gm__PGbf16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief C API L0C-to-GM store: quantizes float L0C src to int8_t with QF322B8_PRE or VQF322B8_PRE and copies to GM dst, with optional ReLU and NZ2ND; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l0c2gm__PGi8_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief C API L0C-to-GM store: quantizes float L0C src to uint8_t with QF322B8_PRE or VQF322B8_PRE and copies to GM dst, with optional ReLU and NZ2ND; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l0c2gm__PGu8_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief C API L0C-to-GM store: copies float L0C src to float GM dst without quantization (NoQuant), with optional ReLU, channel split and NZ2ND layout conversion; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l0c2gm__PGf32_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief C API L0C-to-GM store: quantizes int32_t L0C src to half with DEQF16 or VDEQF16 and copies to GM dst, with optional ReLU, channel split and NZ2ND; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l0c2gm__PGf16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief C API L0C-to-GM store: dequantizes int32_t L0C src to int16_t GM dst, with optional ReLU, channel split and NZ2ND; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l0c2gm__PGi16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief C API L0C-to-GM store: quantizes int32_t L0C src to int8_t with REQ8 or VREQ8 and copies to GM dst, with optional ReLU, channel split and NZ2ND; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l0c2gm__PGi8_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief C API L0C-to-GM store: copies int32_t L0C src to int32_t GM dst with a configurable quantization parameter, plus optional ReLU, channel split and NZ2ND; A2/A3 arch 2201.
 *
 * @override_id asc_copy_l0c2gm__PGi32_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

// ==========asc_load_image_to_cbuf(half/int8_t)==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Loads an image from Global Memory into L1 Buffer dst (half) with hor_size/ver_size, start positions, src_hor_size and top/bot/left/right padding; result in dst (void return).
 *
 * @override_id asc_load_image_to_cbuf__PL1f16_u16_u16_u16_u16_u16_u8_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] hor_size 从源图中加载图片的水平宽度，单位为像素。取值范围：[2, 4094]。
 * @param [in] ver_size 从源图中加载图片的垂直高度，单位为像素。取值范围：[2, 4094]。
 * @param [in] hor_start_pos 加载图片在源图片上的水平起始地址，单位为像素。取值范围：[0, 4095]。
 * @param [in] ver_start_pos 加载图片在源图片上的垂直起始地址，单位为像素。取值范围：[0, 4095]。
 * @param [in] src_hor_size 源图片水平宽度，单位为像素。取值范围：[2, 4094]。
 * @param [in] top_pad_size 目的图片顶部填充的像素数。取值范围：[0, 32]。
 * @param [in] bot_pad_size 目的图片底部填充的像素数。取值范围：[0, 32]。
 * @param [in] left_pad_size 目的图片左边填充的像素数。取值范围：[0, 32]。
 * @param [in] right_pad_size 目的图片右边填充的像素数。取值范围：[0, 32]。
 */
__aicore__ inline void asc_load_image_to_cbuf(
    __cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Loads an image from Global Memory into L1 Buffer dst (int8_t) with hor_size/ver_size, start positions, src_hor_size and top/bot/left/right padding; result in dst (void return).
 *
 * @override_id asc_load_image_to_cbuf__PL1i8_u16_u16_u16_u16_u16_u8_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] hor_size 从源图中加载图片的水平宽度，单位为像素。取值范围：[2, 4094]。
 * @param [in] ver_size 从源图中加载图片的垂直高度，单位为像素。取值范围：[2, 4094]。
 * @param [in] hor_start_pos 加载图片在源图片上的水平起始地址，单位为像素。取值范围：[0, 4095]。
 * @param [in] ver_start_pos 加载图片在源图片上的垂直起始地址，单位为像素。取值范围：[0, 4095]。
 * @param [in] src_hor_size 源图片水平宽度，单位为像素。取值范围：[2, 4094]。
 * @param [in] top_pad_size 目的图片顶部填充的像素数。取值范围：[0, 32]。
 * @param [in] bot_pad_size 目的图片底部填充的像素数。取值范围：[0, 32]。
 * @param [in] left_pad_size 目的图片左边填充的像素数。取值范围：[0, 32]。
 * @param [in] right_pad_size 目的图片右边填充的像素数。取值范围：[0, 32]。
 */
__aicore__ inline void asc_load_image_to_cbuf(
    __cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_store_aux_config
 * @brief Sets scalar quantization parameters (scale, offset, is_signed packed in config) for L0C Buffer copy-out on AIC; actual quantization runs in the later copy-out interface.
 *
 * @override_id asc_set_l0c_copy_prequant__u64
 * @archs 2201 3510
 * @param [in] config Scalar量化参数，由`scale`，`offset`，`is_signed`拼装而成，config各比特位含义参见表2。
 */
__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config);
/**
 * @ingroup simd_c_api.cube_datamove.cube_store_aux_config
 * @brief Sets scalar quantization scale, offset and signedness for L0C Buffer copy-out on AIC from explicit scale/offset/is_signed; quantization itself is done by the later copy-out interface.
 *
 * @override_id asc_set_l0c_copy_prequant__f32_u16_b
 * @archs 3510
 * @param [in] scale 随路scalar量化所使用的缩放系数。取值不能为inf或nan。
 * @param [in] offset 随路量化计算所需的偏移量，取值范围为[0, 511]。不使用偏移量时，配置为0。在REQ8/QF322B8_PRE/REQ4/QF322S4_PRE模式下生效。
 * @param [in] is_signed 量化结果是否为有符号数据。取值为`true`时，量化结果为int8_t；取值为`false`时，量化结果为uint8_t。在REQ8/QF322B8_PRE模式下生效。
 */
__aicore__ inline void asc_set_l0c_copy_prequant(float scale, uint16_t offset, bool is_signed);
/**
 * @ingroup simd_c_api.cube_datamove.cube_store_aux_config
 * @brief Configures nd_num, src_nd_stride and dst_nd_stride for NZ-to-ND conversion during L0C-to-GM/L1 copies on Atlas A2/A3; src_nd_stride unit is 1024B fractal, dst_nd_stride unit is element.
 *
 * @override_id asc_set_l0c_copy_params__u16_u16_u16
 * @archs 2201
 * @param [in] nd_num ND矩阵的数量，取值范围：nd_num∈[1, 65535]。
 * @param [in] src_nd_stride 以分形大小为单位的源步长，源相邻nz矩阵的起始地址间的偏移。  src_nd_stride∈[1, 512]，单位：fractal_size 1024B。
 * @param [in] dst_nd_stride 目的相邻ND矩阵的起始地址间的偏移。单位为元素。   dst_nd_stride∈[1, 65535]。
 */
__aicore__ inline void asc_set_l0c_copy_params(uint16_t nd_num, uint16_t src_nd_stride, uint16_t dst_nd_stride);
// ==========asc_copy_gm2l0b==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies bfloat16_t matrix data from Global Memory to an L0B Buffer destination, starting at the 16x16 block id base_idx with configurable repeat, source stride and destination gap.
 *
 * @override_id asc_copy_gm2l0b__PCBbf16_PGbf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b(
    __cb__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies half matrix data from Global Memory to an L0B Buffer destination, starting at the 16x16 block id base_idx with configurable repeat, source stride and destination gap.
 *
 * @override_id asc_copy_gm2l0b__PCBf16_PGf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b(
    __cb__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies float matrix data from Global Memory to an L0B Buffer destination, starting at the 16x16 block id base_idx with configurable repeat, source stride and destination gap.
 *
 * @override_id asc_copy_gm2l0b__PCBf32_PGf32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b(
    __cb__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int32_t matrix data from Global Memory to an L0B Buffer destination, starting at the 16x16 block id base_idx with configurable repeat, source stride and destination gap.
 *
 * @override_id asc_copy_gm2l0b__PCBi32_PGi32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b(
    __cb__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int8_t matrix data from Global Memory to an L0B Buffer destination, starting at the 16x16 block id base_idx with configurable repeat, source stride and destination gap.
 *
 * @override_id asc_copy_gm2l0b__PCBi8_PGi8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b(
    __cb__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint32_t matrix data from Global Memory to an L0B Buffer destination, starting at the 16x16 block id base_idx with configurable repeat, source stride and destination gap.
 *
 * @override_id asc_copy_gm2l0b__PCBu32_PGu32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b(
    __cb__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint8_t matrix data from Global Memory to an L0B Buffer destination, starting at the 16x16 block id base_idx with configurable repeat, source stride and destination gap.
 *
 * @override_id asc_copy_gm2l0b__PCBu8_PGu8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b(
    __cb__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int4b_t matrix data from Global Memory to an L0B Buffer destination, starting at the 16x16 block id base_idx with configurable repeat, source stride and destination gap.
 *
 * @override_id asc_copy_gm2l0b__PCBi4x2_PGi4x2_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b(
    __cb__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

// ==========asc_copy_gm2l0a bfloat16_t/half/float/int32_t/uint8_t/int8_t/uint32_t/int4b_t===========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies bfloat16_t data from Global Memory (GM) to an L0A Buffer destination, starting at fractal block base_idx and repeating over 512-byte blocks with configurable strides.
 *
 * @override_id asc_copy_gm2l0a__PCAbf16_PGbf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a(
    __ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies half data from Global Memory (GM) to an L0A Buffer destination, starting at fractal block base_idx and repeating over 512-byte blocks with configurable strides.
 *
 * @override_id asc_copy_gm2l0a__PCAf16_PGf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a(
    __ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies float data from Global Memory (GM) to an L0A Buffer destination, starting at fractal block base_idx and repeating over 512-byte blocks with configurable strides.
 *
 * @override_id asc_copy_gm2l0a__PCAf32_PGf32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a(
    __ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int32_t data from Global Memory (GM) to an L0A Buffer destination, starting at fractal block base_idx and repeating over 512-byte blocks with configurable strides.
 *
 * @override_id asc_copy_gm2l0a__PCAi32_PGi32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a(
    __ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint8_t data from Global Memory (GM) to an L0A Buffer destination, starting at fractal block base_idx and repeating over 512-byte blocks with configurable strides.
 *
 * @override_id asc_copy_gm2l0a__PCAu8_PGu8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a(
    __ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int8_t data from Global Memory (GM) to an L0A Buffer destination, starting at fractal block base_idx and repeating over 512-byte blocks with configurable strides.
 *
 * @override_id asc_copy_gm2l0a__PCAi8_PGi8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a(
    __ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint32_t data from Global Memory (GM) to an L0A Buffer destination, starting at fractal block base_idx and repeating over 512-byte blocks with configurable strides.
 *
 * @override_id asc_copy_gm2l0a__PCAu32_PGu32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a(
    __ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int4b_t data from Global Memory (GM) to an L0A Buffer destination, starting at fractal block base_idx and repeating over 512-byte blocks with configurable strides.
 *
 * @override_id asc_copy_gm2l0a__PCAi4x2_PGi4x2_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a(
    __ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

// ==========asc_copy_l0c2l1==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Quantizes FP32 L0C matrix results and copies them to an L1 Buffer half destination, with optional accompanying ReLU, channel split and NZ2ND.
 *
 * @override_id asc_copy_l0c2l1__PL1f16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Quantizes FP32 L0C matrix results and copies them to an L1 Buffer bfloat16_t destination, with optional accompanying ReLU, channel split and NZ2ND.
 *
 * @override_id asc_copy_l0c2l1__PL1bf16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Quantizes FP32 L0C matrix results and copies them to an L1 Buffer int8_t destination, with optional accompanying ReLU, channel split and NZ2ND.
 *
 * @override_id asc_copy_l0c2l1__PL1i8_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Quantizes int32_t L0C matrix results and copies them to an L1 Buffer half destination, with optional accompanying ReLU, channel split and NZ2ND.
 *
 * @override_id asc_copy_l0c2l1__PL1f16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Quantizes int32_t L0C matrix results and copies them to an L1 Buffer int16_t destination, with optional accompanying ReLU, channel split and NZ2ND.
 *
 * @override_id asc_copy_l0c2l1__PL1i16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Quantizes int32_t L0C matrix results and copies them to an L1 Buffer int8_t destination, with optional accompanying ReLU, channel split and NZ2ND.
 *
 * @override_id asc_copy_l0c2l1__PL1i8_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Quantizes int32_t L0C matrix results and copies them to an L1 Buffer uint8_t destination, with optional accompanying ReLU, channel split and NZ2ND.
 *
 * @override_id asc_copy_l0c2l1__PL1u8_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Quantizes FP32 L0C matrix results and copies them to an L1 Buffer destination addressed by void* dst, with optional accompanying ReLU, channel split and NZ2ND.
 *
 * @override_id asc_copy_l0c2l1__PL1v_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ void* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Quantizes int32_t L0C matrix results and copies them to an L1 Buffer destination addressed by void* dst, with optional accompanying ReLU, channel split and NZ2ND.
 *
 * @override_id asc_copy_l0c2l1__PL1v_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre 开启relu。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ void* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

// ==========asc_copy_l12l0a_mx==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies the left-matrix MX scale factors from L1 Buffer src to L0A_MX Buffer dst in 32-byte fp8_e8m0_t fractals, paired with asc_copy_l12l0a; AIC only.
 *
 * @override_id asc_copy_l12l0a_mx__u64_PL1f8e8m0_u16_u16_u8_u8_u16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_mx(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets inner (loop1_size) and outer (loop2_size) iteration counts for asc_copy_gm2l1_align loop-fill mode; used together with loop1/loop2 stride settings.
 *
 * @override_id asc_set_gm2l1_loop_size__u64_u64
 * @archs 3510
 * @param [in] loop1_size 内层循环的循环次数，取值范围为[0, $2^{21}-1$]。
 * @param [in] loop2_size 外层循环的循环次数，取值范围为[0, $2^{23}-1$]。
 */
__aicore__ inline void asc_set_gm2l1_loop_size(uint64_t loop1_size, uint64_t loop2_size);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the inner-loop (loop1) source and destination L1 byte strides between iterations for asc_copy_gm2l1_align loop-fill mode; loop1_dst_stride must be 32-byte aligned.
 *
 * @override_id asc_set_gm2l1_loop1_stride__u64_u64
 * @archs 3510
 * @param [in] loop1_src_stride 内层循环中相邻迭代源操作数的数据块间隔，单位为字节，取值范围为[0, $2^{40}-1$]。
 * @param [in] loop1_dst_stride 内层循环中相邻迭代目的L1数据块间隔，单位为字节，取值范围为[0, $2^{21}-1$]，且必须32字节对齐。
 */
__aicore__ inline void asc_set_gm2l1_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the outer-loop (loop2) source and destination L1 byte strides between iterations for asc_copy_gm2l1_align loop-fill mode; loop2_dst_stride must be 32-byte aligned.
 *
 * @override_id asc_set_gm2l1_loop2_stride__u64_u64
 * @archs 3510
 * @param [in] loop2_src_stride 外层循环中相邻迭代源操作数的数据块间隔，单位为字节，取值范围为[0, $2^{40}-1$]。
 * @param [in] loop2_dst_stride 外层循环中相邻迭代目的L1数据块间隔，单位为字节，取值范围为[0, $2^{21}-1$]，且必须32字节对齐。
 */
__aicore__ inline void asc_set_gm2l1_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Configures ND2Nz/DN2Nz GM-to-L1 destination layout from a caller-packed 64-bit config holding matrix_num, dst_nz_n_stride, dst_nz_c0_stride and dst_nz_matrix_stride; parameter setup only.
 *
 * @override_id asc_set_gm2l1_nz_para__u64
 * @archs 3510
 * @param [in] config ND2Nz或DN2Nz搬运的64bit配置参数。由`matrix_num`，`dst_nz_n_stride`，`dst_nz_c0_stride`与`dst_nz_matrix_stride`拼装而成，各比特位含义参加[表2](#table2)。
 */
__aicore__ inline void asc_set_gm2l1_nz_para(uint64_t config);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Configures ND2Nz/DN2Nz destination layout for GM-to-L1 copies via matrix_num, dst_nz_n_stride, dst_nz_c0_stride and dst_nz_matrix_stride; no data movement, only parameter setup.
 *
 * @override_id asc_set_gm2l1_nz_para__u16_u16_u16_u16
 * @archs 3510
 * @param [in] matrix_num 源操作数中ND矩阵或DN矩阵的数量，取值范围为[0, 4095]。取值为0时，后续搬运接口将被视为NOP（空操作）。
 * @param [in] dst_nz_n_stride 目的Nz矩阵中相邻行起始地址间的偏移。取值范围为[1, 16384]，单位为32字节。
 * @param [in] dst_nz_c0_stride 目的Nz矩阵中相邻Z分形起始地址间的偏移。取值范围为[1, 16384]，单位为32字节。
 * @param [in] dst_nz_matrix_stride 目的操作数中相邻Nz矩阵起始地址间的偏移，取值范围为[0, 65535]，单位为32字节。 &nbsp;&nbsp;&bull; 矩阵个数为1即`matrix_num`设置为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 当`matrix_num`设置大于1时，`dst_nz_matrix_stride = 0`时，后写入的Nz矩阵会覆盖第一个Nz矩阵。
 */
__aicore__ inline void asc_set_gm2l1_nz_para(
    uint16_t matrix_num, uint16_t dst_nz_n_stride, uint16_t dst_nz_c0_stride, uint16_t dst_nz_matrix_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_load_aux_config
 * @brief Sets the constant padding value used by asc_copy_gm2l1_align in Normal/Compact padding and enforced left/right padding modes; AIC only.
 *
 * @override_id asc_set_gm2l1_pad__u32
 * @archs 3510
 * @param [in] pad_val 常量填充值。接口形参为`uint32_t`，`asc_copy_gm2l1_align`根据`dtype`使用对应低位作为单个填充元素的值。
 */
__aicore__ inline void asc_set_gm2l1_pad(uint32_t pad_val);
// ==========asc_copy_l12l0b_mx==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies the right-matrix MX scale factors from L1 Buffer src to L0B_MX Buffer dst in 32-byte fp8_e8m0_t 2x16 fractals, paired with asc_copy_l12l0b; AIC only.
 *
 * @override_id asc_copy_l12l0b_mx__u64_PL1f8e8m0_u16_u16_u8_u8_u16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_mx(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride);

// ==========asc_copy_l12l0a,
// uint8_t/int8_t/fp8_e4m3fn_t/fp8_e5m2_t/hifloat8_t/half/bfloat16_t/int16_t/uint16_t/uint32_t/int32_t/float==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies bfloat16_t (b16) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp8_e4m3fn_t (b8) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp8_e5m2_t (b8) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies half (b16) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies float (b32) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies hifloat8_t (b8) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int16_t (b16) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int32_t (b32) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int8_t (b8) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint16_t (b16) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint32_t (b32) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint8_t (b8) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int4b_t (b4) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp4x2_e2m1_t (b4) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp4x2_e1m2_t (b4) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a__PCAf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies bfloat16_t (b16) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp8_e4m3fn_t (b8) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp8_e5m2_t (b8) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies half (b16) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies float (b32) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies hifloat8_t (b8) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int16_t (b16) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int32_t (b32) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int8_t (b8) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint16_t (b16) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint32_t (b32) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint8_t (b8) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int4b_t (b4) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp4x2_e2m1_t (b4) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp4x2_e1m2_t (b4) 2D fractal data from L1 Buffer src to L0A Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0a_transpose__PCAf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

// ==========asc_copy_l12l0b,
// uint8_t/int8_t/fp8_e4m3fn_t/fp8_e5m2_t/hifloat8_t/half/bfloat16_t/int16_t/uint16_t/uint32_t/int32_t/float==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies bfloat16_t (b16) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp8_e4m3fn_t (b8) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp8_e5m2_t (b8) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies half (b16) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies float (b32) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies hifloat8_t (b8) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int16_t (b16) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int32_t (b32) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int8_t (b8) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint16_t (b16) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint32_t (b32) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint8_t (b8) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int4b_t (b4) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp4x2_e2m1_t (b4) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp4x2_e1m2_t (b4) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals without transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b__PCBf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies bfloat16_t (b16) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp8_e4m3fn_t (b8) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp8_e5m2_t (b8) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies half (b16) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies float (b32) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies hifloat8_t (b8) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int16_t (b16) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int32_t (b32) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int8_t (b8) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint16_t (b16) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint32_t (b32) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies uint8_t (b8) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies int4b_t (b4) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp4x2_e2m1_t (b4) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies fp4x2_e1m2_t (b4) 2D fractal data from L1 Buffer src to L0B Buffer dst in 512-byte fractals with matrix transposition; writes result to dst and returns void; AIC only, arch 3510.
 *
 * @override_id asc_copy_l12l0b_transpose__PCBf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying bfloat16_t data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBbf16_PL1bf16_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying fp8_e4m3fn_t data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBf8e4m3_PL1f8e4m3_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying fp8_e5m2_t data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBf8e5m2_PL1f8e5m2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying half data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBf16_PL1f16_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying float data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBf32_PL1f32_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying hifloat8_t data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBhf8_PL1hf8_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying int32_t data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBi32_PL1i32_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying int8_t data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBi8_PL1i8_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying uint32_t data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBu32_PL1u32_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying uint8_t data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBu8_PL1u8_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying fp4x2_e1m2_t data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBf4e1m2x2_PL1f4e1m2x2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying fp4x2_e2m1_t data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBf4e2m1x2_PL1f4e2m1x2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Transposes 512-byte 2D fractals while copying int4b_t data from L1 Buffer src to L0B Buffer dst under index_id, repeat, stride and gap controls (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l12l0b_trans__PCBi4x2_PL1i4x2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] index_id 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围：[0, $2^{16}-1$]。
 * @param [in] repeat 迭代搬运次数。取值范围 [1, 255]。  对于`b4`数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于`b8`数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于`b16`数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于`b32`数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。
 * @param [in] src_stride 相邻repeat之间，源操作数前一个分形与后一个分形起始地址的间隔，单位512字节。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_gap 相邻repeat之间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围：[0, $2^{16}-1$]。
 * @param [in] dst_frac_gap 同一repeat内目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 * @param [in] src_frac_gap 同一repeat内源操作数前一个分形结束地址与后一个分形起始地址的间隔，单位512字节。指令实际生效步长为传入值加1，并且仅b4/b8/b32下参数生效。取值范围：[0, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

// ==========asc_copy_l12gm==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies data from L1 Buffer src to Global Memory dst in bursts (n_burst, len_burst in 32-byte units) with src_gap/dst_gap between iterations; void return, result in dst.
 *
 * @override_id asc_copy_l12gm__PGv_PL1v_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位为32字节。
 */
__aicore__ inline void asc_copy_l12gm(
    __gm__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

// ==========asc_copy_l12l0c==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies a bfloat16_t matrix from L1 Buffer to L0C Buffer in n_burst contiguous blocks of len_burst (32B units) with src_gap and dst_gap; no type conversion.
 *
 * @override_id asc_copy_l12l0c__PCCbf16_PL1bf16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c(
    __cc__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies a half matrix from L1 Buffer to L0C Buffer in n_burst contiguous blocks of len_burst (32B units) with src_gap and dst_gap; no type conversion.
 *
 * @override_id asc_copy_l12l0c__PCCf16_PL1f16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c(
    __cc__ half* dst, __cbuf__ half* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies a float matrix from L1 Buffer to L0C Buffer, converting float to half, in n_burst contiguous blocks of len_burst (32B units) with src_gap and dst_gap.
 *
 * @override_id asc_copy_l12l0c__PCCf16_PL1f32_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c(
    __cc__ half* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies a float matrix from L1 Buffer to L0C Buffer, converting float to bfloat16_t, in n_burst contiguous blocks of len_burst (32B units) with src_gap and dst_gap.
 *
 * @override_id asc_copy_l12l0c__PCCbf16_PL1f32_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c(
    __cc__ bfloat16_t* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies a float matrix from L1 Buffer to L0C Buffer in n_burst contiguous blocks of len_burst (32B units) with src_gap and dst_gap; no type conversion.
 *
 * @override_id asc_copy_l12l0c__PCCf32_PL1f32_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c(
    __cc__ float* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies a int32_t matrix from L1 Buffer to L0C Buffer in n_burst contiguous blocks of len_burst (32B units) with src_gap and dst_gap; no type conversion.
 *
 * @override_id asc_copy_l12l0c__PCCi32_PL1i32_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c(
    __cc__ int32_t* dst, __cbuf__ int32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies a uint32_t matrix from L1 Buffer to L0C Buffer in n_burst contiguous blocks of len_burst (32B units) with src_gap and dst_gap; no type conversion.
 *
 * @override_id asc_copy_l12l0c__PCCu32_PL1u32_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c(
    __cc__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a fp8_e4m3fn_t NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAf8e4m3_PL1f8e4m3_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a fp8_e5m2_t NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAf8e5m2_PL1f8e5m2_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a hifloat8_t NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAhf8_PL1hf8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a int16_t NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAi16_PL1i16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a uint16_t NC1HWC0 feature-map block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; configure fmatrix, padding and repeat registers first; AIC only.
 *
 * @override_id asc_copy_l12l0a__PCAu16_PL1u16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a fp8_e4m3fn_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBf8e4m3_PL1f8e4m3_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a fp8_e5m2_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBf8e5m2_PL1f8e5m2_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a hifloat8_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBhf8_PL1hf8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a int8_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBi8_PL1i8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a uint8_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBu8_PL1u8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a int16_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBi16_PL1i16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API 3D img2col load: moves a uint16_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with automatic transpose; configure right-matrix registers first; AIC only.
 *
 * @override_id asc_copy_l12l0b__PCBu16_PL1u16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_load_aux_config/asc_set_l13d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

// ==========asc_copy_gm2l1_align==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API GM-to-L1 copy: moves non-32B-aligned uint8_t data from GM src to L1 Buffer dst, padding to 32 bytes in Normal, Compact or left-right mode; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_align__PL1u8_PGu8_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] burst_len 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时burst_len需为2的倍数，dtype为`b32`时burst_len需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] enable_data_select 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`dst_stride`不等于`burst_len`时（即存在padding），`dst_stride`需32字节对齐，否则触发异常（即使`burst_count`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API GM-to-L1 copy: moves non-32B-aligned bfloat16_t data from GM src to L1 Buffer dst, padding to 32 bytes in Normal, Compact or left-right mode; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_align__PL1bf16_PGbf16_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] burst_len 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时burst_len需为2的倍数，dtype为`b32`时burst_len需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] enable_data_select 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`dst_stride`不等于`burst_len`时（即存在padding），`dst_stride`需32字节对齐，否则触发异常（即使`burst_count`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API GM-to-L1 copy: moves non-32B-aligned half data from GM src to L1 Buffer dst, padding to 32 bytes in Normal, Compact or left-right mode; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_align__PL1f16_PGf16_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] burst_len 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时burst_len需为2的倍数，dtype为`b32`时burst_len需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] enable_data_select 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`dst_stride`不等于`burst_len`时（即存在padding），`dst_stride`需32字节对齐，否则触发异常（即使`burst_count`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API GM-to-L1 copy: moves non-32B-aligned float data from GM src to L1 Buffer dst, padding to 32 bytes in Normal, Compact or left-right mode; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_align__PL1f32_PGf32_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] burst_len 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时burst_len需为2的倍数，dtype为`b32`时burst_len需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] enable_data_select 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`dst_stride`不等于`burst_len`时（即存在padding），`dst_stride`需32字节对齐，否则触发异常（即使`burst_count`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API GM-to-L1 copy: moves non-32B-aligned hifloat8_t data from GM src to L1 Buffer dst, padding to 32 bytes in Normal, Compact or left-right mode; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_align__PL1hf8_PGhf8_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] burst_len 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时burst_len需为2的倍数，dtype为`b32`时burst_len需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] enable_data_select 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`dst_stride`不等于`burst_len`时（即存在padding），`dst_stride`需32字节对齐，否则触发异常（即使`burst_count`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API GM-to-L1 copy: moves non-32B-aligned int16_t data from GM src to L1 Buffer dst, padding to 32 bytes in Normal, Compact or left-right mode; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_align__PL1i16_PGi16_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] burst_len 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时burst_len需为2的倍数，dtype为`b32`时burst_len需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] enable_data_select 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`dst_stride`不等于`burst_len`时（即存在padding），`dst_stride`需32字节对齐，否则触发异常（即使`burst_count`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API GM-to-L1 copy: moves non-32B-aligned int32_t data from GM src to L1 Buffer dst, padding to 32 bytes in Normal, Compact or left-right mode; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_align__PL1i32_PGi32_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] burst_len 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时burst_len需为2的倍数，dtype为`b32`时burst_len需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] enable_data_select 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`dst_stride`不等于`burst_len`时（即存在padding），`dst_stride`需32字节对齐，否则触发异常（即使`burst_count`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API GM-to-L1 copy: moves non-32B-aligned int8_t data from GM src to L1 Buffer dst, padding to 32 bytes in Normal, Compact or left-right mode; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_align__PL1i8_PGi8_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] burst_len 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时burst_len需为2的倍数，dtype为`b32`时burst_len需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] enable_data_select 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`dst_stride`不等于`burst_len`时（即存在padding），`dst_stride`需32字节对齐，否则触发异常（即使`burst_count`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API GM-to-L1 copy: moves non-32B-aligned uint16_t data from GM src to L1 Buffer dst, padding to 32 bytes in Normal, Compact or left-right mode; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_align__PL1u16_PGu16_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] burst_len 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时burst_len需为2的倍数，dtype为`b32`时burst_len需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] enable_data_select 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`dst_stride`不等于`burst_len`时（即存在padding），`dst_stride`需32字节对齐，否则触发异常（即使`burst_count`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API GM-to-L1 copy: moves non-32B-aligned uint32_t data from GM src to L1 Buffer dst, padding to 32 bytes in Normal, Compact or left-right mode; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_align__PL1u32_PGu32_u32_u32_u8_u8_b_loadl2_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] burst_len 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时burst_len需为2的倍数，dtype为`b32`时burst_len需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] enable_data_select 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_mode [asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)类型的枚举值，配置数据在L2 Cache中的管理策略。
 * @param [in] src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`dst_stride`不等于`burst_len`时（即存在padding），`dst_stride`需32字节对齐，否则触发异常（即使`burst_count`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

// ==========asc_copy_gm2l1_dn2nz==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a bfloat16_t DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1bf16_PGbf16_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a fp8_e4m3fn_t DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1f8e4m3_PGf8e4m3_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a fp8_e5m2_t DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1f8e5m2_PGf8e5m2_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a half DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1f16_PGf16_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a float DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1f32_PGf32_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a hifloat8_t DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1hf8_PGhf8_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a int16_t DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1i16_PGi16_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a int32_t DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1i32_PGi32_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a int8_t DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1i8_PGi8_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a uint16_t DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1u16_PGu16_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a uint32_t DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1u32_PGu32_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API DN2Nz load: copies a uint8_t DN matrix from GM src to L1 Buffer dst while converting DN to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only, arch 3510.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1u8_PGu8_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src_d_value 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置输入GM数据在L2 Cache中的管理策略。仅支持`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`、`asc_load_l2_cache_mode::NORMAL_PERSISTENT`和`asc_load_l2_cache_mode::NOTALLOC_KEEP`。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] src_nd_matrix_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`src_nd_matrix_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] enable_small_c0 SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);

// ==========asc_copy_gm2l1_nd2nz==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a bfloat16_t N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1bf16_PGbf16_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a fp8_e4m3fn_t N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1f8e4m3_PGf8e4m3_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a fp8_e5m2_t N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1f8e5m2_PGf8e5m2_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a half N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1f16_PGf16_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a float N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1f32_PGf32_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a hifloat8_t N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1hf8_PGhf8_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a int16_t N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1i16_PGi16_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a int32_t N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1i32_PGi32_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a int8_t N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1i8_PGi8_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a uint16_t N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1u16_PGu16_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a uint32_t N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1u32_PGu32_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief C API ND2Nz load: copies a uint8_t N-contiguous ND matrix from GM src to L1 Buffer dst while converting ND to Nz fractal layout; call asc_set_gm2l1_nz_para first; AIC only.
 *
 * @override_id asc_copy_gm2l1_nd2nz__PL1u8_PGu8_u64_loadl2_u16_u32_u64_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0);

// ==========asc_copy_gm2l1==========

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Copies data unchanged from Global Memory src to L1 Buffer dst by high-dimensional slicing, using an asc_channel_pad_mode pad_mode to insert or remove 32-byte-granularity padding.
 *
 * @override_id asc_copy_gm2l1__PL1v_PGv_u32_u32_channelpad_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] n_burst 搬运的数据块的个数。取值范围：[1, $2^{17}-1$]。
 * @param [in] len_burst 搬运的每个数据块长度，单位32字节。取值范围：[1, $2^{17}-1$]。
 * @param [in] pad_mode 使用[asc_channel_pad_mode](../../defs/enum/asc_channel_pad_mode.md)枚举值指示通道padding模式。
 * @param [in] src_stride 源操作数相邻数据块之间的间隔（即前一个数据块起始地址与后一个数据块起始地址的差值），取值范围：[0, $2^{36}-1$]，单位32字节。 &nbsp;&nbsp;&bull; n_burst = 1时，`src_stride`无意义，设置为0即可。
 * @param [in] dst_stride 目的操作数相邻数据块之间的间隔（即前一个数据块起始地址与后一个数据块起始地址的差值），取值范围：[0, $2^{17}-1$]，单位32字节。 &nbsp;&nbsp;&bull; n_burst = 1时，`dst_stride`无意义，设置为0即可。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, asc_channel_pad_mode pad_mode,
    uint64_t src_stride, uint32_t dst_stride);

//=============asc_copy_l12bt===============
// bfloat16_t -> uint64_t
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief High-dimensional copy of L1 Buffer bfloat16_t bias data to BiasTable Buffer for Mmad (n_burst blocks of len_burst with gaps); bfloat16_t converts to float.
 *
 * @override_id asc_copy_l12bt__u64_PL1bf16_u16_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为BiasTable Buffer。目的地址需64字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] conv_control 源操作数为`half`类型时，是否开启随路格式转换。 &nbsp;&nbsp;&bull; `0`：不开启转换，每个`half`元素扩展为32 bit（高16 bit为占位数据）后写入BT； &nbsp;&nbsp;&bull; `1`：开启转换，每个`half`元素转换为`float`后写入BT。仅dtype为`half`时可设置，其他dtype需设置为0。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围 [1, 4095]。
 * @param [in] len_burst 每个连续数据块的长度，单位32字节。取值范围 [1, 65535]。dtype为`float`或`int32_t`时需2对齐（即取值为偶数），否则触发异常。
 * @param [in] source_gap 源端（L1 Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。`n_burst=1`时无意义，置0即可。
 * @param [in] dst_gap 目的端（BiasTable Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。`n_burst=1`时无意义，置0即可。dtype为`float`或`int32_t`时需2对齐（即取值为偶数），否则触发异常。
 */
__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ bfloat16_t* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst,
    uint16_t source_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Contiguous copy of size bytes of L1 Buffer bfloat16_t bias data to BiasTable Buffer for Mmad, with zero source and destination gaps.
 *
 * @override_id asc_copy_l12bt__u64_PL1bf16_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为BiasTable Buffer。目的地址需64字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] size 搬运数据总大小，单位字节。 **注：size需要32字节对齐，若未对齐，搬运量会向下取整到32字节对齐。**
 */
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size);

// half -> uint64_t
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief High-dimensional copy of L1 Buffer half bias data to BiasTable Buffer for Mmad (n_burst blocks of len_burst with gaps); conv_control=1 converts half to float.
 *
 * @override_id asc_copy_l12bt__u64_PL1f16_u16_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为BiasTable Buffer。目的地址需64字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] conv_control 源操作数为`half`类型时，是否开启随路格式转换。 &nbsp;&nbsp;&bull; `0`：不开启转换，每个`half`元素扩展为32 bit（高16 bit为占位数据）后写入BT； &nbsp;&nbsp;&bull; `1`：开启转换，每个`half`元素转换为`float`后写入BT。仅dtype为`half`时可设置，其他dtype需设置为0。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围 [1, 4095]。
 * @param [in] len_burst 每个连续数据块的长度，单位32字节。取值范围 [1, 65535]。dtype为`float`或`int32_t`时需2对齐（即取值为偶数），否则触发异常。
 * @param [in] source_gap 源端（L1 Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。`n_burst=1`时无意义，置0即可。
 * @param [in] dst_gap 目的端（BiasTable Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。`n_burst=1`时无意义，置0即可。dtype为`float`或`int32_t`时需2对齐（即取值为偶数），否则触发异常。
 */
__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ half* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Contiguous copy of size bytes of L1 Buffer half bias data to BiasTable Buffer for Mmad, with zero source and destination gaps.
 *
 * @override_id asc_copy_l12bt__u64_PL1f16_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为BiasTable Buffer。目的地址需64字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] size 搬运数据总大小，单位字节。 **注：size需要32字节对齐，若未对齐，搬运量会向下取整到32字节对齐。**
 */
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ half* src, uint32_t size);

// float -> uint64_t
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief High-dimensional copy of L1 Buffer float bias data to BiasTable Buffer for Mmad (n_burst blocks of len_burst with gaps), using source/destination gap offsets.
 *
 * @override_id asc_copy_l12bt__u64_PL1f32_u16_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为BiasTable Buffer。目的地址需64字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] conv_control 源操作数为`half`类型时，是否开启随路格式转换。 &nbsp;&nbsp;&bull; `0`：不开启转换，每个`half`元素扩展为32 bit（高16 bit为占位数据）后写入BT； &nbsp;&nbsp;&bull; `1`：开启转换，每个`half`元素转换为`float`后写入BT。仅dtype为`half`时可设置，其他dtype需设置为0。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围 [1, 4095]。
 * @param [in] len_burst 每个连续数据块的长度，单位32字节。取值范围 [1, 65535]。dtype为`float`或`int32_t`时需2对齐（即取值为偶数），否则触发异常。
 * @param [in] source_gap 源端（L1 Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。`n_burst=1`时无意义，置0即可。
 * @param [in] dst_gap 目的端（BiasTable Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。`n_burst=1`时无意义，置0即可。dtype为`float`或`int32_t`时需2对齐（即取值为偶数），否则触发异常。
 */
__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ float* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Contiguous copy of size bytes of L1 Buffer float bias data to BiasTable Buffer for Mmad, with zero source and destination gaps.
 *
 * @override_id asc_copy_l12bt__u64_PL1f32_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为BiasTable Buffer。目的地址需64字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] size 搬运数据总大小，单位字节。 **注：size需要32字节对齐，若未对齐，搬运量会向下取整到32字节对齐。**
 */
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ float* src, uint32_t size);

// int32_t -> uint64_t
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief High-dimensional copy of L1 Buffer int32_t bias data to BiasTable Buffer for Mmad (n_burst blocks of len_burst with gaps), using source/destination gap offsets.
 *
 * @override_id asc_copy_l12bt__u64_PL1i32_u16_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为BiasTable Buffer。目的地址需64字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] conv_control 源操作数为`half`类型时，是否开启随路格式转换。 &nbsp;&nbsp;&bull; `0`：不开启转换，每个`half`元素扩展为32 bit（高16 bit为占位数据）后写入BT； &nbsp;&nbsp;&bull; `1`：开启转换，每个`half`元素转换为`float`后写入BT。仅dtype为`half`时可设置，其他dtype需设置为0。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围 [1, 4095]。
 * @param [in] len_burst 每个连续数据块的长度，单位32字节。取值范围 [1, 65535]。dtype为`float`或`int32_t`时需2对齐（即取值为偶数），否则触发异常。
 * @param [in] source_gap 源端（L1 Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。`n_burst=1`时无意义，置0即可。
 * @param [in] dst_gap 目的端（BiasTable Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。`n_burst=1`时无意义，置0即可。dtype为`float`或`int32_t`时需2对齐（即取值为偶数），否则触发异常。
 */
__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ int32_t* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst,
    uint16_t source_gap, uint16_t dst_gap);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Contiguous copy of size bytes of L1 Buffer int32_t bias data to BiasTable Buffer for Mmad, with zero source and destination gaps.
 *
 * @override_id asc_copy_l12bt__u64_PL1i32_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为BiasTable Buffer。目的地址需64字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] size 搬运数据总大小，单位字节。 **注：size需要32字节对齐，若未对齐，搬运量会向下取整到32字节对齐。**
 */
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ int32_t* src, uint32_t size);

// =============asc_copy_l12ub===============
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies data unchanged from L1 Buffer src_addr to Unified Buffer dst_addr (AIV0/AIV1 by sub_blockid) in 32-byte-unit bursts with src/dst gaps; AIC only.
 *
 * @override_id asc_copy_l12ub__PUv_PL1v_i8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst_addr 目的操作数，存储位置为UB。目的地址需32字节对齐。
 * @param [in] src_addr 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] sub_blockid 选择目的Vector Core。 &nbsp;&nbsp;&bull; `0`：AIV0； &nbsp;&nbsp;&bull; `1`：AIV1。
 * @param [in] burst_count 待搬运的连续数据块个数。取值范围 [1, 4095]。
 * @param [in] burst_len 每个连续数据块的长度，单位32字节。取值范围 [1, 65535]。
 * @param [in] src_gap 源操作数（L1 Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。
 * @param [in] dst_gap 目的操作数（UB Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。
 */
__aicore__ inline void asc_copy_l12ub(
    __ubuf__ void* dst_addr, __cbuf__ void* src_addr, int8_t sub_blockid, uint16_t burst_count, uint16_t burst_len,
    uint16_t src_gap, uint16_t dst_gap);

// ==========asc_copy_gm2l1===========

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads bfloat16_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1bf16_PGbf16_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads float 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1f32_PGf32_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads fp8_e4m3fn_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1f8e4m3_PGf8e4m3_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads fp8_e5m2_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1f8e5m2_PGf8e5m2_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads half 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1f16_PGf16_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads hifloat8_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1hf8_PGhf8_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads int16_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1i16_PGi16_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads int32_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1i32_PGi32_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads int8_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1i8_PGi8_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads uint16_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1u16_PGu16_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads uint32_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1u32_PGu32_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads uint8_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1u8_PGu8_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads fp4x2_e1m2_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1f4e1m2x2_PGf4e1m2x2_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads fp4x2_e2m1_t 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1f4e2m1x2_PGf4e2m1x2_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_load
 * @brief Loads raw void* 2D fractal blocks from Global Memory to L1 Buffer dst using m/k start positions and steps, 512-byte fractals, no format conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_gm2l1__PL1v_PGv_u32_u32_u16_u16_u16_loadl2
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] l2_cache_mode 使用[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)枚举值配置数据在L2 Cache中的管理策略。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode);

// ==========asc_copy_l12l0b_trans,
// bfloat16_t/fp8_e4m3fn_t/fp8_e5m2_t/half/float/hifloat8_t/int32_t/int8_t/uint32_t/uint8_t/fp4x2_e1m2_t/fp4x2_e2m1_t/int4b_t==========

// ==========asc_copy_l0c2gm==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the float L0C Buffer matrix result to Global Memory dst as bfloat16_t, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGbf16_PCCf32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the float L0C Buffer matrix result to Global Memory dst as half, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGf16_PCCf32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the float L0C Buffer matrix result to Global Memory dst as fp8_e4m3fn_t, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGf8e4m3_PCCf32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the float L0C Buffer matrix result to Global Memory dst as hifloat8_t, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGhf8_PCCf32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the float L0C Buffer matrix result to Global Memory dst as int8_t, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGi8_PCCf32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the float L0C Buffer matrix result to Global Memory dst as uint8_t, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGu8_PCCf32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the float L0C Buffer matrix result to Global Memory dst as float, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGf32_PCCf32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the int32_t L0C Buffer matrix result to Global Memory dst as bfloat16_t, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGbf16_PCCi32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the int32_t L0C Buffer matrix result to Global Memory dst as half, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGf16_PCCi32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the int32_t L0C Buffer matrix result to Global Memory dst as int8_t, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGi8_PCCi32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the int32_t L0C Buffer matrix result to Global Memory dst as uint8_t, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGu8_PCCi32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the int32_t L0C Buffer matrix result to Global Memory dst as int32_t, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGi32_PCCi32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the float L0C Buffer matrix result to Global Memory dst as int4b_t, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGi4x2_PCCf32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Stores the int32_t L0C Buffer matrix result to Global Memory dst as int4b_t, optionally applying accompanying quantization, ReLU and Nz2ND/Nz2DN conversion (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2gm__PGi4x2_PCCi32_u16_u16_u32_u16_storel2_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);

// ==========asc_copy_l0c2l1==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the float L0C Buffer result into L1 Buffer dst as half, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1f16_PCCf32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the float L0C Buffer result into L1 Buffer dst as bfloat16_t, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1bf16_PCCf32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the float L0C Buffer result into L1 Buffer dst as int8_t, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1i8_PCCf32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the float L0C Buffer result into L1 Buffer dst as uint8_t, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1u8_PCCf32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the float L0C Buffer result into L1 Buffer dst as float, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1f32_PCCf32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the int32_t L0C Buffer result into L1 Buffer dst as half, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1f16_PCCi32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the int32_t L0C Buffer result into L1 Buffer dst as int8_t, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1i8_PCCi32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the int32_t L0C Buffer result into L1 Buffer dst as uint8_t, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1u8_PCCi32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the int32_t L0C Buffer result into L1 Buffer dst as int32_t, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1i32_PCCi32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the float L0C Buffer result into L1 Buffer dst as int4b_t, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1i4x2_PCCf32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the int32_t L0C Buffer result into L1 Buffer dst as int4b_t, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1i4x2_PCCi32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the float L0C Buffer result into L1 Buffer dst as hifloat8_t, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1hf8_PCCf32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the float L0C Buffer result into L1 Buffer dst as fp8_e4m3fn_t, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1f8e4m3_PCCf32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);

/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Moves the int32_t L0C Buffer result into L1 Buffer dst as bfloat16_t, with optional accompanying quantization, ReLU, Nz2ND/Nz2DN conversion or channel split (void; result in dst; AIC only).
 *
 * @override_id asc_copy_l0c2l1__PL1bf16_PCCi32_u16_u16_u32_u16_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 对于Nz输出场景： &nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。 &nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。 对于ND输出场景： &nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 &nbsp;&nbsp;&bull;对于DN输出场景：$m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启unit_flag； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 与计算指令的配合方式见[UnitFlag](../../cube_compute/mmad_compute_key_features/UnitFlag.md)。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。 系数含义及配置步骤见[随路量化](../cube_store_key_features/accompanying_quantization.md)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU； &nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。 模式与配置接口见[随路ReLU](../cube_store_key_features/accompanying_relu.md)。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 特性及支持通路见[F32 Channel Split](../cube_store_key_features/F32-Channel-Split.md)。
 * @param [in] enable_nz2nd 是否开启Nz2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局及步长示意见[NZ2ND](../cube_store_key_features/NZ2ND.md)；关闭格式转换时参见[NZ2NZ](../cube_store_key_features/NZ2NZ.md)。
 * @param [in] enable_nz2dn 是否开启Nz2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。 布局与配置方法见[NZ2DN](../cube_store_key_features/NZ2DN.md)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; `false`：不开启Clip ReLU； &nbsp;&nbsp;&bull; `true`：开启Clip ReLU（scalar 模式）。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);

// ==========asc_copy_l0c2ub==========
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the float matrix result from L0C Buffer to Unified Buffer dst as bfloat16_t with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUbf16_PCCf32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the float matrix result from L0C Buffer to Unified Buffer dst as half with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUf16_PCCf32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the float matrix result from L0C Buffer to Unified Buffer dst as fp8_e4m3fn_t with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUf8e4m3_PCCf32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the float matrix result from L0C Buffer to Unified Buffer dst as hifloat8_t with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUhf8_PCCf32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the float matrix result from L0C Buffer to Unified Buffer dst as int8_t with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUi8_PCCf32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the float matrix result from L0C Buffer to Unified Buffer dst as uint8_t with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUu8_PCCf32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the float matrix result from L0C Buffer to Unified Buffer dst as float with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUf32_PCCf32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the int32_t matrix result from L0C Buffer to Unified Buffer dst as bfloat16_t with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUbf16_PCCi32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the int32_t matrix result from L0C Buffer to Unified Buffer dst as half with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUf16_PCCi32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the int32_t matrix result from L0C Buffer to Unified Buffer dst as int8_t with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUi8_PCCi32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the int32_t matrix result from L0C Buffer to Unified Buffer dst as uint8_t with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUu8_PCCi32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the int32_t matrix result from L0C Buffer to Unified Buffer dst as int32_t with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUi32_PCCi32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the float matrix result from L0C Buffer to Unified Buffer dst as int4b_t with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUi4x2_PCCf32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
/**
 * @ingroup simd_c_api.cube_datamove.cube_compute_store
 * @brief Copies the int32_t matrix result from L0C Buffer to Unified Buffer dst as int4b_t with optional quantization/ReLU/layout conversion and sub_blockid/dual_dst_ctrl selection (void; result in dst).
 *
 * @override_id asc_copy_l0c2ub__PUi4x2_PCCi32_u16_u16_u32_u16_i8_dualdst_unitflag_quantmode_relupre_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);

// ==========deprecated interfaces==========
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_channel_para)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated uint64_t-config setter of the source Nz adjacent-row address offset (bits 63:48, 64-byte units) for Nz2DN during L0C copy-out; use asc_set_l0c_copy_channel_para.
 *
 * @override_id asc_set_l0c2gm_channel_para__u64
 * @archs 3510
 * @param [in] config Nz2DN格式转换的源操作数相关配置。接口不检查或转换字段值，需按照[表2](#table2)完成拼装。
 */
__aicore__ inline void asc_set_l0c2gm_channel_para(uint64_t config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_nz_para)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated setter of matrix count and source/destination offsets for Nz-to-ND conversion during L0C Buffer copy-out (void return); replaced by asc_set_l0c_copy_nz_para.
 *
 * @override_id asc_set_l0c2gm_nz2nd__u64_u64_u64
 * @archs 3510
 * @param [in] nd_num ND矩阵的数量，取值范围为[1, 65535]。
 * @param [in] src_nd_stride 源相邻Nz矩阵之间的偏移（相邻Nz矩阵头与头的间隔）。当nd_num配置为1时，src_nd_stride不生效，配置为0即可。取值范围为[0, 65535]，单位为64字节（`16 × sizeof(dtype)`，`dtype`为L0C Buffer上数据的类型，支持`int32_t`和`float`）。
 * @param [in] dst_nd_stride 目的相邻ND矩阵之间的偏移（相邻ND矩阵头与头的间隔）。当nd_num配置为1时，dst_nd_stride不生效，配置为0即可。取值范围为[0, 2^32-1]，单位为元素。
 */
__aicore__ inline void asc_set_l0c2gm_nz2nd(uint64_t nd_num, uint64_t src_nd_stride, uint64_t dst_nd_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_config)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated setter of the vector ReLU start address, quantization start address and unit_flag enable for data-movement quantization; replaced by asc_set_l0c_copy_config.
 *
 * @override_id asc_set_l0c2gm_config__u64_u64_b
 * @archs 2201 3510
 * @param [in] relu_pre ReLU操作前矢量的起始地址。
 * @param [in] quant_pre 量化操作前矢量的起始地址。
 * @param [in] enable_unit_flag 是否启用unit_flag。unit_flag是一种矩阵计算指令和矩阵搬运指令细粒度的并行，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出，该功能不适用于L0C Buffer累加的场景。
 */
__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre, uint64_t quant_pre, bool enable_unit_flag);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l12l0a_3d_padding)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated uint64_t-typed setter storing the Pad fill value (config) used when calling asc_copy_l12l0a; writes pad attribute (void return); superseded by asc_set_l12l0a_3d_padding.
 *
 * @override_id asc_set_l13d_padding__u64
 * @archs 2201 3510
 * @param [in] config Pad填充值的数值。
 */
__aicore__ inline void asc_set_l13d_padding(uint64_t config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l12l0a_3d_padding)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated half-typed setter storing the Pad fill value (config) used when calling asc_copy_l12l0a; writes pad attribute (void return); superseded by asc_set_l12l0a_3d_padding.
 *
 * @override_id asc_set_l13d_padding__f16
 * @archs 2201 3510
 * @param [in] config Pad填充值的数值。
 */
__aicore__ inline void asc_set_l13d_padding(half config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l12l0a_3d_padding)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated int16_t-typed setter storing the Pad fill value (config) used when calling asc_copy_l12l0a; writes pad attribute (void return); superseded by asc_set_l12l0a_3d_padding.
 *
 * @override_id asc_set_l13d_padding__i16
 * @archs 2201 3510
 * @param [in] config Pad填充值的数值。
 */
__aicore__ inline void asc_set_l13d_padding(int16_t config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l12l0a_3d_padding)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated uint16_t-typed setter storing the Pad fill value (config) used when calling asc_copy_l12l0a; writes pad attribute (void return); superseded by asc_set_l12l0a_3d_padding.
 *
 * @override_id asc_set_l13d_padding__u16
 * @archs 2201 3510
 * @param [in] config Pad填充值的数值。
 */
__aicore__ inline void asc_set_l13d_padding(uint16_t config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l12l0b_3d_padding)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated uint64_t setter of the PADDING_B register storing padding values (bits 31:0) and padding mode (bits 33:32); replaced by asc_set_l12l0b_3d_padding.
 *
 * @override_id asc_set_l12l0_padding_val__u64
 * @archs 3510
 * @param [in] config 待设置的寄存器值。
 */
__aicore__ inline void asc_set_l12l0_padding_val(uint64_t config);

[[deprecated("Deprecated since 9.2.0, Will be removed after 2027/09/07")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated uint64_t setter of the QUANT_POST 64-bit register storing Fixpipe post-processing quantization info (offsets, signedness, M3, shift); no replacement.
 *
 * @override_id asc_set_l0c2gm_quant_post__u64
 * @archs 3510
 * @param [in] config 待设置的寄存器值。
 */
__aicore__ inline void asc_set_l0c2gm_quant_post(uint64_t config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_relu_alpha)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated uint64_t setter of the RELU_ALPHA 64-bit register holding Scalar ReLU alpha values for fixpipe/cube (bits 31:13 pre, 63:45 post); replaced by asc_set_l0c_copy_relu_alpha.
 *
 * @override_id asc_set_l0c2gm_relu_alpha__u64
 * @archs 3510
 * @param [in] config 待设置的寄存器值。
 */
__aicore__ inline void asc_set_l0c2gm_relu_alpha(uint64_t config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_lrelu_alpha)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated setter storing the half-typed Leaky ReLU alpha used by asc_copy_l0c2l1/asc_copy_l0c2gm computations; pass-by-reference config; superseded by asc_set_l0c_copy_lrelu_alpha.
 *
 * @override_id asc_set_l0c2gm_lrelu_alpha__Rf16
 * @archs 2201 3510
 * @param [in] config Leaky ReLU alpha值。
 */
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(half& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_lrelu_alpha)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated setter storing the float-typed Leaky ReLU alpha used by asc_copy_l0c2l1/asc_copy_l0c2gm computations; pass-by-reference config; superseded by asc_set_l0c_copy_lrelu_alpha.
 *
 * @override_id asc_set_l0c2gm_lrelu_alpha__Rf32
 * @archs 2201 3510
 * @param [in] config Leaky ReLU alpha值。
 */
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(float& config);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies bfloat16_t data from GM src to L1 Buffer dst, padding non-32B bursts to 32B in Normal/Compact or left/right mode; result written to dst.
 *
 * @override_id asc_copy_gm2l1_align__PL1bf16_PGbf16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
        uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
        uint32_t dst_stride))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies half data from GM src to L1 Buffer dst, padding non-32B bursts to 32B in Normal/Compact or left/right mode; result written to dst.
 *
 * @override_id asc_copy_gm2l1_align__PL1f16_PGf16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ half* dst, __gm__ half* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
        uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
        uint32_t dst_stride))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies float data from GM src to L1 Buffer dst, padding non-32B bursts to 32B in Normal/Compact or left/right mode; result written to dst.
 *
 * @override_id asc_copy_gm2l1_align__PL1f32_PGf32_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ float* dst, __gm__ float* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies hifloat8_t data from GM src to L1 Buffer dst, padding non-32B bursts to 32B in Normal/Compact or left/right mode; result written to dst.
 *
 * @override_id asc_copy_gm2l1_align__PL1hf8_PGhf8_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies int16_t data from GM src to L1 Buffer dst, padding non-32B bursts to 32B in Normal/Compact or left/right mode; result written to dst.
 *
 * @override_id asc_copy_gm2l1_align__PL1i16_PGi16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies int32_t data from GM src to L1 Buffer dst, padding non-32B bursts to 32B in Normal/Compact or left/right mode; result written to dst.
 *
 * @override_id asc_copy_gm2l1_align__PL1i32_PGi32_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
        uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
        uint32_t dst_stride))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies int8_t data from GM src to L1 Buffer dst, padding non-32B bursts to 32B in Normal/Compact or left/right mode; result written to dst.
 *
 * @override_id asc_copy_gm2l1_align__PL1i8_PGi8_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies uint16_t data from GM src to L1 Buffer dst, padding non-32B bursts to 32B in Normal/Compact or left/right mode; result written to dst.
 *
 * @override_id asc_copy_gm2l1_align__PL1u16_PGu16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies uint32_t data from GM src to L1 Buffer dst, padding non-32B bursts to 32B in Normal/Compact or left/right mode; result written to dst.
 *
 * @override_id asc_copy_gm2l1_align__PL1u32_PGu32_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies uint8_t data from GM src to L1 Buffer dst, padding non-32B bursts to 32B in Normal/Compact or left/right mode; result written to dst.
 *
 * @override_id asc_copy_gm2l1_align__PL1u8_PGu8_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies bfloat16_t data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1bf16_PGbf16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value,
        asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride,
        bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies fp8_e4m3fn_t data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1f8e4m3_PGf8e4m3_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies fp8_e5m2_t data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1f8e5m2_PGf8e5m2_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies half data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1f16_PGf16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies float data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1f32_PGf32_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies hifloat8_t data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1hf8_PGhf8_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies int16_t data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1i16_PGi16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies int32_t data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1i32_PGi32_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies int8_t data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1i8_PGi8_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies uint16_t data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1u16_PGu16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies uint32_t data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1u32_PGu32_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies uint8_t data from GM src to L1 Buffer dst converting DN layout to Nz, with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz__PL1u8_PGu8_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value,
        asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride,
        bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, asc_channel_pad_mode pad_mode,
        uint64_t src_stride, uint32_t dst_stride))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copies data from GM src to L1 Buffer dst by high-dimensional slicing using uint8_t pad_func_mode to insert/remove 32-byte-granularity padding; use asc_channel_pad_mode overload.
 *
 * @override_id asc_copy_gm2l1__PL1v_PGv_u32_u32_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] n_burst 搬运的数据块的个数。取值范围：[1, $2^{17}-1$]。
 * @param [in] len_burst 搬运的每个数据块长度，单位32字节。取值范围：[1, $2^{17}-1$]。
 * @param [in] pad_func_mode 搬运过程中的填充或删除数据模式，取值范围：[0, 8]。填充模式是指在目的的数据中插入padding值，padding值需要通过[asc_set_gm2l1_padding](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_padding.md)设置。删除模式是指在从搬运的每个32字节中移除部分数据。 &nbsp;&nbsp;&bull; `0`：不做padding； &nbsp;&nbsp;&bull; `1`：每有1个字节的数据，插入31个padding值，其中插入的每个padding的位宽为b8； &nbsp;&nbsp;&bull; `2`：每有2个字节的数据，插入15个padding值，其中插入的每个padding的位宽为b16； &nbsp;&nbsp;&bull; `3`：每有4个字节的数据，插入14个padding值，其中插入的每个padding的位宽为b16； &nbsp;&nbsp;&bull; `4`：每有8个字节的数据，插入12个padding值，其中插入的每个padding的位宽为b16；  &nbsp;&nbsp;&bull; `5`：每有16个字节的数据，插入8个padding值，其中插入的每个padding的位宽为b16； &nbsp;&nbsp;&bull; `6`：每有32个字节的数据，移除28个最高有效位数据，只保留4个最低有效位字节； &nbsp;&nbsp;&bull; `7`：每有32个字节的数据，移除24个最高有效位数据，只保留8个最低有效位字节； &nbsp;&nbsp;&bull; `8`：每有32个字节的数据，移除16个最高有效位数据，只保留16个最低有效位字节。
 * @param [in] src_stride 源操作数相邻数据块之间的间隔（即前一个数据块起始地址与后一个数据块起始地址的差值），取值范围：[0, $2^{36}-1$]，单位32字节。 &nbsp;&nbsp;&bull; n_burst = 1时，`src_stride`无意义，设置为0即可。
 * @param [in] dst_stride 目的操作数相邻数据块之间的间隔（即前一个数据块起始地址与后一个数据块起始地址的差值），取值范围：[0, $2^{17}-1$]，单位32字节。 &nbsp;&nbsp;&bull; n_burst = 1时，`dst_stride`无意义，设置为0即可。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, uint8_t pad_func_mode,
    uint64_t src_stride, uint32_t dst_stride);

// ==========asc_copy_l0c2l1==========
// half  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copying the float L0C Buffer result to L1 Buffer dst as half with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1__PL1f16_PCCf32_u16_u16_u32_u16_u8_u8_u64_u8_b_b_u64_u8_b_u8_u8_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en);

// bfloat16_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copying the float L0C Buffer result to L1 Buffer dst as bfloat16_t with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1__PL1bf16_PCCf32_u16_u16_u32_u16_u8_u8_u64_u8_b_b_u64_u8_b_u8_u8_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en);

// int8_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copying the float L0C Buffer result to L1 Buffer dst as int8_t with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1__PL1i8_PCCf32_u16_u16_u32_u16_u8_u8_u64_u8_b_b_u64_u8_b_u8_u8_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en);

// uint8_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copying the float L0C Buffer result to L1 Buffer dst as uint8_t with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1__PL1u8_PCCf32_u16_u16_u32_u16_u8_u8_u64_u8_b_b_u64_u8_b_u8_u8_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en);

// float  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copying the float L0C Buffer result to L1 Buffer dst as float with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1__PL1f32_PCCf32_u16_u16_u32_u16_u8_u8_u64_u8_b_b_u64_u8_b_u8_u8_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en);

// half int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copying the int32_t L0C Buffer result to L1 Buffer dst as half with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1__PL1f16_PCCi32_u16_u16_u32_u16_u8_u8_u64_u8_b_b_u64_u8_b_u8_u8_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en);

// int8_t int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copying the int32_t L0C Buffer result to L1 Buffer dst as int8_t with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1__PL1i8_PCCi32_u16_u16_u32_u16_u8_u8_u64_u8_b_b_u64_u8_b_u8_u8_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en);

// uint8_t int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copying the int32_t L0C Buffer result to L1 Buffer dst as uint8_t with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1__PL1u8_PCCi32_u16_u16_u32_u16_u8_u8_u64_u8_b_b_u64_u8_b_u8_u8_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en);

// int32_t int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copying the int32_t L0C Buffer result to L1 Buffer dst as int32_t with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1__PL1i32_PCCi32_u16_u16_u32_u16_u8_u8_u64_u8_b_b_u64_u8_b_u8_u8_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en);

// int4b_t float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copying the float L0C Buffer result to L1 Buffer dst as int4b_t with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1__PL1i4x2_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] l2_cache_mode 无效参数，用户无需关注，传入0即可。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 * @param [in] broadcast_en 无效参数，用户无需关注，传入false即可。
 * @param [in] enable_nz2dn 是否开启NZ2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// int4b_t int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated overload copying the int32_t L0C Buffer result to L1 Buffer dst as int4b_t with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1__PL1i4x2_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] l2_cache_mode 无效参数，用户无需关注，传入0即可。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 * @param [in] broadcast_en 无效参数，用户无需关注，传入false即可。
 * @param [in] enable_nz2dn 是否开启NZ2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// half float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// bfloat16_t float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2l1(
        __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// ==========asc_copy_l0c2gm==========
// bfloat16_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to GM dst as bfloat16_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGbf16_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// half  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
        asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
        asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
        bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to GM dst as half, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGf16_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// fp8_e4m3fn_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to GM dst as fp8_e4m3fn_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGf8e4m3_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// hifloat8_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to GM dst as hifloat8_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGhf8_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// int8_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to GM dst as int8_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGi8_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// uint8_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to GM dst as uint8_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGu8_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// float  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to GM dst as float, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGf32_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// bfloat16_t  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as bfloat16_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGbf16_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// half  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as half, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGf16_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// int8_t  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as int8_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGi8_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// uint8_t  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as uint8_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGu8_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// int32_t  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as int32_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGi32_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// int4b_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to GM dst as int4b_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGi4x2_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// int4b_t  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2gm(
        __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as int4b_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm__PGi4x2_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// ==========asc_copy_l0c2ub==========
// bfloat16_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to UB dst as bfloat16_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUbf16_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// half  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to UB dst as half, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUf16_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// fp8_e4m3fn_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to UB dst as fp8_e4m3fn_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUf8e4m3_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// hifloat8_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to UB dst as hifloat8_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUhf8_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// int8  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to UB dst as int8_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUi8_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// uint8  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to UB dst as uint8_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUu8_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// float  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to UB dst as float, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUf32_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// bfloat16_t  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as bfloat16_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUbf16_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// half  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as half, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUf16_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// int8  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as int8_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUi8_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// uint8  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as uint8_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUu8_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// int32_t  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as int32_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUi32_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// int4b_t  float
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer float data to UB dst as int4b_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUi4x2_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

// int4b_t  int32_t
ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_l0c2ub(
        __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
        asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
        bool enable_nz2dn, bool enable_clip_relu_pre))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as int4b_t, optionally applying quantization, ReLU and Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub__PUi4x2_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of bfloat16_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1bf16_PGbf16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of float 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1f32_PGf32_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of fp8_e4m3fn_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1f8e4m3_PGf8e4m3_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of fp8_e5m2_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1f8e5m2_PGf8e5m2_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
        uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of half 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1f16_PGf16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of hifloat8_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1hf8_PGhf8_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of int16_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1i16_PGi16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of int32_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1i32_PGi32_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of int8_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1i8_PGi8_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of uint16_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1u16_PGu16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of uint32_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1u32_PGu32_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of uint8_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1u8_PGu8_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of fp4x2_e1m2_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1f4e1m2x2_PGf4e1m2x2_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of fp4x2_e2m1_t 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1f4e2m1x2_PGf4e2m1x2_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
        uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning copy of void* 2D GM-to-L1 fractals (512-byte units, no format conversion) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode; AIC only.
 *
 * @override_id asc_copy_gm2l1__PL1v_PGv_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

#if (ASC_DEVKIT_VERSION_NUM >= 90200000)
// ==========asc_copy_l0c2l1 (9201 unique: quant_pre_rnd)==========
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);

// ==========asc_prefetch_* (9201 unique)==========
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ int8_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ uint8_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ hifloat8_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ fp8_e5m2_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ fp8_e4m3fn_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ int16_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ uint16_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ half* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ bfloat16_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ int32_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ uint32_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ float* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ int8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ uint8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ int16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ uint16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ half* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ int32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ uint32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ float* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ int8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ uint8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ int16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ uint16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ half* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ int32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ uint32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ float* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride);
__aicore__ inline void asc_prefetch_stop();

// ==========asc_copy_l0c2gm==========
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre);

// ==========asc_copy_l0c2ub==========
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre);

__aicore__ inline void asc_copy_l12l0a(
    __ca__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(
    __cb__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0, bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0, bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t burst_count, uint32_t burst_len, asc_channel_pad_mode pad_mode,
    bool enable_pre_load_ignore_sync, uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode);

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode);

#endif

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CUBE_DATAMOVE_CUBE_DATAMOVE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CUBE_DATAMOVE_CUBE_DATAMOVE_H

#endif
