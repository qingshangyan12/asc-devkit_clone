/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_ALGORITHM_BOUNDARY_SYNC_H
#define MC2_ALGORITHM_BOUNDARY_SYNC_H
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace AscendC {
enum class Mc2AlgorithmBoundarySyncMode : uint8_t {
    DISABLED = 0,
    INPUT_READY_GROUP = 1,
    OUTPUT_DEFERRED_GROUP = 2,
};
}

namespace mc2_boundary {
constexpr uint32_t MAGIC = 0x4d423031U;
constexpr uint16_t VERSION = 1U;
constexpr uint32_t NOTIFY_LIMIT = 64U;

// Private wire extension. Never append fields to the cross-SO OpParam structure.
struct Config {
    uint32_t magic = MAGIC;
    uint16_t version = VERSION;
    uint16_t size = sizeof(Config);
    uint32_t mode = 0U;
    uint32_t groupId = 0U;
    uint32_t totalTurns = 0U;
};
static_assert(sizeof(Config) == 20U, "Boundary extension wire size changed");
static_assert(offsetof(Config, totalTurns) == 16U, "Boundary extension wire layout changed");

inline bool Valid(const Config& config)
{
    return config.magic == MAGIC && config.version == VERSION && config.size == sizeof(Config) &&
           config.mode <= 2U && (config.mode == 0U || config.totalTurns > 0U);
}

// Unknown/absent magic is the legacy tiling format. Recognized, malformed records fail.
inline bool Decode(const void* bytes, size_t size, Config& config, bool requireExtension = false)
{
    config = Config{};
    if (bytes == nullptr || size < sizeof(Config)) {
        return false;
    }
    uint32_t magic = 0U;
    std::memcpy(&magic, bytes, sizeof(magic));
    if (magic != MAGIC) {
        return !requireExtension;
    }
    std::memcpy(&config, bytes, sizeof(config));
    return Valid(config);
}

inline void Encode(void* bytes, const Config& config)
{
    std::memcpy(bytes, &config, sizeof(config));
}

// Only the first entry and final exit of a complete executor may use this policy.
struct TurnPolicy {
    bool enabled = false;
    bool skipEntry = false;
    uint32_t exitNotify = 0U;
    uint32_t communicationThreads = 0U;
    uint32_t activeThreads = 0U;
    // Diagnostic only; not part of the wire format or task-cache identity.
    const char* fallbackReason = "disabled";
};

// Preserve the executor's existing channel-selection rule, including legacy SoleNHR.
inline bool ConfigureAllGatherChannels(bool aicpu, bool isPod, const char* algorithm)
{
    return aicpu && (isPod || std::strcmp(algorithm, "AicpuAllGatherSoleNHR") != 0);
}

inline bool NotifyBudget(uint32_t base, uint32_t threads, uint32_t turns, uint32_t& required)
{
    required = base;
    if (threads < 2U || base >= NOTIFY_LIMIT || turns == 0U ||
        threads - 1U > (NOTIFY_LIMIT - base) / turns) {
        return false;
    }
    required = base + turns * (threads - 1U);
    return true;
}
}
#endif
