/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef HCCL_COMM_OP_H
#define HCCL_COMM_OP_H

#include <memory>
#include "hccl_common.h"
#include <unordered_map>
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/legacy/unified_platform/resource/buffer/buffer.h"
#include "common.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/legacy/common/types/op_mode.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/legacy/framework/dfx/common/task_info.h"
#include "../../../../../../../../../../../include/adv_api/hccl/internal/hcomm/pkg_inc/hccl/hccl_diag.h"

namespace hccl {
    
std::shared_ptr<Hccl::DfxOpInfo> ConvertToDfxOpInfo(const HcclDfxOpInfo& dfxOpInfo);
}
int32_t HcommThreadRegisterDfx(ThreadHandle thread, std::function<HcclResult(u32, u32, const Hccl::TaskParam&, u64)> callback);
#endif