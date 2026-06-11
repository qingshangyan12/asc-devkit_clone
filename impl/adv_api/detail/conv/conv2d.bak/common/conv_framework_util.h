/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*!
 * \file conv_framework_util.h
 * \brief
 */

#ifndef CONV_FRAMEWORK_UTIL_H
#define CONV_FRAMEWORK_UTIL_H

#include "kernel_utils.h"

using namespace AscendC;

#ifdef ASC_OP_DEBUG_TEST
#define ASC_OP_LOGD(fmt, ...)       \
    do {                            \
        printf(fmt, ##__VA_ARGS__); \
    } while (0)
#else
#define ASC_OP_LOGD(fmt, ...)
#endif

// Define struct _has_impl_MEMBER for selecting impl definition.
#define CONV_DECLARE_REG_IMPL(MEMBER, args...) namespace __ConvFramework {                                            \
    template<typename T, typename U>                                                                                  \
    struct _has_impl_##MEMBER {                                                                                       \
        template<typename C>                                                                                          \
        static auto check(int)->decltype(std::declval<typename C::template MEMBER<U, 0, ##args>>(), TrueType());      \
        template<typename C>                                                                                          \
        static FalseType check(...);                                                                                  \
        enum { value = IsSameType<decltype(check<T>(0)), TrueType>::value };                                          \
    };                                                                                                                \
}

// When Config::MEMBER is exist, using MEMBER = Config::MEMBER, either using MEMBER = Current::MEMBER.
#define CONV_REG_IMPL(Config, Current, MEMBER)                                                                        \
    template<bool Default, class T>                                                                                   \
    struct __##MEMBER { using Type = typename Current::MEMBER<Intf, ImplType>; };                                     \
    template<class T>                                                                                                 \
    struct __##MEMBER<true, T> { using Type = typename T::template MEMBER<Intf, ImplType>; };                         \
    using MEMBER = typename __##MEMBER<Current::__ConvFramework::_has_impl_##MEMBER<Config, Intf>::value, Config>::Type

//  Declare default call with return NAMESPACE::TypeFalse.
#define CONV_DECLARE_CHECK_FUN(T, NAMESPACE)                                                                          \
    template<class... Ts>                                                                                             \
    static __aicore__ inline NAMESPACE::TypeFalse call(Intf* self, Ts... args) { return (NAMESPACE::TypeFalse){0}; }

// Check whether the fun T::call exists.
#define CONV_CHECK_FUN(T, NAMESPACE, ... )                                                                            \
    (!IsSameType<decltype(T::call(__VA_ARGS__)), NAMESPACE::TypeFalse>::value)

// Check whether the fun T::call<ARGS> exists.
#define CONV_CHECK_FUN_TEMPLATE(T, NAMESPACE, ARG, ...)                                                               \
    (!IsSameType<decltype(T::template call<ARG>(__VA_ARGS__)), NAMESPACE::TypeFalse>::value)

// Define struct _has_member_MEMBER for check whether MEMBER exists.
#define CONV_DECLARE_CHECK_MEMBER(MEMBER) namespace __ConvFramework {                                                 \
    template <typename T>                                                                                             \
    struct _has_member_##MEMBER {                                                                                     \
        template<typename U>                                                                                          \
        static void check(decltype(&U::MEMBER));                                                                      \
        template<typename U>                                                                                          \
        static int check(...);                                                                                        \
        enum { value = IsSameType<decltype(check<T>(0)), void>::value };                                              \
    };                                                                                                                \
}

// Check whether OBJ::MEMBER exists.
#define CONV_CHECK_MEMBER(OBJ, MEMBER) (__ConvFramework::_has_member_##MEMBER<typename OBJ>::value)

// Define func _get_MEMBER_value for get constexpr member real value
#define CONV_DECLARE_DEFINE_MEMBER(OBJ, MEMBER, TYPE, DEFAULT) namespace __ConvFramework {                            \
    template <typename T>                                                                                             \
    constexpr TYPE _get_##MEMBER##_value(TYPE value)                                                                  \
    {                                                                                                                 \
        if constexpr (T::MEMBER > DEFAULT) {                                                                          \
            return T::MEMBER;                                                                                         \
        } else {                                                                                                      \
            return value;                                                                                             \
        }                                                                                                             \
    }                                                                                                                 \
}

// Define constexpr member
#define CONV_DEFINE_MEMBER(OBJ, MEMBER, DEFAULT, TYPE)                                                                \
    constexpr static TYPE MEMBER = __ConvFramework::_get_##MEMBER##_value<OBJ>(DEFAULT)

// Define func T_M_checkdefine() for define const or not const var.
#define CONV_DECLARE_DEFINE_STRUCT(T, M, U) namespace __ConvFramework {                                               \
    template <typename T>                                                                                             \
    struct T##_##M {                                                                                                  \
        U M;                                                                                                          \
        constexpr static bool __CONST_TYPE_##M = false;                                                               \
    };                                                                                                                \
    template<typename T>                                                                                              \
    struct T##_CT_##M {                                                                                               \
        constexpr static U M = T::M;                                                                                  \
        constexpr static bool __CONST_TYPE_##M = true;                                                                \
    };                                                                                                                \
    template <typename T>                                                                                             \
    static constexpr bool _is_const_##T##_##M() {                                                                     \
        if constexpr (T::M > 0) {return true;} else {return false;}                                                   \
    };                                                                                                                \
    template <typename T>                                                                                             \
    typename Conditional<_is_const_##T##_##M<T>(), T##_CT_##M<T>, T##_##M<T>>::type T##_##M##_checkdefine();          \
}

// When T::M > 0, define const var M, either define not const var M.
#define CONV_DEFINE_STUCT(T, M) public decltype(__ConvFramework::T##_##M##_checkdefine<T>())

// Define not const var T FIELD
#define CONV_DEFINE_STUCT_FIELD(T, FIELD)                                                                             \
    T FIELD;                                                                                                          \
    constexpr static bool __CONST_TYPE_##FIELD = false

// Check whether T::M is const var.
#define CONV_CHECK_CONST(T, M) (T::__CONST_TYPE_##M)

#endif // CONV_FRAMEWORK_UTIL_H