/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// ═══════════════════════════════════════════════════════════════════════════════════════════════════
// AICPU ST 目录结构（utils 三库 + 用例层，依赖方向 mc2_aicpu → utils 单向）：
//   utils/aicpu_scenario/  编排库（本目录）：场景配置 + VerifyScenario 全链装配与四层校验调度
//                          ↔ CCU 侧 ccu_scenario/
//   utils/aicpu_executor/  执行引擎库：TaskGraphExecutor（任务图真执行/notify 同步/死锁定位）
//                          + aicpu_float_convert（数值转换，golden 与执行器共用同一舍入策略）
//                          ↔ CCU 侧 hccl_proxy/ccu_primitives_stub（录制）+ sim_world/ccu_sim_world（回放）
//   utils/aicpu_verifier/  校验库：AicpuDagChecker（happens-before + 区间重叠内存冲突检测）
//                          ↔ CCU 侧 hccl_verifier/ccu_checker*（CCU 塞旧目录，AICPU 独立新目录
//                            以避免与旧语义校验体系纠缠）
//   testcase/mc2_aicpu/    用例与定制层：framework/（框架自测，测上述三库）+ <原语>/（原语定制，
//                          原语级类实现本文件的虚函数契约）——新增算法变体只改用例层，零改 utils
// ═══════════════════════════════════════════════════════════════════════════════════════════════════
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

    // 场景能力守卫（纯判定）：reduceLike=true（原语声明为归约系）且 64bit/PROD 时拒绝并给出
    // 人读解释；可测返回 true。public：框架自测（独立 TEST）直接访问，先例同 MakeInputByte。
    // isReduceLikeOp 由 VerifyScenario 以 this->IsReduceLikeOp() 桥接（原语声明见下方虚函数）。
    // 判定条件与产品 LocalReduce wrapper 的直算转发条件对齐（alg_data_trans_wrapper.cc:738-744），
    // 详见 aicpu_fixture.cc 实现处的边界注释。
    static bool CheckScenarioSupported(const AicpuStScenario& scenario, bool isReduceLikeOp, std::string& reason);

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

    // golden 期望输出；按 actualAlgName（selector 实际选中）选择算法变体——与
    // scenario.expectedAlgName 解耦（后者仅为算法选择断言字段，空=跳过断言，不影响 golden 选择）。
    // 返回空 vector 表示跳过逐字节比对（零长度场景天然为空）。
    // 非零长度场景返回空属于 adapter bug——默认会 FAIL（防假绿）；链路级冒烟等
    // 刻意不比对的场景应 override ExpectOutputCheck 返回 false 显式豁免。
    virtual std::vector<uint8_t> BuildExpectedOutput(
        uint32_t rankSize, const AicpuStScenario& scenario, RankId rank, const std::string& actualAlgName) = 0;

    // 非零场景的 golden 空守卫开关（默认开）：BuildExpectedOutput 意外返回空时 FAIL
    virtual bool ExpectOutputCheck() { return true; }

    // 本原语的数据面是否包含元素级归约运算（SUM/PROD/MAX/MIN）——原语固有属性（opType 决定），
    // 与场景无关。归约系原语（ReduceScatter/AllReduce/Reduce 等）数据面会调 LocalReduce，
    // 存在产品直算路径（64bit/PROD 场景被 CheckScenarioSupported 拒绝，见上方注释）；
    // 纯搬运原语（AllGather/AllToAll 等）永不调归约、无直算路径，保持默认 false。
    // 覆盖式声明而非框架层 opType 清单：接入新原语必然实现本类（虚函数契约），声明漏不了——
    // 清单式实现会随原语演化静默漂移（漏登 → 守卫失效 → 64bit/PROD 场景 SIGSEGV 崩进程）。
    virtual bool IsReduceLikeOp() const { return false; }

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
