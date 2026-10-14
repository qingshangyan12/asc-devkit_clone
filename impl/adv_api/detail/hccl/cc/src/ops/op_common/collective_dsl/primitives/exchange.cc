// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "exchange.h"
#include <algorithm>
#include <stdexcept>

namespace asc { namespace ir {
Exchange mesh(Rank p, const Ranks& ranks, const MeshSlices& slices)
{
    // Mesh 不猜数据布局：算法已经逐 peer 写明了 src/dst。
    // 只生成一轮可并发的传输；core 配对所有参与者后补齐收发完成依赖。
    Exchange e{p, ranks, "mesh", {Round{}}};
    for (const auto& peer : slices) {
        if (peer.first == p || std::find(ranks.begin(), ranks.end(), peer.first) == ranks.end())
            throw std::invalid_argument("invalid Mesh peer");
        for (Transfer t : peer.second) {
            if ((t.op == Op::Write && t.dst.rank != peer.first) ||
                (t.op == Op::Read && t.src.rank != peer.first) ||
                (t.op != Op::Write && t.op != Op::Read)) throw std::invalid_argument("Mesh direction mismatch");
            e.rounds[0].transfers.push_back(t);
        }
    }
    return e;
}
Exchange nhr(Rank p, const Ranks& ranks, const NhrSlices& slices)
{
    auto found = std::find(ranks.begin(), ranks.end(), p);
    if (found == ranks.end() || !slices.addresses ||
        (slices.direction != Op::Read && slices.direction != Op::Write))
        throw std::invalid_argument("invalid NHR input");
    const uint64_t m = ranks.size(), r = found - ranks.begin();
    uint32_t steps = 0;
    for (uint64_t power = 1; power < m; power *= 2) ++steps;
    Exchange e{p, ranks, "nhr", {}};
    for (uint32_t t = 0; t < steps; ++t) {
        // 对照 HCCL InsTempAllGatherNHR::GetStepInfo：
        // delta 从最高二次幂递减；模 m 和 blockCount 支持非 2 次幂通信域。
        // 用 group 下标运算，再映射回全局 rank，不假设 rank 编号连续。
        const uint64_t delta = uint64_t{1} << (steps - 1 - t);
        const uint64_t send = (r + delta) % m, recv = (r + m - delta % m) % m;
        const uint64_t blockCount = (m - 1 + delta) / (2 * delta);
        // m=8、p 位于下标 0 时：delta=4/2/1，Write 的 block 分别为
        // {0}、{0,4}、{0,6,4,2}。最后两组包含前几轮收到的块。
        // Read 读取的是 recv rank 会发送的块，所以 base 必须是 recv，而不是 r。
        Round round;
        for (uint64_t i = 0; i < blockCount; ++i) {
            const uint64_t base = slices.direction == Op::Write ? r : recv;
            const Rank block = ranks[(base + m - (i * 2 * delta) % m) % m];
            const Rank peer = ranks[slices.direction == Op::Write ? send : recv];
            const Slices local = slices.addresses(p, block);
            const Slices remote = slices.addresses(peer, block);
            // 相同 block 的两个 holder 必须给出相同数量/长度的槽位。
            // 数量在此检查；长度与地址方向由 core 的 Transfer 校验统一检查。
            if (local.size() != remote.size()) throw std::invalid_argument("NHR block layout mismatch");
            for (size_t j = 0; j < local.size(); ++j) {
                round.transfers.push_back(slices.direction == Op::Write
                    ? Write(remote[j], local[j]) : Read(local[j], remote[j]));
            }
        }
        // 轮间等待由 Queue.submit 展开，不在这里做 Host 阻塞等待。
        e.rounds.push_back(round);
    }
    return e;
}
} }
