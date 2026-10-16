/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · unified 路由表（方案 §10.2 A5 灰度圈定 + §12-8 逐算法灰度纪律）
 *
 * 岔路口判定：每个通信请求进来，走老路径（selector→executor→template）还是
 * unified 新路径（Compile→Materialize），由本文件唯一决定。
 *
 * 路由表是数据不是代码：扩圈定 = 加行，删圈定 = 删行——不碰控制流。
 * ★ 圈了未实现（未过 Validate 的动作集）= BLOCKER，审查必查。
 *
 * 双开关（与 §12-8 灰度纪律一致）：
 *   ① 编译期：MC2_UNIFIED_RESOURCE（R6.5 后默认开——用户拍板：编译产物常带 unified，
 *      灰度收敛到运行期开关；置 MC2_UNIFIED_RESOURCE=0 可显式关闭。默认开的安全性
 *      依据：B0 拆闸后 unified 失败仍灰度回退老路径，且未设环境变量时运行期关）
 *   ② 运行期：环境变量 MC2_USE_UNIFIED_RES（默认关——未设置/空/=0 关，非零值开；
 *      仿 op_common.cc:2570 getenv 惯例）
 *   ③ 路由表：op ∈ kUnifiedRoutes 且其展开动作集全部已交付
 * 三者同时满足才走新路径。
 *
 * 灰度回退（≠C2 快速失败）：unified 分叉任一步失败（Compile/Validate/Materialize/
 * 序列化）→ HCCL_ERROR 日志 + 回退老路径完整重走。这是**灰度路径自身故障的容错**，
 * 与 §12-7 C2 的"用户钉错配置快速失败"是两回事：C2 管用户输入错，这里管新代码坏。
 */
#ifndef REFACTOR_POC_UNIFIED_ROUTE_H
#define REFACTOR_POC_UNIFIED_ROUTE_H

#include "action_spec.h"
#include "backend_registry.h"   // Engine（UnifiedAlgTag 的引擎名拼接）

// devkit 真实头（HcclCMDType；unified_route 属 inc/ 契约层，与 action_spec.h 同级口径）
#include "alg_param.h"

#include <cstdlib>

namespace mc2_ops_hccl::refactor {

// ---------------------------------------------------------------------------
// 路由表：每交付一格扩一行（P2 落 ReduceSlice 后解注释 RS/AR 行）
// kind 字段是"该 op 走 unified 时展开的核心动作"——用于人工核对路由范围
// 与已交付动作集一致；真正的完备性由 Compile 期 Validate 保证（未交付动作
// 快速失败，但那意味着路由表圈错了，属 BLOCKER 级配置错误）。
// ---------------------------------------------------------------------------
struct RouteEntry {
    HcclCMDType op;
    ActionKind kind;    // 该 op 在 unified 下的展开动作（需已过 Validate 交付集）
};

static const RouteEntry kUnifiedRoutes[] = {
    // 第三轮灰度圈定：仅 AG × AICPU；第五轮（尾款轮，2026-09-21）恢复 CCU 行——
    // 摘除原因"kernel 真注册链与 OpResCtx 回填半状态"已由 M1（HcclCcuKernelRegisterUnified
    // 真注册）+ M2（device 写入方 + blob 通路）+ M3（ProcessCcuTiling 分叉处组装交付物）
    // 补齐。路由表本身不带引擎维度（同 op 两行会在 UseUnifiedPath 线性匹配中重合），
    // 引擎分流由 L1Adapter::EngineOf(opParam) 在 Compile 内完成——本行激活 = AG 全引擎
    //（AICPU 维持既有路径，CCU 新增），CCU 专属守卫（checkOnly/组装失败）在分叉处。
    { HcclCMDType::HCCL_CMD_ALLGATHER, ActionKind::CollectAll },
    // R7 · WP-2（2026-10-06）：RS 行激活。前置条件已全部就位——
    //   ① L3/L4/序列化 v3 RS 布局/server REDUCE_SLICE 分支（R6 交付）；
    //   ② B0 引擎拒收 R6.5 已拆闸（WP-C3：AG 全引擎放行，入口无拒收分支）；
    //   ③ CCU 写入方 RS 帧实例化（WP-1：CcuPrepareForUnifiedPlan 帧级动作 ID 分叉，
    //      ReduceSlice 帧按 v2 并发 RS 写入方逐式覆写槽 1..15）；
    //   ④ device 侧 CcuPrepareForOp 的 RS 分支 158 分派（WP-1 同轮放开）。
    // R7 · WP-2（2026-10-06）：RS 行激活；R12 · WP-9③（2026-10-09）半激活注撤除——
    // RS × AICPU 后端（l4/backend_aicpu_reduce_slice.h）已注册，本行 = RS 全引擎
    //（AICPU 走 v4 段帧消费链 + 写侧动词序；CCU 走 v3/v5 双格式）。灰度同款双开关
    //（MC2_UNIFIED_RESOURCE/MC2_USE_UNIFIED_RES——未设 env 时 RS×AICPU 仍走老路径，
    // 本表只在三开关判定通过后生效）。
    { HcclCMDType::HCCL_CMD_REDUCE_SCATTER, ActionKind::ReduceSlice },
    // { HcclCMDType::HCCL_CMD_ALLREDUCE, ActionKind::CollectAll },  // one_shot/two_shot 段形态
    // { HcclCMDType::HCCL_CMD_ALLTOALL, ActionKind::ExchangeAll },
    // { HcclCMDType::HCCL_CMD_ALLTOALLV, ActionKind::ExchangeAll },
};

// 岔路口判定（存量侧唯一调用点：GetOpParam / ProcessCcuTiling 内的灰度分叉）
// env 值语义（MAJOR-3-②）：读值判断，"未设置/空/=0" 为关、非零值开
//（与 HCCL 生态 HCCL_ENABLE_OPEN_AICPU 的 "=1 开" 直觉一致，op_common.cc:2570 惯例）
inline bool UseUnifiedPath(int op)
{
#if !defined(MC2_UNIFIED_RESOURCE)
    (void)op;
    return false;                       // 显式 MC2_UNIFIED_RESOURCE=0：关闭（编译期逃生门）
#else
    static const bool envOn = []() {
        const char* v = std::getenv("MC2_USE_UNIFIED_RES");
        if (v == nullptr || v[0] == '\0' || v[0] == '0') {
            return false;               // 未设置 / 空 / =0 → 关
        }
        return true;                    // 非零值 → 开
    }();
    if (!envOn) {
        return false;
    }
    for (const RouteEntry& r : kUnifiedRoutes) {
        if (static_cast<HcclCMDType>(op) == r.op) return true;
    }
    return false;
#endif
}

// ---------------------------------------------------------------------------
// R12 · v5 动词序执行器灰度开关（2026-10-09，主会话裁决 R12-1③）：unified 路径
// 已命中（UseUnifiedPath）之上的**第二层**分叉——CCU 引擎的 blob 产出从 v4 段帧
// 切 v5 动词帧（host 组装段按本开关产 v5 blob + "CcuVerb" 前缀注册名）。
// 双开关与 §12-8 灰度纪律同款：
//   ① 编译期：MC2_UNIFIED_VERB_RESOURCE（默认**关**——与 MC2_UNIFIED_RESOURCE 的
//      默认开不同：v5 是机制重建轮，默认关 = 合入零行为变化；置 OFF 可显式确认关闭）
//   ② 运行期：env MC2_USE_UNIFIED_VERB（默认关——未设置/空/=0 关，非零值开）
// 两开关同时开 + UseUnifiedPath 命中（AG/RS 路由行）才走 v5。三层判据保 v3/v4
// 链路零变化：env 未设时即使宏开也恒 v4。
// ---------------------------------------------------------------------------
inline bool UseUnifiedVerbPath(int op)
{
#if !defined(MC2_UNIFIED_VERB_RESOURCE)
    (void)op;
    return false;                       // 编译期默认关（R12 机制轮零行为变化）
#else
    if (!UseUnifiedPath(op)) {
        return false;                   // 未进 unified 路由 → 无 verb 分叉可言
    }
    static const bool verbEnvOn = []() {
        const char* v = std::getenv("MC2_USE_UNIFIED_VERB");
        if (v == nullptr || v[0] == '\0' || v[0] == '0') {
            return false;               // 未设置 / 空 / =0 → 关（v4 段帧，现状行为）
        }
        return true;                    // 非零值 → 开（v5 动词帧）
    }();
    return verbEnvOn;
#endif
}

// ---------------------------------------------------------------------------
// R14 · A 形态执行期并发灰度开关（2026-10-10，挂账 #6——AICPU 消费方编排器）：
// UseUnifiedVerbPath 之上的**消费方编排**分叉——AICPU 消费方的段分派循环从
// "串行 for" 切 "按 sync 位编排的并发执行器"（段间 sync=false 的相邻段按域绑
// 线程组并发派发，sync=true 处合取屏障，l5/segment_orchestrator.h）。
// 与 VERB 家族同式三层（MC2_UNIFIED_VERB_RESOURCE 同款）：
//   ① 编译期：MC2_UNIFIED_CONCURRENCY_RESOURCE（默认**关**——env 缺省关时
//      编译产物零行为变化；纯编排能力轮，不动路由不动协议）
//   ② 运行期：env MC2_USE_UNIFIED_CONCURRENCY（默认关——未设置/空/=0 关，
//      非零值开）
//   ③ 范围：AICPU 消费方内部（ConsumeUnifiedPlanAicpu 的段循环分叉点）——
//      CCU server 侧不动（帧序消费天然串行，CCU 的 A 形态并发走"每域一
//      kernel"既有路径）
// 两开关同时开才走并发编排器；env 未设时即使宏开也恒串行 for（既有循环零变化）。
// ---------------------------------------------------------------------------
inline bool UseUnifiedConcurrencyPath()
{
#if !defined(MC2_UNIFIED_CONCURRENCY_RESOURCE)
    return false;                       // 编译期默认关（R14 编排能力轮零行为变化）
#else
    static const bool concEnvOn = []() {
        const char* v = std::getenv("MC2_USE_UNIFIED_CONCURRENCY");
        if (v == nullptr || v[0] == '\0' || v[0] == '0') {
            return false;               // 未设置 / 空 / =0 → 关（串行 for，现状行为）
        }
        return true;                    // 非零值 → 开（并发编排器）
    }();
    return concEnvOn;
#endif
}

// ---------------------------------------------------------------------------
// R15 · ① TierParams 承载灰度开关（2026-10-11，M0_R15_TRIPLE.md §一/§十-5）：
// tier 九值经骨架段 0 帧槽 16..24 进 v4 blob（方案甲——不升版本、CCU 写入方/
// server 只读槽 0..15 零影响、AICPU D2H 直取）。三层灰度与 CONCURRENCY 家族
// 同式：
//   ① 编译期：MC2_UNIFIED_TIER_RESOURCE（默认**关**——合入零行为变化）
//   ② 运行期：env MC2_USE_UNIFIED_TIER（默认关——未设置/空/=0 关，非零值开）
//   ③ 范围：AICPU 引擎 tier 骨架承载（SerializePlanV3 的 tierPlan 传参 +
//      NHR 骨架段放行 gate 的引擎半边在调用点 hccl_alloc_ctx_res.cc——
//      engine==AICPU && 本开关；CCU 降级路径维持 NHR 拒收）
// A/B 展开仍在消费方 exec 期（R6-③ 裁决不预编）——本开关只管"tier 参数是否
// 进 blob"，消费方展开分叉见 aicpu_consumer.h（槽 16==1 判别）。
// ---------------------------------------------------------------------------
inline bool UseUnifiedTierPath(int op)
{
#if !defined(MC2_UNIFIED_TIER_RESOURCE)
    (void)op;
    return false;                       // 编译期默认关（R15 承载轮零行为变化）
#else
    static const bool tierEnvOn = []() {
        const char* v = std::getenv("MC2_USE_UNIFIED_TIER");
        if (v == nullptr || v[0] == '\0' || v[0] == '0') {
            return false;               // 未设置 / 空 / =0 → 关（tier 不进 blob，现状行为）
        }
        return true;                    // 非零值 → 开（tier 九值写槽 16..24）
    }();
    return tierEnvOn;
#endif
}

// ---------------------------------------------------------------------------
// R15 · ② NHR AICPU 执行面灰度开关（2026-10-11，M0_R15 §四/§十）：
// backend_aicpu_reduce_slice.h 的 NHR Emit 分支 + hcomm_verb_sink.h 的
// DoNhrRun 接线。三层同式：
//   ① 编译期：MC2_UNIFIED_NHR_AICPU_RESOURCE（默认**关**）
//   ② 运行期：env MC2_USE_UNIFIED_NHR_AICPU（默认关）
//   ③ 范围：AICPU 半边 NHR 段（mesh 写侧分支一行不动；ungated NHR 段照旧
//      全连接迭代 ChannelOf Fail = 现状行为）
// ---------------------------------------------------------------------------
inline bool UseUnifiedNhrAicpuPath()
{
#if !defined(MC2_UNIFIED_NHR_AICPU_RESOURCE)
    return false;                       // 编译期默认关（R15 执行面轮零行为变化）
#else
    static const bool nhrEnvOn = []() {
        const char* v = std::getenv("MC2_USE_UNIFIED_NHR_AICPU");
        if (v == nullptr || v[0] == '\0' || v[0] == '0') {
            return false;               // 未设置 / 空 / =0 → 关（NHR 段维持 mesh 分支现状）
        }
        return true;                    // 非零值 → 开（NHR Emit 分支 + sink DoNhrRun）
    }();
    return nhrEnvOn;
#endif
}

// ---------------------------------------------------------------------------
// R18 · #11 全删轮（用户裁定 2026-10-14）：UseUnifiedNhrExpandPath 灰度开关删除
// ——展开形态转正为 NHR 唯一 Emit 形态（宏动词 NHR_RUN 路径整体删除），形态
// 选择不复存在；NHR AICPU 执行面整体灰度仍由 UseUnifiedNhrAicpuPath 承载。
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// C10 · unified 路径的 algTag 缓存键（§10.2 C10）
// 现状：SetOpParamAlgTag（op_common.cc:2442）= tag_algName_launchMode（CCU 拼类型）
// unified：algName 消失，algConfig 规范形本身即唯一键（语法即身份——相对 algName
// 的结构性优势）。资源类别名（RES_RESUSE_ALG）的等价物：同一 Plan（segments 逐字段
// 相等）即同一资源类，序列化 hash 做后缀的备选方案留 P2，本轮先语法串。
//
// MAJOR-3-① 规范化（三轮审查）：algConfig 先 Parse 再规范输出——空串→"sole"、
// 空格 trim、等价形归一，保证 "sole[mesh]" / "sole[ mesh ]" / "" 三种输入同键。
// ---------------------------------------------------------------------------
inline std::string UnifiedAlgTag(const std::string& algConfig, Engine engine)
{
    // 规范形（MAJOR-3-①）：两侧 trim + 空串归 "sole"（§6.8 不填→默认配方）。
    // 非法 algConfig（Parse 失败）原样透传——Compile 期已拦，tag 仅做日志可辨。
    std::string canonical;
    const size_t b = algConfig.find_first_not_of(" \t");
    if (b == std::string::npos) {
        canonical = "sole";                 // 全空白/空串 → 默认配方
    } else {
        const size_t e = algConfig.find_last_not_of(" \t");
        canonical = algConfig.substr(b, e - b + 1U);
    }
    const char* engineName = (engine == Engine::CCU) ? "ccu" : "aicpu";
    return "tag_unified_" + canonical + "_" + std::string(engineName);
}

} // namespace mc2_ops_hccl::refactor

#endif
