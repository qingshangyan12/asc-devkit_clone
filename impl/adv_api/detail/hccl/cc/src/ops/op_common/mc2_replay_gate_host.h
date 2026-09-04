/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_REPLAY_GATE_HOST_H
#define MC2_REPLAY_GATE_HOST_H

#include <algorithm>
#include <cstring>
#include <type_traits>
#include "op_common.h"
#include "hccl_res_dl.h"
#include "log.h"

namespace mc2_ops_hccl {

// Private host-only sidecar. It owns no Thread and is not part of the device resource ABI.
struct Mc2ReplayGateHostContext {
    uint32_t magic = 0x4d433247U;
    uint32_t version = 1U;
    uint32_t valid = 1U;
    uint32_t algorithmThreadNum = 0U;
    uint32_t notifyIdx = 0U;
    uint32_t reserved = 0U;
    ThreadHandle mainThread = 0;
    ThreadHandle releaseThread = 0;
    uint64_t resourceAddress = 0;
    uint64_t resourceSize = 0;
};
static_assert(std::is_trivially_copyable<Mc2ReplayGateHostContext>::value, "Gate sidecar must be POD storage");

inline bool IsMc2AllReduceReplayGateRequested(const OpParam& param)
{
    return param.opType == HCCL_CMD_ALLREDUCE && param.engine == COMM_ENGINE_AICPU_TS;
}

inline HcclResult Mc2ReplayGateContextTag(const OpParam& param, std::string& tag)
{
    const size_t length = strnlen(param.algTag, sizeof(param.algTag));
    CHK_PRT_RET(length == 0 || length == sizeof(param.algTag),
        HCCL_ERROR("[MC2_REPLAY_GATE][Host] invalid algorithm tag."), HCCL_E_PARA);
    tag = std::string(param.algTag, length) + "_mc2rg_v1";
    CHK_PRT_RET(tag.size() >= ALG_TAG_LENGTH,
        HCCL_ERROR("[MC2_REPLAY_GATE][Host] sidecar tag is too long."), HCCL_E_PARA);
    return HCCL_SUCCESS;
}

inline HcclResult PublishMc2ReplayGateHostContext(
    HcclComm comm, const OpParam& param, const AlgResourceCtxSerializable& resCtx,
    void* resource, uint64_t resourceSize)
{
    if (!IsMc2AllReduceReplayGateRequested(param)) {
        return HCCL_SUCCESS;
    }
    CHK_PRT_RET(resCtx.threads.empty() || param.deviceOrderThread == 0 || resource == nullptr || resourceSize == 0,
        HCCL_ERROR("[MC2_REPLAY_GATE][Host] incomplete resources at publication."), HCCL_E_INTERNAL);
    std::string tag;
    CHK_RET(Mc2ReplayGateContextTag(param, tag));
    Mc2ReplayGateHostContext record;
    record.algorithmThreadNum = static_cast<uint32_t>(resCtx.threads.size());
    record.mainThread = resCtx.threads.front();
    record.releaseThread = param.deviceOrderThread;
    uint32_t maxNotifyNum = resCtx.notifyNumOnMainThread;
    for (uint32_t count : resCtx.notifyNumPerThread) {
        maxNotifyNum = std::max(maxNotifyNum, count);
    }
    record.notifyIdx = maxNotifyNum + 1U;
    record.resourceAddress = reinterpret_cast<uint64_t>(resource);
    record.resourceSize = resourceSize;
    void* storage = nullptr;
    uint64_t size = 0;
    if (HcclEngineCtxGet(comm, tag.c_str(), COMM_ENGINE_CPU_TS, &storage, &size) != HCCL_SUCCESS) {
        size = sizeof(record);
        CHK_RET(HcclEngineCtxCreate(comm, tag.c_str(), COMM_ENGINE_CPU_TS, size, &storage));
    }
    CHK_PRT_RET(storage == nullptr || size != sizeof(record),
        HCCL_ERROR("[MC2_REPLAY_GATE][Host] invalid sidecar storage."), HCCL_E_INTERNAL);
    // Publish only after threads, channels and the device resource context are complete.
    CHK_RET(HcclEngineCtxCopy(comm, COMM_ENGINE_CPU_TS, tag.c_str(), &record, sizeof(record), 0));
    HCCL_DEBUG("[MC2_REPLAY_GATE][Host][ENABLE] mode=pooled_tail main[0x%llx] release[0x%llx] N[%u] notifyIdx[%u].",
        record.mainThread, record.releaseThread, record.algorithmThreadNum, record.notifyIdx);
    return HCCL_SUCCESS;
}

inline HcclResult RestoreMc2ReplayGateHostContext(
    HcclComm comm, OpParam& param, void* resource, uint64_t resourceSize)
{
    if (!IsMc2AllReduceReplayGateRequested(param)) {
        return HCCL_SUCCESS;
    }
    param.deviceOrderThread = 0;
    std::string tag;
    CHK_RET(Mc2ReplayGateContextTag(param, tag));
    void* storage = nullptr;
    uint64_t size = 0;
    const HcclResult ret = HcclEngineCtxGet(comm, tag.c_str(), COMM_ENGINE_CPU_TS, &storage, &size);
    CHK_PRT_RET(ret != HCCL_SUCCESS || storage == nullptr || size != sizeof(Mc2ReplayGateHostContext),
        HCCL_ERROR("[MC2_REPLAY_GATE][Host] missing or invalid sidecar for reused resources."), HCCL_E_INTERNAL);
    Mc2ReplayGateHostContext record;
    std::memcpy(&record, storage, sizeof(record));
    CHK_PRT_RET(record.magic != 0x4d433247U || record.version != 1U || record.valid != 1U ||
        record.algorithmThreadNum == 0 || record.mainThread == 0 || record.releaseThread == 0 ||
        record.mainThread == record.releaseThread || record.notifyIdx == 0 || resource == nullptr ||
        record.resourceAddress != reinterpret_cast<uint64_t>(resource) || record.resourceSize != resourceSize,
        HCCL_ERROR("[MC2_REPLAY_GATE][Host] reused resource/sidecar mismatch."), HCCL_E_INTERNAL);
    ThreadHandle mainThread = 0;
    uint32_t notifyNum = 0;
    CHK_RET(GetMainThreadInfo(comm, param, mainThread, notifyNum));
    CHK_PRT_RET(mainThread != record.mainThread || notifyNum != record.notifyIdx,
        HCCL_ERROR("[MC2_REPLAY_GATE][Host] reused main thread/notify mismatch."), HCCL_E_INTERNAL);
    param.deviceOrderThread = record.releaseThread;
    HCCL_DEBUG("[MC2_REPLAY_GATE][Host][REUSE] mode=pooled_tail main[0x%llx] release[0x%llx] N[%u] notifyIdx[%u].",
        record.mainThread, record.releaseThread, record.algorithmThreadNum, record.notifyIdx);
    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl
#endif // MC2_REPLAY_GATE_HOST_H
