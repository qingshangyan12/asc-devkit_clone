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
 * \file hccl_ccu_v0.h
 * \brief CCU implementation for NPU ARCH 3101
 */
#ifndef IMPL_V310_HCCL_CCU_V0_H
#define IMPL_V310_HCCL_CCU_V0_H

#include "hccl_ccu_base.h"
#include "../../ccu/hccl_ccu_v0_prepare.h"

namespace AscendC {

template<const auto &config>
__aicore__ inline void
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::InitInner(GM_ADDR context, HcclTilingVersion version)
{
    InitInnerBase(context, version);
    isInited_ = true;
}

template<const auto &config>
template<pipe_t pipe>
__aicore__ inline int32_t HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::Wait(HcclHandle handleId)
{
    auto ret = CheckWaitParams(handleId);
    if (ret) {
        return ret;
    }

    uint8_t reqId = handleReqId_[handleId] + handleFinishCnt_[handleId];
    reqId %= CCU_MAX_MSG_NUM;
    GM_ADDR waitCKEAddr = GetWaitCkeAddr(reqId);

#ifndef __CCE_KT_TEST__ 
    while(true) {
        uint64_t waitCke = ReadHBMData(waitCKEAddr);
        if (waitCke != 0) {
            if (workingFlag_) {
                finishNumTemp_++;
                WriteHBMData(finishCntGM_, finishNumTemp_);
                WriteHBMData(waitCKEAddr, CCU_MSG_CKE_INIT_VALUE);
            }
            break;
        }
        if (!workingFlag_) {
            uint64_t finshCnt = ReadHBMData(finishCntGM_);
            if (finshCnt > finishNum_) {
                break;
            }
        }
    }
#endif

    WaitDone(handleId, reqId);
    KERNEL_LOG(KERNEL_INFO, "ApiClient Wait finish finishCntGM_:%d, finishCnt_:%d", *finishCntGM_, handleFinishCnt_[handleId]);
    return HCCL_SUCCESS;
}

}
#endif
