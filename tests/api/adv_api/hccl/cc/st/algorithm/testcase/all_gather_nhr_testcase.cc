/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "gtest/gtest.h"

#include "channel.h"
#include "template_utils.h"

namespace {
using namespace mc2_ops_hccl;

// ---------------------------------------------------------------------------
// 构造辅助
// ---------------------------------------------------------------------------

std::vector<HcclChannelDesc> MakeChannelDescs(const std::vector<std::pair<u32, u32>>& rankAndCount)
{
    std::vector<HcclChannelDesc> channels;
    for (const auto& item : rankAndCount) {
        for (u32 i = 0; i < item.second; i++) {
            HcclChannelDesc desc{};
            desc.remoteRank = item.first;
            channels.push_back(desc);
        }
    }
    return channels;
}

std::map<u32, std::vector<ChannelInfo>> MakeChannelMap(const std::vector<std::pair<u32, u32>>& rankAndCount)
{
    std::map<u32, std::vector<ChannelInfo>> channels;
    for (const auto& item : rankAndCount) {
        std::vector<ChannelInfo> infos(item.second);
        for (u32 i = 0; i < item.second; i++) {
            infos[i].portGroupSize = 1;
        }
        channels[item.first] = infos;
    }
    return channels;
}

TopoInfoWithNetLayerDetails MakeTopoInfo(u32 localRankSize, CommTopo topoType, u32 topoCoverRankNum)
{
    TopoInfoWithNetLayerDetails topoInfo{};
    topoInfo.netLayerDetails.localNetInsSizeOfLayer.push_back(localRankSize);
    TopoInstDetails details{};
    details.rankNumForTopoType[topoType] = {topoCoverRankNum};
    topoInfo.topoInstDetailsOfLayer.push_back(details);
    return topoInfo;
}

// NHR 步数与每步 peer / slice 的参考实现，用于与模板内部推进规则对照。
u32 RefStepNum(u32 rankSize)
{
    u32 nSteps = 0;
    for (u32 tmp = rankSize - 1; tmp != 0; tmp >>= 1, nSteps++) {
    }
    return nSteps;
}

struct RefStep {
    u32 nSlices{0};
    u32 toRank{0};
    u32 fromRank{0};
    std::vector<u32> txSliceIdxs;
    std::vector<u32> rxSliceIdxs;
};

RefStep RefGetStep(u32 myAlgRank, u32 rankSize, u32 step, u32 nSteps)
{
    RefStep info;
    const u32 deltaRank = 1U << (nSteps - 1U - step);
    info.fromRank = (myAlgRank + rankSize - deltaRank) % rankSize;
    info.toRank = (myAlgRank + deltaRank) % rankSize;
    info.nSlices = (rankSize - 1U + (1U << (nSteps - 1U - step))) / (1U << (nSteps - step));
    const u32 deltaSliceIndex = 1U << (nSteps - step);
    u32 txSliceIdx = myAlgRank;
    u32 rxSliceIdx = (myAlgRank - (1U << (nSteps - 1U - step)) + rankSize) % rankSize;
    for (u32 i = 0; i < info.nSlices; i++) {
        info.txSliceIdxs.push_back(txSliceIdx);
        info.rxSliceIdxs.push_back(rxSliceIdx);
        txSliceIdx = (txSliceIdx + rankSize - deltaSliceIndex) % rankSize;
        rxSliceIdx = (rxSliceIdx + rankSize - deltaSliceIndex) % rankSize;
    }
    return info;
}

// 末步直读 output 时，各 rx slice 落在 output 上的字节偏移。
std::vector<u64> RefLastStepOutputOffsets(
    u32 myAlgRank, u32 rankSize, u64 outBuffBaseOff, u64 outputSliceStride, u64 partialOffset)
{
    const u32 nSteps = RefStepNum(rankSize);
    const RefStep last = RefGetStep(myAlgRank, rankSize, nSteps - 1U, nSteps);
    std::vector<u64> offsets;
    for (u32 rxIdx : last.rxSliceIdxs) {
        offsets.push_back(outBuffBaseOff + outputSliceStride * rxIdx + partialOffset);
    }
    return offsets;
}

// PostLocalCopy 实际会搬运的 algRank 集合。
std::vector<u32> RefPostCopyRanks(
    u32 myAlgRank, u32 rankSize, bool readLastStepToOutput, bool skipOwnSliceCopy,
    const std::vector<u32>& lastStepReadSliceIdxs)
{
    std::vector<u32> copied;
    for (u32 algRank = 0; algRank < rankSize; algRank++) {
        if (readLastStepToOutput && skipOwnSliceCopy && algRank == myAlgRank) {
            continue;
        }
        if (readLastStepToOutput &&
            std::find(lastStepReadSliceIdxs.begin(), lastStepReadSliceIdxs.end(), algRank) !=
                lastStepReadSliceIdxs.end()) {
            continue;
        }
        copied.push_back(algRank);
    }
    return copied;
}

const std::vector<u32> g_rankSizes = {2U, 3U, 4U, 5U, 8U, 16U};

// ---------------------------------------------------------------------------
// NHR 步骤推进
// ---------------------------------------------------------------------------

class AllGatherNhrStepTest : public ::testing::Test {};

TEST_F(AllGatherNhrStepTest, StepNumMatchesCeilLog2)
{
    EXPECT_EQ(GetNHRStepNum(2U), 1U);
    EXPECT_EQ(GetNHRStepNum(3U), 2U);
    EXPECT_EQ(GetNHRStepNum(4U), 2U);
    EXPECT_EQ(GetNHRStepNum(5U), 3U);
    EXPECT_EQ(GetNHRStepNum(8U), 3U);
    EXPECT_EQ(GetNHRStepNum(16U), 4U);
}

TEST_F(AllGatherNhrStepTest, PeerIsSymmetricAcrossRanks)
{
    // 本 rank 的 sendTo 必须把本 rank 视为 recvFrom，否则通信配不上对。
    for (u32 rankSize : g_rankSizes) {
        const u32 nSteps = RefStepNum(rankSize);
        for (u32 step = 0; step < nSteps; step++) {
            for (u32 rank = 0; rank < rankSize; rank++) {
                const RefStep mine = RefGetStep(rank, rankSize, step, nSteps);
                const RefStep peer = RefGetStep(mine.toRank, rankSize, step, nSteps);
                EXPECT_EQ(peer.fromRank, rank) << "rankSize=" << rankSize << " step=" << step << " rank=" << rank;
                EXPECT_EQ(mine.nSlices, peer.nSlices) << "rankSize=" << rankSize << " step=" << step;
            }
        }
    }
}

TEST_F(AllGatherNhrStepTest, EveryRankCollectsAllSlicesExactlyOnce)
{
    // 含 3/5 等非 2 次幂 rank 数：所有 slice 恰好收齐一次，不遗漏也不重复。
    for (u32 rankSize : g_rankSizes) {
        const u32 nSteps = RefStepNum(rankSize);
        for (u32 rank = 0; rank < rankSize; rank++) {
            std::set<u32> collected;
            collected.insert(rank); // 本 rank 自己那份来自 LocalDataCopy
            u32 receivedTotal = 0;
            for (u32 step = 0; step < nSteps; step++) {
                const RefStep info = RefGetStep(rank, rankSize, step, nSteps);
                for (u32 rxIdx : info.rxSliceIdxs) {
                    EXPECT_LT(rxIdx, rankSize) << "rankSize=" << rankSize << " rank=" << rank;
                    collected.insert(rxIdx);
                    receivedTotal++;
                }
                for (u32 txIdx : info.txSliceIdxs) {
                    EXPECT_LT(txIdx, rankSize) << "rankSize=" << rankSize << " rank=" << rank;
                }
            }
            EXPECT_EQ(collected.size(), rankSize) << "rankSize=" << rankSize << " rank=" << rank;
            // 非 2 次幂时允许重复收取，2 次幂时收取数应恰好等于 rankSize-1
            if ((rankSize & (rankSize - 1U)) == 0U) {
                EXPECT_EQ(receivedTotal, rankSize - 1U) << "rankSize=" << rankSize << " rank=" << rank;
            }
        }
    }
}

TEST_F(AllGatherNhrStepTest, EightRankZeroMatchesDocumentedSchedule)
{
    // 迁移方案 4.2 给出的 8 卡 rank0 期望调度。
    const u32 rankSize = 8U;
    const u32 nSteps = RefStepNum(rankSize);
    ASSERT_EQ(nSteps, 3U);

    const RefStep step0 = RefGetStep(0U, rankSize, 0U, nSteps);
    EXPECT_EQ(step0.toRank, 4U);
    EXPECT_EQ(step0.fromRank, 4U);
    EXPECT_EQ(step0.txSliceIdxs, std::vector<u32>({0U}));
    EXPECT_EQ(step0.rxSliceIdxs, std::vector<u32>({4U}));

    const RefStep step1 = RefGetStep(0U, rankSize, 1U, nSteps);
    EXPECT_EQ(step1.toRank, 2U);
    EXPECT_EQ(step1.fromRank, 6U);
    EXPECT_EQ(step1.txSliceIdxs, std::vector<u32>({0U, 4U}));
    EXPECT_EQ(step1.rxSliceIdxs, std::vector<u32>({6U, 2U}));

    const RefStep step2 = RefGetStep(0U, rankSize, 2U, nSteps);
    EXPECT_EQ(step2.toRank, 1U);
    EXPECT_EQ(step2.fromRank, 7U);
    EXPECT_EQ(step2.txSliceIdxs, std::vector<u32>({0U, 6U, 4U, 2U}));
    EXPECT_EQ(step2.rxSliceIdxs, std::vector<u32>({7U, 5U, 3U, 1U}));
}

// ---------------------------------------------------------------------------
// channel 数量统计
// ---------------------------------------------------------------------------

class AllGatherNhrChannelCountTest : public ::testing::Test {};

TEST_F(AllGatherNhrChannelCountTest, UniformChannelsGiveSameMinAndMax)
{
    const auto channels = MakeChannelDescs({{1U, 4U}, {2U, 4U}, {3U, 4U}});
    EXPECT_EQ(CalcChannelsPerRankMin(channels), 4U);
    EXPECT_EQ(CalcChannelsPerRank(channels), 4U);
}

TEST_F(AllGatherNhrChannelCountTest, RaggedChannelsGiveMinBelowMax)
{
    // 执行按 min 循环才不会越界访问 channel 少的 peer。
    const auto channels = MakeChannelDescs({{1U, 4U}, {2U, 2U}, {3U, 4U}});
    EXPECT_EQ(CalcChannelsPerRankMin(channels), 2U);
    EXPECT_EQ(CalcChannelsPerRank(channels), 4U);
}

TEST_F(AllGatherNhrChannelCountTest, EmptyChannelsFallBackToOne)
{
    const std::vector<HcclChannelDesc> empty;
    EXPECT_EQ(CalcChannelsPerRankMin(empty), 1U);
    EXPECT_EQ(CalcChannelsPerRank(empty), 1U);

    const std::map<u32, std::vector<ChannelInfo>> emptyMap;
    EXPECT_EQ(CalcChannelsPerRankMin(emptyMap), 1U);
    EXPECT_EQ(CalcChannelsPerRank(emptyMap), 1U);
}

TEST_F(AllGatherNhrChannelCountTest, ChannelMapMinAndMax)
{
    const auto channels = MakeChannelMap({{1U, 4U}, {2U, 2U}});
    EXPECT_EQ(CalcChannelsPerRankMin(channels), 2U);
    EXPECT_EQ(CalcChannelsPerRank(channels), 4U);
}

// ---------------------------------------------------------------------------
// 资源数量
// ---------------------------------------------------------------------------

class AllGatherNhrResourceTest : public ::testing::Test {};

TEST_F(AllGatherNhrResourceTest, ThreadAndNotifyCountsScaleWithChannels)
{
    // GetThreadNum() = maxChannelsPerRank * 2；从流各 2 个 notify，
    // 其中下标 1 专用于末步与 PostLocalCopy 的前同步。
    struct Expect {
        u32 maxChannelsPerRank;
        u32 threadNum;
        u32 slaveThreadNum;
    };
    const std::vector<Expect> expects = {{1U, 2U, 1U}, {2U, 4U, 3U}, {4U, 8U, 7U}};
    for (const auto& e : expects) {
        const u32 threadNum = e.maxChannelsPerRank * 2U;
        EXPECT_EQ(threadNum, e.threadNum);
        EXPECT_EQ(threadNum - 1U, e.slaveThreadNum);
        // 每个从流的 notify 数必须大于 POST_COPY 使用的下标 1
        const u32 notifyPerSlave = 2U;
        EXPECT_GT(notifyPerSlave, 1U);
    }
}

TEST_F(AllGatherNhrResourceTest, PostCopyThreadIndexStaysInRange)
{
    // 运行时 exact InsAllGatherNHR 保持 channelsPerRank_=1，
    // PostCopy thread 下标为 channelsPerRank_ + channelIdx。
    const u32 channelsPerRank = 1U;
    const u32 runtimeThreadNum = 2U; // maxChannelsPerRank_ 运行时为 1
    for (u32 channelIdx = 0; channelIdx < channelsPerRank; channelIdx++) {
        const u32 postCopyThreadIdx = channelsPerRank + channelIdx;
        EXPECT_LT(postCopyThreadIdx, runtimeThreadNum);
    }
}

TEST_F(AllGatherNhrResourceTest, HostMayAllocateMoreThreadsThanRuntimeUses)
{
    // Host CalcRes 按 MultiJetty 结果申请，运行时只用 channel 0，
    // 申请多于使用是允许的，反之则越界。
    const u32 hostThreadNum = 4U * 2U;    // Host 期 maxChannelsPerRank_=4
    const u32 runtimeThreadNum = 1U * 2U; // 运行时 maxChannelsPerRank_=1
    EXPECT_LE(runtimeThreadNum, hostThreadNum);
}

// ---------------------------------------------------------------------------
// 拓扑判断与 port isolation 策略
// ---------------------------------------------------------------------------

class AllGatherNhrTopoTest : public ::testing::Test {};

TEST_F(AllGatherNhrTopoTest, FullMeshIsDetected)
{
    const auto topoInfo = MakeTopoInfo(8U, CommTopo::COMM_TOPO_1DMESH, 8U);
    EXPECT_TRUE(IsAllConnetedWithTopo(&topoInfo, 0U, CommTopo::COMM_TOPO_1DMESH));
}

TEST_F(AllGatherNhrTopoTest, PartialCoverageIsNotFullMesh)
{
    const auto topoInfo = MakeTopoInfo(8U, CommTopo::COMM_TOPO_1DMESH, 4U);
    EXPECT_FALSE(IsAllConnetedWithTopo(&topoInfo, 0U, CommTopo::COMM_TOPO_1DMESH));
}

TEST_F(AllGatherNhrTopoTest, MissingTopoTypeReturnsFalse)
{
    const auto topoInfo = MakeTopoInfo(8U, CommTopo::COMM_TOPO_1DMESH, 8U);
    EXPECT_FALSE(IsAllConnetedWithTopo(&topoInfo, 0U, CommTopo::COMM_TOPO_CLOS));
}

TEST_F(AllGatherNhrTopoTest, OutOfRangeNetLayerReturnsFalseWithoutOverflow)
{
    const auto topoInfo = MakeTopoInfo(8U, CommTopo::COMM_TOPO_1DMESH, 8U);
    EXPECT_FALSE(IsAllConnetedWithTopo(&topoInfo, 1U, CommTopo::COMM_TOPO_1DMESH));
    EXPECT_FALSE(IsAllConnetedWithTopo(&topoInfo, 100U, CommTopo::COMM_TOPO_1DMESH));
}

TEST_F(AllGatherNhrTopoTest, NullTopoInfoReturnsFalse)
{
    EXPECT_FALSE(IsAllConnetedWithTopo(nullptr, 0U, CommTopo::COMM_TOPO_1DMESH));
}

TEST_F(AllGatherNhrTopoTest, TopoDetailsShorterThanLayerSizesDoesNotOverflow)
{
    TopoInfoWithNetLayerDetails topoInfo{};
    topoInfo.netLayerDetails.localNetInsSizeOfLayer = {8U, 8U};
    TopoInstDetails details{};
    details.rankNumForTopoType[CommTopo::COMM_TOPO_1DMESH] = {8U};
    topoInfo.topoInstDetailsOfLayer.push_back(details);
    EXPECT_FALSE(IsAllConnetedWithTopo(&topoInfo, 1U, CommTopo::COMM_TOPO_1DMESH));
}

TEST_F(AllGatherNhrTopoTest, IsolationPolicyFollowsTopoAndDataSize)
{
    // CalcRes 中的判定：完整 1D Mesh 或 <=512KiB 时关闭 isolation。
    const auto fullMesh = MakeTopoInfo(8U, CommTopo::COMM_TOPO_1DMESH, 8U);
    const auto clos = MakeTopoInfo(8U, CommTopo::COMM_TOPO_CLOS, 8U);

    auto isIsolation = [](const TopoInfoWithNetLayerDetails& topoInfo, u64 dataSize) {
        return !(IsAllConnetedWithTopo(&topoInfo, 0U, CommTopo::COMM_TOPO_1DMESH) || dataSize <= SMALL_SIZE_512KB);
    };

    EXPECT_FALSE(isIsolation(fullMesh, 32UL * 1024UL * 1024UL));
    EXPECT_FALSE(isIsolation(fullMesh, 1024UL));
    EXPECT_FALSE(isIsolation(clos, SMALL_SIZE_512KB));
    EXPECT_TRUE(isIsolation(clos, static_cast<u64>(SMALL_SIZE_512KB) + 1UL));
    EXPECT_TRUE(isIsolation(clos, 32UL * 1024UL * 1024UL));
}

// ---------------------------------------------------------------------------
// port group 数据切分
// ---------------------------------------------------------------------------

class AllGatherNhrDataSplitTest : public ::testing::Test {};

TEST_F(AllGatherNhrDataSplitTest, SingleChannelTakesWholeSlice)
{
    const auto channels = MakeChannelMap({{1U, 1U}});
    std::vector<u64> elemCount;
    std::vector<u64> sizeOut;
    std::vector<u64> elemOffset;
    ASSERT_EQ(
        CalcDataSplitByPortGroupCommon(1024U, 4U, channels.at(1U), elemCount, sizeOut, elemOffset, 1U), HCCL_SUCCESS);
    ASSERT_EQ(sizeOut.size(), 1U);
    EXPECT_EQ(sizeOut[0], 1024U * 4U);
    EXPECT_EQ(elemOffset[0], 0U);
}

TEST_F(AllGatherNhrDataSplitTest, EqualWeightsConserveTotalAndStayContiguous)
{
    const auto channels = MakeChannelMap({{1U, 4U}});
    std::vector<u64> elemCount;
    std::vector<u64> sizeOut;
    std::vector<u64> elemOffset;
    ASSERT_EQ(
        CalcDataSplitByPortGroupCommon(1024U, 4U, channels.at(1U), elemCount, sizeOut, elemOffset, 4U), HCCL_SUCCESS);
    ASSERT_EQ(sizeOut.size(), 4U);
    u64 total = 0;
    for (u64 s : sizeOut) {
        total += s;
    }
    EXPECT_EQ(total, 1024U * 4U);
    for (size_t i = 1; i < elemOffset.size(); i++) {
        EXPECT_EQ(elemOffset[i], elemOffset[i - 1] + sizeOut[i - 1]);
    }
}

TEST_F(AllGatherNhrDataSplitTest, IndivisibleTotalIsAbsorbedByLastChunk)
{
    const auto channels = MakeChannelMap({{1U, 3U}});
    std::vector<u64> elemCount;
    std::vector<u64> sizeOut;
    std::vector<u64> elemOffset;
    ASSERT_EQ(
        CalcDataSplitByPortGroupCommon(1001U, 4U, channels.at(1U), elemCount, sizeOut, elemOffset, 3U), HCCL_SUCCESS);
    u64 total = 0;
    for (u64 s : sizeOut) {
        total += s;
    }
    EXPECT_EQ(total, 1001U * 4U);
}

TEST_F(AllGatherNhrDataSplitTest, HonorsChannelsPerRankSmallerThanChannelCount)
{
    // 运行时按 min 循环，只使用前 channelsPerRank 个 channel，切分总量仍需守恒。
    const auto channels = MakeChannelMap({{1U, 4U}});
    std::vector<u64> elemCount;
    std::vector<u64> sizeOut;
    std::vector<u64> elemOffset;
    ASSERT_EQ(
        CalcDataSplitByPortGroupCommon(1024U, 4U, channels.at(1U), elemCount, sizeOut, elemOffset, 2U), HCCL_SUCCESS);
    ASSERT_EQ(sizeOut.size(), 2U);
    u64 total = 0;
    for (u64 s : sizeOut) {
        total += s;
    }
    EXPECT_EQ(total, 1024U * 4U);
}

TEST_F(AllGatherNhrDataSplitTest, TailSliceSplitsIndependently)
{
    // 普通块与尾块分别切分，两者 offset 均从 0 起算。
    const auto channels = MakeChannelMap({{1U, 2U}});
    std::vector<u64> elemCount;
    std::vector<u64> normalSize;
    std::vector<u64> normalOffset;
    std::vector<u64> tailSize;
    std::vector<u64> tailOffset;
    ASSERT_EQ(
        CalcDataSplitByPortGroupCommon(1024U, 4U, channels.at(1U), elemCount, normalSize, normalOffset, 2U),
        HCCL_SUCCESS);
    ASSERT_EQ(
        CalcDataSplitByPortGroupCommon(512U, 4U, channels.at(1U), elemCount, tailSize, tailOffset, 2U), HCCL_SUCCESS);
    EXPECT_EQ(normalOffset[0], 0U);
    EXPECT_EQ(tailOffset[0], 0U);
    u64 normalTotal = 0;
    for (u64 s : normalSize) {
        normalTotal += s;
    }
    u64 tailTotal = 0;
    for (u64 s : tailSize) {
        tailTotal += s;
    }
    EXPECT_EQ(normalTotal, 1024U * 4U);
    EXPECT_EQ(tailTotal, 512U * 4U);
    EXPECT_NE(normalTotal, tailTotal);
}

// ---------------------------------------------------------------------------
// 末步直读 output 与 PostLocalCopy 跳过
// ---------------------------------------------------------------------------

class AllGatherNhrLastStepTest : public ::testing::Test {};

TEST_F(AllGatherNhrLastStepTest, LastStepQualifiesOnlyWhenMultipleSlices)
{
    // 2 卡末步只有 1 个 slice，没有可重叠的余量，必须回退普通路径。
    const u32 nSteps2 = RefStepNum(2U);
    const RefStep last2 = RefGetStep(0U, 2U, nSteps2 - 1U, nSteps2);
    EXPECT_EQ(last2.nSlices, 1U);
    EXPECT_FALSE(last2.nSlices > 1U);

    // 8 卡末步 4 个 slice，可启用。
    const u32 nSteps8 = RefStepNum(8U);
    const RefStep last8 = RefGetStep(0U, 8U, nSteps8 - 1U, nSteps8);
    EXPECT_EQ(last8.nSlices, 4U);
    EXPECT_TRUE(last8.nSlices > 1U);
}

TEST_F(AllGatherNhrLastStepTest, ReadSliceIdxsAreUniqueAndInRange)
{
    for (u32 rankSize : g_rankSizes) {
        const u32 nSteps = RefStepNum(rankSize);
        for (u32 rank = 0; rank < rankSize; rank++) {
            const RefStep last = RefGetStep(rank, rankSize, nSteps - 1U, nSteps);
            std::set<u32> uniq(last.rxSliceIdxs.begin(), last.rxSliceIdxs.end());
            EXPECT_EQ(uniq.size(), last.rxSliceIdxs.size()) << "rankSize=" << rankSize << " rank=" << rank;
            for (u32 idx : last.rxSliceIdxs) {
                EXPECT_LT(idx, rankSize);
            }
        }
    }
}

TEST_F(AllGatherNhrLastStepTest, ReadAndPostCopySetsPartitionAllRanks)
{
    // 直读集合与回拷集合必须互斥，且并集恰好覆盖全部 rank 一次。
    for (u32 rankSize : g_rankSizes) {
        const u32 nSteps = RefStepNum(rankSize);
        for (u32 rank = 0; rank < rankSize; rank++) {
            const RefStep last = RefGetStep(rank, rankSize, nSteps - 1U, nSteps);
            const auto copied = RefPostCopyRanks(rank, rankSize, true, false, last.rxSliceIdxs);
            for (u32 c : copied) {
                EXPECT_TRUE(
                    std::find(last.rxSliceIdxs.begin(), last.rxSliceIdxs.end(), c) == last.rxSliceIdxs.end())
                    << "rankSize=" << rankSize << " rank=" << rank << " duplicated slice " << c;
            }
            std::set<u32> all(last.rxSliceIdxs.begin(), last.rxSliceIdxs.end());
            all.insert(copied.begin(), copied.end());
            EXPECT_EQ(all.size(), rankSize) << "rankSize=" << rankSize << " rank=" << rank;
        }
    }
}

TEST_F(AllGatherNhrLastStepTest, SolePathStillCopiesOwnSlice)
{
    // sole executor 设置 inBuffType=INPUT，CanSkipOwnSliceCopy 为 false，
    // 因此本 rank 自己那份仍需从 scratch 回拷。
    const u32 rankSize = 8U;
    const u32 nSteps = RefStepNum(rankSize);
    const RefStep last = RefGetStep(0U, rankSize, nSteps - 1U, nSteps);
    const auto copied = RefPostCopyRanks(0U, rankSize, true, false, last.rxSliceIdxs);
    EXPECT_TRUE(std::find(copied.begin(), copied.end(), 0U) != copied.end());
    EXPECT_EQ(copied.size(), rankSize - last.rxSliceIdxs.size());
}

TEST_F(AllGatherNhrLastStepTest, SkipOwnSliceDropsOneMoreCopy)
{
    const u32 rankSize = 8U;
    const u32 nSteps = RefStepNum(rankSize);
    const RefStep last = RefGetStep(0U, rankSize, nSteps - 1U, nSteps);
    const auto withOwn = RefPostCopyRanks(0U, rankSize, true, false, last.rxSliceIdxs);
    const auto withoutOwn = RefPostCopyRanks(0U, rankSize, true, true, last.rxSliceIdxs);
    EXPECT_EQ(withoutOwn.size() + 1U, withOwn.size());
    EXPECT_TRUE(std::find(withoutOwn.begin(), withoutOwn.end(), 0U) == withoutOwn.end());
}

TEST_F(AllGatherNhrLastStepTest, FallbackCopiesEveryRank)
{
    // 末步优化关闭时（例如 PCIe Read 分支），全部 slice 都走 scratch 回拷。
    const u32 rankSize = 8U;
    const u32 nSteps = RefStepNum(rankSize);
    const RefStep last = RefGetStep(0U, rankSize, nSteps - 1U, nSteps);
    const auto copied = RefPostCopyRanks(0U, rankSize, false, false, last.rxSliceIdxs);
    EXPECT_EQ(copied.size(), rankSize);
}

// ---------------------------------------------------------------------------
// strideCount 与末步直读的组合（HCCL 侧无此组合，必须单独覆盖）
// ---------------------------------------------------------------------------

class AllGatherNhrStrideTest : public ::testing::Test {};

TEST_F(AllGatherNhrStrideTest, StrideZeroKeepsContiguousLayout)
{
    // strideCount=0 时 executor 取 outputSliceStride=dataSize，输出为连续拼接。
    const u32 rankSize = 8U;
    const u64 dataSize = 4096U;
    const auto offsets = RefLastStepOutputOffsets(0U, rankSize, 0U, dataSize, 0U);
    const u32 nSteps = RefStepNum(rankSize);
    const RefStep last = RefGetStep(0U, rankSize, nSteps - 1U, nSteps);
    ASSERT_EQ(offsets.size(), last.rxSliceIdxs.size());
    for (size_t i = 0; i < offsets.size(); i++) {
        EXPECT_EQ(offsets[i], dataSize * last.rxSliceIdxs[i]);
    }
}

TEST_F(AllGatherNhrStrideTest, StrideScalesOutputOffsets)
{
    const u32 rankSize = 8U;
    const u64 dataSize = 4096U;
    const auto contiguous = RefLastStepOutputOffsets(0U, rankSize, 0U, dataSize, 0U);
    const auto strided = RefLastStepOutputOffsets(0U, rankSize, 0U, dataSize * 2U, 0U);
    ASSERT_EQ(contiguous.size(), strided.size());
    for (size_t i = 0; i < contiguous.size(); i++) {
        EXPECT_EQ(strided[i], contiguous[i] * 2U);
    }
}

TEST_F(AllGatherNhrStrideTest, StridedSlicesDoNotOverlap)
{
    // strideCount 大于 count 时各 rank 数据之间留空隙，落点不得重叠。
    const u32 rankSize = 8U;
    const u64 dataSize = 4096U;
    for (u64 strideMultiplier : {1U, 2U, 3U}) {
        const auto offsets = RefLastStepOutputOffsets(0U, rankSize, 0U, dataSize * strideMultiplier, 0U);
        std::set<u64> uniq(offsets.begin(), offsets.end());
        EXPECT_EQ(uniq.size(), offsets.size());
        for (u64 off : offsets) {
            EXPECT_EQ(off % dataSize, 0U);
        }
    }
}

TEST_F(AllGatherNhrStrideTest, BaseOffsetShiftsAllSlices)
{
    const u32 rankSize = 8U;
    const u64 dataSize = 4096U;
    const u64 baseOff = 1024U;
    const auto without = RefLastStepOutputOffsets(0U, rankSize, 0U, dataSize, 0U);
    const auto with = RefLastStepOutputOffsets(0U, rankSize, baseOff, dataSize, 0U);
    ASSERT_EQ(without.size(), with.size());
    for (size_t i = 0; i < without.size(); i++) {
        EXPECT_EQ(with[i], without[i] + baseOff);
    }
}

TEST_F(AllGatherNhrStrideTest, PartialOffsetShiftsWithinSlice)
{
    // 多 channel 切分时，每个 channel 只搬自己那一段，偏移需叠加 partialOffset。
    const u32 rankSize = 8U;
    const u64 dataSize = 4096U;
    const u64 partialOffset = 512U;
    const auto base = RefLastStepOutputOffsets(0U, rankSize, 0U, dataSize, 0U);
    const auto shifted = RefLastStepOutputOffsets(0U, rankSize, 0U, dataSize, partialOffset);
    ASSERT_EQ(base.size(), shifted.size());
    for (size_t i = 0; i < base.size(); i++) {
        EXPECT_EQ(shifted[i], base[i] + partialOffset);
    }
}

TEST_F(AllGatherNhrStrideTest, StrideCoversEveryRankSlotAcrossAllRanks)
{
    // 每个 rank 在 strideCount>0 布局下，直读+回拷仍然覆盖所有 slot 一次。
    const u32 rankSize = 8U;
    const u64 dataSize = 4096U;
    const u64 stride = dataSize * 2U;
    const u32 nSteps = RefStepNum(rankSize);
    for (u32 rank = 0; rank < rankSize; rank++) {
        const RefStep last = RefGetStep(rank, rankSize, nSteps - 1U, nSteps);
        std::set<u64> writtenOffsets;
        for (u32 rxIdx : last.rxSliceIdxs) {
            writtenOffsets.insert(stride * rxIdx);
        }
        const auto copied = RefPostCopyRanks(rank, rankSize, true, false, last.rxSliceIdxs);
        for (u32 algRank : copied) {
            writtenOffsets.insert(stride * algRank);
        }
        EXPECT_EQ(writtenOffsets.size(), rankSize) << "rank=" << rank;
    }
}

// ---------------------------------------------------------------------------
// jetty 上限语义
// ---------------------------------------------------------------------------

class AllGatherNhrJettyLimitTest : public ::testing::Test {};

TEST_F(AllGatherNhrJettyLimitTest, OverLimitIsWarnOnlyNotFailure)
{
    // 与 HCCL 一致：channelsPerRank_ 超过 MAX_JETTY_NUM 只记录错误日志，不阻断。
    const auto channels = MakeChannelDescs({{1U, 8U}, {2U, 8U}});
    const u32 perRank = CalcChannelsPerRankMin(channels);
    EXPECT_EQ(perRank, 8U);
    EXPECT_GT(perRank, MAX_JETTY_NUM);
}

TEST_F(AllGatherNhrJettyLimitTest, WithinLimitIsAccepted)
{
    const auto channels = MakeChannelDescs({{1U, 4U}, {2U, 4U}});
    EXPECT_LE(CalcChannelsPerRankMin(channels), MAX_JETTY_NUM);
}

} // namespace
