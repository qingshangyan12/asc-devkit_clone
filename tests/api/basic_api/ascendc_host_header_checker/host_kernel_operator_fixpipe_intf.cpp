/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_fixpipe_intf.h"

//template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
//__aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src,
//    const FixpipeParamsV220& intriParams);
void HostTestFixpipeL0CToL1() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::FixpipeParamsV220 intriParams;
    intriParams.cburstNum = 1;
    intriParams.burstLen = 64;
    AscendC::Fixpipe<float, float>(dst, src, intriParams);
}

//template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
//    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
//__aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src,
//    const LocalTensor<S>& cbufWorkspace, const FixpipeParamsV220& intriParams);
void HostTestFixpipeL0CToL1Deq() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::LocalTensor<uint64_t> cbufWorkspace;
    AscendC::FixpipeParamsV220 intriParams;
    intriParams.cburstNum = 1;
    intriParams.burstLen = 64;
    AscendC::Fixpipe<float, float>(dst, src, cbufWorkspace, intriParams);
}

//template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
//__aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src,
//    const FixpipeParamsV220& intriParams);
void HostTestFixpipeL0CToGM() {
    AscendC::GlobalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::FixpipeParamsV220 intriParams;
    intriParams.cburstNum = 1;
    intriParams.burstLen = 64;
    AscendC::Fixpipe<float, float>(dst, src, intriParams);
}

//template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
//    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
//__aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src,
//    const LocalTensor<S>& cbufWorkspace, const FixpipeParamsV220& intriParams);
void HostTestFixpipeL0CToGMDeq() {
    AscendC::GlobalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    AscendC::LocalTensor<uint64_t> cbufWorkspace;
    AscendC::FixpipeParamsV220 intriParams;
    intriParams.cburstNum = 1;
    intriParams.burstLen = 64;
    AscendC::Fixpipe<float, float>(dst, src, cbufWorkspace, intriParams);
}