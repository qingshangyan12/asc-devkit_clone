/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_vec_binary_intf.h"


//template <typename T, typename U>
//__aicore__ inline void ShiftLeft(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
//    const LocalTensor<U>& src1, const int32_t& count);
void HostTest_ShiftLeft() {
    AscendC::LocalTensor<int32_t> dst;
    AscendC::LocalTensor<int32_t> src0;
    AscendC::LocalTensor<int32_t> src1;
    int32_t count = 256;
    AscendC::ShiftLeft<int32_t, int32_t>(dst, src0, src1, count);
}


//template <typename T, typename U>
//__aicore__ inline void ShiftRight(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
//    const LocalTensor<U>& src1, const int32_t& count);
void HostTest_ShiftRight() {
    AscendC::LocalTensor<int32_t> dst;
    AscendC::LocalTensor<int32_t> src0;
    AscendC::LocalTensor<int32_t> src1;
    int32_t count = 256;
    AscendC::ShiftRight<int32_t, int32_t>(dst, src0, src1, count);
}


//template <typename T>
//__aicore__ inline void Prelu(const LocalTensor<T>& dst, const LocalTensor<T> &src0, 
//    const LocalTensor<T> &src1, const uint32_t count);
void HostTest_Prelu() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint32_t count = 256;
    AscendC::Prelu<float>(dst, src0, src1, count);
}


//template <typename T>
//__aicore__ inline void Mull(const LocalTensor<T>& dst0, const LocalTensor<T>& dst1,
//    const LocalTensor<T>& src0, const LocalTensor<T>& src1, const uint32_t count);
void HostTest_Mull() {
    AscendC::LocalTensor<float> dst0;
    AscendC::LocalTensor<float> dst1;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint32_t count = 256;
    AscendC::Mull<float>(dst0, dst1, src0, src1, count);
}


//template <typename T>
//__aicore__ inline void AbsSub(const LocalTensor<T> &dst, const LocalTensor<T> &src0, 
//    const LocalTensor<T> &src1, const uint32_t count);
void HostTest_AbsSub() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint32_t count = 256;
    AscendC::AbsSub<float>(dst, src0, src1, count);
}


//template <typename T>
//__aicore__ inline void FusedAbsSub(const LocalTensor<T> &dst, const LocalTensor<T> &src0, 
//    const LocalTensor<T> &src1, const uint32_t count);
void HostTest_FusedAbsSub() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint32_t count = 256;
    AscendC::FusedAbsSub<float>(dst, src0, src1, count);
}


//template <typename T, typename U>
//__aicore__ inline void ExpSub(const LocalTensor<T> &dst, const LocalTensor<U> &src0, 
//    const LocalTensor<U> &src1, const uint32_t count);
void HostTest_ExpSub() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint32_t count = 256;
    AscendC::ExpSub<float, float>(dst, src0, src1, count);
}


//template <typename T, typename U>
//__aicore__ inline void FusedExpSub(const LocalTensor<T> &dst, const LocalTensor<U> &src0, 
//    const LocalTensor<U> &src1, const uint32_t count);
void HostTest_FusedExpSub() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    uint32_t count = 256;
    AscendC::FusedExpSub<float, float>(dst, src0, src1, count);
}