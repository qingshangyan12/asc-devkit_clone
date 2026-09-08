/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "kfc_callable_task_registry.h"
#include "log.h"

namespace mc2_ops_hccl {

KfcCallableTaskRegistry& KfcCallableTaskRegistry::Instance()
{
    static KfcCallableTaskRegistry globalKfcCallableTaskRegistry;
    return globalKfcCallableTaskRegistry;
}

HcclResult KfcCallableTaskRegistry::Register(
    HcclCMDType opType, KfcServerRole role, const char* funcName, KfcCallableTaskFunc func)
{
    const std::lock_guard<std::mutex> lock(mu_);
    auto& roleRegistry = taskRegistry_[opType];
    if (roleRegistry.count(role) != 0U) {
        HCCL_WARNING(
            "[KfcCallableTaskRegistry]Task opType[%u], role[%u] already registered.", static_cast<uint32_t>(opType),
            static_cast<uint32_t>(role));
        return HcclResult::HCCL_E_INTERNAL;
    }
    roleRegistry.emplace(role, KfcCallableTask{funcName, func});
    return HcclResult::HCCL_SUCCESS;
}

bool KfcCallableTaskRegistry::Get(HcclCMDType opType, KfcServerRole role, KfcCallableTask& task) const
{
    const std::lock_guard<std::mutex> lock(mu_);
    const auto opIter = taskRegistry_.find(opType);
    if (opIter == taskRegistry_.end()) {
        return false;
    }
    const auto roleIter = opIter->second.find(role);
    if (roleIter == opIter->second.end()) {
        return false;
    }
    task = roleIter->second;
    return true;
}

} // namespace mc2_ops_hccl
