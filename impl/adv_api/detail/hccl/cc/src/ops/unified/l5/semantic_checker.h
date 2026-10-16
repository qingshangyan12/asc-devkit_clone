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
 * R17 · ②：语义后置条件 checker（设计债 MSCCLang 对照 10a——B1 语义正确性空档）
 *
 * 分工（M0_R17 §三）：Validate 查可支持性（拓扑/dtype/能力）→ 本件查语义正确性
 * （数据流闭包）→ ST checker 查两引擎等价 → golden 查旧实现基线。本件补的是
 * "新组合无 golden 基线"的空档：动作符号执行沿 VerbCall 序列 + DataLayout 检查
 * output 每片是否被正确来源覆盖。
 *
 * 挂钩形态（M0 必答 3 裁定）：独立诊断入口纯函数——不挂 Compile（真链路 Compile
 * 期 count=0 零几何恒平凡通过，挂钩价值为零，登记 P2 随"exec 期现值重建件"评估）。
 * 测试/golden 交叉锚直接喂；与 DryRun 同基建（核心函数第二参 = PipelineModel 的
 * perSegCalls 同源产物）。
 *
 * 符号执行模型（M0 §四定稿）：
 *   · 不做地址值域重叠仲裁——按"动词语义 + SPMD 镜像约定"符号化；
 *   · 镜像约定：X.WRITE(peer=Y, src=in[A]) ⇒ Y.cclbuf[X 槽] ≡ X.in[A]；本地读
 *     cclbuf 槽查贡献表，无贡献记录 = violation（"读未写区"——病灶反例捕获机制）；
 *   · SYMMEM：transport 声明优先于地址推断（in/out 同域别名）；
 *   · NHR_RUN：全 rank 联合符号执行（逐 rank CalcNhrSteps 同步推步，O(n²·logn·slice)）；
 *     skipStep0Tx 按"非 DMA"建模（isDmaRead 是通道协议知识，checker 无通道表，
 *     DMA 几何差登记 P2 边界——被跳槽位的上料拷贝不被读，语义等价）。
 *
 * 已知边界（不做不吹）：WRITE 远端消费序不验证；两引擎混排 Plan 不校验引擎间等价
 * （ST checker 职责）；SYNC_* 对数据流无影响（"序不越屏障"增强不做，登记）。
 */
#ifndef REFACTOR_POC_SEMANTIC_CHECKER_H
#define REFACTOR_POC_SEMANTIC_CHECKER_H

#include "action_spec.h"        // ActionSpec / DataLayout 访问器族
#include "backend_registry.h"   // Engine
#include "nhr_steps.h"          // CalcNhrSteps（NHR_RUN 步展开——纯函数单源）
#include "nhr_slice_geometry.h" // R18 · #11：展开形态贡献集核对（NhrSliceOf 同源）
#include <algorithm>            // R18 · #11：贡献多重集排序比对
#include "verb_capture.h"       // CaptureVerbCalls（便捷件的捕获单源）
#include "verb_sink.h"          // VerbCall

#include <map>
#include <set>
#include <string>
#include <vector>

namespace mc2_ops_hccl::refactor {

// ---------------------------------------------------------------------------
// 判定报告
// ---------------------------------------------------------------------------
struct SemanticReport {
    bool pass{true};
    std::vector<std::string> violations;
    // 断言拼接便捷件（violation 摘要——st 测试 Check 消息用）
    std::string errOf() const
    {
        if (violations.empty()) {
            return std::string();
        }
        return " [first violation: " + violations.front() + "]";
    }
};

// ---------------------------------------------------------------------------
// 符号区间表：字节区间 → 来源表达式（"哪个 rank 的哪片"）
// 离散化：动词边界并集做断点。来源符号 = "r<r>:<in|out|cclbuf>@<off>"（r=userRank）
// ⊕ 前缀 = 累积表达式（REDUCE/ReadReduce 语义）。
// ---------------------------------------------------------------------------
struct SymRange {
    u64 off{0};
    u64 len{0};
    std::string symbol;    // 来源表达式（空 = 未写）
};

class SymMemory {
public:
    // 写区间（覆盖式——后写覆盖先写）
    void Write(u64 off, u64 len, const std::string& sym)
    {
        EraseOverlap(off, off + len);
        ranges_.push_back(SymRange{off, len, sym});
    }
    // 累积写（REDUCE 语义：dst ← dst ⊕ src——贡献追加）
    void Reduce(u64 off, u64 len, const std::string& sym, std::vector<std::string>& violations,
                const std::string& tag)
    {
        std::vector<SymRange> covered = Query(off, off + len);
        if (covered.empty()) {
            violations.push_back(tag + ": REDUCE reads unwritten region [" +
                                 std::to_string(off) + "," + std::to_string(off + len) + ")");
            return;
        }
        for (const SymRange& r : covered) {
            if (r.symbol.empty()) {
                continue;
            }
            contributions_[off].push_back(sym);
        }
        EraseOverlap(off, off + len);
        ranges_.push_back(SymRange{off, len, "⊕(" + sym + ")"});
    }
    // 读区间核对：期望符号在覆盖区间上逐段匹配（未覆盖/来源不符 = violation）
    void CheckExpect(u64 off, u64 len, const std::string& expect,
                     std::vector<std::string>& violations, const std::string& tag) const
    {
        std::vector<SymRange> covered = Query(off, off + len);
        u64 coveredLen = 0;
        for (const SymRange& r : covered) {
            coveredLen += r.len;
            if (r.symbol != expect) {
                violations.push_back(tag + ": region [" + std::to_string(r.off) + "," +
                                     std::to_string(r.off + r.len) + ") = '" + r.symbol +
                                     "' expected '" + expect + "'");
            }
        }
        if (coveredLen < len) {
            violations.push_back(tag + ": region [" + std::to_string(off + coveredLen) + "," +
                                 std::to_string(off + len) + ") uncovered");
        }
    }
    // 读区间贡献查询（REDUCE 累积闭包核对）
    const std::vector<std::string>& Contributions(u64 off) const
    {
        static const std::vector<std::string> kEmpty;
        const auto it = contributions_.find(off);
        return (it != contributions_.end()) ? it->second : kEmpty;
    }
    std::vector<SymRange> Query(u64 lo, u64 hi) const
    {
        std::vector<SymRange> out;
        for (const SymRange& r : ranges_) {
            if (r.off < hi && (r.off + r.len) > lo) {
                out.push_back(r);
            }
        }
        return out;
    }

private:
    void EraseOverlap(u64 lo, u64 hi)
    {
        std::vector<SymRange> next;
        for (const SymRange& r : ranges_) {
            if (r.off >= hi || (r.off + r.len) <= lo) {
                next.push_back(r);   // 无交
                continue;
            }
            // 交叠：切割保留两残段
            if (r.off < lo) {
                next.push_back(SymRange{r.off, lo - r.off, r.symbol});
            }
            if (r.off + r.len > hi) {
                next.push_back(SymRange{hi, r.off + r.len - hi, r.symbol});
            }
        }
        ranges_ = next;
    }
    std::vector<SymRange> ranges_;
    std::map<u64, std::vector<std::string> > contributions_;
};

// ---------------------------------------------------------------------------
// 符号构造便捷件（M0 §四"符号值"定稿）：本地区间 → "r<user>:<域>@<off>" 表达式
// ---------------------------------------------------------------------------

// 本地域符号：src 落在哪个域（in/out/cclbuf）按基址步进推导——离散步进匹配
inline std::string LocalSym(const ActionSpec& a, u64 off, u32 userRank)
{
    // in 域：inBaseOff + rpt*inRptStride + inStride*r（步进网格匹配）
    const u64 inStride = (a.layout.inputSliceStride != 0U) ? a.layout.inputSliceStride
                                                           : a.layout.outputSliceStride;
    if (inStride != 0U && off >= a.layout.inBaseOff) {
        const u64 d = off - a.layout.inBaseOff;
        if (d % inStride == 0U) {
            return "r" + std::to_string(userRank) + ":in@" + std::to_string(off);
        }
    }
    if (a.layout.sliceSize != 0U && off >= a.layout.scratchBaseOff) {
        const u64 d = off - a.layout.scratchBaseOff;
        if (d % a.layout.sliceSize == 0U) {
            return "r" + std::to_string(userRank) + ":cclbuf@" + std::to_string(off);
        }
    }
    return "r" + std::to_string(userRank) + ":out@" + std::to_string(off);
}

// READ 的对端源符号（M0 §四 SPMD 镜像约定）：
//   SYMMEM  → 对端 out 自身片（in/out 同域别名）→ "r<p>:out@<src>"
//   中转    → 对端 cclbuf 上料槽：镜像约定穿透一层——对端槽内容 = 对端上料 COPY
//             产物 = 对端 in[InOffset(peerAlgRank)]（SPMD 全员同序动词，我方视角
//             经镜像断言对端必发）→ 符号记 "r<p>:in@InOffset(peerAlg)"
//   peerAlgRank 由 dst=OutOffset(r) 反查（BuildArgs peers 序：dst 逐片落位）
inline std::string ReadSrcSymbol(const ActionSpec& a, const VerbCall& vc)
{
    if (a.transport == TransportPolicy::SYMMEM) {
        return "r" + std::to_string(vc.peer) + ":out@" + std::to_string(vc.src);
    }
    const u32 n = a.domain.Size();
    for (u32 r = 0; r < n; ++r) {
        if (a.layout.OutOffset(r) == vc.dst) {
            return "r" + std::to_string(vc.peer) + ":in@" +
                   std::to_string(a.layout.InOffset(r));
        }
    }
    return "r" + std::to_string(vc.peer) + ":cclbuf@" + std::to_string(vc.src);
}

// REDUCE 的 cclbuf 槽符号（AICPU 逐条形态：src=SB+S*r → 槽号 r）
inline std::string CclbufSlotSymbol(const ActionSpec& a, u64 srcOff)
{
    const u64 S = a.layout.sliceSize;
    if (S != 0U && srcOff >= a.layout.scratchBaseOff) {
        const u64 slot = (srcOff - a.layout.scratchBaseOff) / S;
        return "slot" + std::to_string(slot);
    }
    return "cclbuf@" + std::to_string(srcOff);
}

// COPY 的来源符号（本 rank 视角：src 区间 → 本地域表达式）
inline std::string Src(const ActionSpec& a, u64 srcOff, u32 userRank)
{
    return LocalSym(a, srcOff, userRank);
}

// ---------------------------------------------------------------------------
// 核心纯函数：段序列 + 逐段动词序 → 后置条件判定（零 Emit 依赖，可单测直喂）
// ---------------------------------------------------------------------------
inline SemanticReport SemanticCheck(const std::vector<ActionSpec>& segs,
                                    const std::vector<std::vector<VerbCall> >& callsPerSeg)
{
    SemanticReport rep;
    if (segs.size() != callsPerSeg.size()) {
        rep.pass = false;
        rep.violations.push_back("segment/calls count mismatch: " +
                                 std::to_string(segs.size()) + " vs " +
                                 std::to_string(callsPerSeg.size()));
        return rep;
    }
    for (size_t i = 0; i < segs.size(); ++i) {
        const ActionSpec& a = segs[i];
        const std::vector<VerbCall>& calls = callsPerSeg[i];
        const std::string tag = "seg" + std::to_string(i);
        const u32 n = a.domain.Size();
        const u32 me = a.domain.MyAlgRank();

        if (a.kind == ActionKind::CollectAll) {
            // ---- CollectAll 后置条件：∀r∈域 out[OutOffset(r)] ≡ r 的 input 片 ----
            SymMemory outMem;
            const u64 myUser = a.domain.ranks[me < n ? me : 0];
            for (const VerbCall& vc : calls) {
                switch (vc.verb) {
                    case VerbCall::COPY:
                        outMem.Write(vc.dst, vc.size, Src(a, vc.src, myUser));
                        break;
                    case VerbCall::READ: {
                        // READ(peer)：dst=本地 out 落位 ← src=对端片（SYMMEM：对端
                        // out 的自身片；中转：对端 cclbuf 上料槽）。src 几何核对
                        //（病灶反例② 捕获：符号从 dst 反查推导会吞掉 src 错位——
                        // src 期望式独立核对：peerAlg 由 dst=OutOffset(r) 反查后
                        // ScratchOffset(r) 中转 / OutOffset(r) SYMMEM）
                        u32 peerAlg = 0xFFFFFFFFU;
                        for (u32 r = 0; r < n; ++r) {
                            if (a.layout.OutOffset(r) == vc.dst) {
                                peerAlg = r;
                                break;
                            }
                        }
                        if (peerAlg != 0xFFFFFFFFU) {
                            const u64 expectSrc = (a.transport == TransportPolicy::SYMMEM)
                                                      ? a.layout.OutOffset(peerAlg)
                                                      : a.layout.ScratchOffset(peerAlg, n);
                            if (vc.src != expectSrc) {
                                rep.violations.push_back(
                                    tag + ": READ src " + std::to_string(vc.src) +
                                    " != expected " + std::to_string(expectSrc) +
                                    " (geometry misaligned)");
                            }
                        }
                        const std::string srcSym = ReadSrcSymbol(a, vc);
                        outMem.Write(vc.dst, vc.size, srcSym);
                        break;
                    }
                    case VerbCall::BROADCAST: {
                        // CCU AG：一条广播含本 rank 落位 + 全对端片
                        for (u32 r = 0; r < n; ++r) {
                            const u64 sz = a.layout.SliceSizeOf(r, n);
                            const u64 dst = a.layout.OutOffset(r);
                            outMem.Write(dst, sz, "r" + std::to_string(a.domain.ranks[r]) +
                                                       ":in@" + std::to_string(a.layout.InOffset(r)));
                        }
                        break;
                    }
                    default:
                        break;   // SYNC/WRITE/POST_COPY 对 AG 后置条件无贡献
                }
            }
            // 逐片核对
            for (u32 r = 0; r < n; ++r) {
                const u64 sz = a.layout.SliceSizeOf(r, n);
                const std::string expect = "r" + std::to_string(a.domain.ranks[r]) + ":in@" +
                                           std::to_string(a.layout.InOffset(r));
                outMem.CheckExpect(a.layout.OutOffset(r), sz, expect, rep.violations, tag);
            }
        } else if (a.kind == ActionKind::ReduceSlice) {
            if (a.pattern == CommPattern::NHR) {
                // ---- ReduceSlice·NHR 后置条件：全 rank 联合符号执行 ----
                // （M0 §四：逐 rank CalcNhrSteps 同步推步——步表是 me 的纯函数；
                //   本 rank 视角核对 cclbuf 累积贡献 = 全部对端 + 本 rank 的 me 片）
                // R18 · #11：展开形态先判（SLICE_REDUCE 存在 = 展开形态——动词级
                // 判据非 pattern 级，两形态都判是对称化验收锚）。展开形态核对：
                // SLICE_REDUCE 贡献集（(peer, slot) 对）== CalcNhrSteps 全步
                // tx/rx 展开（等价锁判据——M0 §三-3）；CH_SYNC 数据流无影响。
                bool hasSliceReduce = false;
                for (const VerbCall& vc : calls) {
                    if (vc.verb == VerbCall::SLICE_REDUCE) {
                        hasSliceReduce = true;
                        break;
                    }
                }
                const VerbCall* run = nullptr;
                for (const VerbCall& vc : calls) {
                    if (vc.verb == VerbCall::NHR_RUN) {
                        run = &vc;
                    }
                }
                if (hasSliceReduce) {
                    // 展开形态：贡献集等式（几何经公共件 NhrSliceOf——与 Emit/sink
                    // 同源，REVIEW #16；对称化验收：无 pattern 分叉的动词级判定）
                    // 动词内的 (peerWR 槽序, sliceIdx) 贡献对 vs 步表推导全集
                    std::vector<std::pair<u32, u32> > verbPairs;
                    for (const VerbCall& vc : calls) {
                        if (vc.verb != VerbCall::SLICE_REDUCE || !vc.nhr.valid) {
                            continue;
                        }
                        const std::vector<NhrStepInfo> steps =
                            CalcNhrSteps(vc.nhr.myAlgRank, vc.nhr.rankNum);
                        if (vc.nhr.stepIdx >= steps.size()) {
                            rep.violations.push_back(tag + ": SLICE_REDUCE stepIdx out of range");
                            continue;
                        }
                        const NhrStepInfo& st = steps[vc.nhr.stepIdx];
                        if (vc.nhr.sliceIdx >= st.nSlices) {
                            rep.violations.push_back(tag + ": SLICE_REDUCE sliceIdx out of range");
                            continue;
                        }
                        // 贡献对：WR 侧 (sendTo, tx) + RD 侧 (recvFrom, rx)——
                        // 方向无关动词两侧都计（执行端自判其一，语义两侧等价）
                        verbPairs.push_back(std::make_pair(st.sendTo, st.txSliceIdxs[vc.nhr.sliceIdx]));
                        verbPairs.push_back(std::make_pair(st.recvFrom, st.rxSliceIdxs[vc.nhr.sliceIdx]));
                    }
                    // 步表推导全集（同公式独立展开）
                    std::vector<std::pair<u32, u32> > expectPairs;
                    for (const NhrStepInfo& st : CalcNhrSteps(me, n)) {
                        for (u32 i = 0; i < st.nSlices; ++i) {
                            expectPairs.push_back(std::make_pair(st.sendTo, st.txSliceIdxs[i]));
                            expectPairs.push_back(std::make_pair(st.recvFrom, st.rxSliceIdxs[i]));
                        }
                    }
                    if (verbPairs.size() != expectPairs.size()) {
                        rep.violations.push_back(tag + ": SLICE_REDUCE count " +
                                                 std::to_string(verbPairs.size()) + " != expected " +
                                                 std::to_string(expectPairs.size()));
                    } else {
                        // 集合相等（多重集）
                        std::vector<std::pair<u32, u32> > a = verbPairs, b = expectPairs;
                        std::sort(a.begin(), a.end());
                        std::sort(b.begin(), b.end());
                        if (a != b) {
                            rep.violations.push_back(tag + ": SLICE_REDUCE contribution set != step-table expansion");
                        }
                    }
                    // 上料/归位序位核对（分口径——R18 全删轮补：AICPU 形态含
                    // COPY（上料×n+归位×1）；CCU 形态无 COPY（步进流本体在 server
                    // 帧内承载，动词序仅 SYNC+CH_SYNC+SLICE_REDUCE）。判据动词级
                    //（序内有无 COPY，#28——与 ValidateVerbSegment 展开分支同式）
                    u64 upCount = 0;
                    bool seenStep = false;
                    bool hasCopy = false;
                    const VerbCall* backCopy = nullptr;
                    for (const VerbCall& vc : calls) {
                        if (vc.verb == VerbCall::SLICE_REDUCE) {
                            seenStep = true;
                        } else if (vc.verb == VerbCall::COPY) {
                            hasCopy = true;
                            if (!seenStep) {
                                ++upCount;
                            } else if (backCopy == nullptr) {
                                backCopy = &vc;
                            }
                        }
                    }
                    if (hasCopy) {
                        if (upCount != n) {
                            rep.violations.push_back(tag + ": NHR upload COPY count " +
                                                     std::to_string(upCount) + " != n " +
                                                     std::to_string(n));
                        }
                        if (backCopy == nullptr) {
                            rep.violations.push_back(tag + ": NHR back-copy missing");
                        } else if (backCopy->dst != a.layout.OutOffset(me)) {
                            rep.violations.push_back(tag + ": NHR back-copy dst mismatch");
                        }
                    }
                    // 展开形态 return（跳过下方宏形态判定）
                    rep.pass = rep.violations.empty();
                    // 本段 NHR 判定完成——外层循环 continue 由 kind 分支结构承担
                    // （此处用 goto 太重——下方宏形态块用 run==nullptr 判跳过）
                }
                if (hasSliceReduce) {
                    // 展开形态已判（上方贡献集等式）——宏形态块跳过
                } else if (run == nullptr) {
                    rep.violations.push_back(tag + ": NHR segment missing NHR_RUN verb");
                } else {
                    // 11 标量载荷几何核对挂 P2（CCU 载荷本就不在 VerbCall——本轮
                    // 核对动词级：上料/归位/NHR_RUN 存在性 + 步表闭包）
                    // 我方 me 片的最终值 = ⊕_r in_r[me 片]：核对方式——各 rank 的步表
                    // tx 序覆盖全集（步表是纯函数，几何核对 = 步公式核对）
                    std::vector<u32> myTxUnion;
                    for (u32 r = 0; r < n; ++r) {
                        const std::vector<NhrStepInfo> steps = CalcNhrSteps(r, n);
                        std::vector<u32> txAll;
                        for (const NhrStepInfo& st : steps) {
                            for (u32 s = 0; s < st.nSlices; ++s) {
                                txAll.push_back(st.txSliceIdxs[s]);
                            }
                        }
                        if (r == me) {
                            myTxUnion = txAll;
                        }
                        // 任意 rank 的 tx 并集 = 除自身外全集（NHR 步公式闭包）
                        std::set<u32> txSet(txAll.begin(), txAll.end());
                        if (txSet.size() + 1U != n) {
                            rep.violations.push_back(tag + ": rank " + std::to_string(r) +
                                                     " tx union size " + std::to_string(txSet.size()) +
                                                     " != n-1 " + std::to_string(n - 1U));
                        }
                    }
                    (void)myTxUnion;
                    // 上料/归位几何核对（动词级，按引擎形态）：
                    //   AICPU（10 动词形态）：上料 COPY×n（src=inBase+stride*r →
                    //     dst=SB+S*r）+ 归位 COPY×1（src=SB+S*me → dst=OutOffset(me)）
                    //   CCU（3 动词形态）：上料/归位在 server 帧内展开（步进流整段
                    //     一帧承载）——动词级只核 NHR_RUN 存在性（上方已判）
                    u64 copyCount = 0;
                    for (const VerbCall& vc : calls) {
                        if (vc.verb == VerbCall::COPY) {
                            ++copyCount;
                        }
                    }
                    if (copyCount > 0U) {
                        // AICPU 形态：COPY 数 = n（上料）+ 1（归位）。上料/归位判定用
                        // **序位**（Emit 序实证 backend_aicpu_reduce_slice.h :74-137
                        // 上料 COPY×n 在 NHR_RUN 前 / :156-160 归位 COPY×1 在后）——
                        // 值域判定在 tier 半片形态有歧义（ForChunk 后 inBase/outBase/
                        // SB 可三者重合，上料 r=0 与归位逐字段相等——探针 seg2 实证），
                        // 序位是唯一稳定判据；几何另核对（归位 dst==OutOffset(me)）
                        u64 upCount = 0;
                        bool seenRun = false;
                        const VerbCall* backCopy = nullptr;
                        for (const VerbCall& vc : calls) {
                            if (vc.verb == VerbCall::NHR_RUN) {
                                seenRun = true;
                            } else if (vc.verb == VerbCall::COPY) {
                                if (!seenRun) {
                                    ++upCount;   // NHR_RUN 前：上料
                                } else if (backCopy == nullptr) {
                                    backCopy = &vc;   // NHR_RUN 后首条：归位
                                }
                            }
                        }
                        if (upCount != n) {
                            rep.violations.push_back(tag + ": NHR upload COPY count " +
                                                     std::to_string(upCount) + " != n " +
                                                     std::to_string(n));
                        }
                        if (backCopy == nullptr) {
                            rep.violations.push_back(tag + ": NHR back-copy (scratch[me]→out[me]) missing");
                        } else if (backCopy->dst != a.layout.OutOffset(me)) {
                            rep.violations.push_back(tag + ": NHR back-copy dst " +
                                                     std::to_string(backCopy->dst) +
                                                     " != OutOffset(me) " +
                                                     std::to_string(a.layout.OutOffset(me)));
                        }
                    }
                }
            } else {
                // ---- ReduceSlice·mesh 后置条件：out[me] ≡ ⊕_r in_r[me 片]，贡献数 = n ----
                // AICPU：COPY(InOffset(me)→OutOffset(me)) 落位 + REDUCE×(n−1)（src=cclbuf
                // 槽 r=对端 WRITE 的 in[me 片]）+ WRITE×(n−1)（镜像贡献记录）
                SymMemory outMem;
                u64 meOutOff = a.layout.OutOffset(me);
                u64 meSlice = a.layout.SliceSizeOf(me, n);
                std::vector<std::string> reduceSrcs;
                bool hasCopyInit = false;
                u64 readCount = 0;
                u64 writeCount = 0;
                u64 reduceCount = 0;
                // 前置形态扫描：REDUCE 恰 1 条且 src=scratchBaseOff 起点 → CCU 全槽
                // 折叠形态（槽语义跨 cclbuf+input 两域，≤8P 本 rank 半边直读 input）
                bool ccuFoldForm = false;
                for (const VerbCall& vc : calls) {
                    if (vc.verb == VerbCall::REDUCE) {
                        ++reduceCount;
                        if (vc.src == a.layout.scratchBaseOff) {
                            ccuFoldForm = true;
                        }
                    }
                }
                ccuFoldForm = ccuFoldForm && (reduceCount == 1U);
                reduceCount = 0;
                for (const VerbCall& vc : calls) {
                    switch (vc.verb) {
                        case VerbCall::WRITE: {
                            ++writeCount;   // AICPU 镜像贡献（我写对端 cclbuf 的 me 槽）
                            break;
                        }
                        case VerbCall::COPY:
                            if (vc.dst == meOutOff && vc.size == meSlice) {
                                hasCopyInit = true;
                                outMem.Write(vc.dst, vc.size, Src(a, vc.src, a.domain.ranks[me]));
                            }
                            break;
                        case VerbCall::READ: {
                            ++readCount;   // CCU：读对端 input 的 me 片 → cclbuf 槽
                            reduceSrcs.push_back(ReadSrcSymbol(a, vc));
                            break;
                        }
                        case VerbCall::REDUCE: {
                            ++reduceCount;
                            // AICPU 逐条（src=SB+S·r → 槽符号）；CCU 单条全槽折叠
                            // （src=scratchBaseOff 起点，含本 rank input 直读半边）
                            const std::string srcSym =
                                ccuFoldForm ? std::string("ccuFold(allSlots)")
                                            : CclbufSlotSymbol(a, vc.src);
                            reduceSrcs.push_back(srcSym);
                            if (srcSym == "ccuFold(allSlots)") {
                                // CCU 全槽折叠：槽语义跨 cclbuf+input 两域（≤8P 本 rank
                                // 半边 Phase3 直读 input 不经 cclbuf）——不走 outMem 累积
                                // 判定（"reads unwritten" 误报），折叠首写 + Σ 式由
                                // READ/REDUCE 计数闭包承载
                                outMem.Write(vc.dst, vc.size, "⊕" + srcSym);
                            } else {
                                outMem.Reduce(vc.dst, vc.size, srcSym, rep.violations, tag);
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
                // 贡献闭包核对（形态判定 → 各自的 Σ 式）：
                //   CCU：REDUCE×1（全槽折叠含本 rank 半边）+ READ×(n−1)
                //   AICPU：COPY 落位 + WRITE×(n−1) 镜像 + REDUCE×(n−1) 累积
                if (reduceCount == 1U) {
                    if (readCount != n - 1U) {
                        rep.violations.push_back(tag + ": RS CCU READ count " +
                                                 std::to_string(readCount) + " != n-1 " +
                                                 std::to_string(n - 1U) + " (Σ not closed)");
                    }
                } else if (writeCount == n - 1U && reduceCount == n - 1U) {
                    if (!hasCopyInit) {
                        rep.violations.push_back(tag + ": RS AICPU initial COPY (in[me]→out[me]) missing");
                    }
                } else {
                    rep.violations.push_back(tag + ": RS reduce contributions " +
                                             std::to_string(reduceCount) + " writes " +
                                             std::to_string(writeCount) + " != n-1 " +
                                             std::to_string(n - 1U) + " (Σ not closed)");
                }
            }
        }
        // ExchangeAll/LocalCopy/LocalReduce：R17 交付面外（kind 未交付后端）——跳过
        // 不发明后置条件（M0 §四边界同款"不做不吹"）
    }
    rep.pass = rep.violations.empty();
    return rep;
}

// ---------------------------------------------------------------------------
// 便捷件：Plan + engine → 逐段捕获 → 核心函数（与 DryRun 同一捕获单源）
// ---------------------------------------------------------------------------
inline SemanticReport SemanticCheckPlan(const Plan& plan, Engine engine)
{
    std::vector<std::vector<VerbCall> > callsPerSeg(plan.segments.size());
    for (size_t i = 0; i < plan.segments.size(); ++i) {
        callsPerSeg[i] = CaptureVerbCalls(plan.segments[i], engine);
    }
    return SemanticCheck(plan.segments, callsPerSeg);
}

} // namespace mc2_ops_hccl::refactor

#endif // REFACTOR_POC_SEMANTIC_CHECKER_H
