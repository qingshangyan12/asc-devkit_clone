/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef AICPU_ST_SCENARIO_H
#define AICPU_ST_SCENARIO_H

#include <cstdint>
#include <string>

#include "hccl_types.h"
#include "hccl_common.h"
#include "sim_common.h"

namespace HcclSim {
namespace AicpuSt {

struct AicpuStScenario {
    TopoMeta topoMeta; // rankSize 从 topoMeta 自动推导
    DevType devType = DevType::DEV_TYPE_950;
    HcclDataType dataType = HCCL_DATA_TYPE_FP16;
    HcclCMDType opType = HcclCMDType::HCCL_CMD_ALLGATHER;

    // 期望 selector 选出的算法名（空=跳过断言；与 golden 选择解耦，golden 按实际选中算法键控）
    std::string expectedAlgName;
    // 经 tiling.algConfig 强制指定算法（空=自动选择，等价 "default"）
    std::string algConfig;

    // per-rank 元素数（AllGather 语义）
    uint64_t count = 0;

    // Reduce 系算子的归约类型（AllReduce/ReduceScatter 用；AllGather 忽略）
    HcclReduceOp reduceType = HcclReduceOp::HCCL_REDUCE_SUM;
};

} // namespace AicpuSt
} // namespace HcclSim

#endif // AICPU_ST_SCENARIO_H
