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
#include "ccu_task_arg_v1.h"
#include "hccl_common.h"
#include "hcomm_primitives.h"
#include "ccu_primitives_stub.h"

#include <cstdint>

namespace ccu = ::AscendC::ccu;

#define HCCL_INFO(...) ((void)0)
#define HCCL_DEBUG(...) ((void)0)
#define HCCL_ERROR(...) ((void)0)
#define CCU_CHK_RET(call) do { CcuResult _result = static_cast<CcuResult>(call); if (_result != CCU_SUCCESS) { return _result; } } while (0)

namespace mc2_ops_hccl {

using u64 = uint64_t;

struct OpParam {
    struct DataDescriptor {
        HcclDataType dataType{HCCL_DATA_TYPE_FP16};
        HcclDataType outputType{HCCL_DATA_TYPE_FP16};
    } DataDes;

    HcclCMDType opType{HcclCMDType::HCCL_CMD_ALLTOALLV};
    HcclReduceOp reduceType{HCCL_REDUCE_SUM};
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

inline void InitCcuKernelCtxBase(CcuKernelCtxBase&) {}

inline CcuResult GroupCopy(
    CcuKernelCtxBase&, ccu::LocalAddr destination, ccu::LocalAddr source, const GroupOpSizeVars& goSize)
{
    HcclSim::CcuSt::CompilerContext::Current().AppendGroupCopy(destination.handle, source.handle,
        goSize.residual.handle,
        {goSize.addrOffset.handle, goSize.loopParam.handle, goSize.parallelParam.handle, goSize.residual.handle});
    return CCU_SUCCESS;
}

} // namespace mc2_ops_hccl

#endif // HCCL_CCU_KFC_ST_KERNEL_ALG_BASE_SHIM_H
