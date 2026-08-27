/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_to_all_v_mc2_checker.h"

namespace HcclSim {
namespace CcuSt {
namespace {
CheckResult Failure(const std::string &message, Result result = Result::CONTRACT_ERROR)
{
    return CheckResult{result, message};
}
} // namespace

CheckResult CcuAllToAllVMc2Checker::CheckMessage(
    const std::array<uint64_t, CCU_KFC_WORDS_PER_SLOT> &message,
    const RankMemory &memory, uint32_t rankSize, uint64_t expectedOpId) const
{
    if (rankSize < 2 || rankSize > 16 || message[0] != expectedOpId ||
        DecodeKfcCollective(message[0]) != CCU_KFC_ALL_TO_ALL_V ||
        message[1] != memory.InputAddress() || message[2] != memory.OutputAddress()) {
        return Failure("AllToAllV KFC message opId or input/output address binding is invalid");
    }
    const uint64_t tableBase = memory.ParameterAddress();
    const uint64_t tableBytes = static_cast<uint64_t>(rankSize) * ALL_TO_ALL_V_EXT_FIELD_COUNT * sizeof(uint64_t);
    const uint64_t memoryBytes = memory.parameter.size() * sizeof(uint64_t);
    if (message[9] < tableBase || message[9] - tableBase > memoryBytes ||
        tableBytes > memoryBytes - (message[9] - tableBase)) {
        return Failure("AllToAllV KFC peer table is outside rank-local memory", Result::OUT_OF_RANGE);
    }
    return {};
}

CheckResult CcuAllToAllVMc2Checker::CheckPeerTable(const RankMemory &memory, uint64_t tableAddress,
    const std::vector<AllToAllVPeerParam> &peers) const
{
    if (tableAddress != memory.ParameterAddress() ||
        memory.parameter.size() != peers.size() * ALL_TO_ALL_V_EXT_FIELD_COUNT) {
        return Failure("AllToAllV KFC peer table base or size is invalid");
    }
    for (size_t peer = 0; peer < peers.size(); ++peer) {
        const size_t base = peer * ALL_TO_ALL_V_EXT_FIELD_COUNT;
        if (memory.parameter[base] != peers[peer].tailSize ||
            memory.parameter[base + 1U] != peers[peer].loopNum ||
            memory.parameter[base + 2U] != peers[peer].sendOffset ||
            memory.parameter[base + 3U] != peers[peer].recvOffset) {
            return Failure("AllToAllV KFC peer table does not match the current four-field ABI");
        }
    }
    return {};
}

} // namespace CcuSt
} // namespace HcclSim
