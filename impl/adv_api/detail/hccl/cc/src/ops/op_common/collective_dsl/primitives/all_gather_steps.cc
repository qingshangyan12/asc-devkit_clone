// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "all_gather_steps.h"
#include <algorithm>
#include <stdexcept>
namespace mc2_ops_hccl { namespace dsl {
std::vector<TransferRound> ExpandAllGather(const Stage& s, Rank rank)
{
    const auto found=std::find(s.ranks.begin(),s.ranks.end(),rank);
    if(found==s.ranks.end()) throw std::invalid_argument("rank outside stage group");
    const uint32_t n=s.ranks.size(), me=found-s.ranks.begin();
    std::vector<TransferRound> rounds;
    if(s.primitive==Primitive::MESH_ALL_GATHER) {
        TransferRound round;
        for(uint32_t i=1;i<n;++i) { Rank peer=s.ranks[(me+i)%n];
            round.push_back({peer,peer,s.ownersByRank.at(rank),s.ownersByRank.at(peer),s.bytes}); }
        rounds.push_back(round);return rounds;
    }
    if(s.primitive!=Primitive::NHR_ALL_GATHER) throw std::invalid_argument("not an AllGather primitive");
    // 与 HCCL GetStepInfo 相同：距离从最高二次幂递减，非 2 次幂使用环模和 ceil 切片数。
    uint32_t steps=0; for(uint64_t power=1;power<n;power<<=1)++steps;
    for(uint32_t t=0;t<steps;++t) {
        const uint32_t delta=1U<<(steps-1-t), send=(me+delta)%n, recv=(me+n-delta%n)%n;
        const uint32_t count=(n-1+delta)/(2*delta);
        ExchangeStep e{s.ranks[send],s.ranks[recv],{},{},s.bytes};
        for(uint32_t i=0;i<count;++i) {
            const uint32_t tx=(me+n-(uint64_t(i)*2*delta)%n)%n;
            const uint32_t rx=(recv+n-(uint64_t(i)*2*delta)%n)%n;
            const auto& a=s.ownersByRank.at(s.ranks[tx]); const auto& b=s.ownersByRank.at(s.ranks[rx]);
            e.txOwners.insert(e.txOwners.end(),a.begin(),a.end()); e.rxOwners.insert(e.rxOwners.end(),b.begin(),b.end());
        }
        rounds.push_back({e});
    }
    return rounds;
}
} }
