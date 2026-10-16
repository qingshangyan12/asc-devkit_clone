/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef AICPU_ST_KERNEL_RUNNER_H
#define AICPU_ST_KERNEL_RUNNER_H

#include <memory>
#include <string>

#include "alg_param.h"

namespace HcclSim {
namespace AicpuSt {

// ═══════════════════════════════════════════════════════════════════════════════════
// AICPU kernel run 的 ST 执行器（真机角色：AICPU 处理器上一次通信 kernel 执行的 host 等价物）
//
// 收编背景：ST 此前经 HcclExecOp(op_common.cc:421，产品废弃代码，整条传统链
// HcclExecOp→HcclAicpuKernelEntranceLaunch→AicpuKernelLaunch→aclrt launch→
// HcclLaunchAicpuKernel(kernel_launch.cc) 待删)驱动被测算法。本类将其 AICPU_TS 分支
// 与 kernel run（950 路径）的逻辑收编为 ST 自持实现，调用边界收至
// executor->Orchestrate——与活跃链最后一跳（cann_dispatch_bridge.cc:136 /
// comm_kfc_open_kernel_executor.cc:200 对 executor->Orchestrate 的调用）形态一致。
//
// 收编原则：
//  - 死代码（kernel_launch.cc 独有：反序列化调用序、RestoreVarData*、handshake 序列）→ 收编本体
//  - 活函数（HcclGetAlgRes/GetMainThreadInfo/GetUnfoldThreadInfo/CaptureSlaveStreams/
//    HcclThreadAcquireWithStream/InitHcommBatchTransferOnThreadSupported/registry）→ 原样调用
//  - 纯 DFX 上报（HcclDfxRegOpInfo/HcclReportAicpuKernel/HcommProfiling*——ST 桩为空实现、
//    不产生任务图足迹）→ 省略，注释标注原位置
//  - 反序列化缓存（g_cacheManager，thread_local）→ 省略：ST 每用例独立 SimWorld，缓存无意义，
//    直接反序列化行为等价（反序列化结果相同、不产生任务图足迹）
//
// 语义说明：launch（下发任务）语义在 ST 无对应物——真机 rtAicpuKernelLaunchExWithArgs
// 的下发为了异步；ST 全同步模型中"下发+执行"合一于本 Runner（ aclrt 桩直调时期即如此）。
//
// 保真验证：收编前后以 AICPU_ST_DUMP_EXECLOG 执行日志逐用例对照（零漂移），
// handshake 任务对（4 个 notify 点）是对照重点——它们参与任务图同步语义与 DAG 检测。
// ═══════════════════════════════════════════════════════════════════════════════════
class AicpuStKernelRunner {
public:
    // 驱动一次 AICPU kernel run：资源（复用 GetOpParamResCtx 已分配的结果）→ handshake →
    // 反序列化 → executor->Orchestrate（被测物）→ 完成通知。
    // 入参与 fixture 原 HcclExecOp 调用点对齐（resPack 参数属 CCU 分支，AICPU 不需要）。
    static HcclResult Run(
        HcclComm comm, mc2_ops_hccl::OpParam& param,
        std::unique_ptr<mc2_ops_hccl::TopoInfoWithNetLayerDetails>& topoInfo, const std::string& algName);
};

} // namespace AicpuSt
} // namespace HcclSim

#endif // AICPU_ST_KERNEL_RUNNER_H
