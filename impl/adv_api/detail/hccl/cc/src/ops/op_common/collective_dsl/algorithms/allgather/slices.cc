// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "slices.h"
#include <stdexcept>
namespace asc { namespace ir {
Groups initialOwners(uint32_t ranks)
{
    Groups owners(ranks);
    for (Rank r = 0; r < ranks; ++r) owners[r] = {r};
    return owners;
}
Event placeInput(Queue& q, Rank p, const AllGather& a)
{
    // 先用除法检查总容量，避免 p*bytes 或 rankCount*bytes 整数溢出后误通过检查。
    if (a.input.rank != p || a.input.program != a.output.program ||
        a.input.bytes > a.output.bytes / a.output.program->rankCount())
        throw std::invalid_argument("invalid AllGather input/output layout");
    Slice own = a.output[p].slice(p * a.input.bytes, a.input.bytes);
    // 完全同址 in-place 无须搬运，但仍返回 queue 完成 event，保留前置依赖。
    if (own.buffer == a.input.buffer && own.offset == a.input.offset) return q.join();
    return q.submit(LocalCopy(own, a.input));
}
MeshSlices meshSlices(Rank p, const Ranks& ranks, const Groups& owners,
                     const AllGather& a, uint64_t begin, uint64_t bytes, Op direction)
{
    if (direction != Op::Read && direction != Op::Write) throw std::invalid_argument("invalid transfer direction");
    MeshSlices slices;
    for (Rank peer : ranks) {
        if (peer == p) continue;
        // Write 发出 owners[p]；Read 拉取 owners[peer]。owner 的位置始终明确为 owner*sendBytes。
        for (Rank owner : owners.at(direction == Op::Write ? p : peer)) {
            if (owner >= a.output.program->rankCount()) throw std::invalid_argument("invalid owner rank");
            const uint64_t offset = owner * a.input.bytes + begin;
            Slice local = a.output[p].slice(offset, bytes), remote = a.output[peer].slice(offset, bytes);
            slices[peer].push_back(direction == Op::Write ? Write(remote, local) : Read(local, remote));
        }
    }
    return slices;
}
NhrSlices nhrSlices(const Groups& owners, const AllGather& a, uint64_t begin, uint64_t bytes, Op direction)
{
    // block 可以是前一级 Mesh 收集的一组 owner；回调只构造存储地址。
    // NHR 轮次/peer/block 公式在 primitives/exchange.cc，绝不在这里重新实现。
    return {direction, [owners, a, begin, bytes](Rank holder, Rank block) {
        Slices slices;
        for (Rank owner : owners.at(block)) {
            if (owner >= a.output.program->rankCount()) throw std::invalid_argument("invalid owner rank");
            slices.push_back(a.output[holder].slice(owner * a.input.bytes + begin, bytes));
        }
        return slices;
    }};
}
} }
