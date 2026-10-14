// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_COLLECTIVE_DSL_NATIVE_BINDING_TRACE_H
#define MC2_COLLECTIVE_DSL_NATIVE_BINDING_TRACE_H
#include "collective_plan.h"
#include <string>
#include <vector>
namespace mc2_ops_hccl {
namespace dsl {
enum class NativeAction { BIND_RESOURCES, LOWER };
struct NativeTraceEvent {
    StageId stage;
    Primitive primitive;
    NativeAction action;
    const char* implementation;
};
void EnableNativeBindingTrace(bool enable);
void ClearNativeBindingTrace();
void RecordNativeBinding(const Stage& stage, NativeAction action, const char* implementation);
std::vector<NativeTraceEvent> NativeBindingTrace();
bool ComparePlanToNativeTrace(const Plan& plan, const std::vector<NativeTraceEvent>& trace, std::string& difference);
} // namespace dsl
} // namespace mc2_ops_hccl
#endif
