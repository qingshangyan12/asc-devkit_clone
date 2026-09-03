/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_AICPU_TASK_CACHE_RUNNER_H
#define MC2_AICPU_TASK_CACHE_RUNNER_H

#include "hcomm_primitives.h"
#include "log.h"
#include "mc2_aicpu_replay_gate.h"
#include "mc2_aicpu_task_cache.h"

namespace mc2_ops_hccl {

inline bool IsMc2TaskCacheOp(HcclCMDType opType)
{
    return opType == HCCL_CMD_ALLREDUCE || opType == HCCL_CMD_ALLGATHER || opType == HCCL_CMD_REDUCE_SCATTER ||
           opType == HCCL_CMD_ALLTOALLV || opType == HCCL_CMD_ALLTOALL;
}

inline void ClearMc2TaskCacheBestEffort(
    const Mc2AicpuTaskCachePlan& plan, const char* stage, Mc2AicpuTaskCacheManager& manager)
{
    if (!plan.enabled) {
        return;
    }
    const HcclResult ret = Mc2AicpuTaskCacheClear(plan);
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("[MC2_TASK_CACHE][Clear] stage[%s], tag[%s], ret[%d].", stage, plan.tag.c_str(), ret);
        return;
    }
    manager.Unregister(plan.comm, plan.tag);
}

template <typename Func>
HcclResult RunMc2ExecutorBatch(const OpParam& param, Func func, const Mc2AicpuReplayGate& replayGate)
{
    const HcclResult armRet = ArmMc2AicpuReplayGate(replayGate);
    HcclResult execRet = HCCL_SUCCESS;
    if (armRet == HCCL_SUCCESS) {
        execRet = func();
    }

    // BatchModeEnd can publish the gate and a partial executor graph even when the executor returned an error.
    const bool mayBePublished = replayGate.enabled && armRet == HCCL_SUCCESS;
    const HcclResult endRet = static_cast<HcclResult>(HcommBatchModeEnd(param.algTag));
    const HcclResult releaseRet = mayBePublished ? ReleaseMc2AicpuReplayGate(replayGate) : HCCL_SUCCESS;
    if (releaseRet != HCCL_SUCCESS) {
        HCCL_ERROR(
            "[MC2_REPLAY_GATE][Release] executor fallback failed, algTag[%s], ret[%d].", param.algTag,
            static_cast<int>(releaseRet));
    }

    if (armRet != HCCL_SUCCESS) {
        return armRet;
    }
    if (execRet != HCCL_SUCCESS) {
        return execRet;
    }
    if (endRet != HCCL_SUCCESS) {
        return endRet;
    }
    return releaseRet;
}

template <typename Func>
HcclResult RunWithMc2TaskCache(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx, Func func,
    Mc2AicpuTaskCacheManager& manager = Mc2AicpuTaskCacheManager::Instance())
{
    if (!IsMc2TaskCacheOp(param.opType)) {
        return func();
    }

    Mc2AicpuReplayGate replayGate;
    CHK_RET(BuildMc2AicpuReplayGate(param, resCtx, replayGate));

    HCCL_DEBUG(
        "[MC2_BATCH][Start] opType[%u], algName[%s], algTag[%s].", static_cast<u32>(param.opType), param.algName,
        param.algTag);
    HcclResult startRet = static_cast<HcclResult>(HcommBatchModeStart(param.algTag));
    CHK_PRT_RET(
        startRet != HCCL_SUCCESS,
        HCCL_ERROR(
            "[MC2_BATCH][Start] failed, ret[%d], opType[%u], algName[%s], algTag[%s].", static_cast<int>(startRet),
            static_cast<u32>(param.opType), param.algName, param.algTag),
        startRet);

    bool batchActive = true;
    Mc2AicpuTaskCachePlan plan;
    HcclResult policyRet = BuildMc2AicpuTaskCachePlan(param, resCtx, plan);
    if (policyRet != HCCL_SUCCESS || !plan.enabled) {
        if (policyRet != HCCL_SUCCESS) {
            HCCL_WARNING("[MC2_TASK_CACHE][Policy] failed and fallback to executor, ret[%d].", policyRet);
        }
        return RunMc2ExecutorBatch(param, func, replayGate);
    }

    const Mc2TaskCacheReserveResult reserveResult = manager.TryReserve(plan.comm, plan.tag);
    if (reserveResult == Mc2TaskCacheReserveResult::FULL) {
        return RunMc2ExecutorBatch(param, func, replayGate);
    }

    bool isHit = false;
    HcclResult lookupRet = Mc2AicpuTaskCacheLookup(plan, isHit);
    if (lookupRet != HCCL_SUCCESS) {
        HCCL_WARNING(
            "[MC2_TASK_CACHE][Lookup] failed and fallback to executor, tag[%s], ret[%d].", plan.tag.c_str(), lookupRet);
        ClearMc2TaskCacheBestEffort(plan, "lookup", manager);
        return RunMc2ExecutorBatch(param, func, replayGate);
    }

    if (isHit) {
        // A cache hit replays the gate wait captured on the miss path. Do not enqueue a second wait.
        const HcclResult executeRet = Mc2AicpuTaskCacheExecute(plan);
        const HcclResult endRet = static_cast<HcclResult>(HcommBatchModeEnd(param.algTag));
        const HcclResult releaseRet = ReleaseMc2AicpuReplayGate(replayGate);
        HCCL_INFO(
            "[MC2_TASK_CACHE][Hit] tag[%s], executeRet[%d], batchEndRet[%d], releaseRet[%d].", plan.tag.c_str(),
            executeRet, endRet, releaseRet);
        if (executeRet != HCCL_SUCCESS) {
            ClearMc2TaskCacheBestEffort(plan, "execute", manager);
            return executeRet;
        }
        if (endRet != HCCL_SUCCESS) {
            ClearMc2TaskCacheBestEffort(plan, "hit batch end", manager);
            return endRet;
        }
        if (releaseRet != HCCL_SUCCESS) {
            ClearMc2TaskCacheBestEffort(plan, "hit release", manager);
            return releaseRet;
        }
        return HCCL_SUCCESS;
    }

    const HcclResult cacheStartRet = Mc2AicpuTaskCacheStart(plan);
    if (cacheStartRet != HCCL_SUCCESS) {
        HCCL_WARNING(
            "[MC2_TASK_CACHE][Start] failed and fallback to executor, tag[%s], ret[%d].", plan.tag.c_str(),
            cacheStartRet);
        ClearMc2TaskCacheBestEffort(plan, "start", manager);
        return RunMc2ExecutorBatch(param, func, replayGate);
    }

    const HcclResult armRet = ArmMc2AicpuReplayGate(replayGate);
    HcclResult execRet = HCCL_SUCCESS;
    if (armRet == HCCL_SUCCESS) {
        execRet = func();
    }

    HcclResult flushRet = static_cast<HcclResult>(HcommBatchModeEnd(param.algTag));
    batchActive = false;
    HcclResult restartRet = HCCL_SUCCESS;
    if (flushRet == HCCL_SUCCESS) {
        restartRet = static_cast<HcclResult>(HcommBatchModeStart(param.algTag));
        batchActive = (restartRet == HCCL_SUCCESS);
    }

    HcclResult cacheEndRet = HCCL_SUCCESS;
    if (armRet == HCCL_SUCCESS && execRet == HCCL_SUCCESS && flushRet == HCCL_SUCCESS &&
        restartRet == HCCL_SUCCESS) {
        cacheEndRet = Mc2AicpuTaskCacheEnd(plan);
        if (cacheEndRet != HCCL_SUCCESS) {
            ClearMc2TaskCacheBestEffort(plan, "cache end", manager);
        }
    } else {
        ClearMc2TaskCacheBestEffort(plan, "capture failure", manager);
    }

    HcclResult endRet = HCCL_SUCCESS;
    if (batchActive) {
        endRet = static_cast<HcclResult>(HcommBatchModeEnd(param.algTag));
        if (endRet != HCCL_SUCCESS) {
            ClearMc2TaskCacheBestEffort(plan, "final batch end", manager);
        }
    }
    // Once the first BatchModeEnd has been attempted, the wait and a partial graph may already be visible to
    // hardware. Always release that wait after the final cache bookkeeping batch has been closed.
    const bool mayBePublished = replayGate.enabled && armRet == HCCL_SUCCESS;
    const HcclResult releaseRet = mayBePublished ? ReleaseMc2AicpuReplayGate(replayGate) : HCCL_SUCCESS;
    if (releaseRet != HCCL_SUCCESS) {
        HCCL_ERROR(
            "[MC2_REPLAY_GATE][Release] capture failed, algTag[%s], ret[%d].", param.algTag,
            static_cast<int>(releaseRet));
        ClearMc2TaskCacheBestEffort(plan, "capture release", manager);
    }
    HCCL_DEBUG(
        "[MC2_BATCH][End] opType[%u], algName[%s], algTag[%s], armRet[%d], execRet[%d], flushRet[%d], "
        "restartRet[%d], cacheEndRet[%d], endRet[%d], releaseRet[%d].",
        static_cast<u32>(param.opType), param.algName, param.algTag, static_cast<int>(armRet),
        static_cast<int>(execRet), static_cast<int>(flushRet), static_cast<int>(restartRet),
        static_cast<int>(cacheEndRet), static_cast<int>(endRet), static_cast<int>(releaseRet));

    if (armRet != HCCL_SUCCESS) {
        return armRet;
    }
    if (execRet != HCCL_SUCCESS) {
        return execRet;
    }
    if (flushRet != HCCL_SUCCESS) {
        return flushRet;
    }
    if (restartRet != HCCL_SUCCESS) {
        return restartRet;
    }
    if (cacheEndRet != HCCL_SUCCESS) {
        return cacheEndRet;
    }
    if (endRet != HCCL_SUCCESS) {
        return endRet;
    }
    if (releaseRet != HCCL_SUCCESS) {
        return releaseRet;
    }
    HCCL_INFO("[MC2_TASK_CACHE][Capture] tag[%s] published.", plan.tag.c_str());
    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl

#endif // MC2_AICPU_TASK_CACHE_RUNNER_H
