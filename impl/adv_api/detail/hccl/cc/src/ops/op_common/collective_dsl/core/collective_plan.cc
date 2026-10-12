// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#include "collective_plan.h"

#include <stdexcept>

namespace mc2_ops_hccl {
namespace dsl {

Builder::Builder(const std::string& name, Rank rank)
{
    plan_.name = name;
    plan_.rank = rank;
}

Event Builder::NewEvent() { return Event{plan_.rank, nextEvent_++}; }

Event Builder::LocalCopy(Queue queue, const Slice& dst, const Slice& src)
{
    if (dst.rank != plan_.rank || src.rank != plan_.rank || dst.bytes != src.bytes) {
        throw std::invalid_argument("LocalCopy requires equal-sized local slices");
    }
    Instruction ins;
    ins.op = Op::LOCAL_COPY;
    ins.queue = queue;
    ins.dst = dst;
    ins.src = src;
    ins.completion = NewEvent();
    plan_.instructions.push_back(ins);
    return ins.completion;
}

Event Builder::Exchange(
    Queue queue, const Channel& send, const Channel& recv, const std::vector<Slice>& txDst,
    const std::vector<Slice>& txSrc, const std::vector<Slice>& rxDst, const std::vector<Slice>& rxSrc)
{
    if (txDst.size() != txSrc.size() || rxDst.size() != rxSrc.size()) {
        throw std::invalid_argument("Exchange source/destination list size differs");
    }
    for (size_t i = 0; i < txSrc.size(); ++i) {
        if (txSrc[i].rank != plan_.rank || txDst[i].rank != send.peer || txSrc[i].bytes != txDst[i].bytes) {
            throw std::invalid_argument("invalid Exchange tx slice");
        }
    }
    for (size_t i = 0; i < rxSrc.size(); ++i) {
        if (rxDst[i].rank != plan_.rank || rxSrc[i].rank != recv.peer || rxSrc[i].bytes != rxDst[i].bytes) {
            throw std::invalid_argument("invalid Exchange rx slice");
        }
    }

    Instruction ins;
    ins.op = Op::EXCHANGE;
    ins.queue = queue;
    ins.send = send;
    ins.recv = recv;
    ins.txDst = txDst;
    ins.txSrc = txSrc;
    ins.rxDst = rxDst;
    ins.rxSrc = rxSrc;
    ins.completion = NewEvent();
    plan_.instructions.push_back(ins);
    return ins.completion;
}

void Builder::Wait(Queue queue, const std::vector<Event>& events)
{
    if (events.empty()) {
        return;
    }
    Instruction ins;
    ins.op = Op::WAIT;
    ins.queue = queue;
    ins.dependencies = events;
    plan_.instructions.push_back(ins);
}

Plan Builder::Finish(const std::vector<Event>& events)
{
    plan_.finalEvents = events;
    Instruction ins;
    ins.op = Op::COMPLETE;
    ins.dependencies = events;
    plan_.instructions.push_back(ins);
    return plan_;
}

} // namespace dsl
} // namespace mc2_ops_hccl
