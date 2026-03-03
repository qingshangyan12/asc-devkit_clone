#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"
#include <mockcpp/mockcpp.hpp>

namespace {
    template<bool transpose>
    struct LoadDataTraitHolder {
        static constexpr AscendC::Te::LoadDataTrait trait = AscendC::Te::LoadDataTrait(transpose);
    };
}

class TEST_TENSOR_API_LOAD_DATA : public testing::Test {
protected:
    bool load_cbuf_to_ca_call_flag;
    bool is_mock_load_cbuf_to_ca;
    void SetUp() override {
        AscendC::SetGCoreType(1);
        load_cbuf_to_ca_call_flag = false;
        is_mock_load_cbuf_to_ca = true;
    }

    void TearDown() override {
        AscendC::SetGCoreType(0);
        load_cbuf_to_ca_call_flag = false;
        is_mock_load_cbuf_to_ca = false;
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
    LoadData<LoadDataTraitHolder<TRANSPOSE>::trait>(dstTensor, srcTensor);  \
    \
    EXPECT_FALSE(load_cbuf_to_##DST_TAG##_call_flag);  \
}

TEST_TENSOR_API_LOAD_DATA_IGNORE(float, 16, 16, Zz, Nz, L0A, L0A, ca, ca, false);
// TEST_TENSOR_API_LOAD_DATA_IGNORE(float, 17, 17, Zz, Nz, L0B, L0B, cb, cb, true);
// TEST_TENSOR_API_LOAD_DATA_IGNORE(float, 18, 18, Zz, Nz, L0A, L0B, ca, cb, false);

#define TEST_TENSOR_API_LOAD_DATA(TYPE, M, N, SRC_FORMAT, DST_FORMAT, SRC_POS, DST_POS, SRC_TAG, DST_TAG, TRANSPOSE, COORD_I, COORD_J) \
TEST_F(TEST_TENSOR_API_LOAD_DATA, TestLoadData_##TYPE##M##N##SRC_FORMAT##DST_FORMAT##SRC_POS##DST_POS##SRC_TAG##DST_TAG##TRANSPOSE##COORD_I##COORD_J) { \
    using namespace AscendC::Te; \
    __##DST_TAG##__ TYPE dst[M * N] = {0}; \
    auto dstIterator = Make##DST_POS##memPtr(dst); \
    auto dstMatrixLayout = Make##DST_FORMAT##Layout<TYPE>(M, N); \
    auto dstTensor = MakeTensor(dstIterator, dstMatrixLayout); \
 \
    __##SRC_TAG##__ TYPE src[M * N] = {0}; \
    auto srcIterator = Make##SRC_POS##memPtr(src); \
    auto srcMatrixLayout = Make##SRC_FORMAT##Layout<TYPE>(M, N); \
    auto srcTensor = MakeTensor(srcIterator, srcMatrixLayout); \
 \
    auto coord = MakeCoord(AscendC::Std::Int<COORD_I>{}, AscendC::Std::Int<COORD_J>{}); \
    LoadData<LoadDataTraitHolder<TRANSPOSE>::trait>(dstTensor, srcTensor, coord);  \
 \
    mockcpp::GlobalMockObject::verify(); \
}


// l1 -> l0A NZ2NZ非转置，覆盖所有TYPE，覆盖传入Coord
// TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(half, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(half, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(float, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(float, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, Nz, Nz, L1, L0A, cbuf, ca, false, 1, 1);
// // ZN2NZ转置，覆盖所有TYPE，覆盖传入Coord
// TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(half, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(half, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(float, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(float, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, Zn, Nz, L1, L0A, cbuf, ca, true, 1, 1);
// // l1 -> l0B ZN2ZN非转置，覆盖所有TYPE，覆盖传入Coord
// TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(half, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(half, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(float, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(float, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, Zn, Zn, L1, L0B, cbuf, cb, false, 1, 1);
// // l1 -> l0B NZ2ZN转置，覆盖所有TYPE，覆盖传入Coord
// TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(bfloat16_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(half, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(half, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(float, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(float, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int32_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint32_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int8_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint8_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(int16_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 1, 1);
// TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 0, 0);
// TEST_TENSOR_API_LOAD_DATA(uint16_t, 16, 16, Zn, Nz, L1, L0B, cbuf, cb, true, 1, 1);

