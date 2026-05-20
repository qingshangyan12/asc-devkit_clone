/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You can not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_data_copy_intf.h"

//template <typename T, bool enableSmallC0 = false>
//__aicore__ inline __inout_pipe__(MTE2) void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src,
//    const Dn2NzParams& intriParams);
void HostTestDataCopyDn2Nz() {
    AscendC::LocalTensor<float> dst;
    AscendC::GlobalTensor<float> src;
    AscendC::Dn2NzParams intriParams(1, 16, 32, 1024, 64, 8, 16, 512);
    AscendC::DataCopy(dst, src, intriParams);
}

//template <typename T, bool isSetMask = true>
//__aicore__ inline __inout_pipe__(V) void Copy(const LocalTensor<T> &dst, const LocalTensor<T> &src,
//    const uint32_t count);
void HostTestCopyCount() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    uint32_t count = 128;
    AscendC::Copy<float>(dst, src, count);
}

//template <typename T, typename U,
//          typename Std::enable_if<Std::is_same<PrimT<T>, bfloat16_t>::value && Std::is_same<PrimT<U>, float>::value,
//                                  bool>::type = true>
//__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<U>& src,
//                                const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams);
void HostTestDataCopyFloatToBfloat16() {
    AscendC::LocalTensor<AscendC::bfloat16_t> dst;
    AscendC::LocalTensor<float> src;
    AscendC::DataCopyParams intriParams;
    intriParams.blockCount = 1;
    intriParams.blockLen = 64;
    AscendC::DataCopyEnhancedParams enhancedParams;
    enhancedParams.enhancedParamsMode = AscendC::EnhancedParamsMode::ENHANCED_FLOAT16_TO_BFLOAT16;
    AscendC::DataCopy(dst, src, intriParams, enhancedParams);
}


//template <typename T, uint8_t dim, const NdDmaConfig &config = kDefaultNdDmaConfig>
//__aicore__ inline void DataCopy(const LocalTensor<T> &dst, const GlobalTensor<T> &src,
//    const MultiCopyParams<T, dim> &params);
void HostTestDataCopyMultiCopyParams() {
    AscendC::LocalTensor<float> dst;
    AscendC::GlobalTensor<float> src;
    AscendC::MultiCopyParams<float, 2> params;
    params.loopInfo.loopSrcStride[0] = 64;
    params.loopInfo.loopSrcStride[1] = 1024;
    params.loopInfo.loopDstStride[0] = 32;
    params.loopInfo.loopDstStride[1] = 512;
    params.loopInfo.loopSize[0] = 16;
    params.loopInfo.loopSize[1] = 8;
    params.constantValue = 0.0f;
    AscendC::DataCopy(dst, src, params);
}

//__aicore__ inline void NdDmaDci();
void HostTestNdDmaDci() {
    AscendC::NdDmaDci();
}

//__aicore__ inline void SetLoopModePara(const LoopModeParams& loopParams, DataCopyMVType type);
void HostTestSetLoopModePara() {
    AscendC::LoopModeParams loopParams(2, 2, 80, 128, 160, 288);
    AscendC::SetLoopModePara(loopParams, AscendC::DataCopyMVType::OUT_TO_UB);
}

//__aicore__ inline void ResetLoopModePara(DataCopyMVType type);
void HostTestResetLoopModePara() {
    AscendC::ResetLoopModePara(AscendC::DataCopyMVType::OUT_TO_UB);
}





