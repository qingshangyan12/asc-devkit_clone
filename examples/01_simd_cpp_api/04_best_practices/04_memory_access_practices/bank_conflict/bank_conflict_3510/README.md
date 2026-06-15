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
| 1 | dst = src0 + src1 | **完全不冲突**：src0=src1，硬件合并为单次读取，dst不同bank | **无冲突** | 0x0000 | 0x0000 | 0x4100 | 1,120,614 | **性能基准** |
| 2 | dst = src0 + src1 | **src0与dst重叠**: src1不同bank，读写冲突 | **读写冲突** | 0x0000 | 0x4100 | 0x0000 | 1,472,830 | src0/dst重叠，在Ascend 950PR/Ascend 950DT中读写地址重叠，读写冲突仍然发生 |
| 3 | dst = src0 + src1 | **全冲突(读读+读写)**: 三路同 bank=0，无循环展开 | **全冲突** | 0x0000 | 0x4000 | 0x8000 | 1,420,634 | 全冲突参照，地址连续减少了部分开销 |
| 4 | dst = src0 + src1 | 消除读读冲突：src0/src1 不同 bank | 仅读写冲突 | 0x0000 | 0x4100 | 0x8900 | 1,440,511 | 读读冲突开销小 |
| 5 | dst = src0 + src1 | 消除读写冲突：src0/src1 同 bank，dst 不同 bank | 仅读读冲突 | 0x0000 | 0x4000 | 0x8100 | 1,330,843 | 读写冲突开销大，优化后收益明显 |
| 6 | dst = src0 + src1 | 全冲突 + 循环展开：同 Case 3，循环展开优化 | **全冲突** | 0x0000 | 0x4000 | 0x8000 | 1,330,608 | 循环展开有正向收益 |
| 7 | dst = src0 + src1 | 全冲突 + 双搬入 DINTLV_B32 | 读写冲突 | 0x0000 | 0x4000 | 0x8000 | 1,480,654 | 双搬入接口无收益 |

### 离散访存场景（Case 8-11）

| case | 计算公式 | 场景描述 | subbank 冲突情况 | index 地址 | data 地址 | index 模式 | 打点性能(cycles) | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 8 | Gather + Add + Scatter | Gather数据索引全为0，起始地址全部重叠 | 同地址合并为单次读取，无冲突 | 0x0000 | 0x0400 | 全 0 | 650,677 | 基准参照 |
| 9 | Gather + Add + Scatter | Gather数据索引[0..7]重复 | 同地址合并读取，无冲突 | 0x0000 | 0x0400 | [0..7]×32 重复 | 650,835 | 同地址合并无冲突 |
| 10 | Gather + Add + Scatter | Gather数据索引[0..255]连续 | 跨 bank 分散，无冲突 | 0x0000 | 0x0400 | [0..255] 连续 | 650,697 | 跨bank分散无冲突 |
| 11 | Gather + Add + Scatter | Gather数据索引0、128交替重复 | **同 subbank 不同行冲突** | 0x0000 | 0x0400 | 0、128 交替 | **750,709** | 对比 Case 8，性能劣化 |

### 非连续搬运跨步对比（Case 12-15）

| case | 计算公式 | 场景描述 | 冲突情况 | src0 首地址 | dst 首地址 | 搬运方式 | 打点性能(cycles) | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 12 | dst = src0 | 512元素按行计算，连续8个DataBlock，stride=1 | 无冲突 | 0x0000 | 0x4100 | DataBlock连续搬运 | 442,224 | 基准 |
| 13 | dst = src0 | **512元素按列计算，间隔256B 8个不连续DataBlock，stride=8** | **读读冲突** | 0x0000 | 0x4100 | DataBlock非连续搬运 | 1,510,649 | 竖着搬运慢 3.42x |
| 14 | dst = src0 | **srcBlockStride=0**：同一 REPEAT 内 8 个 DataBlock 均指向同一基地址 | 同地址合并 | 0x0000 | 0x4100 | DataBlock stride=0 | 443,499 | 同地址合并，无额外开销 |
| 15 | dst = src0 | **srcRepeatStride=0**：8 轮迭代固定从 src0[0..63] 加载 | 无冲突 | 0x0000 | 0x4100 | DataBlock stride=1,repeat=0 | 440,810 | 同首地址重复读，无冲突 |



各 case 的详细分析见下文对应章节。

## Case 1：二读一写，src0与src1重叠，dst不同bank，无冲突

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| dst | `0x4100` | `010000010 0 000 00000` | 1 | 0 |

### UB 读写示意

![case1](./figures/case1.png)


### 分析结论

- src0 和 src1 地址完全重叠，共用同一块 UB 空间。硬件地址匹配合并后只触发一次读，有效变为一读一写。
- 由于dst在bank1，与src0/src1不在同一 bank，**不发生冲突**。

## Case 2：二读一写，src0与dst地址重叠

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x4100` | `010000010 0 000 00000` | 1 | 0 |
| dst | `0x0000` | `000000000 0 000 00000` | 0 | 0 |

### UB 读写示意

![case2](./figures/case2.png)

### 分析结论

- src0 与 dst 地址重叠（同 bank 0 同 depth 行），读取 src0 和写入 dst 同时进行，**存在读写冲突**，而在 Atlas A2/A3 系列产品上，读写地址重叠能够被硬件优化从而消除冲突。
- src1 在 bank1（同 BG 不同 bank），与 src0 无读读冲突。

## Case 3：二读一写，全冲突基准 — src0、src1和dst同一bank，无循环展开

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x4000` | `010000000 0 000 00000` | 0 | 0 |
| dst | `0x8000` | `100000000 0 000 00000` | 0 | 0 |

### UB 读写示意

![case3](./figures/case3.png)

### 分析结论

- src0、src1、dst 三路首地址均映射到 bank 0：虽然 8 个 DataBlock 分散在 BG 0-7，但三路 tensor 的对应 DataBlock 都落在相同 bank，**存在读读冲突和读写冲突**。

## Case 4：二读一写，消除读读冲突 — src0与src1不同bank，dst与src1同bank

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x4100` | `010000010 0 000 00000` | 1 | 0 |
| dst | `0x8900` | `100010010 0 000 00000` | 1 | 0 |

### UB 读写示意

![case4](./figures/case4.png)

### 分析结论

- src0=bank0，src1=bank1，dst=bank1。src0 和 src1 落在不同 bank，读读冲突消除。但 src1 和 dst 同在 bank1，仍存在读写冲突。
- 消除读读冲突后性能未提升，说明 **3510 上读读冲突开销极小，不是瓶颈**。

## Case 5：二读一写，消除读写冲突 — src0与src1同bank，dst在不同bank

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x4000` | `010000000 0 000 00000` | 0 | 0 |
| dst | `0x8100` | `100000010 0 000 00000` | 1 | 0 |

### UB 读写示意

![case5](./figures/case5.png)

### 分析结论

- src0=bank0，src1=bank0，dst=bank1。src0 和 src1 同落在 bank0，存在读读冲突。但 dst 在 bank1，读写冲突消除。
- 与 Case 4 相比：Case 5 快 8.1%，说明读写冲突开销大，消除后性能有提升。

## Case 6：二读一写，全冲突 + 循环展开

### 地址解码

地址配置同 Case 3，详见 Case 3 地址解码。

### UB 读写示意

读写冲突示意同 Case 3，详见 Case 3 UB 读写示意。

### 分析结论

- 地址布局与 Case 3 完全相同，冲突特征相同。
- 唯一差异是使用循环展开，每次处理两个 repeat，增加指令级并行。
- 对比case3，循环展开在该场景下有正收益（属于指令级优化，非 bank 冲突优化）。

## Case 7：二读一写，双搬入 DINTLV_B32 交织读取

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x4000` | `010000000 0 000 00000` | 0 | 0 |
| dst | `0x8000` | `100000000 0 000 00000` | 0 | 0 |

### UB 读写示意

![case7](./figures/case7.png)

### 分析结论

- 使用 `LoadDist::DIST_DINTLV_B32` 双搬入模式，一条指令同时取回 src0 和 src1（各取 64 bytes 交错排列），消除读读冲突。
- 对比case3无明显收益。

## Case 8：离散读写，Gather指令的数据索引全为0，起始地址全部重叠

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| index | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| data | `0x0400` | `000001000 0 000 00000` | 0 | 0 |
| dst | `0x0400` | `000001000 0 000 00000` | 0 | 0 |

### UB 读写示意

![case8](./figures/case8.png)

- index 模式：全部为 0，即每次 Gather 都从 data[0] 读取同一个位置。

### 分析结论

- index 全 0，Gather 所有元素命中同一地址 → 硬件通过地址合并将多次请求合并为单次读取，因此无subbank冲突。

## Case 9：离散读写，Gather指令的数据索引在[0..7]重复

### 地址解码

同 Case 8 地址配置。

### UB 读写示意

![case9](./figures/case9.png)

- 索引排布：`[0, 1, 2, 3, 4, 5, 6, 7]` 重复 32 次（共 256 个元素），由 Gather 读取；
- data buffer 起始地址 `0x0400`，每条 Gather 指令从 data 中按 index 取 64 个 float32；
- dst 与 data 地址重叠（`0x0400`），Scatter 写回同一区域。

### 分析结论

- Gather 从 data 中按 index 取 64 个元素：index 仅有 [0..7] 共 8 个 unique 值，大量元素命中相同地址 → 硬件自动合并同地址请求，同地址只需一次读取，降低了subbank冲突。

## Case 10：离散读写，Gather指令的数据索引在[0..255]连续

### 地址解码

同 Case 8 地址配置。

### UB 读写示意

![case10](./figures/case10.png)

- index 模式：`[0, 1, 2, ..., 255]` 连续递增，每条 Gather 指令处理 64 个连续 index；
- data buffer 起始地址 `0x0400`，dst 与 data 地址重叠。

### 分析结论

- Gather 跨 8 个 bank 分散读取，每 subbank 仅 2 次读操作。连续 index 自然分散到不同 subbank，降低了subbank冲突。

## Case 11：离散读写，Gather指令的数据索引0,128交替

### 地址解码

同 Case 8 地址配置。

### UB 读写示意

![case11](./figures/case11.png)

- index 模式：偶数位置为 `0`，奇数位置为 `128`（`[0, 128, 0, 128, ...]` 交替）；
- `0` 和 `128` 在 Ascend 950PR/Ascend 950DT 上都落入 bank 0 的同一 subbank，但属于**不同 depth 行**。

### 分析结论

- Gather 命中**同 subbank + 不同起始地址**（相同 subbank，不同 depth 行），单读口需排队，subbank冲突明显。

## Case 12：对512个（8*64）元素进行加一操作，按行计算，每次repeat读取8个连续的DataBlock

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| dst | `0x4100` | `010000010 0 000 00000` | 1 | 0 |

### UB 读写示意

![case12](./figures/case12.png)

### 搬运方式

迭代8轮 ，每轮读写8个连续DataBlock。

### 分析结论

- 每次迭代的8个DataBlock 连续排布（间隔 32B），等价于普通 `LoadAlign`指令，此时单条非连续搬运指令8个DataBlock分散到不同的bankgroup，不发生冲突。

## Case 13：对512个（8*64）元素进行加一操作，按列计算，每次repeat读取8个不连续的DataBlock

### 地址解码

同 Case 12 地址配置。

### UB 读写示意

![case13](./figures/case13.png)

### 搬运方式

迭代8轮 ，每轮读写8个DataBlock，每个DataBlock间隔256B。

### 分析结论

- 每次迭代的8个DataBlock均从同一bank group的不同depth行读取，命中**同 bank group 不同行**，发生bank冲突，对比case12性能劣化。

## Case 14：一读一写，同DataBlock重复读取

### 地址解码

| 操作数 | 首地址 | 18 位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| dst | `0x4100` | `010000010 0 000 00000` | 1 | 0 |

### UB 读写示意

![case14](./figures/case14.png)

### 搬运方式

迭代8轮，每轮读取的8个DataBlock均指向同一32B基地址，写回时8个DataBlock连续排布。

### 分析结论

- 同一迭代内8个DataBlock 全部命中同一地址，硬件自动合并为单次读取，寄存器被填充为 8 份相同数据，无额外开销。

## Case 15：一读一写，每次迭代从同一首地址读取

### 地址解码

同 Case 12 地址配置。

### UB 读写示意

![case15](./figures/case15.png)

### 搬运方式

迭代8轮，每轮从固定首地址 src0（不偏移）读取8个连续的DataBlock，写回时8个DataBlock连续排布。

### 分析结论

- 8 轮迭代均从同一个首地址加载，每次迭代从同一首地址读取几乎无额外开销。

## 小结

- **二读一写冲突必然发生**：Ascend 950PR/Ascend 950DT 仅有 8 个 BG，一条 VF 指令覆盖全部 BG。2R1W 模式下只要 src0 ≠ src1，每个 BG 必然发生二读一写，超出单个 BG 的读写能力（最多二读或一读一写），冲突不可避免。**地址重叠（Case 1）是唯一能让二读一写完全无冲突的方式**（硬件将两读合并为一读），在Atlas A2/A3 系列产品中， 可以通过读写地址重叠触发硬件优化，在Ascend 950PR/Ascend 950DT中没有触发。
- **读写冲突开销大于读读冲突**：消除读写冲突后的收益大于消除读读冲突后的收益。
- **双搬入无收益**：Case 7 vs Case 3 = 1,480,654 vs 1,420,634 cycles（慢 4.2%）。
- **循环展开带来额外收益**（非 bank 冲突原因）：地址重叠（Case 1）省 Load，比全冲突快 21.1%；循环展开（Case 6 vs Case 3）有 6.3% 收益。
- **同DataBlock数据重复读取和同Repeat重复读取性能无劣化**：Case 14（443,499 cycles）和 Case 15（440,810 cycles）与基准 Case 12（442,224 cycles）性能相当。

## 目录结构

```text
├── bank_conflict.asc              // 算子 host + device 代码
├── config.h                      // 13 个 Case 的地址/类型配置
├── vf_func.h                     // Vector 函数实现
├── data_utils.h                  // 数据读写工具
├── CMakeLists.txt                // 编译工程
├── test.sh                       // 测试脚本
├── figures/                      // 各 Case UB 内存分布图
│   ├── case1.png
│   ├── case2.png
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
│   ├── case13.png
│   ├── case14.png
│   └── case15.png
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
export SCENARIO_NUM=1          # 1-15 可选
mkdir -p build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-Ascend 950PR/Ascend 950DT -DCASE_NUM=${SCENARIO_NUM}
make -j
python3 ../scripts/gen_data.py --case ${SCENARIO_NUM}
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
| `SCENARIO_NUM` | 1-13 | 切换测试场景 |
| `CMAKE_ASC_RUN_MODE` | `npu` | 上板运行模式 |
