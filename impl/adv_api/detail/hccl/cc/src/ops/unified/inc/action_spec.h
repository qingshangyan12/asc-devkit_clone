/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L3 语义动作层的值对象定义（引擎无关，不含任何引擎头）
 *
 * 对应现状：TemplateDataParams（布局）+ subCommRanks/channels（域）+ 各 template
 * 私有成员（channelsPerRank_ / supportSymmetricMemory_ / dataType_ …）散落在
 * 65 份 AICPU template 与 46 份 CCU kernel 里，每份各自解释。
 */
#ifndef REFACTOR_POC_ACTION_SPEC_H
#define REFACTOR_POC_ACTION_SPEC_H

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "alg_param.h"   // devkit 真实类型 u32/u64/HcclDataType…

namespace mc2_ops_hccl::refactor {

// u32/u64 复用 devkit 定义（base.h：unsigned int / unsigned long long），
// 不自定义以免与真实接口签名不匹配

// ---------------------------------------------------------------- 公共组件 A
// DataLayout：全部 offset 公式的唯一实现点（方案 §6.7）
// 字段承接现有 TemplateDataParams，语义不变；差别是"公式只写一遍"。
struct DataLayout {
    u64 sliceSize{0};
    u64 tailSize{0};                 // count 不被 N 整除时末 rank 的短片
    u64 inputSliceStride{0};
    u64 outputSliceStride{0};        // strideCount 的落点（喂计算的布局敏感需求）
    u64 repeatNum{1};
    u64 inputRepeatStride{0};
    u64 outputRepeatStride{0};
    u64 inBaseOff{0};
    u64 outBaseOff{0};
    u64 scratchBaseOff{0};
    u32 dataTypeSize{2};

    // 现状：这三个式子在 ins_temp_all_gather_mesh_1D.cc 的 RunAllGatherMesh /
    // LocalDataCopy / PostLocalCopy 中各写一遍（第 163/164/172/174/240/241/306/307 行），
    // 同一套式子在 AG/RS/AR/A2A 四族 template 里再各抄一遍。
    u64 InOffset(u32 algRank, u32 rpt = 0) const
    {
        return inBaseOff + rpt * inputRepeatStride + inputSliceStride * algRank;
    }
    u64 OutOffset(u32 algRank, u32 rpt = 0) const
    {
        return outBaseOff + rpt * outputRepeatStride + outputSliceStride * algRank;
    }
    u64 ScratchOffset(u32 algRank, u32 rankNum, u32 rpt = 0) const
    {
        return scratchBaseOff + rpt * (sliceSize * rankNum) + sliceSize * algRank;
    }
    // tail 判定的唯一入口——现状是每处 `if (tailSize != 0 && algRank == N-1)` 手写
    u64 SliceSizeOf(u32 algRank, u32 rankNum) const
    {
        return (tailSize != 0 && algRank == rankNum - 1) ? tailSize : sliceSize;
    }
    u64 CountOf(u32 algRank, u32 rankNum) const { return SliceSizeOf(algRank, rankNum) / dataTypeSize; }

    // RS input 步长的三分支唯一实现点（R16 · 8b 收敛）：inputSliceStride 非零
    //（exec 期注入后）直用；为零（Compile 期——BuildRequest 对 input 不设步长，
    // l1_adapter.h:117-118）退 outputSliceStride（RS 的 input/output 步长同式同值，
    // BuildRequest 同式编码）。原七处内联三元式（action_reduce_slice.h
    // :181/:215、backend_aicpu_reduce_slice.h:89/:155/:179、verb_serializer.h:210
    // 及 consumer 侧 msg 形态）收敛于此。
    u64 RsInStride() const
    {
        return (inputSliceStride != 0U) ? inputSliceStride : outputSliceStride;
    }

    // two_shot：RS 段的 output 即 AG 段的 input（配方声明数据流边时调用）
    DataLayout ForSecondStage() const
    {
        DataLayout next = *this;
        next.inputSliceStride = outputSliceStride;
        next.inBaseOff = outBaseOff;
        return next;
    }
    // two_shot 段间衔接（§12-9 B2 决策，2026-09-20）：对齐现状 hcclBuff 中转语义——
    // RS 段把各 rank 的片归并到 scratch[0]（reduceDstSlice offset = hcclBuffBaseOffset，
    // ins_temp_all_reduce_mesh_1D_two_shot.cc:297），AG 段对任意 peer 的取数源恒为
    // scratch[0]（sendSrcSlice/recvSrcSlice 均 hcclBuffBaseOffset，:363/:367）——即
    // 本函数产出的布局 InOffset(peer) 对所有 peer 等值（scratch 基址 + 0 号槽）。
    // ForSecondStage 保留给 sequence 配方的域间衔接（前域 output 真的就是后域 input）。
    DataLayout ForSecondStageViaScratch() const
    {
        DataLayout next = *this;
        // input 半边改指 scratch 0 号槽：基址 = scratch 基址，步长 = 0
        //（AG 段逐 peer 取的是同一份归并结果，非逐 peer 槽位——§12-9"不改流水实现"）
        next.inBaseOff = scratchBaseOff;
        next.inputSliceStride = 0;
        // output 半边不变：AG 段仍写真实 output（outBaseOff/outputSliceStride 语义不动）
        return next;
    }
    // ChunkPipeline：切出第 i 个子段（段形态限定词 .chunk 的落点）
    DataLayout ForChunk(u32 i, u32 nSeg) const
    {
        DataLayout seg = *this;
        seg.sliceSize = sliceSize / nSeg;
        seg.inBaseOff = inBaseOff + i * seg.sliceSize;
        seg.outBaseOff = outBaseOff + i * seg.sliceSize;
        seg.tailSize = (i == nSeg - 1) ? (sliceSize - seg.sliceSize * (nSeg - 1)) : 0;
        return seg;
    }
    // 切轮（§10.2 A2）：OrchestrateLoop 逐轮推进 baseOff 的唯一实现点。
    // 与 ForChunk 的差别：轮内 sliceSize = 本轮实际字节（末轮为余量），不带 tail 语义
    // （tail 是 rank 维的尾块，轮是数据维的切分，两者正交）；repeat 不变。
    // roundStride = 满轮步长（末轮 slice 是余量，但下一轮起点仍按满轮推进——
    // 现状 OrchestrateLoop 的 processedDataCount 语义）。
    // tailSize 分支注：tailSize ≥ roundSlice（尾 rank 块大于尾轮字节）时当前置 0 是
    // 保守简化，P2 落真后端前补专项断言或判非法。
    DataLayout ForRound(u32 round, u64 roundSlice, u64 roundStride) const
    {
        DataLayout r = *this;
        r.inBaseOff = inBaseOff + round * roundStride;
        r.outBaseOff = outBaseOff + round * roundStride;
        r.sliceSize = roundSlice;
        r.tailSize = (tailSize != 0 && tailSize < roundSlice) ? tailSize : 0; // 尾轮不足整轮时
        return r;
    }
};

// ---------------------------------------------------------------- 动作与域
enum class ActionKind { CollectAll, ReduceSlice, ExchangeAll, LocalCopy, LocalReduce };

// pattern：域内遍历方式（不是链路类型，方案 §6.8）
enum class CommPattern { AUTO, MESH, NHR, RING };
// 段形态限定词
enum class SegShape { NONE, ONE_SHOT, TWO_SHOT };
// 内存路径
enum class TransportPolicy { AUTO, MEM2MEM, VIA_CCUBUF, SYMMEM };

struct DomainSpec {
    std::vector<u32> ranks;          // 现状 subCommRanks_[0]
    u32 myRank{0};
    u32 MyAlgRank() const            // 现状 GetAlgRank()，每 template 调一次
    {
        for (u32 i = 0; i < ranks.size(); ++i) {
            if (ranks[i] == myRank) return i;
        }
        return 0;
    }
    u32 Size() const { return static_cast<u32>(ranks.size()); }
};

struct ReduceSpec {
    int reduceOp{0};
    int dataType{0};
    int outputType{0};
};

struct SyncSpec {
    bool preBarrier{true};
    bool postBarrier{true};
};

// ActionSpec：Plan.segments 的元素。引擎无关，可序列化。
struct ActionSpec {
    ActionKind kind{ActionKind::CollectAll};
    DataLayout layout;
    DomainSpec domain;
    ReduceSpec reduce;
    CommPattern pattern{CommPattern::AUTO};
    SegShape shape{SegShape::NONE};
    u32 channelsPerRank{1};          // single/multi-channel 限定词的落点
    // R13（M0_R13_NHR_V5.md §三）：NHR 段 jetty 数（v2 链的 srcArg->portNum 语义）。
    // 缺省 1 == devkit RS NHR 链恒值（KFC_RS_NHR_PORT_NUM=1）——加法不改法：既有
    // Plan（MESH 段）不设该字段时零变化；>1 是 kernel 能力，协议面保留（帧槽 10）。
    u32 nhrJettyNum{1};
    u64 token{0};                    // 内存 token（devkit GetToken 产出，进消息槽）
    TransportPolicy transport{TransportPolicy::AUTO};
    SyncSpec sync;
    std::vector<u32> inputRanks;     // 数据所有权契约（段间数据流边）
    std::vector<u32> outputRanks;
};

// ---------------------------------------------------------------- 资源申报
struct ResCounters {                 // 七类 CCU 硬件资源
    u32 loop{0}, ccuBuf{0}, variable{0}, address{0}, event{0}, ccuThread{0}, instruction{0};
    ResCounters& operator+=(const ResCounters& o)
    {
        loop += o.loop; ccuBuf += o.ccuBuf; variable += o.variable; address += o.address;
        event += o.event; ccuThread += o.ccuThread; instruction += o.instruction;
        return *this;
    }
};

struct ChannelReq {                  // 通道申请（引擎无关，两引擎共用）
    u32 remoteRank{0};
    u32 count{1};                    // channelsPerRank
};

// kernel 注册计划（§10.2 C9）：真实链路每个 CCU kernel 一条 CcuKernelInfo
// {kernelFuncName, channels} 进 resourceRequest（现状用法见 ccu_temp_all_gather_2dies_mesh_1D.cc:88，
// 双 die 两 kernel 各带各的通道组）——"kernelFunc 跟动作走"的申报载体
struct CcuKernelPlan {
    std::string kernelFuncName;      // 动作 kernel 名（每动作一个，非算法名）
    std::vector<ChannelReq> channels;
};

struct ResourceClaim {
    std::map<u32, std::map<u32, ResCounters>> ccu;   // [dieId][resGroup]
    u32 slaveThreadNum{0};
    u32 notifyNumOnMainThread{0};
    std::vector<u32> notifyNumPerThread;
    std::vector<ChannelReq> channels;
    std::vector<CcuKernelPlan> kernels;              // kernel 注册条目（逐条追加，不合并）

    ResourceClaim& operator+=(const ResourceClaim& o)
    {
        // C++14：不用结构化绑定（devkit -std=c++14 -Werror 硬约束）
        for (auto itDie = o.ccu.begin(); itDie != o.ccu.end(); ++itDie) {
            const u32 die = itDie->first;
            for (auto itGrp = itDie->second.begin(); itGrp != itDie->second.end(); ++itGrp) {
                const u32 grp = itGrp->first;
                ccu[die][grp] += itGrp->second;
            }
        }
        slaveThreadNum = std::max(slaveThreadNum, o.slaveThreadNum);   // 线程可复用，取峰值
        notifyNumOnMainThread += o.notifyNumOnMainThread;
        notifyNumPerThread.insert(notifyNumPerThread.end(), o.notifyNumPerThread.begin(),
                                  o.notifyNumPerThread.end());
        channels.insert(channels.end(), o.channels.begin(), o.channels.end());
        kernels.insert(kernels.end(), o.kernels.begin(), o.kernels.end());
        return *this;
    }
};

// 域分组唯一实现点（R16 · 8c 收敛）：domain.ranks 序列相等去重（首现序）。
// domainOfSegOut 非空时逐段域号一并产出（编排器 R14 :170-186 与 Σ 申报 R15
// :265-278 原两处内联循环单源化——两处去重逻辑逐字同构，仅编排器多产 domainOfSeg）。
inline std::vector<std::vector<u32>> GroupDomains(const std::vector<ActionSpec>& segs,
                                                  std::vector<u32>* domainOfSegOut = nullptr)
{
    std::vector<std::vector<u32>> domainRanks;
    if (domainOfSegOut != nullptr) {
        domainOfSegOut->assign(segs.size(), 0xFFFFFFFFU);
    }
    for (size_t i = 0; i < segs.size(); ++i) {
        const std::vector<u32>& ranks = segs[i].domain.ranks;
        bool found = false;
        for (size_t k = 0; k < domainRanks.size(); ++k) {
            if (domainRanks[k] == ranks) {
                if (domainOfSegOut != nullptr) {
                    (*domainOfSegOut)[i] = static_cast<u32>(k);
                }
                found = true;
                break;
            }
        }
        if (!found) {
            if (domainOfSegOut != nullptr) {
                (*domainOfSegOut)[i] = static_cast<u32>(domainRanks.size());
            }
            domainRanks.push_back(ranks);
        }
    }
    return domainRanks;
}

// ---------------------------------------------------------------------------
// 域线程预算与并发申报（R15 · ③ 首版 → R19 · #12 B 档定稿）。
//
// DomainThreadTotalNeed(seg)：单段线程总量（main+slaves——编排器切池口径）。
// pattern-aware（R15 必答 F/H 定稿）：
//   RS·NHR → max(1, channelsPerRank)（模板 GetThreadNum=1×channelsPerRank，
//     ins_temp_reduce_scatter_nhr.cc:72；devkit 恒 1）
//   RS·MESH → max(1, n)（backend_aicpu_reduce_slice.h:53 ClaimEngineRes 同式）
//   AG → max(1, n−1)（backend_aicpu_collect_all.h:42 同式；**不按 pattern 分叉**
//     ——AG NHR AICPU 执行面未交付，保持 ClaimEngineRes 既有口径防 sole[nhr] AG
//     申报漂移）
// 与 R14 编排器 DomainThreadNeed 的关系：编排器委托本函数（单源——两侧公式
// 一致由 ST R15-③ 断言锁）。ClaimEngineRes（backend 层引擎申报）R16 · 8g 起
// 委托 ClaimAicpuThreadRes（同文件上方）——申报公式单源三处收敛为一处。
//
// CalcConcurrencySlaveClaim(segs)：CONCURRENT_SUM 档公式（R19 · #12 B 档，
// 2026-10-15 用户裁定，M0_R19_THREAD_CLAIM.md）：
//   claim = Σ_d (DomainThreadTotalNeed_d − 1) + (D − 1)
// （D = 域数，GroupDomains 去重——R16 · 8c 唯一实现点）。SPLIT/PARALLEL 配方
// 经 ClaimFormulaOf 查表绑定消费（l2/recipe_sole.h），与 env 无关。老树对照
// （行号本轮 grep 核实）：
//   · SPLIT 与老树 temp0+temp1+1 **恰同式零差**（RS ins_reduce_scatter_
//     concurrent_executor.cc:68-69 / AG ins_v2_all_gather_concurrent_executor.
//     cc:89——域0 main 兼控制位 = D−1 的 D=2 特例）；
//   · PARALLEL 比老树 intra+inter+2 **少 1**（RS ins_reduce_scatter_parallel_
//     executor.cc:76-77 / AG ins_v2_all_gather_parallel_executor.cc:130-131 的
//     专职控制线程职能已被编排器域 main 交叉握手接管，恒 ≤ 老树申报量）；
//   · D=1 退化恒等式：(need−1)+0 = need−1 = 单域 MAX_REUSE 自然值（两档单域
//     恒等，ST R19 断言锁）。
// MAX_REUSE 档（SOLE/SEQUENCE）不走本函数——operator+= max 累积（:224）即
// 终值。R15 旧口径（Σ(need−1)+D，gated env 开关）随 R19 退役：申报面与 env
// 解耦，执行面编排器灰度（aicpu_consumer.h UseUnifiedConcurrencyPath）独立
// 保留。R15 首版的"1 条冗余=老控制线程位"（M0_R15 §十-3 登记真机复核后可
// 一行改）即本式兑现。
// ---------------------------------------------------------------------------
inline u32 DomainThreadTotalNeed(const ActionSpec& seg)
{
    const u32 n = seg.domain.Size();
    u32 t = 1;
    if (seg.kind == ActionKind::ReduceSlice) {
        if (seg.pattern == CommPattern::NHR) {
            t = (seg.channelsPerRank > 1U) ? seg.channelsPerRank : 1U;   // NHR：1×channelsPerRank
        } else {
            t = (n > 1U) ? n : 1U;                                        // mesh RS：N
        }
    } else {
        t = (n > 1U) ? (n - 1U) : 1U;                                     // AG：N-1
    }
    return t;
}

// AICPU 线程申报唯一实现点（R16 · 8g 收敛）：threadNum 从 DomainThreadTotalNeed
// 单源取值（AG：N-1 / RS·mesh：N——真差异仅此一处，主线程是否参与数据动词），
// notify 三件套（slave 数、逐线程各 1、main 收 N-1）两后端逐字同模板。
// 原 backend_aicpu_collect_all.h:41-45 与 backend_aicpu_reduce_slice.h:54-58
// 两处镜像收敛于此；backend 侧 ClaimEngineRes 改委托本函数（等价由 ST R16 段
// 断言锁：两后端输出 == 旧手写模板）。
inline void ClaimAicpuThreadRes(const ActionSpec& a, ResourceClaim& claim)
{
    const u32 threadNum = DomainThreadTotalNeed(a);
    claim.slaveThreadNum = threadNum - 1;
    claim.notifyNumPerThread.assign(claim.slaveThreadNum, 1);
    claim.notifyNumOnMainThread = threadNum - 1;
}

inline u32 CalcConcurrencySlaveClaim(const std::vector<ActionSpec>& segs)
{
    // R19 · #12 B 档：Σ_d(need_d − 1) + (D − 1)——空 segs 守卫（D=0 时上式
    // 退化出 −1，直接报 0——不存在的段序列无申报）。域分组（GroupDomains
    // 唯一实现点）→ 逐域 max（同域段混排两动作时保守——R14 DomainThreadNeed
    // 同口径）→ Σ (need−1) + (D−1)（控制位由编排器域 main 交叉握手承接）
    if (segs.empty()) {
        return 0;
    }
    const std::vector<std::vector<u32>> domainRanks = GroupDomains(segs);
    u32 slaveTotal = 0;
    for (size_t d = 0; d < domainRanks.size(); ++d) {
        u32 need = 1;
        for (size_t i = 0; i < segs.size(); ++i) {
            if (segs[i].domain.ranks == domainRanks[d]) {
                const u32 t = DomainThreadTotalNeed(segs[i]);
                need = (need > t) ? need : t;
            }
        }
        slaveTotal += (need > 0U) ? (need - 1U) : 0U;
    }
    return slaveTotal + (domainRanks.size() > 0U ? static_cast<u32>(domainRanks.size()) - 1U : 0U);
}

// ---------------------------------------------------------------------------
// R9 · 档位参数（TierParams）：selector 数据量分档的 Plan 化（§3.6-② 档位参数）
//
// 老方案把 4MiB 阈值焊在 selector 代码里（ziliao 树 reduce_scatter_auto_selector.cc:19
// `PCIE_BASIC_RS_MAX_DATA_SIZE = 4*1024*1024`、:378-381 的 if/else 选两个算法名）；
// 新方案把同一判断降维成 Plan 里随行的一小块数据：改阈值=调参，不再改代码换名。
//
// 机制同构的活样板：AR one_shot/two_shot 分档（recipe_sole.h:118-122——AR 资源期
// 就有 sliceSize 可在 Expand 内判）。RS 的差别只有一点：资源期 count=0（R6 BLOCKER
// 根因），所以 4MiB 档位必须挂到 **exec 期实例化层**（Instantiate 单决策点，
// aicpu_consumer.h 消费侧）——判断时机后移，形态=骨架段带双形态参数集，
// msg.count 到手后按选中的模板把骨架段复制/实例化成最终序列（§3.6-③）。
//
// 缺省语义（加法不改法）：TierParams 缺省=无档位=现状行为零变化——既有 Plan
// （sole/sequence/split 及无 tier 的 parallel）不携带 tier 字段时，消费方走
// 原有"骨架即全量"路径，逐字段零变化（防退化断言锁死，见 poc_unified_expand_test
// R9 节）。hasTier=false 是唯一开关判据（缺省 false）。
//
// 数据字段（全部资源期已知——R6 裁决③"带宽是拓扑属性资源期已知"）：
//   threshBytes   4MiB（对齐 selector :19 常数；totalBytes < THRESH → A 交错，
//                 否则 B 步进——严格小于，对齐 selector :378 `dataSize <` 同式）
//   A（交错形态）：partRatio 0.5/0.5（对齐 GetParallelDataSplit 现状——ziliao 树
//                 ins_reduce_scatter_parallel_executor.cc:229-234 硬编码 splitData=0.5
//                 两份；hccl 主干 :456-471 已演进为 CalcParallelDataSplitRatio 端口
//                 配平公式（template_utils.cc:314-337 RS_WITH_LOCAL_REDUCE 时间系数
//                 + :358 八分位量化），缺省 fallback 仍 0.5（alg_param.h:639）——
//                 R9 取 0.5 基准（ziliao 口径），端口配平系数化挂 P3（见 R9 记录））
//   B（步进形态）：带宽加权逐片（非均匀！主会话 02:31 裁决对质结论）——两轴片宽
//                 按 CalReducescatterDataSize2D 的等比链式递推（ziliao
//                 omnipipe_data_slice_calc.cc:345-463：bandwidthRatio=yB/xB :359、
//                 首片 :371-378、链式 x[i]=y[i-1]/y[i]=x[i]*ratio/(xRankSize-1)
//                 :397-399、末两步 finStepMark 拆步 :405-418）——所以存**带宽
//                 有理数对**（{num, den} × 两轴，四 u64——分子分母忠实形态，不
//                 降维为单一 double：裁决"单一比值引入精度与序列化口径问题"；
//                 nhr 等效带宽 eqBw=bw/(rankSize-1) 常为非整数如 35/3，有理数
//                 表示无损）+ 对齐粒度 alignBytes，片宽序列由展开器按公式逐片
//                 计算（unified 侧纯函数，见 inc/tier_expand.h——公式只写一遍纪律）。
// ---------------------------------------------------------------------------
struct TierInterleave {                 // A 形态（< THRESH）：两域各取半块交错两批
    u64 partRatioNum{1};                // part0 = totalBytes * partRatioNum / partRatioDen
    u64 partRatioDen{2};                // 缺省 1/2 == 0.5（:229-234 口径）
};

struct TierStepwise {                   // B 形态（≥ THRESH）：两域逐小片交替步进
    // 带宽以**分子/分母有理数对**承载（不存 double——Plan 是纯数据可序列化，浮点
    // 序列化口径与跨端一致性是坑；也不存单一比值——主会话 02:31 裁决：分子分母
    // 是忠实形态，bandwidthRatio=yB/xB 在 :359 就是两带宽值的比）。
    // 语义：bwFastNum/bwFastDen = 快轴（mesh，level0）等效带宽；
    //       bwSlowNum/bwSlowDen = 慢轴（nhr，level1）等效带宽（eqBw 口径
    //       bw/(rankSize-1)，ziliao ins_v2_reduce_scatter_omnipipe_executor.cc:365-366
    //       ——PCIeMix 16 卡场景 = 35/(4-1) 即 num=35/den=3）。
    // 带宽来源（ziliao :343-356）：PCIeMix 8 卡=29/16 卡=35（BW_OMNI_PCIE_*_RS_CLOS）、
    // mesh 缺省 50（BW_OMNI_DEFAULT）——TierParams 由配方按拓扑形态填值。
    u64 bwFastNum{0};
    u64 bwFastDen{1};
    u64 bwSlowNum{0};
    u64 bwSlowDen{1};
    u64 alignBytes{512};                // 片宽对齐粒度（AICPU=512 / CCU=128，
                                        //   ziliao omnipipe_data_slice_calc.h:21-22）
};

struct TierParams {
    bool hasTier{false};                // 缺省 false = 无档位 = 现状行为零变化
    u64 threshBytes{4ULL * 1024 * 1024};    // 4MiB（selector :19 同值）
    TierInterleave interleave;           // A 模板系数
    TierStepwise stepwise;               // B 模板系数
};

// Plan：配方展开的产物，纯数据（可序列化 / 可缓存 / 可 dump）
struct Plan {
    // 序列化格式 ABI 版本（§6.3 version 字段升版守门）：v3 = KFC 消息槽定长格式
    // （真序列化，第四轮 A4 交付，实现见 plan_serializer.h 的 SerializePlanV3；
    // 本文件内 SerializePlanForCtx 是 v1 文本摘要的沿用，仅日志/维测用）
    u32 version{3};
    std::vector<ActionSpec> segments;
    ResourceClaim claim;
    // R9 · 档位参数（§3.6-②）：RS parallel 骨架段的双形态展开系数。缺省 hasTier=
    // false = 不携带 = 现状行为零变化（防退化断言锁死）。★序列化语义（主会话
    // 02:31 预防性确认）：blob 仍是**骨架段序列**——v4 blob 头部段数 = 骨架段数
    //（parallel RS = 每域 1 段共 2 段），A/B 展开在**消费方 exec 期**（R6-③ 裁决
    // "不预编两个候选 Plan" + RS 资源期 count=0 展不开——TierParams 不进 v4 blob
    // 槽位，host 侧 Plan 对象携带即可，消费方经 host 上下文取得；tier 进 blob
    // 协议面（v5）随 RS NHR 一并评估——两件事都动协议，合并一轮审查）。
    TierParams tier;
};

// ---------------------------------------------------------------------------
// Plan 序列化摘要（v1 文本格式，日志/维测专用）：
//   ★第四轮（A4，2026-09-21）已交付真序列化 v3（plan_serializer.h 的
//   SerializePlanV3，定长槽位二进制格式，可重建执行参数；三轮 MAJOR 降级声明
//   至此兑现，Plan.version 已升 3）。本摘要格式保留作日志 dump——ctx 旁路
//   （HcclSerializeUnifiedPlan）按 planBlob.size() 分配，真注册链接线（下一轮）
//   切换到 SerializePlanV3 的字节视图（vector<u64> → string 视图搬运），
//   此函数届时退役或降级为 HCCL_DEBUG 专用。
// ---------------------------------------------------------------------------
inline std::string SerializePlanForCtx(const Plan& plan)
{
    std::string s = "PLAN v" + std::to_string(plan.version) +
                    " segs:" + std::to_string(plan.segments.size()) + "\n";
    for (const ActionSpec& a : plan.segments) {
        s += "seg kind:" + std::to_string(static_cast<int>(a.kind)) +
             " pattern:" + std::to_string(static_cast<int>(a.pattern)) +
             " shape:" + std::to_string(static_cast<int>(a.shape)) +
             " transport:" + std::to_string(static_cast<int>(a.transport)) +
             " chan:" + std::to_string(a.channelsPerRank) +
             " slice:" + std::to_string(a.layout.sliceSize) + "\n";
    }
    s += "claim threads:" + std::to_string(plan.claim.slaveThreadNum) +
         " channels:" + std::to_string(plan.claim.channels.size()) +
         " kernels:" + std::to_string(plan.claim.kernels.size()) + "\n";
    return s;
}

// L1 产出的通信订单
struct PrimitiveRequest {
    int op{0};
    DataLayout layout;
    ReduceSpec reduce;
    DomainSpec domain;               // 由 DomainPlan 推导（用户不感知，方案 §6.8）
    bool allConnected{true};         // LinkProfile：域内是否全互联 → AUTO 选 pattern
    bool supportSymmetricMemory{false};
    u64 inputVa{0};                  // 用户 input 设备虚地址（§10.2 B8：token 计算源，0=未知）
    u64 outputVa{0};                 // 用户 output 设备虚地址（同上）
};

} // namespace mc2_ops_hccl::refactor

// v3 序列化实现在 plan_serializer.h（依赖 backend_registry.h 的 LoopSplitter 完整
// 类型；backend_registry.h 又 include 本文件——单独成头破循环，值对象文件保持零
// 引擎依赖的增长面）
#include "plan_serializer.h"

#endif
