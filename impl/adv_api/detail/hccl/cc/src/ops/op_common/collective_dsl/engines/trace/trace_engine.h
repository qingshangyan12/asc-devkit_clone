// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#ifndef MC2_COLLECTIVE_DSL_TRACE_ENGINE_H
#define MC2_COLLECTIVE_DSL_TRACE_ENGINE_H

#include "collective_plan.h"

#include <iosfwd>
#include <string>

namespace mc2_ops_hccl {
namespace dsl {

void WriteTrace(const Plan& plan, std::ostream& output);
bool CompareTrace(std::istream& expected, std::istream& actual, std::string& difference);

} // namespace dsl
} // namespace mc2_ops_hccl

#endif
