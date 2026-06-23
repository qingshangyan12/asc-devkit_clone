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
 * \file barrier_impl_def.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/ain/impl/barrier_impl_def.h is an internal header file and must not be used directly. " \
    "Please use public interface headers.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_IMPL_DEF_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_AIN_IMPL_BARRIER_IMPL_DEF_H
#define IMPL_ADV_API_DETAIL_AIN_IMPL_BARRIER_IMPL_DEF_H

#include "temp_hccl_def.h"  // TODO

namespace ain {
namespace device {

struct AinBarrierSession_internal {
    Ain ain;   // TDOO
    HcclTeam team;    // barrier 作用域
    // void* team;
    HcclAinBarrierHandle handle;
    // void* handle;
    int index;   // barrier资源索引      signal0 + index * nranks
    HcclAinSignal_t signal;

    template<bool EnableTimeout>
    AIN_DEVICE HcclResult syncInternal(AinMemoryOrder order, uint64_t timeoutCycles);
};

}  // namespace device
}  // namespace ain

#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_IMPL_DEF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BARRIER_IMPL_DEF_H__
#endif