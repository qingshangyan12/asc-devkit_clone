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
    HCCL_INFO("[CcuKernelKfcServer] InitResource!");
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuKernelKfcServer::Algorithm()
{
    //DONE:删除CcuKernelKfcServer::GenOpSelector()
    if (id == 0) {
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
    CcuRep::Variable token = CreateVariable();
    Load(token);

    CcuRep::Variable opAddr = CreateVariable();

    CcuRep::Variable repeatCond = CreateVariable();
    repeatCond = 0;

    CcuRep::Variable turnStartSig = CreateVariable();
    turnStartSig = 0;
    CcuRep::Variable turnEndSig = CreateVariable();
    turnEndSig = 1;

    CcuRep::Variable waitStartAddr = CreateVariable();
    waitStartAddr = waitAddr_;
    CcuRep::Variable recordStartAddr = CreateVariable();
    recordStartAddr = recordAddr_;
    CcuRep::Variable paramStartAddr = CreateVariable();
    paramStartAddr = paramAddr_;
    CcuRep::Variable waitAddr = CreateVariable();
    waitAddr = waitAddr_;
    CcuRep::Variable recordAddr = CreateVariable();
    recordAddr = recordAddr_;
    CcuRep::Variable paramAddr = CreateVariable();
    paramAddr = paramAddr_;

    CcuRep::Variable ckeSize = CreateVariable();
    ckeSize = CCU_ONE_PARAM_SIZE;
    CcuRep::Variable paramSize = CreateVariable();
    paramSize = CCU_PARAM_NUM_MAX * CCU_ONE_PARAM_SIZE;

    CcuRep::Variable queueIdx = CreateVariable();
    queueIdx = 0;
    CcuRep::Variable queueEnd = CreateVariable();
    queueEnd = CCU_TASK_NUM_MAX;
    CcuRep::Variable one = CreateVariable();
    one = 1;

    array<CcuRep::Variable, CCU_PARAM_NUM_PER_DIE> param;
    for (uint32_t i = 0; i < CCU_PARAM_NUM_PER_DIE; ++i) {
        param[i] = ccu::Array<CcuRep::Variable> vars(10);       // TODO：需要确定这里的个数
    }

    CCU_WHILE(repeatCond == 0)
    {
        if (waitAddr_ > (UINT64_MAX - (CCU_TASK_NUM_MAX - 1) * CCU_ONE_PARAM_SIZE)
            || recordAddr_ > (UINT64_MAX - (CCU_TASK_NUM_MAX - 1) * CCU_ONE_PARAM_SIZE)
            || paramAddr_ > (UINT64_MAX - (CCU_TASK_NUM_MAX - 1) * CCU_PARAM_NUM_MAX * CCU_ONE_PARAM_SIZE)) {
            HCCL_ERROR("[CcuKernelKfcServer] GenCircularQueue Failed: integer overflow occurs");
            return;
        }

        WaitTurnStartSig(waitAddr, turnStartSig);

        LoadFuncParamFromMemory(paramAddr, param);

        MissionPreSync(param[HBM_PARAM_IDX_0]);

        CCU_IF(param[HBM_PARAM_IDX_0] == INVALID_U64)
        {
            CCU_BREAK;
        }

        const uint32_t getDieId = DIE0_ID;      // TODO：GetDieId()函数的替换待HCCL方提供
        std::string funcName = OP_SELECTOR_LABEL + "_" + std::to_string(getDieId) + "_" + std::to_string(missionIndex_);
        auto selectFunc = Func(funcName);
        selectFunc.SetInArg(param[HBM_PARAM_IDX_0]);
        selectFunc.SetOutArg(opAddr);
        selectFunc.AppendToContext();

        CCU_IF(opAddr == INVALID_U64)
        {
            CCU_BREAK;
        }

        hcomm::CcuRep::Variable opType;
        opType = param[HBM_PARAM_IDX_0];    
        CCU_IF(opType == CcuKernelAlltoAllMesh1D) {
            ccu::callFunc<CcuKernelAlltoAllMesh1D>(param[HBM_PARAM_IDX_1], param[HBM_PARAM_IDX_2],token, 
                param[HBM_PARAM_IDX_3], param[HBM_PARAM_IDX_4], param[HBM_PARAM_IDX_5],
                param[HBM_PARAM_IDX_6], param[HBM_PARAM_IDX_7], param[HBM_PARAM_IDX_8],
                param[HBM_PARAM_IDX_9], param[HBM_PARAM_IDX_10]);
        }

        MissionPostSync();

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

void Mc2KfcServer::WaitTurnStartSig(const CcuRep::Variable &hbmSigAddr, CcuRep::Variable &turnStartSig)
{
    const uint32_t getDieId = DIE0_ID;          // TODO：GetDieId()函数的替换待HCCL方提供
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
            EventRecord(importDieSig, 1);
        } else if (getDieId == DIE1_ID) {
            EventWait(exportDieSig, 1);
        }
    }
}

void Mc2KfcServer::SetTurnEndSig(const CcuRep::Variable &hbmSigAddr, const CcuRep::Variable &turnEndSig)
{
    const uint32_t getDieId = DIE0_ID;          // TODO：GetDieId()函数的替换待HCCL方提供
    if (dieNum_ == SINGLE_DIE) {
        Store(turnEndSig, hbmSigAddr);
    } else {
        if (getDieId == DIE0_ID) {
            EventWait(exportDieSig, 1);
            Store(turnEndSig, hbmSigAddr);
        } else if (getDieId == DIE1_ID) {
            EventRecord(importDieSig, 1);
        }
    }
}

void Mc2KfcServer::LoadFuncParamFromMemory(CcuRep::Variable &paramAddr, array<CcuRep::Variable, CCU_PARAM_NUM_PER_DIE> &param)
{
    CcuRep::Variable doubleDie = CreateVariable();
    doubleDie = CCU_PARAM_NUM_PER_DIE * CCU_ONE_PARAM_SIZE;
    CcuRep::Variable addr = CreateVariable();
    addr = paramAddr;
    const uint32_t getDieId = DIE0_ID;          // TODO：GetDieId()函数的替换待HCCL方提供
    if (dieNum_ == DOUBLE_DIE && getDieId == DIE1_ID) {
        addr += doubleDie;
    }
    Load(addr, param[0], CCU_PARAM_NUM_PER_DIE);
}

void Mc2SlaveKfcServer::GenCircularQueue()
{
    // 算子签名
    CcuRep::Variable signature = CreateVariable();
    // 存放《选择函数返回的FuncCall地址》的寄存器，选择函数的出参，循环队列内部使用
    CcuRep::Variable opAddr = CreateVariable();

    // 存放《控制repeat循环执行的条件》的寄存器
    CcuRep::Variable repeatCond = CreateVariable();
    repeatCond                  = 0;

    CCU_WHILE(repeatCond == 0)
    {
        // 在context中依次加入8轮指令
        MissionPreSync(signature);

        // 第一个参数为opCode, 如果参数中opCode非法则跳出循环队列
        CCU_IF(signature == INVALID_U64)
        {
            CCU_BREAK;
        }

        // 调用OpSelector
        std::string funcName
            = OP_SELECTOR_LABEL + "_" + std::to_string(GetDieId()) + "_" + std::to_string(missionIndex);
        auto selectFunc = Func(funcName);
        selectFunc.SetInArg(signature);
        selectFunc.SetOutArg(opAddr);
        selectFunc.AppendToContext();

        // 检查OpSelector是否命中算子，如果没命中则跳出循环队列
        CCU_IF(opAddr == INVALID_U64)
        {
            CCU_BREAK;
        }

        // 调用算子Func
        auto opFunc = Func(opAddr);

        opFunc.AppendToContext();

        MissionPostSync();
    }
}

void CcuKernelKfcServer::MissionPreSync(CcuRep::Variable &func)
{
    if (missionNum_ == 1) {
        return;
    }
    if (missionIndex_ == 0) {
        for (uint32_t i = 0; i < missionNum_ - 1; ++i) {
            LocalCtxPostVar(func, importMissionVar[i], importMissionSig[i]);    // TODO：这里怎么改
        }
    } else {
        EventWait(exportMissoinSig[0]);
        func = exportMissionVar[0];
    }
}

void CcuKernelKfcServer::MissionPostSync()
{
    if (missionNum_ == 1) {
        return;
    }
    if (missionIndex_ == 0) {
        for (uint32_t i = 0; i < missionNum_ - 1; ++i) {
            EventWait(exportMissoinSig[i]);
        }
    } else {
        EventRecord(importMissionSig[0]);
    }
}

std::vector<uint64_t> CcuKernelKfcServer::GeneArgs(const CcuTaskArg &arg)
{
    const CcuTaskArgKfcServer *taskArg = dynamic_cast<const CcuTaskArgKfcServer *>(&arg);
    uint64_t tokenInfo = taskArg->token_;
    return {tokenInfo};
}

}