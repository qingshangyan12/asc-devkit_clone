// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "all_gather_algorithms.h"
#include <algorithm>
#include <stdexcept>
namespace mc2_ops_hccl {
namespace dsl {
namespace {
std::vector<Ranks> SelfOwners(uint32_t n)
{
    std::vector<Ranks> owners(n);
    for (Rank r = 0; r < n; ++r)
        owners[r] = {r};
    return owners;
}
void Check(const AllGatherArgs& a)
{
    if (a.rank >= a.rankSize || !a.sendBytes)
        throw std::invalid_argument("invalid args");
}
Builder Start(const char* name, const AllGatherArgs& a)
{
    Check(a);
    return Builder(name, a.rank, a.rankSize, a.sendBytes);
}
} // namespace
Plan SoleMesh(const AllGatherArgs& a, const Dimension& mesh)
{
    Builder p = Start("AllGatherSoleMesh", a);
    StageId ready = p.PlaceInput();
    return p.Finish({Mesh(p, 1, mesh.At(a.rank), SelfOwners(a.rankSize), {0, a.sendBytes}, {ready})});
}
Plan SoleNhr(const AllGatherArgs& a, const Dimension& nhr)
{
    Builder p = Start("AllGatherSoleNhr", a);
    StageId ready = p.PlaceInput();
    return p.Finish({Nhr(p, 1, nhr.At(a.rank), SelfOwners(a.rankSize), {0, a.sendBytes}, {ready})});
}
Plan ConcurrentMeshNhr(const AllGatherArgs& a, const Dimension& group, uint64_t split)
{
    Builder p = Start("AllGatherConcurrentMeshNhr", a);
    if (split > a.sendBytes)
        throw std::invalid_argument("split");
    StageId ready = p.PlaceInput();
    auto own = SelfOwners(a.rankSize);
    return p.Finish(
        {Mesh(p, 1, group.At(a.rank), own, {0, split}, {ready}),
         Nhr(p, 2, group.At(a.rank), own, {split, a.sendBytes}, {ready})});
}
Plan ParallelMeshNhr(const AllGatherArgs& a, const Dimension& mesh, const Dimension& nhr, uint64_t split)
{
    Builder p = Start("AllGatherParallelMeshNhr", a);
    if (split > a.sendBytes)
        throw std::invalid_argument("split");
    StageId ready = p.PlaceInput();
    auto own = SelfOwners(a.rankSize);
    StageId m = Mesh(p, 1, mesh.At(a.rank), own, {0, split}, {ready});
    StageId n = Nhr(p, 2, nhr.At(a.rank), own, {split, a.sendBytes}, {ready});
    return p.Finish(
        {Nhr(p, 1, nhr.At(a.rank), mesh.groups, {0, split}, {m}),
         Mesh(p, 2, mesh.At(a.rank), nhr.groups, {split, a.sendBytes}, {n})});
}
Plan PipelineMeshNhr(const AllGatherArgs& a, const Dimension& mesh, const Dimension& nhr, uint64_t chunk)
{
    Builder p = Start("AllGatherPipelineMeshNhr", a);
    if (!chunk)
        throw std::invalid_argument("chunk");
    StageId ready = p.PlaceInput();
    auto own = SelfOwners(a.rankSize);
    std::vector<StageId> done;
    for (uint64_t begin = 0; begin < a.sendBytes; begin += chunk) {
        Interval part{begin, std::min(begin + chunk, a.sendBytes)};
        StageId m = Mesh(p, 1, mesh.At(a.rank), own, part, {ready});
        done.push_back(Nhr(p, 2, nhr.At(a.rank), mesh.groups, part, {m}));
    }
    return p.Finish(done);
}
} // namespace dsl
} // namespace mc2_ops_hccl
