/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "alg_param.h"
#include "hccl_types.h"
#include "execute_selector.h"
#include <memory>
#include <string>

extern bool HcclCheckLogLevel(int logType, int moduleId);
extern bool IsErrorToWarn();

bool HcclCheckLogLevel(int logType, int moduleId)
{
    return false;
}

bool IsErrorToWarn()
{
    return false;
}

namespace ops_hccl {

HcclResult HcomCheckDataType(HcclDataType dataType)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult HcclGetOpExpansionMode(HcclComm comm, OpParam& param)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult HcclCalcTopoInfo(HcclComm comm, OpParam& param, std::unique_ptr<TopoInfoWithNetLayerDetails>& topoInfo)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult InitRankInfo(HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo)
{
    return HcclResult::HCCL_SUCCESS;
}

void GetScatterOpInfo(const void* opInfo, char* output, size_t size)
{
}

unsigned int HcclLaunchAicpuKernel(OpParam* param)
{
    return 0;
}

bool CheckAsymmetricTopoSupport(HcclCMDType cmdType, const TopoInfoWithNetLayerDetails* topoInfo)
{
    return true;
}

ExecuteSelector::ExecuteSelector()
{
}

HcclResult ExecuteSelector::Run(OpParam& param, TopoInfoWithNetLayerDetails* topoInfo, std::string& algName) const
{
    algName = "CcuAllReduceMesh1D";
    return HcclResult::HCCL_SUCCESS;
}

HcclResult SetCommEngine(OpParam& param)
{
    return HcclResult::HCCL_SUCCESS;
}

}