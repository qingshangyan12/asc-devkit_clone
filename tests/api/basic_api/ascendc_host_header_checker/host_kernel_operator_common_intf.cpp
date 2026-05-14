/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_common_intf.h"


//template <int8_t startBit, int8_t endBit>
//__aicore__ static inline void SetCtrlSpr(int64_t value);
void HostTest_SetCtrlSpr() {
    int64_t value = 1;
    AscendC::SetCtrlSpr<0, 7>(value);
}


//template <int8_t startBit, int8_t endBit>
//__aicore__ static inline int64_t GetCtrlSpr();
void HostTest_GetCtrlSpr() {
    AscendC::GetCtrlSpr<0, 7>();
}


//template <int8_t startBit, int8_t endBit>
//__aicore__ static inline void ResetCtrlSpr();
void HostTest_ResetCtrlSpr() {
    AscendC::ResetCtrlSpr<0, 7>();
}