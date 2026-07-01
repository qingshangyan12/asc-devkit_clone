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

namespace AscendC{

AIN_DEVICE AinBarrierSession::AinBarrierSession(
    Ain* ain, HcclTeamTagWorld, uint32_t index) 
{
    auto devComm = reinterpret_cast<__gm__ HcclDevComm *>(ain->getDevComm());
    auto aivRes = reinterpret_cast<AivRes *>(reinterpret_cast<uint64_t>(devComm->AivRes));
    this->ain = ain;
    this->team = HcclTeamWorld(devComm);
    this->handle = aivRes->ainWorldBarrier;
    this->index = index;
    this->signal = this->handle.signal0 + index * this->team.nRanks; 
}

AIN_DEVICE void AinBarrierSession::sync(AinMemoryOrder order) {
    this->template syncInternal<false>(order, 0);
}

AIN_DEVICE HcclResult AinBarrierSession::sync(AinMemoryOrder order, uint64_t timeoutCycles) {
    return this->template syncInternal<true>(order, timeoutCycles);
}

template<bool EnableTimeout>
AIN_DEVICE HcclResult AinBarrierSessionImpl::syncInternal(AinMemoryOrder order, uint64_t timeoutCycles)
{
    AscendC::LocalTensor<uint8_t> hcommLocal = AscendC::LocalTensor<uint8_t>(
        AscendC::TPosition::VECOUT, 0, AscendC::HCOMM_URMA_TMP_BUF_SIZE);    
    AscendC::AinDescriptorUbuf hcommUbuf{
        reinterpret_cast<__ubuf__ uint8_t*>(hcommLocal.GetPhyAddr()),
        AscendC::HCOMM_URMA_TMP_BUF_SIZE,
        0
    };

    uint64_t startCycle;
    HcclResult ret = HcclResult::HCCL_SUCCESS;   // 这里需要 #include "hccl_res.h"，删除？

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
            
            this->ain->signal(this->team, peer, AinSignalInc{this->signal + this->team.rank}, hcommUbuf);
            this->ain->flush();  // 需要吗？

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
                        ret = HcclResult::HCCL_E_TIMEOUT;
                        break;
                    }
                }
            } else {
                this->ain->waitSignal(this->signal + peer, waitVal, 32);
            }
            
        }
    }

    // 全核同步
    AscendC::SyncAll<true>();
    return ret;
}

}  // namespace AscendC

#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_IMPL_H__
#endif