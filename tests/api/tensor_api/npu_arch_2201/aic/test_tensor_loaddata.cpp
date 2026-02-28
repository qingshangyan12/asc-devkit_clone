#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"
#include <mockcpp/mockcpp.hpp>

class TEST_TENSOR_API_LOAD_DATA : public testing::Test {
protected:
    void SetUp() override {
        AscendC::SetGCoreType(1);
        img2colv2_cbuf_to_ca_call_flag = false;
        is_mock_img2colv2_cbuf_to_ca = true;
        img2colv2_cbuf_to_cb_call_flag = false;
        is_mock_img2colv2_cbuf_to_cb = true;
    }

    void TearDown() override {
        AscendC::SetGCoreType(0);
        img2colv2_cbuf_to_ca_call_flag = false;
        is_mock_img2colv2_cbuf_to_ca = false;
        img2colv2_cbuf_to_cb_call_flag = false;
        is_mock_img2colv2_cbuf_to_cb = false;
    }
};

// l1 -> l0
// LoadDataIgnore
#define TEST_TENSOR_API_LOAD_DATA_IGNORE(TYPE, M, N, SRC_FORMAT, DST_FORMAT, SRC_POS, DST_POS, SRC_TAG, DST_TAG, TRANSPOSE)  \
TEST_F(TEST_TENSOR_API_LOAD_DATA, TestLoadDataRouting_##TYPE##M##N##SRC_FORMAT##DST_FORMAT##SRC_POS##DST_POS##SRC_TAG##DST_TAG##TRANSPOSE##LoadDataIgnoreDoesNothing) {  \
    using namespace AscendC::Te;  \
    __##DST_TAG##__ TYPE dst[M * N] = {0};  \
    auto dstIterator = Make##SRC_POS##memPtr(dst);  \
    auto dstMatrixLayout = Make##DST_FORMAT##Layout<TYPE>(M, N);  \
    auto dstTensor = MakeTensor(dstIterator, dstMatrixLayout);  \
    \
    __##SRC_TAG##__ TYPE src[M * N] = {0};  \
    auto srcIterator = Make##SRC_POS##memPtr(src);  \
    auto srcMatrixLayout = Make##SRC_FORMAT##Layout<TYPE>(M, N);  \
    auto srcTensor = MakeTensor(srcIterator, srcMatrixLayout);  \
    \
    constexpr static LoadDataTrait trait(TRANSPOSE); \
    LoadData<trait>(dstTensor, srcTensor);  \
    \
    EXPECT_FALSE(img2colv2_cbuf_to_##DST_TAG##_call_flag);  \
}

TEST_TENSOR_API_LOAD_DATA_IGNORE(float, 16, 16, ZZ, NZ, L0A, L0A, ca, ca, false);
TEST_TENSOR_API_LOAD_DATA_IGNORE(float, 17, 17, ZZ, NZ, L0B, L0B, cb, cb, true);
TEST_TENSOR_API_LOAD_DATA_IGNORE(float, 18, 18, ZZ, NZ, L0A, L0B, ca, cb, false);

#define TEST_TENSOR_API_LOAD_DATA(TYPE, M, N, SRC_FORMAT, DST_FORMAT, SRC_POS, DST_POS, SRC_TAG, DST_TAG, TRANSPOSE, COORD_I, COORD_J) \
TEST_F(TEST_TENSOR_API_LOAD_DATA, TestLoadData_##TYPE##M##N##SRC_FORMAT##DST_FORMAT##SRC_POS##DST_POS##SRC_TAG##DST_TAG##TRANSPOSE##COORD_I##COORD_J) { \
    using namespace AscendC::Te; \
    __##DST_TAG##__ TYPE dst[M * N] = {0}; \
    img2colv2_##SRC_TAG##_to_##DST_TAG##_origin_dst_addr = dst; \
    auto dstIterator = Make##DST_POS##memPtr(dst); \
    auto dstMatrixLayout = Make##DST_FORMAT##Layout<TYPE>(M, N); \
    auto dstTensor = MakeTensor(dstIterator, dstMatrixLayout); \
 \
    __##SRC_TAG##__ TYPE src[M * N] = {0}; \
    img2colv2_##SRC_TAG##_to_##DST_TAG##_origin_src_addr = src; \
    auto srcIterator = Make##SRC_POS##memPtr(src); \
    auto srcMatrixLayout = Make##SRC_FORMAT##Layout<TYPE>(M, N); \
    auto srcTensor = MakeTensor(srcIterator, srcMatrixLayout); \
 \
    auto coord = MakeCoord(AscendC::Std::Int<COORD_I>{}, AscendC::Std::Int<COORD_J>{}); \
    constexpr static LoadDataTrait trait(TRANSPOSE); \
    LoadData<trait>(dstTensor, srcTensor, coord);  \
 \
    mockcpp::GlobalMockObject::verify(); \
}


// l1 -> l0A NZ2ZZ非转置，覆盖所有TYPE，覆盖传入Coord
TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(half, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(half, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(float, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(float, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, NZ, ZZ, L1, L0A, cbuf, ca, false, 1, 1);
// NZ2ZN转置，覆盖所有TYPE，覆盖传入Coord
TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(half, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(half, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(float, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(float, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, NZ, ZN, L1, L0A, cbuf, ca, true, 1, 1);
// l1 -> l0B ZN2ZN非转置，覆盖所有TYPE，覆盖传入Coord
TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(half, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(half, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(float, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(float, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, ZN, ZN, L1, L0B, cbuf, cb, false, 1, 1);
// l1 -> l0B NZ2ZN转置，覆盖所有TYPE，覆盖传入Coord
TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(half, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(half, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(float, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(float, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 1, 1);
TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 0, 0);
TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, NZ, ZN, L1, L0B, cbuf, cb, true, 1, 1);

