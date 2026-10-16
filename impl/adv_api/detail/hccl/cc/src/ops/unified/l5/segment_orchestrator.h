/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * R14 · A 形态执行期并发编排器（挂账 #6 兑现——AICPU 消费方半边）
 *
 * 定位：R9 交付的段序列 sync 位（tier_expand.h A 交错形态：批内 post=F / 批边界
 * post=T）在此获得执行期消费——"按 sync 位编排的并发执行器"。R9 裁决"执行期并发
 * 解锁挂 P3"（当时消费方串行仍语义正确）的本轮兑现件。
 *
 * 编排器三件事（方案文档 :1587 / M0_R14 §五规格）：
 *   ① 派发异步化按域绑线程组——域数通常 2（快+慢），从全局线程池按域分派子组
 *      （域 0 = threads[0..n0)，域 1 = threads[n0..n0+n1)，…首现序），driver 闭包
 *      注入域子束（threads 切片 + 全通道表——两域 peer 集不相交，通道天然按域分）
 *   ② in-flight 集维护——批划分 = 扫 postBarrier（post=T 段收批）；批内 post=F 的
 *      段下发后**补域内 Post 两段序**（段自身无 post 屏障动词，批窗关闭语义由
 *      编排器承载——与 HcommVerbSink::DoSyncPost 同构裸原语）
 *   ③ 合取屏障（§3.4.5：批成员区间边界屏障）——批尾 D≥2 时域 mains 互 notify
 *      交叉对（先 record 全下发、后 wait 全下发——M0 §三防御序）
 *
 * 归属：l5/（引擎头消费面——裸调 HcommThreadNotifyRecordOnThread/
 * HcommThreadNotifyWaitOnThread 做屏障，与 hcomm_verb_sink.h 同级理由：真原语
 * 外露，不进 inc/ 值对象层；host 自检经 shim 桩链接同 aicpu_consumer 链）。
 *
 * 机制依据（M0_R14 §三，考古反证 + 复核 A 项补证）：
 *   · hcomm 线程 notify 是**异步下发**（指令进目标线程队列即返回）——存量
 *     PostSyncInterThreads "先 wait 全部后 record 全部"若 host 阻塞即死锁，
 *     在产反证（ins_temp_reduce_scatter_mesh_1D.cc:124/:130 每轮循环）。
 *   · notify 是 **V/P 配对计数信号量**——存量 KernelRun 逐轮复用同 idx 对不
 *     失效，反证非 sticky 事件；配对完成后槽可无限复用（跨批屏障复用槽 0 安全）。
 *   · 两段循环序照抄 DoSyncPre/DoSyncPost（hcomm_verb_sink.h:354-413 真源
 *     alg_data_trans_wrapper.cc:1010-1078）——不做逐对 record-wait 交错。
 *   · **下发者模型（复核 A 项定稿）**：全部 hcomm 原语由 **host 单线程下发**——
 *     存量 Pre/PostSyncInterThreads 的调用点全在 host 的 KernelRun/executor 代码
 *     路径（wrapper.cc:1033-1074 的四循环均 host for 循环直调；"OnThread" 形参
 *     thread 指明**指令落在哪个线程队列**，而非"由该线程发起调用"——HcommReadOnThread
 *     同理：host 调用把 DMA 指令挂到从线程队列后即返回，host 侧同步等待另有
 *     HcommThreadJoin（hcomm_primitives_dl.h:88，RS 64bit 分支
 *     ins_temp_reduce_scatter_mesh_1D.cc:137 用）。因此批尾交叉对的四条指令全部
 *     由 host 顺序下发（record×2 先于 wait×2，全异步即返回）——无"t4 自己发
 *     record 时 t0 还没等到"的时序障碍。
 *
 * 降级路径（M0 §二：双域并发需 Σ need 而池不足 → 串行重放，不拒错）：池 <
 * Σ need → degraded=true + warnings 记录 + 逐段全局束驱动（= 既有串行 for
 * 语义）。单域内部 need > 子组（不降级）由 sink 既有 "exceeds thread count"
 * Fail 透传（不吞错）。R19 · #12 后 parallel/split 配方恒按 Σneed 申报
 * （CONCURRENT_SUM 查表——claim 与本编排器实需一致，降级主因回归真机池不足
 * 而非申报口径错配）。
 *
 * 纯新增纪律：不含 TierParams/A-B 展开（消费方 blob 可达面的承载是 P2，M0 §四）；
 * 输入是**已展开的段序列**（ActionSpec + sync 位齐全）；不改 aicpu_consumer.h
 * 既有串行循环（消费方以 env 门控分叉调用，WP-2）。
 */
#ifndef REFACTOR_POC_SEGMENT_ORCHESTRATOR_H
#define REFACTOR_POC_SEGMENT_ORCHESTRATOR_H

#include "action_spec.h"    // ActionSpec / SyncSpec（段序列与 sync 位）
#include "verb_sink.h"      // VerbCall（driver 闭包的动词载荷类型）
#include "hcomm_verb_sink.h"   // HcommVerbRes（driver 闭包的资源束类型——域子束与
                               // 全局束同型；include 即见 ThreadHandle/ChannelInfo）

#include "alg_param.h"      // hcomm 原语 extern 声明（ThreadHandle/notify 槽）

#include <algorithm>
#include <functional>     // std::function（SegmentDriver 闭包）
#include <map>
#include <string>
#include <vector>

namespace mc2_ops_hccl::refactor {

// ---------------------------------------------------------------------------
// 输入面：段序列 + 全局资源束 + 段驱动闭包
//
// SegmentDriver：消费方注入的"如何驱动一段"——(段值, 域子束) → 段动词序。
// aicpu_consumer 的注入形态 = InstantiateSegment/InstantiateRsSegment（单源重建）
// + BuildArgs + backend Emit 进 HcommVerbSink(res=域子束)。子束 threads 已切好，
// sink 的 READ/WRITE 计数在子束内起算（域组线程从 threads[0] 起算——与全局池
// 串行时的计数语义同构，见 M0 §一组 1：域 0 组以 t0 为 main 即 threads[0]）。
// ---------------------------------------------------------------------------
struct OrchestratorRes {
    std::vector<ThreadHandle> threads;                    // 全局池（threads[0]=全局 main）
    std::map<u32, std::vector<ChannelInfo>> channels;     // 全通道表（按 peer 查——两域
                                                          // peer 集不相交，共享无害）
};

struct OrchestrateSegResult {
    HcclResult ret{HCCL_SUCCESS};
    std::string err;                       // 失败原因（ret != SUCCESS 时非空）
    std::string trace;                     // 段动词 trace（TraceSink 口径，诊断/对拍）
    std::vector<VerbCall> calls;           // 段动词序（等价判据的搬运集合输入）
};

using SegmentDriver = std::function<OrchestrateSegResult(
    const ActionSpec& seg, const HcommVerbRes& domainRes)>;

// ---------------------------------------------------------------------------
// 输出面：编排结果（诊断/测试断言用——与 UnifiedExecResult 同族）
// ---------------------------------------------------------------------------
struct OrchestratorResult {
    HcclResult ret{HCCL_SUCCESS};
    std::string err;
    u32 segmentsRun{0};                    // 成功驱动的段数
    u32 batches{0};                        // 批数（post=T 收批——串行段序列 = 段数）
    bool degraded{false};                  // 线程不足降级（串行重放路径）
    std::vector<std::string> warnings;     // 降级/异常的可观测记录（真链路由 exec
                                           // 薄壳转 HCCL_WARNING——shim 面无日志宏）
    std::vector<std::string> perSegTrace;  // 逐段 trace（与串行 for 的 perSegTrace
                                           // 同口径——等价对拍断言的锚）
    std::vector<u32> batchTail;            // 各批尾段 segIdx（批结构断言用）
    std::vector<u32> domainOfSeg;          // 逐段域号（域映射断言用）
    // 编排屏障指令记录（下发序；driver 产段动词前编排器不产指令——屏障都在段序
    // 间隙，位置由 segIdx 隐含，此表仅供测试断言屏障存在性与顺序）
    struct BarrierRec {
        u32 segIdxAfter{0};                // 屏障跟在第几段之后（u32Max=末尾后）
        bool domainInternal{false};        // true=域内补 Post（批内 post=F 段收尾）
        u32 domainId{0};                   // 域内补 Post 所属域；跨域合取=0xFFFFFFFF
    };
    std::vector<BarrierRec> barriers;
};

// ---------------------------------------------------------------------------
// 域线程预算（R15 · ③ pattern-aware 化——M0_R15 必答 F/H 定稿）：委托
// action_spec.h 的 DomainThreadTotalNeed（单源：与 Σ 申报 CalcConcurrencySlaveClaim
// 同一公式，ST R15-③ 断言锁两处一致）。R14 首版公式（RS 恒 n / AG 恒 n−1）对 NHR
// 段错算（NHR threadNum=1×channelsPerRank≠n）——R15 前 NHR 段不可达（序列化拒收）
// ，行为影响面 = 零。
// ---------------------------------------------------------------------------
inline u32 DomainThreadNeed(const std::vector<ActionSpec>& segs, size_t domainFirst,
                            size_t domainCount)
{
    u32 need = 1;
    for (size_t i = domainFirst; i < domainFirst + domainCount; ++i) {
        const u32 t = DomainThreadTotalNeed(segs[i]);
        need = (need > t) ? need : t;
    }
    return need;
}

// ---------------------------------------------------------------------------
// 编排主体：段序列 → 并发批窗执行。
//
// 算法（M0 §五判据的实现面）：
//   1. 域分组：domain.ranks 序列相等去重（首现序）——同域段的 ranks 向量逐项相等
//   2. 线程预算：Σ DomainThreadNeed；池不足 → 降级串行（逐段全局束，不编排）
//   3. 批划分：i 段 post=T → i 收批（尾段 post=F → 补一个"末批"——不静默丢段）
//   4. 批内并发：每段发 driver(段, 域子束)；段间无屏障（sync=F 的语义）
//   5. 批内 post=F 段收尾：域内补 Post 两段序（DoSyncPost 同构裸原语）
//   6. 批尾 D≥2：域 mains 互 notify 交叉对（先 record 后 wait，M0 §三防御序）
//   7. D==1：批 = 段独立执行（post=T 段自带 DoSyncPost——域内封闭，无跨域对）
//
// 线程分配（首现序切池）：域 d 子组 = threads[base_d .. base_d+need_d)，
// base_d = Σ need_{<d}；域 mains = 子组 threads[0]。域 0 的 main = 全局 threads[0]
//（与全局 main 重合——M0 §三：域组封闭，槽空间按线程独立，不撞）。
// ---------------------------------------------------------------------------
inline OrchestratorResult OrchestrateSegments(const std::vector<ActionSpec>& segs,
                                               const OrchestratorRes& res,
                                               const SegmentDriver& driver)
{
    OrchestratorResult out;
    if (segs.empty()) {
        out.ret = HCCL_E_PARA;
        out.err = "OrchestrateSegments: empty segment sequence";
        return out;
    }
    if (driver == nullptr) {
        out.ret = HCCL_E_PARA;
        out.err = "OrchestrateSegments: null driver";
        return out;
    }

    // 1. 域分组（domain.ranks 相等去重，首现序——R16 · 8c：GroupDomains 唯一实现点，
    // 与 Σ 申报 CalcConcurrencySlaveClaim 同一实现共享）
    std::vector<std::vector<u32> > domainRanks = GroupDomains(segs, &out.domainOfSeg);
    const u32 D = static_cast<u32>(domainRanks.size());

    // 2. 线程预算与切池；不足降级（M0 §二：Σ need vs 池大小）
    u64 totalNeed = 0;
    std::vector<u32> needPerDomain;
    for (size_t d = 0; d < domainRanks.size(); ++d) {
        size_t first = 0;
        size_t count = 0;
        for (size_t i = 0; i < segs.size(); ++i) {
            if (out.domainOfSeg[i] == static_cast<u32>(d)) {
                if (count == 0) {
                    first = i;
                }
                ++count;
            }
        }
        const u32 nd = DomainThreadNeed(segs, first, count);
        needPerDomain.push_back(nd);
        totalNeed += nd;
    }
    // 单域预检（M0 复核 B 项）：need[d] > 池大小（连单域都喂不饱）时**预检降级**
    // ——不硬跑（sink 的 "exceeds thread count" Fail 在动词越界那一条才触发，
    // 半途 Fail 留悬空动词；预检降级走串行 = 既有 for 循环的 Fail 语义与时机
    // 完全一致——既有行为零变化原则。真"池 ≥ 单域 need"的形态走正常并发切池）。
    for (u32 d = 0; d < D; ++d) {
        if (static_cast<u64>(needPerDomain[d]) > static_cast<u64>(res.threads.size())) {
            out.degraded = true;
            out.warnings.push_back("domain " + std::to_string(d) + " needs " +
                                   std::to_string(needPerDomain[d]) +
                                   " threads > pool " + std::to_string(res.threads.size()) +
                                   " (would fail mid-segment) — pre-check serial fallback");
            HcommVerbRes globalRes;
            globalRes.threads = res.threads;
            globalRes.channels = res.channels;
            for (size_t i = 0; i < segs.size(); ++i) {
                const OrchestrateSegResult sr = driver(segs[i], globalRes);
                if (sr.ret != HCCL_SUCCESS) {
                    out.ret = sr.ret;
                    out.err = "segment " + std::to_string(i) + ": " + sr.err;
                    out.segmentsRun = static_cast<u32>(i);
                    return out;
                }
                out.perSegTrace.push_back(sr.trace);
                out.segmentsRun = static_cast<u32>(i + 1);
            }
            out.batches = static_cast<u32>(segs.size());
            for (size_t i = 0; i < segs.size(); ++i) {
                out.batchTail.push_back(static_cast<u32>(i));
            }
            return out;
        }
    }
    if (static_cast<u64>(res.threads.size()) < totalNeed) {
        // 降级串行：逐段全局束驱动（= 既有 for 循环语义——sink 计数全局起算、
        // 段序全序、无编排屏障）。不拒错（M0 §二裁决：用既有池，WARNING 可观测）。
        out.degraded = true;
        out.warnings.push_back("threads " + std::to_string(res.threads.size()) +
                               " < required " + std::to_string(totalNeed) +
                               " (pool smaller than sum of domain needs) — "
                               "falling back to serial dispatch");
        HcommVerbRes globalRes;
        globalRes.threads = res.threads;
        globalRes.channels = res.channels;
        for (size_t i = 0; i < segs.size(); ++i) {
            const OrchestrateSegResult sr = driver(segs[i], globalRes);
            if (sr.ret != HCCL_SUCCESS) {
                out.ret = sr.ret;
                out.err = "segment " + std::to_string(i) + ": " + sr.err;
                out.segmentsRun = static_cast<u32>(i);
                return out;
            }
            out.perSegTrace.push_back(sr.trace);
            out.segmentsRun = static_cast<u32>(i + 1);
        }
        // 串行下批=段（每段独立窗，post 位不产生编排动作——段自带屏障动词）
        out.batches = static_cast<u32>(segs.size());
        for (size_t i = 0; i < segs.size(); ++i) {
            out.batchTail.push_back(static_cast<u32>(i));
        }
        return out;
    }

    std::vector<HcommVerbRes> domainRes(D);
    {
        size_t base = 0;
        for (u32 d = 0; d < D; ++d) {
            const u32 need = needPerDomain[d];
            domainRes[d].threads.assign(res.threads.begin() + static_cast<long>(base),
                                        res.threads.begin() + static_cast<long>(base + need));
            domainRes[d].channels = res.channels;   // 全表共享（peer 集不相交）
            base += need;
        }
    }

    // 两段序屏障的裸原语便捷件（hcomm_verb_sink.h DoSyncPre/DoSyncPost 同构——
    // 两处循环序照抄，只是线程组换成域子组）
    const auto domainPostBarrier = [&res](const HcommVerbRes& dr) {
        // R15 · ②（M0_R15 必答 F-3）：单线程组 no-op（return 0）——对齐模板
        // threadNum>1 门槛（ins_temp_reduce_scatter_nhr.cc:121/:143：单线程域无
        // 域内合取对象）；批窗关闭语义由批尾跨域交叉对承载（域 mains notify 槽
        // 按线程独立，1 线程 main 参与交叉对无障碍）。R14 首版 return -1 对
        // NHR 域（need=1）的批内 post=F 段必错——R15 前 NHR 段不可达（序列化
        // 拒收），既有可连性状零变化（mesh 域 n>1 恒 ≥2 不触此分支）。
        if (dr.threads.size() < 2) {
            return 0;
        }
        const ThreadHandle main = dr.threads[0];
        // 第一段：wait 全部（main 逐个等 sub 的 idx 0..S-1）
        for (size_t i = 1; i < dr.threads.size(); ++i) {
            if (HcommThreadNotifyWaitOnThread(main, static_cast<u32>(i - 1), CUSTOM_TIMEOUT) != 0) {
                return -2;
            }
        }
        // 第二段：record 全下发（逐 sub → main，idx i-1）
        for (size_t i = 1; i < dr.threads.size(); ++i) {
            if (HcommThreadNotifyRecordOnThread(dr.threads[i], main,
                                                static_cast<u32>(i - 1)) != 0) {
                return -3;
            }
        }
        return 0;
    };

    // 3. 批划分（post=T 收批；尾段 post=F 补末批——不静默丢段）
    std::vector<size_t> batchTailIdx;
    for (size_t i = 0; i < segs.size(); ++i) {
        if (segs[i].sync.postBarrier) {
            batchTailIdx.push_back(i);
        }
    }
    if (batchTailIdx.empty() || batchTailIdx.back() != segs.size() - 1) {
        batchTailIdx.push_back(segs.size() - 1);
    }
    out.batches = static_cast<u32>(batchTailIdx.size());
    out.batchTail.assign(batchTailIdx.begin(), batchTailIdx.end());

    // 4-7. 逐批执行：段并发下发 + 域内补 Post + 批尾合取
    size_t segBegin = 0;
    for (size_t b = 0; b < batchTailIdx.size(); ++b) {
        const size_t tail = batchTailIdx[b];
        // 4. 批内段全部下发（driver 产段动词——真原语由 sink 侧下发，此处 host
        //    调用序即"派发序"；两域段的 driver 调用交错与否不影响语义（M0 §五
        //    三不相交独立性），按段序顺序调用）
        for (size_t i = segBegin; i <= tail; ++i) {
            const u32 d = out.domainOfSeg[i];
            const OrchestrateSegResult sr = driver(segs[i], domainRes[d]);
            if (sr.ret != HCCL_SUCCESS) {
                out.ret = sr.ret;
                out.err = "segment " + std::to_string(i) + ": " + sr.err;
                out.segmentsRun = static_cast<u32>(i);
                return out;
            }
            out.perSegTrace.push_back(sr.trace);
            out.segmentsRun = static_cast<u32>(i + 1);
        }
        // 5. 域内补 Post：批内 post=F 的段（尾段 post=T 自带屏障动词，不补）
        //    —— 批内该域有多段（或单段 post=F）才需要（单段 post=F 且域只此一段
        //    = 无并发窗，但仍按批语义补域内 Post：批窗关闭对齐老 PostSync 语义）
        for (size_t i = segBegin; i <= tail; ++i) {
            if (!segs[i].sync.postBarrier) {
                const u32 d = out.domainOfSeg[i];
                // 同批同域去重（域内补 Post 一次覆盖批内该域全部 in-flight 段）
                bool firstInBatch = true;
                for (size_t j = segBegin; j < i; ++j) {
                    if (out.domainOfSeg[j] == d && !segs[j].sync.postBarrier) {
                        firstInBatch = false;
                        break;
                    }
                }
                if (firstInBatch) {
                    const int32_t br = domainPostBarrier(domainRes[d]);
                    if (br != 0) {
                        out.ret = HCCL_E_INTERNAL;
                        out.err = "batch " + std::to_string(b) + " domain " +
                                  std::to_string(d) + " internal post barrier failed (" +
                                  std::to_string(br) + ")";
                        return out;
                    }
                    OrchestratorResult::BarrierRec rec;
                    rec.segIdxAfter = static_cast<u32>(tail);
                    rec.domainInternal = true;
                    rec.domainId = d;
                    out.barriers.push_back(rec);
                }
            }
        }
        // 6. 批尾跨域合取（D>=2 才有"合取"对象；单域批无此对）
        if (D >= 2U) {
            // 批内出现过的域集合（合取成员 = 批内实际 in-flight 的域，非全 D——
            // A 形态批 1/批 2 都是双域，但单域批（如退化序列）不发明对端等待）
            std::vector<u32> liveDomains;
            for (size_t i = segBegin; i <= tail; ++i) {
                const u32 d = out.domainOfSeg[i];
                bool in = false;
                for (size_t k = 0; k < liveDomains.size(); ++k) {
                    if (liveDomains[k] == d) {
                        in = true;
                        break;
                    }
                }
                if (!in) {
                    liveDomains.push_back(d);
                }
            }
            if (liveDomains.size() >= 2U) {
                // 域 mains 互 notify 交叉对：先 record 全下发（每对 (i,j) i<j 双向
                // 各一条——对方 main 的槽 0，批配对后回零可复用，M0 §三），后 wait
                // 全下发（每 main 等"其余 mains 的槽 0"——合取语义：任一 main 到达
                // 此点前，其余 main 的 wait 不通过）
                for (size_t a = 0; a < liveDomains.size(); ++a) {
                    for (size_t bb = a + 1; bb < liveDomains.size(); ++bb) {
                        const ThreadHandle ma = domainRes[liveDomains[a]].threads[0];
                        const ThreadHandle mb = domainRes[liveDomains[bb]].threads[0];
                        if (HcommThreadNotifyRecordOnThread(ma, mb, 0) != 0 ||
                            HcommThreadNotifyRecordOnThread(mb, ma, 0) != 0) {
                            out.ret = HCCL_E_INTERNAL;
                            out.err = "batch " + std::to_string(b) + " cross-domain "
                                      "conjunction record failed";
                            return out;
                        }
                    }
                }
                for (size_t a = 0; a < liveDomains.size(); ++a) {
                    for (size_t bb = 0; bb < liveDomains.size(); ++bb) {
                        if (a == bb) {
                            continue;
                        }
                        const ThreadHandle waiter = domainRes[liveDomains[a]].threads[0];
                        if (HcommThreadNotifyWaitOnThread(waiter, 0, CUSTOM_TIMEOUT) != 0) {
                            out.ret = HCCL_E_INTERNAL;
                            out.err = "batch " + std::to_string(b) + " cross-domain "
                                      "conjunction wait failed";
                            return out;
                        }
                    }
                }
                OrchestratorResult::BarrierRec rec;
                rec.segIdxAfter = static_cast<u32>(tail);
                rec.domainInternal = false;
                rec.domainId = 0xFFFFFFFFU;
                out.barriers.push_back(rec);
            }
        }
        segBegin = tail + 1;
    }
    return out;
}

} // namespace mc2_ops_hccl::refactor

#endif
