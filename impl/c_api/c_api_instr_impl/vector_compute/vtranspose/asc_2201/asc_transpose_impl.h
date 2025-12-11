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
 * \file subs_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_COMPUTE_VTRANSPOSE_ASC_2201_ASC_TRANSPOSE_IMPL_H
#define IMPL_INSTR_VECTOR_COMPUTE_VTRANSPOSE_ASC_2201_ASC_TRANSPOSE_IMPL_H

#include "c_api_interf_util.h"

namespace CApiInternal {

template<typename T>
__aicore__ inline void asc_transpose_impl(__ubuf__ T* dst, __ubuf__ T* src)
{
    vtranspose(dst, src);
}

template<typename T>
__aicore__ inline void asc_transpose_sync_impl(__ubuf__ T* dst, __ubuf__ T* src)
{
    asc_transpose_impl<T>(dst, src);
}

}

#endif