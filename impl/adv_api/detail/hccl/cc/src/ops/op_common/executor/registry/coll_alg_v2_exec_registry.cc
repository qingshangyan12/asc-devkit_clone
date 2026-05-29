/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "coll_alg_v2_exec_registry.h"

namespace ops_hccl {
    class InsCollAlgBase;
    
    class CollAlgExecRegistryV2 {
    public:
        static CollAlgExecRegistryV2& Instance();
        HcclResult Register(HcclCMDType type, const std::string& tag, 
                           const std::function<InsCollAlgBase*()>& creator);
        InsCollAlgBase* GetAlgExec(HcclCMDType type, const std::string& tag);
    };
}

namespace mc2_ops_hccl {

CollAlgExecRegistryV2 &CollAlgExecRegistryV2::Instance()
{
    HCCL_INFO("[mc2_ops_hccl::CollAlgExecRegistryV2::Instance] delegating to CANN ops_hccl library");
    
    ops_hccl::CollAlgExecRegistryV2& cannInstance = ops_hccl::CollAlgExecRegistryV2::Instance();
    
    return reinterpret_cast<CollAlgExecRegistryV2&>(cannInstance);
}

HcclResult CollAlgExecRegistryV2::Register(const HcclCMDType type, const std::string &tag,
    const CollExecCreatorV2 &collExecCreator)
{
    HCCL_INFO("[mc2_ops_hccl::CollAlgExecRegistryV2::Register] delegating to CANN library, type[%d], tag[%s]",
        static_cast<int>(type), tag.c_str());
    
    ops_hccl::CollAlgExecRegistryV2& cannInstance = ops_hccl::CollAlgExecRegistryV2::Instance();
    
    const std::function<ops_hccl::InsCollAlgBase*()>& cannCreator = 
        reinterpret_cast<const std::function<ops_hccl::InsCollAlgBase*()>&>(collExecCreator);
    
    return cannInstance.Register(type, tag, cannCreator);
}

std::unique_ptr<InsCollAlgBase> CollAlgExecRegistryV2::GetAlgExec(const HcclCMDType type,
    const std::string &tag)
{
    HCCL_INFO("[mc2_ops_hccl::CollAlgExecRegistryV2::GetAlgExec] delegating to CANN library, type[%d], tag[%s]",
        static_cast<int>(type), tag.c_str());
    
    ops_hccl::CollAlgExecRegistryV2& cannInstance = ops_hccl::CollAlgExecRegistryV2::Instance();
    
    ops_hccl::InsCollAlgBase* cannExecutor = cannInstance.GetAlgExec(type, tag);
    
    if (cannExecutor == nullptr) {
        HCCL_INFO("[mc2_ops_hccl::CollAlgExecRegistryV2::GetAlgExec] CANN returned nullptr");
        return nullptr;
    }
    
    HCCL_INFO("[mc2_ops_hccl::CollAlgExecRegistryV2::GetAlgExec] CANN returned executor[%p]", cannExecutor);
    
    InsCollAlgBase* localExecutor = reinterpret_cast<InsCollAlgBase*>(cannExecutor);
    return std::unique_ptr<InsCollAlgBase>(localExecutor);
}

}