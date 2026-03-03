/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file hccl_ccu_v1.h
 * \brief CCU implementation for NPU ARCH 9201
 */
#ifndef IMPL_V310_HCCL_CCU_V1_H
#define IMPL_V310_HCCL_CCU_V1_H

#include "hccl_ccu_base.h"
#include "../../ccu/hccl_ccu_v0_prepare.h"

namespace AscendC {

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::InitInner(GM_ADDR context, HcclTilingVersion version)
{
    InitInnerBase(context, version);

    if (workingFlag_) {
        uint64_t phyAddr = 0;
        if (g_coreType == AscendC::AIV) {
            phyAddr = HW_BASE_ADDR + get_only_coreid() * HW_CORE_OFFSET +
                HW_AIV_ADDR_OFFSET + HW_CORE_SUB_OFFSET + SPR_ID * SPR_OFFSET;
        } else {
            phyAddr = HW_BASE_ADDR + get_only_coreid() * HW_CORE_OFFSET + HW_CORE_SUB_OFFSET + SPR_ID * SPR_OFFSET;
        }
        set_ipc_scb_15(0x0);
        WriteHBMData(hcclContext_->sprOffset, phyAddr);
        WriteHBMData(hcclContext_->sprOffset + sizeof(uint64_t), HSCB_ADD);
    }
    isInited_ = true;
}

template<const auto &config>
template<pipe_t pipe>
__aicore__ inline int32_t HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::Wait(HcclHandle handleId)
{
    ASCENDC_HCCL_API_ASSERT(isInited_, { return HCCL_FAILED; },
        "Call Wait failed, please ensure Hccl::Init func has been called successfully already!");
    ASCENDC_HCCL_API_ASSERT(handleCommitCnt_[handleId] > 0, { return HCCL_FAILED; },
        "Call Wait failed,  commitCnt [%u] is invalid.", handleCommitCnt_[handleId]);
    ASCENDC_HCCL_API_ASSERT(handleId >INVALID_HANDLE_ID && handleId < HCCL_MAX_HANDLE_ID, { return HCCL_FAILED; },
        "Call Wait failed,  chandleId is[%d], expected in range of [0, %d).", handleId, HCCL_MAX_HANDLE_ID);
    ASCENDC_HCCL_API_ASSERT(handleId < curHandleId_, { return HCCL_FAILED; },
        "Call Wait failed,  handleId = %u is invalid please call Preapre Interface before Wait.", handleId);

    if (unlikely(handleId >= HCCL_MAX_HANDLE_ID || handleId <= INVALID_HANDLE_ID)) {
        KERNEL_LOG(KERNEL_ERROR,  "Call Wait failed, handleId[%u] is invalid.", handleId);
        return HCCL_FAILED;
    }
    if (unlikely(handleNeedCommitCnt_[handleId] <= 0)) {
        KERNEL_LOG(KERNEL_ERROR,  "Call Wait failed, handleNeedCommitCnt_[%u] = %u is invalid.",
                handleId, handleNeedCommitCnt_[handleId]);
        return HCCL_FAILED;
    }

    handleWaitCnt_[handleId]++;

#ifndef __CCE_KT_TEST__ 
    do {
        uint64_t fCnt = handleId == 0 ? handleWaitCnt_[handleId] :
            handleFinishCnt_[handleId - 1] + handleWaitCnt_[handleId];
        if (!workingFlag_) {
            if (ReadHBMData(finishCntGM_) >= fCnt) {
                break;
            }
        }
        if (workingFlag_) {
            __wait_ast_scb(SPR_ID, fCnt, pipe);
            finishNumTemp_++;
            WriteHBMData(finishCntGM_, finishNumTemp_);
            break;
        }
    } while (true);
#endif

    WaitDone(handleId);
    return HCCL_SUCCESS;
}

}
#endif
