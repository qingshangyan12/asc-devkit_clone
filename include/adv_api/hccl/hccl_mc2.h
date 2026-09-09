/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_MC2_H
#define HCCL_MC2_H

#include <hccl/hccl_types.h>
#include <hccl/hccl_res.h>
#include <ccu/ccu_types.h>
#include <cstdint>

struct Mc2CcArgs {
    static constexpr uint32_t GROUP_NAME_SIZE = 128U;
    static constexpr uint32_t ALG_CONFIG_SIZE = 128U;

    HcclDataType srcDataType;
    HcclDataType dstDataType;
    HcclReduceOp reduceType;
    char groupName[GROUP_NAME_SIZE];
    char algConfig[ALG_CONFIG_SIZE];
    CommEngine commEngine;
    uint64_t reverse;

    void Init()
    {
        srcDataType = HCCL_DATA_TYPE_FP16;
        dstDataType = HCCL_DATA_TYPE_FP16;
        reduceType = HCCL_REDUCE_SUM;
        groupName[0] = '\0';
    }
};

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
    uint32_t numBlocks;
    uint32_t reserve;
    uint32_t phyDieMask;
} HcommCcuSchd;

/**
 * @brief  Alloc Mc2CcArgs memory
 * @param ccArgs A pointer to the allocated Mc2CcArgs memory.
 */
extern HcclResult __attribute__((visibility("default"))) Mc2GetCcArgs(void** ccArgs);

/**
 * @brief  Free Mc2CcArgs memory
 * @param ccArgs A pointer to the Mc2CcArgs memory.
 */
extern HcclResult __attribute__((visibility("default"))) Mc2FreeCcArgs(void* ccArgs);

/**
 * @brief  Set the source data type param of Mc2CcArgs
 * @param ccArgs A pointer to the Mc2CcArgs.
 * @param srcDataType The source data type to set.
 */
extern HcclResult __attribute__((visibility("default"))) Mc2SetCcSrcDataType(void* ccArgs, uint8_t srcDataType);

/**
 * @brief  Set the destination data type param of Mc2CcArgs
 * @param ccArgs A pointer to the Mc2CcArgs.
 * @param dstDataType The destination data type to set.
 */
extern HcclResult __attribute__((visibility("default"))) Mc2SetCcDstDataType(void* ccArgs, uint8_t dstDataType);

/**
 * @brief  Set the reduce type param of Mc2CcArgs
 * @param ccArgs A pointer to the Mc2CcArgs.
 * @param reduceType The reduce type to set.
 */
extern HcclResult __attribute__((visibility("default"))) Mc2SetCcReduceType(void* ccArgs, uint32_t reduceType);

/**
 * @brief  Set the algConfig param of Mc2CcArgs
 * @param ccArgs A pointer to the Mc2CcArgs.
 * @param algConfig The algConfig to set.
 */
extern HcclResult __attribute__((visibility("default"))) Mc2SetCcAlgConfig(void* ccArgs, char* algConfig);

/**
 * @brief  Set the comm engine param of Mc2CcArgs
 * @param ccArgs A pointer to the Mc2CcArgs.
 * @param commEngine COMM_ENGINE_AICPU, COMM_ENGINE_AICPU_TS, COMM_ENGINE_AIV or COMM_ENGINE_CCU.
 */
extern HcclResult __attribute__((visibility("default"))) Mc2SetCcCommEngine(void* ccArgs, uint8_t commEngine);

/**
 * @brief Set the group name of Mc2CcArgs.
 * @param ccArgs A pointer to the Mc2CcArgs.
 * @param groupName The group name to set.
 */
extern HcclResult __attribute__((visibility("default"))) Mc2SetCcGroupName(void* ccArgs, char* groupName);

/**
 * @brief Acquire AICPU communication resources.
 * @param comm The communication domain.
 * @param ccType The HcclCMDType communication operation.
 * @param ccArgs The communication arguments with algConfig and commEngine set.
 * @param ccResCtx The output device address of OpResCtx, owned by comm.
 * @param ccResCtxSize The output size of OpResCtx in bytes.
 * @param schd Optional HcommCcuSchd output; zeroed for AICPU.
 */
extern HcclResult __attribute__((visibility("default"))) Mc2AcquireCcResCtx(
    HcclComm comm, uint8_t ccType, void* ccArgs, void** ccResCtx, uint32_t* ccResCtxSize, void* schd);

/**
 * @brief Allocate communication resource by MC2 Tiling data
 * @param comm A pointer identifying the communication resource based on.
 * @param stream A pointer identifying the stream information.
 * @param mc2Tiling MC2 tiling data structure containing algorithm configuration.
 * @param opResCtx Output pointer to store the created communication context.
 * @return HcclResult - HCCL_SUCCESS on success, error code otherwise.
 *
 * @note This interface is used for MC2 compilation scenario to pre-allocate
 *       communication resources based on tiling information.
 */
extern HcclResult __attribute__((visibility("default"))) HcclAllocComResourceByTiling(
    HcclComm comm, void* stream, void* mc2Tiling, void** opResCtx);

/**
 * @brief Check whether communication resource can be allocated for the given MC2 tiling
 * @param comm A pointer identifying the communication resource based on.
 * @param stream A pointer identifying the stream information.
 * @param mc2Tiling MC2 tiling data structure containing algorithm configuration.
 * @return HcclResult - HCCL_SUCCESS on success, error code otherwise.
 *
 * @note CheckOnly pre-check: reuses the same argument parsing / algorithm selection / resource
 *       calculation / resource acquisition logic as the real allocation path to probe whether
 *       resources are sufficient; HCCL_E_UNAVAIL is translated to HCCL_E_RES_NOT_SUFFICIENT,
 *       and the final OpResCtx is not persisted to device.
 */
extern HcclResult __attribute__((visibility("default"))) CheckOpResSufficient(
    HcclComm comm, void* stream, void* mc2Tiling);

extern CcuResult CcuKernelLaunch(HcclComm comm, void* opResCtx);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HCCL_MC2_H
