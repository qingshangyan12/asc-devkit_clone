// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_DSL_AICPU_ALL_GATHER_EXECUTOR_H
#define MC2_DSL_AICPU_ALL_GATHER_EXECUTOR_H
#include "collective_plan.h"
#include "alg_param.h"
#include "template_utils.h"
namespace mc2_ops_hccl { namespace dsl {
HcclResult ExecuteAicpuAllGather(const Stage&,Rank,const OpParam&,const TemplateDataParams&,TemplateResource&);
} }
#endif
