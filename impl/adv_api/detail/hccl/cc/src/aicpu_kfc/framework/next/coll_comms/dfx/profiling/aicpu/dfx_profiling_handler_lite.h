/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 *
 * Ported from hcomm PR#3878 (DfxProfilingHandlerLite replaces ProfilingHandlerLite).
 * Header-only: reporting methods that depend on Msprof backend are inline no-ops until
 * the .cc reporting logic is ported (requires Msprof APIs not available in this snapshot).
 */
#ifndef HCCL_DFXPROFILING_HANDLER_LITE_H
#define HCCL_DFXPROFILING_HANDLER_LITE_H

#include <cstdint>
#include <unordered_map>
#include "hccl/hccl_types.h"
#include "hccl/base.h"
#include "dfx_circular_queue.h"
#include "res_pub.h"

namespace Hccl {

enum DfxMainStreamTaskType : uint8_t { HEAD = 0, TAIL = 1 };

enum DfxProfilingLevel : uint8_t { L0 = 0, L1 = 1 };

struct DfxFlagTaskInfo {
    uint32_t taskId;
    DfxMainStreamTaskType type;
};

class DfxProfilingHandlerLite {
public:
    static DfxProfilingHandlerLite& GetInstance()
    {
        static DfxProfilingHandlerLite instance;
        return instance;
    }

    HcclResult Init()
    {
        initializedFlag_ = true;
        return HCCL_SUCCESS;
    }

    void UpdateProfSwitch() {}

    inline void SetProL0On(bool val) { enableHcclL0_ = val; }

    inline void SetProL1On(bool val) { enableHcclL1_ = val; }

    inline bool GetProfL0State() const { return enableHcclL0_; }

    inline bool GetProfL1State() const { return enableHcclL1_; }

    inline void SetCachedCommInfo(u64 groupName, u32 localRank, u32 rankSize)
    {
        cachedGroupName_ = groupName;
        cachedLocalRank_ = localRank;
        cachedRankSize_ = rankSize;
    }

    inline void SetCachedChannelRemoteRankIdMap(const std::unordered_map<u64, u32>* mapPtr)
    {
        cachedChannelRemoteRankIdMap_ = mapPtr;
    }

    inline HcclResult SetCurrDfxOpInfo(const DfxDfxOpInfo* dfxOpInfo)
    {
        currDfxOpInfo_ = dfxOpInfo;
        return HCCL_SUCCESS;
    }

    inline const DfxDfxOpInfo* GetCurrDfxOpInfo() const { return currDfxOpInfo_; }

    uint64_t GetProfHashId(const char* name, uint32_t len) const
    {
        if (name == nullptr || len == 0) {
            return DFX_INVALID_U64;
        }
        uint64_t h = 1469598103934665603ULL;
        for (uint32_t i = 0; i < len; ++i) {
            h ^= static_cast<uint64_t>(static_cast<uint8_t>(name[i]));
            h *= 1099511628211ULL;
        }
        return h;
    }

    void ReportHcclOpInfo(const DfxDfxOpInfo& opInfo) const {}
    void ReportMainStreamTask(const DfxFlagTaskInfo& flagTaskInfo) const {}
    void ReportStreamTaskDetails(TaskInfoCircularQueue& taskQueue) const {}
    void ReportStreamTaskDetailsLog(TaskInfoCircularQueue& taskQueue) const {}

private:
    DfxProfilingHandlerLite() = default;
    ~DfxProfilingHandlerLite() = default;
    DfxProfilingHandlerLite(const DfxProfilingHandlerLite&) = delete;
    DfxProfilingHandlerLite& operator=(const DfxProfilingHandlerLite&) = delete;

    bool enableHcclL0_{false};
    bool enableHcclL1_{false};
    bool initializedFlag_{false};
    uint64_t cachedGroupName_{DFX_INVALID_U64};
    u32 cachedRankSize_{0};
    u32 cachedLocalRank_{INVALID_U32};
    const std::unordered_map<u64, u32>* cachedChannelRemoteRankIdMap_{nullptr};
    const DfxDfxOpInfo* currDfxOpInfo_{nullptr};
};

} // namespace Hccl

#endif // HCCL_DFXPROFILING_HANDLER_LITE_H
