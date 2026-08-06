# Matmul C-API高性能样例

## 概述

本样例使用Ascend C C-API实现float16矩阵乘，并提供单缓冲基线和L1 Buffer/L0 Buffer Ping/Pong双缓冲两个场景。两个场景使用相同的矩阵规格、Cube分块、核数和计算接口，用于对比三级流水重叠带来的性能变化。

## 支持的产品及CANN软件版本

**表1**  支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```text
matmul_c_api_high_performance
├── scripts
│   ├── gen_data.py          # 生成输入数据和真值数据
│   └── verify_result.py     # 校验输出结果
├── CMakeLists.txt           # 编译工程文件
├── data_utils.h             # 二进制数据读写工具
├── mmad_double_buffer.asc   # Kernel及Host侧实现
├── README.md
└── README_en.md
```

## 样例描述

样例计算`C = A x B`。输入A和B由固定随机种子生成；B在写入文件前完成转置，使K轴在GM中连续，便于按照K块搬运。输入、输出及用于精度校验的真值数据如下表所示。

**表2**  输入输出数据规格

| 数据 | 输入/输出 | 数据文件 | 数据类型 | 逻辑Shape | 文件及GM排布 | 数据说明 |
| --- | --- | --- | --- | --- | --- | --- |
| A | 输入 | `input/x1_gm.bin` | float16 | `[M, K] = [1024, 1024]` | ND，按`[M, K]`存储 | 在区间`[-0.1, 0.1)`内随机生成。 |
| B | 输入 | `input/x2_gm.bin` | float16 | `[K, N] = [1024, 1024]` | 转置后的ND，按`[N, K]`存储 | 在区间`[-0.1, 0.1)`内随机生成。 |
| C | 输出 | `output/output.bin` | float32 | `[M, N] = [1024, 1024]` | ND，按`[M, N]`存储 | Kernel计算结果 |
| golden | 真值 | `output/golden.bin` | float32 | `[M, N] = [1024, 1024]` | ND，按`[M, N]`存储 | 使用float32输入执行`np.matmul`生成。 |

矩阵沿M、N和K轴进行分块，分块及多核计算参数如下表所示。

**表3**  矩阵分块及多核计算参数

| 参数 | 取值 | 说明 |
| --- | --- | --- |
| 矩阵规格 | `[M, N, K] = [1024, 1024, 1024]` | `C[M, N] = A[M, K] x B[K, N]` |
| 基本块规格 | `[TILE_M, TILE_N, TILE_K] = [128, 128, 64]` | 每轮MMAD的M、N、K范围 |
| M轴分块数 | 8 | `MATRIX_M / TILE_M` |
| N轴分块数 | 8 | `MATRIX_N / TILE_N` |
| 逻辑Cube核数 | 64 | `8 * 8`，每个核计算一个输出块。 |
| 单核输出块 | `[128, 128]` float32 | 共16384个元素，占65536字节L0C Buffer空间。 |
| K轴循环次数 | 16 | `MATRIX_K / TILE_K` |
| 单轮A数据块 | `[128, 64]` float16 | 共8192个元素，占16384字节。 |
| 单轮B数据块 | `[128, 64]` float16 | B转置后的`[N, K]`数据块，共8192个元素，占16384字节。 |

`block_idx`用于确定每个逻辑Cube核负责的输出块：`m_tile_idx = block_idx % 8`，`n_tile_idx = block_idx / 8`。每个核固定处理一个`[128, 128]`的C块，并沿K轴依次累加16个A/B数据块。第0轮调用`asc_mmad`时初始化L0C Buffer，后续15轮在同一L0C Buffer块上继续累加。

一次完整矩阵乘涉及的数据搬运路径为GM -> L1 Buffer -> L0A Buffer/L0B Buffer -> L0C Buffer -> GM，各存储单元的数据排布如下表所示。

**表4**  数据搬运路径及数据排布

| 存储单元 | 数据 | 数据排布 | 搬运或计算接口 |
| --- | --- | --- | --- |
| GM | A、转置后的B | ND | - |
| L1 Buffer | A、B数据块 | Nz | `asc_set_gm2l1_nz_para`、`asc_copy_gm2l1_nd2nz` |
| L0A Buffer | A数据块 | Nz | `asc_copy_l12l0a` |
| L0B Buffer | B数据块 | Zn | `asc_copy_l12l0b` |
| L0C Buffer | C输出块 | Nz，float32 | `asc_mmad` |
| GM | C | ND，float32 | `asc_set_l0c2gm_nz2nd`、`asc_copy_l0c2gm` |

编译参数`SCENARIO_NUM`用于选择单缓冲基线或L1 Buffer/L0 Buffer双缓冲优化。两个场景的输入、矩阵规格、分块方式、核数和MMAD计算量完全相同，仅A/B缓冲区数量和流水同步方式不同。

**表5**  场景配置

| `SCENARIO_NUM` | Kernel | 场景 | L1 Buffer | L0A Buffer/L0B Buffer | L0C Buffer | 流水关系 |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | `mmad_baseline` | 单缓冲基线 | A、B各一块，共32768字节 | A、B各一块，共32768字节 | 一块，共65536字节 | 每个K块完成搬入、加载、MMAD和全流水同步后再处理下一块。 |
| 1 | `mmad_double_buffer` | 双缓冲优化 | A、B各两块Ping/Pong，共65536字节 | A、B各两块Ping/Pong，共65536字节 | 一块，共65536字节 | MTE2、MTE1、M流水处理不同K块并形成跨轮重叠。 |

### 场景0：单缓冲基线

基线Kernel `mmad_baseline`在L1 Buffer、L0A Buffer和L0B Buffer中只为A/B各分配一块缓冲区，16轮K循环反复使用相同的缓冲区。各缓冲区的用途如下表所示。

**表6**  单缓冲区配置

| 缓冲区 | 数组 | 元素数 | 字节数 | 用途 |
| --- | --- | --- | --- | --- |
| L1 Buffer A | `a_l1` | 8192个float16 | 16384 | 保存当前K轮从GM搬入并转换为Nz的A块。 |
| L1 Buffer B | `b_l1` | 8192个float16 | 16384 | 保存当前K轮从GM搬入并转换为Nz的B块。 |
| L0A Buffer | `a_l0` | 8192个float16 | 16384 | 保存当前K轮参与MMAD的A块。 |
| L0B Buffer | `b_l0` | 8192个float16 | 16384 | 保存当前K轮参与MMAD的B块。 |
| L0C Buffer | `c_l0` | 16384个float32 | 65536 | 累加16轮MMAD得到的C输出块。 |

每轮K循环按照以下顺序执行：

1. MTE2流水分别调用`copy_a_to_l1`和`copy_b_to_l1`，通过`asc_copy_gm2l1_nd2nz`将当前A/B块从GM搬入L1 Buffer并由ND转换为Nz；
2. MTE2完成搬入并调用`asc_unlock`释放Mutex后，MTE1通过`asc_lock`获取相同Mutex，随后调用`asc_copy_l12l0a`和`asc_copy_l12l0b`将A/B加载到L0A Buffer/L0B Buffer；
3. MTE1完成加载并释放Mutex后，M流水获取相同Mutex并调用`asc_mmad`计算`[128, 64] x [64, 128]`。第0轮初始化L0C Buffer，后续轮次将结果累加到L0C Buffer；
4. 每轮末尾调用`asc_sync_pipe(PIPE_ALL)`，等待当前轮的MTE2、MTE1和M流水全部完成后，下一轮才能覆盖L1 Buffer和L0 Buffer中的相同缓冲区。

因此，当M流水计算第n个K块时，MTE1不能提前加载第n+1个K块，MTE2也不能提前搬入后续K块。三个阶段在每轮内依次推进，无法在不同K轮之间重叠。该场景作为L1 Buffer/L0 Buffer双缓冲优化的性能基线。

### 场景1：L1 Buffer/L0 Buffer双缓冲

优化Kernel `mmad_double_buffer`分别为L1 Buffer和L0 Buffer中的A/B数据分配Ping/Pong两组缓冲区。偶数K轮使用Ping，其L1 Buffer和L0 Buffer同步分别使用Mutex 0和Mutex 2；奇数K轮使用Pong，其L1 Buffer和L0 Buffer同步分别使用Mutex 1和Mutex 3。相邻K轮不会读写L1 Buffer或L0 Buffer中的同一组缓冲区。L0C Buffer不参与Ping/Pong切换，16轮MMAD始终累加到同一个`c_l0`输出块。

**表7**  Ping/Pong Buffer配置

| K轮次 | 缓冲区组 | L1 Mutex ID | L0 Mutex ID | L1 Buffer | L0 Buffer |
| --- | --- | --- | --- | --- | --- |
| 偶数轮 | Ping | `0` | `2` | `a_l1_ping`、`b_l1_ping` | `a_l0_ping`、`b_l0_ping` |
| 奇数轮 | Pong | `1` | `3` | `a_l1_pong`、`b_l1_pong` | `a_l0_pong`、`b_l0_pong` |

循环内通过[asc_lock](../../../../../docs/zh/api/SIMD-API/c_api/sync/asc_lock.md)和[asc_unlock](../../../../../docs/zh/api/SIMD-API/c_api/sync/asc_unlock.md)分别建立L1 Buffer和L0 Buffer的Mutex链，控制缓冲区的生产、消费与复用。L1和L0使用不同的Mutex ID，使MTE2只需等待MTE1读完L1 Buffer，不必等待当前轮MMAD结束。

**表8**  双缓冲同步关系

| 同步方向 | Mutex ID | 保证的依赖 | 缓冲区复用条件 |
| --- | --- | --- | --- |
| `MTE2 -> MTE1` | L1 Mutex：`0/1` | MTE2完成A/B搬入并释放Mutex后，MTE1才能获取相同Mutex并读取当前组L1 Buffer。 | 防止MTE1读取L1 Buffer中尚未搬完的数据。 |
| `MTE1 -> MTE2` | L1 Mutex：`0/1` | MTE1读取L1 Buffer并释放Mutex后，后续复用该组缓冲区的MTE2才能获取相同Mutex。 | 防止MTE2提前覆盖L1 Buffer中仍在加载的数据。 |
| `MTE1 -> M` | L0 Mutex：`2/3` | MTE1完成A/B加载并释放Mutex后，M流水才能获取相同Mutex并执行当前轮MMAD。 | 防止MMAD读取L0 Buffer中尚未加载完成的数据。 |
| `M -> MTE1` | L0 Mutex：`2/3` | MMAD读取L0 Buffer并释放Mutex后，后续复用该组缓冲区的MTE1才能获取相同Mutex。 | 防止MTE1提前覆盖L0 Buffer中仍在计算的数据。 |

以连续三个K轮为例：第0轮使用Ping，第1轮使用Pong，第2轮再次使用Ping。当M流水使用Ping计算第0轮时，MTE1可以将第1轮Pong数据从L1 Buffer加载到L0 Buffer，MTE2还可以将第2轮数据搬入L1 Buffer中已经释放的Ping缓冲区。进入稳定流水后，MTE2、MTE1和M流水可分别处理相邻的不同K块，从而隐藏部分GM到L1 Buffer搬运及L1 Buffer到L0 Buffer加载的等待时间。

16轮K循环结束后，FIX流水获取最后一轮使用的L0 Mutex。该Mutex由最后一次MMAD释放，因此可以保证L0C Buffer累加完成。FIX流水随后调用`asc_copy_l0c2gm`将L0C Buffer中的Nz结果转换为ND并写回GM，最后通过`asc_sync_pipe(PIPE_ALL)`等待全部流水完成。

两个场景的核心区别如下表所示。

**表9**  单缓冲与双缓冲对比

| 对比项 | 场景0：单缓冲基线 | 场景1：L1 Buffer/L0 Buffer双缓冲 |
| --- | --- | --- |
| L1 Buffer中的A/B缓冲区 | 每个矩阵一块，共32768字节 | 每个矩阵两块Ping/Pong，共65536字节。 |
| L0A Buffer/L0B Buffer | 每个矩阵一块，共32768字节 | 每个矩阵两块Ping/Pong，共65536字节。 |
| L0C Buffer | 一块，累加16轮MMAD | 一块，累加16轮MMAD。 |
| 相邻K轮使用的A/B缓冲区 | 相同，必须等待上一轮结束后复用。 | 不同，在Ping和Pong之间交替。 |
| 轮末同步 | 每轮执行`asc_sync_pipe(PIPE_ALL)` | 循环内使用两组L1 Mutex和两组L0 Mutex管理局部依赖，仅在K循环结束后执行全流水同步。 |
| 跨轮流水重叠 | 不支持 | MTE2、MTE1、M流水可处理不同K块。 |
| 优化代价 | L1 Buffer和L0 Buffer占用较少。 | A/B缓冲区在L1 Buffer和L0 Buffer中的占用均增加一倍。 |

## 性能对比方法

两个场景除缓冲区数量和同步方式外，其余计算条件一致。测试时应在同一硬件、CANN版本和系统负载条件下分别采集两个场景，且每次性能采集前均需通过精度校验。

### 测试条件

**表10**  性能测试配置

| 配置项 | 配置值 |
| --- | --- |
| 芯片版本 | Ascend 950PR |
| CANN软件版本 | 待补充（不低于CANN 9.1.0） |
| NPU架构 | `dav-3510` |
| 输入数据类型 | A、B为float16 |
| 输出数据类型 | C为float32 |
| 矩阵Shape | `M=N=K=1024` |
| Cube Tile | `TILE_M=128`、`TILE_K=64`、`TILE_N=128` |
| Block数 | 64 |
| K循环次数 | 每个输出Tile执行16轮MMAD累加 |
| 测试次数 | 每个场景预热1次，正式采集至少5次，取Task Duration中位数 |

测试期间应保持设备频率、功耗模式及系统负载一致，避免其他任务占用NPU。两个场景必须使用同一组输入数据，并保持矩阵Shape、分块参数、Block数和编译选项一致。

### 性能指标

性能数据通过msOpProf工具采集，主要查看`PipeUtilization.csv`中的以下字段。

**表11**  性能指标说明

| 指标 | 说明 |
| --- | --- |
| `Task Duration(μs)` | 整个Task的执行时间，端到端性能对比以该指标为准。 |
| `Block Num` | Kernel启动的Block数量，本样例应为64。 |
| `aicore_time(μs)` | Task在AI Core上的平均执行时间。 |
| `aic_mac_time(μs)` / `aic_mac_ratio` | Cube计算单元执行MMAD的耗时及其占比。 |
| `aic_scalar_time(μs)` / `aic_scalar_ratio` | Scalar指令耗时及其占比。 |
| `aic_mte1_time(μs)` / `aic_mte1_ratio` | MTE1流水执行L1 Buffer到L0A Buffer/L0B Buffer加载的耗时及其占比。 |
| `aic_mte2_time(μs)` / `aic_mte2_ratio` | MTE2流水执行GM到L1 Buffer搬入的耗时及其占比。 |
| `aic_fixpipe_time(μs)` / `aic_fixpipe_ratio` | FIX流水执行L0C Buffer到GM搬出的耗时及其占比。 |

开启L1 Buffer/L0 Buffer双缓冲后，MTE2、MTE1和M流水可以重叠执行，因此各流水耗时或占比不能简单相加得到`Task Duration`。最终优化效果应以`Task Duration`为主，并结合MTE2、MTE1、Cube和FIX指标判断瓶颈是否发生变化。

### Ascend 950PR性能对比

下表为本样例在Ascend 950PR芯片上的性能数据。

**表12**  L1 Buffer/L0 Buffer双缓冲性能对比

| 场景 | 优化配置 | Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 场景0 | 单缓冲基线 | 37.684 | 64 | 36.47 | 7.06 | 0.194 | 1.387 | 0.038 | 3.147 | 0.086 | 21.169 | 0.58 | 1.287 | 0.035 |
| 场景1 | L1 Buffer/L0 Buffer双缓冲 | 20.792 | 64 | 19.9 | 7.213 | 0.362 | 1.698 | 0.085 | 4.099 | 0.206 | 14.863 | 0.747 | 1.325 | 0.067 |

**优化效果分析**：

设场景0的`Task Duration`为$T_{\text{baseline}}$，场景1的`Task Duration`为$T_{\text{double-buffer}}$。以场景0为基线，双缓冲加速比为

$$
S = \frac{T_{\text{baseline}}}{T_{\text{double-buffer}}}
  = \frac{37.684}{20.792}
  \approx 1.812
$$

端到端耗时下降比例为

$$
R = \frac{T_{\text{baseline}} - T_{\text{double-buffer}}}{T_{\text{baseline}}} \times 100\%
  = \frac{37.684 - 20.792}{37.684} \times 100\%
  \approx 44.83\%
$$

- `Task Duration`由37.684μs下降至20.792μs，减少16.892μs，端到端耗时下降44.83%，对应约1.812倍加速。
- `aicore_time`由36.47μs下降至19.9μs，下降45.43%，与`Task Duration`的变化趋势一致，说明收益主要来自Kernel内部流水优化。
- `aic_mac_time`由7.06μs变为7.213μs，仅增加0.153μs（2.17%），说明两个场景的Cube计算量和计算效率基本一致，性能提升并非来自计算量变化。
- `aic_mte2_time`由21.169μs下降至14.863μs，下降6.306μs（29.79%），说明双缓冲减少了GM到L1 Buffer搬入流水的等待，是端到端耗时下降的主要来源。
- `aic_mte1_time`由3.147μs增加至4.099μs，`aic_scalar_time`由1.387μs增加至1.698μs，分别增加30.25%和22.42%。双缓冲引入了Ping/Pong选择和Mutex管理，并改变了流水等待分布；这些指标包含流水执行和等待时间，不能直接解释为单次搬运或Scalar指令变慢。两项增加的绝对耗时均小于1μs，没有抵消MTE2与端到端收益。
- `aic_fixpipe_time`由1.287μs变为1.325μs，仅增加0.038μs，两个场景的结果搬出开销基本一致。

**理论性能分析**：

本样例的矩阵规格为$M=N=K=1024$，使用64个AI Core。Ascend 950PR的Cube主频为1.65GHz，每个AI Core每cycle可完成$16\times16\times16$次乘加运算。仅考虑理想Cube计算吞吐，不包含数据搬运、格式转换、流水同步和指令调度等开销时，理论Cube耗时为

$$
T_{\text{cube,theory}}
= \frac{M \times N \times K}
       {16 \times 16 \times 16 \times \text{核数} \times \text{Cube主频}}
= \frac{1024 \times 1024 \times 1024}
       {16 \times 16 \times 16 \times 64 \times 1.65 \times 10^9}
\approx 2.482\ \mu s
$$

场景0的`aic_mac_time`为7.06μs，约为理论Cube耗时的2.844倍，相对理论值的偏差为

$$
E_{\text{cube,baseline}}
= \frac{7.06 - 2.482}{2.482} \times 100\%
\approx 184.40\%
$$

场景1的`aic_mac_time`为7.213μs，约为理论Cube耗时的2.906倍，相对理论值的偏差为

$$
E_{\text{cube,double-buffer}}
= \frac{7.213 - 2.482}{2.482} \times 100\%
\approx 190.56\%
$$

理论值仅包含理想Cube指令吞吐，不包含K循环控制、地址计算、同步和指令调度等开销。两个场景的实测MAC耗时接近，进一步说明双缓冲没有改变Cube工作量，其性能收益来自搬入、加载与计算的跨轮重叠。

每个输出Tile需要从GM读取对应的A Tile和B Tile。A矩阵会被8个N方向Tile重复读取，B矩阵会被8个M方向Tile重复读取，因此不考虑Cache命中时，Kernel的理论读入数据总量为

$$
D_{\text{read}}
= \left(\frac{N}{TILE_N} \times M \times K
+ \frac{M}{TILE_M} \times K \times N\right) \times sizeof(half)
= 32\ \text{MiB}
$$

float32输出矩阵的写出数据量为

$$
D_{\text{write}} = M \times N \times sizeof(float) = 4\ \text{MiB}
$$

**流水与带宽分析**：

使用上述32MiB理论读入量和`aic_mte2_time`估算聚合有效读带宽，场景0为

$$
BW_{\text{read,baseline}}
= \frac{32\ \text{MiB}}{21.169\ \mu s}
\approx 1.585\ \text{TB/s}
$$

场景1为

$$
BW_{\text{read,double-buffer}}
= \frac{32\ \text{MiB}}{14.863\ \mu s}
\approx 2.258\ \text{TB/s}
$$

场景1的聚合有效读带宽比场景0提高约42.43%。该数值基于各核重复读取的数据总量，包含L2 Cache命中和ND到Nz格式转换的综合效果，不等同于HBM物理带宽。

场景0的MAC、MTE1、MTE2和FIX流水占比之和为89.5%，各阶段基本串行执行。场景1中四条流水占比之和达到138.2%，超过100%，说明MTE2搬入、MTE1加载和Cube计算已经跨K轮重叠。虽然场景1的`aic_mte2_ratio`由58.0%升至74.7%，但其绝对耗时下降了29.79%；占比上升主要是因为总`aicore_time`显著缩短，不能解释为MTE2性能下降。MTE2仍是占比最高的流水，后续优化应继续关注数据搬入效率和数据复用。

#### 双缓冲优化总结

- **当前效果**：场景1通过L1 Buffer和L0 Buffer中的两组Ping/Pong缓冲区，使MTE2搬入、MTE1加载和Cube计算处理不同K Tile。实测`Task Duration`下降44.83%，获得约1.812倍加速；四条主要流水占比之和超过100%，验证了跨轮流水重叠已经生效。
- **优点**：双缓冲在不改变矩阵计算量和结果的情况下显著隐藏数据搬运与加载等待时间。L1和L0使用独立Mutex，MTE2可在MTE1释放L1 Buffer后提前搬入后续数据，无需等待当前MMAD结束，聚合有效读带宽提高约42.43%。
- **局限**：A/B在L1 Buffer中的占用由32768字节增加至65536字节，在L0A Buffer/L0B Buffer中的占用也由32768字节增加至65536字节，并引入Ping/Pong选择和Mutex同步开销。场景1的MTE2占比仍达到74.7%，数据搬入仍是最主要的流水瓶颈。
- **后续优化方向**：在保持两个场景计算条件一致的前提下，可进一步评估不同Tile尺寸、UnitFlag、L2 Cache复用和数据搬运粒度，重点降低MTE2耗时；同时结合多轮测试结果确认1.812倍加速的稳定性，并使用Memory和L2 Cache指标区分HBM访问与Cache复用带来的带宽收益。

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
./c_api_mmad_double_buffer_example
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

将`SCENARIO_NUM`改为`1`并重新执行CMake、编译、运行和校验，即可测试双缓冲场景。切换场景前需要清理build目录中的CMake缓存。

**表13**  编译选项

| 编译选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构 |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式 |
| `SCENARIO_NUM` | `0`、`1`（默认） | 单缓冲基线、双缓冲优化 |

精度校验成功时输出：

```text
test pass!
```
