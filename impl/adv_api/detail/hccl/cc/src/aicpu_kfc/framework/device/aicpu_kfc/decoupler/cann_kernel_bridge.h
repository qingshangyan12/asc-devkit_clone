/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

// cann_kernel_bridge.h
//
// 本阶段目标: 打通链路。通过 dlopen CANN 的 libscatter_aicpu_kernel.so，借用其
// ops_hccl::CollAlgExecRegistryV2::GetAlgExec + 返回 executor 的 Orchestrate，
// 使 alltoall 场景由 CANN 内部派发到 ops_hccl::InsTempAlltoAllVMesh1D::KernelRun。
//
// 由于 CANN 包未安装内部头，这里手写到 Orchestrate 为止的虚函数槽位一致的抽象基类
// (cann_abi 命名空间)。OpParam / AlgResourceCtxSerializable 的布局已在 asc alg_param.h
// 中与 CANN 对齐(asc 末尾的 DevAicpuOpConfig opConfig 与 resCtx 的
// isHcommBatchTransferOnThreadSupported 即为对齐字段)，故直接复用 mc2_ops_hccl 的类型，
// 不再维护镜像副本。
#ifndef MC2_OPS_HCCL_CANN_KERNEL_BRIDGE_H
#define MC2_OPS_HCCL_CANN_KERNEL_BRIDGE_H

#include <string>
#include "alg_param.h"

namespace mc2_ops_hccl {
namespace cann_abi {

// 与 CANN ops_hccl::InsCollAlgBase 虚函数槽位一致的抽象基类。
// 仅需到 Orchestrate 为止的槽位顺序正确(dtor, Describe, CalcAlgHierarchyInfo,
// CalcRes, Orchestrate)，即可通过 vtable 正确派发 Orchestrate。后续虚函数无关。
// 参数类型只需 ABI 兼容(引用=指针)，签名细节不影响 vtable 槽位。
// OpParam / AlgResourceCtxSerializable 解析为 mc2_ops_hccl 的同名类型，其布局已与 CANN 对齐。
class InsCollAlgBase {
public:
    virtual ~InsCollAlgBase() {}
    virtual std::string Describe() const = 0;                       // slot: Describe
    virtual int CalcAlgHierarchyInfo(void*, void*, void*) = 0;      // slot: CalcAlgHierarchyInfo
    virtual int CalcRes(void*, const void*, const void*, const void*, void*) = 0; // slot: CalcRes
    virtual HcclResult Orchestrate(const OpParam &param,
                                   const AlgResourceCtxSerializable &resCtx) = 0; // slot: Orchestrate
};

}  // namespace cann_abi

// 旁路入口: 经 dlopen 调 CANN 的 GetAlgExec + Orchestrate。
// 入参为 asc 布局的 OpParam / AlgResourceCtxSerializable(调用方已 DeSerialize)，
// 其布局已与 CANN 对齐，直接传入 CANN executor，无需逐字段转换。
// 返回 HCCL_SUCCESS 表示已成功派发到 CANN executor; 其它表示失败(调用方应回退)。
HcclResult LaunchViaCann(const OpParam &ascParam, const AlgResourceCtxSerializable &ascResCtx);

}  // namespace mc2_ops_hccl

#endif  // MC2_OPS_HCCL_CANN_KERNEL_BRIDGE_H
