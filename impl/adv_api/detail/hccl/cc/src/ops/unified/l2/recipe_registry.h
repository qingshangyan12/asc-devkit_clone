/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L2 结构层 · 配方注册表 + 多域配方（parallel/sequence/split，方案 §6.2 配方清单）
 *
 * devkit 现有注册项 → 新 algConfig 的逐项映射（16 个 REGISTER_EXEC_V2 全量）：
 *  旧 algName                          | 新 algConfig                | 承接配方
 *  -------------------------------------|-----------------------------|------------
 *  InsAllGatherMesh1D                   | sole[mesh]                  | RecipeSole
 *  InsAllGatherNHR                      | sole[nhr]                   | RecipeSole
 *  CcuAllGatherMesh1DMem2Mem            | sole[mesh,mem2mem]          | RecipeSole
 *  InsReduceScatterMesh1D               | sole[mesh]                  | RecipeSole
 *  InsReduceScatterNHR                  | sole[nhr]                   | RecipeSole
 *  InsReduceScatterMesh1DMeshChunk      | sole[mesh.chunk]            | RecipeSole
 *  InsReduceScatterAicpuReduceNHR       | sole[nhr]                   | RecipeSole
 *  CcuKfcReduceScatterMesh1DMem2Mem     | sole[mesh,mem2mem]          | RecipeSole
 *  AicpuAllReduceSoleMeshOneShot        | sole[mesh.one_shot]         | RecipeSole
 *  AicpuAllReduceSoleMeshTwoShot        | sole[mesh.two_shot]         | RecipeSole
 *  AicpuAlltoAllSoleMesh                | sole[mesh]                  | RecipeSole
 *  AicpuAlltoAllVSoleMesh               | sole[mesh]                  | RecipeSole（V=变长表透传）
 *  AicpuAlltoAllSoleMeshSingleChannel   | sole[mesh.single-channel]   | RecipeSole
 *  CcuAlltoAllVMesh1D                   | sole[mesh]                  | RecipeSole
 *  InsReduceScatterMesh1DZAxisDetour    | 不迁移（ZAxisDetour 裁定）    | —
 *  CcuKfcServer                         | 非算法（server 载体）        | —
 *
 * hccl 仓待迁移形态（配方预留，§6.2 配方清单时点列）：
 *  AicpuAllGatherParallelMeshNHR              | parallel[mesh,nhr]        | RecipeParallel（首批）
 *  CcuSchedAllGatherParallelMeshNHRMultiLink  | parallel[mesh,nhr.multi-channel] | RecipeParallel
 *  AicpuAllGatherConcurMeshNHR                | split[mesh,nhr]           | RecipeSplit（待定·UBX）
 *  AicpuAllReduceConcurMeshTwoShotNHR         | split[mesh.two_shot,nhr]  | RecipeSplit
 *  CcuSchedAllGatherSequenceMeshMesh          | sequence[mesh,mesh]       | RecipeSequence（待定）
 */
#ifndef REFACTOR_POC_RECIPE_REGISTRY_H
#define REFACTOR_POC_RECIPE_REGISTRY_H

#include "action_spec.h"
#include "alg_config_parser.h"
#include "recipe_sole.h"
#include "tier_expand.h"   // R9：TierParams 字段面（RecipeParallel RS 骨架档位挂载）

namespace mc2_ops_hccl::refactor {

// ------------------------------------------------ parallel（多域交错，机内填机间）
// 两域交错下发、互不依赖（段间无数据流边）—— 16P/UBX/PCIeMix 主力，首批交付
class RecipeParallel : public IRecipe {
public:
    const char* Name() const override { return "parallel"; }
    u32 DomainsRequired() const override { return 2; }

    HcclResult Expand(const PrimitiveRequest& req, const AlgConfig& cfg, const ExpandCtx& ctx,
                Plan& plan, std::string& err) const override
    {
        if (SegmentBuilder::CheckStageCount(cfg, DomainsRequired(), ctx.domainCount, err) !=
            HCCL_SUCCESS) {
            return HCCL_E_PARA;
        }
        const auto kind = ActionForOp(req.op);
        const auto transport = SegmentBuilder::ResolveTransport(cfg, req);
        for (u32 d = 0; d < DomainsRequired(); ++d) {
            const StageConfig st = (d < cfg.stages.size()) ? cfg.stages[d] : StageConfig{};
            PrimitiveRequest domainReq = req;
            domainReq.domain.ranks = DomainRanks(ctx, d, req);
            const HcclResult buildRet = SegmentBuilder::BuildStage(domainReq, st, d, ctx, kind,
                                                                    transport, plan.segments, err);
            if (buildRet != HCCL_SUCCESS) return buildRet;   // 错误码透传（不吞不改，§6.9）
        }
        // parallel 的各段互不依赖：inputRanks 各自独立（与 sequence 的区别就在这里）
        SegmentBuilder::SplitRounds(plan.segments, ctx.capacity);   // §10.2 A2
        SegmentBuilder::SumRes(plan);
        // R9 · WP-1（2026-10-07）：RS 双域配方挂档位参数——骨架（每域 1 段）+
        // TierParams 随 Plan 下发，A/B 段序列展开在**消费方 exec 期**（Instantiate
        // 单决策点，§3.6-②③：RS 资源期 count=0 展不开 + R6-③"不预编两个候选
        // Plan"）。加法不改法：非 RS op / hasTier 未置位时本块零执行，既有 Plan
        // 逐字段零变化（防退化断言：poc_unified_expand_test R9 节）。
        // 带宽值来源 = 拓扑属性（ziliao ins_v2_reduce_scatter_omnipipe_executor.cc
        // :343-356）：PCIeMix 慢轴按 rankSizeLevel1 分档——2 server=29
        //（BW_OMNI_PCIE_EIGHT_RS_CLOS）/4 server=35（BW_OMNI_PCIE_SIXTEEN_RS_CLOS）、
        // 快轴 mesh 缺省 50（BW_OMNI_DEFAULT）；慢轴 eqBw 口径 = bw/(rankSize-1)
        //（ziliao :365-366）——分母展开进有理数对（35/3 场景无损）。
        // ★终审 MAJOR-2 修复（2026-10-07）：原实现硬编码 35（16 卡常数），8 卡档
        //   （域 1 成员 2，29 常数）被错挂 35——现按域 1 成员数分档 {2→29, 4→35}；
        //   其余慢轴成员数（UBX CLOS 225、非 PCIeMix 缺省等）tier 不置位——骨架
        //   直跑语义安全（不产步进序列 = 不涉带宽公式，无静默错跑面），per-domain
        //   实测带宽注入随 §6.11 DomainPlan/LinkProfile 落地（登记 R9 记录，P2）。
        if (kind == ActionKind::ReduceSlice && !plan.segments.empty() &&
            plan.segments.size() >= DomainsRequired()) {
            const u32 slowRankSize = plan.segments[1].domain.Size();
            u64 slowBwNum = 0;                          // 0 = 无已知分档 → tier 不置位
            if (slowRankSize == 2U) {
                slowBwNum = 29U;                        // BW_OMNI_PCIE_EIGHT_RS_CLOS（ziliao :28）
            } else if (slowRankSize == 4U) {
                slowBwNum = 35U;                        // BW_OMNI_PCIE_SIXTEEN_RS_CLOS（ziliao :29）
            }
            if (slowBwNum != 0U) {
                plan.tier.hasTier = true;               // 骨架携带双形态参数集
                plan.tier.threshBytes = TierParams{}.threshBytes;   // 4MiB（selector :19 同值）
                plan.tier.interleave = TierInterleave{};            // partRatio 1/2（:229-234 口径）
                plan.tier.stepwise.bwFastNum = 50U;     // BW_OMNI_DEFAULT（ziliao :26）
                plan.tier.stepwise.bwFastDen = 1U;
                plan.tier.stepwise.bwSlowNum = slowBwNum;
                plan.tier.stepwise.bwSlowDen = (slowRankSize > 1U) ? slowRankSize - 1U : 1U;   // eqBw :365-366
                plan.tier.stepwise.alignBytes = 512U;   // HCCL_MIN_SLICE_ALIGN_OMNIPIPE（ziliao :21）
            }
        }
        return HCCL_SUCCESS;
    }

protected:
    // 域成员：ctx.hierarchyRanks 逐层给出（L1Adapter 接口同源）；单域数据时退化全域
    static std::vector<u32> DomainRanks(const ExpandCtx& ctx, u32 d, const PrimitiveRequest& req)
    {
        if (d < ctx.hierarchyRanks.size()) return ctx.hierarchyRanks[d];
        return req.domain.ranks;
    }
};

// ------------------------------------------- sequence（多域按序，前域输出=后域输入）
// inter 域 → intra 域：前域末段 outputRanks 链到后域首段 inputRanks（数据流边协议，§6.3）
class RecipeSequence : public RecipeParallel {
public:
    const char* Name() const override { return "sequence"; }

    HcclResult Expand(const PrimitiveRequest& req, const AlgConfig& cfg, const ExpandCtx& ctx,
                Plan& plan, std::string& err) const override
    {
        if (SegmentBuilder::CheckStageCount(cfg, DomainsRequired(), ctx.domainCount, err) !=
            HCCL_SUCCESS) {
            return HCCL_E_PARA;
        }
        const auto kind = ActionForOp(req.op);
        const auto transport = SegmentBuilder::ResolveTransport(cfg, req);
        for (u32 d = 0; d < DomainsRequired(); ++d) {
            const StageConfig st = (d < cfg.stages.size()) ? cfg.stages[d] : StageConfig{};
            PrimitiveRequest domainReq = req;
            domainReq.domain.ranks = DomainRanks(ctx, d, req);
            if (d > 0) {
                // 前域输出布局 = 后域输入布局（机间输出=机内输入，现状 sequence executor
                // 手写；此处为通用协议）
                domainReq.layout = plan.segments.back().layout.ForSecondStage();
            }
            const size_t before = plan.segments.size();
            const HcclResult buildRet = SegmentBuilder::BuildStage(domainReq, st, d, ctx, kind,
                                                                    transport, plan.segments, err);
            if (buildRet != HCCL_SUCCESS) return buildRet;   // 错误码透传（不吞不改，§6.9）
            if (d > 0 && before > 0) {
                plan.segments[before].inputRanks = plan.segments[before - 1].outputRanks; // ★数据流边
            }
        }
        SegmentBuilder::SplitRounds(plan.segments, ctx.capacity);   // §10.2 A2
        SegmentBuilder::SumRes(plan);
        return HCCL_SUCCESS;
    }
};

// ---------------------------------- split（Concurrent：数据按比例拆分，双域并发跑）
// 与 parallel 的区别：同一份数据切两半，两域各跑一半（带宽相加）—— 仅 UBX 4P，待实测定
class RecipeSplit : public RecipeParallel {
public:
    const char* Name() const override { return "split"; }

    HcclResult Expand(const PrimitiveRequest& req, const AlgConfig& cfg, const ExpandCtx& ctx,
                Plan& plan, std::string& err) const override
    {
        if (SegmentBuilder::CheckStageCount(cfg, DomainsRequired(), ctx.domainCount, err) !=
            HCCL_SUCCESS) {
            return HCCL_E_PARA;
        }
        const auto kind = ActionForOp(req.op);
        const auto transport = SegmentBuilder::ResolveTransport(cfg, req);
        constexpr u64 SPLIT_NUM = 2;
        for (u32 d = 0; d < DomainsRequired(); ++d) {
            const StageConfig st = (d < cfg.stages.size()) ? cfg.stages[d] : StageConfig{};
            PrimitiveRequest half = req;
            half.domain.ranks = DomainRanks(ctx, d, req);
            half.layout = req.layout.ForChunk(d, SPLIT_NUM);   // 数据切分，非域切分
            const HcclResult buildRet = SegmentBuilder::BuildStage(half, st, d, ctx, kind, transport,
                                                                    plan.segments, err);
            if (buildRet != HCCL_SUCCESS) return buildRet;   // 错误码透传（不吞不改，§6.9）
        }
        SegmentBuilder::SplitRounds(plan.segments, ctx.capacity);   // §10.2 A2
        SegmentBuilder::SumRes(plan);
        return HCCL_SUCCESS;
    }
};

// ---------------------------------------------------------------- 注册表
// 键 = Recipe 枚举（algConfig 主键，op 由 PrimitiveRequest 携带，各管一个维度，§6.8）
class RecipeRegistry {
public:
    static IRecipe* Get(Recipe r)
    {
        static RecipeSole sole;
        static RecipeParallel parallel;
        static RecipeSequence sequence;
        static RecipeSplit split;
        switch (r) {
            case Recipe::SOLE:     return &sole;
            case Recipe::PARALLEL: return &parallel;
            case Recipe::SEQUENCE: return &sequence;
            case Recipe::SPLIT:    return &split;
            default: return nullptr;                     // pipeline 已裁剪（保留数学，§6.2）
        }
    }

    // 默认配方：不填 algConfig → sole（per-topology 默认表含数据量分档，PoC 取 sole，§6.8）
    static Recipe DefaultOf(const AlgConfig& cfg)
    {
        return cfg.valid ? cfg.recipe : Recipe::SOLE;
    }
};

} // namespace mc2_ops_hccl::refactor

#endif
