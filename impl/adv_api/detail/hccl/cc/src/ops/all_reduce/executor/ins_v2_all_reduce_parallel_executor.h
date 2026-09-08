/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCLV2_INS_ALL_REDUCE_PARALLEL_EXECUTOR_H
#define HCCLV2_INS_ALL_REDUCE_PARALLEL_EXECUTOR_H

#include "coll_alg_v2_exec_registry.h"
#include "executor_v2_base.h"
#include "topo_match_multilevel.h"

namespace mc2_ops_hccl {

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
class InsAllReduceParallelExecutor : public InsCollAlgBase {
public:
    explicit InsAllReduceParallelExecutor();
    ~InsAllReduceParallelExecutor() override = default;

    std::string Describe() const override { return "Instruction based AllReduce Parallel Executor."; }

    HcclResult CalcRes(
        HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
        const AlgHierarchyInfoForAllLevel& algHierarchyInfo, AlgResourceRequest& resourceRequest) override;
    HcclResult Orchestrate(const OpParam& param, const AlgResourceCtxSerializable& resCtx) override;
    HcclResult CalcAlgHierarchyInfo(
        HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo, AlgHierarchyInfoForAllLevel& algHierarchyInfo) override;

private:
    static u32 CalcRankIdInter(u32 i, u32 j, u32 part2);
    static u32 CalcRankIdIntra(u32 i, u32 j, u32 part1);
    static u64 GetRankSize(const std::vector<std::vector<u32>>& hierarchy);

    HcclResult GetHierarchyInfo(
        const TopoInfoWithNetLayerDetails& topoInfo, const AlgHierarchyInfoForAllLevel& algHierarchyInfo,
        std::vector<std::vector<u32>>& intraHierarchyInfo, std::vector<std::vector<u32>>& interHierarchyInfo) const;
    HcclResult PrepareResForTemplate(
        InsAlgTemplate0& tempAlgIntra, InsAlgTemplate1& tempAlgInter, InsAlgTemplate2& tempAlgIntra1,
        InsAlgTemplate3& tempAlgInter1);
    HcclResult PrepareResForTemplate23(
        InsAlgTemplate0& tempAlgIntra, InsAlgTemplate2& tempAlgIntra1, InsAlgTemplate3& tempAlgInter1);
    void GetParallelDataSplit(std::vector<float>& splitDataSize) const;

    void FillDataMap(
        u64 sliceCount, u32 part1, u32 part2, std::map<u32, std::pair<u64, u64>>& dataMap, bool isInter) const;
    void FillOffsetMaps(
        const std::map<u32, std::pair<u64, u64>>& dataMap, u32 part1, u32 part2, std::map<u32, u64>& agMap,
        bool isInter) const;
    void CalcInterDataAllRank(u64 sliceCount, u32 part1, u32 part2, std::map<u32, std::pair<u64, u64>>& dataMap);
    void CalcIntraDataAllRank(u64 sliceCount, u32 part1, u32 part2, std::map<u32, std::pair<u64, u64>>& dataMap);
    void GenDataParamsAllRank(u64 sliceCount, u32 localRankSize, TemplateDataParams& dataParams) const;
    void GenDataParamsBufferType(
        BufferType inBuffType, BufferType outBuffType, BufferType hcclBuffType, TemplateDataParams& dataParams) const;
    void GenDataParams(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 sliceCount,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, u32 localRankSize, u64 inputOffset,
        u64 outputOffset, u64 hcclBuffOffset) const;
    void PrepareDataParamsInter(u64 dataOffset, u64 currCountPart, u64 scratchOffsetCount);
    void PrepareDataParamsIntra(u64 dataOffset, u64 currCountPart, u64 scratchOffsetCount);

    HcclResult PrepareTemplateResource(
        const AlgResourceCtxSerializable& resCtx, TemplateResource& intraResource, TemplateResource& interResource)
        const;
    HcclResult GenInsQues(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, InsAlgTemplate0& tempAlgIntra,
        InsAlgTemplate1& tempAlgInter, InsAlgTemplate2& tempAlgIntra1, InsAlgTemplate3& tempAlgInter1);
    HcclResult RunTemplateIntra0(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource,
        InsAlgTemplate0& tempAlg);
    HcclResult RunTemplateInter1(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource,
        InsAlgTemplate1& tempAlg);
    HcclResult RunTemplateInter0(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource,
        InsAlgTemplate1& tempAlg);
    HcclResult RunTemplateIntra1(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource,
        InsAlgTemplate0& tempAlg);
    HcclResult RunTemplateInter01(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource,
        InsAlgTemplate3& tempAlg);
    HcclResult RunTemplateIntra11(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource,
        InsAlgTemplate2& tempAlg);
    HcclResult RunTemplateIntra01(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource,
        InsAlgTemplate2& tempAlg);
    HcclResult RunTemplateInter11(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource,
        InsAlgTemplate3& tempAlg);

    inline u64 RoundDown(u64 dividend, u64 divisor) const
    {
        return divisor == 0 ? dividend : dividend / divisor;
    }

    std::vector<std::vector<u32>> temp0HierarchyInfo_;
    std::vector<std::vector<u32>> temp1HierarchyInfo_;
    std::vector<ThreadHandle> threads_;
    std::vector<ThreadHandle> intraThreads_;
    std::vector<ThreadHandle> interThreads_;
    ThreadHandle mainThread_;
    std::vector<ThreadHandle> templateMainThreads_;
    std::vector<u32> syncNotifyOnTemplates_;
    std::vector<u32> syncNotifyOnMain_;
    std::vector<std::map<u32, std::vector<ChannelInfo>>> remoteRankToChannelInfo_;
    std::map<u32, std::vector<ChannelInfo>> intraLinks_;
    std::map<u32, std::vector<ChannelInfo>> interLinks_;

    std::map<u32, u32> tempVirtRankMapInter_;
    std::map<u32, u32> tempVirtRankMapIntra_;
    std::map<u32, u64> rankBaseOffInterAGMap_;
    std::map<u32, u64> rankBaseOffIntraAGMap_;
    std::map<u32, std::pair<u64, u64>> nhrPartDataMap_;
    std::map<u32, std::pair<u64, u64>> meshPartDataMap_;

    std::vector<u64> allRankSliceSize_;
    u32 intraLocalRankSize_{0};
    u32 interLocalRankSize_{0};
    u64 dataOffset0Inter_{0};
    u64 currCountPart0_{0};
    u64 scratchOffsetCountInterStage1_{0};
    u64 dataOffset0Intra_{0};
    u64 currCountPart1_{0};
    u64 scratchOffsetCountIntraStage1_{0};
    double multipleDimensionSplitRatio_{0.5};
    MultipleDimensionSplitRatioSource multipleDimensionSplitRatioSource_
        = MultipleDimensionSplitRatioSource::BUILTIN_FORMULA;
};

} // namespace mc2_ops_hccl

#endif // HCCLV2_INS_ALL_REDUCE_PARALLEL_EXECUTOR_H
