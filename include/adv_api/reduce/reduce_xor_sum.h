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
 * \file reduce_xor_sum.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "reduce_xor_sum.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_REDUCE_XOR_SUM_H__
#endif

#ifndef LIB_REDUCE_REDUCE_XOR_SUM_H
#define LIB_REDUCE_REDUCE_XOR_SUM_H

#include "kernel_tensor.h"
#include "kernel_basic_intf.h"
#include "../../../impl/basic_api/kernel_pop_stack_buffer.h"
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
#include "../../../impl/adv_api/detail/reduce/reduce_xor_sum/reduce_xor_sum_3510_impl.h"
#else
#include "../../../impl/adv_api/detail/reduce/reduce_xor_sum/reduce_xor_sum_common_impl.h"
#endif
#if ASCENDC_CPU_DEBUG
#include "../../../impl/basic_api/kernel_log.h"
#include <type_traits>
#endif

namespace AscendC {
#pragma begin_pipe(V)
/*
 * @ingroup ReduceXorSum
 * @brief f(x) = sum(a ^ b)
 *        If the final calculation result is beyond the range of int16,
 *        the calculation result is not guaranteed.
 * @tparam T: Input and output data types, int16
 * @tparam isReuseSrc: Whether temporary variables can reuse the input memory.
 * @param [out] dstTensor: output LocalTensor, the minimum shape is 16.
 * @param [in] src0Tensor: input0 LocalTensor
 * @param [in] src1Tensor: input1 LocalTensor
 * @param [in] sharedTmpBuffer: input local temporary Tensor
 * @param [in] calCount: amount of data to be calculated
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.reduction_operations.reducexorsum_interface
 * @override_id ReduceXorSum__Rltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_Rltg1x2_u8_u32
 * @archs 3510 2201 2002
 * @brief 按照元素执行Xor（按位异或）运算，并将计算结果ReduceSum求和。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。输出值需要sizeof(T)大小的空间进行保存。开发者要根据该大小和框架的对齐要求来为dstTensor分配实际内存空间。**注意**：遵循框架对内存开辟的要求（开辟内存的大小满足32Byte对齐），即sizeof(T)不是32Byte对齐时，需要向上进行32Byte对齐。为了对齐而多开辟的内存空间不填值，为一些随机值。
 * @param [in] src0Tensor 源操作数0。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] src1Tensor 源操作数1。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于ReduceXorSum计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetReduceXorSumMaxMinTmpSize](GetReduceXorSumMaxMinTmpSize.md)。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void ReduceXorSum(
    LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor,
    LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }
#if (                                                                                                 \
    defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3510 || \
                              __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113))
    ReduceXorSumCompute<T, isReuseSource>(dstTensor, src0Tensor, src1Tensor, sharedTmpBuffer, calCount);
#endif
}

/*
 * @ingroup ReduceXorSum
 * @brief f(x) = sum(a ^ b)
 *        If the final calculation result is beyond the range of int16,
 *        the calculation result is not guaranteed.
 * @tparam T: Input and output data types, int16
 * @tparam isReuseSrc: Whether temporary variables can reuse the input memory.
 * @param [out] dstTensor: output LocalTensor, the minimum shape is 16.
 * @param [in] src0Tensor: input0 LocalTensor
 * @param [in] src1Tensor: input1 LocalTensor
 * @param [in] calCount: amount of data to be calculated
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.reduction_operations.reducexorsum_interface
 * @override_id ReduceXorSum__Rltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_u32
 * @archs 3510 2201 2002
 * @brief 按照元素执行Xor（按位异或）运算，并将计算结果ReduceSum求和。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。输出值需要sizeof(T)大小的空间进行保存。开发者要根据该大小和框架的对齐要求来为dstTensor分配实际内存空间。**注意**：遵循框架对内存开辟的要求（开辟内存的大小满足32Byte对齐），即sizeof(T)不是32Byte对齐时，需要向上进行32Byte对齐。为了对齐而多开辟的内存空间不填值，为一些随机值。
 * @param [in] src0Tensor 源操作数0。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] src1Tensor 源操作数1。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void ReduceXorSum(
    LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor,
    const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }
#if (                                                                                                 \
    defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3510 || \
                              __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113))
    LocalTensor<uint8_t> tmp;
    const bool ret = PopStackBuffer<uint8_t, TPosition::LCM>(tmp);
    ASCENDC_ASSERT((ret), { KERNEL_LOG(KERNEL_ERROR, "PopStackBuffer Error!"); });

    ReduceXorSumCompute<T, isReuseSource>(dstTensor, src0Tensor, src1Tensor, tmp, calCount);
#endif
}
#pragma end_pipe
} // namespace AscendC

#endif // LIB_REDUCE_XOR_SUM_REDUCE_XOR_SUM_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_REDUCE_XOR_SUM_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_REDUCE_XOR_SUM_H__
#endif
