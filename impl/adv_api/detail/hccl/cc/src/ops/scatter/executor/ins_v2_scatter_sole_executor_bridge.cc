/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "ins_v2_scatter_sole_executor.h"

namespace hccl {
    class CollScatterExecutor;
    
    //声明CANN库的函数
    extern HcclResult CollScatterExecutorOrchestrate(void* executor, void* param, void* resCtx);
}

namespace mc2_ops_hccl {

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2ScatterSoleExecutor<AlgTopoMatch, InsAlgTemplate>::Orchestrate(
    const OpParam &param, const AlgResourceCtxSerializable &resCtx)
{
    HCCL_INFO("[InsV2ScatterSoleExecutor][Orchestrate] delegating to CANN hccl::CollScatterExecutor");
    
    //参数类型需要转换 - 这里使用reinterpret_cast假设内存布局相同
    void* cannParam = reinterpret_cast<void*>(&param);
    void* cannResCtx = reinterpret_cast<void*>(&resCtx);
    
    //调用CANN库的Orchestrate
    HcclResult ret = hccl::CollScatterExecutorOrchestrate(nullptr, cannParam, cannResCtx);
    
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("[InsV2ScatterSoleExecutor][Orchestrate] CANN returned error[%d]", ret);
        return ret;
    }
    
    HCCL_INFO("[InsV2ScatterSoleExecutor][Orchestrate] CANN orchestrate succeeded");
    return HCCL_SUCCESS;
}

}