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

// host 侧算法选择桥: 经 dlopen libhccl.so 调 CANN ops_hccl::ExecuteSelector::Run,
// 选出算法名并回写 param(engine/opExecuteConfig 等)。
HcclResult SelectViaCann(OpParam &ascParam, TopoInfoWithNetLayerDetails *ascTopo, std::string &algName);

// host 侧 executor 桥: 经 dlopen libhccl.so 调 CANN ops_hccl::CollAlgExecRegistryV2::GetAlgExec
// 取真正的 host 侧 executor。三算子(ALLTOALL/ALLTOALLV/ALLREDUCE)的本地 V2 executor 已被 A5 删除,
// 资源申请(HcclGetAlgRes 内首次 CalcAlgHierarchyInfo/CalcRes)需用 libhccl 里真正的 executor。
// 取不到返回 nullptr, 由调用方报错。
std::unique_ptr<InsCollAlgBase> GetAlgExecViaCann(HcclCMDType opType, const std::string &algName);

// host 侧资源申请桥: 经 dlopen libhccl.so 调 CANN ops_hccl::HcclGetAlgRes 算资源。
// 三算子的 executor 来自 GetAlgExecViaCann(libhccl 侧), 故 executor 生成 + CalcRes + resCtx
// 全在 libhccl 内完结, vtable/dtor 不跨 ABI。符号未就绪时返回 HCCL_E_NOT_SUPPORT, 调用方回退本地。
HcclResult GetAlgResViaCann(HcclComm comm, OpParam &param, std::unique_ptr<InsCollAlgBase> &executor,
    TopoInfoWithNetLayerDetails *topoInfo, std::unique_ptr<AlgResourceCtxSerializable> &resCtxHost,
    void **resCtxSequence, bool &isResourceReused);

}  // namespace mc2_ops_hccl

#endif  // MC2_OPS_HCCL_CANN_HOST_BRIDGE_H
