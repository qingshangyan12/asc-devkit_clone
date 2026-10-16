/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L4 AICPU 后端 · CollectAll
 *
 * 与现状 ins_temp_all_gather_mesh_1D.cc（358 行）的对应关系：
 *   保留：SendRecvRead 动词调用、LocalCopy、Pre/PostSyncInterThreads 线程编排骨架
 *   抽走：offset 手算（→ DataLayout）、tail if（→ SliceSizeOf）、遍历顺序（→ pattern）、
 *         线程/notify 公式（→ 本文件 ClaimEngineRes 一处）、
 *         symmem 分支（→ ActionSpec.transport + BuildArgs 已算好地址）
 *   注意：本后端服务"所有走 CollectAll 的算法"——AG sole/parallel 各域/NHR/RING 共用此一份
 *
 * 动词发射（§10.2 A1）：Emit 内只产 VerbCall 经 EmitCtx.sink 发射（空 sink 时 TraceSink
 * 兜底拼 trace，第一轮行为不变）；动词→Hcomm 原语的映射表在 l5/verb_sink.h（引擎知识唯一居所）。
 */
#ifndef REFACTOR_POC_BACKEND_AICPU_COLLECT_ALL_H
#define REFACTOR_POC_BACKEND_AICPU_COLLECT_ALL_H

#include "action_collect_all.h"
#include "backend_registry.h"

namespace mc2_ops_hccl::refactor {

class AicpuCollectAll : public IActionBackend {
public:
    Engine Kind() const override { return Engine::AICPU; }

    BackendCaps Caps() const override
    {
        // AICPU：支持对称内存与 64bit 规约；逐段固定开销 ~43us（实测，方案 §1.4-⑤）
        return BackendCaps{/*symmem*/ true, /*64bit*/ true, /*maxSplits*/ 4, /*overheadUs*/ 43};
    }

    // 线程/notify 公式（R16 · 8g：改委托 ClaimAicpuThreadRes 单源件——action_spec.h。
    // 现状考古：同一段公式在 65 个 AICPU template 的 GetRes()/CalcRes() 里各写一遍
    //（ins_temp_all_gather_mesh_1D.cc:33-41 与 ins_temp_reduce_scatter_mesh_1D.cc:63-68
    // 仅"threadNum 取 N-1 还是 N"一处差异，其余逐字相同——该差异由
    // DomainThreadTotalNeed 的 AG/RS 分叉承载，两后端只剩一行委托）
    void ClaimEngineRes(const ActionSpec& a, ResourceClaim& claim) const override
    {
        ClaimAicpuThreadRes(a, claim);   // AG：threadNum = max(1, n-1)
    }

    // Emit：只使用七个公共语义动词（§6.4.1），不出现 offset 计算
    void Emit(const ActionSpec& a, const void* argsRaw, EmitCtx& ctx) const override
    {
        const auto& args = *static_cast<const CollectAllArgs*>(argsRaw);

        // ① 本地归位（现状 LocalDataCopy 的语义部分；skip 判定由 layout 提供）
        if (args.inputAddrOff != args.outputAddrOff) {
            Fire(ctx, VerbCall{VerbCall::COPY, args.inputAddrOff, args.outputAddrOff,
                               args.mySliceSize});
        }
        if (a.transport == TransportPolicy::VIA_CCUBUF) {
            Fire(ctx, VerbCall{VerbCall::COPY, args.inputAddrOff,
                               a.layout.ScratchOffset(args.myAlgRank, args.rankNum),
                               args.mySliceSize});
        }

        // ② 段前屏障（现状 PreSyncInterThreads）
        if (a.sync.preBarrier && args.peers.size() > 1) {
            Fire(ctx, VerbCall{VerbCall::SYNC_PEER_PRE});
        }

        // ③ 逐 peer 读取——每 peer 一个从线程，参数全部来自 BuildArgs
        //    现状此处是 120~217 行的地址推导 + SendRecvRead；重构后只剩动词与参数
        for (const auto& p : args.peers) {
            Fire(ctx, VerbCall{VerbCall::READ, p.srcOff, p.dstOff, p.sliceSize, p.peerRank});
        }

        // ④ 段后屏障（现状 PostSyncInterThreads）
        if (a.sync.postBarrier && args.peers.size() > 1) {
            Fire(ctx, VerbCall{VerbCall::SYNC_PEER_POST});
        }
        // 注：无段后回拷（§10.2 B4 勘误）：现状 ins_temp_all_gather_mesh_1D.cc 的
        // PostLocalCopy（:278 定义）在该文件无调用点（死代码）——真实语义是 read 直达
        // output（:170 rxDstPtr=outputPtr）。POST_COPY 动词保留在词汇表（P2 若需中转
        // 形态再启用，且彼时 read 目的地须改为 scratch），本后端不发。
    }

private:
    // 发射便捷：sink 注入优先，空则 TraceSink 兜底（第一轮 trace 口径不变）
    static void Fire(EmitCtx& ctx, const VerbCall& call)
    {
        FireVerb(ctx.sink, &ctx.trace, call);
    }
};

REGISTER_ACTION_BACKEND(ActionKind::CollectAll, Engine::AICPU, AicpuCollectAll)

} // namespace mc2_ops_hccl::refactor

#endif
