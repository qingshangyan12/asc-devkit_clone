/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_CCU_KFC_SERVER_ADAPTER_H
#define HCCL_CCU_KFC_SERVER_ADAPTER_H

#include "ccu_kernel_proxy.h"

namespace HcclSim {
namespace CcuSt {

enum class KfcServerTestRole : uint32_t {
    DEFAULT = 0,
    ALL_GATHER_MESH = 1,
    ALL_GATHER_NHR = 2,
};

struct KfcServerKernelConfig {
    uint32_t rankId{0};
    uint32_t rankSize{0};
    uint64_t opId{0};
    std::vector<ChannelHandle> channels;
    KfcServerTestRole role{KfcServerTestRole::DEFAULT};
};

Result CaptureKfcServerKernel(void *kernelArg);

} // namespace CcuSt
} // namespace HcclSim

#endif // HCCL_CCU_KFC_SERVER_ADAPTER_H
