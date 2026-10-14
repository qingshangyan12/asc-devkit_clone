// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "ccu_all_gather_server.h"
namespace mc2_ops_hccl { namespace dsl {
CcuResult BuildCcuAllGatherServer(ccu::Variable mailbox,ccu::Variable token,const ChannelHandle channels[],
    uint32_t channelCount,uint32_t rankSize,uint32_t rank,uint32_t capacity)
{
    if(!capacity)return CCU_E_PARA;
    ccu::Variable submitAddr,doneAddr,statusAddr,requestAddr,seen,submitted,stop,offset,one,inverse,zero,slot,slotStride,failed;
    requestAddr=mailbox;submitAddr=mailbox;offset=64;submitAddr+=offset;
    doneAddr=mailbox;offset=128;doneAddr+=offset;
    statusAddr=mailbox;offset=136;statusAddr+=offset;
    seen=1;submitted=0;stop=0;one=1;inverse=UINT64_MAX-1;zero=0;slot=0;slotStride=sizeof(CcuMailbox);
    ccu::Array<ccu::Variable> request(8);
    CCU_WHILE(stop==0){
        // Load 是真实 CCU HBM load 指令；CCU 硬件消费提交序号，Host 不轮询。
        CCU_CHK_RET(ccu::Load(submitAddr,submitted));
        CCU_WHILE(submitted!=seen){CCU_CHK_RET(ccu::Load(submitAddr,submitted));}
        CCU_CHK_RET(ccu::Load(requestAddr,request,8));
        failed=0;
        // 比较请求序号和 STOP 状态。所有 rank 必须进入相同的 collective，防止误写对端的下一轮 output。
        for(uint32_t i=0;i<channelCount;++i){
            CCU_CHK_RET(ccu::WriteVariableWithNotify(channels[i],seen,3,0,16));
            CCU_CHK_RET(ccu::WriteVariableWithNotify(channels[i],request[5],4,0,32));
        }
        for(uint32_t i=0;i<channelCount;++i){
            CCU_CHK_RET(ccu::NotifyWait(channels[i],0,48));
            ccu::Variable peerSequence,peerStop;
            peerSequence=ccu::GetResByChannel<ccu::Variable>(channels[i],3);
            peerStop=ccu::GetResByChannel<ccu::Variable>(channels[i],4);
            CCU_IF(peerSequence!=seen){failed=1;}
            CCU_IF(peerStop!=request[5]){failed=1;}
        }
        // 一个 rank 检测到错误，传播给所有 peer，统一跳过数据路径并退出。
        for(uint32_t i=0;i<channelCount;++i)CCU_CHK_RET(ccu::WriteVariableWithNotify(channels[i],failed,5,0,64));
        for(uint32_t i=0;i<channelCount;++i){
            CCU_CHK_RET(ccu::NotifyWait(channels[i],0,64));ccu::Variable peerFailed;
            peerFailed=ccu::GetResByChannel<ccu::Variable>(channels[i],5);CCU_IF(peerFailed!=0){failed=1;}
        }
        CCU_IF(failed==0){
          CCU_IF(request[5]==0){
            CCU_CHK_RET(BuildCcuAllGatherMesh(request[0],request[1],token,request[2],request[3],zero,zero,zero,zero,
                zero,inverse,zero,zero,zero,request[4],channels,channelCount,rankSize,rank));
        }
          CCU_ELSE {stop=1;}
        }
        CCU_ELSE {stop=1;}
        CCU_CHK_RET(ccu::Store(statusAddr,failed));
        CCU_CHK_RET(ccu::Store(doneAddr,seen));
        seen+=one;slot+=one;
        requestAddr+=slotStride;submitAddr+=slotStride;doneAddr+=slotStride;statusAddr+=slotStride;
        CCU_IF(slot==static_cast<uint64_t>(capacity)){
            slot=0;requestAddr=mailbox;submitAddr=mailbox;offset=64;submitAddr+=offset;
            doneAddr=mailbox;offset=128;doneAddr+=offset;
            statusAddr=mailbox;offset=136;statusAddr+=offset;
        }
    }
    return CCU_SUCCESS;
}
CcuResult CcuAllGatherServerEntry(const CcuServerBinding* binding)
{
    if(!binding)return CCU_E_PTR;
    ccu::Variable mailbox,token;
    CCU_CHK_RET(ccu::LoadArg(mailbox,0));CCU_CHK_RET(ccu::LoadArg(token,1));
    return BuildCcuAllGatherServer(mailbox,token,binding->channels,binding->channelCount,binding->rankSize,binding->rank,binding->capacity);
}
} }
