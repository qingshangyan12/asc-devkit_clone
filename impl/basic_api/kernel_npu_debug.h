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
 * \file kernel_npu_debug.h
 * \brief
 */
#ifndef ASCENDC_MODULE_OPERATOR_KERNEL_NPU_DEBUG_H
#define ASCENDC_MODULE_OPERATOR_KERNEL_NPU_DEBUG_H

#include "kernel_log.h"
#include "utils/kernel_utils_ceil_oom_que.h"
#include "utils/kernel_utils_constants.h"
#include "kernel_struct_fixpipe.h"
#include "kernel_tensor.h"
#include "kernel_struct_mm.h"

namespace AscendC {
template <typename T>
struct NamedTensor {
    __aicore__ NamedTensor(const LocalTensor<T>& tensorIn, const __gm__ char* tensorNameIn)
        : tensor(tensorIn), tensorName(tensorNameIn) {}

    LocalTensor<T> tensor;
    const __gm__ char* tensorName;
};

__aicore__ inline __gm__ const char* GetTPositionName(TPosition pos)
{
    switch (pos) {
        case TPosition::GM:
            return "GM";
        case TPosition::A1:
            return "A1";
        case TPosition::B1:
            return "B1";
        case TPosition::C1:
            return "C1";
        case TPosition::A2:
            return "A2";
        case TPosition::B2:
            return "B2";
        case TPosition::C2:
            return "C2";
        case TPosition::CO1:
            return "CO1";
        case TPosition::CO2:
            return "CO2";
        case TPosition::VECIN:
            return "VECIN";
        case TPosition::VECOUT:
            return "VECOUT";
        case TPosition::VECCALC:
            return "VECCALC / LCM";
        case TPosition::SPM:
            return "SPM / SHM";
        case TPosition::TSCM:
            return "TSCM";
        case TPosition::C2PIPE2GM:
            return "C2PIPE2GM";
        case TPosition::C2PIPE2LOCAL:
            return "C2PIPE2LOCAL";
        case TPosition::MAX:
            return "MAX";
        default:
            return "UNKNOWN";
    }
}

__aicore__ inline void ReportNotSupport(bool isSupported, const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((isSupported), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "%s is not supported on current device.\n",
        apiName));
}

template <TPosition... expectedPos, typename T>
__aicore__ inline void CheckTensorTPosition(const LocalTensor<T>& inputTensor, const __gm__ char* tensorName,
    const __gm__ char* posName, const __gm__ char* apiName)
{
    TPosition currentPos = static_cast<TPosition>(inputTensor.GetPosition());
    bool tPosValid = ((currentPos == expectedPos) || ...);
    ASCENDC_DEBUG_ASSERT((tPosValid), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check %s tensor TPosition in %s, "
        "supported TPositions are %s, current TPosition is %s.\n", tensorName, apiName, posName,
        GetTPositionName(currentPos)));
}

template <typename T>
__aicore__ inline void CheckTensorAlignment(const LocalTensor<T>& inputTensor, uint32_t alignBytes,
    const __gm__ char* tensorName,  const __gm__ char* apiName)
{
    uint64_t tensorAddr = reinterpret_cast<uint64_t>(inputTensor.GetPhyAddr());
#ifdef ASCENDC_CPU_DEBUG
    auto positionHardMap = ConstDefiner::Instance().positionHardMap;
    TPosition tensorPos = static_cast<TPosition>(inputTensor.GetPosition());
    tensorAddr = tensorAddr -
        reinterpret_cast<uint64_t>(ConstDefiner::Instance().hardwareCpuBufferMap.at(positionHardMap.at(tensorPos)));
#endif
    ASCENDC_DEBUG_ASSERT((tensorAddr % alignBytes == 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check %s start "
        "address alignment in %s, its start address must align with %u bytes, current start address is %llu.\n",
        tensorName, apiName, alignBytes, static_cast<unsigned long long>(tensorAddr)));
}

template <typename T>
__aicore__ inline void CheckValueRange(T value, T valueLow, T valueHigh, const __gm__ char* paramName,
    const __gm__ char* apiName)
{
    if constexpr (SupportType<T, int8_t, int16_t, int32_t, int64_t>()) {
        int64_t castValue = static_cast<int64_t>(value);
        int64_t castLow = static_cast<int64_t>(valueLow);
        int64_t castHigh = static_cast<int64_t>(valueHigh);
        ASCENDC_DEBUG_ASSERT((value <= valueHigh && value >= valueLow), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
            "check %s value in %s, its valid range is [%lld, %lld], current value is %lld.\n", paramName, apiName,
            castLow, castHigh, castValue));
    } else if constexpr (SupportType<T, uint8_t, uint16_t, uint32_t, uint64_t>()) {
        uint64_t castValue = static_cast<uint64_t>(value);
        uint64_t castLow = static_cast<uint64_t>(valueLow);
        uint64_t castHigh = static_cast<uint64_t>(valueHigh);
        ASCENDC_DEBUG_ASSERT((value <= valueHigh && value >= valueLow), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
            "check %s value in %s, its valid range is [%llu, %llu], current value is %llu.\n", paramName, apiName,
            castLow, castHigh, castValue));
    } else {
        ASCENDC_DEBUG_ASSERT((false), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Current dtype is not supported for check "
            "function CheckValueRange.\n"));
    }
}

template <typename T>
__aicore__ inline void CheckMaskArray(const uint64_t mask[], const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((mask != nullptr), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check mask[] in %s, "
        "it is nullptr.\n", apiName));
    if constexpr (sizeof(T) == 2) {
        ASCENDC_DEBUG_ASSERT(!(mask[0] == 0 && mask[1] == 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check "
            "mask[] value in %s, when sizeof(T) == 2, mask[0] and mask[1] cannot be both 0.\n", apiName));
    } else if constexpr (sizeof(T) == 4) {
        ASCENDC_DEBUG_ASSERT((mask[0] != 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check mask[] value in %s, "
            "when sizeof(T) == 4, mask[0] cannot be 0.\n", apiName));
    } else if constexpr (sizeof(T) == 8) {
        ASCENDC_DEBUG_ASSERT((mask[0] != 0 && mask[0] <= UINT32_MAX), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
            "check mask[] value in %s, when sizeof(T) == 8, mask[0] should be in range (0, UINT32_MAX], current value "
            "is %llu.\n", apiName, mask[0]));
    }
}

template <typename T>
__aicore__ inline void CheckMaskValue(const uint64_t mask, const __gm__ char* apiName)
{
    if constexpr (sizeof(T) == 2) {
        CheckValueRange<uint64_t>(mask, 1, ONE_REPEAT_BYTE_SIZE / sizeof(T), "mask when sizeof(T) == 2" , apiName);
    } else if constexpr (sizeof(T) == 4) {
        CheckValueRange<uint64_t>(mask, 1, ONE_REPEAT_BYTE_SIZE / sizeof(T), "mask when sizeof(T) == 4" , apiName);
    } else if constexpr (sizeof(T) == 8) {
        CheckValueRange<uint64_t>(mask, 1, ONE_REPEAT_BYTE_SIZE / sizeof(T), "mask when sizeof(T) == 8" , apiName);
    }
}

template <typename... Ts>
__aicore__ inline void CheckVectorTensor(const __gm__ char* apiName, const Ts& ...tensors)
{
    (CheckTensorTPosition<TPosition::VECIN, TPosition::VECCALC, TPosition::VECOUT>(tensors.tensor, tensors.tensorName,
        "VECIN / VECCALC / VECOUT", apiName), ...);
    (CheckTensorAlignment(tensors.tensor, ONE_BLK_SIZE, tensors.tensorName, apiName), ...);
}

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_KERNEL_NPU_DEBUG_H
