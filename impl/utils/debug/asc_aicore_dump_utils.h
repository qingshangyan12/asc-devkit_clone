/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// Architecture selection only; behavior belongs in the selected implementation.
#ifndef IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_UTILS_H
#define IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_UTILS_H

#if __NPU_ARCH__ == 2201
#include "impl/utils/debug/npu_arch_2201/asc_aicore_dump_utils.h"
#elif __NPU_ARCH__ == 3510
#include "impl/utils/debug/npu_arch_3510/asc_aicore_dump_utils.h"
#elif __NPU_ARCH__ == 9201
#include "impl/utils/debug/npu_arch_9201/asc_aicore_dump_utils.h"
#elif __NPU_ARCH__ == 9202
#include "impl/utils/debug/npu_arch_9202/asc_aicore_dump_utils.h"
#elif __NPU_ARCH__ == 5102
#include "impl/utils/debug/npu_arch_5102/asc_aicore_dump_utils.h"
#elif __NPU_ARCH__ == 5162
#include "impl/utils/debug/npu_arch_5162/asc_aicore_dump_utils.h"
#endif

#endif
