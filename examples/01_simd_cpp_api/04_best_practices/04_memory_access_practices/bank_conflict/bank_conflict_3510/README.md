# NPU 架构版本 351x Bank 冲突算子直调样例

## 概述

本样例演示`351x`架构上 UB Bank 冲突的不同场景及其性能影响。通过 11 个 Case 对比同一计算逻辑在不同地址布局、指令模式下的性能差异，总结规避 Bank 冲突的最佳实践。

## 支持的产品

- Ascend 950PR / Ascend 950DT

## 351x UB 关键特性

351x UB 结构：
- 256KB UB，8 个 bank group，每组 2 个 bank（共 16 bank），每 bank 512 行 × 32B = 16KB
- 每组 2 个读口 + 1 个写口
- 低位地址交织

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

地址的物理 bank 编号：

| addr | bank | addr | bank | addr | bank |
|:----:|:----:|:----:|:----:|:----:|:----:|
| 0x0000 | 0 | 0x4000 | 0 | 0x8000 | 0 |
| 0x4100 | 8 | 0x8900 | 8 | 0x0400 | 0 |

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

## 样例描述

### 算子场景

共 11 个 Case，通过 `CMakeLists.txt` 编译时传入 `-DCASE_NUM=N` 切换。

| Case | VF 函数 | 场景 | 冲突特征 |
|:----:|:--------:|:----|:---------|
| 1 | AddVF | 二读一写 (x+y=z)，无地址间隔 | 读读冲突 + 读写冲突（基准） |
| 2 | AddSingleVF | 同 Case 1，无循环展开 | 同上，对比展开对冲突的影响 |
| 3 | AddVF | 二读一写，地址偏移 0x100 | 读读冲突消除 |
| 4 | AddVF | 地址重叠 (src0==src1) | 读指令减半 |
| 5 | AddWithInstrLoadVF | 双搬入 DINTLV_B32 | 无读读冲突 |
| 6 | AddsVF | 一读一写，同 bank | 读写冲突 |
| 7 | AddsVF | 一读一写，地址偏移 0x100 | 无冲突 |
| 8 | GatherAndScatterVF | 离散访存，idx [0..7] 重复 | 同地址可 coalesce |
| 9 | GatherAndScatterVF | 离散访存，idx 连续 | 跨 bank 自动分散 |
| 10 | GatherAndScatterVF | **subbank 冲突** idx [0,128] 交替 | 同 subbank 不同行，不可 coalesce |
| 11 | GatherAndScatterVF | 无 subbank 冲突 idx 全 0 | 全部 coalesce |

### Case 地址配置与冲突分析

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

### Case 1-5 详细配置: 二读一写 / 双搬入

| Case | src0 地址 | src1 地址 | dst 地址 | TOTAL_LENGTH | 备注 |
|:----:|:---------:|:---------:|:--------:|:------------:|:-----|
| 1 | 0x0000 | 0x4000 | 0x8000 | 4096 | AddVF，循环展开 |
| 2 | 0x0000 | 0x4000 | 0x8000 | 4096 | AddSingleVF，无展开 |
| 3 | 0x0000 | 0x4100 | 0x8900 | 4096 | 地址偏移消除读读冲突 |
| 4 | 0x0000 | **0x0000** | 0x4100 | 4096 | 地址重叠，读指令减半 |
| 5 | 0x0000 | 0x4000 | 0x8000 | 4096 | 双搬入 DINTLV_B32 |

**Case 1-5 UB 内存分布图**：

| Case 1/2 | Case 3 | Case 4 | Case 5 |
|:--------:|:------:|:------:|:------:|
| <img src="./figures/case1_2.jpg" width="180"> | <img src="./figures/case3.jpg" width="180"> | <img src="./figures/case4.jpg" width="180"> | <img src="./figures/case5.jpg" width="180"> |

### Case 6-7 详细配置: 一读一写

| Case | src0 地址 | src1 地址 | dst 地址 | TOTAL_LENGTH | 备注 |
|:----:|:---------:|:---------:|:--------:|:------------:|:-----|
| 6 | 0x0000 | 0x0000 | 0x4000 | 4096 | 同 bank 读写冲突 |
| 7 | 0x0000 | 0x0000 | 0x4100 | 4096 | 消除读写冲突 |

**Case 6-7 UB 内存分布图**：

| Case 6 | Case 7 |
|:------:|:------:|
| <img src="./figures/case6.jpg" width="270"> | <img src="./figures/case7.jpg" width="270"> |

### Case 8-11 详细配置: 离散访存

| Case | index 地址 | data 地址 | index 模式 | 特征 |
|:----:|:----------:|:---------:|:----------|:-----|
| 8 | 0x0000 | 0x0400 | 重复 [0..7] | 同地址可 coalesce |
| 9 | 0x0000 | 0x0400 | 连续 [0..255] | 跨 bank 分散 |
| 10 | 0x0000 | 0x0400 | 交替 [0,128] | **同 subbank 不同行冲突** |
| 11 | 0x0000 | 0x0400 | 全 0 | 全部 coalesce |

**Case 8-11 UB 内存分布图**：

| Case 8 | Case 9 | Case 10 | Case 11 |
|:------:|:------:|:-------:|:-------:|
| <img src="./figures/case8.jpg" width="180"> | <img src="./figures/case9.jpg" width="180"> | <img src="./figures/case10.jpg" width="180"> | <img src="./figures/case11.jpg" width="180"> |

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

## 目录结构

```text
├── bank_conflict.asc              // 算子 host + device 代码
├── config.h                      // 11 个 Case 的地址/类型配置
├── vf_func.h                     // Vector 函数实现
├── data_utils.h                  // 数据读写工具
├── CMakeLists.txt                // 编译工程
├── test.sh                       // 测试脚本
├── figures/                      // 各 Case UB 内存分布图
│   ├── case1_2.jpg
│   ├── case3.jpg
│   ├── case4.jpg
│   ├── case5.jpg
│   ├── case6.jpg
│   ├── case7.jpg
│   ├── case8.jpg
│   ├── case9.jpg
│   ├── case10.jpg
│   └── case11.jpg
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
export CASE_NUM=1          # 1-11 可选
mkdir -p build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCASE_NUM=${CASE_NUM}
make -j
python3 ../scripts/gen_data.py --case ${CASE_NUM}
./demo                     # 精度验证
python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin
msprof op ./demo           # 性能 profiling
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
| `CASE_NUM` | 1-11 | 切换测试场景 |
| `CMAKE_ASC_RUN_MODE` | `npu` | 上板运行模式 |

## 性能分析

### 关键指标说明

| CSV 文件 | 关键字段 | 含义 |
|:--------:|:---------|:-----|
| `OpBasicInfo.csv` | Task Duration(us) | 算子总耗时 |
| `PipeUtilization.csv` | aiv_time(us) | 向量单元总活跃时间 |
| `PipeUtilization.csv` | aiv_vec_time(us) | 向量计算时间 |
| `PipeUtilization.csv` | aiv_scalar_time | 标量操作时间 |
| `PipeUtilization.csv` | aiv_mte2_ratio | MTE2 数据搬运占比 |
| `ResourceConflictRatio.csv` | aic_vec_ldu_cflt_ratio | 向量 Load 单元冲突占比 |
| `ResourceConflictRatio.csv` | aic_vec_stu_cflt_ratio | 向量 Store 单元冲突占比 |

### 实测结果

| Case | Task(us) | aiv_time(us) | aiv_vec(us) | 特征 |
|:----:|:--------:|:-----------:|:-----------:|:-----|
| 1 | 82.6 | 81.6 | 0.987 | 三路冲突，循环展开（基准） |
| 2 | 88.0 | 87.1 | 0.988 | 同 Case 1 无展开（+6.5%） |
| 3 | 89.7 | 88.5 | 0.986 | 消除读读冲突 |
| 4 | 70.1 | 69.1 | 0.983 | **地址重叠（最快）** |
| 5 | 91.9 | 90.9 | 0.987 | 双搬入（最慢） |
| 6 | 69.6 | 68.5 | 0.982 | 同 bank 读写冲突 |
| 7 | 70.4 | 69.1 | 0.982 | 消除读写冲突 |
| 8 | 41.5 | 40.3 | 0.977 | idx 重复→coalesce |
| 9 | 41.7 | 40.5 | 0.973 | idx 连续→跨 bank |
| 10 | **47.4** | **46.4** | 0.980 | **subbank 冲突（+14%）** |
| 11 | 41.4 | 40.3 | 0.977 | 全 coalesce |

### 结论

1. **aiv_vec 恒定**（~0.98 us）——向量计算管线不受冲突影响
2. **VF 层面 bank 冲突不可测**——低位交织 + 2读口/BG 让传统 bank 冲突不是瓶颈
3. **Subbank 冲突才是真正可测的瓶颈**——Gather/Scatter 的 index 模式直接影响 subbank 排队深度，Case 10 vs Case 11 慢 ~14%
4. **循环展开对冲突场景有正收益**——Case 1 vs Case 2 = 82.6 vs 88.0 us（+6.5%）
5. **地址重叠收益最大**——Case 4 省一条 Load，比基准快 15%
6. **Task ≈ aiv_time + 1 us**——向量单元占满整个算子执行
7. **调优优先级**：地址重叠/复用 > 避免 subbank 不同行冲突 > 标量指令优化 > 地址间隔

## 与 220x 的差异

351x 的 bank 冲突表现与 220x 有本质差异：
- 220x 使用**高位交织**，连续 256B 内所有 DataBlock 落在一个 BG，冲突概率高
- 351x 的**低位交织** + 2 读口/BG + 8 DB 分散到 8 个 BG 使冲突容错性大幅提升
- 220x 的调优策略（padding/地址间隔）在 351x 上收益有限
