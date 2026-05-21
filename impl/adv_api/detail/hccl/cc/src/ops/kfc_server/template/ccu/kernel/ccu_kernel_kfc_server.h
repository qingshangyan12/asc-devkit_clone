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

struct CcuKfcKernelStruct {

    // HBM上的每轮开始信号的首地址
    uint64_t waitAddr_{0};
    // HBM上的每轮完成信号的首地址
    uint64_t recordAddr_{0};
    // HBM上的算子执行参数首地址
    uint64_t paramAddr_{0};
    // Die数量，用于判断单双Die，默认为1即单Die
    uint32_t dieNum_{1};
    // 用于Die间同步信号
    CcuRep::MaskSignal exportDieSig;
    CcuRep::MaskSignal importDieSig;

    uint32_t missionNum_{1};
    uint32_t missionIndex_{0};
    // 用于Mission间同步信号
    std::vector<CcuRep::MaskSignal> exportMissoinSig;
    std::vector<CcuRep::MaskSignal> importMissionSig;
    // 用于Mission间同步变量
    std::vector<CcuRep::Variable> exportMissionVar;
    std::vector<CcuRep::Variable> importMissionVar;

}

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
    explicit CcuTaskArgKfcServer()
    {
        HCCL_DEBUG("[CcuTaskArgKfcServer] CcuTaskArgKfcServer");
    }
};

class CcuKernelKfcServer : public CcuKernelAlgBase {
public:
    CcuKernelKfcServer(const CcuKfcKernelStruct &arg), kfcServerArgs(arg); // 待定
    ~CcuKernelKfcServer() override {}

    HcclResult Algorithm() override;
    std::vector<uint64_t> GeneArgs(const hcomm::CcuTaskArg &arg) override;

private:
    virtual void GenCircularQueue() = 0;
    void MissionPreSync(CcuRep::Variable &func);
    void MissionPostSync();

    CcuKfcKernelStruct kfcServerArgs;

    HcclResult InitResource();
};

class Mc2KfcServer : public CcuKernelKfcServer {
public:
    Mc2KfcServer() = default;
    ~Mc2KfcServer() override {}

protected:
    void GenCircularQueue() override;

private:
    void WaitTurnStartSig(const CcuRep::Variable &hbmSigAddr, CcuRep::Variable &turnStartSig);
    void SetTurnEndSig(const CcuRep::Variable &hbmSigAddr, const CcuRep::Variable &turnEndSig);
    void LoadFuncParamFromMemory(CcuRep::Variable &paramAddr, std::array<CcuRep::Variable, CCU_PARAM_NUM_PER_DIE> &param);
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