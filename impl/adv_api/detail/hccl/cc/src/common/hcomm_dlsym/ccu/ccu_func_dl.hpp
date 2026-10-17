/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_FUNC_DL_HPP
#define CCU_FUNC_DL_HPP

#include <cstdint>
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "ccu_types_dl.h"
#include "ccu_primitives_impl_dl.h"
#include "ccu_variable_dl.hpp"
#include "ccu_array_dl.hpp"

namespace AscendC {
namespace ccu {

namespace internal {

// ---- LambdaTraits：只支持非泛型 lambda / 普通成员函数指针 operator() ----
template <typename T>
struct FunctorTraits : FunctorTraits<decltype(&T::operator())> {};

template <typename C, typename R, typename... A>
struct FunctorTraits<R (C::*)(A...) const> {
    static constexpr std::size_t Arity = sizeof...(A);
    using ReturnT = R;
    template <std::size_t I>
    struct Arg {
        using type = typename std::tuple_element<I, std::tuple<A...> >::type;
    };
};

template <typename C, typename R, typename... A>
struct FunctorTraits<R (C::*)(A...)> {
    static constexpr std::size_t Arity = sizeof...(A);
    using ReturnT = R;
};

// ---- 把 lambda 包装成 std::function<void(Variable*)>，按 N 展开 ----
template <typename Lambda, std::size_t... Is>
inline std::function<void(Variable*)> MakeBodyImpl(Lambda body, std::index_sequence<Is...>)
{
    return [body](Variable* formals) { body(formals[Is]...); };
}

template <typename Lambda, std::size_t N>
inline std::function<void(Variable*)> MakeBody(Lambda body)
{
    return MakeBodyImpl(body, std::make_index_sequence<N>{});
}

} // namespace internal

class Func {
public:
    // ctor：从 lambda 推导形参个数 N，类型擦除到 body_。
    // 要求 lambda 返回 void、形参全部为 ccu::Variable（运行期由 kernel 校验形参数量）。
    template <typename Lambda>
    explicit Func(Lambda body)
        : numIn_(internal::FunctorTraits<Lambda>::Arity),
          body_(internal::MakeBody<Lambda, internal::FunctorTraits<Lambda>::Arity>(body))
    {
        static_assert(
            std::is_same<typename internal::FunctorTraits<Lambda>::ReturnT, void>::value,
            "ccu::Func: lambda must return void (no return value supported)");
    }

    // 命名 func：lambda 唯一形参为 ccu::Array<ccu::Variable>&，按下标顺序读参，numIn 为入参个数。
    // 与匿名 func 不同，**构造即注册**：声明处就把函数体录进当前正在注册的 kernel，name 是它在该
    // kernel 内的唯一身份，之后可被 HcommCcuFunctionGet 按名字查到入口 pc，并被别的 kernel 用
    // ccu::CallFunc(pc, args) 调用。由此带来两条硬约束：
    //   1) 只能在 kernel 函数体内声明（声明点即绑定点，kernel 外没有可绑定的 kernel）；
    //   2) 被 lambda 捕获的资源必须在声明之前准备好，因为函数体就在构造时执行。
    // 构造函数无法回传错误码，失败走 CCU_THROW_IF_FAILED 抛出，由 kernel 注册路径的异常处理转成
    // 注册失败。
    template <typename Lambda>
    Func(Lambda body, uint32_t numIn, const char* name) : numIn_(numIn), usesArray_(true)
    {
        static_assert(
            std::is_same<typename internal::FunctorTraits<Lambda>::ReturnT, void>::value,
            "ccu::Func: lambda must return void (no return value supported)");
        static_assert(
            internal::FunctorTraits<Lambda>::Arity == 1,
            "ccu::Func: named func lambda must take exactly one ccu::Array<ccu::Variable>& parameter");

        uint64_t handle = 0;
        CCU_THROW_IF_FAILED(::CcuFuncBlockBeginNamed(name, &handle), "ccu::Func: CcuFuncBlockBeginNamed failed");
        // 形参走连续块分配，下标与调用方 Array 的实参一一对应
        Array<Variable> formals(numIn);
        for (uint32_t i = 0; i < numIn; i++) {
            CCU_THROW_IF_FAILED(
                ::CcuFuncDefineInArg(handle, formals[i].handle), "ccu::Func: CcuFuncDefineInArg failed");
        }
        body(formals);
        CCU_THROW_IF_FAILED(::CcuFuncBlockEnd(handle), "ccu::Func: CcuFuncBlockEnd failed");
    }

    Func(const Func&) = delete;
    Func& operator=(const Func&) = delete;

    uint32_t NumIn() const { return numIn_; }
    const void* Key() const { return static_cast<const void*>(this); }
    bool UsesArrayArg() const { return usesArray_; }

    // 在合成模式下调用：把 N 个 formal Variable 展开传给原 lambda。
    void RunBody(Variable* formals) const { body_(formals); }

private:
    uint32_t numIn_{0};
    std::function<void(Variable*)> body_;
    bool usesArray_{false};
};

// ccu::CallFunc：global / static ccu::Func 引用 NTTP（C++14 合法）。
// 用法：static ccu::Func MyAdd(lambda); ... ccu::CallFunc<MyAdd>(x, y);
template <Func& Obj, typename... Args>
inline CcuResult CallFunc(Args... args)
{
    // 命名 func 只能按 pc 跨 kernel 调用。
    if (Obj.UsesArrayArg()) {
        return CcuResult::CCU_E_NOT_SUPPORT;
    }
    constexpr std::size_t kArgN = sizeof...(Args);
    if (static_cast<uint32_t>(kArgN) != Obj.NumIn()) {
        return CcuResult::CCU_E_PARA;
    }

    uint64_t handle = 0;
    CCU_THROW_IF_FAILED(::CcuFuncBlockLookup(Obj.Key(), &handle), "CallFunc: CcuFuncBlockLookup failed");
    if (handle == 0) {
        // 第一次进入：合成 FuncBlock
        CCU_THROW_IF_FAILED(::CcuFuncBlockBegin(Obj.Key(), &handle), "CallFunc: CcuFuncBlockBegin failed");

        // 形参：按 lambda 形参个数 alloc，并注册为 FuncBlock 的 in args
        std::vector<Variable> formals(Obj.NumIn());
        for (uint32_t i = 0; i < Obj.NumIn(); i++) {
            CCU_THROW_IF_FAILED(::CcuVariableAlloc(&formals[i].handle), "CallFunc: CcuVariableAlloc failed");
            CCU_THROW_IF_FAILED(::CcuFuncDefineInArg(handle, formals[i].handle), "CallFunc: CcuFuncDefineInArg failed");
        }

        // 执行 lambda（在合成模式下 emit 到 FuncBlock）
        Obj.RunBody(formals.empty() ? nullptr : formals.data());

        CCU_THROW_IF_FAILED(::CcuFuncBlockEnd(handle), "CallFunc: CcuFuncBlockEnd failed");
    }

    // emit 一条 FuncCall 跳转
    std::vector< ::CcuVariableHandle> actuals(kArgN);
    {
        std::size_t i = 0;
        // C++14 兼容的参数包展开
        using expand_t = int[];
        (void)expand_t{0, ((void)(actuals[i++] = args.handle), 0)...};
        (void)i;
    }
    return ::CcuFuncCall(handle, actuals.empty() ? nullptr : actuals.data(), static_cast<uint32_t>(kArgN));
}

// ccu::CallFunc(pc, args)：按运行期 pc 跨 kernel 调用命名 func，pc 由 HcommCcuFunctionGet 查出后经 taskArgs 传入。
// 实参只接受 Array，被调 func 按同样的下标顺序取用；被调形参少于实参时多余的自动忽略。
// 用法：ccu::Variable pc{}; ccu::LoadArg(pc, 0); ccu::Array<ccu::Variable> args(2); ccu::CallFunc(pc, args);
inline CcuResult CallFunc(const Variable& funcPc, Array<Variable>& args)
{
    if (args.size() > CCU_FUNC_TASK_ARG_MAX) {
        return CcuResult::CCU_E_PARA;
    }
    std::vector< ::CcuVariableHandle> actuals(args.size());
    for (uint32_t i = 0; i < args.size(); i++) {
        actuals[i] = args[i].handle;
    }
    return ::CcuFuncCallByAddr(funcPc.handle, actuals.empty() ? nullptr : actuals.data(), args.size());
}

// Use existing variable handles in host-selected order; the sources need not form an Array.
inline CcuResult CallFunc(const Variable& funcPc, const std::vector<Variable>& args)
{
    if (args.size() > CCU_FUNC_TASK_ARG_MAX) {
        return CcuResult::CCU_E_PARA;
    }
    std::vector< ::CcuVariableHandle> actuals;
    actuals.reserve(args.size());
    for (const auto& arg : args) {
        actuals.push_back(arg.handle);
    }
    return ::CcuFuncCallByAddr(funcPc.handle, actuals.empty() ? nullptr : actuals.data(), args.size());
}

} // namespace ccu
} // namespace AscendC

#endif // CCU_FUNC_DL_HPP
