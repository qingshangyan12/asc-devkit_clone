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
#include "ccu_temp_kfc_half_alltoallv_mesh_1D.h"

namespace mc2_ops_hccl {

// 变长区 varData 内含 4 段，每段 rankSize 个 u64：sendCounts/recvCounts/sdispls/rdispls。
// 源实现按 [sizeA|sizeB] 与 [offsetA|offsetB] 各 2*rankSize 读取，二者段长一致。
constexpr uint32_t HA2AV_SLICE_SEG_NUM = 2;

CcuTempKfcHalfAlltoAllVMesh1D::CcuTempKfcHalfAlltoAllVMesh1D(
    const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuTempKfcMesh1DMem2MemBase(param, rankId, subCommRanks)
{}

CcuTempKfcHalfAlltoAllVMesh1D::~CcuTempKfcHalfAlltoAllVMesh1D() {}

HcclResult CcuTempKfcHalfAlltoAllVMesh1D::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    CHK_RET(GetRes(resourceRequest));
    // 阶段一单 mission，只申请 1 个 kernel；阶段二恢复双 mission 时改为 2
    resourceRequest.ccuKernelNum.push_back(1);

    CcuKernelInfo kernelInfo;
    strcpy_s(kernelInfo.kernelFuncName, sizeof(kernelInfo.kernelFuncName), "CcuKernelHalfAlltoAllVMesh1D");

    std::vector<HcclChannelDesc> channelDescs;
    if (topoInfo->level0Topo != Level0Shape::MESH_1D_CLOS) {
        CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));
    } else {
        CHK_RET(CalcChannelRequestMesh1DWithPriorityTopo(
            comm, param, topoInfo, subCommRanks_, channelDescs, CommTopo::COMM_TOPO_1DMESH));
        for (const auto& channel : channelDescs) {
            if (channel.channelProtocol != COMM_PROTOCOL_UBC_CTP) {
                HCCL_ERROR(
                    "[CcuTempKfcHalfAlltoAllVMesh1D][CalcRes] channelProtocol: %u", channel.channelProtocol);
                return HCCL_E_INTERNAL;
            }
        }
    }

    // CcuKernelKfcServer 在注册前替换该占位并复用这些 channel
    kernelInfo.channels = channelDescs;
    // 本端 window 基址：设备侧 AlltoAllvWrite 契约不传 recvBuf，本 rank 自留分片的落点
    // 需在建图期确定（对应源实现 CtxArg 里的 cclBufferAddr），由 KfcServer 继承。
    //
    // 取 param.ccuScratchAddr（= OpResCtx::res[0]，由 AllocCcuOpResCtx 分配的
    // "_ccu_scratch"），与设备侧 ccuParam_.scratchAddr 同源。不能取 param.outputPtr
    // （MC2 路径下恒为 nullptr），也不能取 buffInfo_.hcclBuff（HcclGetHcclBuffer 返回的
    // cclBuffer 是另一块内存）——PreSync 会把该地址广播给所有对端当作 Write 目标，
    // 取错会让全域写入落到错误地址，而非仅影响本 rank 的 GroupCopy。
    if (param.ccuScratchAddr == 0) {
        HCCL_ERROR("[CcuTempKfcHalfAlltoAllVMesh1D][CalcRes] ccuScratchAddr is zero, local window base unavailable.");
        return HCCL_E_PARA;
    }
    kernelInfo.localWinAddr = param.ccuScratchAddr;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);

    HCCL_DEBUG(
        "[CcuTempKfcHalfAlltoAllVMesh1D::CalcRes] channelDescs.size()=%llu, dimsize=%llu, ccuKernelInfos.size()=%llu",
        channelDescs.size(), subCommRanks_[0].size(), resourceRequest.ccuKernelInfos.size());
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcHalfAlltoAllVMesh1D::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    buffInfo_ = templateDataParams.buffInfo;

    const uint64_t userInAddr = PointerToAddr(buffInfo_.inputPtr) + buffInfo_.inBuffBaseOff;
    const uint64_t myBufferAddr = PointerToAddr(buffInfo_.hcclBuff.addr) + buffInfo_.hcclBuffBaseOff;
    uint64_t token = 0;
    CHK_RET(GetToken(buffInfo_, token));

    // TODO(阶段一待与 MC2 侧对齐)：变长区段序与设备可见性。
    // 这里假设 varData 起始为 sendSize 段（2*rankSize 个 u64），紧随 sendOffset 段。
    // 若 MC2 实际布局为 sendCounts/recvCounts/sdispls/rdispls 四段，需按段偏移调整。
    const uint64_t varBaseAddr = PointerToAddr(const_cast<void*>(static_cast<const void*>(param.varData)));
    const uint64_t sliceSegBytes = static_cast<uint64_t>(templateRankSize_) * HA2AV_SLICE_SEG_NUM * sizeof(u64);
    const uint64_t sendSizeAddr = varBaseAddr;
    const uint64_t sendOffsetAddr = varBaseAddr + sliceSegBytes;

    // 本端在对端 cclBuffer 中的落点：status*winSize/2 + rankId*GRID_SIZE 在 MC2 侧折算为 slice stride
    const uint64_t recvOffset = templateDataParams.outputSliceStride * mySubCommRank_;

    // 本 rank 自己那片的大小，用于本地 GroupCopy
    uint64_t localSliceSize = templateDataParams.sliceSize;
    if (templateDataParams.tailSize != 0 && mySubCommRank_ == templateRankSize_ - 1) {
        localSliceSize = templateDataParams.tailSize;
    }
    if (localSliceSize == 0) {
        return HcclResult::HCCL_SUCCESS;
    }

    LoopGroupConfig config{};
    config.msInterleave = CCU_MS_INTERLEAVE;
    config.loopCount = CCU_MS_LOCAL_COPY_LOOP_COUNT;
    config.memSlice = CCU_MS_SIZE * LOCAL_COPY_MS_PER_LOOP;
    const auto goSize = CalGoSize(localSliceSize, config);

    CcuKernelSubmitInfo submitInfo{};
    submitInfo.kernelHandle = templateResource.ccuKernels[0];
    // 顺序须与设备侧 CcuPrepareForAllToAllVWrite 的 xnData_ 布局一致：
    // xn[1]=sendBuf, xn[2]=sendSizes, xn[3]=sendOffsets, xn[4..7]=goSize, xn[8]=remoteWinOffset
    (void)myBufferAddr;
    CHK_RET(FillCachedArgs(
        submitInfo, userInAddr, sendSizeAddr, sendOffsetAddr, goSize[0], goSize[1], goSize[2], goSize[3], recvOffset,
        token));
    templateResource.submitInfos.push_back(submitInfo);
    return HcclResult::HCCL_SUCCESS;
}

u64 CcuTempKfcHalfAlltoAllVMesh1D::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return 0;
}
} // namespace mc2_ops_hccl
