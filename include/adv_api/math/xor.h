/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file xor.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "xor.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_XOR_H__
#endif

#ifndef LIB_MATH_XOR_H
#define LIB_MATH_XOR_H

#include "kernel_tensor.h"
#include "kernel_basic_intf.h"
#include "../../../impl/basic_api/kernel_pop_stack_buffer.h"
#include "../../../impl/adv_api/detail/math/xor/xor_common_impl.h"
#if ASCENDC_CPU_DEBUG
#include "../../../impl/basic_api/kernel_log.h"
#include <type_traits>
#endif

namespace AscendC {
#pragma begin_pipe(V)
/*
 * @brief Xor Computes the element-wise logical XOR of the given input tensors. Zeros are treated as False and nonzeros
 * are treated as True. Mathematical formulas: 0^0=0�?^1=1�?^0=1�?^1=0
 * @ingroup xor
 * @param [out] dstTensor, output LocalTensor
 * @param [in] srcTensor0, input LocalTensor
 * @param [in] srcTensor1, input LocalTensor
 * @param [in] sharedTmpBuffer, input local temporary Tensor
 * @param [in] calCount, amount of input data to be calculated
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.math_compute.xor_interface
 * @override_id Xor__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8_u32
 * @archs 3510 2201 3002 2002
 * @brief 按元素执行Xor运算。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] src0Tensor 源操作数0。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] src1Tensor 源操作数1。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于Xor内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetXorMaxMinTmpSize](GetXorMaxMinTmpSize.md)。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void Xor(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor,
    const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }
    static_assert(
        (std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value),
        "Failed to check the data types, current api support data types are int16_t/uint16_t.");

#if (                                                                                                \
    defined(__NPU_ARCH__) &&                                                                         \
    (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3002 || __NPU_ARCH__ == 3510 || \
     __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113))
    XorImpl<T, isReuseSource>(dstTensor, src0Tensor, src1Tensor, sharedTmpBuffer, calCount);
#endif
}
/*
 * @brief Xor Computes the element-wise logical XOR of the given input tensors. Zeros are treated as False and nonzeros
 * are treated as True. Mathematical formulas: 0^0=0�?^1=1�?^0=1�?^1=0
 * @ingroup xor
 * @param [out] dstTensor, output LocalTensor
 * @param [in] srcTensor0, input LocalTensor
 * @param [in] srcTensor1, input LocalTensor
 * @param [in] sharedTmpBuffer, input local temporary Tensor
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.math_compute.xor_interface
 * @override_id Xor__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8
 * @archs 3510 2201 3002 2002
 * @brief 按元素执行Xor运算。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] src0Tensor 源操作数0。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] src1Tensor 源操作数1。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于Xor内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetXorMaxMinTmpSize](GetXorMaxMinTmpSize.md)。
 */
__aicore__ inline void Xor(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor,
    const LocalTensor<uint8_t>& sharedTmpBuffer)
{
#if defined(ASCENDC_CPU_DEBUG) && (ASCENDC_CPU_DEBUG == 1)
    bool result = (src0Tensor.GetSize() == src1Tensor.GetSize());
    ASCENDC_ASSERT(result, {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to check src0Tensor size %u and src1Tensor size %u, "
            "should be equal When calCount parameter is not included.",
            src0Tensor.GetSize(), src1Tensor.GetSize());
    });
#endif
    Xor<T, isReuseSource>(dstTensor, src0Tensor, src1Tensor, sharedTmpBuffer, src0Tensor.GetSize());
}

/*
 * @brief Xor Computes the element-wise logical XOR of the given input tensors. Zeros are treated as False and nonzeros
 * are treated as True. Mathematical formulas: 0^0=0�?^1=1�?^0=1�?^1=0
 * @ingroup xor
 * @param [out] dstTensor, output LocalTensor
 * @param [in] srcTensor0, input LocalTensor
 * @param [in] srcTensor1, input LocalTensor
 * @param [in] calCount, amount of input data to be calculated
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.math_compute.xor_interface
 * @override_id Xor__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_u32
 * @archs 3510 2201 3002 2002
 * @brief 按元素执行Xor运算。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] src0Tensor 源操作数0。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] src1Tensor 源操作数1。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void Xor(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor,
    const uint32_t calCount)
{
    LocalTensor<uint8_t> sharedTmpBuffer;
    bool ans = PopStackBuffer<uint8_t, TPosition::LCM>(sharedTmpBuffer);
    ASCENDC_ASSERT((ans), { KERNEL_LOG(KERNEL_ERROR, "PopStackBuffer Error!"); });

    Xor<T, isReuseSource>(dstTensor, src0Tensor, src1Tensor, sharedTmpBuffer, calCount);
}

/*
 * @brief Xor Computes the element-wise logical XOR of the given input tensors. Zeros are treated as False and nonzeros
 * are treated as True. Mathematical formulas: 0^0=0�?^1=1�?^0=1�?^1=0
 * @ingroup xor
 * @param [out] dstTensor, output LocalTensor
 * @param [in] srcTensor0, input LocalTensor
 * @param [in] srcTensor1, input LocalTensor
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.math_compute.xor_interface
 * @override_id Xor__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT
 * @archs 3510 2201 3002 2002
 * @brief 按元素执行Xor运算。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] src0Tensor 源操作数0。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] src1Tensor 源操作数1。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 */
__aicore__ inline void Xor(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor)
{
#if defined(ASCENDC_CPU_DEBUG) && (ASCENDC_CPU_DEBUG == 1)
    bool result = (src0Tensor.GetSize() == src1Tensor.GetSize());
    ASCENDC_ASSERT(result, {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to check src0Tensor size %u and src1Tensor size %u, "
            "should be equal When calCount parameter is not included.",
            src0Tensor.GetSize(), src1Tensor.GetSize());
    });
#endif
    Xor<T, isReuseSource>(dstTensor, src0Tensor, src1Tensor, src0Tensor.GetSize());
}
#pragma end_pipe
} // namespace AscendC
#endif // LIB_MATH_XOR_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_XOR_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_XOR_H__
#endif
