/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file be in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_ST_SCENARIO_H
#define CCU_ST_SCENARIO_H

#include <cstdint>
#include <string>
#include <vector>
#include "hccl_types.h"
#include "hccl_common.h"

namespace HcclSim {
namespace CcuSt {

struct CcuStScenario {
    uint32_t rankSize = 2;
    HcclDataType dataType = HCCL_DATA_TYPE_FP16;
    HcclCMDType opType = HcclCMDType::HCCL_CMD_ALLTOALLV;

    // Expected algorithm name selected by selector (verified in VerifyScenario)
    // Empty string means skip algName assertion
    std::string expectedAlgName;

    // Force a specific algorithm via tiling.algConfig (overrides selector)
    // Empty string means use "default" (let selector choose)
    std::string algConfig;

    // sizes[sourceRank][destinationRank] = sendSize (bytes)
    // AllToAllV/AllToAll use this matrix; AllToAll (equal-length) has identical rows
    std::vector<std::vector<uint64_t>> sizes;

    // Per-rank element count for equal-length collectives (AllGather/ReduceScatter)
    // dataSize = count * DataTypeSizeGet(dataType)
    uint64_t count = 0;

    // Reduce operation type (ReduceScatter/AllReduce)
    HcclReduceOp reduceType = HCCL_REDUCE_SUM;

    // Output data type for low-precision reduce (e.g. FP16 input → FP32 output)
    // HCCL_DATA_TYPE_RESERVED means same as dataType
    HcclDataType outputType = HCCL_DATA_TYPE_RESERVED;

    // negative test controls
    bool corruptOutput = false;
    bool swapTaskArgs = false;
    bool missingTaskArg = false;
    bool outOfRangeParam = false;
};

} // namespace CcuSt
} // namespace HcclSim

#endif // CCU_ST_SCENARIO_H
