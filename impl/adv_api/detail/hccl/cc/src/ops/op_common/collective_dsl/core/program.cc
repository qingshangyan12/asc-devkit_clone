// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "program.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace asc { namespace ir {
Slice Slice::slice(uint64_t begin, uint64_t length) const
{
    if (begin > bytes || length > bytes - begin) throw std::out_of_range("slice exceeds buffer view");
    return {program, buffer, rank, offset + begin, length};
}
Slice Buffer::operator[](Rank rank) const
{
    if (rank >= program->rankCount()) throw std::out_of_range("buffer rank outside program");
    return {program, id, rank, 0, bytes};
}
Transfer LocalCopy(Slice dst, Slice src) { return {Op::LocalCopy, dst, src}; }
Transfer Read(Slice dst, Slice src) { return {Op::Read, dst, src}; }
Transfer Write(Slice dst, Slice src) { return {Op::Write, dst, src}; }

Program::Program(uint32_t ranks) : ranks_(ranks)
{
    if (!ranks) throw std::invalid_argument("empty rank domain");
}
void Program::checkMutable() const
{
    if (finished_) throw std::logic_error("program already finished");
}
Buffer Program::buffer(const std::string& name, uint64_t bytes)
{
    checkMutable();
    Buffer b{this, static_cast<uint32_t>(buffers_.size()), bytes, name};
    buffers_.push_back(b);
    return b;
}
Queue Program::queue(Rank rank, uint32_t id)
{
    checkMutable();
    if (rank >= ranks_) throw std::out_of_range("queue rank outside program");
    return Queue(*this, rank, id);
}
void Program::check(Transfer t, Rank rank) const
{
    for (Slice s : {t.src, t.dst}) {
        if (s.program != this || s.rank >= ranks_ || s.buffer >= buffers_.size())
            throw std::invalid_argument("slice belongs to a different program or invalid buffer");
        const uint64_t capacity = buffers_[s.buffer].bytes;
        if (s.offset > capacity || s.bytes > capacity - s.offset) throw std::out_of_range("invalid slice");
    }
    if (t.src.bytes != t.dst.bytes) throw std::invalid_argument("transfer size mismatch");
    if (t.src.bytes && t.src.rank == t.dst.rank && t.src.buffer == t.dst.buffer &&
        t.src.offset != t.dst.offset && t.src.offset < t.dst.offset + t.dst.bytes &&
        t.dst.offset < t.src.offset + t.src.bytes)
        throw std::invalid_argument("partially overlapping copy is not supported");
    if ((t.op == Op::LocalCopy && (t.src.rank != rank || t.dst.rank != rank)) ||
        (t.op == Op::Read && (t.dst.rank != rank || t.src.rank == rank)) ||
        (t.op == Op::Write && (t.src.rank != rank || t.dst.rank == rank)) || t.op == Op::Join)
        throw std::invalid_argument("invalid local/remote transfer direction");
}
uint32_t Program::append(Rank rank, uint32_t queue, Transfer t,
                        const std::vector<uint32_t>& after, const std::string& label)
{
    nodes_.push_back({rank, queue, t, after, label});
    return static_cast<uint32_t>(nodes_.size() - 1);
}
uint32_t Program::join(Rank rank, uint32_t queue, const std::vector<uint32_t>& after, const std::string& label)
{
    return append(rank, queue, {Op::Join, {}, {}}, after, label);
}
Event Queue::submit(Transfer t)
{
    // 单笔操作承接本 queue 的前置条件；完成后成为后续操作的前置节点。
    // 只写图，不执行 memcpy，更不会在 Host 等待这个 Event。
    program_->checkMutable();
    program_->check(t, rank_);
    auto& state = program_->queues_[{rank_, id_}];
    uint32_t node = program_->append(rank_, id_, t, state.after, "transfer");
    state.after = {node};
    return {program_, node};
}
void Queue::enqueueWait(Event e)
{
    program_->checkMutable();
    if (e.program != program_ || e.id >= program_->nodes_.size()) throw std::invalid_argument("invalid event");
    program_->queues_[{rank_, id_}].after.push_back(e.id);
}
Event Queue::join()
{
    program_->checkMutable();
    auto& state = program_->queues_[{rank_, id_}];
    uint32_t done = program_->join(rank_, id_, state.after, "queue/done");
    state.after = {done};
    return {program_, done};
}
Event Queue::submit(const Exchange& e)
{
    // 例：NHR 有三轮，则产生 round0 transfers→done0→round1 transfers→done1→...
    // submit 时只知道本 rank 的操作。finish 才能把对端的接收/源复用条件连进来。
    program_->checkMutable();
    if (e.rank != rank_ || e.ranks.empty()) throw std::invalid_argument("exchange rank mismatch");
    Ranks unique = e.ranks;
    std::sort(unique.begin(), unique.end());
    if (unique.back() >= program_->ranks_ || std::adjacent_find(unique.begin(), unique.end()) != unique.end() ||
        !std::binary_search(unique.begin(), unique.end(), rank_)) throw std::invalid_argument("invalid exchange group");
    auto& state = program_->queues_[{rank_, id_}];
    const uint32_t call = state.exchanges++;
    // key 只用于跨 rank 配对同一次 primitive，不是硬件 signal 编号。
    // queue 的 exchange 次序与 ranks 顺序必须在参与者之间一致；不一致在 finish 报错。
    for (uint32_t round = 0; round < e.rounds.size(); ++round) {
        std::ostringstream key;
        key << e.name << '/' << id_ << '/' << call << '/' << round;
        for (Rank r : e.ranks) key << '/' << r;
        auto& group = program_->groups_[key.str()];
        group.ranks = e.ranks;
        Program::Participant p;
        p.before = state.after;
        for (Transfer t : e.rounds[round].transfers) {
            program_->check(t, rank_);
            if (t.op == Op::LocalCopy) throw std::invalid_argument("exchange expects Read/Write");
            const Rank peer = t.op == Op::Write ? t.dst.rank : t.src.rank;
            if (std::find(e.ranks.begin(), e.ranks.end(), peer) == e.ranks.end())
                throw std::invalid_argument("peer outside exchange group");
            p.transfers.push_back(program_->append(rank_, id_, t, p.before, key.str()));
        }
        p.done = program_->join(rank_, id_, p.before, key.str() + "/done");
        group.participants.emplace(rank_, p);
        state.after = {p.done};
    }
    // 单 rank NHR 没有轮次，但仍返回合法完成节点，保持 queue API 一致。
    const uint32_t done = program_->join(rank_, id_, state.after, e.name + "/done");
    state.after = {done};
    return {program_, done};
}
void Program::finish()
{
    if (finished_) return;
    for (const auto& entry : groups_)
        if (entry.second.participants.size() != entry.second.ranks.size())
            throw std::invalid_argument("unmatched exchange: " + entry.first);
    for (auto& entry : groups_) {
        auto& g = entry.second;
        if (g.participants.size() != g.ranks.size()) throw std::invalid_argument("unmatched exchange: " + entry.first);
        for (auto& part : g.participants) {
            auto& p = part.second;
            for (uint32_t id : p.transfers) {
                auto& node = nodes_[id];
                const Rank peer = node.transfer.op == Op::Write ? node.transfer.dst.rank : node.transfer.src.rank;
                auto& remote = g.participants.at(peer);
                // 同时满足本地和远端的 buffer-ready：不能在对端初始化前写入或读取。
                node.after.insert(node.after.end(), remote.before.begin(), remote.before.end());
                // 这笔传输既影响发起者完成，也影响远端完成。
                // Write: 对端得到新数据；Read: 对端源数据可以安全复用。
                // 因此下一 NHR round 真正依赖收到的数据，而非只依赖自己的发送。
                nodes_[p.done].after.push_back(id);
                nodes_[remote.done].after.push_back(id);
            }
        }
    }
    finished_ = true;
}
} }
