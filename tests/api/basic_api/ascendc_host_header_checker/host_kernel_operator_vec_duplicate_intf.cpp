/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_vec_duplicate_intf.h"

//template <typename T>
//__aicore__ inline void Duplicate(const LocalTensor<T>& dst, const LocalTensor<T>& src,
//    const int32_t& count);
void HostTestDuplicateSrc() {
    AscendC::LocalTensor<float> dst;
    AscendC::LocalTensor<float> src;
    int32_t count = 128;
    AscendC::Duplicate<float>(dst, src, count);
}

//template <typename T>
//__aicore__ inline void Interleave(const LocalTensor<T>& dst0, const LocalTensor<T>& dst1,
//    const LocalTensor<T>& src0, const LocalTensor<T>& src1, const int32_t count);
void HostTestInterleave() {
    AscendC::LocalTensor<float> dst0;
    AscendC::LocalTensor<float> dst1;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    int32_t count = 128;
    AscendC::Interleave<float>(dst0, dst1, src0, src1, count);
}

//template <typename T>
//__aicore__ inline void DeInterleave(const LocalTensor<T>& dst0, const LocalTensor<T>& dst1,
//    const LocalTensor<T>& src0, const LocalTensor<T>& src1, const int32_t count);
void HostTestDeInterleave1() {
    AscendC::LocalTensor<float> dst0;
    AscendC::LocalTensor<float> dst1;
    AscendC::LocalTensor<float> src0;
    AscendC::LocalTensor<float> src1;
    int32_t count = 128;
    AscendC::DeInterleave<float>(dst0, dst1, src0, src1, count);
}

//template <typename T>
//__aicore__ inline void DeInterleave(const LocalTensor<T>& dst0, const LocalTensor<T>& dst1,
//    const LocalTensor<T>& src, const int32_t srcCount);
void HostTestDeInterleave2() {
    AscendC::LocalTensor<float> dst0;
    AscendC::LocalTensor<float> dst1;
    AscendC::LocalTensor<float> src;
    int32_t srcCount = 128;
    AscendC::DeInterleave<float>(dst0, dst1, src, srcCount);
}