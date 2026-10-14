// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "aiv_program_builder.h"
#include "all_gather_steps.h"
namespace mc2_ops_hccl { namespace dsl {
std::vector<AivCommand> BuildAivProgram(const Plan& plan)
{
    std::vector<AivCommand> code;uint64_t signal=0;
    for(const Stage& stage:plan.stages){
        if(stage.primitive==Primitive::PLACE_INPUT){
            code.push_back({AivOp::COPY_INPUT,plan.rank,0,plan.rank*plan.bytesPerRank,plan.bytesPerRank,0});continue;
        }
        for(const auto& round:ExpandAllGather(stage,plan.rank)){
            // 每轮有独立 flag 槽。phaseReady 和 phaseDone 都必须覆盖 group，防止快 rank 覆盖工作区。
            for(Rank peer:stage.ranks)code.push_back({AivOp::BARRIER,peer,0,0,0,signal});
            ++signal;
            for(const auto& e:round)for(Rank owner:e.rxOwners){uint64_t off=owner*plan.bytesPerRank+e.bytes.begin;
                code.push_back({AivOp::READ_PEER,e.recvPeer,off,off,e.bytes.end-e.bytes.begin,0});}
            for(Rank peer:stage.ranks)code.push_back({AivOp::BARRIER,peer,0,0,0,signal});
            ++signal;
        }
    }
    return code;
}
} }
