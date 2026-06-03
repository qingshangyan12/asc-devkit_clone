# SincosCompute性能调优样例

## 概述

本样例以elemwise运算sincos为例，介绍Ascend C SIMT编程方式下的最大线程束配置思路。样例包含两个场景，两个场景的唯一差异就是是否设置了\_\_launch_bounds__，基线版本中未设置（默认设置为1024），在此场景中出现了寄存器溢出（使用编译选项--cce-res-usage），优化案例中设置了512的线程数，在此场景中没有出现寄存器溢出。

Ascend C提供了编译选项`--cce-res-usage`来查看是否出现寄存器溢出，在此样例中两个case的编译信息如下所示，在1024线程版本中，可以看到使用了32个寄存器，并使用了32Bytes的stack空间，说明出现了溢出，512线程的版本中，使用了48个寄存器，没有出现寄存器溢出。当出现寄存器溢出时，中间数据需要溢出到Global Memory，从而导致性能出现下降。

```
[BISHENG] Function properties for _Z18sincos_thread_1024PfS_S_m_simt_entry: Stack size: 32 bytes, Used register number: 32
[BISHENG] Function properties for _Z17sincos_thread_512PfS_S_m_simt_entry: Stack size: 0 bytes, Used register number: 48
```

**表 1**  \_\_launch_bounds__的Thread数量与每个Thread可用寄存器数

<a name="table167091123820"></a>
<table><thead align="left"><tr id="row11709152311216"><th class="cellrowborder" valign="top" width="49.230000000000004%" id="mcps1.2.3.1.1"><p id="p47094233211"><a name="p47094233211"></a><a name="p47094233211"></a><span id="ph196872010141319"><a name="ph196872010141319"></a><a name="ph196872010141319"></a>blockDim大小</span></p>
</th>
<th class="cellrowborder" valign="top" width="50.77%" id="mcps1.2.3.1.2"><p id="p870962310211"><a name="p870962310211"></a><a name="p870962310211"></a>每个Thread可用寄存器个数(个)</p>
</th>
</tr>
</thead>
<tbody><tr id="row070912231821"><td class="cellrowborder" valign="top" width="49.230000000000004%" headers="mcps1.2.3.1.1 "><p id="p14710323828"><a name="p14710323828"></a><a name="p14710323828"></a>1025~2048</p>
</td>
<td class="cellrowborder" valign="top" width="50.77%" headers="mcps1.2.3.1.2 "><p id="p6710623323"><a name="p6710623323"></a><a name="p6710623323"></a>16</p>
</td>
</tr>
<tr id="row1671016231122"><td class="cellrowborder" valign="top" width="49.230000000000004%" headers="mcps1.2.3.1.1 "><p id="p97101423728"><a name="p97101423728"></a><a name="p97101423728"></a>513~1024</p>
</td>
<td class="cellrowborder" valign="top" width="50.77%" headers="mcps1.2.3.1.2 "><p id="p1571010235215"><a name="p1571010235215"></a><a name="p1571010235215"></a>32</p>
</td>
</tr>
<tr id="row171017234215"><td class="cellrowborder" valign="top" width="49.230000000000004%" headers="mcps1.2.3.1.1 "><p id="p971062316210"><a name="p971062316210"></a><a name="p971062316210"></a>257~512</p>
</td>
<td class="cellrowborder" valign="top" width="50.77%" headers="mcps1.2.3.1.2 "><p id="p137102235212"><a name="p137102235212"></a><a name="p137102235212"></a>64</p>
</td>
</tr>
<tr id="row1425604114155"><td class="cellrowborder" valign="top" width="49.230000000000004%" headers="mcps1.2.3.1.1 "><p id="p5256124181516"><a name="p5256124181516"></a><a name="p5256124181516"></a>1~256</p>
</td>
<td class="cellrowborder" valign="top" width="50.77%" headers="mcps1.2.3.1.2 "><p id="p42562418157"><a name="p42562418157"></a><a name="p42562418157"></a>127</p>
</td>
</tr>
</tbody>
</table>


## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \> CANN 9.0.0

## 目录结构介绍

```text
├── sincos_compute/
│   ├── CMakeLists.txt              // cmake编译文件
│   ├── sincos_compute.asc          // sincos样例实现
│   └── README.md
```

## 样例描述

- 样例功能

  使用sincos函数计算结果
(float* input, float* output_sin, float* output_cos,  uint64_t total_length)
- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">SinCosCompute</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[65536]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td><td align="center">output_sin</td><td align="center">[65536]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">output_cos</td><td align="center">[65536]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">sincos_thread_1024 / sincos_thread_1024</td></tr>
  </table>

## 样例实现

### sincos计算说明

本样例使用Ascend C提供的sincos接口计算sin/cos结果，样例中设置了固定shape，每个线程计算16个输入值，计算流程如下：

1. 根据算子设置的切分计算每个核的起始地址，之后计算地址索引
2. 调用Ascend C提供的sincosf接口，同时计算sin/cos结果

### 样例实现说明

本样例通过2个独立的kernel来体现\_\_launch_bounds__的效果，每个kernel对应特定的Case版本。

| Case   | 实现特点                                                                 | 使用的核函数                     | 优化特性                |
|--------|----------------------------------------------------------------------|----------------------------|---------------------|
| Case 0 | 不设置lauch bounds，使用默认值                                | sincos_thread_1024  | 基线版本，未配置\_\_launch_bounds__          |
| Case 1 | 根据实际算子的计算规模，配置\_\_launch_bounds__ | sincos_thread_512 | 编译器使用用户指定的配置值进行相应优化 |

#### 1. 线程可用寄存器数量的规格
根据硬件规格表，512场景下单个线程可以使用64个寄存器，如果用户不指定，则默认值为1024线程只能使用32个寄存器。指定合适的寄存器数量，有助于用户发挥出硬件的最优性能。
通常来说，对于计算bound的算子，使用寄存器数量较多，通常配置为512~1024线程；
对于数据搬运类算子，一般使用2048线程；


---

### 性能指标说明

| 指标                  | 说明                          |
|---------------------|-----------------------------|
| Task Duration(us)   | 整个任务执行的总时间，算子执行时间以该参数为准     |
| DCache Read GM      | DCache从Global Memory读取数据的次数 |
| DCache Read Vector  | Vector Core从DCache读取数据的次数   |
| DCache Write Vector | Vector Core向DCache写入数据的次数   |

---

### Case 0: 基线版本

**样例目标**：不配置launch bounds，观察算子性能影响

**核心实现**：
- 直接读取`input[idx]`和`sin_table[n]` / `sin_table[n+1]`的数据
- 使用线性差值进行计算数据结果
- 线程直接写回`output[idx]`
- 所有数据访问使用默认方式加载，`input`、`sin_table`和`output`数据共同竞争DCache空间

当访问大量输入和输出数据时，sin查找表的数据会被换出DCache，导致后续查表时需要重新从GM加载，增加访存延迟。

**关键代码**：

```cpp

__global__ __launch_bounds__(512) void sincos_thread_512(float* input, float* output_sin, float* output_cos)
```

**性能数据**：

| Task Duration(us) | DCache Read GM | DCache Read Vector | DCache Write Vector |
|:-----------------:|:-----------------:|:---------------------:|:----------------------:|
|       56.82       |       5064        |         2048          |          6144          |

**分析**：

Case 0的Task Duration为56.82us，DCache Read GM为5064次，作为基线版本。

Case0的场景中，加载input，output时会把数据缓存在DCache上，替换Dcache中某个缓存数据，假设sin2的数据被替换掉了，当后续某个线程计算时需要sin2的数据，就会出现cache miss，需要从GM重新加载。此外，被缓存到DCache上的input和output数据实际上不会再被使用，不需要缓存，不应该占用DCache空间。


优化方向：加载input，output的时不需要缓存DCache上，保证table数据常驻DCache，后续在使用sin2数据的时，可以直接从DCache加载，不需要GM重新加载数据。

---

### Case 1: 配置launch bounds版本

**优化目标**：提示编译器真实规模，充分利用可用寄存器资源，发挥最佳性能

**核心优化**：
- 指定`__launch_bounds__`规模为512

**关键代码**：

```cpp
float x = asc_ldcg(&input[idx]);
float index_float = x * static_cast<float>(table_length) / PI;
uint32_t n = static_cast<uint32_t>(floorf(index_float));
float frac = index_float - static_cast<float>(n);
float low_val = asc_ldca(&sin_table[n]);
float high_val = asc_ldca(&sin_table[n + 1]);
float y = sign * (low_val + frac * (high_val - low_val));
asc_stcg(&output[idx], y);
```

**性能数据**：

| Task Duration(us) | DCache Read GM(次) | DCache Read Vector(次) | DCache Write Vector(次) |
|:-----------------:|:-----------------:|:---------------------:|:----------------------:|
|      50.895       |       3531        |         2048          |          6144          |

**分析**：
- 相比Case 0基线版本，Task Duration从56.82us降低到50.895us，耗时下降约10.4%
- DCache Read GM从5064次降至3531次，减少约30.2%（减少1533次Global Memory访问），这是性能提升的关键因素
- DCache Read Vector和DCache Write Vector保持不变，说明优化没有增加额外开销
- sin表数据常驻DCache后，避免被输入/输出数据挤出，减少了从Global Memory重新加载sin表的次数
---

## 性能对比总结

### Ascend 950PR性能数据

**综合优化效果**：
- 从Case 0基线版本到Case 1优化版本，Task Duration从56.82us降低到50.895us，耗时下降约10.4%
- DCache Read GM从5064次降至3531次，减少约30.2%，减少1533次Global Memory访问

| Case version | Task Duration(us) | Task Duration相对Case 0 | 优化点                   |
|--------------|-------------------|-----------------------|-----------------------|
| Case 0       | 56.82             | **1x**                | 基线版本，所有数据共同竞争DCache空间 |
| Case 1       | 50.895            | **0.90x耗时**           | 数据缓存优化，sin表常驻DCache空间 |

## 调优建议

1. **识别是否出现寄存器溢出**：使用`--cce-res-usage`编译选项，观察是否出现寄存器溢出

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
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510;make -j;  # 编译工程
  ./demo 1024                    # 执行基线样例
  ./demo 512                     # 执行优化样例
  ```

- 编译选项说明

  | 选项                        | 可选值        | 说明                                                |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT） |

  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy is verification passed.
  ```

## 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof op ./demo 1024   # 分析基线case的性能
msprof op ./demo 512    # 分析优化case的性能
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
