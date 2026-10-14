// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "sim_engine.h"
#include <algorithm>
#include <functional>
#include <random>
#include <stdexcept>

namespace asc { namespace ir {
SimEngine::SimEngine(const Program& program) : program_(program)
{
    if (!program.finished()) throw std::logic_error("finish program before executing");
    for (Buffer b : program.buffers()) {
        std::vector<Memory> ranks(program.rankCount());
        for (auto& memory : ranks) {
            memory.data.resize(b.bytes);
            memory.initialized.resize(b.bytes, false);
        }
        memory_.push_back(std::move(ranks));
    }
}
void SimEngine::check(Slice s) const
{
    if (s.program != &program_ || s.buffer >= memory_.size() || s.rank >= program_.rankCount())
        throw std::invalid_argument("invalid simulation slice");
    const uint64_t size = memory_[s.buffer][s.rank].data.size();
    if (s.offset > size || s.bytes > size - s.offset) throw std::out_of_range("simulation slice exceeds buffer");
}
void SimEngine::set(Slice s, const std::vector<uint8_t>& bytes)
{
    check(s);
    if (bytes.size() != s.bytes) throw std::invalid_argument("input size mismatch");
    auto& m = memory_[s.buffer][s.rank];
    std::copy(bytes.begin(), bytes.end(), m.data.begin() + s.offset);
    std::fill(m.initialized.begin() + s.offset, m.initialized.begin() + s.offset + s.bytes, true);
}
std::vector<uint8_t> SimEngine::get(Slice s) const
{
    check(s);
    const auto& m = memory_[s.buffer][s.rank];
    for (uint64_t i = 0; i < s.bytes; ++i)
        if (!m.initialized[s.offset + i]) throw std::runtime_error("read before data publication");
    return {m.data.begin() + s.offset, m.data.begin() + s.offset + s.bytes};
}
void SimEngine::checkHazards() const
{
    const auto& nodes = program_.nodes();
    // 先求依赖图的传递闭包：before[b][a] 表示 a 必须先于 b。
    // 这是离线测试的 O(nodes²) 存储，不是部署到设备的执行器或资源算法。
    std::vector<std::vector<bool>> before(nodes.size(), std::vector<bool>(nodes.size(), false));
    std::vector<uint8_t> visiting(nodes.size());
    std::function<void(uint32_t)> visit = [&](uint32_t id) {
        if (id >= nodes.size()) throw std::runtime_error("invalid dependency");
        if (visiting[id] == 1) throw std::runtime_error("dependency cycle");
        if (visiting[id] == 2) return;
        visiting[id] = 1;
        for (uint32_t dep : nodes[id].after) {
            visit(dep);
            before[id][dep] = true;
            for (uint32_t i = 0; i < nodes.size(); ++i) before[id][i] = before[id][i] || before[dep][i];
        }
        visiting[id] = 2;
    };
    for (uint32_t i = 0; i < nodes.size(); ++i) visit(i);
    auto overlaps = [](Slice a, Slice b) {
        return a.bytes && b.bytes && a.buffer == b.buffer && a.rank == b.rank &&
            a.offset < b.offset + b.bytes && b.offset < a.offset + a.bytes;
    };
    // 仅随机跑到正确结果不够：未建立顺序的读写冲突必须被拒绝，
    // 即使某个随机 seed 恰好以“正确顺序”执行，仍然算缺失同步。
    for (uint32_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].transfer.op == Op::Join) continue;
        for (uint32_t j = i + 1; j < nodes.size(); ++j) {
            if (nodes[j].transfer.op == Op::Join || before[i][j] || before[j][i]) continue;
            const Transfer a = nodes[i].transfer, b = nodes[j].transfer;
            if (overlaps(a.dst, b.src) || overlaps(a.src, b.dst) || overlaps(a.dst, b.dst))
                throw std::runtime_error("unordered buffer read/write hazard");
        }
    }
}
void SimEngine::run(uint32_t seed)
{
    if (!order_.empty()) throw std::logic_error("create a new engine for each replay");
    checkHazards();
    const auto& nodes = program_.nodes();
    std::vector<bool> done(nodes.size(), false);
    std::mt19937 random(seed);
    while (order_.size() < nodes.size()) {
        std::vector<uint32_t> ready;
        for (uint32_t i = 0; i < nodes.size(); ++i) {
            if (done[i]) continue;
            bool available = true;
            for (uint32_t dep : nodes[i].after) available = available && done.at(dep);
            if (available) ready.push_back(i);
        }
        if (ready.empty()) throw std::runtime_error("execution deadlock");
        uint32_t id = ready[std::uniform_int_distribution<size_t>(0, ready.size() - 1)(random)];
        const Transfer t = nodes[id].transfer;
        // 单笔 Transfer 暂按完成时整体可见处理；实际设备还需验证分片可见性/ACK 协议。
        // get 在写入前复制源，支持合法的同址 in-place copy。
        if (t.op != Op::Join) set(t.dst, get(t.src));
        done[id] = true;
        order_.push_back(id);
    }
}
} }
