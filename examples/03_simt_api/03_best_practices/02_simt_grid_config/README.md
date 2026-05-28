# gridDim配置与vf调用优化样例

## 概述

本样例以Gather+Adds融合计算为例，介绍Ascend C SIMD与SIMT混合编程方式下的gridDim配置和vf函数调用优化思路。样例包含7个空壳核函数场景（测试block切换与vf切换头开销）以及3个逐步优化的实际计算场景，从基础场景开始，逐步调整核数与物理核匹配、优化vf循环位置，展示SIMD与SIMT混合编程方式下的性能调优路径。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── 02_simt_grid_config
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── grid_config.asc        // 样例实现
│   ├── figures                // 性能曲线图
│   └── README.md
```

## 样例描述

- 计算公式：

  $$
  output[i][j] = input[index[i][j]] + 1.0
  $$

  - input为输入张量，形状为[1100000]，数据类型为float
  - index为索引张量，形状为[1024, 1024]，数据类型为uint32，索引范围为[0, 1100000)
  - output为输出张量，形状为[1024, 1024]，数据类型为float
  - 计算过程：根据index[i][j]索引从input中采集对应位置的数据，加1.0后写入output[i][j]位置

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">GatherAdds</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[1100000]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">index</td><td align="center">[1024, 1024]</td><td align="center">uint32</td><td align="center">ND</td></tr>
  <tr><td align="center">output</td><td align="center">[1024, 1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">empty_kernel / gather_adds_kernel / gather_adds_kernel_vf_inside</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过10个独立场景展示不同的gridDim配置与vf调用策略。

| Case   | 实现特点                                           | 使用的核函数                          | 优化特性                                |
| ------ | -------------------------------------------------- | ------------------------------------- | --------------------------------------- |
| Case 1 | 36核，1次vf调用                                    | empty_kernel                             | 空壳核函数基线                          |
| Case 2 | 64核（物理核数），1次vf调用                        | empty_kernel                             | 空壳核函数基线                          |
| Case 3 | 128核，1次vf调用                                   | empty_kernel                             | 空壳核函数基线                          |
| Case 4 | 1024核，1次vf调用                                  | empty_kernel                             | 空壳核函数基线（核数远超物理核）        |
| Case 5 | 36核，2次vf调用                                   | empty_kernel                             | 空壳核函数，测试多次vf调用开销          |
| Case 6 | 1024核，2次vf调用                                 | empty_kernel                             | 空壳核函数，测试多次vf调用开销          |
| Case 7 | 64核（物理核数），14次vf调用                                  | empty_kernel                             | 空壳核函数，测试多次vf调用开销          |
| Case 8 | 1024核，每核处理1024个数据，1次vf调用                 | gather_adds_kernel                    | 基础计算场景，核数远超物理核            |
| Case 9 | 64核（物理核数），每核处理16384个数据，vf循环在vf函数外           | gather_adds_kernel                    | 核数匹配物理核，vf循环在vf函数外        |
| Case 10| 64核（物理核数），每核处理16384个数据，vf循环在vf函数内           | gather_adds_kernel_vf_inside          | 核数匹配物理核，vf循环在vf函数内        |

#### 核心概念：逻辑核与物理核

在SIMD与SIMT混合编程模型中：

- **物理核**：硬件实际拥有的物理核数，本样例目标平台Ascend 950PR/Ascend 950DT的AIV核数为64
- **逻辑核**：通过gridDim配置启动的逻辑核数量，即`gridDim.x`
- **核数匹配原则**：当逻辑核数量小于等于物理核数量时，各核可并行执行；当逻辑核数量超过物理核数量时，多余逻辑核需要串行等待执行

本样例通过对比Case 8（1024核）与Case 9/10（64核），展示核数配置对性能的影响。

#### 核心概念：vf循环位置优化

在SIMD与SIMT混合编程模型中，vf函数调用存在一定开销。对于需要多次处理数据的场景，有两种实现方式：

- **vf循环在vf函数外**：核函数内部通过for循环多次调用vf函数，每次调用处理一部分数据（分配的线程数，即`blockDim.x`）
- **vf循环在vf函数内**：vf函数内部通过for循环处理多个数据（本样例中一个线程处理4个数据），每个逻辑核只调用一次vf函数

本样例通过对比Case 9与Case 10，展示vf循环位置对性能的影响。

### 性能指标说明

| 指标                | 说明                                                                                      |
| ------------------- | ----------------------------------------------------------------------------------------- |
| Task Duration(us)   | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间                  |
| aiv_time(us)        | Task在AI Vector Core上的理论执行时间，单位为us                                            |
| aiv_total_cycles    | 该Task被分配到每个AI Vector Core计算单元上后，每个AI Vector Core计算单元上的执行cycle总数 |
| aiv_vec_time(us)    | vec类型指令（向量类运算指令）耗时，单位为us                                               |
| aiv_vec_ratio       | vec类型指令（向量类运算指令）的cycle数在total cycle数中的占用比                           |
| aiv_scalar_time(us) | scalar类型指令（标量类运算指令）耗时，单位为us                                            |
| aiv_scalar_ratio    | scalar类型指令（标量类运算指令）的cycle数在total cycle数中的占用比                        |

除Task Duration外，本例中其余指标均展示的为所有block上性能指标的平均值。

### Case 1-7: 空壳核函数测试场景

**样例目标**：建立vf调用开销基线，测试不同核数和vf调用次数下的调度开销

**核心实现**：

- 核函数`empty_kernel`仅调用空壳vf函数`simt_empty`和`simd_empty`
- 空壳vf函数内部无实际计算逻辑，用于隔离测试gridDim配置和vf调用带来的开销
- 场景1-4测试不同核数（36/64/128/1024）下单次vf调用的开销
- 场景5-7测试多次vf调用的开销变化

**关键代码**：

```cpp
__simt_vf__ __launch_bounds__(THREAD_NUM) inline void simt_empty(...)
{
}

__simd_vf__ inline void simd_empty(...)
{
}

__global__ __vector__ void empty_kernel(...)
{
    for (uint32_t vf_idx = 0; vf_idx < vf_call_times; vf_idx++) {
        asc_vf_call<simt_empty>(...);
        asc_vf_call<simd_empty>(...);
    }
}
```

**性能数据**：

| Case | Task Duration(us) | aiv_time(us) | aiv_total_cycles | aiv_vec_time(us) | aiv_vec_ratio | aiv_scalar_time(us) | aiv_scalar_ratio |
| :--: | :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|  1   |      4.329       |    3.65      |     186777       |      0.56        |     0.154     |        1.049        |      0.287      |
|  2   |      4.484       |    3.63      |     307762       |      0.379       |     0.104     |        1.2          |      0.331      |
|  3   |      5.412       |    4.61      |     381513       |      0.647       |     0.14      |        1.654        |      0.359      |
|  4   |      13.349      |    12.57     |     591347       |      2.437       |     0.194     |        5.971        |      0.475      |
|  5   |      4.434       |    3.62      |     185748       |      0.437       |     0.121     |        0.995        |      0.275      |
|  6   |      13.794      |    13.04     |     683926       |      3.226       |     0.247     |        6.057        |      0.464      |
|  7   |      5.243       |    4.58      |     409265       |      1.177       |     0.257     |        1.192        |      0.26       |

**分析**：

- Case 1-4对比不同核数下的调度开销：36核（Case 1）耗时最低（4.329us），1024核（Case 4）耗时最高（13.349us），耗时增长约3倍，说明当逻辑核数远超物理核数时，头开销显著提升
- Case 1、5对比36核下不同vf调用次数的开销：1次vf调用耗时4.329us，2次vf调用耗时4.434us（增长约2%）
- Case 2、7对比64核下不同vf调用次数的开销：1次vf调用耗时4.484us，14次vf调用耗时5.243us（增长约16%）
- Case 4、6对比1024核下不同vf调用次数的开销：1次vf调用耗时13.349us，2次vf调用耗时13.794us（增长约3%）
- 空壳核函数场景下，核数对Task Duration影响显著，vf调用次数影响相对较小

---

### Case 8-10: Gather+Adds计算场景

**样例目标**：通过逐步调整gridDim配置和vf循环位置，展示Gather+Adds融合计算场景的性能调优路径，从核数远超物理核的基础场景，逐步优化至核数匹配+vf循环内移的最优场景

**核心实现**：

三个场景均使用相同的vf函数`simt_gather`和`simd_adds`实现Gather+Adds融合计算，差异在于gridDim配置和vf循环位置：

| 配置项 | Case 8 | Case 9 | Case 10 |
| ------ | ------ | -------| -------- |
| 逻辑核数 | 1024（远超物理核） | 64（等于物理核） | 64（等于物理核） |
| 每核处理数据量 | 1024 | 16384 | 16384 |
| vf循环位置 | vf函数外 | vf函数外 | vf函数内 |
| 核函数内vf调用次数 | 1 | 4 | 1 |
| vf函数内循环次数 | 1 | 1 | 4 |

- Case 8：启动1024个逻辑核，远超64个物理核，各逻辑核串行等待执行，每个逻辑核处理1024个数据，核函数内调用1次vf
- Case 9：启动64个逻辑核匹配物理核数，各核并行执行，每个逻辑核处理16384个数据，核函数内for循环调用16次vf（vf循环在vf函数外）
- Case 10：在Case 9基础上，将vf循环从vf函数外移到vf函数内，核函数只调用1次vf，vf函数内部循环16次处理所有数据

**关键代码**：

Case 8与Case 9均使用核函数`gather_adds_kernel`，Case 10使用核函数`gather_adds_kernel_vf_inside`。三个场景使用相同的vf函数`simt_gather`和`simd_adds`，核函数的关键差异在于vf调用方式：

```cpp
// simt_gather: SIMT vf函数关键点 —— vf_loop_times控制循环次数
// vf_loop_times=1时处理单个数据段（Case 8/9），vf_loop_times>1时循环处理多个数据段（Case 10）
for (uint32_t vf_idx = 0; vf_idx < vf_loop_times; vf_idx++) {
    uint32_t idx = threadIdx.x + index_start + vf_idx * THREAD_NUM;
    uint32_t gather_idx = index[idx];
    gather_output[threadIdx.x + vf_idx * THREAD_NUM] = input[gather_idx];
}

// Case 8/9关键点：核函数内for循环多次调用vf —— vf循环在vf函数外
for (uint32_t vf_idx = 0; vf_idx < vf_call_times; vf_idx++) {
    asc_vf_call<simt_gather>(dim3(THREAD_NUM), ..., 1);  // vf_loop_times=1
    asc_vf_call<simd_adds>(...);
    AscendC::DataCopy(output_global, adds_output, THREAD_NUM);
}

// Case 10关键点：核函数只调用1次vf —— vf循环在vf函数内
asc_vf_call<simt_gather>(dim3(THREAD_NUM), ..., vf_loop_times);  // vf_loop_times>1
asc_vf_call<simd_adds>(...);
AscendC::DataCopy(output_global, adds_output, rows_per_block);
```

**性能数据**：

| Case | Task Duration(us) | aiv_time(us) | aiv_total_cycles | aiv_vec_time(us) | aiv_vec_ratio | aiv_scalar_time(us) | aiv_scalar_ratio |
| :--: | :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|  8   |      56.868       |    55.89     |     5122965      |      41.138      |     0.736     |        3.696        |      0.066       |
|  9   |      15.608       |    14.71     |     1451511      |      11.442      |     0.778     |        1.046        |      0.071       |
|  10  |      15.197       |    14.24     |     1389444      |      10.145      |     0.712     |        0.997        |      0.07        |

**分析**：

- Case 8作为基础场景，逻辑核数（1024）远超物理核数（64），导致大量逻辑核串行执行，Task Duration高达56.868us
- Case 8→Case 9：核数从1024调整为64匹配物理核数后，Task Duration从56.868us降低到15.608us，耗时下降约72.5%，性能提升约3.64倍。各核并行执行消除串行等待开销，aiv_vec_ratio从0.736提升到0.778
- Case 9→Case 10：vf循环从vf函数外移到vf函数内，Task Duration从15.608us降低到15.197us，耗时下降约2.6%。核函数内vf调用次数从16次降到1次，aiv_vec_time从11.442us降低到10.145us
- Case 8→Case 10整体优化效果：Task Duration从56.868us降低到15.197us，性能提升约3.74倍。核数匹配优化贡献最大，vf循环内移优化贡献较小

**优化手段**：

- **Case 8→Case 9**：将逻辑核数从1024调整为64，匹配物理核数，各核并行执行，消除串行等待开销
- **Case 9→Case 10**：将vf循环从vf函数外移到vf函数内，核函数只调用1次vf，减少vf调用次数。

---

## 性能对比总结

### Ascend 950PR性能数据

**空壳核函数场景对比**：

| Case version | Task Duration(us) | 核数配置 | vf调用次数 | Task Duration相对Case 1 |
| ------------ | ----------------- | -------- | ---------- | ----------------------- |
| Case 1       | 4.329             | 36       | 1          | **1x**                  |
| Case 2       | 4.484             | 64       | 1          | **1.04x耗时**           |
| Case 3       | 5.412             | 128      | 1          | **1.25x耗时**           |
| Case 4       | 13.349            | 1024     | 1          | **3.09x耗时**           |
| Case 5       | 4.434             | 36       | 2          | **1.02x耗时**           |
| Case 6       | 13.794            | 1024     | 2          | **3.19x耗时**           |
| Case 7       | 5.243             | 64       | 14         | **1.21x耗时**           |

**实际计算场景综合优化效果**：

| Case version | Task Duration(us) | Task Duration相对Case 8 | 优化点                                |
| ------------ | ----------------- | ----------------------- | ------------------------------------- |
| Case 8       | 56.868            | **1x**                  | 基础场景，1024核串行执行              |
| Case 9       | 15.608            | **0.27x耗时**           | 64核并行执行，vf循环在vf函数外        |
| Case 10      | 15.197            | **0.27x耗时**           | 64核并行执行，vf循环在vf函数内        |

**综合优化效果**：

- 从Case 8（1024核）到Case 10（64核+vf循环内移），Task Duration从56.868us降低到15.197us，整体性能提升约3.74倍
- 核数匹配优化贡献最大（Case 8→Case 9），性能提升约3.64倍
- vf循环内移优化贡献较小（Case 9→Case 10），性能提升约2.6%

### 性能曲线图

下图展示了10个场景的Task Duration变化趋势，左侧为空壳核函数场景按核数分组排列，右侧为实际计算场景的优化路径：

<img src="./figures/task_duration_curve.svg" width="80%">

**性能曲线分析**：

1. **同核数下vf调用次数影响**：
   - 36核下，场景1（1vf, 4.329us）→场景5（2vf, 4.434us），增长约2%
   - 64核下，场景2（1vf, 4.484us）→场景7（14vf, 5.243us），增长约16%
   - 1024核下，场景4（1vf, 13.349us）→场景6（2vf, 13.794us），增长约3%
   - 说明vf调用切换开销相对较小

2. **核数对头开销的影响（不同核数分组对比）**：
   - 36核组（4.329us/4.434us）→64核组（4.484us/5.243us）→128核（5.412us）→1024核组（13.349us/13.794us）
   - 核数从36增加到1024（约28倍），Task Duration增长约3倍
   - 当逻辑核数远超物理核数时，头开销显著提升

3. **实际计算场景优化路径（Case 8→Case 9→Case 10）**：
   - Case 8（56.868us）→Case 9（15.608us）→Case 10（15.197us）
   - 核数匹配优化带来约72.5%的耗时下降
   - vf循环内移优化带来约2.6%的额外收益
   - 整体优化路径清晰，核数匹配是关键优化点

## 调优建议

1. **核数匹配物理核**：逻辑核数量应尽量匹配物理核数量，避免过多逻辑核串行执行带来的调度开销
2. **减少vf调用次数**：对于需要多次处理数据的场景，优先考虑将循环放在vf函数内部，而非在核函数内多次调用vf

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

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
  SCENARIO_NUM=8                       # 选择执行场景，可选1-10
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./grid_config                        # 执行样例
  ```

- 编译选项说明

  | 选项             | 可选值      | 说明              |
  | ---------------- | ----------- | ----------------- |
  | `SCENARIO_NUM` | `1`-`10` | 样例类型，默认为1 |

  执行结果如下，说明精度对比成功。


  ```text
  [Success] Case accuracy is verification passed.
  ```

## 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./grid_config   # 分析性能
```

当前目录下会生成PROF_前缀的文件夹，`mindstudio_profiler_output`目录保存Host和各个Device的性能数据汇总，性能数据分析推荐查看该目录下文件

```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # 保存Host和各个Device的性能数据汇总
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```
查看具体的性能分析结果：
```
# 查看Task Duration 以及各项数据
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```
