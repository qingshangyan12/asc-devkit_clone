// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "ccu_all_gather_mesh.h"
#include "all_gather_algorithms.h"
#include "all_gather_steps.h"
namespace mc2_ops_hccl { namespace dsl {
CcuResult BuildCcuAllGatherMesh(
    ccu::Variable input,ccu::Variable output,ccu::Variable token,ccu::Variable outputOffset,
    ccu::Variable bytes,ccu::Variable go0,ccu::Variable go1,ccu::Variable go2,ccu::Variable go3,
    ccu::Variable inputOffset,ccu::Variable repeatInverse,ccu::Variable inputStride,ccu::Variable outputStride,
    ccu::Variable tailBytes,ccu::Variable inplace,const ChannelHandle channels[],uint32_t channelCount,
    uint32_t rankSize,uint32_t rank)
{
    (void)go0;(void)go1;(void)go2;(void)go3;
    if(!rankSize||rank>=rankSize||channelCount!=rankSize-1)return CCU_E_INTERNAL;
    Ranks ranks;for(Rank r=0;r<rankSize;++r)ranks.push_back(r);
    Dimension dimension;dimension.groups.assign(rankSize,ranks);
    // host 生成 DSL 步骤；字节数在每次 mission 中由 CCU Variable 绑定。
    const Plan plan=SoleMesh({rank,rankSize,1},dimension);
    const auto rounds=ExpandAllGather(plan.stages.back(),rank);
    ccu::LocalAddr source,local;source.addr=input;source.addr+=inputOffset;source.token=token;
    local.addr=output;local.addr+=outputOffset;local.token=token;
    ccu::Variable one;one=1;
    ccu::Variable count;count=repeatInverse;
    ccu::Variable size;size=bytes;
    if(rank+1==rankSize){CCU_IF(tailBytes!=0){size=tailBytes;}}
    ccu::Event localDone;
    std::vector<ccu::RemoteAddr> destinations(channelCount);
    std::vector<ccu::Event> done(channelCount);
    for(uint32_t i=0;i<channelCount;++i){
        CCU_CHK_RET(ccu::WriteVariableWithNotify(channels[i],output,1,0,2));
        CCU_CHK_RET(ccu::WriteVariableWithNotify(channels[i],token,2,0,4));
    }
    for(uint32_t i=0;i<channelCount;++i){
        CCU_CHK_RET(ccu::NotifyWait(channels[i],0,6));
        // GetResByChannel 返回对端 XN 句柄；地址运算必须写入独立本地 Variable，避免污染句柄。
        destinations[i].addr=ccu::GetResByChannel<ccu::Variable>(channels[i],1);
        destinations[i].addr+=outputOffset;
        destinations[i].token=ccu::GetResByChannel<ccu::Variable>(channels[i],2);
    }
    CCU_WHILE(count!=UINT64_MAX){
        count+=one;
        CCU_IF(size!=0){
            for(const auto& e:rounds[0]){
                const uint32_t channel=e.sendPeer<rank?e.sendPeer:e.sendPeer-1;
                CCU_CHK_RET(ccu::Write(channels[channel],destinations[channel],source,size,done[channel],1));
            }
            CCU_IF(inplace==0){
                CCU_CHK_RET(ccu::LocalCopy(local,source,size,localDone,1));
                CCU_CHK_RET(ccu::EventWait(localDone,1));
            }
            for(uint32_t i=0;i<channelCount;++i)CCU_CHK_RET(ccu::EventWait(done[i],1));
        }
        // 每个 repeat 完成后再通知 peer；下一次复用输入/输出必须等待所有 rank 完成。
        for(uint32_t i=0;i<channelCount;++i)CCU_CHK_RET(ccu::NotifyRecord(channels[i],0,8));
        for(uint32_t i=0;i<channelCount;++i)CCU_CHK_RET(ccu::NotifyWait(channels[i],0,8));
        source.addr+=inputStride;local.addr+=outputStride;
        for(uint32_t i=0;i<channelCount;++i)destinations[i].addr+=outputStride;
    }
    return CCU_SUCCESS;
}
} }
