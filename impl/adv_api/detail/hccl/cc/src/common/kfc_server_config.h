/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_KFC_SERVER_CONFIG_H
#define HCCL_KFC_SERVER_CONFIG_H

#include <vector>
#include "alg_param.h"

namespace mc2_ops_hccl {

struct KfcServerOpConfig {
    std::vector<uint64_t> opIds;
    std::vector<OpParam> opParams;
};

inline thread_local KfcServerOpConfig g_kfcServerOpConfig;

inline uint64_t BuildKfcOpId(const OpParam& param)
{
    constexpr uint64_t fieldMask = 0x7f;
    constexpr uint32_t dataTypeShift = 8;
    constexpr uint32_t reduceTypeShift = 16;
    constexpr uint32_t outputDataTypeShift = 24;
    const bool isAllToAll = param.opType == HcclCMDType::HCCL_CMD_ALLTOALL ||
                            param.opType == HcclCMDType::HCCL_CMD_ALLTOALLV;
    const HcclDataType dataType = isAllToAll ? param.all2AllVDataDes.sendType : param.DataDes.dataType;
    const HcclDataType outputDataType = isAllToAll ? param.all2AllVDataDes.recvType : param.DataDes.outputType;
    const uint64_t reduceType = param.opType == HcclCMDType::HCCL_CMD_ALLREDUCE ?
                                    static_cast<uint64_t>(param.reduceType) : 0U;
    return ((static_cast<uint64_t>(outputDataType) & fieldMask) << outputDataTypeShift) |
           ((reduceType & fieldMask) << reduceTypeShift) |
           ((static_cast<uint64_t>(dataType) & fieldMask) << dataTypeShift) |
           (static_cast<uint64_t>(param.opType) & fieldMask);
}

inline void AddKfcServerOp(KfcServerOpConfig& config, const OpParam& param)
{
    const uint64_t opId = BuildKfcOpId(param);
    for (uint64_t configuredOpId : config.opIds) {
        if (configuredOpId == opId) {
            return;
        }
    }
    config.opIds.push_back(opId);
    config.opParams.push_back(param);
}

inline void ConfigureKfcServerOps(const std::vector<OpParam>& opParams)
{
    g_kfcServerOpConfig = KfcServerOpConfig{};
    for (const OpParam& param : opParams) {
        AddKfcServerOp(g_kfcServerOpConfig, param);
    }
}

inline uint64_t BuildKfcServerCacheHash(const char* algTag, const std::vector<OpParam>& opParams)
{
    constexpr uint64_t fnvOffsetBasis = 14695981039346656037ULL;
    constexpr uint64_t fnvPrime = 1099511628211ULL;
    uint64_t hash = fnvOffsetBasis;
    for (const char* cursor = algTag; cursor != nullptr && *cursor != '\0'; ++cursor) {
        hash = (hash ^ static_cast<uint8_t>(*cursor)) * fnvPrime;
    }
    for (const OpParam& param : opParams) {
        uint64_t opId = BuildKfcOpId(param);
        for (uint32_t byte = 0; byte < sizeof(opId); ++byte) {
            hash = (hash ^ static_cast<uint8_t>(opId >> (byte * 8U))) * fnvPrime;
        }
    }
    return hash;
}

} // namespace mc2_ops_hccl
#endif
