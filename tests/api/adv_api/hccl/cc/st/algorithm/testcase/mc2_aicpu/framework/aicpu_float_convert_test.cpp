/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software: you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>

#include <cmath>
#include <cstdint>
#include <cstring>

#include "aicpu_float_convert.h"
#include "sim_common.h"

using namespace HcclSim::AicpuSt;

// 穷举：全部 65536 个 half 位模式 half→float→half 恒等（NaN 保持 NaN）
TEST(AicpuStFloatConvert, HalfRoundTripAllBitPatterns)
{
    for (uint32_t h = 0; h <= 0xFFFF; ++h) {
        const uint16_t bits = static_cast<uint16_t>(h);
        const float f = HalfToFloat(bits);
        const uint16_t back = FloatToHalf(f);
        if (std::isnan(f)) {
            EXPECT_TRUE((back & 0x7C00) == 0x7C00 && (back & 0x3FF) != 0)
                << "NaN bit pattern 0x" << std::hex << h << " lost NaN-ness, back=0x" << back;
        } else {
            EXPECT_EQ(back, bits) << "roundtrip mismatch at 0x" << std::hex << h << " back=0x" << back;
        }
    }
}

// RNE：中点入偶（normal / 次正规 / 溢出边界）
TEST(AicpuStFloatConvert, FloatToHalfRoundsMidpointToEven)
{
    EXPECT_EQ(FloatToHalf(1.0f + 0.5f * (1.0f / 1024.0f)), 0x3C00); // 1.0 与 1.0+2^-10 中点 → 入偶 0x3C00
    EXPECT_EQ(FloatToHalf(1.0f + 1.5f * (1.0f / 1024.0f)), 0x3C02); // 0x3C01 与 0x3C02 中点 → 入偶 0x3C02
    EXPECT_EQ(FloatToHalf(1.5f * (1.0f / 16777216.0f)), 0x0002);    // 次正规 0x0001/0x0002 中点 → 入偶
    EXPECT_EQ(FloatToHalf(65520.0f), 0x7C00);                       // 65504/Inf 中点 → 入偶 Inf
    EXPECT_EQ(FloatToHalf(65519.9f), 0x7BFF);                       // 中点以下保持最大有限
    // 常规 RNE（最近值舍入）：2.9f = 2.900000095367431640625，最近 half 为 0x41CD(2.900390625)，
    // 距离 0.00039 < 半 ULP(2^-10)；0x4200(3.0) 距离 0.1——RNE 舍到最近值而非 3.0，
    // 舍到 3.0 需值不小于 2.9990234375（0x41FF/0x4200 中点）。原断言 0x4200 与 IEEE RNE 不符，已修正
    EXPECT_EQ(FloatToHalf(2.9f), 0x41CD);
    EXPECT_EQ(FloatToHalf(1.0f / 16777216.0f), 0x0001); // 最小次正规
}

// 饱和转换
TEST(AicpuStFloatConvert, StoreElemSaturatesIntegers)
{
    u8 buf8[1] = {0};
    StoreElem(buf8, HcclDataType::HCCL_DATA_TYPE_INT8, 200.0f);
    EXPECT_EQ(buf8[0], 0x7F);
    StoreElem(buf8, HcclDataType::HCCL_DATA_TYPE_INT8, -200.0f);
    EXPECT_EQ(buf8[0], 0x80);
    StoreElem(buf8, HcclDataType::HCCL_DATA_TYPE_INT8, 100.0f);
    EXPECT_EQ(static_cast<int8_t>(buf8[0]), 100);

    u8 buf32[4] = {0, 0, 0, 0};
    StoreElem(buf32, HcclDataType::HCCL_DATA_TYPE_INT32, 3.0e9f);
    int32_t out = 0;
    memcpy(&out, buf32, sizeof(out));
    EXPECT_EQ(out, 2147483647);
    StoreElem(buf32, HcclDataType::HCCL_DATA_TYPE_INT32, -3.0e9f);
    memcpy(&out, buf32, sizeof(out));
    EXPECT_EQ(out, (-2147483647 - 1));
    StoreElem(buf32, HcclDataType::HCCL_DATA_TYPE_INT32, 12345.0f);
    memcpy(&out, buf32, sizeof(out));
    EXPECT_EQ(out, 12345);
}

// 穷举：全部 65536 个 BFP16 位模式 bfp16→float→bfp16 恒等（BFP16 ⊂ FP32，截断返回必还原）；
// 附截断语义锚点（BFP16 按截断而非 RNE，与 FP16 不同——见头文件声明）
TEST(AicpuStFloatConvert, Bfp16RoundTripAllBitPatterns)
{
    for (uint32_t v = 0; v <= 0xFFFF; ++v) {
        const uint16_t bits = static_cast<uint16_t>(v);
        EXPECT_EQ(FloatToBfp16(Bfp16ToFloat(bits)), bits) << "roundtrip mismatch at 0x" << std::hex << v;
    }
    // 截断锚点：BFP16 尾数 7 位，[1,2) 区间 ULP=2^-7、半 ULP=2^-8
    EXPECT_EQ(FloatToBfp16(1.0f + (1.0f / 512.0f)), 0x3F80); // 1/4 ULP：半 ULP 以下截断 → 1.0
    EXPECT_EQ(
        FloatToBfp16(1.0f + (1.0f / 256.0f) + (1.0f / 512.0f)),
        0x3F80); // 3/4 ULP：超半 ULP 仍截断 → 1.0（RNE 会入到 0x3F81）
    EXPECT_EQ(FloatToBfp16(1.0f + (1.0f / 128.0f) + (1.0f / 256.0f)), 0x3F81); // 1.5 ULP：超 1 ULP 截断进位 → 1 + 2^-7
}
