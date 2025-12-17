/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_SET_L0C_COPY_PARAMS_IMPL_NPU_ARCH_2201_ASC_SET_L0C_COPY_PARAMS_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_CUBE_DATAMOVE_C_API_IMPL_SET_L0C_COPY_PARAMS_IMPL_NPU_ARCH_2201_ASC_SET_L0C_COPY_PARAMS_IMPL_H

namespace CApiInternal {

__aicore__ inline void set_l0c_copy_params_impl(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride)
{
    uint64_t config = 0;
    config = config | (static_cast<uint64_t>(ndNum));             // ND_PARA[15:0], nd number.
    config = config | (static_cast<uint64_t>(srcNdStride) << 16); // ND_PARA[31:16], src nd stride.
    config = config | (static_cast<uint64_t>(dstNdStride) << 32); // ND_PARA[47:32], dst nd stride.
    set_nd_para(config);
}
}

#endif