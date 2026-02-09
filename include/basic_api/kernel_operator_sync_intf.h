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
 * \file kernel_operator_block_sync_intf.h
 * \brief
 */

#ifndef ASCENDC_MODULE_OPERATOR_SYNC_INTF_H
#define ASCENDC_MODULE_OPERATOR_SYNC_INTF_H

#include "common_types.h"

namespace AscendC {

template <HardEvent event>
__aicore__ inline void SetFlag(int32_t eventID);

template <HardEvent event>
__aicore__ inline void WaitFlag(int32_t eventID);

template <pipe_t pipe>
__aicore__ inline void PipeBarrier();

} // namespace AscendC

#include "../../impl/basic_api/kernel_operator_sync_intf_impl.h"
#endif // KERNEL_SYNC_INTF_H