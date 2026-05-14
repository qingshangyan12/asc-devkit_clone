/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_vec_binary_scalar_intf.h"


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Adds(const U& dst, const S& src0, const V& src1, uint64_t mask[],
//    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Adds_1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 1.5f;
    uint64_t mask[2] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint8_t repeatTime = 4;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Adds<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Adds(const U& dst, const S& src0, const V& src1,
//    uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Adds_2() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 2.0f;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 8;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Adds<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Adds(const U& dst, const S& src0, const V& src1, const int32_t& count);
void HostTest_Adds_3() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 3.5f;
    int32_t count = 256;
    AscendC::Adds<AscendC::BinaryDefaultType>(dst, src0, src1, count);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Muls(const U& dst, const S& src0, const V& src1, uint64_t mask[],
//    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Muls_1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 2.5f;
    uint64_t mask[2] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint8_t repeatTime = 4;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Muls<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Muls(const U& dst, const S& src0, const V& src1,
//    uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Muls_2() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 1.5f;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 8;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Muls<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Muls(const U& dst, const S& src0, const V& src1, const int32_t& count);
void HostTest_Muls_3() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 2.5f;
    int32_t count = 256;
    AscendC::Muls<AscendC::BinaryDefaultType>(dst, src0, src1, count);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Maxs(const U& dst, const S& src0, const V& src1, uint64_t mask[],
//    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Maxs_1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 5.0f;
    uint64_t mask[2] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint8_t repeatTime = 4;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Maxs<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Maxs(const U& dst, const S& src0, const V& src1,
//    uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Maxs_2() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 8.0f;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 8;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Maxs<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Maxs(const U& dst, const S& src0, const V& src1, const int32_t& count);
void HostTest_Maxs_3() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 10.0f;
    int32_t count = 256;
    AscendC::Maxs<AscendC::BinaryDefaultType>(dst, src0, src1, count);
}



//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Mins(const U& dst, const S& src0, const V& src1, uint64_t mask[],
//    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Mins_1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 3.0f;
    uint64_t mask[2] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint8_t repeatTime = 4;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Mins<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}



//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Mins(const U& dst, const S& src0, const V& src1,
//    uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Mins_2() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 6.0f;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 8;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Mins<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}



//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Mins(const U& dst, const S& src0, const V& src1, const int32_t& count);
void HostTest_Mins_3() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 15.0f;
    int32_t count = 512;
    AscendC::Mins<AscendC::BinaryDefaultType>(dst, src0, src1, count);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Subs(const U& dst, const S& src0, const V& src1, uint64_t mask[],
//    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Subs_1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 2.5f;
    uint64_t mask[2] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint8_t repeatTime = 4;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Subs<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Subs(const U& dst, const S& src0, const V& src1,
//    uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Subs_2() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 3.0f;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 8;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Subs<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Subs(const U& dst, const S& src0, const V& src1, const int32_t& count);
void HostTest_Subs_3() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 4.5f;
    int32_t count = 256;
    AscendC::Subs<AscendC::BinaryDefaultType>(dst, src0, src1, count);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Divs(const U& dst, const S& src0, const V& src1, uint64_t mask[],
//    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Divs_1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 3.0f;
    uint64_t mask[2] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint8_t repeatTime = 4;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Divs<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Divs(const U& dst, const S& src0, const V& src1,
//    uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Divs_2() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 4.0f;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 8;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Divs<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Divs(const U& dst, const S& src0, const V& src1, const int32_t& count);
void HostTest_Divs_3() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 5.0f;
    int32_t count = 256;
    AscendC::Divs<AscendC::BinaryDefaultType>(dst, src0, src1, count);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Ands(const U& dst, const S& src0, const V& src1, uint64_t mask[],
//    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Ands_1() {
    AscendC::LocalTensor<int32_t> dst;
    AscendC::LocalTensor<int32_t> src0;
    int32_t src1 = 0xFF;
    uint64_t mask[2] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint8_t repeatTime = 4;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Ands<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Ands(const U& dst, const S& src0, const V& src1,
//    uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Ands_2() {
    AscendC::LocalTensor<int32_t> dst;
    AscendC::LocalTensor<int32_t> src0;
    int32_t src1 = 0xFF;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 8;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Ands<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Ands(const U& dst, const S& src0, const V& src1, const int32_t& count);
void HostTest_Ands_3() {
    AscendC::LocalTensor<int32_t> dst;
    AscendC::LocalTensor<int32_t> src0;
    int32_t src1 = 0xFF;
    int32_t count = 256;
    AscendC::Ands<AscendC::BinaryDefaultType>(dst, src0, src1, count);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Ors(const U& dst, const S& src0, const V& src1, uint64_t mask[],
//    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Ors_1() {
    AscendC::LocalTensor<int32_t> dst;
    AscendC::LocalTensor<int32_t> src0;
    int32_t src1 = 0xFF;
    uint64_t mask[2] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint8_t repeatTime = 4;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Ors<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Ors(const U& dst, const S& src0, const V& src1,
//    uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);
void HostTest_Ors_2() {
    AscendC::LocalTensor<int32_t> dst;
    AscendC::LocalTensor<int32_t> src0;
    int32_t src1 = 0xFF;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint8_t repeatTime = 8;
    AscendC::UnaryRepeatParams repeatParams(1, 1, 8, 8);
    AscendC::Ors<AscendC::BinaryDefaultType>(dst, src0, src1, mask, repeatTime, repeatParams);
}


//template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG,
//          typename U, typename S, typename V>
//__aicore__ inline void Ors(const U& dst, const S& src0, const V& src1, const int32_t& count);
void HostTest_Ors_3() {
    AscendC::LocalTensor<int32_t> dst;
    AscendC::LocalTensor<int32_t> src0;
    int32_t src1 = 0xFF;
    int32_t count = 256;
    AscendC::Ors<AscendC::BinaryDefaultType>(dst, src0, src1, count);
}


//template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, 
//    const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
//__aicore__ inline void MulsCast(const T2 &dst, const T3 &src0, const T4 &src1, const uint32_t count);
void HostTest_MulsCast() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 2.0f;
    uint32_t count = 256;
    AscendC::MulsCast<AscendC::BinaryDefaultType>(dst, src0, src1, count);
}


//template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, 
//    const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
//__aicore__ inline void FusedMulsCast(const T2 &dst, const T3 &src0, const T4 &src1, const uint32_t count);
void HostTest_FusedMulsCast() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    float src1 = 3.0f;
    uint32_t count = 256;
    AscendC::FusedMulsCast<AscendC::BinaryDefaultType>(dst, src0, src1, count);
}
















