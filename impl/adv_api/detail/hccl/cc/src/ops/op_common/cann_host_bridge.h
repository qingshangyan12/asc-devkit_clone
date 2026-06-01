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
#include <string>
#include "alg_param.h"

namespace mc2_ops_hccl {

// host 侧算法选择桥: 经 dlopen libhccl.so 调 CANN ops_hccl::ExecuteSelector::Run,
// 选出算法名并回写 param(engine/opExecuteConfig 等)。
HcclResult SelectViaCann(OpParam &ascParam, TopoInfoWithNetLayerDetails *ascTopo, std::string &algName);

// // host 侧 resCtx 生成桥: 经 dlopen libhccl.so 调 CANN ops_hccl::HcclGetAlgRes,
// // 产出 CANN 布局 resCtx 并由 CANN 内部 memcpy 到 device, 取回 device ctx 句柄。
// HcclResult GetAlgResViaCann(HcclComm comm, OpParam &ascParam, TopoInfoWithNetLayerDetails *ascTopo,
//                             const std::string &algName, void **resCtxSequence, u64 &ctxSize);

}  // namespace mc2_ops_hccl

#endif  // MC2_OPS_HCCL_CANN_HOST_BRIDGE_H
