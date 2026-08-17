/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file bessel_i0_tiling.h
 * \brief Host-side temporary-space query interfaces of the BesselI0 high-level API.
 *
 * This RegBase implementation needs no sharedTmpBuffer at all: the constants required inside
 * the VF body (8.0/200.0/1.0/32.0) are materialized directly in the register domain via
 * Reg::Duplicate, so no UB constant pool is needed. Both query functions therefore always
 * return 0, following the GetXxxMaxMinTmpSize / GetXxxTmpBufferFactorSize signature convention
 * of the high-level API family.
 */
#ifndef LIB_MATH_BESSEL_I0_TILING_H
#define LIB_MATH_BESSEL_I0_TILING_H

#include <cstdint>

#include "../utils/types.h"

namespace AscendC {

/*!
 * \brief Query the maximum/minimum temporary space (sharedTmpBuffer), in bytes, required by
 * BesselI0.
 *
 * This implementation needs no temporary space and always returns maxValue = minValue = 0,
 * which matches the established "0 means no temporary space is required" semantics of the
 * high-level API family.
 *
 * \param [in] srcShape: input shape information. Unused (the result is shape-independent);
 *        kept to follow the family signature.
 * \param [in] typeSize: size of the input data type, in bytes. Unused.
 * \param [in] isReuseSource: whether to reuse the input space of the source operand. Unused.
 * \param [out] maxValue: maximum temporary space required, always 0.
 * \param [out] minValue: minimum temporary space required, always 0.
 */
void GetBesselI0MaxMinTmpSize(
    const AscendC::TensorShape& srcShape, const uint32_t typeSize, const bool isReuseSource, uint32_t& maxValue,
    uint32_t& minValue);

/*!
 * \brief Query the temporary-space scaling factors of BesselI0 (for callers that extrapolate
 * temporary-space demand linearly with the element count).
 *
 * This implementation needs no temporary space and always returns
 * maxLiveNodeCount = extraBuffer = 0.
 *
 * \param [in] typeSize: size of the input data type, in bytes. Unused.
 * \param [out] maxLiveNodeCount: maximum number of live nodes used for temporary-space
 *        estimation, always 0.
 * \param [out] extraBuffer: size of the extra temporary space, always 0.
 */
void GetBesselI0TmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuffer);

} // namespace AscendC

#endif // LIB_MATH_BESSEL_I0_TILING_H
