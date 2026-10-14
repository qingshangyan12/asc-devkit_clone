// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "aicpu_all_gather_binding.h"
#include "channel.h"
#include "aicpu_all_gather_executor.h"
namespace mc2_ops_hccl { namespace dsl {
HcclResult BindAicpuAllGather(const Stage& s,const AscendPlanContext& c,AlgResourceRequest& r)
{
    if(s.ranks.empty())return HCCL_E_PARA;
    std::vector<HcclChannelDesc> channels;
    if(s.primitive==Primitive::MESH_ALL_GATHER) {
        CHK_RET(CalcChannelRequestMesh1D(c.comm,c.op,&c.topo,{s.ranks},channels));
    } else if(s.primitive==Primitive::NHR_ALL_GATHER) {
        CHK_RET(CalcChannelRequestNhr(c.comm,c.op,&c.topo,{s.ranks},channels));
    } else return HCCL_E_NOT_SUPPORT;
    r.channels.push_back(channels);
    // Mesh 并行推进每个 peer；NHR 一条 thread 顺序推进各轮，channel 的 ACK/DATA_SIGNAL 建立远端依赖。
    const uint32_t threads=s.primitive==Primitive::MESH_ALL_GATHER?std::max<size_t>(1,s.ranks.size()-1):1;
    r.slaveThreadNum=threads-1;r.notifyNumOnMainThread=threads-1;r.notifyNumPerThread.assign(threads-1,1);
    return HCCL_SUCCESS;
}
HcclResult LowerAicpuAllGather(const Stage& s,Rank rank,const OpParam& op,const std::vector<std::vector<u32>>& groups,
    const TemplateDataParams& data,TemplateResource& resource)
{
    (void)groups; return ExecuteAicpuAllGather(s,rank,op,data,resource);
}
} }
