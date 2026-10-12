// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "native_binding_trace.h"
#include <set>
namespace mc2_ops_hccl {
namespace dsl {
namespace {
thread_local bool enabled = false;
thread_local std::vector<NativeTraceEvent> events;
} // namespace
void EnableNativeBindingTrace(bool enable) { enabled = enable; }
void ClearNativeBindingTrace() { events.clear(); }
void RecordNativeBinding(const Stage& stage, NativeAction action, const char* implementation)
{
    if (enabled)
        events.push_back({stage.id, stage.primitive, action, implementation});
}
std::vector<NativeTraceEvent> NativeBindingTrace() { return events; }
bool ComparePlanToNativeTrace(const Plan& plan, const std::vector<NativeTraceEvent>& trace, std::string& difference)
{
    std::set<StageId> lowered;
    for (const auto& e : trace)
        if (e.action == NativeAction::LOWER)
            lowered.insert(e.stage);
    for (const Stage& s : plan.stages)
        if (s.primitive != Primitive::PLACE_INPUT && lowered.count(s.id) == 0) {
            difference = "native trace did not lower stage " + std::to_string(s.id);
            return false;
        }
    difference.clear();
    return true;
}
} // namespace dsl
} // namespace mc2_ops_hccl
