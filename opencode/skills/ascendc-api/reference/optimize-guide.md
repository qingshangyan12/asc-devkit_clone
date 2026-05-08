# 性能优化注意事项

本文档提供 AscendC 算子开发中的性能优化关键要点和最佳实践。

---

## 1. Tiling（分块）策略

### 核心思想

将大尺寸 Tensor 切分为多个小块（Tile），逐块搬入片上 UB 进行计算，实现计算与数据搬运的流水并行。

### 分块大小选取原则

```
Tile大小 = f(UB容量, 数据类型, 数据复用需求)
```

| 考虑因素 | 说明 |
|---------|------|
| **UB 容量** | Tile 总大小不超过可用 UB 空间（典型 ~192KB-256KB） |
| **对齐要求** | Tile 大小应为 32B 对齐（硬件burst传输单位） |
| **double buffer** | 需要 2x 缓冲区时，每个tile大小减半 |
| **数据类型** | fp32=4B, fp16/bf16=2B, int8=1B，影响有效元素数 |

### 示例：分块计算

```cpp
// 标准 C++（一次处理全部）
for (int i = 0; i < totalLen; i++) {
    dst[i] = src[i] * scale + bias;
}

// AscendC 优化版本（分块 + double buffer）
uint32_t tileLen = 2048;  // 分块大小
uint32_t loopCount = totalLen / tileLen;
for (uint32_t i = 0; i < loopCount; i++) {
    AscendC::DataCopy(xLocal, xGm[i * tileLen], tileLen);
    AscendC::DataCopy(yLocal, yGm[i * tileLen], tileLen);
    AscendC::Muls(xLocal, xLocal, scale, tileLen);
    AscendC::Adds(zLocal, xLocal, bias, tileLen);
    AscendC::DataCopy(zGm[i * tileLen], zLocal, tileLen);
}
```

---

## 2. Double Buffer（双缓冲/乒乓缓冲）

### 原理

同时分配两块 UB 缓冲区（ping 和 pong），当一块在进行计算时，另一块执行数据搬运，实现**计算与搬运重叠**。

### 双缓冲 vs 单缓冲

```
单缓冲（串行）: [搬运1][计算1][搬运2][计算2][搬运3][计算3] ...
双缓冲（并行）: [搬运1][搬运2][搬运3] ...
                [等待  ][计算1][计算2][计算3] ...
```

### 实现要点

```cpp
// 分配 ping/pong 双缓冲
AscendC::LocalTensor<float> xPing = ubAlloc.Alloc<float, TILE_LEN>();
AscendC::LocalTensor<float> xPong = ubAlloc.Alloc<float, TILE_LEN>();

// 使用 SetFlag/WaitFlag 同步 ping-pong 切换
for (uint32_t i = 0; i < loopCount; i++) {
    if (i % 2 == 0) {
        // 使用 Ping 计算，预取到 Pong
        AscendC::DataCopy(xPong, xGm[(i+1) * TILE_LEN], TILE_LEN);
        AscendC::SetFlag<PIPE_S>(event0);  // 搬运标志
        AscendC::WaitFlag<PIPE_S>(event1); // 等待上次搬运
        AscendC::Mul(zPing, xPing, yPing, TILE_LEN);
        AscendC::DataCopy(zGm[i * TILE_LEN], zPing, TILE_LEN);
        AscendC::SetFlag<PIPE_V>(event2);  // 计算标志
    } else {
        // 使用 Pong 计算，预取到 Ping
        AscendC::DataCopy(xPing, xGm[(i+1) * TILE_LEN], TILE_LEN);
        AscendC::SetFlag<PIPE_S>(event1);
        AscendC::WaitFlag<PIPE_S>(event0);
        AscendC::Mul(zPong, xPong, yPong, TILE_LEN);
        AscendC::DataCopy(zGm[i * TILE_LEN], zPong, TILE_LEN);
        AscendC::SetFlag<PIPE_V>(event3);
    }
}
```

> **提示**：Tpipe/Tque 框架API 已内置双缓冲机制，开发者只需声明缓冲队列的深度即可。

---

## 3. 内存带宽优化

### 3.1 数据搬运合并

尽量在一次 `DataCopy` 中搬运连续大块数据，减少搬运次数：

```cpp
// 差：分开搬运
AscendC::DataCopy(xLocal[0], xGm[0], 256);
AscendC::DataCopy(xLocal[256], xGm[256], 256);

// 好：合并搬运
AscendC::DataCopy(xLocal, xGm, 512);
```

### 3.2 对齐要求

| 数据类型 | UB 地址对齐 | 搬运大小对齐 |
|---------|-----------|------------|
| 通用 | 32B | 32B |
| float/half | 元素对齐 | 32B 倍数 |

### 3.3 减少 GM 访问

- 尽量在 UB 上完成多步计算后再写回 GM
- 充分利用 UB 空间缓存中间结果
- 对于可复用的数据，在 UB 中保留而不是重复搬运

```cpp
// 差：频繁访问 GM
for (i) {
    AscendC::DataCopy(tmp, xGm[i], len);
    AscendC::Muls(tmp, tmp, scale, len);
    AscendC::DataCopy(zGm[i], tmp, len);
}

// 好：在 UB 中缓存复用数据
// 如果 scale 操作需要多次使用数据，保留在 UB
AscendC::DataCopy(xBuf, xGm[offset], blockLen);
AscendC::Muls(xBuf, xBuf, scale, blockLen);     // 操作1
AscendC::Adds(xBuf, xBuf, bias, blockLen);       // 操作2
AscendC::Exp(xBuf, xBuf, blockLen);              // 操作3
AscendC::DataCopy(zGm[offset], xBuf, blockLen);  // 一次性写回
```

---

## 4. 计算优化

### 4.1 向量化优先

AscendC 的核心优势是 SIMD 向量计算。优先使用向量 API（如 `AscendC::Add`）而非标量循环。

### 4.2 融合操作

利用硬件融合指令减少中间结果写回和搬运：

| 替换方案 | 融合API | 优势 |
|---------|---------|------|
| `Mul + Add` 两条指令 | `Fma(dst, a, b, c)` | 减少1次写回 |
| `Mul + Add` (轴加) | `Axpy(dst, a, s)` | 乘加融合 |
| `Mul + Add` 到自身 | `FusedMulDstAdd(a, b, c)` | Dst参与融合 |

### 4.3 计算流水

合理编排指令调度，避免流水线停顿：

```cpp
// 插入 PipeBarrier 确保前序向量计算完成
AscendC::PipeBarrier<PIPE_V>();
// 然后进行下一轮计算
```

---

## 5. Cube 运算优化（矩阵乘）

### 5.1 数据格式

- 矩阵乘使用 **NZ** (N1Z) 格式可获得最优性能
- 在搬运前通过 `DataCopyPad` 进行 ND→NZ 格式转换

### 5.2 Cube 与 Vector 流水

```cpp
// Cube 计算与 Vector 计算可以流水并行
// 使用独立的同步事件分开管理
AscendC::SetFlag<PIPE_M>(cubeDoneEvent);    // Cube 流水事件
AscendC::SetFlag<PIPE_V>(vecDoneEvent);     // Vector 流水事件
```

---

## 6. 性能分析工具

### 6.1 常用分析手段

| 工具 | 获取指标 | 层级 |
|------|---------|------|
| `msprof` | 算子整体耗时 | 算子级 |
| `DumpTensor` | 中间Tensor值（CPU模式） | 元素级 |
| Prof Trace API | 内核代码段耗时 | 代码段级 |

### 6.2 Prof Trace 使用

```cpp
#include "kernel_prof_trace_intf.h"

AscendC::ProfTraceStart(0);   // 标记开始
// ... 要测量的代码段 ...
AscendC::ProfTraceStop(0);    // 标记结束
```

---

## 7. 常见性能问题排查清单

| 现象 | 可能原因 | 排查方向 |
|------|---------|---------|
| 整体耗时高于预期 | 搬运占比过高 | 增大tile大小，减少搬运次数 |
| 同规模下性能不稳定 | 对齐问题 | 检查tile大小是否32B对齐 |
| Cube算子性能差 | 数据格式未优化 | 检查是否使用NZ格式 |
| 多核加速比不理想 | 负载不均衡 | 检查数据分配是否均匀 |
| 尾块处理耗时长 | 尾块单独处理效率低 | 将尾块合并到正常循环中 |

---

## 8. 性能优化决策流程

```
1. 确定性能瓶颈
   ├── 搬运瓶颈 → 增大tile / 合并搬运 / 减少冗余搬运
   ├── 计算瓶颈 → 使用融合指令 / 优化指令排布
   ├── 同步瓶颈 → 减少SetFlag/WaitFlag / 使用更强的解耦
   └── 多核瓶颈 → 负载均衡 / 减少块间同步

2. 验证优化效果
   ├── CPU 模式功能验证
   ├── NPU 模式性能测量（msprof）
   └── 与对标（如GPU实现）对比
```

---

## 9. API层级与性能关系

| API层级 | 可控粒度 | 极限性能 | 开发效率 |
|---------|---------|---------|---------|
| Tpipe/Tque 框架API | 低 | 中-高 | 高 |
| 基础API (C++ Tensor) | 中 | 高 | 中 |
| C API (指针) | 高 | 极高 | 低 |
| Reg API (寄存器级) | 极高 | 极限 | 极低 |

**原则**：优先用高层级 API 快速验证，确认算法正确后，再针对性能瓶颈向下钻取到更低层级 API 进行精细调优。
