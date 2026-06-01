# gridDim配置性能调优样例

## 概述

本样例以Gather计算为例，介绍Ascend C SIMT编程方式下的gridDim配置调优思路。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── grid_dim_config
│   ├── figures                // README中的图片资源
│   ├── CMakeLists.txt         // 编译工程文件
│   └── grid_config.asc        // Ascend C算子实现 & 调用样例
```

## 样例描述

- 计算公式：

  $$
  output[i][j] = input[index[i][j]]
  $$

  - input为输入张量，形状为[2100000]，数据类型为float
  - index为索引张量，数据类型为uint32，索引范围为[0,2100000)；大shape为[1024,2048]，中shape为[64,2048]，小shape为[8,2048]
  - output为输出张量，数据类型为float；大shape为[1024,2048]，中shape为[64,2048]，小shape为[8,2048]
  - 计算过程：根据index[i][j]索引从input中采集对应位置的数据，写入output[i][j]位置

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Gather</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[2100000]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">index</td><td align="center">大shape: [1024,2048]<br>中shape: [64,2048]<br>小shape: [8,2048]</td><td align="center">uint32</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">大shape: [1024,2048]<br>中shape: [64,2048]<br>小shape: [8,2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">empty_kernel / gather_naive_kernel / gather_strided_kernel</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例分为4类Case，先建立调度基线，再分别观察大shape、中shape和小shape下的gridDim选择。

| Case   | Shape / 规模                      | 核函数                                  | 对比目的                                  |
| ------ | --------------------------------- | --------------------------------------- | ----------------------------------------- |
| Case 0 | 大shape，[1024,2048]=2097152      | empty_kernel                            | 对比`gridDim`为1 / 32 / 64 / 128 / 1024时的空核调度开销 |
| Case 1 | 大shape，[1024,2048]=2097152      | gather_naive_kernel / gather_strided_kernel | 对比`gridDim`为1024 / 64 / 32时的性能差异 |
| Case 2 | 中shape，[64,2048]=131072         | gather_strided_kernel                   | 对比`gridDim`为16 / 32 / 64 / 128时的性能差异 |
| Case 3 | 小shape，[8,2048]=16384           | gather_strided_kernel                   | 对比`gridDim`为4 / 8 / 16 / 32 / 64时的性能差异 |

### 性能指标说明

| 指标                | 说明                                                                                      |
| ------------------- | ----------------------------------------------------------------------------------------- |
| Task Duration(us)   | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间                  |

### Case 0: 空核函数调度基线

**样例目标**：用空核函数建立不同gridDim下的Task调度耗时基线。

**核心实现**：

- 核函数`empty_kernel`函数体为空，仅承担Task调度开销
- 通过`SCENARIO_NUM=1~5`控制`<<<...>>>`设置五种gridDim配置：1 / 32 / 64 / 128 / 1024
- Host侧先通过`aclrtMemcpy(DEVICE_TO_DEVICE)`把input拷贝到output，保证精度校验通过，再启动空kernel仅测调度开销

**关键代码**：

```cpp
__global__ __launch_bounds__(MAX_THREAD_NUM) void empty_kernel(
    float* input, uint32_t* index, float* output,
    uint32_t input_total_length, uint32_t index_total_length)
{
}

// gridDim 由 SCENARIO_NUM 选择：1 / 32 / 64 / 128 / 1024，每block线程数固定2048
empty_kernel<<<blocks_per_grid, MAX_THREAD_NUM, 0, stream>>>(
    input_device, index_device, output_device,
    input_total_length, index_total_length);
```

**性能数据**：

<img src="figures/fig0_case0_empty_curve.png" alt="Case 0 空核调度基线 Task Duration 随核数变化" width="600" />


**分析**：

由于硬件限制，一个物理核同一时刻只能驻留并执行一个block，每个block的启动和调度执行都会引入固定开销。因此在空核场景下，`gridDim`配置得越大，需要启动和调度的block越多，Task Duration中体现出来的调度开销也越大。本样例实测环境的物理核数为64，可参考`grid_config.asc`中的`query_aiv_core_num`接口查询。

从上图可以看出，随`gridDim`从1增加到1024，空核调度耗时呈"先平缓、后陡增"的两段式变化，拐点大致出现在物理核数64附近：

- **1 → 32 → 64**：`gridDim`不超过物理核数64，需要启动和调度的block数量较少，调度固定开销增长有限，Task Duration从1.221us缓慢增加到2.173us，曲线整体平缓。
- **64 → 128 → 1024**：`gridDim`超过物理核数64后，block数量继续增加，多block启动和调度的固定开销开始明显累积，Task Duration从2.173us增加到3.141us，并进一步上升到16.422us。

由此可总结出两条规律：

1. **`gridDim`不超过物理核数时，调度耗时增长平缓**：`gridDim`为1、32、64时block数量较少，启动调度固定开销有限，Task Duration仅从1.221us缓慢增加到2.173us。
2. **`gridDim`超过物理核数后，调度固定开销明显累积**：`gridDim`从128增加到1024时，需要启动和调度的block数量大幅增加。由于空核本身几乎不做计算，此时Task Duration主要体现为多block启动和调度带来的固定开销。

---

### Case 1: 大shape场景，Gather配置不同gridDim性能对比

**样例目标**：对比同一大shape`1024*2048=2097152`下执行Gather操作，配置不同gridDim的性能差异。

#### 单线程单数据

**核心实现**：

- 通过`SCENARIO_NUM=6`控制`<<<...>>>`设置gridDim为1024，执行单线程单数据场景，核函数`gather_naive_kernel`中**每个线程只处理1个元素**，用全局线程号`blockIdx.x * blockDim.x + threadIdx.x`定位唯一一个元素，从GM上的`index[]`取下标，再从GM上的`input[]`读取数据写入`output[]`
- gridDim配置为1024、每block 2048线程，`gridDim.x * blockDim.x`恰好等于元素总数，每个线程处理1个元素

**关键代码**：

```cpp
// 基础写法：每个线程只处理1个元素，无for循环
uint32_t global_idx = blockIdx.x * blockDim.x + threadIdx.x;

uint32_t gather_idx = index[global_idx];
if (gather_idx >= input_total_length) { gather_idx = 0; }
output[global_idx] = input[gather_idx];
```

**性能数据**：

| Task Duration(us) |
| :---------------: |
|      78.289       |

#### 单线程多数据

**优化目标**：在gather语义不变的前提下，改用"grid-stride循环"写法——**每个线程处理多个元素（单线程多数据）**，通过减少gridDim的值，来减少调度开销。

**核心实现**：

- 通过`SCENARIO_NUM=7/8`控制`<<<...>>>`设置两种gridDim配置：64 / 32，执行单线程多数据场景，核函数`gather_strided_kernel`每个线程从全局编号`global_idx`起步，以总线程数`gridDim.x * blockDim.x`为步长循环处理多个元素
- 每block线程数固定为硬件上限2048；`gridDim`动态配置：
  - `gridDim=64`：总线程数`64 * 2048 = 131072`，每个线程循环`2097152 / 131072 = 16`次
  - `gridDim=32`：总线程数`32 * 2048 = 65536`，每个线程循环`2097152 / 65536 = 32`次

**关键代码**：

```cpp
uint32_t global_idx = blockIdx.x * blockDim.x + threadIdx.x;
uint32_t stride = gridDim.x * blockDim.x;

for (uint32_t i = global_idx; i < index_total_length; i += stride) {
    uint32_t gather_idx = index[i];
    if (gather_idx >= input_total_length) { gather_idx = 0; }
    output[i] = input[gather_idx];
}
```

**性能数据**：

| gridDim | Task Duration(us) |
| :-----: | :---------------: |
|   64    |      58.583       |
|   32    |      110.540      |

#### Case 1 三种场景性能对比

| 写法 | gridDim | Task Duration(us) | 说明 |
| :--: | :-----: | :---------------: | ---- |
| 单线程多数据 | 32 | 110.540 | `gridDim`偏小，并行核数不足 |
| **单线程多数据** | **64** | **58.583** | **`gridDim`匹配物理核数，最优** |
| 单线程单数据 | 1024 | 78.289 | `gridDim`过大，多block调度开销高 |

<img src="figures/fig3_case1_large_curve.png" alt="Case 1 大shape(2097152) Task Duration 随核数变化" width="600" />

**分析**：

- **`gridDim=64`相比`gridDim=1024`的单线程单数据写法，Task Duration从78.289us降低到58.583us，耗时下降约25.2%，性能提升约1.34倍**。Gather读写总量和每block线程数均不变，提升主要来自`gridDim`从1024降到64后，启动和调度的block数量显著减少
- **`gridDim=32`反而最慢，110.540us，比`gridDim=64`慢约88.7%、几乎翻倍**。同样的单线程多数据写法，把`gridDim`从64进一步降到32后，只用到一半物理核，参与并行的核数减半、每个block承担的工作量翻倍，而调度开销在`gridDim`为32和64时相差无几（见Case 0：`gridDim=32`为1.557us、`gridDim=64`为2.173us），耗时增加主要来自并行核数不足
- 更关键的是：**`gridDim=32`（110.540us）慢于`gridDim=1024`（78.289us）**。这说明大shape场景下，`gridDim`过小导致并行核数不足、单核工作量过重，比`gridDim`过大带来的多block调度开销影响更大

**结论**：大shape下应优先让`gridDim`匹配实际物理核数。本样例中`gridDim=64`既避免了`gridDim=1024`带来的大量block调度开销，又避免了`gridDim=32`并行核数不足、单核工作量过重，因此是Case 1中的最优配置。

---
### Case 2: 中shape场景，Gather配置不同gridDim性能对比

**样例目标**：选取一个中等规模的shape`64*2048=131072`，对比该shape下执行Gather操作，配置不同gridDim的性能差异。

**核心实现**：

- 通过`SCENARIO_NUM=9/10/11/12`控制`<<<...>>>`设置四种gridDim配置：16 / 32 / 64 / 128，核函数与Case 1的单线程多数据写法完全相同，仅将shape调整为`64*2048=131072`个元素
- 每核处理的元素数量 = `131072 / gridDim`：16核8192、32核4096、64核2048、128核1024。相比小shape的几百元素，单核工作量仍然可观
- 每block线程数按每核实际元素数自适应：16 / 32 / 64核每核元素数均≥2048，线程数取上限2048；128核每核仅1024元素，线程数自适应降到1024

**性能数据**：

| gridDim | Elements Per Block | Threads Per Block | Task Duration(us) | 说明                       |
| :-----: | :----------------: | :---------------: | :---------------: | -------------------------- |
|   16    |        8192        |       2048        |      17.644       | 核数偏少，单核工作量重     |
|   32    |        4096        |       2048        |      10.907       | 增加核数仍有明显收益       |
| **64**  |      **2048**      |     **2048**      |     **8.629**     | **最优，`gridDim`匹配物理核数** |
|   128   |        1024        |       1024        |      10.964       | 超过物理核数，耗时上升      |

<img src="figures/fig1_case2_131072_curve.png" alt="Case 2 中shape(131072) Task Duration 随核数变化" width="600" />


**分析**：
- 16 → 32 → 64：核数翻倍时Task Duration持续下降（17.644 → 10.907 → 8.629）。数据量较大、每核工作量充足（≥2048元素），在`gridDim`达到物理核上限前，增加并行核数的收益始终明显
- `gridDim`从64增加到128后，Task Duration从8.629us升到10.964us，**耗时上升约27.1%**。此时需要启动和调度的block数量继续增加，多block调度固定开销开始影响整体耗时

**结论**：中等规模shape同样优先使用`gridDim=64`，即让`gridDim`匹配物理核数；继续增大`gridDim`收益有限，反而会引入更多block调度开销。

---

### Case 3: 小shape场景，Gather配置不同gridDim性能对比

**样例目标**：选取一个小规模shape`8*2048=16384`，对比该shape下执行Gather操作，配置不同gridDim的性能差异。

**核心实现**：

- 通过`SCENARIO_NUM=13/14/15/16/17`控制`<<<...>>>`设置五种gridDim配置：4 / 8 / 16 / 32 / 64，核函数与Case 1的单线程多数据写法完全相同，仅将shape调整为`8*2048=16384`个元素
- 每核处理的元素数量 = `16384 / gridDim`：4核4096、8核2048、16核1024、32核512、64核256
- 每block线程数按每核实际元素数自适应：4 / 8核每核元素数≥2048，线程数取上限2048；16 / 32 / 64核每核元素数小于2048，线程数自适应降到每核实际元素数（1024 / 512 / 256），消除空转线程

**性能数据**：

| gridDim | Elements Per Block | Threads Per Block | Task Duration(us) | 说明                     |
| :-----: | :----------------: | :---------------: | :---------------: | ------------------------ |
|    4    |        4096        |       2048        |      10.831       | 核数不足，单核工作量重   |
|    8    |        2048        |       2048        |       6.569       | 增加核数仍有明显收益     |
|   16    |        1024        |       1024        |       4.500       | 核数偏少，单核工作量偏重 |
| **32**  |       **512**      |      **512**      |     **3.696**     | **最优核数，Task Duration最低** |
|   64    |        256         |        256        |       4.133       | 使用全部物理核，调度固定开销超过收益 |

<img src="figures/fig2_case3_16384_curve.png" alt="Case 3 小shape(16384) Task Duration 随核数变化" width="600" />


**分析**：

- 折线呈"先降后升"的趋势，**最优核数为32（物理核数的一半）**，并非使用全部物理核的64核
- 4 → 8 → 16 → 32：核数增加时Task Duration持续下降（10.831 → 6.569 → 4.500 → 3.696）。小核数下单核工作量过重，增加并行核数的收益明显
- 32 → 64：核数翻倍，Task Duration从3.696us升到4.133us，**耗时上升约11.8%**。此时每核工作量已很小、有效计算时间短，新增核数带来的调度开销反而超过并行收益
- 与Case 2的131072对照：同样的kernel，shape小到一定程度后最优核数从64退回到32，说明"最优核数"随shape缩小而变化，必须实测

**结论**：小shape无法直接沿用"使用全部物理核"的经验，必须参考本Case的方式设置多档核数实测。数据量越小，最优核数越小于物理核数，本样例环境下16384的最优核数为32。

---

## 性能对比总结

各Case的性能数据见前述对应表格，本节汇总跨Case的横向规律：

1. **`gridDim`与物理核数的匹配是gridDim调优的核心**：`gridDim`远超物理核数时，会启动和调度过多block，调度固定开销明显累积（Case 0：`gridDim=1024`调度基线16.422us，约为`gridDim=64`的2.173us的7.6倍）；`gridDim`过小时，并行核数不足、单block工作量增加（Case 1：`gridDim=32`为110.540us，慢于`gridDim=1024`的78.289us）。

2. **最优`gridDim`随shape缩小而下移，必须实测**：同一grid-stride写法下，大 / 中shape的最优`gridDim`是物理核数（Case 1/2均为64）；但小shape的最优`gridDim`退回到物理核数的一半（Case 3为32，32→64耗时上升约11.8%）。数据量越大，最优`gridDim`越靠近物理核上限；数据量越小，越偏离物理核上限。因此小shape不能直接沿用"使用全部物理核"的经验，应设置多档`gridDim`用msprof Task Duration确定最低点。

3. **线程数应按每核实际元素数自适应**：每block线程数不必固定在硬件上限2048。当每核处理的元素数小于2048时，固定2048线程会让多出的线程在stride循环边界外完全空转。样例的`resolve_thread_num`默认把线程数取为`min(2048, 每核元素数)`：每核元素数≥2048时取2048（如Case 2的16/32/64核、Case 3的4/8核），小于2048时降到实际值（如Case 2的128核→1024，Case 3的16/32/64核→1024/512/256），消除空转线程。


## 调优建议

1. **大shape / 中shape，优先让`gridDim`匹配物理核数**：`gridDim`过小会导致并行核数不足、单block工作量变重；`gridDim`过大则会引入更多block启动和调度开销。
2. **小shape需实测核数性能**：当数据量较小、每核工作量很小时，启动更多核带来的调度固定开销会超过收益，最优核数不一定是最大物理核数。应参考Case 3的方式设置核数、用msprof Task Duration确定最低点。数据量越大最优核数越靠近物理核数。
3. **线程数按每核实际元素数自适应**：每block线程数不必固定在硬件上限2048。当每核处理的元素数小于2048时，stride循环边界会让多出的线程完全空转，样例的`resolve_thread_num`默认把线程数调到每核实际元素数即可减少线程空转、提升性能（见Case 3的16/32/64核档）。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

  - 默认路径，root用户安装CANN软件包

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包

    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行

  ```bash
  SCENARIO_NUM=7                                                                     # 选择执行场景，可选1-17
  mkdir build && cd build;                                                           # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./grid_config                                                                      # 执行样例
  ```

- 编译选项说明

  | 选项                       | 可选值      | 说明                                                          |
  | -------------------------- | ----------- | ------------------------------------------------------------- |
  | `CMAKE_ASC_ARCHITECTURES`  | `dav-3510`  | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT）    |
  | `SCENARIO_NUM`             | `1`-`17`    | 样例类型，默认为1                                             |

  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy is verification passed.
  ```

## 性能分析

使用`msprof`工具获取详细性能数据：

```bash
msprof op ./grid_config   # 分析case的性能
```

命令完成后，会在默认目录下生成以"OPPROF_{timestamp}_XXX"命名的文件夹，性能数据文件夹结构示例如下：

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
