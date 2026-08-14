/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_ACLRTC_KERNEL_SPECIALIZATION_LOG_H
#define ASCENDC_ACLRTC_KERNEL_SPECIALIZATION_LOG_H

#if !defined(UT_TEST) && !defined(ST_TEST)
#include "dlog_pub.h"
#define ACLRTC_LOG_ERROR(format, ...) \
    dlog_error(static_cast<int32_t>(ASCENDCKERNEL), "[aclrtc-specialization] " format "\n", ##__VA_ARGS__)
#define ACLRTC_LOG_WARNING(format, ...) \
    dlog_warn(static_cast<int32_t>(ASCENDCKERNEL), "[aclrtc-specialization] " format "\n", ##__VA_ARGS__)
#define ACLRTC_LOG_INFO(format, ...) \
    dlog_info(static_cast<int32_t>(ASCENDCKERNEL), "[aclrtc-specialization] " format "\n", ##__VA_ARGS__)
#elif defined(UT_TEST)
namespace ascendc {
namespace aclrtc {
void RecordKernelSpecializationTestLog(const char* severity, const char* format, ...) noexcept;
} // namespace aclrtc
} // namespace ascendc
#define ACLRTC_LOG_ERROR(format, ...) \
    ::ascendc::aclrtc::RecordKernelSpecializationTestLog("ERROR", format, ##__VA_ARGS__)
#define ACLRTC_LOG_WARNING(format, ...) \
    ::ascendc::aclrtc::RecordKernelSpecializationTestLog("WARNING", format, ##__VA_ARGS__)
#define ACLRTC_LOG_INFO(format, ...) \
    ::ascendc::aclrtc::RecordKernelSpecializationTestLog("INFO", format, ##__VA_ARGS__)
#else
#define ACLRTC_LOG_ERROR(format, ...) ((void)0)
#define ACLRTC_LOG_WARNING(format, ...) ((void)0)
#define ACLRTC_LOG_INFO(format, ...) ((void)0)
#endif

#endif // ASCENDC_ACLRTC_KERNEL_SPECIALIZATION_LOG_H
