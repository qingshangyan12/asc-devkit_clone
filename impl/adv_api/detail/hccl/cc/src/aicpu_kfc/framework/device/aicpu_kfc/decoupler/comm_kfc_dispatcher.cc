/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "comm_kfc_dispatcher.h"

#include <array>
#include <cstdint>

#include "hccl_msg.h"

#include "common/aicpu_kfc_utils.h"
#include "common/aicpu_timing_recorder.h"
#include "comm_kfc_aicpu_server.h"

using namespace HcclApi;

namespace {
using TimingCounter = Hccl::AicpuTimingRecorder::Counter;
using TimingMetric = Hccl::AicpuTimingRecorder::Metric;
using TimingPolicy = Hccl::AicpuTimingRecorder::RecordPolicy;
using TimingStage = Hccl::AicpuTimingRecorder::Stage;

struct PrepareProgress {
    u32 lastFinishedCnt;
    u32 finishedCntAtExpandEnd;
    uint64_t expandEndNs;
    uint64_t firstFinishedObservedNs;
    uint64_t allFinishedObservedNs;
    u32 seqNum;
    u32 msgPos;
    u32 repeatCnt;
    bool active;
    bool completed;
};

struct AscCommServerInfo {
    CommKfcAicpuServer serverIns;
    HcclMsg msg{};
    std::shared_ptr<HcclMsgExt> extMsg;
    std::array<PrepareProgress, HCCL_MSG_CNT> prepareProgress{};
    std::array<u32, HCCL_MSG_CNT> incompletePreparePos{};
    u32 msgPos{0U};
    u32 retryCnt{0U};
    u32 incompletePrepareCnt{0U};
    bool finalizeFlag{false};
    bool finishFlag{false};
    AscCommServerInfo(u32 groupIdx) : serverIns(groupIdx) { extMsg = std::make_shared<HcclMsgExt>(); }
};
static constexpr u32 MAX_RETRY_CNT = 10U;
constexpr char COMPLETE_STATUS[] = "complete";

void LoadBarrier()
{
#ifdef __aarch64__
    __asm__ __volatile__("dsb ld" : : : "memory");
#endif
}

u32 GetFinishedTurnCnt(const AscCommServerInfo& server, u32 msgPos)
{
    const HcclMsgArea* msgArea = server.serverIns.GetMsgAreaAddr();
    if (msgArea == nullptr || msgPos >= HCCL_MSG_CNT) {
        return 0U;
    }
    const auto* finishedCnt =
        reinterpret_cast<volatile const u32*>(&(msgArea->commMsg.singleMsg.finishedTurnCnt[msgPos].cnt));
    return *finishedCnt;
}

void RemoveIncompletePrepare(AscCommServerInfo& server, u32 index)
{
    if (index >= server.incompletePrepareCnt) {
        return;
    }
    --server.incompletePrepareCnt;
    server.incompletePreparePos[index] = server.incompletePreparePos[server.incompletePrepareCnt];
}

bool RemoveIncompletePrepareByPos(AscCommServerInfo& server, u32 msgPos)
{
    for (u32 i = 0U; i < server.incompletePrepareCnt; ++i) {
        if (server.incompletePreparePos[i] == msgPos) {
            RemoveIncompletePrepare(server, i);
            return true;
        }
    }
    return false;
}

void MarkPrepareCompleted(
    PrepareProgress& progress, u32& incompletePrepareCnt, uint64_t observedNs,
    Hccl::AicpuTimingRecorder& timingRecorder)
{
    progress.completed = true;
    progress.allFinishedObservedNs = observedNs;
    if (incompletePrepareCnt > 0U) {
        --incompletePrepareCnt;
    }
    if (observedNs >= progress.expandEndNs) {
        (void)timingRecorder.AddMetric(TimingMetric::SUBMIT_TO_COMPLETION_OBSERVED, observedNs - progress.expandEndNs);
    }
}

HcclResult RegisterPrepareProgress(
    AscCommServerInfo& server, u32 seqNum, u32 msgPos, u32 repeatCnt, uint64_t expandEndNs, u32& incompletePrepareCnt,
    Hccl::AicpuTimingRecorder& timingRecorder)
{
    CHK_PRT_RET(msgPos >= HCCL_MSG_CNT, HCCL_ERROR("Invalid message index %u.", msgPos), HCCL_E_PARA);
    CHK_PTR_NULL(server.serverIns.GetMsgAreaAddr());

    PrepareProgress& progress = server.prepareProgress[msgPos];
    if (progress.active && !progress.completed) {
        HCCL_WARNING(
            "Replace incomplete timing progress at message index %u, old sequence %u, new sequence %u.", msgPos,
            progress.seqNum, seqNum);
        if (RemoveIncompletePrepareByPos(server, msgPos) && incompletePrepareCnt > 0U) {
            --incompletePrepareCnt;
        }
    }

    LoadBarrier();
    const u32 finishedCnt = GetFinishedTurnCnt(server, msgPos);
    progress = {};
    progress.lastFinishedCnt = finishedCnt;
    progress.finishedCntAtExpandEnd = finishedCnt;
    progress.expandEndNs = expandEndNs;
    progress.seqNum = seqNum;
    progress.msgPos = msgPos;
    progress.repeatCnt = repeatCnt;
    progress.active = true;

    if (finishedCnt > 0U) {
        progress.firstFinishedObservedNs = expandEndNs;
        (void)timingRecorder.RecordAt(TimingStage::FIRST_COMM_TURN_FINISHED_OBSERVED, expandEndNs);
        (void)timingRecorder.IncrementCounter(TimingCounter::COMM_OVERLAP_EXPAND);
    }

    if (repeatCnt == 0U || finishedCnt >= repeatCnt) {
        progress.completed = true;
        progress.allFinishedObservedNs = expandEndNs;
        (void)timingRecorder.AddMetric(TimingMetric::SUBMIT_TO_COMPLETION_OBSERVED, 0U);
        if (incompletePrepareCnt == 0U) {
            (void)timingRecorder.RecordAt(
                TimingStage::ALL_COMMUNICATION_FINISHED_OBSERVED, expandEndNs, TimingPolicy::KEEP_LATEST);
        }
        return HCCL_SUCCESS;
    }

    server.incompletePreparePos[server.incompletePrepareCnt] = msgPos;
    ++server.incompletePrepareCnt;
    ++incompletePrepareCnt;
    return HCCL_SUCCESS;
}

void ObserveCommunicationProgress(
    AscCommServerInfo& server, u32& incompletePrepareCnt, Hccl::AicpuTimingRecorder* timingRecorder)
{
    if (timingRecorder == nullptr || server.incompletePrepareCnt == 0U ||
        server.serverIns.GetMsgAreaAddr() == nullptr) {
        return;
    }

    LoadBarrier();
    uint64_t latestObservedNs = 0U;
    bool completedInThisPass = false;
    u32 incompleteIndex = 0U;
    while (incompleteIndex < server.incompletePrepareCnt) {
        PrepareProgress& progress = server.prepareProgress[server.incompletePreparePos[incompleteIndex]];
        if (!progress.active || progress.completed) {
            RemoveIncompletePrepare(server, incompleteIndex);
            if (incompletePrepareCnt > 0U) {
                --incompletePrepareCnt;
            }
            continue;
        }

        const u32 finishedCnt = GetFinishedTurnCnt(server, progress.msgPos);
        if (finishedCnt == progress.lastFinishedCnt) {
            ++incompleteIndex;
            continue;
        }
        progress.lastFinishedCnt = finishedCnt;
        latestObservedNs = timingRecorder->Now();
        if (finishedCnt > 0U && progress.firstFinishedObservedNs == 0U) {
            progress.firstFinishedObservedNs = latestObservedNs;
            (void)timingRecorder->RecordAt(TimingStage::FIRST_COMM_TURN_FINISHED_OBSERVED, latestObservedNs);
        }
        if (finishedCnt >= progress.repeatCnt) {
            MarkPrepareCompleted(progress, incompletePrepareCnt, latestObservedNs, *timingRecorder);
            RemoveIncompletePrepare(server, incompleteIndex);
            completedInThisPass = true;
            continue;
        }
        ++incompleteIndex;
    }

    if (completedInThisPass && incompletePrepareCnt == 0U) {
        (void)timingRecorder->RecordAt(
            TimingStage::ALL_COMMUNICATION_FINISHED_OBSERVED, latestObservedNs, TimingPolicy::KEEP_LATEST);
    }
}

HcclResult CreateServerList(void* args[], u32 ctxNum, std::vector<AscCommServerInfo>& serverList)
{
    CHK_PRT_RET(ctxNum == 0U, HCCL_ERROR("Invalid context number."), HCCL_E_PARA);
    std::unordered_map<uintptr_t, size_t> workspaceToServerIdx{};
    for (u32 i = 0U; i < ctxNum; ++i) {
        const HcclApi::OpResCtx* ctx = static_cast<const HcclApi::OpResCtx*>(args[i]);
        CHK_PTR_NULL(ctx);
        const uintptr_t workspace = static_cast<uintptr_t>(ctx->workspace);
        auto it = workspaceToServerIdx.find(workspace);
        if (it == workspaceToServerIdx.end()) {
            const u32 serverIdx = static_cast<u32>(serverList.size());
            AscCommServerInfo server(serverIdx);
            CHK_SMART_PTR_NULL(server.extMsg);
            HCCL_INFO("Server for group %u is created.", serverIdx);
            serverList.emplace_back(server);
            it = workspaceToServerIdx.emplace(workspace, serverList.size() - 1U).first;
        }
        CHK_PRT_RET(
            serverList[it->second].serverIns.AddOpContext(ctx) != HCCL_SUCCESS,
            HCCL_ERROR("Failed to add op for group %zu.", it->second), HCCL_E_INTERNAL);
    }
    return HCCL_SUCCESS;
}

HcclResult GetCurrentMsg(AscCommServerInfo& server)
{
    if (server.retryCnt > 0) {
        CHK_PRT_RET(
            server.retryCnt > MAX_RETRY_CNT, HCCL_ERROR("Retry count %d exceeds max value.", server.retryCnt),
            HCCL_E_INTERNAL);
        HCCL_INFO(
            "Process cache message %s at seq num %u, retry count %u.",
            AicpuKfcUtils::GetMsgSimpleStr(server.msg).c_str(), server.msgPos, server.retryCnt);
        if (static_cast<HcclCMDType>(server.msg.commType.prepareType) == HCCL_CMD_ALLTOALLV) {
            HCCL_INFO(
                "Process cache extended message %s at seq num %u.",
                AicpuKfcUtils::GetMsgSimpleStr(server.serverIns.GetRankNum(), *(server.extMsg)).c_str(), server.msgPos);
        }
        return HCCL_SUCCESS;
    }

    auto& msgBaseAddr = server.serverIns.GetMsgAreaAddr()->commMsg.singleMsg;
    HcclResult ret = AicpuKfcUtils::ReadMsgFromMemory(msgBaseAddr.sendMsgs + server.msgPos, server.msg);
    if (ret != HCCL_SUCCESS) {
        return ret;
    }

    if (static_cast<HcclCMDType>(server.msg.commType.prepareType) == HCCL_CMD_ALLTOALLV) {
        ret = AicpuKfcUtils::ReadMsgFromMemory(
            msgBaseAddr.paramExtMsgList + server.msgPos, server.serverIns.GetRankNum(), *(server.extMsg));
    }

    return ret;
}

HcclResult RecordFinalizeReceived(
    u32 serverNum, u32& finalizeCnt, bool& isLastFinalize, Hccl::AicpuTimingRecorder* timingRecorder)
{
    CHK_PRT_RET(
        finalizeCnt >= serverNum,
        HCCL_ERROR("Invalid finalize count %u, expected less than server number %u.", finalizeCnt, serverNum),
        HCCL_E_INTERNAL);
    const bool isFirstFinalize = finalizeCnt == 0U;
    ++finalizeCnt;
    isLastFinalize = finalizeCnt == serverNum;
    if (timingRecorder != nullptr) {
        (void)timingRecorder->IncrementCounter(TimingCounter::FINALIZE);
        if (isFirstFinalize) {
            (void)timingRecorder->Record(TimingStage::FIRST_FINALIZE_RECEIVED);
        }
        if (isLastFinalize) {
            (void)timingRecorder->Record(TimingStage::LAST_FINALIZE_RECEIVED);
        }
    }
    return HCCL_SUCCESS;
}

HcclResult FinalizeProcess(AscCommServerInfo& server, bool isLastFinalize, Hccl::AicpuTimingRecorder* timingRecorder)
{
    CHK_RET(server.serverIns.Finalize(server.msgPos));
    if (isLastFinalize && timingRecorder != nullptr) {
        (void)timingRecorder->Record(TimingStage::LAST_FINALIZE_PROCESSED);
    }
    server.finalizeFlag = true;
    return HCCL_SUCCESS;
}

HcclResult InterGroupSyncProcess(
    std::vector<AscCommServerInfo>& serverList, u32 curGroupIdx, Hccl::AicpuTimingRecorder* timingRecorder)
{
    auto& server = serverList[curGroupIdx];
    const u32 groupId = static_cast<u32>(server.msg.addMsg.v0Msg.commDepGroupID);
    const HcclHandle handleId = server.msg.addMsg.v0Msg.commDepHandleID;
    CHK_PRT_RET(
        groupId >= serverList.size() || groupId == curGroupIdx || handleId < 0,
        HCCL_ERROR(
            "Invalid handle id %d or group id %u, current group id %u/%u.", handleId, groupId, curGroupIdx,
            serverList.size()),
        HCCL_E_PARA);
    HcclResult ret = server.serverIns.InterGroupSync(serverList[groupId].serverIns, handleId);
    if (ret == HCCL_SUCCESS) {
        server.retryCnt = 0;
        server.msgPos = (server.msgPos + 1U) % HCCL_MSG_CNT;
        HCCL_INFO("Group %u added wait sqe for group %u handle id %d successfully.", curGroupIdx, groupId, handleId);
    } else if (ret == HCCL_E_AGAIN) {
        ++(server.retryCnt);
        if (timingRecorder != nullptr) {
            (void)timingRecorder->IncrementCounter(TimingCounter::INTER_GROUP_SYNC_RETRY);
        }
        HCCL_INFO("Group sync(%u-%u) will be retried at seq num %u.", curGroupIdx, groupId, server.msgPos);
    } else {
        HCCL_ERROR("Group sync(%u-%u) failed, handle id %d, error code %u.", groupId, handleId, ret);
        return ret;
    }
    return HCCL_SUCCESS;
}

HcclResult PrepareProcess(
    AscCommServerInfo& server, u32& expectSeqNum, u32& incompletePrepareCnt, Hccl::AicpuTimingRecorder* timingRecorder)
{
    const u32 seqNum = static_cast<u32>(server.msg.addMsg.v1Msg.seqNum);
    if (expectSeqNum != seqNum) {
        HCCL_INFO("Expect seq id %u but receive %u.", expectSeqNum, seqNum);
        ++(server.retryCnt);
        if (timingRecorder != nullptr) {
            (void)timingRecorder->IncrementCounter(TimingCounter::SEQ_RETRY);
        }
    } else {
        if (timingRecorder != nullptr) {
            (void)timingRecorder->Record(TimingStage::FIRST_PREPARE_ACCEPTED);
            (void)timingRecorder->IncrementCounter(TimingCounter::PREPARE);
        }
        HcclResult ret = server.serverIns.Orchestrate(server.msg, *(server.extMsg), server.msgPos, timingRecorder);
        CHK_RET(ret);
        uint64_t bookkeepingBeginNs = 0U;
        if (timingRecorder != nullptr) {
            const uint64_t expandEndNs = timingRecorder->Now();
            if (timingRecorder->IsFullEnabled()) {
                bookkeepingBeginNs = expandEndNs;
            }
            (void)timingRecorder->RecordAt(TimingStage::FIRST_PREPARE_EXPAND_END, expandEndNs);
            (void)timingRecorder->RecordAt(
                TimingStage::LAST_PREPARE_EXPAND_END, expandEndNs, TimingPolicy::KEEP_LATEST);
            CHK_RET(RegisterPrepareProgress(
                server, seqNum, server.msgPos, static_cast<u32>(server.msg.addMsg.v1Msg.repeatCnt), expandEndNs,
                incompletePrepareCnt, *timingRecorder));
        }
        server.msgPos = (server.msgPos + 1U) % HCCL_MSG_CNT;
        ++expectSeqNum;
        server.retryCnt = 0;
        if (timingRecorder != nullptr && timingRecorder->IsFullEnabled()) {
            const uint64_t bookkeepingEndNs = timingRecorder->Now();
            if (bookkeepingEndNs >= bookkeepingBeginNs) {
                (void)timingRecorder->AddMetric(
                    TimingMetric::PREPARE_BOOKKEEPING, bookkeepingEndNs - bookkeepingBeginNs);
            }
        }
    }
    return HCCL_SUCCESS;
}

HcclResult GroupServerProcess(
    std::vector<AscCommServerInfo>& serverList, u32 groupIdx, u32& expectSeq, u32& finalizeCnt, u32& finishCnt,
    u32& incompletePrepareCnt, Hccl::AicpuTimingRecorder* timingRecorder)
{
    auto& server = serverList[groupIdx];
    ObserveCommunicationProgress(server, incompletePrepareCnt, timingRecorder);
    if (server.finishFlag) {
        return HCCL_SUCCESS;
    }

    HcclResult ret;
    if (server.finalizeFlag) {
        bool isFinish = false;
        CHK_RET(server.serverIns.IsAllTaskFinished(server.msgPos, isFinish));
        if (isFinish) {
            server.finishFlag = true;
            ++finishCnt;
            HCCL_INFO("Group %u is finished, total finished number %u/%u.", groupIdx, finishCnt, serverList.size());
        } else {
            ret = server.serverIns.CheckTimeOut(server.msgPos);
            if (ret != HCCL_SUCCESS) {
                return ret;
            }
        }
        return HCCL_SUCCESS;
    }

    ret = GetCurrentMsg(server);
    if (ret == HCCL_E_AGAIN) {
        if (timingRecorder != nullptr) {
            (void)timingRecorder->IncrementCounter(TimingCounter::EMPTY_POLL);
        }
        ret = server.serverIns.CheckTimeOut(server.msgPos);
        if (ret != HCCL_SUCCESS) {
            return ret;
        }
        return HCCL_SUCCESS;
    }
    CHK_RET(ret);

    HCCL_INFO(
        "Process message for group %u, kernel index %u, message index %u.", groupIdx,
        static_cast<u32>(server.msg.addMsg.v1Msg.seqNum), server.msgPos);
    switch (server.msg.commType.msgType) {
        case ControlMsgType::HCCL_CMD_FINALIZE: {
            bool isLastFinalize = false;
            CHK_RET(RecordFinalizeReceived(
                static_cast<u32>(serverList.size()), finalizeCnt, isLastFinalize, timingRecorder));
            ObserveCommunicationProgress(server, incompletePrepareCnt, timingRecorder);
            if (timingRecorder != nullptr && server.incompletePrepareCnt != 0U) {
                (void)timingRecorder->IncrementCounter(TimingCounter::FINALIZE_BEFORE_COMM_FINISH);
            }
            CHK_RET(FinalizeProcess(server, isLastFinalize, timingRecorder));
            break;
        }
        case ControlMsgType::HCCL_CMD_INTER_GROUP_SYNC:
            CHK_RET(InterGroupSyncProcess(serverList, groupIdx, timingRecorder));
            break;
        case ControlMsgType::HCCL_CMD_INIT:
        case ControlMsgType::HCCL_CMD_BARRIER:
        case ControlMsgType::HCCL_CMD_MAX:
            // Keep the existing control-message path without treating it as a communication Prepare.
            CHK_RET(PrepareProcess(server, expectSeq, incompletePrepareCnt, nullptr));
            break;
        default:
            if (timingRecorder != nullptr) {
                (void)timingRecorder->Record(TimingStage::FIRST_PREPARE_RECEIVED);
            }
            CHK_RET(PrepareProcess(server, expectSeq, incompletePrepareCnt, timingRecorder));
            break;
    }
    return HCCL_SUCCESS;
}

u32 RunDispatcher(void* args[], u32 ctxNum, Hccl::AicpuTimingRecorder* timingRecorder)
{
    std::vector<AscCommServerInfo> serverList{};
    CHK_RET(CreateServerList(args, ctxNum, serverList));
    if (timingRecorder != nullptr) {
        (void)timingRecorder->Record(TimingStage::SERVER_CONTEXT_READY);
    }

    u32 finishCnt = 0U;
    u32 finalizeCnt = 0U;
    u32 expectSeqNum = 0U;
    u32 incompletePrepareCnt = 0U;
    while (finishCnt != serverList.size()) {
        for (u32 i = 0U; i < serverList.size(); ++i) {
            HcclResult ret = GroupServerProcess(
                serverList, i, expectSeqNum, finalizeCnt, finishCnt, incompletePrepareCnt, timingRecorder);
            CHK_RET(serverList[i].serverIns.ErrorDfxProcess(ret));
        }
    }

    if (timingRecorder != nullptr) {
        for (AscCommServerInfo& server : serverList) {
            ObserveCommunicationProgress(server, incompletePrepareCnt, timingRecorder);
        }
        (void)timingRecorder->Record(TimingStage::ALL_SERVERS_FINISHED);
        (void)timingRecorder->Record(TimingStage::REPORT_BEGIN);
        timingRecorder->Report(COMPLETE_STATUS);
    }

    return HCCL_SUCCESS;
}
} // namespace

u32 CommKfcDispatcher::Run(void* args[], u32 ctxNum) { return RunDispatcher(args, ctxNum, nullptr); }

u32 CommKfcDispatcher::Run(void* args[], u32 ctxNum, Hccl::AicpuTimingRecorder& timingRecorder)
{
    return RunDispatcher(args, ctxNum, timingRecorder.IsEnabled() ? &timingRecorder : nullptr);
}
