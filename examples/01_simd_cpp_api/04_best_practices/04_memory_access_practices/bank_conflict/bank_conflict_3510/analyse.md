# 351x UB Bank 冲突实测分析

## A5 UB 地址解码

A5 的 UB 地址按低位交织：`ADDR = {DEPTH[8:0], BANK[0], BG[2:0], WIDTH[4:0]}`

| 字段 | 位域 | 说明 |
|------|------|------|
| `WIDTH[4:0]` | addr[4:0] | bank 内 32B 行内偏移 (0-31 bytes) |
| `BG[2:0]`    | addr[7:5] | bank group 选择 (0-7) |
| `BANK[0]`    | addr[8]   | bank group 内 bank 选择 (0=bank0, 1=bank1) |
| `DEPTH[8:0]` | addr[17:9] | bank 内深度/行号 (0-511) |

给定字节地址 `A`：
```
物理 bank = addr[8:5] = (A >> 5) & 0xF   (0-15)
BG        = addr[7:5] = (A >> 5) & 0x7
BANK      = addr[8]   = (A >> 8) & 0x1
```

## DataBlock 映射

一条 VF Load/Store 指令处理 256B = 8 个 DataBlock。每 DB = 32B，BG 每 32B 递增，BANK 每 256B 翻转。

从 `base` 地址开始的 8 个 DB 映射：

| DB | 偏移 | 物理 bank | BG | BANK |
|:--:|:----:|:---------:|:--:|:----:|
| 0 | +0   | b | b&7 | b>>3 |
| 1 | +0x20 | b+1 | b+1&7 | (b+1)>>3 |
| ... | ... | ... | ... | ... |
| 7 | +0xE0 | b+7 | b+7&7 | (b+7)>>3 |

其中 `b = (base >> 5) & 0xF`。8 DB 覆盖 8 个不同 BG（BG 0-7），若 b<8 则 BANK=0，否则 BANK=1。

关键性质：**一个 256B 的向量指令完整覆盖 BG 0-7 各一次**，单条指令内不可能有 BG 冲突。

冲突只发生在**不同 tensor 的对应 DB 落在相同物理 bank** 时。

## Subbank 与 Gather/Scatter 冲突

A5 每个 bank（32B 宽）分为 **4 个 subbank**，各 8B 宽 × 512 行深度，各自有 **独立读口**。

```
bank 内部 layout（一行 32B）：
┌────────┬────────┬────────┬────────┐
│ sub0   │ sub1   │ sub2   │ sub3   │
│  8B    │  8B    │  8B    │  8B    │
│ 2×f32  │ 2×f32  │ 2×f32  │ 2×f32  │
└────────┴────────┴────────┴────────┘
```

subbank 选择：`addr[4:3]`，每 subbank 内 2 个 float32 由 `addr[2]` 区分。

### Gather 冲突规则

- 命中**不同 subbank** → 无冲突（各自独立读口）
- 命中**同 subbank + 同起始地址** → coalesce → 无冲突（硬件合并为一次读）
- 命中**同 subbank + 不同起始地址** → **冲突**（单读口需排队）
- Gather/Scatter 与 ld/st/MTE 冲突 → 每拍一条，反压其余指令

### 各 case 实测

| Case | index 模式 | subbank 行为 | Task(us) |
|:----:|:----------|:------------|:--------:|
| 8 | [0..7]×32 | 每 subbank 仅 2 个 unique addr → 2 次读 | 41.5 |
| 9 | [0..63] 连续 | 跨 8 个 bank，每 subbank 2 次读 | 41.7 |
| 10 | [0,128]×32 | **同 subbank 不同行** → subbank 排队 → 冲突 | **47.4** |
| 11 | [0]×64 | 全部同地址 → coalesce 为 1 次 | 41.4 |

**Case 10（47.4us）比 Case 11（41.4us）慢 ~14%**——Gather 中 subbank 单读口在一次指令内只能服务一个 depth 行，当 index 交替命中两个不同行时，必须排队。

## Case 地址配置与冲突分析

| Case | VF 函数 | xLocal | yLocal | zLocal | 物理 bank 分析 |
|:----:|:--------|:------:|:------:|:------:|:--------------|
| 1 | AddVF | 0x0000 | 0x4000 | 0x8000 | 全部 bank0：三路同 bank（基准） |
| 2 | AddSingleVF | 0x0000 | 0x4000 | 0x8000 | 同 Case 1，无循环展开 |
| 3 | AddVF | 0x0000 | 0x4100 | 0x8900 | src0=bank0，src1+dst=bank8 |
| 4 | AddVF | 0x0000 | 0x0000 | 0x4100 | src0==src1=bank0，dst=bank8 |
| 5 | AddWithInstrLoadVF | 0x0000 | 0x4000 | 0x8000 | 单源 Load bank0 |
| 6 | AddsVF | 0x0000 | 0x0000 | 0x4000 | src+dst 均在 bank0 |
| 7 | AddsVF | 0x0000 | 0x0000 | 0x4100 | src=bank0，dst=bank8 |
| 8 | GatherAndScatterVF | 0x0000 | 0x0400 | 0x0400 | idx=[0..7]×32 |
| 9 | GatherAndScatterVF | 0x0000 | 0x0400 | 0x0400 | idx=[0..255] |
| 10 | GatherAndScatterVF | 0x0000 | 0x0400 | 0x0400 | idx=[0,128]×32，subbank 冲突 |
| 11 | GatherAndScatterVF | 0x0000 | 0x0400 | 0x0400 | idx 全 0，全部 coalesce |

地址的物理 bank 编号 = `(addr >> 5) & 0xF`：

| addr | bank | addr | bank | addr | bank |
|:----:|:----:|:----:|:----:|:----:|:----:|
| 0x0000 | 0 | 0x4000 | 0 | 0x8000 | 0 |
| 0x4100 | 8 | 0x8900 | 8 | 0x0400 | 0 |

## 实测性能数据

```
Case  Task(us)  aiv_time(us)  aiv_vec(us)  特征
1     82.6      81.6          0.987         三路冲突，循环展开（基准）
2     88.0      87.1          0.988         同 Case 1 无展开（+6.5%）
3     89.7      88.5          0.986         消除读读冲突
4     70.1      69.1          0.983         地址重叠（最快）
5     91.9      90.9          0.987         双搬入（最慢）
6     69.6      68.5          0.982         同 bank 读写冲突
7     70.4      69.1          0.982         消除读写冲突
8     41.5      40.3          0.977         idx 重复→coalesce
9     41.7      40.5          0.973         idx 连续→跨 bank
10    47.4      46.4          0.980         **subbank 冲突**
11    41.4      40.3          0.977         全 coalesce
```

### 关键发现

1. **aiv_vec 恒定**（~0.98 us）——向量计算管线不受冲突影响

2. **Subbank 冲突可测（~14%）**
   Case 10（同 subbank 不同行）vs Case 11（全 coalesce）：**47.4 vs 41.4 us**。Gather 中，subbank 单读口在一次指令内只能服务一个 depth 行。

3. **循环展开对冲突场景有正收益**
   Case 1（展开）vs Case 2（无展开）：82.6 vs 88.0 us（+6.5%）。

4. **VF 层面 bank 冲突不可测**
   Case 1 vs 3，Case 6 vs 7：差异在噪声范围。

5. **地址重叠收益最大**
   Case 4 省一条 Load，比基准快 15%。

### 结论

- VF 层面，**bank 冲突不是瓶颈**——低位交织 + 2读口/BG 让冲突不可测
- 离散访存层面，**subbank 冲突才是真正可测的瓶颈**——Gather/Scatter 的 index 模式直接影响 subbank 排队深度
- 循环展开对重度冲突有正收益，但对无冲突场景可能反噬
- 调优优先级：地址重叠/复用 > 避免 subbank 不同行冲突 > 标量指令优化 > 地址间隔

## 与 220x 的差异

351x 的 bank 冲突表现与 220x 有本质差异：
- 220x 使用**高位交织**，连续 256B 内所有 DataBlock 落在一个 BG，冲突概率高
- 351x 的**低位交织** + 2 读口/BG + 8 DB 分散到 8 个 BG 使冲突容错性大幅提升
- 220x 的调优策略（padding/地址间隔）在 351x 上收益有限
