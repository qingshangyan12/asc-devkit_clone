/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file barrier_impl.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/ain/impl/barrier_impl.h is an internal header file and must not be used directly. " \
    "Please use public interface headers.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_IMPL_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_AIN_IMPL_BARRIER_IMPL_H
#define IMPL_ADV_API_DETAIL_AIN_IMPL_BARRIER_IMPL_H

#include "barrier_impl_def.h"

namespace ain {
namespace device {

AIN_DEVICE AinBarrierSession::AinBarrierSession(
    Ain& ain, HcclTeam team, HcclAinBarrierHandle handle, uint32_t index 
    ):
    AinBarrierSession_internal{ain, team, handle, (int)index} {
    this->signal = handle.signal0 + index * team.nRanks;
}

// AIN_DEVICE AinBarrierSession::AinBarrierSession(
//     Ain& ain, HcclTeamTagWorld, uint32_t index 
//     ):
//     AinBarrierSession(ain, HcclTeamWorld(ain.devComm_), ain.devComm_.AivRes->ainWorldBarrier, index) {
// }

// AIN_DEVICE HcclResult AinBarrierSession::AinBarrierSession(
//     Ain& ain, HcclTeamTagRail, uint32_t index 
//     ):
//     AinBarrierSession(ain, HcclTeamRail(ain.devComm_), ain.devComm_.AivRes->ainRailBarrier, index) {
// }

AIN_DEVICE void AinBarrierSession::sync(AinMemoryOrder order) {
    this->template syncInternal<false>(order, 0);
}

AIN_DEVICE HcclResult AinBarrierSession::sync(AinMemoryOrder order, uint64_t timeoutCycles) {
    return this->template syncInternal<true>(order, timeoutCycles);
}

template<bool EnableTimeout>
AIN_DEVICE HcclResult AinBarrierSession_internal::syncInternal(AinMemoryOrder order, uint64_t timeoutCycles)
{
    uint64_t startCycle;
    HcclResult ret = HCCL_SUCCESS;   // TODO

    int32_t vecId = AscendC::GetBlockIdx();    // vec核ID
    int32_t vecSize = AscendC::GetBlockNum() * AscendC::GetTaskRation();  // AIV核数

    // 全核同步
    AscendC::SyncAll<true>(); 

    if constexpr (EnableTimeout) {
        startCycle = AscendC::GetSystemCycle();   // 记录开始时间
    }

    if ASCEND_IS_AIV {
        for (int32_t i = vecId; i < this->team.nRanks - 1; i += vecSize) { 
            int peer = 1 + this->team.rank + i;
            if (this->team.nRanks <= peer) {
                peer -= this->team.nRanks;
            }
            
            this->ain.signal(
                this->team, peer, 
                AinSignalInc{this->signal + this->team.rank} 
                // DescriptorUb descriptor = AinNoDescriptorUb(),  // 可选临时UB描述符空间
                // AinCommitFlags commitFlags = AIN_COMMIT_IMMED  // 是否延后提交WQE doorbell
            );

            uint32_t* shadowPtr = (uint32_t*)this->ain.getSignalShadowPtr(this->signal + peer);
            uint32_t waitVal = ++(*shadowPtr);

            if constexpr (EnableTimeout) {
                while (true) {
                    uint64_t got = this->ain.readSignal(
                        this->signal + peer,   // signalId
                        32,   // bits  读取有效位宽
                        // AinMemoryOrder order = AIN_MEMORY_ORDER_ACQUIRE // 读取后的本地内存序
                    );
                    if (got >= waitVal) { // TODO：是否替换rollingLessEq
                        break;
                    }
                    uint64_t endCycle = AscendC::GetSystemCycle();
                    if (endCycle - startCycle >= timeoutCycles) {
                        ret = HCCL_TIMEOUT;
                        break;
                    }
                }
            } else {
                this->ain.waitSignal(this->signal + peer, waitVal, 32   // bits  读取有效位宽
                        // AinMemoryOrder order = AIN_MEMORY_ORDER_ACQUIRE // 读取后的本地内存序 
                );
            }
            
        }
    }

    // 全核同步
    AscendC::SyncAll<true>();
    return ret;
}

}  // namespace device
}  // namespace ain

#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_IMPL_H__
#endif