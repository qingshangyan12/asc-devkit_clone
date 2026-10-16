/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * R12 · WP-5：L4 Emit 序捕获（host 组装段的 v5 序列化输入）——Plan 段 → VerbSegment
 *
 * 消费链：hccl_alloc_ctx_res.cc GetUnifiedOpParamResCtxImpl ④ 段（v5 灰度分叉）调
 * 本函数逐段捕获 Emit 动词序，交 verb_serializer.h 编 v5 帧序列。
 *
 * 归属：l5/（引擎头消费面——与 hcomm_verb_sink.h 同级理由：BuildArgs 在 l3/ 动作头、
 * BackendRegistry 在 inc/，但"调真后端 Emit"这一步依赖 l4 后端的完整注册面，本文件
 * include l4 头族即触发 REGISTER_ACTION_BACKEND 静态注册，不进 inc/ 值对象层）。
 */
#ifndef REFACTOR_POC_VERB_CAPTURE_H
#define REFACTOR_POC_VERB_CAPTURE_H

#include "action_collect_all.h"    // CollectAll::BuildArgs
#include "action_reduce_slice.h"   // ReduceSlice::BuildArgs
#include "backend_registry.h"      // BackendRegistry::Get / EmitCtx
#include "loop_splitter.h"         // LoopSplitter（BuildArgs 形参）
#include "verb_sink.h"             // VerbCall / IVerbSink
#include "verb_serializer.h"       // VerbSegment

namespace mc2_ops_hccl::refactor {

// VerbCall 捕获 sink（EmitCtx.sink 注入面——与 TraceSink/HcommVerbSink 同型第三实现）
struct CaptureVerbSink : public IVerbSink {
    std::vector<VerbCall> calls;
    void Emit(const VerbCall& call) override { calls.push_back(call); }
};

// R17 · #9（M0_R17 §一提取件）：段级动词序捕获单源——"BuildArgs 分派 → 真后端
// Emit → sink 捕获"。v5 生产链（CaptureVerbSegment）/ DryRun（pipeline_model.h）
// / 语义 checker（semantic_checker.h）三消费方同一实现（REVIEW_CHECKLIST #16：
// 不自建第二份 Emit——DryRun 主锁 = 本函数直连对拍）。
// 空 calls 语义与 CaptureVerbSegment 一致（后端未注册 / BuildArgs 拒收 NHR 等）。
inline std::vector<VerbCall> CaptureVerbCalls(const ActionSpec& seg, Engine engine)
{
    std::vector<VerbCall> out;
    IActionBackend* be = BackendRegistry::Get(seg.kind, engine);
    if (be == nullptr) {
        return out;   // 后端未注册：空序（调用方降级处理）
    }
    CaptureVerbSink sink;
    EmitCtx ctx;
    ctx.sink = &sink;
    if (seg.kind == ActionKind::CollectAll) {
        LoopSplitter splitter;
        CollectAllArgs args = CollectAll::BuildArgs(seg, splitter);
        be->Emit(seg, &args, ctx);
    } else if (seg.kind == ActionKind::ReduceSlice) {
        ReduceSliceArgs args;
        std::string berr;
        if (ReduceSlice::BuildArgs(seg, args, berr) != HCCL_SUCCESS) {
            return out;   // NHR 拒收等：空序
        }
        be->Emit(seg, &args, ctx);
    } else {
        return out;   // ExchangeAll/LocalCopy/LocalReduce：交付面外
    }
    out = sink.calls;
    return out;
}

// 段 Emit 序捕获：Plan.segments[i] → BuildArgs → 真后端 Emit（sink 捕获）→ VerbSegment。
// 调用方以 calls.empty() 判失败（BuildArgs 拒收 NHR / 后端未注册等形态）。
// R17 · #9：Emit 半边改委托 CaptureVerbCalls（单源提取——行为零变化，既有
// v5 golden 测试自动锁）。
inline VerbSegment CaptureVerbSegment(const Plan& plan, size_t segIdx, Engine engine)
{
    VerbSegment vs;
    const ActionSpec& seg = plan.segments[segIdx];
    vs.seg = seg;
    vs.actionKind = (seg.kind == ActionKind::CollectAll) ? VerbCall::BROADCAST
                                                         : VerbCall::REDUCE;
    if (seg.kind != ActionKind::CollectAll && seg.kind != ActionKind::ReduceSlice) {
        return vs;   // ExchangeAll/LocalCopy/LocalReduce：R12 交付面外，calls 空
    }
    vs.calls = CaptureVerbCalls(seg, engine);
    return vs;
}

} // namespace mc2_ops_hccl::refactor

#endif
