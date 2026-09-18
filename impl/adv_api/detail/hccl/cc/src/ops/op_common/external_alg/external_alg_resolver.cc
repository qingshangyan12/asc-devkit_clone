/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "external_alg_resolver.h"

#include <algorithm>

#include "external_alg_rules.h"
#include "log.h"

namespace mc2_ops_hccl {
namespace {
const char* EngineName(AlgEngine engine)
{
    switch (engine) {
        case AlgEngine::CCU:
            return "CCU";
        case AlgEngine::DPU:
            return "DPU";
        case AlgEngine::AICPU:
        default:
            return "AICPU";
    }
}

// opParam.engine 是请求引擎（CommEngine），与候选引擎（AlgEngine）是两个枚举，分开命名
const char* EngineName(CommEngine engine)
{
    switch (engine) {
        case CommEngine::COMM_ENGINE_CPU:
            return "CPU";
        case CommEngine::COMM_ENGINE_CPU_TS:
            return "CPU_TS";
        case CommEngine::COMM_ENGINE_AICPU:
            return "AICPU";
        case CommEngine::COMM_ENGINE_AICPU_TS:
            return "AICPU_TS";
        case CommEngine::COMM_ENGINE_AIV:
            return "AIV";
        case CommEngine::COMM_ENGINE_CCU:
            return "CCU";
        case CommEngine::COMM_ENGINE_RESERVED:
        default:
            return "RESERVED";
    }
}

// 进程级候选表（首次使用时聚合一次；static-init 已全部完成后才可能被调用）
const std::map<ExtAlgKey, std::vector<AlgCandidate>>& CandidateTable()
{
    static const std::map<ExtAlgKey, std::vector<AlgCandidate>> table =
        BuildCandidateTable(AlgMetaRegistry::Instance().GetAll());
    return table;
}

std::string DescribeCandidates(const std::vector<AlgCandidate>& candidates)
{
    std::string text;
    for (const AlgCandidate& cand : candidates) {
        if (!text.empty()) {
            text += ", ";
        }
        text += cand.registeredName;
        if (cand.variantCondId != -1) {
            text += "(" + std::string(VariantCondName(cand.variantCondId)) + ")";
        }
        if (cand.machineFlagId != -1) {
            text += "(" + std::string(MachineFlagName(cand.machineFlagId)) + ")";
        }
    }
    return text;
}
} // namespace

std::map<ExtAlgKey, std::vector<AlgCandidate>> BuildCandidateTable(const std::vector<AlgMetaRow>& rows)
{
    std::map<ExtAlgKey, std::vector<AlgCandidate>> table;
    for (const AlgMetaRow& row : rows) {
        table[std::make_pair(row.cmd, std::string(row.meta.externalName))].push_back(
            AlgCandidate{row.registeredName, row.meta.engine, row.meta.variantCondId, row.meta.machineFlagId});
    }
    for (auto& entry : table) {
        std::sort(entry.second.begin(), entry.second.end(), [](const AlgCandidate& a, const AlgCandidate& b) {
            const int specA = (a.variantCondId != -1 ? 1 : 0) + (a.machineFlagId != -1 ? 1 : 0);
            const int specB = (b.variantCondId != -1 ? 1 : 0) + (b.machineFlagId != -1 ? 1 : 0);
            if (specA != specB) {
                return specA > specB; // 特异性降序
            }
            return a.registeredName < b.registeredName; // 名字字典序
        });
    }
    return table;
}

const std::vector<AlgCandidate>& GetExternalCandidates(HcclCMDType cmd, const std::string& canonicalExtName)
{
    // 函数级 static：免疫跨编译单元 static-init 顺序的理论风险
    static const std::vector<AlgCandidate> kEmpty;
    const auto& table = CandidateTable();
    const auto it = table.find(std::make_pair(cmd, canonicalExtName));
    return it == table.end() ? kEmpty : it->second;
}

bool HasCcuCandidate(HcclCMDType cmd, const std::string& canonicalExtName)
{
    for (const AlgCandidate& cand : GetExternalCandidates(cmd, canonicalExtName)) {
        if (cand.engine == AlgEngine::CCU) {
            return true;
        }
    }
    return false;
}

ResolveStatus ResolveExternalAlg(
    const OpParam& opParam, const TopoInfoWithNetLayerDetails* topoInfo, const ExternalAlgSpec& spec,
    std::string& algName, std::string& errMsg)
{
    algName.clear();
    errMsg.clear();
    const std::vector<AlgCandidate>& candidates = GetExternalCandidates(opParam.opType, spec.canonical());
    const std::string candList = DescribeCandidates(candidates);

    // 选名审计日志（spec §12：一条 INFO 汇总；engine 打枚举名与 resolved 行风格一致）
    HCCL_INFO(
        "[MC2_EXT_ALG] op[%u] ext[%s] engine[%s] candidates(%zu): [%s]", static_cast<u32>(opParam.opType),
        spec.canonical().c_str(), EngineName(opParam.engine), candidates.size(), candList.c_str());

    for (const AlgCandidate& cand : candidates) {
        if (!EngineMatch(cand.engine, opParam)) { // (1) 引擎
            continue;
        }
        // (2) 变体条件：ID 双边判断消除注册侧写错值的 OOB 调用面（不在 Register() 做值域校验，
        // 因 rules.h 已 include alg_meta_registry.h，反向 include 会循环依赖）
        if (cand.variantCondId >= 0 && cand.variantCondId < VARIANT_COND_COUNT &&
            !VARIANT_COND_TABLE[cand.variantCondId](topoInfo, opParam)) {
            continue;
        }
        // (2') 机型 flag：同上双边判断
        if (cand.machineFlagId >= 0 && cand.machineFlagId < MACHINE_FLAG_COUNT &&
            !MACHINE_FLAG_TABLE[cand.machineFlagId](topoInfo)) {
            continue;
        }
        algName = cand.registeredName;
        HCCL_INFO("[MC2_EXT_ALG] resolved: %s (engine=%s)", algName.c_str(), EngineName(cand.engine));
        return ResolveStatus::RESOLVED;
    }

    // 报错三要素：外部名原文 / 候选列表 / 失败原因（opType 无现成名字映射，保持数字）
    const std::string reason =
        candidates.empty() ?
            "(no candidate registered in this build for engine " + std::string(EngineName(opParam.engine)) + ")" :
            "(all candidates rejected by rules for engine " + std::string(EngineName(opParam.engine)) + ")";
    errMsg = "resolve failed, ext=\"" + spec.canonical() + "\" op=" + std::to_string(static_cast<u32>(opParam.opType)) +
             ", candidates=[" + candList + "] " + reason;
    HCCL_ERROR("[MC2_EXT_ALG] ERROR: %s", errMsg.c_str());
    return ResolveStatus::ERROR_INVALID;
}
} // namespace mc2_ops_hccl
