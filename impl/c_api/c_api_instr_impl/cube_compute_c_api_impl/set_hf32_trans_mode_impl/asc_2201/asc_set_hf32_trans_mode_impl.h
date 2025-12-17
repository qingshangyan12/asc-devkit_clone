/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_set_hf32_trans_mode_impl.h
 * \brief
 */
#ifndef IMPL_C_API_INSTR_CUBE_COMPUTE_SET_CTRL_ASC_2201_ASC_SET_HF32_TRANS_MODE_IMPL_H
#define IMPL_C_API_INSTR_CUBE_COMPUTE_SET_CTRL_ASC_2201_ASC_SET_HF32_TRANS_MODE_IMPL_H

namespace CApiInternal {

__aicore__ inline void asc_set_hf32_trans_mode_impl(bool hf32_transmode)
{
    constexpr int32_t hf32_trans_mode_bit=47;
    if(hf32_transmode){
        set_ctrl(sbitset1(get_ctrl(), hf32_trans_mode_bit));
    }else{
        set_ctrl(sbitset0(get_ctrl(), hf32_trans_mode_bit));
    }
}

} // namespace CApiInternal

#endif