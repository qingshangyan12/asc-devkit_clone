/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_vec_cmpsel_intf.h"

//template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, bool isSetMask = true,
//          const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
//__aicore__ inline void Compares(const T2& dst, const T3& src0, const T4& src1, CMPMODE cmpMode,
//    const uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTestCompares1() {
    AscendC::LocalTensor<uint8_t> dst;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint64_t mask[2] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint8_t repeatTime = 1;
    AscendC::UnaryRepeatParams repeatParams;
    AscendC::Compares<float, float>(dst, src0, src1, AscendC::CMPMODE::EQ, mask, repeatTime, repeatParams);
}

//template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, bool isSetMask = true,
//          const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
//__aicore__ inline void Compares(const T2& dst, const T3& src0, const T4& src1, CMPMODE cmpMode,
//    const uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTestCompares2() {
    AscendC::LocalTensor<uint8_t> dst;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 1;
    AscendC::UnaryRepeatParams repeatParams;
    AscendC::Compares<float, float>(dst, src0, src1, AscendC::CMPMODE::EQ, mask, repeatTime, repeatParams);
}

//template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, bool isSetMask = true,
//          const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
//__aicore__ inline void Compares(const T2& dst, const T3& src0, const T4& src1, CMPMODE cmpMode,
//    uint32_t count);
void HostTestCompares3() {
    AscendC::LocalTensor<uint8_t> dst;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint32_t count = 128;
    AscendC::Compares<float, float>(dst, src0, src1, AscendC::CMPMODE::EQ, count);
}

//template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, bool isSetMask = true,
//          const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
//__aicore__ inline void CompareScalar(const T2& dst, const T3& src0, const T4& src1, CMPMODE cmpMode,
//    const uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTestCompareScalar1() {
    AscendC::LocalTensor<uint8_t> dst;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint64_t mask[2] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint8_t repeatTime = 1;
    AscendC::UnaryRepeatParams repeatParams;
    AscendC::CompareScalar<float, float>(dst, src0, src1, AscendC::CMPMODE::EQ, mask, repeatTime, repeatParams);
}

//template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, bool isSetMask = true,
//          const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
//__aicore__ inline void CompareScalar(const T2& dst, const T3& src0, const T4& src1, CMPMODE cmpMode,
//    const uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTestCompareScalar2() {
    AscendC::LocalTensor<uint8_t> dst;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 1;
    AscendC::UnaryRepeatParams repeatParams;
    AscendC::CompareScalar<float, float>(dst, src0, src1, AscendC::CMPMODE::EQ, mask, repeatTime, repeatParams);
}

//template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, bool isSetMask = true,
//          const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
//__aicore__ inline void CompareScalar(const T2& dst, const T3& src0, const T4& src1, CMPMODE cmpMode,
//    uint32_t count);
void HostTestCompareScalar3() {
    AscendC::LocalTensor<uint8_t> dst;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint32_t count = 128;
    AscendC::CompareScalar<float, float>(dst, src0, src1, AscendC::CMPMODE::EQ, count);
}

//template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, bool isSetMask = true,
//          const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
//__aicore__ inline void Select(const T2& dst, const LocalTensor<T1>& selMask, const T3& src0, const T4& src1,
//    SELMODE selMode, uint64_t mask[], uint8_t repeatTime, const BinaryRepeatParams& repeatParams);
void HostTestSelect1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<uint8_t> selMask;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint64_t mask[2] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint8_t repeatTime = 1;
    AscendC::BinaryRepeatParams repeatParams;
    AscendC::Select<float, uint8_t>(dst, selMask, src0, src1, AscendC::SELMODE::SEL_MODE_1, mask, repeatTime, repeatParams);
}

//template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, bool isSetMask = true,
//          const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
//__aicore__ inline void Select(const T2& dst, const LocalTensor<T1>& selMask, const T3& src0, const T4& src1,
//    SELMODE selMode, uint64_t mask, uint8_t repeatTime, const BinaryRepeatParams& repeatParams);
void HostTestSelect2() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<uint8_t> selMask;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 1;
    AscendC::BinaryRepeatParams repeatParams;
    AscendC::Select<float, uint8_t>(dst, selMask, src0, src1, AscendC::SELMODE::SEL_MODE_1, mask, repeatTime, repeatParams);
}

//template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, const BinaryConfig &config = DEFAULT_BINARY_CONFIG,
//          typename T2, typename T3, typename T4>
//__aicore__ inline void Select(const T2& dst, const LocalTensor<T1>& selMask, const T3& src0, const T4& src1,
//    SELMODE selMode, uint32_t count);
void HostTestSelect3() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<uint8_t> selMask;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint32_t count = 128;
    AscendC::Select<float, uint8_t>(dst, selMask, src0, src1, AscendC::SELMODE::SEL_MODE_1, count);
}