/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_PRINT_H
#define CCU_PRINT_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>

#include "ccu_primitives_dl.hpp"
#include "ccu_types.h"

namespace AscendC {
namespace ccu {
namespace dfx {

/* ring depth for each ccu::printf invocation */
constexpr uint64_t CCU_PRINT_RING_DEPTH = 16;

struct PrintInfoKey {
    const char* filename;
    int lineno;
    const char* fmtstr;
};

// head and tail are byte offsets in argvArray. Each record contains a shared
// sequence followed by the call site's fixed number of arguments.
struct PrintInfoValue {
    uint64_t tail;
    uint64_t head;
    uint64_t argvArray[];
};

struct PrintInfoHandle {
    PrintInfoValue* value;
    uint64_t* valueAddressCell;
    uint64_t* sequenceAddressCell;
    uint64_t argc;
    uint64_t recordBytes;
    uint64_t ringBytes;
    size_t allocationBytes;
};

namespace detail {

struct CcuPrintTestAccess;

bool ValidateFormat(const char* fmtstr, size_t argc);
bool Format(const char* fmtstr, const uint64_t* argv, size_t argc, std::string& output);

template <typename... Args>
struct AreCcuVariables;

template <>
struct AreCcuVariables<> : std::true_type {};

template <typename First, typename... Rest>
struct AreCcuVariables<First, Rest...>
    : std::integral_constant<
          bool,
          std::is_same<typename std::decay<First>::type, ccu::Variable>::value && AreCcuVariables<Rest...>::value> {};

inline CcuResult StoreArgs(ccu::Variable&, const ccu::Variable&) { return CCU_SUCCESS; }

template <typename First, typename... Rest>
CcuResult StoreArgs(ccu::Variable& address, const ccu::Variable& wordBytes, const First& first, const Rest&... rest)
{
    CcuResult result = static_cast<CcuResult>(ccu::Store(address, first));
    if (result != CCU_SUCCESS) {
        return result;
    }
    address += wordBytes;
    return StoreArgs(address, wordBytes, rest...);
}

} // namespace detail

class CcuPrintStream {
public:
    static CcuPrintStream* GetInstance();

    PrintInfoHandle* AllocPrintInfo(const char* filename, int lineno, const char* fmtstr, size_t argc);
    void Stop();

private:
    friend struct detail::CcuPrintTestAccess;

    CcuPrintStream();
    ~CcuPrintStream();
    CcuPrintStream(const CcuPrintStream&) = delete;
    CcuPrintStream& operator=(const CcuPrintStream&) = delete;

    struct Impl;
    std::unique_ptr<Impl> impl_;
};

template <typename... Args>
CcuResult EmitPrintf(PrintInfoHandle* handle, const Args&... args)
{
    static_assert(detail::AreCcuVariables<Args...>::value, "every CCU_PRINTF argument must be ccu::Variable");
    if (handle == nullptr) {
        return CCU_SUCCESS;
    }

    ccu::Variable buffer;
    ccu::Variable tail;
    ccu::Variable nextTail;
    ccu::Variable address;
    ccu::Variable value;

    // valueAddressCell is GM. CCU loads the PrintInfoValue GM address into an
    // XN register instead of trying to access a Host stack address.
    CcuResult result = static_cast<CcuResult>(ccu::Load(reinterpret_cast<uint64_t>(handle->valueAddressCell), buffer));
    if (result != CCU_SUCCESS) {
        return result;
    }

    address = buffer;
    result = static_cast<CcuResult>(ccu::Load(address, tail));
    if (result != CCU_SUCCESS) {
        return result;
    }

    value = handle->recordBytes;
    nextTail = tail + value;
    CCU_IF(nextTail == handle->ringBytes) { nextTail = 0; }

    value = offsetof(PrintInfoValue, argvArray);
    address = buffer + value;
    address += tail;

    // All printf call sites and missions share this best-effort sequence.
    // CCU has no atomic read-modify-write primitive for local GM, so
    // concurrent emitters may observe the same value. Equal sequence values
    // are intentionally allowed: those records were emitted close enough
    // together that their relative order cannot be established here.
    result = static_cast<CcuResult>(ccu::Load(reinterpret_cast<uint64_t>(handle->sequenceAddressCell), tail));
    if (result != CCU_SUCCESS) {
        return result;
    }
    result = static_cast<CcuResult>(ccu::Load(tail, buffer));
    if (result != CCU_SUCCESS) {
        return result;
    }
    value = 1;
    buffer += value;
    result = static_cast<CcuResult>(ccu::Store(tail, buffer));
    if (result != CCU_SUCCESS) {
        return result;
    }
    result = static_cast<CcuResult>(ccu::Store(address, buffer));
    if (result != CCU_SUCCESS) {
        return result;
    }

    value = sizeof(uint64_t);
    address += value;
    result = detail::StoreArgs(address, value, args...);
    if (result != CCU_SUCCESS) {
        return result;
    }

    // Publishing tail after argvArray keeps partially written records hidden
    // from the Host consumer. The ring is best-effort and may overwrite old
    // records when the consumer falls behind.
    result = static_cast<CcuResult>(ccu::Load(reinterpret_cast<uint64_t>(handle->valueAddressCell), address));
    if (result != CCU_SUCCESS) {
        return result;
    }
    return static_cast<CcuResult>(ccu::Store(address, nextTail));
}

template <size_t FormatSize, typename... Args>
CcuResult RegisterAndEmit(const char* filename, int lineno, const char (&fmtstr)[FormatSize], const Args&... args)
{
    PrintInfoHandle* handle = CcuPrintStream::GetInstance()->AllocPrintInfo(filename, lineno, fmtstr, sizeof...(Args));
    return EmitPrintf(handle, args...);
}

#define CCU_PRINTF(fmt, ...)                                                                                       \
    do {                                                                                                           \
        CcuResult ccuPrintResult = ::AscendC::ccu::dfx::RegisterAndEmit(__FILE__, __LINE__, (fmt), ##__VA_ARGS__); \
        if (ccuPrintResult != CCU_SUCCESS) {                                                                       \
            return ccuPrintResult;                                                                                 \
        }                                                                                                          \
    } while (0)

} // namespace dfx
} // namespace ccu
} // namespace AscendC

#endif // CCU_PRINT_H
