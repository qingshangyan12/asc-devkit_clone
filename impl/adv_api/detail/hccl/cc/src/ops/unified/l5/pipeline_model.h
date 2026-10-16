/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. Do not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
/**
 * R17 · ①：PipelineModel / DryRun（设计债第四类 #9——"依据分层建模型方便推导"）
 *
 * 定位：纯函数链 Compile 期口径 → 一张流水推导表。老架构看 kernel 代码即看流水；
 * 分层后流水拆散在五处（段结构/sync 位/动词线程/原语映射）——本件把五处投影
 * 成一张表（steps[seq/batch/domainId/threadId/verb/几何] + batchTail + barriers
 * + claim），各层可单独 dump（各积木自带函数）、全链 dump = 本件。
 *
 * M0_R17_PIPELINE.md（六必答定稿）的实现面：
 *   · driver 单源 = CaptureVerbCalls（verb_capture.h 提取件，与 v5 生产链同一
 *     Emit 捕获实现——不自建第二份 Emit）；
 *   · 编排步 = OrchestrateSegments 单源（批划分/域切池/barriers 全部由编排器
 *     产出，REVIEW_CHECKLIST #16 单源派生纪律）；DryRun 合成足额池
 *     （Σ DomainThreadNeed → degraded 恒 false）；
 *   · threadId 推演 = 查 kAicpuVerbMap 的 ThreadPolicy 列（R16 · #7 交付面）
 *     按段内独立计数器推演（与 sink ThreadByCounter 同构，hcomm_verb_sink.h:671
 *     ——DryRun 不真驱动，纯表推演）；全局口径 = 域 base + 域内槽位。
 *
 * 归属 l5/（与 verb_capture.h 同级同理由）：include 后端注册面触发静态注册 +
 * 编排器的 alg_param.h ThreadHandle 面，不进 inc/ 值对象层。
 *
 * 边界（M0 §一/#22 对拍锁声明）：
 *   · DryRun 是编译期口径——exec 期 msg 现值重算 sliceSize 的动态量不在模型内；
 *   · 主锁 = DryRun 逐段动词序 == CaptureVerbCalls 直连（常跑断言，st R17 段）；
 *   · CCU 段 threadId 恒 0（kCcuVerbMap 全 NA，微码自驱动无线程概念）。
 */
#ifndef REFACTOR_POC_PIPELINE_MODEL_H
#define REFACTOR_POC_PIPELINE_MODEL_H

#include "action_spec.h"        // ActionSpec / DomainThreadTotalNeed / ResourceClaim
#include "backend_registry.h"   // Engine
#include "segment_orchestrator.h"   // OrchestrateSegments / OrchestratorRes / HcommVerbRes
#include "verb_capture.h"       // CaptureVerbCalls（R17 提取件——Emit 捕获单源）
#include "verb_sink.h"          // VerbCall / ThreadPolicyOf

#include <string>
#include <vector>

namespace mc2_ops_hccl::refactor {

// ---------------------------------------------------------------------------
// 流水推导表（M0_R17 §二定稿结构）
// ---------------------------------------------------------------------------
struct PipelineStep {
    u32 seq{0};         // 全序号（编排器逐段驱动序）
    u32 batch{0};       // 编排批号（DryRun 恒走编排算法——单批单段形态 batch=0）
    u32 domainId{0};    // 域号（编排器 domainOfSeg 产物）
    u32 threadId{0};    // 池内全局槽位（域 base + 域内槽位；CCU 段恒 0）
    bool mainPlusAll{false};    // SYNC 两态：true = main 下发 + 全 sub 参与（MAIN_PLUS_SLAVES）
    VerbCall::Verb verb{VerbCall::COPY};
    u64 src{0};
    u64 dst{0};
    u64 size{0};
    u32 segIdx{0};      // 源段号
    // NHR_RUN 的 BY_CHANNEL 展开行：channelIdx（非 BY_CHANNEL 恒 0）——1 动词 ↔
    // 多 step 行（载荷 channelsPerRank 上界，devkit 恒 1 → 1 行）
    u32 channelIdx{0};
};

struct PipelineModel {
    HcclResult ret{HCCL_SUCCESS};
    std::string err;
    std::vector<PipelineStep> steps;
    std::vector<u32> batchTail;     // 各批尾段 segIdx（OrchestratorResult 转录）
    std::vector<OrchestratorResult::BarrierRec> barriers;   // 屏障记录（域内补 Post/跨域合取）
    ResourceClaim claim;            // Plan.claim 转录（资源画像）
    bool degraded{false};           // 线程不足降级标记（DryRun 足额池恒 false）
    u32 batches{0};                 // 批数
    // 域线程预算快照（切池口径推导依据：base_d = Σ_{<d} need_d）
    std::vector<u32> domainNeed;
};

// ---------------------------------------------------------------------------
// DryRun 主链：段序列（已 tier 展开——tier 骨架先 ExpandTieredSegments）→ 流水表
//
// 输入 = Compile 产物的段序列（Plan.segments，或 tier 展开段）；engine 决定 Emit
// 后端与线程策略表。合成足额池（threads 数 = Σ DomainThreadNeed）走编排器——
// 批结构/域切池/barriers 与生产编排算法同源（OrchestrateSegments 单源）。
// ---------------------------------------------------------------------------
inline PipelineModel DryRun(const std::vector<ActionSpec>& segs, Engine engine,
                            const ResourceClaim* claimIn = nullptr)
{
    PipelineModel model;
    if (segs.empty()) {
        model.ret = HCCL_E_PARA;
        model.err = "DryRun: empty segment sequence";
        return model;
    }
    // 域线程预算（编排器同式）：Σ DomainThreadNeed → 合成足额池
    std::vector<std::vector<u32> > domainRanks;
    std::vector<u32> domainOfSeg;
    domainRanks = GroupDomains(segs, &domainOfSeg);
    const u32 D = static_cast<u32>(domainRanks.size());
    std::vector<u32> needPerDomain(D, 0);
    u32 totalNeed = 0;
    for (u32 d = 0; d < D; ++d) {
        u32 need = 1;
        for (size_t i = 0; i < segs.size(); ++i) {
            if (domainOfSeg[i] == d) {
                const u32 t = DomainThreadTotalNeed(segs[i]);
                need = (need > t) ? need : t;
            }
        }
        needPerDomain[d] = need;
        totalNeed += need;
    }
    model.domainNeed = needPerDomain;

    OrchestratorRes res;
    for (u32 i = 0; i < totalNeed; ++i) {
        res.threads.push_back(static_cast<ThreadHandle>(0x1000U + i));   // 合成池（值仅占位）
    }
    // driver 闭包 = Emit 捕获单源件（CaptureVerbCalls——不自建第二 Emit）。
    // 段动词旁路收集：OrchestrateSegResult.calls 现成字段。
    // driver 闭包 = Emit 捕获单源件（CaptureVerbCalls——M0 §一裁定：DryRun 不走
    // exec 期真驱动链 DriveSegmentViaSink，Emit 步与 v5 生产链同源）。编排器单源
    // 产出批结构/域切池/barriers（REVIEW_CHECKLIST #16——DryRun 不自建第二份批划分）。
    const OrchestratorResult orch = OrchestrateSegments(
        segs, res, [engine](const ActionSpec& a, const HcommVerbRes& domainRes) {
            (void)domainRes;    // 捕获链零资源依赖（M0 §一裁定）
            OrchestrateSegResult out;
            out.calls = CaptureVerbCalls(a, engine);
            return out;
        });
    if (orch.ret != HCCL_SUCCESS) {
        model.ret = orch.ret;
        model.err = "DryRun orchestrate: " + orch.err;
        return model;
    }
    // 逐段动词收集（ret SUCCESS = 全段驱动完成；与 driver 内调用同函数——主锁
    // "DryRun 逐段动词序 == CaptureVerbCalls 直连"的直连侧，st R17 段常跑断言）
    std::vector<std::vector<VerbCall> > callsPerSeg(segs.size());
    for (size_t i = 0; i < segs.size(); ++i) {
        callsPerSeg[i] = CaptureVerbCalls(segs[i], engine);
    }

    // threadId 推演：查表 ThreadPolicyOf + 段内独立计数器（与 sink 同构）
    // 域 base = Σ_{<d} need（编排器切池同式——全局 index 口径）
    std::vector<u32> domainBase(D, 0);
    {
        u32 base = 0;
        for (u32 d = 0; d < D; ++d) {
            domainBase[d] = base;
            base += needPerDomain[d];
        }
    }
    const bool isCcu = (engine == Engine::CCU);
    u32 seq = 0;
    for (size_t i = 0; i < segs.size(); ++i) {
        const u32 d = domainOfSeg[i];
        const u32 base = domainBase[d];
        u32 readCount = 0;    // 段内 READ 计数（COUNTER_OFF_0——ThreadByCounter 同构）
        u32 writeCount = 0;   // 段内 WRITE 计数（COUNTER_OFF_1）
        for (const VerbCall& vc : callsPerSeg[i]) {
            PipelineStep st;
            st.segIdx = static_cast<u32>(i);
            st.domainId = d;
            st.verb = vc.verb;
            st.src = vc.src;
            st.dst = vc.dst;
            st.size = vc.size;
            if (isCcu) {
                st.threadId = 0;    // CCU 微码自驱动无线程概念（kCcuVerbMap 全 NA）
            } else {
                switch (ThreadPolicyOf(vc.verb, /*isCcu*/ false)) {
                    case ThreadPolicy::MAIN:
                        st.threadId = base;
                        break;
                    case ThreadPolicy::COUNTER_OFF_0:
                        st.threadId = base + readCount;
                        ++readCount;
                        break;
                    case ThreadPolicy::COUNTER_OFF_1:
                        st.threadId = base + writeCount + 1U;
                        ++writeCount;
                        break;
                    case ThreadPolicy::BY_CHANNEL: {
                        // NHR_RUN：channelsPerRank 循环上界 → 展开 C 行（M0 §二）；
                        // devkit 恒 1 → 1 行
                        const u32 ch = (vc.nhr.valid && vc.nhr.channelsPerRank > 0U)
                                           ? vc.nhr.channelsPerRank : 1U;
                        for (u32 c = 0; c < ch; ++c) {
                            PipelineStep chStep = st;
                            chStep.seq = seq;
                            chStep.threadId = base + c;
                            chStep.channelIdx = c;
                            model.steps.push_back(chStep);
                            ++seq;
                        }
                        continue;   // 本动词已按通道展开，跳过通用 push_back
                    }
                    case ThreadPolicy::MAIN_PLUS_SLAVES:
                        st.threadId = base;
                        st.mainPlusAll = true;
                        break;
                    case ThreadPolicy::NOT_WIRED:
                    case ThreadPolicy::NA:
                    default:
                        st.threadId = 0;
                        break;
                }
            }
            st.seq = seq;
            model.steps.push_back(st);
            ++seq;
        }
    }

    // 批号回填（step 所属批 = segIdx ≤ batchTail[b] 的最小 b——编排器口径）
    model.batchTail = orch.batchTail;
    model.batches = orch.batches;
    model.barriers = orch.barriers;
    model.degraded = orch.degraded;
    {
        std::vector<u32> segBatch(segs.size(), 0);
        for (size_t i = 0; i < segs.size(); ++i) {
            u32 b = 0;
            for (size_t t = 0; t < orch.batchTail.size(); ++t) {
                if (i <= orch.batchTail[t]) {
                    b = static_cast<u32>(t);
                    break;
                }
                b = static_cast<u32>(t);   // 超尾段（防御——批尾含末段时不达）
            }
            segBatch[i] = b;
        }
        for (size_t s = 0; s < model.steps.size(); ++s) {
            model.steps[s].batch = segBatch[model.steps[s].segIdx];
        }
    }
    if (claimIn != nullptr) {
        model.claim = *claimIn;
    }
    return model;
}

// ---------------------------------------------------------------------------
// 全链 dump：文本表（seq/batch/dom/thr/verb/几何 一行一步）——"各层单独 dump =
// 各积木自带函数；DryRun 全链 dump = 本件"（M0 §二 dump 面声明）
// ---------------------------------------------------------------------------
inline std::string RenderPipelineModel(const PipelineModel& m)
{
    std::string s = "PIPELINE steps:" + std::to_string(m.steps.size()) +
                    " batches:" + std::to_string(m.batches) +
                    " degraded:" + (m.degraded ? "1" : "0") + "\n";
    for (const PipelineStep& st : m.steps) {
        s += "  #" + std::to_string(st.seq) +
             " b" + std::to_string(st.batch) +
             " d" + std::to_string(st.domainId) +
             " t" + std::to_string(st.threadId) +
             (st.mainPlusAll ? "(main+all)" : "") +
             " " + VerbName(st.verb) +
             "(" + std::to_string(st.src) + "->" + std::to_string(st.dst) +
             "," + std::to_string(st.size);
        if (st.verb == VerbCall::READ || st.verb == VerbCall::WRITE) {
            s += ",peer" + std::to_string(static_cast<u32>(0));   // peer 槽位见 VerbCall（表渲染不冗余）
        }
        if (st.verb == VerbCall::NHR_RUN && st.channelIdx != 0U) {
            s += ",ch" + std::to_string(st.channelIdx);
        }
        s += ") seg" + std::to_string(st.segIdx) + "\n";
    }
    for (const auto& b : m.batchTail) {
        s += "  batchTail:" + std::to_string(b) + "\n";
    }
    for (const auto& b : m.barriers) {
        s += "  barrier afterSeg:" + std::to_string(b.segIdxAfter) +
             (b.domainInternal ? (" domainInternal:" + std::to_string(b.domainId))
                              : " cross-domain") + "\n";
    }
    return s;
}

} // namespace mc2_ops_hccl::refactor

#endif // REFACTOR_POC_PIPELINE_MODEL_H
