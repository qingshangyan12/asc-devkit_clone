/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "log.h"
#include "alg_param.h"
#include "alg_data_trans_wrapper.h"
#include "coll_alg_v2_exec_registry.h"
#include "cann_dispatch_bridge.h"
#include "cann_host_bridge.h"
#include "hcomm_primitives.h"
#include "task_cache/mc2_aicpu_task_cache.h"
#include "task_cache/mc2_aicpu_task_cache_runner.h"
// R10 · F1 断点②（KFC server 链 unified 分叉，2026-10-08）：unified AICPU 消费方
// + 真原语后端（unified/exec/ 与 unified/l5/——与 kernel_launch.cc :23-26 同式）。
// include 链（decoupler CMakeLists 的 ccl_kernel_decoupler_open_exec 目标）：
//   unified/exec/unified_aicpu_exec.h —— IsUnifiedAicpuAlgName / RunUnifiedAicpuOp
//     （其自身 include alg_param.h——已在 ${OPEN_HCCL_SRC_ROOT}/ops/op_common/inc）
//   unified/l5/hcomm_verb_sink.h —— HcommVerbSink / MakeHcommVerbRes
//     （其自身 include verb_sink.h——词汇表半边在 unified/inc/）
// 两目录（exec/、l5/）与 inc/ 一并进目标 include 路径（CMakeLists 同轮扩展）。
#include "unified_aicpu_exec.h"
#include "hcomm_verb_sink.h"

namespace {
mc2_ops_hccl::OpParam* AsOpenOpParam(std::vector<uint8_t>& opParam)
{
    return reinterpret_cast<mc2_ops_hccl::OpParam*>(opParam.data());
}

u64 GetDataTypeSize(HcclDataType dataType)
{
    switch (dataType) {
        case HCCL_DATA_TYPE_INT8:
        case HCCL_DATA_TYPE_UINT8:
        case HCCL_DATA_TYPE_HIF8:
        case HCCL_DATA_TYPE_FP8E5M2:
        case HCCL_DATA_TYPE_FP8E4M3:
        case HCCL_DATA_TYPE_FP8E8M0:
        case HCCL_DATA_TYPE_MXFP8:
            return 1U;
        case HCCL_DATA_TYPE_INT16:
        case HCCL_DATA_TYPE_UINT16:
        case HCCL_DATA_TYPE_FP16:
        case HCCL_DATA_TYPE_BFP16:
            return 2U;
        case HCCL_DATA_TYPE_INT32:
        case HCCL_DATA_TYPE_UINT32:
        case HCCL_DATA_TYPE_FP32:
            return 4U;
        case HCCL_DATA_TYPE_INT64:
        case HCCL_DATA_TYPE_UINT64:
        case HCCL_DATA_TYPE_FP64:
            return 8U;
        case HCCL_DATA_TYPE_INT128:
            return 16U;
        default:
            return 0U;
    }
}

HcclResult RestoreVarDataAlltoAllV(mc2_ops_hccl::OpParam& param, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    u64 rankSize = resCtx.topoInfo.userRankSize;
    CHK_PRT_RET(
        param.varMemSize != mc2_ops_hccl::ALL_TO_ALL_V_VECTOR_NUM * rankSize * sizeof(u64),
        HCCL_ERROR(
            "[RestoreVarDataAlltoAllV] param.varMemSize [%llu] is invalid,"
            " ALL_TO_ALL_V_VECTOR_NUM is [%u], rankSize is [%u], sizeof(u64) is [%u],",
            param.varMemSize, mc2_ops_hccl::ALL_TO_ALL_V_VECTOR_NUM, rankSize, sizeof(u64)),
        HCCL_E_PARA);

    HCCL_INFO(
        "[RestoreVarDataAlltoAllV] param.varMemSize [%llu],"
        " ALL_TO_ALL_V_VECTOR_NUM is [%u], rankSize is [%u], sizeof(u64) is [%u],",
        param.varMemSize, mc2_ops_hccl::ALL_TO_ALL_V_VECTOR_NUM, rankSize, sizeof(u64));
    for (uint32_t i = 0; i < rankSize; i++) {
        HCCL_INFO(
            "param.all2AllVDataDes.sendCounts[%d]:[%llu], param.all2AllVDataDes.recvCounts[%d]:[%llu], "
            "param.all2AllVDataDes.sdispls[%d]:[%llu], param.all2AllVDataDes.rdispls[%d]:[%llu]",
            i, reinterpret_cast<u64*>(param.all2AllVDataDes.sendCounts)[i], i,
            reinterpret_cast<u64*>(param.all2AllVDataDes.recvCounts)[i], i,
            reinterpret_cast<u64*>(param.all2AllVDataDes.sdispls)[i], i,
            reinterpret_cast<u64*>(param.all2AllVDataDes.rdispls)[i]);
    }
    constexpr u64 SEND_COUNT_IDX = 0;
    constexpr u64 RECV_COUNT_IDX = 1;
    constexpr u64 SEND_DISPL_IDX = 2;
    constexpr u64 RECV_DISPL_IDX = 3;

    u64* data = reinterpret_cast<u64*>(param.varData);
    for (u64 i = 0; i < mc2_ops_hccl::ALL_TO_ALL_V_VECTOR_NUM * rankSize; i++) {
        u64 val = i / rankSize;
        switch (val) {
            case SEND_COUNT_IDX:
                data[i] = reinterpret_cast<u64*>(param.all2AllVDataDes.sendCounts)[i % rankSize];
                break;
            case RECV_COUNT_IDX:
                data[i] = reinterpret_cast<u64*>(param.all2AllVDataDes.recvCounts)[i % rankSize];
                break;
            case SEND_DISPL_IDX:
                data[i] = reinterpret_cast<u64*>(param.all2AllVDataDes.sdispls)[i % rankSize];
                break;
            case RECV_DISPL_IDX:
                data[i] = reinterpret_cast<u64*>(param.all2AllVDataDes.rdispls)[i % rankSize];
                break;
            default:
                break;
        }
    }

    return HCCL_SUCCESS;
}

HcclResult RestoreVarDataIfNeeded(mc2_ops_hccl::OpParam& param, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    HcclResult ret = HCCL_SUCCESS;
    if (param.opType == HCCL_CMD_ALLTOALLV || param.opType == HCCL_CMD_ALLTOALL) {
        ret = RestoreVarDataAlltoAllV(param, resCtx);
    }
    return ret;
}

} // namespace

HcclResult LaunchOpenOpParamDataImpl(
    std::vector<uint8_t>& opParam, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    auto* param = AsOpenOpParam(opParam);
    CHK_PTR_NULL(param);
    HCCL_INFO(
        "[MC2_OPEN_DIAG][LaunchOpenOpParamDataImpl] file[%s:%d], opParamSize %zu, "
        "sizeof(OpParam) %zu, varMemSizeOffset %zu, varMemSize %llu, opType %u, algName[%s].",
        __FILE__, __LINE__, opParam.size(), sizeof(mc2_ops_hccl::OpParam), offsetof(mc2_ops_hccl::OpParam, varMemSize),
        static_cast<unsigned long long>(param->varMemSize), static_cast<u32>(param->opType), param->algName);
    HCCL_INFO(
        "[MC2_OPEN_DIAG][Launch] opType %u, algName[%s], inputPtr %p, outputPtr %p, resCtx %p, ctxSize %llu, stream %p."
        "userRank %u, userRankSize %u, threadNum %zu, cclMemAddr %p, cclMemSize %llu.",
        static_cast<u32>(param->opType), param->algName, param->inputPtr, param->outputPtr, param->resCtx,
        static_cast<unsigned long long>(param->ctxSize), param->stream, resCtx.topoInfo.userRank,
        resCtx.topoInfo.userRankSize, resCtx.threads.size(), resCtx.cclMem.addr,
        static_cast<unsigned long long>(resCtx.cclMem.size));
    if (param->opType == HCCL_CMD_ALLTOALLV || param->opType == HCCL_CMD_ALLTOALL) {
        HCCL_INFO(
            "[MC2_OPEN_DIAG][Launch][AllToAllV] sendDataType %u, recvDataType %u, sendCounts %p, recvCounts %p, "
            "sdispls %p, rdispls %p.",
            static_cast<u32>(param->all2AllVDataDes.sendType), static_cast<u32>(param->all2AllVDataDes.recvType),
            param->all2AllVDataDes.sendCounts, param->all2AllVDataDes.recvCounts, param->all2AllVDataDes.sdispls,
            param->all2AllVDataDes.rdispls);
    } else {
        HCCL_INFO(
            "[MC2_OPEN_DIAG][Launch][Else] count %llu, dataType %u, outputType %u, strideCount %llu.",
            static_cast<unsigned long long>(param->DataDes.count), static_cast<u32>(param->DataDes.dataType),
            static_cast<u32>(param->DataDes.outputType), static_cast<unsigned long long>(param->DataDes.strideCount));
    }
    const u64 dataTypeSize = GetDataTypeSize(param->DataDes.dataType);
    const u64 inputBytes = param->DataDes.count * dataTypeSize;
    const bool isAllGather = (param->opType == HCCL_CMD_ALLGATHER);
    const u64 rankStrideCount = (param->DataDes.strideCount == 0U) ? param->DataDes.count : param->DataDes.strideCount;
    const u64 outputSpanCount =
        isAllGather ? (rankStrideCount * (resCtx.topoInfo.userRankSize == 0U ? 0U : resCtx.topoInfo.userRankSize - 1U) +
                       param->DataDes.count) :
                      param->DataDes.count;
    const u64 outputBytes = outputSpanCount * dataTypeSize;
    const u64 rankStrideBytes = isAllGather ? rankStrideCount * dataTypeSize : 0U;
    const u64 inputBegin = reinterpret_cast<u64>(param->inputPtr);
    const u64 outputBegin = reinterpret_cast<u64>(param->outputPtr);
    HCCL_INFO(
        "[MC2_OPEN_DIAG][LaunchRange] userRank %u, userRankSize %u, dataTypeSize %llu, inputBytes %llu, "
        "rankStrideCount %llu, rankStrideBytes %llu, outputSpanCount %llu, outputBytes %llu, "
        "inputRange [%#llx, %#llx), outputSpan [%#llx, %#llx), outputSpanRankScaled %u, engine %u.",
        resCtx.topoInfo.userRank, resCtx.topoInfo.userRankSize, static_cast<unsigned long long>(dataTypeSize),
        static_cast<unsigned long long>(inputBytes), static_cast<unsigned long long>(rankStrideCount),
        static_cast<unsigned long long>(rankStrideBytes), static_cast<unsigned long long>(outputSpanCount),
        static_cast<unsigned long long>(outputBytes), static_cast<unsigned long long>(inputBegin),
        static_cast<unsigned long long>(inputBegin + inputBytes), static_cast<unsigned long long>(outputBegin),
        static_cast<unsigned long long>(outputBegin + outputBytes), isAllGather, static_cast<u32>(param->engine));

    CHK_RET(RestoreVarDataIfNeeded(*param, resCtx));

    const bool useCannBridge = false;
    if (useCannBridge) {
        HcclResult cannRet = mc2_ops_hccl::RunWithMc2TaskCache(
            *param, resCtx, [&]() -> HcclResult { return mc2_ops_hccl::LaunchViaCann(*param, resCtx); });
        CHK_PRT_RET(
            cannRet != HCCL_SUCCESS,
            HCCL_ERROR(
                "[CannBridge] dispatch failed (ret=%d), opType=%u algName[%s]", static_cast<int>(cannRet),
                static_cast<u32>(param->opType), param->algName),
            cannRet);
        return HCCL_SUCCESS;
    }

    // ------------------------------------------------------------------
    // R10 · F1 断点②（KFC server 链 unified 分叉补齐，2026-10-08）：
    // algName 携带 unified 识别键（资源期 PrepareCcAlgResources unified 成功路径
    // 回填，hccl_alloc_ctx_res.cc :1250 区域 FillOpParamAlgName(kUnifiedAicpuAlgKey)）
    // → 不走存量 executor，走 unified AICPU 消费方（v4 blob 校验 + exec 期现值
    // 实例化 + L4 Emit + HcommVerbSink 真原语）。
    // ★判序硬约束（R6 终审 BLOCKER 教训，与 kernel_launch.cc :358-394 同型）：
    //   "unified_aicpu" 无 CollAlgExecRegistryV2 注册项，GetAlgExec 对它必返
    //   nullptr——若分叉放在查表之后，下方 :196 的 CHK_SMART_PTR_NULL 拦截会先
    //   return，unified 分支永不可达（生产死代码）。分叉必须前移到 GetAlgExec
    //   **之前**；前移后 unified 键根本到不了查表，存量 algName 路径（查表 +
    //   nullptr 拦截 + Orchestrate）行为零变化。
    // ★失败语义（与 kernel_launch.cc 挂点一致）：本链无灰度回退语义——执行期
    //   失败已过资源期灰度回退点，非 SUCCESS 直接上抛（半执行状态回退会造成
    //   数据不一致）。
    // ★本改动仅 host 侧执行路径（executor 函数体），不触 device 侧 __gm__/
    //   地址空间敏感代码（R7 MAJOR-1 教训不适用——无 device 侧数据面改动）。
    // ------------------------------------------------------------------
    if (mc2_ops_hccl::refactor::IsUnifiedAicpuAlgName(param->algName)) {
        // 资源束 threads + 逐 peer 通道（l5/MakeHcommVerbRes——kernel_launch.cc
        // 分叉点同式：同源不重复；sink 内含真原语分发）
        mc2_ops_hccl::refactor::HcommVerbSink unifiedSink(mc2_ops_hccl::refactor::MakeHcommVerbRes(resCtx));
        const HcclResult unifiedRet = mc2_ops_hccl::refactor::RunUnifiedAicpuOp(*param, resCtx, unifiedSink);
        CHK_PRT_RET(
            unifiedRet != HCCL_SUCCESS,
            HCCL_ERROR("[UnifiedExec][AICPU] RunUnifiedAicpuOp failed, ret[%d], algName[%s].",
                static_cast<int>(unifiedRet), param->algName),
            unifiedRet);
        HCCL_INFO("[UnifiedExec][AICPU] unified path done (skipping legacy Orchestrate), algName[%s].",
            param->algName);
        return HCCL_SUCCESS;
    }

    auto executor =
        mc2_ops_hccl::CollAlgExecRegistryV2::Instance().GetAlgExec(param->opType, std::string(param->algName));
    CHK_SMART_PTR_NULL(executor);

    CHK_RET(mc2_ops_hccl::InitHcommBatchTransferOnThreadSupported(resCtx.isHcommBatchTransferOnThreadSupported));
    HcclResult ret = mc2_ops_hccl::RunWithMc2TaskCache(
        *param, resCtx, [&]() -> HcclResult { return executor->Orchestrate(*param, resCtx); });
    CHK_PRT_RET(ret != HCCL_SUCCESS, HCCL_ERROR("orchestrate failed for alg:%s", param->algName), ret);

    return HCCL_SUCCESS;
}
