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
 * \file kernel_fp4_e1m2.cpp
 * \brief
 */
#include "kernel_fp4_e1m2.h"
#include "kernel_fp32.h"

namespace float4_e1m2 {
static bool IsRoundOne(uint32_t sign, uint64_t man, uint16_t truncLen)
{
    (void)sign;
    if (truncLen == 0) {
        return false;
    }
    uint64_t roundingTruncLen = 64;
    uint64_t mask0 = (truncLen >= roundingTruncLen) ? 0 : 0x1ul << truncLen;
    uint64_t mask1 = (truncLen > roundingTruncLen) ? 0 : 0x1ul << (truncLen - 1);
    uint64_t mask2 = mask1 - 1;

    // ROUND_TO_NEAREST
    bool lastBit = ((man & mask0) > 0);      // Last bit after conversion
    bool truncHighBit = ((man & mask1) > 0); // Highest bit in the truncated part
    bool truncLeft = ((man & mask2) > 0);    // Truncated left part (except for the highest bit)
    return (truncHighBit && (truncLeft || lastBit));
}
static void Fp4e1m2Normalize(uint16_t& exp, uint16_t& man)
{
    if (exp >= FP4_E1M2_MAX_EXP) {
        exp = FP4_E1M2_MAX_EXP;
    } else if (exp == 0 && man == FP4_E1M2_MAN_HIDE_BIT) {
        exp++;
        man = 0;
    }
}
uint8_t Fp4e1m2T::BfloatToFp4e1m2(const uint16_t src) const
{
    if (bfloat16::Bf16IsInf(src)) {
        return ((bfloat16::Bf16ExtracSign(src) << FP4_SIGN_INDEX) | FP4_T_POS_MAX);
    }

    if (bfloat16::Bf16IsNan(src)) {
        return FP4_T_NAN;
    }

    uint32_t sRet = static_cast<uint32_t>((src >> bfloat16::BF16_SIGN_INDEX) & 0x1);
    uint32_t ef = static_cast<uint32_t>((src >> bfloat16::BF16_MAN_LEN) & 0xff); // get encoding value of E
    uint32_t mf = static_cast<uint32_t>(src & bfloat16::BF16_MAX_MAN);           // get encoding value of M

    uint16_t mRet = 0;
    uint16_t eRet = 0;
    uint16_t mLenDelta = bfloat16::BF16_MAN_LEN - FP4_E1M2_MAN_LEN; // bf16 man len - e1m2 man len
    bool needRound = false;

    int32_t maxExp = (1 << FP4_E1M2_EXP_LEN) - 1; // max E encoding value of e1m2 is 1
    int32_t expBias =
        (FP4_E1M2_EXP_LEN == 1) ? 1 : (1 << (FP4_E1M2_EXP_LEN - 1)) - 1; // Exponent Bias value of e2m1/e1m2 is 1

    int32_t eNorm = 0;
    if (ef == 0 && mf != 0) {
        eNorm = ef - FP32_EXP_BIAS + 1; // the exp bias of subnormal bf16 is 126
    } else {
        eNorm = ef - FP32_EXP_BIAS; // the exp bias of bf16 is 127
    }

    if ((eNorm > (maxExp - expBias)) || ((eNorm == (maxExp - expBias)) && ((mf >> mLenDelta) == FP4_E1M2_MAX_MAN))) {
        return ((bfloat16::Bf16ExtracSign(src) << FP4_SIGN_INDEX) | FP4_T_POS_MAX); // if overflow ,set to max normal
    } else if (eNorm <= -(expBias)) {                                               // subnormal
        eRet = 0;
        mf = (mf | bfloat16::BF16_MAN_HIDE_BIT);
        mLenDelta -= eNorm + expBias - 1;
        needRound = IsRoundOne(sRet, mf, mLenDelta); // determine if need to carry
        constexpr uint32_t B32_LEN = 32;
        if (mLenDelta <= 0 || mLenDelta >= B32_LEN) {   // 确保右操作数满足要求
            mRet = 0;
        } else {
            mRet = static_cast<uint16_t>(mf >> mLenDelta);
        }
        if (needRound) {
            mRet++;
        }
    } else { // normal
        eRet = static_cast<int16_t>(eNorm + expBias);
        needRound = IsRoundOne(sRet, mf, mLenDelta);
        mRet = static_cast<uint16_t>(mf >> mLenDelta);
        if (needRound) {
            mRet++;
        }
        if (((mRet & FP4_E1M2_MAN_HIDE_BIT) != 0) && (needRound)) {
            eRet++;
            mRet = 0;
        }
    }
    Fp4e1m2Normalize(eRet, mRet);
    return FP4E1M2_CONSTRUCTOR(sRet, eRet, mRet);
}
const uint16_t Fp4e1m2ToBf16[16] = {0x0, 0x3E80, 0x3F00, 0x3F40, 0x3F80, 0x3FA0, 0x3FC0, 0x3FE0,
                                    0x8000, 0xBE80, 0xBF00, 0xBF40, 0xBF80, 0xBFA0, 0xBFC0, 0xBFE0};

static uint16_t Fp4e1m2ToBfloat(const uint8_t fpVal)
{
    uint8_t fp4Val = fpVal & 0xf;
    uint16_t ret = Fp4e1m2ToBf16[fp4Val];
    return ret;
}

Fp4e1m2T::operator bfloat16::Bf16T() const
{
    bfloat16::Bf16T ret;
    ret.val = Fp4e1m2ToBfloat(val);
    return ret;
}
} // namespace float4_e1m2
