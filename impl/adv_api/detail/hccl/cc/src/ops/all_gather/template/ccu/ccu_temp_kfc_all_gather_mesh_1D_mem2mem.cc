/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "channel.h"
#include "hccl_ccu_res.h"
#include "ccu_assist_pub.h"
#include "ccu_kernel_alg_base.h"
#include "ccu_temp_kfc_all_gather_mesh_1D_mem2mem.h"
#include "adapter_acl.h"
#include <unistd.h>

namespace mc2_ops_hccl {

namespace {
// KFC server kernel(CcuKfcServerKernel)的 HBM 参数槽位与 DispatchKfcSubKernel 的 AllGather 布局保持一致，
// 常量来源：ccu_kernel_kfc_server.h / ccu_kernel_kfc_server.cc
constexpr uint32_t KFC_PARAM_NUM_PER_DIE = 32;
constexpr uint32_t KFC_ONE_PARAM_SIZE = 8;
constexpr uint32_t KFC_TASK_NUM_MAX = 64;
constexpr uint32_t KFC_AG_SIGNATURE = 0;          // param[0]：== UINT64_MAX 表示终止
constexpr uint32_t KFC_AG_INPUT = 1;              // param[1]
constexpr uint32_t KFC_AG_OUTPUT = 2;             // param[2]
constexpr uint32_t KFC_AG_IN_OFF = 4;             // param[4]
constexpr uint32_t KFC_AG_OUT_OFF = 5;            // param[5]
constexpr uint32_t KFC_AG_TMP_REPEAT = 6;         // param[6]
constexpr uint32_t KFC_AG_IN_REPEAT_STRIDE = 7;   // param[7]
constexpr uint32_t KFC_AG_OUT_REPEAT_STRIDE = 8;  // param[8]
constexpr uint32_t KFC_AG_NORMAL_SLICE = 9;       // param[9]
constexpr uint32_t KFC_AG_LAST_SLICE = 10;        // param[10]
constexpr uint32_t KFC_AG_IS_IN_OUT_EQ = 11;      // param[11]
constexpr uint32_t KFC_AG_GO_SIZE0 = 12;          // param[12]
constexpr uint32_t KFC_AG_GO_SIZE1 = 13;          // param[13]
constexpr uint32_t KFC_AG_GO_SIZE2 = 14;          // param[14]
constexpr uint32_t KFC_AG_GO_SIZE3 = 15;          // param[15]

HcclResult SubmitAllGatherParamsToKfcServer(
    const TemplateDataParams& templateDataParams, uint64_t inputAddr, uint64_t outputAddr,
    uint64_t currentRankSliceInputOffset, uint64_t currentRankSliceOutputOffset, uint64_t tmpRepeatNum,
    uint64_t inputRepeatStride, uint64_t outputRepeatStride, uint64_t normalSliceSize, uint64_t lastSliceSize,
    uint64_t isInputOutputEqual, const std::vector<uint64_t>& goSize)
{
    if (templateDataParams.ccuKfcArgs.size() < 2) {
        HCCL_ERROR(
            "[CcuTempKfcAllGatherMesh1DMem2Mem] ccuKfcArgs size[%zu] < 2, missing xn/cke addr.",
            templateDataParams.ccuKfcArgs.size());
        return HCCL_E_PARA;
    }
    const uint64_t xnAddr = templateDataParams.ccuKfcArgs[0];
    const uint64_t ckeAddr = templateDataParams.ccuKfcArgs[1];

    // 与 DispatchKfcSubKernel 的 HBM param 布局严格对齐
    std::vector<uint64_t> param(KFC_PARAM_NUM_PER_DIE, 0);
    param[KFC_AG_SIGNATURE] = 0;                      // 有效轮次（非 UINT64_MAX）
    param[KFC_AG_INPUT] = inputAddr;
    param[KFC_AG_OUTPUT] = outputAddr;
    // param[3] 未使用
    param[KFC_AG_IN_OFF] = currentRankSliceInputOffset;
    param[KFC_AG_OUT_OFF] = currentRankSliceOutputOffset;
    param[KFC_AG_TMP_REPEAT] = tmpRepeatNum;
    param[KFC_AG_IN_REPEAT_STRIDE] = inputRepeatStride;
    param[KFC_AG_OUT_REPEAT_STRIDE] = outputRepeatStride;
    param[KFC_AG_NORMAL_SLICE] = normalSliceSize;
    param[KFC_AG_LAST_SLICE] = lastSliceSize;
    param[KFC_AG_IS_IN_OUT_EQ] = isInputOutputEqual;
    param[KFC_AG_GO_SIZE0] = goSize[0];
    param[KFC_AG_GO_SIZE1] = goSize[1];
    param[KFC_AG_GO_SIZE2] = goSize[2];
    param[KFC_AG_GO_SIZE3] = goSize[3];

    HCCL_INFO(
        "[CcuTempKfcAllGatherMesh1DMem2Mem] KFC submit: xnAddr[0x%llx], ckeAddr[0x%llx], "
        "inputAddr[0x%llx], outputAddr[0x%llx], inOff[%llu], outOff[%llu], tmpRepeat[%llu], "
        "normalSlice[%llu], lastSlice[%llu], isInOutEqual[%llu], goSize[%llu,%llu,%llu,%llu].",
        static_cast<unsigned long long>(xnAddr), static_cast<unsigned long long>(ckeAddr),
        static_cast<unsigned long long>(inputAddr), static_cast<unsigned long long>(outputAddr),
        static_cast<unsigned long long>(currentRankSliceInputOffset),
        static_cast<unsigned long long>(currentRankSliceOutputOffset),
        static_cast<unsigned long long>(tmpRepeatNum), static_cast<unsigned long long>(normalSliceSize),
        static_cast<unsigned long long>(lastSliceSize), static_cast<unsigned long long>(isInputOutputEqual),
        static_cast<unsigned long long>(goSize[0]), static_cast<unsigned long long>(goSize[1]),
        static_cast<unsigned long long>(goSize[2]), static_cast<unsigned long long>(goSize[3]));

    // 1) 把本轮 AllGather 参数写入 xnAddr 参数队列（HOST -> DEVICE）
    const size_t paramBytes = param.size() * sizeof(uint64_t);
    aclError ret = aclrtMemcpy(
        reinterpret_cast<void*>(xnAddr), paramBytes, param.data(), paramBytes, ACL_MEMCPY_HOST_TO_DEVICE);
    if (ret != ACL_SUCCESS) {
        HCCL_ERROR("[CcuTempKfcAllGatherMesh1DMem2Mem] write xnAddr failed, ret[%d].", ret);
        return HCCL_E_RUNTIME;
    }

    // 2) 写 turn-start 信号到 waitAddr(= ckeAddr)，触发 KFC server 执行本轮 AllGather
    const uint64_t turnStart = 1;
    ret = aclrtMemcpy(
        reinterpret_cast<void*>(ckeAddr), sizeof(uint64_t), &turnStart, sizeof(uint64_t), ACL_MEMCPY_HOST_TO_DEVICE);
    if (ret != ACL_SUCCESS) {
        HCCL_ERROR(
            "[CcuTempKfcAllGatherMesh1DMem2Mem] signal turn-start to ckeAddr[0x%llx] failed, ret[%d].",
            static_cast<unsigned long long>(ckeAddr), ret);
        return HCCL_E_RUNTIME;
    }
    // 3) 等待 KFC server 执行完本轮 AllGather（轮询 recordAddr = ckeAddr + KFC_TASK_NUM_MAX * KFC_ONE_PARAM_SIZE）
    const uint64_t recordAddr = ckeAddr + static_cast<uint64_t>(KFC_TASK_NUM_MAX) * KFC_ONE_PARAM_SIZE;
    constexpr uint64_t KFC_WAIT_TIMEOUT_US = 1000000; // 1s
    uint64_t turnEnd = 0;
    for (uint64_t waitedUs = 0; waitedUs < KFC_WAIT_TIMEOUT_US; waitedUs += 1000) {
        aclError pollRet = aclrtMemcpy(
            &turnEnd, sizeof(uint64_t), reinterpret_cast<void*>(recordAddr), sizeof(uint64_t),
            ACL_MEMCPY_DEVICE_TO_HOST);
        if (pollRet != ACL_SUCCESS) {
            HCCL_ERROR(
                "[CcuTempKfcAllGatherMesh1DMem2Mem] read recordAddr[0x%llx] failed, ret[%d].",
                static_cast<unsigned long long>(recordAddr), pollRet);
            return HCCL_E_RUNTIME;
        }
        if (turnEnd == 1) {
            break;
        }
        usleep(1000);
    }
    if (turnEnd != 1) {
        HCCL_ERROR(
            "[CcuTempKfcAllGatherMesh1DMem2Mem] wait KFC turn-end timeout, recordAddr[0x%llx], turnEnd[%llu].",
            static_cast<unsigned long long>(recordAddr), static_cast<unsigned long long>(turnEnd));
        return HCCL_E_INTERNAL;
    }
    HCCL_INFO("[CcuTempKfcAllGatherMesh1DMem2Mem] KFC submit done, turn-end confirmed.");
    return HCCL_SUCCESS;
}
} // namespace

CcuTempKfcAllGatherMesh1DMem2Mem::CcuTempKfcAllGatherMesh1DMem2Mem(
    const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuTempKfcMesh1DMem2MemBase(param, rankId, subCommRanks)
{}

CcuTempKfcAllGatherMesh1DMem2Mem::~CcuTempKfcAllGatherMesh1DMem2Mem() {}

HcclResult CcuTempKfcAllGatherMesh1DMem2Mem::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    CHK_RET(GetRes(resourceRequest));
    resourceRequest.ccuKernelNum.push_back(1);

    CcuKernelInfo kernelInfo;
    strcpy_s(kernelInfo.kernelFuncName, sizeof(kernelInfo.kernelFuncName), "CcuKernelAllGatherMesh1DMem2Mem");

    std::vector<HcclChannelDesc> channelDescs;
    if (topoInfo->level0Topo != Level0Shape::MESH_1D_CLOS) {
        CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));
    } else {
        CHK_RET(CalcChannelRequestMesh1DWithPriorityTopo(
            comm, param, topoInfo, subCommRanks_, channelDescs, CommTopo::COMM_TOPO_1DMESH));
        for (const auto& channel : channelDescs) {
            if (channel.channelProtocol != COMM_PROTOCOL_UBC_CTP) {
                HCCL_ERROR("[CcuTempKfcAllGatherMesh1DMem2Mem][CalcRes] channelProtocol: %u", channel.channelProtocol);
                return HCCL_E_INTERNAL;
            }
        }
    }

    // CcuKernelKfcServer replaces this placeholder before registration and reuses these channels.
    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllGatherMesh1DMem2Mem::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    buffInfo_ = templateDataParams.buffInfo;

    const uint64_t inputAddr = PointerToAddr(buffInfo_.inputPtr) + buffInfo_.inBuffBaseOff;
    const uint64_t outputAddr = PointerToAddr(buffInfo_.outputPtr) + buffInfo_.outBuffBaseOff;
    uint64_t token = 0;
    CHK_RET(GetToken(buffInfo_, token));

    const uint64_t inputSliceStride = templateDataParams.inputSliceStride;
    const uint64_t outputSliceStride = templateDataParams.outputSliceStride;
    const uint64_t repeatNum = templateDataParams.repeatNum;
    const uint64_t inputRepeatStride = templateDataParams.inputRepeatStride;
    const uint64_t outputRepeatStride = templateDataParams.outputRepeatStride;
    uint64_t normalSliceSize = templateDataParams.sliceSize;
    const uint64_t lastSliceSize = templateDataParams.tailSize;

    if (templateDataParams.tailSize != 0 && mySubCommRank_ == templateRankSize_ - 1) {
        normalSliceSize = templateDataParams.tailSize;
    }
    if (normalSliceSize == 0 && lastSliceSize == 0) {
        return HcclResult::HCCL_SUCCESS;
    }

    const uint64_t currentRankSliceInputOffset = inputSliceStride * mySubCommRank_;
    const uint64_t currentRankSliceOutputOffset = outputSliceStride * mySubCommRank_;
    const uint64_t tmpRepeatNum = UINT64_MAX - repeatNum;
    const bool inputOutputEqual =
        (inputAddr + currentRankSliceInputOffset == outputAddr + currentRankSliceOutputOffset);
    const uint64_t isInputOutputEqual = static_cast<uint64_t>(inputOutputEqual);

    LoopGroupConfig config{};
    config.msInterleave = CCU_MS_INTERLEAVE;
    config.loopCount = CCU_MS_LOCAL_COPY_LOOP_COUNT;
    config.memSlice = CCU_MS_SIZE * LOCAL_COPY_MS_PER_LOOP;
    const auto goSize = CalGoSize(normalSliceSize, config);

    // 填充参数（布局与 DispatchKfcSubKernel 的 HBM param 对齐：param[0] 为 signature 槽位）
    CcuKernelSubmitInfo submitInfo{};
    submitInfo.kernelHandle = templateResource.ccuKernels[0];
    CHK_RET(FillCachedArgs(
        submitInfo, 0 /*signature*/, inputAddr, outputAddr, 0 /*unused*/, currentRankSliceInputOffset,
        currentRankSliceOutputOffset, tmpRepeatNum, inputRepeatStride, outputRepeatStride, normalSliceSize,
        lastSliceSize, isInputOutputEqual, goSize[0], goSize[1], goSize[2], goSize[3]));
    templateResource.submitInfos.push_back(submitInfo);

    // 把本轮 AllGather 参数写入 KFC server 的 xnAddr 参数队列并触发 cke turn-start 信号
    CHK_RET(SubmitAllGatherParamsToKfcServer(
        templateDataParams, inputAddr, outputAddr, currentRankSliceInputOffset, currentRankSliceOutputOffset,
        tmpRepeatNum, inputRepeatStride, outputRepeatStride, normalSliceSize, lastSliceSize, isInputOutputEqual,
        goSize));

    HCCL_INFO(
        "[CcuTempKfcAllGatherMesh1DMem2Mem][KernelRun] end, token[%llu], submitInfos[%zu].",
        static_cast<unsigned long long>(token), templateResource.submitInfos.size());
    return HcclResult::HCCL_SUCCESS;
}

u64 CcuTempKfcAllGatherMesh1DMem2Mem::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return 0;
}

} // namespace mc2_ops_hccl
