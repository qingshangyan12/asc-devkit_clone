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
 * \file kernel_operator_sync_intf_impl.h
 * \brief
 */
#ifndef ASCENDC_MODULE_OPERATOR_SYNC_INTERFACE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_SYNC_INTERFACE_IMPL_H

#include "kernel_event.h"
#include "utils/std/type_traits/is_same_one_of.h"

namespace AscendC {

template <HardEvent event>
__aicore__ inline constexpr bool JudgeEventValidWhenFlag()
{
    if ASCEND_IS_AIC {
        if (Std::is_same_one_of_v<HardEvent, event, HardEvent::MTE2_V, HardEvent::V_MTE2, HardEvent::MTE3_V,
                             HardEvent::V_MTE3, HardEvent::V_V, HardEvent::S_V,
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101)
                             HardEvent::MTE2_MTE3, HardEvent::MTE3_MTE2, HardEvent::MTE3_S, HardEvent::S_MTE3,
#endif
                             HardEvent::V_S>) {
            return false;
        }
    }
    if ASCEND_IS_AIV {
        if (Std::is_same_one_of_v<HardEvent, event, HardEvent::MTE2_MTE1, HardEvent::MTE1_MTE2, HardEvent::MTE1_M,
                             HardEvent::M_MTE1, HardEvent::M_FIX, HardEvent::FIX_M>) {
            return false;
        }
    }
    return true;
}

template <HardEvent event>
__aicore__ inline void SetFlag(int32_t eventID)
{
    if (JudgeEventValidWhenFlag<event>()) {
        SetFlagImpl<event>(eventID);
    }
}

template <HardEvent event>
__aicore__ inline void WaitFlag(int32_t eventID)
{
    if (JudgeEventValidWhenFlag<event>()) {
        WaitFlagImpl(event, eventID);
    }
}

template <pipe_t pipe>
__aicore__ inline void PipeBarrier()
{
    PipeBarrierImpl<pipe>();
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) ||       \
    (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102) ||       \
    (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3103) ||                                 \
    (__NPU_ARCH__ == 3113))
template <MemDsbT arg0>
__aicore__ inline void DataSyncBarrier()
{
    DataSyncBarrierImpl<arg0>();
}
#endif

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_SYNC_INTERFACE_IMPL_H
