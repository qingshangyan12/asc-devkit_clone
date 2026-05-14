/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_mm_intf.h"


//template <typename T, typename U = T>
//__aicore__ inline void LoadData(const LocalTensor<U>& dst, const LocalTensor<T>& src,
//    const LocalTensor<fp8_e8m0_t>& srcMx, const LoadData2DParamsV2& loadDataParams,
//    const LoadData2DMxParams& loadMxDataParams);
void HostTestLoadDataMx() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::LocalTensor<AscendC::fp8_e8m0_t> srcMx;
    AscendC::LoadData2DParamsV2 loadDataParams(0, 0, 16, 32, 128, 64, false, 0);
    AscendC::LoadData2DMxParams loadMxDataParams(0, 0, 1, 1, 32, 16);
    AscendC::LoadData(dst, src, srcMx, loadDataParams, loadMxDataParams);
}

//template <typename T, typename U>
//__aicore__ inline __inout_pipe__(MTE2) void LoadData(const LocalTensor<T>& dst, const GlobalTensor<U>& src,
//    const LoadData2DParamsV2& loadDataParams, const Nd2NzParamsV2& nd2nzParams);
void HostTestLoadDataNd2NzV2() {
    AscendC::LocalTensor<float> dst;
    AscendC::GlobalTensor<float> src;
    AscendC::LoadData2DParamsV2 loadDataParams(0, 0, 16, 32, 128, 64, false, 0);
    AscendC::Nd2NzParamsV2 nd2nzParams;
    nd2nzParams.lookupTable0 = 0;
    nd2nzParams.lookupTable1 = 0;
    AscendC::LoadData<float, float>(dst, src, loadDataParams, nd2nzParams);
}

//template <typename T, const IsResetLoad3dConfig &defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG,
//    typename U = PrimT<T>, typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type = true>
//__aicore__ inline void LoadDataWithStride(const LocalTensor<T>& dst, const LocalTensor<T>& src,
//    const LoadData3DParamsV2<U>& loadDataParams);
void HostTestLoadDataWithStride() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    uint8_t padList[4] = {1, 1, 1, 1};
    AscendC::LoadData3DParamsV2<float> loadDataParams(padList, 16, 32, 64, 128, 256, 0, 0, 1, 1, 3, 3, 1, 1, false, false, 0.0f);
    AscendC::LoadDataWithStride<float>(dst, src, loadDataParams);
}


//template <TPosition DstPos, TPosition SrcPos, typename T>
//__aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src,
//    const Load3DBitModeParam& loadDataParams);
void HostTestLoadDataBitMode() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    uint8_t padList[4] = {1, 1, 1, 1};
    AscendC::LoadData3DParamsV2<float> loadData3DParams(padList, 16, 32, 64, 128, 256, 0, 0, 1, 1, 3, 3, 1, 1, false, false, 0.0f);
    AscendC::Load3DBitModeParam load3DBitModeParam(loadData3DParams);
    AscendC::LoadData<AscendC::TPosition::A2, AscendC::TPosition::A1, float>(dst, src, load3DBitModeParam);
}


//template <TPosition DstPos, TPosition SrcPos, typename T>
//__aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src,
//    const Load2DBitModeParam& loadDataParams);
void HostTestLoadData2DBitMode() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::LoadData2DParamsV2 loadData2DParams(0, 0, 16, 32, 128, 64, false, 0);
    AscendC::Load2DBitModeParam load2DBitModeParam(loadData2DParams);
    AscendC::LoadData<AscendC::TPosition::A2, AscendC::TPosition::A1, float>(dst, src, load2DBitModeParam);
}


//template <typename T, typename U, typename S>
//__aicore__ inline void Mmad(const LocalTensor<T>& dst, const LocalTensor<U>& fm,
//    const LocalTensor<S>& filter, const MmadBitModeParams& mmadParams);
void HostTestMmadBitMode() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> fm;
    AscendC::LocalTensor<float> filter;
    AscendC::MmadParams mmadParams;
    mmadParams.m = 16;
    mmadParams.n = 32;
    mmadParams.k = 64;
    mmadParams.unitFlag = 0;
    AscendC::MmadBitModeParams mmadBitModeParams(mmadParams);
    AscendC::Mmad<float, float, float>(dst, fm, filter, mmadBitModeParams);
}


//template <typename T, typename U, typename S, typename V>
//__aicore__ inline void Mmad(const LocalTensor<T>& dst, const LocalTensor<U>& fm,
//    const LocalTensor<S>& filter, const LocalTensor<V>& bias, MmadBitModeParams& mmadParams);
void HostTestMmadBitModeWithBias() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> fm;
    AscendC::LocalTensor<float> filter;
    AscendC::LocalTensor<float> bias;
    AscendC::MmadParams mmadParams;
    mmadParams.m = 16;
    mmadParams.n = 32;
    mmadParams.k = 64;
    mmadParams.unitFlag = 0;
    mmadParams.isBias = true;
    AscendC::MmadBitModeParams mmadBitModeParams(mmadParams);
    AscendC::Mmad<float, float, float, float>(dst, fm, filter, bias, mmadBitModeParams);
}


//template <typename T = int32_t, typename U = int8_t,
//    typename Std::enable_if<Std::is_same<PrimT<T>, int32_t>::value, bool>::type = true,
//    typename Std::enable_if<Std::is_same<PrimT<U>, int8_t>::value, bool>::type = true>
//__aicore__ inline void MmadWithSparse(const LocalTensor<T>& dst, const LocalTensor<U>& fm,
//    const LocalTensor<U>& filter, const MmadParams& mmadParams);
void HostTestMmadWithSparse() {
    AscendC::LocalTensor<int32_t> dst;
    AscendC::LocalTensor<int8_t> fm;
    AscendC::LocalTensor<int8_t> filter;
    AscendC::MmadParams mmadParams;
    mmadParams.m = 16;
    mmadParams.n = 32;
    mmadParams.k = 64;
    mmadParams.unitFlag = 0;
    AscendC::MmadWithSparse<int32_t, int8_t>(dst, fm, filter, mmadParams);
}

//template <typename T = int8_t, typename U = uint8_t,
//    typename Std::enable_if<Std::is_same<PrimT<T>, int8_t>::value, bool>::type = true,
//    typename Std::enable_if<Std::is_same<PrimT<U>, uint8_t>::value, bool>::type = true>
//__aicore__ inline void LoadDataWithSparse(const LocalTensor<T> &dst, const LocalTensor<T> &src,
//    const LocalTensor<U> &idx, const LoadData2dParams &loadDataParam);
void HostTestLoadDataWithSparse() {
    AscendC::LocalTensor<int8_t> dst;
    AscendC::LocalTensor<int8_t> src;
    AscendC::LocalTensor<uint8_t> idx;
    AscendC::LoadData2dParams loadDataParam(0, 8, 64, 0, 32, false, 0);
    AscendC::LoadDataWithSparse<int8_t, uint8_t>(dst, src, idx, loadDataParam);
}


//template <typename T = int8_t, typename Std::enable_if<Std::is_same<PrimT<T>, int8_t>::value, bool>::type = true>
//__aicore__ inline void LoadUnzipIndex(const GlobalTensor<T>& src, uint32_t numOfIndexTabEntry);
void HostTestLoadUnzipIndex() {
    AscendC::GlobalTensor<int8_t> src;
    uint32_t numOfIndexTabEntry = 256;
    AscendC::LoadUnzipIndex<int8_t>(src, numOfIndexTabEntry);
}


//__aicore__ inline void SetFmatrix(const SetFMatrixBitModeParams& param,
//    const FmatrixMode &fmatrixMode);
void HostTestSetFmatrixBitMode() {
    uint8_t padList[4] = {1, 1, 1, 1};
    AscendC::LoadData3DParamsV2<float> loadData3DParams(padList, 16, 32, 64, 128, 256, 0, 0, 1, 1, 3, 3, 1, 1, false, false, 0.0f);
    AscendC::SetFMatrixBitModeParams setFMatrixBitModeParams(loadData3DParams);
    AscendC::SetFmatrix(setFMatrixBitModeParams, AscendC::FmatrixMode::FMATRIX_LEFT);
}


//__aicore__ inline void SetLoadDataRepeatWithStride(const LoadDataRepeatParamWithStride& repeatParams);
void HostTestSetLoadDataRepeatWithStride() {
    AscendC::LoadDataRepeatParamWithStride repeatParams(64, 8, 0, 128);
    AscendC::SetLoadDataRepeatWithStride(repeatParams);
}



















