/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/


#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_SYS_VAR_SYS_VAR_H
#define INCLUDE_C_API_SYS_VAR_SYS_VAR_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

#include "impl/c_api/instr_impl/npu_arch_2201/sys_var_impl.h"

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

#include "impl/c_api/instr_impl/npu_arch_3510/sys_var_impl.h"

#endif

__aicore__ inline int64_t asc_get_smmu_tag_version();

__aicore__ inline int64_t asc_get_phy_stack_base();

__aicore__ inline int64_t asc_get_status();

__aicore__ inline int64_t asc_get_sys_virtual_base();

__aicore__ inline int64_t asc_get_ctrl();

__aicore__ inline int64_t asc_get_block_num();

__aicore__ inline int64_t asc_get_system_cycle();

__aicore__ inline void asc_set_ctrl(uint64_t config);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

__aicore__ inline int64_t asc_get_ffts_base_addr();

__aicore__ inline int64_t asc_get_core_id();

__aicore__ inline int64_t asc_get_block_idx();

__aicore__ inline uint64_t asc_get_overflow_status();

__aicore__ inline uint64_t asc_get_phy_buf_addr(uint64_t offset);

__aicore__ inline int64_t asc_get_sub_block_num();

__aicore__ inline int64_t asc_get_sub_block_id();

__aicore__ inline int64_t asc_get_program_counter();

__aicore__ inline void asc_get_arch_ver(uint32_t& core_version);

[[deprecated("NOTICE: asc_get_ar_spr is deprecated. "
             "Please use asc_get_squeeze_status instead.")]]
__aicore__ inline int64_t asc_get_ar_spr();

__aicore__ inline void asc_set_ffts_base_addr(uint64_t config);

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

__aicore__ inline constexpr int64_t asc_get_vf_len();

__aicore__ inline uint64_t asc_get_phy_buf_addr(uint64_t offset);

__aicore__ inline void asc_get_arch_ver(uint32_t& core_version);

__aicore__ inline int64_t asc_get_core_id();

__aicore__ inline void asc_set_l0c2gm_channel_para(uint64_t config);

__aicore__ inline void asc_set_l3d_rpt_b(uint64_t config);

__aicore__ inline void asc_set_l12l0_padding_val(uint64_t config);

__aicore__ inline void asc_set_l0c2gm_quant_post(uint64_t config);

__aicore__ inline void asc_set_l0c2gm_relu_alpha(uint64_t config);

__aicore__ inline int64_t asc_get_squeeze_status();

/*
 * @ingroup CubeDataMove
 * @brief Set MTE2_NZ_PARA register value before calling asc_copy_gm2l1_dn2nz and asc_copy_gm2l1_nd2nz
 * @param [in] config Register value to set. Bit 15:0 is ND/DN matrix count, bit 31:16 is N dimension block interval, bit 47:32 is C0 column interval in same NZ matrix, bit 63:48 is NZ matrix interval between ND/DN matrices
 * @details Sets parameters for format conversion in MTE2_NZ_PARA register before ND/DN to NZ format conversion.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_set_gm2l1_nz_para(uint64_t config);
#endif


#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif
