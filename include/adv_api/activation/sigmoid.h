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
 * \file sigmoid.h
 * \brief Sigmoid is an activation function,
 * Mathematical formulas: Sigmoid(x) = 1 / (1 + exp(-x))
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "sigmoid.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SIGMOID_H__
#endif

#ifndef LIB_SIGMOID_SIGMOID_H
#define LIB_SIGMOID_SIGMOID_H

#include "kernel_tensor.h"
#if defined(__NPU_ARCH__) &&                                                                         \
    (__NPU_ARCH__ == 1001 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || \
     __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
#include "../../../impl/adv_api/detail/activation/sigmoid/sigmoid_common_impl.h"
#endif
namespace AscendC {
#pragma begin_pipe(V)
/*!
 * \ingroup Sigmoid
 * \brief compute Sigmoid elementwisely
 * \tparam T: input dataType, support half/float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason,
 *         not enabled currently.
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor
 * \param [in] sharedTmpBuffer：extra temporary shared space used for intermediate values among calculation process,
 *             whose required space size should refer to corresponding tiling API, which is defined at
 *             sigmoid_tiling.h. Generally, the more space you allocate, the better performance you will achieve,
 *             and the performance reaches peak when buffer size is maximum(calculated by tiling function). Moreover, it
 *             is not guaranteed that the shared space will be cleared after usage, the data could be anything.
 * \param [in] calCount: amount of input data to be calculated
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.sigmoid_interface
 * @override_id Sigmoid__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8_u32
 * @archs 3510 2201 2002 1001 x90 9030
 * @brief 按元素做逻辑回归Sigmoid。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcTensor 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于Sigmoid内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetSigmoidMaxMinTmpSize](GetSigmoidMaxMinTmpSize.md)。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void Sigmoid(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
#if defined(__NPU_ARCH__) &&                                                                         \
    (__NPU_ARCH__ == 1001 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || \
     __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    SigmoidImpl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
#endif
}

/*!
 * \ingroup Sigmoid
 * \brief compute Sigmoid elementwisely
 * \tparam T: input dataType, support half/float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason,
 *         not enabled currently.
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor
 * \param [in] calCount: amount of input data to be calculated
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.sigmoid_interface
 * @override_id Sigmoid__RCltg1x2_pT_RCltg1x2_pT_u32
 * @archs 3510 2201 2002 1001 x90 9030
 * @brief 按元素做逻辑回归Sigmoid。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcTensor 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void Sigmoid(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
#if defined(__NPU_ARCH__) &&                                                                         \
    (__NPU_ARCH__ == 1001 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || \
     __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    SigmoidImpl<T, isReuseSource>(dstTensor, srcTensor, calCount);
#endif
}

/*!
 * \ingroup Sigmoid
 * \brief compute Sigmoid elementwisely
 * \tparam T: input dataType, support half/float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason,
 *         not enabled currently.
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor
 * \param [in] sharedTmpBuffer：extra temporary shared space used for intermediate values among calculation process,
 *             whose required space size should refer to corresponding tiling API, which is defined at
 *             sigmoid_tiling.h. Generally, the more space you allocate, the better performance you will achieve,
 *             and the performance reaches peak when buffer size is maximum(calculated by tiling function). Moreover, it
 *             is not guaranteed that the shared space will be cleared after usage, the data could be anything.
 * \param [in] calCount: amount of input data to be calculated
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.sigmoid_interface
 * @override_id Sigmoid__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8
 * @archs 3510 2201 2002 1001 x90 9030
 * @brief 按元素做逻辑回归Sigmoid。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcTensor 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于Sigmoid内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetSigmoidMaxMinTmpSize](GetSigmoidMaxMinTmpSize.md)。
 */
__aicore__ inline void Sigmoid(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer)
{
    Sigmoid<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, srcTensor.GetSize());
}

/*!
 * \ingroup Sigmoid
 * \brief compute Sigmoid elementwisely
 * \tparam T: input dataType, support half/float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason,
 *         not enabled currently.
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.sigmoid_interface
 * @override_id Sigmoid__RCltg1x2_pT_RCltg1x2_pT
 * @archs 3510 2201 2002 1001 x90 9030
 * @brief 按元素做逻辑回归Sigmoid。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcTensor 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 */
__aicore__ inline void Sigmoid(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
{
    Sigmoid<T, isReuseSource>(dstTensor, srcTensor, srcTensor.GetSize());
}

#pragma end_pipe
} // namespace AscendC
#endif // LIB_SIGMOID_SIGMOID_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SIGMOID_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SIGMOID_H__
#endif
