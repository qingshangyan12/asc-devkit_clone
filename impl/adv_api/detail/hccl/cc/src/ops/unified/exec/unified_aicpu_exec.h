/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * R6 · WP-A2 AICPU unified 执行器对外接口（2026-09-22）
 *
 * 挂点（exec 期分叉，R6 主会话裁决——不注册 CollAlgExecRegistryV2、不改存量
 * executor）：设备侧 Orchestrate 调用点前的 unified 分支（kernel_launch.cc 的
 * GetAlgExec 后 / comm_kfc_open_kernel_executor.cc :197 同点）。分叉判定 =
 * param.algName 携带 unified 识别键（资源期 GetOpParam unified 成功路径回填，
 * hccl_alloc_ctx_res.cc :1233 区域）。
 *
 * 消费方依赖（l5/hcomm_verb_sink.h，WP-A1 交付）：HcommVerbSink 持真原语
 * （READ 五步/SYNC 两段循环/COPY 单步），本层只做 blob 解释 + exec 现值实例化 +
 * L4 Emit 驱动。
 */
#ifndef REFACTOR_EXEC_UNIFIED_AICPU_EXEC_H
#define REFACTOR_EXEC_UNIFIED_AICPU_EXEC_H

#include "alg_param.h"          // OpParam / AlgResourceCtxSerializable / HcclResult

#include <cstring>             // strcmp（判定入口全串比对）

namespace mc2_ops_hccl::refactor {

class HcommVerbSink;

// unified 识别键：资源期成功路径写进 opParam.algName——设备侧
// CollAlgExecRegistryV2::GetAlgExec(opType, algName) 查不到该名（无注册项），
// 分叉判定必须在查表前/后以 strcmp 全串命中（第四轮 BLOCKER-1 教训：不用
// compare(0,N,...) 前缀判定——第二参是长度非字面量长度）。
// C++14 无 inline 变量——constexpr 命名空间级（内部链接，头文件多点包含安全）
constexpr const char* kUnifiedAicpuAlgKey = "unified_aicpu";

// AICPU unified 执行入口（实现见 unified_aicpu_exec.cc）：
//   param  —— exec 期 OpParam（resCtx = unified blob ctx 的 device 地址、
//             ctxSize = blob 字节数；DataDes.count/dataType/strideCount、
//             inputPtr/outputPtr 均为设备侧注入的 exec 现值——adapter 链）
//   resCtx —— 反序列化后的资源上下文（threads/channels/cclMem/algHierarchyInfo）
//   sink   —— 编排方持有的动词 sink（真机传 l5/ HcommVerbSink——真原语）
// 返回非 SUCCESS 上抛（设备侧同 Orchestrate 失败处置——执行期失败已过资源期
// 灰度回退点，不回退：半执行状态回退会造成数据不一致）。
HcclResult RunUnifiedAicpuOp(const OpParam& param, const AlgResourceCtxSerializable& resCtx,
                             HcommVerbSink& sink);

// 分叉判定（挂点用；strcmp 全串比对——无前缀/长度陷阱）
inline bool IsUnifiedAicpuAlgName(const char* algName)
{
    return algName != nullptr && std::strcmp(algName, kUnifiedAicpuAlgKey) == 0;
}

} // namespace mc2_ops_hccl::refactor

#endif
