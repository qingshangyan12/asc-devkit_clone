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
    // 输入数据模式（检测力设计）：
    // - 每 256B 块的前 254 字节为数据字节（17 + rank*41 + offset，周期 256）：检测一切非 256 倍数位移；
    // - 末 2 字节（inBlock 0xFE/0xFF）为块标签：0xFE 位打包（高 4 位 blockIdx 低 4 位 | 低 4 位 rank），
    //   0xFF 为 blockIdx 位 4-11；在 (rank < 16, blockIdx < 4096) 内双射——覆盖 1MB 分片，
    //   检测 256 对齐块置换（含 32KB chunk 乱序）；
    // - 残余盲区：>1MB 分片的 1MB 对齐置换（当前用例上限 1MB，为零盲区；需更大覆盖时增加标签位数）。
    // fill 与 golden 同源（共用本函数），比对自洽。
    // public：adapter 的 golden 构造器（非成员函数）需与 fixture 填充共用同一公式。
    static uint8_t MakeInputByte(RankId rank, uint64_t offset)
    {
        const uint64_t inBlock = offset & 0xFFU;
        if (inBlock >= 0xFEU) {
            const uint64_t blockIdx = offset >> 8;
            if (inBlock == 0xFEU) {
                return static_cast<uint8_t>(((blockIdx & 0x0FU) << 4) | (rank & 0x0FU));
            }
            return static_cast<uint8_t>((blockIdx >> 4) & 0xFFU);
        }
        return static_cast<uint8_t>(17U + rank * 41U + offset);
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
