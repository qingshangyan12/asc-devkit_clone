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
 * \file copy_ubuf_to_gm_impl.h
 * \brief
 */
#ifndef IMPL_INSTR_VECTOR_DATAMOVE_COPY_UBUF_TO_GM_IMPL_ASC_2201_COPY_UBUF_TO_GM_IMPL_H
#define IMPL_INSTR_VECTOR_DATAMOVE_COPY_UBUF_TO_GM_IMPL_ASC_2201_COPY_UBUF_TO_GM_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

namespace CApiInternal {

__aicore__ inline void asc_copy_ub2gm_impl(__gm__ void* dst, __ubuf__ void* src, const asc_copy_config& config)
{
#if ((__NPU_ARCH__ == 2201) && (C_API_CV_SPLIT_CORE_TYPE == C_API_AIV_TYPE))
    copy_ubuf_to_gm(dst, src, config.config);
#endif
}

__aicore__ inline void asc_copy_ub2gm_impl(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_config config;
    config.sid = 0;
    config.n_burst = 1;
    config.burst_len = size / CAPI_ONE_DATABLOCK_SIZE;
    config.src_gap = 0;
    config.dst_gap = 0;
    asc_copy_ub2gm_impl(dst, src, config);
}

__aicore__ inline void asc_copy_ub2gm_sync_impl(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2gm_impl(dst, src, size);
    asc_sync_impl();
}

}

#endif