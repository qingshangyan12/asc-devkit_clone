# L2 Cache切分调优样例

## 概述

本样例使用固定负载对比两种实现：

- 反例no-split：不做L2 Cache切分，每轮把384MB GM数据作为一个整体处理。
- 正例L2-split：按100MB有效工作集阈值切成4个96MB L2 Batch，并在同一Batch上连续完成两轮计算。

样例使用一份384MB的GM数据，每轮在同一Buffer上原地执行`data = data + 1`。每个元素完成两轮计算后，最终结果为`input + 2`。两种实现的逻辑GM->UB搬运量、UB->GM搬运量和Adds计算量相同，差异在于跨轮复用时的L2 Cache工作集大小。

## 支持范围

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：架构为dav-3510，CANN版本不低于9.1.0。
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：架构为dav-2201，CANN版本不低于9.0.0。
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：架构为dav-2201，CANN版本不低于9.0.0。
<!-- end id3 -->

## 目录结构

```text
add_l2_cache_split/
├── add_l2cache_nosplit.asc       // 反例Kernel入口：384MB整体处理
├── add_l2cache_split.asc         // 正例Kernel入口：按L2 Batch切分处理
├── l2_cache_kernel.h             // 固定负载和公共Device处理逻辑
├── l2_cache_runner.h             // 公共Host运行逻辑
├── data_utils.h                  // 二进制文件读写工具
├── CMakeLists.txt                // 双目标编译工程文件
├── scripts/
│   └── gen_data.py               // 生成固定输入
├── README.md                     // 中文说明
└── README_en.md                  // 英文说明
```

## 样例设计

### 固定负载

样例的`data`包含201326592个`half`元素，整份GM数据占384MB。两轮计算每轮都读取并写回整份384MB数据，因此单次Kernel的逻辑GM->UB读量为768MB，逻辑UB->GM写量也为768MB。

```text
逻辑GM->UB读量 = 384MB/轮 * 2轮 = 768MB
逻辑UB->GM写量 = 384MB/轮 * 2轮 = 768MB
```

逻辑GM->UB读量不等于实际GM->L2读量：L2 Cache命中不访问主存储器，只有Cache Miss才会从主存储器取数并填充L2 Cache。

### 反例no-split

反例不做L2 Cache切分，每轮直接处理完整`data[0, 384MB)`范围：

```text
Round 0: data[0, 384MB)
Round 1: data[0, 384MB)
```

虽然每个核内部每次只搬运一个UB Tile，但一轮会持续访问新的GM地址段，直到覆盖384MB末尾。384MB超过100MB有效工作集阈值，Round 0早期访问的数据可能在后续扫描中被替换出L2 Cache。Round 1重新访问起始地址范围时，更可能发生Cache Miss并从GM重新读取。

### 正例L2-split

正例以100MB作为单个L2 Batch的有效工作集阈值，先向上取整得到4个L2 Batch，再将数据等分为每批96MB：

```text
l2BatchNum       = ceil(384MB / 100MB) = 4
l2BatchBytes     = 384MB / 4 = 96MB
l2BatchElements  = 96MB / sizeof(half) = 50331648
```

B0至B3不是额外申请的Buffer，而是单个`data`上的四个连续索引区间。正例执行顺序如下：

```text
Batch B0: Round 0 -> Round 1
Batch B1: Round 0 -> Round 1
Batch B2: Round 0 -> Round 1
Batch B3: Round 0 -> Round 1
```

同一批连续完成两轮计算，中间没有其他大数据范围插入，因此第2轮更有机会从L2 Cache读取第1轮结果。

### 分核和UB Tile

公共Device逻辑按32字节DataBlock切分数据范围，再分配给20个Vector Core。正例的单个96MB L2 Batch包含3145728个DataBlock，前8个核各处理157287个DataBlock，即2516592个`half`元素；其余12个核各处理157286个DataBlock，即2516576个`half`元素。

反例每轮处理完整384MB范围，20个核共同覆盖`data[0, 384MB)`。两种实现的每个核内部都按`MAX_DATA_COPY_LEN = 16384`个`half`元素切分UB Tile。UB Tile只解决UB容量问题，不表示L2 Cache切分粒度；连续循环访问的新GM地址仍会不断填充并替换L2 Cache。

### 多核同步

反例每处理完一轮完整384MB数据后调用软件`AscendC::SyncAll`。正例每个L2 Batch与Round组合结束后调用`AscendC::SyncAll`，确保所有核完成当前阶段的UB->GM搬运后，再进入下一阶段。同步GM空间按每核32字节申请并初始化为0。

## 测试用例

只保留固定负载下的两个正反例：

**表 1**  正反例测试用例

| 用例 | 入口 | L2 Cache切分 | 活跃工作集 | 数据元素数 | 数据量 | 核数 | 轮数 |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: |
| 反例 | `l2_cache_nosplit` | 不切分 | 384MB | 201326592 | 384MB | 20 | 2 |
| 正例 | `l2_cache_split` | 4个96MB Batch | 96MB | 201326592 | 384MB | 20 | 2 |

两者均使用`input/input.bin`作为输入，并将计算结果写入`output/`目录。性能归因时，建议手动对比Profile中的`GM_to_UB_datas(KB)`，确认正反例逻辑GM->UB搬运量接近。

## 编译运行

在本目录执行以下命令：

```bash
python3 scripts/gen_data.py

cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build -j

./build/demo_nosplit
./build/demo_split
```

`demo_nosplit`和`demo_split`是两个独立进程，不需要使用`--mode`选择Kernel。

`CMAKE_ASC_ARCHITECTURES`支持`dav-2201`和`dav-3510`。实际运行时，编译架构需要与当前NPU架构一致。

运行后输出文件为：

```text
output/no_split.bin
output/l2_split.bin
```

## 性能采集与分析

### 使用msopprof采集性能

分别启动独立进程采集正反例，避免同一进程中前一个Kernel残留的Cache状态影响后一个Kernel：

```bash
msopprof --output=./prof_384m_inplace_no_split_1 \
  ./build/demo_nosplit

msopprof --output=./prof_384m_inplace_l2_split_1 \
  ./build/demo_split
```

正反例各采集5次时，每次使用新的输出目录，并交替安排正反例的采集顺序。例如，两个目录组分别使用`prof_384m_inplace_no_split_1`至`prof_384m_inplace_no_split_5`和`prof_384m_inplace_l2_split_1`至`prof_384m_inplace_l2_split_5`。

采集完成后，用户可手动打开各Profile目录下的`OPPROF_*`报告文件，对比`OpBasicInfo.csv`、`PipeUtilization.csv`、`L2Cache.csv`和`Memory.csv`中的关键指标。正反例目录按采集顺序配对，先确认`GM_to_UB_datas(KB)`总量接近，再对比L2 Cache命中率、MTE2耗时和`Task Duration(us)`。

### Profile报告结构

`msopprof`在输出目录下生成`OPPROF_*`报告，主要文件如下：

```text
OPPROF_<timestamp>/
├── OpBasicInfo.csv               // Kernel基本信息和Task Duration
├── PipeUtilization.csv           // 各流水耗时和有效带宽
├── L2Cache.csv                   // L2 Cache读写命中率
├── Memory.csv                    // GM与UB搬运量和有效带宽
├── MemoryL0.csv                  // L0 Buffer带宽
├── MemoryUB.csv                  // Unified Buffer带宽
├── ArithmeticUtilization.csv     // 计算指令利用率
└── ResourceConflictRatio.csv     // 资源冲突比例
```

### 性能指标说明

**表 2**  性能指标

| 字段名 | 含义 | 汇总方式 | 来源文件 |
| --- | --- | --- | --- |
| `Task Duration(us)` | Kernel任务整体耗时 | 取Kernel记录 | `OpBasicInfo.csv` |
| `aiv_time(us)` | Vector Core执行耗时 | 取所有核最大值 | `PipeUtilization.csv` |
| `aiv_mte2_time(us)` | MTE2搬入耗时 | 取所有核平均值 | `PipeUtilization.csv` |
| `aiv_mte2_active_bw(GB/s)` | MTE2活跃期间有效带宽 | 取所有核平均值 | `PipeUtilization.csv` |
| `aiv_gm_to_ub_bw(GB/s)` | GM->UB平均有效带宽 | 取所有核平均值 | `Memory.csv` |
| `GM_to_UB_datas(KB)` | 逻辑GM->UB搬运量 | 对所有核求和 | `Memory.csv` |
| `read_main_memory_datas(KB)` | 主存储器读取计数 | 对所有核求和 | `Memory.csv` |
| `aiv_read_hit_rate(%)` | L2 Cache读命中率 | 取所有核平均值 | `L2Cache.csv` |
| `aiv_write_hit_rate(%)` | L2 Cache写命中率 | 取所有核平均值 | `L2Cache.csv` |
| `aiv_total_hit_rate(%)` | L2 Cache总命中率 | 取所有核平均值 | `L2Cache.csv` |

`aiv_gm_to_ub_bw(GB/s)`是有效带宽，不是GM读取量。该值在优化后可能升高，不能将“带宽降低”作为优化预期。`read_main_memory_datas(KB)`的统计口径与架构和工具版本相关，需要与L2 Cache命中率、MTE2耗时共同分析。

### 384MB负载实测结果

以下数据于2026-07-29基于当前no-split和L2-split实现重新采集，环境为CANN 9.1.0、`dav-2201`编译架构、NPU模式、Device 0、20个Vector Core、NPU频率1650MHz。实机运行时，`output/no_split.bin`与`output/l2_split.bin`逐字节一致，并且201326592个`half`元素均满足输出为`input + 2`。

本轮Profile开启默认Warm Up，正反例各采集5组，并交替安排采集顺序。采集目录为`prof_384m_inplace_20260729_093359_no_split_1`至`prof_384m_inplace_20260729_093359_no_split_5`，以及`prof_384m_inplace_20260729_093359_l2_split_1`至`prof_384m_inplace_20260729_093359_l2_split_5`。

5组Profile的逻辑GM->UB搬运量差异均小于1%，满足A/B公平性要求。

**表 3**  逐组公平性和加速比

| 组号 | 反例`Task Duration(us)` | 正例`Task Duration(us)` | 逻辑GM->UB差异 | 加速比 |
| --- | ---: | ---: | ---: | ---: |
| 1 | 1439.1888 | 1068.6614 | 0.1123% | 1.3467x |
| 2 | 1440.6688 | 1064.2812 | 0.2153% | 1.3537x |
| 3 | 1435.9087 | 1071.9015 | 0.1046% | 1.3396x |
| 4 | 1440.5088 | 1057.2611 | 0.2550% | 1.3625x |
| 5 | 1437.0487 | 1069.6814 | 0.1018% | 1.3434x |

**表 4**  中位数指标对比

| 指标 | 反例no-split | 正例L2-split | 变化 |
| --- | ---: | ---: | ---: |
| `Task Duration(us)` | 1439.1888 | 1068.6614 | 降低25.75% |
| `aiv_time(us)` | 1438.8497 | 1068.1552 | 降低25.76% |
| `aiv_mte2_time(us)` | 1305.0142 | 899.7586 | 降低31.05% |
| `aiv_mte2_active_bw(GB/s)` | 28.8393 | 41.8707 | 提升45.19% |
| `aiv_gm_to_ub_bw(GB/s)` | 26.1645 | 35.2861 | 提升34.86% |
| `GM_to_UB_datas(KB)` | 789086.3750 | 790067.5000 | 提升0.12% |
| `read_main_memory_datas(KB)` | 789067.6250 | 790046.2500 | 提升0.12% |
| `aiv_read_hit_rate(%)` | 1.1922 | 48.7031 | 提升47.5108个百分点 |
| `aiv_write_hit_rate(%)` | 99.9988 | 99.9444 | 降低0.0544个百分点 |
| `aiv_total_hit_rate(%)` | 21.4666 | 59.2068 | 提升37.7402个百分点 |

本轮实测结论是：正例没有减少逻辑GM->UB搬运量，而是通过把跨轮复用工作集从384MB降低到96MB，提高第2轮读取时的L2 Cache命中概率。正例L2读命中率中位数从1.1922%提升到48.7031%，MTE2搬入耗时中位数从1305.0142us降到899.7586us，`Task Duration(us)`中位数从1439.1888us降到1068.6614us，中位数加速比为1.3467x。

`read_main_memory_datas(KB)`中位数与逻辑GM->UB搬运量一样基本持平，在当前架构和工具统计口径下不能单独作为实际主存储器读量降低的判据。分析时应以逻辑搬运量公平性为前提，结合L2 Cache读命中率、MTE2耗时和整体任务耗时进行归因。Profiler诊断显示该样例Vector计算占比较低，主要收益来自数据搬入路径和访问局部性改善。

## 验收标准

- `dav-2201`和`dav-3510`均编译通过。
- 每组正反例Profile的`GM_to_UB_datas(KB)`差异不超过1%。
- 正例的L2 Cache读命中率高于反例，MTE2耗时和`Task Duration(us)`低于反例。
