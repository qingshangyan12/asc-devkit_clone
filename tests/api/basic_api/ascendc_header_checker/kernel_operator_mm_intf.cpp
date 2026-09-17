/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_data_copy_intf.h"
#include "kernel_operator_mm_intf.h"
#endif

// __aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData2DParams&
// loadDataParams);
extern "C" __global__ __aicore__ void KernelTestLoadData1()
{
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::LoadData2DParams loadDataParams;
    AscendC::LoadData(dst, src, loadDataParams);
}

#if (__NPU_ARCH__ != 5102)
// __aicore__ inline __inout_pipe__(MTE2) void LoadData(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const
// LoadData2DParams& loadDataParams);
extern "C" __global__ __aicore__ void KernelTestLoadData2()
{
    AscendC::LocalTensor<half> dst;
    AscendC::GlobalTensor<half> src;
    AscendC::LoadData2DParams loadDataParams;
    AscendC::LoadData(dst, src, loadDataParams);
}
#endif

// __aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData2DParamsV2&
// loadDataParams);
extern "C" __global__ __aicore__ void KernelTestLoadData3()
{
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::LoadData2DParamsV2 loadDataParams;
    AscendC::LoadData(dst, src, loadDataParams);
}

// __aicore__ inline __inout_pipe__(MTE2) void LoadData(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const
// LoadData2DParamsV2& loadDataParams);
extern "C" __global__ __aicore__ void KernelTestLoadData4()
{
    AscendC::LocalTensor<float> dst;
    AscendC::GlobalTensor<float> src;
    AscendC::LoadData2DParamsV2 loadDataParams;
    AscendC::LoadData(dst, src, loadDataParams);
}

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
// __aicore__ inline void LoadData(const LocalTensor<U>& dst, const LocalTensor<T>& src, const LocalTensor<fp8_e8m0_t>&
// srcMx, const LoadData2DParamsV2& loadDataParams, const LoadData2DMxParams& loadMxDataParams);
extern "C" __global__ __aicore__ void KernelTestLoadData5()
{
    AscendC::LocalTensor<fp4x2_e2m1_t> dst;
    AscendC::LocalTensor<fp4x2_e2m1_t> src;
    // AscendC::LocalTensor<AscendC::fp8_e8m0_t> srcMx;
    AscendC::LocalTensor<fp8_e8m0_t> srcMx;
    AscendC::LoadData2DParamsV2 loadDataParams;
    AscendC::LoadData2DMxParams loadMxDataParams;
    AscendC::LoadData(dst, src, srcMx, loadDataParams, loadMxDataParams);
}
#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5102)
// __aicore__ inline __inout_pipe__(MTE2) void LoadData(const LocalTensor<T>& dst, const GlobalTensor<U>& src, const
// LoadData2DParamsV2& loadDataParams, const Nd2NzParamsV2& nd2nzParams)
extern "C" __global__ __aicore__ void KernelTestLoadData6()
{
    AscendC::LocalTensor<int8_t> dst;
    AscendC::GlobalTensor<AscendC::int4b_t> src;
    AscendC::LoadData2DParamsV2 loadDataParams;
    AscendC::Nd2NzParamsV2 nd2nzParams;
    AscendC::LoadData(dst, src, loadDataParams, nd2nzParams);
}
#endif

// __aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV1<U>&
// loadDataParams);
extern "C" __global__ __aicore__ void KernelTestLoadData7()
{
    AscendC::LocalTensor<half> dst;
    AscendC::LocalTensor<half> src;
    AscendC::LoadData3DParamsV1<half> loadDataParams;
    AscendC::LoadData(dst, src, loadDataParams);
}

// __aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV2<U>&
// loadDataParams);
extern "C" __global__ __aicore__ void KernelTestLoadData8()
{
    AscendC::LocalTensor<half> dst;
    AscendC::LocalTensor<half> src;
    AscendC::LoadData3DParamsV2<half> loadDataParams;
    AscendC::LoadData(dst, src, loadDataParams);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
// // template <TPosition DstPos, TPosition SrcPos, typename T>
// // __aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const Load3DBitModeParam&
// loadDataParams); extern "C" __global__ __aicore__ void KernelTestLoadData9() {
//     AscendC::LocalTensor<float> dst;
//     AscendC::LocalTensor<float> src;
//     AscendC::Load3DBitModeParam loadDataParams;
//     AscendC::LoadData(dst, src, loadDataParams);
// }
#endif

// __aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV2Pro&
// loadDataParams);
extern "C" __global__ __aicore__ void KernelTestLoadData10()
{
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::LoadData3DParamsV2Pro loadDataParams;
    AscendC::LoadData(dst, src, loadDataParams);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
// // __aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const Load2DBitModeParam&
// loadDataParams); extern "C" __global__ __aicore__ void KernelTestLoadData11() {
//     AscendC::LocalTensor<float> dst;
//     AscendC::LocalTensor<float> src;
//     AscendC::Load2DBitModeParam loadDataParams;
//     AscendC::LoadData(dst, src, loadDataParams);
// }
#endif

// __aicore__ inline void LoadDataWithTranspose(const LocalTensor<T>& dst, const LocalTensor<T>& src, const
// LoadData2dTransposeParams& loadDataParams);
extern "C" __global__ __aicore__ void KernelTestLoadDataWithTranspose1()
{
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::LoadData2dTransposeParams loadDataParams;
    AscendC::LoadDataWithTranspose(dst, src, loadDataParams);
}

// __aicore__ inline void LoadDataWithTranspose(const LocalTensor<T>& dst, const LocalTensor<T>& src, const
// LoadData2dTransposeParamsV2& loadDataParams);
extern "C" __global__ __aicore__ void KernelTestLoadDataWithTranspose2()
{
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::LoadData2dTransposeParamsV2 loadDataParams;
    AscendC::LoadDataWithTranspose(dst, src, loadDataParams);
}

// __aicore__ inline void Mmad(const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const
// MmadParams& mmadParams);
extern "C" __global__ __aicore__ void KernelTestMmad1()
{
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<half> fm;
    AscendC::LocalTensor<half> filter;
    AscendC::MmadParams mmadParams;
    AscendC::Mmad(dst, fm, filter, mmadParams);
}

// __aicore__ inline void Mmad(const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const
// LocalTensor<V>& bias, const MmadParams& mmadParams);
extern "C" __global__ __aicore__ void KernelTestMmad2()
{
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<half> fm;
    AscendC::LocalTensor<half> filter;
    AscendC::LocalTensor<half> bias;
    AscendC::MmadParams mmadParams;
    AscendC::Mmad(dst, fm, filter, bias, mmadParams);
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
// __aicore__ inline void Mmad(const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const
// MmadBitModeParams& mmadParams);
extern "C" __global__ __aicore__ void KernelTestMmad3()
{
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> fm;
    AscendC::LocalTensor<float> filter;
    AscendC::MmadBitModeParams mmadParams;
    AscendC::Mmad(dst, fm, filter, mmadParams);
}

// __aicore__ inline void Mmad(const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<S>& filter, const
// LocalTensor<V>& bias, MmadBitModeParams& mmadParams);
extern "C" __global__ __aicore__ void KernelTestMmad4()
{
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> fm;
    AscendC::LocalTensor<float> filter;
    AscendC::LocalTensor<float> bias;
    AscendC::MmadBitModeParams mmadParams;
    AscendC::Mmad(dst, fm, filter, bias, mmadParams);
}
#endif

#if __NPU_ARCH__ == 2201
// __aicore__ inline void MmadWithSparse(const LocalTensor<T>& dst, const LocalTensor<U>& fm, const LocalTensor<U>&
// filter, const MmadParams& mmadParams);
extern "C" __global__ __aicore__ void KernelTestMmadWithSparse1()
{
    AscendC::LocalTensor<int32_t> dst;
    AscendC::LocalTensor<int8_t> fm;
    AscendC::LocalTensor<int8_t> filter;
    AscendC::MmadParams mmadParams;
    AscendC::MmadWithSparse(dst, fm, filter, mmadParams);
}

// __aicore__ inline void LoadDataWithSparse(const LocalTensor<T> &dst, const LocalTensor<T> &src, const LocalTensor<U>
// &idx, const LoadData2dParams &loadDataParam);
extern "C" __global__ __aicore__ void KernelTestLoadDataWithSparse1()
{
    AscendC::LocalTensor<int8_t> dst;
    AscendC::LocalTensor<int8_t> src;
    AscendC::LocalTensor<uint8_t> idx;
    AscendC::LoadData2dParams loadDataParam;
    AscendC::LoadDataWithSparse(dst, src, idx, loadDataParam);
}
#endif

#if __NPU_ARCH__ == 2002
// __aicore__ inline void LoadUnzipIndex(const GlobalTensor<T>& src, uint32_t numOfIndexTabEntry);
extern "C" __global__ __aicore__ void KernelTestLoadUnzipIndex1()
{
    AscendC::GlobalTensor<int8_t> src;
    uint32_t numOfIndexTabEntry = 0;
    AscendC::LoadUnzipIndex(src, numOfIndexTabEntry);
}
#endif

#if __NPU_ARCH__ == 1001 || __NPU_ARCH__ == 2002
// __aicore__ inline __inout_pipe__(V) void BroadCastVecToMM(const LocalTensor<T> &dst, const LocalTensor<U> &src, const
// int32_t blockCount, const uint8_t blockLen, const uint8_t srcGap, const uint8_t dstGap);
extern "C" __global__ __aicore__ void KernelTestBroadCastVecToMM1()
{
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    int32_t blockCount = 0;
    uint8_t blockLen = 1;
    uint8_t srcGap = 1;
    uint8_t dstGap = 1;
    AscendC::BroadCastVecToMM(dst, src, blockCount, blockLen, srcGap, dstGap);
}
#endif

// __aicore__ inline void InitConstValue(const LocalTensor<T> &dst, const InitConstValueParams<U>
// &initConstValueParams);
extern "C" __global__ __aicore__ void KernelTestInitConstValue1()
{
    AscendC::LocalTensor<float> dst;
    AscendC::InitConstValueParams<float> initConstValueParams;
    AscendC::InitConstValue(dst, initConstValueParams);
}

// __aicore__ inline void SetLoadDataPaddingValue(const T padValue);
extern "C" __global__ __aicore__ void KernelTestSetLoadDataPaddingValue1()
{
    float padValue = 0.0f;
    AscendC::SetLoadDataPaddingValue(padValue);
}

// __aicore__ inline void SetFmatrix(uint16_t l1H, uint16_t l1W, const uint8_t padList[4], const FmatrixMode
// &fmatrixMode);
extern "C" __global__ __aicore__ void KernelTestSetFmatrix1()
{
    uint16_t l1H = 1;
    uint16_t l1W = 1;
    uint8_t padList[4];
    AscendC::FmatrixMode fmatrixMode{};
    AscendC::SetFmatrix(l1H, l1W, padList, fmatrixMode);
}

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
// __aicore__ inline void SetFmatrix(const SetFMatrixBitModeParams& param, const FmatrixMode &fmatrixMode);
extern "C" __global__ __aicore__ void KernelTestSetFmatrix2()
{
    AscendC::SetFMatrixBitModeParams param;
    AscendC::FmatrixMode fmatrixMode{};
    AscendC::SetFmatrix(param, fmatrixMode);
}
#endif

// __aicore__ inline void SetLoadDataBoundary(uint32_t boundaryValue);
extern "C" __global__ __aicore__ void KernelTestSetLoadDataBoundary1()
{
    uint32_t boundaryValue = 0;
    AscendC::SetLoadDataBoundary(boundaryValue);
}

// __aicore__ inline void SetLoadDataRepeat(const LoadDataRepeatParam& repeatParams);
extern "C" __global__ __aicore__ void KernelTestSetLoadDataRepeat1()
{
    AscendC::LoadDataRepeatParam repeatParams;
    AscendC::SetLoadDataRepeat(repeatParams);
}

#if __NPU_ARCH__ != 5102
// __aicore__ inline void LoadImageToLocal(const LocalTensor<T>& dst, const LoadImageToLocalParams& loadDataParams);
extern "C" __global__ __aicore__ void KernelTestLoadImageToLocal1()
{
    AscendC::LocalTensor<int8_t> dst;
    AscendC::LoadImageToLocalParams loadDataParams;
    AscendC::LoadImageToLocal(dst, loadDataParams);
}
#endif

#if __NPU_ARCH__ != 3510 && __NPU_ARCH__ != 5102
// __aicore__ inline void LoadDataUnzip(const LocalTensor<T>& dst, const GlobalTensor<T>& src);
extern "C" __global__ __aicore__ void KernelTestLoadDataUnzip1()
{
    AscendC::LocalTensor<float> dst;
    AscendC::GlobalTensor<float> src;
    AscendC::LoadDataUnzip(dst, src);
}
#endif

// __aicore__ inline void SetHF32Mode(bool hf32Mode);
extern "C" __global__ __aicore__ void KernelTestSetHF32Mode1()
{
    bool hf32Mode = true;
    AscendC::SetHF32Mode(hf32Mode);
}

// __aicore__ inline void SetHF32TransMode(bool hf32TransMode);
extern "C" __global__ __aicore__ void KernelTestSetHF32TransMode1()
{
    bool hf32TransMode = true;
    AscendC::SetHF32TransMode(hf32TransMode);
}

// __aicore__ inline void SetMMLayoutTransform(bool mmLayoutMode);
extern "C" __global__ __aicore__ void KernelTestSetMMLayoutTransform1()
{
    bool mmLayoutMode = true;
    AscendC::SetMMLayoutTransform(mmLayoutMode);
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
extern "C" __global__ __aicore__ void KernelTestCubePointerLoadData()
{
    __ca__ half* l0a;
    __cb__ half* l0b;
    __cbuf__ half* l1;
    __ubuf__ half* ub;
    AscendC::LoadData2DParams params2d;
    AscendC::LoadData2DParamsV2 params2dV2;
    AscendC::LoadData3DParamsV1<half> params3dV1;
    AscendC::LoadData3DParamsV2<half> params3dV2;
    AscendC::LoadData3DParamsV2Pro params3dPro;
    AscendC::LoadData(l0a, l1, params2d);
    AscendC::LoadData(l0b, l1, params2dV2);
    AscendC::GlobalTensor<half> gmTensor;
    AscendC::LoadData(l0a, gmTensor, params2d);
    AscendC::LoadData(l0b, gmTensor, params2dV2);
    AscendC::LoadData(l0a, l1, params3dV1);
    AscendC::LoadData(l0b, l1, params3dV2);
    AscendC::LoadData(ub, l1, params3dPro);

    AscendC::Load2DBitModeParam bit2d;
    AscendC::Load3DBitModeParam bit3d;
    AscendC::LoadData<AscendC::TPosition::A2, AscendC::TPosition::A1>(l0a, l1, bit2d);
    AscendC::LoadData<AscendC::TPosition::B2, AscendC::TPosition::B1>(l0b, l1, bit3d);

    __ca__ fp4x2_e2m1_t* mxDst;
    __cbuf__ fp4x2_e2m1_t* mxSrc;
    __cbuf__ fp8_e8m0_t* mxScale;
    AscendC::LoadData2DMxParams mxParams;
    AscendC::LoadData(mxDst, mxSrc, mxScale, params2dV2, mxParams);

    AscendC::LoadData2dTransposeParams transposeParams;
    AscendC::LoadData2dTransposeParamsV2 transposeParamsV2;
    AscendC::LoadDataWithTranspose(l0a, l1, transposeParams);
    AscendC::LoadDataWithTranspose(l0b, l1, transposeParamsV2);

    AscendC::LoadImageToLocalParams imageParams;
    AscendC::LoadImageToLocal(l1, imageParams);
    AscendC::InitConstValueParams<half> initParams;
    AscendC::InitConstValue(l0a, initParams);
}

extern "C" __global__ __aicore__ void KernelTestCubePointerMmad()
{
    __cc__ float* dst;
    __ca__ half* fm;
    __cb__ half* filter;
    __cc__ float* bias;
    AscendC::MmadParams params;
    AscendC::Mmad(dst, fm, filter, params);
    AscendC::Mmad(dst, fm, filter, bias, params);
    AscendC::LocalTensor<half> fmTensor;
    AscendC::LocalTensor<half> filterTensor;
    AscendC::Mmad(dst, fmTensor, filterTensor, params);

    __ca__ fp4x2_e2m1_t* mxFm;
    __cb__ fp4x2_e2m1_t* mxFilter;
    AscendC::MmadBitModeParams bitParams;
    AscendC::Mmad(dst, mxFm, mxFilter, bitParams);
    AscendC::Mmad(dst, mxFm, mxFilter, bias, bitParams);
    const AscendC::MmadBitModeParams constBitParams;
    AscendC::Mmad(dst, mxFm, mxFilter, bias, constBitParams);
    AscendC::MmadMx(dst, mxFm, mxFilter, params);
    AscendC::MmadMx(dst, mxFm, mxFilter, bias, params);
    AscendC::MmadMx(dst, mxFm, mxFilter, bitParams);
    AscendC::MmadMx(dst, mxFm, mxFilter, bias, bitParams);
}
#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5102)
extern "C" __global__ __aicore__ void KernelTestCubePointerNd2Nz()
{
    __cbuf__ int8_t* dst;
    __gm__ AscendC::int4b_t* src;
    AscendC::LoadData2DParamsV2 params;
    AscendC::Nd2NzParamsV2 nd2nzParams;
    AscendC::LoadData(dst, src, params, nd2nzParams);
}
#endif
