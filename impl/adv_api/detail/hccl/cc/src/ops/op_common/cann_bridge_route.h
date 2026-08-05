/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_OPS_HCCL_CANN_BRIDGE_ROUTE_H
#define MC2_OPS_HCCL_CANN_BRIDGE_ROUTE_H

#include "alg_param.h"

namespace mc2_ops_hccl {

inline bool UseCannBridgeForAicpuOp(CommEngine engine, HcclCMDType opType)
{
    const bool isAicpu = engine == COMM_ENGINE_AICPU_TS || engine == COMM_ENGINE_AICPU;
    return isAicpu && opType == HcclCMDType::HCCL_CMD_ALLREDUCE;
}

} // namespace mc2_ops_hccl
#endif
