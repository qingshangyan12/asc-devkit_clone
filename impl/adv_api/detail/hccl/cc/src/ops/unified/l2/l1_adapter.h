/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
/**
 * L1 接入层：devkit 真实 OpParam / TopoInfoWithNetLayerDetails → PrimitiveRequest
 *
 * 本文件直接消费 devkit 真实类型（alg_param.h / topo 头），不再自造等价声明——
 * 这是"接在真实 L1 上"的落点。devkit 侧需改动的只有两处（见 §6.8）：
 *   ① GetForcedAlgName 的整串比对 → AlgConfigParser::Parse
 *   ② GetAlgExecViaCann 单键查表 → RecipeRegistry + BackendRegistry 双注册表
 */
#ifndef REFACTOR_L1_ADAPTER_H
#define REFACTOR_L1_ADAPTER_H

#include "action_spec.h"
#include "backend_registry.h"
#include "alg_config_parser.h"
#include "recipe_registry.h"

#include <cstdint>

// devkit 真实头
#include "alg_param.h"
// M3-条件A（M1 审查）：CcuKernelHandle（发射链句柄，ccu_types.h:74 = uint64_t）——
// host 自检 shim 不带该头，此处按底层类型自持定义（与真实头 typedef 逐位一致，
// devkit 树内 include 时 alg_param.h 链已带 ccu 类型面，无冲突——typedef 同型重复
// 声明合法）。改用 include 会在 shim 编译面引入 hcomm 全链，违反 L2 最小依赖。
namespace mc2_ops_hccl::refactor {
using CcuKernelHandle = ::uint64_t;
}

namespace mc2_ops_hccl::refactor {

// ---------------------------------------------------------------------------
// ResourceBroker（§10.2 A5）：unified 侧对"资源申请"的抽象出口。
// 真实现（HcclThreadAcquire / HcclChannelAcquire / CCU kernel 注册）由存量侧
// 注入（op_common.cc 的 UnifiedResourceBroker），unified 头不 include 任何存量
// 符号——保持"L2 不 include 引擎头/存量头"铁律。缺省 NoopBroker 供 host 自检。
//
// 与现状的对应：
//   AcquireThreads  ← HcclThreadAcquire（op_common.cc HcclGetThread :1073 同逻辑）
//   AcquireChannels ← HcclChannelAcquire + 增量去重（CompReqChannelWithExistChannel :778 同式）
//   AcquireCcuKernel← HcommCcuKernelRegister 链（RegisterCcuKernels :1821 输入源换 claim.kernels）
// C1：checkOnly 语义透传——真实现内探测路径资源不足返回 HCCL_E_UNAVAIL，
//     调用方（GetUnifiedOpParamResCtx）翻译为 HCCL_E_RES_NOT_SUFFICIENT（同现状）。
// ---------------------------------------------------------------------------
struct ResourceBroker {
    virtual ~ResourceBroker() = default;
    // AICPU：threadNum = slaveThreadNum+1，notify 数组语义同 AlgResourceRequest
    virtual HcclResult AcquireThreads(u32 threadNum, const std::vector<u32>& notifyNumPerThread,
                                      u32 notifyNumOnMainThread, std::string& err) = 0;
    // 引擎无关通道：逐 remoteRank 申请 count 条（ChannelReq 列表）
    virtual HcclResult AcquireChannels(const std::vector<ChannelReq>& channels, std::string& err) = 0;
    // CCU：微码 kernel 注册（kernelFuncName + 通道组；claim.ccu 七类资源随 kernel 反查）。
    // M3-条件A（M1 审查，2026-09-21）：kernelHandle 只在注册时刻返回一次（无按名反查
    // API），发射期 CcuKernelLaunch 要求 resourceCtx.ccuKernels[0]!=0——出参带回，
    // 由组装段落进 resCtxHost.ccuKernels。
    // R16 · #6：claimCcuOut 可选参数——plan.claim.ccu 的估算申报透传给真实现，
    // 注册链内与驱动反查真值双源对拍（NoopBroker/探测路径忽略，缺省 nullptr 零影响——
    // 既有调用面不变，既有 broker 实现不改签名即可覆写的兼容形态）。
    virtual HcclResult AcquireCcuKernel(const std::vector<CcuKernelPlan>& kernels,
                                        CcuKernelHandle& kernelHandle, std::string& err,
                                        const std::map<u32, std::map<u32, ResCounters>>* claimCcu = nullptr) = 0;
};

// 缺省空实现（host 自检用：不触真实申请，记录调用序列供断言）
struct NoopBroker : ResourceBroker {
    std::vector<std::string> calls;    // 记录 "threads:8" / "chan:7" / "ccuk:1" 供测试断言
    HcclResult AcquireThreads(u32 threadNum, const std::vector<u32>& notifyNumPerThread,
                              u32 notifyNumOnMainThread, std::string& err) override
    {
        (void)err;
        // MAJOR-1（三轮审查）：notify 数真实消费语义在真实现（max+1 公式），
        // NoopBroker 同步记录申请的 notify 数供断言
        u32 maxNotify = notifyNumOnMainThread;
        for (u32 n : notifyNumPerThread) {
            if (n > maxNotify) maxNotify = n;
        }
        calls.push_back("threads:" + std::to_string(threadNum) + "+notify:" + std::to_string(maxNotify + 1U));
        return HCCL_SUCCESS;
    }
    HcclResult AcquireChannels(const std::vector<ChannelReq>& channels, std::string& err) override
    {
        (void)err;
        calls.push_back("chan:" + std::to_string(channels.size()));
        return HCCL_SUCCESS;
    }
    HcclResult AcquireCcuKernel(const std::vector<CcuKernelPlan>& kernels,
                                CcuKernelHandle& kernelHandle, std::string& err,
                                const std::map<u32, std::map<u32, ResCounters>>* claimCcu = nullptr) override
    {
        (void)claimCcu;   // host 自检不消费（R16 · #6 对拍只在真实现）
        (void)err;
        kernelHandle = 0;   // NoopBroker 无真实注册（host 自检），handle 置 0 供断言
        calls.push_back("ccuk:" + std::to_string(kernels.size()));
        return HCCL_SUCCESS;
    }
};

class L1Adapter {
public:
    static u32 DataTypeSizeOf(HcclDataType t) { return DATATYPE_SIZE_TABLE[t]; }

    // OpParam（devkit 真实结构）→ PrimitiveRequest（引擎无关订单）
    // 对应现状：executor OrchestrateLoop 里手工拼 TemplateDataParams 的那一段
    static PrimitiveRequest BuildRequest(
        const OpParam& op, const TopoInfoWithNetLayerDetails& topo,
        const std::vector<std::vector<u32>>& hierarchyRanks)
    {
        PrimitiveRequest req;
        req.op = static_cast<int>(op.opType);
        const u32 dts = DataTypeSizeOf(op.DataDes.dataType);
        req.layout.dataTypeSize = dts;
        req.layout.sliceSize = op.DataDes.count * dts;
        req.layout.tailSize = 0U;
        req.layout.inputSliceStride = 0U;
        req.layout.outputSliceStride =
            (op.DataDes.strideCount == 0U) ? op.DataDes.count * dts : op.DataDes.strideCount * dts;
        req.layout.repeatNum = 1U;
        req.reduce.reduceOp = static_cast<int>(op.reduceType);
        req.reduce.dataType = static_cast<int>(op.DataDes.dataType);
        req.reduce.outputType = static_cast<int>(op.DataDes.outputType);
        req.domain.ranks = hierarchyRanks.empty() ? std::vector<u32>{op.userRank} : hierarchyRanks[0];
        req.domain.myRank = op.userRank;
        // LinkProfile：域内是否全互联（真实拓扑字段判定，与 selector 同源）
        req.allConnected = (topo.level0Topo != Level0Shape::CLOS);
        req.supportSymmetricMemory = op.supportSymmetricMemory;
        // §10.2 B8：token 计算源（input→output 优先级的输入）。devkit OpParam 的
        // inputPtr/outputPtr 是 void*，指针转 u64 与真实 GetToken 的 PointerToAddr 同式
        req.inputVa = reinterpret_cast<u64>(op.inputPtr);
        req.outputVa = reinterpret_cast<u64>(op.outputPtr);
        return req;
    }

    // 引擎映射：CommEngine（MC2 API ServerType）→ 后端选择键
    // 现状 PrepareEngineForAlg 按算法名前缀反推；重构后由 API 直接携带
    static Engine EngineOf(const OpParam& op)
    {
        return (op.engine == CommEngine::COMM_ENGINE_CCU) ? Engine::CCU : Engine::AICPU;
    }

    // 参与域数推导：comm 规模 × DomainPlan（真实 topoLevelNums）
    static u32 DeriveDomainCount(const TopoInfoWithNetLayerDetails& topo, u32 commSize,
                                 const std::vector<std::vector<u32>>& hierarchyRanks)
    {
        if (hierarchyRanks.size() <= 1U) {
            return 1U;
        }
        return (commSize <= hierarchyRanks[0].size()) ? 1U : static_cast<u32>(hierarchyRanks.size());
    }

    // 每域链路形态 → AUTO pattern
    static CommPattern AutoPatternOf(const TopoInfoWithNetLayerDetails& topo, u32 domainIdx)
    {
        if (domainIdx == 0U) {
            return (topo.level0Topo == Level0Shape::CLOS) ? CommPattern::NHR : CommPattern::MESH;
        }
        return CommPattern::NHR;
    }

    // ---------------------------------------------------------------- 入口
    // Compile：一次调用的完整编译链 —— MC2 API 入口到 Expand 的接线
    //（PoC 现状由测试手工串管线；正式版收编为 AlgCompiler::Compile，方案 §6.13/§11）
    //
    // 对应现状两处 devkit 侧改动（本文件头注释 ①②）：
    //   GetForcedAlgName 整串比对 → Parse + RecipeRegistry::Get
    //   GetAlgExecViaCann 单键查表 → Expand 产出 Plan + BackendRegistry 按 (动作,引擎) 分发
    //
    // 返回 HcclResult（§10.2 C7：与真实链路 HcclResult 逐层透传对齐，err 保留做日志细节）。
    // capacity（§10.2 A2 切轮）：缺省无约束 = 单轮语义（既有调用面不变）；
    // 携带 cclBuff/scratchMultiple 时按 OrchestrateLoop 语义切轮。
    static HcclResult Compile(const OpParam& op, const TopoInfoWithNetLayerDetails& topo,
                        const std::vector<std::vector<u32>>& hierarchyRanks, const std::string& algConfig,
                        Plan& plan, std::string& err, const RoundCapacity& capacity = RoundCapacity{},
                        u64 (*tokenOf)(u64 va, u64 size) = nullptr)
    {
        const AlgConfig cfg = AlgConfigParser::Parse(algConfig);
        if (!cfg.valid) {
            err = cfg.err;                          // 退役名 / 语法错：Commit 前报（§6.9）
            return HCCL_E_PARA;
        }
        const PrimitiveRequest req = BuildRequest(op, topo, hierarchyRanks);
        const Engine engine = EngineOf(op);

        IRecipe* recipe = RecipeRegistry::Get(cfg.recipe);
        if (recipe == nullptr) {
            err = "recipe not available (retired or not yet delivered)";
            return HCCL_E_NOT_SUPPORT;
        }
        // ExpandCtx：环境事实（域参与由 comm 规模×DomainPlan 推导，不是选择，§6.8）
        ExpandCtx ctx;
        ctx.domainCount = DeriveDomainCount(topo, topo.userRankSize, hierarchyRanks);  // 真实字段在 TopoInfo（alg_param.h:210）
        ctx.hierarchyRanks = hierarchyRanks;
        ctx.capacity = capacity;
        ctx.tokenOf = tokenOf;                   // §10.2 B8：token 获取函数（空 = 伪 token）
        HcclResult ret = recipe->Expand(req, cfg, ctx, plan, err);
        if (ret != HCCL_SUCCESS) return ret;
        ret = recipe->Validate(plan, engine, err);              // 能力快速失败（不静默降级）
        return ret;
    }

    // ---------------------------------------------------------------- A5 入口
    // Materialize：Compile 后置步骤——Plan.claim（资源申报）→ 逐引擎真实申请。
    // 现状对应：HcclGetAlgRes（op_common.cc:796）对 executor->CalcRes 产出的
    // AlgResourceRequest 做申请；unified 的 ResourceClaim 是其超集（多 CCU 七类
    // 资源与 kernels），翻译规则：
    //   AICPU：slaveThreadNum/notify* 直传（合并语义已对齐：线程 max、notify 追加）
    //          channels → 逐 remoteRank 申请
    //   CCU  ：kernels 透传（kernelFuncName + 通道组），ccu[dieId][resGroup] 随 kernel
    //          注册反查（现状 AccumulateKernelRes :1713 的输入源）
    // broker 为空 = NoopBroker（host 自检路径）。真实现由存量侧注入（灰度分叉处）。
    // 错误路径：申请失败原样上抛（§6.9 不吞不改）——灰度分叉处决定回退老路径。
    // kernelHandleOut（M3-条件A）：CCU 注册返回的 CcuKernelHandle 出参（AICPU 引擎
    // 不触此参，调用方初始化 0）——发射链 CcuKernelLaunch 需要它，注册时刻唯一回传点。
    static HcclResult Materialize(const Plan& plan, Engine engine, ResourceBroker& broker,
                                  std::string& err, CcuKernelHandle& kernelHandleOut)
    {
        const ResourceClaim& claim = plan.claim;
        if (engine == Engine::AICPU) {
            // 线程：slaveThreadNum+1（主+从，同 HcclGetThread :1080 threadNum 语义）
            const u32 threadNum = claim.slaveThreadNum + 1U;
            HcclResult ret = broker.AcquireThreads(threadNum, claim.notifyNumPerThread,
                                                   claim.notifyNumOnMainThread, err);
            if (ret != HCCL_SUCCESS) return ret;
        } else {
            // CCU：微码自驱动不占 AICPU 线程（claim.slaveThreadNum==0，现状同语义），
            // 资源重心在 kernel 注册（七类资源经 HcommCcuKernelQueryResReq 反查）。
            // M3-条件A：kernelHandle 出参带回（NoopBroker 下置 0；真实现
            // HcclCcuKernelRegisterUnified 注册返回值），M3 组装段落进 resCtxHost。
            // R16 · #6：claim.ccu（估算申报）随注册链下发——真实现内与驱动反查
            // 真值"注册时刻双源对拍"（漂移可见化：常规 WARN / env MC2_UNIFIED_RES_
            // DEBUG FAIL）。NoopBroker（host 自检）不消费该参数，零影响。
            HcclResult ret = broker.AcquireCcuKernel(claim.kernels, kernelHandleOut, err,
                                                     &claim.ccu);
            if (ret != HCCL_SUCCESS) return ret;
        }
        // 通道：引擎无关公共件（两引擎同一批 CalcChannelRequest*，§6.5——只申请一次）
        return broker.AcquireChannels(claim.channels, err);
    }
};

} // namespace mc2_ops_hccl::refactor

#endif
