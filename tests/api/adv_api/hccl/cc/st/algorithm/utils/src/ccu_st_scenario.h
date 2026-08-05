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
#include <vector>
#include "hccl_types.h"
#include "hccl_common.h"

namespace HcclSim {
namespace CcuSt {

struct CcuStScenario {
    uint32_t rankSize = 2;
    HcclDataType dataType = HCCL_DATA_TYPE_FP16;
    HcclCMDType opType = HcclCMDType::HCCL_CMD_ALLTOALLV;

    // sizes[sourceRank][destinationRank] = sendSize (bytes)
    // For AllToAll (equal-length), every row must be identical
    std::vector<std::vector<uint64_t>> sizes;

    // negative test controls
    bool corruptOutput = false;
    bool swapTaskArgs = false;
    bool missingTaskArg = false;
    bool outOfRangeParam = false;
};

} // namespace CcuSt
} // namespace HcclSim

#endif // CCU_ST_SCENARIO_H
