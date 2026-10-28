/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software: you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef AICPU_ST_FLOAT_CONVERT_H
#define AICPU_ST_FLOAT_CONVERT_H

#include <cstdint>
#include <cstring>

#include "sim_common.h"

// ST 框架共享的浮点转换函数（header-only）：
// - FP16 转换按 IEEE RNE（中点入偶）+ 次正规 + sNaN 保持
// - BFP16 按截断语义（Arm bfloat16 约定），与 FP16 的 RNE 不同；
//   golden 生成必须使用同一组函数保证策略自洽
// - 整数存储为饱和转换（超范围输入本不应出现，饱和仅为防御 float→int UB）
namespace HcclSim {
namespace AicpuSt {

inline float HalfToFloat(uint16_t h)
{
    const uint32_t sign = static_cast<uint32_t>(h & 0x8000) << 16;
    const uint32_t exp = (h >> 10) & 0x1F;
    const uint32_t frac = h & 0x3FF;
    uint32_t bits = 0;
    if (exp == 0) {
        if (frac == 0) {
            bits = sign;
        } else {
            // 次正规数归一化
            uint32_t e = 0;
            uint32_t f = frac;
            while ((f & 0x400) == 0) {
                f <<= 1;
                ++e;
            }
            bits = sign | ((127 - 15 - e + 1) << 23) | ((f & 0x3FF) << 13);
        }
    } else if (exp == 0x1F) {
        bits = sign | 0x7F800000 | (frac << 13);
    } else {
        bits = sign | ((exp - 15 + 127) << 23) | (frac << 13);
    }
    float out = 0.0f;
    (void)memcpy(&out, &bits, sizeof(out));
    return out;
}

// float→FP16，IEEE RNE（中点入偶）舍入，含次正规与 sNaN 保持
inline uint16_t FloatToHalf(float f)
{
    uint32_t x = 0;
    memcpy(&x, &f, sizeof(x));
    const uint32_t sign = (x >> 16) & 0x8000;
    x &= 0x7FFFFFFF; // 绝对值位模式

    if (x >= 0x7F800000) {
        // Inf/NaN：保留高位尾数；NaN 舍入后尾数为 0 时置 LSB，防 sNaN 静默变 Inf
        uint16_t out = static_cast<uint16_t>(sign | 0x7C00 | ((x >> 13) & 0x3FF));
        if ((x & 0x7FFFFF) != 0 && (out & 0x3FF) == 0) {
            out |= 0x1;
        }
        return out;
    }
    // 溢出→Inf 阈值：0x477FF000 是 65520.0f 的位模式（half 最大有限 65504 与 Inf 的中点，
    // RNE 入偶到 Inf）。正浮点数与位模式同序，故按位模式比较等价于 |值| >= 65520
    if (x >= 0x477FF000u) {
        return static_cast<uint16_t>(sign | 0x7C00);
    }

    const uint32_t exp32 = x >> 23;
    const uint32_t frac32 = x & 0x7FFFFF;
    int32_t e = static_cast<int32_t>(exp32) - 127 + 15;           // half 目标指数；<=0 表示次正规
    const uint32_t mant = (exp32 != 0 ? 0x800000u : 0u) | frac32; // 24 位尾数（含隐含位）

    if (e >= 1) {
        uint32_t m = mant >> 13; // [0x400, 0x800)
        const uint32_t rem = mant & 0x1FFF;
        if (rem > 0x1000 || (rem == 0x1000 && (m & 1u) != 0)) {
            m += 1; // RNE
        }
        if (m == 0x800) { // 进位到下一指数
            m = 0x400;
            e += 1;
        }
        if (e >= 31) { // 阈值分支外的防御（正常不可达）
            return static_cast<uint16_t>(sign | 0x7C00);
        }
        return static_cast<uint16_t>(sign | (static_cast<uint32_t>(e) << 10) | (m - 0x400));
    }
    // e <= 0：目标次正规（或 0）。half 次正规值 = k * 2^-24，k = round(mant >> (14 - e))
    const int32_t shift = 14 - e; // [14, 126]
    uint32_t k = 0;
    uint32_t rem = 0;
    uint32_t halfUlp = 0;
    if (shift < 32) { // mant 仅 24 位，shift>=32 必然舍到 0（也避免 1u<<shift 越界）
        k = mant >> shift;
        rem = mant & ((1u << shift) - 1u);
        halfUlp = 1u << (shift - 1);
    }
    if (rem > halfUlp || (rem == halfUlp && (k & 1u) != 0)) {
        k += 1; // RNE
    }
    if (k == 0x400) { // 次正规进位到最小 normal
        return static_cast<uint16_t>(sign | (1u << 10));
    }
    return static_cast<uint16_t>(sign | k);
}

inline float Bfp16ToFloat(uint16_t v)
{
    const uint32_t bits = static_cast<uint32_t>(v) << 16;
    float out = 0.0f;
    (void)memcpy(&out, &bits, sizeof(out));
    return out;
}

// BFP16 按截断语义（Arm bfloat16 约定），与 FP16 的 RNE 不同；
// golden 生成必须使用同一组函数保证策略自洽
inline uint16_t FloatToBfp16(float f)
{
    uint32_t bits = 0;
    (void)memcpy(&bits, &f, sizeof(bits));
    return static_cast<uint16_t>(bits >> 16);
}

// 逐元素读取。memcpy 到局部变量再读：避免对 u8* 做类型化解引用（别名/对齐理论 UB）
inline float LoadElem(const u8* p, HcclDataType dt)
{
    switch (dt) {
        case HcclDataType::HCCL_DATA_TYPE_INT8: {
            s8 v = 0;
            (void)memcpy(&v, p, sizeof(v));
            return static_cast<float>(v);
        }
        case HcclDataType::HCCL_DATA_TYPE_INT32: {
            s32 v = 0;
            (void)memcpy(&v, p, sizeof(v));
            return static_cast<float>(v);
        }
        case HcclDataType::HCCL_DATA_TYPE_FP16: {
            uint16_t v = 0;
            (void)memcpy(&v, p, sizeof(v));
            return HalfToFloat(v);
        }
        case HcclDataType::HCCL_DATA_TYPE_BFP16: {
            uint16_t v = 0;
            (void)memcpy(&v, p, sizeof(v));
            return Bfp16ToFloat(v);
        }
        case HcclDataType::HCCL_DATA_TYPE_FP32: {
            float v = 0.0f;
            (void)memcpy(&v, p, sizeof(v));
            return v;
        }
        default:
            return 0.0f;
    }
}

// 逐元素写回（memcpy，理由同 LoadElem）。整数饱和为 ST 框架约定（超范围输入本不应出现），
// 目的是消除 float→int 超范围/NaN 的 UB；NaN 输入防御性写 0
inline void StoreElem(u8* p, HcclDataType dt, float v)
{
    switch (dt) {
        case HcclDataType::HCCL_DATA_TYPE_INT8: {
            s8 out = 0;
            if (v != v) { // NaN
                out = 0;
            } else if (v >= 127.0f) {
                out = 127;
            } else if (v <= -128.0f) {
                out = -128;
            } else {
                out = static_cast<s8>(v);
            }
            (void)memcpy(p, &out, sizeof(out));
            return;
        }
        case HcclDataType::HCCL_DATA_TYPE_INT32: {
            s32 out = 0;
            if (v != v) { // NaN
                out = 0;
            } else if (v >= 2147483648.0f) {
                // float 无法精确表示 INT32_MAX（最近可表示值为 2^31），
                // 故以 2^31 为判定上界、饱和为近似上界 INT32_MAX 即可
                out = 2147483647;
            } else if (v <= -2147483648.0f) {
                out = (-2147483647 - 1); // -2^31 精确可表示
            } else {
                out = static_cast<s32>(v);
            }
            (void)memcpy(p, &out, sizeof(out));
            return;
        }
        case HcclDataType::HCCL_DATA_TYPE_FP16: {
            const uint16_t out = FloatToHalf(v);
            (void)memcpy(p, &out, sizeof(out));
            return;
        }
        case HcclDataType::HCCL_DATA_TYPE_BFP16: {
            const uint16_t out = FloatToBfp16(v);
            (void)memcpy(p, &out, sizeof(out));
            return;
        }
        case HcclDataType::HCCL_DATA_TYPE_FP32: {
            (void)memcpy(p, &v, sizeof(v));
            return;
        }
        default:
            return;
    }
}

inline float ApplyOp(float a, float b, HcclReduceOp op)
{
    switch (op) {
        case HcclReduceOp::HCCL_REDUCE_SUM:
            return a + b;
        case HcclReduceOp::HCCL_REDUCE_PROD:
            return a * b;
        case HcclReduceOp::HCCL_REDUCE_MAX:
            return (a > b) ? a : b;
        case HcclReduceOp::HCCL_REDUCE_MIN:
            return (a < b) ? a : b;
        default:
            return b;
    }
}

// ===== 整数精确路径：INT8/INT32 归约经 int64 中间值，规避 float 在 |v| > 2^24 的舍入 =====

inline bool IsIntegerReduceType(HcclDataType dt)
{
    return dt == HcclDataType::HCCL_DATA_TYPE_INT8 || dt == HcclDataType::HCCL_DATA_TYPE_INT32;
}

inline int64_t LoadIntElem(const u8* p, HcclDataType dt)
{
    switch (dt) {
        case HcclDataType::HCCL_DATA_TYPE_INT8: {
            s8 v = 0;
            (void)memcpy(&v, p, sizeof(v));
            return v;
        }
        case HcclDataType::HCCL_DATA_TYPE_INT32: {
            s32 v = 0;
            (void)memcpy(&v, p, sizeof(v));
            return v;
        }
        default:
            return 0;
    }
}

inline void StoreIntElem(u8* p, HcclDataType dt, int64_t v)
{
    switch (dt) {
        case HcclDataType::HCCL_DATA_TYPE_INT8: {
            if (v < -128) {
                v = -128; // 饱和为 ST 框架约定（超范围输入本不应出现）
            }
            if (v > 127) {
                v = 127;
            }
            const s8 out = static_cast<s8>(v);
            (void)memcpy(p, &out, sizeof(out));
            return;
        }
        case HcclDataType::HCCL_DATA_TYPE_INT32: {
            constexpr int64_t kInt32Min = -2147483648LL;
            constexpr int64_t kInt32Max = 2147483647LL;
            if (v < kInt32Min) {
                v = kInt32Min;
            }
            if (v > kInt32Max) {
                v = kInt32Max;
            }
            const s32 out = static_cast<s32>(v);
            (void)memcpy(p, &out, sizeof(out));
            return;
        }
        default:
            return;
    }
}

inline int64_t ApplyIntOp(int64_t a, int64_t b, HcclReduceOp op)
{
    switch (op) {
        case HcclReduceOp::HCCL_REDUCE_SUM:
            return a + b;
        case HcclReduceOp::HCCL_REDUCE_PROD:
            return a * b; // 长链可溢出 int64——golden 与执行器同路径，自洽；声明即可
        case HcclReduceOp::HCCL_REDUCE_MAX:
            return (a > b) ? a : b;
        case HcclReduceOp::HCCL_REDUCE_MIN:
            return (a < b) ? a : b;
        default:
            return b;
    }
}

inline uint64_t ElemSize(HcclDataType dt)
{
    switch (dt) {
        case HcclDataType::HCCL_DATA_TYPE_INT8:
            return 1;
        case HcclDataType::HCCL_DATA_TYPE_INT32:
        case HcclDataType::HCCL_DATA_TYPE_FP32:
            return 4;
        case HcclDataType::HCCL_DATA_TYPE_FP16:
        case HcclDataType::HCCL_DATA_TYPE_BFP16:
            return 2;
        default:
            return 0;
    }
}

} // namespace AicpuSt
} // namespace HcclSim

#endif // AICPU_ST_FLOAT_CONVERT_H
