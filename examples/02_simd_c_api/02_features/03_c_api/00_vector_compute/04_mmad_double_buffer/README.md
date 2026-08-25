# Matmul C-API高性能样例

## 概述

本样例使用Ascend C C-API实现half矩阵乘，分别对比单缓冲、仅L1双缓冲、仅L0双缓冲和L1/L0双缓冲四种配置。Ping/Pong Buffer使MTE2搬入、MTE1加载和Cube计算能够在不同K轮之间重叠。

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
| A | 输入 | `input/x1_gm.bin` | half | `[M, K] = [1024, 1024]` | ND，按`[M, K]`存储 | 在区间`[-0.1, 0.1)`内随机生成。 |
| B | 输入 | `input/x2_gm.bin` | half | `[K, N] = [1024, 1024]` | 转置后的ND，按`[N, K]`存储 | 在区间`[-0.1, 0.1)`内随机生成。 |
| C | 输出 | `output/output.bin` | float | `[M, N] = [1024, 1024]` | ND，按`[M, N]`存储 | Kernel计算结果 |
| golden | 真值 | `output/golden.bin` | float | `[M, N] = [1024, 1024]` | ND，按`[M, N]`存储 | 使用float输入执行`np.matmul`生成。 |

样例计算`C = A x B`，其中`M`、`N`和`K`均为1024。矩阵按`[128, 128, 64]`分块，使用64个逻辑Cube核，每个核负责一个`[128, 128]`输出块。`block_idx`用于分配输出块，核内沿K轴执行16轮MMAD累加。

数据依次经过GM、L1 Buffer、L0A Buffer/L0B Buffer和L0C Buffer，最后写回GM。各阶段的数据排布和接口见表4。

**表4**  数据搬运路径及数据排布

| 存储单元 | 数据 | 数据排布 |
| --- | --- | --- |
| GM | A、转置后的B | ND |
| L1 Buffer | A、B数据块 | Nz |
| L0A Buffer | A数据块 | Nz |
| L0B Buffer | B数据块 | Zn |
| L0C Buffer | C输出块 | Nz，float |
| GM | C | ND，float |

编译参数`SCENARIO_NUM`选择A/B数据在L1和L0中的缓冲配置。四个场景使用相同的矩阵规模、Tile、K循环次数、Block数和MMAD参数，只改变L1和L0A/L0B Buffer的数量。

**表5**  场景配置

| `SCENARIO_NUM` | Kernel | 场景 | L1 Buffer | L0A Buffer/L0B Buffer | L0C Buffer | 流水关系 |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | `mmad_baseline` | 单缓冲基线 | A/B各一组 | A/B各一组 | 一组，用于结果累加 | 每个K块完成搬入、加载、MMAD和全流水同步后再处理下一块。 |
| 1 | `mmad_l1_double_buffer` | 仅L1双缓冲 | A/B Ping/Pong两组 | A/B各一组 | 一组，用于结果累加 | MTE2可提前准备后续K块；MTE1仍需等待单组L0可复用。 |
| 2 | `mmad_l0_double_buffer` | 仅L0双缓冲 | A/B各一组 | A/B Ping/Pong两组 | 一组，用于结果累加 | MTE1可向空闲L0组加载后续K块，并与另一组上的MMAD重叠。 |
| 3 | `mmad_double_buffer` | L1/L0双缓冲 | A/B Ping/Pong两组 | A/B Ping/Pong两组 | 一组，用于结果累加 | MTE2、MTE1、M流水可处理不同K块。 |

### 场景0：单缓冲基线

`mmad_baseline`在L1 Buffer、L0A Buffer和L0B Buffer中只为A/B各分配一块空间，16轮K循环重复使用这些地址。

每轮K循环按照以下顺序执行：

1. MTE2调用`copy_a_to_l1`和`copy_b_to_l1`，将当前A/B块从GM搬入L1 Buffer，同时把数据从ND转换为Nz；
2. MTE2释放Mutex后，MTE1获取该Mutex，将A/B加载到L0A Buffer和L0B Buffer；
3. MTE1释放Mutex后，M流水调用`asc_mmad`计算`[128, 64] x [64, 128]`。第0轮初始化L0C Buffer，后续轮次继续累加；
4. 每轮末尾调用`asc_sync_pipe(PIPE_ALL)`。当前轮全部完成后，下一轮才能复用相同的L1 Buffer和L0 Buffer。

每轮都要等待全流水同步，MTE2、MTE1和M流水只能依次处理同一个K块，不能跨K轮并行。该实现作为性能基线。

### 场景1：仅L1 Buffer双缓冲

`mmad_l1_double_buffer`仅为L1中的A/B数据分配Ping/Pong两组空间，L0A/L0B仍各使用一组空间。偶数轮使用L1 Ping和Mutex 0，奇数轮使用L1 Pong和Mutex 1；单组L0始终使用Mutex 2。

MTE1读完某组L1 Buffer后，MTE2即可向该组写入后续K块，不必等待MMAD结束。由于L0只有一组，MTE1仍需等待前一轮MMAD读完L0后才能加载下一块。

### 场景2：仅L0 Buffer双缓冲

`mmad_l0_double_buffer`仅为L0A/L0B中的A/B数据分配Ping/Pong两组空间，L1中的A/B数据仍各使用一组空间。单组L1使用Mutex 0；偶数轮使用L0 Ping和Mutex 2，奇数轮使用L0 Pong和Mutex 3。

MTE1读完L1后，MTE2即可复用单组L1。MMAD计算某组L0数据时，MTE1可以向另一组L0加载后续K块。

### 场景3：L1 Buffer/L0 Buffer双缓冲

`mmad_double_buffer`为L1和L0中的A/B数据都分配Ping/Pong两组空间。偶数轮使用Ping和Mutex 0/2，奇数轮使用Pong和Mutex 1/3。L0C Buffer不切换，16轮MMAD始终累加到同一个`c_l0`中。

各场景均使用[asc_lock](../../../../../docs/zh/api/SIMD-API/c_api/sync/asc_lock.md)和[asc_unlock](../../../../../docs/zh/api/SIMD-API/c_api/sync/asc_unlock.md)控制Buffer复用依赖。这些Mutex保证数据在被读取前已写完、在被覆盖前已读完，不作为独立的性能优化项。

**表7**  Buffer复用依赖

| 依赖方向 | Mutex ID | 作用 |
| --- | --- | --- |
| `MTE2 -> MTE1`、`MTE1 -> MTE2` | L1：单缓冲为`0`，双缓冲为`0/1` | 控制L1数据搬入与MTE1读取，以及L1 Buffer的再次写入。 |
| `MTE1 -> M`、`M -> MTE1` | L0：单缓冲为`2`，双缓冲为`2/3` | 控制L0加载与MMAD读取，以及L0 Buffer的再次写入。 |

以场景3的连续三轮为例：第0轮使用Ping，第1轮使用Pong，第2轮重新使用Ping。M流水计算第0轮时，MTE1可加载第1轮Pong数据，MTE2可向已被MTE1释放的Ping L1 Buffer搬入第2轮数据。K循环结束后，FIX通过最后一轮对应的L0 Mutex等待MMAD完成，再将结果写回GM。

四个场景依次为单缓冲、仅L1双缓冲、仅L0双缓冲和L1/L0双缓冲，用于比较两级Buffer分别及同时采用Ping/Pong时的流水重叠收益。

## 性能对比方法

性能测试保持矩阵、MMAD参数和输入数据一致，Mutex仅用于表达各Buffer的读写和复用依赖。

### 测试条件

**表10**  性能测试配置

| 配置项 | 配置值 |
| --- | --- |
| 芯片版本 | Ascend 950PR |
| CANN软件版本 | CANN 9.1.0 |
| NPU架构 | `dav-3510` |
| 输入数据类型 | A、B为half |
| 输出数据类型 | C为float |
| 矩阵Shape | `M=N=K=1024` |
| Cube Tile | `TILE_M=128`、`TILE_K=64`、`TILE_N=128` |
| Block数 | 64 |
| K循环次数 | 每个输出Tile执行16轮MMAD累加 |
| 测试次数 | 每个场景预热1次，正式采集至少5次，取Task Duration中位数 |

测试期间保持设备频率和功耗模式不变，避免其他任务占用NPU。四个场景使用同一组输入数据，并采用相同的矩阵Shape、分块参数、Block数和编译选项。

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

启用双缓冲后，部分或全部流水会在不同K轮之间重叠，各流水耗时或占比不能直接相加。端到端性能以`Task Duration`为准，其他指标用于判断时间主要花在哪个阶段。

### Ascend 950PR性能对比

表12列出了四个场景的实测数据。各流水耗时用于分析瓶颈，场景间的端到端性能以`Task Duration`为准。

**表12**  已采集的L1/L0双缓冲性能数据

| 场景 | 优化配置 | Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 场景0 | 单缓冲基线 | 36.879 | 64 | 35.72 | 7.047 | 0.197 | 1.216 | 0.034 | 3.141 | 0.088 | 21.002 | 0.588 | 1.344 | 0.038 |
| 场景1 | L1双缓冲 | 21.029 | 64 | 20.06 | 7.22 | 0.36 | 1.53 | 0.076 | 4.254 | 0.212 | 14.847 | 0.74 | 1.341 | 0.067 |
| 场景2 | L0双缓冲 | 29.062 | 64 | 28.35 | 6.978 | 0.246 | 1.342 | 0.047 | 3.11 | 0.11 | 20.75 | 0.732 | 1.264 | 0.045 |
| 场景3 | L1/L0双缓冲 | 21.5 | 64 | 20.49 | 7.298 | 0.356 | 1.607 | 0.078 | 4.322 | 0.211 | 15.063 | 0.735 | 1.379 | 0.067 |

#### 性能分析

以场景0为基线，场景3启用L1/L0双缓冲后的加速比为

$$
S = \frac{T_{\text{baseline}}}{T_{\text{double-buffer}}}
  = \frac{36.879}{21.500}
  \approx 1.715
$$

端到端耗时降幅为

$$
R = \frac{T_{\text{baseline}} - T_{\text{double-buffer}}}{T_{\text{baseline}}} \times 100\%
  = \frac{36.879 - 21.500}{36.879} \times 100\%
  \approx 41.70\%
$$

`Task Duration`从36.879μs降至21.500μs，减少15.379μs，降幅为41.70%，对应1.715倍加速。`aicore_time`从35.720μs降至20.490μs，变化趋势与端到端耗时一致。

场景0和场景3的`aic_mac_time`只相差0.251μs，Cube计算工作量基本不变。收益来自不同K轮的MTE2搬入、MTE1加载和Cube计算能够重叠执行。`aic_mte2_time`从21.002μs降至15.063μs，是变化最明显的流水指标。

场景3的`aic_mte2_ratio`从58.8%升至73.5%，是占比最高的阶段。占比升高是因为总`aicore_time`缩短，不能理解为MTE2变慢。Profiler中的流水耗时包含执行和等待时间，不能将各流水耗时直接相加，也不能据此判断单次搬运速度。

双缓冲的收益会相互影响，分析L1和L0的单独贡献时，除比较各场景相对场景0的结果外，还应比较已启用另一层双缓冲后的增量收益。

**表13**  L1和L0双缓冲贡献分析方法

| 对比 | 说明 |
| --- | --- |
| 场景0 -> 场景1 | 在单L0条件下，仅L1双缓冲带来的收益。 |
| 场景0 -> 场景2 | 在单L1条件下，仅L0双缓冲带来的收益。 |
| 场景1 -> 场景3 | 已启用L1双缓冲时，L0双缓冲的额外收益。 |
| 场景2 -> 场景3 | 已启用L0双缓冲时，L1双缓冲的额外收益。 |

#### 总结

- **优化方式**：L1和L0均使用Ping/Pong Buffer后，MTE2搬入下一轮K块、MTE1加载当前K块和Cube计算上一轮K块可以并行推进。双缓冲改变的是各阶段的等待关系，不会减少矩阵乘法的Cube计算量。
- **实测收益**：在表12的场景0与场景3对比中，`Task Duration`从36.879μs降至21.500μs，减少15.379μs，降幅为41.70%，加速比为1.715。`aic_mac_time`基本不变，说明收益主要来自流水重叠。
- **分层贡献**：场景1和场景2分别用于观察仅启用L1或仅启用L0双缓冲的效果；再通过场景1到场景3、场景2到场景3的增量对比，判断另一层双缓冲带来的额外收益。由于两层缓冲会相互影响，不能将两者的单独收益直接相加。
- **后续方向**：场景3中MTE2仍是占比最高的阶段。若继续优化，应结合实际采集的Memory和L2 Cache指标，优先检查输入搬运量、L2复用和Tile大小，而不是仅根据单个流水耗时推断带宽变化。

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

将`SCENARIO_NUM`改为`1`、`2`或`3`后重新执行CMake、编译、运行和校验，即可测试对应场景。切换场景前需要清理build目录中的CMake缓存。

**表14**  编译选项

| 编译选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构 |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式 |
| `SCENARIO_NUM` | `0`、`1`、`2`、`3`（默认） | 依次为单缓冲、仅L1双缓冲、仅L0双缓冲、L1/L0双缓冲。 |

精度校验成功时输出：

```text
test pass!
```
