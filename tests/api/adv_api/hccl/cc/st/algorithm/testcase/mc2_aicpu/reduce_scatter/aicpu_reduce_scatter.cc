/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "aicpu_reduce_scatter.h"

#include "aicpu_float_convert.h"
#include "exception_util.h"

namespace HcclSim {
namespace AicpuSt {

std::map<std::string, ReduceScatterExpectationBuilder>& AicpuStReduceScatter::GetVariantRegistry()
{
    static std::map<std::string, ReduceScatterExpectationBuilder> registry;
    return registry;
}

void AicpuStReduceScatter::RegisterVariant(const std::string& algName, const ReduceScatterExpectationBuilder& builder)
{
    GetVariantRegistry()[algName] = builder;
}

void AicpuStReduceScatter::PrepareOpParam(
    OpParam& opParam, uint32_t rankSize, const AicpuStScenario& scenario, const RankMem& mem)
{
    (void)rankSize;
    // RS 的 count = 每 rank 输出元素数（分片大小）；输入为 rankSize 个分片
    opParam.DataDes.count = scenario.count;
    opParam.DataDes.dataType = scenario.dataType; // tiling 已设，此处冗余兜底
    opParam.reduceType = scenario.reduceType;     // tiling 链不填 reduceType，fixture 语义字段
    opParam.inputPtr = mem.inputPtr;
    opParam.outputPtr = mem.outputPtr;
    opParam.inputSize = mem.inputBytes;
    opParam.outputSize = mem.outputBytes;
}

RankMemLayout AicpuStReduceScatter::BuildMemLayout(uint32_t rankSize, const AicpuStScenario& scenario)
{
    RankMemLayout layout;
    const uint64_t sliceBytes = scenario.count * DATATYPE_SIZE_TABLE[scenario.dataType];
    layout.inputBytes = sliceBytes * rankSize; // 每 rank 全量输入（rankSize 个分片）
    layout.outputBytes = sliceBytes;           // 每 rank 输出自己的 1 个分片
    return layout;
}

std::vector<uint8_t> AicpuStReduceScatter::BuildReduceScatterExpectation(
    uint32_t rankSize, const AicpuStScenario& scenario, RankId rank)
{
    // reduceOp 前置校验：ApplyOp/ApplyIntOp 对非法 op 的 default 分支静默返回 b（float_convert
    // 既有行为），golden 若不校验会把配置错误（如误设 RESERVED）伪装成数据失配（"byte
    // mismatch"误导排查方向）。THROW 由 fixture 主线程 catch 转为带因的用例失败。
    switch (scenario.reduceType) {
        case HcclReduceOp::HCCL_REDUCE_SUM:
        case HcclReduceOp::HCCL_REDUCE_PROD:
        case HcclReduceOp::HCCL_REDUCE_MAX:
        case HcclReduceOp::HCCL_REDUCE_MIN:
            break;
        default:
            THROW<HcclSim::InvalidParamsException>(
                "[AicpuStReduceScatter] invalid reduceType[%u] in scenario (golden refuses to guess)",
                static_cast<uint32_t>(scenario.reduceType));
    }
    const uint64_t sliceBytes = scenario.count * DATATYPE_SIZE_TABLE[scenario.dataType];
    const uint64_t elem = ElemSize(scenario.dataType);
    // 64bit 类型走产品 AicpuReduce（wrapper 直接计算，不经任务图 ReduceInto），
    // golden 按同样逐步 load-op-store 语义模拟（与 AicpuReduceTemplate<T> 同源行为）
    const bool is64Bit = scenario.dataType == HcclDataType::HCCL_DATA_TYPE_INT64 ||
                         scenario.dataType == HcclDataType::HCCL_DATA_TYPE_UINT64 ||
                         scenario.dataType == HcclDataType::HCCL_DATA_TYPE_FP64;
    // 执行器与 golden 均不支持的类型（ElemSize=0 且非 64bit）返回空：
    // 非零场景由 golden 空守卫 FAIL（显式暴露而非静默假绿）
    if (sliceBytes == 0 || (elem == 0 && !is64Bit) || (is64Bit && sliceBytes % 8U != 0)) {
        return {};
    }
    const uint64_t elemCount = is64Bit ? sliceBytes / 8U : sliceBytes / elem;

    // 步骤 1：usrOut = 本 rank 输入的第 rank 分片（Mesh1D PostCopy 的 LocalCopy）
    // input 的第 r 分片基址 = r * sliceBytes（inputSliceStride = sliceSize）
    std::vector<uint8_t> expected(sliceBytes);
    for (uint64_t i = 0; i < sliceBytes; ++i) {
        expected[i] = AicpuStFixture::MakeInputByte(rank, rank * sliceBytes + i);
    }

    // 步骤 2：按 rank 升序（跳过自身）逐对端并入——顺序与 Mesh1D PostCopy 的 LocalReduce 循环一致；
    // 对端 cclBuff[rank] 分片 = 对端 input 的第 rank 分片（write-based reduce-scatter 语义）
    const HcclDataType dt = scenario.dataType;
    const HcclReduceOp op = scenario.reduceType;
    uint8_t src[8] = {0};
    for (uint32_t r = 0; r < rankSize; ++r) {
        if (r == rank) {
            continue;
        }
        for (uint64_t i = 0; i < elemCount; ++i) {
            if (is64Bit) {
                // AicpuReduceTemplate<int64_t/uint64_t/double> 同源：memcpy 读出、逐元素 op、memcpy 写回
                for (uint64_t b = 0; b < 8U; ++b) {
                    src[b] = AicpuStFixture::MakeInputByte(r, rank * sliceBytes + i * 8U + b);
                }
                if (dt == HcclDataType::HCCL_DATA_TYPE_FP64) {
                    double dstV = 0.0;
                    double srcV = 0.0;
                    (void)memcpy(&dstV, &expected[i * 8U], sizeof(dstV));
                    (void)memcpy(&srcV, src, sizeof(srcV));
                    const double acc = (op == HcclReduceOp::HCCL_REDUCE_SUM)  ? (dstV + srcV) :
                                       (op == HcclReduceOp::HCCL_REDUCE_PROD) ? (dstV * srcV) :
                                       (op == HcclReduceOp::HCCL_REDUCE_MAX)  ? ((srcV > dstV) ? srcV : dstV) :
                                                                                ((srcV < dstV) ? srcV : dstV);
                    (void)memcpy(&expected[i * 8U], &acc, sizeof(acc));
                } else {
                    int64_t dstV = 0;
                    int64_t srcV = 0;
                    (void)memcpy(&dstV, &expected[i * 8U], sizeof(dstV));
                    (void)memcpy(&srcV, src, sizeof(srcV));
                    // 有符号溢出与产品 AicpuReduceTemplate<int64_t> 同为 gcc wrap 语义；UINT64 同位型
                    const int64_t acc = ApplyIntOp(dstV, srcV, op);
                    (void)memcpy(&expected[i * 8U], &acc, sizeof(acc));
                }
                continue;
            }
            for (uint64_t b = 0; b < elem; ++b) {
                src[b] = AicpuStFixture::MakeInputByte(r, rank * sliceBytes + i * elem + b);
            }
            if (IsIntegerReduceType(dt)) {
                StoreIntElem(
                    &expected[i * elem], dt,
                    ApplyIntOp(LoadIntElem(&expected[i * elem], dt), LoadIntElem(src, dt), op));
            } else {
                StoreElem(&expected[i * elem], dt, ApplyOp(LoadElem(&expected[i * elem], dt), LoadElem(src, dt), op));
            }
        }
    }
    return expected;
}

std::vector<uint8_t> AicpuStReduceScatter::BuildExpectedOutput(
    uint32_t rankSize, const AicpuStScenario& scenario, RankId rank, const std::string& actualAlgName)
{
    // golden 按实际选中算法键控：expectedAlgName 仅为算法选择断言字段（空=跳过断言），
    // 留空模式下 golden 仍按 actualAlgName 正确选择变体
    const auto it = GetVariantRegistry().find(actualAlgName);
    if (it != GetVariantRegistry().end()) {
        return it->second(rankSize, scenario, rank);
    }
    return BuildReduceScatterExpectation(rankSize, scenario, rank);
}

} // namespace AicpuSt
} // namespace HcclSim
