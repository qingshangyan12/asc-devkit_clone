/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * R9 · 档位段模板展开器（TierParams → 最终段序列；§3.6-③ "A、B 段模板"）
 *
 * 定位：Instantiate 单决策点的**纯数据变换半边**——输入 = 骨架 Plan（parallel RS
 * 的每域 1 段）+ TierParams + exec 期 totalBytes；输出 = A 交错 4 段 或 B 步进
 * 2N 段的最终段序列（含逐段 sync 位）。不碰任何设备/引擎头，host 测试层可直接
 * 验证（WP-1 裁决："本函数是纯数据变换，先在 host 测试层验证展开逻辑"）。
 *
 * 老方案行为对齐（每个数字的行号出处，ziliao 树 = 方案文档引用基准）：
 *   A 交错（数据量 < 4MiB）：InsReduceScatterParallelExecutor::OrchestrateLoop
 *     ——ziliao ins_reduce_scatter_parallel_executor.cc:436-460：
 *       批1 = [intra.KernelRun(part0), inter.KernelRun(part1)]（:437-443，两调用
 *       间无等待 = 批内并发）→ PostSync（:447）→ 批2 = [inter(part0), intra(part1)]
 *       （:450-459）→ PostSync（:460）。
 *       part0/part1 = GetParallelDataSplit 的 0.5/0.5（:229-234）沿每块内部元素轴
 *       对半（§1.2 切分算术：dataOffset0=0、dataOffset1=part0 字节）。
 *       sync 位翻译：批内段 postBarrier=false（无等待）、批边界末段 postBarrier=
 *       true（PostSync 语义=合取屏障——撞锚点等批内全部 in-flight 段，§3.4.5
 *       定案：按组语义 {段成员区间, 边界屏障} 而非 ad hoc 锚点；本展开器产出
 *       数据位，等待语义由消费方编排器实现——当前串行消费方退化为顺序执行仍
 *       语义正确，sync 位是给未来并发消费方的正确数据，WP-2 裁决）。
 *   B 步进（数据量 ≥ 4MiB）：InsV2ReduceScatterOmniPipeExecutor::OrchestrateLoop
 *     ——ziliao ins_v2_reduce_scatter_omnipipe_executor.cc:478-545：逐小片
 *     [mesh.KernelRun(s_j), nhr.KernelRun(s_j)]，片间 PreSync/PostSync 屏障
 *     （:436/:447/:450/:460 同型）——两域永不同时飞、步步隔断。
 *       片宽 = CalReducescatterDataSize2D 等比链式（ziliao
 *       omnipipe_data_slice_calc.cc:345-463，公式逐行见下方 CalcStepwiseSlices 注）。
 *       sync 位翻译：全部段 preBarrier=postBarrier=true（步步隔断）。
 *   4MiB 分界：ziliao reduce_scatter_auto_selector.cc:19 PCIE_BASIC_RS_MAX_DATA_
 *     SIZE=4MiB、:378 `dataSize < THRESH → Parallel else OmniPipe`——严格小于
 *     走 A，本展开器 totalBytes < tier.threshBytes 同式（ExpandTieredSegments）。
 *
 * 纯新增纪律：本头不改动既有配方/序列化任何行为；TierParams.hasTier=false 的
 * Plan 永不进入本展开器（调用方 RecipeParallel 仅在 RS+parallel+双域时置 tier，
 * 消费方见 hasTier 才走展开路径）。
 */
#ifndef REFACTOR_POC_TIER_EXPAND_H
#define REFACTOR_POC_TIER_EXPAND_H

#include "action_spec.h"

#include <cmath>
#include <string>
#include <vector>

namespace mc2_ops_hccl::refactor {

// 小工具（公式内用到的两个数学件，单独成函数便于 ST 断言逐值锚定）：
//   ScalePow —— 整数指数幂（std::pow 的整数封装，double 域）
//   CeilLogDiv —— ceil(log(a)/log(b))：老步数分支 :377-378 的
//     ceil(log(xRankSize - bandwidthRatio) / log(omniPipeRatio))。
//     ★真数是 xRankSize − bandwidthRatio（老 :377 原式），不是 xRankSize − 1
//     （终审 MAJOR-1 修复，2026-10-07：两形态在 slowRankSize>ratio>1 时取值不同
//     ——16P PCIeMix 实测老 step=5 vs 错式 step=3）。
//     ★无 b>1 守卫（对齐老代码形态——老代码不做该防御，直接算：b<1 时
//     log(b)<0，真数 a∈(0,1) 时 log(a)<0，负/负=正，公式仍给出有效步数
//     ——16P 档 omni=0.58、a=0.276 实测 log(0.276)/log(0.58)=2.36→ceil 3→
//     step 5 恰依赖此路径；加 b<=1 提前返回会把 5 错成 3，即 MAJOR-1 病态
//     的另一形态）。仅 a<=0（真数非正，慢轴成员数不大于 ratio 时不会进本
//     函数——防御 NaN）。
inline double ScalePow(double base, u32 exp)
{
    double r = 1.0;
    for (u32 i = 0; i < exp; ++i) {
        r *= base;
    }
    return r;
}

inline u32 CeilLogDiv(double a, double b)
{
    if (a <= 0.0 || b <= 0.0 || b == 1.0) {
        return 1U;   // 仅防 NaN/除零（老代码该式在调用点保证 ratio>0；b==1 走特例分支）
    }
    const double v = std::ceil(std::log(a) / std::log(b));
    return (v > 0.0) ? static_cast<u32>(v) : 1U;
}

// ---------------------------------------------------------------------------
// B 形态片宽序列（唯一实现点——CalReducescatterDataSize2D 的 unified 投影）
//
// ★轴序约定（golden 对数后钉死，勿再犯）：老公式的形参 x/y 是**慢轴在前**——
// 老代码内层调用 CalReducescatterDataSize2D(yB, xB, yRankSize, xRankSize, ...)（ziliao
// ins_v2_reduce_scatter_omnipipe_executor.cc:539-542 的内层：慢轴 nhr 占 x 槽、
// 快轴 mesh 占 y 槽；审稿 golden /tmp/r9_golden_b.py 同式）。本投影对齐该轴序：
//   xB 形参 ← 慢轴（nhr）带宽；yB 形参 ← 快轴（mesh）带宽；
//   xRankSize ← 慢轴成员数；yRankSize ← 快轴成员数；
//   输出 xStep = 慢轴片宽序列（slowSlices）、yStep = 快轴片宽序列（fastSlices）。
//
// 公式出处（ziliao omnipipe_data_slice_calc.cc:345-463 的 CalReducescatterDataSize2D，
// 逐行对齐；★行号为 ziliao 树实数，老仓 hccl 主干同名函数在 :382-510（内容多
// multipleDimensionSplitRatio 覆写分支）——引用以 ziliao 为准（方案文档引用基准））：
//   bandwidthRatio = yB / xB（快/慢，PCIeMix 32P 场景 = 50/(35/3) ≈ 4.286）   (:360)
//   omniPipeRatio  = (xRankSize - 1) / bandwidthRatio                         (:362)
//   scale 放大系数 = bandwidthRatio / Σ_{t=0..maxStep-3} omniPipeRatio^t     (:364-368)
//     步数分支（:370-383）：xRankSize > bandwidthRatio 时按对数算步数并把 scale 归 1
//     （step ≤ maxStep 时；真数 = xRankSize − bandwidthRatio，老 :377 原式）；含
//     omniPipeRatio==1 特例（老 :372-375，step = bandwidthRatio + finStepMark——
//     8-server 档 ratio=50/(50/7)=7、omni=7/7=1 恰踩此特例）。xRankSize ≤
//     bandwidthRatio（慢轴 4 卡 < 4.286 的 PCIeMix 32P 主场景）步数恒 maxStep=6
//     且 scale > 1 生效——**golden 首片走的就是 scale 分支**，不可省略。
//   首片（scale > 1 分支，:391-395）:
//     xStep[0] = dataSize * scale * (xRankSize-1)^(step-2)
//                / (((yRankSize-1)*bandwidthRatio + xRankSize-1) * bandwidthRatio^(step-2))
//     （scale == 1 分支 :397-399 为 (xRankSize-bandwidthRatio)*dataSize/分母——
//      仅当 xRankSize > bandwidthRatio 时可达）
//   yStep[0] = xStep[0] * bandwidthRatio * (yRankSize-1) / (xRankSize-1)     (:404-406)
//   链式（index = 1 .. step-3，:410-436）:
//     末轮（index == step-3，:412-421）: yStep[i] = dataSize - sumY；
//       xStep[i] = yStep[i]*(xRankSize-1)/ratio，截断 min(sumY, yStep[i-1])，对齐
//     index == 1（:422-427）: xStep[1] = sumY；yStep[1] = xStep[1]*ratio/(xRankSize-1)
//     其余（:432-436）: xStep[i] = yStep[i-1]；yStep[i] = xStep[i]*ratio/(xRankSize-1)
//   末两步拆步（finStepMark=2，:438-452）:
//     ratio > 10 时 xStep[step-2] = dataSize - sumX 直吃；否则
//     xStep[step-2] = dataSize/(1+ratio)（★double 除法 :444——u64 提升为 double 再
//     赋回截断；先取整分母会把 12582912/5.2857 错算成 /5）截断到余量、对齐；
//     xStep[step-1] = dataSize - sumX - xStep[step-2]（余量守恒，:450）
//     yStep[step-2] = dataSize - xStep[step-2]（:451）；yStep[step-1] = dataSize - yStep[step-2]（:452）
//   步数 step = maxStep（= 6 = MAX_STEP_NUM(5)+1，RS 多拆一步——ziliao
//   omnipipe_data_slice_calc.h:23 + CalcRSOmniPipeSliceInfo:1506 的 +1）
//
// ★sum 口径（主会话 02:31 裁决提醒，断言禁写 sum==total）：两轴累计发送量之和
// ≠ totalBytes——finStepMark 末两步拆步使两轴各自序列的 sum 可以超过单轴总量
// （golden：slow Σ=12,737,536 / fast Σ=25,011,200 / total=12,582,912）。守恒的
// 是"每步 pair 的语义搬运量"与老 kernel 的步序表，不是序列和。
//
// 本投影的简化边界（如实登记，不改公式本体）：
//   · 只做两轴（level0 mesh × level1 nhr）——老 OmniPipe 的第三轴（level2 RoCE，
//     zRankSize>1）不迁移：unified 配方 parallel 恒两域（DomainsRequired()==2），
//     三层域形态属 §3.4.2 边界③ pattern 枚举外的扩展点，R9 范围外。
//   · 外层 z 轴分片（CalcRSOmniPipeSliceInfo :552-561 的 outer/inner 两层嵌套）在
//     zRankSize==1 时退化为单层（PCIeMix 双域场景恒单层）——golden 的 outerStep=1
//     实证，本投影直接取内层。
// ---------------------------------------------------------------------------
struct StepwiseSlicePlan {
    u32 stepNum{0};                     // 步数（= 两轴各自片数）
    std::vector<u64> fastSlices;        // yStep[0..stepNum-1]：快轴（mesh）逐片字节
    std::vector<u64> slowSlices;        // xStep[0..stepNum-1]：慢轴（nhr）逐片字节
};

// B 片宽计算（纯函数）。带宽经 TierParams.stepwise 的**有理数对**传入（无浮点
// 入参——Plan 纯数据纪律）；轴序约定：bwSlow 占公式 xB 槽、bwFast 占 yB 槽
// （见上方轴序约定）。slowRankSize=慢轴（nhr）成员数、fastRankSize=快轴（mesh）
// 成员数。totalBytes = 单块字节（RS 口径：count*dts）。
inline StepwiseSlicePlan CalcStepwiseSlices(u64 totalBytes, u32 slowRankSize, u32 fastRankSize,
                                            u64 bwFastNum, u64 bwFastDen,
                                            u64 bwSlowNum, u64 bwSlowDen, u64 alignBytes)
{
    StepwiseSlicePlan plan;
    if (slowRankSize <= 1U || fastRankSize <= 1U || bwFastNum == 0U || bwSlowNum == 0U ||
        bwFastDen == 0U || bwSlowDen == 0U || totalBytes == 0U || alignBytes == 0U) {
        return plan;   // 单轴/零带宽/零分母退化：老公式 :352-357 单轴直通，unified
                       // 不产生步进序列（双域配方不出现单轴，防御性返回空——caller 拒收）
    }
    const double xB = static_cast<double>(bwSlowNum) / static_cast<double>(bwSlowDen);   // 慢轴占 x 槽
    const double yB = static_cast<double>(bwFastNum) / static_cast<double>(bwFastDen);   // 快轴占 y 槽
    // 步数 6（MAX_STEP_NUM=5 + RS 多拆一步；ziliao CalcRSOmniPipeSliceInfo:1506 的
    // SetMaxStepNumOmni(OMNIPIPE_DEFAULT)+1，OMNIPIPE_DEFAULT 档恒 6）
    constexpr u32 MAX_STEP = 6U;
    constexpr u32 FIN_STEP_MARK = 2U;
    const double bandwidthRatio = yB / xB;                        // :360
    const double omniPipeRatio = static_cast<double>(slowRankSize - 1U) / bandwidthRatio;   // :362
    // scale 放大系数（:364-368）：Σ omniPipeRatio^t, t = 0..maxStep-3
    double scale = 0.0;
    for (u32 t = 0; t + FIN_STEP_MARK < MAX_STEP; ++t) {
        scale += ScalePow(omniPipeRatio, t);
    }
    scale = bandwidthRatio / scale;                               // :368
    u32 step = MAX_STEP;                                          // :370
    // 步数分支（:371-383）：慢轴成员数 > bandwidthRatio 时按对数收步数（scale 归 1）。
    // ★真数 = slowRankSize − bandwidthRatio（老 :377 原式；终审 MAJOR-1 修复，
    //   2026-10-07：原抄成 slowRankSize−1 在 16P PCIeMix 档步数 5 错成 3——
    //   老代码 xRankSize−ratio 与 xRankSize−1 仅在 ratio==1 时同值，golden 32P
    //   主场景 4<4.286 不进本分支故未暴露，第二组值 16P（2>1.724 进分支）抓出）
    if (static_cast<double>(slowRankSize) > bandwidthRatio) {
        if (static_cast<int>(omniPipeRatio) == 1) {
            step = static_cast<u32>(bandwidthRatio) + FIN_STEP_MARK;   // :374 等比 1 特例
        } else {
            step = CeilLogDiv(static_cast<double>(slowRankSize) - bandwidthRatio,
                              omniPipeRatio) + FIN_STEP_MARK;          // :377-378
        }
        if (step <= MAX_STEP) {
            scale = 1.0;                                          // :381
        } else {
            step = MAX_STEP;                                      // :383
        }
    }

    std::vector<u64> x(MAX_STEP, 0);    // xStep = 慢轴片
    std::vector<u64> y(MAX_STEP, 0);    // yStep = 快轴片
    // 首片：scale > 1 分支（:391-395）——PCIeMix 主场景（slowRankSize ≤ ratio）恒走此
    if (scale > 1.0) {
        const double powSlow = ScalePow(static_cast<double>(slowRankSize - 1U),
                                        step - FIN_STEP_MARK);
        const double powRatio = ScalePow(bandwidthRatio, step - FIN_STEP_MARK);
        x[0] = static_cast<u64>(static_cast<double>(totalBytes) * scale * powSlow /
                                ((static_cast<double>(fastRankSize - 1U) * bandwidthRatio +
                                  static_cast<double>(slowRankSize - 1U)) * powRatio));
        x[0] = x[0] / alignBytes * alignBytes;                    // :395 对齐
    } else {
        // scale == 1 分支（:397-399）：仅 slowRankSize > bandwidthRatio 时可达
        x[0] = static_cast<u64>((static_cast<double>(slowRankSize) - bandwidthRatio) *
                                static_cast<double>(totalBytes) /
                                (static_cast<double>(fastRankSize - 1U) * bandwidthRatio +
                                 static_cast<double>(slowRankSize - 1U)));
        x[0] = x[0] / alignBytes * alignBytes;                    // :399 对齐
    }
    if (step == FIN_STEP_MARK + 1U) {
        y[0] = totalBytes - x[0];                                   // :402 单步拆分
    } else {
        y[0] = static_cast<u64>(static_cast<double>(x[0]) * bandwidthRatio *
                                static_cast<double>(fastRankSize - 1U) /
                                static_cast<double>(slowRankSize - 1U));   // :404
        y[0] = y[0] / alignBytes * alignBytes;                      // :406 对齐
    }
    u64 sumX = 0;
    u64 sumY = y[0] + x[0];
    // 链式递推（index = 1 .. step-3；:410-436）
    for (u32 i = 1; i + FIN_STEP_MARK < step; ++i) {
        if (i + FIN_STEP_MARK == step - 1U) {
            // 循环末轮（index == step-finStepMark-1，:412-421）：y 吃余量、x 按 ratio 反推
            y[i] = (totalBytes > sumY) ? (totalBytes - sumY) : 0U;  // :413
            x[i] = static_cast<u64>(static_cast<double>(y[i]) *
                                    static_cast<double>(slowRankSize - 1U) / bandwidthRatio);   // :414
            if (i == 1U && x[i] > sumY) {
                x[i] = sumY;                                        // :416 特例
            } else if (x[i] > y[i - 1U]) {
                x[i] = y[i - 1U];                                   // :418 截断
            }
            x[i] = x[i] / alignBytes * alignBytes;                  // :420 对齐
            sumX += x[i];
            sumY += y[i];
            continue;
        }
        if (i == 1U) {
            x[i] = sumY;                                            // :423（index==1 特例）
        } else {
            x[i] = y[i - 1U];                                       // :433 链式
        }
        y[i] = static_cast<u64>(static_cast<double>(x[i]) * bandwidthRatio /
                                static_cast<double>(slowRankSize - 1U));   // :434
        y[i] = y[i] / alignBytes * alignBytes;                      // :435 对齐
        sumX += x[i];
        sumY += y[i];
    }
    // 末两步拆步（finStepMark=2，:438-452）：倒数第二步按 dataSize/(1+ratio) 截断到
    // 余量，末步吃剩余（x 轴余量守恒 = dataSize - sumX - x[step-2]，:450）。
    // ★ziliao :444 的除法是 **double 除法**（dataSizeEachRank/(1+bandwidthRatio)——
    // u64 被提升为 double，赋回 u64 截断）——不可先取整分母（int(1+ratio)=5 会把
    // 12582912/5.2857 错算成 /5，golden 对不上：2380288 vs 2516480）
    constexpr double BANDWIDTH_RATIO_BOUND = 10.0;   // ziliao omnipipe_data_slice_calc.cc:17
    if (bandwidthRatio > BANDWIDTH_RATIO_BOUND) {
        x[step - FIN_STEP_MARK] = (totalBytes > sumX) ? (totalBytes - sumX) : 0U;   // :443 直吃
    } else {
        x[step - FIN_STEP_MARK] = static_cast<u64>(static_cast<double>(totalBytes) /
                                                   (1.0 + bandwidthRatio));          // :444
        const u64 remainX = (totalBytes > sumX) ? (totalBytes - sumX) : 0U;
        if (x[step - FIN_STEP_MARK] > remainX) {
            x[step - FIN_STEP_MARK] = remainX;                      // :445 截断
        }
        x[step - FIN_STEP_MARK] = x[step - FIN_STEP_MARK] / alignBytes * alignBytes;   // :447
    }
    x[step - 1U] = (totalBytes > sumX + x[step - FIN_STEP_MARK]) ?
        (totalBytes - sumX - x[step - FIN_STEP_MARK]) : 0U;         // :450 余量守恒
    y[step - FIN_STEP_MARK] = (totalBytes > x[step - FIN_STEP_MARK]) ?
        (totalBytes - x[step - FIN_STEP_MARK]) : 0U;                // :451
    y[step - 1U] = (totalBytes > y[step - FIN_STEP_MARK]) ?
        (totalBytes - y[step - FIN_STEP_MARK]) : 0U;                // :452

    plan.stepNum = step;
    plan.slowSlices.assign(x.begin(), x.begin() + step);
    plan.fastSlices.assign(y.begin(), y.begin() + step);
    return plan;
}

// ---------------------------------------------------------------------------
// A/B 展开主体：骨架段序列 + TierParams + totalBytes → 最终段序列
//
// 输入约定：skeleton = parallel 配方产出的骨架段（每域恰 1 段，共 2 段——
// 段 0 = 域 0（mesh，快轴）、段 1 = 域 1（nhr，慢轴）；域序由 cfg.stages 决定）。
// 输出：展开后的段序列（caller 以返回值替换骨架执行——骨架本身的 blob 已落，
// 展开发生在消费方 exec 期，见 Plan.tier 注的序列化语义）。
//
// 失败路径（§6.9 不静默）：骨架形态不符（段数≠2/单轴退化/totalBytes=0）返回
// false + err——caller 拒收显式失败，不降级为骨架直跑（骨架直跑在 RS 语义下
// 是全量单段 = neither A nor B，静默错跑）。
// ---------------------------------------------------------------------------
inline bool ExpandTieredSegments(const std::vector<ActionSpec>& skeleton, const TierParams& tier,
                                 u64 totalBytes, std::vector<ActionSpec>& out, std::string& err)
{
    out.clear();
    if (skeleton.size() != 2U) {
        err = "ExpandTieredSegments: tier skeleton must be exactly 2 segments (one per domain), got " +
              std::to_string(skeleton.size());
        return false;
    }
    if (totalBytes == 0U) {
        err = "ExpandTieredSegments: totalBytes is 0 (exec count not injected?)";
        return false;
    }
    const ActionSpec& segFast = skeleton[0];    // 域 0（mesh）
    const ActionSpec& segSlow = skeleton[1];    // 域 1（nhr，慢轴）
    const u32 fastRankSize = segFast.domain.Size();   // 快轴（mesh）成员数
    const u32 slowRankSize = segSlow.domain.Size();   // 慢轴（nhr）成员数

    if (totalBytes < tier.threshBytes) {
        // ---- A 交错形态（§1.1 图）：[F(P0)∥S(P1)] → [S(P0)∥F(P1)]，批内 F/F、批边界 T ----
        // part0/part1：ForChunk 是数据对半（DataLayout::ForChunk），partRatio 0.5 恰复用
        //（RecipeSplit 数据切半同款——非均匀 ratio 的推广挂 P3，缺省 1/2 对齐 :229-234）
        // 骨架段按 part 切两半（ForChunk(i,2)：sliceSize 对半、baseOff 推进——
        // partRatio 0.5 时 ForChunk 恰等价；非均匀 ratio 的推广挂 P3）
        ActionSpec a[4];
        a[0] = segFast; a[0].layout = segFast.layout.ForChunk(0U, 2U);   // 批1：域0 P0
        a[1] = segSlow; a[1].layout = segSlow.layout.ForChunk(1U, 2U);   // 批1：域1 P1
        a[2] = segSlow; a[2].layout = segSlow.layout.ForChunk(0U, 2U);   // 批2：域1 P0
        a[3] = segFast; a[3].layout = segFast.layout.ForChunk(1U, 2U);   // 批2：域0 P1
        // 排列 [域0(P0)∥域1(P1), 域1(P0)∥域0(P1)]（§3.6-③ A 模板排列）
        out.push_back(a[0]);   // seg0: mesh P0 —— 批1
        out.push_back(a[1]);   // seg1: nhr  P1 —— 批1（与 seg0 并发）
        out.push_back(a[2]);   // seg2: nhr  P0 —— 批2
        out.push_back(a[3]);   // seg3: mesh P1 —— 批2（与 seg2 并发）
        // sync 位：批内段 post=false（无等待，并发窗）；批边界（每批末段）post=true
        //（合取屏障 §3.4.5——撞锚点等批内全部 in-flight 段）。pre 位恒 true：批 1
        // 首段 pre=true 对齐老代码入口 PreSync（:436）；批 2 首段 pre=true 是批边界
        // 屏障的另一面（末段 post=T + 下批首段 pre=T ⇒ 合取等待，双重表达与
        // sequence 段间 T/T 同构，§3.4.1 取值表 parallel 行"批内 F/F、批边界 T"）。
        for (size_t i = 0; i < out.size(); ++i) {
            out[i].sync.preBarrier = true;
            out[i].sync.postBarrier = (i == 1U || i == 3U);   // 批边界 = 段 1、段 3 之后
        }
        return true;
    }

    // ---- B 步进形态（§2.1 图）：[F(s0), S(s0), F(s1), S(s1), …] 逐片串行、段间 T/T ----
    // ★轴序（CalcStepwiseSlices 契约）：slowRankSize 在前（老公式 x 槽）、fastRankSize
    // 在后（y 槽）——见该函数的轴序约定注
    const StepwiseSlicePlan sp = CalcStepwiseSlices(totalBytes, slowRankSize, fastRankSize,
                                                    tier.stepwise.bwFastNum, tier.stepwise.bwFastDen,
                                                    tier.stepwise.bwSlowNum, tier.stepwise.bwSlowDen,
                                                    tier.stepwise.alignBytes);
    if (sp.stepNum == 0U) {
        err = "ExpandTieredSegments: stepwise slice calc degenerated (single-axis or zero bw)";
        return false;
    }
    // 片基址前缀（步 j 的快轴基址 = 前 j 片快轴片宽之和；慢轴同式独立累计——
    // 两轴片宽序列不同（golden：PCIeMix 32P 12MiB 步 0 比恰 10:1），基址各自推进）
    std::vector<u64> fastBase(sp.stepNum, 0);
    std::vector<u64> slowBase(sp.stepNum, 0);
    for (u32 s = 1; s < sp.stepNum; ++s) {
        fastBase[s] = fastBase[s - 1U] + sp.fastSlices[s - 1U];
        slowBase[s] = slowBase[s - 1U] + sp.slowSlices[s - 1U];
    }
    out.reserve(static_cast<size_t>(sp.stepNum) * 2U);
    for (u32 s = 0; s < sp.stepNum; ++s) {
        // 快轴片 s（mesh）：布局 = 骨架布局按片宽/片基址重写
        ActionSpec fast = segFast;
        fast.layout.sliceSize = sp.fastSlices[s];
        fast.layout.inBaseOff = segFast.layout.inBaseOff + fastBase[s];
        fast.sync.preBarrier = true;
        fast.sync.postBarrier = true;    // 片间步步隔断（§2.2 时间轴：PreSync/PostSync 夹每片）
        out.push_back(fast);
        // 慢轴片 s（nhr）
        ActionSpec slow = segSlow;
        slow.layout.sliceSize = sp.slowSlices[s];
        slow.layout.inBaseOff = segSlow.layout.inBaseOff + slowBase[s];
        slow.sync.preBarrier = true;
        slow.sync.postBarrier = true;
        out.push_back(slow);
    }
    return true;
}

} // namespace mc2_ops_hccl::refactor

#endif // REFACTOR_POC_TIER_EXPAND_H
