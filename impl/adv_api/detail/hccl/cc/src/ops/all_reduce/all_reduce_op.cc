/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software; you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "all_reduce_op.h"
#include "op_common_ops.h"
#include <algorithm>
#include <future>
#include <map>
#include <string>

using namespace std;
using namespace mc2_ops_hccl;
extern "C" unsigned int LaunchAicpuKernel(OpParam* param);

HcclResult HcclAllReduce(
    void* sendBuf, void* recvBuf, uint64_t count, HcclDataType dataType, HcclReduceOp op, HcclComm comm,
    aclrtStream stream)
{
    if (!HcclCheckCcuEnableOpen() && !HcclCheckAicpuEnableOpen()) {
        return HcclAllReduceInner(sendBuf, recvBuf, count, dataType, op, comm, stream);
    }
    HCCL_INFO("Start to run execute HcclAllReduce");
    if (GetHcommVersion() < 90000000) {
        return HcclAllReduceInner(sendBuf, recvBuf, count, dataType, op, comm, stream);
    }
    DevType deviceType = DevType::DEV_TYPE_COUNT;
    CHK_RET(hrtGetDeviceType(deviceType));
#ifdef MACRO_DEV_TYPE_NEW
    if (deviceType != DevType::DEV_TYPE_950) {
#else
    if (deviceType != DevType::DEV_TYPE_910_95) {
#endif
        return HcclAllReduceInner(sendBuf, recvBuf, count, dataType, op, comm, stream);
    }
    CHK_PRT_RET(count == 0, HCCL_WARNING("input count is 0, return all reduce success"), HCCL_SUCCESS);

    HcclUs startut = TIME_NOW();
    std::string opTag;
    CHK_RET(AllReduceInitAndCheck(comm, sendBuf, recvBuf, count, dataType, op, stream, opTag));

    CHK_RET(AllReduceEntryLog(sendBuf, recvBuf, count, dataType, op, stream, opTag, "HcclAllReduce"));

    CHK_RET_AND_PRINT_IDE(AllReduceOutPlace(sendBuf, recvBuf, count, dataType, op, comm, stream, opTag), opTag.c_str());

    CHK_RET(LogHcclExit("HcclAllReduce", opTag.c_str(), startut));

    return HCCL_SUCCESS;
}

HcclResult HcclAllReduceGraphMode(
    void* sendBuf, void* recvBuf, uint64_t count, HcclDataType dataType, HcclReduceOp op, const char* group,
    aclrtStream stream, const char* tag, void** streams, size_t streamCount, void* scratchMemAddr,
    uint64_t scratchMemSize)
{
    HCCL_INFO("Start to run execute HcclAllReduceGraphMode");
    HcclComm comm = nullptr;
    HCCL_INFO("[HcclAllReduceGraphMode] get group name: %s", group);
    CHK_RET(HcomGetCommHandleByGroup(group, &comm));
    CHK_PRT_RET(count == 0, HCCL_WARNING("input count is 0, return all reduce success"), HCCL_SUCCESS);

    HcclUs startut = TIME_NOW();
    std::string opTag;
    CHK_RET(AllReduceInitAndCheck(comm, sendBuf, recvBuf, count, dataType, op, stream, opTag));

    CHK_RET(HcclCheckTag(tag));

    ResPackGraphMode resPack;
    if (strncpy_s(resPack.tag, sizeof(resPack.tag), tag, sizeof(resPack.tag) - 1) != 0) {
        HCCL_ERROR("failed to fill resPack.tag");
        return HCCL_E_INTERNAL;
    }
    if (streams != nullptr && streamCount > 0) {
        for (size_t i = 0; i < streamCount; i++) {
            resPack.streams.push_back(static_cast<aclrtStream>(streams[i]));
        }
    }
    resPack.scratchMemAddr = scratchMemAddr;
    resPack.scratchMemSize = scratchMemSize;
    std::string tagStr = tag;

    CHK_RET(AllReduceEntryLog(sendBuf, recvBuf, count, dataType, op, stream, opTag, "HcclAllReduceGraphMode"));

    CHK_RET_AND_PRINT_IDE(
        AllReduceOutPlaceGraphMode(sendBuf, recvBuf, count, dataType, op, comm, stream, tagStr, resPack),
        tagStr.c_str());

    CHK_RET(LogHcclExit("HcclAllReduceGraphMode", opTag.c_str(), startut));

    return HCCL_SUCCESS;
}

namespace mc2_ops_hccl {

HcclResult AllReduceInitAndCheck(
    HcclComm comm, void* sendBuf, void* recvBuf, uint64_t count, HcclDataType dataType, HcclReduceOp op,
    aclrtStream stream, std::string& opTag)
{
    CHK_RET(InitEnvConfig());
    CHK_RET(CheckAllReduceInputPara(comm, sendBuf, recvBuf, stream));
    char commName[COMM_INDENTIFIER_MAX_LENGTH];
    CHK_RET(HcclGetCommName(comm, commName));
    opTag = "AllReduce_" + string(commName);
    CHK_RET(HcclCheckTag(opTag.c_str()));
    CHK_RET(CheckCount(count));
    CHK_RET(CheckDataType(dataType, true));
    u32 rankSize = INVALID_VALUE_RANKSIZE;
    CHK_RET(HcclGetRankSize(comm, &rankSize));
    u32 userRank = INVALID_VALUE_RANKID;
    CHK_RET(HcclGetRankId(comm, &userRank));
    CHK_RET_AND_PRINT_IDE(HcomCheckUserRank(rankSize, userRank), opTag.c_str());
    return HCCL_SUCCESS;
}

HcclResult CheckAllReduceInputPara(
    const HcclComm comm, const void* sendBuf, const void* recvBuf, const aclrtStream stream)
{
    RPT_INPUT_ERR(
        stream == nullptr, "EI0003", std::vector<std::string>({"ccl_op", "value", "parameter", "expect"}),
        std::vector<std::string>({"HcclAllReduce", "nullptr", "stream", "non-null pointer"}));
    CHK_PTR_NULL(stream);
    RPT_INPUT_ERR(
        comm == nullptr, "EI0003", std::vector<std::string>({"ccl_op", "value", "parameter", "expect"}),
        std::vector<std::string>({"HcclAllReduce", "nullptr", "comm", "non-null pointer"}));
    CHK_PTR_NULL(comm);
    RPT_INPUT_ERR(
        sendBuf == nullptr, "EI0003", std::vector<std::string>({"ccl_op", "value", "parameter", "expect"}),
        std::vector<std::string>({"HcclAllReduce", "nullptr", "sendBuf", "non-null pointer"}));
    CHK_PTR_NULL(sendBuf);
    RPT_INPUT_ERR(
        recvBuf == nullptr, "EI0003", std::vector<std::string>({"ccl_op", "value", "parameter", "expect"}),
        std::vector<std::string>({"HcclAllReduce", "nullptr", "recvBuf", "non-null pointer"}));
    CHK_PTR_NULL(recvBuf);
    return HCCL_SUCCESS;
}

HcclResult AllReduceOutPlaceCommon(
    void* sendBuf, void* recvBuf, uint64_t count, HcclDataType dataType, HcclReduceOp op, HcclComm comm,
    aclrtStream stream, const std::string& tag, OpMode opMode, const ResPackGraphMode& resPack)
{
    HCCL_INFO("Start to execute AllReduceOutPlaceCommon");
    u32 userRankSize;
    CHK_RET(HcclGetRankSize(comm, &userRankSize));

    u32 perDataSize = DATATYPE_SIZE_TABLE[dataType];
    u64 inputSize = count * perDataSize;
    u64 outputSize = inputSize;

    OpParam param;
    CHK_RET(HcclGetCommName(comm, param.commName));
    param.stream = stream;
    param.opMode = opMode;

    DevType deviceType = DevType::DEV_TYPE_COUNT;
    CHK_RET(hrtGetDeviceType(deviceType));

    int ret = sprintf_s(param.tag, sizeof(param.tag), "%s", tag.c_str());
    if (ret <= 0) {
        HCCL_ERROR("failed to fill param.tag");
        return HCCL_E_INTERNAL;
    }

    param.inputPtr = sendBuf;
    param.inputSize = inputSize;
    param.outputPtr = recvBuf;
    param.outputSize = outputSize;
    param.hcclComm = comm;
    param.DataDes.count = count;
    param.DataDes.dataType = dataType;
    param.reduceType = op;
    param.opType = HcclCMDType::HCCL_CMD_ALLREDUCE;
    param.enableDetour = false;
    param.deviceType = deviceType;

    std::string algName;
    std::unique_ptr<TopoInfoWithNetLayerDetails> topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    CHK_RET(Selector(comm, param, topoInfo, algName));
    if (ShouldUseInnerOp(param.opExecuteConfig)) {
        return HcclAllReduceInner(sendBuf, recvBuf, count, dataType, op, comm, stream);
    }
    if (userRankSize == 1) {
        HCCL_WARNING("[%s] rankSize == 1, enter SingleRankProc", __func__);
        CHK_RET(SingleRankProc(param));
        return HcclResult::HCCL_SUCCESS;
    }
    CHK_RET(HcclExecOp(comm, param, topoInfo, algName, resPack));
    HCCL_INFO("Execute AllReduceOutPlace success.");
    return HCCL_SUCCESS;
}

HcclResult AllReduceOutPlaceGraphMode(
    void* sendBuf, void* recvBuf, uint64_t count, HcclDataType dataType, HcclReduceOp op, HcclComm comm,
    aclrtStream stream, const std::string& tag, const ResPackGraphMode& resPack)
{
    HCCL_INFO("Start to execute AllReduceOutPlaceGraphMode");
    CHK_RET(
        AllReduceOutPlaceCommon(sendBuf, recvBuf, count, dataType, op, comm, stream, tag, OpMode::OFFLOAD, resPack));
    HCCL_INFO("Execute AllReduceOutPlaceGraphMode success.");
    return HCCL_SUCCESS;
}

HcclResult AllReduceOutPlace(
    void* sendBuf, void* recvBuf, uint64_t count, HcclDataType dataType, HcclReduceOp op, HcclComm comm,
    aclrtStream stream, const std::string& tag)
{
    HCCL_INFO("Start to execute AllReduceOutPlace");
    CHK_RET(AllReduceOutPlaceCommon(
        sendBuf, recvBuf, count, dataType, op, comm, stream, tag, OpMode::OPBASE, ResPackGraphMode()));
    HCCL_INFO("Execute AllReduceOutPlace success.");
    return HCCL_SUCCESS;
}

HcclResult AllReduceEntryLog(
    void* sendBuf, void* recvBuf, uint64_t count, HcclDataType dataType, HcclReduceOp op, aclrtStream stream,
    const std::string& tag, const std::string& opName)
{
    if (GetExternalInputHcclEnableEntryLog()) {
        s32 deviceLogicId = 0;
        ACLCHECK(aclrtGetDevice(&deviceLogicId));
        s32 streamId = 0;
        ACLCHECK(aclrtStreamGetId(stream, &streamId));
        char stackLogBuffer[LOG_TMPBUF_SIZE];
        s32 ret = snprintf_s(
            stackLogBuffer, LOG_TMPBUF_SIZE, LOG_TMPBUF_SIZE - 1U,
            "tag[%s], sendBuf[%p], recvBuf[%p], count[%llu], dataType[%s], op[%d], streamId[%d], deviceLogicId[%d]",
            tag.c_str(), sendBuf, recvBuf, count, GetDataTypeEnumStr(dataType).c_str(), static_cast<s32>(op), streamId,
            deviceLogicId);

        CHK_PRT_CONT(ret == -1, HCCL_WARNING("Failed to build log info, tag[%s].", tag.c_str()));
        std::string logInfo = "Entry-" + opName + ":" + std::string(stackLogBuffer);
        HCCL_RUN_INFO("%s", logInfo.c_str());
    }
    return HCCL_SUCCESS;
}
} // namespace mc2_ops_hccl
