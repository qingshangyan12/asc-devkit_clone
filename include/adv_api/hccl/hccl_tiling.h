/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file hccl_tiling.h
 * \brief
 */
#ifndef LIB_HCCL_HCCL_TILING_H
#define LIB_HCCL_HCCL_TILING_H

#include "internal/hcomm/hccl/hccl_types.h"
#include "internal/hcomm/ccu/ccu_types.h"

#include "hccl_tilingdata.h"
#include "kernel_tiling/kernel_tiling.h"
#include "../../../impl/adv_api/tiling/hccl/hccl_tiling_impl_def.h"

namespace AscendC {

class Mc2CcTilingConfig {
public:
    explicit Mc2CcTilingConfig(
        const std::string& groupName, uint32_t opType, const std::string& algConfig, uint32_t reduceType = 0,
        uint8_t dstDataType = 0, uint8_t srcDataType = 0, uint8_t commEngine = 0);
    virtual ~Mc2CcTilingConfig();

public:
    uint32_t GetTiling(::Mc2InitTiling& tiling);
    uint32_t GetTiling(::Mc2CcTiling& tiling);

public:
    uint32_t SetOpType(uint32_t opType);
    uint32_t SetGroupName(const std::string& groupName);
    uint32_t SetAlgConfig(const std::string& algConfig);
    uint32_t SetReduceType(uint32_t reduceType, uint8_t dstDataType = 0, uint8_t srcDataType = 0);
    uint32_t SetStepSize(uint8_t stepSize);
    uint32_t SetSkipLocalRankCopy(uint8_t skipLocalRankCopy);
    uint32_t SetSkipBufferWindowCopy(uint8_t skipBufferWindowCopy);
    uint32_t SetDebugMode(uint8_t debugMode);
    uint32_t SetQueueNum(uint16_t num);
    uint32_t SetCommBlockNum(uint16_t num);
    uint32_t SetCommEngine(uint8_t commEngine);

private:
    HcclTilingImpl impl_;
};

// 获取相关资源：

// enum class CcType : uint8_t {
//     CC_TYPE_ALLGATHER = 0,
//     CC_TYPE_ALLREDUCE = 1,
// };

using Mc2Result = HcclResult;

Mc2Result Mc2GetCcArgsCcu(void** ccArgs);
Mc2Result Mc2FreeCcArgsCcu(void* ccArgs);
Mc2Result Mc2FreeCcResCtxCcu(void* ccResCtx);
Mc2Result Mc2SetCcCommEngineCcu(void* ccArgs, uint8_t commEngine);
Mc2Result Mc2SetCcAlgConfigCcu(void* ccArgs, char* algConfig);
Mc2Result Mc2SetCcSrcDataTypeCcu(void* ccArgs, uint8_t srcDataType);
Mc2Result Mc2SetCcDstDataTypeCcu(void* ccArgs, uint8_t dstDataType);
Mc2Result Mc2SetCcReduceTypeCcu(void* ccArgs, uint8_t reduceDataType);

// 本接口功能 和 HcclAllocComResourceByTiling 类似，根据comm ccargs获取 ccResCtx
Mc2Result Mc2AcquireCcResCtxCcu(
    HcclComm comm, uint8_t ccType, void* ccArgs, void** ccResCtx, uint32_t* ccResCtxSize, void** schd);

// ccu侧 的算子接口
// #ifndef __ccu_host__
// #define __ccu_host__
// #endif

extern __ccu_host__ void Mc2CcuCcKernel(void* ccResCtx);
// #else
// extern void Mc2CcuCcKernel(void* ccResCtx);
// #endif

} // namespace AscendC
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_HCCL_TILING_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_HCCL_TILING_H__
#endif
