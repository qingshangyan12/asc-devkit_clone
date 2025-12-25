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
 * \file device_sync_functions_impl.h
 * \brief
 */
#ifndef IMPL_SIMT_API_DEVICE_SYNC_FUNCTIONS_IMPL_H
#define IMPL_SIMT_API_DEVICE_SYNC_FUNCTIONS_IMPL_H

__simt_callee__ inline int32_t asc_all(int32_t predicate)
{
    return bisheng::cce::simt::__all(predicate);
}

__simt_callee__ inline int32_t asc_any(int32_t predicate)
{
    return bisheng::cce::simt::__any(predicate);
}

__simt_callee__ inline uint32_t asc_ballot(int32_t predicate)
{
    return bisheng::cce::simt::__ballot(predicate);
}

__simt_callee__ inline uint32_t asc_activemask()
{
    return bisheng::cce::simt::__activemask();
}

__simt_callee__ inline void asc_syncthreads()
{
    __sync_workitems();
}

__simt_callee__ inline void asc_threadfence()
{
    bisheng::cce::simt::__threadfence();
}
#endif  // IMPL_SIMT_API_DEVICE_SYNC_FUNCTIONS_IMPL_H