# Add高性能样例

## 概述

本样例使用Ascend C C API实现float16类型逐元素Add，并提供未开启和开启Ping/Pong Buffer优化两个场景。两个场景使用相同的输入、核数、分块和矢量计算接口，用于观察Ping/Pong Buffer优化对GM/UB数据搬运与Vector计算流水重叠的作用。

## 支持的产品及CANN软件版本

**表 1**  支持的产品及CANN软件版本

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

样例计算两个形状为`[8192, 8192]`的float16矩阵的逐元素加法：

```text
z = x + y
```

输入、输出及用于精度校验的真值数据如下表所示，数据在GM中均按ND格式连续存放。

**表 2**  输入输出数据规格

| 数据 | 输入/输出 | 数据文件 | 数据类型 | Shape | 数据说明 |
| --- | --- | --- | --- | --- | --- |
| x | 输入 | `input/input_x.bin` | float16 | `[8192, 8192]` | 在区间`[1, 100)`内随机生成。 |
| y | 输入 | `input/input_y.bin` | float16 | `[8192, 8192]` | 在区间`[1, 100)`内随机生成。 |
| z | 输出 | `output/output.bin` | float16 | `[8192, 8192]` | Kernel计算结果 |
| golden | 真值 | `output/golden.bin` | float16 | `[8192, 8192]` | 使用`(x + y).astype(np.float16)`生成。 |

两个场景采用相同的多核切分和单轮数据块大小。矩阵沿M轴均分到64个核，每个核处理连续的128行，即1048576个元素。每核共执行49轮，前48轮为满块，最后一轮处理尾块。

**表 3**  多核切分和分块参数

| 参数 | 取值 | 说明 |
| --- | --- | --- |
| 矩阵元素总数 | 67108864 | `8192 * 8192` |
| 核数 | 64 | 每个核处理连续的M轴数据。 |
| 每核处理行数 | 128 | `8192 / 64` |
| 每核处理元素数 | 1048576 | `128 * 8192` |
| 每核循环次数 | 49 | 48个满块和1个尾块 |
| 满块有效元素数 | 21760 | 对应`DATA_COPY_LENGTH`。 |
| 尾块有效元素数 | 4096 | `1048576 - 48 * 21760` |
| 单个UB缓冲区预留长度 | 21840个float16元素 | 对应`MAX_DATA_COPY_LENGTH`，用于划分各缓冲区的UB地址。 |
| VF向量长度 | 256字节 | 每次可处理128个float16元素；满块执行170次VF计算，尾块执行32次。 |

编译参数`SCENARIO_NUM`用于选择未开启或开启Ping/Pong Buffer优化。两个场景的输入数据、多核切分、分块大小和向量计算均保持一致，仅UB缓冲区数量及流水同步方式不同。

**表 4**  场景配置

| `SCENARIO_NUM` | Kernel | 场景 | UB缓冲区配置 | UB地址空间 | 流水关系 |
| --- | --- | --- | --- | --- | --- |
| 0 | `add_baseline` | 未开启Ping/Pong Buffer优化 | 一组x/y/z缓冲区 | `3 * 21840 * sizeof(half)` = 131040字节 | 搬入、计算、搬出完成后再处理下一块。 |
| 1 | `add_double_buffer` | 开启Ping/Pong Buffer优化 | Ping/Pong两组x/y/z缓冲区 | `6 * 21840 * sizeof(half)` = 262080字节 | MTE2、Vector、MTE3处理不同缓冲区，形成跨轮流水重叠。 |

### 场景0：未开启Ping/Pong Buffer优化

未开启Ping/Pong Buffer优化时，Kernel `add_baseline`只分配一组x、y、z缓冲区，三段缓冲区在UB中连续排列。每段预留21840个float16元素，当前轮只读写`current_length`指定的有效数据。

**表 5**  未开启Ping/Pong Buffer优化时的UB布局

| UB缓冲区 | 起始偏移（float16元素） | 满块有效元素数 | 用途 |
| --- | --- | --- | --- |
| `x_local` | 0 | 21760 | 保存当前轮从GM搬入的x。 |
| `y_local` | 21840 | 21760 | 保存当前轮从GM搬入的y。 |
| `z_local` | 43680 | 21760 | 保存当前轮的Add结果。 |

每轮按照以下顺序处理一个数据块：

1. MTE2流水通过两次`asc_copy_gm2ub_align`，分别将当前块的x和y从GM搬入UB；
2. MTE2完成搬入后调用`asc_unlock(PIPE_MTE2, 0)`释放Mutex 0，Vector流水通过`asc_lock(PIPE_V, 0)`等待输入就绪，随后调用`add_vf`。满块按128个float16元素为一组执行170次`asc_add`，尾块执行32次；
3. Vector计算完成后释放Mutex 0，MTE3流水获取同一Mutex后，通过`asc_copy_ub2gm_align`将z从UB搬回GM；
4. 下一轮MTE2再次获取Mutex 0时，会等待上一轮MTE3释放该Mutex，因此只能在上一轮搬出完成后覆盖同一组UB地址。全部循环结束后调用一次`asc_sync_pipe(PIPE_ALL)`完成收尾同步。

因此，假设第n轮正在进行Vector计算，第n+1轮的输入还不能搬入；第n轮正在搬出结果时，第n+1轮的计算也不能开始。各流水只能在单轮内部按依赖执行，无法跨轮重叠，该场景用于提供未开启Ping/Pong Buffer优化时的性能基线。

### 场景1：开启Ping/Pong Buffer优化

开启Ping/Pong Buffer优化时，Kernel `add_double_buffer`将UB划分为两组相互独立的x、y、z缓冲区。偶数轮使用Ping和Mutex 0，奇数轮使用Pong和Mutex 1。相邻轮次使用不同的缓冲区和Mutex，不会读写同一段UB地址，也不会因同步资源相同而相互阻塞。

**表 6**  开启Ping/Pong Buffer优化时的缓冲区布局

| 轮次 | 缓冲区组 | Mutex ID | x起始偏移 | y起始偏移 | z起始偏移 |
| --- | --- | --- | --- | --- | --- |
| 偶数轮 | Ping | 0 | 0 | 21840 | 43680 |
| 奇数轮 | Pong | 1 | 65520 | 87360 | 109200 |

表中的偏移单位均为float16元素。每组缓冲区仍按x、y、z各预留21840个float16元素，单轮的数据搬运量和Vector计算量与未开启Ping/Pong Buffer优化的场景完全相同。区别在于开启优化后不再在每轮末尾执行全流水同步，而是使用`asc_lock`和`asc_unlock`按Mutex管理每组缓冲区的流水依赖。

**表 7**  开启Ping/Pong Buffer优化时的同步关系

| 流水阶段 | Mutex ID | 同步操作 | 保证的依赖 |
| --- | --- | --- | --- |
| MTE2搬入 | Ping为0，Pong为1 | `asc_lock(PIPE_MTE2, mutex_id)`后搬入x、y，完成后`asc_unlock`。 | 同一组缓冲区再次使用前，等待该组上一轮MTE3搬出完成。 |
| Vector计算 | 与当前缓冲区组一致 | `asc_lock(PIPE_V, mutex_id)`后执行`add_vf`，完成后`asc_unlock`。 | Vector等待当前轮MTE2释放Mutex，防止读取尚未搬完的数据。 |
| MTE3搬出 | 与当前缓冲区组一致 | `asc_lock(PIPE_MTE3, mutex_id)`后搬出z，完成后`asc_unlock`。 | MTE3等待当前轮Vector释放Mutex，防止搬出尚未计算完成的数据。 |

每轮根据`loop_idx & 1`选择Mutex 0或1，并依次在MTE2、Vector和MTE3流水上获取、释放该Mutex。同一Mutex将一组缓冲区的三个阶段串联起来，不同Mutex之间互不阻塞。所有49轮指令下发后调用一次`asc_sync_pipe(PIPE_ALL)`，确保最后的搬运和计算结束。接口约束和双Buffer用法可参考[asc_lock](../../../../../docs/zh/api/SIMD-API/C-API/sync/asc_lock.md)和[asc_unlock](../../../../../docs/zh/api/SIMD-API/C-API/sync/asc_unlock.md)。

以连续三轮为例：第0轮使用Ping和Mutex 0，第1轮使用Pong和Mutex 1，第2轮再次使用Ping和Mutex 0。当Vector处理第0轮Ping数据时，MTE2可以搬入第1轮Pong数据；当MTE3搬出第0轮Ping结果时，Vector可以处理第1轮Pong数据。第2轮复用Mutex 0时，需要等待第0轮MTE3释放该Mutex。由此，MTE2、Vector和MTE3能够处理相邻轮次的不同数据块，隐藏部分GM与UB之间的数据搬运等待时间。

两个场景的核心区别如下表所示。

**表 8**  Ping/Pong Buffer优化对比

| 对比项 | 场景0：未开启Ping/Pong Buffer优化 | 场景1：开启Ping/Pong Buffer优化 |
| --- | --- | --- |
| UB缓冲区 | 一组x/y/z，共131040字节 | 两组x/y/z，共262080字节 |
| 相邻轮次使用的地址 | 相同，必须等待上一轮结束后复用。 | 不同，在Ping和Pong之间交替。 |
| 轮末同步 | 使用Mutex 0串联各轮，全部轮次结束后执行一次全同步。 | Ping/Pong分别使用Mutex 0/1管理依赖，全部轮次结束后执行一次全同步。 |
| 跨轮流水重叠 | 不支持 | MTE2、Vector、MTE3可处理不同轮次。 |
| 优化代价 | UB占用较少。 | 以增加一倍UB地址空间换取流水重叠机会。 |

<p align="center">
  <img src="figures/double_buffer.png" width="50%">
</p>

## 性能对比方法

两个场景除缓冲区数量和同步方式外，其余计算条件一致。测试时应在同一硬件、CANN版本和系统负载条件下分别采集两个场景，且每次性能采集前均需通过精度校验。

### 测试条件

**表 9**  性能测试配置

| 配置项 | 配置值 |
| --- | --- |
| 芯片版本 | Ascend 950PR |
| CANN软件版本 | 待补充（不低于CANN 9.1.0） |
| NPU架构 | `dav-3510` |
| 输入/输出数据类型 | float16 |
| 输入/输出Shape | `[8192, 8192]` |
| AI Vector Core数 | 64 |
| 单次分块长度 | 21760个float16元素，尾块为4096个float16元素 |
| 测试次数 | 每个场景预热1次，正式采集至少5次，取Task Duration中位数 |

测试期间应保持设备频率、功耗模式及系统负载一致，避免其他任务占用NPU。两个场景必须使用同一组输入数据，并保持核数、分块参数和编译选项一致。

### 性能指标

性能数据通过msOpProf工具采集，主要查看`PipeUtilization.csv`中的以下字段。

**表 10**  性能指标说明

| 指标 | 说明 |
| --- | --- |
| `Task Duration(μs)` | 整个Task的执行时间，端到端性能对比以该指标为准。 |
| `aiv_time(μs)` | Task在AI Vector Core上的执行时间。 |
| `aiv_vec_time(μs)` / `aiv_vec_ratio` | Vector计算指令耗时及其占比。 |
| `aiv_scalar_time(μs)` / `aiv_scalar_ratio` | Scalar指令耗时及其占比。 |
| `aiv_mte2_time(μs)` / `aiv_mte2_ratio` | MTE2流水执行GM到UB搬入的耗时及其占比。 |
| `aiv_mte3_time(μs)` / `aiv_mte3_ratio` | MTE3流水执行UB到GM搬出的耗时及其占比。 |

开启Ping/Pong Buffer优化后，不同流水可以重叠执行，因此各流水耗时或占比不能简单相加得到`Task Duration`。最终优化效果应以`Task Duration`为主，并结合MTE2、Vector和MTE3指标判断瓶颈是否发生变化。

### 采集步骤

在样例目录执行以下命令。`SCENARIO_NUM=0`表示未开启Ping/Pong Buffer优化，`SCENARIO_NUM=1`表示开启Ping/Pong Buffer优化。两个场景使用独立构建目录，避免CMake缓存相互影响。

```bash
SCENARIO_NUM=0
BUILD_DIR=build_s${SCENARIO_NUM}
cmake -S . -B ${BUILD_DIR} -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM}
cmake --build ${BUILD_DIR} -j

cd ${BUILD_DIR}
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin

# 预热后采集性能数据。
./demo
msopprof ./demo
cat ./OPPROF_*/PipeUtilization.csv
cd ..
```

将`SCENARIO_NUM`改为`1`后重复上述步骤。每个场景至少正式采集5次，记录每次的`Task Duration`，取中位数填入结果表；其余流水指标使用该中位数对应轮次的数据。

### Ascend 950PR性能对比

下表为本样例在Ascend 950PR芯片上运行的性能数据对比。

**表 11**  Ping/Pong Buffer优化性能对比

| 场景 | 优化配置 | Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 场景0 | 未开启Ping/Pong Buffer优化 | 271.472 | 270.18 | 11.117 | 0.041 | 1.553 | 0.006 | 188.566 | 0.698 | 67.182 | 0.249 |
| 场景1 | 开启Ping/Pong Buffer优化 | 268.456 | 267.51 | 10.993 | 0.041 | 1.944 | 0.007 | 258.8 | 0.967 | 137.686 | 0.515 |

**优化效果分析**：

以场景0为基线，开启Ping/Pong Buffer优化后的加速比为

$$
S = \frac{T_{\text{baseline}}}{T_{\text{ping-pong}}}
  = \frac{271.472}{268.456}
  \approx 1.011
$$

端到端耗时下降比例为

$$
R = \frac{T_{\text{baseline}} - T_{\text{ping-pong}}}{T_{\text{baseline}}} \times 100\%
  = \frac{271.472 - 268.456}{271.472} \times 100\%
  \approx 1.11\%
$$

- `Task Duration`由271.472μs下降至268.456μs，减少3.016μs，端到端性能提升约1.11%。
- `aiv_time`由270.18μs下降至267.51μs，下降约0.99%。
- `aiv_vec_time`由11.117μs下降至10.993μs，变化约1.12%，说明两个场景的Vector计算量和计算效率基本一致。
- `aiv_scalar_time`增加0.391μs，虽然相对增幅约25.18%，但绝对耗时和占比均很小，对整体性能影响有限。

**理论性能分析**：

本样例可以计算仅考虑Add向量计算、不包含数据搬运、流水同步、指令调度等开销的理论Vector耗时。Ascend 950PR每cycle可处理128个half数据，主频为1.65GHz，本样例使用64个AI Vector Core，因此理论Vector耗时为

$$
T_{\text{theory}}
= \frac{M \times N}{128 \times 1.65 \times 10^9 \times \text{核数}}
= \frac{8192 \times 8192}{128 \times 1.65 \times 10^9 \times 64}
\approx 4.965\ \mu s
$$

两个场景的`aiv_vec_time`分别为11.117μs和10.993μs，约为理论Vector耗时的2.2倍。理论值只表示Add计算指令在理想吞吐下的耗时，不包含VF循环控制、地址计算、数据搬运和流水同步等实际执行开销，因此不能作为整个Kernel的理论端到端耗时。两个场景的Vector耗时接近，且均远小于MTE2耗时，说明当前端到端瓶颈不在纯Vector计算。

**流水与带宽分析**：

场景0中MTE2、MTE3和Vector流水占比之和为98.8%，各流水基本串行执行。场景1中三条流水占比之和为152.3%，超过100%，说明开启Ping/Pong Buffer后，搬入、计算和搬出已经在不同轮次间发生重叠。

Profiler统计的MTE2和MTE3耗时包含流水执行及等待时间。开启优化后，`aiv_mte2_time`由188.566μs增加至258.8μs，`aiv_mte3_time`由67.182μs增加至137.686μs，这些数值不能直接解释为单次数据搬运变慢，应结合流水重叠和带宽指标判断。

场景0未开启Ping/Pong Buffer，读写流水基本串行，使用输入总数据量估算读带宽：

$$
D_{\text{read}} = M \times N \times sizeof(half) \times 2 = 268435456\ \text{B}
$$

$$
BW_{\text{read}} = \frac{D_{\text{read}}}{T_{mte2}}
= \frac{268435456}{188.566 \times 10^{-6}}
\approx 1.424\ \text{TB/s}
$$

场景1开启Ping/Pong Buffer后，MTE2和MTE3并行执行，使用输入和输出总数据量估算读写混合带宽：

$$
D_{\text{write}} = M \times N \times sizeof(half) = 134217728\ \text{B}
$$

$$
BW_{\text{rw}} = \frac{D_{\text{read}} + D_{\text{write}}}{T_{mte2}}
= \frac{402653184}{258.8 \times 10^{-6}}
\approx 1.556\ \text{TB/s}
$$

场景1的`aiv_mte2_ratio`达到96.7%，说明优化后仍以GM数据搬运为主要瓶颈。Ping/Pong Buffer通过流水重叠提高了读写混合吞吐，但由于Vector计算耗时远小于数据搬运耗时，可被隐藏的计算时间有限，因此端到端收益约为1.11%。

#### 双缓冲优化总结

- **当前效果**：Ping/Pong Buffer通过两组独立UB缓冲区，使MTE2搬入、Vector计算和MTE3搬出能够跨轮重叠。`Task Duration`下降3.016μs，端到端性能提升约1.11%。
- **优点**：在不改变计算结果和单轮计算量的情况下隐藏部分流水等待时间，提高连续分块处理时的读写并行度。当计算耗时与数据搬运耗时接近时，通常可以获得更明显的收益。
- **局限**：UB占用由131040字节增加至262080字节，并引入Ping/Pong地址选择和同步管理开销。本样例实测Vector耗时约11μs，而MTE2耗时超过188μs，计算与搬运耗时差距较大，可被双缓冲隐藏的计算时间有限，因此整体收益受到GM数据搬运瓶颈限制。
- **后续优化方向**：在保持512字节对齐和当前L2 Cache bypass策略的基础上，继续测试不同分块大小和核数配置，优化GM连续访问与搬运参数，减少Scalar地址计算和同步开销。对于实际业务，还可以通过算子融合增加单次数据搬入后的计算量和数据复用率，提高计算与搬运的重叠收益。

## 编译运行

配置CANN环境变量：

```bash
source ${install_path}/cann/set_env.sh
```

在本样例目录编译并运行指定场景：

```bash
SCENARIO_NUM=0
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM} ..
make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

将`SCENARIO_NUM`改为`1`并重新执行CMake、编译、运行和校验，即可测试开启Ping/Pong Buffer优化的场景。切换场景前需要清理build目录中的CMake缓存。

**表 12**  编译选项

| 编译选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构 |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式 |
| `SCENARIO_NUM` | `0`、`1`（默认） | 未开启Ping/Pong Buffer优化、开启Ping/Pong Buffer优化 |

精度校验成功时输出：

```text
test pass!
```

## 功能调试

### printf

`printf`接口提供SIMD和SIMT编程调试场景下的格式化输出功能。在Kernel侧需要输出核索引、分块参数或中间状态时，可以调用[printf](../../../../../docs/zh/api/Utils-API/调测接口/printf.md)接口打印相关内容。使用该接口需要包含头文件`utils/debug/asc_printf.h`。

本样例使用C API编程，获取当前核索引时应调用[asc_get_block_idx](../../../../../docs/zh/api/SIMD-API/C-API/sys_var/asc_get_block_idx.md)，不使用C++ API中的`AscendC::GetBlockIdx`。示例如下：

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
