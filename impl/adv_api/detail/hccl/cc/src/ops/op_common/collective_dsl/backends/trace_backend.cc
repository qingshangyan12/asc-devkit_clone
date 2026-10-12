// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#include "trace_backend.h"

#include <istream>
#include <map>
#include <ostream>
#include <sstream>
#include <vector>

namespace mc2_ops_hccl {
namespace dsl {
namespace {

std::string SliceText(const Slice& slice)
{
    std::ostringstream text;
    text << (slice.buffer == Buffer::INPUT ? "input" : "output") << '@' << slice.rank << ":owner=" << slice.owner
         << ":offset=" << slice.offset << ":bytes=" << slice.bytes;
    return text.str();
}

void SlicesText(const std::vector<Slice>& slices, std::ostream& output)
{
    for (size_t i = 0; i < slices.size(); ++i) {
        if (i != 0) {
            output << ',';
        }
        output << SliceText(slices[i]);
    }
}

void EventsText(const std::vector<Event>& events, std::ostream& output)
{
    for (size_t i = 0; i < events.size(); ++i) {
        if (i != 0) {
            output << ',';
        }
        output << 'e' << events[i].id;
    }
}

} // namespace

void WriteTrace(const Plan& plan, std::ostream& output)
{
    for (const Instruction& ins : plan.instructions) {
        output << "rank=" << plan.rank << " q=";
        if (ins.op == Op::COMPLETE) {
            output << "final op=complete events=";
            EventsText(ins.dependencies, output);
        } else if (ins.op == Op::WAIT) {
            output << ins.queue << " op=wait events=";
            EventsText(ins.dependencies, output);
        } else if (ins.op == Op::LOCAL_COPY) {
            output << ins.queue << " op=local_copy src=" << SliceText(ins.src) << " dst=" << SliceText(ins.dst)
                   << " done=e" << ins.completion.id;
        } else {
            output << ins.queue << " op=exchange send=" << ins.send.peer << '/' << ins.send.path
                   << " recv=" << ins.recv.peer << '/' << ins.recv.path << " tx_src=";
            SlicesText(ins.txSrc, output);
            output << " tx_dst=";
            SlicesText(ins.txDst, output);
            output << " rx_src=";
            SlicesText(ins.rxSrc, output);
            output << " rx_dst=";
            SlicesText(ins.rxDst, output);
            output << " done=e" << ins.completion.id;
        }
        output << '\n';
    }
}

bool CompareTrace(std::istream& expected, std::istream& actual, std::string& difference)
{
    using QueueTrace = std::map<std::string, std::vector<std::string>>;
    const auto read = [](std::istream& input) {
        QueueTrace result;
        std::string line;
        while (std::getline(input, line)) {
            const size_t op = line.find(" op=");
            result[op == std::string::npos ? "invalid" : line.substr(0, op)].push_back(
                op == std::string::npos ? line : line.substr(op + 1));
        }
        return result;
    };
    const QueueTrace lhs = read(expected);
    const QueueTrace rhs = read(actual);
    if (lhs == rhs) {
        difference.clear();
        return true;
    }
    difference = "trace differs after grouping by rank and queue";
    return false;
}

} // namespace dsl
} // namespace mc2_ops_hccl
