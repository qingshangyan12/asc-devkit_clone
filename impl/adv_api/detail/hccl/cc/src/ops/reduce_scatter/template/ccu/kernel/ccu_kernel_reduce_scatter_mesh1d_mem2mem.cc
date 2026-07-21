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
#include "ccu_kernel_reduce_scatter_mesh1d_mem2mem.h"

namespace mc2_ops_hccl {
using namespace hcomm;

// bit序号，每种信号用一个bit
constexpr int INPUT_XN_ID = 0;
constexpr int SCRATCH_XN_ID = 1;
constexpr int TOKEN_XN_ID = 2;
constexpr int POST_SYNC_ID = 3;
// cke序号
constexpr int CKE_IDX_0 = 0;

struct KfcReduceScatterMesh1DMem2MemContext : CcuKernelCtxBase {
    const ChannelHandle *channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};
    
    HcclDataType dataType;
    HcclDataType outputDataType;
    HcclReduceOp reduceOp;

    std::vector<ccu::Variable> input;
    ccu::Variable output;
    std::vector<ccu::Variable> scratch;
    std::vector<ccu::Variable> token;
    ccu::Variable currentRankSliceInputOffset;
    ccu::Variable currentRankSliceOutputOffset;
    ccu::Variable inputRepeatStride;
    ccu::Variable outputRepeatStride;
    ccu::Variable normalSliceSize;
    ccu::Variable lastSliceSize;
    ccu::Variable repeatNum;
    GroupOpSizeVars goSize;
    ccu::LocalAddr myInput;
    std::vector<ccu::RemoteAddr> remoteInput;
    std::vector<ccu::LocalAddr> scratchMem;
    ccu::Event event;
    ccu::Variable flag;
};

static CcuResult InitResource(KfcReduceScatterMesh1DMem2MemContext &ctx)
{
    uint32_t channelIdx = 0;

    if (ctx.channelCount == 0) {
        HCCL_ERROR("[CcuKfcAllGatherMesh1D] channels is empty!");
        return CcuResult::CCU_E_INTERNAL;
    }

    ctx.input.resize(ctx.rankSize);
    ctx.scratch.resize(ctx.rankSize);
    ctx.token.resize(ctx.rankSize);
    ctx.remoteInput.resize(ctx.rankSize);
    ctx.scratchMem.resize(ctx.rankSize);

    for (uint64_t peerId = 0; peerId < ctx.rankSize; peerId++) {
        if (peerId != ctx.rankId) {
            ctx.input[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], INPUT_XN_ID);
            ctx.scratch[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], SCRATCH_XN_ID);
            ctx.token[peerId] = ccu::GetResByChannel<ccu::Variable>(ctx.channels[channelIdx], TOKEN_XN_ID);
            channelIdx++;
        }
    }

    return CCU_SUCCESS;
}

static CcuResult LoadArgs(KfcReduceScatterMesh1DMem2MemContext &ctx, ccu::Variable inputAddr, ccu::Variable outputAddr,
 	                    ccu::Variable tokenInfo, ccu::Variable scratch, ccu::Variable currentRankSliceInputOffset,
 	                    ccu::Variable currentRankSliceOutputOffset, ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride,
 	                    ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable scratchRepeatStride,
 	                    ccu::Variable repeatNum, ccu::Variable addrOffset, ccu::Variable loopParam,
 	                    ccu::Variable parallelParam, ccu::Variable residual)
{
    ctx.input[ctx.rankId] = inputAddr;
    ctx.output = outputAddr;
    ctx.token[ctx.rankId] = tokenInfo;
    ctx.scratch[ctx.rankId] = scratch;
    ctx.currentRankSliceInputOffset = currentRankSliceInputOffset;
    ctx.currentRankSliceOutputOffset = currentRankSliceOutputOffset;
    ctx.inputRepeatStride = inputRepeatStride;
    ctx.outputRepeatStride = outputRepeatStride;
    ctx.normalSliceSize = normalSliceSize;
    ctx.lastSliceSize = lastSliceSize;
    ctx.repeatNum = repeatNum;
    ctx.goSize.addrOffset = addrOffset;
    ctx.goSize.loopParam = loopParam;
    ctx.goSize.parallelParam = parallelParam;
    ctx.goSize.residual = residual;
    return CCU_SUCCESS;
}

static CcuResult PreSync(KfcReduceScatterMesh1DMem2MemContext &ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        CCU_CHK_RET(ccu::WriteVariableWithNotify(ctx.channels[i], ctx.input[ctx.rankId],
                    CKE_IDX_0, INPUT_XN_ID, 1 << INPUT_XN_ID)); // bit index = 1，传递input信息
        CCU_CHK_RET(ccu::WriteVariableWithNotify(ctx.channels[i], ctx.scratch[ctx.rankId],
                    CKE_IDX_0, SCRATCH_XN_ID, 1 << SCRATCH_XN_ID)); // bit index = 2, 传递scratch信息
        CCU_CHK_RET(ccu::WriteVariableWithNotify(ctx.channels[i], ctx.token[ctx.rankId],
                    CKE_IDX_0, TOKEN_XN_ID, 1 << TOKEN_XN_ID)); // bit index = 3，传递output信息
    }
    uint32_t allBit = 1 << INPUT_XN_ID | 1 << SCRATCH_XN_ID | 1 << TOKEN_XN_ID;
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, allBit);
    }
    return CCU_SUCCESS;
}

static CcuResult PostSync(KfcReduceScatterMesh1DMem2MemContext &ctx)
{
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyRecord(ctx.channels[i], CKE_IDX_0, 1 << POST_SYNC_ID); // bit index = 4, 用作后同步。cke都可以用同一个，所以都是CKE_IDX_0
    }
    for (uint32_t i = 0; i < ctx.channelCount; i++) {
        ccu::NotifyWait(ctx.channels[i], CKE_IDX_0, 1 << POST_SYNC_ID);
    }
    return CCU_SUCCESS;
}

// 前向声明
static CcuResult ReduceLoopGroup(KfcReduceScatterMesh1DMem2MemContext &ctx, ccu::LocalAddr outDstOrg);

static CcuResult DoReduceScatter(KfcReduceScatterMesh1DMem2MemContext &ctx)
{
    uint32_t channelId = 0;

    ccu::LocalAddr myOutput;

    myOutput.addr = ctx.output;
    myOutput.addr += ctx.currentRankSliceOutputOffset;
    myOutput.token = ctx.token[ctx.rankId];

    ccu::Variable sliceSize;
    sliceSize = (ctx.rankId == (ctx.rankSize - 1)) ? ctx.lastSliceSize : ctx.normalSliceSize;

    CCU_IF(sliceSize != 0)
    {
        for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
            if (rankIdx == ctx.rankId) {
                ccu::LocalCopy(ctx.scratchMem[rankIdx], ctx.myInput, sliceSize, ctx.event, 1 << rankIdx);
            } else {
                ccu::Read(ctx.channels[channelId], ctx.scratchMem[rankIdx], ctx.remoteInput[rankIdx],
                          sliceSize, ctx.event, 1 << rankIdx);
                channelId++;
            }
        }

        // 等读完所有对端
        ccu::EventWait(ctx.event, (1 << ctx.rankSize) - 1);
        CCU_CHK_RET(ReduceLoopGroup(ctx, myOutput));
    }
    return CCU_SUCCESS;
}

static CcuResult DoRepeatReduceScatter(KfcReduceScatterMesh1DMem2MemContext &ctx)
{
    ccu::Variable scratchOffset = 0;

    for (uint32_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
        if (rankIdx == ctx.rankId) {
            ctx.myInput.addr = ctx.input[rankIdx];
            ctx.myInput.addr += ctx.currentRankSliceInputOffset;
            ctx.myInput.token = ctx.token[rankIdx];
        } else {
            ctx.remoteInput[rankIdx].addr = ctx.input[rankIdx];
            ctx.remoteInput[rankIdx].addr += ctx.currentRankSliceInputOffset;
            ctx.remoteInput[rankIdx].token = ctx.token[rankIdx];
        }

        ctx.scratchMem[rankIdx].addr = ctx.scratch[ctx.rankId];
        ctx.scratchMem[rankIdx].addr += scratchOffset;
        scratchOffset += ctx.normalSliceSize;
        ctx.scratchMem[rankIdx].token = ctx.token[ctx.rankId];
    }

    ccu::Variable repeatNumAdd = 1;
    ctx.flag = 0;
    CCU_WHILE(ctx.repeatNum != UINT64_MAX)
    {
        ctx.repeatNum += repeatNumAdd;
        CCU_IF(ctx.flag == 1)
        {
            //  非第一轮执行时，src 和 dst 已经初始化，需要添加偏移量
            for (uint64_t rankIdx = 0; rankIdx < ctx.rankSize; rankIdx++) {
                if (rankIdx == ctx.rankId) {
                    ctx.myInput.addr += ctx.inputRepeatStride;
                } else {
                    ctx.remoteInput[rankIdx].addr += ctx.inputRepeatStride;
                }
            }
            ctx.output += ctx.outputRepeatStride;
        }
        CCU_CHK_RET(DoReduceScatter(ctx));
        ctx.flag = 1;
    }
    return CCU_SUCCESS;
}

static CcuResult CreateReduceLoop(KfcReduceScatterMesh1DMem2MemContext &ctx, GroupLocalReduceVar &var,
    uint32_t size, HcclDataType dataType, HcclDataType outputDataType, HcclReduceOp opType)
{
    constexpr uint32_t LOOP_NUM = 16;
    AllocGoResource(ctx.moConfig, ctx.moRes, ctx.resourceAllocated, LOOP_NUM);

    const std::string loopType = "reduceScatterLocalReduce";
    if (ctx.IsLoopEntityRegistered(loopType)) {
        return CCU_SUCCESS;
    }
    ctx.CreateLoopEntity(loopType);
    auto &loops = ctx.loopMap[loopType];

    uint32_t expansionNum = GetReduceExpansionNum(opType, dataType, outputDataType);
    constexpr uint32_t LOOP_NUM_2 = 2;
    for (uint32_t index = 0; index < LOOP_NUM_2; index++) {
        var.loopScratch[index].resize(size);

        uint32_t bufBase = index * ctx.moConfig.msInterleave;
        ccu::Event loopEvt = ctx.moRes.completedEvent[index];

        loops.body[index].reset(new ccu::Func(
            [&ctx, &var, index, bufBase, loopEvt, size, dataType, outputDataType, opType]() {
            // Step 1: 将各rank的scratch数据copy到ccuBuf
            for (uint32_t i = 0; i < size; i++) {
                ccu::LocalCopy(ctx.moRes.ccuBuf[bufBase + i], var.loopScratch[index][i],
                               var.loopLen[index], loopEvt, 1 << i);
            }
            ccu::EventWait(loopEvt, (1 << size) - 1);

            // Step 2: LocalReduce
            if (size > 1) {
                ccu::LocalReduce(&ctx.moRes.ccuBuf[bufBase], size, dataType, outputDataType, opType,
                                 var.loopLen[index], loopEvt, 1);
                ccu::EventWait(loopEvt);
            }

            // Step 3: Copy结果到dst
            ccu::LocalCopy(var.loopDst[index], ctx.moRes.ccuBuf[bufBase], var.loopLenExp[index], loopEvt, 1);
            ccu::EventWait(loopEvt, 1);
        }));

        loops.loops[index].reset(new ccu::Loop(loops.loopParam[index], *loops.body[index]));
    }

    return CCU_SUCCESS;
}

static CcuResult ReduceLoopGroup(KfcReduceScatterMesh1DMem2MemContext &ctx, ccu::LocalAddr outDstOrg)
{
    const uint32_t size = ctx.scratchMem.size();

    ccu::LocalAddr dst;
    dst.addr = outDstOrg.addr;
    dst.token = outDstOrg.token;

    std::vector<ccu::LocalAddr> scratch;
    scratch.resize(size);
    for (uint32_t idx = 0; idx < size; idx++) {
        scratch[idx].addr = ctx.scratchMem[idx].addr;
        scratch[idx].token = ctx.scratchMem[idx].token;
    }

    GroupLocalReduceVar var;
    CCU_CHK_RET(CreateReduceLoop(ctx, var, size, ctx.dataType, ctx.outputDataType, ctx.reduceOp));
    auto &loops = ctx.loopMap["reduceScatterLocalReduce"];

    uint32_t expansionNum = GetReduceExpansionNum(ctx.reduceOp, ctx.dataType, ctx.outputDataType);
    ccu::Variable sliceSizeExpansion;

    if (expansionNum != 1) {
        ccu::Variable tmp;
        tmp = GetExpansionParam(expansionNum);
        dst.token = dst.token + tmp;
    }

    // m部分
    CCU_IF(ctx.goSize.loopParam != 0)
    {
        ccu::Variable sliceSize;
        ccu::Variable loopParam;
        loopParam = GetLoopParam(0, ctx.moConfig.memSlice * ctx.moConfig.loopCount, 0);
        loopParam = loopParam + ctx.goSize.loopParam;

        sliceSize = ctx.moConfig.memSlice;
        sliceSizeExpansion = ctx.moConfig.memSlice * expansionNum;

        // 绑定loop0的外部LocalAddr和Variable
        for (uint32_t i = 0; i < size; i++) {
            var.loopScratch[0][i].addr = scratch[i].addr;
            var.loopScratch[0][i].token = scratch[i].token;
        }
        var.loopDst[0].addr = dst.addr;
        var.loopDst[0].token = dst.token;
        var.loopLen[0] = sliceSize;
        var.loopLenExp[0] = sliceSizeExpansion;

        ccu::Variable paraCfg;
        paraCfg = GetParallelParam(ctx.moConfig.loopCount - 1, 0, 1);
        ccu::Variable offsetCfg;
        offsetCfg = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);

        loops.loopParam[0] = loopParam;
        std::vector<ccu::Loop> grpLoops{ *loops.loops[0] };
        ccu::LoopGroup group(paraCfg, offsetCfg, 1, grpLoops);
    }

    // n和p部分
    CCU_IF(ctx.goSize.parallelParam != 0)
    {
        // p部分，加m的偏移
        for (uint32_t i = 0; i < size; i++) {
            scratch[i].addr += ctx.goSize.addrOffset;
        }
        for (uint32_t i = 0; i < expansionNum; i++) {
            dst.addr += ctx.goSize.addrOffset;
        }

        sliceSizeExpansion = 0;
        for (uint32_t i = 0; i < expansionNum; i++) {
            sliceSizeExpansion = sliceSizeExpansion + ctx.goSize.residual;
        }

        // 绑定loop0参数 (p部分)
        for (uint32_t i = 0; i < size; i++) {
            var.loopScratch[0][i].addr = scratch[i].addr;
            var.loopScratch[0][i].token = scratch[i].token;
        }
        var.loopDst[0].addr = dst.addr;
        var.loopDst[0].token = dst.token;
        var.loopLen[0] = ctx.goSize.residual;
        var.loopLenExp[0] = sliceSizeExpansion;

        // n部分，再加p的偏移
        for (uint32_t i = 0; i < size; i++) {
            scratch[i].addr += ctx.goSize.residual;
        }
        for (uint32_t i = 0; i < expansionNum; i++) {
            dst.addr += ctx.goSize.residual;
        }

        ccu::Variable sliceSize;
        sliceSize = ctx.moConfig.memSlice;
        sliceSizeExpansion = ctx.moConfig.memSlice * expansionNum;

        // 绑定loop1参数 (n部分)
        for (uint32_t i = 0; i < size; i++) {
            var.loopScratch[1][i].addr = scratch[i].addr;
            var.loopScratch[1][i].token = scratch[i].token;
        }
        var.loopDst[1].addr = dst.addr;
        var.loopDst[1].token = dst.token;
        var.loopLen[1] = sliceSize;
        var.loopLenExp[1] = sliceSizeExpansion;

        ccu::Variable loopCfg0;
        loopCfg0 = GetLoopParam(0, 0, 1);
        ccu::Variable loopCfg1;
        loopCfg1 = GetLoopParam(0, 0, 1);
        ccu::Variable offsetCfg;
        offsetCfg = GetOffsetParam(ctx.moConfig.memSlice, ctx.moConfig.msInterleave, 1);

        loops.loopParam[0] = loopCfg0;
        loops.loopParam[1] = loopCfg1;
        std::vector<ccu::Loop> grpLoops{ *loops.loops[0], *loops.loops[1] };
        ccu::LoopGroup group(ctx.goSize.parallelParam, offsetCfg, 2, grpLoops);
    }

    return CCU_SUCCESS;
}

CcuResult CcuReduceScatterMesh1DMem2MemKernel(ccu::Variable inputAddr, ccu::Variable outputAddr,
 	                                        ccu::Variable tokenInfo, ccu::Variable scratch, ccu::Variable currentRankSliceInputOffset,
 	                                        ccu::Variable currentRankSliceOutputOffset, ccu::Variable inputRepeatStride, ccu::Variable outputRepeatStride,
 	                                        ccu::Variable normalSliceSize, ccu::Variable lastSliceSize, ccu::Variable scratchRepeatStride,
 	                                        ccu::Variable repeatNum, ccu::Variable addrOffset, ccu::Variable loopParam,
 	                                        ccu::Variable parallelParam, ccu::Variable residual,
 	                                        const ChannelHandle channels[], uint32_t channelCount,
 	                                        uint32_t rankSize, uint32_t rankId, const HcclDataType &dataType,
 	                                        const HcclDataType &outputType, const HcclReduceOp &reduceType) {
    KfcReduceScatterMesh1DMem2MemContext ctx;
    ctx.channels = channels;
    ctx.channelCount = channelCount;
    ctx.rankSize = rankSize;
    ctx.rankId = rankId;
    ctx.dataType = dataType;
    ctx.outputDataType = outputType;
    if (ctx.outputDataType == HcclDataType::HCCL_DATA_TYPE_RESERVED) {
        ctx.outputDataType = dataType;
    }
    ctx.reduceOp = reduceType;
    HCCL_INFO(
        "[CcuKernelReduceScatterMesh1DMem2Mem] Init, KernelArgs are rankId[%u], rankSize_[%u], dataType[%d], "
        "outputDataType[%d], reduceOp[%d]",
        ctx.rankId, ctx.rankSize, ctx.dataType, ctx.outputDataType, ctx.reduceOp);

    ctx.resourceAllocated = false;
    ctx.moConfig.msInterleave = 0;
    ctx.moConfig.loopCount = 0;
    ctx.moConfig.memSlice = 0;
    ctx.moRes.eventCount = 0;
    ctx.moRes.bufCount = 0;

    HCCL_INFO("[CcuKernelReduceScatterMesh1DMem2Mem] ReduceScatterMesh1DMem2Mem run");
    CCU_CHK_RET(InitResource(ctx));
    CCU_CHK_RET(LoadArgs(ctx, inputAddr, outputAddr, tokenInfo, scratch, currentRankSliceInputOffset,
                        currentRankSliceOutputOffset, inputRepeatStride, outputRepeatStride, normalSliceSize,
                        lastSliceSize, scratchRepeatStride, repeatNum, addrOffset, loopParam,
                        parallelParam, residual));
    CCU_CHK_RET(PreSync(ctx));
    CCU_CHK_RET(DoRepeatReduceScatter(ctx));
    CCU_CHK_RET(PostSync(ctx));
    HCCL_INFO("[CcuKernelReduceScatterMesh1DMem2Mem] ReduceScatterMesh1DMem2Mem end");
    
    return CCU_SUCCESS;
}
} // namespace mc2_ops_hccl
