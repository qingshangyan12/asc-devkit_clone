/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_OPS_HCCL_CANN_HOST_BRIDGE_H
#define MC2_OPS_HCCL_CANN_HOST_BRIDGE_H

#include <cstdint>
#include <memory>
#include <string>
#include "alg_param.h"
#include "hccl.h"

namespace mc2_ops_hccl {

class InsCollAlgBase;
struct AlgResourceCtxSerializable;

// 判断该 op 是否需要走 CANN 桥接路径(选路/取执行器/资源准备)。
// allreduce-test: 仅 ALLREDUCE(AICPU/AICPU_TS) 走 CANN 桥接；ALLTOALL/ALLTOALLV 本地。
bool UseCannBridge(const OpParam& param);

// allreduce-test: 判断 algName 是否 devkit 本地注册——forced-alg 场景的归属分流，
// 本地注册的算法名（即使 opType 在桥接列表中）仍走本地路径，避免被拦进 CANN 查表而
// 触发 fallback / CANN 侧查无此名导致执行失败。
bool IsLocalRegisteredAlg(HcclCMDType opType, const std::string& algName);

HcclResult SelectViaCann(OpParam& ascParam, TopoInfoWithNetLayerDetails* ascTopo, std::string& algName);

std::unique_ptr<InsCollAlgBase> GetAlgExecViaCann(HcclCMDType opType, const std::string& algName);

} // namespace mc2_ops_hccl

#endif // MC2_OPS_HCCL_CANN_HOST_BRIDGE_H
