/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

// cann_host_resctx_bridge.h
//
// 本阶段目标: 打通链路。host 侧三算子(ALLTOALL/ALLTOALLV/ALLREDUCE, engine=AICPU)
// 在"算资源 + 产 resCtx"这一步, 经 dlopen libhccl.so 调 CANN 的
// ops_hccl::HcclGetAlgRes, 产出 CANN 布局 resCtx, 与 device 侧 LaunchViaCann
// (dlopen libscatter_aicpu_kernel.so) 端到端对齐。
//
// ABI 现状(machine-checked, 2026-06):
//  - asc 的 OpParam / AlgResourceCtxSerializable 已与 CANN 对齐(OpParam 末尾
//    DevAicpuOpConfig opConfig; resCtx 含 isHcommBatchTransferOnThreadSupported)。
//    → 直接透传 asc 对象, 无需镜像。
//  - asc 的 TopoInfoWithNetLayerDetails 比 CANN 多一个 bool Level1Hd(在 Level1Nhr
//    之后, 由 CalcLevel1Hd 填充, 是 asc 真实功能字段, 不能删), 导致其后字段整体偏移。
//    而 CANN GetAlgResAICPU 会 resCtxHost->topoInfo = *topoInfo 整体值拷贝, 故 topoInfo
//    必须按 CANN 布局传入 → 这里手搓 cann_abi::TopoInfoWithNetLayerDetails 镜像 +
//    字段搬运(跳过 Level1Hd)。
//  - comm(HcclComm/void*)在 CANN HcclGetAlgRes 全链当不透明 C API 句柄使用
//    (HcclGetRankSize/HcclGetHcclBuffer/HcclEngineCtxGet(comm,...)), 这些 C API 在 asc
//    进程由 asc 强符号实现, dlopen libhccl 不抢占 → 直接透传 asc comm 指针。
//  - executor 需 CANN 版: HcclGetAlgRes 内部用它调 CalcAlgHierarchyInfo/CalcRes,
//    故 dlsym CANN CollAlgExecRegistryV2::GetAlgExec(sret ABI)取 CANN executor。
//
// 嵌套类型(Level0Shape/NetLayerDetails/TopoInstDetails/Level0MeshType)两 fork 逐字一致,
// 镜像直接复用 mc2_ops_hccl 同名类型。
#ifndef MC2_OPS_HCCL_CANN_HOST_RESCTX_BRIDGE_H
#define MC2_OPS_HCCL_CANN_HOST_RESCTX_BRIDGE_H

#include <cstdint>
#include "alg_param.h"

namespace mc2_ops_hccl {

// host 侧 resCtx 生成桥: 经 dlopen libhccl.so 调 CANN ops_hccl::HcclGetAlgRes,
// 产出 CANN 布局 resCtx 并由 CANN 内部 memcpy 到 device, 取回 device ctx 句柄。
// 入参:
//   comm        - asc 的 HcclComm(透传)
//   ascParam    - asc OpParam(已与 CANN 布局对齐, 透传; 会被回写 ctxSize 等)
//   ascTopo     - asc topoInfo(内部转成 CANN 布局后传给 CANN)
//   algName     - host 侧 Selector 已选出的算法名
//   resCtxSequence(out) - device ctx 句柄(指向 CANN 序列化后下发的 device blob)
//   ctxSize(out)        - device ctx 字节数
// 返回 HCCL_SUCCESS 表示已成功经 CANN 产 resCtx; 其它表示失败(调用方应回退 asc 路径)。
HcclResult GetAlgResViaCann(HcclComm comm, OpParam &ascParam, TopoInfoWithNetLayerDetails *ascTopo,
                            const std::string &algName, void **resCtxSequence, u64 &ctxSize);

}  // namespace mc2_ops_hccl

#endif  // MC2_OPS_HCCL_CANN_HOST_RESCTX_BRIDGE_H
