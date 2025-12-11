/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_INSTR_VECTOR_COMPUTE_CLEAR_NTHBIT_ASC_2201_CLEAR_NTHBIT_IMPL_H
#define IMPL_C_API_INSTR_VECTOR_COMPUTE_CLEAR_NTHBIT_ASC_2201_CLEAR_NTHBIT_IMPL_H

namespace CApiInternal {

__aicore__ inline uint64_t clear_nthbit_impl(uint64_t bits, int64_t idx)
{
    sbitset0(bits, idx);
}

} // namespace CApiInternal

#endif