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

const uint32_t SINGLE_DIE = 1;
const uint32_t DOUBLE_DIE = 2;
const uint32_t DIE0_ID = 0;
const uint32_t DIE1_ID = 1;
constexpr const char* DIE1_START_SIG = "Die1StartSig";
constexpr const char* DIE1_END_SIG = "Die1EndSig";

ccu::Func AlltoAllMesh1DFunc([](ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                                ccu::Variable srcStride, ccu::Variable srcOffset, ccu::Variable dstOffset,
                                ccu::Variable sliceSize, ccu::Variable goSize1, ccu::Variable goSize2,
                                ccu::Variable goSize3, ccu::Variable goSize4, ccu::Variable argHandle) {
    HCCL_INFO("[CcuKernelKfcServer] start ccuFunc AlltoAllMesh1DFunc!");
    CcuKernelArg arg = reinterpret_cast<CcuKernelArg>(argHandle.handle);
    CcuAlltoAllMesh1DKernel(inputAddr, outputAddr, tokenInfo, srcStride, srcOffset, dstOffset,
                           sliceSize, goSize1, goSize2, goSize3, goSize4, arg);
});

// new---20260603
static CcuResult ParseKernelArg(KfcServerContext &ctx)
{
    return CCU_SUCCESS;
}

static CcuResult InitResource(KfcServerContext &ctx)
{   
    return CCU_SUCCESS;
}

static CcuResult LoadArgs(KfcServerContext &ctx)
{
    uint32_t argId = 0;

    CCU_CHK_RET(ccu::LoadArg(ctx.xnAddr, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.ckeAddr, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.token, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.dieNum, argId++));

    CCU_CHK_RET(ccu::LoadArg(ctx.missionNum, argId++));
    CCU_CHK_RET(ccu::LoadArg(ctx.missionIndex, argId++));

    return CCU_SUCCESS;
}

static CcuResult CompArgs(KfcServerContext &ctx)
{
    // 1、Addr处理
    ccu::Variable addrBias;
    ctx.waitAddr = ctx.ckeAddr;
    addrBias = CCU_TASK_NUM_MAX * CCU_ONE_PARAM_SIZE;       // 偏移8轮的总宽度
    ctx.recordAddr = ctx.ckeAddr + addrBias;

    // 2、DieNum的-----missionId=0
    // 双Die场景才需要导入导出
    CCU_IF(ctx.dieNum == DOUBLE_DIE) {
        // TODO GetDieId()
        uint32_t getDieId = 1;                  
        // 导出信号，Die0: export完成信号给Die1，Die1: export开始信号给Die0
        ctx.exportDieSig = (getDieId == DIE1_ID) ? DIE1_START_SIG : DIE1_END_SIG;

        // 导入信号
        ctx.importDieSig = (getDieId == DIE1_ID) ? DIE1_END_SIG : DIE1_START_SIG;
    }

    /*
    // 3、mission处理----多Mission场景才需要，暂且不需要 
    CCU_IF (ctx.missionNum != 1) {
        // TODO：[这里暂且不会走进来]
        CCU_IF (ctx.missionIndex == 0) {
            // miIndex = 0 为Master，需要missionNum - 1个导入导出信号以及missionNum - 1个导入变量
            ccu::Variable index = 1;
            uint64_t index = 1;
            CCU_WHILE(ctx.missionIndex != index) {
                ctx.exportMissoinSig.push_back();
                ctx.importMissionSig.push_back();
                ctx.importMissionVar.push_back();
            }
        } CCU_ELSE {
            // miIndex > 0 为Slave，需要1个导入导出信号以及1个导出变量
            ctx.importMissionSig.push_back();
            ctx.exportMissoinSig.push_back();
            ctx.exportMissionVar.push_back();
        }
    }
    */
    return CCU_SUCCESS; 
}

void MissionPreSync(ccu::Variable &func, KfcServerContext &ctx)
{
    CCU_IF (ctx.missionNum == 1 ) {
        return;
    }

    // TODO:多mission场景
    CCU_IF (ctx.missionIndex == 0) {

    } CCU_ELSE {

    }
}

void MissionPostSync(KfcServerContext &ctx)
{
    CCU_IF (ctx.missionNum == 1 ) {
        return;
    }

    // TODO:多mission场景
    CCU_IF (ctx.missionIndex == 0) {

    } CCU_ELSE {

    }
}

void WaitTurnStartSig(const ccu::Variable &hbmSigAddr, ccu::Variable &turnStartSig, KfcServerContext &ctx)
{
    // TODO GetDieId()
    const uint32_t getDieId = 1;
    CCU_IF (ctx.dieNum == SINGLE_DIE) {
        // 单Die场景：等待HBM中的信号
        CCU_WHILE(turnStartSig != 1)
        {
            ccu::Load(hbmSigAddr, turnStartSig);     // 循环读HBM对应地址的信号到Xn，直到Xn中的信号值为1
        }
        turnStartSig = 0;                       // reset Xn
        ccu::Store(turnStartSig, hbmSigAddr);        // reset HBM
    } CCU_ELSE {
        // 双Die场景
        if (getDieId == DIE0_ID) {
            // 双Die场景Die0：等待HBM中的信号，收到HBM信号之后再给Die1发信号，通知Die1开始
            CCU_WHILE(turnStartSig != 1)
            {
                ccu::Load(hbmSigAddr, turnStartSig);     // 循环读HBM对应地址的信号到Xn，直到Xn中的信号值为1
            }
            turnStartSig = 0;                       // reset Xn
            ccu::Store(turnStartSig, hbmSigAddr);        // reset HBM
            ccu::EventRecord(ctx.importDieSig, 1);       // 开始给Die1发开始信号
        } else if (getDieId == DIE1_ID) {
            // 双Die场景Die1：等待Die0的信号 
            ccu::EventWait(ctx.exportDieSig, 1);         // 会自动reset CKE
        }
    }
}

void SetTurnEndSig(const ccu::Variable &hbmSigAddr, const ccu::Variable &turnEndSig, KfcServerContext &ctx)
{
    const uint32_t getDieId = 1;    // TODO GetDieId()
    CCU_IF (ctx.dieNum == SINGLE_DIE) {
        // 单Die场景：Set本轮完成信号到HBM
        Store(turnEndSig, hbmSigAddr);
    } CCU_ELSE {
        // 双Die场景
        if (getDieId == DIE0_ID) {
            // 双Die场景Die0，等待Die1执行完成信号，然后Set本轮完成信号到HBM
            ccu::EventWait(ctx.exportDieSig, 1);            // 会自动reset CKE
            ccu::Store(turnEndSig, hbmSigAddr);
        } else if (getDieId == DIE1_ID) {
            // 双Die场景Die1：通知Die0执行完成
            ccu::EventRecord(ctx.importDieSig, 1);
        }
    }
}

void LoadFuncParamFromMemory(ccu::Variable &paramAddr, ccu::Array<ccu::Variable> &param, KfcServerContext &ctx)
{
    // 双Die场景Die1需要读后32个参数，其他场景都是读取前32个参数
    ccu::Variable doubleDie;
    doubleDie = CCU_PARAM_NUM_PER_DIE * CCU_ONE_PARAM_SIZE;
    ccu::Variable addr = paramAddr;

    // TODO GetDieId()
    const uint32_t getDieId = 1;
    CCU_IF (ctx.dieNum == DOUBLE_DIE) {
        if (getDieId == DIE1_ID) {
            addr = addr + doubleDie;
        }  
    }

    // 一次性读取本轮32个参数
    ccu::Load(addr, param, CCU_PARAM_NUM_PER_DIE);
}

void KfcServerGenCircularQueue(CcuKernelArg &arg, KfcServerContext &ctx)
{
    // 存放《控制repeat循环执行的条件》的寄存器
    ccu::Variable repeatCond;
    repeatCond = 0;
    // 存放《轮次执行开始信号》的寄存器
    ccu::Variable turnStartSig;
    turnStartSig = 0;
    // 存放《轮次执行完成信号》的寄存器，在循环中固定为1
    ccu::Variable turnEndSig;
    turnEndSig = 1;

    // 载入 结构体 中的寄存器
    ccu::Variable waitStartAddr = ctx.waitAddr;
    ccu::Variable recordStartAddr = ctx.recordAddr;
    ccu::Variable paramStartAddr = ctx.xnAddr;
    ccu::Variable waitAddr = ctx.waitAddr;
    ccu::Variable recordAddr = ctx.recordAddr;
    ccu::Variable paramAddr = ctx.xnAddr;

    // 其他寄存器
    ccu::Variable ckeSize;
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
        // 等待本轮开始信号
        WaitTurnStartSig(waitAddr, turnStartSig, ctx);

        // 读取本轮参数
        LoadFuncParamFromMemory(paramAddr, param, ctx);

        MissionPreSync(param[HBM_PARAM_IDX_0], ctx);

        ccu::Variable opType;
        opType = param[HBM_PARAM_IDX_0];
        CCU_IF(opType == AlgorithmType::CcuAlltoAllMesh1D) {
            CcuAlltoAllMesh1DKernel(param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], param[HBM_PARAM_IDX_3],
                                    param[HBM_PARAM_IDX_4], param[HBM_PARAM_IDX_5], param[HBM_PARAM_IDX_6],
                                    param[HBM_PARAM_IDX_7], param[HBM_PARAM_IDX_8], param[HBM_PARAM_IDX_9],
                                    param[HBM_PARAM_IDX_10], param[HBM_PARAM_IDX_11], arg);
            /*
            ccu::CallFunc<AlltoAllMesh1DFunc>(param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], param[HBM_PARAM_IDX_3],
                                              param[HBM_PARAM_IDX_4], param[HBM_PARAM_IDX_5], param[HBM_PARAM_IDX_6],
                                              param[HBM_PARAM_IDX_7], param[HBM_PARAM_IDX_8], param[HBM_PARAM_IDX_9],
                                              param[HBM_PARAM_IDX_10], param[HBM_PARAM_IDX_11], argHandle);
            */
        } 

        MissionPostSync(ctx);

        // Set本轮完成信号
        SetTurnEndSig(recordAddr, turnEndSig, ctx);
        waitAddr += ckeSize;
        recordAddr += ckeSize;
        paramAddr += paramSize;
        queueIdx += one;
        CCU_IF (queueIdx == static_cast<u64>(CCU_TASK_NUM_MAX)) {
            waitAddr = waitStartAddr;
            recordAddr = recordStartAddr;
            paramAddr = paramStartAddr;
            queueIdx = 0;
        }
    }    

}

void SlaveKfcServerGenCircularQueue(CcuKernelArg &arg, KfcServerContext &ctx)
{
    // 算子签名
    ccu::Variable signature;

    // 存放《控制repeat循环执行的条件》的寄存器
    ccu::Variable repeatCond;
    repeatCond = 0;

    ccu::Variable paramAddr = ctx.xnAddr;

    // 存放<每轮算子参数>的寄存器
    ccu::Array<ccu::Variable> param(CCU_PARAM_NUM_PER_DIE);

    CCU_WHILE(repeatCond == 0)
    {
        // 在context中依次加入8轮指令
        MissionPreSync(signature, ctx);

        // 读取本轮参数
        LoadFuncParamFromMemory(paramAddr, param, ctx);

        ccu::Variable argHandle;
        CcuAlltoAllMesh1DKernel(param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], param[HBM_PARAM_IDX_3],
                                param[HBM_PARAM_IDX_4], param[HBM_PARAM_IDX_5], param[HBM_PARAM_IDX_6],
                                param[HBM_PARAM_IDX_7], param[HBM_PARAM_IDX_8], param[HBM_PARAM_IDX_9],
                                param[HBM_PARAM_IDX_10], param[HBM_PARAM_IDX_11], arg);
        /*
        argHandle = reinterpret_cast<uint64_t>(arg);
        ccu::CallFunc<AlltoAllMesh1DFunc>(param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2], param[HBM_PARAM_IDX_3],
                                          param[HBM_PARAM_IDX_4], param[HBM_PARAM_IDX_5], param[HBM_PARAM_IDX_6],
                                          param[HBM_PARAM_IDX_7], param[HBM_PARAM_IDX_8], param[HBM_PARAM_IDX_9],
                                          param[HBM_PARAM_IDX_10], param[HBM_PARAM_IDX_11], argHandle);
        */
        MissionPostSync(ctx);
    }
}

// ============================================================================
// 主入口 Kernel 函数
// ============================================================================
CcuResult CcuKfcServerKernel(CcuKernelArg arg)
{
    KfcServerContext ctx;

    HCCL_INFO("[CcuKernelKfcServer] channelCount: [%lu]", arg->channelCount);
    HCCL_INFO("[CcuKernelKfcServer] KfcServer run");
    CCU_CHK_RET(ParseKernelArg(ctx));
    HCCL_INFO("[CcuKernelKfcServer] ParseKernelArg finish");
    CCU_CHK_RET(InitResource(ctx));
    HCCL_INFO("[CcuKernelKfcServer] InitResource finish");
    CCU_CHK_RET(LoadArgs(ctx));
    HCCL_INFO("[CcuKernelKfcServer] LoadArgs finish");
    CCU_CHK_RET(CompArgs(ctx));
    HCCL_INFO("[CcuKernelKfcServer] CompArgs finish");

    HCCL_INFO("[GenCircularQueue] KfcServer start");
    CCU_IF(ctx.missionIndex == 0) {
        KfcServerGenCircularQueue(arg, ctx);
    } CCU_ELSE {
        SlaveKfcServerGenCircularQueue(arg, ctx);
    }

    HCCL_INFO("[CcuKernelKfcServer] KfcServer end");
    return CCU_SUCCESS;
}    
// new---20260603


}// mc2_ops_hccl
