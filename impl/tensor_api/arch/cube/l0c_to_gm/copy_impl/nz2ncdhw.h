/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/tensor_api/arch/cube/l0c_to_gm/copy_impl/nz2ncdhw.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file nz2ncdhw.h
 * \brief L0C(NZ) -> GM(NCDHW) copy via the nz2dn fixpipe path (no quant). conv3D output; the depth
 *        axis Do is treated as 1, so this mirrors NZ->NCHW with the dst shape carrying the extra D.
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_IMPL_NZ2NCDHW_H
#define IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_IMPL_NZ2NCDHW_H

#include "impl/tensor_api/arch/cube/l0c_to_gm/copy_impl/instruction.h"

namespace asc {
namespace te {

class DataCopyL0C2GMNZ2NCDHW {
public:
    template <const copy_l0c_to_gm_trait& trait, typename T, typename U>
    __aicore__ inline static void run(const T& dst, const U& src, const fixpipe_params& params)
    {
        constexpr QuantMode_t quantPre = get_quant_mode<trait.round_mode, T, U>();
        CheckDataType::check_l0_c2_gm_data_type<quantPre, T, U>();

        auto dstLayout = dst.layout();
        auto srcLayout = src.layout();

        // nz2dn needs the channel register (src NZ C0 stride) set, mirroring the DN path's
        // EmitSetRegister non-ND branch. The generic SetRegisterImpl would treat NCDHW as neither
        // ND nor DN and emit SetRegister(1,0,0) without SetChannelPara, so set it explicitly here.
        SetRegisterInstr::set_register(1u, 0u, 0u, GetElement<AttrInfo::Stride, AttrInfo::Column, 0>(srcLayout));

        // dst NCDHW (N, C, D, H, W), N == 1 and Do treated as 1. Like NZ->NCHW, NCDHW memory equals
        // DNExt (D*H*W, C) column-major: nSize = C = Shape[1] (column count), mSize = D*H*W =
        // Shape[2]*Shape[3]*Shape[4] (row count; = H*W when D==1), dstStride = the DN column stride =
        // row count = Stride[1] (= D*H*W).
        uint32_t nSize = Get<1>(dstLayout.shape());
        uint32_t mSize = Get<2>(dstLayout.shape()) * Get<3>(dstLayout.shape()) * Get<4>(dstLayout.shape());
        uint32_t srcStride = GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(srcLayout) / FRACTAL_FIXED;
        uint32_t dstStride = Get<1>(dstLayout.stride());

        uint8_t cacheMode = dst.engine().get_cache_mode();
        bool reluEn = trait.enable_relu;
        uint8_t unitFlag = params.unit_flag;
        bool isChannelSplit = trait.enable_channel_split;

        CopyMatrixCcToGmInstr::data_copy<quantPre, T, U>(
            dst, src, nSize, mSize, srcStride, dstStride, cacheMode, reluEn, unitFlag, isChannelSplit, false, true);
    }
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_IMPL_NZ2NCDHW_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
