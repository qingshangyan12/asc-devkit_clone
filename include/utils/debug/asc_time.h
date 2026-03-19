/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

<<<<<<< HEAD
<<<<<<< HEAD:include/utils/debug/asc_time.h
=======
>>>>>>> upstream/master
/*!
 * \file asc_time.h
 * \brief
 */
#ifndef INCLUDE_UTILS_DEBUG_ASC_TIME_H
#define INCLUDE_UTILS_DEBUG_ASC_TIME_H

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_COMPILER_INTERNAL_HEADERS_ASC_TIME_H__
#endif

#include "simt_api/device_types.h"

<<<<<<< HEAD
=======
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
>>>>>>> upstream/master
namespace __asc_simt_vf {
__SIMT_DEVICE_FUNCTIONS_DECL__ inline uint64_t clock(void);
} // namespace __asc_simt_vf

#include "impl/utils/debug/asc_time_simt_impl.h"
<<<<<<< HEAD

#if defined(__UNDEF_ASCENDC_INCLUDE_COMPILER_INTERNAL_HEADERS_ASC_TIME_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_COMPILER_INTERNAL_HEADERS_ASC_TIME_H__
#endif

#endif
=======
/* !
 * \file kernel_reg_compute_membar_intf.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_MEMBAR_INTF_H__
#endif

#ifndef ASCENDC_MODULE_REG_COMPUTE_MEMBAR_INTERFACE_H
#define ASCENDC_MODULE_REG_COMPUTE_MEMBAR_INTERFACE_H

#include "kernel_reg_compute_utils.h"

namespace AscendC {
namespace Reg {

template <MemType src, MemType dst>
__simd_callee__ inline void LocalMemBar();

} // namespace Reg
} // namespace AscendC

#include "../../impl/basic_api/reg_compute/kernel_reg_compute_membar_intf_impl.h"
#endif // ASCENDC_MODULE_REG_COMPUTE_MEMBAR_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_MEMBAR_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_MEMBAR_INTF_H__
#endif

>>>>>>> upstream/master:include/basic_api/reg_compute/kernel_reg_compute_membar_intf.h
=======
#endif

#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510)
#include "impl/utils/debug/asc_aicore_time_impl.h"
#endif

namespace __asc_aicore {
__aicore__ inline void asc_time_stamp(uint32_t desc_id);

__aicore__ inline void asc_prof_start();

__aicore__ inline void asc_prof_stop();

#if __NPU_ARCH__ == 3510
template<pipe_t pipe = PIPE_S>
__aicore__ inline void asc_mark_stamp(uint16_t idx);

template<pipe_t pipe = PIPE_S, uint16_t idx>
__aicore__ inline void asc_mark_stamp();
#endif
}   // namespace __asc_aicore

#define TRACE_START(...) TRACE_START_IMPL(__VA_ARGS__)
#define TRACE_STOP(...) TRACE_STOP_IMPL(__VA_ARGS__)

#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_COMPILER_INTERNAL_HEADERS_ASC_TIME_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_COMPILER_INTERNAL_HEADERS_ASC_TIME_H__
#endif

#endif
>>>>>>> upstream/master
