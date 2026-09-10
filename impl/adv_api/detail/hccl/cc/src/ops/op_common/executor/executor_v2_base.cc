/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "executor_v2_base.h"
#include "adapter_error_manager_pub.h"
#include "selector/alg_parse.h"

namespace mc2_ops_hccl {

InsCollAlgBase::InsCollAlgBase() {}

InsCollAlgBase::~InsCollAlgBase() {}

std::string InsCollAlgBase::Describe() const
{
    std::string s = "111";
    return s;
}

HcclResult InsCollAlgBase::CalcAlgHierarchyInfoV2(
    TopoInfoWithNetLayerDetails* topoInfo, AlgHierarchyInfoForAllLevel& algHierarchyInfo, const AlgAttrs& algAttrs)
{
    (void)topoInfo;
    (void)algHierarchyInfo;
    (void)algAttrs;
    return HCCL_SUCCESS;
}

AlgAttrs InsCollAlgBase::ParseAlgAttrsFromName(const std::string& algName)
{
    AlgAttrs attrs;
    attrs.name = algName;

    // 按算法名分段解析（对齐 hccl AlgAttrsRegistry::ParseAlgName）：引擎 → opType → executor ×2 → algoTypes 贪婪匹配。
    // Bridge 场景下 CANN 侧 TopoMatchTwoLevel 依赖 algoTypes.size()==2 做两级分组，空 attrs 会导致 forced 校验失败。
    std::string remaining = algName;
    for (const auto& prefix : GetEnginePrefixes()) {
        if (remaining.size() > prefix.size() && remaining.compare(0, prefix.size(), prefix) == 0) {
            remaining = remaining.substr(prefix.size());
            break;
        }
    }
    for (const auto& pattern : GetOpTypePatterns()) {
        const std::string& pascal = pattern.first;
        if (remaining.size() >= pascal.size() && remaining.compare(0, pascal.size(), pascal) == 0) {
            attrs.opType = pattern.second;
            remaining = remaining.substr(pascal.size());
            break;
        }
    }
    for (int pass = 0; pass < 2; ++pass) {
        bool found = false;
        for (const auto& name : GetExecutorNames()) {
            if (remaining.size() >= name.size() && remaining.compare(0, name.size(), name) == 0) {
                remaining = remaining.substr(name.size());
                found = true;
                break;
            }
        }
        if (!found) {
            break;
        }
    }
    size_t pos = 0;
    while (pos < remaining.size()) {
        bool matched = false;
        for (const auto& kv : GetAlgoNameToTypeMap()) {
            if (remaining.compare(pos, kv.first.size(), kv.first) == 0) {
                attrs.algoTypes.push_back(kv.second);
                pos += kv.first.size();
                matched = true;
                break;
            }
        }
        if (!matched) {
            HCCL_WARNING(
                "[InsCollAlgBase][ParseAlgAttrsFromName] skipped unmatched algoName=%s at pos=%zu, remaining='%s'.",
                algName.c_str(), pos, remaining.c_str() + pos);
            attrs.algoTypes.clear();
            break;
        }
    }
    return attrs;
}

AlgAttrs InsCollAlgBase::GetAlgoMeta(const std::string& algName) const { return ParseAlgAttrsFromName(algName); }

HcclResult InsCollAlgBase::RestoreChannelMap(
    const AlgResourceCtxSerializable& resCtx,
    std::vector<std::map<u32, std::vector<ChannelInfo>>>& rankIdToChannelInfo) const
{
    const AlgHierarchyInfoForAllLevel& algHierarchyInfo = resCtx.algHierarchyInfo;
    const size_t infosSize = algHierarchyInfo.infos.size();
    const size_t channelsSize = resCtx.channels.size();
    if (infosSize > channelsSize) {
        HCCL_ERROR(
            "[MC2_OPEN_DIAG][RestoreChannelMap] channel level out of range, infosSize[%zu], "
            "channelsSize[%zu]",
            infosSize, channelsSize);
        return HcclResult::HCCL_E_INTERNAL;
    }
    rankIdToChannelInfo.resize(infosSize);
    for (u32 level = 0; level < infosSize; level++) {
        for (auto& channel : resCtx.channels[level]) {
            u32 remoteRank = channel.remoteRank;
            rankIdToChannelInfo[level][remoteRank].push_back(channel);
        }
        // 不需要再resize内层的map，因为map会自动管理元素
    }
    return HCCL_SUCCESS;
}

HcclResult InsCollAlgBase::SetTempFastLaunchAddr(
    TemplateFastLaunchCtx& tempFastLaunchCtx, void* inputPtr, void* outputPtr, const HcclMem& hcclBuff) const
{
    tempFastLaunchCtx.buffInfo.inputPtr = inputPtr;
    tempFastLaunchCtx.buffInfo.outputPtr = outputPtr;
    tempFastLaunchCtx.buffInfo.hcclBuff = hcclBuff;
    return HCCL_SUCCESS;
}

HcclResult InsCollAlgBase::FastLaunch(const OpParam& param, const CcuFastLaunchCtx* resCtx)
{
    (void)param;
    (void)resCtx;
    HCCL_ERROR("[InsCollAlgBase] Unsupported interface of InsCollAlgBase::FastLaunch!");
    return HcclResult::HCCL_E_INTERNAL;
}
} // namespace mc2_ops_hccl
