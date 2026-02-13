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
#include <mockcpp/mockcpp.hpp>
#include "include/experimental/tensor_api/tensor.h"

class Tensor_Api_Mmad : public testing::Test {
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
using namespace AscendC::Te;

#define TENSOR_TILE_MMAD_EMPTY_ROUTING_INPUT_POS_DISMATCH(DST_POS, FM_POS, FILTER_POS, DST_TYPE, SRC_TYPE, M, N, K,\
        UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL)\
uint8_t A2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t B2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t C2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS[256 * 256 * sizeof(DST_TYPE)] = {0};\
TEST_F(Tensor_Api_Mmad_Empty_Routing, MmadOperationInputPosDismatch##DST_TYPE##_##SRC_TYPE)\
{\
    auto a2Addr = reinterpret_cast<FM_POS SRC_TYPE*>(A2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS);\
    auto l0aIterator = MakeL0BmemPtr(a2Addr);\
    auto l0aMatrixLayout = MakeZZLayout<SRC_TYPE>(M, K);\
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);\
\
    auto b2Addr = reinterpret_cast<FILTER_POS SRC_TYPE*>(B2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS);\
    auto l0bIterator = MakeL0AmemPtr(b2Addr);\
    auto l0bMatrixLayout = MakeZNLayout<SRC_TYPE>(K, N);\
    auto l0bTensor = MakeTensor(l0bIterator, l0bMatrixLayout);\
\
    auto c2Addr = reinterpret_cast<DST_POS DST_TYPE*>(C2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS);\
    auto l0cIterator = MakeL0CmemPtr(c2Addr);\
    auto l0cMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(M, N);\
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);\
\
     MOCKER_CPP(mad, void(__cc__ DST_TYPE *,\
                __cb__ SRC_TYPE *, uint16_t, uint16_t, uint16_t, uint8_t,\
                bool, bool, bool))\
            .times(0);\
    static constexpr MmadTrait triat = {0, 0, 0, 0, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL};\
    Mmad<triat>(l0cTensor, l0aTensor, l0bTensor);\
    GlobalMockObject::verify();\
}

TENSOR_TILE_MMAD_EMPTY_ROUTING_INPUT_POS_DISMATCH(__cc__, __cb__, __ca__, float, half, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_INPUT_POS_DISMATCH(__cc__, __cb__, __ca__, float, float, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_INPUT_POS_DISMATCH(__cc__, __cb__, __ca__, float, bfloat16_t, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_INPUT_POS_DISMATCH(__cc__, __cb__, __ca__, int32_t, int8_t, 32, 32, 32, 0, 0, 0, 0)


#define TENSOR_TILE_MMAD_EMPTY_ROUTING_OUTPUT_POS_DISMATCH(DST_POS, FM_POS, FILTER_POS, DST_TYPE, SRC_TYPE, M, N, K,\
        UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL)\
uint8_t A2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t B2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t C2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS[256 * 256 * sizeof(DST_TYPE)] = {0};\
TEST_F(Tensor_Api_Mmad_Empty_Routing, MmadOperationOutputPosDismatch##DST_TYPE##_##SRC_TYPE)\
{\
    auto a2Addr = reinterpret_cast<FM_POS SRC_TYPE*>(A2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS);\
    auto l0aIterator = MakeL0AmemPtr(a2Addr);\
    auto l0aMatrixLayout = MakeZZLayout<SRC_TYPE>(M, K);\
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);\
\
    auto b2Addr = reinterpret_cast<FILTER_POS SRC_TYPE*>(B2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS);\
    auto l0bIterator = MakeL0BmemPtr(b2Addr);\
    auto l0bMatrixLayout = MakeZNLayout<SRC_TYPE>(K, N);\
    auto l0bTensor = MakeTensor(l0bIterator, l0bMatrixLayout);\
\
    auto c2Addr = reinterpret_cast<DST_POS DST_TYPE*>(C2##DST_TYPE##_##SRC_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS);\
    auto l0cIterator = MakeL0AmemPtr(c2Addr);\
    auto l0cMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(M, N);\
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);\
\
     MOCKER_CPP(mad, void(__cc__ DST_TYPE *,\
                __cb__ SRC_TYPE *, uint16_t, uint16_t, uint16_t, uint8_t,\
                bool, bool, bool))\
            .times(0);\
    static constexpr MmadTrait triat = {0, 0, 0, 0, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL};\
    Mmad<triat>(l0cTensor, l0aTensor, l0bTensor);\
    GlobalMockObject::verify();\
}

TENSOR_TILE_MMAD_EMPTY_ROUTING_OUTPUT_POS_DISMATCH(__ca__, __ca__, __cb__, float, half, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_OUTPUT_POS_DISMATCH(__ca__, __ca__, __cb__, float, float, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_OUTPUT_POS_DISMATCH(__ca__, __ca__, __cb__, float, bfloat16_t, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_OUTPUT_POS_DISMATCH(__ca__, __ca__, __cb__, int32_t, int8_t, 32, 32, 32, 0, 0, 0, 0)


#define TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_INPUT_POS_DISMATCH(DST_POS, FM_POS, FILTER_POS, BIAS_POS, DST_TYPE, SRC_TYPE, BIAS_TYPE,\
        M, N, K, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL)\
uint8_t A2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t B2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t C2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(DST_TYPE)] = {0};\
uint8_t BIAS##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(BIAS_TYPE)] = {0};\
TEST_F(Tensor_Api_Mmad_Empty_Routing, MmadOperationWithBiasInputPosDismatch##DST_TYPE##_##SRC_TYPE)\
{\
    auto a2Addr = reinterpret_cast<FM_POS SRC_TYPE*>(A2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto l0aIterator = MakeL0BmemPtr(a2Addr);\
    auto l0aMatrixLayout = MakeZZLayout<SRC_TYPE>(M, K);\
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);\
\
    auto b2Addr = reinterpret_cast<FILTER_POS SRC_TYPE*>(B2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto l0bIterator = MakeL0AmemPtr(b2Addr);\
    auto l0bMatrixLayout = MakeZNLayout<SRC_TYPE>(K, N);\
    auto l0bTensor = MakeTensor(l0bIterator, l0bMatrixLayout);\
\
    auto c2Addr = reinterpret_cast<DST_POS DST_TYPE*>(C2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto l0cIterator = MakeL0CmemPtr(c2Addr);\
    auto l0cMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(M, N);\
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);\
\
    auto biasAddr = reinterpret_cast<BIAS_POS BIAS_TYPE*>(BIAS##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto biasIterator = MakeL0CmemPtr(biasAddr);\
    auto biasMatrixLayout =  MakeRowMajorLayout<BIAS_TYPE>(M, N);\
    auto biasTensor = MakeTensor(biasIterator, biasMatrixLayout);\
\
     MOCKER_CPP(mad, void(__cc__ DST_TYPE *,\
                __cb__ SRC_TYPE *, uint64_t, uint16_t, uint16_t, uint16_t, uint8_t,\
                bool, bool, bool))\
            .times(0);\
    static constexpr MmadTrait triat = {0, 0, 0, 0, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL};\
    Mmad<triat>(l0cTensor, l0aTensor, l0bTensor, biasTensor);\
    GlobalMockObject::verify();\
}

TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_INPUT_POS_DISMATCH(__cc__, __cb__, __ca__, __cc__, float, float, float, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_INPUT_POS_DISMATCH(__cc__, __cb__, __ca__, __cc__, float, bfloat16_t, float, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_INPUT_POS_DISMATCH(__cc__, __cb__, __ca__, __cc__, float, half, float, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_INPUT_POS_DISMATCH(__cc__, __cb__, __ca__, __cc__, int32_t, int8_t, int32_t, 32, 32, 32, 0, 0, 0, 0)


#define TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_OUTPUT_POS_DISMATCH(DST_POS, FM_POS, FILTER_POS, BIAS_POS, DST_TYPE, SRC_TYPE, BIAS_TYPE,\
        M, N, K, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL)\
uint8_t A2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t B2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t C2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(DST_TYPE)] = {0};\
uint8_t BIAS##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(BIAS_TYPE)] = {0};\
TEST_F(Tensor_Api_Mmad_Empty_Routing, MmadOperationWithBiasInputPosDismatch##DST_TYPE##_##SRC_TYPE)\
{\
    auto a2Addr = reinterpret_cast<FM_POS SRC_TYPE*>(A2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto l0aIterator = MakeL0AmemPtr(a2Addr);\
    auto l0aMatrixLayout = MakeZZLayout<SRC_TYPE>(M, K);\
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);\
\
    auto b2Addr = reinterpret_cast<FILTER_POS SRC_TYPE*>(B2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto l0bIterator = MakeL0BmemPtr(b2Addr);\
    auto l0bMatrixLayout = MakeZNLayout<SRC_TYPE>(K, N);\
    auto l0bTensor = MakeTensor(l0bIterator, l0bMatrixLayout);\
\
    auto c2Addr = reinterpret_cast<DST_POS DST_TYPE*>(C2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto l0cIterator = MakeL0BmemPtr(c2Addr);\
    auto l0cMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(M, N);\
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);\
\
    auto biasAddr = reinterpret_cast<BIAS_POS BIAS_TYPE*>(BIAS##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto biasIterator = MakeL0CmemPtr(biasAddr);\
    auto biasMatrixLayout =  MakeRowMajorLayout<BIAS_TYPE>(M, N);\
    auto biasTensor = MakeTensor(biasIterator, biasMatrixLayout);\
\
     MOCKER_CPP(mad, void(__cc__ DST_TYPE *,\
                __cb__ SRC_TYPE *, uint64_t, uint16_t, uint16_t, uint16_t, uint8_t,\
                bool, bool, bool))\
            .times(0);\
    static constexpr MmadTrait triat = {0, 0, 0, 0, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL};\
    Mmad<triat>(l0cTensor, l0aTensor, l0bTensor, biasTensor);\
    GlobalMockObject::verify();\
}

TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_OUTPUT_POS_DISMATCH(__cb__, __ca__, __cb__, __cc__, float, float, float, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_OUTPUT_POS_DISMATCH(__cb__, __ca__, __cb__, __cc__, float, bfloat16_t, float, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_OUTPUT_POS_DISMATCH(__cb__, __ca__, __cb__, __cc__, float, half, float, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_OUTPUT_POS_DISMATCH(__cb__, __ca__, __cb__, __cc__, int32_t, int8_t, int32_t, 32, 32, 32, 0, 0, 0, 0)


#define TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_BIAS_POS_DISMATCH(DST_POS, FM_POS, FILTER_POS, BIAS_POS, DST_TYPE, SRC_TYPE, BIAS_TYPE,\
        M, N, K, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL)\
uint8_t A2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t B2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(SRC_TYPE)] = {0};\
uint8_t C2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(DST_TYPE)] = {0};\
uint8_t BIAS##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS[256 * 256 * sizeof(BIAS_TYPE)] = {0};\
TEST_F(Tensor_Api_Mmad_Empty_Routing, MmadOperationWithBiasInputPosDismatch##DST_TYPE##_##SRC_TYPE)\
{\
    auto a2Addr = reinterpret_cast<FM_POS SRC_TYPE*>(A2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto l0aIterator = MakeL0AmemPtr(a2Addr);\
    auto l0aMatrixLayout = MakeZZLayout<SRC_TYPE>(M, K);\
    auto l0aTensor = MakeTensor(l0aIterator, l0aMatrixLayout);\
\
    auto b2Addr = reinterpret_cast<FILTER_POS SRC_TYPE*>(B2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto l0bIterator = MakeL0BmemPtr(b2Addr);\
    auto l0bMatrixLayout = MakeZNLayout<SRC_TYPE>(K, N);\
    auto l0bTensor = MakeTensor(l0bIterator, l0bMatrixLayout);\
\
    auto c2Addr = reinterpret_cast<DST_POS DST_TYPE*>(C2##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto l0cIterator = MakeL0BmemPtr(c2Addr);\
    auto l0cMatrixLayout = MakeNZLayout<AscendC::Std::ignore_t>(M, N);\
    auto l0cTensor = MakeTensor(l0cIterator, l0cMatrixLayout);\
\
    auto biasAddr = reinterpret_cast<BIAS_POS BIAS_TYPE*>(BIAS##DST_TYPE##_##SRC_TYPE##_##BIAS_TYPE##_##DST_POS##_##FM_POS##_##FILTER_POS##_##BIAS_POS);\
    auto biasIterator = MakeL0BmemPtr(biasAddr);\
    auto biasMatrixLayout =  MakeRowMajorLayout<BIAS_TYPE>(M, N);\
    auto biasTensor = MakeTensor(biasIterator, biasMatrixLayout);\
\
     MOCKER_CPP(mad, void(__cc__ DST_TYPE *,\
                __cb__ SRC_TYPE *, uint64_t, uint16_t, uint16_t, uint16_t, uint8_t,\
                bool, bool, bool))\
            .times(0);\
    static constexpr MmadTrait triat = {0, 0, 0, 0, UNIT_FLAG, K_DIRECTION_ALIGN, CMATRIX_SOURCE, C_MATRIX_INIT_VAL};\
    Mmad<triat>(l0cTensor, l0aTensor, l0bTensor, biasTensor);\
    GlobalMockObject::verify();\
}

TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_BIAS_POS_DISMATCH(__cc__, __ca__, __cb__, __cb__, float, float, float, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_BIAS_POS_DISMATCH(__cc__, __ca__, __cb__, __cb__, float, bfloat16_t, float, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_BIAS_POS_DISMATCH(__cc__, __ca__, __cb__, __cb__, float, half, float, 16, 16, 16, 0, 0, 0, 0)
TENSOR_TILE_MMAD_EMPTY_ROUTING_WITH_BIAS_BIAS_POS_DISMATCH(__cc__, __ca__, __cb__, __cb__, int32_t, int8_t, int32_t, 32, 32, 32, 0, 0, 0, 0)