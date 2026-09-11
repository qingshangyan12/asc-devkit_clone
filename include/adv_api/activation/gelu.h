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
 * \file gelu.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "gelu.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_GELU_H__
#endif

#ifndef LIB_GELU_GELU_H
#define LIB_GELU_GELU_H
#include "kernel_tensor.h"
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
#include "../../../impl/adv_api/detail/activation/gelu/gelu_3510_impl.h"
#else
#include "../../../impl/adv_api/detail/activation/gelu/gelu_impl.h"
#endif

namespace AscendC {
#pragma begin_pipe(V)
/* !
 * \brief Gelu(x) = x / (1 + e ^ (-1.59576912 * (x + 0.044715 * x ^ 3)))
 * \param [out] dstLocal, output LocalTensor
 * \param [in] srcLocal, input LocalTensor
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] dataSize, number of input data elements
 * \param [in] highPrecision, whether to enable the high-precision interface to improve the calculation accuracy
 * \param [in] highPerformance, whether to enable the high-performance interface to improve the computing efficiency
 */
template <typename T, bool highPrecision = false, bool highPerformance = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.gelu_interface
 * @override_id Gelu__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8_u32
 * @archs 3510 2201 2002 x90 9030
 * @brief 在神经网络中，GELU是一个重要的激活函数，其灵感来源于Relu和Dropout，在激活中引入了随机正则的思想。
 * @param [out] dstLocal 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcLocal 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于接口内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetGeluMaxMinTmpSize](GetGeluMaxMinTmpSize.md)。
 * @param [in] dataSize 实际计算数据元素个数。
 */
__aicore__ inline void Gelu(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t dataSize)
{
    if ASCEND_IS_AIC {
        return;
    }
    GeluImpl<T, highPrecision, highPerformance>(dstLocal, srcLocal, sharedTmpBuffer, dataSize);
}

/* !
 * \ingroup Gelu
 * \param [out] dstLocal, output LocalTensor
 * \param [in] srcLocal, input LocalTensor
 * \param [in] dataSize, number of input data elements
 * \param [in] highPrecision, whether to enable the high-precision interface to improve the calculation accuracy
 * \param [in] highPerformance, whether to enable the high-performance interface to improve the computing efficiency
 */
template <typename T, bool highPrecision = false, bool highPerformance = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.gelu_interface
 * @override_id Gelu__RCltg1x2_pT_RCltg1x2_pT_u32
 * @archs 3510 2201 2002 x90 9030
 * @brief 在神经网络中，GELU是一个重要的激活函数，其灵感来源于Relu和Dropout，在激活中引入了随机正则的思想。
 * @param [out] dstLocal 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcLocal 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] dataSize 实际计算数据元素个数。
 */
__aicore__ inline void Gelu(const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const uint32_t dataSize)
{
    if ASCEND_IS_AIC {
        return;
    }
    GeluImpl<T, highPrecision, highPerformance>(dstLocal, srcLocal, dataSize);
}

/* !
 * \brief FasterGelu(x) = x / (1 + e ^ (-1.702 * x))
 * \param [out] dstLocal, output LocalTensor
 * \param [in] srcLocal, input LocalTensor
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] dataSize, number of input data elements
 * \param [in] highPrecision, whether to enable the high-precision interface to improve the calculation accuracy
 * \param [in] highPerformance, whether to enable the high-performance interface to improve the computing efficiency
 */
template <typename T, bool highPrecision = false, bool highPerformance = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.gelu_interface
 * @override_id FasterGelu__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8_u32
 * @archs 3510 2201 2002 x90 9030
 * @brief 在神经网络中，GELU是一个重要的激活函数，其灵感来源于relu和dropout，在激活中引入了随机正则的思想。为了降低GELU的算力需求，业界提出了FastGelu等版本。本接口FasterGelu是针对FastGelu的化简版本，公式化简可以大幅度提升计算性能。
 * @param [out] dstLocal 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcLocal 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于接口内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetGeluMaxMinTmpSize](GetGeluMaxMinTmpSize.md)。
 * @param [in] dataSize 实际计算数据元素个数。
 */
__aicore__ inline void FasterGelu(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t dataSize)
{
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    ASCENDC_ASSERT((IsSameType<T, half>::value || IsSameType<T, float>::value), {
        KERNEL_LOG(KERNEL_ERROR, "FasterGelu only support data type: float/half");
    });
#endif
    FasterGeluImpl<T, highPrecision, highPerformance>(dstLocal, srcLocal, sharedTmpBuffer, dataSize);
}

/* !
 * \ingroup FasterGelu
 * \param [out] dstLocal, output LocalTensor
 * \param [in] srcLocal, input LocalTensor
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] dataSize, number of input data elements
 * \param [in] highPrecision, whether to enable the high-precision interface to improve the calculation accuracy
 * \param [in] highPerformance, whether to enable the high-performance interface to improve the computing efficiency
 */
template <typename T, bool highPrecision = false, bool highPerformance = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.gelu_interface
 * @override_id FasterGelu__RCltg1x2_pT_RCltg1x2_pT_u32
 * @archs 3510 2201 2002 x90 9030
 * @brief 在神经网络中，GELU是一个重要的激活函数，其灵感来源于relu和dropout，在激活中引入了随机正则的思想。为了降低GELU的算力需求，业界提出了FastGelu等版本。本接口FasterGelu是针对FastGelu的化简版本，公式化简可以大幅度提升计算性能。
 * @param [out] dstLocal 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcLocal 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] dataSize 实际计算数据元素个数。
 */
__aicore__ inline void FasterGelu(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const uint32_t dataSize)
{
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    ASCENDC_ASSERT((IsSameType<T, half>::value || IsSameType<T, float>::value), {
        KERNEL_LOG(KERNEL_ERROR, "FasterGelu only support data type: float/half");
    });
#endif
    FasterGeluImpl<T, highPrecision, highPerformance>(dstLocal, srcLocal, dataSize);
}

/* !
 * \brief sgn(x) = (x + 0.000000000001) / |(x + 0.000000000001)|
 * \brief fast_gelu_v2(x) = x * (sgn(x) * [(-0.1444) * (clip(|0.7071 * x|, max=1.769) - 1.769) ^ 2 + 0.5] + 0.5)
 * \param [out] dstLocal, output LocalTensor
 * \param [in] srcLocal, input LocalTensor
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] dataSize, number of input data elements
 * \param [in] highPrecision, whether to enable the high-precision interface to improve the calculation accuracy
 * \param [in] highPerformance, whether to enable the high-performance interface to improve the computing efficiency
 */
template <typename T, bool highPrecision = false, bool highPerformance = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.gelu_interface
 * @override_id FasterGeluV2__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8_u32
 * @archs 3510 2201 2002
 * @brief 在神经网络中，GELU是一个重要的激活函数，其灵感来源于relu和dropout，在激活中引入了随机正则的思想。为了降低GELU的算力需求，业界提出了FastGeluV2版本。
 * @param [out] dstLocal 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcLocal 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于接口内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetGeluMaxMinTmpSize](GetGeluMaxMinTmpSize.md)。
 * @param [in] dataSize 实际计算数据元素个数。
 */
__aicore__ inline void FasterGeluV2(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t dataSize)
{
    if ASCEND_IS_AIC {
        return;
    }
    ASCENDC_ASSERT((IsSameType<T, half>::value || IsSameType<T, float>::value), {
        KERNEL_LOG(KERNEL_ERROR, "FasterGeluV2 only support data type: float/half");
    });
    FasterGeluV2Impl<T, highPrecision, highPerformance>(dstLocal, srcLocal, sharedTmpBuffer, dataSize);
}

/* !
 * \ingroup FasterGeluV2
 * \param [out] dstLocal, output LocalTensor
 * \param [in] srcLocal, input LocalTensor
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] dataSize, number of input data elements
 * \param [in] highPrecision, whether to enable the high-precision interface to improve the calculation accuracy
 * \param [in] highPerformance, whether to enable the high-performance interface to improve the computing efficiency
 */
template <typename T, bool highPrecision = false, bool highPerformance = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.gelu_interface
 * @override_id FasterGeluV2__RCltg1x2_pT_RCltg1x2_pT_u32
 * @archs 3510 2201 2002
 * @brief 在神经网络中，GELU是一个重要的激活函数，其灵感来源于relu和dropout，在激活中引入了随机正则的思想。为了降低GELU的算力需求，业界提出了FastGeluV2版本。
 * @param [out] dstLocal 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcLocal 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] dataSize 实际计算数据元素个数。
 */
__aicore__ inline void FasterGeluV2(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const uint32_t dataSize)
{
    if ASCEND_IS_AIC {
        return;
    }
    ASCENDC_ASSERT((IsSameType<T, half>::value || IsSameType<T, float>::value), {
        KERNEL_LOG(KERNEL_ERROR, "FasterGeluV2 only support data type: float/half");
    });
    FasterGeluV2Impl<T, highPrecision, highPerformance>(dstLocal, srcLocal, dataSize);
}
#pragma end_pipe
} // namespace AscendC
#endif // LIB_GELU_GELU_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_GELU_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_GELU_H__
#endif
