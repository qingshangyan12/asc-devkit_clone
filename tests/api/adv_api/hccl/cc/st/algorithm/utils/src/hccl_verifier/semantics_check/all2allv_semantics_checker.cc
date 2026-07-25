/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "all2allv_semantics_checker.h"

#include <algorithm>
#include <map>
#include <vector>
#include "log.h"

namespace HcclSim {

HcclResult TaskCheckAll2AllVSemantics(
    std::map<RankId, RankMemorySemantics>& allRankMemSemantics, All2AllDataDesTag& all2AllDataDes)
{
    const u32 rankSize = allRankMemSemantics.size();
    const size_t matrixSize = static_cast<size_t>(rankSize) * rankSize;
    if (all2AllDataDes.sendCountMatrix.size() != matrixSize || all2AllDataDes.sdispls.size() != matrixSize ||
        all2AllDataDes.rdispls.size() != matrixSize) {
        HCCL_ERROR("AllToAllV semantic matrices do not match rank size %u", rankSize);
        return HcclResult::HCCL_E_PARA;
    }

    const u64 sendTypeSize = SIZE_TABLE[all2AllDataDes.sendType];
    const u64 recvTypeSize = SIZE_TABLE[all2AllDataDes.recvType];
    for (RankId dstRank = 0; dstRank < rankSize; ++dstRank) {
        auto rankIt = allRankMemSemantics.find(dstRank);
        if (rankIt == allRankMemSemantics.end()) {
            HCCL_ERROR("Missing rank %u mem semantics", dstRank);
            return HcclResult::HCCL_E_PARA;
        }

        std::vector<u64> checkedSize(rankSize, 0);
        const auto& outputSemantics = rankIt->second[BufferType::OUTPUT];
        for (const auto& semantic : outputSemantics) {
            bool matched = false;
            for (RankId srcRank = 0; srcRank < rankSize && !matched; ++srcRank) {
                const size_t countIndex = static_cast<size_t>(srcRank) * rankSize + dstRank;
                const size_t displIndex = static_cast<size_t>(dstRank) * rankSize + srcRank;
                const u64 expectedSize = all2AllDataDes.sendCountMatrix[countIndex] * recvTypeSize;
                const u64 expectedDstStart = all2AllDataDes.rdispls[displIndex] * recvTypeSize;
                const u64 semanticEnd = semantic.startAddr + semantic.size;
                const u64 expectedDstEnd = expectedDstStart + expectedSize;
                if (semantic.startAddr < expectedDstStart || semanticEnd > expectedDstEnd) {
                    continue;
                }
                const u64 expectedSrcStart = all2AllDataDes.sdispls[countIndex] * sendTypeSize;
                if (semantic.srcBufs.size() != 1 || semantic.srcBufs.begin()->rankId != srcRank ||
                    semantic.srcBufs.begin()->bufType != BufferType::INPUT ||
                    semantic.srcBufs.begin()->srcAddr != expectedSrcStart + semantic.startAddr - expectedDstStart) {
                    HCCL_ERROR(
                        "[rankId:%u]Invalid AllToAllV semantic from rank %u: %s", dstRank, srcRank,
                        semantic.Describe().c_str());
                    return HcclResult::HCCL_E_PARA;
                }
                checkedSize[srcRank] += semantic.size;
                matched = true;
            }
            if (!matched) {
                HCCL_ERROR("[rankId:%u]Unexpected AllToAllV output semantic: %s", dstRank, semantic.Describe().c_str());
                return HcclResult::HCCL_E_PARA;
            }
        }
        for (RankId srcRank = 0; srcRank < rankSize; ++srcRank) {
            const u64 expectedSize = all2AllDataDes.sendCountMatrix[srcRank * rankSize + dstRank] * recvTypeSize;
            if (checkedSize[srcRank] != expectedSize) {
                HCCL_ERROR(
                    "[rankId:%u]AllToAllV data from rank %u has size %llu, expected %llu", dstRank, srcRank,
                    checkedSize[srcRank], expectedSize);
                return HcclResult::HCCL_E_PARA;
            }
        }
    }

    return HcclResult::HCCL_SUCCESS;
}

} // namespace HcclSim
