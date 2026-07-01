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
 * \file ain_impl.h
 * \brief Ain implementation
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/ain/impl/ain_impl.h is an internal header file and must not be used directly. " \
    "Please use public interface headers.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H
#define IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H

#include "ain_impl_def.h"

namespace AscendC {

template <typename T>
AIN_DEVICE AinSignalDescriptor GetSignalDescriptor(const Ain& ain, T action)
{
    (void)ain;
    AinSignalDescriptor desc{};
    desc.type = AIN_SIGNAL_TYPE_INDEXED;
    desc.indexedSignal.signalId = action.signalId;
    return desc;
}

template <typename T>
AIN_DEVICE uint64_t GetSignalOpArg(T action)
{
    if constexpr (AinIsSame<T, AinSignalInc>::value) {
        return 1;
    } else {
        return action.value;
    }
}

template <typename RemoteAction, typename DescriptorUbuf>
AIN_DEVICE void Ain::signal(HcclTeam team, uint32_t peer, RemoteAction action, const DescriptorUbuf &ubuf,
    AinCommitFlags commitFlags)
{
    static_assert(!AinIsSame<DescriptorUbuf, AinDescriptorUbufNone>::value,
        "signal requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    AinSignalDescriptor signalDesc = GetSignalDescriptor(*this, action);
    uint64_t signalOpArg = GetSignalOpArg(action);

    auto hcclDevComm = reinterpret_cast<__gm__ HcclDevComm *>(devComm_);
    auto aivRes = reinterpret_cast<AivRes *>(reinterpret_cast<uint64_t>(hcclDevComm->AivRes));
    ChannelHandle channel = aivRes->entity[peer][0];

    uint64_t signalOffset = signalDesc.indexedSignal.signalId * sizeof(uint64_t);
    auto remoteSignal = reinterpret_cast<__gm__ uint8_t *>(
        reinterpret_cast<uintptr_t>(aivRes->symMemPool) + signalOffset);

    auto amoSignalId = aivRes->ainWorldBarrier.signal0 + team.rank;
    uint64_t amoOffset = amoSignalId * sizeof(uint64_t);
    auto amoSignal = reinterpret_cast<__gm__ uint8_t *>(
        reinterpret_cast<uintptr_t>(aivRes->symMemPool) + amoOffset);

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.AtomicFAA<uint64_t, false>(channel, remoteSignal, amoSignal, signalOpArg);
    } else {
        hcomm_.AtomicFAA(channel, remoteSignal, amoSignal, signalOpArg);
    }
}

AIN_DEVICE uint64_t Ain::readSignal(HcclAinSignal_t signalId, uint32_t bits, AinMemoryOrder order) const
{
    (void)order;
    auto hcclDevComm = reinterpret_cast<__gm__ HcclDevComm *>(devComm_);
    auto aivRes = reinterpret_cast<AivRes *>(reinterpret_cast<uint64_t>(hcclDevComm->AivRes));
    uint64_t signalOffset = signalId * sizeof(uint64_t);
    __gm__ uint64_t *ptr = reinterpret_cast<__gm__ uint64_t *>(
        reinterpret_cast<uintptr_t>(aivRes->symMemPool) + signalOffset);
    uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
    uint64_t got = ld_dev(ptr, 0);
    return mask & got;
}

AIN_DEVICE void Ain::waitSignal(HcclAinSignal_t signalId, uint64_t least, uint32_t bits, AinMemoryOrder order) const
{
    (void)order;
    auto hcclDevComm = reinterpret_cast<__gm__ HcclDevComm *>(devComm_);
    auto aivRes = reinterpret_cast<AivRes *>(reinterpret_cast<uint64_t>(hcclDevComm->AivRes));
    uint64_t signalOffset = signalId * sizeof(uint64_t);
    __gm__ uint64_t *ptr = reinterpret_cast<__gm__ uint64_t *>(
        reinterpret_cast<uintptr_t>(aivRes->symMemPool) + signalOffset);
    uint64_t mask = bits >= 64U ? UINT64_MAX : ((1ULL << bits) - 1ULL);
    while (true) {
        uint64_t got = ld_dev(ptr, 0);
        if (least <= (got & mask)) {
            break;
        }
    }
}

AIN_DEVICE __gm__ uint64_t* Ain::getSignalShadowPtr(HcclAinSignal_t signalId) const
{
    auto hcclDevComm = reinterpret_cast<__gm__ HcclDevComm *>(devComm_);
    auto aivRes = reinterpret_cast<AivRes *>(reinterpret_cast<uint64_t>(hcclDevComm->AivRes));
    uint64_t signalOffset = signalId * sizeof(uint64_t);
    return reinterpret_cast<__gm__ uint64_t *>(
        reinterpret_cast<uintptr_t>(aivRes->ainSignalShadows) + signalOffset);
}

AIN_DEVICE uint64_t Ain::getDevComm() const
{
    return this->devComm_;
}

}  // namespace AscendC

#endif  // IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif
