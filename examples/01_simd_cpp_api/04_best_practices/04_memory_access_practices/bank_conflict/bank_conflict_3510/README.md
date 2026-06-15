# UB Bank 冲突及离散访存场景验证与分析(架构3510)

## UB（Unified Buffer）内存结构

![UB Architecture](./figures/ub_arch.png)

- 在 Ascend 950PR/Ascend 950DT 中，UB 空间大小为 256KB；
- 共有 16 个 **bank**，组成 8 个 **bank group**，每个 bank group 包含 2 个 bank（`bank_id` = 0, 1）；
- 每个 bank 由 512 行组成（`BANK_DEPTH = 512`），每行长度为 32B（`BANK_WIDTH = 32B`），即每个 bank 大小为 16KB；
- 每个bank group最多支持两读或者一读一写。
- bank_number 相同代表是同一个 bank；bank_group_id 相同代表是同一个 bank group。

## subbank内存结构

**subbank内存结构只有在使用离散与聚合API时需要关注**

Ascend 950PR/Ascend 950DT的每个 bank（32B 宽）分为 4 个 subbank，各 8B 宽 × 512 行深度，各有独立读口：

```
bank 内部 layout（一行 32B）：
┌────────┬────────┬────────┬────────┐
│ sub0   │ sub1   │ sub2   │ sub3   │
│  8B    │  8B    │  8B    │  8B    │
└────────┴────────┴────────┴────────┘
```

使用离散与聚合API时，每个元素可能落入不同的 subbank。同一 subbank 内多个不同地址的读取需要排队，产生 subbank 冲突。

## UB 地址编码规则

Ascend 950PR/Ascend 950DT的 UB 18 位地址采用**低位交织**编码规则：

`UB_ADDR[17:0] = { DEPTH[8:0], BANK[0], BG[2:0], WIDTH[4:0] }`

| 位域 | DEPTH[8:0] | BANK[0] | BG[2:0] | WIDTH[4:0] |
| --- | --- | --- | --- | --- |
| 位数 | 9 bits | 1 bit | 3 bits | 5 bits |
| 含义 | row_id（bank 内行号） | bank_id（bank group 内 bank 编号） | bank_group_id | offset（行内偏移） |

**解码示例**：地址 `0x4100`

将地址转为 18 位二进制，按位域划分：`00 0100 0001 0000 0000`

| 位域 | 二进制值 | 十进制 | 含义 |
| --- | --- | --- | --- |
| DEPTH[8:0] | `00 0100 000` | 32 | row_id = 32 |
| BANK[0] | `1` | 1 | bank_id = 1 |
| BG[2:0] | `000` | 0 | bank_group_id = 0 |
| WIDTH[4:0] | `00000` | 0 | offset = 0 |

## DataBlock 映射

一条 VF Load/Store 指令每次处理 256B = 8 个 DataBlock。每个 DataBlock = 32B。

从 `base` 地址开始的 8 个 DataBlock 映射到 8 个不同 bank group：

| DataBlock | 偏移 | 物理 bank | BG |
|:--:|:----:|:---------:|:--:|
| 0 | +0x00 | b | b%7 |
| 1 | +0x20 | b+1 | (b+1)%7 |
| ... | ... | ... | ... |
| 7 | +0xE0 | b+7 | (b+7)%7 |

其中 `b = (base >> 5) & 0xF`。8 个 DataBlock 覆盖 BG 0-7 各一次。

## bank 冲突类型（适用于Reg数据搬入/Reg数据搬出）

根据内存结构，bank 冲突主要分为以下三种类型：
- **读读冲突**：对比Atlas A2/A3系列产品，bankgroup下的读读冲突减少，多个读操作同时访问同一个bank或两个以上读操作同时访问同一个 bank group才会引发冲突。
- **写写冲突**：多个写操作同时访问同一个 bank group。
- **读写冲突**：读操作和写操作同时访问同一个 bank。

## subbank冲突规则（适用离散与聚合API）

- 离散数据属于**不同 subbank**，无冲突。
- 离散数据属于**同 subbank的同起始地址**，无冲突（硬件合并为一次读）。
- 离散数据属于**同 subbank + 不同起始地址**，发生冲突。

## 测试场景概览

本文围绕 bank 冲突和离散访存两个角度展开验证分析。所有场景的输入数据类型为 `float`（离散访存场景的 index 为 `uint32_t`），通过循环展开执行 1000 次以放大性能差异。

> **性能说明**：下表"打点性能"为指令连续执行 1000 次的总 cycles 数（`GetSystemCycle` 打点），数值越低性能越好。

### 基于Reg数据搬入API和Reg数据搬出API的bank冲突场景（Case 1-7）

| case | 计算公式 | 场景描述 | bank 冲突情况 | src0 首地址 | src1 首地址 | dst 首地址 | 打点性能(cycles) | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | dst = src0 + src1 | **地址重叠完全不冲突**：src0=src1，硬件合并为单次读取 | **无冲突** | 0x0000 | 0x0000 | 0x4100 | 1,122,381 | **性能基准** |
| 2 | dst = src0 + src1 | **src0与dst地址重叠**: src1不同bank，读写冲突 | **读写冲突** | 0x0000 | 0x4100 | 0x0000 | TBD | src0/dst重叠 |
| 3 | dst = src0 + src1 | **全冲突(读读+读写)**: 三路同 bank=0，无循环展开 | **全冲突** | 0x0000 | 0x4000 | 0x8000 | 1,422,763 | 全冲突参照 |
| 4 | dst = src0 + src1 | 消除读读冲突：src0/src1 不同 bank | 仅读写冲突 | 0x0000 | 0x4100 | 0x8900 | 1,440,726 | 读读冲突开销极小(2读口可分摊) |
| 5 | dst = src0 + src1 | 消除读写冲突：src0/src1 同 bank，dst 不同 bank | 仅读读冲突 | 0x0000 | 0x4000 | 0x8100 | 1,333,115 | 读写冲突是瓶颈(仅1写口)+地址连续增益 |
| 6 | dst = src0 + src1 | 全冲突 + 循环展开：同 Case 3，循环展开优化 | **全冲突** | 0x0000 | 0x4000 | 0x8000 | 1,330,823 | 循环展开有 6.5% 收益 |
| 7 | dst = src0 + src1 | 全冲突 + 双搬入 DINTLV_B32 | 无读读冲突 | 0x0000 | 0x4000 | 0x8000 | 1,480,826 | 双搬入无收益(慢 4.1%) |

### 离散访存场景（Case 8-11）

| case | 计算公式 | 场景描述 | subbank 冲突情况 | index 地址 | data 地址 | index 模式 | 打点性能(cycles) | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 8 | Gather + Add + Scatter | Gather数据索引全为0，起始地址全部重叠 | 同地址合并为单次读取，无冲突 | 0x0000 | 0x0400 | 全 0 | 650,798 | 基准参照 |
| 9 | Gather + Add + Scatter | Gather数据索引[0..7]重复 | 同地址合并读取，无冲突 | 0x0000 | 0x0400 | [0..7]×32 重复 | 650,886 | 同地址合并无冲突 |
| 10 | Gather + Add + Scatter | Gather数据索引[0..255]连续 | 跨 bank 分散，无冲突 | 0x0000 | 0x0400 | [0..255] 连续 | 650,689 | 跨bank分散无冲突 |
| 11 | Gather + Add + Scatter | Gather数据索引0、128交替重复 | **同 subbank 不同行冲突** | 0x0000 | 0x0400 | 0、128 交替 | **750,828** | 对比 Case 8 |

### 非连续搬运跨步对比（Case 12-13）

| case | 计算公式 | 场景描述 | 冲突情况 | src0 首地址 | dst 首地址 | 搬运方式 | 打点性能(cycles) | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 12 | dst = src0 | 512元素按行计算，连续8个DataBlock，stride=1 | 无冲突 | 0x0000 | 0x4100 | DataBlock连续搬运 | 440,904 | 基准 |
| 13 | dst = src0 | **512元素按列计算，间隔256B 8个不连续DataBlock，stride=8** | **读读冲突** | 0x0000 | 0x4100 | DataBlock非连续搬运 | 1,510,739 | 竖着搬运慢 3.43x |

> **说明**：Case 12/13 的 `TOTAL_LENGTH = 512`，各计算 512 个 float32。

各 case 的详细分析见下文对应章节。

## Case 1：二读一写，地址重叠完全不冲突 — 唯一实现2R1W无冲突的方式

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| dst | `0x4100` | `010000010 0 000 00000` | 1 | 0 |

### UB 读写示意

(同 Case 5 的图)

### 分析结论

- src0 和 src1 地址完全重叠，共用同一块 UB 空间。硬件地址匹配合并后只触发一次 Load，有效变为 1R1W。
- 由于 dst 在 bank1，与 src0/src1 不在同一 bank，**完全不冲突**。这是 3510 上唯一能让 2R1W 完全无冲突的方式。
- Case 1 vs Case 3 = 1,122,381 vs 1,422,763 cycles（快 21.1%）。

## Case 2：二读一写，src0与dst地址重叠读写冲突 — src0=dst=bank0，src1=bank1

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x4100` | `010000010 0 000 00000` | 1 | 0 |
| dst | `0x0000` | `000000000 0 000 00000` | 0 | 0 |

### 分析结论

- src0 与 dst 地址重叠（同 bank 0 同 depth 行），读取 src0 和写入 dst 同时进行，**存在读写冲突**。
- src1 在 bank1（同 BG 不同 bank），与 src0 无读读冲突，但三路 tensor 均在 BG 0，超出单个 BG 的端口能力（最多 2 读或 1 读 1 写）。
- 与 Case 1 对比：Case 1 通过 src0=src1 重叠消除读端口压力（硬件合并为 1 读），Case 2 则是 src0=dst 重叠，读写冲突不可避免。

## Case 3：二读一写，全冲突基准 — src0、src1和dst同一bank，无循环展开

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x4000` | `010000000 0 000 00000` | 0 | 0 |
| dst | `0x8000` | `100000000 0 000 00000` | 0 | 0 |

### UB 读写示意

![case1_2](./figures/case1_2.png)

- 未做循环展开，VF单次迭代内处理 1 个 repeat，共迭代 `total_iterations = 4096 × 4 ÷ 256 = 64` 次。

### 分析结论

- src0、src1、dst 三路首地址均映射到 bank 0：虽然 8 个 DataBlock 分散在 BG 0-7，但三路 tensor 的对应 DataBlock 都落在相同 bank，**存在读读冲突和读写冲突**。
- 作为二读一写场景的全冲突基准 Case，性能为 1,422,763 cycles。

## Case 4：二读一写，消除读读冲突 — src0与src1不同bank，dst与src1同bank

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x4100` | `010000010 0 000 00000` | 1 | 0 |
| dst | `0x8900` | `100010010 0 000 00000` | 1 | 0 |

### UB 读写示意

![case3](./figures/case3.png)

### 分析结论

- src0=bank0，src1=bank1，dst=bank1。src0 和 src1 落在不同 bank，读读冲突消除。但 src1 和 dst 同在 bank1，仍存在读写冲突。
- Case 4 vs Case 3 = 1,440,726 vs 1,422,763 cycles（Case 4 反而慢 1.3%）。消除读读冲突后性能未提升，说明 **3510 上读读冲突开销极小，不是瓶颈**。原因：每个 BG 有 2 个读口，读读冲突可被另一个读口分摊；每个 BG 只有 1 个写口，读写冲突才是严格瓶颈。此外 Case 4 的 buffer 间存在 256B/2048B 空隙，地址不连续也略微削弱性能。

## Case 5：二读一写，消除读写冲突 — src0与src1同bank，dst在不同bank

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x4000` | `010000000 0 000 00000` | 0 | 0 |
| dst | `0x8100` | `100000010 0 000 00000` | 1 | 0 |

### UB 读写示意

![case4](./figures/case4.png)

### 分析结论

- src0=bank0，src1=bank0，dst=bank1。src0 和 src1 同落在 bank0，存在读读冲突。但 dst 在 bank1，读写冲突消除。
- Case 5 vs Case 3 = 1,333,115 vs 1,422,763 cycles（快 6.3%）。与 Case 4 相比：Case 5 快 8.1%，核心原因是消除了读写冲突（每个 BG 只有 1 个写口，是严格瓶颈），而 Case 4 消除的读读冲突（2 个读口可分摊）几乎无价值。次要因素：Case 5 的三块 buffer（0x0000→0x4000→0x8100）完全连续无空隙，地址连续性也带来小幅增益。

## Case 6：二读一写，全冲突 + 循环展开

### 地址解码

地址配置同 Case 3，详见 Case 3 地址解码。

### UB 读写示意

![case1_2](./figures/case1_2.png)

- 循环展开每次迭代处理 2 个 repeat，共迭代 `total_iterations = 4096 × 4 ÷ 256 ÷ 2 = 32` 次。

### 分析结论

- 地址布局与 Case 3 完全相同，冲突特征相同。
- 唯一差异是使用循环展开，每次处理两个 repeat，增加指令级并行。
- Case 6 vs Case 3 = 1,330,823 vs 1,422,763 cycles（快 6.5%），循环展开在该场景下有正收益（属于指令级优化，非 bank 冲突优化）。

## Case 7：二读一写，双搬入 DINTLV_B32 交织读取

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x4000` | `010000000 0 000 00000` | 0 | 0 |
| dst | `0x8000` | `100000000 0 000 00000` | 0 | 0 |

### UB 读写示意

![case5](./figures/case5.png)

### 分析结论

- 使用 `LoadDist::DIST_DINTLV_B32` 双搬入模式，一条指令同时取回 src0 和 src1（各取 64 bytes 交错排列），消除读读冲突。
- Case 7 vs Case 3 = 1,480,826 vs 1,422,763 cycles（慢 4.1%），**双搬入在 Ascend 950PR/Ascend 950DT 上没有带来收益**。

## Case 8：离散读写，Gather指令的数据索引全为0，起始地址全部重叠

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| index | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| data | `0x0400` | `000001000 0 000 00000` | 0 | 0 |
| dst | `0x0400` | `000001000 0 000 00000` | 0 | 0 |

### UB 读写示意

![case11](./figures/case11.png)

- index 模式：全部为 0，即每次 Gather 都从 data[0] 读取同一个位置。

### 分析结论

- index 全 0，Gather 所有 64 个元素命中同一地址 → 硬件通过地址合并将 64 次请求合并为单次读取。
- 性能 650,798 cycles，作为离散访存基准（用于对比 Case 11 的 subbank 冲突开销）。

## Case 9：离散读写，Gather指令的数据索引在[0..7]重复

### 地址解码

同 Case 8 地址配置。

### UB 读写示意

![case8](./figures/case8.png)

- 索引排布：`[0, 1, 2, 3, 4, 5, 6, 7]` 重复 32 次（共 256 个元素），由 Gather 读取；
- data buffer 起始地址 `0x0400`，每条 Gather 指令从 data 中按 index 取 64 个 float32；
- dst 与 data 地址重叠（`0x0400`），Scatter 写回同一区域。

### 分析结论

- Gather 从 data 中按 index 取 64 个元素：index 仅有 [0..7] 共 8 个 unique 值，很多元素命中相同地址 → 硬件自动合并同地址请求，同地址只需一次读取。
- 每 subbank 仅 2 个 unique addr，读取 2 次即完成，**无 subbank 冲突**。性能为 650,886 cycles。

## Case 10：离散读写，Gather指令的数据索引在[0..255]连续

### 地址解码

同 Case 8 地址配置。

### UB 读写示意

![case9](./figures/case9.png)

- index 模式：`[0, 1, 2, ..., 255]` 连续递增，每条 Gather 指令处理 64 个连续 index；
- data buffer 起始地址 `0x0400`，dst 与 data 地址重叠。

### 分析结论

- Gather 跨 8 个 bank 分散读取，每 subbank 仅 2 次读操作。连续 index 自然分散到不同 subbank，**无 subbank 冲突**。
- 性能 650,689 cycles，与 Case 9 相当。

## Case 11：离散读写，Gather指令的数据索引0,128交替

### 地址解码

同 Case 8 地址配置。

### UB 读写示意

![case10](./figures/case10.png)

- index 模式：偶数位置为 `0`，奇数位置为 `128`（`[0, 128, 0, 128, ...]` 交替）；
- `0` 和 `128` 在 Ascend 950PR/Ascend 950DT 上都落入 bank 0 的同一 subbank，但属于**不同 depth 行**。

### 分析结论

- Gather 命中**同 subbank + 不同起始地址**（相同 subbank，不同 depth 行），单读口需排队，取出时间翻倍。
- Case 11 vs Case 8 = 750,828 vs 650,798 cycles（**+15.4%**）。

## Case 12：对512个（8*64）元素进行加一操作，按行计算，每次repeat读取8个连续的DataBlock

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| dst | `0x4100` | `010000010 0 000 00000` | 1 | 0 |

### 搬运方式

![case12](./figures/case12.png)

```
i=0: src0[0..63]     → dst[0..63]
i=1: src0[64..127]   → dst[64..127]
...
i=7: src0[448..511]  → dst[448..511]
```

8 轮 × 每轮 64 元素（8 个连续 DataBlock），横着铺满 512 个 float32。

### 分析结论

- `DATA_BLOCK_COPY` stride=1：8 个 DataBlock 连续排布（间隔 32B），等价于普通 `LoadAlign`。
- 性能 440,904 cycles，作为竖着搬运的基准参照。

## Case 13：对512个（8*64）元素进行加一操作，按列计算，每次repeat读取8个不连续的DataBlock

### 地址解码

同 Case 12 地址配置。

### 搬运方式

![case13](./figures/case13.png)

```
i=0: src0[0..7],  src0[64..71],  ..., src0[448..455]  → dst 同上
i=1: src0[8..15], src0[72..79],..., src0[456..463]  → dst 同上
...
i=7: src0[56..63], ..., src0[504..511]  → dst 同上
```

8 轮 × 8 个 DataBlock，每个 DataBlock 间隔 **256B（stride=8 × 32B = 64 float）**，竖着覆盖 512 个 float32。

### 分析结论

- `DATA_BLOCK_COPY` stride=8：8 个 DataBlock 均从 bankgroup0 的不同 depth 行读取，命中**同 bank group 不同行**。
- Case 13 vs Case 12 = 1,510,739 vs 440,904 cycles（**慢 3.43x**）。


## 小结

- **二读一写冲突必然发生**：Ascend 950PR/Ascend 950DT 仅有 8 个 BG，一条 VF 指令覆盖全部 BG。2R1W 模式下只要 src0 ≠ src1，每个 BG 必然二读一写，超出单个 BG 的端口能力（最多二读或一读一写），冲突不可避免。**地址重叠（Case 1）是唯一能让 2R1W 完全无冲突的方式**（硬件将两读合并为一读）。
- **读写冲突是主要瓶颈，读读冲突可忽略**：Case 5 消除读写冲突后快 6.3%，Case 4 消除读读冲突后反而慢 1.3%。根本原因是 3510 每个 BG 有 2 个读口但仅 1 个写口——读读冲突可被第二个读口分摊，读写冲突无法分摊。
- **双搬入无收益**：Case 7 vs Case 3 = 1,480,826 vs 1,422,763 cycles（慢 4.1%），Ascend 950PR/Ascend 950DT 上读读冲突不是瓶颈。
- **指令优化带来额外收益**（非 bank 冲突原因）：地址重叠（Case 1）省 Load，比全冲突快 21.1%；循环展开（Case 6 vs Case 3）有 6.5% 收益。
- **后续可扩展更多读取模式**：当前仅覆盖了 LoadAlign、DINTLV_B32、DATA_BLOCK_COPY 三种读模式，后续可补充 `DIST_BLK`（同一 DataBlock 重复读取）和不同 stride（2/4/16）的 DATA_BLOCK_COPY 对比，探究不同 LoadDist 下的 bank 冲突差异。

## 目录结构

```text
├── bank_conflict.asc              // 算子 host + device 代码
├── config.h                      // 13 个 Case 的地址/类型配置
├── vf_func.h                     // Vector 函数实现
├── data_utils.h                  // 数据读写工具
├── CMakeLists.txt                // 编译工程
├── test.sh                       // 测试脚本
├── figures/                      // 各 Case UB 内存分布图
│   ├── case1_2.png
│   ├── case3.png
│   ├── case4.png
│   ├── case5.png
│   ├── case6.png
│   ├── case7.png
│   ├── case8.png
│   ├── case9.png
│   ├── case10.png
│   ├── case11.png
│   ├── case12.png
│   └── case13.png
└── scripts/
    ├── gen_data.py               // 测试数据生成
    └── verify_result.py          // 精度验证
```

## 运行方式

### 环境准备

```bash
source /path/to/cann/set_env.sh
```

### 编译与运行（单个 Case）

```bash
export CASE_NUM=1          # 1-13 可选
mkdir -p build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-Ascend 950PR/Ascend 950DT -DCASE_NUM=${CASE_NUM}
make -j
python3 ../scripts/gen_data.py --case ${CASE_NUM}
./demo                     # 精度验证 + 打点性能（输出 cycles 计数到 stdout）
python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin
```

### 批量运行

```bash
bash test.sh             # 精度验证
bash test.sh profile     # 精度验证 + profiling
```

### CMake 选项

| 选项 | 可选值 | 说明 |
|:----:|:------:|:----|
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构版本 |
| `CASE_NUM` | 1-13 | 切换测试场景 |
| `CMAKE_ASC_RUN_MODE` | `npu` | 上板运行模式 |
