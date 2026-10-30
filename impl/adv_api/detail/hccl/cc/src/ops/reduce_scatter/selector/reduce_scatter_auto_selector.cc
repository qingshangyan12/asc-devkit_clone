/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "reduce_scatter_auto_selector.h"
#include "selector_registry.h"

namespace mc2_ops_hccl {
constexpr u32 MAX_RANK_NUM_FOR_CONCURRENT_ALGO = 4;
constexpr u64 RS_AICPU_1D_MAX_DATA_SIZE = 16 * 1024 * 1024;
constexpr u64 RS_AICPU_1D_MIN_DATA_SIZE = 4 * 1024 * 1024;
constexpr u64 RS_AICPU_1D_TWO_LEVEL_DATA_SIZE_THRESHOLD = 1536 * 1024 * 1024;
constexpr u64 RS_AICPU_SEQUENCE_SIZE_THRESHOLD = 4ULL * 1024 * 1024 * 1024;
constexpr u64 RS_2P_DETOUR_DATA_SIZE = 4 * 1024 * 1024;
constexpr u64 OMNI_PCIE_RS_DATA_SIZE = 4 * 1024 * 1024;
constexpr u64 OMNI_UBX_RS_LARGE_SIZE = 16 * 1024 * 1024;
constexpr u32 DEVICE_NUM_PER_MODULE_8 = 8;
// hccl 定义于 alg_param.h；devkit 按既有模式（topo_match_*.cc 局部 EXPECTED_TOPO_LEVEL_NUM_2）在文件内定义
constexpr u32 TOPO_LEVEL_NUM_2 = 2;
constexpr u32 TOPO_LEVEL_NUM_3 = 3;
constexpr u32 MIN_STRICT_RANK_NUM_ORDER_PRESERVED = 2;
// CCU_SCHED 决策树常量（对齐 hccl）
constexpr u64 RS_FLATTEN_MAX_DATA_SIZE = 512 * 1024;
constexpr u64 RS_CCU_64P_MIN_DATA_SIZE = 128 * 1024 * 1024;
constexpr u64 RS_CCU_64P_SEQ_DATA_SIZE = 16 * 1024 * 1024;
constexpr u32 RS_CCU_MAX_RANK_SIZE = 64;
constexpr u32 RS_CCU_2DIE_RANK_SIZE = 16;
constexpr u32 RS_CCU_2DIE_FRAME_NUM = 2;
constexpr u64 RS_CCU_2DIE_MIN_DATA_SIZE = 4 * 1024 * 1024;
constexpr u64 RS_CCU_2DIE_MAX_DATA_SIZE = 16 * 1024 * 1024;
constexpr u64 OMNI_UBX_RS_SCHED_DATA_SIZE = 4 * 1024 * 1024;
constexpr u64 CCU_SCHEDULE_2LEVEL_MAX_PER_RANK_DATA_SIZE = 32ULL * 1024 * 1024;

// hccl order_preserved_common.h::IsNeedStrictModeForOrderPreserved 的 devkit 适配版：
// 确定性级别仅取 HCCL_DETERMINISTIC 环境变量（devkit 无 HCOMM comm 级 dlsym 查询层）
static bool IsNeedStrictModeForOrderPreserved(const OpParam& opParam, u32 rankSize)
{
    u8 deterministicLevel = GetExternalInputHcclDeterministic();
    HcclDataType dataType = opParam.DataDes.dataType;
    HcclReduceOp reduceType = opParam.reduceType;
    return (deterministicLevel == static_cast<u8>(DeterministicEnableLevel::DETERMINISTIC_STRICT)) &&
           (dataType == HcclDataType::HCCL_DATA_TYPE_FP16 || dataType == HcclDataType::HCCL_DATA_TYPE_FP32 ||
            dataType == HcclDataType::HCCL_DATA_TYPE_BFP16 || dataType == HcclDataType::HCCL_DATA_TYPE_FP64) &&
           (reduceType == HcclReduceOp::HCCL_REDUCE_SUM || reduceType == HcclReduceOp::HCCL_REDUCE_PROD) &&
           rankSize > MIN_STRICT_RANK_NUM_ORDER_PRESERVED;
}

// hccl 选路命中但 devkit 未迁移的算法：按迁移约定直接报错不匹配，不做本地替代兜底
static SelectorStatus AlgoNotMigrated(const char* hcclAlgName, const char* cond)
{
    HCCL_ERROR(
        "[ReduceScatterAutoSelector] hccl selects [%s] (%s), which is not migrated in asc-devkit, "
        "return NOT_MATCH.",
        hcclAlgName, cond);
    return SelectorStatus::NOT_MATCH;
}

SelectorStatus ReduceScatterAutoSelector::SelectCcuMsAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[ReduceScatterAutoSelector][%s] start, topoInfo levelNum[%u]", __func__, topoInfo->topoLevelNums);
    (void)configAlgMap;
    if (topoInfo->topoLevelNums > 1) {
        HCCL_WARNING("[ReduceScatterAutoSelector] layerNum > 1 is not supported yet for ccu_ms mode.");
        return SelectorStatus::NOT_MATCH;
    }
    // MS 模式不支持 int8
    CHK_PRT_RET(
        opParam.DataDes.dataType == HcclDataType::HCCL_DATA_TYPE_INT8,
        HCCL_WARNING(
            "[ReduceScatterAutoSelector] dataType[%d] is not supported yet for ccu_ms mode.", opParam.DataDes.dataType),
        SelectorStatus::NOT_MATCH);

    // MS 模式不支持 PROD
    CHK_PRT_RET(
        opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD,
        HCCL_WARNING(
            "[ReduceScatterAutoSelector] ReduceOp[%d] is not supported yet for ccu_ms mode.", opParam.reduceType),
        SelectorStatus::NOT_MATCH);

    if (Is64BitDataType(opParam.DataDes.dataType)) {
        HCCL_WARNING("[ReduceScatterAutoSelector] ccu_ms mode not support INT64, UINT64, FP64.");
        return SelectorStatus::NOT_MATCH;
    }

    SelectorStatus ret = SelectMeshAlgoCcums(topoInfo, opParam, selectAlgName);
    if (ret == SelectorStatus::MATCH) {
        HCCL_INFO("[ReduceScatterAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
    }
    return ret;
}

SelectorStatus ReduceScatterAutoSelector::SelectMeshAlgoCcums(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, std::string& selectAlgName) const
{
    u64 perDataSize = DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    u64 dataSize = opParam.DataDes.count * perDataSize;
    if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
        if (topoInfo->level0MeshType == Level0MeshType::TWO_DIE_REGULAR) {
            selectAlgName = "CcuReduceScatterMesh2Die";
        } else if (topoInfo->level0MeshType == Level0MeshType::TWO_DIE_NOT_REGULAR) {
            HCCL_INFO("[%s] TWO_DIE_NOT_REGULAR not match", __func__);
            return SelectorStatus::NOT_MATCH;
        } else {
            selectAlgName = "CcuReduceScatterMesh1D";
        }
    } else if (topoInfo->level0Topo == Level0Shape::MESH_1D_CLOS) {
        // UBX机型
        bool isMeshNumEqualToClosNum = false;
        bool isClosNumMultipleOfMeshNum = false;
        CHK_PRT_RET(
            CheckMeshNumEqualToClosNum(topoInfo, isMeshNumEqualToClosNum) != HCCL_SUCCESS,
            HCCL_ERROR("[ReduceScatterAutoSelector] CheckMeshNumEqualToClosNum failed."), SelectorStatus::NOT_MATCH);
        CHK_PRT_RET(
            CheckClosNumMultipleOfMeshNum(topoInfo, isClosNumMultipleOfMeshNum) != HCCL_SUCCESS,
            HCCL_ERROR("[ReduceScatterAutoSelector] CheckClosNumMultipleOfMeshNum failed."), SelectorStatus::NOT_MATCH);
        if (isMeshNumEqualToClosNum && topoInfo->userRankSize <= MAX_RANK_NUM_FOR_CONCURRENT_ALGO) {
            // 4P mesh
            if (IsSmallData(dataSize)) {
                // 小数据量，用1d mesh算法
                selectAlgName = "CcuReduceScatterMesh1D";
            } else {
                // 大数据量，用mesh+clos并行算法
                selectAlgName = "CcuReduceScatterConcurrentMeshNHRMs";
            }
        } else if (isClosNumMultipleOfMeshNum && !IsSmallData(dataSize)) {
            HCCL_WARNING("[%s] MESH_1D_CLOS not match.", __func__);
            return SelectorStatus::NOT_MATCH;
        } else {
            selectAlgName = "CcuReduceScatterMesh1D";
        }
    } else {
        HCCL_WARNING(
            "[ReduceScatterAutoSelector] level0Topo[%d] is not supported yet for ccu_ms mode.", topoInfo->level0Topo);
        return SelectorStatus::NOT_MATCH;
    }
    HCCL_INFO("[ReduceScatterAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

// 与 hccl SelectCcuScheduleAlgo 对齐：两级拓扑（Level1Nhr/level0 单实例/CLOS）走 SoleNHR，
// level0 多实例按总数据量分流（小数据 SoleMesh）；hccl 命中但 devkit 未迁移的算法按迁移约定直接报错。
// MC2 注册期无 count（恒 0），数据量条件恒落小数据侧；ST 场景注入真实 count 可覆盖其余分支。
SelectorStatus ReduceScatterAutoSelector::SelectCcuScheduleAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[ReduceScatterAutoSelector][%s] start, topoInfo levelNum[%u]", __func__, topoInfo->topoLevelNums);
    (void)configAlgMap;
    if (topoInfo->level2UbRtp) {
        HCCL_INFO(
            "[ReduceScatterAutoSelector][%s] ccu schedule is not supported with level2UbRtp, reset to default.",
            __func__);
        return SelectorStatus::NOT_MATCH;
    }
    if (topoInfo->topoLevelNums >= TOPO_LEVEL_NUM_3) {
        HCCL_INFO(
            "[ReduceScatterAutoSelector][%s] ccu schedule is not supported when topoLevelNums >= 3(levelNum[%u]), "
            "reset to default.",
            __func__, topoInfo->topoLevelNums);
        return SelectorStatus::NOT_MATCH;
    }

    // 保序模式不支持CCU_SCHED
    CHK_PRT_RET(
        IsNeedStrictModeForOrderPreserved(opParam, topoInfo->userRankSize),
        HCCL_DEBUG(
            "[ReduceScatterAutoSelector] DETERMINISTIC_STRICT mode not supported for CCU_SCHED, reset to default."),
        SelectorStatus::NOT_MATCH);

    // ccu 模式不支持 PROD
    CHK_PRT_RET(
        opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD,
        HCCL_WARNING(
            "[ReduceScatterAutoSelector] ReduceOp[%d] is not supported yet for ccu schedule mode.", opParam.reduceType),
        SelectorStatus::NOT_MATCH);

    // ccu 模式不支持 in-place 场景
    CHK_PRT_RET(
        IsInputOutputOverlap(opParam) == true,
        HCCL_WARNING("[ReduceScatterAutoSelector] ccu schedule mode does not support in-place."),
        SelectorStatus::NOT_MATCH);

    if (Is64BitDataType(opParam.DataDes.dataType)) {
        HCCL_WARNING("[ReduceScatterAutoSelector] ccu_schedule mode does not support INT64, UINT64, FP64.");
        return SelectorStatus::NOT_MATCH;
    }

    u64 perDataSize = DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    u64 dataSize = opParam.DataDes.count * perDataSize;
    const u32 ccuSize = RS_CCU_MAX_RANK_SIZE;
    const u32 frameNum = AutoSelectorBase::CalcFrameNum(topoInfo);

    if (topoInfo->topoLevelNums > 1) {
        if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
            if (dataSize >= CCU_SCHEDULE_2LEVEL_MAX_PER_RANK_DATA_SIZE) {
                HCCL_INFO(
                    "[ReduceScatterAutoSelector] 2 level topo perRankDataSize[%llu] exceeds limit[%llu], reset to "
                    "default.",
                    static_cast<unsigned long long>(dataSize),
                    static_cast<unsigned long long>(CCU_SCHEDULE_2LEVEL_MAX_PER_RANK_DATA_SIZE));
                return SelectorStatus::NOT_MATCH;
            }
            // Level1Nhr 已在 CalcTopoShape 中设置（GCD==1 时为 true）
            if (topoInfo->Level1Nhr) {
                selectAlgName = "CcuSchedReduceScatterSoleNHR";
                HCCL_INFO("[ReduceScatterAutoSelector] Level1Nhr=true, select [%s]", selectAlgName.c_str());
                return SelectorStatus::MATCH;
            }
            if (topoInfo->is2DieFullMesh) {
                HCCL_DEBUG("[ReduceScatterAutoSelector] 2DieFullMesh is not supported yet for ccu schedule mode.");
                return SelectorStatus::NOT_MATCH;
            }
            if (topoInfo->netLayerDetails.localNetInsSizeOfLayer.at(0) > 1) {
                CHK_PRT_RET(
                    opParam.DataDes.dataType == HcclDataType::HCCL_DATA_TYPE_INT8,
                    HCCL_WARNING(
                        "[ReduceScatterAutoSelector] dataType[%d] is not supported yet for ccu schedule mode.",
                        opParam.DataDes.dataType),
                    SelectorStatus::NOT_MATCH);
                // 2*8p 组网且总数据量在 [4M, 16M] 时选择 2Die mem2mem 算法（仅支持 2 框 8 卡组网）
                if (topoInfo->userRankSize == RS_CCU_2DIE_RANK_SIZE && frameNum == RS_CCU_2DIE_FRAME_NUM &&
                    dataSize * topoInfo->userRankSize >= RS_CCU_2DIE_MIN_DATA_SIZE &&
                    dataSize * topoInfo->userRankSize <= RS_CCU_2DIE_MAX_DATA_SIZE) {
                    return AlgoNotMigrated("CcuSchedReduceScatterSoleMesh2Die", "2*8p two-level [4M,16M]");
                } else if (
                    (dataSize * topoInfo->userRankSize) < RS_FLATTEN_MAX_DATA_SIZE &&
                    topoInfo->userRankSize < ccuSize) {
                    selectAlgName = "CcuSchedReduceScatterSoleMesh";
                    return SelectorStatus::MATCH;
                } else if (
                    dataSize * topoInfo->userRankSize < RS_CCU_64P_SEQ_DATA_SIZE && topoInfo->userRankSize < ccuSize &&
                    frameNum <= MAX_FRAME_NUM_FOR_CCU_ALGO) {
                    return AlgoNotMigrated("CcuSchedReduceScatterSequenceMeshMesh", "multi-frame <64P <16M");
                } else if (
                    dataSize * topoInfo->userRankSize <= RS_CCU_64P_SEQ_DATA_SIZE &&
                    topoInfo->userRankSize == ccuSize && frameNum <= MAX_FRAME_NUM_FOR_CCU_ALGO) {
                    return AlgoNotMigrated("CcuSchedReduceScatterSequenceMeshMesh", "multi-frame 64P <=16M");
                } else if (
                    dataSize * topoInfo->userRankSize <= RS_CCU_64P_MIN_DATA_SIZE &&
                    topoInfo->userRankSize == ccuSize && frameNum <= MAX_FRAME_NUM_FOR_CCU_ALGO) {
                    return AlgoNotMigrated("CcuSchedReduceScatterParallelMeshNHR", "multi-frame 64P <=128M");
                } else if (frameNum > MAX_FRAME_NUM_FOR_CCU_ALGO) {
                    // 框数超过 kernel repeatNum 上限，fallback 到 NHR1DMem2Mem
                    HCCL_INFO(
                        "[ReduceScatterAutoSelector] frameNum[%u] > %u, fallback to NHR1DMem2Mem.", frameNum,
                        MAX_FRAME_NUM_FOR_CCU_ALGO);
                    selectAlgName = "CcuSchedReduceScatterSoleNHR";
                    return SelectorStatus::MATCH;
                } else {
                    HCCL_INFO(
                        "[ReduceScatterAutoSelector] multi-frame mesh dataSize[%llu] has no migrated CCU algo, reset "
                        "to default.",
                        static_cast<unsigned long long>(dataSize));
                    return SelectorStatus::NOT_MATCH;
                }
            } else {
                selectAlgName = "CcuSchedReduceScatterSoleNHR";
                return SelectorStatus::MATCH;
            }
        } else if (topoInfo->level0Topo == Level0Shape::CLOS) {
            selectAlgName = "CcuSchedReduceScatterSoleNHR";
        } else {
            HCCL_WARNING(
                "[ReduceScatterAutoSelector][SelectCcuScheduleAlgo] layer0Shape[%d] is not supported yet for ccu "
                "schedule mode.",
                topoInfo->level0Topo);
            return SelectorStatus::NOT_MATCH;
        }
    } else {
        return SelectMeshAlgoCcuSchedule(topoInfo, opParam, selectAlgName);
    }
    HCCL_INFO("[ReduceScatterAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

// UBX（MESH_1D_CLOS）机型，与 hccl SelectMeshAlgoCcuSchedule 的 UBX 分支对齐：
// 同组 4P（meshNum==closNum 且 rankSize<=4）小数据走 SoleMesh、大数据走 Mesh+NHR 并发（ConcurMeshNHRMultiLink）；
// 矩形大数据 hccl 走 ParallelMeshNHRMultiLink / PipeLineMeshNHR（asc-devkit 未迁移）；
// 其余场景（hccl 的"1d NHR 算法"分支）选 SoleNHRMultiLink。pcieMix 由调用方先行分流。
SelectorStatus ReduceScatterAutoSelector::SelectCcuScheduleUBXAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, std::string& selectAlgName) const
{
    u64 perDataSize = DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    u64 dataSize = opParam.DataDes.count * perDataSize;
    bool isMeshNumEqualToClosNum = false;
    bool isClosNumMultipleOfMeshNum = false;
    CHK_PRT_RET(
        CheckMeshNumEqualToClosNum(topoInfo, isMeshNumEqualToClosNum) != HCCL_SUCCESS,
        HCCL_ERROR("[ReduceScatterAutoSelector] CheckMeshNumEqualToClosNum failed."), SelectorStatus::NOT_MATCH);
    CHK_PRT_RET(
        CheckClosNumMultipleOfMeshNum(topoInfo, isClosNumMultipleOfMeshNum) != HCCL_SUCCESS,
        HCCL_ERROR("[ReduceScatterAutoSelector] CheckClosNumMultipleOfMeshNum failed."), SelectorStatus::NOT_MATCH);
    if (isMeshNumEqualToClosNum && topoInfo->userRankSize <= MAX_RANK_NUM_FOR_CONCURRENT_ALGO) {
        // 同一组 4P mesh：小数据走 1d mesh 算法，大数据走 mesh+clos 并行算法
        if (IsSmallData(dataSize)) {
            selectAlgName = "CcuSchedReduceScatterSoleMesh";
            HCCL_INFO("[ReduceScatterAutoSelector][%s] UBX small-data match [%s]", __func__, selectAlgName.c_str());
            return SelectorStatus::MATCH;
        }
        selectAlgName = "CcuSchedReduceScatterConcurMeshNHRMultiLink";
        HCCL_INFO("[ReduceScatterAutoSelector][%s] UBX concurrent Algo match [%s]", __func__, selectAlgName.c_str());
        return SelectorStatus::MATCH;
    }
    if (isClosNumMultipleOfMeshNum && !IsSmallData(dataSize)) {
        // 矩形场景大数据
        if (dataSize < OMNI_UBX_RS_SCHED_DATA_SIZE) {
            return AlgoNotMigrated("CcuSchedReduceScatterParallelMeshNHRMultiLink", "UBX rectangular [512K,4M)");
        }
        return AlgoNotMigrated("CcuSchedReduceScatterPipeLineMeshNHR", "UBX rectangular >=4M");
    }
    // 其余场景：1d NHR 算法
    selectAlgName = "CcuSchedReduceScatterSoleNHRMultiLink";
    HCCL_INFO("[ReduceScatterAutoSelector][%s] UBX NHR Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

// 单层拓扑分发，与 hccl SelectMeshAlgoCcuSchedule 对齐
SelectorStatus ReduceScatterAutoSelector::SelectMeshAlgoCcuSchedule(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, std::string& selectAlgName) const
{
    // ccu 模式不支持 in-place 场景
    CHK_PRT_RET(
        IsInputOutputOverlap(opParam) == true,
        HCCL_WARNING("[ReduceScatterAutoSelector] ccu schedule mode does not support in-place."),
        SelectorStatus::NOT_MATCH);

    CHK_PRT_RET(
        opParam.DataDes.dataType == HcclDataType::HCCL_DATA_TYPE_INT8,
        HCCL_WARNING(
            "[ReduceScatterAutoSelector] dataType[%d] is "
            "not supported yet for ccu_schedule mode with ms reduce.",
            opParam.DataDes.dataType),
        SelectorStatus::NOT_MATCH);

    if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
        return SelectMeshAlgoCcuScheduleMesh1D(topoInfo, opParam, selectAlgName);
    } else if (topoInfo->level0Topo == Level0Shape::MESH_1D_CLOS) {
        // PCIE-SW定制机型，Mesh无法链接全卡时，需要跨pcie链路，不支持ccu模式
        if (topoInfo->level0PcieMix) {
            if (IsLayerAllConnetedWithTopo(topoInfo, 0, CommTopo::COMM_TOPO_1DMESH)) {
                return SelectMeshAlgoCcuScheduleMesh1D(topoInfo, opParam, selectAlgName);
            }
            HCCL_WARNING("[ReduceScatterAutoSelector] pcie mixed topo is not supported yet for ccu sched mode.");
            return SelectorStatus::NOT_MATCH;
        }
        return SelectCcuScheduleUBXAlgo(topoInfo, opParam, selectAlgName);
    } else if (topoInfo->level0Topo == Level0Shape::CLOS) {
        if (topoInfo->level0PcieMix) {
            HCCL_WARNING("[ReduceScatterAutoSelector] pcie mixed topo is not supported yet for ccu schedule mode.");
            return SelectorStatus::NOT_MATCH;
        }
        selectAlgName = "CcuSchedReduceScatterSoleNHR";
    } else {
        HCCL_DEBUG(
            "[ReduceScatterAutoSelector] level0Topo[%d] is not supported yet for ccu sched mode.",
            topoInfo->level0Topo);
        return SelectorStatus::NOT_MATCH;
    }
    HCCL_DEBUG("[%s] Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

// 单层 MESH_1D 算法选择，与 hccl SelectMeshAlgoCcuScheduleMesh1D 对齐（SoleMesh2Die 未迁移）
SelectorStatus ReduceScatterAutoSelector::SelectMeshAlgoCcuScheduleMesh1D(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, std::string& selectAlgName) const
{
    u64 perDataSize = DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    u64 dataSize = opParam.DataDes.count * perDataSize;
    double ratio; // 以8卡为基线确定ratio，用来表示不同卡数对下发的影响系数
    if (topoInfo->userRankSize == 0) {
        HCCL_WARNING("[ReduceScatterAutoSelector]the selector is not set topoInfo->userRankSize]");
        ratio = 1;
    } else {
        ratio = DEFAULT_RANK_SIZE / topoInfo->userRankSize;
    }
    if (dataSize * ratio >= RS_M2M_1D_MAX_DATA_SIZE) {
        HCCL_DEBUG(
            "[ReduceScatterAutoSelector] dataSize[%lu] * ratio[%f] >= MAX_DATA_SIZE[%lu].", dataSize, ratio,
            RS_M2M_1D_MAX_DATA_SIZE);
        return SelectorStatus::NOT_MATCH;
    }
    if (IsInputOutputOverlap(opParam) == true) { // 不支持 in-place 场景
        HCCL_WARNING("[ReduceScatterAutoSelector] ccu_ms mode does not support in-place.");
        return SelectorStatus::NOT_MATCH;
    }
    if (topoInfo->level0MeshType == Level0MeshType::TWO_DIE_REGULAR) {
        return AlgoNotMigrated("CcuSchedReduceScatterSoleMesh2Die", "1-level TWO_DIE_REGULAR");
    } else if (topoInfo->level0MeshType == Level0MeshType::TWO_DIE_NOT_REGULAR) {
        HCCL_DEBUG("[ReduceScatterAutoSelector] TWO_DIE_NOT_REGULAR not match.");
        return SelectorStatus::NOT_MATCH;
    } else {
        selectAlgName = "CcuSchedReduceScatterSoleMesh";
    }
    HCCL_DEBUG("[ReduceScatterAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus ReduceScatterAutoSelector::SelectAicpuAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_DEBUG("[ReduceScatterAutoSelector][%s] start, topoInfo levelNum[%u]", __func__, topoInfo->topoLevelNums);
    (void)configAlgMap;
    u64 perDataSize = DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    u64 dataSize = opParam.DataDes.count * perDataSize;

    if (IsNeedStrictModeForOrderPreserved(opParam, topoInfo->userRankSize)) {
        // hccl 统一走分组all2all实现（AicpuReduceScatterStrictOrderedMesh），devkit 未迁移
        return AlgoNotMigrated("AicpuReduceScatterStrictOrderedMesh", "DETERMINISTIC_STRICT order preserved");
    }

    if (topoInfo->topoLevelNums > 1) {
        bool level0AndLevel1Symetric = topoInfo->level0Symmetric && topoInfo->level1Symmetric;
        if (Is64BitDataType(opParam.DataDes.dataType) || opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD) {
            selectAlgName = "InsReduceScatterAicpuReduceNHR";
        } else if (
            level0AndLevel1Symetric && topoInfo->deviceNumPerModule == DEVICE_NUM_PER_MODULE_8 &&
            topoInfo->topLevelUboe) {
            if (topoInfo->topoLevelNums == TOPO_LEVEL_NUM_2) {
                return AlgoNotMigrated("AicpuReduceScatterPipeLineMeshNHR", "symmetric 2-level uboe");
            } else {
                return AlgoNotMigrated("AicpuReduceScatterPipeLineMeshNHRMesh", "symmetric 3-level uboe");
            }
        } else if (level0AndLevel1Symetric && topoInfo->topoLevelNums == TOPO_LEVEL_NUM_3 && topoInfo->topLevelUboe) {
            selectAlgName = "InsReduceScatterParallelMesh1DNHR";
        } else if (topoInfo->Level1Nhr) {
            selectAlgName = "InsReduceScatterNHR";
            HCCL_INFO("[ReduceScatterAutoSelector] Level1Nhr=true, select [%s]", selectAlgName.c_str());
        } else if (topoInfo->Level0Nhr) {
            selectAlgName = "InsReduceScatterNHR";
        } else if (
            topoInfo->netLayerDetails.localNetInsSizeOfLayer.at(0) > 1 &&
            topoInfo->level0Topo == Level0Shape::MESH_1D) {
            constexpr u64 AICPU_MAX_RANKSIZE = 256;
            constexpr u64 AICPU_2LEVEL_MAX_TOTAL_DATA_SIZE = 1ULL * 1024 * 1024 * 1024;
            if (topoInfo->topoLevelNums == TOPO_LEVEL_NUM_3) {
                if (level0AndLevel1Symetric) {
                    return AlgoNotMigrated("AicpuReduceScatterSequenceMeshConcurNHRNHR", "3-level symmetric mesh");
                } else {
                    selectAlgName = "InsReduceScatterNHR";
                }
            } else if (
                dataSize * topoInfo->userRankSize >= AICPU_2LEVEL_MAX_TOTAL_DATA_SIZE &&
                topoInfo->userRankSize >= AICPU_MAX_RANKSIZE) {
                selectAlgName = "InsReduceScatterParallelMesh1DNHR";
            } else if (dataSize > RS_AICPU_1D_MIN_DATA_SIZE) {
                selectAlgName = (dataSize * topoInfo->userRankSize > RS_AICPU_SEQUENCE_SIZE_THRESHOLD) ?
                                    "InsReduceScatterSequenceMesh1DNhr" :
                                    "InsReduceScatterParallelMesh1DNHR";
            } else {
                selectAlgName = "InsReduceScatterNHR";
            }
        } else if (topoInfo->netLayerDetails.localNetInsSizeOfLayer.at(0) == 1) {
            selectAlgName = "InsReduceScatterNHR";
        } else if (topoInfo->level0Topo == Level0Shape::CLOS) {
            selectAlgName = "AicpuReduceScatterSoleNHRMultiLink";
        } else {
            HCCL_ERROR(
                "[ReduceScatterAutoSelector] topo not match, level0Topo [%d], deviceNumPerModule [%d]",
                topoInfo->level0Topo, topoInfo->netLayerDetails.localNetInsSizeOfLayer.at(0));
            return SelectorStatus::NOT_MATCH;
        }
    } else {
        return SelectMeshAlgoAicpu(topoInfo, opParam, selectAlgName);
    }

    return SelectorStatus::MATCH;
}

SelectorStatus ReduceScatterAutoSelector::SelectMeshAlgoAicpu(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, std::string& selectAlgName) const
{
    u64 perDataSize = DATATYPE_SIZE_TABLE[opParam.DataDes.dataType];
    u64 dataSize = opParam.DataDes.count * perDataSize;
    double ratio;
    if (topoInfo->userRankSize == 0) {
        HCCL_WARNING("[ReduceScatterAutoSelector]the selector is not set userRankSize]");
        ratio = 1;
    } else {
        ratio = (DEFAULT_RANK_SIZE / topoInfo->userRankSize) * (DEFAULT_RANK_SIZE / topoInfo->userRankSize);
    }
    if (topoInfo->level0Topo == Level0Shape::MESH_1D) {
        return SelectMeshAlgoAicpuForMesh1D(topoInfo, opParam, selectAlgName, dataSize, ratio);
    } else if (topoInfo->level0Topo == Level0Shape::CLOS) {
        if (Is64BitDataType(opParam.DataDes.dataType) || opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD) {
            selectAlgName = "InsReduceScatterAicpuReduceNHR";
        } else {
            selectAlgName = "AicpuReduceScatterSoleNHRMultiLink";
        }
    } else if (topoInfo->level0Topo == Level0Shape::MESH_1D_CLOS) {
        bool isClosNumMultipleOfMeshNum = false;
        CHK_PRT_RET(
            CheckClosNumMultipleOfMeshNum(topoInfo, isClosNumMultipleOfMeshNum) != HCCL_SUCCESS,
            HCCL_ERROR("[ReduceScatterAutoSelector] CheckClosNumMultipleOfMeshNum failed."), SelectorStatus::NOT_MATCH);
        return SelectMeshAlgoAicpuForMesh1DClos(
            topoInfo, opParam, dataSize, ratio, isClosNumMultipleOfMeshNum, selectAlgName);
    } else {
        HCCL_WARNING("[ReduceScatterAutoSelector] topo not match");
        return SelectorStatus::NOT_MATCH;
    }
    HCCL_DEBUG("[%s] Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus ReduceScatterAutoSelector::SelectAivAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    (void)configAlgMap;
    HCCL_DEBUG("[ReduceScatterAutoSelector][%s] start, topoInfo levelNum[%u]", __func__, topoInfo->topoLevelNums);
    // aiv 模式不支持 PROD
    CHK_PRT_RET(
        opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD,
        HCCL_WARNING("[ReduceScatterAutoSelector] ReduceOp[%d] is not supported yet for aiv mode.", opParam.reduceType),
        SelectorStatus::NOT_MATCH);

    if (Is64BitDataType(opParam.DataDes.dataType)) {
        HCCL_WARNING("[ReduceScatterAutoSelector] aiv mode not support INT64, UINT64, FP64.");
        return SelectorStatus::NOT_MATCH;
    }

    selectAlgName = "AivReduceScatterMesh1D";
    HCCL_DEBUG("[ReduceScatterAutoSelector][%s] end, selectAlgName[%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus ReduceScatterAutoSelector::SelectDPUAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam,
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, std::string& selectAlgName) const
{
    HCCL_INFO(
        "topoInfo->topoLevelNums is %u, topoInfo->level0Topo is %u", topoInfo->topoLevelNums, topoInfo->level0Topo);
    (void)configAlgMap;
    if (topoInfo->topoLevelNums > 1) {
        selectAlgName = "InsReduceScatterSequenceMeshMeshDPU";
        HCCL_INFO("Using algo InsReduceScatterSequenceMeshMeshDPU");
        return SelectorStatus::MATCH;
    }

    return SelectorStatus::NOT_MATCH;
}

SelectorStatus ReduceScatterAutoSelector::SelectMeshAlgoAicpuForMesh1D(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, std::string& selectAlgName, u64 dataSize,
    double ratio) const
{
    if (Is64BitDataType(opParam.DataDes.dataType) || opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD) {
        selectAlgName = "InsReduceScatterMesh1D";
    } else {
        if (IsTwoLevelNetLayer(topoInfo)) {
            if (topoInfo->userRankSize == 2 && dataSize >= RS_2P_DETOUR_DATA_SIZE) {
                // hccl 走 AicpuReduceScatterSoleMeshConcur（2P 绕行），devkit 未迁移
                return AlgoNotMigrated("AicpuReduceScatterSoleMeshConcur", "2P two-level detour");
            } else if (dataSize * topoInfo->userRankSize > RS_AICPU_1D_TWO_LEVEL_DATA_SIZE_THRESHOLD) {
                selectAlgName = "AicpuReduceScatterSoleMeshChunk";
            } else if (dataSize * ratio > RS_AICPU_1D_MAX_DATA_SIZE) {
                selectAlgName = "AicpuReduceScatterSoleMeshChunk";
            } else {
                selectAlgName = "InsReduceScatterMesh1D";
            }
        } else {
            if (dataSize * ratio > RS_AICPU_1D_MAX_DATA_SIZE) {
                selectAlgName = "AicpuReduceScatterSoleMeshChunk";
            } else {
                selectAlgName = "InsReduceScatterMesh1D";
            }
        }
    }
    HCCL_DEBUG("[%s] Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

SelectorStatus ReduceScatterAutoSelector::SelectMeshAlgoAicpuForMesh1DClos(
    const TopoInfoWithNetLayerDetails* topoInfo, const OpParam& opParam, u64 dataSize, double ratio,
    bool isClosNumMultipleOfMeshNum, std::string& selectAlgName) const
{
    if (topoInfo->level0PcieMix) {
        // PCIE机型算法选择
        if (IsLayerAllConnetedWithTopo(topoInfo, 0, CommTopo::COMM_TOPO_1DMESH)) {
            return SelectMeshAlgoAicpuForMesh1D(topoInfo, opParam, selectAlgName, dataSize, ratio);
        } else if (Is64BitDataType(opParam.DataDes.dataType) || opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD) {
            // hccl 走 AicpuReduceScatterSequenceMeshNHRAicpuReduce，devkit 未迁移
            return AlgoNotMigrated(
                "AicpuReduceScatterSequenceMeshNHRAicpuReduce", "pcieMix non-all-connected 64bit/PROD");
        } else if (dataSize < OMNI_PCIE_RS_DATA_SIZE) {
            selectAlgName = "InsReduceScatterParallelMesh1DNHRPcie";
        } else {
            selectAlgName = "AicpuReduceScatterPipeLinePcie";
        }
    } else if (IsLayerAllConnetedWithTopo(topoInfo, 0, CommTopo::COMM_TOPO_1DMESH)) {
        // MESH_1D 即可链接所有卡， 使用 MESH_1D 算法
        if (Is64BitDataType(opParam.DataDes.dataType) || opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD) {
            selectAlgName = "InsReduceScatterMesh1D";
        } else if (!IsSmallData(dataSize)) {
            selectAlgName = "AicpuReduceScatterConcurMeshNHR";
        } else if (dataSize * ratio > RS_AICPU_1D_MAX_DATA_SIZE) {
            selectAlgName = "AicpuReduceScatterSoleMeshChunk";
        } else {
            selectAlgName = "InsReduceScatterMesh1D";
        }
    } else if (Is64BitDataType(opParam.DataDes.dataType) || opParam.reduceType == HcclReduceOp::HCCL_REDUCE_PROD) {
        selectAlgName = "InsReduceScatterAicpuReduceNHR";
    } else if (isClosNumMultipleOfMeshNum && IsLargeData(dataSize)) {
        if (opParam.supportSymmetricMemory && dataSize >= OMNI_UBX_RS_LARGE_SIZE) {
            // hccl 走 AicpuReduceScatterPipeLineMeshNHR（UBX 对称内存），devkit 未迁移
            return AlgoNotMigrated("AicpuReduceScatterPipeLineMeshNHR", "UBX symmetric-memory large data");
        } else {
            selectAlgName = "AicpuReduceScatterParallelMeshNHRUBX";
        }
    } else {
        selectAlgName = "InsReduceScatterNHR";
    }
    HCCL_DEBUG("[ReduceScatterAutoSelector][%s] Algo match [%s]", __func__, selectAlgName.c_str());
    return SelectorStatus::MATCH;
}

REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_REDUCE_SCATTER, 18, ReduceScatterAutoSelector);
} // namespace mc2_ops_hccl
