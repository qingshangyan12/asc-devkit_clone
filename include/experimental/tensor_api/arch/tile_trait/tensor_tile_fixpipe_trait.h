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
 * \file tensor_tile_fixpipe_intf.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_ARCH_TENSOR_TILE_FIXPIPE_INTF_H
#define INCLUDE_TENSOR_API_ARCH_TENSOR_TILE_FIXPIPE_INTF_H

namespace AscendC {
struct FixpipeTrait {
    __aicore__ constexpr FixpipeTrait() {}

    __aicore__ constexpr FixpipeTrait(const QuantMode_t quantPreIn) : quantPre(quantPreIn) {}

    __aicore__ constexpr FixpipeTrait(
        const QuantMode_t quantPreIn,
        const bool enableReluIn,
        const bool enableChannleSplitIn,
        const uint8_t unitFlagIn,
        const uint8_t dualDstCtlIn
    ) :
        quantPre(quantPreIn),
        enableRelu(enableReluIn),
        enableChannleSplit(enableChannleSplitIn),
        unitFlag(unitFlagIn),
        dualDstCtl(dualDstCtlIn)
    {}

    QuantMode_t quantPre = QuantMode_t::NoQuant;
    bool enableRelu = false;
    bool enableChannleSplit = false;
    uint8_t unitFlag = false;
    uint8_t dualDstCtl = false;
};
constexpr FixpipeTrait DEFAULT_FIXPIPE_TRAIT;
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_ARCH_TENSOR_TILE_FIXPIPE_INTF_H