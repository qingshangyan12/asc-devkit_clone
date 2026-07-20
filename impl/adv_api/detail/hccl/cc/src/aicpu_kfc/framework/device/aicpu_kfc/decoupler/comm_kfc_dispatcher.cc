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

#include "hccl_msg.h"

#include "common/aicpu_kfc_utils.h"
#include "common/aicpu_timing_recorder.h"
#include "comm_kfc_aicpu_server.h"

using namespace HcclApi;

namespace {
struct AscCommServerInfo {
    CommKfcAicpuServer serverIns;
    HcclMsg msg{};
    std::shared_ptr<HcclMsgExt> extMsg;
    u32 msgPos{0U};
    u32 retryCnt{0U};
    bool finalizeFlag{false};
    bool finishFlag{false};
    AscCommServerInfo(u32 groupIdx) : serverIns(groupIdx) { extMsg = std::make_shared<HcclMsgExt>(); }
};
static constexpr u32 MAX_RETRY_CNT = 10U;
constexpr char FIRST_PREPARE_STAGE[] = "first_prepare";
constexpr char LAST_EXPAND_END_STAGE[] = "last_expand_end";
constexpr char LAST_FINALIZE_STAGE[] = "last_finalize";
constexpr char COMPLETE_STATUS[] = "complete";

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

HcclResult FinalizeProcess(
    AscCommServerInfo& server, u32 serverNum, u32& finalizeCnt, Hccl::AicpuTimingRecorder* timingRecorder)
{
    CHK_PRT_RET(
        finalizeCnt >= serverNum,
        HCCL_ERROR("Invalid finalize count %u, expected less than server number %u.", finalizeCnt, serverNum),
        HCCL_E_INTERNAL);
    ++finalizeCnt;
    const bool isLastFinalize = finalizeCnt == serverNum;
    if (isLastFinalize && timingRecorder != nullptr) {
        (void)timingRecorder->Record(LAST_FINALIZE_STAGE);
    }
    CHK_RET(server.serverIns.Finalize(server.msgPos));
    server.finalizeFlag = true;
    if (isLastFinalize && timingRecorder != nullptr) {
        timingRecorder->Report(COMPLETE_STATUS);
    }
    return HCCL_SUCCESS;
}

HcclResult InterGroupSyncProcess(std::vector<AscCommServerInfo>& serverList, u32 curGroupIdx)
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
        HCCL_INFO("Group sync(%u-%u) will be retried at seq num %u.", curGroupIdx, groupId, server.msgPos);
    } else {
        HCCL_ERROR("Group sync(%u-%u) failed, handle id %d, error code %u.", groupId, handleId, ret);
        return ret;
    }
    return HCCL_SUCCESS;
}

HcclResult PrepareProcess(AscCommServerInfo& server, u32& expectSeqNum, Hccl::AicpuTimingRecorder* timingRecorder)
{
    if (timingRecorder != nullptr) {
        (void)timingRecorder->Record(FIRST_PREPARE_STAGE);
    }
    const u32 seqNum = static_cast<u32>(server.msg.addMsg.v1Msg.seqNum);
    if (expectSeqNum != seqNum) {
        HCCL_INFO("Expect seq id %u but receive %u.", expectSeqNum, seqNum);
        ++(server.retryCnt);
    } else {
        HcclResult ret = server.serverIns.Orchestrate(server.msg, *(server.extMsg), server.msgPos);
        CHK_RET(ret);
        if (timingRecorder != nullptr) {
            (void)timingRecorder->Record(LAST_EXPAND_END_STAGE, Hccl::AicpuTimingRecorder::RecordPolicy::KEEP_LATEST);
        }
        server.msgPos = (server.msgPos + 1U) % HCCL_MSG_CNT;
        ++expectSeqNum;
        server.retryCnt = 0;
    }
    return HCCL_SUCCESS;
}

HcclResult GroupServerProcess(
    std::vector<AscCommServerInfo>& serverList, u32 groupIdx, u32& expectSeq, u32& finalizeCnt, u32& finishCnt,
    Hccl::AicpuTimingRecorder* timingRecorder)
{
    auto& server = serverList[groupIdx];
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
        case ControlMsgType::HCCL_CMD_FINALIZE:
            CHK_RET(FinalizeProcess(server, static_cast<u32>(serverList.size()), finalizeCnt, timingRecorder));
            break;
        case ControlMsgType::HCCL_CMD_INTER_GROUP_SYNC:
            CHK_RET(InterGroupSyncProcess(serverList, groupIdx));
            break;
        default:
            CHK_RET(PrepareProcess(server, expectSeq, timingRecorder));
            break;
    }
    return HCCL_SUCCESS;
}

u32 RunDispatcher(void* args[], u32 ctxNum, Hccl::AicpuTimingRecorder* timingRecorder)
{
    std::vector<AscCommServerInfo> serverList{};
    CHK_RET(CreateServerList(args, ctxNum, serverList));

    u32 finishCnt = 0U;
    u32 finalizeCnt = 0U;
    u32 expectSeqNum = 0U;
    while (finishCnt != serverList.size()) {
        for (u32 i = 0U; i < serverList.size(); ++i) {
            HcclResult ret = GroupServerProcess(serverList, i, expectSeqNum, finalizeCnt, finishCnt, timingRecorder);
            CHK_RET(serverList[i].serverIns.ErrorDfxProcess(ret));
        }
    }

    return HCCL_SUCCESS;
}
} // namespace

u32 CommKfcDispatcher::Run(void* args[], u32 ctxNum) { return RunDispatcher(args, ctxNum, nullptr); }

u32 CommKfcDispatcher::Run(void* args[], u32 ctxNum, Hccl::AicpuTimingRecorder& timingRecorder)
{
    return RunDispatcher(args, ctxNum, &timingRecorder);
}
