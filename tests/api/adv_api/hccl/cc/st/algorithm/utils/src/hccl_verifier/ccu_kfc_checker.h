/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_CCU_KFC_CHECKER_H
#define HCCL_CCU_KFC_CHECKER_H

#include "ccu_checker.h"
#include "ccu_kfc_sim_world.h"

namespace HcclSim {
namespace CcuSt {

class CcuKfcChecker {
public:
    CheckResult CheckRegistration(const CcuKfcRegistration &registration,
        const CcuKfcQueueProxy &queue) const;
    CheckResult CheckContext(const std::vector<CcuKfcRegistration> &registrations) const;
    CheckResult CheckServerProgram(const Program &program, uint64_t opId) const;
    CheckResult CheckQueue(const CcuKfcQueueProxy &queue) const;
};

} // namespace CcuSt
} // namespace HcclSim

#endif // HCCL_CCU_KFC_CHECKER_H
