/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_vec_unary_intf.h"

//template <typename T, typename U>
//__aicore__ inline void Abs(const LocalTensor<T>& dst, const LocalTensor<U>& src, const int32_t& count);
void HostTestAbsComplex() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<AscendC::complex32_t> src;
    int32_t count = 128;
    AscendC::Abs<float, AscendC::complex32_t>(dst, src, count);
}

//template <typename T>
//__aicore__ inline void Neg(const LocalTensor<T> &dst, const LocalTensor<T> &src,
//    const uint32_t count);
void HostTestNeg() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    uint32_t count = 128;
    AscendC::Neg<float>(dst, src, count);
}