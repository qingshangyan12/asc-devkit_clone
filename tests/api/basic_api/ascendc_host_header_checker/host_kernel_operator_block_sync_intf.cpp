/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "host_checker.h"

#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_block_sync_intf.h"
#endif

using Int32LocalTensor = AscendC::LocalTensor<int32_t>;
using Int32GlobalTensor = AscendC::GlobalTensor<int32_t>;

CHECK_FUNC_EXISTS(AscendC::DataSyncBarrier<AscendC::MemDsbT::ALL>);
CHECK_FUNC_EXISTS((*static_cast<void (*)(const Int32GlobalTensor&, const Int32LocalTensor&, const int32_t)>(
    &AscendC::SyncAll<true>)));
CHECK_FUNC_EXISTS((AscendC::SyncAll<true, AscendC::DEFAULT_SYNC_ALL_CONFIG>));
