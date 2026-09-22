/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L4 CCU 后端 · CollectAll
 *
 * 与现状的对应关系：
 *   ccu_temp_all_gather_mesh_1D.cc（128 行）：CalcRes 的引擎半边 → ClaimEngineRes；
 *       KernelRun 的地址/offset 推导（86~97 行）→ BuildArgs 已完成，此处只填槽
 *   ccu_kernel_all_gather_mesh1d.cc（159 行）：InitResource/LoadArgs/PreSync/PostSync
 *       → 本后端的公共壳（现状 44 个 kernel 各写一遍）；DoAllGather 的 GroupBroadcast
 *       → 动词 broadcast 一行
 *   CalGoSize → LoopSplitter（BuildArgs 内已算入 args.goSize）
 */
#ifndef REFACTOR_POC_BACKEND_CCU_COLLECT_ALL_H
#define REFACTOR_POC_BACKEND_CCU_COLLECT_ALL_H

#include "action_collect_all.h"
#include "backend_registry.h"

namespace mc2_ops_hccl::refactor {

class CcuCollectAll : public IActionBackend {
public:
    Engine Kind() const override { return Engine::CCU; }

    BackendCaps Caps() const override
    {
        // CCU：规约不支持 64bit/PROD（硬件限制 → Validate 快速失败，不静默降级）
        // 逐段开销待实测（方案 §1.4-⑤），当前假设 0
        return BackendCaps{/*symmem*/ true, /*64bit*/ false, /*maxSplits*/ 4, /*overheadUs*/ 0};
    }

    // CCU 侧资源申报：指名动作 kernel + 驱动反查七类资源
    // 现状 ccu_temp_all_gather_mesh_1D.cc:36-71 —— 差别是 kernelFuncName 从
    // "CcuKernelAllGatherMesh1D"（算法名）变为动作 kernel（每动作一个）
    void ClaimEngineRes(const ActionSpec& a, ResourceClaim& claim) const override
    {
        claim.slaveThreadNum = 0;               // 不占 AICPU 线程（微码自驱动）
        claim.notifyNumOnMainThread = 0;
        // kernel 注册条目（§10.2 C9）：现状 CcuKernelInfo{kernelFuncName, channels} 进
        // resourceRequest（ccu_temp_all_gather_2dies_mesh_1D.cc:88 的用法）；此处按
        // domain × channelsPerRank 展开通道清单，与 CalcRes 引擎无关半边同一份展开
        CcuKernelPlan plan;
        plan.kernelFuncName = "CcuActionCollectAll";
        for (u32 r : a.domain.ranks) {
            if (r == a.domain.myRank) continue;
            plan.channels.push_back(ChannelReq{r, a.channelsPerRank});
        }
        claim.kernels.push_back(plan);
        // 真实实现：HcommCcuKernelQueryResReq("CcuActionCollectAll", args) 让驱动解析微码，
        //          逐 resType 得到数量，再按 [dieId][resGroup] 累加（AccumulateKernelRes）
        ResCounters req;
        req.loop = 1;
        req.ccuBuf = 1;
        req.variable = a.domain.Size() * 2 + 2;  // output/token 每 peer 一对 + offset/goSize
        req.event = 2;                           // Pre/PostSync 各一
        req.instruction = 1;
        claim.ccu[/*dieId*/ 0][/*resGroup*/ 0] += req;
    }

    void Emit(const ActionSpec& a, const void* argsRaw, EmitCtx& ctx) const override
    {
        const auto& args = *static_cast<const CollectAllArgs*>(argsRaw);

        // ① 段前握手：互发地址+等齐（现状 kernel 的 PreSync，44 份拷贝之一）
        if (a.sync.preBarrier) {
            Fire(ctx, VerbCall{VerbCall::SYNC_PEER_PRE});
        }

        // ② 一条声明式原语完成全域广播
        //    现状 DoAllGather：填 dst[] 数组 + GroupBroadcast(channels_, dst, src, groupOpSize_)
        //    重构后 dst[] 的地址全部来自 args.peers（BuildArgs 已按 layout 算好）。
        //    本 rank 落位并入 broadcast（dst 含本 rank 地址）——无 PostLocalCopy 步（B4 仅 AICPU）
        Fire(ctx, VerbCall{VerbCall::BROADCAST, args.outputAddrOff,
                           /*peers*/ args.peers.size(), args.mySliceSize});

        // ③ 段后握手（现状 PostSync）
        if (a.sync.postBarrier) {
            Fire(ctx, VerbCall{VerbCall::SYNC_PEER_POST});
        }
    }

private:
    // 发射便捷：sink 注入优先，空则 TraceSink 兜底（第一轮 trace 口径不变）
    static void Fire(EmitCtx& ctx, const VerbCall& call)
    {
        FireVerb(ctx.sink, &ctx.trace, call);
    }
};

REGISTER_ACTION_BACKEND(ActionKind::CollectAll, Engine::CCU, CcuCollectAll)

} // namespace mc2_ops_hccl::refactor

#endif
