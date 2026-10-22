/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "basic_api/kernel_operator_vec_binary_intf.h"
#include "basic_api/kernel_operator_vec_unary_intf.h"

// Target code-generation and provenance checks: vector_pointer/run_target_headers.sh.
namespace {
using namespace AscendC;

#define CHECK_POINTER_COMBINATIONS(API, ...)    \
    API<T>(dst, src0, src1, __VA_ARGS__);       \
    API<T>(dstPtr, src0, src1, __VA_ARGS__);    \
    API<T>(dst, src0Ptr, src1, __VA_ARGS__);    \
    API<T>(dst, src0, src1Ptr, __VA_ARGS__);    \
    API<T>(dstPtr, src0Ptr, src1, __VA_ARGS__); \
    API<T>(dstPtr, src0, src1Ptr, __VA_ARGS__); \
    API<T>(dst, src0Ptr, src1Ptr, __VA_ARGS__); \
    API<T>(dstPtr, src0Ptr, src1Ptr, __VA_ARGS__)

#define DEFINE_POINTER_CHECK(API)                                                          \
    template <typename T>                                                                  \
    __aicore__ inline void Check##API(                                                     \
        const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1) \
    {                                                                                      \
        __ubuf__ T* dstPtr = (__ubuf__ T*)dst.GetPhyAddr();                                \
        __ubuf__ T* src0Ptr = (__ubuf__ T*)src0.GetPhyAddr();                              \
        __ubuf__ T* src1Ptr = (__ubuf__ T*)src1.GetPhyAddr();                              \
        uint64_t mask[2] = {0xFFFFFFFF, 0};                                                \
        BinaryRepeatParams params = {1, 1, 1, 8, 8, 8};                                    \
        CHECK_POINTER_COMBINATIONS(API, int32_t(32));                                      \
        CHECK_POINTER_COMBINATIONS(API, uint64_t(32), uint8_t(1), params);                 \
        CHECK_POINTER_COMBINATIONS(API, mask, uint8_t(1), params);                         \
        API(dstPtr + 1, src0Ptr + 1, src1Ptr + 1, int32_t(1));                             \
    }

DEFINE_POINTER_CHECK(Add)
DEFINE_POINTER_CHECK(Sub)
DEFINE_POINTER_CHECK(Mul)
DEFINE_POINTER_CHECK(Max)
DEFINE_POINTER_CHECK(Min)
DEFINE_POINTER_CHECK(And)
DEFINE_POINTER_CHECK(Or)
DEFINE_POINTER_CHECK(AddRelu)
DEFINE_POINTER_CHECK(SubRelu)
DEFINE_POINTER_CHECK(Div)
DEFINE_POINTER_CHECK(FusedMulAdd)
DEFINE_POINTER_CHECK(FusedMulAddRelu)
DEFINE_POINTER_CHECK(MulAddDst)
DEFINE_POINTER_CHECK(MulAddRelu)

#undef DEFINE_POINTER_CHECK
#undef CHECK_POINTER_COMBINATIONS
} // namespace

__aicore__ void CheckVectorPointerFloat(
    const AscendC::LocalTensor<float>& dst, const AscendC::LocalTensor<float>& src0,
    const AscendC::LocalTensor<float>& src1)
{
    CheckAdd(dst, src0, src1);
    CheckSub(dst, src0, src1);
    CheckMul(dst, src0, src1);
    CheckMax(dst, src0, src1);
    CheckMin(dst, src0, src1);
    CheckAddRelu(dst, src0, src1);
    CheckSubRelu(dst, src0, src1);
    CheckDiv(dst, src0, src1);
    CheckFusedMulAdd(dst, src0, src1);
    CheckFusedMulAddRelu(dst, src0, src1);
    CheckMulAddDst(dst, src0, src1);
    CheckMulAddRelu(dst, src0, src1);
}

__aicore__ void CheckVectorPointerInteger(
    const AscendC::LocalTensor<int32_t>& dst, const AscendC::LocalTensor<int32_t>& src0,
    const AscendC::LocalTensor<int32_t>& src1)
{
    CheckAnd(dst, src0, src1);
    CheckOr(dst, src0, src1);
}

#define CHECK_UNARY(API)                                    \
    AscendC::API(dst, src, int32_t(32));                    \
    AscendC::API(pd, src, int32_t(32));                     \
    AscendC::API(dst, ps, int32_t(32));                     \
    AscendC::API(pd, ps, int32_t(32));                      \
    AscendC::API(pd, ps, uint64_t(32), uint8_t(1), params); \
    AscendC::API(pd, src, bits, uint8_t(1), params);        \
    AscendC::API<float, false>(pd, ps, bits, uint8_t(1), params)

__aicore__ void CheckVectorPointerUnary(const AscendC::LocalTensor<float>& dst, const AscendC::LocalTensor<float>& src)
{
    __ubuf__ float* pd = (__ubuf__ float*)dst.GetPhyAddr();
    __ubuf__ float* ps = (__ubuf__ float*)src.GetPhyAddr();
    uint64_t bits[2] = {0xFFFFFFFF, 0};
    AscendC::UnaryRepeatParams params{1, 1, 8, 8};
    CHECK_UNARY(Abs);
    CHECK_UNARY(Relu);
    CHECK_UNARY(Exp);
    CHECK_UNARY(Ln);
    CHECK_UNARY(Sqrt);
    CHECK_UNARY(Rsqrt);
    CHECK_UNARY(Reciprocal);
    AscendC::Abs<float, float>(pd, ps, int32_t(32));
    AscendC::Neg(pd, ps, uint32_t(32));
    __ubuf__ float arrayDst[32];
    __ubuf__ float arraySrc[32];
    AscendC::Add(arrayDst, arraySrc, arraySrc, int32_t(32));
    AscendC::Abs(arrayDst, arraySrc, int32_t(32));
}
#undef CHECK_UNARY

__aicore__ void CheckVectorPointerNot(
    const AscendC::LocalTensor<uint16_t>& dst, const AscendC::LocalTensor<uint16_t>& src)
{
    __ubuf__ uint16_t* pd = (__ubuf__ uint16_t*)dst.GetPhyAddr();
    __ubuf__ uint16_t* ps = (__ubuf__ uint16_t*)src.GetPhyAddr();
    uint64_t bits[2] = {~uint64_t(0), 0};
    AscendC::UnaryRepeatParams params{1, 1, 8, 8};
    AscendC::Not(pd, src, int32_t(32));
    AscendC::Not(dst, ps, uint64_t(32), uint8_t(1), params);
    AscendC::Not(pd, ps, bits, uint8_t(1), params);
}
