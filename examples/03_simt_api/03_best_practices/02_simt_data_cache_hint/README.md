# DataCacheHint性能调优样例

## 概述

本样例以查表法计算sin值为例，介绍Ascend C SIMT编程方式下的数据缓存提示优化思路。样例包含1个基线版本以及1个优化版本，基线版本中所有数据使用普通加载/存储指令，输入、输出和sin表数据共同竞争DCache空间；优化版本通过`asc_ldcg`、`asc_ldca`和`asc_stcg`数据缓存提示函数区分不同数据的缓存策略，确保频繁访问的sin查找表常驻DCache，减少Global Memory访问次数，展示SIMT编程方式下数据缓存优化的调优路径。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- > CANN 9.0.0

## 目录结构介绍

```text
├── 02_simt_data_cache_hint
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_cache_hint.asc         // SIMT数据缓存提示样例实现
│   └── README.md
```

## 样例描述

- 计算公式：

  使用查表法计算sin值，通过线性插值提高精度：

  $$index\_float = \frac{x}{\pi} \times table\_length$$

  $$n = \lfloor index\_float \rfloor$$

  $$frac = index\_float - n$$

  $$y = table[n] + frac \times (table[n+1] - table[n])$$

  - input为输入数据，形状为[65536]，数据类型为float，元素值均匀分布在[0, 16π]区间
  - sin_table为sin查找表，形状为[8192]，数据类型为float，覆盖[0, π]区间
  - output为输出数据，形状为[65536]，数据类型为float

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">SinTableLookup</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[65536]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">sin_table</td><td align="center">[8192]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[65536]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">sin_table_lookup_baseline / sin_table_lookup_optimized</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过2个独立的kernel实现不同的数据缓存策略，每个kernel对应特定的Case版本。

| Case   | 实现特点                                           | 使用的核函数                          | 优化特性                                |
| ------ | -------------------------------------------------- | ------------------------------------- | --------------------------------------- |
| Case 0 | 所有数据使用普通加载/存储指令，输入、输出和sin表数据共同竞争DCache空间 | sin_table_lookup_baseline             | 基线版本，无缓存提示优化                    |
| Case 1 | 输入使用asc_ldcg，sin表使用asc_ldca，输出使用asc_stcg | sin_table_lookup_optimized            | 数据缓存提示优化，sin表常驻DCache           |

#### 查表法计算说明

本样例使用查表法计算sin值，核心思路是预先将[0, π]区间的sin值存入查找表，通过线性插值获取任意角度的sin近似值。计算流程如下：

1. 将输入角度归约到[0, π]区间，处理负数和周期性
2. 计算查找表索引：`index_float = x / π × table_length`
3. 取整数索引`n`和插值系数`frac`
4. 从查找表读取`table[n]`和`table[n+1]`
5. 线性插值得到结果：`y = table[n] + frac × (table[n+1] - table[n])`

在查表类计算中，查找表是热点数据，需要频繁访问，而输入数据每个元素仅访问一次、输出数据写入后不再读取。因此，合理区分不同数据的缓存策略是优化的关键。

### 性能指标说明

| 指标                 | 说明                                                      |
| -------------------- | --------------------------------------------------------- |
| Task Duration(us)    | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间 |
| DCache Read GM       | DCache从Global Memory读取数据的次数                         |
| DCache Read Vector   | DCache向Vector Core提供数据的次数                           |
| DCache Write Vector  | Vector Core向DCache写入数据的次数                           |

除Task Duration外，本例中其余指标均展示的为所有block上性能指标的平均值。

### Case 0: 基线版本

**样例目标**：建立查表法计算sin值的耗时基线，观察输入、输出和sin表数据共同竞争DCache空间对性能的影响

**核心实现**：

- 每个线程按stride方式遍历input数据：`idx = threadIdx.x; idx < input_length; idx += blockDim.x`
- 线程直接读取`input[idx]`和`sin_table[n]` / `sin_table[n+1]`
- 线程直接写回`output[idx]`
- 所有数据访问使用普通加载/存储指令，input、sin_table和output数据共同竞争DCache空间

当访问大量输入和输出数据时，sin查找表的数据会被换出DCache，导致后续查表时需要重新从Global Memory加载，增加访存延迟。

**关键代码**：

```cpp
float x = input[idx];
float low_val = sin_table[n];
float high_val = sin_table[n + 1];
output[idx] = sign * (low_val + frac * (high_val - low_val));
```

**性能数据**：

| Task Duration(us) | DCache Read GM | DCache Read Vector | DCache Write Vector |
| :---------------: | :------------: | :----------------: | :-----------------: |
|      56.82        |     5064       |       2048         |       6144          |

**分析**：

- Case 0的Task Duration为56.82us，作为基线版本
- DCache Read GM为5064次，说明存在大量的Global Memory访问
- 当访问大量输入和输出数据时，sin查找表的数据会被换出DCache，导致后续查表时需要重新从Global Memory加载，这是Task Duration较高的主要原因

---

### Case 1: 数据缓存提示优化版本

**优化目标**：通过数据缓存提示函数区分不同数据的缓存策略，确保sin查找表常驻DCache，减少Global Memory访问次数，降低端到端耗时

**核心实现**：

- 使用`asc_ldcg`加载输入数据：输入数据每个元素仅访问一次，使用`asc_ldcg`加载数据，这些数据不需要占用Cache空间
- 使用`asc_ldca`加载sin表数据：sin表是热点数据，需要频繁访问，使用`asc_ldca`加载数据，优先为这些数据分配Cache空间，确保常驻DCache
- 使用`asc_stcg`写入输出数据：输出数据写入后不需要立即占用Cache空间，使用`asc_stcg`写入数据，这些数据直接写入Global Memory，不经过Cache缓存

**关键代码**：

```cpp
float x = asc_ldcg(&input[idx]);
float low_val = asc_ldca(&sin_table[n]);
float high_val = asc_ldca(&sin_table[n + 1]);
float y = sign * (low_val + frac * (high_val - low_val));
asc_stcg(&output[idx], y);
```

**优化手段**：

- 使用`asc_ldcg`加载输入数据，减少输入数据对DCache空间的占用
- 使用`asc_ldca`加载sin表数据，确保sin表优先缓存在DCache中
- 使用`asc_stcg`写入输出数据，避免输出数据占用DCache空间

**性能数据**：

| Task Duration(us) | DCache Read GM | DCache Read Vector | DCache Write Vector |
| :---------------: | :------------: | :----------------: | :-----------------: |
|      50.895       |     3531       |       2048         |       6144          |

**分析**：

- 相比Case 0基线版本，Task Duration从56.82us降低到50.895us，耗时下降约10.4%，整体性能提升约1.12倍
- DCache Read GM从5064降至3531，减少约30.2%（减少1533次Global Memory访问），这是性能提升的关键因素
- DCache Read Vector和DCache Write Vector保持不变，说明优化没有增加额外开销
- sin表数据常驻DCache后，避免被输入/输出数据挤出，减少了从Global Memory重新加载sin表的次数


## 性能对比总结

### Ascend 950PR性能数据

**综合优化效果**：

- 从Case 0基线版本到Case 1优化版本，Task Duration从56.82us降低到50.895us，耗时下降约10.4%，整体性能提升约1.12倍
- DCache Read GM从5064降至3531，减少约30.2%，减少1533次Global Memory访问

| Case version | Task Duration(us) | Task Duration相对Case 0 | 优化点                                |
| ------------ | ----------------- | ----------------------- | ------------------------------------- |
| Case 0       | 56.82             | **1x**                  | 基线版本，所有数据共同竞争DCache空间    |
| Case 1       | 50.895            | **0.90x耗时**           | 数据缓存提示优化，sin表常驻DCache      |

## 调优建议

1. **识别热点数据**：在查表类算子中，查找表是频繁访问的热点数据，应优先保证其常驻DCache。
2. **区分数据访问模式**：对于仅访问一次的数据（如输入数据），使用`asc_ldcg`减少其对DCache空间的占用；对于需要频繁访问的数据（如查找表），使用`asc_ldca`优先分配Cache空间。
3. **避免写回数据占用Cache**：对于写入后不再读取的输出数据，使用`asc_stcg`避免其占用DCache空间。
4. **关注DCache Read GM指标**：DCache Read GM次数的减少直接反映了Cache命中率提升和Global Memory访问延迟降低的效果。

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
  SCENARIO_NUM=1                       # 选择执行场景，可选0-1
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./data_cache_hint                    # 执行样例
  ```

- 编译选项说明

  | 选项             | 可选值      | 说明              |
  | ---------------- | ----------- | ----------------- |
  | `SCENARIO_NUM` | `0`-`1` | 样例类型，默认为0 |

  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy is verification passed.
  ```

## 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof op ./data_cache_hint   # 分析case的性能
```

命令完成后，会在默认目录下生成以"OPPROF_{timestamp}_XXX"命名的文件夹,性能数据文件夹结构示例如下：

```text
├──dump                       # 原始的性能数据，用户无需关注
├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
├──L2Cache.csv                # L2 Cache命中率
├──Memory.csv                 # UB，L1和主存储器读写带宽速率
├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
├──OpBasicInfo.csv            # 算子基础信息
├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
├──ResourceConflictRatio.csv  # UB上的 bank group、bank conflict和资源冲突率在所有指令中的占比
└──visualize_data.bin         # MindStudio Insight呈现文件
```