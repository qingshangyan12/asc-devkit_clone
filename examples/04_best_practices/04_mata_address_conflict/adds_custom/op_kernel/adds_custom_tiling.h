/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under
 * the terms and conditions of CANN Open Software License Agreement Version 2.0
 * (the "License"). Please refer to the License for details. You may not use
 * this file except in compliance with the License. THIS SOFTWARE IS PROVIDED ON
 * AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS
 * FOR A PARTICULAR PURPOSE. See LICENSE in the root of the software repository
 * for the full text of the License.
 */

/* !
 * \file adds_custom_tiling.h
 * \brief
 */

#ifndef ADDS_CUSTOM_TILING_H
#define ADDS_CUSTOM_TILING_H
#include <cstdint>

class AddsCustomTilingData {
public:
    uint32_t m;
    uint32_t n;
    uint32_t tileM;
    uint32_t tileN;
    uint32_t loopOneCore;
};
#endif // ADDS_CUSTOM_TILING_H
