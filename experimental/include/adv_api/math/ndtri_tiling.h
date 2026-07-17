/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file ndtri_tiling.h
 * \brief
 */
#ifndef LIB_MATH_NDTRI_TILING_H
#define LIB_MATH_NDTRI_TILING_H
#include <cstdint>

#include "graph/tensor.h"
namespace AscendC {
/*!
 * \brief The Ndtri interface requires no temporary space on Ascend 950PR: the computation is entirely
 * register resident, so every intermediate lives in a vector register and UB is touched only for the
 * input load and the output store. Both outputs are therefore 0.
 * These interfaces exist so that callers written against the common high level API tiling pattern
 * compile and behave correctly (0 means "no temporary space required").
 *
 * \param [in] typeSize, size of the input data type, in bytes
 * \param [out] maxLiveNodeCount, the multiple of the maximum temporary space to the input occupied space
 * \param [out] extraBuf, the size of the extra temporary space
 */
void GetNdtriTmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuffer);

/*!
 * \brief This interface is used to obtain the maximum and minimum temporary space reserved or applied.
 * For Ndtri both values are 0, meaning no temporary space is needed.
 *
 * \param [in] srcShape, input shape information
 * \param [in] typeSize, size of the input data type, in bytes
 * \param [in] isReuseSource, whether to reuse the input space of the source operand
 * \param [out] maxValue, maximum temporary space required (0: no temporary space is used)
 * \param [out] minValue, minimum temporary space required (0: no temporary space is required)
 */
void GetNdtriMaxMinTmpSize(
    const ge::Shape& srcShape, const uint32_t typeSize, const bool isReuseSource, uint32_t& maxValue,
    uint32_t& minValue);
} // namespace AscendC
#endif // LIB_MATH_NDTRI_TILING_H
