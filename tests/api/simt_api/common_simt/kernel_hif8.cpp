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
 * \file kernel_hif8.cpp
 * \brief
 */
#include <cmath>
#include <algorithm>
#include "kernel_hif8.h"
#include "kernel_fp32.h"
#include "kernel_utils.h"

namespace hifloat8 {
namespace {
// HiFloat8
#define HIF8_SIGN_INDEX (7)
#define HIF8_BIT6_INDEX (6)
#define HIF8_BIT5_INDEX (5)
#define HIF8_BIT4_INDEX (4)
#define HIF8_BIT3_INDEX (3)
#define HIF8_EXTRACT_SIGN(x) (((x) >> HIF8_SIGN_INDEX) & 0x1)
#define HIF8_EXTRACT_BIT6(x) (((x) >> HIF8_BIT6_INDEX) & 0x1)
#define HIF8_EXTRACT_BIT5(x) (((x) >> HIF8_BIT5_INDEX) & 0x1)
#define HIF8_EXTRACT_BIT4(x) (((x) >> HIF8_BIT4_INDEX) & 0x1)
#define HIF8_EXTRACT_BIT3(x) (((x) >> HIF8_BIT3_INDEX) & 0x1)

constexpr int8_t HIF8_NAN = 0x80;
constexpr int8_t HIF8_POS_INF = 0x6F;
constexpr int8_t HIF8_NEG_INF = 0xEF;
constexpr int8_t HIF8_BIT_LEN = 8;

#define HIF8_MAX (0x6E)
#define HIF8_NEG_MAX (0xEE)

uint32_t Hif8ToFloatCommon(const int8_t input)
{
    uint32_t ret = 0;
    uint8_t inputSign = HIF8_EXTRACT_SIGN(input);
    uint8_t bit6 = HIF8_EXTRACT_BIT6(input);
    uint8_t bit5 = HIF8_EXTRACT_BIT5(input);
    uint8_t bit4 = HIF8_EXTRACT_BIT4(input);
    uint8_t bit3 = HIF8_EXTRACT_BIT3(input);
    uint8_t dBitWidth = 0;
    uint8_t expBitWidth = 0;
    uint8_t manBitWidth = 0;

    // special case
    if (input == 0x0) {
        return 0;
    }
    if (input == HIF8_NAN) {
        return FP32_NAN;
    }
    if (input == HIF8_POS_INF) {
        return FP32_POS_INF;
    }
    if (input == HIF8_NEG_INF) {
        return FP32_NEG_INF;
    }

    // denormal value m - 23
    if (((bit6 == 0) && (bit5 == 0) && (bit4 == 0) && (bit3 == 0))) {
        return Fp32Constructor(inputSign, (input & 0x7) - FP32_MAN_LEN + FP32_EXP_BIAS, 0);
    }

    // DBitWidth, ManBitWidth, ExpBitWidth
    if (bit6 == 0) {
        if (bit5 == 0) {
            if (bit4 == 0) {  // D = b000
                expBitWidth = 0;
                dBitWidth = 4;
            } else {  // D = b001
                expBitWidth = 1;
                dBitWidth = 3;
            }
        } else {  // D = b01
            expBitWidth = 2;
            dBitWidth = 2;
        }
    } else {
        if (bit5 == 0) {  // D == b10
            expBitWidth = 3;
        } else {  // D == b11
            expBitWidth = 4;
        }
        dBitWidth = 2;
    }
    manBitWidth = 8 - dBitWidth - expBitWidth - 1;

    uint8_t expBitMask = 0x0;
    // extract exp bits
    for (uint8_t i = 0; i < expBitWidth; i++) {
        expBitMask |= 0x1 << i;  // e.g. expBitWidth = 4 -> 1111
    }

    // cal Exp value
    int8_t exp = 0;
    int8_t expMsb = 0;
    expMsb = (input >> (manBitWidth + expBitWidth - 1)) & 0x1;
    if (expBitWidth != 0) {
        exp = ((input >> manBitWidth) & (expBitMask)) | (1 << (expBitWidth - 1));  // (1xxx)
        if (expMsb != 0) {                                                              // -ve
            exp = -1 * exp;
        }
    }
    exp = exp + FP32_EXP_BIAS;

    // cal Man value
    uint32_t man = 0;
    uint8_t manBitMask = 0;
    for (uint8_t i = 0; i < manBitWidth; i++) {
        manBitMask |= 0x1 << i;
    }
    man = input & manBitMask;
    man = man << (22 - manBitWidth + 1);

    ret = Fp32Constructor(inputSign, (exp & 0xff), man);
    return ret;
}

uint32_t Hif8ToFp32(const int8_t fpVal)
{
    return Hif8ToFloatCommon(fpVal);
}

bool IsRoundOne(uint32_t sign, uint64_t man, uint16_t truncLen)
{
    (void)sign;
    if (truncLen == 0) {
        return false;
    }
    uint64_t roundingTruncLen = 64;
    uint64_t mask0 = (truncLen >= roundingTruncLen) ? 0 : 0x1ul << truncLen;

    uint64_t maskManDecimalHighestBit = mask0 >> 1;
    // ROUND_AWAY_FROM_ZERO
    bool posFlag = (sign == 0) && ((man & maskManDecimalHighestBit) > 0);
    bool negFlag = (sign == 1) && ((man & maskManDecimalHighestBit) > 0);
    return (sign == 0) ? posFlag : negFlag;
}

int16_t HiF8EvToE(int16_t input)
{
    int16_t output;
    if (input < 0) {
        output = -input;
    } else if (input >= 8) {
        output = input - 8;
    } else if (input >= 4) {
        output = input - 4;
    } else if (input >= 2) {
        output = input - 2;
    } else if (input >= 1) {
        output = input - 1;
    } else {
        output = 0;
    }
    return output;
}

}  // namespace


int8_t Hif8T::FloatToHif8(const float src) const
{
    uint32_t srcVal = AscendC::GetScalarBitcodeValue<float, uint32_t>(src);
    int8_t sign = Fp32ExtracSign(srcVal);
    if (Fp32IsNan(srcVal)) {
        return 0x80;  // 0xFE;
    }
    if ((Fp32IsZero(srcVal)) || ((sign != 1) && (srcVal <= 0x33800000)) || ((sign != 0) && (srcVal <= 0xB3800000))) {
        return 0x0;  // 0x7E; // und flow
    }
    if ((sign != 1) && (srcVal >= 0x47200000)) {
        return 0x6f;  // +ve inf // ove flow
    }
    if ((sign != 0) && (srcVal >= 0xC7200000)) {
        return 0xef;  // -ve inf // ove flow
    }

    int16_t expHf = Fp32ExtracExp(srcVal) - FP32_EXP_BIAS;
    int16_t exp = expHf;
    uint8_t sBitWidth = 1;
    uint8_t expBitWidth = 0;
    uint8_t dBitWidth = 0;
    uint8_t manBitWidth = 0;
    uint8_t manBitMask = 0;
    int8_t d = 0;
    uint32_t manHf = Fp32ExtracMan(srcVal);
    uint16_t man = 0;
    int8_t ret = 0;
    if (exp == 0) {
        d = 1;
        dBitWidth = 4;
        expBitWidth = 0;
    } else if ((exp == 1) || (exp == -1)) {
        d = 1;
        dBitWidth = 3;
        expBitWidth = 1;
    } else {
        d = static_cast<int8_t>(std::log2(std::abs(exp)));
        dBitWidth = 2;
        expBitWidth = d + 1;
    }
    manBitWidth = HIF8_BIT_LEN - sBitWidth - dBitWidth - expBitWidth;

    uint8_t signExp = 0;
    constexpr uint16_t expOffset = 15;

    if (expBitWidth == 1) {
        if (((static_cast<uint16_t>(exp) >> expOffset) & 0x1) != 0) {
            exp = 1;
            signExp = 1;
        } else {
            exp = 0;
        }
    } else if (expBitWidth > 1) {
        if (((static_cast<uint16_t>(exp) >> expOffset) & 0x1) != 0) {
            exp = (static_cast<int16_t>(~static_cast<uint32_t>(exp)) + 1);
            signExp = 1;
        } else {
            exp = exp - (1 << d);  // 0XE800
        }
    }

    for (int8_t i = 0; i < manBitWidth; i++) {
        manBitMask |= (0x1 << i);
    }
    man = manHf >> (FP32_MAN_LEN - manBitWidth);

    bool needRound = false;

    // cal the mantissa and rounding
    // HR (TA + SR)
    if ((expHf > -4) && (expHf < 4)) {  // TA
        needRound = IsRoundOne(sign, manHf, (FP32_MAN_LEN - (HIF8_BIT_LEN - (1 + dBitWidth + expBitWidth))));
    } else {                    // SR
        uint16_t fraction = 0;  // F14

        // step1: get the 14 bit thresholdthank
        uint16_t threshold = manHf & 0x3FFF;  // T14: LSB 14 bits
        if (expHf != -23) {
            fraction = (manHf >> (FP32_MAN_LEN - manBitWidth - 14)) & 0x3FFF;
        } else {
            fraction = ((manHf >> (FP32_MAN_LEN - 13)) & 0x1FFF) | 0x2000;
        }

        // step2: compare threshold vs manHf
        if (fraction >= threshold) {
            needRound = true;
        }
    }

    uint32_t mMin = 1 << manBitWidth;
    uint32_t mMax = mMin << 1;
    uint16_t manR = man;
    int16_t expHfR = 0;
    if (needRound) {
        manR++;
    }
    // level up and down
    expHfR = expHf;
    if (expHf < -23) {
        return 0;
    } else if (expHf > -16) {
        while (manR >= mMax) {
            // when man But width need add
            manR = 0;  // to prevent shift out 1
            exp = exp + 1;
            expHfR = expHf + 1;  // Ev +1

            // update bitwidth after rounding if needed
            if ((expHf == -8) || (expHf == -4) || (expHf == -2) || (expHf == -1) || (expHf == 0) || (expHf == 1) ||
                (expHf == 3) || (expHf == 7)) {
                if (signExp != 0) {  // go up
                    expBitWidth = static_cast<uint8_t>(expBitWidth - 1);
                    if (!((expHf == -2) || (expHf == -1))) {
                        manBitWidth = manBitWidth + 1;
                        d = d - 1;
                    }
                } else {  // go down
                    expBitWidth = expBitWidth + 1;
                    if (!((expHf == 1) || (expHf == 0))) {
                        manBitWidth = manBitWidth - 1;
                        d = d + 1;
                    }
                    exp = 0;
                }
                for (int8_t i = 0; i < manBitWidth; i++) {
                    manBitMask |= (0x1 << i);
                }
            }
        }
    } else if ((expHf <= -16) && (expHf >= -23)) {  // denormal //exponent in [-16,-23]
        manR = expHf + 23;
        d = 0;
        if (expHf == -23) {
            // a. needRound = true, exp = -23 (Ev = -23), exp + 1, hardcode the mantissa as 0. the denormal case
            // b. needRound = false, exp = -23 (Ev = -23), hardcode the mantissa as 0 , underflow case
            expHf = -23;
            // case a --> denormal case
            if (!needRound) {
                // case b --> underflow case
                return 0;
            }
        } else if (expHf == -16) {
            // a. needRound = true, exp = -16 (Ev = -16), exp +1, become normal case
            // b. needRound = false, exp = -16 (Ev = -16), exp = 7, turn to denormal case
            if (needRound) {  // case a, go to normal value --> to lower level
                manR = 0;
                expHfR = -15;  // expHf +1 = -16 + 1 = -15
                d = 3;
                dBitWidth = 2;
                expBitWidth = 4;
                manBitWidth = 1;
            }
        }
    }
    man = manR;
    if ((!(expHfR < -16)) && !((expHfR == -16) && (!needRound))) {  // normal case
        exp = HiF8EvToE(expHfR);
        ret = ((static_cast<int8_t>(sign) << HIF8_SIGN_INDEX) |
               (static_cast<int8_t>(d) << (expBitWidth + manBitWidth)) |
               (static_cast<int8_t>(exp) << manBitWidth) |
               (static_cast<int8_t>(man) & manBitMask));
    } else if ((expHf <= -16) && (expHf >= -23)) {  // denormal case // x = (-1)^sign * 2^(M-23)
        if (((expHf == -23) && (needRound)) || ((expHf == -16) && (!needRound))) {
            exp = 0;
            d = 0;
            expBitWidth = 0;
        }
        if (needRound) {
            man = man + 1;
        }
        ret = (((sign) << HIF8_SIGN_INDEX) | (0 << 3) | man);
        // merge exp in fp32 srcVal as the M in hif8 output
    }
    return ret;
}

Hif8T::operator float() const
{
    return AscendC::GetScalarBitcodeValue<uint32_t, float>(Hif8ToFp32(val));
}

float Hif8T::ToFloat() const
{
    return AscendC::GetScalarBitcodeValue<uint32_t, float>(Hif8ToFp32(val));
}
}  // namespace hifloat8
