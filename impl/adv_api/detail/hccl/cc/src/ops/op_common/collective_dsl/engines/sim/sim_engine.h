// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#pragma once
#include "program.h"

namespace asc { namespace ir {
// engine 负责执行 IR，而不是重新实现 Mesh/NHR。
// 这个 engine 使用 CPU 内存和随机就绪调度，测试数据正确性及依赖是否充分。
// 它不是 HComm/CCU 的 mock ABI，也不能证明设备指令、硬件可见性或性能正确。
class SimEngine {
public:
    explicit SimEngine(const Program& program);
    void set(Slice dst, const std::vector<uint8_t>& bytes); // 初始化真实输入，不预填 output
    std::vector<uint8_t> get(Slice src) const;
    void run(uint32_t seed);
    const std::vector<uint32_t>& executionOrder() const { return order_; }
private:
    struct Memory { std::vector<uint8_t> data; std::vector<bool> initialized; };
    void check(Slice slice) const;
    void checkHazards() const;
    const Program& program_;
    std::vector<std::vector<Memory>> memory_; // buffer → rank → bytes
    std::vector<uint32_t> order_; // 插桩记录实际调度顺序，非另一份伪执行 trace
};
} }
