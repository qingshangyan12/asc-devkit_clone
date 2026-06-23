/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file barrier.h
 * \brief Ain device interface
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_H__
#endif

#ifndef INCLUDE_ADV_API_AIN_BARRIER_H
#define INCLUDE_ADV_API_AIN_BARRIER_H

#include "adv_api/hcomm/hcomm.h"
#include "ain.h"
#include "../../../impl/adv_api/detail/ain/impl/barrier_impl_def.h"

namespace ain {
namespace device {

struct AinBarrierSession : AinBarrierSession_internal{
    AIN_DEVICE AinBarrierSession(Ain& ain, HcclTeam team, HcclAinBarrierHandle handle, uint32_t index);  
    AIN_DEVICE AinBarrierSession(Ain& ain, HcclTeamTagWorld, uint32_t index);

    // AIN_DEVICE AinBarrierSession(Ain& ain, void* team, HcclAinBarrierHandle handle, uint32_t index);  
    
    // handle不传，从ain的资源里取 -> 怎么区别不同类型的Barrier
    // AIN_DEVICE AinBarrierSession(Ain& ain, HcclTeam team, uint32_t index);
    AIN_DEVICE void sync(AinMemoryOrder order);
    AIN_DEVICE HcclResult sync(AinMemoryOrder order, uint64_t timeoutCycles);
};

}  // namespace device
}  // namespace ain

#include "../../../impl/adv_api/detail/ain/impl/barrier_impl.h"
#endif // AIN_BARRIER_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_H__
#endif
