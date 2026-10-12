/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "aicpu_all_gather.h"

namespace HcclSim {
namespace AicpuSt {

std::map<std::string, ExpectationBuilder>& AicpuStAllGather::GetVariantRegistry()
{
    static std::map<std::string, ExpectationBuilder> registry;
    return registry;
}

void AicpuStAllGather::RegisterVariant(const std::string& algName, const ExpectationBuilder& builder)
{
    GetVariantRegistry()[algName] = builder;
}

void AicpuStAllGather::PrepareOpParam(
    OpParam& opParam, uint32_t rankSize, const AicpuStScenario& scenario, const RankMem& mem)
{
    (void)rankSize;
    opParam.DataDes.count = scenario.count;
    opParam.DataDes.dataType = scenario.dataType; // tiling 已设，此处冗余兜底
    opParam.inputPtr = mem.inputPtr;
    opParam.outputPtr = mem.outputPtr;
    opParam.inputSize = mem.inputBytes;
    opParam.outputSize = mem.outputBytes;
}

std::vector<uint8_t> AicpuStAllGather::BuildConcatExpectation(uint32_t rankSize, const AicpuStScenario& scenario)
{
    // 默认 AllGather 拼接语义：output[r] = concat(input[0..N-1])
    const uint64_t sliceBytes = scenario.count * DATATYPE_SIZE_TABLE[scenario.dataType];
    std::vector<uint8_t> expected;
    expected.reserve(sliceBytes * rankSize);
    for (uint32_t src = 0; src < rankSize; ++src) {
        for (uint64_t i = 0; i < sliceBytes; ++i) {
            expected.push_back(AicpuStFixture::MakeInputByte(src, i));
        }
    }
    return expected;
}

std::vector<uint8_t> AicpuStAllGather::BuildExpectedOutput(
    uint32_t rankSize, const AicpuStScenario& scenario, RankId rank, const std::string& actualAlgName)
{
    // golden 按实际选中算法键控：expectedAlgName 仅为算法选择断言字段（空=跳过断言），
    // 留空模式下 golden 仍按 actualAlgName 正确选择变体
    const auto it = GetVariantRegistry().find(actualAlgName);
    if (it != GetVariantRegistry().end()) {
        return it->second(rankSize, scenario, rank);
    }
    return BuildConcatExpectation(rankSize, scenario);
}

} // namespace AicpuSt
} // namespace HcclSim
