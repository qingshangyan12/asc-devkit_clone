/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_FUNC_HPP
#define CCU_FUNC_HPP

#include <cstdint>
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <vector>

#include "ccu_log.h"
#include "ccu_variable.hpp"

namespace ccu {

namespace internal {

template <typename T>
struct FunctorTraits : FunctorTraits<decltype(&T::operator())> {};

template <typename C, typename R, typename... A>
struct FunctorTraits<R (C::*)(A...) const> {
    static constexpr std::size_t Arity = sizeof...(A);
    using ReturnT = R;
};

template <typename C, typename R, typename... A>
struct FunctorTraits<R (C::*)(A...)> {
    static constexpr std::size_t Arity = sizeof...(A);
    using ReturnT = R;
};

template <typename Lambda, std::size_t... I>
std::function<void(Variable*)> MakeBodyImpl(Lambda fn, std::index_sequence<I...>)
{
    return [fn](Variable* formals) { fn(formals[I]...); };
}

template <typename Lambda, std::size_t Arity>
std::function<void(Variable*)> MakeBody(Lambda fn)
{
    return MakeBodyImpl(fn, std::make_index_sequence<Arity>{});
}

} // namespace internal

class Func {
public:
    template <typename Lambda>
    explicit Func(Lambda body)
        : numIn_(internal::FunctorTraits<Lambda>::Arity),
          body_(internal::MakeBody<Lambda, internal::FunctorTraits<Lambda>::Arity>(body))
    {
        static_assert(std::is_same<typename internal::FunctorTraits<Lambda>::ReturnT, void>::value,
                      "ccu::Func: lambda must return void (no return value supported)");
    }

    Func(const Func&)            = delete;
    Func& operator=(const Func&) = delete;

    uint32_t    NumIn() const { return numIn_; }
    const void* Key() const   { return static_cast<const void*>(this); }

    void RunBody(Variable* formals) const { body_(formals); }

private:
    uint32_t                      numIn_{0};
    std::function<void(Variable*)> body_;
};

template <Func& Obj, typename... Args>
inline CcuResult CallFunc(Args... args)
{
    constexpr std::size_t kArgN = sizeof...(Args);
    if (static_cast<uint32_t>(kArgN) != Obj.NumIn()) {
        return CcuResult::CCU_E_PARA;
    }

    uint64_t handle = 0;
    CCU_CHK_RET(::CcuFuncBlockLookup(Obj.Key(), &handle));
    if (handle == 0) {
        CCU_CHK_RET(::CcuFuncBlockBegin(Obj.Key(), &handle));

        std::vector<Variable> formals(Obj.NumIn());
        for (uint32_t i = 0; i < Obj.NumIn(); i++) {
            CCU_CHK_RET(::CcuVariableAlloc(&formals[i].handle));
            CCU_CHK_RET(::CcuFuncDefineInArg(handle, formals[i].handle));
        }

        Obj.RunBody(formals.empty() ? nullptr : formals.data());

        CCU_CHK_RET(::CcuFuncBlockEnd(handle));
    }

    std::vector<::CcuVariableHandle> actuals(kArgN);
    {
        std::size_t i = 0;
        using expand_t = int[];
        (void)expand_t{0, ((void)(actuals[i++] = args.handle), 0)...};
        (void)i;
    }
    return ::CcuFuncCall(handle, actuals.empty() ? nullptr : actuals.data(), static_cast<uint32_t>(kArgN));
}

} // namespace ccu

#endif
