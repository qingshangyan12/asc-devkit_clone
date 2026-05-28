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

namespace ops_hccl {

using namespace hcomm;
using namespace std;

const string OP_SELECTOR_LABEL = "OpSelector";
const uint32_t HBM_PARAM_IDX_0 = 0;
const uint32_t HBM_PARAM_IDX_1 = 1;
const uint32_t HBM_PARAM_IDX_2 = 2;
const uint32_t HBM_PARAM_IDX_3 = 3;

const uint32_t SINGLE_DIE = 1;
const uint32_t DOUBLE_DIE = 2;
const uint32_t DIE0_ID = 0;
const uint32_t DIE1_ID = 1;
const string DIE1_START_SIG = "Die1StartSig";
const string DIE1_END_SIG = "Die1EndSig";

CcuKernelKfcServer::CcuKernelKfcServer(const CcuKernelArg &arg)
    : CcuKernelAlgBase(arg)
{
    const CcuKernelArgKfcServer *kernelArg = dynamic_cast<const CcuKernelArgKfcServer *>(&arg);
    rankId_ = kernelArg->rankId_;
    rankSize_ = kernelArg->dimSize_;
    channels_ = kernelArg->channels;
    loadFromMem_ = kernelArg->loadFromMem_;
    HCCL_INFO("[CcuKernelKfcServer] rankId_ = %d, rankSize_ = %d, loadFromMem_ = %d", rankId_, rankSize_, loadFromMem_);
}

HcclResult CcuKernelKfcServer::InitResource()
{
    HCCL_INFO("[CcuKernelKfcServer] InitResource start !");
    return HcclResult::HCCL_SUCCESS;
}

void CcuKernelKfcServer ::LoadArgs()
{
    LoadArg(xnAddr_, 0);        // Done：加载参数
    LoadArg(ckeAddr_, 1);
    LoadArg(token_, 2);
    LoadArg(dieNum_, 3);
    LoadArg(missionNum_, 4);
    LoadArg(missionIndex_, 5);
}

void CcuKernelKfcServer::SetCommAddr(ccu::Variable syncAddr)
{
    waitAddr_ = syncAddr;
    if (syncAddr > (UINT64_MAX - CCU_TASK_NUM_MAX * CCU_ONE_PARAM_SIZE)) {
        HCCL_ERROR("MC2 High Level API SetDieNum Failed: integer overflow occurs");
        return;
    }
    recordAddr_ = syncAddr + CCU_TASK_NUM_MAX * CCU_ONE_PARAM_SIZE; // 偏移8轮的总宽度
}

void CcuKernelKfcServer::SetDieNum(ccu::Variable dieNum)
{
    // 参数合法值判断
    die_num_ = dieNum;
    uint32_t getDieId = 1;                                                  //TODO GetDieId()
    bool isDieNumValid = (dieNum == SINGLE_DIE || dieNum == DOUBLE_DIE);
    bool isDieIdValid  = (getDieId == DIE0_ID || getDieId == DIE1_ID);          
    if (!(isDieNumValid && isDieIdValid)) {
        HCCL_ERROR("MC2 High Level API SetDieNum Failed: Invalid Die Config");
        return;
    }

    if (dieNum == DOUBLE_DIE) { // 双Die场景才需要导入导出
        // 导出信号
        // Die0: export完成信号给Die1，Die1: export开始信号给Die0
        exportDieSig_ = DIE1_END_SIG;        

        // 导入信号
        importDieSig_ = DIE1_START_SIG
    }
}

void CcuKernelKfcServer::SetMissionNumAndId(ccu::Variable miNum, ccu::Variable miIndex)
{
    if (missionIndex_ >= miNum) {
        HCCL_ERROR("MC2 High Level API SetMissionNumAndId Failed: Invalid Mission Config");
        return;
    }

    uint32_t getDieId = 1;
    if (miNum > 1) { // 多Mission场景才需要导入导出
        if (miIndex == 0) {
            // miIndex = 0 为Master，需要missionNum - 1个导入导出信号以及missionNum - 1个导入变量
            for (uint32_t i = 0; i < miNum - 1; ++i) {
                exportMissoinSig_ = "master_sig_0_1";
                importMissionSig_ = "slave_sig_0_1";
            }
        } else {
            // miIndex > 0 为Slave，需要1个导入导出信号以及1个导出变量
            // TODO：[这里暂且不会走进来]
        }
    }
}

HcclResult CcuKernelKfcServer::Algorithm()
{
    InitResource();
    LoadArgs();                                
    SetCommAddr(ckeAddr_);                    
    SetDieNum(dieNum_);                         
    SetMissionNumAndId(missionNum_, missionIndex_); 
    if (missionIndex_ == 0) {
        Mc2KfcServer server;
        server.GenCircularQueue(); // 这里需要做0卡判断，走不同逻辑
    } else {
        Mc2SlaveKfcServer server;
        server.GenCircularQueue();
    }
    return HcclResult::HCCL_SUCCESS;
}

void Mc2KfcServer::GenCircularQueue()
{
    // 控制<repeat循环执行条件>的寄存器
    ccu::Variable repeatCond;
    repeatCond = 0;

    // <轮次执行开始信号>的寄存器
    ccu::Variable turnStartSig;
    turnStartSig = 0;
    ccu::Variable turnEndSig;
    turnEndSig = 1;

    ccu::Variable waitStartAddr;
    waitStartAddr = waitAddr_;
    ccu::Variable recordStartAddr;
    recordStartAddr = recordAddr_;
    ccu::Variable paramStartAddr;
    paramStartAddr = xnAddr_;
    ccu::Variable waitAddr;
    waitAddr = waitAddr_;
    ccu::Variable recordAddr;
    recordAddr = recordAddr_;
    ccu::Variable paramAddr;
    paramAddr = xnAddr_;

    ccu::Variable ckeSize;
    ckeSize = CCU_ONE_PARAM_SIZE;
    ccu::Variable paramSize;
    paramSize = CCU_PARAM_NUM_MAX * CCU_ONE_PARAM_SIZE;

    ccu::Variable queueIdx;
    queueIdx = 0;
    ccu::Variable queueEnd;
    queueEnd = CCU_TASK_NUM_MAX;
    ccu::Variable one;
    one = 1;

    // 存放<每轮算子参数>的寄存器
    ccu::Array<ccu::Variable> param(CCU_PARAM_NUM_PER_DIE);

    CCU_WHILE(repeatCond == 0)
    {
        if (waitAddr_ > (UINT64_MAX - (CCU_TASK_NUM_MAX - 1) * CCU_ONE_PARAM_SIZE)
            || recordAddr_ > (UINT64_MAX - (CCU_TASK_NUM_MAX - 1) * CCU_ONE_PARAM_SIZE)
            || xnAddr_ > (UINT64_MAX - (CCU_TASK_NUM_MAX - 1) * CCU_PARAM_NUM_MAX * CCU_ONE_PARAM_SIZE)) {
            HCCL_ERROR("[CcuKernelKfcServer] GenCircularQueue Failed: integer overflow occurs");
            return;
        }

        // 等待本轮开始信号
        WaitTurnStartSig(waitAddr, turnStartSig);

        // 读取本轮参数
        LoadFuncParamFromMemory(paramAddr, param);

        MissionPreSync(param[HBM_PARAM_IDX_0]);

        CCU_IF(param[HBM_PARAM_IDX_0] == INVALID_U64)
        {
            CCU_BREAK;
        }

        hcomm::ccu::Variable opType;
        opType = param[HBM_PARAM_IDX_0];
        CCU_IF(opType == CcuKernelAlltoAllMesh1D) {
            ccu::callFunc<CcuKernelAlltoAllMesh1D>(param, token_);   //给通信算子自己解析
        }

        MissionPostSync();

        // Set本轮完成信号
        SetTurnEndSig(recordAddr, turnEndSig);
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

void Mc2KfcServer::WaitTurnStartSig(const ccu::Variable &hbmSigAddr, ccu::Variable &turnStartSig)
{
    const uint32_t getDieId = 1;
    if (dieNum_ == SINGLE_DIE) {
        CCU_WHILE(turnStartSig != 1)
        {
            Load(hbmSigAddr, turnStartSig);
        }
        turnStartSig = 0;
        Store(turnStartSig, hbmSigAddr);
    } else {
        if (getDieId == DIE0_ID) {
            CCU_WHILE(turnStartSig != 1)
            {
                Load(hbmSigAddr, turnStartSig);
            }
            turnStartSig = 0;
            Store(turnStartSig, hbmSigAddr);
            EventRecord(importDieSig_, 1);
        } else if (getDieId == DIE1_ID) {
            EventWait(exportDieSig_, 1);
        }
    }
}

void Mc2KfcServer::SetTurnEndSig(const ccu::Variable &hbmSigAddr, const ccu::Variable &turnEndSig)
{
    const uint32_t getDieId = 1;
    if (dieNum_ == SINGLE_DIE) {
        Store(turnEndSig, hbmSigAddr);
    } else {
        if (getDieId == DIE0_ID) {
            EventWait(exportDieSig_, 1);
            Store(turnEndSig, hbmSigAddr);
        } else if (getDieId == DIE1_ID) {
            EventRecord(importDieSig_, 1);
        }
    }
}

void Mc2KfcServer::LoadFuncParamFromMemory(ccu::Variable &paramAddr, array<ccu::Variable, CCU_PARAM_NUM_PER_DIE> &param)
{
    ccu::Variable doubleDie;
    doubleDie = CCU_PARAM_NUM_PER_DIE * CCU_ONE_PARAM_SIZE;
    ccu::Variable addr;
    addr = paramAddr;
    const uint32_t getDieId = 1;
    if (dieNum_ == DOUBLE_DIE && getDieId == DIE1_ID) {
        addr += doubleDie;
    }
    Load(addr, param[0], CCU_PARAM_NUM_PER_DIE);
}

void Mc2SlaveKfcServer::GenCircularQueue()
{
    // 算子签名
    ccu::Variable signature;

    // 存放《控制repeat循环执行的条件》的寄存器
    ccu::Variable repeatCond;
    repeatCond                  = 0;

    ccu::Variable paramAddr;
    paramAddr = xnAddr_;

    // 存放<每轮算子参数>的寄存器
    ccu::Array<ccu::Variable> param(CCU_PARAM_NUM_PER_DIE);

    CCU_WHILE(repeatCond == 0)
    {
        // 在context中依次加入8轮指令
        MissionPreSync(signature);

        // 第一个参数为opCode, 如果参数中opCode非法则跳出循环队列
        CCU_IF(signature == INVALID_U64)
        {
            CCU_BREAK;
        }

        // 读取本轮参数
        LoadFuncParamFromMemory(paramAddr, param);
        hcomm::ccu::Variable opType;
        opType = param[HBM_PARAM_IDX_0];
        CCU_IF(opType == CcuKernelAlltoAllMesh1D) {
            ccu::callFunc<CcuKernelAlltoAllMesh1D>(param, token_);   //给通信算子自己解析
        }

        MissionPostSync();
    }
}

void CcuKernelKfcServer::MissionPreSync(ccu::Variable &func)
{
    if (missionNum_ == 1) {
        return;
    }
    if (missionIndex_ == 0) {
        // TODO：这里怎么改 [这里暂时不会走进来]
        for (uint32_t i = 0; i < missionNum_ - 1; ++i) {
            LocalCtxPostVar(func, importMissionVar_, importMissionSig_);    
        }
    } else {
        // TODO：这里暂时不会走进来
    }
}

void CcuKernelKfcServer::MissionPostSync()
{
    if (missionNum_ == 1) {
        return;
    }
    if (missionIndex_ == 0) {
        for (uint32_t i = 0; i < missionNum_ - 1; ++i) {
            EventWait(exportMissoinSig_);
        }
    } else {
        EventRecord(importMissionSig_);
    }
}

std::vector<uint64_t> CcuKernelKfcServer::GeneArgs(const CcuTaskArg &arg)
{
    const CcuTaskArgKfcServer *taskArg = dynamic_cast<const CcuTaskArgKfcServer *>(&arg);
    uint64_t xnAddr = taskArg->xnAddr_;
    uint64_t ckeAddr = taskArg->ckeAddr_;
    uint64_t token = taskArg->token_;
    uint64_t dieNum = taskArg->dieNum_;
    uint64_t missionNum = taskArg->missionNum_;
    uint64_t missionIndex = taskArg->missionIndex_;
    return {xnAddr, ckeAddr, token, dieNum, missionNum, missionIndex};
}

}