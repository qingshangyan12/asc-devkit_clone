# gridDim配置与vf调用优化样例

## 概述

本样例以Gather+Adds融合计算为例，介绍Ascend C SIMD与SIMT混合编程方式下的gridDim配置和vf函数调用优化思路。样例包含5个Case（23个场景），从空壳核函数头开销测试到不同shape下的gridDim/blockDim配置优化。

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
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">empty_kernel / gather_adds_kernel</td></tr>
  </table>

## 场景配置总表

| SCENARIO_NUM | Case | num_blocks | thread_num | vf_call_times | use_gather_adds | input_total_length | index_rows | 说明 |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---|
| 1 | Case 0 | 1 | 2048 | 1 | false | 2200000 | 1024 | 1核, 1 vf (空壳) |
| 2 | Case 0 | 32 | 2048 | 1 | false | 2200000 | 1024 | 32核, 1 vf (空壳) |
| 3 | Case 0 | 64 | 2048 | 1 | false | 2200000 | 1024 | 64核, 1 vf (空壳) |
| 4 | Case 0 | 128 | 2048 | 1 | false | 2200000 | 1024 | 128核, 1 vf (空壳) |
| 5 | Case 0 | 1024 | 2048 | 1 | false | 2200000 | 1024 | 1024核, 1 vf (空壳) |
| 6 | Case 1 | 32 | 2048 | 2 | false | 2200000 | 1024 | 32核, 2 vf (空壳) |
| 7 | Case 1 | 64 | 2048 | 2 | false | 2200000 | 1024 | 64核, 2 vf (空壳) |
| 8 | Case 1 | 1024 | 2048 | 2 | false | 2200000 | 1024 | 1024核, 2 vf (空壳) |
| 9 | Case 2 | 1024 | 2048 | 1 | true | 2200000 | 1024 | 1024核, 1数据/线程 |
| 10 | Case 2 | 128 | 2048 | 8 | true | 2200000 | 1024 | 128核, 8数据/线程 |
| 11 | Case 2 | 64 | 2048 | 16 | true | 2200000 | 1024 | 64核, 16数据/线程 |
| 12 | Case 3 | 16 | 2048 | 4 | true | 200000 | 64 | 16核, 4数据/线程 |
| 13 | Case 3 | 32 | 2048 | 2 | true | 200000 | 64 | 32核, 2数据/线程 |
| 14 | Case 3 | 64 | 2048 | 1 | true | 200000 | 64 | 64核, 1数据/线程 |
| 15 | Case 3 | 128 | 2048 | 1 | true | 200000 | 64 | 128核, 1数据/线程 |
| 16 | Case 4 | 4 | 2048 | 2 | true | 20000 | 8 | 4核, 2数据/线程 |
| 17 | Case 4 | 8 | 2048 | 1 | true | 20000 | 8 | 8核, 1数据/线程 |
| 18 | Case 4 | 16 | 2048 | 1 | true | 20000 | 8 | 16核, 1数据/线程 |
| 19 | Case 4 | 32 | 2048 | 1 | true | 20000 | 8 | 32核, 1数据/线程 |
| 20 | Case 4 | 64 | 2048 | 1 | true | 20000 | 8 | 64核, 1数据/线程 |
| 21 | Case 4 | 16 | 1024 | 1 | true | 20000 | 8 | 16核, 1024线程 |
| 22 | Case 4 | 32 | 512 | 1 | true | 20000 | 8 | 32核, 512线程 |
| 23 | Case 4 | 64 | 256 | 1 | true | 20000 | 8 | 64核, 256线程 |

> **vf_call_times计算规则**：对gather+adds场景，vf_call_times = 每个逻辑核处理的数据量(rows_per_block) / 分配线程数(thread_num)，最小值为1。rows_per_block = index_rows × INDEX_WIDTH / num_blocks。

## 样例实现

### Case实现说明

本样例通过5个Case展示不同的gridDim配置与vf调用策略，每个Case包含多个场景（SCENARIO_NUM）。

| Case | SCENARIO_NUM | 实现特点 | 使用的核函数 | 优化特性 |
| ---- | ------------ | -------- | ----------- | -------- |
| Case 0 | 1-5 | 不同逻辑核个数（1/32/64/128/1024），1次vf调用 | empty_kernel | 展示逻辑核个数对头开销的影响 |
| Case 1 | 6-8 | 不同vf调用次数（2次），32/64/1024核 | empty_kernel | 展示vf调用次数对头开销的影响 |
| Case 2 | 9-11 | 大shape(2200000), 1024/128/64核 | gather_adds_kernel | gridDim配置优化路径 |
| Case 3 | 12-15 | 中shape(200000), 64行, 不同核数配置 | gather_adds_kernel | 线程数据分布对性能的影响 |
| Case 4 | 16-23 | 小shape(20000), 8行, 不同核数与线程数对比 | gather_adds_kernel | blockDim与gridDim联合调优 |

#### 核心概念

**vf循环位置优化**

在SIMD与SIMT混合编程模型中，vf函数调用存在一定开销。对于需要多次处理数据的场景，有两种实现方式：

- **vf循环在vf函数外**：核函数内部通过for循环多次调用vf函数，每次调用处理一部分数据（分配的线程数，即`blockDim.x`）
- **vf循环在vf函数内**：vf函数内部通过for循环处理多个数据，每个逻辑核只调用一次vf函数

本样例所有gather+adds场景均采用vf循环在vf函数内的方式，通过`vf_loop_times`参数控制每个线程处理的数据量。

**thread_num参数**

gather+adds场景通过`thread_num`参数控制vf函数实际启动的线程数。场景21-23使用1024/512/256线程，与场景18-20（2048线程）形成对比，展示不同blockDim对性能的影响。

```cpp
// gather_adds_kernel中通过thread_num适配不同场景
asc_vf_call<simt_gather>(dim3(thread_num), ..., vf_loop_times, thread_num);
```

### 性能指标说明

| 指标 | 说明 |
| ---- | ---- |
| Task Duration(us) | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间 |

### Case 0: 逻辑核个数对头开销的影响

**样例目标**：测试不同逻辑核个数下的调度头开销，展示逻辑核数远超可用核数时头开销显著提升

**核心实现**：

- 核函数`empty_kernel`仅调用空壳vf函数`simt_empty`和`simd_empty`
- 空壳vf函数内部无实际计算逻辑，用于隔离测试gridDim配置带来的开销
- 场景1-5分别启动1、32、64、128、1024个逻辑核，每次调用1次vf

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

| SCENARIO_NUM | 逻辑核数 | Task Duration(us) |
| :----------: | :------: | :---------------: |
| 1 | 1 | TBD |
| 2 | 32 | TBD |
| 3 | 64 | 4.57 |
| 4 | 128 | 5.05 |
| 5 | 1024 | 14.244 |

**分析**：

- 64核耗时4.57us，128核耗时5.05us，1024核耗时14.244us，逻辑核数远超可用核数时头开销显著提升
- 场景1（1核）和场景2（32核）数据待补充

---

### Case 1: vf调用次数对头开销的影响

**样例目标**：测试不同vf调用次数下的调度头开销，展示vf调用次数对Task Duration影响相对较小

**核心实现**：

- 与Case 0使用相同的空壳核函数`empty_kernel`
- 场景6-8分别启动32、64、1024个逻辑核，每次调用2次vf
- 与Case 0对应场景对比：场景3（64核1次vf）vs场景7（64核2次vf），场景5（1024核1次vf）vs场景8（1024核2次vf）

**性能数据**：

| SCENARIO_NUM | 逻辑核数 | vf调用次数 | Task Duration(us) |
| :----------: | :------: | :--------: | :---------------: |
| 6 | 32 | 2 | TBD |
| 7 | 64 | 2 | TBD |
| 8 | 1024 | 2 | 16.2 |

**分析**：

- 1024核下：1次vf调用耗时14.244us（场景5），2次vf调用耗时16.2us（场景8），增长约14%
- vf调用次数对Task Duration影响相对较小，核数配置是影响调度开销的主要因素
- 32核和64核的2次vf数据待补充

---

### Case 2: Gather+Adds大shape场景的gridDim配置优化

**样例目标**：通过调整gridDim配置，展示大shape(2200000)下Gather+Adds融合计算的性能优化路径

**核心实现**：

三个场景均使用核函数`gather_adds_kernel`，差异在于逻辑核个数和每个线程处理的数据量：

- input形状为[2200000]，index和output形状为[1024, 2048]，线程数为2048

| 配置项 | 场景9 | 场景10 | 场景11 |
| ------ | ------ | ------ | ------ |
| 逻辑核数 | 1024 | 128 | 64 |
| 每个线程处理数据量 | 1 | 8 | 16 |

- 场景9：启动1024个逻辑核，每个线程处理1个数据
- 场景10：启动128个逻辑核，每个线程处理8个数据
- 场景11：启动64个逻辑核，每个线程处理16个数据

**关键代码**：

```cpp
// simt_gather: SIMT vf函数关键点 —— vf_loop_times控制循环次数
for (uint32_t vf_idx = 0; vf_idx < vf_loop_times; vf_idx++) {
    uint32_t idx = threadIdx.x + index_start + vf_idx * thread_num;
    uint32_t gather_idx = index[idx];
    gather_output[threadIdx.x + vf_idx * thread_num] = input[gather_idx];
}

// gather_adds_kernel: 核函数通过thread_num适配不同场景
asc_vf_call<simt_gather>(dim3(thread_num), ..., vf_loop_times, thread_num);
asc_vf_call<simd_adds>(...);
AscendC::DataCopy(output_global, adds_output, rows_per_block);
```

**性能数据**：

| SCENARIO_NUM | Case | Task Duration(us) |
| :----------: | :--: | :---------------: |
| 9 | 2 | 81.004 |
| 10 | 2 | TBD |
| 11 | 2 | 63.731 |

**分析**：

- 场景9（1024核）作为基础场景，Task Duration高达81.004us
- 场景9→场景11：将逻辑核数从1024调整为64，Task Duration从81.004us降低到63.731us，性能提升约1.27倍
- 场景10（128核, 8数据/线程）数据待补充

**优化手段**：

- **逐步减少逻辑核数**：1024核→128核→64核，减少串行等待开销

---

### Case 3: Gather+Adds中shape场景的gridDim配置优化

**样例目标**：探索中shape(200000)下不同逻辑核个数对Gather+Adds融合计算性能的影响

**核心实现**：

四个场景均使用核函数`gather_adds_kernel`，差异在于逻辑核个数和每个线程处理的数据量：

- input形状为[200000]，index和output形状为[64, 2048]，线程数为2048

| 配置项 | 场景12 | 场景13 | 场景14 | 场景15 |
| ------ | ------ | ------ | ------ | ------ |
| 逻辑核数 | 16 | 32 | 64 | 128 |
| 每个线程处理数据量 | 4 | 2 | 1 | 1 |

- 场景12：16逻辑核，每个线程处理4个数据
- 场景13：32逻辑核，每个线程处理2个数据
- 场景14：64逻辑核，每个线程处理1个数据
- 场景15：128逻辑核，每个线程处理1个数据

四个场景的总数据量相同（64×2048=131072），通过调整逻辑核个数和每个线程处理的数据量，探索不同gridDim配置下的性能差异。

**性能数据**：

| SCENARIO_NUM | Case | Task Duration(us) |
| :----------: | :--: | :---------------: |
| 12 | 3 | 81.596 |
| 13 | 3 | 53.324 |
| 14 | 3 | 41.346 |
| 15 | 3 | TBD |

**分析**：

- 16核（81.596us）→32核（53.324us）→64核（41.346us），逻辑核数增加，Task Duration降低
- 64核配置下性能最优，减少逻辑核数会导致每个核处理更多数据，计算时间增加
- 场景15（128核）数据待补充

---

### Case 4: Gather+Adds小shape场景的gridDim与blockDim联合调优

**样例目标**：探索小shape(20000)下逻辑核个数与线程数对Gather+Adds融合计算性能的综合影响

**核心实现**：

八个场景均使用核函数`gather_adds_kernel`，差异在于逻辑核个数和线程数配置：

- input形状为[20000]，index和output形状为[8, 2048]

**场景16-20：相同线程数(2048T)，不同核数**

| 配置项 | 场景16 | 场景17 | 场景18 | 场景19 | 场景20 |
| ------ | ------ | ------ | ------ | ------ | ------ |
| 逻辑核数 | 4 | 8 | 16 | 32 | 64 |
| 线程数 | 2048 | 2048 | 2048 | 2048 | 2048 |
| vf_call_times | 2 | 1 | 1 | 1 | 1 |
| 每个线程处理数据量 | 2 | 1 | 1 | 1 | 1 |

- 场景16：4逻辑核，每个线程处理2个数据
- 场景17：8逻辑核，每个线程处理1个数据
- 场景18-20：16/32/64逻辑核，每个线程处理1个数据

**场景21-23：不同线程数，核数与场景18-20对应**

| 配置项 | 场景21 | 场景22 | 场景23 |
| ------ | ------ | ------ | ------ |
| 逻辑核数 | 16 | 32 | 64 |
| 线程数 | 1024 | 512 | 256 |
| vf_call_times | 1 | 1 | 1 |
| 每个线程处理数据量 | 1 | 1 | 1 |

**对比分析**：

| 对比组 | 2048线程场景 | 调整thread_num场景 | 对比意义 |
| ------ | ------ | ------ | ------ |
| 场景18 vs 21 | 16核, 2048T | 16核, 1024T | 不同blockDim对比 |
| 场景19 vs 22 | 32核, 2048T | 32核, 512T | 不同blockDim对比 |
| 场景20 vs 23 | 64核, 2048T | 64核, 256T | 不同blockDim对比 |

**性能数据**：

| SCENARIO_NUM | Case | Task Duration(us) |
| :----------: | :--: | :---------------: |
| 16 | 4 | TBD |
| 17 | 4 | TBD |
| 18 | 4 | TBD |
| 19 | 4 | TBD |
| 20 | 4 | TBD |
| 21 | 4 | TBD |
| 22 | 4 | TBD |
| 23 | 4 | TBD |

---

## 性能对比总结

### Ascend 950PR性能数据

| Case | SCENARIO_NUM | Task Duration(us) | 逻辑核数 | 线程数 | 每个线程处理数据量 | 说明 |
| ---- | ------------ | ----------------- | -------- | ------ | ------------------ | ---- |
| Case 0 | 1 | TBD | 1 | 2048 | - | 1核空壳基线 |
| Case 0 | 2 | TBD | 32 | 2048 | - | 32核空壳基线 |
| Case 0 | 3 | 4.57 | 64 | 2048 | - | 64核空壳基线 |
| Case 0 | 4 | 5.05 | 128 | 2048 | - | 128核空壳基线 |
| Case 0 | 5 | 14.244 | 1024 | 2048 | - | 1024核空壳基线 |
| Case 1 | 6 | TBD | 32 | 2048 | - | 32核2次vf |
| Case 1 | 7 | TBD | 64 | 2048 | - | 64核2次vf |
| Case 1 | 8 | 16.2 | 1024 | 2048 | - | 1024核2次vf |
| Case 2 | 9 | 81.004 | 1024 | 2048 | 1 | 1024核基础场景 |
| Case 2 | 10 | TBD | 128 | 2048 | 8 | 128核优化场景 |
| Case 2 | 11 | 63.731 | 64 | 2048 | 16 | 64核优化场景 |
| Case 3 | 12 | 81.596 | 16 | 2048 | 4 | 中shape16核 |
| Case 3 | 13 | 53.324 | 32 | 2048 | 2 | 中shape32核 |
| Case 3 | 14 | 41.346 | 64 | 2048 | 1 | 中shape64核 |
| Case 3 | 15 | TBD | 128 | 2048 | 1 | 中shape128核 |
| Case 4 | 16 | TBD | 4 | 2048 | 2 | 小shape4核 |
| Case 4 | 17 | TBD | 8 | 2048 | 1 | 小shape8核 |
| Case 4 | 18 | TBD | 16 | 2048 | 1 | 小shape16核 |
| Case 4 | 19 | TBD | 32 | 2048 | 1 | 小shape32核 |
| Case 4 | 20 | TBD | 64 | 2048 | 1 | 小shape64核 |
| Case 4 | 21 | TBD | 16 | 1024 | 1 | 小shape16核(1024T) |
| Case 4 | 22 | TBD | 32 | 512 | 1 | 小shape32核(512T) |
| Case 4 | 23 | TBD | 64 | 256 | 1 | 小shape64核(256T) |

**关键发现**：

1. **逻辑核数对调度开销的影响**（Case 0）：64核→1024核，Task Duration增长约3.1倍
2. **vf调用次数对调度开销的影响**（Case 1）：1024核下，1次vf→2次vf增长约14%，影响相对较小
3. **逻辑核数对计算性能的影响**：
   - Case 2：1024核→64核，Task Duration降低约21%
   - Case 3：16核→64核，Task Duration降低约50%
4. **blockDim对性能的影响**（Case 4对比）：场景18-20（2048T）vs场景21-23（1024/512/256T），数据待补充
5. **数据量与核数的匹配**：数据量较大时（Case 2/3），核数过多导致串行等待；数据量较小时（Case 4），增加核数可能提升并行收益

### 性能曲线图

下图展示了23个场景的Task Duration变化趋势，按Case分组排列：

<img src="./figures/task_duration_curve.svg" width="80%">

## 调优建议

1. **合理配置逻辑核数**：逻辑核数量不宜过多，避免过多逻辑核串行执行带来的调度开销；也不宜过少，需保证足够的并行度
2. **匹配数据量与核数**：数据量较大时适当减少核数，数据量较小时适当增加核数，以平衡并行收益与调度开销
3. **优化线程数据分布**：通过调整每个线程处理的数据量，在核数与并行度之间找到最优平衡点
4. **合理设置blockDim**：通过调整thread_num，在核数与线程数之间找到最优配置

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
  SCENARIO_NUM=9                       # 选择执行场景，可选1-23
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./grid_config                        # 执行样例
  ```

- 编译选项说明

  | 选项             | 可选值      | 说明              |
  | ---------------- | ----------- | ----------------- |
  | `SCENARIO_NUM` | `1`-`23` | 样例类型，默认为1 |

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