/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCLV2_STREAM_LITE_H
#define HCCLV2_STREAM_LITE_H
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "types.h"
#include "rtsq_base.h"
#include "dfx_circular_queue.h"
namespace Hccl {

class StreamLite {
public:
    explicit StreamLite(std::vector<char>& uniqueId);
    StreamLite(u32 id, u32 sqIds, u32 phyId, u32 cqIds);
    StreamLite(u32 id, u32 sqIds, u32 phyId, u32 cqIds, bool launchFlag);

    inline u32 GetId() const { return id; }

    inline u32 GetSqId() const { return sqId; }

    inline u32 GetCqId() const { return cqId; }

    inline u32 GetDevPhyId() const { return devPhyId; }

    RtsqBase* GetRtsq() const;

    std::string Describe() const;

    inline DfxTaskInfo* NextTaskSlot() const
    {
        if (taskInfos_.IsFull()) {
            ReportStreamTask();
        }
        return static_cast<DfxTaskInfo*>(taskInfos_.NextSlot());
    }

    inline void SetReportStreamTaskCallback(std::function<void(TaskInfoCircularQueue*)> callback)
    {
        reportStreamTaskCallback_ = std::move(callback);
    }

    inline bool HasReportStreamTaskCallback() const { return static_cast<bool>(reportStreamTaskCallback_); }

    inline void ReportStreamTask() const
    {
        if (reportStreamTaskCallback_ != nullptr) {
            reportStreamTaskCallback_(&taskInfos_);
        }
    }

    inline void SetGetLatestDfxOpInfoCallback(std::function<const void*()> callback)
    {
        getLatestDfxOpInfoCallback_ = std::move(callback);
    }

    inline const void* GetLatestDfxOpInfo() const
    {
        return (getLatestDfxOpInfoCallback_ != nullptr) ? getLatestDfxOpInfoCallback_() : nullptr;
    }

private:
    u32 id;
    u32 sqId;
    u32 devPhyId{0};
    u32 cqId{0};

    std::unique_ptr<RtsqBase> rtsq;

    // DFX: 流级任务信息环形队列 + 上报回调（mutable 以支持 const 方法写入）
    mutable TaskInfoCircularQueue taskInfos_;
    std::function<void(TaskInfoCircularQueue*)> reportStreamTaskCallback_{nullptr};
    std::function<const void*()> getLatestDfxOpInfoCallback_{nullptr};
};

} // namespace Hccl

#endif
