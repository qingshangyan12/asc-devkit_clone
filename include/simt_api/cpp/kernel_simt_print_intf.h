/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef ASCENDC_MODULE_SIMT_PRINT_INTERFACE_H
#define ASCENDC_MODULE_SIMT_PRINT_INTERFACE_H

#include "simt_api/device_types.h"
#include "utils/debug/asc_printf.h"

#ifndef PRINTF
#define PRINTF printf;
#endif

namespace AscendC {
namespace Simt {

#ifndef ASCENDC_CPU_DEBUG
#if !defined(__CHECK_FEATURE_AT_PRECOMPILE) || (defined(__CHECK_FEATURE_AT_PRECOMPILE) && defined(__NPU_DEVICE__))
using __asc_simt_vf::printf;
#endif
#endif

}  // namespace Simt
}  // namespace AscendC

#endif  // ASCENDC_MODULE_SIMT_PRINT_INTERFACE_H
