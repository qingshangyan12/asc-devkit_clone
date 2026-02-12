#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"
#include <mockcpp/mockcpp.hpp>

class TEST_LOAD_DATA : public testing::Test {
protected:
    void SetUp() override {
        AscendC::SetGCoreType(1);
    }

    void TearDown() override {
        AscendC::SetGCoreType(0);
    }
};

// L1 -> L0A
using namespace AscendC::Te;

// LoadDataIgnore
TEST_F(TEST_LOAD_DATA, TestLoadDataRouting_LoadDataIgnoreDoesNothing) {

    auto destAddr = reinterpret_cast<__ca__ float*>(0);
    auto l0aIterator = MakeL0AmemPtr(destAddr);
    auto l0aMatrixLayout = MakeNZLayout<float>(16, 16);
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);

    auto srcAddr = reinterpret_cast<__cbuf__ float*>(0);
    auto l1Iterator = MakeL1memPtr(srcAddr);
    auto l1MatrixLayout = MakeNZLayout<float>(16, 16);
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout);

    // L0A -> L1  Ignore
    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l1Tensor, l0aTensor);
}

#define LOAD_DATA_TEST_NZ2ZZ_NoCoord(TYPE, L0_SRC) \
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_NZ2ZZ_NoCoord_##TYPE) { \
 \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    originDstAddr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    originSrcAddr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeNZLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
    is_mock_img2colv2_cbuf_to_ca = true; \
    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0Tensor, l1Tensor); \
    is_mock_img2colv2_cbuf_to_ca = false; \
 \
    mockcpp::GlobalMockObject::reset(); \
}
#define LOAD_DATA_TEST_NZ2ZZ_Coord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_NZ2ZZ_Coord_##TYPE) { \
 \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    originDstAddr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    originSrcAddr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeNZLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
    auto coordZero = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{}); \
    is_mock_img2colv2_cbuf_to_ca = true; \
    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0Tensor, l1Tensor, coordZero); \
    is_mock_img2colv2_cbuf_to_ca = false; \
    mockcpp::GlobalMockObject::reset(); \
}
#define LOAD_DATA_TEST_ZN2ZZ_NoCoord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_ZN2ZZ_NoCoord_##TYPE) { \
 \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    originDstAddr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    originSrcAddr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
     constexpr static LoadDataTrait trait(true);\
    is_mock_img2colv2_cbuf_to_ca = true; \
    LoadData<trait>(l0Tensor, l1Tensor); \
    is_mock_img2colv2_cbuf_to_ca = false; \
 \
    mockcpp::GlobalMockObject::reset(); \
}
#define LOAD_DATA_TEST_ZN2ZZ_Coord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_ZN2ZZ_Coord_##TYPE) { \
 \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    originDstAddr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    originSrcAddr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
    constexpr static LoadDataTrait trait(true);\
    auto coordZero = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{}); \
    is_mock_img2colv2_cbuf_to_ca = true; \
    LoadData<trait>(l0Tensor, l1Tensor, coordZero); \
    is_mock_img2colv2_cbuf_to_ca = false; \
 \
    mockcpp::GlobalMockObject::reset(); \
}

LOAD_DATA_TEST_NZ2ZZ_NoCoord(bfloat16_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(bfloat16_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(bfloat16_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(bfloat16_t, A);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(bfloat16_t, B);
LOAD_DATA_TEST_NZ2ZZ_Coord(bfloat16_t, B);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(bfloat16_t, B);
LOAD_DATA_TEST_ZN2ZZ_Coord(bfloat16_t, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(half, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(half, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(half, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(half, A);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(half, B);
LOAD_DATA_TEST_NZ2ZZ_Coord(half, B);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(half, B);
LOAD_DATA_TEST_ZN2ZZ_Coord(half, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(float, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(float, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(float, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(float, A);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(float, B);
LOAD_DATA_TEST_NZ2ZZ_Coord(float, B);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(float, B);
LOAD_DATA_TEST_ZN2ZZ_Coord(float, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(int32_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(int32_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(int32_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(int32_t, A);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(int32_t, B);
LOAD_DATA_TEST_NZ2ZZ_Coord(int32_t, B);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(int32_t, B);
LOAD_DATA_TEST_ZN2ZZ_Coord(int32_t, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(uint32_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(uint32_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(uint32_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(uint32_t, A);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(uint32_t, B);
LOAD_DATA_TEST_NZ2ZZ_Coord(uint32_t, B);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(uint32_t, B);
LOAD_DATA_TEST_ZN2ZZ_Coord(uint32_t, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(int8_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(int8_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(int8_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(int8_t, A);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(int8_t, B);
LOAD_DATA_TEST_NZ2ZZ_Coord(int8_t, B);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(int8_t, B);
LOAD_DATA_TEST_ZN2ZZ_Coord(int8_t, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(uint8_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(uint8_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(uint8_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(uint8_t, A);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(uint8_t, B);
LOAD_DATA_TEST_NZ2ZZ_Coord(uint8_t, B);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(uint8_t, B);
LOAD_DATA_TEST_ZN2ZZ_Coord(uint8_t, B);
/* TODO cann包stub缺少类型支持 */ 
LOAD_DATA_TEST_NZ2ZZ_NoCoord(int16_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(int16_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(int16_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(int16_t, A);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(int16_t, B);
LOAD_DATA_TEST_NZ2ZZ_Coord(int16_t, B);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(int16_t, B);
LOAD_DATA_TEST_ZN2ZZ_Coord(int16_t, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(uint16_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(uint16_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(uint16_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(uint16_t, A);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(uint16_t, B);
LOAD_DATA_TEST_NZ2ZZ_Coord(uint16_t, B);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(uint16_t, B);
LOAD_DATA_TEST_ZN2ZZ_Coord(uint16_t, B);

