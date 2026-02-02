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
 * \file kernel_operator_sync_intf.h
 * \brief
 */

#ifndef ASCENDC_MODULE_OPERATOR_SYNC_INTF_H
#define ASCENDC_MODULE_OPERATOR_SYNC_INTF_H

#include "kernel_event.h"

namespace AscendC {

template <HardEvent event>
__aicore__ inline void SetFlag(int32_t eventID)
{
    if ASCEND_IS_AIC {
        if constexpr (event == HardEvent::MTE2_V || event == HardEvent::V_MTE2 || event == HardEvent::MTE3_V
                      || event == HardEvent::V_MTE3 || event == HardEvent::V_V || event == HardEvent::S_V ||
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101)
                      event == HardEvent::V_S || event == HardEvent::MTE2_MTE3 || event == HardEvent::MTE3_MTE2
                      || event == HardEvent::MTE3_S || event == HardEvent::S_MTE3) {
#else
                      event == HardEvent::V_S) {
#endif
            return;
        }
    }
    if ASCEND_IS_AIV {
        if constexpr ((event == HardEvent::MTE2_MTE1) || (event == HardEvent::MTE1_MTE2) ||
                      (event == HardEvent::MTE1_M) || (event == HardEvent::M_MTE1) || (event == HardEvent::M_FIX) ||
                      (event == HardEvent::FIX_M)) {
            return;
        }
    }
    SetFlagImpl<event>(eventID);
}

template <HardEvent event>
__aicore__ inline void WaitFlag(int32_t eventID)
{
    if ASCEND_IS_AIC {
        if constexpr (event == HardEvent::MTE2_V || event == HardEvent::V_MTE2 || event == HardEvent::MTE3_V
                      || event == HardEvent::V_MTE3 || event == HardEvent::V_V || event == HardEvent::S_V ||
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101)
                      event == HardEvent::V_S || event == HardEvent::MTE2_MTE3 || event == HardEvent::MTE3_MTE2
                      || event == HardEvent::MTE3_S || event == HardEvent::S_MTE3) {
#else
                      event == HardEvent::V_S) {
#endif
            return;
        }
    }
    if ASCEND_IS_AIV {
        if constexpr ((event == HardEvent::MTE2_MTE1) || (event == HardEvent::MTE1_MTE2) ||
                      (event == HardEvent::MTE1_M) || (event == HardEvent::M_MTE1) || (event == HardEvent::M_FIX) ||
                      (event == HardEvent::FIX_M)) {
            return;
        }
    }
    WaitFlagImpl(event, eventID);
}

template <pipe_t pipe>
__aicore__ inline void PipeBarrier()
{
    PipeBarrierImpl<pipe>();
}

template<pipe_t src, pipe_t dst>
class TQueSync {
public:
    __aicore__ inline void SetFlag(TEventID id);
    __aicore__ inline void WaitFlag(TEventID id);
};

} // namespace AscendC

#endif // KERNEL_BLOCK_SYNC_INTF_H