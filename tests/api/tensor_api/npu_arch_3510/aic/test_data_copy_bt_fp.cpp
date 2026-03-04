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
#include "include/experimental/tensor_api/tensor.h"
#include <mockcpp/mockcpp.hpp>

class TEST_TENSOR_API_L12FP : public testing::Test {
protected:
    void SetUp() override {
        AscendC::SetGCoreType(1);
        is_mock_copy_matrix_cc_to_gm = true;
    }
    
    void TearDown() override {
        AscendC::SetGCoreType(0);
        is_mock_copy_matrix_cc_to_gm = false;
    }
};

using namespace AscendC::Te;

enum class CubeFormat {
    ND = 0,
    NZ,
    DN,
};

template <CubeFormat FORMAT, typename TYPE>
struct InputInfo {
    constexpr static CubeFormat format = FORMAT;
    using T = TYPE;
};

template <class L1_TYPE, class FP_TYPE>
class TestCase {
    using FpT = typename FP_TYPE::T;
    using L1T = typename L1_TYPE::T;

public:
    __aicore__ inline TestCase() {}
    __aicore__ inline void TestRun(int32_t n)
    {
        nLength_ = n;
        l1Addr = reinterpret_cast<__cbuf__ L1T*>(0);
        fpAddr = reinterpret_cast<__cc__ FpT*>(0);

        auto l1Iterator = MakeL1memPtr(l1Addr);
        auto l1MatrixLayout = MakeNDLayout<L1T>(1, nLength_);
        auto l1Tensor = MakeTensor(l1Iterator, l1MatrixLayout);

        auto fpIterator = MakeFixbufmemPtr(fpAddr);
        auto fpMatrixLayout = MakeNDLayout<FpT>(1, nLength_);
        auto fpTensor = MakeTensor(fpIterator, l1MatrixLayout);

        DataCopy<DEFAULT_DATA_COPY_TRAIT>(fpIterator, l1Addr); \
    }


private:
    int32_t nLength_ = 0;

    __cbuf__ L1T* l1Addr;
    __fbuf__ FpT* fpAddr;

};

template <class L1_TYPE, class FP_TYPE>
__aicore__ inline void TestFixpipe(int32_t n, int32_t usedCoreNum)
{
    // cube core cases, ignore vector core
    if (g_coreType == AscendC::AIV) {
        return;
    }

    using L1_T = typename L1_TYPE::T;
    using FP_T = typename FP_TYPE::T;

    if (block_idx >= usedCoreNum) {
        return;
    }

    TestCase<L1_TYPE, FP_TYPE> ins;
    ins.TestRun(n);
}

#define KERNEL_TENSOR_API_L12FP_ND2ND(coreNum, N, DTYPE) \
    TEST_F(TEST_TENSOR_API_L12FP, kernel_tensor_api_l12fp_##coreNum##_##N##_##DTYPE) \
    { \
        uint8_t cGM[M * N * sizeof(C_DType)] = {0}; \
        typedef InputInfo<CubeFormat::NZ, DTYPE> l0cType; \
        typedef InputInfo<CubeFormat::C_Format, C_DType> cType; \
        TestFixpipe<l0cType, cType, QuantMode_t::Quant_Mode, Has_Coord>(cGM, M, N, coreNum); \
        for (uint32_t i = 0; i < M * N; i++) { \
            EXPECT_EQ(cGM[i], 0x00); \
        } \
    }

KERNEL_TENSOR_API_L12FP_ND2ND(1, 16, float)
KERNEL_TENSOR_API_L12FP_ND2ND(1, 16, float)
KERNEL_TENSOR_API_L12FP_ND2ND(1, 64, float)
KERNEL_TENSOR_API_L12FP_ND2ND(1, 64, float)
KERNEL_TENSOR_API_L12FP_ND2ND(1, 16, float)
