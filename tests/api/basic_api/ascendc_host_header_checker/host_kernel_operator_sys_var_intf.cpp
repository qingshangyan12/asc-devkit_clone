/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_sys_var_intf.h"


//template <SpecialPurposeReg spr>
//__aicore__ inline int64_t GetSpr();
void HostTest_GetSpr() {
    AscendC::GetSpr<AscendC::SpecialPurposeReg::SPR_PC>();
}


//template <SpecialPurposeReg spr>
//__aicore__ inline void ClearSpr();
void HostTest_ClearSpr() {
    AscendC::ClearSpr<AscendC::SpecialPurposeReg::SPR_PC>();
}


//__aicore__ inline constexpr uint32_t GetUBSizeInBytes();
void HostTest_GetUBSizeInBytes() {
    AscendC::GetUBSizeInBytes();
}


//__aicore__ inline constexpr uint32_t GetVecLen();
void HostTest_GetVecLen() {
    AscendC::GetVecLen();
}


//__aicore__ inline uint32_t GetRuntimeUBSize();
void HostTest_GetRuntimeUBSize() {
    AscendC::GetRuntimeUBSize();
}


//__aicore__ inline __ssbuf__ void* GetSsbufBaseAddr();
void HostTest_GetSsbufBaseAddr() {
    AscendC::GetSsbufBaseAddr();
}