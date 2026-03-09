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
#include "kernel_tensor.h"

namespace AscendC {

__aicore__ inline void ReportNotSupport(bool isSupported, const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((isSupported), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "%s is not supported on current device.", 
        apiName));
}

// TODO: 要Tposition
__aicore__ inline void CheckTensorPosition(bool isSupported, const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((isSupported), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "%s is not supported on current device.", 
        apiName));
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
__aicore__ inline void CheckValueRange(uint64_t value, uint64_t valueLow, uint64_t valueHigh, const __gm__ char* tensorName,
     const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((value <= valueHigh && value >= valueLow), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check "
            "%s value in %s, its valid range is [%]" 
        apiName));
}

__aicore__ inline void CheckValueRange(uint64_t value, uint64_t valueLow, uint64_t valueHigh, const __gm__ char* tensorName,
     const __gm__ char* apiName)
{
    ASCENDC_DEBUG_ASSERT((value <= valueHigh && value >= valueLow), KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check "
            "%s value in %s, its valid range is [%]" 
        apiName));
}


} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_KERNEL_NPU_DEBUG_H