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
 * \file clamp.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "clamp.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CLAMP_H__
#endif

#ifndef LIB_MATH_CLAMP_H
#define LIB_MATH_CLAMP_H
#include "kernel_tensor.h"
#include "include/adv_api/math/clamp_utils.h"
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2002 || __NPU_ARCH__ == 2201)
#include "../../../impl/adv_api/detail/math/clamp/clamp_common_impl.h"
#elif defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
#include "../../../impl/adv_api/detail/math/clamp/clamp_3510_impl.h"
#endif
#include "../../../impl/basic_api/kernel_pop_stack_buffer.h"

namespace AscendC {
/* !
 * \brief This function implements replaces numbers greater than scalar with scalar
 * (e.g. ClampMax(2) means to replace numbers greater than 2 with 2 ). For details about the interface description, see
 * https://pytorch.org/docs/stable/generated/torch.clamp.html.
 *
 * \note support data type: half and float
 *
 * \param [out] dstTensor, output LocalTensor
 * \param [in] srcTensor, input LocalTensor
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] calCount, amount of data to be calculated
 */

/* **************************************************************************************************
 * ClampMax                                           *
 * ************************************************************************************************* */
#pragma begin_pipe(V)
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.math_compute.clamp_interface
 * @override_id ClampMax__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8_pT_u32
 * @archs 3510 2201 2002
 * @brief 将srcTensor中大于scalar的数替换为scalar，小于等于scalar的数保持不变，作为dstTensor输出。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcTensor 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于ClampMax内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetClampMaxMinTmpSize](GetClampMaxMinTmpSize.md)。
 * @param [in] scalar scalar数据，数据类型与srcTensor一致，支持数据类型为：half、float。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void ClampMax(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const T scalar, const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3510 || \
                              __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    ClampMaxImpl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, scalar, calCount);
#endif
}

template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.math_compute.clamp_interface
 * @override_id ClampMax__RCltg1x2_pT_RCltg1x2_pT_pT_u32
 * @archs 3510 2201 2002
 * @brief 将srcTensor中大于scalar的数替换为scalar，小于等于scalar的数保持不变，作为dstTensor输出。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcTensor 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] scalar scalar数据，数据类型与srcTensor一致，支持数据类型为：half、float。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void ClampMax(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const T scalar, const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }
    // Using the stack space to allocate tmpbuf
    LocalTensor<uint8_t> sharedTmpBuffer;
    bool ret = PopStackBuffer<uint8_t, TPosition::LCM>(sharedTmpBuffer);
    ASCENDC_ASSERT((ret), { KERNEL_LOG(KERNEL_ERROR, "PopStackBuffer Error!"); });
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3510 || \
                              __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    ClampMaxImpl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, scalar, calCount);
#endif
}

/* !
 * \brief This function implements replace numbers less than scalar with scalar
 * (e.g. ClampMin(2) means to replace numbers less than 2 with 2 ). For details about the interface description, see
 * https://pytorch.org/docs/stable/generated/torch.clamp.html.
 *
 * \note support data type: half and float
 *
 * \param [out] dstTensor, output LocalTensor
 * \param [in] srcTensor, input LocalTensor
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] calCount, amount of data to be calculated
 */
/* **************************************************************************************************
 * ClampMin                                           *
 * ************************************************************************************************* */

template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.math_compute.clamp_interface
 * @override_id ClampMin__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8_pT_u32
 * @archs 3510 2201 2002
 * @brief 将srcTensor中小于scalar的数替换为scalar，大于等于scalar的数保持不变，作为dstTensor输出。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcTensor 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于ClampMin内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetClampMaxMinTmpSize](GetClampMaxMinTmpSize.md)。
 * @param [in] scalar scalar数据，数据类型与srcTensor一致，支持数据类型为：half、float。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void ClampMin(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const T scalar, const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3510 || \
                              __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    ClampMinImpl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, scalar, calCount);
#endif
}

template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.math_compute.clamp_interface
 * @override_id ClampMin__RCltg1x2_pT_RCltg1x2_pT_pT_u32
 * @archs 3510 2201 2002
 * @brief 将srcTensor中小于scalar的数替换为scalar，大于等于scalar的数保持不变，作为dstTensor输出。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcTensor 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] scalar scalar数据，数据类型与srcTensor一致，支持数据类型为：half、float。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void ClampMin(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const T scalar, const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }
    // Using the stack space to allocate tmpbuf
    LocalTensor<uint8_t> sharedTmpBuffer;
    bool ret = PopStackBuffer<uint8_t, TPosition::LCM>(sharedTmpBuffer);
    ASCENDC_ASSERT((ret), { KERNEL_LOG(KERNEL_ERROR, "PopStackBuffer Error!"); });
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3510 || \
                              __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    ClampMinImpl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, scalar, calCount);
#endif
}

/* !
 * \brief This function implements replaces numbers greater than min with min and less than max with max
 * (e.g. Clamp(2, 5) means to replace numbers greater than 2 with 2 and less than 5 with 5).
 * For details about the interface description, see
 * https://pytorch.org/docs/stable/generated/torch.clamp.html.
 *
 * \param [out] dstTensor, output LocalTensor
 * \param [in] srcTensor, input LocalTensor
 * \param [in] min, input the min value
 * \param [in] max, input the max value
 * \param [in] count, amount of data to be calculated
 */

/* ***************************************************************************************************
 * Clamp                                                                                             *
 * ***************************************************************************************************/
template <const ClampConfig& config = DEFAULT_CLAMP_CONFIG, typename T, typename U, typename S>
/**
 * @ingroup simd_api.adv_api.math_compute.clamp_interface
 * @override_id Clamp__RCltg1x2_pT_RCltg1x2_pT_RCpU_RCpS_u32
 * @archs 3510
 * @brief 将输入中除nan值以外大于max的数替换为max，小于min的数替换为min，小于等于max和大于等于min的数保持不变，作为输出。当min大于max时，将除nan值外所有值替换为max。min和max可以为标量或LocalTensor。
 * @param [out] dst 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] src 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型与目的操作数保持一致。
 * @param [in] min 数据下限。类型为标量或LocalTensor，类型为LocalTensor时，支持的TPosition为VECIN/VECCALC/VECOUT。数据类型与目的操作数保持一致。
 * @param [in] max 数据上限。类型为标量或LocalTensor，类型为LocalTensor时，支持的TPosition为VECIN/VECCALC/VECOUT。数据类型与目的操作数保持一致。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void Clamp(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const U& min, const S& max, const uint32_t count)
{
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    ClampImpl<config, T, U, S>(dst, src, min, max, count);
#endif
}
#pragma end_pipe
} // namespace AscendC
#endif // LIB_MATH_CLAMP_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CLAMP_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CLAMP_H__
#endif
