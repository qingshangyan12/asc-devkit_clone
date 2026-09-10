/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software: you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_OPS_HCCL_ALG_PARSE_H
#define MC2_OPS_HCCL_ALG_PARSE_H

#include <string>
#include <vector>

#include "alg_attrs.h"

namespace mc2_ops_hccl {

// AlgoType 枚举：算法模板类型（与算法名分段一一对应，对齐 hccl alg_parse.h）
enum class AlgoType : uint8_t {
    MESH,
    MESH_2DIE,
    MESH_ONESHOT,
    MESH_TWOSHOT,
    MESH_CONCUR,
    MESH_MULTILINK,
    MESH_CHUNK,
    MESH_CHUNK_TWOSHOT,
    NHR,
    NHR_MULTILINK,
    NHR_AICPU_REDUCE,
    MESH_SINGLE_CHANNEL,
    MESH_CONCURRENT,
    NHR_MULTIJETTY,
    MESH_MULTIJETTY,
    UNKNOWN,
};

// 引擎前缀（Pascal）：出现在算法名首部
inline const std::vector<std::string>& GetEnginePrefixes()
{
    static const std::vector<std::string> prefixes = {"CcuSched", "CcuMS", "Aiv", "Aicpu"};
    return prefixes;
}

// opType 段（Pascal）：引擎段之后
inline const std::vector<std::pair<std::string, HcclCMDType>>& GetOpTypePatterns()
{
    static const std::vector<std::pair<std::string, HcclCMDType>> patterns = {
        {"ReduceScatterV", HcclCMDType::HCCL_CMD_REDUCE_SCATTER_V},
        {"ReduceScatter", HcclCMDType::HCCL_CMD_REDUCE_SCATTER},
        {"AllGatherV", HcclCMDType::HCCL_CMD_ALLGATHER_V},
        {"AllGather", HcclCMDType::HCCL_CMD_ALLGATHER},
        {"AllReduce", HcclCMDType::HCCL_CMD_ALLREDUCE},
        {"AllToAllVC", HcclCMDType::HCCL_CMD_ALLTOALLVC},
        {"AllToAllV", HcclCMDType::HCCL_CMD_ALLTOALLV},
        {"AllToAll", HcclCMDType::HCCL_CMD_ALLTOALL},
        {"Barrier", HcclCMDType::HCCL_CMD_BARRIER},
        {"BatchSendRecv", HcclCMDType::HCCL_CMD_BATCH_SEND_RECV},
        {"Broadcast", HcclCMDType::HCCL_CMD_BROADCAST},
        {"Reduce", HcclCMDType::HCCL_CMD_REDUCE},
        {"Scatter", HcclCMDType::HCCL_CMD_SCATTER},
        {"Send", HcclCMDType::HCCL_CMD_SEND},
        {"Recv", HcclCMDType::HCCL_CMD_RECEIVE}};
    return patterns;
}

// executor 段（Pascal，可连续出现两级）
inline const std::vector<std::string>& GetExecutorNames()
{
    static const std::vector<std::string> names = {"Sole",     "Sequence", "Parallel",
                                                   "PipeLine", "Concur",   "StrictOrdered"};
    return names;
}

// algoTypes 段（Pascal，按最长优先贪婪匹配）
inline const std::vector<std::pair<std::string, AlgoType>>& GetAlgoNameToTypeMap()
{
    static const std::vector<std::pair<std::string, AlgoType>> map = {
        {"MeshChunkTwoShot", AlgoType::MESH_CHUNK_TWOSHOT},
        {"MeshMultiJetty", AlgoType::MESH_MULTIJETTY},
        {"MeshConcurrent", AlgoType::MESH_CONCURRENT},
        {"NHRAicpuReduce", AlgoType::NHR_AICPU_REDUCE},
        {"NHRMultiJetty", AlgoType::NHR_MULTIJETTY},
        {"NHRMultiLink", AlgoType::NHR_MULTILINK},
        {"MeshChunk", AlgoType::MESH_CHUNK},
        {"MeshMultiLink", AlgoType::MESH_MULTILINK},
        {"MeshOneShot", AlgoType::MESH_ONESHOT},
        {"MeshTwoShot", AlgoType::MESH_TWOSHOT},
        {"MeshConcur", AlgoType::MESH_CONCUR},
        {"MeshSingleChannel", AlgoType::MESH_SINGLE_CHANNEL},
        {"Mesh2Die", AlgoType::MESH_2DIE},
        {"Mesh", AlgoType::MESH},
        {"NHR", AlgoType::NHR},
    };
    return map;
}

} // namespace mc2_ops_hccl

#endif // MC2_OPS_HCCL_ALG_PARSE_H
