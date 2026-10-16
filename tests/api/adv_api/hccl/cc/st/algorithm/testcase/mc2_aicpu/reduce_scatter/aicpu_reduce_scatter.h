/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef AICPU_ST_REDUCE_SCATTER_H
#define AICPU_ST_REDUCE_SCATTER_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "aicpu_fixture.h"
#include "alg_param.h"

namespace HcclSim {
namespace AicpuSt {

// 期望输出构造器：入参 (rankSize, scenario, rank)，返回该 rank 的期望输出字节
using ReduceScatterExpectationBuilder = std::function<std::vector<uint8_t>(uint32_t, const AicpuStScenario&, RankId)>;

class AicpuStReduceScatter : public AicpuStFixture {
public:
    // 按算法名注册变体的 golden 构造器；查找键为 selector 实际选中的算法名（与
    // scenario.expectedAlgName 解耦——后者仅为断言字段）。未注册的算法走默认分片归约语义
    static void RegisterVariant(const std::string& algName, const ReduceScatterExpectationBuilder& builder);

    // 分片归约语义 golden：output[r] = elem-wise reduce(input[0..N-1]) 的第 r 分片。
    // 归约顺序契约（浮点顺序敏感）：模拟 Mesh1D 模板 PostCopy 实际执行序——usrOut 先经
    // LocalCopy 置为本 rank 输入的第 r 分片，再按 rank 升序（跳过自身）逐对端 LocalReduce
    // （对端 cclBuff[r] 分片 = 对端 input 的第 r 分片，write-based reduce-scatter 语义）；
    // 每步 load-op-store 与执行器 ReduceInto 共用 float_convert 同一舍入策略，浮点逐位一致。
    static std::vector<uint8_t> BuildReduceScatterExpectation(
        uint32_t rankSize, const AicpuStScenario& scenario, RankId rank);

protected:
    void PrepareOpParam(
        OpParam& opParam, uint32_t rankSize, const AicpuStScenario& scenario, const RankMem& mem) override;

    // 归约系声明：RS 数据面调 LocalReduce（存在产品直算路径），64bit/PROD 场景被能力守卫拒绝
    bool IsReduceLikeOp() const override { return true; }

    // ReduceScatter 语义：每 rank 输出 1 个分片（count 元素），输入为 rankSize 个分片——
    // 非 AllGather 默认布局，必须 override
    RankMemLayout BuildMemLayout(uint32_t rankSize, const AicpuStScenario& scenario) override;

    std::vector<uint8_t> BuildExpectedOutput(
        uint32_t rankSize, const AicpuStScenario& scenario, RankId rank, const std::string& actualAlgName) override;

private:
    static std::map<std::string, ReduceScatterExpectationBuilder>& GetVariantRegistry();
};

} // namespace AicpuSt
} // namespace HcclSim

#endif // AICPU_ST_REDUCE_SCATTER_H
