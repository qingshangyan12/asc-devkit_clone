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
    "impl/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/fixpipe_l0c2out.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file fixpipe_l0c2out.h
 * \brief Refactored implementation using strategy pattern for better maintainability
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FIXPIPE_L0C2OUT_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FIXPIPE_L0C2OUT_H

// Unified strategy-based implementation
#include "impl/experimental/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/strategies/nz_conversion.h"

namespace AscendC {
namespace Te {

/**
 * \brief Fixpipe L0C to output executor for 3510 architecture
 *
 * This class provides the main entry point for Fixpipe operations.
 * It uses the strategy pattern internally for better code organization.
 */
class FixpipeL0C2Out3510 {
public:
    /**
     * \brief Execute Fixpipe operation
     * \tparam trait Fixpipe trait configuration
     * \tparam T Destination tensor type
     * \tparam U Source tensor type
     * \tparam Params Parameter pack types
     * \param dst Destination tensor
     * \param src Source tensor
     * \param params Additional parameters
     */
    template <const FixpipeTrait& trait, typename T, typename U, typename... Params>
    __aicore__ inline static void Run(const T& dst, const U& src, const Params&... params)
    {
        Execute<trait>(dst, src, params...);
    }

private:
    /**
     * \brief Internal execution with automatic quantization mode detection
     */
    template <const FixpipeTrait& trait, typename T, typename U, typename... Params>
    __aicore__ inline static void Execute(const T& dst, const U& src, const Params&... params)
    {
        constexpr auto quantPre = GetFixpipeQuantPre<trait, T, U>();

        // Use new strategy-based executor
        if constexpr (IsL0cNZFormat<U>::value) {
            if constexpr (IsNZFormat<T>::value || IsL0cNZFormat<T>::value) {
                NzToNzStrategy::Execute<trait, quantPre, T, U>(dst, src, params...);
            } else if constexpr (IsNDFormat<T>::value) {
                NzToNdStrategy::Execute<trait, quantPre, T, U>(dst, src, params...);
            } else if constexpr (IsDNFormat<T>::value) {
                NzToDnStrategy::Execute<trait, quantPre, T, U>(dst, src, params...);
            }
        }
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FIXPIPE_L0C2OUT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
