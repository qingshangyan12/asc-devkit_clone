# gridDim配置与vf调用优化样例

## 概述

本样例以Gather+Adds融合计算为例，介绍Ascend C SIMD与SIMT混合编程方式下的gridDim配置和vf函数调用优化思路。样例包含2个空壳核函数场景以及1个逐步优化的实际计算场景。

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

  $$output[i][j] = input[index[i][j]] + 1.0$$

  - input为输入张量，数据类型为float，形状因Case不同而异：Case 0/1/2为[2200000]，Case 3为[200000]，Case 4为[20000]
  - index为索引张量，数据类型为uint32，形状因Case不同而异：Case 0/1/2为[1024, 2048]，Case 3为[64, 2048]，Case 4为[8, 2048]，索引范围为[0, input_total_length)
  - output为输出张量，数据类型为float，形状与index相同
  - 计算过程：根据index[i][j]索引从input中采集对应位置的数据，加1.0后写入output[i][j]位置

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">GatherAdds</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[2200000]/[200000]/[20000]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">index</td><td align="center">[1024,2048]/[64,2048]/[8,2048]</td><td align="center">uint32</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">output</td><td align="center">[1024,2048]/[64,2048]/[8,2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">empty_kernel / gather_adds_kernel / gather_adds_kernel_vf_inside</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过4个Case展示不同的gridDim配置与vf调用策略，每个Case包含多个场景（SCENARIO_NUM）。

| Case   | SCENARIO_NUM | 实现特点                                           | 使用的核函数                          | 优化特性                                |
| ------ | ------------ | -------------------------------------------------- | ------------------------------------- | --------------------------------------- |
| Case 0 | 1-4          | 不同逻辑核个数（36/64/128/1024），1次vf调用       | empty_kernel                         | 展示逻辑核个数对头开销的影响            |
| Case 1 | 5-6          | 不同vf调用次数（1次/2次），36核和1024核            | empty_kernel                         | 展示vf调用次数对头开销的影响            |
| Case 2 | 7-8          | 1024核vf外 vs 64核vf内                             | gather_adds_kernel / gather_adds_kernel_vf_inside | gridDim配置与vf循环位置优化路径         |
| Case 3 | 9-11         | 64/32/16核，每个线程处理1/2/4个数据                | gather_adds_kernel_vf_inside         | 线程数据分布对性能的影响                |
| Case 4 | 12-15        | 8/4/2/1核，每个线程处理1/2/4/8个数据               | gather_adds_kernel_vf_inside         | 线程数据分布对性能的影响                |

#### 核心概念：vf循环位置优化

在SIMD与SIMT混合编程模型中，vf函数调用存在一定开销。对于需要多次处理数据的场景，有两种实现方式：

- **vf循环在vf函数外**：核函数内部通过for循环多次调用vf函数，每次调用处理一部分数据（分配的线程数，即`blockDim.x`）
- **vf循环在vf函数内**：vf函数内部通过for循环处理多个数据，每个逻辑核只调用一次vf函数

本样例通过对比Case 2中场景7与场景8，展示vf循环位置对性能的影响。

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

### Case 0: 逻辑核个数对头开销的影响

**样例目标**：测试不同逻辑核个数下的调度头开销，展示逻辑核数远超可用核数时头开销显著提升

**核心实现**：

- 核函数`empty_kernel`仅调用空壳vf函数`simt_empty`和`simd_empty`
- 空壳vf函数内部无实际计算逻辑，用于隔离测试gridDim配置带来的开销
- 场景1-4分别启动36、64、128、1024个逻辑核，每次调用1次vf

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

| SCENARIO_NUM | 逻辑核数 | Task Duration(us) | aiv_time(us) | aiv_total_cycles | aiv_vec_time(us) | aiv_vec_ratio | aiv_scalar_time(us) | aiv_scalar_ratio |
| :----------: | :------: | :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|  1           | 36       |      4.329       |    3.65      |     186777       |      0.56        |     0.154     |        1.049        |      0.287      |
|  2           | 64       |      4.484       |    3.63      |     307762       |      0.379       |     0.104     |        1.2          |      0.331      |
|  3           | 128      |      5.412       |    4.61      |     381513       |      0.647       |     0.14      |        1.654        |      0.359      |
|  4           | 1024     |      13.349      |    12.57     |     591347       |      2.437       |     0.194     |        5.971        |      0.475      |

**分析**：

- 逻辑核数从36增加到1024（约28倍），Task Duration从4.329us增长到13.349us（约3倍），说明当逻辑核数远超可用核数时，头开销显著提升
- 36核耗时最低（4.329us），1024核耗时最高（13.349us），核数配置对调度开销影响显著

---

### Case 1: vf调用次数对头开销的影响

**样例目标**：测试不同vf调用次数下的调度头开销，展示vf调用次数对Task Duration影响相对较小

**核心实现**：

- 与Case 0使用相同的空壳核函数`empty_kernel`
- 场景5：36核，2次vf调用；场景6：1024核，2次vf调用
- 与Case 0对应场景对比：场景1（36核1次vf）vs场景5（36核2次vf），场景4（1024核1次vf）vs场景6（1024核2次vf）

**性能数据**：

| SCENARIO_NUM | 逻辑核数 | vf调用次数 | Task Duration(us) | aiv_time(us) | aiv_total_cycles | aiv_vec_time(us) | aiv_vec_ratio | aiv_scalar_time(us) | aiv_scalar_ratio |
| :----------: | :------: | :--------: | :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|  5           | 36       | 2          |      4.434       |    3.62      |     185748       |      0.437       |     0.121     |        0.995        |      0.275      |
|  6           | 1024     | 2          |      13.794      |    13.04     |     683926       |      3.226       |     0.247     |        6.057        |      0.464      |

**分析**：

- 36核下：1次vf调用耗时4.329us（场景1），2次vf调用耗时4.434us（场景5），增长约2%
- 1024核下：1次vf调用耗时13.349us（场景4），2次vf调用耗时13.794us（场景6），增长约3%
- vf调用次数对Task Duration影响相对较小，核数配置是影响调度开销的主要因素

---

### Case 2: Gather+Adds计算场景的gridDim与vf循环优化

**样例目标**：通过调整gridDim配置和vf循环位置，展示Gather+Adds融合计算场景的性能优化路径

**核心实现**：

两个场景均使用相同的vf函数`simt_gather`和`simd_adds`实现Gather+Adds融合计算，差异在于gridDim配置和vf循环位置：

| 配置项 | 场景7 | 场景8 |
| ------ | ------ | ------ |
| 逻辑核数 | 1024 | 64 |
| 每个线程处理数据量 | 1 | 16 |
| vf循环位置 | vf函数外 | vf函数内 |
| 核函数 | gather_adds_kernel | gather_adds_kernel_vf_inside |

- 场景7：启动1024个逻辑核，每个线程处理1个数据，核函数内调用1次vf（vf循环在vf函数外）
- 场景8：启动64个逻辑核，每个线程处理16个数据，核函数只调用1次vf，vf函数内部循环16次处理所有数据（vf循环在vf函数内）

**关键代码**：

场景7使用核函数`gather_adds_kernel`（vf循环在vf函数外），场景8使用核函数`gather_adds_kernel_vf_inside`（vf循环在vf函数内）。两个场景使用相同的vf函数`simt_gather`和`simd_adds`，核函数的关键差异在于vf调用方式：

```cpp
// simt_gather: SIMT vf函数关键点 —— vf_loop_times控制循环次数
// vf_loop_times=1时处理单个数据段（场景7），vf_loop_times>1时循环处理多个数据段（场景8）
for (uint32_t vf_idx = 0; vf_idx < vf_loop_times; vf_idx++) {
    uint32_t idx = threadIdx.x + index_start + vf_idx * THREAD_NUM;
    uint32_t gather_idx = index[idx];
    gather_output[threadIdx.x + vf_idx * THREAD_NUM] = input[gather_idx];
}

// 场景7关键点：核函数内for循环多次调用vf —— vf循环在vf函数外
for (uint32_t vf_idx = 0; vf_idx < vf_call_times; vf_idx++) {
    asc_vf_call<simt_gather>(dim3(THREAD_NUM), ..., 1);  // vf_loop_times=1
    asc_vf_call<simd_adds>(...);
    AscendC::DataCopy(output_global, adds_output, THREAD_NUM);
}

// 场景8关键点：核函数只调用1次vf —— vf循环在vf函数内
asc_vf_call<simt_gather>(dim3(THREAD_NUM), ..., vf_loop_times);  // vf_loop_times>1
asc_vf_call<simd_adds>(...);
AscendC::DataCopy(output_global, adds_output, rows_per_block);
```

**性能数据**：

| SCENARIO_NUM | Case | Task Duration(us) | aiv_time(us) | aiv_total_cycles | aiv_vec_time(us) | aiv_vec_ratio | aiv_scalar_time(us) | aiv_scalar_ratio |
| :----------: | :--: | :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|  7           |  2   |      56.868       |    55.89     |     5122965      |      41.138      |     0.736     |        3.696        |      0.066       |
|  8           |  2   |      15.197       |    14.24     |     1389444      |      10.145      |     0.712     |        0.997        |      0.07        |

**分析**：

- 场景7作为基础场景，逻辑核数较多（1024），各逻辑核串行等待执行，Task Duration高达56.868us
- 场景7→场景8：将逻辑核数从1024调整为64，同时将vf循环从vf函数外移到vf函数内，Task Duration从56.868us降低到15.197us，性能提升约3.74倍。核数调整是主要优化因素，vf循环内移带来额外收益

**优化手段**：

- **减少逻辑核数**：将逻辑核数从1024调整为64，各核并行执行，消除串行等待开销
- **vf循环内移**：将vf循环从vf函数外移到vf函数内，核函数只调用1次vf，减少vf调用次数

---

### Case 3: 线程数据分布对性能的影响（64行场景）

**样例目标**：探索不同逻辑核个数与每个线程处理数据量对Gather+Adds融合计算性能的影响

**核心实现**：

三个场景均使用核函数`gather_adds_kernel_vf_inside`，vf循环在vf函数内，差异在于逻辑核个数和每个线程处理的数据量：

- input形状为[200000]，index和output形状为[64, 2048]，线程数为2048

| 配置项 | 场景9 | 场景10 | 场景11 |
| ------ | ------ | ------ | ------ |
| 逻辑核数 | 64 | 32 | 16 |
| 每个线程处理数据量 | 1 | 2 | 4 |
| vf函数内循环次数 | 1 | 2 | 4 |

- 场景9：64逻辑核，每个线程处理1个数据
- 场景10：32逻辑核，每个线程处理2个数据
- 场景11：16逻辑核，每个线程处理4个数据

三个场景的总数据量相同（64×2048=131072），通过调整逻辑核个数和每个线程处理的数据量，探索不同gridDim配置下的性能差异。

---

### Case 4: 线程数据分布对性能的影响（8行场景）

**样例目标**：探索极端逻辑核个数配置下每个线程处理数据量对Gather+Adds融合计算性能的影响

**核心实现**：

四个场景均使用核函数`gather_adds_kernel_vf_inside`，vf循环在vf函数内，差异在于逻辑核个数和每个线程处理的数据量：

- input形状为[20000]，index和output形状为[8, 2048]，线程数为2048

| 配置项 | 场景12 | 场景13 | 场景14 | 场景15 |
| ------ | ------ | ------ | ------ | ------ |
| 逻辑核数 | 8 | 4 | 2 | 1 |
| 每个线程处理数据量 | 1 | 2 | 4 | 8 |
| vf函数内循环次数 | 1 | 2 | 4 | 8 |

- 场景12：8逻辑核，每个线程处理1个数据
- 场景13：4逻辑核，每个线程处理2个数据
- 场景14：2逻辑核，每个线程处理4个数据
- 场景15：1逻辑核，每个线程处理8个数据

四个场景的总数据量相同（8×2048=16384），通过调整逻辑核个数和每个线程处理的数据量，探索极端配置下的性能差异。

---

## 性能对比总结

### Ascend 950PR性能数据

**空壳核函数场景对比**：

| Case | SCENARIO_NUM | Task Duration(us) | 逻辑核数 | vf调用次数 | 说明 |
| ---- | ------------ | ----------------- | -------- | ---------- | ---- |
| Case 0 | 1 | 4.329 | 36 | 1 | 36核基线 |
| Case 0 | 2 | 4.484 | 64 | 1 | 64核基线 |
| Case 0 | 3 | 5.412 | 128 | 1 | 128核基线 |
| Case 0 | 4 | 13.349 | 1024 | 1 | 1024核基线 |
| Case 1 | 5 | 4.434 | 36 | 2 | 36核2次vf |
| Case 1 | 6 | 13.794 | 1024 | 2 | 1024核2次vf |

**Case 2优化效果**：

| Case | SCENARIO_NUM | Task Duration(us) | Task Duration相对场景7 | 优化点                                |
| ---- | ------------ | ----------------- | ----------------------- | ------------------------------------- |
| Case 2 | 7 | 56.868 | **1x** | 基础场景，1024核串行执行              |
| Case 2 | 8 | 15.197 | **0.27x耗时** | 64核并行执行，vf循环在vf函数内        |

**综合优化效果**：

- 从场景7（1024核）到场景8（64核+vf循环内移），Task Duration从56.868us降低到15.197us，整体性能提升约3.74倍
- 核数调整是主要优化因素，vf循环内移带来额外收益

### 性能曲线图

下图展示了场景1-8的Task Duration变化趋势，左侧为空壳核函数场景按核数分组排列，右侧为实际计算场景的优化路径：

<img src="./figures/task_duration_curve.svg" width="80%">

**性能曲线分析**：

1. **逻辑核个数对头开销的影响（Case 0）**：
   - 36核（4.329us）→64核（4.484us）→128核（5.412us）→1024核（13.349us）
   - 核数从36增加到1024（约28倍），Task Duration增长约3倍
   - 当逻辑核数远超可用核数时，头开销显著提升

2. **vf调用次数对头开销的影响（Case 0 vs Case 1）**：
   - 36核下，场景1（1vf, 4.329us）→场景5（2vf, 4.434us），增长约2%
   - 1024核下，场景4（1vf, 13.349us）→场景6（2vf, 13.794us），增长约3%
   - 说明vf调用切换开销相对较小

3. **实际计算场景优化路径（Case 2）**：
   - 场景7（56.868us）→场景8（15.197us）
   - 核数调整与vf循环内移带来约72.7%的耗时下降
   - 整体性能提升约3.74倍

## 调优建议

1. **合理配置逻辑核数**：逻辑核数量不宜过多，避免过多逻辑核串行执行带来的调度开销
2. **减少vf调用次数**：对于需要多次处理数据的场景，优先考虑将循环放在vf函数内部，而非在核函数内多次调用vf
3. **优化线程数据分布**：对于数据量较小的场景，可通过减少逻辑核数、增加每个线程处理的数据量来优化性能

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
  SCENARIO_NUM=7                       # 选择执行场景，可选1-15
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./grid_config                        # 执行样例
  ```

- 编译选项说明

  | 选项             | 可选值      | 说明              |
  | ---------------- | ----------- | ----------------- |
  | `SCENARIO_NUM` | `1`-`15` | 样例类型，默认为1 |

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