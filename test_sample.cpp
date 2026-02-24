#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "include/experimental/tensor_api/tensor.h"

class TEST_TENSOR_API_MMAD : public testing::Test {
protected:
    void SetUp()
    {
        AscendC::SetGCoreType(1);
    }
    void TearDown()
    {
        AscendC::SetGCoreType(0);
    }
};

#define MMAD_TEST(DST_TYPE, SRC_TYPE, M, N, K, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL) \
void MmadTEST(__CC__ DST_TYPE* dst, __ca__ SRC_TYPE* fm, __cb__ SRC_TYPE* filter, uint16_t m, uint16_t k, uint16_t n, \
            uint8_t unitFlag, bool kDirectionAlign, bool cmatrixSource, bool cmatrixInitVal) \
{\
    EXPECT_EQ(dst, reinterpret_cast<__cc__ DST_TYPE*>(0));\
    EXPECT_EQ(fm, reinterpret_cast<__ca__ SRC_TYPE*>(0));\
    EXPECT_EQ(filter, reinterpret_cast<__cb__ DST_TYPE*>(0));\
    EXPECT_EQ(m, M);\
    EXPECT_EQ(n, N);\
    EXPECT_EQ(k, K);\
    EXPECT_EQ(kDirectionAlign, K_DIRECTION_ALIGN);\
    EXPECT_EQ(cmatrixSource, CMATRIX_SOURCE);\
    EXPECT_EQ(cmatrixInitVal, C_MATRIX_INIT_VAL);\
}\
\
TEST_F(TEST_TENSOR_API_MMAD, MmadOperation) \
{\
    auto a2addr = reinterpret_cast<__ca__ SRC_TYPE*>(0); \
    auto l0aIterator = MakeL0AmemPtr(a2addr);\
    auto l0aMatrixLayout = MakeZZLayout<SRC_TYPE>(M, K); \
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout); \
\
    auto b2addr = reinterpret_cast<__cb__ SRC_TYPE*>(0); \
    auto l0bIterator = MakeL0AmemPtr(b2addr);\
    auto l0bMatrixLayout = MakeZZLayout<SRC_TYPE>(M, K); \
    auto l0bTensor = MakeTensor(l0bIterator, l0bMatrixLayout); \
\
    auto c2addr = reinterpret_cast<__cc__ DST_TYPE*>(0); \
    auto l0cIterator = MakeL0AmemPtr(c2addr);\
    auto l0cMatrixLayout = MakeZZLayout<AscendC::STD::ignore_t>(M, K); \
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout); \
\
    MOCK_CPP(mad, void(__cc__ DST_TYPE *, __ca__ SRC_TYPE *, __cb__ SRC_TYPE *, uint16_t, uint16_t, uint16_t, uint8_t, bool, bool, bool))\
    .times(1)\
    .will(invoke(MmadTest));\
    static constexpr MmadTrait trait = {0, 0, 0, 0, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL}; \
    Mmad<trait>(l0cTensor, l0aTensor, l0bTensor);\
    GlobalMockObject::verify();\
}

MMAD_TEST(float, half, 16, 16, 16, 0, 0, 0, 0);