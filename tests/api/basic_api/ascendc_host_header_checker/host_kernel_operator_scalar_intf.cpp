/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_scalar_intf.h"

//template <typename T>
//__aicore__ inline void GetUintDivMagicAndShift(T& magic, T& shift, T divisor);
void HostTestGetUintDivMagicAndShift() {
    uint32_t magic = 0;
    uint32_t shift = 0;
    uint32_t divisor = 10;
    AscendC::GetUintDivMagicAndShift<uint32_t>(magic, shift, divisor);
}

//template <typename T>
//__aicore__ inline void WriteGmByPassDCache(__gm__ T* addr, T value);
void HostTestWriteGmByPassDCache() {
    __gm__ float* addr = nullptr;
    float value = 1.0f;
    AscendC::WriteGmByPassDCache<float>(addr, value);
}

//template <typename T>
//__aicore__ inline T ReadGmByPassDCache(__gm__ T* addr);
void HostTestReadGmByPassDCache() {
    __gm__ float* addr = nullptr;
    AscendC::ReadGmByPassDCache<float>(addr);
}