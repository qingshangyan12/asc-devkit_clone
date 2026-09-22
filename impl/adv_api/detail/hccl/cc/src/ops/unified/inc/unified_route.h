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
    // R6 备注行（RS 段生成/L3/序列化 v3/server 分支本轮已交付，**暂不激活**）：
    // 激活条件 = R7 随 CCU lowering（Emit 真原语接线）+ ST 落地——届时解注释本行并补
    // "拒收已移除"翻转断言（R7 清单已立案）。当前激活会圈进未接线面：
    //   ① ReduceSlice 的 AICPU 后端未落（Validate 快速失败 → 恒回退，无收益）；
    //   ② CCU 引擎被 GetUnifiedOpParamResCtxImpl 的 WP-B0 拒收（count=0 实例化未落，
    //      Mc2CcTilingInner 无 count → 资源期 sliceSize=0 全零 Plan，R6 实证）。
    // 开关位：本行解注释即激活（路由表是数据不是代码，扩圈定=加行）；
    // WP-B0 的引擎拒收移除是另一开关（alloc_ctx_res.cc GetUnifiedOpParamResCtxImpl
    // 入口的 EngineOf 分支），两开关独立——R7 先移 B0 再开路由行。
    // { HcclCMDType::HCCL_CMD_REDUCE_SCATTER, ActionKind::ReduceSlice },
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
