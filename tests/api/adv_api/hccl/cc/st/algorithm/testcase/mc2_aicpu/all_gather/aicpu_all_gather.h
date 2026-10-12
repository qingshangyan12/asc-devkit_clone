/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef AICPU_ST_ALL_GATHER_H
#define AICPU_ST_ALL_GATHER_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "aicpu_fixture.h"
#include "alg_param.h"

namespace HcclSim {
namespace AicpuSt {

// 期望输出构造器：入参 (rankSize, scenario, rank)，返回该 rank 的期望输出字节
using ExpectationBuilder = std::function<std::vector<uint8_t>(uint32_t, const AicpuStScenario&, RankId)>;

class AicpuStAllGather : public AicpuStFixture {
public:
    // 按算法名注册变体的 golden 构造器；查找键为 selector 实际选中的算法名（与
    // scenario.expectedAlgName 解耦——后者仅为断言字段）。未注册的算法走默认拼接语义
    static void RegisterVariant(const std::string& algName, const ExpectationBuilder& builder);

    // 拼接语义 golden：output[r] = concat(input[0..N-1])；默认回退与各算法变体共用
    static std::vector<uint8_t> BuildConcatExpectation(uint32_t rankSize, const AicpuStScenario& scenario);

protected:
    void PrepareOpParam(
        OpParam& opParam, uint32_t rankSize, const AicpuStScenario& scenario, const RankMem& mem) override;

    std::vector<uint8_t> BuildExpectedOutput(
        uint32_t rankSize, const AicpuStScenario& scenario, RankId rank, const std::string& actualAlgName) override;

private:
    static std::map<std::string, ExpectationBuilder>& GetVariantRegistry();
};

} // namespace AicpuSt
} // namespace HcclSim

#endif // AICPU_ST_ALL_GATHER_H
