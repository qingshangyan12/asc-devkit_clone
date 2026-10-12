// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#include "all_gather_primitives.h"

#include <algorithm>
#include <stdexcept>

namespace mc2_ops_hccl {
namespace dsl {
namespace {

Slice Out(Rank rank, Rank owner, Interval data)
{
    return Slice{rank, Buffer::OUTPUT, owner, data.begin, data.end - data.begin};
}

size_t RankIndex(const Ranks& ranks, Rank rank)
{
    const auto it = std::find(ranks.begin(), ranks.end(), rank);
    if (it == ranks.end()) {
        throw std::invalid_argument("rank is not in the communication group");
    }
    return static_cast<size_t>(it - ranks.begin());
}

uint32_t CeilLog2(uint32_t value)
{
    uint32_t result = 0;
    for (uint32_t power = 1; power < value; power <<= 1U) {
        ++result;
    }
    return result;
}

std::vector<Interval> Split(Interval data, const std::vector<Channel>& channels)
{
    if (channels.empty()) {
        throw std::invalid_argument("peer has no channel");
    }
    uint64_t totalWeight = 0;
    for (const Channel& channel : channels) {
        if (channel.weight == 0) {
            throw std::invalid_argument("channel weight is zero");
        }
        totalWeight += channel.weight;
    }

    std::vector<Interval> result;
    uint64_t cursor = data.begin;
    uint64_t assigned = 0;
    const uint64_t bytes = data.end - data.begin;
    for (size_t i = 0; i < channels.size(); ++i) {
        const uint64_t part = i + 1 == channels.size() ? bytes - assigned : bytes * channels[i].weight / totalWeight;
        result.push_back(Interval{cursor, cursor + part});
        cursor += part;
        assigned += part;
    }
    return result;
}

void AddSlices(
    Rank local, Rank remote, const Ranks& owners, Interval data, std::vector<Slice>& remoteSlices,
    std::vector<Slice>& localSlices)
{
    if (data.begin == data.end) {
        return;
    }
    for (Rank owner : owners) {
        remoteSlices.push_back(Out(remote, owner, data));
        localSlices.push_back(Out(local, owner, data));
    }
}

Ranks NhrOwners(
    const Ranks& ranks, const std::vector<Ranks>& owners, size_t senderIndex, uint32_t delta, uint32_t count)
{
    Ranks result;
    for (uint32_t i = 0; i < count; ++i) {
        const size_t index =
            (senderIndex + ranks.size() - (static_cast<size_t>(i) * 2U * delta) % ranks.size()) % ranks.size();
        const Ranks& block = owners.at(ranks[index]);
        result.insert(result.end(), block.begin(), block.end());
    }
    return result;
}

} // namespace

const Ranks& Dimension::At(Rank rank) const { return groups.at(rank); }

const std::vector<Channel>& ChannelLayout::To(Rank peer) const { return byPeer.at(peer); }

std::vector<Event> Mesh(
    Builder& ir, Queue queue, const AllGatherArgs& args, const ChannelLayout& channels, const Ranks& ranks,
    const std::vector<Ranks>& owners, Interval data, const std::vector<Event>& dependsOn)
{
    RankIndex(ranks, args.rank);
    ir.Wait(queue, dependsOn);
    std::vector<Event> done;
    for (Rank peer : ranks) {
        if (peer == args.rank) {
            continue;
        }
        const std::vector<Channel>& links = channels.To(peer);
        const std::vector<Interval> lanes = Split(data, links);
        for (size_t path = 0; path < links.size(); ++path) {
            std::vector<Slice> txDst;
            std::vector<Slice> txSrc;
            std::vector<Slice> rxDst;
            std::vector<Slice> rxSrc;
            AddSlices(args.rank, peer, owners.at(args.rank), lanes[path], txDst, txSrc);
            AddSlices(args.rank, peer, owners.at(peer), lanes[path], rxSrc, rxDst);
            if (!txSrc.empty() || !rxSrc.empty()) {
                done.push_back(ir.Exchange(queue, links[path], links[path], txDst, txSrc, rxDst, rxSrc));
            }
        }
    }
    return done;
}

std::vector<Event> Nhr(
    Builder& ir, Queue queue, const AllGatherArgs& args, const ChannelLayout& channels, const Ranks& ranks,
    const std::vector<Ranks>& owners, Interval data, const std::vector<Event>& dependsOn)
{
    const size_t me = RankIndex(ranks, args.rank);
    ir.Wait(queue, dependsOn);
    if (ranks.size() <= 1) {
        return dependsOn;
    }

    std::vector<Event> done;
    const uint32_t steps = CeilLog2(static_cast<uint32_t>(ranks.size()));
    for (uint32_t step = 0; step < steps; ++step) {
        const uint32_t delta = 1U << (steps - 1U - step);
        const size_t sendIndex = (me + delta) % ranks.size();
        const size_t recvIndex = (me + ranks.size() - delta % ranks.size()) % ranks.size();
        const Rank sendPeer = ranks[sendIndex];
        const Rank recvPeer = ranks[recvIndex];
        const uint32_t count = (static_cast<uint32_t>(ranks.size()) - 1U + delta) / (2U * delta);
        const Ranks txOwners = NhrOwners(ranks, owners, me, delta, count);
        const Ranks rxOwners = NhrOwners(ranks, owners, recvIndex, delta, count);
        const std::vector<Channel>& sendLinks = channels.To(sendPeer);
        const std::vector<Channel>& recvLinks = channels.To(recvPeer);
        if (sendLinks.size() != recvLinks.size()) {
            throw std::invalid_argument("NHR send/receive channel count differs");
        }
        const std::vector<Interval> txLanes = Split(data, sendLinks);
        const std::vector<Interval> rxLanes = Split(data, recvLinks);

        done.clear();
        for (size_t path = 0; path < sendLinks.size(); ++path) {
            std::vector<Slice> txDst;
            std::vector<Slice> txSrc;
            std::vector<Slice> rxDst;
            std::vector<Slice> rxSrc;
            AddSlices(args.rank, sendPeer, txOwners, txLanes[path], txDst, txSrc);
            AddSlices(args.rank, recvPeer, rxOwners, rxLanes[path], rxSrc, rxDst);
            if (!txSrc.empty() || !rxSrc.empty()) {
                done.push_back(ir.Exchange(queue, sendLinks[path], recvLinks[path], txDst, txSrc, rxDst, rxSrc));
            }
        }
        if (step + 1U < steps) {
            ir.Wait(queue, done);
        }
    }
    return done;
}

} // namespace dsl
} // namespace mc2_ops_hccl
