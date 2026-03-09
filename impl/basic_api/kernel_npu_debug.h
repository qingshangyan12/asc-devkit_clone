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

#include "impl/basic_api/kernel_log.h"
#include "impl/basic_api/utils/kernel_utils_ceil_oom_que.h"
#include "kernel_tensor.h"

namespace AscendC {

__aicore__ inline void ReportNotSupport(bool isSupported, const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((isSupported), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "%s is not supported on current device.", 
        apiName));
}

template<typename T>
__aicore__ inline void CheckTensorTPosition(const LocalTensor<T>& inputTensor, TPosition expectedPos[], uint32_t posNum,
    const __gm__ char* tensorName,  const __gm__ char* posName, const __gm__ char* apiName)
{
    bool tPosValid = false;
    TPosition currentPos = static_cast<TPosition>(inputTensor.GetPosition());
    for (uint32_t i = 0; i < posNum; i++) {
        if (currentPos == expectedPos[i]) {
            tPosValid = true;
            break;
        }
    }
    const __gm__ char* curPosName = "GM";
    if (currentPos == TPosition::A1) {
        curPosName = "A1";
    } else if (currentPos == TPosition::B1) {
        curPosName = "B1";
    } else if (currentPos == TPosition::C1) {
        curPosName = "C1";
    } else if (currentPos == TPosition::A2) {
        curPosName = "A2";
    } else if (currentPos == TPosition::B2) {
        curPosName = "B2";
    } else if (currentPos == TPosition::C2) {
        curPosName = "C2";
    } else if (currentPos == TPosition::CO1) {
        curPosName = "CO1";
    } else if (currentPos == TPosition::CO2) {
        curPosName = "CO2";
    } else if (currentPos == TPosition::VECIN) {
        curPosName = "VECIN";
    } else if (currentPos == TPosition::VECOUT) {
        curPosName = "VECOUT";
    } else if (currentPos == TPosition::VECCALC) {
        curPosName = "VECCALC";
    } else if (currentPos == TPosition::LCM) {
        curPosName = "LCM";
    } else if (currentPos == TPosition::SPM) {
        curPosName = "SPM";
    } else if (currentPos == TPosition::SHM) {
        curPosName = "SHM";
    } else if (currentPos == TPosition::TSCM) {
        curPosName = "TSCM";
    } else if (currentPos == TPosition::C2PIPE2GM) {
        curPosName = "C2PIPE2GM";
    } else if (currentPos == TPosition::C2PIPE2LOCAL) {
        curPosName = "C2PIPE2LOCAL";
    } else if (currentPos == TPosition::MAX) {
        curPosName = "MAX";
    }
    ASCENDC_DEBUG_ASSERT((tPosValid), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check %s tensor TPosition in %s, "
        "supported TPositions are %s, current TPosition is %s.", tensorName, apiName, posName, curPosName));
}

template<typename T>
__aicore__ inline void CheckTensorAlignment(const LocalTensor<T>& inputTensor, uint32_t alignBytes, 
    const __gm__ char* tensorName,  const __gm__ char* apiName)
{   
    uint64_t tensorAddr = inputTensor.GetPhyAddr();
#ifdef ASCENDC_CPU_DEBUG
    auto positionHardMap = ConstDefiner::Instance().positionHardMap;
    int32_t tensorPos = inputTensor.GetPosition();
    tensorAddr = tensorAddr - 
        reinterpret_cast<uint64_t>(ConstDefiner::Instance().hardwareCpuBufferMap.at(positionHardMap.at(tensorPos)));
#endif
    ASCENDC_DEBUG_ASSERT((tensorAddr % alignBytes == 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check %s start "
        "address alignment in %s, its start address must align with %uB, current start address is %u.", tensorName, 
        apiName, alignBytes, tensorAddr));
}

// 写一个int64_t的版本作为通用，因为有api是有负数范围的。uint64_t的也要留着
template<typename T>
__aicore__ inline void CheckValueRange(T value, T valueLow, T valueHigh, const __gm__ char* paramName,
    const __gm__ char* apiName)
{
    if constexpr(SupportType<T, int8_t, int16_t, int32_t, int64_t>()) {
        int64_t castValue = static_cast<int64_t>(value);
        ASCENDC_DEBUG_ASSERT((value <= valueHigh && value >= valueLow), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
            "check %s value in %s, its valid range is [%lld, %lld], current value is %lld", paramName, apiName, 
            valueLow, valueHigh, value));
    } else if constexpr(SupportType<T, uint8_t, uint16_t, uint32_t, uint64_t>()) {
        uint64_t castValue = static_cast<uint64_t>(value);
        ASCENDC_DEBUG_ASSERT((value <= valueHigh && value >= valueLow), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to "
            "check %s value in %s, its valid range is [%llu, %llu], current value is %llu", paramName, apiName, 
            valueLow, valueHigh, value));
    } else {
        ASCENDC_DEBUG_ASSERT((false), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Current dtype is not supported for check "
            "function CheckValueRange."));
    }
}

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_KERNEL_NPU_DEBUG_H