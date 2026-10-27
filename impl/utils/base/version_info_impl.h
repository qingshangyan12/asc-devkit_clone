/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_UTILS_BASE_VERSION_INFO_IMPL_H
#define IMPL_UTILS_BASE_VERSION_INFO_IMPL_H

namespace __asc_detail {
__aicore__ inline void ReadDevkitVersion(__gm__ char*& versionStr, uint64_t& version, uint64_t& timeStamp)
{
#ifdef ASC_DEVKIT_VERSION_STR
    versionStr = const_cast<__gm__ char*>(ASC_DEVKIT_VERSION_STR);
#else
    versionStr = const_cast<__gm__ char*>("Unknown ASC_DEVKIT version");
#endif
#ifdef ASC_DEVKIT_TIMESTAMP
    timeStamp = static_cast<uint64_t>(ASC_DEVKIT_TIMESTAMP);
#else
    timeStamp = 0;
#endif
#ifdef ASC_DEVKIT_VERSION
    version = static_cast<uint64_t>(ASC_DEVKIT_VERSION);
#else
    version = 0;
#endif
}
} // namespace __asc_detail
#endif
