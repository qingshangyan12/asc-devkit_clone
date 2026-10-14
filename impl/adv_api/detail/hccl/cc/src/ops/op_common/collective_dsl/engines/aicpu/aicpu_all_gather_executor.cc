// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "aicpu_all_gather_executor.h"
#include "all_gather_steps.h"
#include "alg_data_trans_wrapper.h"
#include "hcomm_primitives_dl.h"
#include <algorithm>
namespace mc2_ops_hccl { namespace dsl {
HcclResult ExecuteAicpuAllGather(const Stage& s,Rank rank,const OpParam& op,const TemplateDataParams& d,TemplateResource& r)
{
    if(d.sliceSize==0&&d.tailSize==0)return HCCL_SUCCESS;
    if(r.threads.empty()||s.ranks.empty())return HCCL_E_PARA;
    const auto pos=std::find(s.ranks.begin(),s.ranks.end(),rank);
    if(pos==s.ranks.end())return HCCL_E_PARA;
    const uint32_t me=pos-s.ranks.begin(),n=s.ranks.size(), dtype=DATATYPE_SIZE_TABLE[op.DataDes.dataType];
    if(!dtype||d.sliceSize%dtype||d.tailSize%dtype)return HCCL_E_PARA;
    const bool direct=d.enableRemoteMemAccess||op.supportSymmetricMemory;
    void* work=direct?d.buffInfo.outputPtr:d.buffInfo.hcclBuff.addr;
    if(!work||!d.buffInfo.inputPtr||!d.buffInfo.outputPtr)return HCCL_E_PTR;
    if(!direct&&d.repeatNum&&(d.sliceSize> d.buffInfo.hcclBuff.size/n/d.repeatNum))return HCCL_E_PARA;
    const auto slot=[&](Rank owner){return uint32_t(std::find(s.ranks.begin(),s.ranks.end(),owner)-s.ranks.begin());};
    const auto size=[&](uint32_t owner){return owner+1==n&&d.tailSize?d.tailSize:d.sliceSize;};
    const auto offset=[&](uint32_t owner,uint64_t repeat){return direct?
        d.buffInfo.outBuffBaseOff+repeat*d.outputRepeatStride+owner*d.outputSliceStride:
        d.buffInfo.hcclBuffBaseOff+(repeat*n+owner)*d.sliceSize;};
    // input 先发布到工作区；后续 NHR 从工作区转发已收集的数据，不能继续从 input 发。
    for(uint64_t repeat=0;repeat<d.repeatNum;++repeat){
        DataSlice src(d.buffInfo.inputPtr,d.buffInfo.inBuffBaseOff+repeat*d.inputRepeatStride+me*d.inputSliceStride,size(me),size(me)/dtype);
        DataSlice dst(work,offset(me,repeat),size(me),size(me)/dtype);
        CHK_RET(LocalCopy(r.threads[0],src,dst));
    }
    const auto rounds=ExpandAllGather(s,rank);
    const size_t workers=s.primitive==Primitive::MESH_ALL_GATHER?std::max<size_t>(1,n-1):1;
    if(r.threads.size()<workers)return HCCL_E_PARA;
    std::vector<ThreadHandle> slaves(r.threads.begin()+1,r.threads.begin()+workers);
    std::vector<u32> start(slaves.size(),0),finish;for(u32 i=0;i<slaves.size();++i)finish.push_back(i);
    if(!slaves.empty())CHK_RET(PreSyncInterThreads(r.threads[0],slaves,start));
    for(const auto& round:rounds)for(size_t i=0;i<round.size();++i){
        const auto& e=round[i];
        auto tx=r.channels.find(e.sendPeer),rx=r.channels.find(e.recvPeer);
        if(tx==r.channels.end()||rx==r.channels.end()||tx->second.empty()||rx->second.empty())return HCCL_E_PARA;
        const ChannelInfo& send=tx->second[0];const ChannelInfo& recv=rx->second[0];
        void* remoteSend=direct?send.remoteOutputGraphMode.addr:send.remoteCclMem.addr;
        void* remoteRecv=direct?recv.remoteOutputGraphMode.addr:recv.remoteCclMem.addr;
        if(op.supportSymmetricMemory){
            CHK_RET(HcclSymWinGetPeerPointer(op.outputSymWindow,op.outputOffset,e.sendPeer,&remoteSend));
            CHK_RET(HcclSymWinGetPeerPointer(op.outputSymWindow,op.outputOffset,e.recvPeer,&remoteRecv));
        }
        if(!remoteSend||!remoteRecv)return HCCL_E_PTR;
        std::vector<DataSlice> ts,td,rs,rd;
        const auto append=[&](const Ranks& owners,void* remote,std::vector<DataSlice>& localSlices,std::vector<DataSlice>& remoteSlices){
            for(uint64_t repeat=0;repeat<d.repeatNum;++repeat)for(Rank owner:owners){
                const u32 j=slot(owner);const uint64_t end=std::min<uint64_t>(e.bytes.end,size(j));
                if(j>=n||end<=e.bytes.begin)continue;
                uint64_t bytes=end-e.bytes.begin,off=offset(j,repeat)+e.bytes.begin;
                localSlices.emplace_back(work,off,bytes,bytes/dtype);remoteSlices.emplace_back(remote,off,bytes,bytes/dtype);
            }
        };
        append(e.txOwners,remoteSend,ts,td);append(e.rxOwners,remoteRecv,rd,rs);
        // 真正的 Read 和 ready/done channel 通知发生在该 wrapper 内。
        CHK_RET(SendRecvRead(SendRecvInfo(TxRxChannels(send,recv),TxRxSlicesList({ts,td},{rs,rd})),r.threads[i%workers]));
    }
    if(!slaves.empty())CHK_RET(PostSyncInterThreads(r.threads[0],slaves,finish));
    if(!direct)for(uint64_t repeat=0;repeat<d.repeatNum;++repeat)for(u32 owner=0;owner<n;++owner){
        DataSlice src(work,offset(owner,repeat),size(owner),size(owner)/dtype);
        DataSlice dst(d.buffInfo.outputPtr,d.buffInfo.outBuffBaseOff+repeat*d.outputRepeatStride+owner*d.outputSliceStride,size(owner),size(owner)/dtype);
        CHK_RET(LocalCopy(r.threads[0],src,dst));
    }
    return HCCL_SUCCESS;
}
} }
