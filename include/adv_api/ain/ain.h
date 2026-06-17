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
#ifndef INCLUDE_ADV_API_AIN_AIN_H
#define INCLUDE_ADV_API_AIN_AIN_H

#include "kernel_basic_intf.h"
#include "ain_common.h"
#include "../hcomm/hcomm.h"

namespace AscendC {

/*!
 * @class Ain
 * @brief This class provides device-side one-sided communication primitives (put/get) layered on top
 *        of the Hcomm point-to-point engine. It resolves the per-peer communication channel from the
 *        device communication table, translates symmetric-window handles into remote/local addresses,
 *        and either rings the doorbell immediately or batches tasks for a later flush().
 *        The typical usage of this class is as follows:
 *          1) Construct Ain from the device communication context.
 *          2) Issue put/get tasks against a peer, optionally with AIN_COMMIT_DELAYED to batch them.
 *          3) Call flush() to drain all channels that have pending delayed tasks.
 * @note All members run on the AICORE and are always inlined (see AIN_DEVICE).
 */
class Ain {
public:
    /*!
     * @brief Construct an Ain instance bound to a device communication context.
     * @param [in] devComm: Pointer to the device communication table in global memory.
     * @param [in] contextIndex: Index of the communication context to operate on.
     */
    AIN_DEVICE Ain(__gm__ void *devComm, int contextIndex);
    AIN_DEVICE ~Ain();

    /*!
     * @brief Drain every channel that has pending (delayed) tasks, then clear the pending set.
     * @note Must be called to guarantee completion of tasks issued with AIN_COMMIT_DELAYED.
     */
    AIN_DEVICE void flush();

    /*!
     * @brief Issue a one-sided write (put) of len bytes from a local window to a remote peer's window.
     * @tparam RemoteAction: Optional remote-side action tag, AinRemoteNone by default.
     * @tparam LocalAction: Optional local-side action tag, AinLocalNone by default.
     * @tparam DescriptorUbuf: UB workspace descriptor type, AinDescriptorUbuf by default.
     * @param [in] team: Communication team handle (reserved; currently unused).
     * @param [in] peer: Destination rank id.
     * @param [in] dstWin: Remote symmetric-window handle.
     * @param [in] dstOffset: Byte offset within the remote window.
     * @param [in] srcWin: Local symmetric-window handle.
     * @param [in] srcOffset: Byte offset within the local window.
     * @param [in] bytes: Number of bytes to transfer.
     * @param [in] ubuf: UB workspace used by the underlying Hcomm engine.
     * @param [in] remoteAction: Optional remote action argument.
     * @param [in] localAction: Optional local action argument.
     * @param [in] commitFlags: AIN_COMMIT_IMMED to ring the doorbell now, AIN_COMMIT_DELAYED to batch.
     * @note Requires a valid DescriptorUbuf; AinDescriptorUbufNone is rejected at compile time.
     */
    template <typename RemoteAction = AinRemoteNone, typename LocalAction = AinLocalNone,
              typename DescriptorUbuf = AinDescriptorUbuf>
    AIN_DEVICE void put(__gm__ void *team, uint32_t peer, __gm__ void *dstWin, uint64_t dstOffset,
                        __gm__ void *srcWin, uint64_t srcOffset, uint64_t bytes,
                        const DescriptorUbuf &ubuf = DescriptorUbuf{},
                        RemoteAction remoteAction = AinRemoteNone{}, LocalAction localAction = AinLocalNone{},
                        AinCommitFlags commitFlags = AIN_COMMIT_IMMED);

    /*!
     * @brief Issue a one-sided read (get) of len bytes from a remote peer's window into a local window.
     * @tparam DescriptorUbuf: UB workspace descriptor type, AinDescriptorUbuf by default.
     * @param [in] team: Communication team handle (reserved; currently unused).
     * @param [in] peer: Source rank id.
     * @param [in] dstWin: Local symmetric-window handle (destination of the read).
     * @param [in] dstOffset: Byte offset within the destination window.
     * @param [in] srcWin: Remote symmetric-window handle (source of the read).
     * @param [in] srcOffset: Byte offset within the source window.
     * @param [in] bytes: Number of bytes to transfer.
     * @param [in] ubuf: UB workspace used by the underlying Hcomm engine.
     * @param [in] commitFlags: AIN_COMMIT_IMMED to ring the doorbell now, AIN_COMMIT_DELAYED to batch.
     * @note Requires a valid DescriptorUbuf; AinDescriptorUbufNone is rejected at compile time.
     */
    template <typename DescriptorUbuf = AinDescriptorUbuf>
    AIN_DEVICE void get(__gm__ void *team, uint32_t peer, __gm__ void *dstWin, uint64_t dstOffset,
                        __gm__ void *srcWin, uint64_t srcOffset, uint64_t bytes,
                        const DescriptorUbuf &ubuf = DescriptorUbuf{},
                        AinCommitFlags commitFlags = AIN_COMMIT_IMMED);

    uint64_t devComm_;
    int contextIndex_;

private:
    static constexpr uint32_t MAX_PENDING_CHANNELS = 64;

    /*!
     * @brief Decode a symmetric-window handle into its window index.
     */
    AIN_DEVICE uint32_t decode_window(__gm__ void *window) const;

    /*!
     * @brief Record a channel as having pending delayed tasks, flushing first if the set is full.
     */
    AIN_DEVICE void record_channel(uint64_t channelId);

    Hcomm<COMM_PROTOCOL_UBC_CTP> hcomm_;
    uint64_t pendingChannels_[MAX_PENDING_CHANNELS] = {};
    uint32_t pendingChannelCount_ = 0;
};

/*!
 * @class AinBarrierSession
 * @brief A non-copyable barrier session bound to an Ain instance and a communication team. It implements an
 *        all-rank synchronization (sync) by exchanging monotonically increasing signal values with every
 *        remote rank through the underlying Hcomm engine and spinning until all peers have caught up.
 */
class AinBarrierSession {
public:
    /*!
     * @brief Construct a barrier session.
     * @param [in] ain: The owning Ain instance.
     * @param [in] team: Team id this session synchronizes over.
     * @param [in] index: Session index within the team.
     */
    AIN_DEVICE AinBarrierSession(Ain *ain, int32_t team, uint32_t index);
    AIN_DEVICE ~AinBarrierSession();
    AinBarrierSession(AinBarrierSession const &) = delete; // Sessions are not copyable

    /*!
     * @brief Clean and invalidate a single cache line at addr, with compiler barriers to prevent the
     *        dcci from being optimized away.
     * @param [in] addr: Address of the cache line to flush.
     */
    AIN_DEVICE void dcci_cacheline(__gm__ uint8_t *addr);

    /*!
     * @brief Perform an all-rank barrier across the team.
     * @tparam DescriptorUbuf: UB workspace descriptor type, AinDescriptorUbuf by default.
     * @param [in] memOrder: Memory ordering requested for the barrier.
     * @param [in] ubuf: UB workspace used by the underlying Hcomm engine.
     */
    template <typename DescriptorUbuf = AinDescriptorUbuf>
    AIN_DEVICE void sync(int32_t memOrder, const DescriptorUbuf &ubuf = AinDescriptorUbuf{});

    Ain *ain_;
    int32_t team_;
    uint32_t index_;
};

} // namespace AscendC

#include "../../../impl/adv_api/detail/ain/impl/ain_impl.h"

#endif // INCLUDE_ADV_API_AIN_AIN_H
