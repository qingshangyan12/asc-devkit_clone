/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "aicpu_kfc_interface.h"

#include "common/aicpu_hccl_common.h"
#include "common/aicpu_hccl_def.h"
#include "aicpu_kfc/decoupler/comm_kfc_dispatcher.h"
#include "common/aicpu_kfc_utils.h"

using namespace HcclApi;
extern "C" {
__attribute__((visibility("default"))) uint32_t Mc2ServerKernel(void *args[])
{
    if (args == nullptr) {
        HCCL_ERROR("args is null.");
        return HCCL_E_PARA;
    }
    constexpr int DESC_POS = 0;
    uint64_t desc_value = u64(args[DESC_POS]);
    uint64_t *desc_addr = &desc_value;
    CommKfcParamDesc *desc = reinterpret_cast<CommKfcParamDesc*>(desc_addr);
    AicpuKfcUtils::PrintHcclCommParamDesc(*desc);
    return CommKfcDispatcher::Run(&(args[1]), desc->itemNum);
}
}
