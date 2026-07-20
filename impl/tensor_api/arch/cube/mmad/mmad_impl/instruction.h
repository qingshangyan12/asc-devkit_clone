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
    "impl/tensor_api/arch/cube/mmad/mmad_impl/instruction.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file instruction.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_MMAD_MMAD_IMPL_INSTRUCTION_H
#define IMPL_TENSOR_API_ARCH_CUBE_MMAD_MMAD_IMPL_INSTRUCTION_H

#include "impl/tensor_api/arch/utils/arch_utils.h"
#include "impl/tensor_api/tensor/pointer_pattern.h"
#include "impl/tensor_api/tensor/tensor_impl.h"
#include "kernel_npu_debug.h"

namespace AscendC {
namespace Te {

__aicore__ inline static void SetCtrlForhifloat8()
{
    uint64_t oriConfig = asc_get_ctrl();
    uint64_t config = oriConfig | HIFLOAT8_MMAD_CTRL_MASK;
    asc_set_ctrl(config);
}

__aicore__ inline static void ResetCtrlForhifloat8()
{
    uint64_t oriConfig = asc_get_ctrl();
    uint64_t config = oriConfig & ~HIFLOAT8_MMAD_CTRL_MASK;
    asc_set_ctrl(config);
}

class MmadInstr {
public:
    template <typename T, typename U, typename S, typename... Params>
    __aicore__ inline static void Mmad(const T& dst, const U& fm, const S& filter, const Params&... params)
    {
        // MTE2
        MmadImpl(dst.Data().Get(), fm.Data().Get(), filter.Data().Get(), params...);
    }

private:
    template <typename T, typename U, typename S>
    __aicore__ inline static void MmadImpl(
        __cc__ T* dst, __ca__ U* fm, __cb__ S* filter, uint16_t m, uint16_t k, uint16_t n, uint8_t unitFlag,
        bool disableGemv, bool cmatrixSource, bool cmatrixInitVal)
    {
        if ASCEND_IS_AIV {
            return;
        }
        ASCENDC_DEBUG_ASSERT((m <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.m in MmadInstr, MmadParams.m should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((n <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.n in MmadInstr, MmadParams.n should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((k <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.k in MmadInstr, MmadParams.k should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((unitFlag == 0 || unitFlag == 2 || unitFlag == 3), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.unitFlag in MmadInstr, supported values are 0, 2, and 3."));
        if constexpr (Std::is_same_v<U, hifloat8_t> && Std::is_same_v<S, hifloat8_t>) {
            SetCtrlForhifloat8();
            asc_mmad(
                dst, reinterpret_cast<__ca__ fp8_e4m3fn_t*>(fm), reinterpret_cast<__cb__ fp8_e4m3fn_t*>(filter), m, k,
                n, unitFlag, disableGemv, cmatrixSource, cmatrixInitVal);
            ResetCtrlForhifloat8();
        } else {
            asc_mmad(dst, fm, filter, m, k, n, unitFlag, disableGemv, cmatrixSource, cmatrixInitVal);
        }
    }
};

class MmadBiasInstr {
public:
    template <typename T, typename U, typename S, typename V, typename... Params>
    __aicore__ inline static void Mmad(
        const T& dst, const U& fm, const S& filter, const V& bias, const Params&... params)
    {
        // MTE2
        MmadImpl(
            dst.Data().Get(), fm.Data().Get(), filter.Data().Get(), reinterpret_cast<uint64_t>(bias.Data().Get()),
            params...);
    }

private:
    template <typename T, typename U, typename S>
    __aicore__ inline static void MmadImpl(
        __cc__ T* dst, __ca__ U* fm, __cb__ S* filter, uint64_t bias, uint16_t m, uint16_t k, uint16_t n,
        int8_t unitFlag, bool disableGemv, bool cmatrixSource, bool cmatrixInitVal)
    {
        if ASCEND_IS_AIV {
            return;
        }

        ASCENDC_DEBUG_ASSERT((m <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.m in MmadBiasInstr, MmadParams.m should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((n <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.n in MmadBiasInstr, MmadParams.n should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((k <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.k in MmadBiasInstr, MmadParams.k should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((unitFlag == 0 || unitFlag == 2 || unitFlag == 3), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.unitFlag in MmadBiasInstr, supported values are 0, 2, and 3."));
        uint64_t xd = ((uint64_t)dst) & 0xffffffffULL | ((bias & 0xffffffffULL) << 32);
        if constexpr (Std::is_same_v<U, hifloat8_t> && Std::is_same_v<S, hifloat8_t>) {
            SetCtrlForhifloat8();
            asc_mmad(
                (__cc__ T*)xd, reinterpret_cast<__ca__ fp8_e4m3fn_t*>(fm),
                reinterpret_cast<__cb__ fp8_e4m3fn_t*>(filter), m, k, n, unitFlag, disableGemv, cmatrixSource,
                cmatrixInitVal);
            ResetCtrlForhifloat8();
        } else {
            asc_mmad((__cc__ T*)xd, fm, filter, m, k, n, unitFlag, disableGemv, cmatrixSource, cmatrixInitVal);
        }
    }
};

class MmadMxInstr {
public:
    template <typename T, typename U, typename S, typename... Params>
    __aicore__ inline static void Mmad(const T& dst, const U& fm, const S& filter, const Params&... params)
    {
        // MTE2
        MmadImpl(dst.Data().Get(), fm.Data().Get(), filter.Data().Get(), params...);
    }

private:
    template <typename T, typename U, typename S>
    __aicore__ inline static void MmadImpl(
        __cc__ T* dst, __ca__ U* fm, __cb__ S* filter, uint16_t m, uint16_t k, uint16_t n, uint8_t unitFlag,
        bool disableGemv, bool cmatrixSource, bool cmatrixInitVal)
    {
        if ASCEND_IS_AIV {
            return;
        }
        ASCENDC_DEBUG_ASSERT((m <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.m in MmadMxInstr, MmadParams.m should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((n <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.n in MmadMxInstr, MmadParams.n should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((k <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.k in MmadMxInstr, MmadParams.k should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((unitFlag == 0 || unitFlag == 2 || unitFlag == 3), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.unitFlag in MmadMxInstr, supported values are 0, 2, and 3."));
        asc_mmad_mx(dst, fm, filter, m, k, n, unitFlag, disableGemv, cmatrixSource, cmatrixInitVal);
    }
};

class MmadMxBiasInstr {
public:
    template <typename T, typename U, typename S, typename V, typename... Params>
    __aicore__ inline static void Mmad(
        const T& dst, const U& fm, const S& filter, const V& bias, const Params&... params)
    {
        // MTE2
        MmadImpl(
            dst.Data().Get(), fm.Data().Get(), filter.Data().Get(), reinterpret_cast<uint64_t>(bias.Data().Get()),
            params...);
    }

private:
    template <typename T, typename U, typename S>
    __aicore__ inline static void MmadImpl(
        __cc__ T* dst, __ca__ U* fm, __cb__ S* filter, uint64_t bias, uint16_t m, uint16_t k, uint16_t n,
        int8_t unitFlag, bool disableGemv, bool cmatrixSource, bool cmatrixInitVal)
    {
        if ASCEND_IS_AIV {
            return;
        }
        ASCENDC_DEBUG_ASSERT((m <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.m in MmadMxBiasInstr, MmadParams.m should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((n <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.n in MmadMxBiasInstr, MmadParams.n should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((k <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.k in MmadMxBiasInstr, MmadParams.k should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((unitFlag == 0 || unitFlag == 2 || unitFlag == 3), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check MmadParams.unitFlag in MmadMxBiasInstr, supported values are 0, 2, and 3."));
        uint64_t xd = ((uint64_t)dst) & 0xffffffffULL | ((bias & 0xffffffffULL) << 32);
        asc_mmad_mx((__cc__ T*)xd, fm, filter, m, k, n, unitFlag, disableGemv, cmatrixSource, cmatrixInitVal);
    }
};

} // namespace Te
} // namespace AscendC

#endif

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
