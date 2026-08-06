# 融合向量计算双Buffer性能样例

## 概述

本样例使用Ascend C C API实现half融合向量计算，并对比单Buffer和Ping/Pong双Buffer两种实现。通过编译参数调整寄存器内的融合计算量，在不增加GM搬运量的情况下观察Vector时间变化，以及不同计算强度下双Buffer的性能收益。

## 支持的产品及CANN软件版本

**表1**  支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```text
add_high_performance
├── figures
│   └── double_buffer.png    # Ping/Pong Buffer流水示意图
├── scripts
│   ├── gen_data.py          # 生成输入数据和真值数据
│   └── verify_result.py     # 校验输出结果
├── CMakeLists.txt           # 编译工程文件
├── add.asc                  # Kernel及Host侧实现
├── data_utils.h             # 二进制数据读写工具
├── README.md
└── README_en.md
```

## 样例描述

样例对两个形状为`[8192, 8192]`的half矩阵执行逐元素递推计算。首先计算

$$
t_0 = x + y
$$

然后执行`FUSED_ITERATIONS`轮递推：

$$
t_{i+1} = t_i \times x + y
$$

最终输出$z=t_R$，其中$R$为`FUSED_ITERATIONS`。中间结果保存在Vector寄存器中，每轮始终只从GM读取一次x和y，并写回一次z。

`FUSED_ITERATIONS`支持`0`、`2`、`4`、`8`和`12`，对应的Vector算术指令数分别为`1`、`5`、`9`、`17`和`25`。其中`0`表示只计算`z=x+y`。

输入、输出和真值数据见表2，均以ND格式连续存放在GM中。

**表2**  输入输出数据规格

| 数据 | 输入/输出 | 数据文件 | 数据类型 | Shape | 数据说明 |
| --- | --- | --- | --- | --- | --- |
| x | 输入 | `input/input_x.bin` | half | `[8192, 8192]` | 在区间`[-0.1, 0.1)`内随机生成。 |
| y | 输入 | `input/input_y.bin` | half | `[8192, 8192]` | 在区间`[-0.1, 0.1)`内随机生成。 |
| z | 输出 | `output/output.bin` | half | `[8192, 8192]` | Kernel计算结果 |
| golden | 真值 | `output/golden.bin` | half | `[8192, 8192]` | 按Kernel相同的递推顺序生成，每次乘法和加法后转换为half。 |

两个场景使用相同的切核和分块参数。矩阵沿M轴分给64个核，每核处理连续128行。具体计算见表3。

**表3**  多核切分和分块参数

| 参数 | 取值 | 说明 |
| --- | --- | --- |
| 矩阵元素总数 | 67108864 | `8192 * 8192` |
| 核数 | 64 | 每个核处理连续的M轴数据。 |
| 每核处理行数 | 128 | `8192 / 64` |
| 每核处理元素数 | 1048576 | `128 * 8192` |
| 单个UB缓冲区预留长度 | 21840个half元素 | 对应`MAX_DATA_COPY_LENGTH`，用于划分各缓冲区的UB地址。 |
| 满块有效元素数 | 21760 | 对应`DATA_COPY_LENGTH`，取不超过21840且满足512字节对齐的最大长度。 |
| 尾块有效元素数 | 4096 | `1048576 % 21760` |
| 每核循环次数 | 49 | `1048576 / 21760`的商为48，余4096个元素，即48个满块和1个尾块。 |
| VF向量长度 | 256字节 | 每次可处理128个half元素；满块执行170次VF计算，尾块执行32次。 |
| 融合递推次数 | `0`、`2`、`4`、`8`、`12` | 对应`FUSED_ITERATIONS`，默认值为`8`。 |
| 每个VF迭代的算术指令数 | `1 + 2 * FUSED_ITERATIONS` | 1次初始Add，每轮递推增加1次Mul和1次Add。 |

编译参数`SCENARIO_NUM`决定使用单Buffer还是Ping/Pong双Buffer，`FUSED_ITERATIONS`决定融合递推次数。对比Buffer场景时，两个场景必须使用相同的`FUSED_ITERATIONS`。

**表4**  场景配置

| `SCENARIO_NUM` | Kernel | 场景 | UB缓冲区配置 | UB地址空间 | 流水关系 |
| --- | --- | --- | --- | --- | --- |
| 0 | `add_baseline` | 未开启Ping/Pong Buffer优化 | 一组x/y/z缓冲区 | `3 * 21840 * sizeof(half)` = 131040字节 | 搬入、计算、搬出完成后再处理下一块。 |
| 1 | `add_double_buffer` | 开启Ping/Pong Buffer优化 | Ping/Pong两组x/y/z缓冲区 | `6 * 21840 * sizeof(half)` = 262080字节 | MTE2、Vector、MTE3处理不同缓冲区，形成跨轮流水重叠。 |

### 场景0：未开启Ping/Pong Buffer优化

`add_baseline`只分配一组连续排列的x、y、z Buffer。每个Buffer预留21840个half元素，实际读写长度由`current_length`指定。

**表5**  未开启Ping/Pong Buffer优化时的UB布局

| UB缓冲区 | 起始偏移（half元素） | 满块有效元素数 | 用途 |
| --- | --- | --- | --- |
| `x_local` | 0 | 21760 | 保存当前轮从GM搬入的x。 |
| `y_local` | 21840 | 21760 | 保存当前轮从GM搬入的y。 |
| `z_local` | 43680 | 21760 | 保存当前轮的融合计算结果。 |

每轮按照以下顺序处理一个数据块：

1. MTE2调用两次`asc_copy_gm2ub_align`，将当前块的x和y从GM搬入UB；
2. MTE2释放Mutex 0后，Vector获取该Mutex并调用`fused_compute_vf`。每个VF迭代执行`1 + 2 * FUSED_ITERATIONS`条算术指令；
3. Vector释放Mutex 0后，MTE3获取该Mutex并调用`asc_copy_ub2gm_align`，将z搬回GM；
4. MTE3释放Mutex 0后，下一轮MTE2才能覆盖这组UB地址。循环结束时调用`asc_sync_pipe(PIPE_ALL)`等待所有操作完成。

三个阶段共用一个Mutex，执行顺序固定为MTE2、Vector、MTE3。下一轮必须等当前轮搬出完成后才能复用UB，因此不同轮次之间不能并行。该实现作为性能基线。

### 场景1：开启Ping/Pong Buffer优化

`add_double_buffer`在UB中分配Ping和Pong两组x、y、z Buffer。偶数轮使用Ping，奇数轮使用Pong。每组使用两个Mutex：输入Mutex管理x、y的搬入和读取，输出Mutex管理z的写入和搬出。

**表6**  开启Ping/Pong Buffer优化时的缓冲区布局

| 轮次 | 缓冲区组 | 输入Mutex ID | 输出Mutex ID | x起始偏移 | y起始偏移 | z起始偏移 |
| --- | --- | --- | --- | --- | --- | --- |
| 偶数轮 | Ping | 0 | 2 | 0 | 21840 | 43680 |
| 奇数轮 | Pong | 1 | 3 | 65520 | 87360 | 109200 |

表中的偏移以half元素为单位，每个Buffer预留21840个元素。双Buffer不改变每轮的搬运量和计算量，`asc_lock`和`asc_unlock`用于控制两组Buffer的读写顺序。

**表7**  开启Ping/Pong Buffer优化时的同步关系

| 流水阶段 | Mutex ID | 同步操作 | 保证的依赖 |
| --- | --- | --- | --- |
| MTE2搬入 | 输入Mutex：Ping为0，Pong为1 | 获取输入Mutex后搬入x、y，完成后释放。 | 同一组x、y再次写入前，等待该组上一轮Vector读取完成。 |
| Vector计算 | 当前组的输入Mutex和输出Mutex | 依次获取两个Mutex后执行`fused_compute_vf`，完成后分别释放。 | 等待当前轮x、y搬入完成，并等待上一轮MTE3读完同组z。 |
| MTE3搬出 | 输出Mutex：Ping为2，Pong为3 | 获取输出Mutex后搬出z，完成后释放。 | 等待当前轮Vector写完z。 |

`loop_idx & 1`用于选择Ping或Pong。输入Mutex为0/1，对应的输出Mutex为2/3。输入Mutex保护x、y的搬入和读取，输出Mutex保护z的写入和搬出。

Vector读完x、y并释放输入Mutex后，MTE2可以向同组Buffer搬入下一块数据。MTE3通过输出Mutex等待z计算完成。MTE2不访问z，MTE3不访问x和y，因此搬入和搬出可以同时执行。49轮指令下发完毕后，调用`asc_sync_pipe(PIPE_ALL)`等待所有操作结束。接口说明参见[asc_lock](../../../../../../docs/zh/api/SIMD-API/c_api/sync/asc_lock.md)和[asc_unlock](../../../../../../docs/zh/api/SIMD-API/c_api/sync/asc_unlock.md)。

以连续三轮为例，第0轮使用Ping和Mutex 0/2，第1轮使用Pong和Mutex 1/3，第2轮重新使用Ping和Mutex 0/2。第0轮Vector读完x、y后，第2轮MTE2可以覆盖Ping中的x、y，此时第0轮MTE3仍可读取Ping中的z。第2轮Vector写入z之前，仍要等待第0轮MTE3读完z。

**表8**  Ping/Pong Buffer优化对比

| 对比项 | 场景0：未开启Ping/Pong Buffer优化 | 场景1：开启Ping/Pong Buffer优化 |
| --- | --- | --- |
| UB缓冲区 | 一组x/y/z，共131040字节 | 两组x/y/z，共262080字节 |
| 相邻轮次使用的地址 | 相同，必须等待上一轮结束后复用。 | 不同，在Ping和Pong之间交替。 |
| 流水同步 | 使用Mutex 0串联各轮，全部轮次结束后执行一次全同步。 | Ping/Pong的输入Mutex为0/1、输出Mutex为2/3，全部轮次结束后执行一次全同步。 |
| 跨轮流水重叠 | 不支持 | MTE2、Vector、MTE3可处理不同轮次；同组MTE2搬入与MTE3搬出也可并行。 |
| 优化代价 | UB占用较少。 | 以增加一倍UB地址空间换取流水重叠机会。 |

<p align="center">
  <img src="figures/double_buffer.png" width="50%">
</p>

上图展示了相邻数据块在MTE2、Vector和MTE3流水上的重叠关系。

## 性能对比方法

同一组`FUSED_ITERATIONS`测试中，两个场景只改变Buffer数量和同步方式。融合计算量、输入数据和分块参数保持一致。

### 测试条件

**表9**  性能测试配置

| 配置项 | 配置值 |
| --- | --- |
| 芯片版本 | Ascend 950PR |
| CANN软件版本 | CANN 9.2.0 |
| NPU架构 | `dav-3510` |
| 输入/输出数据类型 | half |
| 输入/输出Shape | `[8192, 8192]` |
| AI Vector Core数 | 64 |
| 单次分块长度 | 21760个half元素，尾块为4096个half元素 |
| 融合递推次数 | `0`、`2`、`4`、`8`、`12`，每档分别测试两个Buffer场景 |
| 测试次数 | 每个场景预热1次，正式采集至少5次，取Task Duration中位数 |

测试期间保持设备频率和功耗模式不变，避免其他任务占用NPU。两个场景使用同一组输入数据，并采用相同的核数、分块参数和编译选项。

### 性能指标

使用msOpProf采集性能数据，表10中的指标取自`PipeUtilization.csv`。

**表10**  性能指标说明

| 指标 | 说明 |
| --- | --- |
| `Task Duration(μs)` | 整个Task的执行时间，端到端性能对比以该指标为准。 |
| `aiv_time(μs)` | Task在AI Vector Core上的执行时间。 |
| `aiv_vec_time(μs)` / `aiv_vec_ratio` | Vector计算指令耗时及其占比。 |
| `aiv_scalar_time(μs)` / `aiv_scalar_ratio` | Scalar指令耗时及其占比。 |
| `aiv_mte2_time(μs)` / `aiv_mte2_ratio` | MTE2流水执行GM到UB搬入的耗时及其占比。 |
| `aiv_mte3_time(μs)` / `aiv_mte3_ratio` | MTE3流水执行UB到GM搬出的耗时及其占比。 |

Ping/Pong双Buffer会使不同流水重叠，各流水耗时或占比不能直接相加。端到端性能以`Task Duration`为准，其他指标用于定位主要耗时。

### Ascend 950PR性能对比

表11为原始逐元素Add配置的测试结果，对应`FUSED_ITERATIONS=0`。这些数据用于说明计算量较小时的表现，不能作为其他融合档位的结果。

**表11**  `FUSED_ITERATIONS=0`时的Ping/Pong Buffer性能对比

| 场景 | 优化配置 | Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 场景0 | 未开启Ping/Pong Buffer优化 | 273.59 | 272.31 | 11.225 | 0.041 | 1.739 | 0.006 | 190.77 | 0.701 | 66.739 | 0.245 |
| 场景1 | 开启Ping/Pong Buffer优化 | 267.841 | 266.36 | 10.958 | 0.041 | 2.153 | 0.008 | 262.328 | 0.985 | 145.807 | 0.547 |

#### 端到端结果

以场景0为基线，开启Ping/Pong Buffer优化后的加速比为

$$
S = \frac{T_{\text{baseline}}}{T_{\text{ping-pong}}}
  = \frac{273.59}{267.841}
  \approx 1.021
$$

端到端耗时下降比例为

$$
R = \frac{T_{\text{baseline}} - T_{\text{ping-pong}}}{T_{\text{baseline}}} \times 100\%
  = \frac{273.59 - 267.841}{273.59} \times 100\%
  \approx 2.10\%
$$

`Task Duration`从273.59μs降至267.841μs，减少5.749μs，降幅为2.10%。`aiv_time`从272.31μs降至266.36μs，减少5.95μs，变化趋势与`Task Duration`一致。

两个场景的`aiv_vec_time`分别为11.225μs和10.958μs，相差0.267μs，Vector计算耗时基本不变。`aiv_scalar_time`增加了0.414μs，但在总耗时中的占比仍不到1%，对端到端性能影响较小。

#### 理论性能分析

按Ascend 950PR每cycle处理128个half数据、主频1.65GHz和64个AI Vector Core计算，`FUSED_ITERATIONS=0`时理想Add吞吐对应的Vector耗时为

$$
T_{\text{theory}}
= \frac{M \times N}{128 \times 1.65 \times 10^9 \times \text{核数}}
= \frac{8192 \times 8192}{128 \times 1.65 \times 10^9 \times 64}
\approx 4.965\ \mu s
$$

该理论值只包含Add指令，不包含VF循环、地址计算、数据搬运和同步。实测`aiv_vec_time`约为11μs，是理论值的2.2倍，但仍远小于MTE2耗时。增加`FUSED_ITERATIONS`后，应以实测VEC时间为准，不能按指令数直接线性推算。

#### 流水与带宽分析

场景0的MTE2、MTE3和Vector流水占比之和为98.7%，三个阶段基本串行。场景1的占比之和为157.3%，超过100%，说明搬入、计算和搬出已在不同轮次间重叠执行。

Profiler中的MTE2和MTE3耗时包含流水执行和等待时间。双Buffer改变了流水重叠关系，不能根据这两个指标判断单次搬运是否变慢。

场景0的读写流水基本串行，按输入总数据量估算读带宽：

$$
D_{\text{read}} = M \times N \times sizeof(half) \times 2 = 268435456\ \text{B}
$$

$$
BW_{\text{read}} = \frac{D_{\text{read}}}{T_{mte2}}
= \frac{268435456}{190.77 \times 10^{-6}}
\approx 1.407\ \text{TB/s}
$$

场景1中MTE2和MTE3并行，按输入和输出总数据量估算读写混合带宽：

$$
D_{\text{write}} = M \times N \times sizeof(half) = 134217728\ \text{B}
$$

$$
BW_{\text{rw}} = \frac{D_{\text{read}} + D_{\text{write}}}{T_{mte2}}
= \frac{402653184}{262.328 \times 10^{-6}}
\approx 1.535\ \text{TB/s}
$$

场景0的读带宽估算值为1.407TB/s，场景1的读写混合带宽估算值为1.535TB/s。一个只统计读数据，另一个同时统计读写数据，不能用两者的差值表示性能提升幅度。

场景1的`aiv_mte2_ratio`为98.5%，主要耗时仍在GM数据搬运。`FUSED_ITERATIONS=0`时Vector计算时间较短，可通过流水重叠隐藏的时间有限，端到端耗时下降2.10%。

#### 融合计算分档测试

按表12记录各档数据。先观察场景0中的`aiv_vec_time`与`aiv_mte2_time`，选择两者最接近的融合档位，再比较该档位下场景0和场景1的`Task Duration`。建议将`aiv_vec_time / aiv_mte2_time`处于`0.8`到`1.2`之间的档位作为重点分析对象。

**表12**  融合计算性能记录

| `FUSED_ITERATIONS` | 算术指令数 | `SCENARIO_NUM` | Buffer配置 | Task Duration(μs) | aiv_vec_time(μs) | aiv_mte2_time(μs) | aiv_mte3_time(μs) | 相对场景0降幅 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0 | 1 | 0/1 | 单Buffer/双Buffer | 见表11 | 见表11 | 见表11 | 见表11 | 见表11 |
| 2 | 5 | 0/1 | 单Buffer/双Buffer | 待采集 | 待采集 | 待采集 | 待采集 | 待计算 |
| 4 | 9 | 0/1 | 单Buffer/双Buffer | 待采集 | 待采集 | 待采集 | 待采集 | 待计算 |
| 8 | 17 | 0/1 | 单Buffer/双Buffer | 待采集 | 待采集 | 待采集 | 待采集 | 待计算 |
| 12 | 25 | 0/1 | 单Buffer/双Buffer | 待采集 | 待采集 | 待采集 | 待采集 | 待计算 |

每个融合档位都必须重新生成匹配的`golden.bin`，并分别完成场景0和场景1的精度校验。不同`FUSED_ITERATIONS`对应不同计算任务，不能直接用它们的Task Duration计算双Buffer加速比。

#### 总结

Ping/Pong双Buffer额外占用131040字节UB，使MTE2、Vector和MTE3能够跨轮并行。`FUSED_ITERATIONS=0`时，`Task Duration`减少5.749μs，降幅为2.10%。融合计算档位用于提高Vector计算占比，并观察VEC与MTE2耗时接近时双Buffer的实际收益；最终结论以同一融合档位的实测结果为准。

## 编译运行

配置CANN环境变量：

```bash
source ${install_path}/cann/set_env.sh
```

在本样例目录编译并运行指定场景：

```bash
SCENARIO_NUM=0
FUSED_ITERATIONS=8
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
    -DSCENARIO_NUM=${SCENARIO_NUM} \
    -DFUSED_ITERATIONS=${FUSED_ITERATIONS} ..
make -j
python3 ../scripts/gen_data.py --fused-iterations ${FUSED_ITERATIONS}
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

修改`FUSED_ITERATIONS`后，CMake参数和`gen_data.py`参数必须保持一致。将`SCENARIO_NUM`改为`1`即可测试相同融合计算下的Ping/Pong双Buffer场景。切换编译参数前需要清理build目录中的CMake缓存。

**表13**  编译选项

| 编译选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构 |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式 |
| `SCENARIO_NUM` | `0`、`1`（默认） | 未开启Ping/Pong Buffer优化、开启Ping/Pong Buffer优化 |
| `FUSED_ITERATIONS` | `0`、`2`、`4`、`8`（默认）、`12` | $t_{i+1}=t_i \times x+y$的递推次数。 |

精度校验成功时输出：

```text
test pass!
```

## 功能调试

### printf

`printf`接口提供SIMD和SIMT编程调试场景下的格式化输出功能。在Kernel侧需要输出核索引、分块参数或中间状态时，可以调用[printf](../../../../../../docs/zh/api/Utils-API/tuning_interface/printf.md)接口打印相关内容。使用该接口需要包含头文件`utils/debug/asc_printf.h`。

本样例使用C API编程，获取当前核索引时应调用[asc_get_block_idx](../../../../../../docs/zh/api/SIMD-API/c_api/sys_var/asc_get_block_idx.md)，不使用C++ API中的`AscendC::GetBlockIdx`。示例如下：

```cpp
#include "utils/debug/asc_printf.h"

__vector__ __global__ void add_debug()
{
    printf("add blockIdx=%ld\n", asc_get_block_idx());
}
```

> **注意：** `printf`接口会增加算子的执行时间并影响性能测试结果，通常只在功能调试阶段使用。进行性能采集前应删除或关闭打印代码，也可以按需设置`ASCENDC_DUMP=0`关闭打印功能。

### msOpProf工具介绍
msOpProf工具是单算子性能分析工具。包含msopprof和msopprof simulator两种使用方式。该工具协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

- 上板性能采集

    通过上板性能采集，可以直接测定算子昇腾AI处理器上的运行时间。该方式适合在板环境中快速定位算子性能问题。

    基于可执行文件`demo`通过msopprof执行算子调优：
    ```bash
    msopprof ./demo
    ```

    - 性能数据说明
      命令完成后，会在默认目录下生成以“OPPROF_{timestamp}_XXX”命名的文件夹,性能数据文件夹结构示例如下：

      ```bash
      ├──dump                       # 原始的性能数据，用户无需关注
      ├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
      ├──L2Cache.csv                # L2 Cache命中率，影响MTE2，建议合理规划数据搬运逻辑，增加命中率
      ├──Memory.csv                 # UB，L1和主存储器读写带宽速率
      ├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
      ├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
      ├──OpBasicInfo.csv            # 算子基础信息
      ├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
      ├──ResourceConflictRatio.csv  # UB上的bank group、bank conflict和资源冲突率在所有指令中的占比
      └──visualize_data.bin         # MindStudio Insight呈现文件
      ```

查看具体的性能分析结果：

```bash
# 查看Task Duration 以及各项数据
cat ./OPPROF_*/PipeUtilization.csv
```
