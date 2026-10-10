/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef AICPU_ST_FIXTURE_H
#define AICPU_ST_FIXTURE_H

#include <gtest/gtest.h>

#include <cstdint>
#include <string>
#include <vector>

#include "aicpu_scenario.h"
#include "alg_param.h"
#include "hccl_tiling_msg.h"

extern "C" void SetTestLogLevel(uint32_t level);

namespace HcclSim {
namespace AicpuSt {

// 产品侧类型导入（alg_param.h/hccl_tiling_msg.h 均位于产品命名空间）
using HcclApi::Mc2CcTilingInner;
using mc2_ops_hccl::DATATYPE_SIZE_TABLE;
using mc2_ops_hccl::OpParam;

// 每 rank 输入输出内存布局（fixture 在 rank 线程内 aclrtSetDevice 后分配）
struct RankMemLayout {
    uint64_t inputBytes = 0;
    uint64_t outputBytes = 0;
};

struct RankMem {
    void* inputPtr = nullptr;
    void* outputPtr = nullptr;
    uint64_t inputBytes = 0;
    uint64_t outputBytes = 0;
};

struct RankResult {
    bool ok = false;
    std::string algName;
    std::string error;
};

class AicpuStFixture : public ::testing::Test {
public:
    // 输入数据模式：rank 与 byte 偏移的可区分编码，保证逐字节可溯源。
    // rank < 16 时字节模式全局唯一（rank ≥ 16 会回绕混叠，当前拓扑不超过 8 rank）。
    // public：adapter 的 golden 构造器（非成员函数）需与 fixture 填充共用同一公式。
    static uint8_t MakeInputByte(RankId rank, uint64_t offset)
    {
        return static_cast<uint8_t>((rank + 1) * 0x10 + (offset & 0x0F));
    }

protected:
    void VerifyScenario(const AicpuStScenario& scenario);

    void EnableLog(uint32_t level = 0) { SetTestLogLevel(level); }

    void DisableLog() { SetTestLogLevel(0x03); }

    // ===== 子类必须实现的虚函数 =====

    // 填 OpParam 中 tiling 未携带的算子语义字段（count/inputPtr/outputPtr 等）。
    // InitOpParamByTiling 会将 DataDes.count 置 0、inputPtr/outputPtr 清零，
    // selector 与 CalcRes 依赖这些字段，必须在 selector 之前补齐。
    virtual void PrepareOpParam(
        OpParam& opParam, uint32_t rankSize, const AicpuStScenario& scenario, const RankMem& mem) = 0;

    // 每 rank 输入输出字节数
    // 注意：默认布局按 AllGather 语义（输出 = rankSize × 输入）——非 AllGather 算子必须 override。
    virtual RankMemLayout BuildMemLayout(uint32_t rankSize, const AicpuStScenario& scenario)
    {
        RankMemLayout layout;
        layout.inputBytes = scenario.count * DATATYPE_SIZE_TABLE[scenario.dataType];
        // AllGather 语义：输出 = rankSize × 每 rank 输入（SimNpu::GetSlice 严格按块大小校验，
        // 输出不足会在模板 PostLocalCopy 写远端 rank 分片时越界失败）
        layout.outputBytes = layout.inputBytes * rankSize;
        return layout;
    }

    // golden 期望输出；返回空 vector 表示跳过逐字节比对（零长度场景天然为空）
    virtual std::vector<uint8_t> BuildExpectedOutput(
        uint32_t rankSize, const AicpuStScenario& scenario, RankId rank) = 0;

    // ===== 框架提供的静态工具 =====

    static uint32_t CountRanks(const TopoMeta& topoMeta);

    static Mc2CcTilingInner BuildTiling(HcclCMDType opType, HcclDataType dataType, const std::string& algConfig);

private:
    // 单 rank 全链路实际执行体（成员函数以访问 protected 虚函数/静态工具）
    static void RunRankInner(
        AicpuStFixture* fixture, const AicpuStScenario& scenario, uint32_t rank, uint32_t rankSize,
        const RankMemLayout& layout, RankResult& result);

    // RunRankInner 的异常保护包装：rank 线程内未捕获异常会 std::terminate 崩掉整个测试进程，
    // 此处将异常转为 result.error 优雅报告（comm 若泄漏由 SimWorld::Deinit 兜底回收）
    static void RunRank(
        AicpuStFixture* fixture, const AicpuStScenario& scenario, uint32_t rank, uint32_t rankSize,
        const RankMemLayout& layout, RankResult& result);
};

} // namespace AicpuSt
} // namespace HcclSim

#endif // AICPU_ST_FIXTURE_H
