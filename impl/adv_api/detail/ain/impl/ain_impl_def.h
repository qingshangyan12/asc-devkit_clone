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
 * \file ain_impl_def.h
 * \brief Ain implementation definition
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message(                                                                                             \
    "impl/adv_api/detail/ain/impl/ain_impl_def.h is an internal header file and must not be used directly. " \
    "Please use public interface headers.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_DEF_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_DEF_H
#define IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_DEF_H

#include <cstddef>
#include <cstdint>
#include "../../../../basic_api/kernel_utils.h"

namespace AscendC {

template <CommProtocol commProtocol>
class Ain;

template <CommProtocol commProtocol>
class AinBarrierSessionImpl {
public:
    Ain<commProtocol>* ain;
    AinTeam team;    // barrier 作用域
    uint32_t index;   // barrier资源索引
    AinSignal signal;  // barrier资源的起始signal index

    template<bool EnableTimeout, typename DescriptorUbuf>
    AIN_DEVICE int32_t syncImpl(AinMemoryOrder order, uint64_t timeoutCycles, const DescriptorUbuf& ubuf);
};

}  // namespace AscendC

#endif  // IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_DEF_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_DEF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_DEF_H__
#endif
