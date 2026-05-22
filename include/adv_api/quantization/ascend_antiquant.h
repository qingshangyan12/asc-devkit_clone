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
 * \file ascend_antiquant.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASCEND_ANTIQUANT_H__
#endif

#ifndef LIB_QUANTIZATION_ASCEND_ANTIQUANT_H
#define LIB_QUANTIZATION_ASCEND_ANTIQUANT_H
#include "kernel_tensor.h"
#include "include/adv_api/quantization/ascend_antiquant_utils.h"
#if defined(__NPU_ARCH__)
#include "../../../impl/adv_api/detail/quantization/antiquant/ascend_antiquant_impl.h"
#endif
namespace AscendC {
#pragma begin_pipe(V)
/* !
 * \brief compute dst = scale * (src + offset)
 * \param [out] dst, output LocalTensor
 * \param [in] src, input LocalTensor
 * \param [in] offset, input LocalTensor
 * \param [in] scale, input LocalTensor
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] K, src height/width when isTranspos is false/true
 * \param [in] shapeInfo, input offset/scale shape
 * \param [in] isTranspose, enable transpose of input
 */
template <typename InputDataType, typename OutputDataType, bool isTranspose>
__aicore__ inline void AscendAntiQuant(const LocalTensor<OutputDataType>& dst, const LocalTensor<InputDataType>& src,
    const LocalTensor<OutputDataType>& offset, const LocalTensor<OutputDataType>& scale,
    const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t k, const AntiQuantShapeInfo& shapeInfo = {})
{
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__)
    AscendAntiQuantImpl<InputDataType, OutputDataType, isTranspose>(dst, src, offset, scale, sharedTmpBuffer, k,
        shapeInfo);
#endif
}

/* !
 * \ingroup AscendAntiQuant
 * \param [out] dst, output LocalTensor
 * \param [in] src, input LocalTensor
 * \param [in] scale, input LocalTensor
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] K, src height/width when isTranspos is false/true
 * \param [in] shapeInfo, input offset/scale shape
 * \param [in] isTranspose, enable transpose of input
 */
template <typename InputDataType, typename OutputDataType, bool isTranspose>
__aicore__ inline void AscendAntiQuant(const LocalTensor<OutputDataType>& dst, const LocalTensor<InputDataType>& src,
    const LocalTensor<OutputDataType>& scale, const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t k,
    const AntiQuantShapeInfo& shapeInfo = {})
{
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__)
    AscendAntiQuantImpl<InputDataType, OutputDataType, isTranspose>(dst, src, scale, sharedTmpBuffer, k, shapeInfo);
#endif
}

/* !
 * \ingroup AscendAntiQuant
 * \param [out] dst, output LocalTensor
 * \param [in] src, input LocalTensor
 * \param [in] offset, input LocalTensor
 * \param [in] scale, input LocalTensor
 * \param [in] K, src height/width when isTranspos is false/true
 * \param [in] shapeInfo, input offset/scale shape
 * \param [in] isTranspose, enable transpose of input
 */
template <typename InputDataType, typename OutputDataType, bool isTranspose>
__aicore__ inline void AscendAntiQuant(const LocalTensor<OutputDataType>& dst, const LocalTensor<InputDataType>& src,
    const LocalTensor<OutputDataType>& offset, const LocalTensor<OutputDataType>& scale, const uint32_t k,
    const AntiQuantShapeInfo& shapeInfo = {})
{
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__)
    AscendAntiQuantImpl<InputDataType, OutputDataType, isTranspose>(dst, src, offset, scale, k, shapeInfo);
#endif
}

/* !
 * \ingroup AscendAntiQuant
 * \param [out] dst, output LocalTensor
 * \param [in] src, input LocalTensor
 * \param [in] offset, input Scalar
 * \param [in] scale, input Scalar
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] K, src height/width when isTranspos is false/true
 * \param [in] shapeInfo, input offset/scale shape
 * \param [in] isTranspose, enable transpose of input
 */
template <typename InputDataType, typename OutputDataType, bool isTranspose>
__aicore__ inline void AscendAntiQuant(const LocalTensor<OutputDataType>& dst, const LocalTensor<InputDataType>& src,
    const OutputDataType offset, const OutputDataType scale, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t k, const AntiQuantShapeInfo& shapeInfo = {})
{
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__)
    AscendAntiQuantImpl<InputDataType, OutputDataType, isTranspose>(dst, src, offset, scale, sharedTmpBuffer, k,
        shapeInfo);
#endif
}

/* !
 * \ingroup AscendAntiQuant
 * \param [out] dst, output LocalTensor
 * \param [in] src, input LocalTensor
 * \param [in] scale, input Scalar
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] K, src height/width when isTranspos is false/true
 * \param [in] shapeInfo, input offset/scale shape
 * \param [in] isTranspose, enable transpose of input
 */
template <typename InputDataType, typename OutputDataType, bool isTranspose>
__aicore__ inline void AscendAntiQuant(const LocalTensor<OutputDataType>& dst, const LocalTensor<InputDataType>& src,
    const OutputDataType scale, const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t k,
    const AntiQuantShapeInfo& shapeInfo = {})
{
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__)
    AscendAntiQuantImpl<InputDataType, OutputDataType, isTranspose>(dst, src, scale, sharedTmpBuffer, k, shapeInfo);
#endif
}

/* !
 * \ingroup AscendAntiQuant
 * \param [out] dst, output LocalTensor
 * \param [in] src, input LocalTensor
 * \param [in] offset, input Scalar
 * \param [in] scale, input Scalar
 * \param [in] K, src height/width when isTranspos is false/true
 * \param [in] shapeInfo, input offset/scale shape
 * \param [in] isTranspose, enable transpose of input
 */
template <typename InputDataType, typename OutputDataType, bool isTranspose>
__aicore__ inline void AscendAntiQuant(const LocalTensor<OutputDataType>& dst, const LocalTensor<InputDataType>& src,
    const OutputDataType offset, const OutputDataType scale, const uint32_t k, const AntiQuantShapeInfo& shapeInfo = {})
{
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__)
    AscendAntiQuantImpl<InputDataType, OutputDataType, isTranspose>(dst, src, offset, scale, k, shapeInfo);
#endif
}

#if !defined(__NPU_ARCH__) || (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102))
template <typename InputDataType, typename OutputDataType, bool isTranspose>
__aicore__ inline void AscendAntiQuant(const LocalTensor<OutputDataType>& dst, const LocalTensor<InputDataType>& src,
    const LocalTensor<fp8_e8m0_t>& scale, const uint32_t k, const AntiQuantShapeInfo& shapeInfo = {})
{
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
    AscendAntiQuantImpl<InputDataType, OutputDataType, isTranspose>(dst, src, scale, k, shapeInfo);
#endif
}

template <typename InputDataType, typename OutputDataType, bool isTranspose>
__aicore__ inline void AscendAntiQuant(const LocalTensor<OutputDataType>& dst, const LocalTensor<InputDataType>& src,
    const LocalTensor<fp8_e8m0_t>& scale, const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t k,
    const AntiQuantShapeInfo& shapeInfo = {})
{
    if ASCEND_IS_AIC {
        return;
    }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
    AscendAntiQuantImpl<InputDataType, OutputDataType, isTranspose>(dst, src, scale, sharedTmpBuffer, k, shapeInfo);
#endif
}

template <typename dstT, typename srcT, typename scaleT, const AscendAntiQuantConfig& config, const AscendAntiQuantPolicy& policy>
__aicore__ inline void AscendAntiQuant(const LocalTensor<dstT>& dstTensor, const LocalTensor<srcT>& srcTensor,
                                       const LocalTensor<scaleT> &scaleTensor, const LocalTensor<scaleT> &offsetTensor,
                                       const AscendAntiQuantParam& para)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
    AscendAntiQuantImpl<dstT, srcT, scaleT, config, policy>(dstTensor, srcTensor, scaleTensor, offsetTensor, para);
#endif
}

template <typename dstT, typename srcT, typename scaleT, const AscendAntiQuantConfig& config, const AscendAntiQuantPolicy& policy>
__aicore__ inline void AscendAntiQuant(const LocalTensor<dstT>& dstTensor, const LocalTensor<srcT>& srcTensor,
                                       const LocalTensor<scaleT> &scaleTensor, const LocalTensor<scaleT> &offsetTensor,
                                       const LocalTensor<uint8_t> &sharedTmpBuffer, const AscendAntiQuantParam& para)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
    AscendAntiQuantImpl<dstT, srcT, scaleT, config, policy>(dstTensor, srcTensor, sharedTmpBuffer, scaleTensor, offsetTensor, para);
#endif
}
#endif

#pragma end_pipe
} // namespace AscendC
#endif // LIB_QUANTIZATION_ASCEND_ANTIQUANT_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASCEND_ANTIQUANT_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASCEND_ANTIQUANT_H__
#endif
