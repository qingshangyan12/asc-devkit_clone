// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_DSL_AIV_PROGRAM_BUILDER_H
#define MC2_DSL_AIV_PROGRAM_BUILDER_H
#include "collective_plan.h"
#include "aiv_program.h"
namespace mc2_ops_hccl { namespace dsl {
std::vector<AivCommand> BuildAivProgram(const Plan& plan);
} }
#endif
