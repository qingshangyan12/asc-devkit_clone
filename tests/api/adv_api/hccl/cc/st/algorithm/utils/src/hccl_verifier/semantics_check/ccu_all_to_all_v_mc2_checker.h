/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_CCU_ALL_TO_ALL_V_MC2_CHECKER_H
#define HCCL_CCU_ALL_TO_ALL_V_MC2_CHECKER_H

#include "ccu_all_to_all_v_semantics_checker.h"
#include "ccu_kfc_proxy.h"

namespace HcclSim {
namespace CcuSt {

class CcuAllToAllVMc2Checker {
public:
    CheckResult CheckMessage(const std::array<uint64_t, CCU_KFC_WORDS_PER_SLOT> &message,
        const RankMemory &memory, uint32_t rankSize, uint64_t expectedOpId) const;
    CheckResult CheckPeerTable(const RankMemory &memory, uint64_t tableAddress,
        const std::vector<AllToAllVPeerParam> &peers) const;
};

} // namespace CcuSt
} // namespace HcclSim

#endif // HCCL_CCU_ALL_TO_ALL_V_MC2_CHECKER_H
