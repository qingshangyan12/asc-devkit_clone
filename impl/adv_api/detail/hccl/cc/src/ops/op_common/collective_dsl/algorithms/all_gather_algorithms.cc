// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#include "all_gather_algorithms.h"

#include <algorithm>
#include <stdexcept>

namespace mc2_ops_hccl {
namespace dsl {
namespace {

constexpr Queue COPY = 0;
constexpr Queue FIRST = 1;
constexpr Queue SECOND = 2;

void Check(const AllGatherArgs& args)
{
    if (args.rank >= args.rankSize || args.sendBytes == 0) {
        throw std::invalid_argument("invalid AllGather arguments");
    }
}

std::vector<Ranks> SelfOwners(uint32_t rankSize)
{
    std::vector<Ranks> result(rankSize);
    for (Rank rank = 0; rank < rankSize; ++rank) {
        result[rank].push_back(rank);
    }
    return result;
}

std::vector<Event> Join(const std::vector<Event>& a, const std::vector<Event>& b)
{
    std::vector<Event> result(a);
    result.insert(result.end(), b.begin(), b.end());
    return result;
}

Event PlaceInput(Builder& ir, const AllGatherArgs& args)
{
    const Slice input{args.rank, Buffer::INPUT, args.rank, 0, args.sendBytes};
    const Slice output{args.rank, Buffer::OUTPUT, args.rank, 0, args.sendBytes};
    return ir.LocalCopy(COPY, output, input);
}

} // namespace

Plan SoleMesh(const AllGatherArgs& args, const ChannelLayout& channels, const Dimension& mesh)
{
    Check(args);
    Builder ir("AllGatherSoleMesh", args.rank);
    const Event ready = PlaceInput(ir, args);
    return ir.Finish(Mesh(
        ir, FIRST, args, channels, mesh.At(args.rank), SelfOwners(args.rankSize), Interval{0, args.sendBytes},
        {ready}));
}

Plan SoleNhr(const AllGatherArgs& args, const ChannelLayout& channels, const Dimension& nhr)
{
    Check(args);
    Builder ir("AllGatherSoleNhr", args.rank);
    const Event ready = PlaceInput(ir, args);
    return ir.Finish(Nhr(
        ir, FIRST, args, channels, nhr.At(args.rank), SelfOwners(args.rankSize), Interval{0, args.sendBytes}, {ready}));
}

Plan ConcurrentMeshNhr(const AllGatherArgs& args, const ChannelLayout& channels, const Dimension& whole, uint64_t split)
{
    Check(args);
    if (split > args.sendBytes) {
        throw std::invalid_argument("split exceeds sendBytes");
    }
    Builder ir("AllGatherConcurrentMeshNhr", args.rank);
    const Event ready = PlaceInput(ir, args);
    const std::vector<Ranks> owners = SelfOwners(args.rankSize);
    const std::vector<Event> a = Mesh(ir, FIRST, args, channels, whole.At(args.rank), owners, {0, split}, {ready});
    const std::vector<Event> b =
        Nhr(ir, SECOND, args, channels, whole.At(args.rank), owners, {split, args.sendBytes}, {ready});
    return ir.Finish(Join(a, b));
}

Plan ParallelMeshNhr(
    const AllGatherArgs& args, const ChannelLayout& channels, const Dimension& mesh, const Dimension& nhr,
    uint64_t split)
{
    Check(args);
    if (split > args.sendBytes) {
        throw std::invalid_argument("split exceeds sendBytes");
    }
    Builder ir("AllGatherParallelMeshNhr", args.rank);
    const Event ready = PlaceInput(ir, args);
    const std::vector<Ranks> self = SelfOwners(args.rankSize);
    const std::vector<Event> meshFirst = Mesh(ir, FIRST, args, channels, mesh.At(args.rank), self, {0, split}, {ready});
    const std::vector<Event> nhrFirst =
        Nhr(ir, SECOND, args, channels, nhr.At(args.rank), self, {split, args.sendBytes}, {ready});
    const std::vector<Event> a = Nhr(ir, FIRST, args, channels, nhr.At(args.rank), mesh.groups, {0, split}, meshFirst);
    const std::vector<Event> b =
        Mesh(ir, SECOND, args, channels, mesh.At(args.rank), nhr.groups, {split, args.sendBytes}, nhrFirst);
    return ir.Finish(Join(a, b));
}

Plan PipelineMeshNhr(
    const AllGatherArgs& args, const ChannelLayout& channels, const Dimension& mesh, const Dimension& nhr,
    uint64_t chunk)
{
    Check(args);
    if (chunk == 0) {
        throw std::invalid_argument("chunk is zero");
    }
    Builder ir("AllGatherPipelineMeshNhr", args.rank);
    const Event ready = PlaceInput(ir, args);
    const std::vector<Ranks> self = SelfOwners(args.rankSize);
    std::vector<Event> final;
    for (uint64_t begin = 0; begin < args.sendBytes; begin += chunk) {
        const Interval part{begin, std::min(begin + chunk, args.sendBytes)};
        const std::vector<Event> first = Mesh(ir, FIRST, args, channels, mesh.At(args.rank), self, part, {ready});
        final = Join(final, Nhr(ir, SECOND, args, channels, nhr.At(args.rank), mesh.groups, part, first));
    }
    return ir.Finish(final);
}

} // namespace dsl
} // namespace mc2_ops_hccl
