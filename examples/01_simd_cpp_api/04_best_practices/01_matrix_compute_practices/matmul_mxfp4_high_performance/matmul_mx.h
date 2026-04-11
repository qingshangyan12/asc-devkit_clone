/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file matmul.h
 * \brief Header file for matmul_mx case0~case2 implementation
 */

#ifndef MATMUL_H
#define MATMUL_H

#include "data_utils.h"
#include "kernel_tiling/kernel_tiling.h"
#include "tiling/platform/platform_ascendc.h"
#include "tiling/tiling_api.h"
#include "acl/acl.h"
#include "kernel_operator.h"
#define ASCENDC_CUBE_ONLY
#include "lib/matmul_intf.h"

constexpr uint32_t M = 8192;
constexpr uint32_t N = 8192;
constexpr uint32_t K = 8192;
constexpr bool IS_TRANS_A = false;
constexpr bool IS_TRANS_B = false;
constexpr bool IS_BIAS = false;

constexpr uint32_t BASE_M = 256;
constexpr uint32_t BASE_K = 256;
constexpr uint32_t BASE_N = 256;

constexpr uint32_t SINGLE_M = 2048;
constexpr uint32_t SINGLE_K = 8192;
constexpr uint32_t SINGLE_N = 1024;

constexpr int SCALE_CEIL_NUMBER = 64;
constexpr int SCALE_NUMBER = 2;

template <typename TilingType>
__aicore__ inline void CopyTiling(TilingType* tiling, GM_ADDR tilingGM)
{
    uint32_t* ptr = reinterpret_cast<uint32_t*>(tiling);
    auto tiling32 = reinterpret_cast<__gm__ uint32_t*>(tilingGM);
    for (uint32_t i = 0; i < sizeof(TilingType) / sizeof(uint32_t); i++, ptr++) {
        *ptr = *(tiling32 + i);
    }
}

template <typename AType, typename BType, typename CType, typename BiasType, bool isMdl = false>
class MatmulKernel {
public:
    static constexpr auto MatmulConfig = isMdl ? CFG_MDL : CFG_NORM;
    __aicore__ inline MatmulKernel(){};
    __aicore__ inline void Init(
        GM_ADDR a, GM_ADDR b, GM_ADDR as, GM_ADDR bs, GM_ADDR bias, GM_ADDR c, const TCubeTiling& tiling);
    __aicore__ inline void Process(AscendC::TPipe* pipe);

    using aType =
        AscendC::MatmulTypeWithScale<AscendC::TPosition::GM, AscendC::TPosition::GM, CubeFormat::ND, AType, false>;
    using bType =
        AscendC::MatmulTypeWithScale<AscendC::TPosition::GM, AscendC::TPosition::GM, CubeFormat::ND, BType, false>;
    using cType = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>;
    using biasType = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>;

    AscendC::MatmulImpl<
        aType, bType, cType, biasType, MatmulConfig, AscendC::MatmulCallBackFunc<nullptr, nullptr, nullptr>,
        AscendC::Impl::Detail::MatmulWithScalePolicy>
        matmulObj;

private:
    __aicore__ inline void CalcOffset(
        int32_t blockIdx, int32_t& offsetA, int32_t& offsetB, int32_t& offsetAscale, int32_t& offsetBscale,
        int32_t& offsetC, int32_t& offsetBias);

    AscendC::GlobalTensor<fp4x2_e1m2_t> aGlobal;
    AscendC::GlobalTensor<fp4x2_e1m2_t> bGlobal;
    AscendC::GlobalTensor<CType> cGlobal;
    AscendC::GlobalTensor<BiasType> biasGlobal;
    AscendC::GlobalTensor<fp8_e8m0_t> asGlobal;
    AscendC::GlobalTensor<fp8_e8m0_t> bsGlobal;
    TCubeTiling tiling;
};

template <typename AType, typename BType, typename CType, typename BiasType, bool isMdl>
__aicore__ inline void MatmulKernel<AType, BType, CType, BiasType, isMdl>::Init(
    GM_ADDR a, GM_ADDR b, GM_ADDR as, GM_ADDR bs, GM_ADDR bias, GM_ADDR c, const TCubeTiling& tiling)
{
    this->tiling = tiling;

    int sK = (tiling.Ka + SCALE_CEIL_NUMBER - 1) / SCALE_CEIL_NUMBER * SCALE_NUMBER;
    aGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ AType*>(a), tiling.M * tiling.Ka);
    bGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ BType*>(b), tiling.Kb * tiling.N);
    cGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ CType*>(c), tiling.M * tiling.N);
    biasGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ BiasType*>(bias), tiling.N);
    asGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ fp8_e8m0_t*>(as), tiling.M * sK);
    bsGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ fp8_e8m0_t*>(bs), sK * tiling.N);

    int32_t offsetA = 0, offsetB = 0, offsetC = 0, offsetBias = 0;
    int32_t offsetAscale = 0;
    int32_t offsetBscale = 0;
    CalcOffset(AscendC::GetBlockIdx(), offsetA, offsetB, offsetAscale, offsetBscale, offsetC, offsetBias);
    aGlobal = aGlobal[offsetA];
    bGlobal = bGlobal[offsetB];
    cGlobal = cGlobal[offsetC];
    biasGlobal = biasGlobal[offsetBias];
    asGlobal = asGlobal[offsetAscale];
    bsGlobal = bsGlobal[offsetBscale];

    if (GetSysWorkSpacePtr() == nullptr) {
        return;
    }
}

template <typename AType, typename BType, typename CType, typename BiasType, bool isMdl>
__aicore__ inline void MatmulKernel<AType, BType, CType, BiasType, isMdl>::Process(AscendC::TPipe* pipe)
{
    if (AscendC::GetBlockIdx() >= tiling.usedCoreNum) {
        return;
    }

    matmulObj.SetTensorA(aGlobal, IS_TRANS_A);
    matmulObj.SetTensorB(bGlobal, IS_TRANS_B);
    matmulObj.SetTensorScaleA(asGlobal, false);
    matmulObj.SetTensorScaleB(bsGlobal, false);

    if (tiling.isBias) {
        matmulObj.SetBias(biasGlobal);
    }
    matmulObj.IterateAll(cGlobal);
    matmulObj.End();
}

template <typename AType, typename BType, typename CType, typename BiasType, bool isMdl>
__aicore__ inline void MatmulKernel<AType, BType, CType, BiasType, isMdl>::CalcOffset(
    int32_t blockIdx, int32_t& offsetA, int32_t& offsetB, int32_t& offsetAscale, int32_t& offsetBscale,
    int32_t& offsetC, int32_t& offsetBias)
{
    const TCubeTiling& tiling = this->tiling;
    auto mSingleBlocks = (tiling.M + tiling.singleCoreM - 1) / tiling.singleCoreM;
    auto mCoreIndx = blockIdx % mSingleBlocks;
    auto nCoreIndx = blockIdx / mSingleBlocks;

    offsetA = mCoreIndx * tiling.Ka * tiling.singleCoreM;
    if (IS_TRANS_A) {
        offsetA = mCoreIndx * tiling.singleCoreM;
    }
    offsetB = nCoreIndx * tiling.singleCoreN;
    if (IS_TRANS_B) {
        offsetB = nCoreIndx * tiling.Kb * tiling.singleCoreN;
    }

    int sK = (tiling.Ka + SCALE_CEIL_NUMBER - 1) / SCALE_CEIL_NUMBER * SCALE_NUMBER;
    offsetAscale = mCoreIndx * sK * tiling.singleCoreM;
    offsetBscale = nCoreIndx * tiling.singleCoreN * SCALE_NUMBER;

    offsetC = mCoreIndx * tiling.N * tiling.singleCoreM + nCoreIndx * tiling.singleCoreN;
    offsetBias = nCoreIndx * tiling.singleCoreN;

    int tailM = tiling.M - mCoreIndx * tiling.singleCoreM;
    tailM = tailM < tiling.singleCoreM ? tailM : tiling.singleCoreM;
    int tailN = tiling.N - nCoreIndx * tiling.singleCoreN;
    tailN = tailN < tiling.singleCoreN ? tailN : tiling.singleCoreN;
    if (tailM < tiling.singleCoreM || tailN < tiling.singleCoreN) {
        matmulObj.SetTail(tailM, tailN);
    }
}

template <typename TilingType>
{
    tilingApi.SetAType(
        matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT4_E1M2,
        IS_TRANS_A);
    tilingApi.SetBType(
        matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT4_E1M2,
        IS_TRANS_B);
    tilingApi.SetCType(
        matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
    tilingApi.SetBiasType(
        matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
    tilingApi.SetScaleAType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, false);
    tilingApi.SetScaleBType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, false);

    tilingApi.SetOrgShape(M, N, K);
    tilingApi.SetShape(M, N, K);
    tilingApi.EnableBias(IS_BIAS);
    tilingApi.SetBufferSpace(-1, -1, -1);
    tilingApi.SetMadType(matmul_tiling::MatrixMadType::MXMODE);
}

template <typename TilingType, bool isTiling = false>
inline void SetTilingTypes(TilingType& tilingApi, optiling::TCubeTiling& tilingData)
{
    SetTilingCommon(tilingApi);
    if constexpr (isTiling) {
        tilingApi.SetFixSplit(BASE_M, BASE_N, BASE_K);
    } else {
        tilingApi.SetFixSplit(64, 64, 64);
    }
    int64_t res = tilingApi.GetTiling(tilingData);
    if (res == -1) {
        std::cout << "gen tiling failed" << std::endl;
    }
}

void SetL1(optiling::TCubeTiling& tilingData)
{
    tilingData.set_depthA1(1);
    tilingData.set_depthB1(1);
    tilingData.set_stepM(1);
    tilingData.set_stepN(1);
    tilingData.set_stepKa(1);
    tilingData.set_stepKb(1);
}
template <bool isTiling = false>
void GenerateTilingSingleCore(platform_ascendc::PlatformAscendC* ascendcPlatform, uint8_t* tilingBuf);

template <bool IsBaselineMX = false, bool IsMXScale = false>
void GenerateTilingMultiCore(platform_ascendc::PlatformAscendC* ascendcPlatform, uint8_t* tilingBuf);

#endif
