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
