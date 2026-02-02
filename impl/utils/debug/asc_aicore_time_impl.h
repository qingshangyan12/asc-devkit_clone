/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file asc_aicore_time_impl.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_AICORE_TIME_IMPL_H
#define IMPL_UTILS_DEBUG_ASC_AICORE_TIME_IMPL_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_TIME_IMPL__
#warning "asc_aicore_time_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future."
#endif

#include "impl/utils/debug/asc_debug_types.h"
#include "impl/utils/debug/asc_debug_utils.h"

#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3501 || __NPU_ARCH__ == 3101
namespace __asc_aicore {
__aicore__ inline void asc_time_stamp_impl(uint32_t desc_id)
{
    __gm__ DebugBlockHeadInfo* block_info = get_block_info();
    if (block_info == nullptr) {
        return;
    }
    uint32_t tlv_len = sizeof(TimeStampTlv);
    if (!check_ringbuf_space(block_info, tlv_len)) {
        return;
    }

    __gm__ TimeStampTlv* time_stamp_tlv = reinterpret_cast<__gm__ TimeStampTlv*>(get_ringbuf_tlv_addr(block_info));
    time_stamp_tlv->type = static_cast<uint32_t>(DumpType::DUMP_TIME_STAMP);
    time_stamp_tlv->length = tlv_len - sizeof(uint32_t[2]);
    time_stamp_tlv->descId = desc_id;
    time_stamp_tlv->blockIdx = static_cast<uint16_t>(asc_get_block_idx());
    time_stamp_tlv->resv = static_cast<uint16_t>(0U);
    time_stamp_tlv->cycle = asc_get_system_cycle();
    time_stamp_tlv->pc = static_cast<uint64_t>(asc_get_program_counter());
    time_stamp_tlv->entry = static_cast<uint64_t>(__get_entry_sys_cnt()); 
    time_stamp_tlv->resvMem[0] = static_cast<uint32_t>(0U);
    time_stamp_tlv->resvMem[1] = static_cast<uint32_t>(0U);
    asc_entire_dcci((__gm__ uint64_t*)time_stamp_tlv);

    __gm__ DebugBlockWriteInfo* write_info = get_block_write_info(block_info);

    update_write_info(write_info, tlv_len);
}

__aicore__ inline void asc_time_stamp(uint32_t desc_id)
{
    enable_asc_diagnostics();
    asc_time_stamp_impl(desc_id);
}

__aicore__ inline void asc_prof_start()
{
    bisheng::cce::metrics_prof_start();
}

__aicore__ inline void asc_prof_stop()
{
    bisheng::cce::metrics_prof_stop();
}

#if __NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3501
template<pipe_t pipe = PIPE_S>
__aicore__ inline void asc_mark_stamp(uint16_t idx)
{
    bisheng::cce::mark_stamp<pipe, index>();
}

template<pipe_t pipe = PIPE_S, uint16_t idx>
__aicore__ inline void asc_mark_stamp()
{
    bisheng::cce::mark_stamp<pipe, index>();
}
#endif
} // namespace __asc_aicore
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_TIME_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_TIME_IMPL__
#endif

#endif // IMPL_UTILS_DEBUG_ASC_AICORE_TIME_IMPL_H