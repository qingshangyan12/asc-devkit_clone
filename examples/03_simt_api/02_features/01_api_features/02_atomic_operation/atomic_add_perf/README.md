# asc_atomic_add原子加性能样例

## 概述

本样例以`asc_atomic_add()`原子加为例，在相同原子加总次数下，展示内存层级（Global Memory / Unified Buffer）、数据类型（int32 / int64）、以及是否使用返回值对原子加性能的影响，并给出相应的优化指导。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── atomic_add_perf
│   ├── CMakeLists.txt         // 样例构建脚本
│   ├── atomic_add_perf.asc    // 样例代码
│   ├── README.md
│   └── README_en.md
```

## 样例描述

本样例通过`SCENARIO_NUM`选择一个待测场景，每次运行只启动一个目标Kernel。6个场景两两配对，形成3组对照。

**场景清单**：每个场景由「内存位置 + 数据类型 + 是否使用返回值」三个属性确定。

| 场景编号 | 内存位置 | 数据类型 | 是否使用返回值 |
|:---:|:---:|:---:|:---:|
| 1 | Global Memory | int32_t | 是 |
| 2 | Global Memory | int32_t | 否 |
| 3 | Global Memory | int64_t | 是 |
| 4 | Global Memory | int64_t | 否 |
| 5 | Unified Buffer | int32_t | 是 |
| 6 | Unified Buffer | int32_t | 否 |

**3组对照**：每组只改变表中一个属性、其余保持一致，从而单独对比该属性对性能的影响。

| 对照维度 | 参与场景 | 唯一变化的属性 | 对应下文 |
|---|---|---|:---:|
| 内存位置 | 场景1 ↔ 场景5 | Global Memory → Unified Buffer | Case 1 |
| 是否使用返回值 | 场景1 ↔ 场景2（int32）<br>场景3 ↔ 场景4（int64） | 使用返回值 → 不使用返回值 | Case 2 |
| 数据类型 | 场景1 ↔ 场景3 | int32_t → int64_t | Case 3 |



## 样例实现

本节先说明6个场景共用的计算逻辑与性能指标，再以三组对照（Case 1~3）逐一分析内存层级、返回值、数据类型对原子加性能的影响。每组对照只改变一个属性、其余保持一致，并给出对应的`msprof op`实测数据与优化建议。

### 计算逻辑

 每个ThreadBlock拥有`SLOTS`个计数器（`SLOTS`为编译期可配的计数器个数），Block内每个线程按`线程号 % SLOTS`选定其中一个计数器，循环做`ITERATIONS`次原子加。因此原子加总次数为：

```text
TOTAL_ATOMIC_ADDS = BLOCK_NUM * THREADS_PER_BLOCK * ITERATIONS
```

GM与UB两种场景下线程到计数器的映射完全一致，竞争程度严格相同，**唯一变量是计数器所在的内存层级**：

- **GM场景**：所有Block的计数器统一放在GM上，排成`BLOCK_NUM × SLOTS`的数组，每个Block使用其中属于自身的一段。
- **UB场景**：每个Block将自身的`SLOTS`个计数器放在各自的UB上，并在kernel结束前将本Block的计数值写回GM。

两种场景均在host侧逐个计数器校验计数值，并校验总和等于`TOTAL_ATOMIC_ADDS`。

**竞争程度**由「每个计数器被多少线程共用」决定，即`SLOTS`与`THREADS_PER_BLOCK`的关系。`SLOTS >= THREADS_PER_BLOCK`时每个线程独占一个计数器（无同地址竞争）；`SLOTS < THREADS_PER_BLOCK`时每个计数器被`ceil(THREADS_PER_BLOCK / SLOTS)`个线程争用。下文性能数据取`SLOTS = THREADS_PER_BLOCK = 256`，即每个线程独占一个计数器、无同地址竞争的基准场景。

**带返回值场景**：每次`asc_atomic_add()`返回的旧值写入线程本地`volatile`变量，防止返回值路径被编译器优化掉，同时避免引入额外累加和GM写回。

### 性能指标说明

| 指标                | 说明                                                                                      |
| ------------------- | ----------------------------------------------------------------------------------------- |
| Task Duration（μs）   | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间                  |

> **采集配置说明：** 下文性能数据统一采用`BLOCK_NUM=48`、`THREADS_PER_BLOCK=256`、`ITERATIONS=256`、`SLOTS=256`（`total_atomic_adds=3145728`）。此时每个线程独占一个计数器，**无同地址原子竞争**，GM与UB的线程映射完全一致，从而可作为同竞争程度下的公平对比。

### Case 1：内存层级——GM原子加vsUB原子加

**样例目标**：相同原子加总次数下，对比原子操作打在Global Memory与Unified Buffer上的性能差异。

**核心实现**：通过`SCENARIO_NUM`取值1（GM int32带返回值）和5（UB int32带返回值）分别启动对应kernel。`SLOTS=256`下每线程独占一个计数器，无同地址竞争，唯一差异是计数器所在内存层级。

**性能数据**：

| 场景 | 内存 | Task Duration（μs） |
|:---:|:---:|---:|
| 1 | Global Memory | 2539.92 |
| 5 | Unified Buffer | 59.03 |

**分析**：

- 在竞争程度完全一致（每线程独占一个计数器）、无同地址争用的前提下，GM场景约2540μs，UB场景约59μs，相差约 **43倍**。该差异仅由内存层级引起，不受竞争范围影响。


**结论**：可在UB完成的原子累加，不宜放到GM上执行。

---

### Case 2：返回值开销

**样例目标**：对比`asc_atomic_add()`使用返回值（读回旧值）与不使用返回值的性能差异。

**核心实现**：通过`SCENARIO_NUM`取值1/2（GM int32）、3/4（GM int64）、5/6（UB int32）成对对照，成对场景仅在「是否使用返回值」上不同。

**性能数据**：

| 数据类型 | 带返回值（μs） | 不带返回值（μs） | 带返回值/不带返回值比值 |
|:---:|---:|---:|:---:|
| GM int32（场景1 vs 2） | 2539.92 | 48.97 | 51.9× |
| GM int64（场景3 vs 4） | 1362.38 | 1377.40 | 0.989× |
| UB int32（场景5 vs 6） | 59.03 | 57.79 | 1.02× |

**分析**：

- **GM int32**：不使用返回值时硬件会优化，比带返回值快约52倍。
- **UB int32**：不使用返回值也会有硬件优化，但UB原子本身就很快，带/不带返回值基本持平（约1.02倍）。
- **GM int64**：无硬件优化，带/不带返回值耗时基本持平。

**结论**：非必要业务场景，不要使用接口的返回值；并且优先使用int32，而不是int64。

---

### Case 3：数据类型——int32与int64（同cacheline排队）

**样例目标**：对比int32与int64原子加的性能。

**核心实现**：通过`SCENARIO_NUM`取值1（GM int32带返回值）与3（GM int64带返回值）对照，两者线程映射完全相同（每Block 256线程按`线程号 % 256`各占一个计数器，无同地址竞争），计数器均按元素紧密排布。

**性能数据**：

| 数据类型 | Task Duration（μs） | 每128B cacheline容纳计数器数 | 同cacheline线程数 |
|:---:|---:|:---:|:---:|
| int32（场景1） | 2539.92 | 128B/4B = 32 | 32 |
| int64（场景3） | 1362.38 | 128B/8B = 16 | 16 |

**分析**：

- 与直觉相悖：即使每个线程独占一个计数器、**不存在两个线程修改同一计数器的情况**，窄类型int32仍比宽类型int64慢约 **1.86倍**（2539.92 vs 1362.38）。
- 根因在于 **同一条cacheline上的原子操作需要排队**：GM原子操作以一条**128B cacheline**为最小串行化单位，落在同一条cacheline上的原子操作必须排队逐笔执行，即使它们修改的是各自独立、彼此不冲突的地址。int32每条cacheline可容纳32个计数器，int64仅容纳16个，因此int32每条cacheline上排队的线程数恰为int64的2倍，耗时也近似翻倍。
- 该现象与int32类型本身无关，仅源于紧密排布下每条cacheline容纳的计数器数量更多。

**结论**：不应凭「类型更窄则更快」的直觉为紧密排布的 GM 原子目标选择 int32。即使各线程访问的地址互不冲突，窄类型每条cacheline容纳的数据更多，同一条cacheline上排队的原子操作也更多，反而更慢。可通过为数据添加间距（padding），使每条cacheline只放置更少的数据来缓解。

---

## 性能对比总结

**全场景Task Duration汇总**（`BLOCK_NUM=48`、`THREADS_PER_BLOCK=256`、`ITERATIONS=256`、`SLOTS=256`，`total_atomic_adds=3145728`，无同地址竞争）：

| 场景 | 内存 | 数据类型 | 返回值 | Task Duration（μs） |
|:---:|:---:|:---:|:---:|---:|
| 1 | Global Memory | int32 | 是 | 2539.92 |
| 2 | Global Memory | int32 | 否 | 48.97 |
| 3 | Global Memory | int64 | 是 | 1362.38 |
| 4 | Global Memory | int64 | 否 | 1377.40 |
| 5 | Unified Buffer | int32 | 是 | 59.03 |
| 6 | Unified Buffer | int32 | 否 | 57.79 |

**三组对照收益**：

| 优化维度 | 关键对照 | 收益量级 | 建议 |
|---|---|---|---|
| 内存层级 | UB（59μs） vs GM（2540μs） | ~43× | 原子累加在UB上执行 |
| 返回值 | int32不使用返回值（49μs） vs带返回值（2540μs） | ~52× | 非业务需要，不使用返回值|
| 数据类型 | int32紧密排布（2540μs） vs int64紧密排布（1362μs） | ~0.5× | 不应仅按类型宽窄推断原子性能，紧密布局下窄类型同cacheline排队更多 |

## 调优建议

1. **优先降低原子操作的内存层级**：可在UB中完成的原子累加不宜放到GM。
2. **纯计数场景不使用返回值**：非必要业务场景，不要使用接口的返回值；并且优先使用int32，而不是int64，因为int32在不使用返回值的时候会进行优化，int64则没有。
3. **不应以数据类型宽窄推断原子性能**：性能取决于每条cacheline容纳的原子操作的目标个数，而非类型本身。即使各线程访问的地址互不冲突，紧密布局下窄类型每条cacheline容纳更多数据，同一条cacheline上排队的原子操作也更多，反而更慢。高竞争或紧密布局场景可为数据添加间距（padding），使每条cacheline放置少数原子操作的目标。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 样例执行

  编译并运行场景1：

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..
  make -j
  ./atomic_add_perf
  ```

  编译选项说明：

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM` | `1`-`6` | 待测场景编号 |
  | `BLOCK_NUM` | 正整数 | ThreadBlock数量 |
  | `THREADS_PER_BLOCK` | 正整数 | 每个ThreadBlock中的线程数 |
  | `ITERATIONS` | 正整数 | 每个线程执行的原子加次数 |
  | `SLOTS` | 正整数 | 每个Block的计数器个数，默认`1024`。`SLOTS < THREADS_PER_BLOCK`时同一个计数器会被多个线程共用（产生同地址竞争），共用线程数为`ceil(THREADS_PER_BLOCK / SLOTS)`；`SLOTS >= THREADS_PER_BLOCK`时每个线程独占一个计数器，无同地址竞争 |
  | `SKIP_VALIDATION` | `ON`/`OFF` | 是否跳过结果校验，默认`OFF`。使用`msprof op`采集GM场景时建议设为`ON` |

  执行结果如下，说明正确性校验通过。

  ```text
  Scenario 1: GM int32 with return value
  Workload: blocks=48 threads_per_block=64 iterations=256 slots=1024 total_atomic_adds=786432
  Validation passed
  ```
## 性能分析

使用`msprof`工具获取详细性能数据：

```bash
msprof op ./atomic_add_perf   # 分析case的性能
```

命令完成后，会在默认目录下生成以“OPPROF_{timestamp}_XXX”命名的文件夹，性能数据文件夹结构示例如下：

```text
├──dump                       # 原始的性能数据，用户无需关注
├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
├──L2Cache.csv                # L2 Cache命中率
├──Memory.csv                 # UB，L1和主存储器读写带宽速率
├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
├──OpBasicInfo.csv            # 算子基础信息
├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
├──ResourceConflictRatio.csv  # UB上的bank group、bank conflict和资源冲突率在所有指令中的占比
└──visualize_data.bin         # MindStudio Insight呈现文件
```

> **关于GM场景的Validation failed：** GM场景的计数器数组仅在host侧分配时清零一次，kernel只累加、不在入口清零。`msprof op`的warmup+replay会在同一块GM内存上重复执行kernel，counter被累加多次，因此严格校验模式下会报`Validation failed`（计数为期望值的整数倍）。这是replay机制与校验逻辑的固有冲突，**不影响profiling数据采集**，metric仍正常落盘。采集GM场景时建议通过`-DSKIP_VALIDATION=ON`跳过校验。UB场景的计数器数组在kernel入口自清零，对replay幂等，校验仍passed。
