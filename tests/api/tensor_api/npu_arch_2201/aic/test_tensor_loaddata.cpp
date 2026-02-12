#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"
#include <mockcpp/mockcpp.hpp>

class TEST_LOAD_DATA : public testing::Test {
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
    img2colv2_cbuf_to_ca_origin_dst_addr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    img2colv2_cbuf_to_ca_origin_src_addr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeNZLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0Tensor, l1Tensor); \
 \
    mockcpp::GlobalMockObject::reset(); \
}
#define LOAD_DATA_TEST_NZ2ZZ_Coord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_NZ2ZZ_Coord_##TYPE) { \
 \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    img2colv2_cbuf_to_ca_origin_dst_addr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    img2colv2_cbuf_to_ca_origin_src_addr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeNZLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
    auto coordZero = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{}); \
    is_mock_img2colv2_cbuf_to_ca = true; \
    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0Tensor, l1Tensor, coordZero); \
    mockcpp::GlobalMockObject::reset(); \
}
#define LOAD_DATA_TEST_ZN2ZZ_NoCoord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_ZN2ZZ_NoCoord_##TYPE) { \
 \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    img2colv2_cbuf_to_ca_origin_dst_addr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    img2colv2_cbuf_to_ca_origin_src_addr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
     constexpr static LoadDataTrait trait(true);\
    is_mock_img2colv2_cbuf_to_ca = true; \
    LoadData<trait>(l0Tensor, l1Tensor); \
 \
    mockcpp::GlobalMockObject::reset(); \
}
#define LOAD_DATA_TEST_ZN2ZZ_Coord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_ZN2ZZ_Coord_##TYPE) { \
 \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    img2colv2_cbuf_to_ca_origin_dst_addr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    img2colv2_cbuf_to_ca_origin_src_addr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
    constexpr static LoadDataTrait trait(true);\
    auto coordZero = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{}); \
    is_mock_img2colv2_cbuf_to_ca = true; \
    LoadData<trait>(l0Tensor, l1Tensor, coordZero); \
 \
    mockcpp::GlobalMockObject::reset(); \
}

#define LOAD_DATA_TEST_ZN2ZN_NoCoord(TYPE, L0_SRC) \
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_ZN2ZN_NoCoord_##TYPE) { \
    MOCKER_CPP(load_cbuf_to_cb, void(__ca__ bfloat16_t*, __cbuf__ bfloat16_t*, \
        uint16_t, uint8_t, uint16_t, uint16_t, uint8_t, bool, addr_cal_mode_t)) \
        .times(1) \
        .will(invoke(static_cast<void(*)(__ca__ bfloat16_t*, __cbuf__ bfloat16_t*, \
                        uint16_t, uint8_t, uint16_t, uint16_t, uint8_t, bool, addr_cal_mode_t)>( \
                [](__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t baseIdx, \
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, uint8_t sid, \
                bool transpose, addr_cal_mode_t addr_cal_mode) { \
                    std::cout << "hello" << std::endl; \
                } \
    ))); \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    img2colv2_cbuf_to_cb_origin_dst_addr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    img2colv2_cbuf_to_cb_origin_src_addr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0Tensor, l1Tensor); \
 \
    mockcpp::GlobalMockObject::reset(); \
}
#define LOAD_DATA_TEST_ZN2ZN_Coord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_ZN2ZN_Coord_##TYPE) { \
 \
    MOCKER_CPP(load_cbuf_to_cb, void(__ca__ bfloat16_t*, __cbuf__ bfloat16_t*, \
        uint16_t, uint8_t, uint16_t, uint16_t, uint8_t, bool, addr_cal_mode_t)) \
        .times(1) \
        .will(invoke(static_cast<void(*)(__ca__ bfloat16_t*, __cbuf__ bfloat16_t*, \
            uint16_t, uint8_t, uint16_t, uint16_t, uint8_t, bool, addr_cal_mode_t)>( \
            [](__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t baseIdx, \
            uint8_t repeat, uint16_t srcStride, uint16_t dstStride, uint8_t sid, \
            bool transpose, addr_cal_mode_t addr_cal_mode) { \
             std::cout << "hello" << std::endl; \
             } \
    ))); \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    img2colv2_cbuf_to_cb_origin_dst_addr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    img2colv2_cbuf_to_cb_origin_src_addr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
    auto coordZero = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{}); \
    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0Tensor, l1Tensor, coordZero); \
    mockcpp::GlobalMockObject::reset(); \
}
#define LOAD_DATA_TEST_NZ2ZN_NoCoord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_NZ2ZN_NoCoord_##TYPE) { \
 \
    MOCKER_CPP(load_cbuf_to_cb, void(__ca__ bfloat16_t*, __cbuf__ bfloat16_t*, \
        uint16_t, uint8_t, uint16_t, uint16_t, uint8_t, bool, addr_cal_mode_t)) \
        .times(1) \
        .will(invoke(static_cast<void(*)(__ca__ bfloat16_t*, __cbuf__ bfloat16_t*, \
            uint16_t, uint8_t, uint16_t, uint16_t, uint8_t, bool, addr_cal_mode_t)>( \
            [](__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t baseIdx, \
            uint8_t repeat, uint16_t srcStride, uint16_t dstStride, uint8_t sid, \
            bool transpose, addr_cal_mode_t addr_cal_mode) { \
                std::cout << "hello" << std::endl; \
            } \
    ))); \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    img2colv2_cbuf_to_cb_origin_dst_addr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    img2colv2_cbuf_to_cb_origin_src_addr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeNZLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
     constexpr static LoadDataTrait trait(true);\
    LoadData<trait>(l0Tensor, l1Tensor); \
 \
    mockcpp::GlobalMockObject::reset(); \
}
#define LOAD_DATA_TEST_NZ2ZN_Coord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_NZ2ZN_Coord_##TYPE) { \
 \
    MOCKER_CPP(load_cbuf_to_cb, void(__ca__ bfloat16_t*, __cbuf__ bfloat16_t*, \
        uint16_t, uint8_t, uint16_t, uint16_t, uint8_t, bool, addr_cal_mode_t)) \
        .times(1) \
        .will(invoke(static_cast<void(*)(__ca__ bfloat16_t*, __cbuf__ bfloat16_t*, \
        uint16_t, uint8_t, uint16_t, uint16_t, uint8_t, bool, addr_cal_mode_t)>( \
        [](__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t baseIdx, \
        uint8_t repeat, uint16_t srcStride, uint16_t dstStride, uint8_t sid, \
        bool transpose, addr_cal_mode_t addr_cal_mode) { \
            std::cout << "hello" << std::endl; \
        } \
    ))); \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    img2colv2_cbuf_to_cb_origin_dst_addr = destAddr; \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    img2colv2_cbuf_to_cb_origin_src_addr = srcAddr; \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeNZLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
    constexpr static LoadDataTrait trait(true);\
    auto coordZero = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{}); \
    LoadData<trait>(l0Tensor, l1Tensor, coordZero); \
 \
    mockcpp::GlobalMockObject::reset(); \
}

LOAD_DATA_TEST_NZ2ZZ_NoCoord(bfloat16_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(bfloat16_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(bfloat16_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(bfloat16_t, A);
LOAD_DATA_TEST_ZN2ZN_NoCoord(bfloat16_t, B);
LOAD_DATA_TEST_ZN2ZN_Coord(bfloat16_t, B);
LOAD_DATA_TEST_NZ2ZN_NoCoord(bfloat16_t, B);
LOAD_DATA_TEST_NZ2ZN_Coord(bfloat16_t, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(half, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(half, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(half, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(half, A);
LOAD_DATA_TEST_ZN2ZN_NoCoord(half, B);
LOAD_DATA_TEST_ZN2ZN_Coord(half, B);
LOAD_DATA_TEST_NZ2ZN_NoCoord(half, B);
LOAD_DATA_TEST_NZ2ZN_Coord(half, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(float, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(float, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(float, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(float, A);
LOAD_DATA_TEST_ZN2ZN_NoCoord(float, B);
LOAD_DATA_TEST_ZN2ZN_Coord(float, B);
LOAD_DATA_TEST_NZ2ZN_NoCoord(float, B);
LOAD_DATA_TEST_NZ2ZN_Coord(float, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(int32_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(int32_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(int32_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(int32_t, A);
LOAD_DATA_TEST_ZN2ZN_NoCoord(int32_t, B);
LOAD_DATA_TEST_ZN2ZN_Coord(int32_t, B);
LOAD_DATA_TEST_NZ2ZN_NoCoord(int32_t, B);
LOAD_DATA_TEST_NZ2ZN_Coord(int32_t, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(uint32_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(uint32_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(uint32_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(uint32_t, A);
LOAD_DATA_TEST_ZN2ZN_NoCoord(uint32_t, B);
LOAD_DATA_TEST_ZN2ZN_Coord(uint32_t, B);
LOAD_DATA_TEST_NZ2ZN_NoCoord(uint32_t, B);
LOAD_DATA_TEST_NZ2ZN_Coord(uint32_t, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(int8_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(int8_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(int8_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(int8_t, A);
LOAD_DATA_TEST_ZN2ZN_NoCoord(int8_t, B);
LOAD_DATA_TEST_ZN2ZN_Coord(int8_t, B);
LOAD_DATA_TEST_NZ2ZN_NoCoord(int8_t, B);
LOAD_DATA_TEST_NZ2ZN_Coord(int8_t, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(uint8_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(uint8_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(uint8_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(uint8_t, A);
LOAD_DATA_TEST_ZN2ZN_NoCoord(uint8_t, B);
LOAD_DATA_TEST_ZN2ZN_Coord(uint8_t, B);
LOAD_DATA_TEST_NZ2ZN_NoCoord(uint8_t, B);
LOAD_DATA_TEST_NZ2ZN_Coord(uint8_t, B);
/* TODO cann包stub缺少img2colv2_cbuf_to_ca类型支持 */
LOAD_DATA_TEST_NZ2ZZ_NoCoord(int16_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(int16_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(int16_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(int16_t, A);
LOAD_DATA_TEST_ZN2ZN_NoCoord(int16_t, B);
LOAD_DATA_TEST_ZN2ZN_Coord(int16_t, B);
LOAD_DATA_TEST_NZ2ZN_NoCoord(int16_t, B);
LOAD_DATA_TEST_NZ2ZN_Coord(int16_t, B);
LOAD_DATA_TEST_NZ2ZZ_NoCoord(uint16_t, A);
LOAD_DATA_TEST_NZ2ZZ_Coord(uint16_t, A);
LOAD_DATA_TEST_ZN2ZZ_NoCoord(uint16_t, A);
LOAD_DATA_TEST_ZN2ZZ_Coord(uint16_t, A);
LOAD_DATA_TEST_ZN2ZN_NoCoord(uint16_t, B);
LOAD_DATA_TEST_ZN2ZN_Coord(uint16_t, B);
LOAD_DATA_TEST_NZ2ZN_NoCoord(uint16_t, B);
LOAD_DATA_TEST_NZ2ZN_Coord(uint16_t, B);

