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
    "impl/tensor_api/arch/vector/cast/instruction.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file instruction.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_H
#define IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_H

#include "impl/tensor_api/tensor/pointer_pattern.h"
#include "impl/tensor_api/tensor/tensor_impl.h"
#include "impl/tensor_api/arch/utils/arch_utils.h"

namespace AscendC {
namespace Te {
namespace Inst {

class UpdateMask {
public:
    template <typename T, typename U>
    __simd_callee__ inline static vector_bool Run(U& value)
    {
        if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return asc_update_mask_b32(value);
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return asc_update_mask_b16(value);
        } else if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return asc_update_mask_b8(value);
        } else {
            return vector_bool{};
        }
    }
};

class CreateMask {
public:
    template <typename T, typename PatType>
    __simd_callee__ inline static vector_bool Run()
    {
        if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return asc_create_mask_b32(PatType());
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return asc_create_mask_b16(PatType());
        } else if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return asc_create_mask_b8(PatType());
        } else {
            return vector_bool{};
        }
    }
};

class Ceil {
public:
    template <typename T>
    __simd_callee__ inline static void Run(T& dst, T src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_ceil(dst, src, mask);
        }
    }
};

class U82U16 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_uint82uint16(dst, src, mask);
        }
    }
};

class BF162E1M2RD {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rd(dst, src, mask);
        }
    }
};

class BF162E1M2X2RDV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rd_v2(dst, src, mask);
        }
    }
};

class BF162E1M2X2RDV3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rd_v3(dst, src, mask);
        }
    }
};

class BF162E1M2X2RDV4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rd_v4(dst, src, mask);
        }
    }
};

class BF162E1M2X2RN {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rn(dst, src, mask);
        }
    }
};

class BF162E1M2X2RNV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rn_v2(dst, src, mask);
        }
    }
};

class BF162E1M2X2RNV3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rn_v3(dst, src, mask);
        }
    }
};

class BF162E1M2X2RNV4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rn_v4(dst, src, mask);
        }
    }
};

class BF162E1M2X2RNA {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rna(dst, src, mask);
        }
    }
};

class BF162E1M2X2RNAV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rna_v2(dst, src, mask);
        }
    }
};

class BF162E1M2X2RNAV3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rna_v3(dst, src, mask);
        }
    }
};

class BF162E1M2X2RNAV4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rna_v4(dst, src, mask);
        }
    }
};

class BF162E1M2X2RU {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_ru(dst, src, mask);
        }
    }
};

class BF162E1M2X2RUV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_ru_v2(dst, src, mask);
        }
    }
};

class BF162E1M2X2RUV3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_ru_v3(dst, src, mask);
        }
    }
};

class BF162E1M2X2RUV4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_ru_v4(dst, src, mask);
        }
    }
};

class BF162E1M2X2RZ {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rz(dst, src, mask);
        }
    }
};

class BF162E1M2X2RZV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rz_v2(dst, src, mask);
        }
    }
};

class BF162E1M2X2RZV3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rz_v3(dst, src, mask);
        }
    }
};

class BF162E1M2X2RZV4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e1m2x2_rz_v4(dst, src, mask);
        }
    }
};

class E1M2X22BF16 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_e1m2x22bfloat16(dst, src, mask);
        }
    }
};

class E1M2X22BF16V2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_e1m2x22bfloat16_v2(dst, src, mask);
        }
    }
};

class E1M2X22BF16V3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_e1m2x22bfloat16_v3(dst, src, mask);
        }
    }
};

class E1M2X22BF16V4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_e1m2x22bfloat16_v4(dst, src, mask);
        }
    }
};

class BF162E2M1X2RD {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rd(dst, src, mask);
        }
    }
};

class BF162E2M1X2RDV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rd_v2(dst, src, mask);
        }
    }
};

class BF162E2M1X2RDV3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rd_v3(dst, src, mask);
        }
    }
};

class BF162E2M1X2RDV4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rd_v4(dst, src, mask);
        }
    }
};

class BF162E2M1X2RN {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rn(dst, src, mask);
        }
    }
};

class BF162E2M1X2RNV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rn_v2(dst, src, mask);
        }
    }
};

class BF162E2M1X2RNV3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rn_v3(dst, src, mask);
        }
    }
};

class BF162E2M1X2RNV4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rn_v4(dst, src, mask);
        }
    }
};

class BF162E2M1X2RNA {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rna(dst, src, mask);
        }
    }
};

class BF162E2M1X2RNAV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rna_v2(dst, src, mask);
        }
    }
};

class BF162E2M1X2RNAV3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rna_v3(dst, src, mask);
        }
    }
};

class BF162E2M1X2RNAV4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rna_v4(dst, src, mask);
        }
    }
};

class BF162E2M1X2RU {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_ru(dst, src, mask);
        }
    }
};

class BF162E2M1X2RUV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_ru_v2(dst, src, mask);
        }
    }
};

class BF162E2M1X2RUV3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_ru_v3(dst, src, mask);
        }
    }
};

class BF162E2M1X2RUV4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_ru_v4(dst, src, mask);
        }
    }
};

class BF162E2M1X2RZ {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rz(dst, src, mask);
        }
    }
};

class BF162E2M1X2RZV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rz_v2(dst, src, mask);
        }
    }
};

class BF162E2M1X2RZV3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rz_v3(dst, src, mask);
        }
    }
};

class BF162E2M1X2RZV4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_bfloat162e2m1x2_rz_v4(dst, src, mask);
        }
    }
};

class E2M1X22BF16 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_e2m1x22bfloat16(dst, src, mask);
        }
    }
};

class E2M1X22BF16V2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_e2m1x22bfloat16_v2(dst, src, mask);
        }
    }
};

class E2M1X22BF16V3 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_e2m1x22bfloat16_v3(dst, src, mask);
        }
    }
};

class E2M1X22BF16V4 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_e2m1x22bfloat16_v4(dst, src, mask);
        }
    }
};

class Float2HalfRD {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rd(dst, src, mask);
        }
    }
};

class Float2HalfRDSat {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rd_sat(dst, src, mask);
        }
    }
};

class Float2HalfRDV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rd_v2(dst, src, mask);
        }
    }
};

class Float2HalfRDSatV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rd_sat_v2(dst, src, mask);
        }
    }
};

class Float2HalfRN {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rn(dst, src, mask);
        }
    }
};

class Float2HalfRNSat {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rn_sat(dst, src, mask);
        }
    }
};

class Float2HalfRNV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rn_v2(dst, src, mask);
        }
    }
};

class Float2HalfRNSatV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rn_sat_v2(dst, src, mask);
        }
    }
};

class Float2HalfRNA {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rna(dst, src, mask);
        }
    }
};

class Float2HalfRNASat {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rna_sat(dst, src, mask);
        }
    }
};

class Float2HalfRNAV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rna_v2(dst, src, mask);
        }
    }
};

class Float2HalfRNASatV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rna_sat_v2(dst, src, mask);
        }
    }
};

class Float2HalfRO {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_ro(dst, src, mask);
        }
    }
};

class Float2HalfROSat {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_ro_sat(dst, src, mask);
        }
    }
};

class Float2HalfROV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_ro_v2(dst, src, mask);
        }
    }
};

class Float2HalfROSatV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_ro_sat_v2(dst, src, mask);
        }
    }
};

class Float2HalfRU {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_ru(dst, src, mask);
        }
    }
};

class Float2HalfRUSat {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_ru_sat(dst, src, mask);
        }
    }
};

class Float2HalfRUV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_ru_v2(dst, src, mask);
        }
    }
};

class Float2HalfRUSatV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_ru_sat_v2(dst, src, mask);
        }
    }
};

class Float2HalfRZ {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rz(dst, src, mask);
        }
    }
};

class Float2HalfRZSat {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rz_sat(dst, src, mask);
        }
    }
};

class Float2HalfRZV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rz_v2(dst, src, mask);
        }
    }
};

class Float2HalfRZSatV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_float2half_rz_sat_v2(dst, src, mask);
        }
    }
};

class Half2Int8RD {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rd(dst, src, mask);
        }
    }
};

class Half2Int8RDSat {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rd_sat(dst, src, mask);
        }
    }
};

class Half2Int8RDV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rd_v2(dst, src, mask);
        }
    }
};

class Half2Int8RDSatV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rd_sat_v2(dst, src, mask);
        }
    }
};

class Half2Int8RN {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rn(dst, src, mask);
        }
    }
};

class Half2Int8RNSat {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rn_sat(dst, src, mask);
        }
    }
};

class Half2Int8RNV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rn_v2(dst, src, mask);
        }
    }
};

class Half2Int8RNSatV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rn_sat_v2(dst, src, mask);
        }
    }
};

class Half2Int8RNA {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rna(dst, src, mask);
        }
    }
};

class Half2Int8RNASat {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rna_sat(dst, src, mask);
        }
    }
};

class Half2Int8RNAV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rna_v2(dst, src, mask);
        }
    }
};

class Half2Int8RNASatV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rna_sat_v2(dst, src, mask);
        }
    }
};

class Half2Int8RU {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_ru(dst, src, mask);
        }
    }
};

class Half2Int8RUSat {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_ru_sat(dst, src, mask);
        }
    }
};

class Half2Int8RUV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_ru_v2(dst, src, mask);
        }
    }
};

class Half2Int8RUSatV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_ru_sat_v2(dst, src, mask);
        }
    }
};

class Half2Int8RZ {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rz(dst, src, mask);
        }
    }
};

class Half2Int8RZSat {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rz_sat(dst, src, mask);
        }
    }
};

class Half2Int8RZV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rz_v2(dst, src, mask);
        }
    }
};

class Half2Int8RZSatV2 {
public:
    template <typename T, typename U>
    __simd_callee__ inline static void Run(T& dst, U src, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_half2int8_rz_sat_v2(dst, src, mask);
        }
    }
};

}}}

#endif // IMPL_TENSOR_API_ARCH_VECTOR_CAST_INSTRUCTION_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
