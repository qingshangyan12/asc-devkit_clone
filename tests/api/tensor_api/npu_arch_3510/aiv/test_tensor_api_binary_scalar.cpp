/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "tensor_api/tensor.h"

class Tensor_Api_Vector_BinaryScalar_3510 : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}

    void SetUp() override {}

    void TearDown() override {}
};

template<typename T, typename Func, typename S>
__aicore__ inline void TestTransformBinaryScalar(__gm__ T* z, __gm__ T* x, S value, __ubuf__ T zUB[2048], __ubuf__ T xUB[2048])
{
    constexpr uint32_t TILE_LENGTH = 2048;
    constexpr uint32_t BLK_NUM = 1;

    using namespace AscendC::Te;
    asc_init();

    constexpr uint8_t cacheMode = 0;
    constexpr uint32_t burstLength = TILE_LENGTH * sizeof(T);
    constexpr uint64_t srcStride = 0;
    constexpr uint32_t dstStride = 0;

    auto gmPtrX = MakeMemPtr<Location::GM>(x);
    auto gmPtrZ = MakeMemPtr<Location::GM>(z);

    auto xGm = MakeTensor(gmPtrX, MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));
    auto zGm = MakeTensor(gmPtrZ, MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));

    auto xLocal = MakeTensor(MakeMemPtr(xUB), MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));
    auto zLocal = MakeTensor(MakeMemPtr(zUB), MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));

    asc_copy_gm2ub_align(xLocal.Data().Get(), xGm.Data().Get(), BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);

    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

    Transform<Func>(zLocal, xLocal, value);

    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);

    asc_copy_ub2gm_align(zGm.Data().Get(), zLocal.Data().Get(), BLK_NUM, burstLength, cacheMode, srcStride, dstStride);
}

#define VECTOR_BINARY_SCALAR_3510(Function, DataType, ScalarType, ScalarValue) \
TEST_F(Tensor_Api_Vector_BinaryScalar_3510, Vector_##Function##_##DataType) \
{   \
    constexpr uint32_t TILE_LENGTH = 2048;  \
    \
    __gm__ DataType x[TILE_LENGTH] = {0};  \
    __gm__ DataType z[TILE_LENGTH] = {0};  \
    \
    __ubuf__ DataType xUB[TILE_LENGTH] = {0};  \
    __ubuf__ DataType zUB[TILE_LENGTH] = {0};  \
    \
    TestTransformBinaryScalar<DataType, AscendC::Te::Inst::Function>(z, x, ScalarType(ScalarValue), zUB, xUB);   \
    EXPECT_EQ(z[0], zUB[0]); \
}

VECTOR_BINARY_SCALAR_3510(AddScalar, uint8_t, uint8_t, 1)
VECTOR_BINARY_SCALAR_3510(AddScalar, int8_t, int8_t, 1)
VECTOR_BINARY_SCALAR_3510(AddScalar, uint16_t, uint16_t, 1)
VECTOR_BINARY_SCALAR_3510(AddScalar, int16_t, int16_t, 1)
VECTOR_BINARY_SCALAR_3510(AddScalar, half, half, 1)
VECTOR_BINARY_SCALAR_3510(AddScalar, int32_t, int32_t, 1)
VECTOR_BINARY_SCALAR_3510(AddScalar, uint32_t, uint32_t, 1)
VECTOR_BINARY_SCALAR_3510(AddScalar, bfloat16_t, bfloat16_t, 1)
VECTOR_BINARY_SCALAR_3510(AddScalar, float, float, 1)

VECTOR_BINARY_SCALAR_3510(MulScalar, uint16_t, uint16_t, 2)
VECTOR_BINARY_SCALAR_3510(MulScalar, int16_t, int16_t, 2)
VECTOR_BINARY_SCALAR_3510(MulScalar, int32_t, int32_t, 2)
VECTOR_BINARY_SCALAR_3510(MulScalar, uint32_t, uint32_t, 2)
VECTOR_BINARY_SCALAR_3510(MulScalar, half, half, 2)
VECTOR_BINARY_SCALAR_3510(MulScalar, float, float, 2)

VECTOR_BINARY_SCALAR_3510(MaxScalar, uint8_t, uint8_t, 5)
VECTOR_BINARY_SCALAR_3510(MaxScalar, int8_t, int8_t, 5)
VECTOR_BINARY_SCALAR_3510(MaxScalar, uint16_t, uint16_t, 5)
VECTOR_BINARY_SCALAR_3510(MaxScalar, int16_t, int16_t, 5)
VECTOR_BINARY_SCALAR_3510(MaxScalar, int32_t, int32_t, 5)
VECTOR_BINARY_SCALAR_3510(MaxScalar, uint32_t, uint32_t, 5)
VECTOR_BINARY_SCALAR_3510(MaxScalar, half, half, 5)
VECTOR_BINARY_SCALAR_3510(MaxScalar, float, float, 5)
VECTOR_BINARY_SCALAR_3510(MaxScalar, bfloat16_t, bfloat16_t, 5)

VECTOR_BINARY_SCALAR_3510(MinScalar, uint8_t, uint8_t, 5)
VECTOR_BINARY_SCALAR_3510(MinScalar, int8_t, int8_t, 5)
VECTOR_BINARY_SCALAR_3510(MinScalar, uint16_t, uint16_t, 5)
VECTOR_BINARY_SCALAR_3510(MinScalar, int16_t, int16_t, 5)
VECTOR_BINARY_SCALAR_3510(MinScalar, int32_t, int32_t, 5)
VECTOR_BINARY_SCALAR_3510(MinScalar, uint32_t, uint32_t, 5)
VECTOR_BINARY_SCALAR_3510(MinScalar, half, half, 5)
VECTOR_BINARY_SCALAR_3510(MinScalar, float, float, 5)

VECTOR_BINARY_SCALAR_3510(Axpy, half, half, 2)
VECTOR_BINARY_SCALAR_3510(Axpy, float, float, 2)

VECTOR_BINARY_SCALAR_3510(ShiftLeftScalar, int8_t, int16_t, 2)
VECTOR_BINARY_SCALAR_3510(ShiftLeftScalar, int16_t, int16_t, 2)
VECTOR_BINARY_SCALAR_3510(ShiftLeftScalar, int32_t, int16_t, 2)

VECTOR_BINARY_SCALAR_3510(ShiftRightScalar, int8_t, int16_t, 2)
VECTOR_BINARY_SCALAR_3510(ShiftRightScalar, int16_t, int16_t, 2)
VECTOR_BINARY_SCALAR_3510(ShiftRightScalar, int32_t, int16_t, 2)

template<typename DstDataType, typename SrcDataType, typename Func, typename S>
__aicore__ inline void TestTransformBinaryScalarMixed(__gm__ DstDataType* z, __gm__ SrcDataType* x, S value, __ubuf__ DstDataType zUB[2048], __ubuf__ SrcDataType xUB[2048])
{
    constexpr uint32_t TILE_LENGTH = 2048;
    constexpr uint32_t BLK_NUM = 1;

    using namespace AscendC::Te;
    asc_init();

    constexpr uint8_t cacheMode = 0;
    constexpr uint32_t burstLength = TILE_LENGTH * sizeof(SrcDataType);
    constexpr uint64_t srcStride = 0;
    constexpr uint32_t dstStride = 0;

    auto gmPtrX = MakeMemPtr<Location::GM>(x);
    auto gmPtrZ = MakeMemPtr<Location::GM>(z);

    auto xGm = MakeTensor(gmPtrX, MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));
    auto zGm = MakeTensor(gmPtrZ, MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));

    auto xLocal = MakeTensor(MakeMemPtr(xUB), MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));
    auto zLocal = MakeTensor(MakeMemPtr(zUB), MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));

    asc_copy_gm2ub_align(xLocal.Data().Get(), xGm.Data().Get(), BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);

    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

    Transform<Func>(zLocal, xLocal, value);

    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);

    asc_copy_ub2gm_align(zGm.Data().Get(), zLocal.Data().Get(), BLK_NUM, TILE_LENGTH * sizeof(DstDataType), cacheMode, srcStride, dstStride);
}

#define VECTOR_BINARY_SCALAR_MIXED_3510(Function, DstDataType, SrcDataType, ScalarType, ScalarValue) \
TEST_F(Tensor_Api_Vector_BinaryScalar_3510, Vector_##Function##_##DstDataType##_##SrcDataType) \
{   \
    constexpr uint32_t TILE_LENGTH = 2048;  \
    \
    __gm__ SrcDataType x[TILE_LENGTH] = {0};  \
    __gm__ DstDataType z[TILE_LENGTH] = {0};  \
    \
    __ubuf__ SrcDataType xUB[TILE_LENGTH] = {0};  \
    __ubuf__ DstDataType zUB[TILE_LENGTH] = {0};  \
    \
    TestTransformBinaryScalarMixed<DstDataType, SrcDataType, AscendC::Te::Inst::Function>(z, x, ScalarType(ScalarValue), zUB, xUB);   \
    EXPECT_EQ(z[0], zUB[0]); \
}

VECTOR_BINARY_SCALAR_MIXED_3510(MulsEven, half, float, float, 2)
VECTOR_BINARY_SCALAR_MIXED_3510(MulsOdd, half, float, float, 2)

template<typename SrcDataType, typename Func, typename S>
__aicore__ inline void TestTransformBinaryScalarCompare(__gm__ uint8_t* z, __gm__ SrcDataType* x, S value, __ubuf__ uint8_t zUB[2048], __ubuf__ SrcDataType xUB[2048])
{
    constexpr uint32_t TILE_LENGTH = 2048;
    constexpr uint32_t BLK_NUM = 1;

    using namespace AscendC::Te;
    asc_init();

    constexpr uint8_t cacheMode = 0;
    constexpr uint32_t burstLength = TILE_LENGTH * sizeof(SrcDataType);
    constexpr uint64_t srcStride = 0;
    constexpr uint32_t dstStride = 0;

    auto gmPtrX = MakeMemPtr<Location::GM>(x);
    auto gmPtrZ = MakeMemPtr<Location::GM>(z);

    auto xGm = MakeTensor(gmPtrX, MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));
    auto zGm = MakeTensor(gmPtrZ, MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));

    auto xLocal = MakeTensor(MakeMemPtr(xUB), MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));
    auto zLocal = MakeTensor(MakeMemPtr(zUB), MakeFrameLayout<NDLayoutPtn>(_1{}, AscendC::Std::Int<TILE_LENGTH>{}));

    asc_copy_gm2ub_align(xLocal.Data().Get(), xGm.Data().Get(), BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);

    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

    Transform<Func>(zLocal, xLocal, value);

    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);

    asc_copy_ub2gm_align(zGm.Data().Get(), zLocal.Data().Get(), BLK_NUM, TILE_LENGTH * sizeof(uint8_t), cacheMode, srcStride, dstStride);
}

#define VECTOR_BINARY_SCALAR_COMPARE_3510(Function, SrcDataType, ScalarType, ScalarValue) \
TEST_F(Tensor_Api_Vector_BinaryScalar_3510, Vector_##Function##_##SrcDataType) \
{   \
    constexpr uint32_t TILE_LENGTH = 2048;  \
    \
    __gm__ SrcDataType x[TILE_LENGTH] = {0};  \
    __gm__ uint8_t z[TILE_LENGTH] = {0};  \
    \
    __ubuf__ SrcDataType xUB[TILE_LENGTH] = {0};  \
    __ubuf__ uint8_t zUB[TILE_LENGTH] = {0};  \
    \
    TestTransformBinaryScalarCompare<SrcDataType, AscendC::Te::Inst::Function>(z, x, ScalarType(ScalarValue), zUB, xUB);   \
    EXPECT_EQ(z[0], zUB[0]); \
}

VECTOR_BINARY_SCALAR_COMPARE_3510(EqScalar, uint8_t, uint8_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(EqScalar, int8_t, int8_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(EqScalar, uint16_t, uint16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(EqScalar, int16_t, int16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(EqScalar, half, half, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(EqScalar, int32_t, int32_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(EqScalar, uint32_t, uint32_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(EqScalar, bfloat16_t, bfloat16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(EqScalar, float, float, 5)

VECTOR_BINARY_SCALAR_COMPARE_3510(GeScalar, uint8_t, uint8_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(GeScalar, int8_t, int8_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(GeScalar, uint16_t, uint16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(GeScalar, int16_t, int16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(GeScalar, half, half, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(GeScalar, int32_t, int32_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(GeScalar, uint32_t, uint32_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(GeScalar, bfloat16_t, bfloat16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(GeScalar, float, float, 5)

VECTOR_BINARY_SCALAR_COMPARE_3510(LeScalar, uint8_t, uint8_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LeScalar, int8_t, int8_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LeScalar, uint16_t, uint16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LeScalar, int16_t, int16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LeScalar, half, half, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LeScalar, int32_t, int32_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LeScalar, uint32_t, uint32_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LeScalar, bfloat16_t, bfloat16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LeScalar, float, float, 5)

VECTOR_BINARY_SCALAR_COMPARE_3510(LtScalar, uint8_t, uint8_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LtScalar, int8_t, int8_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LtScalar, uint16_t, uint16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LtScalar, int16_t, int16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LtScalar, half, half, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LtScalar, int32_t, int32_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LtScalar, uint32_t, uint32_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LtScalar, bfloat16_t, bfloat16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(LtScalar, float, float, 5)

VECTOR_BINARY_SCALAR_COMPARE_3510(NeScalar, uint8_t, uint8_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(NeScalar, int8_t, int8_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(NeScalar, uint16_t, uint16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(NeScalar, int16_t, int16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(NeScalar, half, half, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(NeScalar, int32_t, int32_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(NeScalar, uint32_t, uint32_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(NeScalar, bfloat16_t, bfloat16_t, 5)
VECTOR_BINARY_SCALAR_COMPARE_3510(NeScalar, float, float, 5)
