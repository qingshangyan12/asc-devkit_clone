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
 * \file kernel_tquebind_isaivtscm_impl.h
 * \brief
 */

#ifndef IMPL_BASIC_API_KERNEL_TQUEBIND_ISAIVTSCM_IMPL_H
#define IMPL_BASIC_API_KERNEL_TQUEBIND_ISAIVTSCM_IMPL_H

namespace AscendC {
// begin impl of IsAivTscm used by tquebind
__aicore__ inline constexpr bool IsAivTscm(TPosition src, TPosition dst)
{
#if __NPU_ARCH__ == 2201
    if (GetPosition(src, dst) == TPosition::TSCM) {
        return true;
    }
#else
    (void)(src);
    (void)(dst);
#endif
    return false;
}

template <TPosition src, TPosition dst, int32_t depth, auto mask>
__aicore__ inline void TQueBind<src, dst, depth, mask>::FreeBuffer(TBufHandle buf)
{
    auto ptr = reinterpret_cast<TBufType*>(buf);
    ASCENDC_DEBUG_ASSERT((this->bufStart <= ptr && ptr < this->bufStart + this->bufNum),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "ptr is %p, which should be in range [%p, %p)", ptr, this->bufStart,
            this->bufStart + this->bufNum));
    ASCENDC_DEBUG_ASSERT((ptr->state != TBufState::FREE),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "ptr state is %d, which can not be FREE", static_cast<int32_t>(ptr->state)));
    if constexpr (depth == 0) {
        if constexpr (!IsAivTscm(src, dst)) {
            // in 220 version, event changed from v to M_MTE1 on condition C1 -> C2
            SetFlag<freeBufEvt>(ptr->freeBufEvtID);
        }
    } else {
        if constexpr (!IsAivTscm(src, dst)) {
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ != 1001) && (__NPU_ARCH__ != 2002)
            // in 220 version, event changed from v to M_MTE1 on condition C1 -> C2
            ptr->freeBufEvtID = GetTPipePtr()->AllocEventID<freeBufEvt>();
            SetFlag<freeBufEvt>(ptr->freeBufEvtID);
            if constexpr (enableLoopQueue) {
                ptr->freeBufEvt = freeBufEvt;
            }
#else
             if constexpr (src == TPosition::C1 || (src == TPosition::CO2 && dst == TPosition::VECIN)) {
                SetFlag<freeBufEvt>(0); // insert pipe_v without eventID
                ASCENDC_DEBUG_ASSERT((ptr->freeBufEvtID == INVALID_TEVENTID),
                            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "freebuf event id can not be -1"));
            } else {
                ptr->freeBufEvtID = GetTPipePtr()->AllocEventID<freeBufEvt>();
                SetFlag<freeBufEvt>(ptr->freeBufEvtID);
            }
#endif
        } else if constexpr (srcHardType == Hardware::GM) {
            if ASCEND_IS_AIC {
                ptr->freeBufEvtID = GetTPipePtr()->AllocEventID<freeBufEvt>();
                SetFlag<freeBufEvt>(ptr->freeBufEvtID);
                if constexpr (enableLoopQueue) {
                    ptr->freeBufEvt = freeBufEvt;
                }
            }
        }
    }
    ptr->state = TBufState::FREE;
    if constexpr (depth != 0) {
        this->bufUsedCount--;
    }
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    constexpr Hardware bufferType = GetBufferPos(src, dst);
    auto absAddr = GetTPipePtr()->g_tpipeImpl.bufPoolBaseAddr_[static_cast<uint8_t>(bufferType)].absAddr;
    AscendCBufFree(static_cast<uint8_t>(bufferType), static_cast<uint8_t>(GetPosition(src, dst)),
        (uint64_t)(absAddr + ptr->address), static_cast<uint64_t>(ptr->dataLen));
#endif // ASCENDC_CPU_DEBUG
    return;
}

template <TPosition src, TPosition dst, int32_t depth, auto mask>
__aicore__ inline TBufHandle TQueBind<src, dst, depth, mask>::AllocBuffer()
{
    DEBUG_CODE(int32_t size = 0);
    ASCENDC_DEBUG_ASSERT((bufNum > 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "bufNum is %d, which must be larger than 0", static_cast<int32_t>(bufNum)));
    TBufType* ret;
    do {
        ret = this->bufStart + this->bufCursor;
        if constexpr (config.bufferNumber != 1) {
            this->bufCursor += 1;
            if (this->bufCursor == this->bufNum) {
                this->bufCursor = 0;
            }
        }
        if (ret->state == TBufState::FREE) {
            ret->state = TBufState::OCCUPIED;
            if constexpr (IsAivTscm(src, dst)) {
                if constexpr (srcHardType == Hardware::UB) {
                    break;
                } else if constexpr (srcHardType == Hardware::GM) {
                    if ASCEND_IS_AIV {
                        break;
                    }
                }
            }
            if (ret->freeBufEvtID != INVALID_TEVENTID) {
                if constexpr (enableLoopQueue) {
                    if (freeBufEvt == ret->freeBufEvt) {
                        WaitFlag<freeBufEvt>(ret->freeBufEvtID);
                        GetTPipePtr()->ReleaseEventID<freeBufEvt>(ret->freeBufEvtID);
                        ret->freeBufEvtID = INVALID_TEVENTID;
                    } else if (freeBufEvt == HardEvent::V_MTE2 && ret->freeBufEvt == HardEvent::MTE3_V) {
                        WaitFlag<HardEvent::MTE3_V>(ret->freeBufEvtID);
                        GetTPipePtr()->ReleaseEventID<HardEvent::MTE3_V>(ret->freeBufEvtID);
                        ret->freeBufEvtID = INVALID_TEVENTID;
                        TEventID evtId = GetTPipePtr()->AllocEventID<HardEvent::MTE3_MTE2>();
                        SetFlag<HardEvent::MTE3_MTE2>(evtId);
                        WaitFlag<HardEvent::MTE3_MTE2>(evtId);
                        GetTPipePtr()->ReleaseEventID<HardEvent::MTE3_MTE2>(evtId);
                    } else if (freeBufEvt == HardEvent::MTE3_V && ret->freeBufEvt == HardEvent::V_MTE2) {
                        WaitFlag<HardEvent::V_MTE2>(ret->freeBufEvtID);
                        GetTPipePtr()->ReleaseEventID<HardEvent::V_MTE2>(ret->freeBufEvtID);
                        ret->freeBufEvtID = INVALID_TEVENTID;
                    } else {
                        ASCENDC_DEBUG_ASSERT(false,
                            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "there is something wrong with free buf event"));
                    }
                } else {
                    WaitFlag<freeBufEvt>(ret->freeBufEvtID);
                    GetTPipePtr()->ReleaseEventID<freeBufEvt>(ret->freeBufEvtID);
                    ret->freeBufEvtID = INVALID_TEVENTID;
                }
            }
            break;
        }
#if defined(ASCENDC_CPU_DEBUG) && (ASCENDC_CPU_DEBUG == 1)
        ASCENDC_DEBUG_ASSERT((++size <= this->bufNum),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "size is %d, which exceed limits %d", size, static_cast<int32_t>(this->bufNum)));
#endif
    } while (true);
    this->bufUsedCount++;
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    constexpr Hardware bufferType = GetBufferPos(src, dst);
    auto absAddr = GetTPipePtr()->g_tpipeImpl.bufPoolBaseAddr_[static_cast<uint8_t>(bufferType)].absAddr;
    AscendCBufAlloc(static_cast<uint8_t>(bufferType), static_cast<uint8_t>(GetPosition(src, dst)),
        reinterpret_cast<uint64_t>(absAddr + ret->address), static_cast<uint64_t>(ret->dataLen));
    if (this->bufPoolHandle != 0U) {
        AscendCUpdateTbufPoolStatus(this->bufPoolHandle, false);
        AscendCTBufPoolResetCheck(static_cast<uint8_t>(GetPosition(srcPosition, dstPosition)),
            reinterpret_cast<uint64_t>(absAddr + ret->address),
            static_cast<uint64_t>(ret->dataLen),
            this->bufPoolHandle);
    }
#endif // ASCENDC_CPU_DEBUG
    return reinterpret_cast<TBufHandle>(ret);
}
} // namespace AscendC
#endif // IMPL_BASIC_API_KERNEL_TQUEBIND_ISAIVTSCM_IMPL_H