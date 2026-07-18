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
 * \file ain_common.h
 * \brief Ain common definitions
 */
#ifndef INCLUDE_ADV_API_AIN_AIN_COMMON_H
#define INCLUDE_ADV_API_AIN_AIN_COMMON_H

#include <cstddef>
#include <cstdint>

#ifndef AIN_DEVICE
/*!
 * @brief Force-inline qualifier for Ain device-side member functions.
 *        All Ain methods run on AICORE and are always inlined.
 */
#define AIN_DEVICE __attribute__((always_inline)) __aicore__ __inline__
#endif

#define AIN_MASK_ALL 0u

namespace AscendC {

typedef uint32_t AinSignal;

using AinCommSymWindow = __gm__ void*;
using AinTeam = __gm__ void*;

/*!
 * @brief Placeholder tag for "no remote action" in the put primitive.
 */
typedef struct {
} AinRemoteNone;

/*!
 * @brief Placeholder tag for "no local action" in the put primitive.
 */
typedef struct {
} AinLocalNone;

/*!
 * @brief Placeholder tag indicating that no valid UB descriptor was supplied.
 *        Passing this type to put/get is rejected at compile time.
 */
typedef struct {
} AinDescriptorUbufNone;

/*!
 * @brief Describes the UB workspace handed to the underlying Hcomm engine.
 */
typedef struct {
    __ubuf__ uint8_t* addr; /*!< Start address of the UB buffer. */
    uint32_t bytes;         /*!< Length of the UB buffer in bytes. */
    uint32_t eventId;       /*!< Event id reserved for synchronization with the comm engine. */
} AinDescriptorUbuf;

/*!
 * @brief Commit behavior for put/get tasks.
 */
enum AinCommitFlags {
    AIN_COMMIT_IMMED = 0,         /*!< Assemble the WQE and ring the doorbell immediately. */
    AIN_COMMIT_DELAYED = (1 << 0) /*!< Assemble the WQE only; caller drains later through flush(). */
};

enum AinMemoryOrder { AIN_MEMORY_ORDER_DEFAULT };

typedef struct {
    AinSignal signalId;
    uint64_t value;
} AinSignalAdd;

typedef struct {
    AinSignal signalId;
} AinSignalInc;

typedef struct {
    AinCommSymWindow signalWindow;
    AinSignal signalOffset;
    uint64_t value;
} AinVASignalAdd;

typedef struct {
    AinCommSymWindow signalWindow;
    AinSignal signalOffset;
} AinVASignalInc;

} // namespace AscendC

#endif // INCLUDE_ADV_API_AIN_AIN_COMMON_H
