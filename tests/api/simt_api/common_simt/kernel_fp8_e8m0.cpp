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
 * \file kernel_fp8_e8m0.cpp
 * \brief
 */
#include "kernel_fp8_e8m0.h"
#include "kernel_fp32.h"
#include "kernel_utils.h"

namespace float8_e8m0 {
namespace {
// FP8 (E8M0)
constexpr uint8_t FP8E8M0_EXP_MASK = 0xFF;

bool Fp8e8m0IsNan(const uint8_t& x)
{
    return (x == FP8E8M0_EXP_MASK);
}

uint8_t Fp8e8m0ExtracExp(uint8_t x)
{
    return x;
}

void ExtractFp8e8m0(const uint8_t val, uint8_t& s, uint8_t& e, uint8_t& m)
{
    // 1.Extract
    s = 0;
    e = static_cast<int16_t>(Fp8e8m0ExtracExp(val));
    m = 0;
}

uint32_t Fp8e8m0ToFp32(const uint8_t fpVal)
{
    uint32_t ret = 0;

    if (fpVal == 0x0) {
        return 0x400000;
    }

    if (Fp8e8m0IsNan(fpVal)) {
        return FP32_NAN;
    }

    uint8_t fp8Sign = 0;
    uint8_t fp8Man = 0;
    uint8_t fp8Exp = 0;
    ExtractFp8e8m0(fpVal, fp8Sign, fp8Exp, fp8Man);

    uint32_t fVal = Fp32Constructor(fp8Sign, fp8Exp, fp8Man);
    ret = fVal;
    return ret;
}
}  // namespace

uint8_t Fp8e8m0T::FloatToFp8e8m0(const float src) const
{
    uint32_t srcVal = AscendC::GetScalarBitcodeValue<float, uint32_t>(src);
    if (Fp32IsNan(srcVal)) {
        return 0xFF;
    }
    uint32_t ef = Fp32ExtracExp(srcVal);

    return ef;
}

Fp8e8m0T::operator float() const
{
    return AscendC::GetScalarBitcodeValue<uint32_t, float>(Fp8e8m0ToFp32(val));
}

float Fp8e8m0T::ToFloat() const
{
    return AscendC::GetScalarBitcodeValue<uint32_t, float>(Fp8e8m0ToFp32(val));
}
} // namespace float8_e8m0