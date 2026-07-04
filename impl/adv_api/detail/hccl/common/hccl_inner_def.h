/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file hccl_inner_def.h
 * \brief
 */
#ifndef IMPL_HCCL_HCCL_INNER_DEF_H
#define IMPL_HCCL_HCCL_INNER_DEF_H

#include "include/adv_api/hccl/internal/hccl_msg.h"
#include "include/adv_api/hccl/internal/hccl_tiling_msg.h"

using namespace HcclApi;
namespace AscendC {
constexpr int32_t HCCL_FAILED = -1;
constexpr int32_t HCCL_SUCCESS = 0;
constexpr uint8_t HCCL_ONLY_COMPUTE = 1U;
constexpr uint8_t HCCL_ASCEND910B = 1U;
constexpr uint32_t MAX_DCCI_CNT = 64U;
constexpr uint64_t DATA_TYPE_MAP[] = {1, 2, 4, 2, 4, 8, 8, 1, 8, 4, 8, 2, 0, 0, 1, 1, 1, 1, 0};

// Used to calc xor checksum for HcclMsg
constexpr uint32_t HCCL_MSG_DATA_CNT = 16U;
struct DataBlock {
    uint32_t data[HCCL_MSG_DATA_CNT];
};

struct AlltoAllvWriteParamExt {
    uint64_t sendOffsets;
    uint64_t sendSizes;
    uint64_t remoteWinOffset;
};

struct CommonPrepareParam {
    HcclCommType commType;
    GM_ADDR sendBuf;
    GM_ADDR recvBuf;
    uint64_t count;
    HcclDataType dataType;
    HcclDataType dstDataType;
    HcclReduceOp op;
    uint64_t strideCount;
    uint8_t repeat = 1U;
    AlltoAllVParamExt paramExt;       // only used by AlltoAllV
    AlltoAllvWriteParamExt wParamExt; // only used by AlltoAllvWrite
};

struct MemDetails {
    uint64_t size;
    uint64_t addr;
    uint32_t key;
};

struct IbVerbsData {
    MemDetails remoteInput;
    MemDetails remoteOutput;
    MemDetails localInput;
    MemDetails localOutput;
    uint8_t res[24];
};

constexpr uint32_t HCCL_MAX_RANK_NUM = 32U;
constexpr uint32_t HCCL_MAX_RANK_NUM_V310 = 64U;
struct HcclCombineOpParam {
    uint64_t workSpace;     // Address for communication between client and server,
                            // hccl requests and clears
    uint64_t workSpaceSize; // Space for communication between client and server
    uint32_t rankId;        // id of this rank
    uint32_t rankNum;       // num of ranks in this comm group
    uint64_t winSize;       // size of each windows memory
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
    uint64_t windowsIn[HCCL_MAX_RANK_NUM_V310];
    uint64_t windowsOut[HCCL_MAX_RANK_NUM_V310];
    GM_ADDR xnOffset;
    GM_ADDR ckeOffset;
    uint8_t res[16];
    uint32_t opType[MAX_CC_TILING_NUM];
    uint8_t algorithmType[MAX_CC_TILING_NUM];
    uint8_t res2[7744];
#else
    uint64_t windowsIn[HCCL_MAX_RANK_NUM];
    uint64_t windowsOut[HCCL_MAX_RANK_NUM];
    uint8_t res[8328];
#endif
    uint8_t multiFlag;
    __gm__ IbVerbsData* data;
};

namespace HcclContextDef {
struct HcclRankRelationResV2 {
    uint32_t remoteUsrRankId = 0;
    uint32_t remoteWorldRank = 0;
    uint64_t windowsIn = 0;
    uint64_t windowsOut = 0;
    uint64_t windowsExp = 0;
};

struct RemoteResPtr {
    HcclRankRelationResV2* nextHostPtr;
    HcclRankRelationResV2* nextDevicePtr;
};

struct HcclOpResParam {
    uint64_t workSpace;
    uint64_t workSpaceSize;
    uint32_t rankId;
    uint32_t rankNum;
    uint64_t winSize;
    uint64_t localWindowsIn;
    uint64_t localWindowsOut;
    char hcomId[128];
    uint64_t winExpSize;
    uint64_t localWindowsExp;
    uint32_t rWinStart;
};
} // namespace HcclContextDef
} // namespace AscendC
#endif // IMPL_HCCL_HCCL_INNER_DEF_H
