/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L3 语义动作 ReduceSlice（= ReduceScatter 的语义本体，R6/2026-09-21）
 *
 * 语义契约（引擎无关，一句话可判对错）：
 *   本域 output 的第 me 块 == Σ(i 遍历域内全部 algRank) rank i 的 input 第 me 块
 *   （Σ 按 ReduceSpec.reduceOp 归约）
 *
 * 对应现状（本轮只对齐 MESH 形态；NHR 形态属 R9，BuildArgs/序列化双处拒收）：
 *   AICPU  ins_temp_reduce_scatter_mesh_1D.cc（RecvWrite + LocalReduce 两步）
 *   CCU    ccu_temp_kfc_reduce_scatter_mesh_1D_mem2mem.cc（注册）+
 *          ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc（三阶段：批量 Read 到
 *          scratch → 批量 EventWait → GroupLocalReduce/Pairwise 折叠）
 *
 * ★ RS 消息槽布局唯一基准 = v2 **并发布局**（KFC_CONCURRENT_RS_MESH_* 槽 1..15 紧凑
 *   区间，kfc_server_message.h:155-172）——server :327-339 REDUCE_SCATTER_MESH 分支读
 *   的是该区间；设备侧写入方 CcuPrepareForConcurrentReduceScatterM2M
 *   （hccl_ccu_v0_prepare.h:528-575）三方逐槽一致（R6 主会话裁决四）。
 *   SoleMesh host 写入方（ccu_temp_reduce_scatter_mesh_1D_mem2mem.cc:136-140 的
 *   12 参 homm 兼容布局 + token 占槽 3）是干扰项，不抄。token 不占槽（v2/v3 同格局：
 *   server 恒取 ctx.token）。
 *
 * 【M0 考古三必答（2026-09-21，逐字段证据）】
 * ① RS 参数序全表（槽 1..15，对齐 server :327-339 读取序 / kernel 形参序 /
 *    设备侧写入方 :547-563 写入序三方一致）：
 *    [1]=input 基址  [2]=output 基址  [3]=scratch 中转地址  [4]=rankSliceOffset
 *    (= rankId*sliceStride，纯 rank 偏移——kernel InitReduceScatterAddr :266-271 对
 *    myInput/remoteInput 统一加该偏移；currentRankSliceOutputOffset 在 kernel 内恒 0
 *    :119，**无槽**——RS 的 output 无 per-rank 偏移)  [5]=chunkSize
 *    [6]=chunkLoopNum(=UINT64_MAX-chunkCount 倒计数)  [7]=tailSize  [8..11]=full-go
 *    四元组(offset/loopParam/parallelParam/residual)  [12..15]=tail-go 四元组。
 *    共 15 槽 + 槽 0 动作 ID = 恰好贴 KFC_CONCURRENT_RS_MESH_PARAM_NUM=16 上限，
 *    v3 单段 32 槽容纳无需升版宽（第四轮预警解除，R6 实证）。
 *    channels/channelCount/rankSize/rankId 同 v2 从 ctx.arg 取（不占槽）；
 *    dataType/outputType/reduceType 同 v2 从 ctx.arg->opParam 取（不占槽，见 ③）。
 * ② goSize RS config 独立声明（裁决五）：{loopCount=16, memSlice=4096}
 *    （loopSize=65536）——与 CollectAll 的写入方 config {8,32768} 分开声明禁止照抄。
 *    依据：CCU_LOOP_COUNT_M2M_RE=16（hccl_inner_def.h:172）+ CCU_MEMSLICE_SIZE=4096
 *    （:176）+ 设备侧写入方 CalcGoSize(chunkSize, CCU_LOOP_COUNT_M2M_RE,
 *    CCU_MEMSLICE_SIZE, ...)（:561-563）+ kernel 侧本地常量 CCU_LOOP_COUNT_M2M_RE=16
 *    及其"必须与宿主 CalcGoSize loopCount 一致否则 goSize 与 LoopGroup 步进错位"
 *    的注释（ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc:26-30）。full/tail 两组
 *    分别按 chunkSize/meshTailSize 算；chunkSize = scratchSize(64MB)/rankNum/128*128
 *    （:552-553，与 host 侧 alloc_ctx_res.cc:266 的 4*MC2_WORKSPACE_SIZE=64MB 双端契约）。
 * ③ dataType/outputType/reduceType 槽位语义：**不占槽，从 ctx.arg->opParam 取**
 *    （server :337-339 与 v2 所有 RS 分支同式）。v3 头部三槽（本轮扩）是**纯维测
 *    dump**（R6 尾款终裁：AICPU 消费方一律以 exec 期 msg 现值为唯一权威源，头部
 *    值仅日志/trace 用，不作语义判定输入）——CCU server 分支不读头部
 *    （ctx.arg->opParam 现状不动）。
 *    ★存量语义分歧登记（P2 债台账项，只登记不改行为）：AICPU exec 期适配器强等
 *    dataType（comm_kfc_open_kernel_adapter.cc:141
 *    param->DataDes.outputType = param->DataDes.dataType）vs AG prepare 的
 *    outputType=dstDataType（hccl_alloc_ctx_res.cc:510）——RS 段的 outputType 语义
 *    在两链路不同源，P2 接 AICPU 消费方时统一。
 */
#ifndef REFACTOR_POC_ACTION_REDUCE_SLICE_H
#define REFACTOR_POC_ACTION_REDUCE_SLICE_H

#include "action_args.h"
#include "action_collect_all.h"   // CollectAll::NextPeer（pattern 遍历序的唯一实现点，复用不重写）
#include "action_spec.h"
#include "backend_registry.h"
#include "loop_splitter.h"

namespace mc2_ops_hccl::refactor {

// RS 的 goSize 独立 config 与 chunk 参数（RsGoSizeConfig / RsChunkParams /
// CalcRsChunks）在 inc/loop_splitter.h（值对象最小依赖层）——本头经 include
// "loop_splitter.h" 复用同一实现点；下沉理由与证据链见该头注（R6/裁决五：
// {loopCount=16, memSlice=4096} 与 CollectAll 的 {8,32768} 分开声明禁止照抄）。

// BuildArgs 的产物（ArgBundle）：消息槽排布 + 后端展开所需的逐 peer 参数。
// 槽位部分不在此重复声明字段——v3 序列化（plan_serializer.h）与 RS 段布局是同一份
// KFC_CONCURRENT_RS_MESH_* 契约，字段名直接复用语义命名（不引入第二套 ACTION_ARGS
// 宏展开：RS 的 15 槽是既有 v2 契约的镜像，不是新排布）。
struct ReduceSliceArgs {
    u64 inputAddrOff{0};            // 我的 input 基址（段内 offset）
    u64 outputAddrOff{0};           // 我的 output 基址（RS 无 per-rank output 偏移）
    u64 scratchAddrOff{0};          // scratch 中转基址（hcclBuff 语义）
    u64 rankSliceOffset{0};         // rankId*inputSliceStride（纯偏移，不含基址）
    RsChunkParams chunks;           // chunk 三元组 + full/tail goSize 四元组
    u64 repeatNumInv{0};            // UINT64_MAX - repeatNum
    u64 mySliceSize{0};
    u64 myCount{0};
    u32 myAlgRank{0};
    u32 rankNum{0};
    // 逐 peer 参数（RS mesh：每 peer 从对端 input 的"我的片"处读 → 落本地 scratch 槽）
    struct PeerSlot {
        u32 peerRank{0};
        u32 peerAlgRank{0};
        u64 srcOff{0};               // 从对端 input 哪里取（= 对端 input 基址 + rankSliceOffset）
        u64 dstOff{0};               // 落到本地 scratch 哪个槽（scratch[peer]）
        u64 sliceSize{0};
        u64 count{0};
    };
    std::vector<PeerSlot> peers;
};

class ReduceSlice {
public:
    // ---------------------------------------------------------------- 接口①
    // CalcRes：资源申报（§6.5，形态对齐 CollectAll::CalcRes）
    //   通道申请：引擎无关公共件（现状 RS/AG 各调一次 CalcChannelRequestMesh1D，逐字相同）
    //   引擎相关量：后端条目自报（CCU kernel 注册条目 + scratch 需求在 L4 侧申报）
    static ResourceClaim CalcRes(const ActionSpec& a)
    {
        ResourceClaim claim;
        for (u32 r : a.domain.ranks) {
            if (r == a.domain.myRank) continue;
            claim.channels.push_back(ChannelReq{r, a.channelsPerRank});
        }
        if (auto* be = BackendRegistry::Peek(ActionKind::ReduceSlice, Engine::CCU)) {
            be->ClaimEngineRes(a, claim);
        }
        if (auto* be = BackendRegistry::Peek(ActionKind::ReduceSlice, Engine::AICPU)) {
            be->ClaimEngineRes(a, claim);
        }
        return claim;
    }

    // ---------------------------------------------------------------- 接口②
    // BuildArgs：把 ActionSpec 编译为后端无关参数集（全部 offset 经 DataLayout 访问器）
    //   仅受理 MESH 形态（NHR 是 R9 交付物——RS NHR 消息槽起 16 跨 20 槽超 v3 单段
    //   32 槽边界，SerializeSegmentV3 亦拒收 NHR，双处同一防线）
    static HcclResult BuildArgs(const ActionSpec& a, ReduceSliceArgs& args, std::string& err)
    {
        if (a.pattern == CommPattern::NHR) {
            err = "ReduceSlice NHR pattern not in R6 scope (v3 record width 32 slots < NHR 20-slot span at 16)";
            return HCCL_E_NOT_SUPPORT;
        }
        const u32 me = a.domain.MyAlgRank();
        const u32 n = a.domain.Size();

        args.myAlgRank = me;
        args.rankNum = n;
        args.inputAddrOff = a.layout.inBaseOff;
        args.outputAddrOff = a.layout.outBaseOff;
        args.scratchAddrOff = a.layout.scratchBaseOff;
        args.mySliceSize = a.layout.SliceSizeOf(me, n);
        args.myCount = a.layout.CountOf(me, n);
        // rankSliceOffset = 纯 rank 偏移（不含 inBaseOff——kernel 侧 myInput.addr =
        // input[rankId] + rankSliceOffset，基址槽与偏移槽分列，v2 写入方 :551 同式）。
        // RS 的 inputSliceStride 在布局语义上 = 各 rank 片在 input 里的间隔（现状
        // executor :152-153：strideCount==0 时取全量 dataSize）
        args.rankSliceOffset = a.layout.inputSliceStride * me;
        // repeatNumInv 不占 RS 消息槽：v2 并发布局 [1..15] 无此参数——kernel 三阶段的
        // repeatNum 单 die 恒硬编码 UINT64_MAX-1（kernel :393-395，与并发 RS 的
        // KFC_CONCURRENT_RS_NHR_REPEAT_NUM_INV 是两码事），段级 repeat 由 chunk 循环承载。
        // 本字段仅供 ArgBundle 消费方（后端 Emit/AICPU 侧）重建语义，不进槽。
        args.repeatNumInv = ~0ULL - a.layout.repeatNum;
        // chunk 参数：sliceSize 用**本 rank 视角的片字节**（现状 v2 写入方按
        // commParam->count（每 rank 输出份）计算——mesh 全互联下各 rank 同值，
        // 尾片差异由 tailSize 布局字段表达、不在 chunk 公式内）
        args.chunks = CalcRsChunks(args.mySliceSize, n);

        // 逐 peer：RS mesh 读对端 input 的"我的片"（src 恒 = 对端基址 + 我的偏移），
        // 落本地 scratch 的"peer 槽"（scratch 按 peerAlgRank 分槽，kernel
        // InitReduceScatterAddr :275-277 的 scratchOffset += sliceSize 推进同式）
        for (u32 step = 0; step < n - 1; ++step) {
            const u32 peerAlgRank = CollectAll::NextPeer(a.pattern, me, step, n);
            ReduceSliceArgs::PeerSlot slot;
            slot.peerRank = a.domain.ranks[peerAlgRank];
            slot.peerAlgRank = peerAlgRank;
            slot.sliceSize = args.mySliceSize;
            slot.count = args.myCount;
            slot.srcOff = a.layout.inBaseOff + args.rankSliceOffset;
            slot.dstOff = a.layout.scratchBaseOff + a.layout.sliceSize * peerAlgRank;
            args.peers.push_back(slot);
        }
        return HCCL_SUCCESS;
    }
};

} // namespace mc2_ops_hccl::refactor

#endif
