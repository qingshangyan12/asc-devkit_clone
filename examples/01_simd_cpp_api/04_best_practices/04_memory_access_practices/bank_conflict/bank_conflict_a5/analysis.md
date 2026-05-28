# A5 UB Bank 冲突及地址重叠场景验证与分析（基于 Mul 指令）

## A5 UB 内存结构

在 A5 架构上，UB（Unified Buffer）空间总大小为 **256KB**，采用 **低位地址交织** 编码方式。

与 A2/A3 架构的关键区别：

| 参数 | A2/A3 | A5 |
|---|---|---|
| 总容量 | 192KB | 256KB |
| Bank Group 数 | 16 | 8 |
| 每个 BG 的 Bank 数 | 3 | 2 |
| 总 Bank 数 | 48 | 16 |
| 每个 Bank 大小 | 4KB (128行) | 16KB (512行) |
| Bank Group 大小 | 64KB | 32KB |
| 每个 BG 的读口数 | 1 | **2** |
| 每个 BG 的写口数 | 1 | 1 |
| 地址交织方式 | 高位 `{BANK[1:0], DEPTH, BG, WIDTH}` | **低位** `{DEPTH, BANK, BG, WIDTH}` |
| Subbank | 无 | 每 bank 含 4 个 subbank (每个 8B 宽) |

### A5 UB 地址编码规则

A5 采用 **19 位** 地址编码（UB 256KB = 2^18，但编码使用了 19 位来表示 depth 扩展）：

```
UB_ADDR[18:0] = { DEPTH[8:0], BANK[0], BG[2:0], WIDTH[4:0] }
```

| 位域 | DEPTH[8:0] | BANK[0] | BG[2:0] | WIDTH[4:0] |
|---|---|---|---|---|
| 位数 | 9 bits | 1 bit | 3 bits | 5 bits |
| 含义 | row_id（bank 内行号） | bank 编号 (0/1) | bank group 编号 (0-7) | 行内偏移 (0-31) |

**地址计算公式**：

```
addr = depth * 512 + bank * 256 + bg * 32 + width
```

其中：
- `depth`：0~511（总共 512 行/bank）
- `bank`：0 或 1
- `bg`：0~7
- `width`：0~31

### 低位交织与地址连续扫描

A5 采用低位交织，在连续地址空间中，每 32B 轮换一次 bank group：

```
地址     | depth | bank | bg | 说明
0x0000   | 0     | 0    | 0  | bg=0, bank=0, row=0
0x0020   | 0     | 0    | 1  | bg=1, bank=0, row=0
...
0x00E0   | 0     | 0    | 7  | bg=7, bank=0, row=0
0x0100   | 0     | 1    | 0  | bg=0, bank=1, row=0
...
0x01E0   | 0     | 1    | 7  | bg=7, bank=1, row=0
0x0200   | 1     | 0    | 0  | bg=0, bank=0, row=1 (下一个 depth)
```

对比 A2/A3 的高位交织（每 64KB 才轮换 bank group），A5 的 bank group 在地址空间中密集交错排列：
- 走完 16 个 32B 块（512B）回到同一 bank 的下一行
- 单个 bank 的连续两行之间相隔 512B = 0x200（在 A2/A3 上为 0x4000）
- 相邻的 32B 块始终位于不同的 bank group，天然有利于缓解读读冲突

## Bank 冲突类型

- **读读冲突**：多个读操作同时访问同一 bank group。A5 每个 BG 有 **2 个读口**，因此 ≤2 个并发读不会触发读读冲突（相比 A2/A3 的 1 读口差异显著）。
- **写写冲突**：多个写操作同时访问同一 bank group。
- **读写冲突**：读操作和写操作同时访问同一 bank。A5 每 bank 16KB（512行），bank 是读写冲突的判定单位。

**A5 核心结论**：由于只有 2 个 bank/bg，对于 2 输入 1 输出的算子（如 Mul），必然有一个源操作数与目的操作数共享同一个 bank。读写冲突较 A2/A3（3 个 bank/bg）更易触发，但可通过硬件 dst 延迟优化来规避。

### Subbank 与离散访存

A5 每个 bank 分为 4 个 subbank，每个 subbank 宽 8B（行数与 bank 一致，512 行 × 8B = 4KB）。Subbank 仅在 **gather/scatter 等离散访存接口** 中感知：

| 规则 | 说明 |
|---|---|
| 不同 subbank | 不冲突 |
| 同 subbank 同起始地址 | 不冲突（访问合并） |
| 同 subbank 不同起始地址 | 冲突 |
| 与连续访存/MTE | 单拍只能执行一条 gather/scatter 指令 |

## 测试场景概览

所有场景的输入数据类型为 `float`，数据长度为 4096，Mul 指令参数为：
`{dstBlkStride=1, src0BlkStride=1, src1BlkStride=1, dstRepStride=8, src0RepStride=8, src1RepStride=8}`

> **性能说明**：下表"预期性能"为 Mul 指令连续执行 1000 次的相对 cycle 数（以 Case 1 为基线 = 1.0x）。

| Case | 场景描述 | buf0 | buf1 | buf2 | 预期相对性能 | 关键验证点 |
|---|---|---|---|---|---|---|
| 1 | dst 完全覆盖 src1（硬件优化） | 0x0 | 0x10100 | 0x10100 | 1.0x（基线） | 硬件 dst 延迟规避读写冲突 |
| 2 | dst 偏移 src1（破坏 HW 对齐） | 0x0 | 0x10100 | 0x10120 | ~1.5x | 读写冲突导致性能下降 |
| 3 | 2 读同 bank + dst 异 bank | 0x0 | 0x4000 | 0x20100 | ~1.0x | A5 2 读口消除读读冲突 |
| 4 | 2 读同 bank + dst 同 bank | 0x0 | 0x4000 | 0x8000 | ~1.5x | 读写冲突 + 同 bank 读验证 |

## Case 1：无冲突 —— dst 覆盖 src1（硬件优化）

### 地址解码

| 操作数 | 首地址 | depth | bank | bg | width |
|---|---|---|---|---|---|
| src0 | `0x0` | 0 | 0 | 0 | 0 |
| src1 | `0x10100` | 128 | 1 | 0 | 0 |
| dst | `0x10100` | 128 | 1 | 0 | 0 |

### 分析

- src0 和 src1 位于不同 bank group（src0 遍历 bg=0-7, bank=0 vs src1 遍历 bg=0-7, bank=1），无读读冲突（且 A5 有 2 读口更无影响）。
- src0 和 dst 位于不同 bank（bank=0 vs bank=1），无读写冲突。
- src1 和 dst 地址完全重叠。硬件检测到 dst 写入与 src1 读取在每次 repeat 内访问相同的 bank 序列，自动将 dst 写入延后 1 个 repeat，避免读写冲突。

**预期执行时序**：

| | 第 1 次 repeat | 第 2 次 repeat | 第 3 次 repeat | ... |
|---|---|---|---|---|
| src0 读 | bank=0, bg=0-7 | bank=1, bg=0-7 | bank=0, bg=0-7 | ... |
| src1 读 | bank=1, bg=0-7 | bank=0, bg=0-7 | bank=1, bg=0-7 | ... |
| dst 写 | — | bank=1, bg=0-7 | bank=0, bg=0-7 | ... |

## Case 2：读写冲突 —— dst 偏移 1 个 BG

### 地址解码

| 操作数 | 首地址 | depth | bank | bg |
|---|---|---|---|---|
| src0 | `0x0` | 0 | 0 | 0 |
| src1 | `0x10100` | 128 | 1 | 0 |
| dst | `0x10120` | 128 | 1 | 1 |

### 分析

dst 比 src1 偏移 0x20（= 1 个 BG），导致 dst 和 src1 在每次 repeat 内访问部分相同的 bank，破坏了硬件 dst 延迟优化的对齐条件：

- src1 第 1 次 repeat：bank=1, bg=0-7
- dst 第 1 次 repeat：bank=1, bg=1-8(0) 即 bg=1-7 + bg=0

bg=1-7 期间 src1 和 dst 同时访问 bank=1 → **读写冲突**。在 A2/A3 的对应场景（case 7）中同样存在此类冲突。

## Case 3：A5 2 读口优势 —— 同 bank 双读 + dst 异 bank

### 地址解码

| 操作数 | 首地址 | depth | bank | bg |
|---|---|---|---|---|
| src0 | `0x0` | 0 | 0 | 0 |
| src1 | `0x4000` | 32 | 0 | 0 |
| dst | `0x20100` | 256 | 1 | 0 |

### 分析

- src0（depth=0, bank=0）和 src1（depth=32, bank=0）均位于 **bank=0**。
- 在 A2/A3 上（1 读口/BG），同一 bank 的双读会产生严重的读读冲突（性能 ~1.7x 基线）。
- 在 **A5 上（2 读口/BG）**，两个读操作并行执行，**无读读冲突**，预期性能与基线一致。
- dst 位于 bank=1，与两个源均在不同 bank → 无读写冲突。

这是 A5 相对 A2/A3 的核心性能优势场景。

## Case 4：读写冲突 —— 同 bank 双读 + dst 同 bank

### 地址解码

| 操作数 | 首地址 | depth | bank | bg |
|---|---|---|---|---|
| src0 | `0x0` | 0 | 0 | 0 |
| src1 | `0x4000` | 32 | 0 | 0 |
| dst | `0x8000` | 64 | 0 | 0 |

### 分析

- 三个操作数均位于 **bank=0**。
- src0（depth=0）与 src1（depth=32）：A5 的 2 读口保证双读无冲突。
- dst（depth=64）与 src0（depth=0）或 src1（depth=32）：**同 bank 读写冲突**。dst 每次 repeat 的写入与 src0/src1 的读取共享 bank=0，且 depth 不重叠不触发硬件优化。预期性能显著劣于 Case 3。

## 小结

### A5 相比 A2/A3 的 Bank 冲突变化

| 场景 | A2/A3 表现 | A5 表现 |
|---|---|---|
| 2 个输入读同一 bank/bg | 读读冲突（1 读口） | **无冲突**（2 读口） |
| 读写访问同一 bank（地址对齐） | 硬件优化延迟 dst | 同样支持硬件优化 |
| 读写访问同一 bank（地址偏移） | 读写冲突 | 读写冲突 |
| 3 操作数在 2 个 bank 间分配 | 不可行（3 banks） | **至少一对共享 bank** |

### 核心要点

1. **A5 的 2 读口/BG** 彻底消除了 2 输入算子的读读冲突，这是相对于 A2/A3 的显著性能优势。
2. **只有 2 个 bank/BG** 使得读写冲突更易触发（必然有一对操作数共享 bank），但硬件 dst 延迟优化在地址完全对齐时仍有效。
3. **低位地址交织**使得 bank group 在地址空间中密集交错，有利于分散访问、进一步减少冲突概率。
4. **Subbank** 仅在离散访存（gather/scatter）场景中感知，其冲突规则与连续访存独立。
