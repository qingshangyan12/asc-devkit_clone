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
#pragma message(                                                                                         \
    "impl/adv_api/detail/ain/impl/ain_impl.h is an internal header file and must not be used directly. " \
    "Please use public interface headers.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H
#define IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H

#include "../../hcomm/common/hcomm_inner_def.h"

namespace AscendC {

constexpr uint32_t MY_RANK_POS = 0;
constexpr uint32_t RANK_SIZE_POS = 1;
constexpr uint32_t SIGNAL_POS = 2; // 共享signal起始索引
constexpr uint32_t SIGNAL_BUF_POS = 3; // 本地共享signal内存地址
constexpr uint32_t SHADOW_SIGNAL_BUF_POS = 4;
constexpr uint32_t CHANNEL_POS = 5;

enum AinSignalType {
    AIN_SIGNAL_TYPE_NONE,
    AIN_SIGNAL_TYPE_VA,
    AIN_SIGNAL_TYPE_INDEXED,
};

typedef struct {
    AinSignalType type;
    union {
        struct {
        } vaSignal;

        struct {
            AinSignal signalId;

        } indexedSignal;
    };
} AinSignalDescriptor;

template <typename T, typename U>
struct AinIsSame { static constexpr bool value = false; };
template <typename T>
struct AinIsSame<T, T> { static constexpr bool value = true; };

template <unsigned commEngineMask>
AIN_DEVICE Ain::Ain(AinDevComm devComm, int contextIndex)
    : devComm_(reinterpret_cast<uint64_t>(devComm)), contextIndex_(contextIndex) {}

AIN_DEVICE Ain::~Ain() {}

AIN_DEVICE void Ain::flush()
{
    auto devCommTable = reinterpret_cast<__gm__ uint64_t *>(devComm_);
    uint32_t myRank = devCommTable[MY_RANK_POS];
    uint32_t rankSize = devCommTable[RANK_SIZE_POS];
    for (uint32_t i = 0; i < rankSize; ++i) {
        if (i == myRank) {
            continue;
        }
        hcomm_.Drain(devCommTable[i + CHANNEL_POS]);
    }
}

template <typename RemoteAction, typename LocalAction, typename DescriptorUbuf>
AIN_DEVICE void Ain::put(AinTeam team, uint32_t peer, AinCommSymWindow dstWin, uint64_t dstOffset,
                         AinCommSymWindow srcWin, uint64_t srcOffset, uint64_t bytes, RemoteAction remoteAction,
                         LocalAction localAction, const DescriptorUbuf &ubuf, AinCommitFlags commitFlags)
{
    static_assert(!AinIsSame<DescriptorUbuf, AinDescriptorUbufNone>::value,
                  "put requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    (void)team;
    (void)remoteAction;
    (void)localAction;
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    uint32_t dstWinIdx = decode_window(dstWin);
    uint32_t srcWinIdx = decode_window(srcWin);
    auto devCommTable = reinterpret_cast<__gm__ uint64_t *>(devComm_);
    size_t actualPos = CHANNEL_POS + peer;
    uint64_t channel = devCommTable[actualPos];
    auto channelEntity = reinterpret_cast<__gm__ ChannelEntity *>(devCommTable[actualPos]);

    auto remoteAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->remoteBufferAddr[dstWinIdx].bufferInfo.rma.addr + dstOffset);
    auto localAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->localBufferAddr[srcWinIdx].bufferInfo.rma.addr + srcOffset);

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.WriteNbi<false>(channel, remoteAddr, localAddr, bytes);
    } else {
        hcomm_.WriteNbi(channel, remoteAddr, localAddr, bytes);
    }
}

template <typename DescriptorUbuf>
AIN_DEVICE void Ain::get(AinTeam team, uint32_t peer, AinCommSymWindow dstWin, uint64_t dstOffset,
                         AinCommSymWindow srcWin, uint64_t srcOffset, uint64_t bytes, const DescriptorUbuf &ubuf,
                         AinCommitFlags commitFlags)
{
    static_assert(!AinIsSame<DescriptorUbuf, AinDescriptorUbufNone>::value,
                  "get requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    (void)team;
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    uint32_t dstWinIdx = decode_window(dstWin);
    uint32_t srcWinIdx = decode_window(srcWin);
    auto devCommTable = reinterpret_cast<__gm__ uint64_t *>(devComm_);
    size_t actualPos = CHANNEL_POS + peer;
    uint64_t channel = devCommTable[actualPos];
    auto channelEntity = reinterpret_cast<__gm__ ChannelEntity *>(devCommTable[actualPos]);

    auto remoteAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->remoteBufferAddr[dstWinIdx].bufferInfo.rma.addr + dstOffset);
    auto localAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->localBufferAddr[srcWinIdx].bufferInfo.rma.addr + srcOffset);

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.ReadNbi<false>(channel, remoteAddr, localAddr, bytes);
    } else {
        hcomm_.ReadNbi(channel, remoteAddr, localAddr, bytes);
    }
}

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
    if constexpr (IsSameType<T, AinSignalInc>::value) {
        return 1;
    } else {
        return action.value;
    }
}

AIN_DEVICE uint32_t Ain::decode_window(__gm__ AinCommSymWindow window) const
{
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(window));
}

AIN_DEVICE AinBarrierSession::AinBarrierSession(Ain *ain, AinTeam team, uint32_t index)
    : ain_(ain), team_(team), index_(index) {}

AIN_DEVICE AinBarrierSession::~AinBarrierSession() {}

AIN_DEVICE void AinBarrierSession::dcci_cacheline(__gm__ uint8_t *addr)
{
    GlobalTensor<uint8_t> global;
    global.SetGlobalBuffer(addr);

    // Important: add hint to avoid dcci being optimized by compiler
    __asm__ __volatile__("" ::: "memory");
    DataCacheCleanAndInvalid<uint8_t, CacheLine::SINGLE_CACHE_LINE, DcciDst::CACHELINE_OUT>(global);
    __asm__ __volatile__("" ::: "memory");
}

template <typename DescriptorUbuf>
AIN_DEVICE void AinBarrierSession::sync(AinMemoryOrder memOrder, const DescriptorUbuf &ubuf)
{
    (void)memOrder;
    auto devCommTable = reinterpret_cast<__gm__ uint64_t *>(ain_->devComm_);
    uint32_t myRank = devCommTable[MY_RANK_POS];
    uint32_t rankSize = devCommTable[RANK_SIZE_POS];
    uint32_t signalPos = devCommTable[SIGNAL_POS];

    Hcomm<COMM_PROTOCOL_UBC_CTP> hcomm;
    hcomm.Init(ubuf.addr, ubuf.bytes);

    auto shadowSignalBuf = reinterpret_cast<__gm__ uint64_t *>(devCommTable[SHADOW_SIGNAL_BUF_POS]);
    SyncAll<true>();
    if ASCEND_IS_AIV {
        for (int32_t i = 0; i < static_cast<int32_t>(rankSize); i += 1) {
            if (i == static_cast<int32_t>(myRank)) {
                continue;
            } else {
                shadowSignalBuf[i] += 1;
                dcci_cacheline(reinterpret_cast<__gm__ uint8_t *>(shadowSignalBuf + i));
                auto channelEntity = reinterpret_cast<__gm__ ChannelEntity *>(devCommTable[i + CHANNEL_POS]);
                auto remoteAddr = reinterpret_cast<__gm__ uint8_t *>(
                    channelEntity->remoteBufferAddr[signalPos].bufferInfo.rma.addr);
                hcomm.WriteNbi(devCommTable[i + CHANNEL_POS], remoteAddr + myRank * sizeof(uint64_t),
                               reinterpret_cast<__gm__ uint8_t *>(shadowSignalBuf + i), sizeof(uint64_t));
                hcomm.Drain(devCommTable[i + CHANNEL_POS]);
            }
        }

        for (int32_t i = 0; i < static_cast<int32_t>(rankSize); i += 1) {
            if (i == static_cast<int32_t>(myRank)) {
                continue;
            } else {
                auto channelEntity = reinterpret_cast<__gm__ ChannelEntity *>(devCommTable[i + CHANNEL_POS]);
                auto localAddr = reinterpret_cast<__gm__ uint64_t *>(
                    channelEntity->localBufferAddr[signalPos].bufferInfo.rma.addr);
                do {
                    dcci_cacheline(reinterpret_cast<__gm__ uint8_t *>(localAddr + i));
                    if (*(localAddr + i) >= shadowSignalBuf[i]) {
                        break;
                    }
                } while (true);
            }
        }
    }
    SyncAll<true>();
}

} // namespace AscendC

#endif // IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif
