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
#warning                                                                                                               \
    "impl/tensor_api/arch/vector/ub_to_gm/copy_impl/instruction.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file instruction.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_UB_TO_GM_COPY_IMPL_INSTRUCTION_H
#define IMPL_TENSOR_API_ARCH_VECTOR_UB_TO_GM_COPY_IMPL_INSTRUCTION_H

#include "impl/tensor_api/tensor/pointer_pattern.h"
#include "impl/tensor_api/tensor/tensor_impl.h"
#include "impl/tensor_api/arch/utils/arch_utils.h"

namespace asc {
namespace te {

class copy_ubuf_to_gm_align_v2_instr {
public:
    template <typename T, typename U, typename... Params>
    __aicore__ inline static void data_copy(const T& dst, const U& src, const Params&... params)
    {
        using src_type = typename U::element_type;
        if constexpr (sizeof(src_type) == 1) {
            copy_ubuf_to_gm_align_v2((__gm__ uint8_t*)(dst.data().get()), (__ubuf__ uint8_t*)(src.data().get()),
                                     params...);
        } else if constexpr (sizeof(src_type) == 2) {
            copy_ubuf_to_gm_align_v2((__gm__ uint16_t*)(dst.data().get()), (__ubuf__ uint16_t*)(src.data().get()),
                                     params...);
        } else if constexpr (sizeof(src_type) == 4 || sizeof(src_type) == 8) {
            copy_ubuf_to_gm_align_v2((__gm__ uint32_t*)(dst.data().get()), (__ubuf__ uint32_t*)(src.data().get()),
                                     params...);
        } else {
            static_assert(sizeof(src_type) == 1 || sizeof(src_type) == 2 || sizeof(src_type) == 4
                              || sizeof(src_type) == 8,
                          "Unsupported data type size for CopyUbufToGmAlignV2");
        }
    }

    template <typename T>
    __aicore__ inline static void
    copy_ubuf_to_gm_align_v2(__gm__ T* dst, __ubuf__ T* src, const uint16_t block_count, const uint32_t block_len,
                             const int64_t src_stride, const int64_t dst_stride,
                             const asc_store_l2_cache_mode cache_mode = asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM)
    {
        if ASCEND_IS_AIC {
            return;
        }
        asc_copy_ub2gm_align(dst, src, block_count, block_len, cache_mode, dst_stride,
                             static_cast<uint32_t>(src_stride));
    }
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_UB_TO_GM_COPY_IMPL_INSTRUCTION_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
