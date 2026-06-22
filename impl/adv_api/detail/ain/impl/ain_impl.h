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
    "impl/adv_api/detail/ain/impl/ain_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/ain/ain.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H
#define IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_H

#include "../../hcomm/common/hcomm_inner_def.h"

namespace AscendC {

AIN_DEVICE Ain::Ain(__gm__ void *devComm, int contextIndex)
    : devComm_(reinterpret_cast<uint64_t>(devComm)), contextIndex_(contextIndex) {}

AIN_DEVICE Ain::~Ain() {}

AIN_DEVICE void Ain::flush()
{
    for (uint32_t i = 0; i < pendingChannelCount_; ++i) {
        hcomm_.Drain(pendingChannels_[i]);
    }
    pendingChannelCount_ = 0;
}

template <typename RemoteAction, typename LocalAction, typename DescriptorUbuf>
AIN_DEVICE void Ain::put(__gm__ void *team, uint32_t peer, __gm__ void *dstWin, uint64_t dstOffset,
                         __gm__ void *srcWin, uint64_t srcOffset, uint64_t bytes, const DescriptorUbuf &ubuf,
                         RemoteAction remoteAction, LocalAction localAction, AinCommitFlags commitFlags)
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
    size_t actualPos = 4 + peer;
    uint64_t channel = devCommTable[actualPos];
    auto channelEntity = reinterpret_cast<__gm__ ChannelEntity *>(devCommTable[actualPos]);

    auto remoteAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->remoteBufferAddr[dstWinIdx].bufferInfo.rma.addr + dstOffset);
    auto localAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->localBufferAddr[srcWinIdx].bufferInfo.rma.addr + srcOffset);

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.WriteNbi<false>(channel, remoteAddr, localAddr, bytes);
        record_channel(channel);
    } else {
        hcomm_.WriteNbi(channel, remoteAddr, localAddr, bytes);
    }
}

template <typename DescriptorUbuf>
AIN_DEVICE void Ain::get(__gm__ void *team, uint32_t peer, __gm__ void *dstWin, uint64_t dstOffset,
                         __gm__ void *srcWin, uint64_t srcOffset, uint64_t bytes, const DescriptorUbuf &ubuf,
                         AinCommitFlags commitFlags)
{
    static_assert(!AinIsSame<DescriptorUbuf, AinDescriptorUbufNone>::value,
                  "get requires a valid DescriptorUbuf; AinDescriptorUbufNone is not allowed");
    (void)team;
    hcomm_.Init(ubuf.addr, ubuf.bytes);

    uint32_t dstWinIdx = decode_window(dstWin);
    uint32_t srcWinIdx = decode_window(srcWin);
    auto devCommTable = reinterpret_cast<__gm__ uint64_t *>(devComm_);
    size_t actualPos = 4 + peer;
    uint64_t channel = devCommTable[actualPos];
    auto channelEntity = reinterpret_cast<__gm__ ChannelEntity *>(devCommTable[actualPos]);

    auto remoteAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->remoteBufferAddr[dstWinIdx].bufferInfo.rma.addr + dstOffset);
    auto localAddr = reinterpret_cast<__gm__ uint8_t *>(
        channelEntity->localBufferAddr[srcWinIdx].bufferInfo.rma.addr + srcOffset);

    if (commitFlags == AIN_COMMIT_DELAYED) {
        hcomm_.ReadNbi<false>(channel, remoteAddr, localAddr, bytes);
        record_channel(channel);
    } else {
        hcomm_.ReadNbi(channel, remoteAddr, localAddr, bytes);
    }
}

AIN_DEVICE uint32_t Ain::decode_window(__gm__ void *window) const
{
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(window));
}

AIN_DEVICE void Ain::record_channel(uint64_t channelId)
{
    for (uint32_t i = 0; i < pendingChannelCount_; ++i) {
        if (pendingChannels_[i] == channelId) {
            return;
        }
    }

    if (pendingChannelCount_ >= MAX_PENDING_CHANNELS) {
        flush();
    }
    pendingChannels_[pendingChannelCount_++] = channelId;
}

AIN_DEVICE AinBarrierSession::AinBarrierSession(Ain *ain, int32_t team, uint32_t index)
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
AIN_DEVICE void AinBarrierSession::sync(int32_t memOrder, const DescriptorUbuf &ubuf)
{
    (void)memOrder;
    auto devCommTable = reinterpret_cast<__gm__ uint64_t *>(ain_->devComm_);
    uint32_t myRank = devCommTable[0];
    uint32_t rankSize = devCommTable[1];
    uint32_t windowIdx = devCommTable[2];

    int32_t vec_id = GetBlockIdx();
    int32_t vec_size = GetBlockNum() * GetTaskRation();

    int32_t k = 8;
    k = k < static_cast<int32_t>(rankSize) ? k : static_cast<int32_t>(rankSize);
    k = k < vec_size ? k : vec_size;

    Hcomm<COMM_PROTOCOL_UBC_CTP> hcomm;
    hcomm.Init(ubuf.addr, ubuf.bytes);

    auto shadowSignalBuf = reinterpret_cast<__gm__ uint64_t *>(devCommTable[3]);
    SyncAll<true>();
    if ASCEND_IS_AIV {
        for (int32_t i = vec_id; i < static_cast<int32_t>(rankSize); i += k) {
            if (i == static_cast<int32_t>(myRank)) {
                continue;
            } else {
                shadowSignalBuf[i] += 1;
                dcci_cacheline(reinterpret_cast<__gm__ uint8_t *>(shadowSignalBuf + i));
                auto channelEntity = reinterpret_cast<__gm__ ChannelEntity *>(devCommTable[i + 4]);
                auto remoteAddr = reinterpret_cast<__gm__ uint8_t *>(
                    channelEntity->remoteBufferAddr[windowIdx].bufferInfo.rma.addr);
                hcomm.WriteNbi(devCommTable[i + 4], remoteAddr + myRank * sizeof(uint64_t),
                               reinterpret_cast<__gm__ uint8_t *>(shadowSignalBuf + i), sizeof(uint64_t));
                hcomm.Drain(devCommTable[i + 4]);
            }
        }

        for (int32_t i = vec_id; i < static_cast<int32_t>(rankSize); i += k) {
            if (i == static_cast<int32_t>(myRank)) {
                continue;
            } else {
                auto channelEntity = reinterpret_cast<__gm__ ChannelEntity *>(devCommTable[i + 4]);
                auto localAddr = reinterpret_cast<__gm__ uint64_t *>(
                    channelEntity->localBufferAddr[windowIdx].bufferInfo.rma.addr);
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
