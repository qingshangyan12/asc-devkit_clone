/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_C_API_INSTR_IMPL_CUBE_COMPUTE_C_API_IMPL_SET_L1_3D_PADDING_VALUE_IMPL_NPU_ARCH_2201_ASC_SET_L1_3D_PADDING_VALUE_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_CUBE_COMPUTE_C_API_IMPL_SET_L1_3D_PADDING_VALUE_IMPL_NPU_ARCH_2201_ASC_SET_L1_3D_PADDING_VALUE_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_set_l1_3d_padding_value_impl(uint64_t config)
{
    if ASC_IS_AIC {
        uint64_t paddingValue = 0;
        uint64_t padValueShiftBit = 8;
        paddingValue = (config << padValueShiftBit) | (config & 0xFF);
        set_padding(paddingValue);
    }
}

__aicore__ inline void asc_set_l1_3d_padding_value_impl(half config)
{
    if ASC_IS_AIC {
        uint64_t paddingValue = 0;
        uint64_t padValueShiftBit = 8;
        asc_scalar_bitcode_half bitcode;
        bitcode.input=config;
        set_padding(bitcode.config);
    }
}

__aicore__ inline void asc_set_l1_3d_padding_value_impl(int16_t config)
{
    if ASC_IS_AIC {
        uint64_t paddingValue = 0;
        uint64_t padValueShiftBit = 8;
        asc_scalar_bitcode_int16_t bitcode;
        bitcode.input=config;
        set_padding(bitcode.config);
    }
}

__aicore__ inline void asc_set_l1_3d_padding_value_impl(uint16_t config)
{
    if ASC_IS_AIC {
        uint64_t paddingValue = 0;
        uint64_t padValueShiftBit = 8;
        asc_scalar_bitcode_uint16_t bitcode;
        bitcode.input=config;
        set_padding(bitcode.config);
    }
}

#endif