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
 * \file kernel_reg_compute_gather_mask_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_reg_compute_gather_mask_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_GATHER_MASK_INTF_H__
#endif

#ifndef ASCENDC_MODULE_REG_COMPUTE_GATHER_MASK_INTERFACE_H
#define ASCENDC_MODULE_REG_COMPUTE_GATHER_MASK_INTERFACE_H

#include "kernel_reg_compute_common_intf.h"
#include "kernel_reg_compute_utils.h"

namespace AscendC {
namespace Reg {
template <typename T = DefaultType, GatherMaskMode store = GatherMaskMode::NO_STORE_REG, typename U>
__simd_callee__ inline void Squeeze(U& dstReg, U& srcReg, MaskReg& mask);

template <typename T = DefaultType, typename U>
__simd_callee__ inline void Unsqueeze(U& dstReg, MaskReg& mask);

template <SpecialPurposeReg spr>
/**
 * @ingroup simd_api.basic_api.reg_vector_compute.system_variable_access
 * @override_id ClearSpr__z
 * @archs 3510
 * @brief 该接口用于对指定的特殊目的寄存器（SpecialPurposeReg）进行清零操作。通过模板参数spr指定目标特殊寄存器，调用后该寄存器的值被置为0，当前支持的特殊寄存器见[表 SpecialPurposeReg模板参数说明](#table2)。
 */
__simd_callee__ inline void ClearSpr();

template <typename T = DefaultType, typename U = DefaultType, typename S, typename V>
/**
 * @ingroup simd_api.basic_api.reg_vector_compute.scatter_operations
 * @override_id Gather__RpS_RpS_RpV
 * @archs 3510
 * @brief 该指令会根据索引值indexReg将源操作数srcReg按元素收集到目的操作数dstReg中。
 * @param [out] dstReg 目的操作数。类型为[RegTensor](../register_data_types/RegTensor.md)。
 * @param [in] srcReg 源操作数。类型为[RegTensor](../register_data_types/RegTensor.md)。
 * @param [in] indexReg 索引值，单位：元素。类型为[RegTensor](../register_data_types/RegTensor.md)。数据类型的位宽需要与目的操作数的位宽保持一致。indexReg中的值可以重复。如果indexReg中索引值超出当前RegTensor中能存储的最大数据元素个数时，按照如下方式处理：RegTensor所能存储的最大数据元素个数为[VL](../register_data_types/RegTensor.md)/sizeof(T)，indexReg中索引值为i，索引值更新为i % (VL/sizeof(T))。例：元素为half数据类型的srcReg，有128个元素，当indexReg中索引值为168时，索引值更新为168 % 128 = 40。
 */
__simd_callee__ inline void Gather(S& dstReg, S& srcReg, V& indexReg);
} // namespace Reg
} // namespace AscendC

#include "../../../impl/basic_api/reg_compute/kernel_reg_compute_gather_mask_intf_impl.h"
#endif // ASCENDC_MODULE_REG_COMPUTE_COPY_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_GATHER_MASK_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_GATHER_MASK_INTF_H__
#endif
