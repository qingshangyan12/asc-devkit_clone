/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef KFC_CALLABLE_TASK_REGISTRY_H
#define KFC_CALLABLE_TASK_REGISTRY_H

#include <map>
#include <mutex>

#include "ccu_types.h"
#include "hccl_types.h"
#include "kfc_server_protocol.h"

namespace mc2_ops_hccl {

using KfcCallableTaskFunc = CcuResult (*)(CcuKernelArg);

struct KfcCallableTask {
    const char* funcName = nullptr;
    KfcCallableTaskFunc func = nullptr;
};

class KfcCallableTaskRegistry {
public:
    static KfcCallableTaskRegistry& Instance();
    HcclResult Register(HcclCMDType opType, KfcServerRole role, const char* funcName, KfcCallableTaskFunc func);
    bool Get(HcclCMDType opType, KfcServerRole role, KfcCallableTask& task) const;

private:
    std::map<HcclCMDType, std::map<KfcServerRole, KfcCallableTask>> taskRegistry_;
    mutable std::mutex mu_;
};

#define REGISTER_KFC_CALLABLE_TASK_HELPER(ctr, opType, role, taskName, taskFunc) \
    static HcclResult g_kfc_callable_task_##ctr =                                \
        KfcCallableTaskRegistry::Instance().Register(opType, role, #taskName, taskFunc)

#define REGISTER_KFC_CALLABLE_TASK_HELPER_1(ctr, opType, role, taskName, taskFunc) \
    REGISTER_KFC_CALLABLE_TASK_HELPER(ctr, opType, role, taskName, taskFunc)

#define REGISTER_KFC_CALLABLE_TASK(opType, role, taskName, taskFunc) \
    REGISTER_KFC_CALLABLE_TASK_HELPER_1(__COUNTER__, opType, role, taskName, taskFunc)

} // namespace mc2_ops_hccl
#endif // KFC_CALLABLE_TASK_REGISTRY_H
