/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ALG_META_REGISTRY_H
#define ALG_META_REGISTRY_H

#include <map>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "hccl_types.h" // HcclCMDType / HcclResult

namespace mc2_ops_hccl {

// 候选引擎维度。显式声明，不按内部名前缀推导（前缀有历史包袱，如 InsAllGatherMeshNhrDPU 是
// AICPU sequence 编排 + DPU 卸载）。扩展位：CcuMS、AIV（hccl 平移时加枚举值）。
enum class AlgEngine { AICPU, CCU, DPU };

// 注册现场的 sidecar 元数据。variantCondId/machineFlagId 取值是 external_alg_rules.h 的
// VariantCondId/MachineFlagId 枚举；-1（*_NONE）表示该槽位无条件（裸名兜底候选）。
struct AlgMeta {
    AlgEngine engine = AlgEngine::AICPU;
    const char* externalName = ""; // 规范化外部名（与 parser 产出同构），如 "sole[mesh]"
    int variantCondId = -1;
    int machineFlagId = -1;
    // 同键内候选定序（hccl 平移的 flag 重叠场景逃生门，devkit 46 行当前全 0）：
    // 排序键第二级（specificity 降序 > priority 降序 > registeredName 字典序），
    // 数值大者先试漏斗。只影响排队顺序，不影响 ①②②′ 判定——priority 高的候选
    // 条件不命中照样出局，裸名（双 -1）仍兜底；特异性第一级保证裸名永不被 priority
    // 抬到特化候选之前。
    int priority = 0;
};

// 一行 sidecar 记录（枚举接口用）
struct AlgMetaRow {
    HcclCMDType cmd;
    std::string registeredName;
    AlgMeta meta;
};

class AlgMetaRegistry {
public:
    static AlgMetaRegistry& Instance();

    // 返回 HCCL_SUCCESS / HCCL_E_PARA(重复注册)。static-init 期间由 REGISTER_ALG_META 调用。
    HcclResult Register(HcclCMDType cmd, const std::string& registeredName, const AlgMeta& meta);
    const AlgMeta* Get(HcclCMDType cmd, const std::string& registeredName) const;
    // 全量快照（UT 满射断言 / 消歧器聚合用）
    std::vector<AlgMetaRow> GetAll() const;

private:
    AlgMetaRegistry() = default;
    std::map<std::pair<HcclCMDType, std::string>, AlgMeta> metas_;
    mutable std::mutex mu_;
};

// sidecar 注册宏（与 REGISTER_EXEC_V2 同款 static-init 套路，见
// coll_alg_v2_exec_registry.h:46-48）。必须写在主注册同一条件编译块内，
// 保证 sidecar 与主注册同生共死（守卫天然同步）。
// 第 7 参 priority 显式写出（无裁决意图恒填 0，见 AlgMeta::priority）：槽位在每行现场写好，
// 平移/扩展时改数字即可；将来任何加参企图会在全部现场编译报错，签名冻结自执行。
#define REGISTER_ALG_META_HELPER(ctr, cmd, regName, engine, extName, condId, flagId, prio) \
    static HcclResult g_alg_meta_##regName##_##ctr = AlgMetaRegistry::Instance().Register( \
        cmd, std::string(#regName), AlgMeta{engine, extName, condId, flagId, prio})

#define REGISTER_ALG_META_HELPER_1(ctr, cmd, regName, engine, extName, condId, flagId, prio) \
    REGISTER_ALG_META_HELPER(ctr, cmd, regName, engine, extName, condId, flagId, prio)

#define REGISTER_ALG_META(cmd, regName, engine, extName, condId, flagId, prio) \
    REGISTER_ALG_META_HELPER_1(__COUNTER__, cmd, regName, engine, extName, condId, flagId, prio)

} // namespace mc2_ops_hccl
#endif // ALG_META_REGISTRY_H
