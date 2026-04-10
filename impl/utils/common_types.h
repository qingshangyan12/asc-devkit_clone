/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file common_types.h
 * \brief
 */
#ifndef IMPL_UTILS_COMMON_TYPES_H
#define IMPL_UTILS_COMMON_TYPES_H

#include <cstdint>
#include <type_traits>

namespace AscendC {
enum class Hardware : uint8_t { GM, UB, L1, L0A, L0B, L0C, BIAS, FIXBUF, MAX };
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3510)) || defined(__ASC_NPU_HOST__)
    using fp4x2_e2m1_t = float4_e2m1x2_t;
    using fp4x2_e1m2_t = float4_e1m2x2_t;
    using fp8_e5m2_t = float8_e5m2_t;
    using fp8_e4m3fn_t = float8_e4m3_t;
    using fp8_e8m0_t = float8_e8m0_t;
#else
    using fp4x2_e2m1_t = uint8_t;
    using fp4x2_e1m2_t = uint8_t;
    using fp8_e5m2_t = uint8_t;
    using fp8_e4m3fn_t = uint8_t;
    using fp8_e8m0_t = uint8_t;
#endif

template <bool condition, class T1, class T2>
struct Conditional {
    using type = T1;
};

template <class T1, class T2>
struct Conditional<false, T1, T2> {
    using type = T2;
};

template <int bitNum, bool sign = true>
struct IntegerSubType {
    static int const kBits = bitNum;
    static bool const kSigned = sign;

    using T = typename Conditional<kSigned, int8_t, uint8_t>::type;
    using Storage = uint8_t;

    static Storage const mask = Storage(((static_cast<uint64_t>(1)) << static_cast<uint32_t>(kBits)) - 1);
    Storage storage;
    __aicore__ inline IntegerSubType() = default;

    __aicore__ inline IntegerSubType(uint32_t value)
        : storage(reinterpret_cast<Storage const &>(value) & mask) {}

    __aicore__ inline IntegerSubType(int32_t value)
        : storage(reinterpret_cast<Storage const &>(value) & mask) {}

    __aicore__ inline operator T() const
    {
        if (kSigned && ((storage & Storage(static_cast<uint64_t>(1) << static_cast<uint32_t>(kBits - 1))) != 0)) {
            // Sign extend
            return T(storage) | ~T(mask);
        }
        return T(storage);
    }

    __aicore__ inline bool operator == (IntegerSubType const &rhs) const
    {
        return storage == rhs.storage;
    }

    __aicore__ inline bool operator != (IntegerSubType const &rhs) const
    {
        return storage != rhs.storage;
    }

    __aicore__ inline bool operator > (IntegerSubType const &rhs) const
    {
        bool lhsIsNeg = (this->storage & (static_cast<uint64_t>(1) << static_cast<uint32_t>(this->kBits - 1)));
        bool rhsIsNeg = (rhs.storage & (static_cast<uint64_t>(1) << static_cast<uint32_t>(rhs.kBits - 1)));
        if (kSigned && (lhsIsNeg != rhsIsNeg)) {
            return (!lhsIsNeg) && rhsIsNeg;
        }
        return this->storage > rhs.storage;
    }

    __aicore__ inline bool operator >= (IntegerSubType const &rhs) const
    {
        bool lhsIsNeg = (this->storage & (static_cast<uint64_t>(1) << static_cast<uint32_t>(this->kBits - 1)));
        bool rhsIsNeg = (rhs.storage & (static_cast<uint64_t>(1) << static_cast<uint32_t>(rhs.kBits - 1)));
        if (kSigned && (lhsIsNeg != rhsIsNeg)) {
            return (!lhsIsNeg) && rhsIsNeg;
        }
        return storage >= rhs.storage;
    }

    __aicore__ inline bool operator < (IntegerSubType const &rhs) const
    {
        return !(*this >= rhs);
    }

    __aicore__ inline bool operator <= (IntegerSubType const &rhs) const
    {
        return !(*this > rhs);
    }
};

constexpr uint32_t INT4_BIT_NUM = 4;
using int4b_t = IntegerSubType<INT4_BIT_NUM, true>;

#if (__NPU_ARCH__ == 5102)
using int2b_t = IntegerSubType<INT2_BIT_NUM, true>;
using int1b_t = IntegerSubType<INT1_BIT_NUM, true>;
using uint1b_t = IntegerSubType<INT1_BIT_NUM, false>;
#endif

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
using uint4b_t = IntegerSubType<INT4_BIT_NUM, false>;
using uint3b_t = IntegerSubType<INT3_BIT_NUM, false>;
using uint2b_t = IntegerSubType<INT2_BIT_NUM, false>;
#endif

#endif