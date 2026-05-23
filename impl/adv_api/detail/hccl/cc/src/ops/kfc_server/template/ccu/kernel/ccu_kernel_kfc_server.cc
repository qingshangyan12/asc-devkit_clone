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

void CcuKernelKfcServer::SetCommAddr(uint64_t syncAddr, uint64_t paramAddr)
{
    waitAddr_ = syncAddr;
    if (syncAddr > (UINT64_MAX - CCU_TASK_NUM_MAX * CCU_ONE_PARAM_SIZE)) {
        // THROW<InvalidParamsException>("MC2 High Level API SetDieNum Failed: integer overflow occurs");  //TODO：添加打印
    }
    recordAddr_ = syncAddr + CCU_TASK_NUM_MAX * CCU_ONE_PARAM_SIZE; // 偏移8轮的总宽度
    paramAddr_  = paramAddr;
}

void CcuKernelKfcServer::SetDieNum(uint32_t dieNum)
{
    dieNum_ = dieNum;
    // 参数合法值判断
    bool isDieNumValid = (dieNum_ == SINGLE_DIE || dieNum_ == DOUBLE_DIE);
    bool isDieIdValid  = (GetDieId() == DIE0_ID || GetDieId() == DIE1_ID);
    if (!(isDieNumValid && isDieIdValid)) {
        // THROW<InvalidParamsException>("MC2 High Level API SetDieNum Failed: Invalid Die Config"); //TODO:添加打印
    }

    if (dieNum_ == DOUBLE_DIE) { // 双Die场景才需要导入导出
        // 导出信号
        exportDieSig = CreateMaskSignal();
        // Die0: export完成信号给Die1，Die1: export开始信号给Die0
        const string &exportSigLabel = (GetDieId() == DIE1_ID) ? DIE1_START_SIG : DIE1_END_SIG;
        ExportMaskSignal(exportDieSig, exportSigLabel);

        // 导入信号
        const string &importSigLabel = (GetDieId() == DIE1_ID) ? DIE1_END_SIG : DIE1_START_SIG;
        importDieSig                 = ImportMaskSignal(importSigLabel);
    }
}

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
    // 载入Token
    CcuRep::Variable token = CreateVariable();
    Load(token);                                    // TODO:原来的这个Load()应该替换成什么呢？  LoadVariable()--->Load()                                  

    // 控制<repeat循环执行条件>的寄存器
    CcuRep::Variable repeatCond = CreateVariable();
    repeatCond = 0;

    // <轮次执行开始信号>的寄存器
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

    // 存放<每轮算子参数>的寄存器
    ccu::Array<CcuRep::Variable> param(CCU_PARAM_NUM_PER_DIE);

    CCU_WHILE(repeatCond == 0)
    {
        if (waitAddr_ > (UINT64_MAX - (CCU_TASK_NUM_MAX - 1) * CCU_ONE_PARAM_SIZE)
            || recordAddr_ > (UINT64_MAX - (CCU_TASK_NUM_MAX - 1) * CCU_ONE_PARAM_SIZE)
            || paramAddr_ > (UINT64_MAX - (CCU_TASK_NUM_MAX - 1) * CCU_PARAM_NUM_MAX * CCU_ONE_PARAM_SIZE)) {
            HCCL_ERROR("[CcuKernelKfcServer] GenCircularQueue Failed: integer overflow occurs");
            return;
        }

        // 等待本轮开始信号
        WaitTurnStartSig(waitAddr, turnStartSig);

        // 读取本轮参数
        LoadFuncParamFromMemory(paramAddr, param);  //TODO:paramAddr这个参数是通信算子(如ALLTOALL的)？

        MissionPreSync(param[HBM_PARAM_IDX_0]);

        CCU_IF(param[HBM_PARAM_IDX_0] == INVALID_U64)
        {
            CCU_BREAK;
        }

        hcomm::CcuRep::Variable opType;
        opType = param[HBM_PARAM_IDX_0];
        CCU_IF(opType == CcuKernelAlltoAllMesh1D) {
            ccu::callFunc<CcuKernelAlltoAllMesh1D>(param, token);   //给通信算子自己解析
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

void Mc2KfcServer::WaitTurnStartSig(const CcuRep::Variable &hbmSigAddr, CcuRep::Variable &turnStartSig)
{
    const uint32_t getDieId = DIE0_ID;          // WAIT
    if (dieNum_ == SINGLE_DIE) {
        CCU_WHILE(turnStartSig != 1)
        {
            // Done:原来LoadVariable()里面是两个CcuRep::Variable入参
            // 现在Load有几个版本：Load(Variable addrVar, Variable v)，Load(uint64_t addr, Variable v)
            Load(hbmSigAddr, turnStartSig);
        }
        turnStartSig = 0;
        Store(turnStartSig, hbmSigAddr);        // DONE：同上
    } else {
        if (getDieId == DIE0_ID) {
            CCU_WHILE(turnStartSig != 1)
            {
                Load(hbmSigAddr, turnStartSig);
            }
            turnStartSig = 0;
            Store(turnStartSig, hbmSigAddr);
            // TODO:原来的LocalCtxPost(const CcuRep::MaskSignal &sig, uint32_t mask)
            // EventRecord(const char *notifyTag, uint16_t mask = 1);
            // 即原来的CcuRep::MaskSignal能不能等效替换为const char*
            EventRecord(importDieSig_, 1);
        } else if (getDieId == DIE1_ID) {
            EventWait(exportDieSig_, 1);        // TODO:同上
        }
    }
}

void Mc2KfcServer::SetTurnEndSig(const CcuRep::Variable &hbmSigAddr, const CcuRep::Variable &turnEndSig)
{
    const uint32_t getDieId = DIE0_ID;          // WAIT
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

void Mc2KfcServer::LoadFuncParamFromMemory(CcuRep::Variable &paramAddr, array<CcuRep::Variable, CCU_PARAM_NUM_PER_DIE> &param)
{
    CcuRep::Variable doubleDie = CreateVariable();
    doubleDie = CCU_PARAM_NUM_PER_DIE * CCU_ONE_PARAM_SIZE;
    CcuRep::Variable addr = CreateVariable();
    addr = paramAddr;
    const uint32_t getDieId = DIE0_ID;          // WAIT
    if (dieNum_ == DOUBLE_DIE && getDieId == DIE1_ID) {
        addr += doubleDie;
    }
    Load(addr, param[0], CCU_PARAM_NUM_PER_DIE);
}

void Mc2SlaveKfcServer::GenCircularQueue()
{
    // 算子签名
    CcuRep::Variable signature = CreateVariable();

    // 存放《控制repeat循环执行的条件》的寄存器
    CcuRep::Variable repeatCond = CreateVariable();
    repeatCond                  = 0;

    CcuRep::Variable paramAddr = CreateVariable();
    paramAddr = paramAddr_;

    // 存放<每轮算子参数>的寄存器
    ccu::Array<CcuRep::Variable> param(CCU_PARAM_NUM_PER_DIE);

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
        LoadFuncParamFromMemory(paramAddr, param);  //TODO:paramAddr这个参数是通信算子(如ALLTOALL的)？
        hcomm::CcuRep::Variable opType;
        opType = param[HBM_PARAM_IDX_0];
        CCU_IF(opType == CcuKernelAlltoAllMesh1D) {
            ccu::callFunc<CcuKernelAlltoAllMesh1D>(param, token);   //给通信算子自己解析
        }

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
            LocalCtxPostVar(func, importMissionVar_[i], importMissionSig_[i]);    // TODO：这里怎么改
        }
    } else {
        // DONE,单个入参版本EventWait(const char *notifyTag, uint16_t mask = 1)
        EventWait(exportMissoinSig_[0]);
        func = exportMissionVar_[0];
    }
}

void CcuKernelKfcServer::MissionPostSync()
{
    if (missionNum_ == 1) {
        return;
    }
    if (missionIndex_ == 0) {
        for (uint32_t i = 0; i < missionNum_ - 1; ++i) {
            // DONE,单个入参版本EventWait(const char *notifyTag, uint16_t mask = 1)
            EventWait(exportMissoinSig_[i]);
        }
    } else {
        //DONE，单个入参版本EventRecord(const char *notifyTag, uint16_t mask = 1)
        EventRecord(importMissionSig_[0]);
    }
}

std::vector<uint64_t> CcuKernelKfcServer::GeneArgs(const CcuTaskArg &arg)
{
    // TODO：问题1：关于CcuRep::MaskSignal。
        //方案1：传入进来 CcuRep::MaskSignal，这里强转const char *notifyTag----当前使用
        //方案2：直接传进来const char *
    // 问题2：CcuRep::Variable如何转uintt64
    // 问题3：vector<>如何处理    

    const CcuTaskArgKfcServer *taskArg = dynamic_cast<const CcuTaskArgKfcServer *>(&arg);
    uint64_t waitAddr = taskArg->waitAddr_;
    uint64_t recordAddr = taskArg->recordAddr_;
    uint64_t paramAddr = taskArg->paramAddr_;
    uint64_t dieNum = taskArg->dieNum_;
    const char *exportDieSig = static_cast<const char *>(taskArg->exportDieSig_);
    const char *importDieSig = static_cast<const char *>(taskArg->importDieSig_);
    uint64_t missionNum = taskArg->missionNum_;
    uint64_t missionIndex = taskArg->missionIndex_;

    std::vector<const char*> exportMissoinSig;
    exportMissoinSig.reserve(taskArg->exportMissoinSig_.size());
    for (const auto& s : taskArg->exportMissoinSig_) {
        exportMissoinSig.push_back(static_cast<const char*>(s));
    }

    std::vector<const char*> importMissionSig;
    importMissionSig.reserve(taskArg->importMissionSig_.size());
    for (const auto& s : taskArg->importMissionSig_) {
        importMissionSig.push_back(static_cast<const char*>(s));
    }

    std::vector<CcuRep::Variable> exportMissionVar = taskArg->exportMissionVar_;
    std::vector<CcuRep::Variable> importMissionVar = taskArg->importMissionVar_;
    return {waitAddr, recordAddr, paramAddr,dieNum,exportDieSig,importDieSig, missionNum, missionIndex,
            exportMissoinSig, importMissionSig, exportMissionVar, importMissionVar};
}

}