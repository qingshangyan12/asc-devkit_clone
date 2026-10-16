/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * R15 · ②：NHR 步几何纯函数（GetStepInfoList 公式投影，M0_R15_TRIPLE.md §四）
 *
 * 定位：步公式的唯一实现点——Emit（步 0 跳过集预判）/ hcomm_verb_sink DoNhrRun
 *（sink 侧步循环）/ ST golden 三处共用一份（"公式只写一遍"纪律）。
 *
 * 公式出处（reduce_scatter/template/aicpu/ins_temp_reduce_scatter_nhr.cc
 * GetStepInfoList :372-420 逐式；nSteps 同式 op_common/template/template_utils.cc
 * GetNHRStepNum :26-32）：
 *   nSteps  = floor(log2(n))——逐位右数
 *   sendTo  = (me + n − 2^s) % n      （RS 方向：发往"减 delta"侧）
 *   recvFrom= (me + 2^s) % n          （收自"加 delta"侧）
 *   nSlices = (n − 1 + 2^s) / 2^(s+1)（每步发收份数；Σ_s nSlices = n−1）
 *   txIdx   首值 sendTo，步进 −2^(s+1)（mod n）
 *   rxIdx   首值 me，同步进
 *   步 s 的 peer 值域 = {me±2^s} 并集 = NHR 建链集 {me±2^t : t=0..}（升序去重，
 *   CalcNHRChannelConnect 同式 channel_request.cc:56-73）——N=8 me=3 时
 *   sendTo∈{2,1,7}、recvFrom∈{4,5,7}，并集 {1,2,4,5,7} 恰为建链集 5 条
 *   （±4 同 rank 去重），ChannelOf 零 Fail（M0_R15 §五裁决 1）。
 *
 * 纯新增纪律：零引擎依赖（不 include 任何引擎头，inc/ 值对象居所）；CCU server
 * 侧 VerbNhrRun 的既有实现不动（"既有分支一行不动"）——两侧公式同源性由 ST
 * 等价断言传递锁（AICPU 动词展开 vs kernel 基线、CCU 帧展开 vs 同一基线 ⇒ 传递相等）。
 */
#ifndef REFACTOR_POC_NHR_STEPS_H
#define REFACTOR_POC_NHR_STEPS_H

#include "alg_param.h"   // u32（devkit 真实类型别名，值对象最小依赖）

#include <vector>

namespace mc2_ops_hccl::refactor {

struct NhrStepInfo {
    u32 step{0};
    u32 sendTo{0};          // 目标 rank 的 algRank（发往）
    u32 recvFrom{0};        // 来源 rank 的 algRank（收自）
    u32 nSlices{0};         // 本步发/收份数
    std::vector<u32> txSliceIdxs;   // 发送片序（首 sendTo，步进 −2^(s+1) mod n）
    std::vector<u32> rxSliceIdxs;   // 接收片序（首 me，同步进）
};

// 步数：floor(log2(n))（GetNHRStepNum :26-32 同式——n-1 逐位右移计数）
inline u32 CalcNhrStepNum(u32 n)
{
    u32 s = 0;
    for (u32 t = n - 1U; t != 0U; t >>= 1U) {
        ++s;
    }
    return s;
}

// 步几何全表（GetStepInfoList :386-417 逐式）。n<=1 退化返回空表（单 rank 无步）。
inline std::vector<NhrStepInfo> CalcNhrSteps(u32 me, u32 n)
{
    std::vector<NhrStepInfo> steps;
    if (n <= 1U) {
        return steps;
    }
    const u32 nSteps = CalcNhrStepNum(n);
    steps.resize(nSteps);
    for (u32 step = 0; step < nSteps; ++step) {
        const u32 deltaRank = 1U << step;
        const u32 deltaSlice = 1U << (step + 1U);
        NhrStepInfo& st = steps[step];
        st.step = step;
        st.sendTo = (me + n - deltaRank) % n;
        st.recvFrom = (me + deltaRank) % n;
        st.nSlices = (n - 1U + deltaRank) / deltaSlice;
        st.txSliceIdxs.reserve(st.nSlices);
        st.rxSliceIdxs.reserve(st.nSlices);
        u32 txSliceIdx = st.sendTo;
        u32 rxSliceIdx = me;
        for (u32 i = 0; i < st.nSlices; ++i) {
            st.txSliceIdxs.push_back(txSliceIdx);
            st.rxSliceIdxs.push_back(rxSliceIdx);
            txSliceIdx = (txSliceIdx + n - deltaSlice) % n;
            rxSliceIdx = (rxSliceIdx + n - deltaSlice) % n;
        }
    }
    return steps;
}

// NHR 建链集（CalcNHRChannelConnect :56-73 同式：{me±2^t} 升序去重——步公式
// peer 值域的闭包）。R16 · 8a：本函数升格为建链集**唯一实现点**——L3
// ReduceSlice::CalcRes / L4 CCU ClaimEngineRes NHR 分支两处内联循环已回收为
// 本调用（原"五端单源"注释口径修正：unified 树两端经本函数单源；注册链
// BuildUnifiedChannelDescs 分叉（CalcChannelRequestNhrMultiJetty）与 server
// VerbNhrRun 的 connectRanks 重建是**跨树独立同源**——L3 值对象层不 include
// 引擎头的铁律不可破，两侧同源性由 ST 传递等价断言锁（AICPU 动词展开 vs
// kernel 基线、CCU 帧展开 vs 同一基线 ⇒ 传递相等，REVIEW_CHECKLIST #18 四端
// 纪律原文不动）。用于 DoNhrRun 的通道查表键序（建链集升序 == 注册通道序）。
inline std::vector<u32> CalcNhrConnectRanks(u32 me, u32 n)
{
    std::vector<u32> connectRanks;
    if (n <= 1U) {
        return connectRanks;
    }
    for (u32 delta = 1U; delta < n; delta <<= 1U) {
        const u32 pos = (me + delta) % n;
        const u32 neg = (me + n - delta) % n;
        bool posIn = false;
        bool negIn = false;
        for (size_t k = 0; k < connectRanks.size(); ++k) {
            if (connectRanks[k] == pos) posIn = true;
            if (connectRanks[k] == neg) negIn = true;
        }
        if (!posIn) connectRanks.push_back(pos);
        if (!negIn && neg != pos) connectRanks.push_back(neg);
    }
    // 升序（插入序是 ±对序，非升序——与 CalcNHRChannelConnect 的 std::set 升序对齐）
    for (size_t i = 0; i + 1 < connectRanks.size(); ++i) {
        for (size_t j = i + 1; j < connectRanks.size(); ++j) {
            if (connectRanks[j] < connectRanks[i]) {
                const u32 tmp = connectRanks[i];
                connectRanks[i] = connectRanks[j];
                connectRanks[j] = tmp;
            }
        }
    }
    return connectRanks;
}

} // namespace mc2_ops_hccl::refactor

#endif // REFACTOR_POC_NHR_STEPS_H
