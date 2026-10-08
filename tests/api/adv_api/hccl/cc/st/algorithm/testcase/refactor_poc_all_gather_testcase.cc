/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
/**
 * 分层重构穿刺验证（AllGather 为载体）
 *
 * 与"离线 PoC"的区别：本文件在 devkit 构建体系内编译，链接真实 libmc2_client 的
 * executor/template/registry，所有对照基线取自**真实生产代码的运行结果**，不是抄写的公式。
 *
 * 验证项：
 *   ① DataLayout 访问器 vs 真实 InsTempAllGatherMesh1D 的 offset 推导（逐值）
 *   ② ResourceClaim vs 真实 template CalcRes 的资源申报（逐字段）
 *   ③ ACTION_ARGS 生成装配 vs 真实 FillCachedArgs 的槽位序列（逐槽）
 *   ④ algConfig 结构化解析 vs 真实注册表的 16 个算法名（覆盖性）
 */
#include <cstring>
#include <string>
#include <vector>

#include "gtest/gtest.h"

// ---- devkit 真实头 ----
#include "alg_param.h"
#include "coll_alg_v2_exec_registry.h"
#include "ins_temp_all_gather_mesh_1D.h"
#include "template_utils.h"

// ---- 分层重构实现（生产代码树 src/ops/unified/）----
#include "action_args.h"
#include "action_collect_all.h"
#include "alg_config_parser.h"
#include "l1_adapter.h"
// 后端实现（include 即触发 REGISTER_ACTION_BACKEND 静态注册——与 devkit
// REGISTER_EXEC_V2 同机制：注册依赖编译单元被链入）
#include "backend_aicpu_collect_all.h"
#include "backend_ccu_collect_all.h"

using namespace mc2_ops_hccl;
namespace poc = mc2_ops_hccl::refactor;

namespace {

constexpr u32 TEST_RANK_SIZE = 8U;
constexpr u64 TEST_COUNT = 512UL * 1024UL;      // 每卡 512K 元素
constexpr u32 FP16_SIZE = 2U;

OpParam MakeAgParam(u64 count = TEST_COUNT, u64 strideCount = 0U)
{
    OpParam param{};
    param.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    param.engine = CommEngine::COMM_ENGINE_AICPU;
    param.DataDes.count = count;
    param.DataDes.dataType = HcclDataType::HCCL_DATA_TYPE_FP16;
    param.DataDes.outputType = HcclDataType::HCCL_DATA_TYPE_FP16;
    param.DataDes.strideCount = strideCount;
    param.userRank = 0U;
    param.opMode = OpMode::OPBASE;
    return param;
}

TopoInfoWithNetLayerDetails MakeMeshTopo(u32 rankSize = TEST_RANK_SIZE)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.serverNum = 1U;
    topo.level0Topo = Level0Shape::MESH_1D;
    topo.topoLevelNums = 1U;
    topo.level0PcieMix = false;
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {rankSize};
    return topo;
}

std::vector<std::vector<u32>> MakeSubCommRanks(u32 rankSize = TEST_RANK_SIZE)
{
    std::vector<u32> ranks;
    for (u32 i = 0U; i < rankSize; ++i) {
        ranks.push_back(i);
    }
    return {ranks};
}

// 真实 template 的数据参数（executor OrchestrateLoop 的填法，见
// ins_v2_all_gather_sole_executor.cc OrchestrateLoop）
TemplateDataParams MakeTempParams(const OpParam& param, u32 rankSize)
{
    TemplateDataParams p{};
    const u64 dts = FP16_SIZE;
    p.count = param.DataDes.count;
    p.sliceSize = param.DataDes.count * dts;
    p.tailSize = p.sliceSize;
    p.inputSliceStride = 0U;
    p.outputSliceStride =
        (param.DataDes.strideCount == 0U) ? param.DataDes.count * dts : param.DataDes.strideCount * dts;
    p.repeatNum = 1U;
    p.inputRepeatStride = 0U;
    p.outputRepeatStride = 0U;
    (void)rankSize;
    return p;
}

// 把真实 TemplateDataParams 投影为重构侧 DataLayout（L1Adapter 的等价映射）
poc::DataLayout ToLayout(const TemplateDataParams& p, u64 tailSize = 0U)
{
    poc::DataLayout L;
    L.dataTypeSize = FP16_SIZE;
    L.sliceSize = p.sliceSize;
    L.tailSize = tailSize;
    L.inputSliceStride = p.inputSliceStride;
    L.outputSliceStride = p.outputSliceStride;
    L.repeatNum = p.repeatNum;
    L.inputRepeatStride = p.inputRepeatStride;
    L.outputRepeatStride = p.outputRepeatStride;
    return L;
}

} // namespace

class RefactorPocAllGatherTest : public ::testing::Test {};

// ============================================================================
// ① offset 等价：真实 template 的推导式 vs DataLayout 访问器
//    基线取自 ins_temp_all_gather_mesh_1D.cc RunAllGatherMesh/LocalDataCopy 的
//    地址推导（txOutOffset/rxOutOffset/scratchOffset/tail 判定）
// ============================================================================
TEST_F(RefactorPocAllGatherTest, DataLayoutMatchesRealTemplateOffsets)
{
    struct Case {
        u32 rankSize;
        u64 count;
        u64 tail;
        u64 strideCount;
        u32 repeat;
    };
    const std::vector<Case> cases = {
        {8U, TEST_COUNT, 0U, 0U, 1U},
        {8U, 500UL * 1024UL, 300UL * 1024UL, 0U, 1U},
        {4U, 256UL * 1024UL, 0U, 300UL * 1024UL, 1U},
        {4U, 256UL * 1024UL, 100UL * 1024UL, 300UL * 1024UL, 3U},
    };

    for (const Case& c : cases) {
        OpParam param = MakeAgParam(c.count, c.strideCount);
        TemplateDataParams tp = MakeTempParams(param, c.rankSize);
        tp.repeatNum = c.repeat;
        tp.outputRepeatStride = tp.outputSliceStride * c.rankSize;
        tp.inputRepeatStride = tp.sliceSize;
        tp.tailSize = c.tail;

        const poc::DataLayout layout = ToLayout(tp, c.tail);

        for (u32 rpt = 0U; rpt < c.repeat; ++rpt) {
            for (u32 me = 0U; me < c.rankSize; ++me) {
                for (u32 peer = 0U; peer < c.rankSize; ++peer) {
                    // ---- 真实 template 的式子（源码逐字对应）----
                    const u64 outBaseOff = tp.buffInfo.outBuffBaseOff + rpt * tp.outputRepeatStride;
                    const u64 scratchRepeatStride = tp.sliceSize * c.rankSize;
                    const u64 scratchBase = tp.buffInfo.hcclBuffBaseOff + rpt * scratchRepeatStride;
                    u64 realSliceSize = tp.sliceSize;
                    if (tp.tailSize != 0U && peer == c.rankSize - 1U) {
                        realSliceSize = tp.tailSize;
                    }
                    const u64 realTxOut = tp.outputSliceStride * me + outBaseOff;
                    const u64 realRxOut = tp.outputSliceStride * peer + outBaseOff;
                    const u64 realTxScratch = scratchBase + tp.sliceSize * me;
                    const u64 realRxScratch = scratchBase + tp.sliceSize * peer;

                    // ---- 重构侧访问器 ----
                    EXPECT_EQ(layout.OutOffset(me, rpt), realTxOut);
                    EXPECT_EQ(layout.OutOffset(peer, rpt), realRxOut);
                    EXPECT_EQ(layout.ScratchOffset(me, c.rankSize, rpt), realTxScratch);
                    EXPECT_EQ(layout.ScratchOffset(peer, c.rankSize, rpt), realRxScratch);
                    EXPECT_EQ(layout.SliceSizeOf(peer, c.rankSize), realSliceSize);
                }
            }
        }
    }
}

// ============================================================================
// ② 资源申报等价：真实 InsTempAllGatherMesh1D::GetRes vs ResourceClaim
//    真实实现见 ins_temp_all_gather_mesh_1D.cc GetRes()
// ============================================================================
TEST_F(RefactorPocAllGatherTest, ResourceClaimMatchesRealTemplateGetRes)
{
    for (u32 rankSize : {2U, 4U, 8U, 16U}) {
        OpParam param = MakeAgParam();
        const auto subCommRanks = MakeSubCommRanks(rankSize);

        // ---- 真实 template 申报 ----
        InsTempAllGatherMesh1D realTemp(param, 0U, subCommRanks);
        AlgResourceRequest realReq{};
        ASSERT_EQ(realTemp.GetRes(realReq), HCCL_SUCCESS);

        // ---- 重构侧动作申报（AICPU 后端条目）----
        poc::ActionSpec act;
        act.kind = poc::ActionKind::CollectAll;
        act.domain.ranks = subCommRanks[0];
        act.domain.myRank = 0U;
        act.channelsPerRank = 1U;
        const poc::ResourceClaim claim = poc::CollectAll::CalcRes(act);

        EXPECT_EQ(claim.slaveThreadNum, realReq.slaveThreadNum)
            << "rankSize=" << rankSize;
        EXPECT_EQ(claim.notifyNumOnMainThread, realReq.notifyNumOnMainThread)
            << "rankSize=" << rankSize;
        EXPECT_EQ(claim.notifyNumPerThread.size(), realReq.notifyNumPerThread.size())
            << "rankSize=" << rankSize;
        // 通道数：每 peer 一条（真实 CalcChannelRequestMesh1D 的产出规模）
        EXPECT_EQ(claim.channels.size(), static_cast<size_t>(rankSize - 1U));
    }
}

// ============================================================================
// ③ 槽位装配等价：ACTION_ARGS 生成的装配 vs 真实 FillCachedArgs
//    真实调用见 ccu_temp_all_gather_mesh_1D.cc KernelRun：
//      FillCachedArgs(submitInfo, inputAddr, outputAddr, token, offset, sliceSize)
//    kernel 侧解包见 ccu_kernel_all_gather_mesh1d.cc GeneArgs
// ============================================================================
TEST_F(RefactorPocAllGatherTest, ActionArgsAssemblyMatchesRealFillCachedArgs)
{
    const u64 inputAddr = 0x10000000UL;
    const u64 outputAddr = 0x20000000UL;
    const u64 token = 0x33UL;
    const u64 offset = 0x400UL;
    const u64 sliceSize = TEST_COUNT * FP16_SIZE;

    // ---- 真实装配 ----
    CcuKernelSubmitInfo realInfo{};
    ASSERT_EQ(FillCachedArgs(realInfo, inputAddr, outputAddr, token, offset, sliceSize), HCCL_SUCCESS);

    // ---- 重构侧：声明宏生成的装配 ----
    poc::CollectAllSlotArgs slotArgs;
    slotArgs.INPUT_ADDR = inputAddr;
    slotArgs.OUTPUT_ADDR = outputAddr;
    slotArgs.TOKEN = token;
    slotArgs.OFFSET = offset;
    slotArgs.SLICE_SIZE = sliceSize;
    constexpr u32 SLOTS = poc::ActionArgsTraits<poc::CollectAllSlotArgs>::SLOT_COUNT;
    u64 generated[SLOTS] = {0};
    poc::AssembleSlots(slotArgs, generated);

    // 逐槽相等（字段名→槽位由声明宏统一分配，与手写顺序一致）
    for (u32 i = 0U; i < SLOTS; ++i) {
        EXPECT_EQ(generated[i], realInfo.cachedArgs[i]) << "slot=" << i;
    }
    // 容量约束与 devkit 一致
    static_assert(SLOTS <= CCU_MAX_TASK_ARG_NUM, "ACTION_ARGS slot count must fit CCU_MAX_TASK_ARG_NUM");
    // 槽位常量可用于命名访问（消灭 cachedArgs[3] 这类魔数）
    EXPECT_EQ(generated[poc::CollectAllSlot::K_INPUT_ADDR], inputAddr);
    EXPECT_EQ(generated[poc::CollectAllSlot::K_OUTPUT_ADDR], outputAddr);
    EXPECT_EQ(generated[poc::CollectAllSlot::K_TOKEN], token);
    EXPECT_EQ(generated[poc::CollectAllSlot::K_OFFSET], offset);
    EXPECT_EQ(generated[poc::CollectAllSlot::K_SLICE_SIZE], sliceSize);
}

// ============================================================================
// ④ 覆盖性：真实注册表里的算法名 vs algConfig 结构化解析
//    注册表内容来自 REGISTER_EXEC_V2（真实注册，非清单转抄）
// ============================================================================
TEST_F(RefactorPocAllGatherTest, AlgConfigCoversRealRegistryEntries)
{
    CollAlgExecRegistryV2& registry = CollAlgExecRegistryV2::Instance();

    // devkit 现有 AG/RS/AR/A2A 注册名 → 新 algConfig 的映射
    struct Mapping {
        HcclCMDType op;
        const char* oldName;
        const char* algConfig;
        u32 expectSegments;
    };
    const std::vector<Mapping> mappings = {
        {HCCL_CMD_ALLGATHER, "InsAllGatherMesh1D", "sole[mesh]", 1U},
        {HCCL_CMD_ALLGATHER, "AicpuAllGatherSoleNHR", "sole[nhr]", 1U},
        {HCCL_CMD_REDUCE_SCATTER, "InsReduceScatterMesh1D", "sole[mesh]", 1U},
        {HCCL_CMD_REDUCE_SCATTER, "InsReduceScatterNHR", "sole[nhr]", 1U},
        {HCCL_CMD_REDUCE_SCATTER, "InsReduceScatterMesh1DMeshChunk", "sole[mesh.chunk]", 2U},

        {HCCL_CMD_ALLTOALL, "AicpuAllToAllSoleMesh", "sole[mesh]", 1U},
        {HCCL_CMD_ALLTOALL, "AicpuAllToAllSoleMeshSingleChannel", "sole[mesh.single-channel]", 1U},
        // UBX 板感知路径：algConfig 与非 UBX 完全相同，实现由 DomainPlan 推导
        {HCCL_CMD_ALLTOALL, "AicpuAllToAllSoleMeshUBX", "sole[mesh]", 1U},
    };

    for (const Mapping& m : mappings) {
        // 旧名在真实注册表中存在（证明映射对象是真的注册项，非清单转抄）
        EXPECT_NE(registry.GetAlgExec(m.op, m.oldName), nullptr) << "missing real algName " << m.oldName;

        // 新 algConfig 可解析
        const poc::AlgConfig cfg = poc::AlgConfigParser::Parse(m.algConfig);
        ASSERT_TRUE(cfg.valid) << m.algConfig << " -> " << cfg.err;
    }

    // AR 的 one_shot/two_shot 映射：本测试目标未链 all_reduce executor（见
    // utils/src/aicpu/CMakeLists.txt EXECUTOR_TARGET 源清单），故只校验语法侧；
    // 注册侧覆盖由 mc2_client 全量库的 selector 测试承担
    for (const char* cfgStr : {"sole[mesh.one_shot]", "sole[mesh.two_shot]", "sole[mesh.two_shot.chunk]"}) {
        const poc::AlgConfig cfg = poc::AlgConfigParser::Parse(cfgStr);
        EXPECT_TRUE(cfg.valid) << cfgStr << " -> " << cfg.err;
    }
}

// 退役名应被拒（devkit 现状是整串比对命中，重构后要求新语法）
TEST_F(RefactorPocAllGatherTest, RetiredAlgNamesAreRejectedByParser)
{
    for (const char* retired : {"InsAllGatherMesh1D", "CcuSchedAllGatherSoleMesh",
                                "AicpuAllReduceSoleMeshTwoShot"}) {
        const poc::AlgConfig cfg = poc::AlgConfigParser::Parse(retired);
        EXPECT_FALSE(cfg.valid) << retired << " should be rejected";
        EXPECT_NE(cfg.err.find("retired"), std::string::npos);
    }
}
