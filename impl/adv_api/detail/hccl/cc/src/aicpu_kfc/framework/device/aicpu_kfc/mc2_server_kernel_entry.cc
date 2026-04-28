/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include <cstdint>

#include "aicpu_kfc/decoupler/comm_kfc_dispatcher.h"
#include "common/aicpu_kfc_def.h"
#include "common/aicpu_kfc_utils.h"
#include "hccl_msg.h"
#include "log.h"

using namespace HcclApi;

extern "C" __attribute__((visibility("default"))) uint32_t Mc2ServerKernel(void *args[])
{
    HCCL_INFO("YYYYYY asc mc2 [Mc2ServerKernel:entry] start, args[%p], file[%s]", args, __FILE__);
    if (args == nullptr) {
        HCCL_ERROR("YYYYYY asc mc2 [Mc2ServerKernel:entry] args is null.");
        HCCL_ERROR("args is null.");
        return HCCL_E_PARA;
    }

    constexpr int DESC_POS = 0;
    uint64_t descValue = reinterpret_cast<uint64_t>(args[DESC_POS]);
    auto *desc = reinterpret_cast<CommKfcParamDesc *>(&descValue);
    HCCL_INFO("YYYYYY asc mc2 [Mc2ServerKernel:entry] desc decoded, desc[%p], descValue[0x%llx], version[%llu], "
        "itemNum[%llu], tilingOff[%llu], isDyn[%llu]", desc, static_cast<unsigned long long>(descValue),
        static_cast<unsigned long long>(desc->version), static_cast<unsigned long long>(desc->itemNum),
        static_cast<unsigned long long>(desc->tilingOff), static_cast<unsigned long long>(desc->isDyn));
    AicpuKfcUtils::PrintHcclCommParamDesc(*desc);
    uint32_t ret = CommKfcDispatcher::Run(&(args[1]), desc->itemNum);
    HCCL_INFO("YYYYYY asc mc2 [Mc2ServerKernel:entry] end, ret[%u]", ret);
    return ret;
}
