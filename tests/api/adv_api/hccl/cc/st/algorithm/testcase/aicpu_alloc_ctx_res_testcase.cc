/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "sim_communicator.h"
#include "sim_world.h"
#include "hccl_alloc_ctx_res.h"
#include "hccl_msg.h"
#include "comm_kfc_open_kernel_adapter.h"
#include "op_common.h"
#include "utils.h"

namespace mc2_ops_hccl {
extern bool g_rejectDirectAclrtMemcpy;
extern uint32_t g_aclrtMemcpyCallCount;
extern uint32_t g_hcclEngineCtxCopyCallCount;
} // namespace mc2_ops_hccl

HcclResult LaunchOpenOpParamDataImpl(
    std::vector<uint8_t>&, const mc2_ops_hccl::AlgResourceCtxSerializable&)
{
    return HCCL_SUCCESS;
}

namespace checker {
namespace {
using namespace HcclSim;
using namespace mc2_ops_hccl;

class AicpuAllocCtxResTest : public testing::Test {
protected:
    void SetUp() override
    {
        const TopoMeta topoMeta{{{0U}}};
        SimWorld::Global()->Init(topoMeta, DevType::DEV_TYPE_950);
        ASSERT_EQ(Sim_HcclCommInitClusterInfo(topoMeta, 0U, &comm_), HCCL_SUCCESS);
        ASSERT_NE(comm_, nullptr);
        g_rejectDirectAclrtMemcpy = false;
        g_aclrtMemcpyCallCount = 0U;
        g_hcclEngineCtxCopyCallCount = 0U;
    }

    void TearDown() override
    {
        g_rejectDirectAclrtMemcpy = false;
        g_aclrtMemcpyCallCount = 0U;
        g_hcclEngineCtxCopyCallCount = 0U;
        if (comm_ != nullptr) {
            EXPECT_EQ(HcclCommDestroy(comm_), HCCL_SUCCESS);
            comm_ = nullptr;
        }
        SimWorld::Global()->Deinit();
    }

    HcclComm comm_ = nullptr;
};

TEST_F(AicpuAllocCtxResTest, CaptureSafeCopyCoversOpParamAndOpResCtx)
{
    constexpr uint32_t tilingOffset = 64U;
    Mc2InitTilingInner initTiling{};
    initTiling.offset[0] = tilingOffset;

    Mc2CcTilingInner ccTiling{};
    ccTiling.commEngine = static_cast<uint8_t>(OpExecuteConfig::AICPU_TS);
    const void* ccTilingList[MAX_CC_TILING_NUM] = {&ccTiling};

    OpParam opParam{};
    opParam.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    const std::vector<OpParam> opParams{opParam};

    g_rejectDirectAclrtMemcpy = true;
    void* opResCtxPtr = nullptr;
    ASSERT_EQ(
        HcclAllocOpResCtx(comm_, "aicpu_capture_safe_copy", opParams, &initTiling, ccTilingList, &opResCtxPtr),
        HCCL_SUCCESS);

    EXPECT_EQ(g_aclrtMemcpyCallCount, 0U);
    EXPECT_EQ(g_hcclEngineCtxCopyCallCount, 2U);
    ASSERT_NE(opResCtxPtr, nullptr);

    const auto* opResCtx = static_cast<const OpResCtx*>(opResCtxPtr);
    EXPECT_EQ(opResCtx->rankId, 0U);
    EXPECT_EQ(opResCtx->rankSize, 1U);
    EXPECT_EQ(opResCtx->algInfo[0].offset, tilingOffset);
    ASSERT_NE(opResCtx->algInfo[0].opParam, 0U);

    const auto* copiedOpParam = reinterpret_cast<const OpParam*>(opResCtx->algInfo[0].opParam);
    EXPECT_EQ(copiedOpParam->opType, HcclCMDType::HCCL_CMD_ALLGATHER);
}

TEST_F(AicpuAllocCtxResTest, OpParamAllocationIncludesVariableMemoryAndPublishesSize)
{
    constexpr uint32_t tilingOffset = 64U;
    constexpr uint64_t variableSize = 32U;
    Mc2InitTilingInner initTiling{};
    initTiling.offset[0] = tilingOffset;

    Mc2CcTilingInner ccTiling{};
    ccTiling.commEngine = static_cast<uint8_t>(OpExecuteConfig::AICPU_TS);
    const void* ccTilingList[MAX_CC_TILING_NUM] = {&ccTiling};

    OpParam opParam{};
    opParam.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    opParam.varMemSize = variableSize;
    const std::vector<OpParam> opParams{opParam};

    void* opResCtxPtr = nullptr;
    ASSERT_EQ(
        HcclAllocOpResCtx(comm_, "aicpu_variable_op_param", opParams, &initTiling, ccTilingList, &opResCtxPtr),
        HCCL_SUCCESS);
    ASSERT_NE(opResCtxPtr, nullptr);

    const auto* opResCtx = static_cast<const OpResCtx*>(opResCtxPtr);
    EXPECT_EQ(opResCtx->algInfo[0].opParamSize, sizeof(OpParam) + variableSize);
    ASSERT_NE(opResCtx->algInfo[0].opParam, 0U);
    const auto* copiedOpParam = reinterpret_cast<const OpParam*>(opResCtx->algInfo[0].opParam);
    EXPECT_EQ(copiedOpParam->varMemSize, variableSize);
    const auto* serialized = reinterpret_cast<const uint8_t*>(copiedOpParam);
    for (size_t i = sizeof(OpParam); i < opResCtx->algInfo[0].opParamSize; ++i) {
        EXPECT_EQ(serialized[i], 0U);
    }
}

std::vector<uint8_t> MakeSerializedOpParam(uint64_t variableSize, size_t allocationSize)
{
    OpParam param{};
    param.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    param.varMemSize = variableSize;
    EXPECT_EQ(strcpy_s(param.commName, sizeof(param.commName), "test_comm"), EOK);
    std::vector<uint8_t> serialized(allocationSize, 0U);
    if (serialized.size() >= sizeof(param)) {
        EXPECT_EQ(memcpy_s(serialized.data(), serialized.size(), &param, sizeof(param)), EOK);
    }
    return serialized;
}

TEST(OpenOpParamBoundaryTest, LoadRejectsAllocationShorterThanFixedHeader)
{
    uint64_t storage = 0U;
    std::string commName;
    std::vector<uint8_t> loaded;

    EXPECT_EQ(
        LoadOpenOpParamData(reinterpret_cast<uint64_t>(&storage), sizeof(storage), commName, loaded), HCCL_E_PARA);
}

TEST(OpenOpParamBoundaryTest, LoadRejectsVariableSizeAdditionOverflow)
{
    std::vector<uint8_t> serialized = MakeSerializedOpParam(UINT64_MAX, sizeof(OpParam));
    std::string commName;
    std::vector<uint8_t> loaded;

    EXPECT_EQ(
        LoadOpenOpParamData(
            reinterpret_cast<uint64_t>(serialized.data()), serialized.size(), commName, loaded),
        HCCL_E_PARA);
}

TEST(OpenOpParamBoundaryTest, LoadRejectsDeclaredSizePastAllocation)
{
    std::vector<uint8_t> serialized = MakeSerializedOpParam(1U, sizeof(OpParam));
    std::string commName;
    std::vector<uint8_t> loaded;

    EXPECT_EQ(
        LoadOpenOpParamData(
            reinterpret_cast<uint64_t>(serialized.data()), serialized.size(), commName, loaded),
        HCCL_E_PARA);
}

TEST(OpenOpParamBoundaryTest, LoadAcceptsExactSerializedAllocation)
{
    constexpr uint64_t variableSize = 16U;
    std::vector<uint8_t> serialized = MakeSerializedOpParam(variableSize, sizeof(OpParam) + variableSize);
    std::string commName;
    std::vector<uint8_t> loaded;

    ASSERT_EQ(
        LoadOpenOpParamData(
            reinterpret_cast<uint64_t>(serialized.data()), serialized.size(), commName, loaded),
        HCCL_SUCCESS);
    EXPECT_EQ(commName, "test_comm");
    EXPECT_EQ(loaded, serialized);
}

TEST(OpenOpParamBoundaryTest, FormatRejectsShortAndIncompleteBaseBuffers)
{
    HcclApi::HcclMsg msg{};
    msg.commType.prepareType = AscendC::HcclCMDType::HCCL_CMD_ALLGATHER;
    msg.addMsg.v1Msg.hcclDataType = AscendC::HcclDataType::HCCL_DATA_TYPE_INT8;
    HcclApi::HcclMsgExt extMsg{};
    std::vector<uint8_t> runOpParam;

    std::vector<uint8_t> shortBase(sizeof(OpParam) - 1U, 0U);
    EXPECT_EQ(
        FormatOpenOpParamDataFromMsg(shortBase, msg, extMsg, 1U, 0U, nullptr, runOpParam), HCCL_E_PARA);

    std::vector<uint8_t> incompleteBase = MakeSerializedOpParam(1U, sizeof(OpParam));
    EXPECT_EQ(
        FormatOpenOpParamDataFromMsg(incompleteBase, msg, extMsg, 1U, 0U, nullptr, runOpParam), HCCL_E_PARA);
}

TEST(OpenOpParamBoundaryTest, FormatAcceptsExactBaseBuffer)
{
    std::vector<uint8_t> baseOpParam = MakeSerializedOpParam(0U, sizeof(OpParam));
    HcclApi::HcclMsg msg{};
    msg.commType.prepareType = AscendC::HcclCMDType::HCCL_CMD_ALLGATHER;
    msg.addMsg.v1Msg.hcclDataType = AscendC::HcclDataType::HCCL_DATA_TYPE_INT8;
    HcclApi::HcclMsgExt extMsg{};
    std::vector<uint8_t> runOpParam;

    ASSERT_EQ(
        FormatOpenOpParamDataFromMsg(baseOpParam, msg, extMsg, 1U, 0U, nullptr, runOpParam), HCCL_SUCCESS);
    EXPECT_EQ(runOpParam.size(), sizeof(OpParam));
}

TEST_F(AicpuAllocCtxResTest, ConvertAlltoAllParamFillsDisplacements)
{
    constexpr u32 rankSize = 3U;
    constexpr u64 recvCount = 5U;
    std::vector<u64> sdispls(rankSize);
    std::vector<u64> rdispls(rankSize);

    ASSERT_EQ(ConvertAlltoAllParam(recvCount, rankSize, sdispls, rdispls), HCCL_SUCCESS);
    EXPECT_EQ(sdispls, (std::vector<u64>{0U, 5U, 10U}));
    EXPECT_EQ(rdispls, (std::vector<u64>{0U, 5U, 10U}));
}

TEST_F(AicpuAllocCtxResTest, ConvertAlltoAllParamRejectsShortSendDisplacements)
{
    constexpr u32 rankSize = 3U;
    std::vector<u64> sdispls(rankSize - 1U);
    std::vector<u64> rdispls(rankSize);

    EXPECT_EQ(ConvertAlltoAllParam(1U, rankSize, sdispls, rdispls), HCCL_E_PARA);
}

TEST_F(AicpuAllocCtxResTest, ConvertAlltoAllParamRejectsShortReceiveDisplacements)
{
    constexpr u32 rankSize = 3U;
    std::vector<u64> sdispls(rankSize);
    std::vector<u64> rdispls(rankSize - 1U);

    EXPECT_EQ(ConvertAlltoAllParam(1U, rankSize, sdispls, rdispls), HCCL_E_PARA);
}

TEST(AicpuHostBoundaryTest, ResourceCtxDeserializeRejectsInvalidTopoInfoSize)
{
    AlgResourceCtxSerializable source{};
    source.topoInfo.userRank = 7U;
    std::vector<char> data = source.Serialize();
    ASSERT_GT(source.topoInfoSeqSize, 0U);
    ASSERT_GT(data.size(), static_cast<size_t>(source.topoInfoSeqSize) + sizeof(source.topoInfoSeqSize));

    const size_t sizeFieldOffset = data.size() - source.topoInfoSeqSize - sizeof(source.topoInfoSeqSize);
    const u32 invalidTopoInfoSize = static_cast<u32>(data.size() + 1U);
    ASSERT_EQ(
        memcpy_s(
            data.data() + sizeFieldOffset, data.size() - sizeFieldOffset, &invalidTopoInfoSize,
            sizeof(invalidTopoInfoSize)),
        EOK);

    AlgResourceCtxSerializable restored{};
    restored.topoInfo.userRank = 9U;
    restored.DeSerialize(data);

    const TopoInfoWithNetLayerDetails emptyTopo{};
    EXPECT_EQ(restored.topoInfoSeqSize, 0U);
    EXPECT_EQ(restored.topoInfo.userRank, emptyTopo.userRank);
}

TEST(AicpuHostBoundaryTest, StringFormatHandlesSuccessAndTruncationFailure)
{
    EXPECT_EQ(mc2_ops_hccl::StringFormat("rank-%u", 3U), "rank-3");

    const std::string oversized(BUFSIZ * 2U, 'x');
    EXPECT_TRUE(mc2_ops_hccl::StringFormat("%s", oversized.c_str()).empty());
}

TEST(AicpuHostBoundaryTest, CcuChannelRequestRejectsCapacityOverflowBeforeAcquire)
{
    AlgResourceRequest request{};
    CcuKernelInfo kernelInfo{};
    kernelInfo.channels.resize(CCU_MAX_RANK_SIZE + 1U);
    request.ccuKernelInfos.emplace_back(std::move(kernelInfo));
    OpParam param{};

    EXPECT_EQ(HcclGetChannelForCcu(nullptr, param, request), HCCL_E_PARA);
}

TEST(AicpuHostBoundaryTest, HcclMsgRankSizeContractMatchesExtendedMessageCapacity)
{
    EXPECT_FALSE(HcclApi::IsValidHcclMsgRankSize(0U));
    EXPECT_TRUE(HcclApi::IsValidHcclMsgRankSize(1U));
    EXPECT_TRUE(HcclApi::IsValidHcclMsgRankSize(HcclApi::HCCL_MAX_RANK_NUM_V2));
    EXPECT_FALSE(HcclApi::IsValidHcclMsgRankSize(HcclApi::HCCL_MAX_RANK_NUM_V2 + 1U));
}

TEST(BinaryStreamBoundaryTest, ValidContainersRoundTrip)
{
    const std::string expectedString = "rank-sequence";
    const std::vector<u32> expectedVector{1U, 4U, 9U};
    const std::map<u32, std::string> expectedMap{{1U, "one"}, {2U, "two"}};

    BinaryStream output;
    output << expectedString << expectedVector << expectedMap;
    const std::string payload = output.GetString();
    std::vector<char> data(payload.begin(), payload.end());

    std::string actualString;
    std::vector<u32> actualVector;
    std::map<u32, std::string> actualMap;
    BinaryStream input(data);
    input >> actualString >> actualVector >> actualMap;

    EXPECT_TRUE(input.Good());
    EXPECT_EQ(actualString, expectedString);
    EXPECT_EQ(actualVector, expectedVector);
    EXPECT_EQ(actualMap, expectedMap);
}

TEST(BinaryStreamBoundaryTest, TruncatedScalarStopsFurtherReads)
{
    std::vector<char> data(sizeof(u32) - 1U, 0);
    BinaryStream input(data);
    u32 first = 7U;
    u32 second = 11U;

    input >> first >> second;

    EXPECT_FALSE(input.Good());
    EXPECT_EQ(first, 7U);
    EXPECT_EQ(second, 11U);
}

TEST(BinaryStreamBoundaryTest, RejectsOversizedStringWithoutChangingOutput)
{
    constexpr size_t invalidSize = BinaryStream::MAX_CONTAINER_BYTES + 1U;
    std::vector<char> data(sizeof(invalidSize));
    ASSERT_EQ(memcpy_s(data.data(), data.size(), &invalidSize, sizeof(invalidSize)), EOK);
    BinaryStream input(data);
    std::string value = "unchanged";

    input >> value;

    EXPECT_FALSE(input.Good());
    EXPECT_EQ(value, "unchanged");
}

TEST(BinaryStreamBoundaryTest, RejectsTruncatedVectorWithoutChangingOutput)
{
    constexpr size_t elementCount = 2U;
    constexpr u32 onlyElement = 5U;
    std::vector<char> data(sizeof(elementCount) + sizeof(onlyElement));
    ASSERT_EQ(memcpy_s(data.data(), data.size(), &elementCount, sizeof(elementCount)), EOK);
    ASSERT_EQ(
        memcpy_s(data.data() + sizeof(elementCount), sizeof(onlyElement), &onlyElement, sizeof(onlyElement)), EOK);
    BinaryStream input(data);
    std::vector<u32> value{9U};

    input >> value;

    EXPECT_FALSE(input.Good());
    EXPECT_EQ(value, (std::vector<u32>{9U}));
}

TEST(BinaryStreamBoundaryTest, RejectsExcessiveElementCountBeforeAllocation)
{
    constexpr size_t invalidSize = BinaryStream::MAX_CONTAINER_ELEMENTS + 1U;
    std::vector<char> data(sizeof(invalidSize));
    ASSERT_EQ(memcpy_s(data.data(), data.size(), &invalidSize, sizeof(invalidSize)), EOK);
    BinaryStream input(data);
    std::vector<uint8_t> value;

    input >> value;

    EXPECT_FALSE(input.Good());
    EXPECT_TRUE(value.empty());
}

struct LargeBinaryValue {
    uint8_t data[256];
};

TEST(BinaryStreamBoundaryTest, RejectsContainerByteSizeOverflowBeforeAllocation)
{
    constexpr size_t invalidSize = BinaryStream::MAX_CONTAINER_BYTES / sizeof(LargeBinaryValue) + 1U;
    static_assert(invalidSize <= BinaryStream::MAX_CONTAINER_ELEMENTS, "test must reach the byte-size boundary");
    std::vector<char> data(sizeof(invalidSize));
    ASSERT_EQ(memcpy_s(data.data(), data.size(), &invalidSize, sizeof(invalidSize)), EOK);
    BinaryStream input(data);
    std::vector<LargeBinaryValue> value;

    input >> value;

    EXPECT_FALSE(input.Good());
    EXPECT_TRUE(value.empty());
}

TEST(BinaryStreamBoundaryTest, TruncatedMapDoesNotCommitPartialResult)
{
    constexpr size_t elementCount = 1U;
    constexpr u32 keyOnly = 3U;
    std::vector<char> data(sizeof(elementCount) + sizeof(keyOnly));
    ASSERT_EQ(memcpy_s(data.data(), data.size(), &elementCount, sizeof(elementCount)), EOK);
    ASSERT_EQ(memcpy_s(data.data() + sizeof(elementCount), sizeof(keyOnly), &keyOnly, sizeof(keyOnly)), EOK);
    BinaryStream input(data);
    std::map<u32, u32> value{{8U, 13U}};

    input >> value;

    EXPECT_FALSE(input.Good());
    EXPECT_EQ(value, (std::map<u32, u32>{{8U, 13U}}));
}

} // namespace
} // namespace checker
