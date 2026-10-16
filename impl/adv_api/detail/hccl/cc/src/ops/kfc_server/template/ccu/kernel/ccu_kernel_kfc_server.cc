/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kernel_alg_base.h"
#include "ccu_temp_kfc_all_gather_nhr_1D_multi_jetty_mem2mem.h"
#include "../../../../all_gather/template/ccu/kernel/ccu_kernel_kfc_all_gather_mesh1d_mem2mem.h"
#include "../../../../all_gather/template/ccu/kernel/ccu_kernel_kfc_all_gather_nhr1d_mem2mem.h"
#include "../../../../all_gather/template/ccu/kernel/ccu_kernel_kfc_all_gather_nhr1d_multi_jetty_mem2mem.h"
#include "../../../../all_reduce/template/ccu/kernel/ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.h"
#include "../../../../reduce_scatter/template/ccu/kernel/ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.h"
#include "../../../../reduce_scatter/template/ccu/kernel/ccu_kernel_kfc_reduce_scatter_nhr1d_multi_jetty_mem2mem.h"
#include "../../../../reduce_scatter/template/ccu/kernel/ccu_kernel_kfc_reduce_scatter_nhr1d_2die_mem2mem.h"
#include "../../../../reduce_scatter/template/ccu/kernel/ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem_peer_only.h"
#include "../../../../reduce_scatter/template/ccu/kernel/ccu_kernel_kfc_reduce_scatter_nhr1d_multi_jetty_mem2mem.h"
#include "../../../../all_to_all_v/template/ccu/kernel/ccu_kernel_all_to_all_mesh1d.h"
#include "../../../../all_to_all_v/template/ccu/kernel/ccu_kernel_all_to_all_v_mesh1d.h"
#include "../../../../all_to_all_v/template/ccu/kernel/ccu_kernel_all_to_all_mesh1d_multi_jetty.h"
#include "ccu_kernel_kfc_server.h"
#include "ccu_variable_dl.hpp"
#include "ccu_func_dl.hpp"
#include "mc2_type.h"
#include "hccl_alloc_ctx_res.h"

namespace mc2_ops_hccl {

using namespace hcomm;
using namespace std;

static_assert(
    KFC_CONCURRENT_AG_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE,
    "Concurrent AllGather parameters exceed the KFC server load width");
static_assert(
    KFC_AG_SOLE_NHR_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE,
    "SoleNHR AllGather parameters exceed the KFC server load width");
static_assert(
    KFC_CONCURRENT_A2A_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE,
    "Concurrent AllToAll parameters exceed the KFC server load width");
static_assert(
    KFC_A2A_MJ_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE, "AllToAll MultiJetty parameters exceed the KFC server load width");
static_assert(
    KFC_CONCURRENT_RS_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE,
    "Concurrent ReduceScatter parameters exceed the KFC server load width");
static_assert(KFC_MAX_MISSION_NUM == 2U, "KFC server CKE partitioning expects two missions");
static_assert(KFC_SIGNAL_REGIONS_PER_MISSION == 2U, "Each KFC mission requires commit and done regions");
// v3 三处同名登记（§6.12 性质）：server 分支 ID ↔ 协议槽 0 常量 ↔ unified ActionKind 枚举。
// 前两者在本文件构造性对齐（分支直接引用 KFC_UNIFIED_ACTION_COLLECT_ALL /
// KFC_UNIFIED_ACTION_REDUCE_SLICE）；与 unified ActionKind 的跨树对齐由 unified 测试
// poc_unified_serialize_v3_test.cc 以编译期常量对比守门（unified 树不 include devkit
// CCU 侧头，由测试镜像常量核验）。
static_assert(KFC_UNIFIED_ACTION_COLLECT_ALL == 0U, "v3 slot0 action id must stay aligned with ActionKind::CollectAll");
static_assert(KFC_UNIFIED_ACTION_REDUCE_SLICE == 1U, "v3 slot0 action id must stay aligned with ActionKind::ReduceSlice");

const uint32_t HBM_PARAM_IDX_0 = 0;
const uint32_t HBM_PARAM_IDX_1 = 1;
const uint32_t HBM_PARAM_IDX_2 = 2;
const uint32_t HBM_PARAM_IDX_3 = 3;
const uint32_t HBM_PARAM_IDX_4 = 4;
const uint32_t HBM_PARAM_IDX_5 = 5;
const uint32_t HBM_PARAM_IDX_6 = 6;
const uint32_t HBM_PARAM_IDX_7 = 7;
const uint32_t HBM_PARAM_IDX_8 = 8;
const uint32_t HBM_PARAM_IDX_9 = 9;
const uint32_t HBM_PARAM_IDX_10 = 10;
const uint32_t HBM_PARAM_IDX_11 = 11;
const uint32_t HBM_PARAM_IDX_12 = 12;
const uint32_t HBM_PARAM_IDX_13 = 13;
const uint32_t HBM_PARAM_IDX_14 = 14;
const uint32_t HBM_PARAM_IDX_15 = 15;
const uint32_t HBM_PARAM_IDX_16 = 16;
const uint32_t HBM_PARAM_IDX_17 = 17;
const uint32_t HBM_PARAM_IDX_18 = 18;
const uint32_t HBM_PARAM_IDX_19 = 19;
const uint32_t HBM_PARAM_IDX_20 = 20;
const uint32_t HBM_PARAM_IDX_21 = 21;
const uint32_t HBM_PARAM_IDX_22 = 22;
const uint32_t HBM_PARAM_IDX_23 = 23;
const uint32_t HBM_PARAM_IDX_24 = 24;
const uint32_t HBM_PARAM_IDX_25 = 25;
// KFC reduce_scatter specific parameters (at positions [13..23], not used by homm CCU server template)
const uint32_t HBM_PARAM_IDX_KFC_CHUNK_SIZE = 13;
const uint32_t HBM_PARAM_IDX_KFC_CHUNK_LOOP_NUM = 14;
const uint32_t HBM_PARAM_IDX_KFC_TAIL_SIZE = 15;
const uint32_t HBM_PARAM_IDX_KFC_FULL_GO_ADDR_OFFSET = 16;
const uint32_t HBM_PARAM_IDX_KFC_FULL_GO_LOOP_PARAM = 17;
const uint32_t HBM_PARAM_IDX_KFC_FULL_GO_PARALLEL_PARAM = 18;
const uint32_t HBM_PARAM_IDX_KFC_FULL_GO_RESIDUAL = 19;
const uint32_t HBM_PARAM_IDX_KFC_TAIL_GO_ADDR_OFFSET = 20;
const uint32_t HBM_PARAM_IDX_KFC_TAIL_GO_LOOP_PARAM = 21;
const uint32_t HBM_PARAM_IDX_KFC_TAIL_GO_PARALLEL_PARAM = 22;
const uint32_t HBM_PARAM_IDX_KFC_TAIL_GO_RESIDUAL = 23;

const uint32_t SINGLE_DIE = 1;
const uint32_t DOUBLE_DIE = 2;
const uint32_t DIE0_ID = 0;
const uint32_t DIE1_ID = 1;
constexpr const char* DIE1_START_SIG = "Die1StartSig";
constexpr const char* DIE1_END_SIG = "Die1EndSig";
// R13：NHR 步间屏障 CKE 位（M0_R13 §一——老 NHR kernel 同值：STEP_PRE=1<<2、
// STEP_POST=1<<3，ccu_kernel_kfc_...nhr1d_...cc:21-22；与 v5 XN 表 POST=4 不冲突
// ——CKE 位是通道通知位，非 XN 变量号）。
static constexpr uint16_t RS_NHR_STEP_PRE_SYNC_BIT = 1U << 2;
static constexpr uint16_t RS_NHR_STEP_POST_SYNC_BIT = 1U << 3;

static CcuResult ParseKernelArg(KfcServerContext& ctx, const CcuKernelArgKfcServer* kernelArg)
{
    ctx.arg = kernelArg;
    return CCU_SUCCESS;
}

static CcuResult InitResource(const KfcServerContext& ctx)
{
    (void)ctx;
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(KfcServerContext& ctx)
{
    uint32_t argId = 0;

    // 顺序需与 kfcServerArgs 中的字段顺序一致：xnAddr, ckeAddr, dieNum, missionNum, missionIndex, token
    CCU_CHK_RET(ccu::LoadArg(ctx.xnAddr, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.ckeAddr, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.dieNum, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.missionNum, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.missionIndex, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.token, argId++));

    return CCU_SUCCESS;
}

static CcuResult CompArgs(KfcServerContext& ctx)
{
    ccu::Variable regionSize;
    ctx.waitAddr = ctx.ckeAddr;
    regionSize = CCU_TASK_NUM_MAX * CCU_ONE_PARAM_SIZE;
    ctx.recordAddr = ctx.ckeAddr + regionSize;
    CCU_IF(ctx.missionIndex == 1)
    {
        ctx.waitAddr = ctx.recordAddr + regionSize;
        ctx.recordAddr = ctx.waitAddr + regionSize;
    }
    return CCU_SUCCESS;
}

void MissionPreSync(const ccu::Variable& func, const KfcServerContext& ctx)
{
    (void)func;
    (void)ctx;
}

void MissionPostSync(const KfcServerContext& ctx) { (void)ctx; }

CcuResult WaitTurnStartSig(const ccu::Variable& hbmSigAddr, ccu::Variable& turnStartSig, const KfcServerContext& ctx)
{
    (void)ctx;
    const uint32_t getDieId = 1;
    // 单Die场景：等待HBM中的信号
    CCU_WHILE(turnStartSig != 1)
    {
        CCU_CHK_RET(ccu::Load(hbmSigAddr, turnStartSig)); // 循环读HBM对应地址的信号到Xn，直到Xn中的信号值为1
    }
    turnStartSig = 0;                                  // reset Xn
    CCU_CHK_RET(ccu::Store(hbmSigAddr, turnStartSig)); // reset HBM
    return CCU_SUCCESS;
}

CcuResult SetTurnEndSig(const ccu::Variable& hbmSigAddr, const ccu::Variable& turnEndSig, const KfcServerContext& ctx)
{
    (void)ctx;
    const uint32_t getDieId = 1;
    CCU_CHK_RET(ccu::Store(hbmSigAddr, turnEndSig));
    return CCU_SUCCESS;
}

CcuResult LoadFuncParamFromMemory(
    const ccu::Variable& paramAddr, ccu::Array<ccu::Variable>& param, const KfcServerContext& ctx)
{
    (void)ctx;
    // 双Die场景Die1需要读后32个参数，其他场景都是读取前32个参数
    ccu::Variable doubleDie;
    doubleDie = CCU_PARAM_NUM_PER_DIE * CCU_ONE_PARAM_SIZE;
    ccu::Variable addr;
    addr = paramAddr;

    // 一次性读取本轮32个参数
    CCU_CHK_RET(ccu::Load(addr, param, CCU_PARAM_NUM_PER_DIE));
    return CCU_SUCCESS;
}

// ============================================================================
// v3 分发（§12-11 甲路线 2026-09-21）：unified Plan 轮级动作 ID 分发。
// 与 v2 的差别：v2 分发判据是实例级（role/algName 随 CCU 实例创建固定），
// v3 判据是每轮消息槽数据——param[0] 的动作 ID（KFC_UNIFIED_ACTION_*，值与
// unified ActionKind 枚举对齐，kfc_server_message.h 已 static_assert 三处登记）。
// 格式与读取宽度同 v2：每段一条 32×u64 记录（LoadFuncParamFromMemory 一次 Load），
// 出口哨兵同 param[0]==INVALID_U64（记录由 device 写入方按队列协议追加）。
//
// 直接调用既有 kernel 函数（v2 风格），不引入 ccu::CallFunc——公共逻辑（若后续
// 动作 kernel 共享 Pre/PostSync 壳）未来可提取为 CallFunc FuncBlock 备选，机制轮
// 不动设备侧资源模型（零新增 FuncBlock 查找/合成）。
// ============================================================================
static void DispatchUnifiedPlanV3(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx)
{
    CCU_IF(param[KFC_UNIFIED_ACTION_ID] == static_cast<u64>(KFC_UNIFIED_ACTION_COLLECT_ALL))
    {
        // 槽位转发序 == v2 AG else 分支（ccu_kernel_kfc_server.cc AG else 的读取序）：
        // KFC_UNIFIED_COLLECT_ALL_* 槽 1..14 与 KFC_CONCURRENT_AG_MESH_* 槽 1..14
        // 逐槽相等（kfc_server_message.h static_assert 守门）；token 语义同 v2——
        // 槽位参数不含 token，恒取 ctx.token。
        HCCL_INFO("[KFC][Unified][Dispatch] v3 action COLLECT_ALL -> CcuKfcAllGatherMesh1DMem2MemKernel");
        CcuKfcAllGatherMesh1DMem2MemKernel(
            param[KFC_UNIFIED_COLLECT_ALL_INPUT], param[KFC_UNIFIED_COLLECT_ALL_OUTPUT], ctx.token,
            param[KFC_UNIFIED_COLLECT_ALL_OUTPUT_OFFSET], param[KFC_UNIFIED_COLLECT_ALL_SLICE_SIZE],
            param[KFC_UNIFIED_COLLECT_ALL_GO_SIZE_0], param[KFC_UNIFIED_COLLECT_ALL_GO_SIZE_1],
            param[KFC_UNIFIED_COLLECT_ALL_GO_SIZE_2], param[KFC_UNIFIED_COLLECT_ALL_GO_SIZE_3],
            param[KFC_UNIFIED_COLLECT_ALL_CURRENT_RANK_SLICE_INPUT_OFFSET],
            param[KFC_UNIFIED_COLLECT_ALL_REPEAT_NUM_INV], param[KFC_UNIFIED_COLLECT_ALL_INPUT_REPEAT_STRIDE],
            param[KFC_UNIFIED_COLLECT_ALL_OUTPUT_REPEAT_STRIDE],
            param[KFC_UNIFIED_COLLECT_ALL_LAST_SLICE_SIZE], param[KFC_UNIFIED_COLLECT_ALL_INPUT_OUTPUT_EQUAL],
            ctx.arg->channels, ctx.arg->channelCount, static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId);
    }
    // R6（2026-09-21）：ReduceSlice 段分发——目标 CcuReduceScatterMesh1DMem2MemKernel
    //（既有 kernel，v2 风格直调）。槽位转发序 == v2 REDUCE_SCATTER_MESH 分支（:327-339
    // 的参数转发序）：KFC_UNIFIED_REDUCE_SLICE_* 槽 1..15 与 KFC_CONCURRENT_RS_MESH_*
    // 槽 1..15 逐槽相等（kfc_server_message.h static_assert 守门）；token 同 v2 恒取
    // ctx.token；dataType/outputType/reduceType 同 v2 从 ctx.arg->opParam 取（不占槽）。
    CCU_IF(param[KFC_UNIFIED_ACTION_ID] == static_cast<u64>(KFC_UNIFIED_ACTION_REDUCE_SLICE))
    {
        HCCL_INFO("[KFC][Unified][Dispatch] v3 action REDUCE_SLICE -> CcuReduceScatterMesh1DMem2MemKernel");
        CcuReduceScatterMesh1DMem2MemKernel(
            param[KFC_UNIFIED_REDUCE_SLICE_INPUT], param[KFC_UNIFIED_REDUCE_SLICE_OUTPUT], ctx.token,
            param[KFC_UNIFIED_REDUCE_SLICE_SCRATCH], param[KFC_UNIFIED_REDUCE_SLICE_RANK_SLICE_OFFSET],
            param[KFC_UNIFIED_REDUCE_SLICE_CHUNK_SIZE], param[KFC_UNIFIED_REDUCE_SLICE_CHUNK_LOOP_NUM],
            param[KFC_UNIFIED_REDUCE_SLICE_TAIL_SIZE], param[KFC_UNIFIED_REDUCE_SLICE_FULL_GO_ADDR_OFFSET],
            param[KFC_UNIFIED_REDUCE_SLICE_FULL_GO_LOOP_PARAM], param[KFC_UNIFIED_REDUCE_SLICE_FULL_GO_PARALLEL_PARAM],
            param[KFC_UNIFIED_REDUCE_SLICE_FULL_GO_RESIDUAL], param[KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_ADDR_OFFSET],
            param[KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_LOOP_PARAM], param[KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_PARALLEL_PARAM],
            param[KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_RESIDUAL], ctx.arg->channels, ctx.arg->channelCount,
            static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->opParam.DataDes.dataType,
            ctx.arg->opParam.DataDes.outputType, ctx.arg->opParam.reduceType);
    }
    // 未知动作 ID：server 微码循环内无法快速失败给 host（无同步回传通道，且
    // DispatchKfcSubKernel 调用点不检查返回值——v2 各分支 kernel 返回值同样丢弃），
    // 按现状惯例 HCCL_ERROR 打日志，队列循环继续消费后续记录。param[0] 是设备侧
    // 运行时值，gen 期不可读（v2 分支同样不打印槽值），日志不带值。真注册链落地后
    // 升级为 host 下发前 Validate 拦截（server 侧只处理已校验 ID）。
    // R6 起已知合法 ID 集合 = {COLLECT_ALL=0, REDUCE_SLICE=1}——下两条 CCU_IF 的
    // "非 COLLECT_ALL / 非 REDUCE_SLICE"判据合取即为未知 ID 判定（CCU 条件表达式
    // 无逻辑与，按既有单比较式并列）。
    CCU_IF(param[KFC_UNIFIED_ACTION_ID] != static_cast<u64>(KFC_UNIFIED_ACTION_COLLECT_ALL))
    {
        CCU_IF(param[KFC_UNIFIED_ACTION_ID] != static_cast<u64>(KFC_UNIFIED_ACTION_REDUCE_SLICE))
        {
            HCCL_ERROR("[KFC][Unified][Dispatch] unknown action id in unified plan record (slot0)");
        }
    }
}

// ============================================================================
// v5 动词分发（R12 · CCU 动词序执行器，2026-10-09）：动词 ID → 原语组合。
// 用户方向："基础 kernel（组原语+叶子原语）保留复用；整体流水 kernel 退役；server
// 从动作 ID→整体 kernel 直调改为动词 ID→原语组合"。与 v3 的差别：v3 分发目标是
// CcuKfcAllGatherMesh1DMem2MemKernel / CcuReduceScatterMesh1DMem2MemKernel 整体
// 流水 kernel（内含 PreSync/三阶段/PostSync 全套）；v5 分发目标是**单条原语**——
// sync 位置、批次语义（先全发起再全等）、段间交替全部由帧序数据化承载（M0
// unified/test/M0_R12_VERB_EXECUTOR.md），微码侧只留原语调用。
//
// 帧布局（kfc_server_message.h v5 段，两侧镜像常量三方互锁）：
//   槽 0 = 动词 ID（KFC_UNIFIED_VERB_*，值 == VerbCall::Verb 枚举 0..7）
//   数据帧（READ/WRITE/COPY）：槽 1=src、槽 2=dst、槽 3=size
//   BROADCAST：槽 1=src、槽 2=localDst、槽 3=size、槽 4=remoteDst、槽 5..8=goSize
//   REDUCE：槽 1=src(scratch)、槽 2=dst(output)、槽 3=size、槽 5..8=goSize
//   SYNC_PEER_PRE：槽 1=XN 掩码、槽 2..4=本端 IN/SCR/OUT 基址（TOK 恒 ctx.token）
//   SYNC_PEER_POST：槽 1=POST 掩码（0x10）
// 通道隐式映射（R12-1 裁决①）：数据帧不携 channelIdx——段内 READ/WRITE 按通道序
// 隐式消费（游标随数据帧推进、非数据帧不动；host Validate 前置校验帧数==channelCount
// + 迭代序==注册通道序——单源派生，R12-4）。
// XN 统一表 {IN=0,SCR=1,TOK=2,OUT=3,POST=4}：SYNC_PRE 掩码逐 XN 一条
// WriteVariableWithNotify 广播本端基址（老 kernel PreSync 同式）+ NotifyWait 等齐；
// 对端地址不进帧（GetResByChannel 消费侧取值，裁决④）。
//
// 灰度：role==UNIFIED_VERB（"CcuVerb" 前缀第二档注册）→ 本分发；v2/v3 分支零变化。
// ============================================================================

// SYNC_PEER_PRE：掩码逐 XN 一条 WriteVariableWithNotify（每通道一组）→ 每通道
// NotifyWait 等齐。老 AG PreSync（ag_m2m.cc:115-123）/老 RS PreSync（rs_m2m.cc:
// 136-147）同式——差别是掩码数据化（帧槽 1），AG 段 OUT|TOK / RS 段 IN|SCR|TOK。
static CcuResult VerbSyncPre(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx, KfcServerContext::VerbExecState& ve)
{
    // 段边界复位（R12-8 裁决）：对端变量缓存按段覆写（last-segment-wins），数据帧游标
    // 归零。追加语义在混排（AG 段→RS 段，掩码 0xC→0x7）下会使 peerTok 跨段残留旧值
    // （AG 段填 peerOut/peerTok、RS 段填 peerIn/peerScr/peerTok——TOK 是唯一跨形态共享
    // 向量），RS 段 READ/WRITE 以 [ch] 下标取缓存即静默错位。R12 灰度仅单段形态，此
    // clear 是防御性硬化：把"last-segment-wins"从 P2 备忘落成代码事实（AR two_shot
    // 激活时混排语义验证仍挂 R13）。
    ve.peerIn.clear();
    ve.peerScr.clear();
    ve.peerOut.clear();
    ve.peerTok.clear();
    ve.dataCursor = 0;
    const uint32_t xnMask = static_cast<uint32_t>(KFC_VERB_XN_MASK_RS_PRE);   // 掩码集合已知：两形态
    const uint32_t frameMask = static_cast<uint32_t>(0);                       // gen 期不可读槽值——
    (void)frameMask;                                                          // 按 param 比较分派（下方 CCU_IF）
    // ★server 微码 gen 期不能对 Variable 做位与（param 槽值是设备运行时值）——
    // 掩码分派按"槽 1 == 已知掩码字面量"的 CCU_IF 并列实现（与 v3 未知 ID 判定
    // 同型的单比较式），每形态内逐 XN 常量展开。
    CCU_IF(param[KFC_VERB_SLOT_ARG0] == static_cast<u64>(KFC_VERB_XN_MASK_AG_PRE))
    {
        // AG 段：广播 OUT 基址（槽 4）+ TOK（ctx.token）——逐通道两条
        for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
            CCU_CHK_RET(ccu::WriteVariableWithNotify(
                ctx.arg->channels[i], param[KFC_VERB_SLOT_ARG3], KFC_VERB_XN_OUT, 0,
                static_cast<uint16_t>(1U << KFC_VERB_XN_OUT)));
            CCU_CHK_RET(ccu::WriteVariableWithNotify(
                ctx.arg->channels[i], ctx.token, KFC_VERB_XN_TOK, 0,
                static_cast<uint16_t>(1U << KFC_VERB_XN_TOK)));
        }
        const uint16_t allBits = static_cast<uint16_t>((1U << KFC_VERB_XN_OUT) | (1U << KFC_VERB_XN_TOK));
        for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
            CCU_CHK_RET(ccu::NotifyWait(ctx.arg->channels[i], 0, allBits));
        }
        // 消费侧缓存对端 OUT/TOK（READ 消费 OUT、数据组装消费 TOK）
        for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
            ve.peerOut.push_back(ccu::GetResByChannel<ccu::Variable>(ctx.arg->channels[i], KFC_VERB_XN_OUT));
            ve.peerTok.push_back(ccu::GetResByChannel<ccu::Variable>(ctx.arg->channels[i], KFC_VERB_XN_TOK));
        }
    }
    CCU_IF(param[KFC_VERB_SLOT_ARG0] == static_cast<u64>(KFC_VERB_XN_MASK_RS_PRE))
    {
        // RS 段：广播 IN 基址（槽 2）+ SCR 基址（槽 3）+ TOK（ctx.token）
        for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
            CCU_CHK_RET(ccu::WriteVariableWithNotify(
                ctx.arg->channels[i], param[KFC_VERB_SLOT_ARG1], KFC_VERB_XN_IN, 0,
                static_cast<uint16_t>(1U << KFC_VERB_XN_IN)));
            CCU_CHK_RET(ccu::WriteVariableWithNotify(
                ctx.arg->channels[i], param[KFC_VERB_SLOT_ARG2], KFC_VERB_XN_SCR, 0,
                static_cast<uint16_t>(1U << KFC_VERB_XN_SCR)));
            CCU_CHK_RET(ccu::WriteVariableWithNotify(
                ctx.arg->channels[i], ctx.token, KFC_VERB_XN_TOK, 0,
                static_cast<uint16_t>(1U << KFC_VERB_XN_TOK)));
        }
        const uint16_t allBits =
            static_cast<uint16_t>((1U << KFC_VERB_XN_IN) | (1U << KFC_VERB_XN_SCR) | (1U << KFC_VERB_XN_TOK));
        for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
            CCU_CHK_RET(ccu::NotifyWait(ctx.arg->channels[i], 0, allBits));
        }
        for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
            ve.peerIn.push_back(ccu::GetResByChannel<ccu::Variable>(ctx.arg->channels[i], KFC_VERB_XN_IN));
            ve.peerScr.push_back(ccu::GetResByChannel<ccu::Variable>(ctx.arg->channels[i], KFC_VERB_XN_SCR));
            ve.peerTok.push_back(ccu::GetResByChannel<ccu::Variable>(ctx.arg->channels[i], KFC_VERB_XN_TOK));
        }
    }
    // R13 · NHR 段 SYNC_PRE（M0_R13_NHR_V5.md §五）：掩码 0x5 = IN|TOK——广播本端
    // IN 基址（槽 2，XN=IN）+ TOK（ctx.token），缓存对端 IN/TOK（WriteReduce 的
    // remoteDst 组址源 = peerIn[chIdx]；SCR/OUT 位掩码不含不广播）。老 NHR PreSync
    //（ccu_kernel_kfc_...nhr1d_...cc:80-94）同构——差别是掩码数据化（帧槽 1）+ XN
    // 位迁 v5 统一表（老表 INPUT=0/TOKEN=1 → TOK=2，M0_R13 §五）。
    CCU_IF(param[KFC_VERB_SLOT_ARG0] == static_cast<u64>(KFC_VERB_XN_MASK_NHR_PRE))
    {
        for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
            CCU_CHK_RET(ccu::WriteVariableWithNotify(
                ctx.arg->channels[i], param[KFC_VERB_SLOT_ARG1], KFC_VERB_XN_IN, 0,
                static_cast<uint16_t>(1U << KFC_VERB_XN_IN)));
            CCU_CHK_RET(ccu::WriteVariableWithNotify(
                ctx.arg->channels[i], ctx.token, KFC_VERB_XN_TOK, 0,
                static_cast<uint16_t>(1U << KFC_VERB_XN_TOK)));
        }
        const uint16_t allBits =
            static_cast<uint16_t>((1U << KFC_VERB_XN_IN) | (1U << KFC_VERB_XN_TOK));
        for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
            CCU_CHK_RET(ccu::NotifyWait(ctx.arg->channels[i], 0, allBits));
        }
        for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
            ve.peerIn.push_back(ccu::GetResByChannel<ccu::Variable>(ctx.arg->channels[i], KFC_VERB_XN_IN));
            ve.peerTok.push_back(ccu::GetResByChannel<ccu::Variable>(ctx.arg->channels[i], KFC_VERB_XN_TOK));
        }
    }
    // 段边界：数据帧游标归零（隐式映射按段配对）——复位已前移至入口（R12-8：掩码
    // 分派之前统一 clear+归零，避免未知掩码帧跳过复位）。
    // R12-9 修复轮 P0-1：本端基址缓存（槽 2..4 的写入方注入值——数据帧组址的
    // 本端半边；对端半边在上方 push_back）。两形态共用（三槽无条件缓存——
    // AG 段的 localIn 供 BROADCAST src 合成、RS 段的 localOut 供 REDUCE dst 合成，
    // 掩码不含的基址对端不取值，缓存无害）。
    ve.localIn = param[KFC_VERB_SLOT_ARG1];
    ve.localScr = param[KFC_VERB_SLOT_ARG2];
    ve.localOut = param[KFC_VERB_SLOT_ARG3];
    (void)xnMask;
    return CCU_SUCCESS;
}

// SYNC_PEER_POST：每通道 NotifyRecord + NotifyWait（老 PostSync :127-137 同式）
static CcuResult VerbSyncPost(KfcServerContext& ctx)
{
    const uint16_t postBit = static_cast<uint16_t>(1U << KFC_VERB_XN_POST);
    for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.arg->channels[i], 0, postBit));
    }
    for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.arg->channels[i], 0, postBit));
    }
    return CCU_SUCCESS;
}

// READ：ccu::Read(ch, localDst, remoteSrc, size)（老 RS Phase1 :180-182 消费式）。
// 通道隐式映射：dataCursor 是 host 值（帧序即通道序——host Validate 已校验
// 帧数==channelCount，游标不越界）。R12-9 修复轮 P0-1 后帧槽是纯偏移：
//   dst = 本端 SCR 基址（ve.localScr，SYNC_PRE 槽 3 缓存——写入方注入 scratchAddr）
//         + 帧 dst 偏移（peer 槽）
//   src = 对端 IN 基址（ve.peerIn，XN 交换）+ 帧 src 偏移（rankSliceOffset——我的片）
// token：本端 localDst 用 ctx.token（本端合法内存），remoteSrc 用对端 TOK。
static CcuResult VerbRead(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx, KfcServerContext::VerbExecState& ve,
                          ccu::Event& evt)
{
    const uint32_t ch = ve.dataCursor;
    ccu::LocalAddr localDst;
    localDst.addr = ve.localScr;
    localDst.addr += param[KFC_VERB_SLOT_ARG1];   // dst = 本端 SCR + peer 槽偏移（数据落
                                                  // 本端 scratch，老 kernel scratchMem_ 同式）
    localDst.token = ctx.token;
    ccu::RemoteAddr remoteSrc;
    remoteSrc.addr = ve.peerIn[ch];               // 对端 IN + 我的片偏移（XN 交换基址）
    remoteSrc.addr += param[KFC_VERB_SLOT_ARG0];
    remoteSrc.token = ve.peerTok[ch];
    const uint16_t mask = static_cast<uint16_t>(1U << (ch % 16U));
    CCU_CHK_RET(ccu::Read(ctx.arg->channels[ch], localDst, remoteSrc, param[KFC_VERB_SLOT_ARG2], evt, mask));
    ++ve.dataCursor;
    return CCU_SUCCESS;
}

// WRITE：ccu::Write(ch, remoteDst, localSrc, size)（老 AG Phase1 :176-177 消费式）。
// P0-1 后帧槽是纯偏移：localSrc = 本端 IN（ve.localIn）+ peer 份偏移；
// remoteDst = 对端 SCR（ve.peerScr，XN 交换）+ 我的槽偏移。
static CcuResult VerbWrite(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx, KfcServerContext::VerbExecState& ve,
                           ccu::Event& evt)
{
    const uint32_t ch = ve.dataCursor;
    ccu::LocalAddr localSrc;
    localSrc.addr = ve.localIn;
    localSrc.addr += param[KFC_VERB_SLOT_ARG0];
    localSrc.token = ctx.token;
    ccu::RemoteAddr remoteDst;
    remoteDst.addr = ve.peerScr[ch];
    remoteDst.addr += param[KFC_VERB_SLOT_ARG1];
    remoteDst.token = ve.peerTok[ch];
    const uint16_t mask = static_cast<uint16_t>(1U << (ch % 16U));
    CCU_CHK_RET(ccu::Write(ctx.arg->channels[ch], remoteDst, localSrc, param[KFC_VERB_SLOT_ARG2], evt, mask));
    ++ve.dataCursor;
    return CCU_SUCCESS;
}

// COPY：ccu::LocalCopy(dst, src, size)（>8P RS 本 rank 上料，老 kernel :176-179 消费式；
// AG 本地归位同式 ag_m2m.cc:131）。本地搬运不走通道（游标不动——隐式映射只索引
// READ/WRITE，COPY 无对端）。P0-1 后帧槽是纯偏移：src = 本端 IN（ve.localIn）+
// rankSliceOffset、dst = 本端 SCR（ve.localScr）+ 我的槽偏移。
static CcuResult VerbCopy(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx, KfcServerContext::VerbExecState& ve,
                          ccu::Event& evt)
{
    ccu::LocalAddr src;
    src.addr = ve.localIn;
    src.addr += param[KFC_VERB_SLOT_ARG0];
    src.token = ctx.token;
    ccu::LocalAddr dst;
    dst.addr = ve.localScr;
    dst.addr += param[KFC_VERB_SLOT_ARG1];
    dst.token = ctx.token;
    CCU_CHK_RET(ccu::LocalCopy(dst, src, param[KFC_VERB_SLOT_ARG2], evt, 1));
    return CCU_SUCCESS;
}

// BROADCAST：GroupBroadcast（组原语复用层——ccu_kernel_alg_base.cc:301）。goSize
// 从帧槽 5..8 直取（写入方按 exec sliceSize 设备侧重算）。本地落位并入扇出
//（dst 含本 rank 地址，老 kernel DoAllGather 同语义）。P0-1/P0-2 后帧槽是纯偏移：
//   src = 本端 IN（ve.localIn）+ 帧 src 偏移（AG 的 InOffset(me) 结构量，恒 0）
//   localDst = 本端 OUT（ve.localOut，SYNC_PRE 槽 4 缓存——写入方注入 recvBuf）+
//              帧 dst 偏移（P0-2：rankId×stride 纯偏移）
//   remote[i] = 对端 OUT（ve.peerOut，XN 交换）+ 帧 remoteDst 偏移（同偏移口径）
static CcuResult VerbBroadcast(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx, KfcServerContext::VerbExecState& ve)
{
    GroupOpSizeVars goSize;
    goSize.addrOffset = param[KFC_VERB_GO_SIZE_0];
    goSize.loopParam = param[KFC_VERB_GO_SIZE_1];
    goSize.parallelParam = param[KFC_VERB_GO_SIZE_2];
    goSize.residual = param[KFC_VERB_GO_SIZE_3];

    ccu::LocalAddr src;
    src.addr = ve.localIn;
    src.addr += param[KFC_VERB_SLOT_ARG0];
    src.token = ctx.token;
    ccu::LocalAddr localDst;
    localDst.addr = ve.localOut;
    localDst.addr += param[KFC_VERB_SLOT_ARG1];   // P0-2：本端 OUT 落位偏移（rankId×stride）
    localDst.token = ctx.token;
    std::vector<ccu::RemoteAddr> dst;
    dst.reserve(ctx.arg->channelCount);
    for (uint32_t i = 0; i < ctx.arg->channelCount; ++i) {
        ccu::RemoteAddr r;
        r.addr = ve.peerOut[i];
        r.addr += param[KFC_VERB_SLOT_ARG3];   // remoteDstOff（对端 output 里我的片）
        r.token = ve.peerTok[i];
        dst.push_back(r);
    }
    CCU_CHK_RET(GroupBroadcast(ctx, ctx.arg->channels, ctx.arg->channelCount, localDst, dst, src, goSize));
    // 广播独占本段通道表（声明式扇出已覆盖全部通道——后续数据帧不与共存，
    // 序列化器 Emit 序保证；游标置满防越界）
    ve.dataCursor = ctx.arg->channelCount;
    return CCU_SUCCESS;
}

// REDUCE：≤8P GroupLocalReduce / >8P PairwiseLocalReduce（组原语复用层，老 kernel
// DoReduceScatter :244-253 分叉同判据）。本地折叠不走通道（游标不动）。P0-1 后
// 帧槽 1/2 结构量恒 0：outDst = 本端 OUT（ve.localOut，SYNC_PRE 槽 4 缓存——写入方
// 注入 recvBuf）+ 帧 dst 偏移；scratch 基址 = 本端 SCR（ve.localScr，槽 3 缓存——
// 写入方注入 scratchAddr）+ 帧 src 偏移（恒 0），逐槽 size 步进同老 kernel。
static CcuResult VerbReduce(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx, KfcServerContext::VerbExecState& ve)
{
    GroupOpSizeVars goSize;
    goSize.addrOffset = param[KFC_VERB_GO_SIZE_0];
    goSize.loopParam = param[KFC_VERB_GO_SIZE_1];
    goSize.parallelParam = param[KFC_VERB_GO_SIZE_2];
    goSize.residual = param[KFC_VERB_GO_SIZE_3];

    ccu::LocalAddr outDst;
    outDst.addr = ve.localOut;
    outDst.addr += param[KFC_VERB_SLOT_ARG1];
    outDst.token = ctx.token;
    // scratch 全槽：基址 = 本端 SCR（ve.localScr）+ 帧 src 偏移，逐槽 size 步进
    //（老 kernel InitReduceScatterAddr :275-277 同式——槽序 = rankIdx 升序）
    std::vector<ccu::LocalAddr> scratch;
    scratch.reserve(ctx.arg->rankSize);
    ccu::Variable off;
    off = 0;
    for (uint32_t i = 0; i < ctx.arg->rankSize; ++i) {
        ccu::LocalAddr s;
        s.addr = ve.localScr;
        s.addr += param[KFC_VERB_SLOT_ARG0];
        s.addr += off;
        s.token = ctx.token;
        scratch.push_back(s);
        off += param[KFC_VERB_SLOT_ARG2];
    }
    if (ctx.arg->rankSize <= 8U) {
        // ≤8P：本 rank 槽替换为 input 直读（老 kernel :246-248——零搬运占位语义）
        CCU_CHK_RET(GroupLocalReduce(
            ctx, outDst, scratch, goSize, ctx.arg->opParam.DataDes.dataType,
            ctx.arg->opParam.DataDes.outputType, ctx.arg->opParam.reduceType));
    } else {
        // >8P：Pairwise 折叠链（老 kernel :204-232 同式——remainPieces 链 N→..→1）
        ccu::Event evt;
        uint32_t remainPieces = ctx.arg->rankSize;
        while (remainPieces > 1U) {
            const uint32_t reducePieces = remainPieces / 2U;
            const uint32_t srcIdx = remainPieces - reducePieces;
            ccu::Variable len;
            len = param[KFC_VERB_SLOT_ARG2];
            for (uint32_t i = 0; i + 1 < reducePieces; ++i) {
                len += param[KFC_VERB_SLOT_ARG2];
            }
            ccu::LocalReduce(scratch[0], scratch[srcIdx], len, ctx.arg->opParam.DataDes.dataType,
                             ctx.arg->opParam.reduceType, evt, 1);
            ccu::EventWait(evt, 1);
            remainPieces -= reducePieces;
        }
        ccu::LocalCopy(outDst, scratch[0], param[KFC_VERB_SLOT_ARG2], evt, 1);
        ccu::EventWait(evt, 1);
    }
    return CCU_SUCCESS;
}

// R18 · #11 全删轮（用户裁定 2026-10-14）：VerbNhrRun（NHR_RUN 紧凑帧的 server
// 消费方——host 期重建步表 + gen 期展开四层循环 + 收尾 LocalCopy）整体删除。
// NHR 步进流一律走逐动词帧（VerbSliceReduce/VerbChSync，下方）；宏/展开均未经
// 真机验证，双路径无保留价值。CCU Emit 已切展开动词序（backend_ccu_reduce_slice.h）。

// R18 · #11（M0_R18 §四）：NHR 展开动词——SLICE_REDUCE/CH_SYNC 的 server 分支。
// 与 VerbNhrRun 同源（ccu::WriteReduce 微码 :714/:732 现役；connectRanks 通道
// 序重建）；差别：步/slice 循环外置到动词帧序（host Emit 展开），本处只执行
// 单条 slice/握手 + repeat 帧内循环。**不走隐式游标**（dataCursor 不动——
// 帧 peer 槽显式查表，M0_R18 §四：SLICE_REDUCE/CH_SYNC 不计 dataFrames）。
static CcuResult VerbSliceReduce(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx,
                                 KfcServerContext::VerbExecState& ve)
{
    const uint32_t rankSize = static_cast<uint32_t>(ctx.arg->rankSize);
    const uint32_t rankId = ctx.arg->rankId;
    if (rankSize == 0U || rankId >= rankSize) {
        HCCL_ERROR("[KFC][Unified][Verb] SLICE_REDUCE invalid: rankSize[%u] rankId[%u]",
            rankSize, rankId);
        return CCU_E_INTERNAL;
    }
    // 帧槽（M0_R18 §四）：1=step 2=sliceIdx 3=size 4=peerWR 5=peerRD 7..10=repeat 种子
    const uint32_t step = static_cast<uint32_t>(param[KFC_VERB_SR_STEP]);
    const uint32_t sliceIdx = static_cast<uint32_t>(param[KFC_VERB_SR_SLICE_IDX]);
    const uint64_t size = param[KFC_VERB_SR_SIZE];
    const uint32_t peerWr = static_cast<uint32_t>(param[KFC_VERB_SR_PEER_WR]);
    const uint32_t peerRd = static_cast<uint32_t>(param[KFC_VERB_SR_PEER_RD]);
    const uint64_t rptNumInv = param[KFC_VERB_SR_REPEAT_NUM_INV];
    const uint64_t rptNum = (rptNumInv == 0ULL) ? 1ULL : (~0ULL - rptNumInv);
    const uint64_t inRepStride = param[KFC_VERB_SR_IN_REPEAT_STRIDE];
    // repeat 帧内循环（循环维不展开——M0_R18 §二；单 slice 的 rpt 偏移推进）
    for (uint64_t rpt = 0; rpt < rptNum; ++rpt) {
        const uint64_t rptOff = rpt * inRepStride;
        // 方向自判：本实现 WR 侧（发对端）——RD（DMA 拉）形态 CCU v5 首版登记
        // P2（M0_R18 §四；AICPU 侧 DoSliceReduce 双侧已实现——两侧等价由 host
        // checker 判，本侧按 devkit NHR 链 WR 语义执行）
        (void)peerRd;
        const uint32_t peerAlg = peerWr;
        if (peerAlg >= rankSize) {
            HCCL_ERROR("[KFC][Unified][Verb] SLICE_REDUCE peer[%u] out of rankSize[%u]",
                peerAlg, rankSize);
            return CCU_E_INTERNAL;
        }
        // 通道显式查表：connectRanks 建链集重建（VerbNhrRun :620-641 同式——升序
        // 去重 = 通道注册序）+ chIdxOf 编号；peer 的 algRank 即域内 rank 序号
        //（devkit NHR 链单域——与 VerbNhrRun 的 toRank/fromRank 同域语义）
        std::vector<uint32_t> connectRanks;
        for (uint32_t delta = 1; delta < rankSize; delta <<= 1) {
            const uint32_t pos = (rankId + delta) % rankSize;
            const uint32_t neg = (rankId + rankSize - delta) % rankSize;
            bool posIn = false;
            bool negIn = false;
            for (size_t k = 0; k < connectRanks.size(); ++k) {
                if (connectRanks[k] == pos) posIn = true;
                if (connectRanks[k] == neg) negIn = true;
            }
            if (!posIn) connectRanks.push_back(pos);
            if (!negIn && neg != pos) connectRanks.push_back(neg);
        }
        for (size_t i = 0; i + 1 < connectRanks.size(); ++i) {
            for (size_t j = i + 1; j < connectRanks.size(); ++j) {
                if (connectRanks[j] < connectRanks[i]) {
                    const uint32_t tmp = connectRanks[i];
                    connectRanks[i] = connectRanks[j];
                    connectRanks[j] = tmp;
                }
            }
        }
        std::vector<uint32_t> chIdxOf(static_cast<size_t>(rankSize), 0xFFFFFFFFU);
        for (size_t k = 0; k < connectRanks.size(); ++k) {
            chIdxOf[connectRanks[k]] = static_cast<uint32_t>(k);
        }
        const uint32_t chIdx = chIdxOf[peerAlg];
        if (chIdx == 0xFFFFFFFFU || chIdx >= ctx.arg->channelCount) {
            HCCL_ERROR("[KFC][Unified][Verb] SLICE_REDUCE peer[%u] not in connect set", peerAlg);
            return CCU_E_INTERNAL;
        }
        // tx 序重建（VerbNhrRun GetStepInfo 同式——首 sendTo 步进 −2^(s+1) mod n）；
        // 与 host NhrSliceOf 公共件同式（跨树独立同源，#18 四端纪律）
        const uint32_t deltaRank = 1U << step;
        const uint32_t deltaSlice = 1U << (step + 1U);
        const uint32_t nSlices = (rankSize - 1U + deltaRank) / deltaSlice;
        if (sliceIdx >= nSlices) {
            HCCL_ERROR("[KFC][Unified][Verb] SLICE_REDUCE step[%u] slice[%u] out of range",
                step, sliceIdx);
            return CCU_E_INTERNAL;
        }
        uint32_t txIdx = (rankId + rankSize - deltaRank) % rankSize;   // 首 = sendTo
        for (uint32_t i = 0; i < sliceIdx; ++i) {
            txIdx = (txIdx + rankSize - deltaSlice) % rankSize;
        }
        ccu::RemoteAddr remoteDst;
        remoteDst.addr = ve.peerScr[chIdx];
        remoteDst.addr += size * txIdx + rptOff;
        remoteDst.token = ve.peerTok[chIdx];
        ccu::LocalAddr localSrc;
        const bool skipStep0Tx = (step == 0U) && (rankSize > 1U);
        if (skipStep0Tx) {
            localSrc.addr = ve.localIn;
            localSrc.addr += param[KFC_VERB_SR_INPUT_SLICE_STRIDE] * txIdx + rptOff;
        } else {
            localSrc.addr = ve.localScr;
            localSrc.addr += size * txIdx + rptOff;
        }
        localSrc.token = ctx.token;
        const uint16_t mask = static_cast<uint16_t>(1U << (chIdx % 16U));
        CCU_CHK_RET(ccu::WriteReduce(ctx.arg->channels[chIdx], remoteDst, localSrc, size,
                                     ve.evt, mask));
    }
    return CCU_SUCCESS;
}

// R18 · #11：CH_SYNC——通道级握手半边（dir 四态 {0=ACK-Rec,1=ACK-Wait,2=DATA-Rec,
// 3=DATA-Wait}）。NotifyRecord/Wait 通道级（CKE_IDX_ACK/DATA_SIGNAL——与
// VerbNhrRun 步间屏障同原语族）；peer 显式查表（dataCursor 不动）。
static CcuResult VerbChSync(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx,
                            KfcServerContext::VerbExecState& ve)
{
    const uint32_t rankSize = static_cast<uint32_t>(ctx.arg->rankSize);
    const uint32_t rankId = ctx.arg->rankId;
    const uint32_t peerAlg = static_cast<uint32_t>(param[KFC_VERB_CS_PEER]);
    const uint32_t dir = static_cast<uint32_t>(param[KFC_VERB_CS_DIR]);
    const uint32_t idx = static_cast<uint32_t>(param[KFC_VERB_CS_IDX]);
    if (rankSize == 0U || rankId >= rankSize || peerAlg >= rankSize || dir > 3U) {
        HCCL_ERROR("[KFC][Unified][Verb] CH_SYNC invalid: dir[%u] peer[%u]", dir, peerAlg);
        return CCU_E_INTERNAL;
    }
    // connectRanks 重建（VerbNhrRun :620-641 同式）——peer 查表，dataCursor 不动
    std::vector<uint32_t> connectRanks;
    for (uint32_t delta = 1; delta < rankSize; delta <<= 1) {
        const uint32_t pos = (rankId + delta) % rankSize;
        const uint32_t neg = (rankId + rankSize - delta) % rankSize;
        bool posIn = false;
        bool negIn = false;
        for (size_t k = 0; k < connectRanks.size(); ++k) {
            if (connectRanks[k] == pos) posIn = true;
            if (connectRanks[k] == neg) negIn = true;
        }
        if (!posIn) connectRanks.push_back(pos);
        if (!negIn && neg != pos) connectRanks.push_back(neg);
    }
    for (size_t i = 0; i + 1 < connectRanks.size(); ++i) {
        for (size_t j = i + 1; j < connectRanks.size(); ++j) {
            if (connectRanks[j] < connectRanks[i]) {
                const uint32_t tmp = connectRanks[i];
                connectRanks[i] = connectRanks[j];
                connectRanks[j] = tmp;
            }
        }
    }
    std::vector<uint32_t> chIdxOf(static_cast<size_t>(rankSize), 0xFFFFFFFFU);
    for (size_t k = 0; k < connectRanks.size(); ++k) {
        chIdxOf[connectRanks[k]] = static_cast<uint32_t>(k);
    }
    const uint32_t chIdx = chIdxOf[peerAlg];
    if (chIdx == 0xFFFFFFFFU || chIdx >= ctx.arg->channelCount) {
        HCCL_ERROR("[KFC][Unified][Verb] CH_SYNC peer[%u] not in connect set", peerAlg);
        return CCU_E_INTERNAL;
    }
    if (dir & 1U) {
        CCU_CHK_RET(ccu::NotifyWait(ctx.arg->channels[chIdx], idx));
    } else {
        CCU_CHK_RET(ccu::NotifyRecord(ctx.arg->channels[chIdx], idx));
    }
    return CCU_SUCCESS;
}

static void DispatchUnifiedVerbs(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx,
                                 KfcServerContext::VerbExecState& ve)
{
    ccu::Event evt;   // 帧内事件（数据帧发起与等待共享——批次末 flush 由下一非数据帧/
                      // 段边界触发，老 kernel Phase1/Phase2 分离语义的帧序化承载，M0 §四）
    CCU_IF(param[KFC_UNIFIED_VERB_ID] == static_cast<u64>(KFC_UNIFIED_VERB_SYNC_PRE))
    {
        HCCL_INFO("[KFC][Unified][Verb] SYNC_PEER_PRE (mask in slot1)");
        VerbSyncPre(param, ctx, ve);
    }
    CCU_IF(param[KFC_UNIFIED_VERB_ID] == static_cast<u64>(KFC_UNIFIED_VERB_SYNC_POST))
    {
        HCCL_INFO("[KFC][Unified][Verb] SYNC_PEER_POST");
        VerbSyncPost(ctx);
    }
    CCU_IF(param[KFC_UNIFIED_VERB_ID] == static_cast<u64>(KFC_UNIFIED_VERB_READ))
    {
        VerbRead(param, ctx, ve, evt);
    }
    CCU_IF(param[KFC_UNIFIED_VERB_ID] == static_cast<u64>(KFC_UNIFIED_VERB_WRITE))
    {
        VerbWrite(param, ctx, ve, evt);
    }
    CCU_IF(param[KFC_UNIFIED_VERB_ID] == static_cast<u64>(KFC_UNIFIED_VERB_COPY))
    {
        VerbCopy(param, ctx, ve, evt);
    }
    CCU_IF(param[KFC_UNIFIED_VERB_ID] == static_cast<u64>(KFC_UNIFIED_VERB_BROADCAST))
    {
        VerbBroadcast(param, ctx, ve);
    }
    CCU_IF(param[KFC_UNIFIED_VERB_ID] == static_cast<u64>(KFC_UNIFIED_VERB_REDUCE))
    {
        VerbReduce(param, ctx, ve);
    }
    // R18 · #11：NHR 展开动词（步/slice 循环外置到帧序——逐条执行；NHR_RUN 宏
    // 帧分发 case 随 VerbNhrRun 全删轮删除）
    CCU_IF(param[KFC_UNIFIED_VERB_ID] == static_cast<u64>(KFC_UNIFIED_VERB_SLICE_REDUCE))
    {
        VerbSliceReduce(param, ctx, ve);
    }
    CCU_IF(param[KFC_UNIFIED_VERB_ID] == static_cast<u64>(KFC_UNIFIED_VERB_CH_SYNC))
    {
        VerbChSync(param, ctx, ve);
    }
    // POST_COPY：CCU 侧不发射（AICPU 专用，kCcuVerbMap 占位行）——归入未知 ID 兜底日志
    CCU_IF(param[KFC_UNIFIED_VERB_ID] != static_cast<u64>(KFC_UNIFIED_VERB_SYNC_PRE))
    {
        CCU_IF(param[KFC_UNIFIED_VERB_ID] != static_cast<u64>(KFC_UNIFIED_VERB_SYNC_POST))
        {
            CCU_IF(param[KFC_UNIFIED_VERB_ID] != static_cast<u64>(KFC_UNIFIED_VERB_READ))
            {
                CCU_IF(param[KFC_UNIFIED_VERB_ID] != static_cast<u64>(KFC_UNIFIED_VERB_WRITE))
                {
                    CCU_IF(param[KFC_UNIFIED_VERB_ID] != static_cast<u64>(KFC_UNIFIED_VERB_COPY))
                    {
                        CCU_IF(param[KFC_UNIFIED_VERB_ID] != static_cast<u64>(KFC_UNIFIED_VERB_BROADCAST))
                        {
                            CCU_IF(param[KFC_UNIFIED_VERB_ID] != static_cast<u64>(KFC_UNIFIED_VERB_REDUCE))
                            {
                                CCU_IF(param[KFC_UNIFIED_VERB_ID] != static_cast<u64>(KFC_UNIFIED_VERB_SLICE_REDUCE))
                                {
                                    CCU_IF(param[KFC_UNIFIED_VERB_ID] != static_cast<u64>(KFC_UNIFIED_VERB_CH_SYNC))
                                    {
                                        HCCL_ERROR("[KFC][Unified][Verb] unknown verb id in frame (slot0)");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

static void DispatchKfcSubKernel(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx)
{
    // v3 分叉（§12-11 甲路线）：UNIFIED_PLAN 实例不走 v2 opType/role if-else——
    // 分发判据在每轮 param[0] 动作 ID，不在实例属性。v2 既有分支一行不动。
    if (ctx.arg->role == KfcServerRole::UNIFIED_PLAN) {
        DispatchUnifiedPlanV3(param, ctx);
        return;
    }
    // v5 分叉（R12 · 动词序执行器，2026-10-09）：UNIFIED_VERB 实例（"CcuVerb" 前缀
    // 第二档注册）按逐帧 param[0] 动词 ID → 原语组合（DispatchUnifiedVerbs）。
    // v2/v3 分支零变化（纯新增分支，先于 v2 opType 判定——role 是实例级编译期值）。
    if (ctx.arg->role == KfcServerRole::UNIFIED_VERB) {
        // VerbExecContext 跨帧承载段内状态（对端变量缓存 + 数据帧游标）——server
        // 循环内实例化一次、逐帧传递。对端缓存生命周期 = 实例（SYNC_PRE 覆写），
        // 游标按段归零（VerbSyncPre 内）。
        DispatchUnifiedVerbs(param, ctx, ctx.verbExec);
        return;
    }
    if (ctx.arg->opParam.opType == HcclCMDType::HCCL_CMD_ALLGATHER) {
        const auto* arg = static_cast<const KfcParallelAllGatherArg*>(ctx.arg->algArg.get());
        if (ctx.arg->algSubType == KFC_PARALLEL_ALL_GATHER_SUB_TYPE && ctx.arg->role == KfcServerRole::ALL_GATHER_NHR) {
            CcuKfcParallelAllGatherNHR1DMultiJettyMem2MemKernel(
                param[KFC_PARALLEL_AG_INPUT], param[KFC_PARALLEL_AG_OUTPUT], ctx.token,
                param[KFC_PARALLEL_AG_OUTPUT_STRIDE], param[KFC_PARALLEL_AG_PART0_SIZE],
                param[KFC_PARALLEL_AG_PART1_SIZE], param[KFC_PARALLEL_AG_PART1_OFFSET],
                param[KFC_PARALLEL_AG_MESH_PHASE_DONE_ADDR], param[KFC_PARALLEL_AG_NHR_PHASE_DONE_ADDR],
                param[KFC_PARALLEL_AG_PART0_SLICE_PER_JETTY], param[KFC_PARALLEL_AG_PART0_LAST_SLICE_PER_JETTY],
                param[KFC_PARALLEL_AG_PART1_SLICE_PER_JETTY], param[KFC_PARALLEL_AG_PART1_LAST_SLICE_PER_JETTY],
                param[KFC_PARALLEL_AG_PART0_GO_SIZE_0], param[KFC_PARALLEL_AG_PART0_GO_SIZE_1],
                param[KFC_PARALLEL_AG_PART0_GO_SIZE_2], param[KFC_PARALLEL_AG_PART0_GO_SIZE_3],
                param[KFC_PARALLEL_AG_PART1_GO_SIZE_0], param[KFC_PARALLEL_AG_PART1_GO_SIZE_1],
                param[KFC_PARALLEL_AG_PART1_GO_SIZE_2], param[KFC_PARALLEL_AG_PART1_GO_SIZE_3], ctx.arg->channels,
                ctx.arg->channelCount, arg->rankSizeLevel0, arg->rankIdxLevel0, arg->rankSizeLevel1, arg->rankIdxLevel1,
                arg->jettyNum, arg->stepInfoVector, arg->rank2ChannelIdx);
        } else if (ctx.arg->algSubType == KFC_PARALLEL_ALL_GATHER_SUB_TYPE) {
            CcuKfcParallelAllGatherMesh1DMem2MemKernel(
                param[KFC_PARALLEL_AG_INPUT], param[KFC_PARALLEL_AG_OUTPUT], ctx.token,
                param[KFC_PARALLEL_AG_OUTPUT_STRIDE], param[KFC_PARALLEL_AG_PART0_SIZE],
                param[KFC_PARALLEL_AG_PART1_SIZE], param[KFC_PARALLEL_AG_PART1_OFFSET],
                param[KFC_PARALLEL_AG_MESH_PHASE_DONE_ADDR], param[KFC_PARALLEL_AG_NHR_PHASE_DONE_ADDR],
                param[KFC_PARALLEL_AG_PART0_GO_SIZE_0], param[KFC_PARALLEL_AG_PART0_GO_SIZE_1],
                param[KFC_PARALLEL_AG_PART0_GO_SIZE_2], param[KFC_PARALLEL_AG_PART0_GO_SIZE_3],
                param[KFC_PARALLEL_AG_PART1_GO_SIZE_0], param[KFC_PARALLEL_AG_PART1_GO_SIZE_1],
                param[KFC_PARALLEL_AG_PART1_GO_SIZE_2], param[KFC_PARALLEL_AG_PART1_GO_SIZE_3], ctx.arg->channels,
                ctx.arg->channelCount, arg->rankSizeLevel0, arg->rankIdxLevel0, arg->rankSizeLevel1,
                arg->rankIdxLevel1);
        } else if (ctx.arg->role == KfcServerRole::ALL_GATHER_NHR) {
            CcuKfcAllGatherNHR1DMultiJettyMem2MemKernel(
                param[KFC_CONCURRENT_AG_NHR_INPUT], param[KFC_CONCURRENT_AG_NHR_OUTPUT], ctx.token,
                param[KFC_CONCURRENT_AG_NHR_SLICE_SIZE], param[KFC_CONCURRENT_AG_NHR_SLICE_SIZE_PER_JETTY],
                param[KFC_CONCURRENT_AG_NHR_LAST_SLICE_SIZE_PER_JETTY], param[KFC_CONCURRENT_AG_NHR_REPEAT_NUM_INV],
                param[KFC_CONCURRENT_AG_NHR_INPUT_SLICE_STRIDE], param[KFC_CONCURRENT_AG_NHR_OUTPUT_SLICE_STRIDE],
                param[KFC_CONCURRENT_AG_NHR_INPUT_REPEAT_STRIDE], param[KFC_CONCURRENT_AG_NHR_OUTPUT_REPEAT_STRIDE],
                param[KFC_CONCURRENT_AG_NHR_INPUT_OUTPUT_EQUAL], param[KFC_CONCURRENT_AG_NHR_GO_SIZE_0],
                param[KFC_CONCURRENT_AG_NHR_GO_SIZE_1], param[KFC_CONCURRENT_AG_NHR_GO_SIZE_2],
                param[KFC_CONCURRENT_AG_NHR_GO_SIZE_3], ctx.arg->channels, ctx.arg->channelCount,
                static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->jettyNum,
                ctx.arg->nhrStepInfoVector, ctx.arg->nhrRank2ChannelIdx);
        } else if (ctx.arg->role == KfcServerRole::ALL_GATHER_SOLE_NHR) {
            // SoleNHR（CcuSchedAllGatherSoleNHR）单帧布局见 KfcAllGatherSoleNhrParamIndex
            // （8 槽，[0]=INPUT 兼作哨兵，常量参数 kernel 内联，token 由 server ctx 注入）。
            CcuKernelKfcAllGatherNHR1DMem2MemKernel(
                param[KFC_AG_SOLE_NHR_INPUT], param[KFC_AG_SOLE_NHR_OUTPUT], ctx.token,
                param[KFC_AG_SOLE_NHR_DIE0_SIZE], param[KFC_AG_SOLE_NHR_OUTPUT_SLICE_STRIDE],
                param[KFC_AG_SOLE_NHR_GO_SIZE_0], param[KFC_AG_SOLE_NHR_GO_SIZE_1], param[KFC_AG_SOLE_NHR_GO_SIZE_2],
                param[KFC_AG_SOLE_NHR_GO_SIZE_3], ctx.arg->channels, ctx.arg->channelCount,
                static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->nhrStepInfoVector,
                ctx.arg->nhrRank2ChannelIdx);
        } else {
            // The queue payload layout is
            // [0]=op, [1]=input, [2]=output, [3]=outputOffset, [4]=sliceSize,
            // [5..8]=goSize, [9]=inputOffset, [10]=repeat, [11..12]=strides,
            // [13]=lastSliceSize, [14]=inplace.  The memory token is immutable
            // for the KFC server lifetime and comes from ctx.token.
            HCCL_INFO("[KFC][AllGather][Dispatch] route queue payload to CcuKfcAllGatherMesh1DMem2MemKernel, "
                      "token=serverContext");

            CcuKfcAllGatherMesh1DMem2MemKernel(
                param[KFC_CONCURRENT_AG_MESH_INPUT], param[KFC_CONCURRENT_AG_MESH_OUTPUT], ctx.token,
                param[KFC_CONCURRENT_AG_MESH_OUTPUT_OFFSET], param[KFC_CONCURRENT_AG_MESH_SLICE_SIZE],
                param[KFC_CONCURRENT_AG_MESH_GO_SIZE_0], param[KFC_CONCURRENT_AG_MESH_GO_SIZE_1],
                param[KFC_CONCURRENT_AG_MESH_GO_SIZE_2], param[KFC_CONCURRENT_AG_MESH_GO_SIZE_3],
                param[KFC_CONCURRENT_AG_MESH_CURRENT_RANK_SLICE_INPUT_OFFSET],
                param[KFC_CONCURRENT_AG_MESH_REPEAT_NUM_INV], param[KFC_CONCURRENT_AG_MESH_INPUT_REPEAT_STRIDE],
                param[KFC_CONCURRENT_AG_MESH_OUTPUT_REPEAT_STRIDE], param[KFC_CONCURRENT_AG_MESH_LAST_SLICE_SIZE],
                param[KFC_CONCURRENT_AG_MESH_INPUT_OUTPUT_EQUAL], ctx.arg->channels, ctx.arg->channelCount,
                static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId);
        }
    } else if (ctx.arg->opParam.opType == HcclCMDType::HCCL_CMD_REDUCE_SCATTER) {
        if (ctx.arg->opParam.algName != nullptr &&
            std::strcmp(ctx.arg->opParam.algName, KFC_REDUCE_SCATTER_PEER_ONLY_ALG_NAME) == 0) {
            CcuReduceScatterMeshMem2Mem1DPeerOnlyKernel(
                param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], ctx.token, param[HBM_PARAM_IDX_4],
                param[HBM_PARAM_IDX_5], param[HBM_PARAM_IDX_7], param[HBM_PARAM_IDX_8], ctx.arg->channels,
                ctx.arg->channelCount, static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId,
                ctx.arg->opParam.DataDes.dataType, ctx.arg->opParam.reduceType);
        } else if (ctx.arg->role == KfcServerRole::REDUCE_SCATTER_SOLE_NHR) {
            // SoleNHRMultiLink 单帧布局见 KfcReduceScatterSoleNhrParamIndex（[0]=opId，[1..9] 为 NHR 参数）。
            CcuKfcReduceScatterNHR1DMultiJettyMem2MemKernel(
                param[KFC_RS_SOLE_NHR_INPUT], param[KFC_RS_SOLE_NHR_OUTPUT], ctx.token,
                param[KFC_RS_SOLE_NHR_SLICE_SIZE], param[KFC_RS_SOLE_NHR_INPUT_SLICE_STRIDE],
                param[KFC_RS_SOLE_NHR_SLICE_ONE_JETTY_SIZE], param[KFC_RS_SOLE_NHR_SLICE_LAST_JETTY_SIZE],
                param[KFC_RS_SOLE_NHR_REPEAT_NUM_INV], param[KFC_RS_SOLE_NHR_INPUT_REPEAT_STRIDE],
                param[KFC_RS_SOLE_NHR_OUTPUT_REPEAT_STRIDE], ctx.arg->channels, ctx.arg->channelCount,
                static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->jettyNum,
                ctx.arg->opParam.DataDes.dataType, ctx.arg->opParam.DataDes.outputType, ctx.arg->opParam.reduceType,
                ctx.arg->nhrStepInfoVector, ctx.arg->nhrRank2ChannelIdx);
        } else if (ctx.arg->role == KfcServerRole::REDUCE_SCATTER_SOLE_NHR_2DIE) {
            // SoleNHR 2Die 单帧布局见 KfcReduceScatterSoleNhr2DieParamIndex（[0]=opId，[1..20] 共 20 参数，
            // token 走 ctx.token）；双 die 两 mission 共享区间，axisId/axisSize 来自 kernelArg 区分实例。
            CcuKfcReduceScatterNHR1D2DieMem2MemKernel(
                param[KFC_RS_SOLE_NHR_2DIE_INPUT], param[KFC_RS_SOLE_NHR_2DIE_OUTPUT], ctx.token,
                param[KFC_RS_SOLE_NHR_2DIE_DIE0_SIZE], param[KFC_RS_SOLE_NHR_2DIE_DIE1_SIZE],
                param[KFC_RS_SOLE_NHR_2DIE_DIE0_LAST_SLICE_SIZE], param[KFC_RS_SOLE_NHR_2DIE_DIE1_LAST_SLICE_SIZE],
                param[KFC_RS_SOLE_NHR_2DIE_INPUT_SLICE_STRIDE],
                param[KFC_RS_SOLE_NHR_2DIE_CURRENT_RANK_SLICE_OUTPUT_OFFSET],
                param[KFC_RS_SOLE_NHR_2DIE_INPUT_REPEAT_STRIDE], param[KFC_RS_SOLE_NHR_2DIE_OUTPUT_REPEAT_STRIDE],
                param[KFC_RS_SOLE_NHR_2DIE_REPEAT_NUM_VAR], param[KFC_RS_SOLE_NHR_2DIE_IS_INPUT_OUTPUT_EQUAL],
                param[KFC_RS_SOLE_NHR_2DIE_GO_SIZE_NORMAL_0], param[KFC_RS_SOLE_NHR_2DIE_GO_SIZE_NORMAL_1],
                param[KFC_RS_SOLE_NHR_2DIE_GO_SIZE_NORMAL_2], param[KFC_RS_SOLE_NHR_2DIE_GO_SIZE_NORMAL_3],
                param[KFC_RS_SOLE_NHR_2DIE_GO_SIZE_LAST_0], param[KFC_RS_SOLE_NHR_2DIE_GO_SIZE_LAST_1],
                param[KFC_RS_SOLE_NHR_2DIE_GO_SIZE_LAST_2], param[KFC_RS_SOLE_NHR_2DIE_GO_SIZE_LAST_3],
                param[KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_NORMAL_0], param[KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_NORMAL_1],
                param[KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_NORMAL_2], param[KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_NORMAL_3],
                param[KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_LAST_0], param[KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_LAST_1],
                param[KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_LAST_2], param[KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_LAST_3],
                ctx.arg->channels, ctx.arg->channelCount, static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId,
                ctx.arg->axisId, ctx.arg->axisSize, ctx.arg->opParam.DataDes.dataType,
                ctx.arg->opParam.DataDes.outputType, ctx.arg->opParam.reduceType, ctx.arg->nhrStepInfoVector,
                ctx.arg->nhrRank2ChannelIdx);
        } else if (ctx.arg->role == KfcServerRole::REDUCE_SCATTER_NHR) {
            // 并发 RS 的 NHR mission 读 [16..24] 区间（见 KfcConcurrentReduceScatterParamIndex）。
            CcuKfcReduceScatterNHR1DMultiJettyMem2MemKernel(
                param[KFC_CONCURRENT_RS_NHR_INPUT], param[KFC_CONCURRENT_RS_NHR_OUTPUT], ctx.token,
                param[KFC_CONCURRENT_RS_NHR_SLICE_SIZE], param[KFC_CONCURRENT_RS_NHR_INPUT_SLICE_STRIDE],
                param[KFC_CONCURRENT_RS_NHR_SLICE_ONE_JETTY_SIZE], param[KFC_CONCURRENT_RS_NHR_SLICE_LAST_JETTY_SIZE],
                param[KFC_CONCURRENT_RS_NHR_REPEAT_NUM_INV], param[KFC_CONCURRENT_RS_NHR_INPUT_REPEAT_STRIDE],
                param[KFC_CONCURRENT_RS_NHR_OUTPUT_REPEAT_STRIDE], ctx.arg->channels, ctx.arg->channelCount,
                static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->jettyNum,
                ctx.arg->opParam.DataDes.dataType, ctx.arg->opParam.DataDes.outputType, ctx.arg->opParam.reduceType,
                ctx.arg->nhrStepInfoVector, ctx.arg->nhrRank2ChannelIdx);
        } else if (ctx.arg->role == KfcServerRole::REDUCE_SCATTER_MESH) {
            // 并发 RS 的 Mesh mission 读 [1..15] 紧凑区间，形参顺序与 kernel 定义一致。
            CcuReduceScatterMesh1DMem2MemKernel(
                param[KFC_CONCURRENT_RS_MESH_INPUT], param[KFC_CONCURRENT_RS_MESH_OUTPUT], ctx.token,
                param[KFC_CONCURRENT_RS_MESH_SCRATCH], param[KFC_CONCURRENT_RS_MESH_RANK_SLICE_OFFSET],
                param[KFC_CONCURRENT_RS_MESH_CHUNK_SIZE], param[KFC_CONCURRENT_RS_MESH_CHUNK_LOOP_NUM],
                param[KFC_CONCURRENT_RS_MESH_TAIL_SIZE], param[KFC_CONCURRENT_RS_MESH_FULL_GO_ADDR_OFFSET],
                param[KFC_CONCURRENT_RS_MESH_FULL_GO_LOOP_PARAM], param[KFC_CONCURRENT_RS_MESH_FULL_GO_PARALLEL_PARAM],
                param[KFC_CONCURRENT_RS_MESH_FULL_GO_RESIDUAL], param[KFC_CONCURRENT_RS_MESH_TAIL_GO_ADDR_OFFSET],
                param[KFC_CONCURRENT_RS_MESH_TAIL_GO_LOOP_PARAM], param[KFC_CONCURRENT_RS_MESH_TAIL_GO_PARALLEL_PARAM],
                param[KFC_CONCURRENT_RS_MESH_TAIL_GO_RESIDUAL], ctx.arg->channels, ctx.arg->channelCount,
                static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->opParam.DataDes.dataType,
                ctx.arg->opParam.DataDes.outputType, ctx.arg->opParam.reduceType);
        } else {
            // SoleMesh 路径保持 homm 兼容布局：[0..12] homm 专用 + [13..23] KFC chunk 参数。
            CcuReduceScatterMesh1DMem2MemKernel(
                param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], ctx.token, param[HBM_PARAM_IDX_3],
                param[HBM_PARAM_IDX_4], param[HBM_PARAM_IDX_KFC_CHUNK_SIZE], param[HBM_PARAM_IDX_KFC_CHUNK_LOOP_NUM],
                param[HBM_PARAM_IDX_KFC_TAIL_SIZE], param[HBM_PARAM_IDX_KFC_FULL_GO_ADDR_OFFSET],
                param[HBM_PARAM_IDX_KFC_FULL_GO_LOOP_PARAM], param[HBM_PARAM_IDX_KFC_FULL_GO_PARALLEL_PARAM],
                param[HBM_PARAM_IDX_KFC_FULL_GO_RESIDUAL], param[HBM_PARAM_IDX_KFC_TAIL_GO_ADDR_OFFSET],
                param[HBM_PARAM_IDX_KFC_TAIL_GO_LOOP_PARAM], param[HBM_PARAM_IDX_KFC_TAIL_GO_PARALLEL_PARAM],
                param[HBM_PARAM_IDX_KFC_TAIL_GO_RESIDUAL], ctx.arg->channels, ctx.arg->channelCount,
                static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->opParam.DataDes.dataType,
                ctx.arg->opParam.DataDes.outputType, ctx.arg->opParam.reduceType);
        }
    } else if (ctx.arg->opParam.opType == HcclCMDType::HCCL_CMD_ALLREDUCE) {
        CcuKfcAllReduceMesh1DMem2MemKernel(
            param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], ctx.token, param[HBM_PARAM_IDX_3], param[HBM_PARAM_IDX_4],
            param[HBM_PARAM_IDX_5], param[HBM_PARAM_IDX_6], param[HBM_PARAM_IDX_7], param[HBM_PARAM_IDX_8],
            param[HBM_PARAM_IDX_9], param[HBM_PARAM_IDX_10], param[HBM_PARAM_IDX_11], param[HBM_PARAM_IDX_12],
            param[HBM_PARAM_IDX_13], param[HBM_PARAM_IDX_14], param[HBM_PARAM_IDX_15], param[HBM_PARAM_IDX_16],
            param[HBM_PARAM_IDX_17], param[HBM_PARAM_IDX_18], param[HBM_PARAM_IDX_19], param[HBM_PARAM_IDX_20],
            param[HBM_PARAM_IDX_21], param[HBM_PARAM_IDX_22], param[HBM_PARAM_IDX_23], param[HBM_PARAM_IDX_24],
            param[HBM_PARAM_IDX_25], ctx.arg->channels, ctx.arg->channelCount, static_cast<uint32_t>(ctx.arg->rankSize),
            ctx.arg->rankId, ctx.arg->opParam.DataDes.dataType, ctx.arg->opParam.DataDes.outputType,
            ctx.arg->opParam.reduceType);
    } else if (ctx.arg->opParam.opType == HcclCMDType::HCCL_CMD_ALLTOALLV) {
        CcuAlltoAllVMesh1DKernel(
            param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], ctx.token, param[HBM_PARAM_IDX_3], param[HBM_PARAM_IDX_4],
            param[HBM_PARAM_IDX_9], param[HBM_PARAM_IDX_5], param[HBM_PARAM_IDX_6], param[HBM_PARAM_IDX_7],
            param[HBM_PARAM_IDX_8], ctx.arg->channels, ctx.arg->channelCount, static_cast<uint32_t>(ctx.arg->rankSize),
            ctx.arg->rankId);
    } else if (ctx.arg->opParam.opType == HcclCMDType::HCCL_CMD_ALLTOALL) {
        if (ctx.arg->role == KfcServerRole::ALL_TO_ALL_MULTI_JETTY) {
            CcuAllToAllMesh1DMultiJettyKernel(
                param[KFC_A2A_MJ_INPUT], param[KFC_A2A_MJ_OUTPUT], ctx.token, param[KFC_A2A_MJ_SLICE_SIZE],
                param[KFC_A2A_MJ_SRC_STRIDE], param[KFC_A2A_MJ_SRC_OFFSET], param[KFC_A2A_MJ_DST_OFFSET],
                param[KFC_A2A_MJ_GO_SIZE_0], param[KFC_A2A_MJ_GO_SIZE_1], param[KFC_A2A_MJ_GO_SIZE_2],
                param[KFC_A2A_MJ_GO_SIZE_3], param[KFC_A2A_MJ_SLICE_SIZE_PER_JETTY],
                param[KFC_A2A_MJ_LAST_SLICE_SIZE_PER_JETTY], ctx.arg->channels, ctx.arg->channelCount,
                static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->jettyNum);
        } else if (ctx.arg->role == KfcServerRole::ALL_TO_ALL_MESH) {
            CcuAllToAllMesh1DMultiJettyKernel(
                param[KFC_CONCURRENT_A2A_MESH_INPUT], param[KFC_CONCURRENT_A2A_MESH_OUTPUT], ctx.token,
                param[KFC_CONCURRENT_A2A_MESH_SLICE_SIZE], param[KFC_CONCURRENT_A2A_MESH_SRC_STRIDE],
                param[KFC_CONCURRENT_A2A_MESH_SRC_OFFSET], param[KFC_CONCURRENT_A2A_MESH_DST_OFFSET],
                param[KFC_CONCURRENT_A2A_MESH_GO_SIZE_0], param[KFC_CONCURRENT_A2A_MESH_GO_SIZE_1],
                param[KFC_CONCURRENT_A2A_MESH_GO_SIZE_2], param[KFC_CONCURRENT_A2A_MESH_GO_SIZE_3],
                param[KFC_CONCURRENT_A2A_MESH_SLICE_SIZE_PER_JETTY],
                param[KFC_CONCURRENT_A2A_MESH_LAST_SLICE_SIZE_PER_JETTY], ctx.arg->channels, ctx.arg->channelCount,
                static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->jettyNum);
        } else if (ctx.arg->role == KfcServerRole::ALL_TO_ALL_CLOS) {
            CcuAllToAllMesh1DMultiJettyKernel(
                param[KFC_CONCURRENT_A2A_CLOS_INPUT], param[KFC_CONCURRENT_A2A_CLOS_OUTPUT], ctx.token,
                param[KFC_CONCURRENT_A2A_CLOS_SLICE_SIZE], param[KFC_CONCURRENT_A2A_CLOS_SRC_STRIDE],
                param[KFC_CONCURRENT_A2A_CLOS_SRC_OFFSET], param[KFC_CONCURRENT_A2A_CLOS_DST_OFFSET],
                param[KFC_CONCURRENT_A2A_CLOS_GO_SIZE_0], param[KFC_CONCURRENT_A2A_CLOS_GO_SIZE_1],
                param[KFC_CONCURRENT_A2A_CLOS_GO_SIZE_2], param[KFC_CONCURRENT_A2A_CLOS_GO_SIZE_3],
                param[KFC_CONCURRENT_A2A_CLOS_SLICE_SIZE_PER_JETTY],
                param[KFC_CONCURRENT_A2A_CLOS_LAST_SLICE_SIZE_PER_JETTY], ctx.arg->channels, ctx.arg->channelCount,
                static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->jettyNum);
        } else {
            CcuAlltoAllMesh1DKernel(
                param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], ctx.token, param[HBM_PARAM_IDX_3],
                param[HBM_PARAM_IDX_4], param[HBM_PARAM_IDX_5], param[HBM_PARAM_IDX_6], param[HBM_PARAM_IDX_7],
                param[HBM_PARAM_IDX_8], param[HBM_PARAM_IDX_9], param[HBM_PARAM_IDX_10], ctx.arg->channels,
                ctx.arg->channelCount, static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->loadFromMem);
        }
    }
}

CcuResult KfcServerGenCircularQueue(KfcServerContext& ctx)
{
    HCCL_INFO("[KfcServerGenCircularQueue] KfcServerGenCircularQueue start");

    ccu::Variable repeatCond; // 存放《控制repeat循环执行的条件》的寄存器
    repeatCond = 0;
    ccu::Variable turnStartSig; // 存放《轮次执行开始信号》的寄存器
    turnStartSig = 0;
    ccu::Variable turnEndSig; // 存放《轮次执行完成信号》的寄存器，在循环中固定为1
    turnEndSig = 1;

    // 使用独立XN保存队列游标，避免Variable拷贝构造与context基址共享handle。
    ccu::Variable waitAddr;
    ccu::Variable recordAddr;
    ccu::Variable paramAddr;
    waitAddr = ctx.waitAddr;
    recordAddr = ctx.recordAddr;
    paramAddr = ctx.xnAddr;

    ccu::Variable ckeSize; // 其他寄存器
    ckeSize = CCU_ONE_PARAM_SIZE;
    ccu::Variable paramSize;
    paramSize = CCU_PARAM_NUM_MAX * CCU_ONE_PARAM_SIZE;

    ccu::Variable queueIdx;
    ccu::Variable queueEnd;
    ccu::Variable one;
    queueIdx = 0;
    one = 1;
    queueEnd = CCU_TASK_NUM_MAX;

    // 存放<每轮算子参数>的寄存器
    ccu::Array<ccu::Variable> param(CCU_PARAM_NUM_PER_DIE);

    CCU_WHILE(repeatCond == 0)
    {
        CCU_CHK_RET(WaitTurnStartSig(waitAddr, turnStartSig, ctx));  // 等待本轮开始信号
        CCU_CHK_RET(LoadFuncParamFromMemory(paramAddr, param, ctx)); // 读取本轮参数
        CCU_IF(param[HBM_PARAM_IDX_0] == INVALID_U64) { repeatCond = 1; }
        CCU_IF(param[HBM_PARAM_IDX_0] != INVALID_U64)
        {
            MissionPreSync(param[HBM_PARAM_IDX_0], ctx);
            DispatchKfcSubKernel(param, ctx);
            MissionPostSync(ctx);
            CCU_CHK_RET(SetTurnEndSig(recordAddr, turnEndSig, ctx));
        }
        waitAddr += ckeSize;
        recordAddr += ckeSize;
        paramAddr += paramSize;
        queueIdx += one;
        CCU_IF(queueIdx == static_cast<u64>(CCU_TASK_NUM_MAX))
        {
            waitAddr = ctx.waitAddr;
            recordAddr = ctx.recordAddr;
            paramAddr = ctx.xnAddr;
            queueIdx = 0;
        }
    }
    return CCU_SUCCESS;
}

CcuResult SlaveKfcServerGenCircularQueue(KfcServerContext& ctx)
{
    ccu::Variable signature; // 算子签名

    ccu::Variable repeatCond; // 存放《控制repeat循环执行的条件》的寄存器
    repeatCond = 0;

    ccu::Variable paramAddr;
    paramAddr = ctx.xnAddr;

    ccu::Array<ccu::Variable> param(CCU_PARAM_NUM_PER_DIE); // 存放<每轮算子参数>的寄存器

    CCU_WHILE(repeatCond == 0)
    {
        MissionPreSync(signature, ctx);                              // 在context中依次加入8轮指令
        CCU_CHK_RET(LoadFuncParamFromMemory(paramAddr, param, ctx)); // 读取本轮参数

        CCU_IF(param[HBM_PARAM_IDX_0] == INVALID_U64) { repeatCond = 1; }
        CCU_IF(param[HBM_PARAM_IDX_0] != INVALID_U64) { DispatchKfcSubKernel(param, ctx); }
        MissionPostSync(ctx);
    }
    return CCU_SUCCESS;
}

// ============================================================================
// 主入口 Kernel 函数
// ============================================================================
CcuResult CcuKfcServerKernel(CcuKernelArg arg)
{
    auto* kernelArg = static_cast<CcuKernelArgKfcServer*>(arg);
    KfcServerContext ctx;
    HCCL_INFO("[CcuKernelKfcServer] KfcServer run");
    CCU_CHK_RET(ParseKernelArg(ctx, kernelArg));
    HCCL_INFO("[CcuKernelKfcServer] ParseKernelArg finish");
    CCU_CHK_RET(InitResource(ctx));
    HCCL_INFO("[CcuKernelKfcServer] InitResource finish");
    CCU_CHK_RET(LoadArgs(ctx));
    HCCL_INFO("[CcuKernelKfcServer] LoadArgs finish");
    CCU_CHK_RET(CompArgs(ctx));
    HCCL_INFO("[CcuKernelKfcServer] CompArgs finish");
    HCCL_INFO("[GenCircularQueue] KfcServer start");
    CCU_CHK_RET(KfcServerGenCircularQueue(ctx));
    HCCL_INFO("[CcuKernelKfcServer] KfcServer end");
    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
