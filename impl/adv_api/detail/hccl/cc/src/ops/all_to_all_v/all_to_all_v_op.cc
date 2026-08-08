/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "all_to_all_v_op.h"
#include "op_common_ops.h"
#include <algorithm>
#include <future>
#include <map>
#include <string>

using namespace std;
using namespace mc2_ops_hccl;

namespace mc2_ops_hccl {

HcclResult CheckAllToAllVInputPara(
    const HcclComm comm, const void* sendBuf, const void* recvBuf, const aclrtStream stream)
{
    RPT_INPUT_ERR(
        stream == nullptr, "EI0003", std::vector<std::string>({"ccl_op", "value", "parameter", "expect"}),
        std::vector<std::string>({"HcclAllToAllV", "nullptr", "stream", "non-null pointer"}));
    CHK_PTR_NULL(stream);
    RPT_INPUT_ERR(
        comm == nullptr, "EI0003", std::vector<std::string>({"ccl_op", "value", "parameter", "expect"}),
        std::vector<std::string>({"HcclAllToAllV", "nullptr", "comm", "non-null pointer"}));
    CHK_PTR_NULL(comm);
    RPT_INPUT_ERR(
        sendBuf == nullptr, "EI0003", std::vector<std::string>({"ccl_op", "value", "parameter", "expect"}),
        std::vector<std::string>({"HcclAllToAllV", "nullptr", "sendBuf", "non-null pointer"}));
    CHK_PTR_NULL(sendBuf);
    RPT_INPUT_ERR(
        recvBuf == nullptr, "EI0003", std::vector<std::string>({"ccl_op", "value", "parameter", "expect"}),
        std::vector<std::string>({"HcclAllToAllV", "nullptr", "recvBuf", "non-null pointer"}));
    CHK_PTR_NULL(recvBuf);
    return HCCL_SUCCESS;
}

HcclResult AllToAllVInitAndCheck(
    HcclComm comm, void* sendBuf, void* recvBuf, HcclDataType sendType, HcclDataType recvType, aclrtStream stream,
    std::string& opTag)
{
    CHK_RET(InitEnvConfig());
    CHK_RET(CheckAllToAllVInputPara(comm, sendBuf, recvBuf, stream));
    char commName[COMM_INDENTIFIER_MAX_LENGTH];
    CHK_RET(HcclGetCommName(comm, commName));
    opTag = "AllToAllV_" + string(commName);
    CHK_RET(HcclCheckTag(opTag.c_str()));
    CHK_RET(CheckDataType(sendType, false));
    CHK_RET(CheckDataType(recvType, false));
    u32 rankSize = INVALID_VALUE_RANKSIZE;
    CHK_RET(HcclGetRankSize(comm, &rankSize));
    u32 userRank = INVALID_VALUE_RANKID;
    CHK_RET(HcclGetRankId(comm, &userRank));
    CHK_RET_AND_PRINT_IDE(HcomCheckUserRank(rankSize, userRank), opTag.c_str());
    return HCCL_SUCCESS;
}

HcclResult AllToAllVEntryLog(
    void* sendBuf, void* recvBuf, HcclDataType sendType, HcclDataType recvType, aclrtStream stream,
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
            "tag[%s], sendBuf[%p], recvBuf[%p], sendType[%s], recvType[%s], streamId[%d], deviceLogicId[%d]",
            tag.c_str(), sendBuf, recvBuf, GetDataTypeEnumStr(sendType).c_str(), GetDataTypeEnumStr(recvType).c_str(),
            streamId, deviceLogicId);
        CHK_PRT_CONT(ret == -1, HCCL_WARNING("Failed to build log info, tag[%s].", tag.c_str()));
        std::string logInfo = "Entry-" + opName + ":" + std::string(stackLogBuffer);
        HCCL_RUN_INFO("%s", logInfo.c_str());
    }
    return HCCL_SUCCESS;
}

HcclResult AllToAllVOutPlaceCommon(
    void* sendBuf, void* recvBuf, const void* sendCounts, const void* sdispls, const void* recvCounts,
    const void* rdispls, HcclDataType sendType, HcclDataType recvType, HcclComm comm, aclrtStream stream,
    const std::string& tag, OpMode opMode, const ResPackGraphMode& resPack)
{
    HCCL_INFO("Start to execute AllToAllVOutPlaceCommon");
    u32 userRankSize;
    CHK_RET(HcclGetRankSize(comm, &userRankSize));

    u32 perSendDataSize = DATATYPE_SIZE_TABLE[sendType];
    u32 perRecvDataSize = DATATYPE_SIZE_TABLE[recvType];

    u64 inputSize = 0;
    u64 outputSize = 0;
    for (u32 i = 0; i < userRankSize; i++) {
        inputSize = std::max(inputSize, static_cast<u64>(
            reinterpret_cast<const int64_t*>(sendCounts)[i] * perSendDataSize +
            reinterpret_cast<const int64_t*>(sdispls)[i] * perSendDataSize));
        outputSize = std::max(outputSize, static_cast<u64>(
            reinterpret_cast<const int64_t*>(recvCounts)[i] * perRecvDataSize +
            reinterpret_cast<const int64_t*>(rdispls)[i] * perRecvDataSize));
    }

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
    param.DataDes.dataType = sendType;
    param.DataDes.outputType = recvType;
    param.opType = HcclCMDType::HCCL_CMD_HALF_ALLTOALLV;
    param.enableDetour = false;
    param.deviceType = deviceType;

    param.all2AllVDataDes.sendType = sendType;
    param.all2AllVDataDes.recvType = recvType;
    param.all2AllVDataDes.sendCounts = const_cast<void*>(sendCounts);
    param.all2AllVDataDes.recvCounts = const_cast<void*>(recvCounts);
    param.all2AllVDataDes.sdispls = const_cast<void*>(sdispls);
    param.all2AllVDataDes.rdispls = const_cast<void*>(rdispls);

    std::string algName;
    std::unique_ptr<TopoInfoWithNetLayerDetails> topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    CHK_RET(Selector(comm, param, topoInfo, algName));

    if (userRankSize == 1) {
        HCCL_WARNING("[%s] rankSize == 1, enter SingleRankProc", __func__);
        CHK_RET(SingleRankProc(param));
        return HcclResult::HCCL_SUCCESS;
    }
    CHK_RET(HcclExecOp(comm, param, topoInfo, algName, resPack));
    HCCL_INFO("Execute AllToAllVOutPlace success.");
    return HCCL_SUCCESS;
}

HcclResult AllToAllVOutPlace(
    void* sendBuf, void* recvBuf, const void* sendCounts, const void* sdispls, const void* recvCounts,
    const void* rdispls, HcclDataType sendType, HcclDataType recvType, HcclComm comm, aclrtStream stream,
    const std::string& tag)
{
    HCCL_INFO("Start to execute AllToAllVOutPlace");
    CHK_RET(AllToAllVOutPlaceCommon(
        sendBuf, recvBuf, sendCounts, sdispls, recvCounts, rdispls, sendType, recvType, comm, stream, tag,
        OpMode::OPBASE, ResPackGraphMode()));
    HCCL_INFO("Execute AllToAllVOutPlace success.");
    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl
