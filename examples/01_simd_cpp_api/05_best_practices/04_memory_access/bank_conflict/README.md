# UB Bank 冲突及地址重叠场景验证与分析（基于 Add 指令）

## UB（Unified Buffer）内存结构

![UB 内存结构图](./figures/UB%E5%86%85%E5%AD%98%E7%BB%93%E6%9E%84%E5%9B%BE.jpg)

- 在 A2/A3 架构上，UB 空间大小为 192KB；
- 共有 48 个 **bank**，组成 16 个 **bank group**，每个 bank group 包含 3 个 bank（`bank_id` = 0, 1, 2）；
- 每个 bank 由 128 行组成（`BANK_DEPTH = 128`），每行长度为 32B（`BANK_WIDTH = 32B`），即每个 bank 大小为 4KB；
- **bank 全局编码规则**：`bank_number = bank_group_id + bank_id * 16`。
- bank_number相同代表是同一个bank；bank_group_id相同代表是同一个bank group。

## UB 地址编码规则

UB 18 位地址编码遵循如下规则：

`UB_ADDR[17:0] = { BANK[1:0], BANK_DEPTH[6:0], BG[3:0], BANK_WIDTH[4:0] }`

| 位域 | BANK[1:0] | BANK_DEPTH[6:0] | BG[3:0] | BANK_WIDTH[4:0] |
| --- | --- | --- | --- | --- |
| 位数 | 2 bits | 7 bits | 4 bits | 5 bits |
| 含义 | bank_id | row_id（bank 内行号） | bank_group_id（bank group 编号） | offset（行内偏移） |

**解码示例**：地址 `0x10020`

将地址转为 18 位二进制，按位域划分：`01 0000000 0001 00000`

| 位域 | 二进制值 | 十进制 | 含义 |
| --- | --- | --- | --- |
| BANK[1:0] | `01` | 1 | bank_id = 1 |
| BANK_DEPTH[6:0] | `0000000` | 0 | row_id = 0 |
| BG[3:0] | `0001` | 1 | bank_group_id = 1 |
| BANK_WIDTH[4:0] | `00000` | 0 | offset = 0 |

结合 [UB 内存结构图](#ubunified-buffer-内存结构)和 bank 编号规则，该地址位于 bank group 1 内 bank_id 为 1 的 bank，即全局 bank 17（`1 + 1 × 16 = 17`），行号为 0。

## bank冲突类型

根据内存结构，bank冲突主要分为以下三种类型：
- 读读冲突：多个读操作，同时尝试访问同一个bank group；
- 写写冲突：多个写操作，同时尝试访问同一个bank group；
- 读写冲突：读操作和写操作，同时尝试访问同一个bank。

## 测试场景概览

本文围绕 bank 冲突和地址重叠两个角度展开验证分析。所有场景的输入数据类型为 `float`，数据长度为 4096，采用连续读写方式（`BlkStride = 1`，`RepStride = 8`，`mask = 64`），特殊配置在对应 case 中单独说明。

> **性能说明**：下表"打点性能"为 Add 指令连续执行 1000 次的cycle数，数值越低性能越好。

| case | 计算公式 | 场景描述 | bank 冲突情况 | src0 首地址 | src1 首地址 | dst 首地址 | 打点性能 | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | dst = src0 + src1 | src0/src1/dst 首地址分布在不同 bank group 的不同 bank | 无读读、读写冲突 | 0 | 0x10100 | 0x20000 | 2418 | 基线性能参照（无bank冲突） |
| 2 | dst = src0 + src1 | src0/src1 首地址在同一个 bank | **有读读冲突** | 0 | 0x4000 | 0x10000 | 4195 | 验证读读冲突（多个读操作访问同一个bank） |
| 3 | dst = src0 + src1 | src0/src1 首地址在同一个 bank group | **有读读冲突** | 0 | 0x10000 | 0x14100 | 4195 | 验证读读冲突（多个读操作访问同一个bank group） |
| 4 | dst = src0 + src1 | src0/dst 首地址在同一个 bank | 无读写冲突（硬件优化） | 0 | 0x10100 | 0x4000 | 2501 | 作为 case 5 的基线参照 |
| 5 | dst = src0 + src1 | src0/dst 首地址不在同一个 bank | **有读写冲突** | 0 | 0x10100 | 0x4020 | 3612 | 验证读写冲突（对比case5） |
| 6 | dst = src0 + src1 | src1/dst 首地址在同一个 bank，<br>src1/dst 地址完全重叠 | 无读写冲突（硬件优化） | 0 | 0x10100 | 0x10100 | 2501 | 验证src/dst地址重叠（无读写冲突，硬件优化） |
| 7 | dst = src0 + src1 | src0 每次repeat重复读取同一 datablock（src0BlkStride=0, src0RepStride=1），<br>src1/dst 地址完全重叠 | **有读读冲突** | 0 | 0x10100 | 0x10100 | 3778 | 对比 case 6/case 8，验证 src0RepStride 配置跟bank冲突的关联 |
| 8 | dst = src0 + src1 | src0 每次repeat重复读取同一 datablock（src0BlkStride=0, src0RepStride=8），<br>src1/dst 地址完全重叠 | 无读读、读写冲突 | 0 | 0x10100 | 0x10100 | 2501 | 对比 case 7，验证 src0RepStride 配置跟bank冲突的关联 |

各 case 的详细分析见下文对应章节。

## Case 1：无读读冲突、无读写冲突

### 地址解码

| 操作数 | 首地址 | 18 位二进制（BANK DEPTH BG WIDTH） | bank_id | bank_group_id | 全局 bank 编号 |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x10100` | `01 0000000 1000 00000` | 1 | 8 | 24 |
| dst | `0x20000` | `10 0000000 0000 00000` | 2 | 0 | 32 |

### UB 读写示意

![case1](./figures/case1.jpg)

- 每 8 个绿色数据块（32B）代表 src0 每次 repeat 读取的数据（256B）；
- 每 8 个黄色数据块（32B）代表 src1 每次 repeat 读取的数据（256B）；
- 每 8 个蓝色数据块（32B）代表 dst 每次 repeat 写入的数据（256B）；
- 处理完所有数据所需迭代次数：`repeat_times = 数据长度 × sizeof(float) ÷ 每次repeat处理量 = 4096 × 4 ÷ 256 = 64`。

### 分析结论

- 第1次repeat：src0（bank_id=0, bg 0-7）与 src1（bank_id=1, bg 8-15）：repeat 的所有 datablock 位于不同 bank group，后续 repeat 依次类推，**无读读冲突**。
- 第1次repeat：src0/src1 读取与 dst 写入（bank_id=2, bg 0-7）：repeat 的所有 datablock 位于不同 bank，后续 repeat 依次类推，**无读写冲突**。

## Case 2：src0/src1 首地址在同一个 bank——读读冲突

### 地址解码

| 操作数 | 首地址 | 18 位二进制（BANK DEPTH BG WIDTH） | bank_id | bank_group_id | 全局 bank 编号 |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x4000` | `00 0100000 0000 00000` | 0 | 0 | 0 |
| dst | `0x10000` | `01 0000000 0000 00000` | 1 | 0 | 16 |

### UB 读写示意

![case2](./figures/case2.jpg)

- 每 8 个绿色数据块（32B）代表 src0 每次 repeat 读取的数据（256B）；
- 每 8 个黄色数据块（32B）代表 src1 每次 repeat 读取的数据（256B）；
- 每 8 个蓝色数据块（32B）代表 dst 每次 repeat 写入的数据（256B）；
- 处理完所有数据所需迭代次数：`repeat_times = 4096 × 4 ÷ 256 = 64`。

### 分析结论

- 第1次repeat：src0（bank_id=0, bg 0-7）与 src1（bank_id=0, bg 0-7）：repeat 的所有 datablock 位于相同 bank group 且 bank_id 相同，后续 repeat 依次类推，**存在读读冲突**。
- 第1次repeat：src0/src1 读取（bank_id=0, bg 0-7）与 dst 写入（bank_id=1, bg 0-7）：bank_id 不同，不在相同 bank，后续 repeat 依次类推，**无读写冲突**。

## Case 3：src0/src1 首地址在同一个 bank group——读读冲突

### 地址解码

| 操作数 | 首地址 | 18 位二进制（BANK DEPTH BG WIDTH） | bank_id | bank_group_id | 全局 bank 编号 |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x10000` | `01 0000000 0000 00000` | 1 | 0 | 16 |
| dst | `0x14100` | `01 0100000 1000 00000` | 1 | 8 | 24 |

### UB 读写示意

![case3](./figures/case3.jpg)

- 每 8 个绿色数据块（32B）代表 src0 每次 repeat 读取的数据（256B）；
- 每 8 个黄色数据块（32B）代表 src1 每次 repeat 读取的数据（256B）；
- 每 8 个蓝色数据块（32B）代表 dst 每次 repeat 写入的数据（256B）；
- 处理完所有数据所需迭代次数：`repeat_times = 4096 × 4 ÷ 256 = 64`。

### 分析结论

- 第1次repeat：src0（bank_id=0, bg 0-7）与 src1（bank_id=1, bg 0-7）：repeat 的所有 datablock 位于相同 bank group，后续 repeat 依次类推，**存在读读冲突**。
- 第1次repeat：src0/src1 读取（bank_id=0/1, bg 0-7）与 dst 写入（bank_id=1, bg 8-15）：repeat 的所有 datablock 位于不同 bank ，后续 repeat 依次类推，**无读写冲突**。

> **对比 case 2**：case 2 中 src0/src1 在同一 bank（bank_id 和 bank_group_id 均相同），case 3 中在同一 bank group（仅 bank_group_id 相同）。两者均触发读读冲突，性能开销相同（4195 vs 4195），说明读读冲突的判定依据是 bank group 而非 bank。

## Case 4：src0/dst 首地址在同一个 bank——无读写冲突（硬件优化）

### 地址解码

| 操作数 | 首地址 | 18 位二进制（BANK DEPTH BG WIDTH） | bank_id | bank_group_id | 全局 bank 编号 |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x10100` | `01 0000000 1000 00000` | 1 | 8 | 24 |
| dst | `0x4000` | `00 0100000 0000 00000` | 0 | 0 | 0 |

### UB 读写示意

![case4](./figures/case4.jpg)

- 每 8 个绿色数据块（32B）代表 src0 每次 repeat 读取的数据（256B）；
- 每 8 个黄色数据块（32B）代表 src1 每次 repeat 读取的数据（256B）；
- 每 8 个蓝色数据块（32B）代表 dst 每次 repeat 写入的数据（256B）；
- 处理完所有数据所需迭代次数：`repeat_times = 4096 × 4 ÷ 256 = 64`。

### 分析结论

- 第1次repeat：src0（bank_id=0, bg 0-7）与 src1（bank_id=1, bg 8-15）：repeat 的所有 datablock 位于不同 bank group，后续 repeat 依次类推，**无读读冲突**。
- 第1次repeat：src0 读取（bank_id=0, bg 0-7）与 dst 写入（bank_id=0, bg 0-7）：repeat 的所有 datablock 位于相同 bank，理论上存在读写冲突；但硬件自动将 dst 写入延后 1 个 repeat，使读写操作错开，后续 repeat 依次类推，**实际无读写冲突**。

**理论执行时序**（未优化）：

| | 第 1 次 repeat | 第 2 次 repeat | 第 3 次 repeat | ... |
| --- | --- | --- | --- | --- |
| src0 读 | bank 0-7 (bg 0-7) | bank 8-15 (bg 8-15) | bank 0-7 (bg 0-7) | ... |
| src1 读 | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | bank 24-31 (bg 8-15) | ... |
| dst 写 | bank 0-7 (bg 0-7) | bank 8-15 (bg 8-15) | bank 0-7 (bg 0-7) | ... |

理论结果：src0 与 dst 每次 repeat 访问相同 bank，存在读写冲突。

**实际执行时序**（硬件优化）：

| | 第 1 次 repeat | 第 2 次 repeat | 第 3 次 repeat | ... |
| --- | --- | --- | --- | --- |
| src0 读 | bank 0-7 (bg 0-7) | bank 8-15 (bg 8-15) | bank 0-7 (bg 0-7) | ... |
| src1 读 | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | bank 24-31 (bg 8-15) | ... |
| dst 写 | — | bank 0-7 (bg 0-7) | bank 8-15 (bg 8-15) | ... |

实际结果：硬件将 dst 写入延后 1 个 repeat，与 src0 读取错开，避免读写冲突。

## Case 5：src0/dst 首地址不在同一个 bank——读写冲突

### 地址解码

| 操作数 | 首地址 | 18 位二进制（BANK DEPTH BG WIDTH） | bank_id | bank_group_id | 全局 bank 编号 |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x10100` | `01 0000000 1000 00000` | 1 | 8 | 24 |
| dst | `0x4020` | `00 0100000 0001 00000` | 0 | 1 | 1 |

### UB 读写示意

![case5](./figures/case5.jpg)

- 每 8 个绿色数据块（32B）代表 src0 每次 repeat 读取的数据（256B）；
- 每 8 个黄色数据块（32B）代表 src1 每次 repeat 读取的数据（256B）；
- 每 8 个蓝色数据块（32B）代表 dst 每次 repeat 写入的数据（256B）；
- 处理完所有数据所需迭代次数：`repeat_times = 4096 × 4 ÷ 256 = 64`。

### 分析结论

- 第1次repeat：src0（bank_id=0, bg 0-7）与 src1（bank_id=1, bg 8-15）：repeat 的所有 datablock 位于不同 bank group，后续 repeat 依次类推，**无读读冲突**。
- 第1次repeat：src0 读取（bank_id=0, bg 0-7）与 dst 写入（bank_id=0, bg 1-8）：repeat 会访问到相同的 bank 1-7，后续 repeat 依次类推，**存在读写冲突**。

**UB 读写过程示意**：

| | 第 1 次 repeat | 第 2 次 repeat | 第 3 次 repeat | ... |
| --- | --- | --- | --- | --- |
| src0 读 | bank 0-7 (bg 0-7) | bank 8-15 (bg 8-15) | bank 0-7 (bg 0-7) | ... |
| src1 读 | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | bank 24-31 (bg 8-15) | ... |
| dst 写 | bank 1-8 (bg 1-8) | bank 9-15-0 (bg 9-15-0) | bank 1-8 (bg 1-8) | ... |

分析结果：src0 与 dst 每次 repeat 都会访问到相同的 bank，导致读写冲突。

## Case 6：src1/dst 首地址在同一个 bank，src1/dst 地址重叠——无读写冲突（硬件优化）

### 地址解码

| 操作数 | 首地址 | 18 位二进制（BANK DEPTH BG WIDTH） | bank_id | bank_group_id | 全局 bank 编号 |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x10100` | `01 0000000 1000 00000` | 1 | 8 | 24 |
| dst | `0x10100` | `01 0000000 1000 00000` | 1 | 8 | 24 |

### UB 读写示意

![case6](./figures/case6.jpg)

- 每 8 个绿色数据块（32B）代表 src0 每次 repeat 读取的数据（256B）；
- 每 8 个黄色数据块代表每次repeat，src1 读取与 dst 写入同一块数据空间（256B），即 src1 和 dst 地址完全重叠；
- 处理完所有数据所需迭代次数：`repeat_times = 4096 × 4 ÷ 256 = 64`。

### 分析结论

- 第1次repeat：src0（bank_id=0, bg 0-7）与 src1（bank_id=1, bg 8-15）：repeat 的所有 datablock 位于不同 bank group，后续 repeat 依次类推，**无读读冲突**。
- 第1次repeat：src1 读取（bank_id=1, bg 8-15）与 dst 写入（bank_id=1, bg 8-15）：两者地址完全重叠，理论上存在读写冲突风险；但硬件自动将 dst 写入延后 1 个 repeat，使读写操作错开，后续 repeat 依次类推，**实际无读写冲突**。

**理论执行时序**（未优化）：

| | 第 1 次 repeat | 第 2 次 repeat | 第 3 次 repeat | ... |
| --- | --- | --- | --- | --- |
| src0 读 | bank 0-7 (bg 0-7) | bank 8-15 (bg 8-15) | bank 0-7 (bg 0-7) | ... |
| src1 读 | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | bank 24-31 (bg 8-15) | ... |
| dst 写 | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | bank 24-31 (bg 8-15) | ... |

理论结果：src1 与 dst 每次 repeat 都会访问到相同的 bank，会出现读写冲突。

**实际执行时序**（硬件优化）：

| | 第 1 次 repeat | 第 2 次 repeat | 第 3 次 repeat | ... |
| --- | --- | --- | --- | --- |
| src0 读 | bank 0-7 (bg 0-7) | bank 0-7 (bg 0-7) | bank 0-7 (bg 0-7) | ... |
| src1 读 | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | bank 24-31 (bg 8-15) | ... |
| dst 写 | — | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | ... |

实际结果：硬件将 dst 写入延后 1 个 repeat，与 src1 读取错开，避免读写冲突。

## Case 7：src0 重复读取同一 datablock（src0BlkStride=0, src0RepStride=1），src1/dst 地址重叠——读读冲突

> **特殊配置**：src0 的 `src0BlkStride=0`（单次 repeat 内重复读取同一 datablock），`src0RepStride=1`（每次 repeat 间隔 1 个 datablock，即 32B）。

### 地址解码

| 操作数 | 首地址 | 18 位二进制（BANK DEPTH BG WIDTH） | bank_id | bank_group_id | 全局 bank 编号 |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x10100` | `01 0000000 1000 00000` | 1 | 8 | 24 |
| dst | `0x10100` | `01 0000000 1000 00000` | 1 | 8 | 24 |

### UB 读写示意

![case7](./figures/case7.jpg)

- 每次重复读取同 1 个绿色数据块（32B）共 8 次（src0BlkStride=0）；
- 每 8 个黄色数据块代表每次repeat，src1 读取与 dst 写入同一块数据空间（256B），即 src1 和 dst 地址完全重叠；
- 处理完所有数据所需迭代次数：`repeat_times = 4096 × 4 ÷ 256 = 64`。

### 分析结论

- 第1次repeat：src0（bank_id=0, bg 0）与 src1（bank_id=1, bg 8-15）：src0 仅读取单个 datablock （bg 0），与 src1 不在相同 bank group，无读读冲突。但后续 repeat 中 src0 的 bank group 依次递增（bg 1, 2, ...），每逢偶数次 repeat 与 src1 的 bank group 范围重叠，整体**存在读读冲突**。
- 第1次repeat：src1 读取（bank_id=1, bg 8-15）与 dst 写入（bank_id=1, bg 8-15）：两者地址完全重叠，硬件自动将 dst 写入延后 1 个 repeat，使读写操作错开，后续 repeat 依次类推，**无读写冲突**。

**UB 读写过程示意**：

| | 第 1 次 repeat | 第 2 次 repeat | 第 3 次 repeat | 第 4 次 repeat | ... |
| --- | --- | --- | --- | --- | --- |
| src0 读 | bank 0 (bg 0) | bank 1 (bg 1) | bank 2 (bg 2) | bank 3 (bg 3) | ... |
| src1 读 | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | ... |
| dst 写 | - | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | bank 24-31 (bg 8-15) | ... |

分析结果：
- 第 2 次 repeat：src0 读 bg 1，src1 读 bg 0-7 → bg 1 重叠，读读冲突；
- 第 4 次 repeat：src0 读 bg 3，src1 读 bg 0-7 → bg 3 重叠，读读冲突；
- ...
- dst 写入延后 1 个 repeat，不与 src1 产生读写冲突。

## Case 8：src0 重复读取同一 datablock（src0BlkStride=0, src0RepStride=8），src1/dst 地址重叠——无冲突

> **特殊配置**：src0 的 `src0BlkStride=0`（单次 repeat 内重复读取同一 datablock），`src0RepStride=8`（每次 repeat 间隔 8 个 datablock，即 256B）。

### 地址解码

| 操作数 | 首地址 | 18 位二进制（BANK DEPTH BG WIDTH） | bank_id | bank_group_id | 全局 bank 编号 |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x10100` | `01 0000000 1000 00000` | 1 | 8 | 24 |
| dst | `0x10100` | `01 0000000 1000 00000` | 1 | 8 | 24 |

### UB 读写示意

![case8](./figures/case8.jpg)

- 每次重复读取同 1 个绿色数据块（32B）共 8 次（src0BlkStride=0）；
- 每 8 个黄色数据块代表每次repeat，src1 读取与 dst 写入同一块数据空间（256B），即 src1 和 dst 地址完全重叠；
- 处理完所有数据所需迭代次数：`repeat_times = 4096 × 4 ÷ 256 = 64`。

### 分析结论

- 第1次repeat：src0（bank_id=0, bg 0）与 src1（bank_id=1, bg 8-15）：src0 仅读取单个 datablock，与 src1 不在相同 bank group。后续 repeat 中 src0 的 bank group 按 bg 0, 8, 0, 8, ... 交替，始终与 src1 的 bank group 范围错开，整体**无读读冲突**。
- 第1次repeat：src1 读取（bank_id=1, bg 8-15）与 dst 写入（bank_id=1, bg 8-15）：两者地址完全重叠，硬件自动将 dst 写入延后 1 个 repeat，使读写操作错开，后续 repeat 依次类推，**无读写冲突**。

**UB 读写过程示意**：

| | 第 1 次 repeat | 第 2 次 repeat | 第 3 次 repeat | 第 4 次 repeat | ... |
| --- | --- | --- | --- | --- | --- |
| src0 读 | bank 0 (bg 0) | bank 8 (bg 8) | bank 0 (bg 0) | bank 8 (bg 8) | ... |
| src1 读 | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | ... |
| dst 写 | - | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | bank 24-31 (bg 8-15) | ... |

分析结果：
- src0（bg 0 或 8）与 src1（bg 0-7 或 8-15）始终不重叠：当 src0 在 bg 0 时，src1 在 bg 8-15；当 src0 在 bg 8 时，src1 在 bg 0-7。因此无读读冲突。
- dst 写入延后 1 个 repeat，不与 src1 产生读写冲突。

> **对比 case 7**：case 7 的 `src0RepStride=1` 导致 src0 按步长 1 递增遍历 bank group（bg 0, 1, 2, 3, ...），与 src1 的 bank group 范围频繁重叠；case 8 的 `src0RepStride=8` 使 src0 在 bg 0 和 8 之间交替，恰好与 src1 错开。可以有效消除读读冲突。

## 小结

- 读读冲突：只要两个读操作访问同一 bank group（不论 bank_id 是否相同），即触发读读冲突。
- 读写冲突：读写操作访问同一 bank，会触发读写冲突（同一个bank一拍只能1读或1写）。例外情况：读操作与写操作repeat首个datablock在同一个bank，后面的7个datablock也分别在同一个bank，硬件可将写入延后 1 个 repeat 以规避读写冲突的情况。
- blkStride=0的场景：repeat内不会重复读取，只读1次。repStride=0的场景：repeat间不会重复读取，只读1次。
