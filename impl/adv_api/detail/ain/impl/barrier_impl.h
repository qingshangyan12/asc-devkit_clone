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

namespace AscendC {

AIN_DEVICE AinBarrierSession::AinBarrierSession(
    Ain* ain, AinTeamTagWorld, uint32_t index)
{
    auto devComm = reinterpret_cast<__gm__ HcclDevComm *>(ain->devComm_);
    auto aivRes = reinterpret_cast<AivRes *>(reinterpret_cast<uint64_t>(devComm->AivRes));
    this->ain = ain;
    this->team = AinTeamWorld(devComm);
    this->handle = aivRes->ainWorldBarrier;
    this->index = index;
    this->signal = this->handle.signal0 + index * this->team.nRanks;
}

template<typename DescriptorUbuf>
AIN_DEVICE void AinBarrierSession::sync(AinMemoryOrder order, const DescriptorUbuf &ubuf) {
    this->template syncInternal<false>(order, 0);
}

template<typename DescriptorUbuf>
AIN_DEVICE int32_t AinBarrierSession::sync(AinMemoryOrder order, uint64_t timeoutCycles, const DescriptorUbuf &ubuf) {
    return this->template syncInternal<true>(order, timeoutCycles);
}

template<bool EnableTimeout, typename DescriptorUbuf>
AIN_DEVICE int32_t AinBarrierSessionImpl::syncInternal(AinMemoryOrder order, uint64_t timeoutCycles, 
    const DescriptorUbuf &ubuf)
{
    uint64_t startCycle;
    if constexpr (EnableTimeout) {
        startCycle = AscendC::GetSystemCycle(); 
    }

    for (int32_t i = 0; i < this->team.nRanks - 1; ++i) {
        int peer = 1 + this->team.rank + i;
        if (this->team.nRanks <= peer) {
            peer -= this->team.nRanks;
        }
        
        this->ain->signal(this->team, peer, AinSignalInc{this->signal + this->team.rank}, ubuf);
        this->ain->flush();

        __gm__ uint32_t* shadowPtr = (__gm__ uint32_t*)this->ain->getSignalShadowPtr(this->signal + peer);
        uint32_t waitVal = ++(*shadowPtr);

        if constexpr (EnableTimeout) {
            while (true) {
                uint64_t got = this->ain->readSignal(this->signal + peer, 32);
                if (got >= waitVal) {
                    break;
                }
                uint64_t endCycle = AscendC::GetSystemCycle();
                if (endCycle - startCycle >= timeoutCycles) {
                    return -1;
                }
            }
        } else {
            this->ain->waitSignal(this->signal + peer, waitVal, 32);
        }
    }
    
    return 0;
}

}  // namespace AscendC

#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_IMPL_H__
#endif