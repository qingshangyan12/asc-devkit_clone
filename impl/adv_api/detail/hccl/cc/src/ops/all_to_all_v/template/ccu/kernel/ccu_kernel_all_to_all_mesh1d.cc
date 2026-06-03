/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "ccu_kernel_alg_base.h"
#include "ccu_kernel_all_to_all_mesh1d.h"

namespace mc2_ops_hccl {
using namespace hcomm;

// 静态成员变量初始化（仅初始化简单类型）
uint64_t CcuKernelAlltoAllMesh1D::rankSize_ = 0;
uint32_t CcuKernelAlltoAllMesh1D::rankId_ = 0;
HcclDataType CcuKernelAlltoAllMesh1D::dataType_ = HcclDataType::HCCL_DATA_TYPE_RESERVED;
HcclDataType CcuKernelAlltoAllMesh1D::outputDataType_ = HcclDataType::HCCL_DATA_TYPE_RESERVED;
bool CcuKernelAlltoAllMesh1D::loadFromMem_ = false;
// 其他成员依赖默认构造函数（vector、Variable、LocalAddr等）

constexpr int INPUT_XN_ID  = 0;
constexpr int OUTPUT_XN_ID = 1;
constexpr int TOKEN_XN_ID  = 2;
constexpr int CKE_IDX_0    = 0;
constexpr int CKE_IDX_1    = 1;
constexpr int CKE_IDX_2    = 2;

CcuKernelAlltoAllMesh1D::CcuKernelAlltoAllMesh1D(const CcuKernelArg &arg)
    : CcuKernelAlgBase(arg)
{
    const CcuKernelArgAlltoAllMesh1D *kernelArg
        = dynamic_cast<const CcuKernelArgAlltoAllMesh1D *>(&arg);

    rankId_ = kernelArg->rankId_;
    channels_ = kernelArg->channels;
    rankSize_ = kernelArg->dimSize_;
    loadFromMem_ = kernelArg->loadFromMem_;
}

HcclResult CcuKernelAlltoAllMesh1D::InitResource(ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                                  ccu::Variable srcStride, ccu::Variable srcOffset, ccu::Variable dstOffset,
                                  ccu::Variable sliceSize, ccu::Variable goSize1, ccu::Variable goSize2,
                                  ccu::Variable goSize3, ccu::Variable goSize4)
{
    uint16_t channelIdx = 0;
    if (channels_.size() == 0) {
        HCCL_ERROR("[CcuKernelAlltoAllMesh1D] channels is empty!");
        return HcclResult::HCCL_E_INTERNAL;
    }

    // 按照rank号从小到大遍历channels，遇到本rank就填充本地资源，否则依次取远端资源，要求给框架返回的Link同样是按顺序排列的
    for (uint64_t peerId = 0; peerId < rankSize_; peerId++) {
        if (peerId == rankId_) {
            input_.push_back(inputAddr);
            output_.push_back(outputAddr);
            token_.push_back(tokenInfo);
        } else {
            HCCL_DEBUG("[CcuKernelAlltoAllMesh1D] MyRank[%u], PeerId[%u], ChannelId[%u]",
                       rankId_, peerId, channelIdx);
            ccu::Variable inputVar = ccu::GetResByChannel<ccu::Variable>(channels_[channelIdx], INPUT_XN_ID);
            input_.push_back(inputVar);
            ccu::Variable outputVar = ccu::GetResByChannel<ccu::Variable>(channels_[channelIdx], OUTPUT_XN_ID);
            output_.push_back(outputVar);
            ccu::Variable tokenVar = ccu::GetResByChannel<ccu::Variable>(channels_[channelIdx], TOKEN_XN_ID);
            token_.push_back(tokenVar);
            channelIdx++;
        }
    }
    sliceSize_   = sliceSize;
    srcStride_   = srcStride;
    srcOffset_   = srcOffset;
    dstOffset_   = dstOffset;
    groupOpSize_.addrOffset = goSize1;
    groupOpSize_.loopParam = goSize2;
    groupOpSize_.parallelParam = goSize3;
    groupOpSize_.residual = goSize4;
    return HcclResult::HCCL_SUCCESS;
}

void CcuKernelAlltoAllMesh1D::PreSync()
{
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D PreSync begin.");
    for (ChannelHandle channel : channels_) {
        ccu::WriteVariableWithNotify(channel, output_[rankId_],
            OUTPUT_XN_ID, CKE_IDX_0, 1 << OUTPUT_XN_ID);
        ccu::WriteVariableWithNotify(channel, output_[rankId_],
            TOKEN_XN_ID, CKE_IDX_0, 1 << TOKEN_XN_ID);
    }

    uint32_t allBit = (1 << OUTPUT_XN_ID) | (1 << TOKEN_XN_ID);
    for (auto &ch: channels_) {
        ccu::NotifyWait(ch, CKE_IDX_0, allBit);
    }
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D PreSync end.");
}

void CcuKernelAlltoAllMesh1D::PostSync()
{
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D PostSync begin.");
    uint16_t postBit = 1 << 5;
    for (auto &ch : channels_) {
        ccu::NotifyRecord(ch, CKE_IDX_1, postBit);
    }
    for (auto &ch : channels_) {
        ccu::NotifyWait(ch, CKE_IDX_1, postBit);
    }
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] AlltoAllMesh1D PostSync end.");
}

void CcuKernelAlltoAllMesh1D::DoAlltoAll()
{
    HCCL_INFO("DoAlltoAll Start.");
    CcuKernelCtxBase ctx;
    ctx.resourceAllocated = false;
    // ctx.IsLoopResRegistered = false;
    ctx.moConfig.msInterleave = 0;
    ctx.moConfig.loopCount = 0;
    ctx.moConfig.memSlice = 0;
    ctx.moRes.eventCount = 0;
    ctx.moRes.bufCount = 0;
    ctx.enginePool = 0;

    std::vector<ccu::LocalAddr> src(rankSize_);
    std::vector<ccu::RemoteAddr> dst(rankSize_);
    ccu::LocalAddr localDst;

    for (uint64_t r = 0; r < rankSize_; r++) {
        if (r != rankId_) {
            dst[r].token = token_[r];
            dst[r].addr = output_[r];
            dst[r].addr += dstOffset_;
        } else {
            localDst.token = token_[r];
            localDst.addr = output_[r];
            localDst.addr += dstOffset_;
        }

        src[r].addr = srcOffset_;
        src[r].token = token_[r];
        for (uint64_t i = 0; i < r; i++) {
            src[r].addr += srcStride_;
        }
    }

    uint32_t channelId = 0;
    uint16_t allBit = ((1 << rankSize_) - 1) & (~(1 << rankId_)); // 仅rankid位为0，其他位为1，代表远端准备好了

    if (loadFromMem_) {
        for(uint64_t r = 0; r < rankSize_; r++) {
            if (r == rankId_) {
                ccu::LocalCopy(localDst, src[r], sliceSize_, event_, 1 << r);
            }
            else {
                ccu::Write(channels_[channelId], dst[r], src[r], sliceSize_, event_, 1 << r);
                channelId++;
            }
        }
        // 等读完所有对端
        ccu::EventWait(event_, (1 << rankSize_) - 1);
    } else {
        for(uint64_t r = 0; r < rankSize_; r++) {
            if (r != rankId_) {
                ccu::Write(channels_[channelId], dst[r], src[r], sliceSize_, event_, 1 << r);
                channelId++;
            }
        }
        mc2_ops_hccl::GroupCopy(ctx, localDst, src[rankId_], groupOpSize_);
        ccu::EventWait(event_, allBit);
    }
}

ccu::Func CcuKernelAlltoAllMesh1D([](ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                                    ccu::Variable srcStride, ccu::Variable srcOffset, ccu::Variable dstOffset,
                                    ccu::Variable sliceSize, ccu::Variable goSize1, ccu::Variable goSize2,
                                    ccu::Variable goSize3, ccu::Variable goSize4) {
    CcuKernelAlltoAllMesh1D::KernelCcuAlltoAllMesh1D(inputAddr, outputAddr, tokenInfo, srcStride, srcOffset, dstOffset,
                                                    sliceSize, goSize1, goSize2, goSize3, goSize4);
});

HcclResult CcuKernelAlltoAllMesh1D::KernelCcuAlltoAllMesh1D(ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                                              ccu::Variable srcStride, ccu::Variable srcOffset, ccu::Variable dstOffset,
                                              ccu::Variable sliceSize, ccu::Variable goSize1, ccu::Variable goSize2,
                                              ccu::Variable goSize3, ccu::Variable goSize4)
{
    HCCL_INFO("[ccuAllToAllMesh1D_context] AllToAllMesh1D run.");

    CHK_RET(InitResource(inputAddr, outputAddr, tokenInfo, srcStride, srcOffset, dstOffset,
                        sliceSize, goSize1, goSize2, goSize3, goSize4));

    PreSync();
    DoAlltoAll();
    PostSync();

    HCCL_INFO("[AllToAllAlgo] AllToAllMesh1D end");
    
    return HcclResult::HCCL_SUCCESS;
}

// ==================== Algorithm() 框架接口占位符 ====================
HcclResult CcuKernelAlltoAllMesh1D::Algorithm()
{
    HCCL_INFO("[CcuKernelAlltoAllMesh1D] Algorithm entry (framework placeholder)");
    return HcclResult::HCCL_SUCCESS;
}

std::vector<uint64_t> CcuKernelAlltoAllMesh1D::GeneArgs(const CcuTaskArg &arg)
{
    const CcuTaskArgAlltoAllMesh1D *taskArg
        = dynamic_cast<const CcuTaskArgAlltoAllMesh1D *>(&arg);
    uint64_t inputAddr  = taskArg->inputAddr_;
    uint64_t outputAddr = taskArg->outputAddr_;
    uint64_t tokenInfo  = taskArg->token_;

    uint64_t srcStride = taskArg->srcStride_;
    uint64_t srcOffset = taskArg->srcOffset_;
    uint64_t dstOffset = taskArg->dstOffset_;

    uint64_t sliceSize  = taskArg->sliceSize_;
    auto     goSize     = CalGoSize(sliceSize);

    std::vector<uint64_t> taskArgs = {
        inputAddr,  outputAddr,  tokenInfo, sliceSize, 
        srcStride,  srcOffset,
        dstOffset, goSize[0], goSize[1], goSize[2], goSize[3]
    };

    HCCL_INFO("[CcuKernelAlltoAllMesh1D] TaskArgs: inputAddr[%llu], outputAddr[%llu], "
               "srcStride[%llu], srcOffset[%llu],"
               "dstOffset[%llu], sliceSize[%llu], goSize: [%llu], [%llu], [%llu], [%llu]",
               inputAddr, outputAddr, srcStride, srcOffset,
               dstOffset, sliceSize, goSize[0], goSize[1], goSize[2], goSize[3]);
    return taskArgs;
}

}// namespace mc2_ops_hccl
