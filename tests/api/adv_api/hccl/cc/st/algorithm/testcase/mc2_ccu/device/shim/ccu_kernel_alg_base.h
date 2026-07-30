/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_CCU_KFC_ST_KERNEL_ALG_BASE_SHIM_H
#define HCCL_CCU_KFC_ST_KERNEL_ALG_BASE_SHIM_H

#include "ccu_types.h"
#include "hcomm_primitives.h"
#include "ccu_primitives_stub.h"

#include <cstdint>

namespace ccu = ::AscendC::ccu;

namespace hcomm {
class CcuTaskArg {};
} // namespace hcomm

#define HCCL_INFO(...) ((void)0)
#define HCCL_DEBUG(...) ((void)0)
#define HCCL_ERROR(...) ((void)0)
#define CCU_CHK_RET(call) do { CcuResult _result = static_cast<CcuResult>(call); if (_result != CCU_SUCCESS) { return _result; } } while (0)

namespace mc2_ops_hccl {

using u64 = uint64_t;
using HcclDataType = uint32_t;

enum class HcclCMDType : uint32_t {
    HCCL_CMD_ALLTOALLV = 8,
    HCCL_CMD_ALLTOALL = 10
};

struct OpParam {
    HcclCMDType opType{HcclCMDType::HCCL_CMD_ALLTOALLV};
    bool isMc2{false};
};

constexpr uint32_t CCU_MAX_RANK_SIZE = 64;

struct CcuKernelArgBase {
    ChannelHandle channels[CCU_MAX_RANK_SIZE]{};
    uint32_t channelCount{0};
};

struct CcuKernelCtxBase {};

struct GroupOpSizeVars {
    ccu::Variable addrOffset;
    ccu::Variable loopParam;
    ccu::Variable parallelParam;
    ccu::Variable residual;
};

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_KFC_ST_KERNEL_ALG_BASE_SHIM_H
