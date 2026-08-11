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
#include "topo_host.h"
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
    HCCL_INFO("[j_test][HcclAllReduce] enter mc2 path, count[%llu], dataType[%u], op[%u]", count,
        static_cast<uint32_t>(dataType), static_cast<uint32_t>(op));
    if (GetHcommVersion() < 90000000) { // compat handle
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
    HcclUs startut = TIME_NOW(); // 走老流程的判断时间不统计在内
    CHK_RET(InitEnvConfig());

    OpParam param;
    CHK_PRT_RET(count == 0, HCCL_WARNING("input count is 0, return all reduce success"), HCCL_SUCCESS);
    CHK_RET(CheckAllReduceInputPara(comm, sendBuf, recvBuf, stream));
    u32 rankSize = INVALID_VALUE_RANKSIZE;
    CHK_RET(HcclGetRankSize(comm, &rankSize));
    u32 userRank = INVALID_VALUE_RANKID;
    CHK_RET(HcclGetRankId(comm, &userRank));
    CHK_RET_AND_PRINT_IDE(HcomCheckUserRank(rankSize, userRank), "");
    CHK_RET(CheckCount(count));
    CHK_RET(CheckDataType(dataType, true));
    CHK_RET(HcclGetCommName(comm, param.commName));
    int ret = sprintf_s(param.tag, sizeof(param.tag), "AllReduce_%s", param.commName);
    CHK_PRT_RET((ret <= 0), "failed to fill param.tag", HCCL_E_INTERNAL);
    CHK_RET(HcclCheckTag(param.tag));

    CHK_RET(AllReduceEntryLog(sendBuf, recvBuf, count, dataType, op, stream, param.tag, "HcclAllReduce"));

    CHK_RET(AllReduceOutPlace(param, sendBuf, recvBuf, count, dataType, op, comm, stream, rankSize));

    CHK_RET(LogHcclExit("HcclAllReduce", param.tag, startut));

    return HCCL_SUCCESS;
}

namespace mc2_ops_hccl {
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

static HcclResult PrepareAllReduceParam(
    OpParam& param, void* sendBuf, void* recvBuf, uint64_t count, HcclDataType dataType, HcclReduceOp op,
    HcclComm comm, aclrtStream stream, OpMode opMode)
{
    u32 perDataSize = DATATYPE_SIZE_TABLE[dataType];
    u64 inputSize = count * perDataSize;
    u64 outputSize = inputSize;

    param.stream = stream;
    param.reduceType = op;
    param.opMode = opMode;

    DevType deviceType = DevType::DEV_TYPE_COUNT;
    CHK_RET(hrtGetDeviceType(deviceType));

    param.inputPtr = sendBuf;
    param.inputSize = inputSize;
    param.outputPtr = recvBuf;
    param.outputSize = outputSize;
    param.DataDes.count = count;
    param.DataDes.dataType = dataType;
    param.opType = HcclCMDType::HCCL_CMD_ALLREDUCE;
    param.enableDetour = false;
    param.deviceType = deviceType;

    return HCCL_SUCCESS;
}

HcclResult AllReduceOutPlace(
    OpParam& param, void* sendBuf, void* recvBuf, uint64_t count, HcclDataType dataType, HcclReduceOp op,
    HcclComm comm, aclrtStream stream, u32 userRankSize)
{
    HCCL_INFO("Start to execute AllReduceOutPlace");
    HCCL_INFO("[j_test][AllReduceOutPlace] enter, rankSize[%u], engine[%u]", userRankSize,
        static_cast<uint32_t>(param.engine));
    CHK_RET(PrepareAllReduceParam(param, sendBuf, recvBuf, count, dataType, op, comm, stream, OpMode::OPBASE));

    std::string algName;
    std::unique_ptr<TopoInfoWithNetLayerDetails> topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    CHK_RET(Selector(comm, param, topoInfo, algName));
    HCCL_INFO("[j_test][AllReduceOutPlace] selector returned algName[%s]", algName.c_str());
    if (ShouldUseInnerOp(param.opExecuteConfig)) {
        return HcclAllReduceInner(sendBuf, recvBuf, count, dataType, op, comm, stream);
    }
    if (userRankSize == 1) {
        HCCL_WARNING("[%s] ranksize == 1, enter SingleRankProc", __func__);
        CHK_RET(SingleRankProc(param));
        return HcclResult::HCCL_SUCCESS;
    }
    CHK_RET(HcclExecOp(comm, param, topoInfo, algName));
    HCCL_INFO("[j_test][AllReduceOutPlace] HcclExecOp success, algName[%s]", algName.c_str());
    HCCL_INFO("Execute AllReduceOutPlace success.");
    return HCCL_SUCCESS;
}

HcclResult AllReduceEntryLog(
    void* sendBuf, void* recvBuf, uint64_t count, HcclDataType dataType, HcclReduceOp op, aclrtStream stream,
    const char* tag, const std::string& opName)
{
    if (GetExternalInputHcclEnableEntryLog()) {
        s32 deviceLogicId = 0;
        ACLCHECK(aclrtGetDevice(&deviceLogicId));
        s32 streamId = 0;
        ACLCHECK(aclrtStreamGetId(stream, &streamId));
        char stackLogBuffer[LOG_TMPBUF_SIZE];
        s32 ret = snprintf_s(
            stackLogBuffer, LOG_TMPBUF_SIZE, LOG_TMPBUF_SIZE - 1U,
            "tag[%s], sendBuf[%p], recvBuf[%p], count[%llu], dataType[%s], reduceOp[%s], streamId[%d], "
            "deviceLogicId[%d]",
            tag, sendBuf, recvBuf, count, GetDataTypeEnumStr(dataType).c_str(), GetReduceOpEnumStr(op).c_str(),
            streamId, deviceLogicId);

        CHK_PRT_CONT(ret == -1, HCCL_WARNING("Failed to build log info, tag[%s].", tag));
        std::string logInfo = "Entry-" + opName + ":" + std::string(stackLogBuffer);
        HCCL_RUN_INFO("%s", logInfo.c_str());
    }
    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl
