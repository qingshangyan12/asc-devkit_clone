/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L4 CCU 后端 · ReduceSlice（R6/2026-09-21）
 *
 * 与现状的对应关系（本轮只接 MESH 形态；真原语不在本轮 CCU 侧范围——见 Emit 注）：
 *   ccu_temp_kfc_reduce_scatter_mesh_1D_mem2mem.cc（注册期元数据）→ ClaimEngineRes；
 *   ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc（三阶段 kernel）→ Emit 的动词序。
 *
 * DoReduceScatter 三阶段考古（kernel :297-368，Emit 动词序的唯一依据）：
 *   Phase1 DoReduceScatterRead（:162-186）：逐 rank 批量非阻塞读——本 rank 槽位
 *     （rankSize ≤ 8 时）零搬运占位（EventRecord，Phase3 由 GroupLocalReduce 将本
 *     rank 槽替换为 input 直读）；peer 槽位 ccu::Read(ch, scratchMem[peer],
 *     remoteInput[peer], sliceSize, ...)——即"从对端 input 读我的片 → 本地 scratch 槽"。
 *   Phase2 DoReduceScatterWait（:189-201）：批量 EventWait 等齐全部读完成。
 *   Phase3 DoReduceScatter（:235-256）：GroupLocalReduce（≤8 rank，scratch 全槽 +
 *     本 rank 槽替换为 input 直读 → myOutput）或 PairwiseLocalReduce（>8 rank，
 *     log2 两两折叠）。= 动词 READ×(N-1) → REDUCE（本地折叠一步）。
 *   外壳：PreSync（:134-149 互发 input/scratch/token 变量+等齐）/ PostSync（:151-160
 *   完工握手）→ SYNC_PEER_PRE / SYNC_PEER_POST。
 */
#ifndef REFACTOR_POC_BACKEND_CCU_REDUCE_SLICE_H
#define REFACTOR_POC_BACKEND_CCU_REDUCE_SLICE_H

#include "action_reduce_slice.h"
#include "backend_registry.h"

namespace mc2_ops_hccl::refactor {

class CcuReduceSlice : public IActionBackend {
public:
    Engine Kind() const override { return Engine::CCU; }

    BackendCaps Caps() const override
    {
        // CCU：规约不支持 64bit/PROD（与 CcuCollectAll 同限制 → Validate 快速失败）
        return BackendCaps{/*symmem*/ true, /*64bit*/ false, /*maxSplits*/ 4, /*overheadUs*/ 0};
    }

    // CCU 侧资源申报：kernel 注册条目（§10.2 C9）——动作 kernel 名
    // "CcuActionReduceSlice"（"CcuAction" 前缀是 InheritKfcServerKernelArg 的
    // UNIFIED_PLAN role 覆写识别键，poc_unified_kfc_gate_test 已断言前缀泛化命中）。
    // server v3 分支的分发目标 CcuReduceScatterMesh1DMem2MemKernel 经既有
    // KFC server 常驻循环驱动（注册本体 = CcuKernelKfcServer，与 CollectAll 同机制）。
    // scratch 需求：CalcScratchMultiple = rankSize（现状 RS M2M 模板 :76-82 同式——
    // one_shot 场景 scratch 需为 usrIn 的 rankSize 倍），由 host 侧 cclBuff 语义承载
    //（scratchBaseOff 布局字段 + v3 槽 3 scratch 基址），不在 ResourceClaim 七类
    // 资源内单独申报（与现状 KFC RS 路径同口径：scratch 走 OpResCtx.res[0]）。
    void ClaimEngineRes(const ActionSpec& a, ResourceClaim& claim) const override
    {
        claim.slaveThreadNum = 0;               // 微码自驱动，不占 AICPU 线程
        claim.notifyNumOnMainThread = 0;
        CcuKernelPlan plan;
        plan.kernelFuncName = "CcuActionReduceSlice";
        for (u32 r : a.domain.ranks) {
            if (r == a.domain.myRank) continue;
            plan.channels.push_back(ChannelReq{r, a.channelsPerRank});
        }
        claim.kernels.push_back(plan);
        // 真实实现：HcommCcuKernelQueryResReq 让驱动解析微码逐 resType 反查
        //（对照 RS kernel 实际占用：input/scratch/token 每 peer 一对 Variable、
        // events = RS_UNROLL_NUM*ceil(rankSize/16)、1 个 loopGroup）
        ResCounters req;
        req.loop = 1;
        req.ccuBuf = 1;
        req.variable = a.domain.Size() * 3 + 2;   // input/scratch/token 每 peer 三 + chunk/goSize
        req.event = 2;                            // Pre/PostSync 各一（三阶段 events 在 kernel 内部）
        req.instruction = 1;
        claim.ccu[/*dieId*/ 0][/*resGroup*/ 0] += req;
    }

    // Emit：三阶段动词序（DoReduceScatter 考古的引擎无关投影）。
    // ★真原语不在本轮 CCU 侧范围（R7 随 CCU lowering 落地）：本轮动词序供 ST 语义
    // checker 断言（trace 口径）+ 注册链/序列化闭环；verb → ccu::Read/GroupLocalReduce
    // 的映射工单在 verb_sink.h 的 kCcuVerbMap（READ/REDUCE 行已有）。
    //
    // ★动词序与 kernel 三阶段批次语义的差距（R6 审查方点名，注释写明——AICPU 侧
    // 将来不得按动词序逐条直译）：
    //   1. 批次语义丢失：kernel Phase1 是"先全发起（批量非阻塞 ReadNb + EventRecord）
    //      再全等（Phase2 批量 EventWait）"的流水形态（kernel :162-201——读与等
    //      两段分离，读重叠度高）；VerbCall 是单发射模型，READ×(N-1) 逐条 Fire 会把
    //      "先全发起再全等"打散成"发起即等"的串行语义。真原语接线（R7）时须按
    //      ccu::Read 的非阻塞特性还原批次（微码层天然支持——EventRecord/EventWait
    //      分离即 kernel 现状），动词层只表达数据流不表达等待点。
    //   2. >8 rank 的本 rank LocalCopy：kernel Phase1 对 rankSize>8（Pairwise 路径）
    //      的本 rank 槽位做 ccu::LocalCopy(scratch[rankId], myInput)（kernel :176-179
    //      ——>8P 在 scratch 槽位原地折叠，本 rank 数据仍需落入 scratch），≤8P 才是
    //      零搬运占位（EventRecord，Phase3 由 GroupLocalReduce 将本 rank 槽替换为
    //      input 直读，kernel :172-174/:246-248）。动词序按 rankNum 条件发射。
    void Emit(const ActionSpec& a, const void* argsRaw, EmitCtx& ctx) const override
    {
        const auto& args = *static_cast<const ReduceSliceArgs*>(argsRaw);

        // ① 段前握手：互发 input/scratch/token + 等齐（现状 PreSync :134-149）
        if (a.sync.preBarrier) {
            Fire(ctx, VerbCall{VerbCall::SYNC_PEER_PRE});
        }

        // ② >8P：本 rank 片先落 scratch 槽（Pairwise 路径的 Phase1 本 rank 半边，
        //    kernel :176-179 ccu::LocalCopy(scratchMem[me], myInput, sliceSize)）。
        //    ≤8P 无此步（零搬运占位，Phase3 直读 input）。阈值 8 ==
        //    REDUCE_SCATTER_GROUP_REDUCE_MAX_PIECE_CNT（kernel :22，GroupLocalReduce
        //    的最大分片数——≤8 走 GroupLocalReduce、>8 走 PairwiseLocalReduce）
        if (args.rankNum > 8U) {
            Fire(ctx, VerbCall{VerbCall::COPY, args.inputAddrOff + args.rankSliceOffset,
                               args.scratchAddrOff + a.layout.sliceSize * args.myAlgRank,
                               args.mySliceSize});
        }

        // ③ Phase1+2 合并投影：逐 peer READ（对端 input 的"我的片" → 本地 scratch 槽）。
        //    本 rank 片不占 READ 动词（≤8P Phase3 直读 input；>8P 已由 ② 的 COPY 承载）
        for (const auto& p : args.peers) {
            Fire(ctx, VerbCall{VerbCall::READ, p.srcOff, p.dstOff, p.sliceSize, p.peerRank});
        }

        // ④ Phase3：本地折叠（scratch 全槽 + 本 rank input 直读 → 我的 output 片）。
        //    READ 动词语义只覆盖 Phase1 的机间半边；折叠是本地动作，REDUCE 动词承载
        //    （kCcuVerbMap REDUCE 行 = GroupLocalReduce/PairwiseLocalReduce 映射工单）
        Fire(ctx, VerbCall{VerbCall::REDUCE, args.scratchAddrOff, args.outputAddrOff,
                           args.mySliceSize});

        // ⑤ 段后握手（现状 PostSync :151-160）
        if (a.sync.postBarrier) {
            Fire(ctx, VerbCall{VerbCall::SYNC_PEER_POST});
        }
    }

private:
    static void Fire(EmitCtx& ctx, const VerbCall& call)
    {
        FireVerb(ctx.sink, &ctx.trace, call);
    }
};

REGISTER_ACTION_BACKEND(ActionKind::ReduceSlice, Engine::CCU, CcuReduceSlice)

} // namespace mc2_ops_hccl::refactor

#endif
