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
#include "sys_macros.h"

namespace AscendC {
enum class Hardware : uint8_t { GM, UB, L1, L0A, L0B, L0C, BIAS, FIXBUF, MAX };

enum class ExpAlgo {
    INTRINSIC = 0,
    PRECISION_1ULP_FTZ_TRUE,
    PRECISION_1ULP_FTZ_FALSE,
};
struct ExpConfig {
    ExpAlgo algo = ExpAlgo::INTRINSIC;
};

constexpr ExpConfig DEFAULT_EXP_CONFIG = {ExpAlgo::INTRINSIC};

enum class LnAlgo {
    INTRINSIC = 0,
    PRECISION_1ULP_FTZ_TRUE,
    PRECISION_1ULP_FTZ_FALSE,
};
struct LnConfig {
    LnAlgo algo = LnAlgo::INTRINSIC;
};

constexpr LnConfig DEFAULT_LN_CONFIG = {LnAlgo::INTRINSIC};

enum class LogAlgo {
    INTRINSIC = 0,
    PRECISION_1ULP_FTZ_TRUE,
    PRECISION_1ULP_FTZ_FALSE,
};
struct LogConfig {
    LogAlgo algo = LogAlgo::INTRINSIC;
};

constexpr LogConfig DEFAULT_Log_CONFIG = {LogAlgo::INTRINSIC};

enum class Log2Algo {
    INTRINSIC = 0,
    PRECISION_1ULP_FTZ_TRUE,
    PRECISION_1ULP_FTZ_FALSE,
};
struct Log2Config {
    Log2Algo algo = Log2Algo::INTRINSIC;
};

constexpr Log2Config DEFAULT_Log2_CONFIG = {Log2Algo::INTRINSIC};

enum class Log10Algo {
    INTRINSIC = 0,
    PRECISION_1ULP_FTZ_TRUE,
    PRECISION_1ULP_FTZ_FALSE,
};
struct Log10Config {
    Log10Algo algo = Log10Algo::INTRINSIC;
};

constexpr Log10Config DEFAULT_Log10_CONFIG = {Log10Algo::INTRINSIC};

enum class ReciprocalAlgo {
    INTRINSIC = 0,
    PRECISION_1ULP_FTZ_TRUE,
    PRECISION_1ULP_FTZ_FALSE,
};
struct ReciprocalConfig {
    ReciprocalAlgo algo = ReciprocalAlgo::INTRINSIC;
};

constexpr ReciprocalConfig DEFAULT_RECIPROCAL_CONFIG = {ReciprocalAlgo::INTRINSIC};

enum class DivAlgo {
    INTRINSIC = 0,
    DIFF_COMPENSATION,
    PRECISION_1ULP_FTZ_TRUE,
    PRECISION_0ULP_FTZ_TRUE,
    PRECISION_0ULP_FTZ_FALSE,
    PRECISION_1ULP_FTZ_FALSE
};
struct DivConfig {
    DivAlgo algo = DivAlgo::INTRINSIC;
};

constexpr DivConfig DEFAULT_DIV_CONFIG = {DivAlgo::INTRINSIC};

enum class SqrtAlgo {
    INTRINSIC = 0,
    FAST_INVERSE,
    PRECISION_1ULP_FTZ_TRUE,
    PRECISION_0ULP_FTZ_FALSE,
    PRECISION_1ULP_FTZ_FALSE,
};
struct SqrtConfig {
    SqrtAlgo algo = SqrtAlgo::INTRINSIC;
};

constexpr SqrtConfig DEFAULT_SQRT_CONFIG = {SqrtAlgo::INTRINSIC};

enum class RsqrtAlgo {
    INTRINSIC = 0,
    FAST_INVERSE,
    PRECISION_1ULP_FTZ_TRUE,
    PRECISION_0ULP_FTZ_FALSE,
    PRECISION_1ULP_FTZ_FALSE,
};
struct RsqrtConfig {
    RsqrtAlgo algo = RsqrtAlgo::INTRINSIC;
};

constexpr RsqrtConfig DEFAULT_RSQRT_CONFIG = {RsqrtAlgo::INTRINSIC};

enum class ReduceType {
    NONE = -1,
    SUM = 0,
    MAX,
    MIN,
};
} // namespace AscendC

#if !defined(ASCENDC_CPU_DEBUG)
#if (defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3510))) || defined(__NPU_HOST__) || defined(__ASC_NPU_HOST__)
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
#endif

constexpr uint32_t INT4_BIT_NUM = 4;

template <bool condition, class T1, class T2>
struct conditional {
    using type = T1;
};

template <int bit_num, bool sign = true>
struct integer_sub_type {
    static int const k_bits = bit_num;
    static bool const k_signed = sign;

    using T = typename conditional<k_signed, int8_t, uint8_t>::type;
    using Storage = uint8_t;

    static Storage const mask = Storage(((static_cast<uint64_t>(1)) << static_cast<uint32_t>(k_bits)) - 1);
    Storage storage;
    __aicore__ inline integer_sub_type() = default;

    __aicore__ inline integer_sub_type(uint32_t value)
        : storage(reinterpret_cast<Storage const &>(value) & mask) {}

    __aicore__ inline integer_sub_type(int32_t value)
        : storage(reinterpret_cast<Storage const &>(value) & mask) {}

    __aicore__ inline operator T() const
    {
        if (k_signed && ((storage & Storage(static_cast<uint64_t>(1) << static_cast<uint32_t>(k_bits - 1))) != 0)) {
            // Sign extend
            return T(storage) | ~T(mask);
        }
        return T(storage);
    }

    __aicore__ inline bool operator == (integer_sub_type const &rhs) const
    {
        return storage == rhs.storage;
    }

    __aicore__ inline bool operator != (integer_sub_type const &rhs) const
    {
        return storage != rhs.storage;
    }

    __aicore__ inline bool operator > (integer_sub_type const &rhs) const
    {
        bool lhs_is_neg = (this->storage & (static_cast<uint64_t>(1) << static_cast<uint32_t>(this->k_bits - 1)));
        bool rhs_is_neg = (rhs.storage & (static_cast<uint64_t>(1) << static_cast<uint32_t>(rhs.k_bits - 1)));
        if (k_signed && (lhs_is_neg != rhs_is_neg)) {
            return (!lhs_is_neg) && rhs_is_neg;
        }
        return this->storage > rhs.storage;
    }

    __aicore__ inline bool operator >= (integer_sub_type const &rhs) const
    {
        bool lhs_is_neg = (this->storage & (static_cast<uint64_t>(1) << static_cast<uint32_t>(this->k_bits - 1)));
        bool rhs_is_neg = (rhs.storage & (static_cast<uint64_t>(1) << static_cast<uint32_t>(rhs.k_bits - 1)));
        if (k_signed && (lhs_is_neg != rhs_is_neg)) {
            return (!lhs_is_neg) && rhs_is_neg;
        }
        return storage >= rhs.storage;
    }

    __aicore__ inline bool operator < (integer_sub_type const &rhs) const
    {
        return !(*this >= rhs);
    }

    __aicore__ inline bool operator <= (integer_sub_type const &rhs) const
    {
        return !(*this > rhs);
    }
};

using int4b_t = integer_sub_type<INT4_BIT_NUM, true>;

#endif
