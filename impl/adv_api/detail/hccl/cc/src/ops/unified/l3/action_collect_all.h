/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L3 语义动作 CollectAll（= AllGather 的语义本体）
 *
 * 语义契约（引擎无关，一句话可判对错）：
 *   本域 output 的第 i 块 == rank i 的 input 第 i 块（i 遍历域内全部 algRank）
 *
 * 对应现状：
 *   AICPU  ins_temp_all_gather_mesh_1D.cc（358 行）—— 语义 + 布局 + 编排 + 资源混在一起
 *   CCU    ccu_temp_all_gather_mesh_1D.cc（128 行）+ ccu_kernel_all_gather_mesh1d.cc（159 行）
 *   本文件只保留"语义与参数"，编排与引擎原语下沉 L4。
 */
#ifndef REFACTOR_POC_ACTION_COLLECT_ALL_H
#define REFACTOR_POC_ACTION_COLLECT_ALL_H

#include "action_args.h"
#include "action_spec.h"
#include "backend_registry.h"

namespace mc2_ops_hccl::refactor {

// BuildArgs 的产物（ArgBundle）：消息槽排布 + 后端展开所需的逐 peer 参数
struct CollectAllArgs {
    CollectAllSlotArgs slots;        // ← 进消息槽的部分（ACTION_ARGS 声明，见 action_args.h）
    u64 inputAddrOff{0};             // 我的 input 起点（我这一块）
    u64 outputAddrOff{0};            // 我在 output 里的落位
    u64 mySliceSize{0};
    u64 myCount{0};
    u32 myAlgRank{0};
    u32 rankNum{0};
    // 逐 peer 参数（AICPU 按线程展开、CCU 按 dst[] 数组填）
    struct PeerSlot {
        u32 peerRank{0};
        u32 peerAlgRank{0};
        u64 srcOff{0};               // 从对端哪里取（read 语义）
        u64 dstOff{0};               // 落到我的 output 哪里
        u64 sliceSize{0};
        u64 count{0};
    };
    std::vector<PeerSlot> peers;
    // CCU 侧还需要的 loop 三段切分参数（LoopSplitter 产出，AICPU 不消费）
    u64 goSize[4]{0, 0, 0, 0};
};

class CollectAll {
public:
    // ---------------------------------------------------------------- 接口①
    // CalcRes：资源申报（方案 §6.5）
    //   通道申请：引擎无关公共件（现状两引擎各调一次 CalcChannelRequestMesh1D，逐字相同）
    //   引擎相关量：后端条目分派
    static ResourceClaim CalcRes(const ActionSpec& a)
    {
        ResourceClaim claim;
        // ① 引擎无关：按 domain × pattern × channelsPerRank 申请通道
        for (u32 r : a.domain.ranks) {
            if (r == a.domain.myRank) continue;
            claim.channels.push_back(ChannelReq{r, a.channelsPerRank});
        }
        // ② 引擎相关：后端条目自报（CCU 反查 kernel / AICPU 线程公式）
        if (auto* be = BackendRegistry::Peek(ActionKind::CollectAll, Engine::CCU)) {
            be->ClaimEngineRes(a, claim);
        }
        if (auto* be = BackendRegistry::Peek(ActionKind::CollectAll, Engine::AICPU)) {
            be->ClaimEngineRes(a, claim);
        }
        return claim;
    }

    // ---------------------------------------------------------------- 接口②
    // BuildArgs：把 ActionSpec 编译为后端无关参数集
    //   全部 offset 经 DataLayout 访问器（唯一实现点），本函数内不出现任何手算式子
    static CollectAllArgs BuildArgs(const ActionSpec& a, const LoopSplitter& splitter)
    {
        CollectAllArgs args;
        const u32 me = a.domain.MyAlgRank();
        const u32 n = a.domain.Size();

        args.myAlgRank = me;
        args.rankNum = n;
        args.inputAddrOff = a.layout.InOffset(me);
        args.outputAddrOff = a.layout.OutOffset(me);
        args.mySliceSize = a.layout.SliceSizeOf(me, n);
        args.myCount = a.layout.CountOf(me, n);

        // 遍历顺序由 pattern 决定（MESH: 逐个换新 peer；RING: 固定相邻；NHR: 定制序）
        for (u32 step = 0; step < n - 1; ++step) {
            const u32 peerAlgRank = NextPeer(a.pattern, me, step, n);
            CollectAllArgs::PeerSlot slot;
            slot.peerRank = a.domain.ranks[peerAlgRank];
            slot.peerAlgRank = peerAlgRank;
            slot.sliceSize = a.layout.SliceSizeOf(peerAlgRank, n);
            slot.count = a.layout.CountOf(peerAlgRank, n);
            // 对端源地址：three 分支全经 DataLayout 访问器（唯一实现点，§6.7）——
            //   SYMMEM            → 对端 output 直读
            //   VIA_CCUBUF 单段   → 对端 cclBuff 的逐 rank 槽位（ScratchOffset 口径）
            //   VIA_CCUBUF 中转段 → scratch[0] 恒定槽（two_shot 第二段：RS 已把各 rank 片
            //     归并到 scratch[0]，AG 对任意 peer 取同一份——InOffset 步长 0，
            //     ins_temp_all_reduce_mesh_1D_two_shot.cc:297/:363/:367）
            if (a.transport == TransportPolicy::SYMMEM) {
                slot.srcOff = a.layout.OutOffset(peerAlgRank);
            } else if (a.shape == SegShape::TWO_SHOT) {
                slot.srcOff = a.layout.InOffset(peerAlgRank);   // 步长 0 → 恒 scratch 基址
            } else {
                slot.srcOff = a.layout.ScratchOffset(peerAlgRank, n);
            }
            slot.dstOff = a.layout.OutOffset(peerAlgRank);
            args.peers.push_back(slot);
        }

        // CCU 后端需要的 loop 参数（AICPU 后端忽略；三处共用同一 Calc）
        splitter.Calc(args.mySliceSize, args.goSize);

        // 填消息槽排布部分（字段顺序由 ACTION_ARGS 声明统一分配，装配代码生成）
        args.slots.INPUT_ADDR = args.inputAddrOff;
        args.slots.OUTPUT_ADDR = args.outputAddrOff;
        args.slots.TOKEN = a.token;
        args.slots.OFFSET = a.layout.OutOffset(me);
        args.slots.SLICE_SIZE = args.mySliceSize;
        return args;
    }

    // pattern → 第 step 步的 partner（遍历方式的唯一定义点）
    static u32 NextPeer(CommPattern p, u32 me, u32 step, u32 n)
    {
        switch (p) {
            case CommPattern::RING:                    // 固定相邻
                return (me + 1) % n;
            case CommPattern::NHR:                     // 交换链路定制序（示意：步距倍增）
                return (me + (1u << step)) % n;
            case CommPattern::MESH:                    // 每步换一个新 peer
            case CommPattern::AUTO:
            default:
                return (me + 1 + step) % n;            // 现状 ins_temp_all_gather_mesh_1D.cc:104
        }
    }
};

} // namespace mc2_ops_hccl::refactor

#endif
