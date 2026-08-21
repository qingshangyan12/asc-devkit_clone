/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_runtime_checker.h"

#include <algorithm>
#include <cstring>
#include <sstream>

namespace HcclSim {
namespace CcuSt {

namespace {

void ApplyCopy(std::vector<uint8_t> &dst, uint64_t dstOffset,
    const std::vector<uint8_t> &src, uint64_t srcOffset, uint64_t size)
{
    std::copy_n(src.begin() + static_cast<size_t>(srcOffset), static_cast<size_t>(size),
        dst.begin() + static_cast<size_t>(dstOffset));
}

void ApplyReduce(std::vector<uint8_t> &dst, uint64_t dstOffset,
    const std::vector<uint8_t> &src, uint64_t srcOffset, uint64_t size,
    HcclDataType dataType, HcclReduceOp reduceOp)
{
    // TODO: 实现 reduce 逻辑（SUM/MAX/MIN/PROD），当前只支持纯拷贝
    ApplyCopy(dst, dstOffset, src, srcOffset, size);
}

} // namespace

CheckResult CaptureRuntimeExpectation(const ScenarioData &data,
    const CcuStScenario &scenario, RuntimeExpectation &expectation)
{
    const size_t rankSize = data.memories.size();
    if (rankSize == 0) {
        return {Result::PARAM_ERROR, "no memories in scenario data"};
    }
    expectation.outputs.resize(rankSize);
    for (size_t rank = 0; rank < rankSize; ++rank) {
        expectation.outputs[rank] = data.memories[rank].output;
    }
    for (size_t srcRank = 0; srcRank < rankSize; ++srcRank) {
        for (size_t dstRank = 0; dstRank < rankSize; ++dstRank) {
            if (srcRank >= data.srcOffsets.size() || dstRank >= data.srcOffsets[srcRank].size()) {
                continue;
            }
            const uint64_t srcOffset = data.srcOffsets[srcRank][dstRank];
            const uint64_t dstOffset = data.dstOffsets[srcRank][dstRank];
            const uint64_t size = scenario.sizes[srcRank][dstRank];
            const auto &src = data.memories[srcRank].input;
            auto &dst = expectation.outputs[dstRank];
            if (srcOffset + size > src.size() || dstOffset + size > dst.size()) {
                return {Result::OUT_OF_RANGE, "expected transfer is out of range"};
            }
            if (data.reduceOp == HCCL_REDUCE_RESERVED) {
                ApplyCopy(dst, dstOffset, src, srcOffset, size);
            } else {
                ApplyReduce(dst, dstOffset, src, srcOffset, size, scenario.dataType, data.reduceOp);
            }
        }
    }
    return {};
}

CheckResult CheckRuntimeOutput(const std::vector<RankMemory> &memories,
    const RuntimeExpectation &expectation, const RuntimeWorld &world)
{
    if (expectation.outputs.size() != memories.size()) {
        return {Result::CONTRACT_ERROR, "expected output rank count does not match memories"};
    }
    for (size_t rank = 0; rank < memories.size(); ++rank) {
        if (memories[rank].output != expectation.outputs[rank]) {
            const auto mismatch = std::mismatch(memories[rank].output.begin(), memories[rank].output.end(),
                expectation.outputs[rank].begin());
            std::ostringstream message;
            message << "output mismatch at rank " << rank;
            if (mismatch.first != memories[rank].output.end()) {
                message << ", byte " << std::distance(memories[rank].output.begin(), mismatch.first);
            }
            return {Result::CONTRACT_ERROR, message.str()};
        }
    }
    return {};
}

} // namespace CcuSt
} // namespace HcclSim
