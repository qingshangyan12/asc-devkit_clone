/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef HCCL_CCU_KERNEL_ALL_TO_ALL_MESH_1D_H
#define HCCL_CCU_KERNEL_ALL_TO_ALL_MESH_1D_H

#include <vector>
#include <ios>
#include "utils.h"
#include "ccu_kernel.h"
#include "ccu_kernel_utils.h"
#include "ccu_kernel_alg_base.h"
#include "ccu_func_dl.hpp"

namespace mc2_ops_hccl {

class CcuKernelArgAlltoAllMesh1D : public hcomm::CcuKernelArg {
public:
    explicit CcuKernelArgAlltoAllMesh1D(uint64_t dimSize, uint32_t rankId, bool loadFromMem, const OpParam& opParam,
                                            const std::vector<std::vector<uint32_t>>& subCommRanks)
        : dimSize_(dimSize),
          rankId_(rankId),
          loadFromMem_(loadFromMem),
          opParam_(opParam),
          subCommRanks_(subCommRanks)
    {
        HCCL_DEBUG("[CcuKernelArgAlltoAllMesh1D] dimSize: %lu, rankId: %u",
                   dimSize_, rankId_);
    }
    hcomm::CcuKernelSignature GetKernelSignature() const override
    {
        hcomm::CcuKernelSignature signature;
        GenerateCcuKernelSignature(signature, "CcuKernelArgAlltoAllMesh1D", opParam_, subCommRanks_);
        return signature;
    }
    uint64_t                                dimSize_;
    uint32_t                                rankId_;
    bool                                    loadFromMem_;
    OpParam                                 opParam_;
    std::vector<std::vector<uint32_t>>      subCommRanks_;
};

class CcuTaskArgAlltoAllMesh1D : public hcomm::CcuTaskArg {
public:
    explicit CcuTaskArgAlltoAllMesh1D(uint64_t inputAddr, uint64_t outputAddr, uint64_t sliceSize,
        uint64_t token, uint64_t srcOffset, uint64_t dstOffset, uint64_t srcStride, bool loadFromMem) :
        inputAddr_(inputAddr), outputAddr_(outputAddr), sliceSize_(sliceSize), token_(token), 
        srcOffset_(srcOffset), dstOffset_(dstOffset), srcStride_(srcStride), loadFromMem_(loadFromMem)
    {
        HCCL_DEBUG("[CcuTaskArgAlltoAllMesh1D] inputAddr: %lu, outputAddr: %lu, sliceSize: %lu, srcOffset: %lu, "
                   "dstOffset: %lu, srcStride: %lu, loadFromMem: %lu, "
                   "lastSliceSize: %lu, repeatNum: %lu",
                   inputAddr_, outputAddr_, sliceSize_, srcOffset_, dstOffset_, srcStride_, loadFromMem_);
    }

    uint64_t inputAddr_;
    uint64_t outputAddr_;
    uint64_t sliceSize_;
    uint64_t token_;
    uint64_t srcOffset_;
    uint64_t dstOffset_;
    uint64_t srcStride_;
    bool loadFromMem_;
};

class CcuKernelAlltoAllMesh1D : public CcuKernelAlgBase {
public:
    CcuKernelAlltoAllMesh1D(const hcomm::CcuKernelArg &arg);
    ~CcuKernelAlltoAllMesh1D() override {}

    HcclResult Algorithm() override;  // 框架接口占位符
    static HcclResult KernelCcuAlltoAllMesh1D(ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                        ccu::Variable srcStride, ccu::Variable srcOffset, ccu::Variable dstOffset,
                        ccu::Variable sliceSize, ccu::Variable goSize1, ccu::Variable goSize2,
                        ccu::Variable goSize3, ccu::Variable goSize4);
    std::vector<uint64_t> GeneArgs(const hcomm::CcuTaskArg &arg) override;

private:
    static HcclResult InitResource(ccu::Variable inputAddr, ccu::Variable outputAddr, ccu::Variable tokenInfo,
                                    ccu::Variable srcStride, ccu::Variable srcOffset, ccu::Variable dstOffset,
                                    ccu::Variable sliceSize, ccu::Variable goSize1, ccu::Variable goSize2,
                                    ccu::Variable goSize3, ccu::Variable goSize4);
    static void PreSync();
    static void PostSync();
    static void DoAlltoAll();
    
    static uint64_t rankSize_;
    static uint32_t rankId_;
    static HcclDataType dataType_;
    static HcclDataType outputDataType_;
    static ccu::Variable repeatNum_;
    static std::vector<ChannelHandle> channels_;
    static std::vector<ccu::Variable> input_;
    static std::vector<ccu::Variable> output_;
    static std::vector<ccu::Variable> token_;
    static ccu::Variable sliceSize_;
    static ccu::Variable srcStride_;
    static ccu::Variable srcOffset_;
    static ccu::Variable dstOffset_;
    static GroupOpSizeVars groupOpSize_;
    static ccu::LocalAddr                   myDst_;
    static std::vector<ccu::RemoteAddr>     dstAddr_;
    static std::vector<ccu::LocalAddr>      srcAddr_;
    static std::vector<ccu::Variable>       dstToken_;
    static std::vector<ccu::Variable>       srcToken_;
    static ccu::Event event_;
    static ccu::Variable flag_;
    static bool loadFromMem_;
};

}// namespace mc2_ops_hccl
#endif // HCCL_CCU_KERNEL_ALL_TO_ALL_MESH_1D_H
