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
 * \file instruction.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_INSTRUCTION_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_INSTRUCTION_H

#include "impl/experimental/tensor_api/tensor/pointer_impl.h"
#include "impl/experimental/tensor_api/tensor/local_tensor_impl.h"
#include "impl/experimental/tensor_api/arch/arch_utils.h"

namespace AscendC {
namespace Te {
class LoadCbufToCaBase {
public:
    template <const LoadDataTrait& trait, typename T, typename U, typename... Params>
    __aicore__ inline void LoadData(const T& dst, const U& src, const Params& ...params)
    {
        LoadCbufToCa<trait.transposed>(dst.Data().Get(), src.Data().Get(), params...);
    }

private:
    template <bool transpose, typename T>
    __aicore__ inline void LoadCbufToCa(__ca__ T* dst, __cbuf__ T* src, uint16_t mStartPosition,
        uint16_t kStartPosition, uint8_t mStep, uint8_t kStep, int16_t srcStride, uint16_t dstStride)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            constexpr bool isFp4Type = std::is_same<T, fp4x2_e2m1_t>::value || std::is_same<T, fp4x2_e1m2_t>::value;
            if constexpr (isFp4Type) {
                constexpr const int KHALF = 2;
                load_cbuf_to_ca_s4(dst, src, mStartPosition, kStartPosition / KHALF, mStep, kStep / KHALF, srcStride, dstStride, transpose);
            } else {
                load_cbuf_to_ca(dst, src, mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride, transpose);
            }
        }
    }
};

class LoadCbufToCaB8Base {
public:
    template <const LoadDataTrait& trait, typename T, typename U, typename... Params>
    __aicore__ inline void LoadData(const T& dst, const U& src, const Params& ...params)
    {
        LoadCbufToCa<trait.transposed>(dst.Data().Get(), src.Data().Get(), params...);
    }

private:
    template <bool transpose, typename T>
    __aicore__ inline void LoadCbufToCa(__ca__ T* dst, __cbuf__ T* src, uint16_t mStartPosition,
        uint16_t kStartPosition, uint8_t mStep, uint8_t kStep, int16_t srcStride, uint16_t dstStride)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            constexpr const int SHIFT_M_STEP_B8 = 1;
            constexpr const int M_STEP_MIN_VAL_B8 = 2;
            uint16_t mLoop = mStep >> SHIFT_M_STEP_B8;
            uint16_t dstAddrStride = mStep * FRACTAL_FIXED * C0_SIZE;
            mStep = M_STEP_MIN_VAL_B8;
            for (uint16_t idx = 0; idx < mLoop; ++idx) {
                load_cbuf_to_ca(dst, src, mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride, transpose);
                dst += dstAddrStride;
                mStartPosition += M_STEP_MIN_VAL_B8;
            }
        }
    }
};

class LoadCbufToCbBase {
public:
    template <const LoadDataTrait& trait, typename T, typename U, typename... Params>
    __aicore__ inline void LoadData(const T& dst, const U& src, const Params& ...params)
    {
        LoadCbufToCb<trait.transposed>(dst.Data().Get(), src.Data().Get(), params...);
    }

private:
    template <bool transpose, typename T>
    __aicore__ inline void LoadCbufToCb(__cb__ T* dst, __cbuf__ T* src, uint16_t mStartPosition,
        uint16_t kStartPosition, uint8_t mStep, uint8_t kStep, int16_t srcStride, uint16_t dstStride)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            constexpr bool isFp4Type = std::is_same<T, fp4x2_e2m1_t>::value || std::is_same<T, fp4x2_e1m2_t>::value;
            if constexpr (isFp4Type) {
                constexpr const int KHALF = 2;
                load_cbuf_to_cb_s4(dst, src, mStartPosition, kStartPosition / KHALF, mStep, kStep / KHALF, srcStride, dstStride, transpose);
            } else {
                load_cbuf_to_cb(dst, src, mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride, transpose);
            }
        }
    }
};

class LoadCbufToCbB8Base {
public:
    template <const LoadDataTrait& trait, typename T, typename U, typename... Params>
    __aicore__ inline void LoadData(const T& dst, const U& src, const Params& ...params)
    {
        LoadCbufToCb<trait.transposed>(dst.Data().Get(), src.Data().Get(), params...);
    }

private:
    template <bool transpose, typename T>
    __aicore__ inline void LoadCbufToCb(__cb__ T* dst, __cbuf__ T* src, uint16_t mStartPosition,
        uint16_t kStartPosition, uint8_t mStep, uint8_t kStep, int16_t srcStride, uint16_t dstStride)
    {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            constexpr const int SHIFT_M_STEP_B8 = 1;
            constexpr const int M_STEP_MIN_VAL_B8 = 2;
            uint16_t nLoop = mStep >> SHIFT_M_STEP_B8;
            uint16_t dstAddrStride = kStep * FRACTAL_FIXED * C0_SIZE;
            mStep = M_STEP_MIN_VAL_B8;
            for (uint16_t idx = 0; idx < nLoop; ++idx) {
                load_cbuf_to_cb(dst, src, mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride, transpose);
                dst += dstAddrStride;
                mStartPosition += M_STEP_MIN_VAL_B8;
            }
        }
    }
};
} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_INSTRUCTION_H