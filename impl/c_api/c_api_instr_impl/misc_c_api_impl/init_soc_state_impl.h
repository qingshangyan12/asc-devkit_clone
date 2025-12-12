/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
 
#ifndef IMPL_C_API_C_API_INSTR_IMPL_MISC_C_API_IMPL_INIT_SOC_STATE_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_MISC_C_API_IMPL_INIT_SOC_STATE_IMPL_H
 
namespace CApiInternal {
 
__aicore__ inline void asc_init_soc_state_impl()
{
    set_atomic_none();
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    set_mask_norm();
    set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
#endif
}
 
} // namespace CApiInternal
 
#endif