// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace asc { namespace ir {
using Rank = uint32_t;
using Ranks = std::vector<Rank>;
class Program;

// core 只记录“哪个 rank 的哪块内存”，不存 GM 指针、Channel 或硬件寄存器。
// offset/bytes 均以字节为单位；通过 slice() 统一检查越界，而非由算法手写检查。
struct Slice {
    const Program* program;
    uint32_t buffer;
    Rank rank;
    uint64_t offset;
    uint64_t bytes;
    Slice slice(uint64_t begin, uint64_t length) const;
};
struct Buffer {
    const Program* program;
    uint32_t id;
    uint64_t bytes; // 每个 rank 的容量，不是所有 rank 的总容量
    std::string name; // trace 中的可读逻辑名称，不参与地址计算
    Slice operator[](Rank rank) const;
};
enum class Op { LocalCopy, Read, Write, Join };
struct Transfer {
    Op op;
    Slice dst;
    Slice src;
};
// 参数顺序始终为 dst, src。Read 与 Write 的方向由远端 Slice 所属 rank 决定。
Transfer LocalCopy(Slice dst, Slice src);
Transfer Read(Slice localDst, Slice remoteSrc);
Transfer Write(Slice remoteDst, Slice localSrc);

// Event 是执行图节点，不是 CKE 或 GM flag；后端才能给它绑定物理事件。
struct Event { const Program* program; uint32_t id; };
struct Node {
    Rank rank;
    uint32_t queue;
    Transfer transfer;
    // after 允许引用稍后构建的对端节点：rank 0 先构图时 rank 1 的入边尚不存在。
    // finish() 配对各 rank 后才补全入边，因此 engine 必须在 finish 之后读取图。
    std::vector<uint32_t> after;
    std::string label;
};

// primitive 生成 Round，core 不知道 Mesh/NHR 的 peer 公式。
// 一个 Round 中的 transfer 可以并发；下一 Round 会依赖本轮收发完成。
struct Round { std::vector<Transfer> transfers; };
struct Exchange {
    Rank rank;
    Ranks ranks; // 有序通信域；所有参与者必须使用相同顺序
    std::string name;
    std::vector<Round> rounds;
};

class Queue {
public:
    Event submit(Transfer transfer);
    Event submit(const Exchange& exchange);
    void enqueueWait(Event event); // 只添加依赖，不阻塞 Host
    Event join(); // 把当前 queue 的全部前置条件合成一个 event，不搬运数据
private:
    friend class Program;
    Queue(Program& program, Rank rank, uint32_t id) : program_(&program), rank_(rank), id_(id) {}
    Program* program_;
    Rank rank_;
    uint32_t id_;
};

// 此阶段的 Program 保存所有 rank 的图，用于无卡验证跨 rank 依赖。
// 它不是通信域/runtime，也不申请真实 NPU 内存。真实 engine 接入仍需独立完成。
// 核心职责：队列顺序 + 显式等待 + 配对各 rank 同一次 exchange 的收发依赖。
class Program {
public:
    explicit Program(uint32_t ranks);
    Program(const Program&) = delete; // Slice/Event 指向本 Program，禁止复制后悬挂或错绑
    Program& operator=(const Program&) = delete;
    Buffer buffer(const std::string& name, uint64_t bytes);
    Queue queue(Rank rank, uint32_t id);
    void finish(); // 配对跨 rank 收发，之后禁止继续构图
    uint32_t rankCount() const { return ranks_; }
    const std::vector<Node>& nodes() const { return nodes_; }
    const std::vector<Buffer>& buffers() const { return buffers_; }
    bool finished() const { return finished_; }
private:
    friend class Queue;
    struct QueueState { std::vector<uint32_t> after; uint32_t exchanges = 0; };
    struct Participant {
        std::vector<uint32_t> before; // 发布给对端：本轮访问我的 buffer 前必须满足的条件
        std::vector<uint32_t> transfers;
        uint32_t done; // join 节点；既等待自己发起的传输，也等待别人访问自己的传输
    };
    struct Group { Ranks ranks; std::map<Rank, Participant> participants; };
    void checkMutable() const;
    void check(Transfer transfer, Rank rank) const;
    uint32_t append(Rank rank, uint32_t queue, Transfer transfer,
                    const std::vector<uint32_t>& after, const std::string& label);
    uint32_t join(Rank rank, uint32_t queue, const std::vector<uint32_t>& after, const std::string& label);
    uint32_t ranks_;
    bool finished_ = false;
    std::vector<Buffer> buffers_;
    std::vector<Node> nodes_;
    std::map<std::pair<Rank, uint32_t>, QueueState> queues_;
    std::map<std::string, Group> groups_;
};
} }
