# L2 Cache切分调优样例

## 概述

本样例使用固定负载对比两种工作量完全相同、仅L2 Cache级循环顺序不同的实现：

- 反例round-first：`Round -> L2 Batch -> UB Tile`。
- 正例L2-batch-first：`L2 Batch -> Round -> UB Tile`。

样例使用一份384MiB的GM数据，每轮在同一Buffer上原地执行`data = data + 1`。每个元素完成两轮计算后，最终结果为`input + 2`。两种实现的GM->UB搬运量、UB->GM搬运量、Adds计算量和全核同步次数相同，性能差异只来自访问顺序。

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
├── add_l2cache_nosplit.asc       // 反例Kernel入口和循环顺序
├── add_l2cache_split.asc         // 正例Kernel入口和循环顺序
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

样例的`data`包含201326592个`half`元素，整份GM数据占384MiB。以100MiB作为单个L2 Batch的有效工作集阈值时，先向上取整得到4个L2 Batch，再将数据等分为每批96MiB。每个L2 Batch是`data`上一段连续且不重叠的索引范围，包含50331648个`half`元素：

```text
l2BatchNum       = ceil(384MiB / 100MiB) = 4
l2BatchBytes     = 384MiB / 4 = 96MiB
l2BatchElements  = 96MiB / sizeof(half) = 50331648
```

两轮计算每轮都读取并写回整份384MiB数据，因此单次Kernel的逻辑GM->UB读量为768MiB，逻辑UB->GM写量也为768MiB。逻辑GM->UB读量不等于实际GM->L2读量：L2 Cache命中不访问主存储器，只有Cache Miss才会从主存储器取数并填充L2 Cache。

B0至B3不是额外申请的Buffer，而是单个`data`上的四个连续索引区间。每轮都从当前区间搬入UB，完成`Adds`后写回同一GM地址。分批的唯一目的是将一次连续复用的数据范围控制在96MiB，不超过100MiB阈值。

一个L2 Batch包含3145728个32字节DataBlock。使用20个Vector Core时，前8个核各处理157287个DataBlock，即2516592个`half`元素；其余12个核各处理157286个DataBlock，即2516576个`half`元素。每核包含153个完整UB Tile和1个尾部UB Tile，完整Tile容量为16384个元素，尾部Tile分别处理9840或9824个元素。所有搬运长度均保持32字节对齐。

### 反例round-first

假设四个L2 Batch分别为B0至B3，反例执行顺序如下：

```text
Round 0: B0 -> B1 -> B2 -> B3
Round 1: B0 -> B1 -> B2 -> B3
```

Round 0处理B1至B3时，B0可能已被替换出L2 Cache。Round 1再次访问B0时，更可能需要从GM重新读取。

### 正例L2-batch-first

正例只交换外层循环顺序：

```text
Batch B0: Round 0 -> Round 1
Batch B1: Round 0 -> Round 1
Batch B2: Round 0 -> Round 1
Batch B3: Round 0 -> Round 1
```

同一批连续完成两轮计算，中间没有其他大数据批次插入，因此第2轮更有机会从L2 Cache读取第1轮结果。

### 多核同步

每个L2 Batch由20个Vector Core共同处理。每个L2 Batch与Round组合结束后调用软件`AscendC::SyncAll`，确保所有核完成当前阶段的UB->GM搬运后，再进入下一阶段。同步GM空间按每核32字节申请并初始化为0。

## 测试用例

只保留固定负载下的两个正反例：

**表 1**  正反例测试用例

| 用例 | 入口 | 循环结构 | 数据元素数 | 数据量 | 核数 | 批次数 | 轮数 |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: |
| 反例 | `l2_cache_nosplit` | Round优先 | 201326592 | 384MiB | 20 | 4 | 2 |
| 正例 | `l2_cache_split` | L2 Batch优先 | 201326592 | 384MiB | 20 | 4 | 2 |

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

## 性能采集与分析

### 使用msopprof采集性能

分别启动独立进程采集正反例，避免同一进程中前一个Kernel残留的Cache状态影响后一个Kernel：

```bash
msopprof --output=./prof_384m_inplace_round_first_1 \
  ./build/demo_nosplit

msopprof --output=./prof_384m_inplace_l2_batch_first_1 \
  ./build/demo_split
```

正反例各采集5次时，每次使用新的输出目录，并交替安排正反例的采集顺序。例如，两个目录组分别使用`prof_384m_inplace_round_first_1`至`prof_384m_inplace_round_first_5`和`prof_384m_inplace_l2_batch_first_1`至`prof_384m_inplace_l2_batch_first_5`。

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

### 384MiB负载实测结果

原有Profile数据来自不同的数据量或双Buffer实现，不适用于当前单个Buffer、384MiB原地计算负载，不应混入新的性能结论。以下数据于2026-07-28基于当前样例重新采集，环境为CANN 9.1.0、`dav-2201`编译架构、20个Vector Core、NPU频率1650MHz。采集目录为`prof_384m_inplace_20260728_085319_round_first_1`至`prof_384m_inplace_20260728_085319_round_first_5`，以及`prof_384m_inplace_20260728_085319_batch_first_1`至`prof_384m_inplace_20260728_085319_batch_first_5`。

5组Profile的逻辑GM->UB搬运量差异均小于1%，满足A/B公平性要求。

**表 3**  逐组公平性和加速比

| 组号 | 反例`Task Duration(us)` | 正例`Task Duration(us)` | 逻辑GM->UB差异 | 加速比 |
| --- | ---: | ---: | ---: | ---: |
| 1 | 1475.3495 | 1071.5614 | 0.0435% | 1.3768x |
| 2 | 1468.6694 | 1056.6211 | 0.1516% | 1.3900x |
| 3 | 1475.9095 | 1072.4814 | 0.0401% | 1.3762x |
| 4 | 1470.4694 | 1054.2811 | 0.1670% | 1.3948x |
| 5 | 1470.3694 | 1075.2015 | 0.0543% | 1.3675x |

**表 4**  中位数指标对比

| 指标 | 反例round-first | 正例L2-batch-first | 变化 |
| --- | ---: | ---: | ---: |
| `Task Duration(us)` | 1470.4694 | 1071.5614 | 降低27.13% |
| `aiv_time(us)` | 1470.2946 | 1071.1370 | 降低27.15% |
| `aiv_mte2_time(us)` | 1333.5366 | 905.1114 | 降低32.13% |
| `aiv_mte2_active_bw(GB/s)` | 28.2165 | 41.6265 | 提升47.53% |
| `aiv_gm_to_ub_bw(GB/s)` | 25.6037 | 35.1896 | 提升37.44% |
| `GM_to_UB_datas(KB)` | 789575.0000 | 790076.8750 | 提升0.06% |
| `read_main_memory_datas(KB)` | 789592.5000 | 790121.2500 | 提升0.07% |
| `aiv_read_hit_rate(%)` | 1.2606 | 48.7423 | 提升47.4817个百分点 |
| `aiv_write_hit_rate(%)` | 99.9988 | 99.9451 | 降低0.0537个百分点 |
| `aiv_total_hit_rate(%)` | 21.5098 | 59.2343 | 提升37.7245个百分点 |

本轮实测结论是：正例没有减少逻辑GM->UB搬运量，而是通过同一L2 Batch连续完成两轮计算，提高第2轮读取时的L2 Cache命中概率。正例L2读命中率中位数从1.2606%提升到48.7423%，MTE2耗时中位数从1333.5366us降到905.1114us，`Task Duration(us)`中位数从1470.4694us降到1071.5614us，中位数加速比为1.3768x。

`read_main_memory_datas(KB)`中位数与逻辑GM->UB搬运量一样基本持平，在当前架构和工具统计口径下不能单独作为实际主存储器读量降低的判据。分析时应以逻辑搬运量公平性为前提，结合L2 Cache读命中率、MTE2耗时和整体任务耗时进行归因。Profiler诊断显示该样例Vector计算占比较低，主要收益来自数据搬入路径和访问局部性改善。

## 验收标准

- `dav-2201`和`dav-3510`均编译通过。
- 每组正反例Profile的`GM_to_UB_datas(KB)`差异不超过1%。
- 正例的L2 Cache读命中率高于反例，MTE2耗时和`Task Duration(us)`低于反例。
