/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L4 后端注册表（方案 §6.4.3：kernelFunc 跟动作走，不跟算法名走）
 *
 * 对应现状：REGISTER_EXEC_V2(op, algName, executor, topoMatch, template) —— 键是 algName，
 * 96 个算法名 → 46 CCU kernel + 65 AICPU template。
 * 本表键是 (动作, 引擎)：每引擎 4~5 个动作实现，变体（pattern/transport/channels）走参数。
 */
#ifndef REFACTOR_POC_BACKEND_REGISTRY_H
#define REFACTOR_POC_BACKEND_REGISTRY_H

#include "action_spec.h"
#include "loop_splitter.h"   // LoopSplitter（2026-09-21 从本文件拆出——plan_serializer.h
                             // 需其完整类型且不可回引本文件，见 loop_splitter.h 头注）
#include "verb_sink.h"   // EmitCtx.sink / FireVerb（§10.2 A1：动词发射框架，引擎知识居所）
#include <string>

namespace mc2_ops_hccl::refactor {

enum class Engine { AICPU, CCU };

// 能力矩阵：不支持即 Validate 快速失败（不静默降级）
struct BackendCaps {
    bool supportSymmem{false};
    bool support64bitReduce{true};
    u32 maxSplits{4};
    u32 perSplitOverheadUs{0};
};

struct EmitCtx {
    Engine engine{Engine::AICPU};
    std::string trace;               // PoC：把发射出的动词序列记下来，供 ST 语义 checker 断言
    IVerbSink* sink{nullptr};        // §10.2 A1 动词发射目标（可注入；空 = TraceSink 兜底拼 trace）。
                                     // 真机联调时注入 PrimitiveSink 直驱 hcomm 原语（P2）；
                                     // ST checker 挂此口拿结构化 VerbCall。引擎知识只在 l5/。
};

// 后端接口：Emit（下发）+ ClaimEngineRes（引擎相关资源自报）
class IActionBackend {
public:
    virtual ~IActionBackend() = default;
    virtual Engine Kind() const = 0;
    virtual BackendCaps Caps() const = 0;
    virtual void ClaimEngineRes(const ActionSpec& a, ResourceClaim& claim) const = 0;
    virtual void Emit(const ActionSpec& a, const void* args, EmitCtx& ctx) const = 0;
};

class BackendRegistry {
public:
    static void Register(ActionKind k, Engine e, IActionBackend* be) { Table()[Key(k, e)] = be; }
    static IActionBackend* Peek(ActionKind k, Engine e)
    {
        auto it = Table().find(Key(k, e));
        return it == Table().end() ? nullptr : it->second;
    }
    static IActionBackend* Get(ActionKind k, Engine e) { return Peek(k, e); }

private:
    static u64 Key(ActionKind k, Engine e) { return static_cast<u64>(k) * 16 + static_cast<u64>(e); }
    static std::map<u64, IActionBackend*>& Table()
    {
        static std::map<u64, IActionBackend*> t;
        return t;
    }
};

#define REGISTER_ACTION_BACKEND(action, engine, Impl)                       \
    namespace {                                                             \
    struct Impl##Reg {                                                      \
        Impl##Reg() { BackendRegistry::Register(action, engine, new Impl()); } \
    } g_##Impl##Reg;                                                        \
    }

} // namespace mc2_ops_hccl::refactor

#endif
