/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kernel_alg_base.h"
#include "ccu_temp_kfc_all_gather_nhr_1D_multi_jetty_mem2mem.h"
#include "ccu_kernel_kfc_server.h"
#include "ccu_variable_dl.hpp"
#include "ccu_func_dl.hpp"
#include "mc2_type.h"
#include "hccl_alloc_ctx_res.h"

namespace mc2_ops_hccl {
    using namespace hcomm;
    using namespace std;

    static_assert(
        KFC_CONCURRENT_AG_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE,
        "Concurrent AllGather parameters exceed the KFC server load width");
    static_assert(
        KFC_CONCURRENT_A2A_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE,
        "Concurrent AllToAll parameters exceed the KFC server load width");
    static_assert(
        KFC_A2A_MJ_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE,
        "AllToAll MultiJetty parameters exceed the KFC server load width");
    static_assert(
        KFC_CONCURRENT_RS_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE,
        "Concurrent ReduceScatter parameters exceed the KFC server load width");
    static_assert(KFC_MAX_MISSION_NUM == 2U, "KFC server CKE partitioning expects two missions");
    static_assert(KFC_SIGNAL_REGIONS_PER_MISSION == 2U, "Each KFC mission requires commit and done regions");

    const uint32_t HBM_PARAM_IDX_0 = 0;
    const uint32_t HBM_PARAM_IDX_1 = 1;
    const uint32_t HBM_PARAM_IDX_2 = 2;
    const uint32_t HBM_PARAM_IDX_3 = 3;
    const uint32_t HBM_PARAM_IDX_4 = 4;
    const uint32_t HBM_PARAM_IDX_5 = 5;
    const uint32_t HBM_PARAM_IDX_6 = 6;
    const uint32_t HBM_PARAM_IDX_7 = 7;
    const uint32_t HBM_PARAM_IDX_8 = 8;
    const uint32_t HBM_PARAM_IDX_9 = 9;
    const uint32_t HBM_PARAM_IDX_10 = 10;
    const uint32_t HBM_PARAM_IDX_11 = 11;
    const uint32_t HBM_PARAM_IDX_12 = 12;
    const uint32_t HBM_PARAM_IDX_13 = 13;
    const uint32_t HBM_PARAM_IDX_14 = 14;
    const uint32_t HBM_PARAM_IDX_15 = 15;
    const uint32_t HBM_PARAM_IDX_16 = 16;
    const uint32_t HBM_PARAM_IDX_17 = 17;
    const uint32_t HBM_PARAM_IDX_18 = 18;
    const uint32_t HBM_PARAM_IDX_19 = 19;
    const uint32_t HBM_PARAM_IDX_20 = 20;
    const uint32_t HBM_PARAM_IDX_21 = 21;
    const uint32_t HBM_PARAM_IDX_22 = 22;
    const uint32_t HBM_PARAM_IDX_23 = 23;
    const uint32_t HBM_PARAM_IDX_24 = 24;
    const uint32_t HBM_PARAM_IDX_25 = 25;
    // KFC reduce_scatter specific parameters (at positions [13..23], not used by homm CCU server template)
    const uint32_t HBM_PARAM_IDX_KFC_CHUNK_SIZE = 13;
    const uint32_t HBM_PARAM_IDX_KFC_CHUNK_LOOP_NUM = 14;
    const uint32_t HBM_PARAM_IDX_KFC_TAIL_SIZE = 15;
    const uint32_t HBM_PARAM_IDX_KFC_FULL_GO_ADDR_OFFSET = 16;
    const uint32_t HBM_PARAM_IDX_KFC_FULL_GO_LOOP_PARAM = 17;
    const uint32_t HBM_PARAM_IDX_KFC_FULL_GO_PARALLEL_PARAM = 18;
    const uint32_t HBM_PARAM_IDX_KFC_FULL_GO_RESIDUAL = 19;
    const uint32_t HBM_PARAM_IDX_KFC_TAIL_GO_ADDR_OFFSET = 20;
    const uint32_t HBM_PARAM_IDX_KFC_TAIL_GO_LOOP_PARAM = 21;
    const uint32_t HBM_PARAM_IDX_KFC_TAIL_GO_PARALLEL_PARAM = 22;
    const uint32_t HBM_PARAM_IDX_KFC_TAIL_GO_RESIDUAL = 23;

    const uint32_t SINGLE_DIE = 1;
    const uint32_t DOUBLE_DIE = 2;
    const uint32_t DIE0_ID = 0;
    const uint32_t DIE1_ID = 1;
    constexpr const char* DIE1_START_SIG = "Die1StartSig";
    constexpr const char* DIE1_END_SIG = "Die1EndSig";

    static CcuResult ParseKernelArg(KfcServerContext & ctx, const CcuKernelArgKfcServer* kernelArg)
    {
        ctx.arg = kernelArg;
        return CCU_SUCCESS;
    }

    static CcuResult InitResource(const KfcServerContext& ctx)
    {
        (void)ctx;
        return CCU_SUCCESS;
    }

    static CcuResult LoadArgs(KfcServerContext & ctx)
    {
        uint32_t argId = 0;

        // 顺序需与 kfcServerArgs 中的字段顺序一致：xnAddr, ckeAddr, dieNum, missionNum, missionIndex, token
        CCU_CHK_RET(ccu::LoadArg(ctx.xnAddr, argId++));
        CCU_CHK_RET(ccu::LoadArg(ctx.ckeAddr, argId++));
        CCU_CHK_RET(ccu::LoadArg(ctx.dieNum, argId++));
        CCU_CHK_RET(ccu::LoadArg(ctx.missionNum, argId++));
        CCU_CHK_RET(ccu::LoadArg(ctx.missionIndex, argId++));
        CCU_CHK_RET(ccu::LoadArg(ctx.token, argId++));
        CCU_CHK_RET(ccu::LoadArg(ctx.subKernelPc, argId++));

        return CCU_SUCCESS;
    }

    static CcuResult CompArgs(KfcServerContext & ctx)
    {
        ccu::Variable regionSize;
        ctx.waitAddr = ctx.ckeAddr;
        regionSize = CCU_TASK_NUM_MAX * CCU_ONE_PARAM_SIZE;
        ctx.recordAddr = ctx.ckeAddr + regionSize;
        CCU_IF(ctx.missionIndex == 1)
        {
            ctx.waitAddr = ctx.recordAddr + regionSize;
            ctx.recordAddr = ctx.waitAddr + regionSize;
        }
        return CCU_SUCCESS;
    }

    void MissionPreSync(const ccu::Variable& func, const KfcServerContext& ctx)
    {
        (void)func;
        (void)ctx;
    }

    void MissionPostSync(const KfcServerContext& ctx) { (void)ctx; }

    CcuResult WaitTurnStartSig(
        const ccu::Variable& hbmSigAddr, ccu::Variable& turnStartSig, const KfcServerContext& ctx)
    {
        (void)ctx;
        const uint32_t getDieId = 1;
        // 单Die场景：等待HBM中的信号
        CCU_WHILE(turnStartSig != 1)
        {
            CCU_CHK_RET(ccu::Load(hbmSigAddr, turnStartSig)); // 循环读HBM对应地址的信号到Xn，直到Xn中的信号值为1
        }
        turnStartSig = 0;                                  // reset Xn
        CCU_CHK_RET(ccu::Store(hbmSigAddr, turnStartSig)); // reset HBM
        return CCU_SUCCESS;
    }

    CcuResult SetTurnEndSig(
        const ccu::Variable& hbmSigAddr, const ccu::Variable& turnEndSig, const KfcServerContext& ctx)
    {
        (void)ctx;
        const uint32_t getDieId = 1;
        CCU_CHK_RET(ccu::Store(hbmSigAddr, turnEndSig));
        return CCU_SUCCESS;
    }

    CcuResult LoadFuncParamFromMemory(
        const ccu::Variable& paramAddr, ccu::Array<ccu::Variable>& param, const KfcServerContext& ctx)
    {
        (void)ctx;
        // 双Die场景Die1需要读后32个参数，其他场景都是读取前32个参数
        ccu::Variable doubleDie;
        doubleDie = CCU_PARAM_NUM_PER_DIE * CCU_ONE_PARAM_SIZE;
        ccu::Variable addr;
        addr = paramAddr;

        // 一次性读取本轮32个参数
        CCU_CHK_RET(ccu::Load(addr, param, CCU_PARAM_NUM_PER_DIE));
        return CCU_SUCCESS;
    }

    static void InitCanonicalArgs(
        ccu::Array<ccu::Variable>& param, ccu::Array<ccu::Variable>& args, const KfcServerContext& ctx)
    {
        args[0] = param[HBM_PARAM_IDX_1];
        args[1] = param[HBM_PARAM_IDX_2];
        args[2] = ctx.token;
        for (uint32_t i = HBM_PARAM_IDX_3; i < CCU_FUNC_XKERNEL_ARG_MAX; ++i) {
            args[i] = param[i];
        }
    }

    static void CanonicalizeAllGatherArgs(
        ccu::Array<ccu::Variable>& param, ccu::Array<ccu::Variable>& args, const KfcServerContext& ctx)
    {
        if (ctx.arg->algSubType == KFC_PARALLEL_ALL_GATHER_SUB_TYPE) {
            args[0] = param[KFC_PARALLEL_AG_INPUT];
            args[1] = param[KFC_PARALLEL_AG_OUTPUT];
            args[3] = param[KFC_PARALLEL_AG_OUTPUT_STRIDE];
            args[4] = param[KFC_PARALLEL_AG_PART0_SIZE];
            args[5] = param[KFC_PARALLEL_AG_PART1_SIZE];
            args[6] = param[KFC_PARALLEL_AG_PART1_OFFSET];
            args[7] = param[KFC_PARALLEL_AG_MESH_PHASE_DONE_ADDR];
            args[8] = param[KFC_PARALLEL_AG_NHR_PHASE_DONE_ADDR];
            if (ctx.arg->role == KfcServerRole::ALL_GATHER_NHR) {
                args[9] = param[KFC_PARALLEL_AG_PART0_SLICE_PER_JETTY];
                args[10] = param[KFC_PARALLEL_AG_PART0_LAST_SLICE_PER_JETTY];
                args[11] = param[KFC_PARALLEL_AG_PART1_SLICE_PER_JETTY];
                args[12] = param[KFC_PARALLEL_AG_PART1_LAST_SLICE_PER_JETTY];
            }
            const uint32_t goArgOffset = ctx.arg->role == KfcServerRole::ALL_GATHER_NHR ? 13U : 9U;
            for (uint32_t i = 0; i < 4U; ++i) {
                args[goArgOffset + i] = param[KFC_PARALLEL_AG_PART0_GO_SIZE_0 + i];
                args[goArgOffset + 4U + i] = param[KFC_PARALLEL_AG_PART1_GO_SIZE_0 + i];
            }
            return;
        }
        if (ctx.arg->role == KfcServerRole::ALL_GATHER_NHR) {
            args[0] = param[KFC_CONCURRENT_AG_NHR_INPUT];
            args[1] = param[KFC_CONCURRENT_AG_NHR_OUTPUT];
            args[3] = param[KFC_CONCURRENT_AG_NHR_SLICE_SIZE];
            args[4] = param[KFC_CONCURRENT_AG_NHR_SLICE_SIZE_PER_JETTY];
            args[5] = param[KFC_CONCURRENT_AG_NHR_LAST_SLICE_SIZE_PER_JETTY];
            args[6] = param[KFC_CONCURRENT_AG_NHR_REPEAT_NUM_INV];
            args[7] = param[KFC_CONCURRENT_AG_NHR_INPUT_SLICE_STRIDE];
            args[8] = param[KFC_CONCURRENT_AG_NHR_OUTPUT_SLICE_STRIDE];
            args[9] = param[KFC_CONCURRENT_AG_NHR_INPUT_REPEAT_STRIDE];
            args[10] = param[KFC_CONCURRENT_AG_NHR_OUTPUT_REPEAT_STRIDE];
            args[11] = param[KFC_CONCURRENT_AG_NHR_INPUT_OUTPUT_EQUAL];
            for (uint32_t i = 0; i < 4U; ++i) {
                args[12U + i] = param[KFC_CONCURRENT_AG_NHR_GO_SIZE_0 + i];
            }
        }
    }

    static void CanonicalizeReduceScatterArgs(
        ccu::Array<ccu::Variable>& param, ccu::Array<ccu::Variable>& args, const KfcServerContext& ctx)
    {
        if (ctx.arg->role == KfcServerRole::REDUCE_SCATTER_PEER_ONLY) {
            args[3] = param[HBM_PARAM_IDX_4];
            args[4] = param[HBM_PARAM_IDX_7];
            args[5] = param[HBM_PARAM_IDX_8];
            return;
        }
        if (ctx.arg->role == KfcServerRole::REDUCE_SCATTER_NHR) {
            args[0] = param[KFC_CONCURRENT_RS_NHR_INPUT];
            args[1] = param[KFC_CONCURRENT_RS_NHR_OUTPUT];
            for (uint32_t i = 0; i < 7U; ++i) {
                args[3U + i] = param[KFC_CONCURRENT_RS_NHR_SLICE_SIZE + i];
            }
            return;
        }
        if (ctx.arg->role == KfcServerRole::DEFAULT) {
            for (uint32_t i = 0; i < 11U; ++i) {
                args[5U + i] = param[HBM_PARAM_IDX_KFC_CHUNK_SIZE + i];
            }
        }
    }

    static void CanonicalizeAllToAllArgs(
        ccu::Array<ccu::Variable>& param, ccu::Array<ccu::Variable>& args, const KfcServerContext& ctx)
    {
        if (ctx.arg->opType == HcclCMDType::HCCL_CMD_ALLTOALLV) {
            args[5] = param[HBM_PARAM_IDX_9];
            for (uint32_t i = 0; i < 4U; ++i) {
                args[6U + i] = param[HBM_PARAM_IDX_5 + i];
            }
            return;
        }
        if (ctx.arg->role == KfcServerRole::ALL_TO_ALL_CLOS) {
            args[0] = param[KFC_CONCURRENT_A2A_CLOS_INPUT];
            args[1] = param[KFC_CONCURRENT_A2A_CLOS_OUTPUT];
            for (uint32_t i = 0; i < 8U; ++i) {
                args[3U + i] = param[KFC_CONCURRENT_A2A_CLOS_SLICE_SIZE + i];
            }
        }
    }

    static CcuResult CallKfcSubKernel(ccu::Array<ccu::Variable> & param, KfcServerContext & ctx)
    {
        static_assert(CCU_FUNC_XKERNEL_ARG_MAX == CCU_PARAM_NUM_PER_DIE, "KFC Func ABI must match queue parameters");
        ccu::Array<ccu::Variable> args(CCU_FUNC_XKERNEL_ARG_MAX);
        InitCanonicalArgs(param, args, ctx);
        if (ctx.arg->opType == HcclCMDType::HCCL_CMD_ALLGATHER) {
            CanonicalizeAllGatherArgs(param, args, ctx);
        } else if (ctx.arg->opType == HcclCMDType::HCCL_CMD_REDUCE_SCATTER) {
            CanonicalizeReduceScatterArgs(param, args, ctx);
        } else if (ctx.arg->opType == HcclCMDType::HCCL_CMD_ALLTOALL ||
                   ctx.arg->opType == HcclCMDType::HCCL_CMD_ALLTOALLV) {
            CanonicalizeAllToAllArgs(param, args, ctx);
        }
        return ccu::CallFunc(ctx.subKernelPc, args);
    }

    static CcuResult KfcServerGenCircularQueue(KfcServerContext & ctx)
    {
        HCCL_INFO("[KfcServerGenCircularQueue] KfcServerGenCircularQueue start");

        ccu::Variable repeatCond; // 存放《控制repeat循环执行的条件》的寄存器
        repeatCond = 0;
        ccu::Variable turnStartSig; // 存放《轮次执行开始信号》的寄存器
        turnStartSig = 0;
        ccu::Variable turnEndSig; // 存放《轮次执行完成信号》的寄存器，在循环中固定为1
        turnEndSig = 1;

        // 使用独立XN保存队列游标，避免Variable拷贝构造与context基址共享handle。
        ccu::Variable waitAddr;
        ccu::Variable recordAddr;
        ccu::Variable paramAddr;
        waitAddr = ctx.waitAddr;
        recordAddr = ctx.recordAddr;
        paramAddr = ctx.xnAddr;

        ccu::Variable ckeSize; // 其他寄存器
        ckeSize = CCU_ONE_PARAM_SIZE;
        ccu::Variable paramSize;
        paramSize = CCU_PARAM_NUM_MAX * CCU_ONE_PARAM_SIZE;

        ccu::Variable queueIdx;
        ccu::Variable queueEnd;
        ccu::Variable one;
        queueIdx = 0;
        one = 1;
        queueEnd = CCU_TASK_NUM_MAX;

        // 存放<每轮算子参数>的寄存器
        ccu::Array<ccu::Variable> param(CCU_PARAM_NUM_PER_DIE);

        CCU_WHILE(repeatCond == 0)
        {
            CCU_CHK_RET(WaitTurnStartSig(waitAddr, turnStartSig, ctx));  // 等待本轮开始信号
            CCU_CHK_RET(LoadFuncParamFromMemory(paramAddr, param, ctx)); // 读取本轮参数
            CCU_IF(param[HBM_PARAM_IDX_0] == INVALID_U64) { repeatCond = 1; }
            CCU_IF(param[HBM_PARAM_IDX_0] != INVALID_U64)
            {
                MissionPreSync(param[HBM_PARAM_IDX_0], ctx);
                CCU_CHK_RET(CallKfcSubKernel(param, ctx));
                MissionPostSync(ctx);
                CCU_CHK_RET(SetTurnEndSig(recordAddr, turnEndSig, ctx));
            }
            waitAddr += ckeSize;
            recordAddr += ckeSize;
            paramAddr += paramSize;
            queueIdx += one;
            CCU_IF(queueIdx == static_cast<u64>(CCU_TASK_NUM_MAX))
            {
                waitAddr = ctx.waitAddr;
                recordAddr = ctx.recordAddr;
                paramAddr = ctx.xnAddr;
                queueIdx = 0;
            }
        }
        return CCU_SUCCESS;
    }

    static CcuResult SlaveKfcServerGenCircularQueue(KfcServerContext & ctx)
    {
        ccu::Variable signature; // 算子签名

        ccu::Variable repeatCond; // 存放《控制repeat循环执行的条件》的寄存器
        repeatCond = 0;

        ccu::Variable paramAddr;
        paramAddr = ctx.xnAddr;

        ccu::Array<ccu::Variable> param(CCU_PARAM_NUM_PER_DIE); // 存放<每轮算子参数>的寄存器

        CCU_WHILE(repeatCond == 0)
        {
            MissionPreSync(signature, ctx);                              // 在context中依次加入8轮指令
            CCU_CHK_RET(LoadFuncParamFromMemory(paramAddr, param, ctx)); // 读取本轮参数

            CCU_IF(param[HBM_PARAM_IDX_0] == INVALID_U64) { repeatCond = 1; }
            CCU_IF(param[HBM_PARAM_IDX_0] != INVALID_U64) { CCU_CHK_RET(CallKfcSubKernel(param, ctx)); }
            MissionPostSync(ctx);
        }
        return CCU_SUCCESS;
    }

    CcuResult CcuKfcServerKernel(CcuKernelArg arg)
    {
        auto* kernelArg = static_cast<CcuKernelArgKfcServer*>(arg);
        KfcServerContext ctx;
        CCU_CHK_RET(ParseKernelArg(ctx, kernelArg));
        CCU_CHK_RET(InitResource(ctx));
        CCU_CHK_RET(LoadArgs(ctx));
        CCU_CHK_RET(CompArgs(ctx));
        CCU_IF(ctx.missionIndex == 0) { CCU_CHK_RET(KfcServerGenCircularQueue(ctx)); }
        CCU_ELSE { CCU_CHK_RET(SlaveKfcServerGenCircularQueue(ctx)); }
        return CCU_SUCCESS;
    }

} // namespace mc2_ops_hccl
