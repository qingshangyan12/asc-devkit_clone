# Add高性能样例

## 概述

本样例使用Ascend C C API实现float16类型逐元素Add，并提供单缓冲基线和Ping-Pong双缓冲两个场景。两个场景使用相同的输入、核数、分块和矢量计算接口，用于观察双缓冲对GM/UB数据搬运与Vector计算流水重叠的作用。

## 支持的产品及CANN软件版本

**表 1**  支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```text
add_high_performance
├── figures
│   └── double_buffer.png    # 双缓冲流水示意图
├── scripts
│   ├── gen_data.py          # 生成输入数据和真值数据
│   └── verify_result.py     # 校验输出结果
├── CMakeLists.txt           # 编译工程文件
├── c_api_add.asc            # Kernel及Host侧实现
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

编译参数`SCENARIO_NUM`用于选择单缓冲基线或Ping-Pong双缓冲优化。两个场景的输入数据、多核切分、分块大小和向量计算均保持一致，仅UB缓冲区数量及流水同步方式不同。

**表 4**  场景配置

| `SCENARIO_NUM` | Kernel | 场景 | UB缓冲区配置 | UB地址空间 | 流水关系 |
| --- | --- | --- | --- | --- | --- |
| 0 | `add_baseline` | 单缓冲基线 | 一组x/y/z缓冲区 | `3 * 21840 * sizeof(half)` = 131040字节 | 搬入、计算、搬出完成后再处理下一块。 |
| 1 | `add_double_buffer` | 双缓冲优化 | Ping/Pong两组x/y/z缓冲区 | `6 * 21840 * sizeof(half)` = 262080字节 | MTE2、Vector、MTE3处理不同缓冲区，形成跨轮流水重叠。 |

### 场景0：单缓冲基线

基线Kernel `add_baseline`只分配一组x、y、z缓冲区，三段缓冲区在UB中连续排列。每段预留21840个float16元素，当前轮只读写`current_length`指定的有效数据。

**表 5**  单缓冲UB布局

| UB缓冲区 | 起始偏移（float16元素） | 满块有效元素数 | 用途 |
| --- | --- | --- | --- |
| `x_local` | 0 | 21760 | 保存当前轮从GM搬入的x。 |
| `y_local` | 21840 | 21760 | 保存当前轮从GM搬入的y。 |
| `z_local` | 43680 | 21760 | 保存当前轮的Add结果。 |

每轮按照以下顺序处理一个数据块：

1. MTE2流水通过两次`asc_copy_gm2ub_align`，分别将当前块的x和y从GM搬入UB；
2. `MTE2 -> V`同步保证两路输入搬入完成，Vector流水随后调用`add_vf`。满块按128个float16元素为一组执行170次`asc_add`，尾块执行32次；
3. `V -> MTE3`同步保证计算完成，MTE3流水通过`asc_copy_ub2gm_align`将z从UB搬回GM；
4. 每轮末尾调用`asc_sync_pipe(PIPE_ALL)`，等待MTE2、Vector和MTE3全部完成后，下一轮才能覆盖同一组UB地址。

因此，假设第n轮正在进行Vector计算，第n+1轮的输入还不能搬入；第n轮正在搬出结果时，第n+1轮的计算也不能开始。各流水只能在单轮内部按依赖执行，无法跨轮重叠，该场景用于提供未采用双缓冲时的性能基线。

### 场景1：Ping-Pong双缓冲

优化Kernel `add_double_buffer`将UB划分为两组相互独立的x、y、z缓冲区。偶数轮使用Ping和`EVENT_ID0`，奇数轮使用Pong和`EVENT_ID1`，相邻轮次不会读写同一段UB地址。

**表 6**  Ping-Pong缓冲区布局

| 轮次 | 缓冲区组 | Event ID | x起始偏移 | y起始偏移 | z起始偏移 |
| --- | --- | --- | --- | --- | --- |
| 偶数轮 | Ping | `EVENT_ID0` | 0 | 21840 | 43680 |
| 奇数轮 | Pong | `EVENT_ID1` | 65520 | 87360 | 109200 |

表中的偏移单位均为float16元素。每组缓冲区仍按x、y、z各预留21840个float16元素，单轮的数据搬运量和Vector计算量与基线场景完全相同。区别在于不再在每轮末尾执行全流水同步，而是使用`asc_sync_notify`和`asc_sync_wait`分别管理每组缓冲区的生产者与消费者依赖。

**表 7**  双缓冲同步关系

| 同步方向 | 保证的依赖 | 缓冲区复用条件 |
| --- | --- | --- |
| `MTE2 -> V` | x、y搬入完成后，Vector才能读取当前组输入。 | 防止Vector读取尚未搬完的数据。 |
| `V -> MTE2` | Vector读取x、y完成后，MTE2才能向同一组输入缓冲区搬入后续数据。 | 防止MTE2提前覆盖仍在参与计算的输入。 |
| `V -> MTE3` | Vector写入z完成后，MTE3才能搬出当前组结果。 | 防止MTE3搬出尚未计算完成的数据。 |
| `MTE3 -> V` | MTE3搬出z完成后，Vector才能再次写入同一组输出缓冲区。 | 防止Vector提前覆盖尚未搬出的结果。 |

进入循环前，代码先通知两组输入和输出缓冲区均可使用。循环中，每轮只等待本轮对应Event ID的资源状态；Vector读完一组x、y后立即通知MTE2可以复用输入缓冲区，不需要等待该组z搬出完成。所有49轮指令下发后，再等待两组Event并调用一次`asc_sync_pipe(PIPE_ALL)`，确保最后的搬运和计算结束。

以连续三轮为例：第0轮使用Ping，第1轮使用Pong，第2轮再次使用Ping。当Vector处理第0轮Ping数据时，MTE2可以搬入第1轮Pong数据；当MTE3搬出第0轮Ping结果时，Vector可以处理第1轮Pong数据；第0轮的x、y读取完成后，MTE2还可以为第2轮重新填充Ping输入。由此，MTE2、Vector和MTE3能够处理不同轮次的数据块，隐藏部分GM与UB之间的数据搬运等待时间。

两个场景的核心区别如下表所示。

**表 8**  单缓冲与双缓冲对比

| 对比项 | 场景0：单缓冲基线 | 场景1：Ping-Pong双缓冲 |
| --- | --- | --- |
| UB缓冲区 | 一组x/y/z，共131040字节 | 两组x/y/z，共262080字节 |
| 相邻轮次使用的地址 | 相同，必须等待上一轮结束后复用。 | 不同，在Ping和Pong之间交替。 |
| 轮末同步 | 每轮执行`asc_sync_pipe(PIPE_ALL)`。 | 循环内使用两组Event管理局部依赖，全部轮次结束后执行一次全同步。 |
| 跨轮流水重叠 | 不支持 | MTE2、Vector、MTE3可处理不同轮次。 |
| 优化代价 | UB占用较少。 | 以增加一倍UB地址空间换取流水重叠机会。 |

<p align="center">
  <img src="figures/double_buffer.png" width="50%">
</p>

## 性能对比方法

两个场景除缓冲区数量和同步方式外，其余计算条件一致。分别编译`SCENARIO_NUM=0`和`SCENARIO_NUM=1`，使用性能分析工具采集Kernel的Task Duration，以单缓冲场景作为基线：

```text
性能提升比例 = 基线耗时 / 双缓冲耗时 - 1
```

实际收益取决于数据搬运与计算耗时的比例。两者越接近，流水重叠通常越充分；如果瓶颈完全由GM带宽决定，双缓冲收益会受到限制。

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
./c_api_add_example
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

将`SCENARIO_NUM`改为`1`并重新执行CMake、编译、运行和校验，即可测试双缓冲场景。切换场景前需要清理build目录中的CMake缓存。

**表 9**  编译选项

| 编译选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构 |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式 |
| `SCENARIO_NUM` | `0`、`1`（默认） | 单缓冲基线、双缓冲优化 |

精度校验成功时输出：

```text
test pass!
```
