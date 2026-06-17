# UB Bank冲突场景验证与分析(架构3510)

## UB（Unified Buffer）内存结构

![UB Architecture](./figures/ub_arch.png)

- 在Ascend 950PR/Ascend 950DT中，UB空间大小为256KB；
- 共有16个 **bank**，组成8个 **bank group**，每个bank group包含2个bank（`bank_id` = 0, 1）；
- 每个bank由512行组成（`BANK_DEPTH = 512`），每行长度为32B（`BANK_WIDTH = 32B`），即每个bank大小为16KB；
- 每个bank group最多支持两读或者一读一写。
- bank_number相同代表是同一个bank；bank_group_id相同代表是同一个bank group。

## subbank内存结构

**subbank内存结构只有在使用离散与聚合API时需要关注**

Ascend 950PR/Ascend 950DT的每个bank（32B宽）分为4个subbank，各8B宽 × 512行深度：

```
bank内部layout（一行32B）：
┌────────┬────────┬────────┬────────┐
│ sub0   │ sub1   │ sub2   │ sub3   │
│  8B    │  8B    │  8B    │  8B    │
└────────┴────────┴────────┴────────┘
```

使用离散与聚合API时，每个元素可能落入不同的subbank。同一subbank内多个不同地址的读取需要排队，产生subbank冲突。

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

## bank冲突类型（适用于离散与聚合API以外的使用场景）

根据内存结构，bank冲突主要分为以下三种类型：
- **读读冲突**：多个读操作同时访问同一个bank或两个以上读操作同时访问同一个bank group才会引发冲突（Atlas A2/A3 系列产品中多个读操作同时访问同一个bank group就会引发冲突）。
- **写写冲突**：多个写操作同时访问同一个bank group。
- **读写冲突**：读操作和写操作同时访问同一个bank。

## subbank冲突规则（适用离散与聚合API使用场景）

- 离散数据属于**不同subbank**，无冲突。
- 离散数据属于**同subbank的同起始地址**，无冲突（硬件合并为一次读）。
- 离散数据属于**同subbank + 不同起始地址**，发生冲突。

## 测试场景概览

本文围绕连续对齐搬入/搬出、离散与聚合、非连续对齐搬入/搬出三类API的bank冲突场景展开验证分析。所有场景的输入数据类型为 `float`（离散与聚合场景的index为 `uint32_t`），执行1000次以放大性能差异。

> **性能说明**：下表"打点性能"为VF连续执行1000次的总cycles数（`GetSystemCycle` 打点），数值越低性能越好。

### 连续对齐搬入/搬出的bank冲突场景（Case 1-6）

| case | 计算公式 | 场景描述 | bank冲突情况 | src0首地址 | src1首地址 | dst首地址 | 打点性能(cycles) | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | dst = src0 + src1 | src0/src1同bank，发生读读冲突，与dst不同bank，无读写冲突 | 读读冲突 | 0x0000 | 0x5000 | 0xA100 | 1,330,862 | 发生读读冲突 |
| 2 | dst = src0 + src1 | src0/dst同bank，发生读写冲突，与src1不同bank，无读读冲突，**src0/dst重叠** | 读写冲突 | 0x0000 | 0x5100 | 0x0000 | 1,470,506 | 发生读写冲突 |
| 3 | dst = src0 + src1 | src1/dst同bank，发生读写冲突，src0/src1不同bank，无读读冲突 | 读写冲突 | 0x0000 | 0x5100 | 0xA100 | 1,440,524 | 发生读写冲突 |
| 4 | dst = src0 + src1 | **使用双搬入读取** | 读写冲突 | 0x0000 | - | 0xA000 | 1,480,727 | 使用双搬入读取，对比case1性能劣化不明显 |
| 5 | dst = src0 + src1 | src0/src1/dst同bank，既有读读冲突，又有读写冲突，**无循环展开** | 读读冲突+读写冲突 | 0x0000 | 0x5000 | 0xA000 | 1,420,476 | 全冲突场景 |
| 6 | dst = src0 + src1 | src0/src1/dst同bank，既有读读冲突，又有读写冲突，**通过循环展开优化** | 读读冲突+读写冲突 | 0x0000 | 0x5000 | 0xA000 | 1,330,647 | 使用循环展开优化，对比case5性能提升6.3% |

### 离散与聚合场景（Case 7-10）

| case | 计算公式 | 场景描述 | subbank冲突情况 | index地址 | data地址 | index模式 | 打点性能(cycles) | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 7 | Gather + Add + Scatter | Gather数据索引全为0，起始地址全部重叠 | 同地址合并为单次读取，无冲突 | 0x0000 | 0x0400 | 全0 | 650,834 | 基准参照 |
| 8 | Gather + Add + Scatter | Gather数据索引[0..7]重复 | 同地址合并读取，无冲突 | 0x0000 | 0x0400 | [0..7]×32重复 | 650,884 | 数据分散在同一bank的不同sub bank，接近case7性能 |
| 9 | Gather + Add + Scatter | Gather数据索引[0..255]连续 | 跨bank分散，无冲突 | 0x0000 | 0x0400 | [0..255] 连续 | 650,803 | 数据分散在8个bank group的不同sub bank，接近case7性能 |
| 10 | Gather + Add + Scatter | Gather数据索引0、128交替重复 | 同subbank不同行冲突 | 0x0000 | 0x0400 | 0、128交替 | **752,528** | 对比Case 7，性能劣化15.6% |

### 非连续搬运跨步对比（Case 11-14）

| case | 计算公式 | 场景描述 | 冲突情况 | src0首地址 | dst首地址 | 搬运方式 | 打点性能(cycles) | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 11 | dst = src0 | 512元素按行计算，repeat内8个DataBlock连续排布，repeat间间隔256B | 无冲突 | 0x0000 | 0x4100 | DataBlock连续搬运 | 440,732 | 基准 |
| 12 | dst = src0 | 512元素按列计算，repeat内8个DataBlock间间隔256B，repeat间间隔32B | 读读冲突+写写冲突+读写冲突 | 0x0000 | 0x4100 | DataBlock非连续搬运 | 1,511,791 | 触发同一repeat内的bank冲突，对比case11，性能劣化明显 |
| 13 | dst = src0 | 同一repeat内8个DataBlock均从同一起始地址读取数据 | 无冲突 | 0x0000 | 0x4100 | DataBlock stride=0 | 440,875 | 同地址合并，无额外开销，接近case11性能 |
| 14 | dst = src0 | 8轮迭代，每次迭代数据从同一起始地址读取数据 | 无冲突 | 0x0000 | 0x4100 | DataBlock stride=1,repeat=0 | 440,777 | 同首地址重复读，无冲突，接近case11性能 |


各case的详细分析见下文对应章节。

## Case 1：二读一写，src0/src1同bank，发生读读冲突，与dst不同bank，无读写冲突

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x5000` | `000101000 0 000 00000` | 0 | 0 |
| dst | `0xA100` | `001010000 1 000 00000` | 1 | 0 |

### UB读写示意

![case1](./figures/case1.png)

### 分析结论

- src0=bank0，src1=bank0，dst=bank1。src0和src1同落在bank0，存在读读冲突。但dst在bank1，读写冲突消除。
- 与Case 3相比快8.2%，Ascend 950PR/Ascend 950DT的bank group存在2个读口，当同一repeat的读指令冲突后，由于读读之间无数据依赖，可以同时执行其它repeat不冲突的读指令，缓解读读冲突。

## Case 2：二读一写，src0/dst同bank，发生读写冲突，与src1不同bank，无读读冲突，src0/dst重叠

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x5100` | `000101000 1 000 00000` | 1 | 0 |
| dst | `0x0000` | `000000000 0 000 00000` | 0 | 0 |

### UB读写示意

![case2](./figures/case2.png)

### 分析结论

- src0与dst地址重叠（同bank 0同depth行），**存在读写冲突**，并且该冲突在Ascend 950PR/Ascend 950DT上不能被硬件优化。
- src1在bank1（同BG不同bank），与src0无读读冲突。

## Case 3：二读一写，src1/dst同bank，发生读写冲突，src0/src1不同bank，无读读冲突

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x5100` | `000101000 1 000 00000` | 1 | 0 |
| dst | `0xA100` | `001010000 1 000 00000` | 1 | 0 |

### UB读写示意

![case3](./figures/case3.png)

### 分析结论

- src0=bank0，src1=bank1，dst=bank1。src0和src1落在不同bank，无读读冲突，但src1和dst同在bank1，存在读写冲突。

## Case 4：二读一写，使用双搬入读取

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x4000` | `010000000 0 000 00000` | 0 | 0 |
| dst | `0xA000` | `001010000 0 000 00000` | 0 | 0 |

### UB读写示意

![case4](./figures/case4.png)

### 分析结论

- 使用 `LoadDist::DIST_DINTLV_B32` 双搬入模式，一条指令一次读取512B，无读读冲突。
- 对比Case 5无明显收益。

## Case 5：二读一写，src0/src1/dst同bank，既有读读冲突，又有读写冲突，无循环展开

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| src1 | `0x5000` | `000101000 0 000 00000` | 0 | 0 |
| dst | `0xA000` | `001010000 0 000 00000` | 0 | 0 |

### UB读写示意

![case5](./figures/case5.png)

### 分析结论

- src0、src1、dst三路首地址均映射到bank 0：虽然8个DataBlock分散在BG 0-7，但三路tensor的对应DataBlock都落在相同bank，**存在读读冲突和读写冲突**。

## Case 6：二读一写，src0/src1/dst同bank，既有读读冲突，又有读写冲突，通过循环展开优化

### 地址解码

地址配置同Case 5，详见Case 5地址解码。

### UB读写示意

读写冲突示意同Case 5，详见Case 5 UB读写示意。

### 分析结论

- 地址布局与Case 5完全相同，冲突特征相同。
- 唯一差异是使用循环展开，增加指令级并行。
- 对比Case 5，循环展开在该场景下有正收益。

## Case 7：离散读写，Gather数据索引全为0，起始地址全部重叠

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| index | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| data | `0x0400` | `000001000 0 000 00000` | 0 | 0 |
| dst | `0x0400` | `000001000 0 000 00000` | 0 | 0 |

### UB读写示意

![case7](./figures/case7.png)

- index全部为0，即每次Gather都从data[0]读取同一个位置。

### 分析结论

- index全0，Gather所有元素命中同一地址 → 硬件通过地址合并将多次请求合并为单次读取，因此无subbank冲突。

## Case 8：离散读写，Gather指令的数据索引在[0..7]重复

### 地址解码

同Case 7地址配置。

### UB读写示意

![case8](./figures/case8.png)

- 索引排布：`[0, 1, 2, 3, 4, 5, 6, 7]` 重复32次（共256个元素），由Gather读取；
- data buffer起始地址 `0x0400`，每条Gather指令从data中按index取64个float32；
- dst与data地址重叠（`0x0400`），Scatter写回同一区域。

### 分析结论

- Gather从data中按index取64个元素：index仅有 [0..7] 共8个unique值，大量元素命中相同地址 → 硬件自动合并同地址请求，同地址只需一次读取，subbank冲突降低。

## Case 9：离散读写，Gather指令的数据索引在[0..255]连续

### 地址解码

同Case 7地址配置。

### UB读写示意

![case9](./figures/case9.png)

- index模式：`[0, 1, 2, ..., 255]` 连续递增，每条Gather指令处理64个连续index；
- data buffer起始地址 `0x0400`，dst与data地址重叠。

### 分析结论

- Gather跨8个bank分散读取，每subbank仅2次读操作。连续index自然分散到不同subbank，subbank冲突降低。

## Case 10：离散读写，Gather数据索引0、128交替重复

### 地址解码

同Case 7地址配置。

### UB读写示意

![case10](./figures/case10.png)

- index模式：偶数位置为 `0`，奇数位置为 `128`（`[0, 128, 0, 128, ...]` 交替）；
- `0` 和 `128` 在Ascend 950PR/Ascend 950DT上都落入bank 0的同一subbank，但属于**不同depth行**。

### 分析结论

- Gather命中**同subbank + 不同起始地址**（相同subbank，不同depth行），读操作需排队，subbank冲突明显。

## Case 11：512元素按行计算，repeat内8个DataBlock连续排布，repeat间间隔256B

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| dst | `0x4100` | `010000010 0 000 00000` | 1 | 0 |

### UB读写示意

![case11](./figures/case11.png)

### 搬运方式

迭代8轮 ，每轮读写8个连续DataBlock。

### 分析结论

- 每次迭代的8个DataBlock连续排布（间隔32B），等价于普通 `LoadAlign`指令，此时单条非连续搬运指令8个DataBlock分散到不同的bankgroup，不发生冲突。

## Case 12：512元素按列计算，repeat内8个DataBlock间间隔256B，repeat间间隔32B

### 地址解码

同Case 11地址配置。

### UB读写示意

![case12](./figures/case12.png)

### 搬运方式

迭代8轮 ，每轮读写8个DataBlock，每个DataBlock间隔256B。

### 分析结论

- 每次迭代的8个DataBlock均从同一bank group的不同depth行读取，命中**同bank group不同行**，发生bank冲突，对比Case 11性能劣化。

## Case 13：一读一写，同一repeat内8个DataBlock均从同一起始地址读取数据

### 地址解码

| 操作数 | 首地址 | 18位二进制（DEPTH BANK BG WIDTH） | bank_id | bank_group_id |
| --- | --- | --- | --- | --- |
| src0 | `0x0000` | `000000000 0 000 00000` | 0 | 0 |
| dst | `0x4100` | `010000010 0 000 00000` | 1 | 0 |

### UB读写示意

![case13](./figures/case13.png)

### 搬运方式

迭代8轮，每轮读取的8个DataBlock均指向同一32B基地址，写回时8个DataBlock连续排布。

### 分析结论

- 同一迭代内8个DataBlock全部命中同一地址，硬件自动合并为单次读取，寄存器被填充为8份相同数据，无额外开销。

## Case 14：一读一写，8轮迭代，每次迭代数据从同一起始地址读取数据

### 地址解码

同Case 11地址配置。

### UB读写示意

![case14](./figures/case14.png)

### 搬运方式

迭代8轮，每轮从固定首地址src0（不偏移）读取8个连续的DataBlock，写回时8个DataBlock连续排布。

### 分析结论

- 8轮迭代均从同一个首地址加载，每次迭代从同一首地址读取几乎无额外开销。

## 小结

### 连续对齐搬运的bank冲突

1. Ascend 950PR/Ascend 950DT采用低位编址，bank冲突的开销小于Atlas A2/A3系列产品，bank冲突造成的空隙可以被其它指令填满，bank冲突触发减少，通过改变首地址优化bank冲突收益有限。
2. 二读一写下必然存在bank冲突。由于Ascend 950PR/Ascend 950DT一条指令处理256B（8个DataBlock），覆盖全部的8个BG，因此二读一写会发生在一个bank group，超出单个bank group的能力。（在Atlas A2/A3中，由于bank group个数多一倍，冲突可消除），在实际场景中，由于采用Regbase架构，UB读写指令的使用降低，bank冲突往往为非瓶颈点。
3. 架构发生变化，读写地址重叠时，bank冲突依然会发生。case2的src0/dst地址重叠，依然触发了bank冲突。
4. 双搬入指令搬运对性能劣化不明显，case4与case5性能接近。


### 非连续对齐搬运的bank冲突

1. 连续DataBlock排布不产生冲突。case11每个repeat内8个DataBlock间隔32B连续排布，命中8个不同BG，440,732 cycles（基准）。
2. 非连续DataBlock排布触发严重bank冲突（DataBlock间隔256B）。case12按列搬运，每个repeat内8个DataBlock间隔256B，全部落入**同一bank group的不同depth行**，发生读读+写写+读写三类冲突叠加，1,511,791 cycles，比基准慢约3.4倍。
3. repeat内同地址重复读取硬件自动合并。case13同一repeat内8个DataBlock均指向同一32B基地址，硬件合并为单次读取，440,875 cycles，接近基准。
4. repeat间同首地址重复读取无劣化。case14的8轮迭代均从同一首地址加载，440,777 cycles，与基准持平。

### 离散与聚合API的subbank冲突

1. 同地址合并，无subbank冲突。case7 index全为0，所有Gather命中同一地址，硬件合并为单次读取，650,834 cycles（基准）。
2. 少量unique index重复无冲突。case8使用[0..7]×32重复index，仅8个unique地址被大量重复命中，同地址请求硬件合并，650,884 cycles，接近基准。
3. 连续index跨bank分散无冲突。case9使用[0..255]连续递增index，256个unique地址自然分散到不同bank/subBank，650,803 cycles，接近基准。
4. 同subbank不同行触发明显冲突。case10使用0/128交替index，二者落入**同一subbank但不同depth行**，读取需排队，752,528 cycles，比基准慢15.6%。

### bank冲突优化建议

1. 循环展开增加指令双发概率可对冲bank冲突带来的影响。case6在case5全冲突地址布局基础上使用循环展开（每次处理两个repeat），性能得到提升。
2. 同地址重复读取无需额外优化。case13/14的同地址重复读取由硬件自动合并，性能与连续搬运基准持平。


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

### 编译与运行（单个Case）

```bash
export SCENARIO_NUM=1          # 1-14可选
mkdir -p build && cd build
cmake .. -DSCENARIO_NUM=${SCENARIO_NUM}
make -j
python3 ../scripts/gen_data.py --case ${SCENARIO_NUM}
./demo                     # 精度验证 + 打点性能（输出cycles计数到stdout）
python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin
```

### 批量运行

```bash
bash test.sh             # 精度验证
bash test.sh profile     # 精度验证 + profiling
```

### CMake选项

| 选项 | 可选值 | 说明 |
|:----:|:------:|:----|
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构版本 |
| `SCENARIO_NUM` | 1-14 | 切换测试场景 |
| `CMAKE_ASC_RUN_MODE` | `npu` | 上板运行模式 |
