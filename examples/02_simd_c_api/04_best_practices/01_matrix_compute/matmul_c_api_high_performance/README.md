# Matmul C-API高性能样例

## 概述

本样例使用Ascend C C-API实现float16矩阵乘，并对比单缓冲和Ping/Pong双缓冲两种实现。双缓冲交替使用两组L1 Buffer和L0 Buffer，使MTE2搬入、MTE1加载和Cube计算可以跨K轮并行。

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

样例计算`C = A x B`。输入A和B使用固定随机种子生成。B写入文件前先转置，使K轴数据在GM中连续，便于按K块搬运。输入、输出和真值数据见表2。

**表2**  输入输出数据规格

| 数据 | 输入/输出 | 数据文件 | 数据类型 | 逻辑Shape | 文件及GM排布 | 数据说明 |
| --- | --- | --- | --- | --- | --- | --- |
| A | 输入 | `input/x1_gm.bin` | float16 | `[M, K] = [1024, 1024]` | ND，按`[M, K]`存储 | 在区间`[-0.1, 0.1)`内随机生成。 |
| B | 输入 | `input/x2_gm.bin` | float16 | `[K, N] = [1024, 1024]` | 转置后的ND，按`[N, K]`存储 | 在区间`[-0.1, 0.1)`内随机生成。 |
| C | 输出 | `output/output.bin` | float32 | `[M, N] = [1024, 1024]` | ND，按`[M, N]`存储 | Kernel计算结果 |
| golden | 真值 | `output/golden.bin` | float32 | `[M, N] = [1024, 1024]` | ND，按`[M, N]`存储 | 使用float32输入执行`np.matmul`生成。 |

矩阵沿M、N和K轴分块，具体参数见表3。

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

`block_idx`决定每个逻辑Cube核负责的输出块：`m_tile_idx = block_idx % 8`，`n_tile_idx = block_idx / 8`。每个核计算一个`[128, 128]`的C块，沿K轴执行16轮MMAD。第0轮初始化L0C Buffer，后续15轮在同一块空间中累加。

数据依次经过GM、L1 Buffer、L0A Buffer/L0B Buffer和L0C Buffer，最后写回GM。各阶段的数据排布和接口见表4。

**表4**  数据搬运路径及数据排布

| 存储单元 | 数据 | 数据排布 | 搬运或计算接口 |
| --- | --- | --- | --- |
| GM | A、转置后的B | ND | - |
| L1 Buffer | A、B数据块 | Nz | `asc_set_gm2l1_nz_para`、`asc_copy_gm2l1_nd2nz` |
| L0A Buffer | A数据块 | Nz | `asc_copy_l12l0a` |
| L0B Buffer | B数据块 | Zn | `asc_copy_l12l0b` |
| L0C Buffer | C输出块 | Nz，float32 | `asc_mmad` |
| GM | C | ND，float32 | `asc_set_l0c2gm_nz2nd`、`asc_copy_l0c2gm` |

编译参数`SCENARIO_NUM`决定使用单缓冲还是双缓冲。两种实现的A/B缓冲区数量和同步方式不同，其余计算参数相同。

**表5**  场景配置

| `SCENARIO_NUM` | Kernel | 场景 | L1 Buffer | L0A Buffer/L0B Buffer | L0C Buffer | 流水关系 |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | `mmad_baseline` | 单缓冲基线 | A、B各一块，共32768字节 | A、B各一块，共32768字节 | 一块，共65536字节 | 每个K块完成搬入、加载、MMAD和全流水同步后再处理下一块。 |
| 1 | `mmad_double_buffer` | 双缓冲优化 | A、B各两块Ping/Pong，共65536字节 | A、B各两块Ping/Pong，共65536字节 | 一块，共65536字节 | MTE2、MTE1、M流水处理不同K块并形成跨轮重叠。 |

### 场景0：单缓冲基线

`mmad_baseline`在L1 Buffer、L0A Buffer和L0B Buffer中只为A/B各分配一块空间，16轮K循环重复使用这些地址。

**表6**  单缓冲区配置

| 缓冲区 | 数组 | 元素数 | 字节数 | 用途 |
| --- | --- | --- | --- | --- |
| L1 Buffer A | `a_l1` | 8192个float16 | 16384 | 保存当前K轮从GM搬入并转换为Nz的A块。 |
| L1 Buffer B | `b_l1` | 8192个float16 | 16384 | 保存当前K轮从GM搬入并转换为Nz的B块。 |
| L0A Buffer | `a_l0` | 8192个float16 | 16384 | 保存当前K轮参与MMAD的A块。 |
| L0B Buffer | `b_l0` | 8192个float16 | 16384 | 保存当前K轮参与MMAD的B块。 |
| L0C Buffer | `c_l0` | 16384个float32 | 65536 | 累加16轮MMAD得到的C输出块。 |

每轮K循环按照以下顺序执行：

1. MTE2调用`copy_a_to_l1`和`copy_b_to_l1`，将当前A/B块从GM搬入L1 Buffer，同时把数据从ND转换为Nz；
2. MTE2释放Mutex后，MTE1获取该Mutex，将A/B加载到L0A Buffer和L0B Buffer；
3. MTE1释放Mutex后，M流水调用`asc_mmad`计算`[128, 64] x [64, 128]`。第0轮初始化L0C Buffer，后续轮次继续累加；
4. 每轮末尾调用`asc_sync_pipe(PIPE_ALL)`。当前轮全部完成后，下一轮才能复用相同的L1 Buffer和L0 Buffer。

每轮都要等待全流水同步，MTE2、MTE1和M流水只能依次处理同一个K块，不能跨K轮并行。该实现作为性能基线。

### 场景1：L1 Buffer/L0 Buffer双缓冲

`mmad_double_buffer`为L1 Buffer和L0 Buffer中的A/B数据各分配Ping/Pong两组空间。偶数轮使用Ping和Mutex 0/2，奇数轮使用Pong和Mutex 1/3。相邻K轮使用不同的A/B缓冲区。

L0C Buffer不切换，16轮MMAD都累加到同一个`c_l0`中。

**表7**  Ping/Pong Buffer配置

| K轮次 | 缓冲区组 | L1 Mutex ID | L0 Mutex ID | L1 Buffer | L0 Buffer |
| --- | --- | --- | --- | --- | --- |
| 偶数轮 | Ping | `0` | `2` | `a_l1_ping`、`b_l1_ping` | `a_l0_ping`、`b_l0_ping` |
| 奇数轮 | Pong | `1` | `3` | `a_l1_pong`、`b_l1_pong` | `a_l0_pong`、`b_l0_pong` |

循环内使用[asc_lock](../../../../../docs/zh/api/SIMD-API/c_api/sync/asc_lock.md)和[asc_unlock](../../../../../docs/zh/api/SIMD-API/c_api/sync/asc_unlock.md)管理L1 Buffer和L0 Buffer。L1 Mutex为0/1，负责MTE2和MTE1之间的同步；L0 Mutex为2/3，负责MTE1和M流水之间的同步。

MTE1读完一组L1 Buffer并释放L1 Mutex后，MTE2即可复用该组空间，不需要等待当前轮MMAD结束。

**表8**  双缓冲同步关系

| 同步方向 | Mutex ID | 保证的依赖 | 缓冲区复用条件 |
| --- | --- | --- | --- |
| `MTE2 -> MTE1` | L1 Mutex：`0/1` | MTE2完成A/B搬入并释放Mutex后，MTE1才能获取相同Mutex并读取当前组L1 Buffer。 | 防止MTE1读取L1 Buffer中尚未搬完的数据。 |
| `MTE1 -> MTE2` | L1 Mutex：`0/1` | MTE1读取L1 Buffer并释放Mutex后，后续复用该组缓冲区的MTE2才能获取相同Mutex。 | 防止MTE2提前覆盖L1 Buffer中仍在加载的数据。 |
| `MTE1 -> M` | L0 Mutex：`2/3` | MTE1完成A/B加载并释放Mutex后，M流水才能获取相同Mutex并执行当前轮MMAD。 | 防止MMAD读取L0 Buffer中尚未加载完成的数据。 |
| `M -> MTE1` | L0 Mutex：`2/3` | MMAD读取L0 Buffer并释放Mutex后，后续复用该组缓冲区的MTE1才能获取相同Mutex。 | 防止MTE1提前覆盖L0 Buffer中仍在计算的数据。 |

以连续三轮为例，第0轮使用Ping，第1轮使用Pong，第2轮重新使用Ping。M流水计算第0轮时，MTE1可以加载第1轮的Pong数据，MTE2可以向已经释放的Ping L1 Buffer搬入第2轮数据。流水稳定后，三个阶段分别处理不同的K块，计算过程可以覆盖部分搬入和加载时间。

16轮K循环结束后，FIX流水通过最后一轮的L0 Mutex等待MMAD完成，再调用`asc_copy_l0c2gm`将结果从Nz转换为ND并写回GM。最后调用`asc_sync_pipe(PIPE_ALL)`等待所有操作结束。

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

两个场景只改变缓冲区数量和同步方式。

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

测试期间保持设备频率和功耗模式不变，避免其他任务占用NPU。两个场景使用同一组输入数据，并采用相同的矩阵Shape、分块参数、Block数和编译选项。

### 性能指标

使用msOpProf采集性能数据，表11中的指标取自`PipeUtilization.csv`。

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

双缓冲会使MTE2、MTE1和M流水重叠，各流水耗时或占比不能直接相加。端到端性能以`Task Duration`为准，其他指标用于判断时间主要花在哪个阶段。

### Ascend 950PR性能对比

测试结果见表12。

**表12**  L1 Buffer/L0 Buffer双缓冲性能对比

| 场景 | 优化配置 | Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 场景0 | 单缓冲基线 | 37.684 | 64 | 36.47 | 7.06 | 0.194 | 1.387 | 0.038 | 3.147 | 0.086 | 21.169 | 0.58 | 1.287 | 0.035 |
| 场景1 | L1 Buffer/L0 Buffer双缓冲 | 20.792 | 64 | 19.9 | 7.213 | 0.362 | 1.698 | 0.085 | 4.099 | 0.206 | 14.863 | 0.747 | 1.325 | 0.067 |

#### 端到端结果

以场景0为基线，双缓冲加速比为

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

`Task Duration`从37.684μs降至20.792μs，减少16.892μs，降幅为44.83%，对应1.812倍加速。`aicore_time`从36.47μs降至19.9μs，降幅为45.43%，变化趋势与`Task Duration`一致。

两个场景的`aic_mac_time`相差0.153μs，Cube计算耗时基本不变。`aic_mte2_time`从21.169μs降至14.863μs，是变化最明显的流水指标。

`aic_mte1_time`和`aic_scalar_time`有所增加，但增量都小于1μs。Profiler中的流水耗时包含执行和等待时间，不能据此判断单次加载或Scalar指令是否变慢。`aic_fixpipe_time`只相差0.038μs，结果搬出开销基本一致。

#### 理论性能分析

矩阵规格为$M=N=K=1024$，共使用64个AI Core。按Cube主频1.65GHz、每个AI Core每cycle完成$16\times16\times16$次乘加计算，理想Cube耗时为

$$
T_{\text{cube,theory}}
= \frac{M \times N \times K}
       {16 \times 16 \times 16 \times \text{核数} \times \text{Cube主频}}
= \frac{1024 \times 1024 \times 1024}
       {16 \times 16 \times 16 \times 64 \times 1.65 \times 10^9}
\approx 2.482\ \mu s
$$

场景0和场景1的`aic_mac_time`分别为7.06μs和7.213μs，约为理论值的2.84倍和2.91倍。理论值只包含Cube指令，不包含K循环、地址计算、同步和调度。两个场景的实测MAC耗时接近，双缓冲没有改变Cube工作量。性能差异来自搬入、加载和计算的跨轮重叠。

每个输出Tile都要读取对应的A Tile和B Tile。A矩阵由8个N方向Tile重复读取，B矩阵由8个M方向Tile重复读取。不考虑Cache命中时，理论读入量为

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

#### 流水与带宽分析

按32MiB理论读入量和`aic_mte2_time`估算聚合有效读带宽，场景0为

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

场景1的估算值比场景0高42.43%。该数值按各核的重复读取量计算，同时受L2 Cache命中和ND到Nz格式转换影响，不代表HBM物理带宽。

场景0的MAC、MTE1、MTE2和FIX流水占比之和为89.5%，各阶段基本串行。场景1的占比之和为138.2%，说明搬入、加载和计算已在不同K轮间重叠。

场景1的`aic_mte2_ratio`从58.0%升至74.7%，但`aic_mte2_time`下降了29.79%。占比上升是因为总`aicore_time`缩短，不能理解为MTE2变慢。MTE2仍是耗时占比最高的阶段。

#### 双缓冲结论

Ping/Pong双缓冲使MTE2、MTE1和M流水能够处理不同的K块。本次测试中，`Task Duration`减少16.892μs，降幅为44.83%。A/B在L1 Buffer和L0 Buffer中的占用各增加32768字节，并增加了Ping/Pong选择和Mutex同步。

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

将`SCENARIO_NUM`改为`1`后重新执行CMake、编译、运行和校验，即可测试双缓冲场景。切换场景前需要清理build目录中的CMake缓存。

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
