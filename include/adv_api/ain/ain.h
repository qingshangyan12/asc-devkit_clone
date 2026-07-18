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
 * \file ain.h
 * \brief Ain interface
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_H__
#endif

#ifndef INCLUDE_ADV_API_AIN_AIN_H
#define INCLUDE_ADV_API_AIN_AIN_H

#include "../hcomm/hcomm.h"
#include "ain_common.h"
#include "../../../impl/adv_api/detail/ain/impl/ain_impl_def.h"

namespace AscendC {

/*!
 * @class Ain
 * @brief This class provides device-side one-sided communication primitives (put/get) layered on top
 *        of the Hcomm point-to-point engine. It resolves the per-peer communication channel from the
 *        device communication table, translates symmetric-window handles into remote/local addresses,
 *        and either rings the doorbell immediately or leaves tasks to be completed by a later flush().
 */
template <CommProtocol commProtocol = COMM_PROTOCOL_UBC_CTP>
class Ain {
public:
    /*!
     * @brief Construct an Ain instance bound to a device communication context.
     * @param [in] contextIndex: Index of the communication context to operate on.
     */
    AIN_DEVICE Ain(int contextIndex = 0);
    AIN_DEVICE ~Ain();

    /*!
     * @brief Drain every peer channel in the team, blocking until all pending tasks complete.
     * @param [in] team: The communication team handle.
     */
    AIN_DEVICE void flush(AinTeam team);

    /*!
     * @brief Get the channel handle of a given peer without blocking, for deferred wait.
     * @param [in] team: The communication team handle.
     * @param [in] peer: Peer rank id within the team.
     * @param [out] outChannelHandle: Output the resolved peer channel handle.
     * @note Each rank currently has only one channel; channel index 0 is used.
     */
    AIN_DEVICE void flushAsync(AinTeam team, uint32_t peer, ChannelHandle* outChannelHandle);

    /*!
     * @brief Block until all tasks submitted on the given channel are complete.
     * @param [in] channelHandle: The channel handle returned by flushAsync.
     */
    AIN_DEVICE void wait(ChannelHandle& channelHandle);

    /*!
     * @brief Issue a one-sided write (put) from a local window to a remote peer's window.
     * @param [in] team: The communication team handle.
     * @param [in] peer: Peer rank id within the team.
     * @param [in] dstWin: Destination symmetric window of the peer.
     * @param [in] dstOffset: Byte offset into the destination window.
     * @param [in] srcWin: Source symmetric window of the local rank.
     * @param [in] srcOffset: Byte offset into the source window.
     * @param [in] bytes: Number of bytes to transfer.
     * @param [in] remoteAction: Remote signal action fired after write; AinRemoteNone by default.
     * @param [in] localAction: Local action, reserved; AinLocalNone by default.
     * @param [in] ubuf: UB workspace descriptor for the underlying Hcomm engine.
     * @param [in] commitFlags: AIN_COMMIT_IMMED rings the doorbell immediately;
     *                           AIN_COMMIT_DELAYED defers to a later flush().
     */
    template <
        typename RemoteAction = AinRemoteNone, typename LocalAction = AinLocalNone,
        typename DescriptorUbuf = AinDescriptorUbuf, auto const& Config = URMA_DEFAULT_CFG>
    AIN_DEVICE void put(
        AinTeam team, uint32_t peer, AinCommSymWindow dstWin, uint64_t dstOffset, AinCommSymWindow srcWin,
        uint64_t srcOffset, uint64_t bytes, RemoteAction remoteAction = RemoteAction{},
        LocalAction localAction = LocalAction{}, const DescriptorUbuf& ubuf = DescriptorUbuf{},
        AinCommitFlags commitFlags = AIN_COMMIT_IMMED);

    /*!
     * @brief Issue a one-sided read (get) from a remote peer's window into a local window.
     * @param [in] team: The communication team handle.
     * @param [in] peer: Peer rank id within the team.
     * @param [in] dstWin: Destination symmetric window of the local rank.
     * @param [in] dstOffset: Byte offset into the destination window.
     * @param [in] srcWin: Source symmetric window of the peer.
     * @param [in] srcOffset: Byte offset into the source window.
     * @param [in] bytes: Number of bytes to transfer.
     * @param [in] ubuf: UB workspace descriptor for the underlying Hcomm engine.
     * @param [in] commitFlags: AIN_COMMIT_IMMED rings the doorbell immediately;
     *                           AIN_COMMIT_DELAYED defers to a later flush().
     */
    template <typename DescriptorUbuf = AinDescriptorUbuf, auto const& Config = URMA_DEFAULT_CFG>
    AIN_DEVICE void get(
        AinTeam team, uint32_t peer, AinCommSymWindow dstWin, uint64_t dstOffset, AinCommSymWindow srcWin,
        uint64_t srcOffset, uint64_t bytes, const DescriptorUbuf& ubuf = DescriptorUbuf{},
        AinCommitFlags commitFlags = AIN_COMMIT_IMMED);

    /*!
     * @brief Atomically update a remote peer's signal via Fetch-and-Add.
     * @param [in] team: The communication team handle.
     * @param [in] peer: Peer rank id within the team.
     * @param [in] action: Signal action; AinSignalInc adds 1, AinSignalAdd adds a custom value.
     * @param [in] ubuf: UB workspace descriptor for the underlying Hcomm engine.
     * @param [in] commitFlags: AIN_COMMIT_IMMED rings the doorbell immediately;
     *                           AIN_COMMIT_DELAYED defers to a later flush().
     */
    template <typename RemoteAction, typename DescriptorUbuf = AinDescriptorUbuf, auto const& Config = URMA_DEFAULT_CFG>
    AIN_DEVICE void signal(
        AinTeam team, uint32_t peer, RemoteAction action, const DescriptorUbuf& ubuf = DescriptorUbuf{},
        AinCommitFlags commitFlags = AIN_COMMIT_IMMED);

    /*!
     * @brief Read the local signal value, masked to the lower `bits` bits.
     * @param [in] team: The communication team handle.
     * @param [in] signalId: Signal id, i.e. index into the team workspace signal region.
     * @param [in] bits: Number of low-order bits to read (1-64).
     * @param [in] order: Memory order; currently unused.
     * @return The masked signal value.
     */
    AIN_DEVICE uint64_t
    readSignal(AinTeam team, AinSignal signalId, uint32_t bits, AinMemoryOrder order = AIN_MEMORY_ORDER_RELAX) const;

    /*!
     * @brief Block until the local signal reaches at least `least` (masked to `bits` bits).
     * @param [in] team: The communication team handle.
     * @param [in] signalId: Signal id, i.e. index into the team workspace signal region.
     * @param [in] least: Threshold to wait for.
     * @param [in] bits: Number of low-order bits to compare (1-64).
     * @param [in] order: Memory order; currently unused.
     */
    AIN_DEVICE void waitSignal(
        AinTeam team, AinSignal signalId, uint64_t least, uint32_t bits, AinMemoryOrder order = AIN_MEMORY_ORDER_RELAX) const;

    /*!
     * @brief Get the shadow-memory pointer of a local signal for local read-modify-write.
     * @param [in] team: The communication team handle.
     * @param [in] signalId: Signal id, i.e. index into the team workspace signal region.
     * @return Pointer to the signal slot in shadow memory.
     * @note Used by barrier to locally increment the expected value before waiting.
     */
    AIN_DEVICE __gm__ uint64_t* getSignalShadowPtr(AinTeam team, AinSignal signalId) const;

private:
    int contextIndex_ = 0;
    AscendC::Hcomm<commProtocol> hcomm_;
};

template <CommProtocol commProtocol = COMM_PROTOCOL_UBC_CTP>
class AinBarrierSession {
public:
    /*!
     * @brief Construct a barrier session over the given team at the given barrier resource index.
     * @param [in] ain: The Ain instance used for signal/waitSignal.
     * @param [in] team: The communication team handle.
     * @param [in] index: Barrier resource index; each index isolates an independent barrier.
     * @note The signal base is (signalCount + counterCount) + index * memberNum in the workspace.
     */
    AIN_DEVICE AinBarrierSession(Ain<commProtocol>* ain, AinTeam team, uint32_t index);

    /*!
     * @brief Synchronize with all peers in the team, blocking until completion.
     * @param [in] order: Memory order; currently unused.
     * @param [in] ubuf: UB workspace descriptor for the underlying Hcomm engine.
     * @note Implements a ring barrier: signal every other rank, then waitSignal from each.
     */
    template <typename DescriptorUbuf = AinDescriptorUbuf>
    AIN_DEVICE void sync(AinMemoryOrder order, const DescriptorUbuf& ubuf = DescriptorUbuf{});

    /*!
     * @brief Synchronize with all peers in the team with a timeout.
     * @param [in] order: Memory order; currently unused.
     * @param [in] timeoutCycles: Maximum cycles to wait.
     * @param [in] ubuf: UB workspace descriptor for the underlying Hcomm engine.
     * @return 0 on success, -1 on timeout.
     */
    template <typename DescriptorUbuf = AinDescriptorUbuf>
    AIN_DEVICE int32_t
    sync(AinMemoryOrder order, uint64_t timeoutCycles, const DescriptorUbuf& ubuf = DescriptorUbuf{});

private:
    AinBarrierSessionImpl<commProtocol> barrierImpl_;
};

} // namespace AscendC

#include "../../../impl/adv_api/detail/ain/impl/ain_impl.h"

#endif // INCLUDE_ADV_API_AIN_AIN_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_H__
#endif
