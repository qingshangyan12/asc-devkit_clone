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

#ifndef INCLUDE_BASIC_API_KERNEL_OPERATOR_SYNC_INTF_H
#define INCLUDE_BASIC_API_KERNEL_OPERATOR_SYNC_INTF_H

#include "kernel_event.h"

namespace AscendC {

template <HardEvent event>
__aicore__ inline void SetFlag(int32_t eventID);

template <HardEvent event>
__aicore__ inline void WaitFlag(int32_t eventID);

template <pipe_t pipe>
__aicore__ inline void PipeBarrier();

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) ||       \
    (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102) ||       \
    (__NPU_ARCH__ == 3003) ||                                 \
    (__NPU_ARCH__ == 3113))
template <MemDsbT arg0>
__aicore__ inline void DataSyncBarrier();
#endif

} // namespace AscendC

#include "../../impl/basic_api/kernel_operator_sync_intf_impl.h"
#endif // KERNEL_SYNC_INTF_H