/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_CCU_KFC_SIM_WORLD_H
#define HCCL_CCU_KFC_SIM_WORLD_H

#include "ccu_kfc_proxy.h"
#include "ccu_sim_world.h"

namespace HcclSim {
namespace CcuSt {

class CcuKfcSimWorld {
public:
    Result DispatchOne(const std::vector<CcuKfcRegistration> &registrations,
        const std::vector<CcuKfcQueueProxy *> &queues, std::vector<RankMemory> &memories);
    const RuntimeWorld &AlgorithmWorld() const;
    const std::string &LastError() const;
    bool Stopped() const;

private:
    void SetError(const std::string &message);

    RuntimeWorld algorithmWorld_;
    std::string lastError_;
    bool stopped_{false};
};

} // namespace CcuSt
} // namespace HcclSim

#endif // HCCL_CCU_KFC_SIM_WORLD_H
