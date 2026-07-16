# L2 Cache切分调优样例

## 概述

本样例以向量加法为例，展示 **L2 Cache切分**（L2 Cache Tiling）策略对性能的影响。通过对比 **未开启切分（反例）** 与 **开启切分（正例）** 两种实现，展示当数据量超过L2 Cache容量时，L2 Cache切分策略如何提升数据搬运效率。

**核心差异**：
- **反例（nosplit）**：按核数均匀切分数据，每个核处理固定大小的数据块。当总数据量超过L2 Cache大小时，各核访问数据L2 Cache命中率较低（Cache 中仅保留最近访问的数据，之前的数据被逐出（eviction）），导致后续访问时需要从GM重新加载，引发 Cache miss，成为性能瓶颈。
- **正例（split）**：基于L2 Cache容量进行全局Tiling，将总数据切分为多个能完整放入L2 Cache的Tile，所有核先共同处理第一个Tile，再处理下一个Tile。使得数据复用时可直接从L2 Cache读取，提升带宽利用率。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```
├── add_l2_cache_split
│   ├── CMakeLists.txt               // 编译工程文件
│   ├── data_utils.h                 // 数据读入写出函数
│   ├── l2_cache_optimizer.h         // L2 Cache切分优化器
│   ├── add_l2cache_split.asc        // 正例：开启L2 Cache切分
│   ├── add_l2cache_nosplit.asc      // 反例：未开启L2 Cache切分
│   ├── scripts
│   │   ├── gen_data.py              // 生成测试数据
│   │   └── verify_result.py         // 验证计算结果
│   ├── README.md                    // 样例说明文档（中文）
│   └── README_en.md                 // 样例说明文档（英文）
```

## 样例描述

**样例功能**：

  样例实现的是 `TOTAL_LEN = 40000000` 个half类型元素的向量加法。

  Add的计算公式为：

$$
  z = x + y
$$

  - x：输入，长度为40000000，数据类型为half（40000000 × 2B = 80000000B，即 76.29MiB）。
  - y：输入，长度为40000000，数据类型为half（40000000 × 2B = 80000000B，即 76.29MiB）。
  - z：输出，长度为40000000，数据类型为half（40000000 × 2B = 80000000B，即 76.29MiB）。

  三份数据总大小为 40000000 × 2B × 3 = 240000000B（即 228.88MiB），超过Atlas A2系列L2 Cache容量（192MiB），可采用L2 Cache切分策略进行性能优化。

## 核心实现对比

### 数据切分策略差异

| 维度 | 反例（nosplit） | 正例（split） |
|:---|:---|:---|
| 切分方式 | 按核数均匀切分 | 按L2Cache容量做全局Tiling |
| 数据视图 | 每个核独立处理整段数据的固定一段 | 所有20核协同处理同一Tile，按Tile分批 |
| 数据总量感知 | 不感知L2Cache容量 | 通过 `L2CacheOptimizer` 感知L2Cache容量 |
| 核利用率 | 20核全部工作 | 20核全部工作（每个Tile中均参与） |
| L2Cache命中 | 所有核同时访问不重叠数据，总数据超出L2Cache → **Cache miss** | 每Tile 100MiB（由L2_TILE_THRESHOLD=100×1024²控制）放入L2Cache，下一轮访问命中 → **Cache hit** |
| 单时刻L2Cache数据量 | 240000000B/228.88MiB（20核 × 40000000/20 × 3 × 2B） | 最大100MiB（由L2_TILE_THRESHOLD=100×1024²控制，1个Tile总数据量） |

### 反例关键代码（add_l2cache_nosplit.asc）

按核数均匀切分，每个核独立处理自己的固定数据块。核内再分 `TILE_NUM=2` 个子Tile交替处理，外层 `LOOP_COUNT=2` 重复处理同一数据。当回到首轮Tile时L2Cache中已被后续Tile数据替换，导致Cache miss：

```cpp
int64_t blockLen = Ceil(totalLen_, blockNum);
int64_t blockOffset = curBlockIdx * blockLen;
int64_t tileLen = Ceil(curBlockLen, TILE_NUM);

for (int32_t i = 0; i < LOOP_COUNT; i++) {
    for (int32_t j = 0; j < TILE_NUM; j++) {
        // 第j个Tile的全局偏移
        int64_t tileOffset = blockOffset + j * tileLen;
        // 按MAX_DATA_COPY_LEN分块搬运 + 流水线计算
        for (int64_t k = 0; k < totalBlocks; k++) {
            DataCopy x, DataCopy y, Add, DataCopy z
        }
        PipeBarrier<PIPE_ALL>();  // tile间同步
    }
}
```

### 正例关键代码（add_l2cache_split.asc）

通过 `L2CacheOptimizer` 根据L2Cache阈值将全局数据切分为多个Tile。**所有20核协同处理同一个Tile**，Tile内将数据按核数均分，处理完毕通过 `PipeBarrier` 同步后再进入下一Tile：

```cpp
L2CacheOptimizer l2Opt(totalLen_, blockNum);

for (int64_t tileIdx = 0; tileIdx < l2Opt.GetTileNum(); tileIdx++) {
    int64_t tileLen = l2Opt.GetBlockLen(tileIdx);
    int64_t tileOffset = l2Opt.GetBlockOffset(tileIdx);

    // 本Tile内，按核数均分数据
    int64_t perCoreLen = Ceil(tileLen, blockNum);
    int64_t coreStart = tileOffset + curBlockIdx * perCoreLen;
    int64_t coreLen = min(perCoreLen, tileLen - curBlockIdx * perCoreLen);

    // 按MAX_DATA_COPY_LEN分块搬运 + 流水线计算
    for (int64_t i = 0; i < totalBlocks; i++) {
        DataCopy x, DataCopy y, Add, DataCopy z
    }

    PipeBarrier<PIPE_ALL>();  // 所有核同步后再进入下一Tile
}
```

### L2CacheOptimizer 切分逻辑

> 注：`L2_TILE_THRESHOLD = 100 × 1024² = 100MiB` 是示例阈值，不同硬件L2Cache容量不同（如Atlas A2为192MiB，Ascend 950系列可能更大），使用者可根据目标硬件调整该值。阈值建议设置为略小于实际L2Cache容量，为其他数据留出余量。

```cpp
constexpr int64_t L2_TILE_THRESHOLD = 100 * 1024 * 1024; // 100MiB阈值，不同硬件L2Cache大小不同，可根据实际情况调整

L2CacheOptimizer(int64_t totalLen, int64_t blockNum) {
    int64_t totalByteSize = totalLen * sizeof(half) * 3;
    if (totalByteSize <= L2_TILE_THRESHOLD) {
        // 数据量在L2Cache内，不需切分
        tileNum_ = blockNum_;
    } else {
        // 按L2Cache阈值切分为多个Tile
        baseTileLen_ = L2_TILE_THRESHOLD / (3 * sizeof(half));
        tileNum_ = Ceil(totalLen_, baseTileLen_);
    }
}
```

## 编译运行

- 配置环境变量

  请根据当前环境上CANN开发套件包的安装方式配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 编译

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..
  make -j
  ```

  编译后将生成两个可执行文件：
  - `demo_split`：正例，开启L2 Cache切分
  - `demo_nosplit`：反例，未开启L2 Cache切分

  编译选项说明：

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认） | 运行模式：NPU运行 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU架构：dav-2201 对应 Atlas A2/A3 系列，dav-3510 对应 Ascend 950 系列 |

- 生成测试数据并运行

  ```bash
  python3 ../scripts/gen_data.py    # 生成测试输入数据
  ./demo_split                      # 运行正例
  ./demo_nosplit                    # 运行反例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin  # 验证精度
  ```

## 性能采集与分析

### 使用 msopprof 采集性能

```bash
# 采集正例性能
msopprof --output=./prof_split ./demo_split

# 采集反例性能
msopprof --output=./prof_nosplit ./demo_nosplit
```

采集后会在当前目录生成 `OPPROF_*` 目录，文件结构如下：

```
OPPROF_<timestamp>/
├── OpBasicInfo.csv              # 算子基本信息和Task Duration
├── PipeUtilization.csv          # 各Pipeline单元耗时、占比、有效带宽（逐核）
├── L2Cache.csv                  # L2Cache读写命中率（逐核）
├── MemoryL0.csv                 # L0A/L0B/L0C读写带宽
├── Memory.csv                   # UB、L1和主存储器读写带宽
├── MemoryUB.csv                 # Vector和Scalar到UB的读写带宽
├── ArithmeticUtilization.csv    # cube/vector指令cycle占比
└── ResourceConflictRatio.csv    # UB bank group / bank conflict占比
```

### 性能指标说明

本节性能数据在Atlas A2训练系列产品上运行得到，CANN 版本为 CANN 9.1.0。关键指标定义如下：

| 字段名 | 字段含义 | 来源文件 |
|:---:|:---|:---|
| Task Duration(μs) | Task整体耗时，包含调度和在加速器上的执行时间 | OpBasicInfo.csv |
| aiv_time(μs) | Task在AI Vector Core上的执行时间 | PipeUtilization.csv |
| aiv_mte2_time(μs) | mte2类型指令耗时（GM→UB数据搬运），反映数据读入效率 | PipeUtilization.csv |
| aiv_mte2_ratio | mte2指令cycle数在aiv_total_cycles中的占比 | PipeUtilization.csv |
| aiv_mte3_time(μs) | mte3类型指令耗时（UB→GM数据搬运），反映数据写出效率 | PipeUtilization.csv |
| aiv_mte2_active_bw(GB/s) | mte2有效读写带宽，= 读数据量 / mte2耗时。带宽越高说明GM访问效率越高 | PipeUtilization.csv |
| aiv_vec_time(μs) | vec类型指令耗时（向量计算） | PipeUtilization.csv |
| aiv_write_hit_rate(%) | L2Cache写命中率 | L2Cache.csv |
| aiv_read_hit_rate(%) | L2Cache读命中率 | L2Cache.csv |

### 实测性能数据（Atlas A2训练系列）

以下数据采集自 `PipeUtilization.csv`（20核逐核）与 `L2Cache.csv`，汇总后得到。

**表1 整体性能汇总（OpBasicInfo + PipeUtilization）**

| 指标 | 反例（nosplit） | 正例（split） | 加速比 |
|:---|---:|---:|---:|
| Task Duration(μs) | 415.1 | **224.6** | **1.85x** |
| aiv_time(μs) | 402.9 | 215.2 | 1.87x |
| aiv_mte2_time(μs) | 353.9 (87.9%) | 187.4 (87.1%) | **1.89x** |
| aiv_mte3_time(μs) | 111.1 (27.6%) | 53.0 (24.6%) | **2.10x** |
| aiv_vec_time(μs) | 41.2 (10.2%) | 20.6 (9.6%) | 2.00x |

**表2 L2Cache命中率对比（L2Cache 逐核平均值）**

| 指标 | 反例（nosplit） | 正例（split） |
|:---|---:|---:|
| aiv_write_hit_rate | 1.6% | **2.6%** |
| aiv_read_hit_rate | 0.60% | **0.72%** |
| aiv_total_hit_rate | 0.71% | **0.94%** |
| 单时刻L2Cache工作集 | 240000000B/228.88MiB（20核 × 40000000/20 × 3 × 2B） | 最大100MiB（由L2_TILE_THRESHOLD=100×1024²控制，1个Tile总数据量） |

**表3 MTE2有效带宽逐核对比（PipeUtilization blk0）**

| 指标 | 反例（nosplit） | 正例（split） |
|:---|---:|---:|
| aiv_mte2_active_bw | 41.3 GB/s | **57.9 GB/s** |
| aiv_mte2_time (blk0) | 361.2 μs | 128.8 μs |
| aiv_mte2_ratio (blk0) | 89.0% | 84.8% |

### 性能差异分析

**1. MTE2搬运效率是核心差异**

反例中20个核各自处理独立数据，同时刻L2Cache工作集为240000000B（228.88MiB），超出192MiB容量，频繁触发Cache miss重读GM。正例将数据按L2Cache容量切为3个Tile，每Tile最大100MiB（1个Tile的x、y、z三份数据总大小 = L2_TILE_THRESHOLD），完整放入L2Cache：

- 正例 L2Cache 命中率（0.94%）比反例（0.71%）高 32%
- MTE2 有效带宽从 41.3 GB/s 提升至 57.9 GB/s（+40%）
- MTE2 绝对耗时从 353.9 μs 降至 187.4 μs（-47%）

**2. 正例三阶段逐Tile分解（PipeUtilization 逐核拆解）**

| Tile | 数据范围 | per-core 数据量 | 单核 mte2 耗时 | 单核 aiv_time |
|:---|---:|---:|---:|---:|
| Tile 0 | [0, 17.5M) | 0.87M half | ~129 μs | ~152 μs |
| Tile 1 | [17.5M, 34.9M) | 0.87M half | ~190 μs | ~215 μs |
| Tile 2 | [34.9M, 40M) | 0.25M half | ~139 μs | ~163 μs |

三个Tile之间通过 `PipeBarrier<PIPE_ALL>()` 同步后依次处理，所有20核均参与每个Tile。Tile 2 数据量较小但因同步开销，单核 aiv_time 仍约163 μs。总 aiv_time 取所有核中最慢核的累积时间（即约215 μs）。

**3. 反例的 L2Cache Thrashing 机制**

反例中 `LOOP_COUNT=2` 将同一份数据重复计算两次（模拟训练中 data reuse 场景）。每次迭代核内 `TILE_NUM=2` 交替处理前后两段数据：

```
第一轮: [Tile0 → Tile1]  → L2Cache 中最后缓存 Tile1
第二轮: [Tile0 → Tile1]  → 回到 Tile0 时 L2Cache 中已是 Tile1，Cache miss
```

`PipeBarrier` 确保 tile 间同步后数据一致性正确，但第二轮中 Tile0 数据必须从 GM 重新加载。所有20核同时忙，造成 L2Cache Thrashing。

**4. 理论加速来源**

L2Cache 读写混合带宽约 7 TB/s，而 GM 带宽约 1.6 TB/s，相差约 4.4x。实测加速比 1.85x 低于理论值的原因：

- 正例引入了 3 次 `PipeBarrier<PIPE_ALL>` 的同步开销
- L2Cache 命中率虽有提升（0.94% vs 0.71%）但绝对值仍很低——这是 Add 操作"只读一次"的流式访问特征决定的，数据仅在递归/重复使用时才产生 Cache 命中
- 正例的 MTE2 带宽（57.9 GB/s）仍未达到 L2Cache 理论带宽，受限于 MTE2 流水线深度和 Block 维度

## 原理图示

**图1** 未开启L2 Cache切分（反例）

数据按核切分，各核从GM→L2Cache→UB搬运。当外层循环回到首轮数据时，L2Cache中缓存的是末尾数据，需从GM重新加载：

```
Core 0: [Tile0→Tile1→Tile0→Tile1]  ← Tile0第2次访问时L2中已是Tile1
Core 1: [Tile0→Tile1→Tile0→Tile1]
 ...

时间线: Core各自独立处理 → L2Cache频繁换入换出 → 带宽低效
```

**图2** 开启L2 Cache切分（正例）

先按L2Cache容量切分为全局Tile，所有核协同处理同一Tile，处理完毕再进入下一Tile：

```
Tile0: Core0|Core1|...|CoreN  → L2Cache中全量持有Tile0数据
Tile1: Core0|Core1|...|CoreN  → L2Cache中全量持有Tile1数据

时间线: 所有核同步 → L2Cache高效利用 → 带宽提升
```
