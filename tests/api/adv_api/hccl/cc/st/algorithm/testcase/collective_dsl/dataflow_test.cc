// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "allgather.h"
#include "sim_engine.h"
#include <functional>
#include <iostream>
#include <stdexcept>

using namespace asc::ir;
namespace {
void require(bool condition, const char* why)
{
    if (!condition) throw std::runtime_error(why);
}
template<class Function> void rejects(Function test, const char* why)
{
    bool rejected = false;
    try { test(); } catch (const std::exception&) { rejected = true; }
    require(rejected, why);
}
Groups full(uint32_t n)
{
    Ranks all;
    // 故意不是升序：primitive 必须使用 group 下标，而非直接算 global rank。
    for (Rank r = n; r > 0; --r) all.push_back(r - 1);
    return Groups(n, all);
}
std::pair<Groups, Groups> grid(uint32_t rows, uint32_t columns)
{
    uint32_t n = rows * columns;
    Groups mesh(n), nhr(n);
    for (uint32_t row = 0; row < rows; ++row) for (uint32_t col = 0; col < columns; ++col) {
        Rank p = n - 1 - (row * columns + col);
        for (uint32_t x = 0; x < columns; ++x) mesh[p].push_back(n - 1 - (row * columns + x));
        for (uint32_t y = 0; y < rows; ++y) nhr[p].push_back(n - 1 - (y * columns + col));
    }
    return {mesh, nhr};
}
std::vector<uint8_t> input(Rank p, uint64_t bytes)
{
    std::vector<uint8_t> values(bytes);
    for (uint64_t i = 0; i < bytes; ++i) values[i] = static_cast<uint8_t>(p * 31 + i * 7);
    return values;
}
using Build = std::function<Event(Program&, Rank, const AllGather&)>;
void check(uint32_t n, uint64_t bytes, const Build& build, bool inplace = false)
{
    Program graph(n);
    Buffer send = graph.buffer("input", bytes), recv = graph.buffer("output", bytes * n);
    for (Rank p = 0; p < n; ++p) {
        // 同一算法入口：只有传入的 input Slice 改为自己的 output 槽位。
        AllGather a{inplace ? recv[p].slice(p * bytes, bytes) : send[p], recv};
        Event done = build(graph, p, a);
        require(done.program == &graph, "algorithm returned foreign event");
    }
    graph.finish();
    // 多 seed 每次重建 memory，仅初始化输入。输出不是预填的正确答案。
    for (uint32_t seed = 0; seed < 5; ++seed) {
        SimEngine engine(graph);
        for (Rank p = 0; p < n; ++p)
            engine.set(inplace ? recv[p].slice(p * bytes, bytes) : send[p], input(p, bytes));
        engine.run(seed);
        for (Rank p = 0; p < n; ++p) for (Rank owner = 0; owner < n; ++owner)
            require(engine.get(recv[p].slice(owner * bytes, bytes)) == input(owner, bytes), "AllGather output mismatch");
        require(engine.executionOrder().size() == graph.nodes().size(), "missing trace operations");
    }
}
void negativeTests()
{
    Program graph(2);
    Buffer b = graph.buffer("buffer", 4);
    rejects([&] { b[0].slice(3, 2); }, "accepted out-of-bounds slice");
    rejects([&] { graph.queue(0, 0).submit(Read(b[0], b[0])); }, "accepted local Read as remote transfer");
    Program other(2);
    Buffer x = other.buffer("other", 4);
    rejects([&] { graph.queue(0, 0).submit(Write(b[1], x[0])); }, "accepted foreign buffer");
    rejects([&] { graph.queue(0, 0).enqueueWait(other.queue(0, 0).join()); }, "accepted foreign event");
    graph.queue(0, 0).submit(mesh(0, {0, 1}, {{1, {Write(b[1], b[0])}}}));
    rejects([&] { graph.finish(); }, "accepted missing rank participant");

    // 两个 queue 没有依赖却访问同一 output：即使某 seed 偶然顺序正确也必须拒绝。
    Program hazard(1);
    Buffer src = hazard.buffer("src", 4), dst = hazard.buffer("dst", 4);
    hazard.queue(0, 0).submit(LocalCopy(dst[0], src[0]));
    hazard.queue(0, 1).submit(LocalCopy(src[0], dst[0]));
    hazard.finish();
    SimEngine bad(hazard);
    bad.set(src[0], {1, 2, 3, 4});
    rejects([&] { bad.run(0); }, "accepted missing queue dependency");

    Program unpublished(1);
    Buffer unknown = unpublished.buffer("uninitialized", 4), target = unpublished.buffer("target", 4);
    unpublished.queue(0, 0).submit(LocalCopy(target[0], unknown[0]));
    unpublished.finish();
    SimEngine uninitialized(unpublished);
    rejects([&] { uninitialized.run(0); }, "accepted unpublished source");
}
void pipelineOverlap()
{
    Program graph(8);
    Buffer send = graph.buffer("input", 53), recv = graph.buffer("output", 8 * 53);
    auto groups = grid(2, 4);
    for (Rank p = 0; p < 8; ++p)
        allGatherPipeline(graph, p, {send[p], recv}, groups.first, groups.second, 13);
    graph.finish();
    const auto& nodes = graph.nodes();
    auto depends = [&](uint32_t node, uint32_t ancestor) {
        std::vector<bool> visited(nodes.size());
        std::vector<uint32_t> pending{node};
        while (!pending.empty()) {
            uint32_t id = pending.back(); pending.pop_back();
            if (id == ancestor) return true;
            if (visited[id]) continue;
            visited[id] = true;
            pending.insert(pending.end(), nodes[id].after.begin(), nodes[id].after.end());
        }
        return false;
    };
    bool overlaps = false;
    // 找到同一个 rank 的 Mesh(chunk 1) 与 NHR(chunk 0) 两笔传输。
    // 若两者互不依赖，拓扑调度器允许它们同时就绪；若全部被串行化，本测试失败。
    for (uint32_t i = 0; i < nodes.size(); ++i) for (uint32_t j = 0; j < nodes.size(); ++j) {
        const auto& a = nodes[i]; const auto& b = nodes[j];
        if (a.rank == 0 && b.rank == 0 && a.transfer.op == Op::Write && b.transfer.op == Op::Write &&
            a.label.find("mesh/") == 0 && b.label.find("nhr/") == 0 &&
            a.transfer.src.offset % 53 == 13 && b.transfer.src.offset % 53 == 0 &&
            !depends(i, j) && !depends(j, i)) overlaps = true;
    }
    require(overlaps, "Pipeline was serialized into Sequence");
}
}
int main()
{
    uint32_t cases = 0;
    for (Op direction : {Op::Read, Op::Write}) {
        for (uint32_t n : {1U, 2U, 3U, 5U, 8U, 9U, 17U}) for (uint64_t bytes : {0ULL, 1ULL, 53ULL}) {
            Groups all = full(n);
            check(n, bytes, [=](Program& g, Rank p, const AllGather& a) { return allGatherSoleMesh(g, p, a, all, direction); });
            check(n, bytes, [=](Program& g, Rank p, const AllGather& a) { return allGatherSoleNhr(g, p, a, all, direction); });
            cases += 2;
        }
        for (uint64_t split : {0ULL, 17ULL, 53ULL}) {
            Groups all = full(8);
            check(8, 53, [=](Program& g, Rank p, const AllGather& a) { return allGatherConcurrent(g, p, a, all, split, direction); });
            ++cases;
            for (auto shape : {std::make_pair(2U, 4U), std::make_pair(3U, 3U)}) {
                auto groups = grid(shape.first, shape.second);
                check(shape.first * shape.second, 53, [=](Program& g, Rank p, const AllGather& a) {
                    return allGatherParallel(g, p, a, groups.first, groups.second, split, direction);
                });
                ++cases;
            }
        }
        for (auto shape : {std::make_pair(2U, 4U), std::make_pair(3U, 3U)}) {
            auto groups = grid(shape.first, shape.second);
            check(shape.first * shape.second, 53, [=](Program& g, Rank p, const AllGather& a) {
                return allGatherSequence(g, p, a, groups.first, groups.second, direction);
            });
            check(shape.first * shape.second, 53, [=](Program& g, Rank p, const AllGather& a) {
                return allGatherPipeline(g, p, a, groups.first, groups.second, 13, direction);
            });
            cases += 2;
        }
    }
    check(8, 53, [](Program& g, Rank p, const AllGather& a) { return allGatherSoleMesh(g, p, a, full(8)); }, true);
    negativeTests();
    pipelineOverlap();
    std::cout << "dataflow_test: PASS, " << cases + 1 << " cases x 5 schedules; negative tests PASS\n";
}
