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
#include "../../../../all_gather/template/ccu/kernel/ccu_kernel_kfc_all_gather_mesh1d_mem2mem.h"
#include "../../../../reduce_scatter/template/ccu/kernel/ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.h"
#include "../../../../all_reduce/template/ccu/kernel/ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.h"
#include "ccu_kernel_kfc_server.h"
#include "ccu_variable_dl.hpp"
#include "ccu_func_dl.hpp"
#include "mc2_type.h"
#include "hccl_alloc_ctx_res.h"

namespace mc2_ops_hccl {

using namespace hcomm;
using namespace std;

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

const uint32_t SINGLE_DIE = 1;
const uint32_t DOUBLE_DIE = 2;
const uint32_t DIE0_ID = 0;
const uint32_t DIE1_ID = 1;
constexpr const char* DIE1_START_SIG = "Die1StartSig";
constexpr const char* DIE1_END_SIG = "Die1EndSig";

static CcuResult ParseKernelArg(KfcServerContext& ctx, CcuKernelArgKfcServer* kernelArg)
{
    ctx.arg = kernelArg;
    return CCU_SUCCESS;
}

static CcuResult InitResource(KfcServerContext& ctx) { return CCU_SUCCESS; }

static CcuResult LoadArgs(KfcServerContext& ctx)
{
    uint32_t argId = 0;

    // 顺序需�?kfcServerArgs 中的字段顺序一致：xnAddr, ckeAddr, dieNum, missionNum, missionIndex, token
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
    // 1、Addr处理
    ccu::Variable addrBias;
    ctx.waitAddr = ctx.ckeAddr;
    addrBias = CCU_TASK_NUM_MAX * CCU_ONE_PARAM_SIZE; // 偏移8轮的总宽�?
    ctx.recordAddr = ctx.ckeAddr + addrBias;
    return CCU_SUCCESS;
}

void MissionPreSync(ccu::Variable& func, KfcServerContext& ctx)
{
    (void)func;
    (void)ctx;
}

void MissionPostSync(KfcServerContext& ctx) { (void)ctx; }

CcuResult WaitTurnStartSig(const ccu::Variable& hbmSigAddr, ccu::Variable& turnStartSig, KfcServerContext& ctx)
{
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

CcuResult SetTurnEndSig(const ccu::Variable& hbmSigAddr, const ccu::Variable& turnEndSig, KfcServerContext& ctx)
{
    const uint32_t getDieId = 1; // TODO GetDieId()
    CCU_CHK_RET(ccu::Store(hbmSigAddr, turnEndSig));
    return CCU_SUCCESS;
}

CcuResult LoadFuncParamFromMemory(ccu::Variable& paramAddr, ccu::Array<ccu::Variable>& param, KfcServerContext& ctx)
{
    // 双Die场景Die1需要读�?2个参数，其他场景都是读取�?2个参�?
    ccu::Variable doubleDie;
    doubleDie = CCU_PARAM_NUM_PER_DIE * CCU_ONE_PARAM_SIZE;
    ccu::Variable addr;
    addr = paramAddr;

    // 一次性读取本�?2个参�?
    CCU_CHK_RET(ccu::Load(addr, param, CCU_PARAM_NUM_PER_DIE));
    return CCU_SUCCESS;
}

static void DispatchKfcSubKernel(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx)
{
    ccu::Variable opType;
    opType = static_cast<uint64_t>(ctx.arg->opParam.opType);
    HCCL_INFO("[KFC_SERVER] DispatchKfcSubKernel: opType=%u", static_cast<uint32_t>(ctx.arg->opParam.opType));
    // CCU_IF(opType == static_cast<uint64_t>(HcclCMDType::HCCL_CMD_ALLGATHER))
    // {
    //     CcuKfcAllGatherMesh1DMem2MemKernel(
    //         param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], ctx.token, param[HBM_PARAM_IDX_3], param[HBM_PARAM_IDX_4],
    //         param[HBM_PARAM_IDX_5], param[HBM_PARAM_IDX_6], param[HBM_PARAM_IDX_7], param[HBM_PARAM_IDX_8],
    //         ctx.arg->channels, ctx.arg->channelCount, static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId);
    // }
    // CCU_IF(opType == static_cast<uint64_t>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER))
    // {
    //     CcuReduceScatterMesh1DMem2MemKernel(
    //         param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], ctx.token, param[HBM_PARAM_IDX_3], param[HBM_PARAM_IDX_4],
    //         param[HBM_PARAM_IDX_7], param[HBM_PARAM_IDX_8], ctx.arg->channels, ctx.arg->channelCount,
    //         static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->opParam.DataDes.dataType,
    //         ctx.arg->opParam.DataDes.outputType, ctx.arg->opParam.reduceType);
    // }
    CCU_IF(opType == static_cast<uint64_t>(HcclCMDType::HCCL_CMD_ALLREDUCE))
    {
        HCCL_INFO("[KFC_SERVER] dispatching ALLREDUCE kernel, rankSize=%u, rankId=%u, channelCount=%u\n",
                        ctx.arg->rankSize, ctx.arg->rankId, ctx.arg->channelCount);
        // FillCachedArgs order: [0]inputAddr [1]outputAddr [2]token [3]scratchAddr
        //   [4]currentRankSliceInputOffset [5]currentRankSliceOutputOffset
        //   [6]normalSliceSize [7]lastSliceSize [8]mySliceSize [9]sliceOffset
        //   [10]isInputOutputEqual [11]goSize0 [12]goSize1 [13]goSize2 [14]goSize3
        //   [15]inBuffBaseOff [16]outBuffBaseOff [17]hcclBuffBaseOff
        // HBM param[0] = control word; token (cachedArgs[2]) skipped, uses ctx.token;
        // remaining cachedArgs[N>=3] map to param[N] (offset by 1 for control word, minus 1 for skipped token = net 0)
        CcuKfcAllReduceMesh1DMem2MemKernel(
            param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], ctx.token, param[HBM_PARAM_IDX_3], param[HBM_PARAM_IDX_4],
            param[HBM_PARAM_IDX_5], param[HBM_PARAM_IDX_6], param[HBM_PARAM_IDX_7], param[HBM_PARAM_IDX_8],
            param[HBM_PARAM_IDX_9], param[HBM_PARAM_IDX_10], param[HBM_PARAM_IDX_11], param[HBM_PARAM_IDX_12],
            param[HBM_PARAM_IDX_13], param[HBM_PARAM_IDX_14], ctx.arg->channels, ctx.arg->channelCount,
            static_cast<uint32_t>(ctx.arg->rankSize), ctx.arg->rankId, ctx.arg->opParam.DataDes.dataType,
            ctx.arg->opParam.DataDes.outputType, ctx.arg->opParam.reduceType);
        HCCL_INFO("[KFC_SERVER] ALLREDUCE kernel returned\n");
    }
}

CcuResult KfcServerGenCircularQueue(KfcServerContext& ctx)
{
    HCCL_INFO("[KfcServerGenCircularQueue] KfcServerGenCircularQueue start");

    ccu::Variable repeatCond; // 存放《控制repeat循环执行的条件》的寄存�?
    repeatCond = 0;
    ccu::Variable turnStartSig; // 存放《轮次执行开始信号》的寄存�?
    turnStartSig = 0;
    ccu::Variable turnEndSig; // 存放《轮次执行完成信号》的寄存器，在循环中固定�?
    turnEndSig = 1;

    // 使用独立XN保存队列游标，避免Variable拷贝构造与context基址共享handle�?
    ccu::Variable waitAddr;
    ccu::Variable recordAddr;
    ccu::Variable paramAddr;
    waitAddr = ctx.waitAddr;
    recordAddr = ctx.recordAddr;
    paramAddr = ctx.xnAddr;

    ccu::Variable ckeSize; // 其他寄存�?
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
        HCCL_INFO("[KFC_SERVER] CircularQueue: waiting turn start");
        CCU_CHK_RET(WaitTurnStartSig(waitAddr, turnStartSig, ctx));  // 等待本轮开始信�?
        HCCL_INFO("[KFC_SERVER] CircularQueue: turn start received, loading params\n");
        CCU_CHK_RET(LoadFuncParamFromMemory(paramAddr, param, ctx)); // 读取本轮参数
        HCCL_INFO("[KFC_SERVER] CircularQueue: params loaded");
        CCU_IF(param[HBM_PARAM_IDX_0] == INVALID_U64) { repeatCond = 1; }
        CCU_IF(param[HBM_PARAM_IDX_0] != INVALID_U64)
        {
            HCCL_INFO("[KFC_SERVER] CircularQueue: dispatching sub kernel\n");
            MissionPreSync(param[HBM_PARAM_IDX_0], ctx);
            DispatchKfcSubKernel(param, ctx);
            MissionPostSync(ctx);
            HCCL_INFO("[KFC_SERVER] CircularQueue: sub kernel done, setting turn end\n");
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

    ccu::Variable repeatCond; // 存放《控制repeat循环执行的条件》的寄存�?
    repeatCond = 0;

    ccu::Variable paramAddr;
    paramAddr = ctx.xnAddr;

    ccu::Array<ccu::Variable> param(CCU_PARAM_NUM_PER_DIE); // 存放<每轮算子参数>的寄存器

    CCU_WHILE(repeatCond == 0)
    {
        MissionPreSync(signature, ctx);                              // 在context中依次加�?轮指�?
        CCU_CHK_RET(LoadFuncParamFromMemory(paramAddr, param, ctx)); // 读取本轮参数

        CCU_IF(param[HBM_PARAM_IDX_0] == INVALID_U64) { repeatCond = 1; }
        CCU_IF(param[HBM_PARAM_IDX_0] != INVALID_U64) { DispatchKfcSubKernel(param, ctx); }
        MissionPostSync(ctx);
    }
    return CCU_SUCCESS;
}

// ============================================================================
// 主入�?Kernel 函数
// ============================================================================
CcuResult CcuKfcServerKernel(CcuKernelArg arg)
{
    auto* kernelArg = static_cast<CcuKernelArgKfcServer*>(arg);
    KfcServerContext ctx;
    HCCL_INFO("[KFC_SERVER] CcuKfcServerKernel entry, opType=%u, rankSize=%u, rankId=%u\n",
                    static_cast<uint32_t>(kernelArg->opParam.opType), kernelArg->rankSize, kernelArg->rankId);
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
    HCCL_INFO("[KFC_SERVER] entering KfcServerGenCircularQueue\n");
    CCU_IF(ctx.missionIndex == 0) { CCU_CHK_RET(KfcServerGenCircularQueue(ctx)); }
    // CCU_ELSE { CCU_CHK_RET(SlaveKfcServerGenCircularQueue(ctx)); }
    HCCL_INFO("[KFC_SERVER] CcuKfcServerKernel exit\n");
    HCCL_INFO("[CcuKernelKfcServer] KfcServer end");
    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
