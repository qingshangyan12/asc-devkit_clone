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
 * R18 · #11（M0_R18_NHR_EXPAND.md §一定稿）：NHR 逐 slice 几何公共件——
 * "Emit 展开序" 与 "DoNhrRun 步循环" 的**同一实现**（REVIEW #16 单源派生纪律：
 * 几何公式只写一遍，双消费方调用，不自建第二份）。
 *
 * 提取源：hcomm_verb_sink.h DoNhrRun :502-538（R15 交付的原序平移——改造为
 * 调本公共件，原语序 golden（exec T4/T5）锁零漂移）。
 *
 * 方向无关模型（M0 §一关键裁定）：SLICE_REDUCE 帧只带 step/sliceIdx 索引 +
 * 种子载荷，几何由执行端调 NhrSliceOf 推导——帧槽不花在偏移上，几何唯一
 * 实现点在此。WR/RD 是同一 slice 移动的两个视角（步表 tx/rx 双数组），
 * 执行端按通道协议自判（isDmaRead = PCIE → RD 拉对端 / 否则 WR 发对端）。
 *
 * 归属 inc/（与 nhr_steps.h 同级同理由——纯值函数零引擎依赖）。
 */
#ifndef REFACTOR_POC_NHR_SLICE_GEOMETRY_H
#define REFACTOR_POC_NHR_SLICE_GEOMETRY_H

#include "nhr_steps.h"          // NhrStepInfo / CalcNhrSteps（步表纯函数单源）
#include "verb_sink.h"          // VerbCall::NhrRunScalars（种子载荷类型）

namespace mc2_ops_hccl::refactor {

// ---------------------------------------------------------------------------
// 方向无关的逐 slice 几何（M0 §一）：
//   peer = WR 时 st.sendTo / RD 时 st.recvFrom（两侧 peer 不同——步表对称性）
//   localOff  = WR: 本地 tx 源偏移（input 直取或上料槽）/ RD: 本地 scratch rx 槽偏移
//   remoteOff = WR: 对端 buff tx 槽偏移 / RD: 对端 buff rx 槽偏移
//   偏移均为"种子基址 + 结构量"的**偏移半边**（基址合成在执行端：AICPU 通道表
//   remoteCclMem / server ve.localIn 等——与 DoNhrRun 指针合成同分工）
// ---------------------------------------------------------------------------
struct NhrSliceGeom {
    u32 peerAlg{0};     // 步表 algRank（换算 userRank 由调用方——域成员表在调用方语境）
    u64 localOff{0};    // 本地侧**绝对偏移**（WR: src——含 inputBase/scratchBase 基址；RD: scratchBase+S·rx）
    u64 remoteOff{0};   // 对端侧**槽内偏移**（对端基址在通道表/ve 状态——执行端合成）
    u64 size{0};        // SliceSizeOf（slice 维不乘 rpt——rpt 只推进偏移）
};

// skipStep0Tx 判定（DoNhrRun :452 同式——非 DMA 且 n>1 时 step0 的 tx 源直取 input）
inline bool NhrSkipStep0Tx(bool isDmaRead, u32 n)
{
    return (!isDmaRead) && (n > 1U);
}

// repeat 轮偏移（DoNhrRun :505 同式：rpt × inputRepeatStride）
inline u64 NhrRptInOff(const VerbCall::NhrRunScalars& sc, u64 rpt)
{
    return rpt * sc.inputRepeatStride;
}

// 逐 slice 几何（一函数出全部——WR/RD 由 isDmaRead 入参选侧）。
// step/sliceIdx 越界返回 size=0 的退化值（调用方 Validate 已挡——防御性回退
// 非静默吞错语义：执行端对 size=0 的 slice 原语即 no-op，错误在 Validate 报）。
inline NhrSliceGeom NhrSliceOf(const VerbCall::NhrRunScalars& sc, u32 step, u32 sliceIdx,
                               bool isDmaRead, u64 rpt)
{
    NhrSliceGeom g;
    const std::vector<NhrStepInfo> steps = CalcNhrSteps(sc.myAlgRank, sc.rankNum);
    if (step >= steps.size()) {
        return g;
    }
    const NhrStepInfo& st = steps[step];
    if (sliceIdx >= st.nSlices) {
        return g;
    }
    const u64 rptInOff = NhrRptInOff(sc, rpt);
    const u64 txIdx = st.txSliceIdxs[sliceIdx];
    const u64 rxIdx = st.rxSliceIdxs[sliceIdx];
    g.size = sc.sliceSize;
    if (isDmaRead) {
        // 读侧（DoNhrRun :511-515 同式）：RD(recvCh, dst=本地 scratch rx 槽,
        // src=对端 buff rx 槽)——peer=recvFrom，slot 维度用 rxIdx；
        // localOff 含 scratchBase（本地基址调用方持有，直出绝对偏移）
        g.peerAlg = st.recvFrom;
        g.localOff = sc.scratchBase + sc.sliceSize * rxIdx + rptInOff;
        g.remoteOff = sc.sliceSize * rxIdx + rptInOff;
    } else {
        // 写侧（DoNhrRun :520-535 同式）：WR(sendCh, dst=对端 buff tx 槽,
        // src=本地 tx 源)——peer=sendTo，slot 维度用 txIdx；src 三分支：
        // skipStep0Tx && step==0 → input 直取（inputBase + inStride×txIdx）；
        // 否则上料槽（scratchBase + S×txIdx）。localOff 含基址（绝对偏移）
        g.peerAlg = st.sendTo;
        g.remoteOff = sc.sliceSize * txIdx + rptInOff;
        if (NhrSkipStep0Tx(isDmaRead, sc.rankNum) && step == 0U) {
            g.localOff = sc.inputBase + sc.inputSliceStride * txIdx + rptInOff;
        } else {
            g.localOff = sc.scratchBase + sc.sliceSize * txIdx + rptInOff;
        }
    }
    return g;
}

} // namespace mc2_ops_hccl::refactor

#endif // REFACTOR_POC_NHR_SLICE_GEOMETRY_H
