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
* \file cache.h
* \brief
*/
#ifndef INCLUDE_TENSOR_API_UTILS_CACHE_H
#define INCLUDE_TENSOR_API_UTILS_CACHE_H

#include "include/utils/base/sys_macros.h"
#include "include/utils/base/sys_constants.h"
#include "include/utils/common_types.h"

namespace AscendC {
namespace Te {

enum class CacheMode {
    CACHE_MODE_DISABLE = 0,
    CACHE_MODE_NORMAL = 1,
    CACHE_MODE_LAST = 2,
    CACHE_MODE_PERSISTENT = 4
};

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_UTILS_CACHE_H
