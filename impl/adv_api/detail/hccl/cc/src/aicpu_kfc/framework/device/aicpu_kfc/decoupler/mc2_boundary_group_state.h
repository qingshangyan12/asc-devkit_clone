/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_BOUNDARY_GROUP_STATE_H
#define MC2_BOUNDARY_GROUP_STATE_H
#include <set>
#include "mc2_algorithm_boundary_sync.h"

namespace mc2_boundary {
// Submission state, owned by one communication-domain server for one kernel.
// A turn is committed only after its CCoreRecord has been submitted successfully.
class GroupState {
public:
    bool Begin(const Config& config, uint32_t repeat)
    {
        if (!Valid(config) || pending_ || repeat == 0U) {
            return false;
        }
        if (config.mode == 0U) {
            return !active_;
        }
        if (!active_) {
            if (used_.count(config.groupId) != 0U || repeat > config.totalTurns) {
                return false;
            }
            config_ = config;
            turn_ = 0U;
            active_ = true;
            used_.insert(config.groupId);
        }
        return config.mode == config_.mode && config.groupId == config_.groupId &&
               config.totalTurns == config_.totalTurns && repeat <= config_.totalTurns - turn_;
    }
    bool StartTurn()
    {
        if (pending_) {
            return false;
        }
        pending_ = true;
        return true;
    }
    void CommitTurn()
    {
        pending_ = false;
        if (active_ && ++turn_ == config_.totalTurns) {
            active_ = false;
        }
    }
    bool Complete() const { return !active_ && !pending_; }
    uint32_t Turn() const { return turn_; }

private:
    Config config_{};
    std::set<uint32_t> used_;
    uint32_t turn_ = 0U;
    bool active_ = false;
    bool pending_ = false;
};
}
#endif
