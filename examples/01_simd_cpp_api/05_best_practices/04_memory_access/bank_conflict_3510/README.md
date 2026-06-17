# UB Bank冲突场景验证与分析(架构3510)

## UB内存结构介绍

![UB Architecture](./figures/ub_arch.png)

- 在Ascend 950PR/Ascend 950DT中，UB空间大小为256KB；
- 共有16个 **bank**，组成8个 **bank group**，每个bank group包含2个bank（`bank_id` = 0, 1）；
- 每个bank由512行组成（`BANK_DEPTH = 512`），每行长度为32B（`BANK_WIDTH = 32B`），即每个bank大小为16KB；
- 每个bank group最多支持两读或者一读一写。
- bank_number相同代表是同一个bank；bank_group_id相同代表是同一个bank group。

特别的，每个bank（32B宽）分为4个subbank，各8B宽 × 512行深度，**subbank内存结构只有在使用离散与聚合API时需要关注**。
```
bank内部layout（一行32B）：
┌────────┬────────┬────────┬────────┐
│ sub0   │ sub1   │ sub2   │ sub3   │
│  8B    │  8B    │  8B    │  8B    │
└────────┴────────┴────────┴────────┘
```

## UB地址编码规则

Ascend 950PR/Ascend 950DT的UB 18位地址采用**低位交织**编码规则：

`UB_ADDR[17:0] = { DEPTH[8:0], BANK[0], BG[2:0], WIDTH[4:0] }`

| 位域 | DEPTH[8:0] | BANK[0] | BG[2:0] | WIDTH[4:0] |
| --- | --- | --- | --- | --- |
| 位数 | 9 bits | 1 bit | 3 bits | 5 bits |
| 含义 | row_id（bank内行号） | bank_id（bank group内bank编号） | bank_group_id | offset（行内偏移） |

**解码示例**：地址 `0x4100`

将地址转为18位二进制，按位域划分：`00 0100 0001 0000 0000`

| 位域 | 二进制值 | 十进制 | 含义 |
| --- | --- | --- | --- |
| DEPTH[8:0] | `00 0100 000` | 32 | row_id = 32 |
| BANK[0] | `1` | 1 | bank_id = 1 |
| BG[2:0] | `000` | 0 | bank_group_id = 0 |
| WIDTH[4:0] | `00000` | 0 | offset = 0 |

## DataBlock映射

一条读/写指令每次处理256B = 8个DataBlock。每个DataBlock = 32B。

从 `base` 地址开始的8个DataBlock映射到8个不同bank group：

| DataBlock | 偏移 | 物理bank | BG |
|:--:|:----:|:---------:|:--:|
| 0 | +0x00 | b | b%7 |
| 1 | +0x20 | b+1 | (b+1)%7 |
| ... | ... | ... | ... |
| 7 | +0xE0 | b+7 | (b+7)%7 |

其中 `b = (base >> 5) & 0xF`。8个DataBlock覆盖BG 0-7各一次。

## bank冲突类型

根据内存结构，bank冲突主要分为以下三种类型：
- **读读冲突**：多个读操作同时访问同一个bank或两个以上读操作同时访问同一个bank group才会引发冲突（Atlas A2/A3 系列产品中多个读操作同时访问同一个bank group就会引发冲突）。
- **写写冲突**：多个写操作同时访问同一个bank group。
- **读写冲突**：读操作和写操作同时访问同一个bank，或两个读操作和一个写操作同时访问同一个bank group。

**特别的，对于当使用离散与聚合API时，需要注意sub bank冲突规则**：
- 离散数据属于不同subbank时无冲突。
- 离散数据属于同subbank的同起始地址，无冲突（硬件合并为一次读）。
- 离散数据属于同subbank的不同起始地址，发生冲突。

## 测试场景概览

本文围绕连续对齐搬入/搬出、离散与聚合、非连续对齐搬入/搬出三类API的bank冲突场景展开验证分析。所有场景的输入数据类型为 `float`（离散与聚合场景的index为 `uint32_t`），执行1000次以放大性能差异。

> **性能说明**：下表"打点性能"为VF连续执行1000次的总cycles数（使用`GetSystemCycle` 打点），数值越低性能越好。"LDU冲突率"和"STU冲突率"来自 `msprof op --aic-metrics=Default` 采集的 `ResourceConflictRatio` 指标，分别表示向量单元的load单元冲突比例和store单元冲突比例（单位：%）。

### 连续对齐搬入/搬出的bank冲突场景（Case 1-7）

**在Ascend 950PR/Ascend 950DT中，在src0/src1/dst首地址分布在不同bank group的不同bank的场景不存在**  
**Case1-7均为二读一写场景，二读一写场景无法避免读写冲突**

| case | 计算公式 | 场景描述 | bank冲突情况 | src0首地址 | src1首地址 | dst首地址 | 打点性能(cycles) | LDU冲突率(%) | STU冲突率(%) | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | dst = src0 * src1 | src0/src1首地址在同一个bank，dst首地址在不同bank | 读读冲突+读写冲突 | 0x0000 | 0x5000 | 0xA100 | 1,330,642 | 41.0 | 17.7 | LDU冲突显著 |
| 2 | dst = src0 * src1 | src0/dst首地址在同一个bank，src0/src1首地址在同一个bank group | 读写冲突 | 0x0000 | 0x5100 | 0xA000 | 1,470,563 | 0.0 | 26.5 | LDU无冲突，STU冲突率高 |
| 3 | dst = src0 * src1 | src0/dst首地址在同一个bank，且地址重叠，src1在不同bank | 读写冲突 | 0x0000 | 0x5100 | 0xA000 | 1,470,607 | 0.0 | 27.2 | 对比case2，地址重叠未改善STU冲突 |
| 4 | dst = src0 * src1 | src1/dst首地址在同一个bank，src0/src1首地址在同一个bank group | 读写冲突 | 0x0000 | 0x5100 | 0xA100 | 1,440,690 | 0.0 | 25.9 | 对比case2，冲突特征相同 |
| 5 | dst = src0 * src1 | src1/dst首地址在同一个bank，且地址重叠，src0在不同bank | 读写冲突 | 0x0000 | 0x5100 | 0x5100 | 1,440,518 | 0.0 | 26.0 | 对比case3，地址重叠未改善STU冲突 |
| 6 | dst = src0 * src1 | src0/src1/dst同bank，既有读读冲突，又有读写冲突 | 读读冲突+读写冲突 | 0x0000 | 0x5000 | 0xA000 | 1,420,587 | 4.1 | 24.2 | STU冲突显著 |
| 7 | dst = src0 * src1 | 同case6，**对指令进行循环展开** | 读读冲突+读写冲突 | 0x0000 | 0x5000 | 0xA000 | 1,332,991 | 40.2 | 18.0 | 对比case6，LDU冲突增加，STU冲突减少，性能提升6.2% |

### 离散与聚合场景（Case 8-9）

**注：subbank冲突属于更低层级的存储冲突，不被LDU/STU冲突率指标捕获**

| case | 计算公式 | 场景描述 | subbank冲突情况 | index地址 | data地址 | index模式 | 打点性能(cycles) | LDU冲突率(%) | STU冲突率(%) | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 8 | Gather + Add + Scatter | Gather数据索引[0..255]连续 | 跨bank分散，无冲突 | 0x0000 | 0x0400 | 索引[0..255]连续 | 650,995 | - | - | 数据分散在16个bank的不同sub bank中，冲突率低 |
| 9 | Gather + Add + Scatter | Gather数据索引0、128交替重复 | 同subbank不同行冲突 | 0x0000 | 0x0400 | 索引0、128交替 | **750,819** | - | - | 对比Case 8，性能劣化15.2% |

### 非连续搬运跨步对比（Case 10-13）

| case | 计算公式 | 场景描述 | src0首地址 | dst首地址 | 搬运方式 | 打点性能(cycles) | LDU冲突率(%) | STU冲突率(%) | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 10 | dst = src0 | 512元素按行计算，repeat内8个DataBlock连续排布，repeat间间隔256B | 0x0000 | 0x4100 | DataBlock连续搬运 | 440,952 | 0 | 0 | LDU/STU均无冲突 |
| 11 | dst = src0 | 512元素按列计算，repeat内8个DataBlock间间隔256B，repeat间间隔32B  | 0x0000 | 0x4100 | DataBlock非连续搬运 | 1,510,657 | 0 | **52.8** | 对比case10，STU冲突率极高，性能劣化明显 |
| 12 | dst = src0 | 同一repeat内8个DataBlock均从同一起始地址读取数据 | 0x0000 | 0x4100 | DataBlock重复读取 | 440,791 | 0.0 | 0.0 | 对比case10，无劣化，同地址合并，无额外开销 |
| 13 | dst = src0 | 8轮迭代，每次迭代数据从同一起始地址读取数据  | 0x0000 | 0x4100 | repeat重复读取 | 440,770 | 0.1 | 0.0 | 对比case10，无劣化，同地址合并，无额外开销 |


**各case的详细分析见下文对应章节。**

## Case 1：二读一写，src0/src1同bank，发生读读冲突

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x5000` | `000101000 0 000 00000` | 0 | 0 |
| dst | `0xA100` | `001010000 1 000 00000` | 1 | 0 |

### UB读写示意

![case1](./figures/case1.png)

### 分析结论

- src0和src1起始地址同落在bank0，存在读读冲突。但dst起始地址在bank1，无读写冲突。
- 与Case 3相比快8.2%，Ascend 950PR/Ascend 950DT的bank group存在2个读口，当同一repeat的读指令冲突后，由于读读之间无数据依赖，可以同时执行其它repeat不冲突的读指令，缓解读读冲突。


## Case 2：二读一写，src0/dst同bank，src0/src1同bank group，读写冲突

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x5100` | `000101000 1 000 00000` | 1 | 0 |
| dst | `0xA000` | `001010000 0 000 00000` | 0 | 0 |

### UB读写示意

![case2](./figures/case2.png)

### 分析结论

- src0/dst 同落在 bank0（depth 分别为 0 和 80），src1 在 bank1（depth=40），三者位于同一 bank group 0。
- src0/src1 分属不同 bank，无读读冲突。但二读一写超出单个 bank group 的能力上限，发生读写冲突。
- LDU=0% 表明读操作无直接冲突；STU=26.5% 表明写端口因 bank group 总带宽不足而排队，与 Case 1（读读冲突主导）的冲突模式不同。
- 性能低于 Case 1（1,470,563 vs 1,330,642 cycles，慢约 10.5%），说明读写冲突开销高于读读冲突 —— 读读之间无数据依赖可并行执行缓解，读写冲突无法消除。


## Case 3：二读一写，src0与dst地址重叠，读写冲突

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x5100` | `000101000 1 000 00000` | 1 | 0 |
| dst | `0x0000` | `000000000 0 000 00000` | 0 | 0 |

### UB读写示意

![case3](./figures/case3.png)

### 分析结论

- src0 与 dst 起始地址完全相同（0x0000），src1 在 bank1（0x5100），三者同属 BG=0。
- 性能与 Case 2 几乎一致（1,470,607 vs 1,470,563 cycles），LDU=0%/STU=27.2% vs LDU=0%/STU=26.5%。
- 说明在 Ascend 950PR/Ascend 950DT 架构上，读写地址重叠不会消除 bank 冲突 —— 硬件仍将读写视为独立操作，store 端口排队无法避免。这与部分旧架构的行为不同。


## Case 4：二读一写，src1/dst同bank，src0/src1同bank group，读写冲突

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x5100` | `000101000 1 000 00000` | 1 | 0 |
| dst | `0xA100` | `001010000 1 000 00000` | 1 | 0 |

### UB读写示意

![case4](./figures/case4.png)

### 分析结论

- src1/dst 同落在 bank1（depth 分别为 40 和 80），src0 在 bank0（depth=0），三者同属 BG=0。
- 与 Case 2（src0/dst 同 bank0）形成对称：dst 从 bank0 移到 bank1，冲突模式不变。
- 性能 1,440,690 cycles，LDU=0%/STU=25.9%，与 Case 2 接近，说明冲突来源一致 —— 二读一写下 bank group 带宽不足。


## Case 5：二读一写，src1与dst地址重叠，读写冲突

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x5100` | `000101000 1 000 00000` | 1 | 0 |
| dst | `0x5100` | `000101000 1 000 00000` | 1 | 0 |

### UB读写示意

![case5](./figures/case5.png)

### 分析结论

- src1 与 dst 起始地址完全相同（0x5100），src0 在 bank0（0x0000），三者同属 BG=0。
- 1,440,518 cycles，LDU=0%/STU=26.0%，与 Case 4（1,440,690 cycles）基本一致。
- 再次验证：Ascend 950PR/Ascend 950DT 上地址重叠不会消除 bank 冲突，冲突开销与同 bank 不同 depth 场景相同。


## Case 6：二读一写，src0/src1/dst同bank，全冲突，无循环展开

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x5000` | `000101000 0 000 00000` | 0 | 0 |
| dst | `0xA000` | `001010000 0 000 00000` | 0 | 0 |

### UB读写示意

![case6](./figures/case6.png)

### 分析结论

- 三个操作数全落在 bank0（BG=0），depth 分别为 0、40、80。既存在 src0/src1 之间的读读冲突，又存在读写冲突，为最严重的冲突场景。
- 采用无循环展开的 `AddWithoutUnRollVF`，每个迭代仅发射一组 load-load-add-store 指令。
- LDU 仅 4.1%，因为单指令发射时 load/store 不会同时执行，冲突被指令流水线的串行化掩盖，msprof 未充分暴露真实读冲突。
- 1,420,587 cycles 为全冲突场景下无优化的基准性能。


## Case 7：二读一写，src0/src1/dst同bank，全冲突 + 循环展开

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x5000` | `000101000 0 000 00000` | 0 | 0 |
| dst | `0xA000` | `001010000 0 000 00000` | 0 | 0 |

### UB读写示意

![case7](./figures/case7.png)

### 分析结论

- 地址配置与 Case 6 完全相同，区别仅在于采用 2 路循环展开的 `AddWithUnRollVF`，前半段与后半段交替发射指令。
- 循环展开使指令双发概率提高，LDU 从 4.1% → 40.2%，暴露了真实的读读冲突；同时 STU 从 24.2% → 18.0%，说明指令并行释放了一部分存储流水的竞争压力。
- 总 cycles 从 1,420,587 → 1,332,991，性能提升 6.2%。msprof 数据显示 LDU 升高但 STU 降低，说明循环展开将瓶颈从存储竞争转移到了负载竞争，而负载竞争可通过多 repeat 并行部分缓解。


## Case 8：离散与聚合，Gather数据索引连续，跨bank分散无冲突

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| index (src0) | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| data (src1) | `0x0400` | `000001000 0 000 00000` | 0 | 0 |
| dst | `0x0400` | `000001000 0 000 00000` | 0 | 0 |

### UB读写示意

![case8](./figures/case8.png)

### 案例分析

- 使用 `GatherAndScatterVF`，索引取值 [0..255] 连续递增（共 256 个 unique 值）。
- Index 值 0..255 乘以 sizeof(float)=4 后偏移量为 0x0..0x3FC，每个 32B（8 个 float）跨越一个 bank group，256 个元素分散到不同 bank/sub bank 的不同 depth 行。
- 650,995 cycles，各元素自然跨 bank 分散，Gather 读取和 Scatter 写入均无明显的 bank/sub bank 冲突。
- LDU/STU 冲突率指标在此场景下未采集有效值，subbank 冲突属于更低层级的存储冲突，无法被 msprof Default 指标捕获，需通过 cycles 差异对比诊断。


## Case 9：离散与聚合，Gather数据索引交替重复，同subbank冲突

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| index (src0) | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| data (src1) | `0x0400` | `000001000 0 000 00000` | 0 | 0 |
| dst | `0x0400` | `000001000 0 000 00000` | 0 | 0 |

### UB读写示意

![case9](./figures/case9.png)

### 案例分析

- 地址配置与 Case 8 完全相同，区别在于 index 模式为 0、128 交替重复（每个 repeat 内一半元素通过 index=0 读取，另一半通过 index=128 读取）。
- index=0 → 数据地址 0x0400，subbank=0（WIDTH=0）；index=128 → 数据地址 0x0600，同样处于 subbank=0 但 depth=12（与 index=0 的 depth=8 不同）。
- 二者均落在同一 bank 的同一 subbank（sub0），但位于不同 depth 行，离散读取需要在同一 subbank 的不同行间切换，触发 subbank 冲突。
- 750,819 cycles，对比 Case 8（650,995 cycles）劣化 15.2%，说明 subbank 同地址行冲突对 Gather/Scatter 性能有显著影响。


## Case 10：一读一写，按行搬运，DataBlock连续排布，无bank冲突

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| dst | `0x4100` | `000100000 1 000 00000` | 1 | 0 |

### UB读写示意

![case10](./figures/case10.png)

### 分析结论

- 使用 `AddsByRowVF`，`DATA_BLOCK_COPY` 模式 blockStride=1（默认连续）。每个 repeat 内 8 个 DataBlock 间隔 32B 连续排布，沿 UB 地址递增。
- 由于 32B 恰好是 bank group 轮转周期（每 32B 切换一个 bank group），8 个 DataBlock 自然命中 BG 0-7 各一次，无 bank group 竞争。
- src0 和 dst 分属不同 bank（bank0 vs bank1），一读一写在 bank group 能力范围内，不存在冲突。
- 440,952 cycles，LDU=0%/STU=0%，为零冲突基准性能。


## Case 11：一读一写，按列搬运，DataBlock非连续排布，严重bank冲突

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| dst | `0x4100` | `000100000 1 000 00000` | 1 | 0 |

### UB读写示意

![case11](./figures/case11.png)

### 分析结论

- 使用 `AddsByColVF`，`DATA_BLOCK_COPY` 模式 blockStride=8。每个 repeat 内 8 个 DataBlock 间隔 8 × 32B = 256B。
- 256B = 8 × 32B，正好是 8 个 bank group 的完整轮转周期。因此 8 个 DataBlock 全部回到**同一个 bank group 的同一个 bank**，仅 depth 不同（每 256B depth+8）。
- 同一 repeat 内 8 块数据全部竞争同一 bank，同时发生读读冲突（src 端）、写写冲突（dst 端）以及读写冲突，三类冲突叠加。
- 1,510,657 cycles，STU 冲突率高达 52.8%（写冲突为主要瓶颈），性能较 Case 10 劣化约 3.4 倍，是最严重的非连续搬运冲突场景。


## Case 12：一读一写，repeat内同地址重复读取，硬件合并

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| dst | `0x4100` | `000100000 1 000 00000` | 1 | 0 |

### UB读写示意

![case12](./figures/case12.png)

### 分析结论

- 使用 `AddsWithBlockOverlap`，`DATA_BLOCK_COPY` 模式 blockStride=0。同一 repeat 内 8 个 DataBlock 均从同一 32B 基地址读取，dst 按 blockStride=1 写入。
- 硬件自动检测到同一 repeat 内的相同地址读取请求，合并为单次 UB 读取，8 个 DataBlock 共享同一份数据。
- 440,791 cycles，LDU=0%/STU=0%，与 Case 10 基准水平一致。
- 结论：repeat 内同地址重复读取无需额外优化，硬件自动合并避免冗余访问。


## Case 13：一读一写，repeat间同首地址重复读取，无劣化

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| dst | `0x4100` | `000100000 1 000 00000` | 1 | 0 |

### UB读写示意

![case13](./figures/case13.png)

### 分析结论

- 使用 `AddsWithRepeatOverlap`，`DATA_BLOCK_COPY` 模式，所有 8 轮 repeat 迭代均从同一首地址 `src0`（0x0000）加载数据，dst 按不同 offset 写入。
- src 端 8 轮迭代始终读取相同的 256B 数据块，硬件缓存命中，无额外 UB 访问开销。
- 440,770 cycles，LDU 0.1%/STU 0%，与 Case 10 基准持平。
- 结论：repeat 间同首地址重复读取与 Case 12 类似，由硬件自动合并，无性能劣化。


## 小结

1. Ascend 950PR/Ascend 950DT采用低位编址，地址交替分配到不同的存储模块，bank冲突场景少于Atlas A2/A3系列产品。
2. Ascend 950PR/Ascend 950DT的bank冲突的开销小于Atlas A2/A3系列产品，两条指令发生bank冲突可以与其它指令达成并行。
3. 二读一写下必然存在bank冲突。由于Ascend 950PR/Ascend 950DT一条指令处理256B（8个DataBlock），覆盖全部的8个BG，因此二读一写会发生在一个bank group，超出单个bank group的能力。（在Atlas A2/A3中，由于bank group个数多一倍，冲突可消除），在实际场景中，由于采用Regbase架构，UB读写频率降低，bank冲突往往为非瓶颈点。
4. 读读冲突（case1，1,330,642 cycles）开销低于读写冲突（case2，1,470,563 cycles），且可通过循环展开缓解（case7 vs case6：1,332,991 vs 1,420,587，优化6.2%）。从msprof数据看，循环展开使LDU冲突率从4.1%提升到40.2%（暴露了真实读冲突），同时将STU冲突率从24.2%降至18.0%，说明指令双发缓解了存储流水线竞争。
5. 由于架构发生变化，读写地址重叠时，bank冲突依然会发生。case3的src0/dst地址重叠（1,470,607 cycles），LDU=0%/STU=27.2%与case2（LDU=0%/STU=26.5%）一致，证明msprof冲突率与cycle结论一致。
6. 当src0/src1分属同一bank group的不同bank（case2-5），LDU=0%表示load未直接冲突，但STU=26%表明store口因bank group总带宽不足而排队。当src0/src1落在同一bank（case1/7），LDU立即飙升至40%+，两类冲突模式可通过msprof清晰区分。

### 非连续对齐搬运的bank冲突

1. 连续DataBlock排布不产生冲突。case10每个repeat内8个DataBlock间隔32B连续排布，命中8个不同BG，440,952 cycles（基准）。
2. 非连续DataBlock排布触发严重bank冲突（DataBlock间隔256B）。case11按列搬运，每个repeat内8个DataBlock间隔256B，全部落入**同一bank group的不同depth行**，发生读读+写写+读写三类冲突叠加，1,510,657 cycles，比基准慢约3.4倍。msprof显示STU冲突率高达52.8%，以写冲突为主要瓶颈。
3. repeat内同地址重复读取硬件自动合并。case12同一repeat内8个DataBlock均指向同一32B基地址，硬件合并为单次读取，440,791 cycles，接近基准。
4. repeat间同首地址重复读取无劣化。case13的8轮迭代均从同一首地址加载，440,770 cycles，与基准持平。

### 离散与聚合API的subbank冲突

1. 连续index跨bank分散无冲突。case8使用[0..255]连续递增index，256个unique地址自然分散到不同bank/subBank，650,995 cycles，接近基准。
2. 同subbank不同行触发明显冲突。case9使用0/128交替index，二者落入**同一subbank但不同depth行**，读取需排队，750,819 cycles，比基准慢15.3%。注：subbank冲突属于更低层级的存储冲突，不被LDU/STU冲突率指标捕获，需通过cycles差异对比诊断。

### bank冲突优化建议

1. 循环展开增加指令双发概率可减少空闲，降低bank冲突带来的影响。case7（全冲突循环展开，1,332,991 cycles）在case6（全冲突无展开，1,420,587 cycles）基础上使用循环展开，性能提升6.2%。
2. 同地址重复读取无需额外优化。case12/case13的同地址重复读取由硬件自动合并，性能与连续搬运基准持平。

## 目录结构

```text
├── bank_conflict.asc              // 算子host + device代码
├── config.h                      // 14个Case的地址/类型配置
├── vf_func.h                     // Vector函数实现
├── data_utils.h                  // 数据读写工具
├── CMakeLists.txt                // 编译工程
├── test.sh                       // 测试脚本
├── figures/                      // 各Case UB内存分布图
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
│   └── case14.png
└── scripts/
    ├── gen_data.py               // 测试数据生成
    └── verify_result.py          // 精度验证
```

## 运行方式

### 环境准备

```bash
source /path/to/cann/set_env.sh
```

### 编译与运行

```bash
export SCENARIO_NUM=1          # 有效值: 1-13
mkdir -p build && cd build
cmake .. -DSCENARIO_NUM=${SCENARIO_NUM}
make -j
python3 ../scripts/gen_data.py --case ${SCENARIO_NUM}
./demo                     # 精度验证 + 打点性能（输出cycles计数到stdout）
python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin
```

### CMake选项

| 选项 | 可选值 | 说明 |
|:----:|:------:|:----|
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构版本 |
| `SCENARIO_NUM` | 1-13 | 切换测试场景 |
| `CMAKE_ASC_RUN_MODE` | `npu` | 上板运行模式 |

### 冲突性能采集

通过 `msprof` 采集硬件性能计数器，量化bank冲突程度：

```bash
# 编译特定case后，在build目录执行
msprof op --aic-metrics=Default ./demo
```

监控结果保存在 `OPPROF_<timestamp>/` 目录下，关键指标如下：

#### ResourceConflictRatio.csv

| 指标 | 含义 | 说明 |
|------|------|------|
| `aiv_vec_ldu_cflt_ratio` | 向量Load单元冲突率 | 读操作因bank冲突被阻塞的时间比例，越高说明读取冲突越严重 |
| `aiv_vec_stu_cflt_ratio` | 向量Store单元冲突率 | 写操作因bank冲突被阻塞的时间比例，越高说明写入冲突越严重 |
| `aiv_vec_wait_ratio` | 向量单元等待比例 | 向量指令因等待数据就绪而stall的比例 |

#### MemoryUB.csv

| 指标 | 含义 |
|------|------|
| `aiv_ub_read_bw_vector` | 向量单元UB读带宽 (GB/s) |
| `aiv_ub_write_bw_vector` | 向量单元UB写带宽 (GB/s) |

Conflict场景下，UB读写带宽会因冲突排队而降低。例如 Case 7 的读带宽（125 GB/s）高于 Case 6（89.7 GB/s），说明循环展开通过指令并行提高了带宽利用率。
