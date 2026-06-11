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
 * \file asc_create_iter_reg_impl.h
 * \brief asc_create_iter_reg_{b8,b16,b32}_impl：同名重载 1~4 参，原样转发 vag_{b8,b16,b32}
 */

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_CREATE_ITER_REG_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_CREATE_ITER_REG_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl.h"

// f(bxx, 形参列表)：为每个 bxx 生成 4 个重载（1~4 个 offset）
#define ASC_ITER_REG_ARITIES(f, bxx)                                                                 \
    f(bxx, uint32_t offset)                                                                          \
    f(bxx, uint32_t offset0, uint32_t offset1)                                                       \
    f(bxx, uint32_t offset0, uint32_t offset1, uint32_t offset2)                                     \
    f(bxx, uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3)

#define ASC_ITER_REG_IMPL_FN(bxx, ...)                                                               \
    __simd_callee__ inline iter_reg asc_create_iter_reg_##bxx##_impl(__VA_ARGS__)                    \
    {                                                                                                \
        if ASC_IS_AIV {                                                                              \
            return vag_##bxx(__VA_ARGS__);                                                           \
        }                                                                                            \
        return iter_reg{};                                                                           \
    }

ASC_ITER_REG_ARITIES(ASC_ITER_REG_IMPL_FN, b32)
ASC_ITER_REG_ARITIES(ASC_ITER_REG_IMPL_FN, b16)
ASC_ITER_REG_ARITIES(ASC_ITER_REG_IMPL_FN, b8)

#undef ASC_ITER_REG_IMPL_FN

#endif
