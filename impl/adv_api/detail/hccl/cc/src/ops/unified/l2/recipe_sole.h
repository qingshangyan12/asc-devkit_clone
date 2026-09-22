/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L2 结构层 · 配方接口 + sole 配方（方案 §6.2/§6.8）
 *
 * 本文件补上 L1→L2→L3 的映射断点：PrimitiveRequest + AlgConfig → Plan.segments。
 * 与现状的对应关系：
 *   ins_v2_all_gather_sole_executor.cc（198 行）OrchestrateLoop 的切轮约束
 *       → MaxSlicePerRound（单轮上限计算，原样上移）；
 *       "new 具体 template（InsTempAllGatherMesh1D）" → "声明 ActionSpec 序列"
 *       （纯数据构造，不调 L3 执行函数——L2 调 L3 的方式不是函数调用，是数据构造，§6.3）。
 *   sole 一份配方服务 5 原语（AG/RS/AR/A2A/A2AV）—— 现状是每原语一个 *SoleExecutor
 *       + 每 pattern 一个注册项（§6.2 配方清单）。
 */
#ifndef REFACTOR_POC_RECIPE_SOLE_H
#define REFACTOR_POC_RECIPE_SOLE_H

#include "action_collect_all.h"
#include "action_reduce_slice.h"   // R6：ReduceSlice::CalcRes（RS 段资源申报，two_shot 首段）
#include "action_spec.h"
#include "alg_config_parser.h"

#include <algorithm>
#include <string>
#include <vector>

namespace mc2_ops_hccl::refactor {

// 切轮容量约束（§10.2 A2）：现状 OrchestrateLoop 的 maxDataSizePerLoop 两约束
// （UB 传输上限 256MB 与 cclBuff/scratchMultiplier 中转上限）进入编译链
struct RoundCapacity {
    u64 cclBuffSize{0};        // 中转 buffer 总量；0 = 无中转约束（symmem / 不切轮）
    u32 scratchMultiple{0};    // 算法要几份中转（现状 CalcScratchMultiple）
    u64 transportBound{256ULL * 1024 * 1024};  // UB_MAX_DATA_SIZE，硬件传输上限

    bool Unconstrained() const { return cclBuffSize == 0 || scratchMultiple == 0; }
};

// Expand 需要的环境事实（L1 提供；PrimitiveRequest 只带订单本身，方案 §6.8"环境事实"分类）
struct ExpandCtx {
    u32 domainCount{1};                            // L1Adapter::DeriveDomainCount 产出
    std::vector<std::vector<u32>> hierarchyRanks;  // 逐层 rank 列表（多域配方取域成员）
    RoundCapacity capacity{};                      // 切轮容量约束（缺省无约束 = 单轮，§10.2 A2）
    // §10.2 B8 token 获取函数：真实实现 = hcomm::CcuRep::GetTokenInfo(va, size)
    // （虚地址+长度 → CCU 内存访问凭证，ccu_alg_template_base.cc:201-218）。
    // PoC 无 hcomm 环境：空指针时用 va^size 确定性伪 token（测试可预测）；
    // P2 真机联调时由 L1 注入真实现。KFC 路径逐 mission 覆写 token 槽位的语义
    // （op_common.cc:174-177）在编排器落地时承接，PoC 段级单 token。
    u64 (*tokenOf)(u64 va, u64 size){nullptr};
};

class IRecipe {
public:
    virtual ~IRecipe() = default;
    virtual const char* Name() const = 0;
    virtual u32 DomainsRequired() const = 0;       // TopoApplicability：需要几个参与域
    // 返回 HcclResult（§10.2 C7：与真实链路 HcclResult 逐层透传对齐）；err 保留做日志细节
    virtual HcclResult Expand(const PrimitiveRequest& req, const AlgConfig& cfg, const ExpandCtx& ctx,
                              Plan& plan, std::string& err) const = 0;

    // 能力校验：逐段查 BackendRegistry，无实现 / 能力不支持快速失败（§6.9 错误路径，
    // Commit 前报）。正式件签名 Validate(req, BackendCaps)（§6.2），PoC 由注册表代查 Caps。
    HcclResult Validate(const Plan& plan, Engine engine, std::string& err) const;
};

// 原语 → 动作映射表（方案 §5 完备性论证的落地；现状是每原语一个 executor 家族）
inline ActionKind ActionForOp(int op)
{
    switch (static_cast<HcclCMDType>(op)) {
        case HcclCMDType::HCCL_CMD_ALLGATHER:      return ActionKind::CollectAll;
        case HcclCMDType::HCCL_CMD_REDUCE_SCATTER: return ActionKind::ReduceSlice;
        case HcclCMDType::HCCL_CMD_ALLREDUCE:      return ActionKind::CollectAll; // 段形态决定拆段
        default:                                    return ActionKind::ExchangeAll; // A2A/A2AV
    }
}

inline bool IsAllReduce(int op)
{
    return static_cast<HcclCMDType>(op) == HcclCMDType::HCCL_CMD_ALLREDUCE;
}

// ---------------------------------------------------------------------------
// 公共件：段构造 / 段形态展开 / chunk 切分 / 资源求和
// 所有配方共用——这是"配方只描述结构、不重复动作知识"的落点（§6.2）
// ---------------------------------------------------------------------------
class SegmentBuilder {
public:
    // AR 段形态 AUTO 档阈值：现状 selector 按数据量分档（§6.8-③"可钉可 AUTO"）；
    // 8MiB 对应 auto_selector_base.h RS_M2M_1D_MAX_DATA_SIZE 的分档量级
    static constexpr u64 AR_ONE_SHOT_MAX_BYTES = 8ULL * 1024 * 1024;
    // chunk 段数：实测最优 2~4 段（§1.4-③），PoC 取 2
    static constexpr u32 CHUNK_SEG_NUM = 2;
    // 通道上限：multiJetty=跨框端口数（maxPathNum_，最多 4，§6.8 通道类归一）
    static constexpr u32 MAX_PATH_NUM = 4;

    // 一个 stage → 若若干 ActionSpec（段形态 two_shot/one_shot 拆段、chunk 拆 nSeg 段）
    static HcclResult BuildStage(const PrimitiveRequest& req, const StageConfig& st, u32 domainIdx,
                           const ExpandCtx& ctx, ActionKind kind, TransportPolicy transport,
                           std::vector<ActionSpec>& out, std::string& err)
    {
        ActionSpec act;
        act.kind = kind;
        act.layout = req.layout;
        act.domain = req.domain;
        act.reduce = req.reduce;
        act.transport = transport;
        act.inputRanks = req.domain.ranks;
        act.outputRanks = req.domain.ranks;

        // 段形态限定词仅对 AR 合法（one_shot/two_shot 是 AR 的数据量分档，§6.8 限定词表）
        const bool isAr = IsAllReduce(req.op);
        if (st.shape != SegShape::NONE && !isAr) {
            err = "'.one_shot/.two_shot' is only valid for AllReduce";
            return HCCL_E_PARA;
        }
        SegShape shape = st.shape;
        if (isAr && shape == SegShape::NONE) {
            // AUTO 档：按总数据量选（PoC 阈值；正式件按档位表，§6.8）
            const u64 totalBytes = act.layout.sliceSize * act.domain.Size();
            shape = (totalBytes <= AR_ONE_SHOT_MAX_BYTES) ? SegShape::ONE_SHOT : SegShape::TWO_SHOT;
        }
        act.shape = shape;

        // pattern：钉了就用，没钉 AUTO 按互联度选（PoC 用 req.allConnected 投影；
        // 正式件逐域走 L1Adapter::AutoPatternOf(topo, domainIdx) + DomainPlan，§6.11）
        act.pattern = (st.pattern != CommPattern::AUTO) ? st.pattern : AutoPattern(req, domainIdx);
        if (act.pattern == CommPattern::MESH && !req.allConnected) {
            err = "pattern 'mesh' requires fully-connected domain";
            return HCCL_E_NOT_SUPPORT;   // 拓扑不适用：能力类错误（§6.9 错误分类）
        }
        if (st.hd && (act.domain.Size() & (act.domain.Size() - 1)) != 0) {
            err = "'ring.hd' requires rankSize to be power of 2";
            return HCCL_E_PARA;
        }

        // 通道限定词：显式 multi-channel 不带 N 为 AUTO 选档（PoC 取 2）；上限 maxPathNum
        act.channelsPerRank = st.channelsExplicit ? (st.channelsPerRank ? st.channelsPerRank : 2U) : 1U;
        if (act.channelsPerRank > MAX_PATH_NUM) {
            err = "channelsPerRank " + std::to_string(act.channelsPerRank) +
                  " exceeds maxPathNum(4)";
            return HCCL_E_PARA;
        }

        // token 填充（§10.2 B8）：按真实优先级 input→output→scratch（现状
        // CcuAlgTemplateBase::GetToken，ccu_alg_template_base.cc:201-218 的三分支；
        // PoC 的 scratch 半边由 cclBuff 承担，req 无 cclBuff 字段时退化为 input→output）。
        // 必须在 stageSegs 拷贝之前——各段共享同一 token（现状：token 对整次通信不变，
        // 进每个消息槽；KFC 逐 mission 覆写在编排器层做，§6.5）
        act.token = ResolveToken(req, ctx.tokenOf);

        // 段形态展开（§5 完备性论证）：
        //   two_shot  = ReduceSlice → CollectAll（数据流边连接）
        //   one_shot  = CollectAll + LocalReduce（收齐后逐块累加）
        //   R6 起 RS 段已是完整交付动作（L3 BuildArgs + CCU 后端 + v3 序列化 + server
        //   分支）——two_shot 配方的 RS 段生成路径自本轮打通：Expand 产段 → CalcResOf
        //   落 ReduceSlice::CalcRes（CcuActionReduceSlice 注册条目）→ SerializeSegmentV3
        //   RS 布局 → server v3 REDUCE_SLICE 分支。AR 整体不在 R6 交付清单（路由行未
        //   激活，unified_route.h R6 备注），one_shot 的 LocalReduce 后端仍属 P2。
        //   PoC 注：RS 段沿用请求布局（AR 的 full-buffer→per-slice 布局推导为已知
        //   简化——生产语义 per-slice = count*dts/N，本模型 sliceSize = count*dts 全量；
        //   与独立原型同一简化，收口随 AICPU 线的 count 实例化（R7 B-CCU②）一并处理）
        std::vector<ActionSpec> stageSegs;
        if (shape == SegShape::TWO_SHOT) {
            ActionSpec rs = act;
            rs.kind = ActionKind::ReduceSlice;
            ActionSpec ag = act;
            // §12-9 B2 决策（2026-09-20）：对齐现状 hcclBuff 中转——AG 段 input 取 RS 段
            // 落在 scratch 的产出（ForSecondStageViaScratch），不再直写 output 建模。
            // 执行层流水行为不变（RS 全部完成 → AG 开始，数据流边仍表达该约束）
            ag.layout = act.layout.ForSecondStageViaScratch();
            ag.inputRanks = rs.outputRanks;            // ★数据流边：前段输出=后段输入
            stageSegs = {rs, ag};
        } else if (shape == SegShape::ONE_SHOT) {
            ActionSpec ca = act;                       // 收齐
            ActionSpec lr = act;                       // + 本地规约
            lr.kind = ActionKind::LocalReduce;
            lr.inputRanks = ca.outputRanks;            // ★数据流边
            stageSegs = {ca, lr};
        } else {
            stageSegs = {act};
        }

        // chunk 切分限定词：每段再切 nSeg 子段（装饰器=构造期变换，动作种类不变，§6.3）
        for (const auto& seg : stageSegs) {
            if (!st.chunk) {
                out.push_back(seg);
                continue;
            }
            for (u32 i = 0; i < CHUNK_SEG_NUM; ++i) {
                ActionSpec sub = seg;
                sub.layout = seg.layout.ForChunk(i, CHUNK_SEG_NUM);
                out.push_back(sub);
            }
        }
        return HCCL_SUCCESS;
    }

    // 资源申报：逐段求和（§6.5 三段分工）。
    // R6 起 ReduceSlice 的 CalcRes 落地（action_reduce_slice.h——CCU 半边含 kernel
    // 注册条目 CcuActionReduceSlice）；ExchangeAll/LocalReduce 的 CalcRes 随各自
    // 动作文件落地（P2）。通道申请引擎无关、与现状同式（RS/AG 各调一次
    // CalcChannelRequestMesh1D，逐字相同，见 action_collect_all.h CalcRes 注）。
    static void CalcResOf(const ActionSpec& seg, ResourceClaim& claim)
    {
        switch (seg.kind) {
            case ActionKind::CollectAll:
                claim += CollectAll::CalcRes(seg);
                return;
            case ActionKind::ReduceSlice:
                claim += ReduceSlice::CalcRes(seg);
                return;
            case ActionKind::LocalReduce:
                return;   // 本地动作：不占通道（正式件申报 AICPU 向量执行时间）
            default:      // ExchangeAll：通道同式
                for (u32 r : seg.domain.ranks) {
                    if (r == seg.domain.myRank) continue;
                    claim.channels.push_back(ChannelReq{r, seg.channelsPerRank});
                }
                return;
        }
    }

    static void SumRes(Plan& plan)
    {
        for (const auto& seg : plan.segments) CalcResOf(seg, plan.claim);
    }

    // transport 解析：钉了就用；AUTO 按对称内存支持度选（现状 executor 同式）
    static TransportPolicy ResolveTransport(const AlgConfig& cfg, const PrimitiveRequest& req)
    {
        if (cfg.transport != TransportPolicy::AUTO) return cfg.transport;
        return req.supportSymmetricMemory ? TransportPolicy::SYMMEM : TransportPolicy::VIA_CCUBUF;
    }

    // stage 项数校验：与推导出的参与域数一致（§6.8 解析规则 1/2）
    static HcclResult CheckStageCount(const AlgConfig& cfg, u32 required, u32 derived, std::string& err)
    {
        if (cfg.stages.empty()) return HCCL_SUCCESS;    // 整体省略 = 全域 pattern 交给 AUTO
        if (cfg.stages.size() == 1 && required > 1) {
            err = "multi-domain recipe needs full stage list (single item cannot locate a domain)";
            return HCCL_E_PARA;
        }
        if (cfg.stages.size() != required) {
            err = "stage count " + std::to_string(cfg.stages.size()) + " != domains required " +
                  std::to_string(required);
            return HCCL_E_PARA;
        }
        if (required > derived) {
            err = "recipe needs " + std::to_string(required) + " domains but topology derives " +
                  std::to_string(derived);
            return HCCL_E_NOT_SUPPORT;   // 拓扑事实不匹配
        }
        return HCCL_SUCCESS;
    }

private:
    // AUTO pattern：域内直连→MESH，经交换机→NHR（与 L1Adapter::AutoPatternOf 同源；
    // 机间域恒 NHR。PoC 以 req.allConnected 表达 level0 互联度，逐域 LinkProfile 随
    // DomainPlan 落地，§6.11）
    static CommPattern AutoPattern(const PrimitiveRequest& req, u32 domainIdx)
    {
        if (domainIdx == 0) return req.allConnected ? CommPattern::MESH : CommPattern::NHR;
        return CommPattern::NHR;
    }

    // §10.2 B8 token 解析：input→output 优先级（真实三分支的 input→output 半边；
    // scratch/cclBuff 半边 L2 无 cclBuff 上下文，P2 接真资源链时补）。
    // 伪 token = va ^ size：确定性、可测试；va 未知（0）时 token 置 0（现状无 token 场景，
    // 如纯 host 单测，消息槽 TOKEN 位为 0——P2 真机联调时 Validate 拒绝 0 token）
    static u64 ResolveToken(const PrimitiveRequest& req, u64 (*tokenOf)(u64, u64))
    {
        if (req.inputVa != 0) {
            return (tokenOf != nullptr) ? tokenOf(req.inputVa, req.layout.sliceSize)
                                        : (req.inputVa ^ req.layout.sliceSize);
        }
        if (req.outputVa != 0) {
            return (tokenOf != nullptr) ? tokenOf(req.outputVa, req.layout.sliceSize)
                                        : (req.outputVa ^ req.layout.sliceSize);
        }
        return 0;
    }

public:
    // ---------------------------------------------------------------- 切轮
    // SplitRounds（§10.2 A2）：段序列按容量约束切轮，逐轮用 DataLayout::ForRound 推进
    // baseOff 产出每轮的段——现状 OrchestrateLoop 的 for-loop 在编译期展开成 Plan 的段序列。
    //   symmem 例外：不占中转 buffer，单轮完成（现状 executor 同分支）
    //   无约束（capacity.Unconstrained()）同样单轮——保持既有 Compile 调用语义
    static void SplitRounds(std::vector<ActionSpec>& segments, const RoundCapacity& cap)
    {
        if (cap.Unconstrained()) return;
        constexpr u64 ALIGN = 512;                 // 现状对齐粒度（OrchestrateLoop 同值）
        const u64 scratchBound = cap.cclBuffSize / cap.scratchMultiple / ALIGN * ALIGN;
        const u64 maxPerRound = std::min(cap.transportBound, scratchBound);
        if (maxPerRound == 0) return;              // 中转过小：现状此路径不存在（cclBuff 必≥1份）

        std::vector<ActionSpec> rounded;
        rounded.reserve(segments.size());
        for (const ActionSpec& seg : segments) {
            const bool symmem = (seg.transport == TransportPolicy::SYMMEM);
            const u64 slice = seg.layout.sliceSize;
            if (symmem || slice <= maxPerRound) {
                rounded.push_back(seg);            // 单轮（symmem 或一轮装得下）
                continue;
            }
            const u64 loopTimes = (slice + maxPerRound - 1) / maxPerRound;  // ceil
            for (u64 r = 0; r < loopTimes; ++r) {
                const u64 cur = (r == loopTimes - 1) ? (slice - r * maxPerRound) : maxPerRound;
                ActionSpec rd = seg;
                rd.layout = seg.layout.ForRound(static_cast<u32>(r), cur, maxPerRound);
                rounded.push_back(rd);
            }
        }
        segments.swap(rounded);
    }
};

// ---------------------------------------------------------------- sole（单域）
// 覆盖 Mesh1D 全系（含 16P 小数据 NHR 档、AR one_shot/two_shot 档）——§6.2 配方清单
class RecipeSole : public IRecipe {
public:
    const char* Name() const override { return "sole"; }
    u32 DomainsRequired() const override { return 1; }

    HcclResult Expand(const PrimitiveRequest& req, const AlgConfig& cfg, const ExpandCtx& ctx,
                Plan& plan, std::string& err) const override
    {
        if (SegmentBuilder::CheckStageCount(cfg, DomainsRequired(), ctx.domainCount, err) !=
            HCCL_SUCCESS) {
            return HCCL_E_PARA;
        }
        const StageConfig st = cfg.stages.empty() ? StageConfig{} : cfg.stages[0];
        // cfg.order：保序装饰器（缺省关、显式开启）——PoC 占位，作用机制为构造期变换（§6.3）
        const HcclResult buildRet = SegmentBuilder::BuildStage(
            req, st, 0, ctx, ActionForOp(req.op), SegmentBuilder::ResolveTransport(cfg, req),
            plan.segments, err);
        if (buildRet != HCCL_SUCCESS) return buildRet;   // 错误码透传（不吞不改，§6.9）
        // 切轮（§10.2 A2）：容量约束非零时按 OrchestrateLoop 语义切轮
        SegmentBuilder::SplitRounds(plan.segments, ctx.capacity);
        SegmentBuilder::SumRes(plan);
        return HCCL_SUCCESS;
    }

    // 单轮数据上限（现状 OrchestrateLoop 的 maxCountPerLoop 两约束取小，原样上移；
    // 正式件签名 MaxSlicePerRound(req, ResourceCaps)，§6.2）
    static u64 MaxSlicePerRound(u64 cclBuffSize, u32 scratchMultiple, u64 transportBound)
    {
        if (scratchMultiple == 0) return transportBound;
        constexpr u64 ALIGN = 512;
        const u64 scratchBound = cclBuffSize / scratchMultiple / ALIGN * ALIGN;
        return std::min(transportBound, scratchBound);
    }
};

// ---------------------------------------------------------------------------
// 便捷错误构造：Compile 链路的错误码 + 日志细节一次成型（§10.2 C7）
// ---------------------------------------------------------------------------

inline HcclResult IRecipe::Validate(const Plan& plan, Engine engine, std::string& err) const
{
    for (const auto& seg : plan.segments) {
        const IActionBackend* be = BackendRegistry::Peek(seg.kind, engine);
        // 未注册动作：P2 交付范围（R6 后未落 = ExchangeAll/LocalReduce 后端与
        // ReduceSlice 的 AICPU 后端——快速失败并指明缺口，不静默降级，
        // §6.9：构建失败在 Commit 前报）
        if (be == nullptr) {
            err = std::string("no backend registered for action kind #") +
                  std::to_string(static_cast<int>(seg.kind)) +
                  " under this engine (not in P1 delivery scope)";
            return HCCL_E_NOT_SUPPORT;
        }
        const BackendCaps caps = be->Caps();
        if (seg.transport == TransportPolicy::SYMMEM && !caps.supportSymmem) {
            err = "engine does not support symmetric-memory transport";
            return HCCL_E_NOT_SUPPORT;
        }
        if (seg.layout.dataTypeSize == 8 && !caps.support64bitReduce &&
            (seg.kind == ActionKind::ReduceSlice || seg.kind == ActionKind::LocalReduce)) {
            err = "engine does not support 64bit reduce";
            return HCCL_E_NOT_SUPPORT;
        }
    }
    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl::refactor

#endif
