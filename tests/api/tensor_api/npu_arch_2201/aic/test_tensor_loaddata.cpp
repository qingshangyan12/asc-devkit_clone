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


// void mock_img2colv2_cbuf_to_ca(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src,
//      uint16_t stepK, uint16_t stepM, uint16_t posK, uint16_t posM,
//       uint8_t strideW, uint8_t strideH, uint8_t Wk, uint8_t Hk,
//        uint8_t dilationW, uint8_t dilationH, bool filterW,
//         bool filterH, bool transpose, bool fmatrixCtrl, uint16_t sizeChannel) {
//     std::cout << "mock img2colv2_cbuf_to_ca" << std::endl;
// }

// TEST_F(TEST_LOAD_DATA, TestLoadData_NZ2ZZ_bfloat16_t) {
    

//     MOCKER_CPP(set_fmatrix, void(uint64_t))
//         .times(1)
//         .will(invoke(
//              (void(*)(uint64_t))[](uint64_t config) {
//                 std::cout << config << std::endl;
//                 EXPECT_EQ(config, 65552);
//             }
//         )
//     );

//     /** 这个打不上，报错 */
//   MOCKER_CPP(img2colv2_cbuf_to_ca, void(__ca__ bfloat16_t*,__cbuf__ bfloat16_t* ,
//               uint16_t , uint16_t , uint16_t , uint16_t ,
//                uint8_t , uint8_t , uint8_t , uint8_t ,
//                 uint8_t , uint8_t , bool , bool , bool ,
//                  bool , uint16_t))
//         .times(1)
//         .will(invoke(mock_img2colv2_cbuf_to_ca));

//     bfloat16_t dest[16 * 16 * sizeof(bfloat16_t)] = {0};
//     auto destAddr = reinterpret_cast<__ca__ bfloat16_t*>(dest);
//     auto l0aIterator = MakeL0AmemPtr(destAddr);
//     auto l0aMatrixLayout = MakeZZLayout<bfloat16_t>(16, 16);
//     auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);

//     bfloat16_t src[16 * 16 * sizeof(bfloat16_t)] = {0};
//     auto srcAddr = reinterpret_cast<__cbuf__ bfloat16_t*>(src);
//     auto l1Iterator = MakeL1memPtr(srcAddr);
//     auto l1MatrixLayout = MakeNZLayout<bfloat16_t>(16, 16);
//     auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout);

//     LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0aTensor, l1Tensor);


//      mockcpp::GlobalMockObject::reset();
// }

//   MOCKER_CPP(img2colv2_cbuf_to_ca, void(__ca__ TYPE*,__cbuf__ TYPE* , \
//               uint16_t , uint16_t , uint16_t , uint16_t , \
//                uint8_t , uint8_t , uint8_t , uint8_t , \
//                 uint8_t , uint8_t , bool , bool , bool , \
//                  bool , uint16_t)) \
//         .times(1) \
//         .will(invoke(static_cast<void(*)(__ca__ TYPE*,__cbuf__ TYPE* , uint16_t , uint16_t , uint16_t , uint16_t , uint8_t , uint8_t , uint8_t, uint8_t, uint8_t , uint8_t, bool , bool , bool, bool , uint16_t)>( \
//                     [](__ca__ TYPE* dst, __cbuf__ TYPE* src, \
//                         uint16_t stepK, uint16_t stepM, uint16_t posK, uint16_t posM, \
//                         uint8_t strideW, uint8_t strideH, uint8_t Wk, uint8_t Hk, \
//                         uint8_t dilationW, uint8_t dilationH, bool filterW, \
//                         bool filterH, bool transpose, bool fmatrixCtrl, uint16_t sizeChannel) { \
//                         std::cout << "目前mock有问题" << std::endl; \
//                     } \
//                  ))); \

#define LOAD_DATA_TEST_NZ2ZZ_NoCoord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_NZ2ZZ_NoCoord_##TYPE) { \
 \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
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
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeNZLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
    auto coordZero = MakeCoord(AscendC::Std::Int<0>{}, AscendC::Std::Int<0>{}); \
    LoadData<DEFAULT_LOAD_DATA_TRAIT>(l0Tensor, l1Tensor, coordZero); \
    mockcpp::GlobalMockObject::reset(); \
}
#define LOAD_DATA_TEST_ZN2ZZ_NoCoord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_ZN2ZZ_NoCoord_##TYPE) { \
 \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
    auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout); \
 \
     constexpr static LoadDataTrait trait(true);\
    LoadData<trait>(l0Tensor, l1Tensor); \
 \
    mockcpp::GlobalMockObject::reset(); \
}
#define LOAD_DATA_TEST_ZN2ZZ_Coord(TYPE, L0_SRC)\
TEST_F(TEST_LOAD_DATA, TestLoadData_l12l0##L0_SRC##_ZN2ZZ_Coord_##TYPE) { \
 \
    TYPE dest[32 * 32 * sizeof(TYPE)] = {0}; \
    auto destAddr = reinterpret_cast<__ca__ TYPE*>(dest); \
    auto l0Iterator = MakeL0##L0_SRC##memPtr(destAddr); \
    auto l0MatrixLayout = MakeZZLayout<TYPE>(32, 32); \
    auto l0Tensor = MakeTensor(l0Iterator, l0MatrixLayout); \
 \
    TYPE src[32 * 32 * sizeof(TYPE)] = {0}; \
    auto srcAddr = reinterpret_cast<__cbuf__ TYPE*>(src); \
    auto l1Iterator = MakeL1memPtr(srcAddr); \
    auto l1MatrixLayout = MakeZNLayout<TYPE>(32, 32); \
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
/* TODO cann包stub缺少类型支持 */ 
// LOAD_DATA_TEST_NZ2ZZ_NoCoord(int16_t, A);
// LOAD_DATA_TEST_NZ2ZZ_Coord(int16_t, A);
// LOAD_DATA_TEST_ZN2ZZ_NoCoord(int16_t, A);
// LOAD_DATA_TEST_ZN2ZZ_Coord(int16_t, A);
// LOAD_DATA_TEST_NZ2ZZ_NoCoord(int16_t, B);
// LOAD_DATA_TEST_NZ2ZZ_Coord(int16_t, B);
// LOAD_DATA_TEST_ZN2ZZ_NoCoord(int16_t, B);
// LOAD_DATA_TEST_ZN2ZZ_Coord(int16_t, B);
// LOAD_DATA_TEST_NZ2ZZ_NoCoord(uint16_t, A);
// LOAD_DATA_TEST_NZ2ZZ_Coord(uint16_t, A);
// LOAD_DATA_TEST_ZN2ZZ_NoCoord(uint16_t, A);
// LOAD_DATA_TEST_ZN2ZZ_Coord(uint16_t, A);
// LOAD_DATA_TEST_NZ2ZZ_NoCoord(uint16_t, B);
// LOAD_DATA_TEST_NZ2ZZ_Coord(uint16_t, B);
// LOAD_DATA_TEST_ZN2ZZ_NoCoord(uint16_t, B);
// LOAD_DATA_TEST_ZN2ZZ_Coord(uint16_t, B);
/* core dump */
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
