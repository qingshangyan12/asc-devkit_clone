/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DLSYM_COMMON_H
#define DLSYM_COMMON_H

#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DECL_WEAK_FUNC(type, func_name, ...) type func_name(__VA_ARGS__) __attribute__((weak));

#define DEFINE_DLSYM_FUNC(type, func_name, params, args)                                  \
    static bool g_##func_name##Supported = false;                                         \
    using func_name##Func = type (*) params;                                              \
    static func_name##Func g_##func_name##Ptr = nullptr;                                  \
    extern "C" bool HcommIsSupport##func_name(void) { return g_##func_name##Supported; }  \
    extern "C" type func_name params __attribute__((weak));                               \
    extern "C" type func_name params                                                     \
    {                                                                                     \
        if (g_##func_name##Ptr == nullptr) {                                              \
            HCCL_ERROR("[HcclWrapper] %s not supported", __func__);                       \
            return static_cast<type>(-1);                                                 \
        }                                                                                 \
        return g_##func_name##Ptr args;                                                   \
    }

#define DECL_SUPPORT_FLAG(func_name) extern "C" bool HcommIsSupport##func_name(void)

#define INIT_SUPPORT_FLAG(handle, func_name)                                                        \
    do {                                                                                            \
        dlerror();                                                                                  \
        void* ptr = (void*)dlsym(handle, #func_name);                                               \
        const char* err = dlerror();                                                                \
        if (ptr == nullptr || ptr == reinterpret_cast<void*>(&func_name)) {                          \
            g_##func_name##Ptr = nullptr;                                                           \
            g_##func_name##Supported = false;                                                       \
            HCCL_ERROR("[HcclWrapper] %s not supported, %s", #func_name, err ? err : "(no error)"); \
        } else {                                                                                    \
            g_##func_name##Ptr = reinterpret_cast<func_name##Func>(ptr);                            \
            g_##func_name##Supported = true;                                                        \
        }                                                                                           \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif // DLSYM_COMMON_H
