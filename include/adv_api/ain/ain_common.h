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

/*!
 * @brief Force-inline qualifier for Ain device-side member functions.
 *        All Ain/AinBarrierSession methods run on the AICORE and are always inlined.
 */
#define AIN_DEVICE __attribute__((always_inline)) __aicore__ __inline__

namespace AscendC {

/*!
 * @brief Placeholder tag for "no remote action" in the put primitive.
 */
typedef struct {} AinRemoteNone;

/*!
 * @brief Placeholder tag for "no local action" in the put primitive.
 */
typedef struct {} AinLocalNone;

/*!
 * @brief Placeholder tag indicating that no valid UB descriptor was supplied.
 *        Passing this type to put/get is rejected at compile time.
 */
typedef struct {} AinDescriptorUbufNone;

/*!
 * @brief Compile-time type equality trait, true only when both types are identical.
 */
template <typename T, typename U>
struct AinIsSame { static constexpr bool value = false; };
template <typename T>
struct AinIsSame<T, T> { static constexpr bool value = true; };

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

/*!
 * @brief Commit behaviour for put/get tasks.
 */
enum AinCommitFlags {
    AIN_COMMIT_IMMED   = 0,        ///< Default: assemble the WQE and ring the doorbell immediately.
    AIN_COMMIT_DELAYED = (1 << 0)  ///< Assemble the WQE only; ringing the doorbell is deferred to flush().
};

/*!
 * @brief Describes a communication team (a set of ranks participating in collective ops).
 * @param nRanks: Number of ranks in the team.
 * @param rank: The rank id of the local participant within the team.
 * @param stride: Stride used to address ranks within the team.
 * @param ranks: Pointer to the team's rank table.
 */
typedef struct {
    uint32_t nRanks;
    uint32_t rank;
    uint32_t stride;
    void *ranks;
} HcclTeam;

} // namespace AscendC

#endif // INCLUDE_ADV_API_AIN_AIN_COMMON_H
