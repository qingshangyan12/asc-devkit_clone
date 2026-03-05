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
 * \file hccl_ccu_comm.h
 * \brief
 */
#ifndef IMPL_HCCL_CCU_COMM_H
#define IMPL_HCCL_CCU_COMM_H

namespace AscendC {
template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::AllReduce(GM_ADDR sendBuf, GM_ADDR recvBuf, uint64_t count,
                    HcclDataType dataType, HcclReduceOp op, uint8_t repeat)
{
    ASCENDC_HCCL_API_ASSERT(op >= HCCL_REDUCE_SUM && op < HCCL_REDUCE_RESERVED, { return INVALID_HANDLE_ID; },
                            "Call AllReduce failed, param HcclReduceOp is %d, invalid.", static_cast<int32_t>(op));

    return CommonPrepareImpl<commit>({ HcclCMDType::HCCL_CMD_ALLREDUCE, sendBuf, recvBuf, count, dataType, dataType,
                                       op, 0, repeat });
}

template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::AllGather(GM_ADDR sendBuf, GM_ADDR recvBuf, uint64_t sendCount,
                HcclDataType dataType, uint64_t strideCount, uint8_t repeat)
{
    return CommonPrepareImpl<commit>({ HcclCMDType::HCCL_CMD_ALLGATHER, sendBuf, recvBuf, sendCount, dataType, dataType,
                                       HCCL_REDUCE_RESERVED, strideCount, repeat });
}

template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::ReduceScatter(GM_ADDR sendBuf, GM_ADDR recvBuf,
                    uint64_t recvCount, HcclDataType dataType, HcclReduceOp op, uint64_t strideCount, uint8_t repeat)
{
    ASCENDC_HCCL_API_ASSERT(op >= HCCL_REDUCE_SUM && op < HCCL_REDUCE_RESERVED, { return INVALID_HANDLE_ID; },
                            "Call ReduceScatter failed, param HcclReduceOp is %d, invalid.", static_cast<int32_t>(op));
    return CommonPrepareImpl<commit>({ HcclCMDType::HCCL_CMD_REDUCE_SCATTER, sendBuf, recvBuf, recvCount,
                                       dataType, dataType, op, strideCount, repeat });
}

template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::AlltoAll(GM_ADDR sendBuf, GM_ADDR recvBuf, uint64_t dataCount,
                                    HcclDataType dataType, uint64_t strideCount, uint8_t repeat)
{
    return CommonPrepareImpl<commit>({ HcclCMDType::HCCL_CMD_ALLTOALL, sendBuf, recvBuf, dataCount, dataType, dataType,
                                       HCCL_REDUCE_RESERVED, strideCount, repeat });
}

template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::AlltoAllV(GM_ADDR sendBuf, void *sendCounts, void *sdispls,
                                        HcclDataType sendType, GM_ADDR recvBuf, void *recvCounts, void *rdispls,
                                        HcclDataType recvType, uint8_t repeat)
{
    ASCENDC_HCCL_API_ASSERT(sendType == recvType, { return INVALID_HANDLE_ID; },
                            "Call AlltoAllV failed, param sendType[%d] is not equal to recvType[%d], invalid.",
                            static_cast<int32_t>(sendType), static_cast<int32_t>(recvType));
    return CommonPrepareImpl<commit>({ HcclCMDType::HCCL_CMD_ALLTOALLV, sendBuf, recvBuf, 0U, sendType, recvType,
                                       HCCL_REDUCE_RESERVED, 0U, repeat,
                                       {static_cast<uint64_t *>(sendCounts), static_cast<uint64_t *>(sdispls),
                                        static_cast<uint64_t *>(recvCounts), static_cast<uint64_t *>(rdispls)} });
}

template<const auto &config>
template <bool commit>
__aicore__ inline HcclHandle
HcclImpl<HcclServerType::HCCL_SERVER_TYPE_CCU, config>::AlltoAllvWrite(GM_ADDR usrIn, GM_ADDR sendOffsets,
    GM_ADDR sendSizes, uint64_t remoteWinOffset, uint64_t localDataSize)
{
    CommonPrepareParam commonPrepareParam = {HcclCMDType::HCCL_CMD_HALF_ALLTOALLV,
        usrIn,
        usrIn,
        localDataSize,
        HCCL_DATA_TYPE_INT8,
        HCCL_DATA_TYPE_INT8,
        HCCL_REDUCE_RESERVED,
        0,
        1,
        {},
        {reinterpret_cast<uint64_t>(sendOffsets), reinterpret_cast<uint64_t>(sendSizes), remoteWinOffset}
    };

    return CommonPrepareImpl<commit>(commonPrepareParam);
}
}
#endif
