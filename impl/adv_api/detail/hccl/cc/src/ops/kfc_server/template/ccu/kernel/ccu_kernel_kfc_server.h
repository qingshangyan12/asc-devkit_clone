/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_CCU_KERNEL_KFC_SERVER_H
#define HCCL_CCU_KERNEL_KFC_SERVER_H

#include <vector>
#include <ios>
#include <array>
#include <map>
#include "utils.h"
#include "ccu_kernel.h"
#include "ccu_kernel_utils.h"
#include "ccu_kernel_alg_base.h"
#include "../ccu_temp_kfc_server.h"
#include "types/mc2_type.h"

namespace ops_hccl {

class CcuKernelArgKfcServer : public hcomm::CcuKernelArg {
public:
    explicit CcuKernelArgKfcServer(uint64_t dimSize, uint32_t rankId, bool loadFromMem, const OpParam& opParam,
                                   const std::vector<std::vector<uint32_t>>& subCommRanks)
        : dimSize_(dimSize),
          rankId_(rankId),
          loadFromMem_(loadFromMem),
          opParam_(opParam),
          subCommRanks_(subCommRanks)
    {
        HCCL_DEBUG("[CcuKernelArgKfcServer] dimSize: %lu, rankId: %u", dimSize_, rankId_);
    }
    hcomm::CcuKernelSignature GetKernelSignature() const override
    {
        hcomm::CcuKernelSignature signature;
        GenerateCcuKernelSignature(signature, "CcuKernelKfcServer", opParam_, subCommRanks_);
        return signature;
    }
    uint64_t dimSize_;
    uint32_t rankId_;
    OpParam opParam_;
    std::vector<std::vector<uint32_t>> subCommRanks_;
    bool loadFromMem_;
};

class CcuTaskArgKfcServer : public hcomm::CcuTaskArg {
public:
    explicit CcuTaskArgKfcServer(uint64_t xnAddr, uint64_t ckeAddr, uint64_t dieNum,
                                 uint64_t missionNum, uint64_t missionIndex, uint64_t token):
                                xnAddr_(syncAddr), ckeAddr_(ckeAddr), dieNum_(dieNum),
                                missionNum_(missionNum), missionIndex_(missionIndex), token_(token)
    {
        HCCL_DEBUG("[CcuTaskArgKfcServer] syncAddr: %lu, paramAddr: %lu, dieNum: %lu, "
                   "missionNum: %lu, missionIndex: %lu",
                   xnAddr_, ckeAddr_, dieNum_, missionNum_, missionIndex_);
    }
    // HBM上的算子执行参数首地址
    uint64_t xnAddr_{0};
    
    uint64_t ckeAddr_{0};
    // Die数量，用于判断单双Die，默认为1即单Die
    uint64_t dieNum_{1};
    uint64_t missionNum_{1};
    uint64_t missionIndex_{0};
    uint64_t token_;

};

class CcuKernelKfcServer : public CcuKernelAlgBase {
public:
    CcuKernelKfcServer(const hcomm::CcuKernelArg &arg)
    ~CcuKernelKfcServer() override {}

    HcclResult InitResource();
    void LoadArgs();                                            // 参数载入 check
    void SetCommAddr(ccu::Variable syncAddr_);                  // 配置同步需要的参数
    void SetDieNum(ccu::Variable dieNum_);                      // 配置dieNum
    void SetMissionNumAndId(ccu::Variable missionNum_, ccu::Variable missionIndex_);

    HcclResult Algorithm() override; // kfc server算法
    std::vector<uint64_t> GeneArgs(const hcomm::CcuTaskArg &arg) override;

    virtual void GenCircularQueue() = 0;
    void MissionPreSync(ccu::Variable &func);
    void MissionPostSync();

protected:
    ccu::Variable xnAddr_;           // HBM上的算子执行参数首地址
    ccu::Variable ckeAddr_;
    ccu::Variable waitAddr_;            // HBM上的每轮开始信号的首地址
    ccu::Variable recordAddr_;          // HBM上的每轮完成信号的首地址

    ccu::Variable token_;
    ccu::Variable dieNum_;              // Die数量，用于判断单双Die，默认为1即单Die
    ccu::Variable missionNum_; 
    ccu::Variable missionIndex_;

    // 用于Die间同步信号
    ccu::Event exportDieSig_;
    ccu::Event importDieSig_;
    // 用于Mission间同步信号
    std::vector<ccu::Event> exportMissoinSig_{};        // TODO：仔细分析，看如何去掉vector<>
    std::vector<ccu::Event> importMissionSig_{};
    // 用于Mission间同步变量
    std::vector<ccu::Variable> exportMissionVar_{};
    std::vector<ccu::Variable> importMissionVar_{};
private:
    CcuKfcKernelStruct kfcServerArgs;
};

class Mc2KfcServer : public CcuKernelKfcServer {
public:
    Mc2KfcServer() = default;
    ~Mc2KfcServer() override {}

protected:
    void GenCircularQueue() override;

private:
    void WaitTurnStartSig(const ccu::Variable &hbmSigAddr, ccu::Variable &turnStartSig);
    void SetTurnEndSig(const ccu::Variable &hbmSigAddr, const ccu::Variable &turnEndSig);
    void LoadFuncParamFromMemory(ccu::Variable &paramAddr, std::array<ccu::Variable, CCU_PARAM_NUM_PER_DIE> &param);
};

class Mc2SlaveKfcServer : public CcuKernelKfcServer {
public:
    Mc2SlaveKfcServer() = default;
    ~Mc2SlaveKfcServer() override {}

protected:
    void GenCircularQueue() override;

};

}

#endif