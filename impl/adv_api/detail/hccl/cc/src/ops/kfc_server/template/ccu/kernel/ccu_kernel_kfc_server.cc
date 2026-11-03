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
#include "../../../../all_to_all_v/template/ccu/kernel/ccu_kernel_kfc_half_alltoallv_mesh1d.h"
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
static_assert(
    KFC_RS_SOLE_NHR_2DIE_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE,
    "SoleNHR 2Die ReduceScatter parameters overlap server Func arguments");
static_assert(
    KFC_PARALLEL_AG_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE, "Parallel AllGather parameters overlap server Func arguments");
static_assert(KFC_MAX_MISSION_NUM == 2U, "KFC server CKE partitioning expects two missions");
static_assert(KFC_SIGNAL_REGIONS_PER_MISSION == 2U, "Each KFC mission requires commit and done regions");

const uint32_t HBM_PARAM_IDX_0 = 0;

const uint32_t SINGLE_DIE = 1;
const uint32_t DOUBLE_DIE = 2;
const uint32_t DIE0_ID = 0;
const uint32_t DIE1_ID = 1;
constexpr const char* DIE1_START_SIG = "Die1StartSig";
constexpr const char* DIE1_END_SIG = "Die1EndSig";

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
    CCU_CHK_RET(ccu::LoadArg(ctx.subKernelPc, argId++));

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

CcuResult MissionPreSync(const ccu::Variable& func, KfcServerContext& ctx)
{
    (void)func;

    // 单 mission 时短路；门控必须用宿主常量（CCU_IF 两分支构图期都会执行）
    const uint32_t hostMissionNum = GetKfcServerMissionNum(ctx.arg->opParam.algName);
    if (hostMissionNum == 1) {
        return CCU_SUCCESS;
    }

    // tag 栅栏实现 mission 间互斥等待。tag 按发出方 mission 命名，每个 kernel 只
    // export 一个 tag（同名 tag 会在 ProcessInterCtxRes 合并时冲突）；record 注册到
    // imported（发信号侧）、wait 注册到 exported（等信号侧），跨 mission 配对。
    if (ctx.arg->hostMissionIndex == 0) {
        // mission 0: post bit 0, wait bit 1
        CCU_CHK_RET(ccu::EventRecord("MiSync_m0", 1 << 0));
        CCU_CHK_RET(ccu::EventWait("MiSync_m1", 1 << 1));
    } else {
        // mission 1: post bit 1, wait bit 0
        CCU_CHK_RET(ccu::EventRecord("MiSync_m1", 1 << 1));
        CCU_CHK_RET(ccu::EventWait("MiSync_m0", 1 << 0));
    }
    return CCU_SUCCESS;
}

CcuResult MissionPostSync(KfcServerContext& ctx)
{
    // 单 mission 时短路；双 mission 下远程 Post 由算子 kernel 的 PostSync 实现
    const uint32_t hostMissionNum = GetKfcServerMissionNum(ctx.arg->opParam.algName);
    if (hostMissionNum == 1) {
        return CCU_SUCCESS;
    }
    return CCU_SUCCESS;
}

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

// Copy existing handles in the common Func ABI; no Variable assignments are emitted.
static CcuResult CallKfcSubKernel(
    ccu::Array<ccu::Variable>& param, const KfcServerContext& ctx, const ccu::Variable& turnEndAddr)
{
    // HalfAlltoAllV still uses its inline implementation and has no named sub-kernel Func.
    if (ctx.arg->opType == HcclCMDType::HCCL_CMD_HALF_ALLTOALLV) {
        return CcuKfcHalfAlltoAllVMesh1DKernel(
            param[1], param[2], param[3], param[4], param[5], param[6], param[7], param[8], ctx.token,
            ctx.arg->channels, ctx.arg->channelCount, static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId,
            ctx.arg->localWinAddr, ctx.missionNum, ctx.missionIndex, ctx.arg->hostMissionIndex,
            GetKfcServerMissionNum(ctx.arg->opParam.algName));
    }
    static_assert(KFC_FUNC_TURN_END_ARG_INDEX + 1U == CCU_FUNC_PARAM_NUM, "KFC Func argument width mismatch");
    if (param.size() != CCU_PARAM_NUM_PER_DIE) {
        return CCU_E_PARA;
    }
    std::vector<ccu::Variable> args;
    args.reserve(CCU_FUNC_PARAM_NUM);
    for (uint32_t i = 0; i < KFC_FUNC_TOKEN_ARG_INDEX; ++i) {
        args.emplace_back(param[i]);
    }
    args.emplace_back(ctx.token);
    for (uint32_t i = KFC_FUNC_TOKEN_ARG_INDEX; i < CCU_FUNC_PARAM_NUM - 2U; ++i) {
        args.emplace_back(param[i]);
    }
    args.emplace_back(turnEndAddr);
    return ccu::CallFunc(ctx.subKernelPc, args);
}

// SoleNHR AG kernel 在内部（末步同步后、PostSync 前）提前写 turn-end，server 侧跳过
// 本轮 SetTurnEndSig（D3）。判断须与子 Func 的 SoleNHR AG 提前完成行为严格一致
// （opType+role，二者由 algName 唯一决定，不会交叉）。不可对同一 WAIT 槽二次置位：
// AIV 检测清零后残留的 1 会在 64 轮槽位回绕时造成假完成信号。
static bool KernelEmitsEarlyTurnEnd(const KfcServerContext& ctx)
{
    return ctx.arg->opParam.opType == HcclCMDType::HCCL_CMD_ALLGATHER &&
           ctx.arg->role == KfcServerRole::ALL_GATHER_SOLE_NHR;
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
            CCU_CHK_RET(MissionPreSync(param[HBM_PARAM_IDX_0], ctx));
            CCU_CHK_RET(CallKfcSubKernel(param, ctx, recordAddr));
            CCU_CHK_RET(MissionPostSync(ctx));
            // D3：SoleNHR AG kernel 已在末步同步后、PostSync 前写 turn-end（AIV 提前返回），
            // 其余角色维持 dispatch 后统一发放。条件为宿主侧常量，不产生运行时分支指令。
            if (!KernelEmitsEarlyTurnEnd(ctx)) {
                CCU_CHK_RET(SetTurnEndSig(recordAddr, turnEndSig, ctx));
            }
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
    ccu::Variable signature;  // 算子签名
    ccu::Variable repeatCond; // 存放《控制repeat循环执行的条件》的寄存器
    repeatCond = 0;

    ccu::Variable paramAddr;
    paramAddr = ctx.xnAddr;

    // 参数循环队列 wraparound 游标（声明即初始化见下方赋值，参数帧推进与回绕共用）。
    // 不可写为 "Variable x = imm"：Variable 无整型转换构造，须经 operator= 赋值。
    ccu::Variable paramSize;
    paramSize = CCU_PARAM_NUM_MAX * CCU_ONE_PARAM_SIZE;
    ccu::Variable queueIdx;
    queueIdx = 0;
    ccu::Variable one;
    one = 1;

    ccu::Array<ccu::Variable> param(CCU_PARAM_NUM_PER_DIE); // 存放<每轮算子参数>的寄存器

    CCU_WHILE(repeatCond == 0)
    {
        // 哨兵轮次不进 barrier，保持 slave 与 master 的 barrier 对称
        CCU_CHK_RET(LoadFuncParamFromMemory(paramAddr, param, ctx));
        CCU_IF(param[HBM_PARAM_IDX_0] == INVALID_U64) { repeatCond = 1; }
        CCU_IF(param[HBM_PARAM_IDX_0] != INVALID_U64)
        {
            CCU_CHK_RET(MissionPreSync(param[HBM_PARAM_IDX_0], ctx));
            // turnEndAddr 仅 SoleNHR AG 消费；slave 不会以该角色 dispatch（死代码路径，保持编译）。
            CCU_CHK_RET(CallKfcSubKernel(param, ctx, ctx.recordAddr));
            CCU_CHK_RET(MissionPostSync(ctx));
        }
        // 推进参数地址游标，wraparound at CCU_TASK_NUM_MAX
        paramAddr += paramSize;
        queueIdx += one;
        CCU_IF(queueIdx == static_cast<u64>(CCU_TASK_NUM_MAX))
        {
            paramAddr = ctx.xnAddr;
            queueIdx = 0;
        }
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
