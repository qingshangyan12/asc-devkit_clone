/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef TOPO_MATCHER_H
#define TOPO_MATCHER_H

#include <condition_variable>
#include "dispatcher.h"
#include "externalinput_pub.h"
#include "coll_alg_param.h"
#include "alg_template_base_pub.h"
#include "hccl_common.h"
#include "alg_env_config.h"

namespace hccl {
constexpr u32 COMM_LEVEL1_INDEX = COMM_LEVEL1;
using HcclAlgoInfo = struct HcclAlgoInfoDef {
    bool inlineReduceSwitchOn;       // 收到数量时同时完成Reduce计算
    std::string identifier;
    bool isUsedRdmaLevel0;
    bool isSupportAtomicWrite;

    HcclAlgoInfoDef()
        : inlineReduceSwitchOn(true),
        identifier(""),
        isUsedRdmaLevel0(false),
        isSupportAtomicWrite(false)
    {}
};

struct HcclTopoInfo {
    u32 userRank;                    // 通信域 RankID
    u32 userRankSize;                // 通信域的 Rank数量
    u32 devicePhyId;
    s32 deviceLogicId;
    std::vector<u32> nicList;
    bool isSingleMeshAggregation;
    u32 deviceNumPerAggregation;     // 每个module中的Device数量
    u32 superPodNum;                 // 集群中总的超节点数
    DevType deviceType;
    TopoType topoType;
    bool is310P3Common;
    u32 serverNum;
    u32 meshAggregationRankSize;
    u32 multiModuleDiffDeviceNumMode;
    u32 multiSuperPodDiffServerNumMode;
    u32 multiSuperPodDiffDeviceNumMode;
    bool isDiffDeviceType;
    u32 gcdDeviceNumPerAggregation;
    u32 realUserRank;
    bool isDiffDeviceModule;
    u32 moduleNum;
    bool useSuperPodMode;
    std::unordered_map<u32, bool> isUsedRdmaMap;
    std::unordered_map<u32, u32> pairLinkCounter; // server内所有device间的链路类型计数
    bool isARSDoubleRing;

    std::vector<std::vector<std::vector<std::vector<u32>>>> CommPlaneSubGroupVector; // 保存所有 level 的通信分组信息
    std::map<AHCConcOpType, TemplateType> ahcAlgOption;

    HcclTopoInfo()
        : userRank(0),
        userRankSize(0),
        devicePhyId(0),
        deviceLogicId(0),
        nicList(0),
        isSingleMeshAggregation(false),
        deviceNumPerAggregation(0),
        superPodNum(0),
        deviceType(DevType::DEV_TYPE_COUNT),
        topoType(TopoType::TOPO_TYPE_COMMON),
        is310P3Common(false),
        serverNum(0),
        meshAggregationRankSize(0),
        multiModuleDiffDeviceNumMode(0),
        multiSuperPodDiffServerNumMode(0),
        multiSuperPodDiffDeviceNumMode(0),
        isDiffDeviceType(false),
        realUserRank(0),
        isDiffDeviceModule(false),
        moduleNum(0),
        useSuperPodMode(false),
        isARSDoubleRing(true)
    {}
};

using HcclExternalEnable = struct HcclExternalEnableDef {
    u32 enableFfts;
    u32 deterministic;
    u32 intraRoceSwitch;
    u32 dumpDebug;
    u32 interHccsDisable;
    bool aivMode;
    bool aicpuUnfold;
    bool isOnlyAiv;
    s32 execTimeOut;
    std::map<HcclCMDType, std::vector<HcclAlgoType>> algoConfig;

    HcclExternalEnableDef()
        : enableFfts(1),
        deterministic(0),
        intraRoceSwitch(0),
        dumpDebug(0),
        interHccsDisable(0),
        aivMode(false),
        aicpuUnfold(false),
        isOnlyAiv(false),
        execTimeOut(GetInternalExecTimeOut())
    {
        SetDefaultAlgo();
    }
    void SetDefaultAlgo()
    {
        for (u32 opType = 0; opType < static_cast<u32>(HcclCMDType::HCCL_CMD_MAX); opType++) {
            algoConfig[static_cast<HcclCMDType>(opType)] = GetExternalInputHcclAlgoConfig(static_cast<HcclCMDType>(opType));
        }
    }
};
}  // namespace hccl

#endif /* * TOPO_MATCHER_H */
