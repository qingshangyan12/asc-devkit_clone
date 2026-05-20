/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_atomic_intf.h"

//template <typename T>
//__aicore__ inline T AtomicAdd(__gm__ T *address, T value);
void HostTestAtomicAdd() {
    __gm__ float* address = nullptr;
    float value = 1.0f;
    AscendC::AtomicAdd<float>(address, value);
}

//template <typename T>
//__aicore__ inline T AtomicMax(__gm__ T *address, T value);
void HostTestAtomicMax() {
    __gm__ float* address = nullptr;
    float value = 1.0f;
    AscendC::AtomicMax<float>(address, value);
}

//template <typename T>
//__aicore__ inline T AtomicMin(__gm__ T *address, T value);
void HostTestAtomicMin() {
    __gm__ float* address = nullptr;
    float value = 1.0f;
    AscendC::AtomicMin<float>(address, value);
}

//template <typename T>
//__aicore__ inline T AtomicCas(__gm__ T *address, T value1, T value2);
void HostTestAtomicCas() {
    __gm__ float* address = nullptr;
    float value1 = 1.0f;
    float value2 = 2.0f;
    AscendC::AtomicCas<float>(address, value1, value2);
}

//template <typename T>
//__aicore__ inline T AtomicExch(__gm__ T *address, T value);
void HostTestAtomicExch() {
    __gm__ float* address = nullptr;
    float value = 1.0f;
    AscendC::AtomicExch<float>(address, value);
}