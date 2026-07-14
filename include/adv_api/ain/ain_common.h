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
 *        All Ain/AinBarrierSession methods run on the AICORE and are always inlined.
 */
#define AIN_DEVICE __attribute__((always_inline)) __aicore__ __inline__
#endif
#define AIN_MASK_ALL 0u

namespace AscendC {
using AinDevComm = __gm__ void *;
using AinCommSymWindow = void *;
using AinTeam = void *;

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
 * @param addr: Start address of the UB buffer.
 * @param bytes: Length of the UB buffer in bytes.
 * @param eventId: Event id reserved for synchronization with the comm engine.
 */
typedef struct {
    __ubuf__ uint8_t *addr;
    uint32_t bytes;
    uint32_t eventId;
} AinDescriptorUbuf;

typedef uint32_t AinSignal;
typedef struct {
    AinSignal signalId;
    uint64_t value;
} AinSignalAdd;

typedef struct {
    AinSignal signalId;
} AinSignalInc;

/*!
 * @brief Commit behaviour for put/get tasks.
 */
enum AinCommitFlags {
    AIN_COMMIT_IMMED   = 0,        ///< Default: assemble the WQE and ring the doorbell immediately.
    AIN_COMMIT_DELAYED = (1 << 0)  ///< Assemble the WQE only; ringing the doorbell when commit AIN_COMMIT_IMMED.
};

enum AinMemoryOrder {
    AIN_MEMORY_ORDER_DEFAULT
};

constexpr uint32_t AIN_URMA_ODR_SHIFT = 0U;
constexpr uint32_t AIN_URMA_FENCE_SHIFT = 3U;
constexpr uint32_t AIN_URMA_SE_SHIFT = 4U;
constexpr uint32_t AIN_URMA_CQE_SHIFT = 5U;
constexpr uint32_t AIN_URMA_INLINE_EN_SHIFT = 6U;

// 默认配置为AIN_URMA_ODR_STRONG | AIN_URMA_FENCE_ON | AIN_URMA_SE_OFF | AIN_URMA_CQE_ON | AIN_URMA_INLINE_EN_OFF
constexpr uint32_t AIN_URMA_DEFAULT_CONFIG = (5U << AIN_URMA_ODR_SHIFT) | (1U << AIN_URMA_FENCE_SHIFT) |
                                             (0U << AIN_URMA_SE_SHIFT) | (1U << AIN_URMA_CQE_SHIFT) |
                                             (0U << AIN_URMA_INLINE_EN_SHIFT);

constexpr uint32_t AIN_URMA_ODR_UNORDERED = 0U << AIN_URMA_ODR_SHIFT;
constexpr uint32_t AIN_URMA_ODR_RELAXED = 3U << AIN_URMA_ODR_SHIFT;
constexpr uint32_t AIN_URMA_ODR_STRONG = 5U << AIN_URMA_ODR_SHIFT;
constexpr uint32_t AIN_URMA_FENCE_OFF = 0U << AIN_URMA_FENCE_SHIFT;
constexpr uint32_t AIN_URMA_FENCE_ON = 1U << AIN_URMA_FENCE_SHIFT;
constexpr uint32_t AIN_URMA_SE_OFF = 0U << AIN_URMA_SE_SHIFT;
constexpr uint32_t AIN_URMA_SE_ON = 1U << AIN_URMA_SE_SHIFT;
constexpr uint32_t AIN_URMA_CQE_OFF = 0U << AIN_URMA_CQE_SHIFT;
constexpr uint32_t AIN_URMA_CQE_ON = 1U << AIN_URMA_CQE_SHIFT;
constexpr uint32_t AIN_URMA_INLINE_EN_OFF = 0U << AIN_URMA_INLINE_EN_SHIFT;
constexpr uint32_t AIN_URMA_INLINE_EN_ON = 1U << AIN_URMA_INLINE_EN_SHIFT;

template <uint32_t flags = AIN_URMA_DEFAULT_CONFIG>
struct AinUrmaConfig {
    static constexpr uint32_t value = flags;
};

} // namespace AscendC

#endif // INCLUDE_ADV_API_AIN_AIN_COMMON_H
