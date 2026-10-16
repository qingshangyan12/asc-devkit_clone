/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * R12 · WP-9①（用户指令 #2 收编，2026-10-09）：L4 AICPU 后端 · ReduceSlice
 *
 * 与现状的对应关系（AICPU RS 是**写侧**语义——与 CCU 读侧不同，R12-2 约束①）：
 *   ins_temp_reduce_scatter_mesh_1D.cc（307 行）：
 *     KernelRun :63-102  Pre/PostSyncInterThreads → SYNC_PEER_PRE/POST（threadNum=N）
 *     RunReduceScatter :174-249  逐 peer SendRecvBatchWrite（:243）→ WRITE 动词
 *       ——主动侧：本 rank 把自己 input 的 peer 片写到对端 cclBuff 的 me 槽
 *       （src = input 基 + nextRank*inputSliceStride :224-228、
 *         dst = 对端 hcclBuff + me*outputSliceStride :229-233）
 *     PostCopy :104-172  本地半边（in/out buff 型或基址不同 → LocalCopy :134-144 →
 *       COPY；tmpRank≠me 逐个 LocalReduce :155-169 → REDUCE×(N-1)）
 *   ★写侧几何独立推导（禁复用读侧 BuildArgs.peers——那是读侧几何：srcOff=对端
 *     input 我的片/dstOff=本地 scratch peer 槽；R12-2①）：WRITE 的 src 从 layout
 *     直接推导（InOffset(nextRank)——我的 input 里 nextRank 的份）；dst 落对端
 *     cclBuff 的 me 槽（对端基址来自 ChannelInfo.remoteCclMem（alg_param.h:453），
 *     layout 里没有——sink 侧从通道表合成，hcomm_verb_sink.h 的 DoWrite 同口径）。
 *     Emit 只发**段内相对 offset**（dst = me*outputSliceStride 的结构量），绝对地址
 *     由消费方/sink 在通道表命中后合成（与 READ 的 peers.srcOff 同一消费分工）。
 *     环序 nextRank=(me+rankIdx)%N（:182——MESH 语义下与 CollectAll::NextPeer 的
 *     (me+1+step)%N 同集合同构，遍历起点差 1：step=rankIdx-1 对齐，本后端直接复用
 *     NextPeer 保证两后端遍历序单一实现点）。
 */
#ifndef REFACTOR_POC_BACKEND_AICPU_REDUCE_SLICE_H
#define REFACTOR_POC_BACKEND_AICPU_REDUCE_SLICE_H

#include "action_collect_all.h"    // CollectAll::NextPeer（遍历序唯一实现点，复用）
#include "action_reduce_slice.h"
#include "backend_registry.h"
#include "nhr_steps.h"             // R15 · ②：CalcNhrSteps（Emit 侧步 0 判断无需步表——载荷携带，sink 重算）
#include "nhr_slice_geometry.h"   // R18 · #11：NhrSliceOf（展开分支与 DoNhrRun 同源几何）
#include "unified_route.h"         // R15 · ②：UseUnifiedNhrAicpuPath（三层灰度）

namespace mc2_ops_hccl::refactor {

class AicpuReduceSlice : public IActionBackend {
public:
    Engine Kind() const override { return Engine::AICPU; }

    BackendCaps Caps() const override
    {
        // AICPU：支持对称内存与 64bit 规约；逐段固定开销 ~43us（与 AicpuCollectAll 同）
        return BackendCaps{/*symmem*/ true, /*64bit*/ true, /*maxSplits*/ 4, /*overheadUs*/ 43};
    }

    // 线程/notify 公式（R16 · 8g：改委托 ClaimAicpuThreadRes 单源件——action_spec.h；
    // RS：threadNum = max(1, n) = N（≠AG 的 N-1——主线程不下发写，WRITE 用
    // threads[1..N-1]，ins_temp_reduce_scatter_mesh_1D.cc:180-181 queIdx 从 1 起）。
    // R12-2②：sink 侧 DoWrite 独立计数 + 偏移 1 与本口径互为因果。
    void ClaimEngineRes(const ActionSpec& a, ResourceClaim& claim) const override
    {
        ClaimAicpuThreadRes(a, claim);   // RS·mesh：threadNum = max(1, n)
    }

    // Emit：写侧动词序（R12-2 约束①-③ + R12-3②形态）。
    // 几何全部从 layout 直推（DataLayout 访问器唯一实现点），不用读侧 peers。
    void Emit(const ActionSpec& a, const void* argsRaw, EmitCtx& ctx) const override
    {
        const auto& args = *static_cast<const ReduceSliceArgs*>(argsRaw);
        const u32 me = args.myAlgRank;
        const u32 n = args.rankNum;

        // R15 · ② NHR 分支（M0_R15 §四定稿；gated UseUnifiedNhrAicpuPath——
        // ungated 照旧落 mesh 写侧分支 = 现状全连接迭代 ChannelOf Fail 行为零变化）。
        // 动词序（M0 手算二修正 B 口径）= COPY×n（上料全量）→ NHR_RUN×1 → COPY×1
        // （归位）——**恒 10 动词与协议无关**（isDmaRead 是 sink 侧知识，Emit 层
        // 不知——知识不对称拆解：Emit 产物两侧同构，读/写差异全部下沉 sink）。
        // 无 SYNC_PEER_PRE/POST：threadNum=1×channelsPerRank=1（devkit 恒值）→
        // 模板 threadNum>1 门槛不过（ins_temp_reduce_scatter_nhr.cc:121/:143——
        // 单线程域无从线程可同步；主会话裁决 2）；channelsPerRank>1（协议面保留）
        // 时按 sync 位发射。
        if (a.pattern == CommPattern::NHR && UseUnifiedNhrAicpuPath()) {
            // R18 · #11 全删轮（用户裁定 2026-10-14）：本分支为 NHR 的**唯一**
            // Emit 形态（原宏动词 NHR_RUN 分支删除——宏/展开均未经真机验证，
            // 双路径无保留价值）。gate 维持 UseUnifiedNhrAicpuPath（NHR AICPU
            // 执行面整体灰度不变——删的是形态选择，不是执行面）。
            // R18 · #11（M0_R18 §一定稿）：NHR 步进流的**逐动词展开**——与 mesh
            // 的 WRITE×(n-1)/READ×(n-1) 地位对等（宏动词 NHR_RUN 的对称化）。
            // 动词序 = 上料 COPY×n → per step: CH_SYNC(ACK) → SLICE_REDUCE×nSlices
            // → CH_SYNC(DATA) → … → 归位 COPY×1（repeat/jetty 循环维留动词参数
            // 不展开——M0_R18 §二）。SLICE_REDUCE 方向无关（载荷 = nhr 种子 +
            // step/sliceIdx，WR/RD 执行端自判）；CH_SYNC 半边四态。几何全部经
            // NhrSliceOf 公共件（与 DoNhrRun 同源——Emit 只发索引不自算偏移）。
            if (a.sync.preBarrier && a.channelsPerRank > 1U && n > 1U) {
                Fire(ctx, VerbCall{VerbCall::SYNC_PEER_PRE});
            }
            const u64 nhrInStride = a.layout.RsInStride();
            for (u32 r = 0; r < n; ++r) {
                Fire(ctx, VerbCall{VerbCall::COPY,
                                   a.layout.inBaseOff + nhrInStride * r,
                                   a.layout.scratchBaseOff + a.layout.sliceSize * r,
                                   a.layout.SliceSizeOf(r, n)});
            }
            // 种子载荷（与宏形态 NHR_RUN 的 11 标量同源——逐动词复用）
            VerbCall::NhrRunScalars seed;
            seed.valid = true;
            seed.myAlgRank = me;
            seed.rankNum = n;
            seed.channelsPerRank = a.channelsPerRank;
            seed.sliceSize = args.mySliceSize;
            seed.inputBase = a.layout.inBaseOff;
            seed.inputSliceStride = nhrInStride;
            seed.scratchBase = a.layout.scratchBaseOff;
            seed.repeatNum = a.layout.repeatNum;
            seed.inputRepeatStride = a.layout.inputRepeatStride;
            seed.outputRepeatStride = a.layout.outputRepeatStride;
            seed.domainRanks = a.domain.ranks;
            const std::vector<NhrStepInfo> steps = CalcNhrSteps(me, n);
            for (size_t s = 0; s < steps.size(); ++s) {
                const NhrStepInfo& st = steps[s];
                // ACK 半边（DoNhrRun :487-496 的动词化）：先 Record 后 Wait（两动词
                // ——与原两段序一致；WR/RD 方向执行端按通道协议自判取 peer）
                Fire(ctx, MakeChSync(seed, st, /*isAck*/ true, /*isWait*/ false));
                Fire(ctx, MakeChSync(seed, st, /*isAck*/ true, /*isWait*/ true));
                for (u32 i = 0; i < st.nSlices; ++i) {
                    Fire(ctx, MakeSliceReduce(seed, st, static_cast<u32>(s), i));
                }
                // DATA 半边（同四态编码）
                Fire(ctx, MakeChSync(seed, st, /*isAck*/ false, /*isWait*/ false));
                Fire(ctx, MakeChSync(seed, st, /*isAck*/ false, /*isWait*/ true));
            }
            // 归位 COPY×1（与宏形态 ④ 同式）
            Fire(ctx, VerbCall{VerbCall::COPY,
                               a.layout.scratchBaseOff + a.layout.sliceSize * me,
                               a.layout.OutOffset(me),
                               args.mySliceSize});
            if (a.sync.postBarrier && a.channelsPerRank > 1U && n > 1U) {
                Fire(ctx, VerbCall{VerbCall::SYNC_PEER_POST});
            }
            return;
        }


        // ① 段前屏障（现状 PreSyncInterThreads :80-84；threadNum>1 即 N>1）
        if (a.sync.preBarrier && n > 1U) {
            Fire(ctx, VerbCall{VerbCall::SYNC_PEER_PRE});
        }

        // ② 写侧逐 peer：我的 input 里 peer 的份 → 对端 cclBuff 我的槽。
        //    src = InOffset(peerAlgRank)（layout 直推——写侧几何，与读侧 peers.srcOff
        //    不同源）；dst = me*outputSliceStride（对端 cclBuff 的 me 槽结构量——
        //    绝对基址 sink 侧从 ChannelInfo.remoteCclMem 合成）。
        //    size = SliceSizeOf(peerAlgRank)（末 rank 尾片 :186-190 同式）
        //    遍历序 = 升序通道表（R12-9 修复轮 P1-2——与 CCU 读侧/注册序四方统一；
        //    写侧 batch 乱序容许本就声明（R12-2①），升序不破坏）。
        for (u32 peerAlgRank = 0; peerAlgRank < n; ++peerAlgRank) {
            if (peerAlgRank == me) {
                continue;   // 升序跳过 me
            }
            // src = InOffset(peer) 的 stride 三分支（[A8]-c，R12-9 修复轮）：Compile 期
            // inputSliceStride=0（BuildRequest 对 input 不设步长——RS input 步长是
            // exec 期 slot4 语义）→ 退 outputSliceStride（同 BuildArgs rankSliceOffset
            // 三分支，action_reduce_slice.h 同式）——旧形态 InOffset(peer)
            // 恒 0（stride=0 直乘）是缺陷③（写侧 src 恒 0，sink 端写错片）。
            // 双道防线分工：本 host 侧推导覆盖 **AICPU 消费路径**（consumer 重建
            // ActionSpec 后经本 Emit）；设备覆写（hccl_ccu_v0_prepare.h READ 槽 1）
            // 覆盖 **CCU server 路径**——各防一引擎，非冗余。
            //（R16 · 8b：三分支收敛为 DataLayout::RsInStride 访问器）
            const u64 inStride = a.layout.RsInStride();
            Fire(ctx, VerbCall{VerbCall::WRITE,
                               a.layout.inBaseOff + inStride * peerAlgRank,
                               a.layout.outputSliceStride * me,
                               a.layout.SliceSizeOf(peerAlgRank, n),
                               a.domain.ranks[peerAlgRank]});
        }

        // ③ 段后屏障（现状 PostSyncInterThreads :86-90——写的对端完成可见性）
        if (a.sync.postBarrier && n > 1U) {
            Fire(ctx, VerbCall{VerbCall::SYNC_PEER_POST});
        }

        // ④ PostCopy 本地半边（:104-172，在 SYNC_POST 之后——PostCopy 在 KernelRun
        //    返回后执行，R12-2⑦/J7 基线的结构差登记）：
        //    buffsDiffer（in/out buff 型或基址不同 :132-133）→ COPY 我的片 input→output
        //    落位（:134-144）；随后 tmpRank≠me 逐个 REDUCE（对端槽 → output 我的槽，
        //    :155-169）。
        //    buffsDiffer 判据：transport 非 SYMMEM 即中转形态（in/out buff 不同型或
        //    基址不同——PoC 以 transport 投影：VIA_CCUBUF 恒 COPY、SYMMEM 无 COPY）。
        if (a.transport != TransportPolicy::SYMMEM) {
            // COPY src 同 [A8]-c 三分支（InOffset(me) 的 stride 语义同 WRITE src——
            // 旧形态恒 0 是缺陷③的同源半边；R16 · 8b：RsInStride 访问器）
            const u64 cpInStride = a.layout.RsInStride();
            Fire(ctx, VerbCall{VerbCall::COPY,
                               a.layout.inBaseOff + cpInStride * me,
                               a.layout.OutOffset(me),
                               args.mySliceSize});
        }
        for (u32 tmpRank = 0; tmpRank < n; ++tmpRank) {
            if (tmpRank == me) {
                continue;
            }
            Fire(ctx, VerbCall{VerbCall::REDUCE,
                               a.layout.scratchBaseOff + a.layout.sliceSize * tmpRank,
                               a.layout.OutOffset(me),
                               args.mySliceSize});
        }
    }

private:
    // R18 · #11：展开动词构造便捷件（载荷 = nhr 种子 + 索引；几何不下发——
    // 执行端 NhrSliceOf 推导，帧槽只花 step/sliceIdx 两个索引）
    // R18 全删轮补：peer 槽显式进帧（src=peerWR sendTo / dst=peerRD recvFrom——
    // 帧槽 4/5，server VerbSliceReduce 通道查表消费；几何仍由执行端 NhrSliceOf
    // 从种子+索引推导，peer 是显式冗余校验面）
    static VerbCall MakeSliceReduce(const VerbCall::NhrRunScalars& seed, const NhrStepInfo& st,
                                    u32 step, u32 sliceIdx)
    {
        VerbCall vc;
        vc.verb = VerbCall::SLICE_REDUCE;
        vc.src = st.sendTo;       // peerWR（帧槽 4）
        vc.dst = st.recvFrom;     // peerRD（帧槽 5）
        vc.size = seed.sliceSize;
        vc.nhr = seed;
        vc.nhr.stepIdx = step;
        vc.nhr.sliceIdx = sliceIdx;
        return vc;
    }
    // CH_SYNC 半边四态编码（size 槽）：0=ACK-Record / 1=ACK-Wait /
    // 2=DATA-Record / 3=DATA-Wait——每半边两条动词（Record 侧 + Wait 侧，
    // 与 DoNhrRun "先 Record 后 Wait" 的两段序一致）
    static VerbCall MakeChSync(const VerbCall::NhrRunScalars& seed, const NhrStepInfo& st,
                               bool isAck, bool isWait)
    {
        VerbCall vc;
        vc.verb = VerbCall::CH_SYNC;
        vc.src = st.sendTo;       // WR 侧 peer（slot 语义：src=WR peer / dst=RD peer）
        vc.dst = st.recvFrom;     // RD 侧 peer
        vc.size = static_cast<u64>((isAck ? 0U : 2U) + (isWait ? 1U : 0U));
        vc.nhr = seed;
        return vc;
    }
    static void Fire(EmitCtx& ctx, const VerbCall& call)
    {
        FireVerb(ctx.sink, &ctx.trace, call);
    }
};

REGISTER_ACTION_BACKEND(ActionKind::ReduceSlice, Engine::AICPU, AicpuReduceSlice)

} // namespace mc2_ops_hccl::refactor

#endif
