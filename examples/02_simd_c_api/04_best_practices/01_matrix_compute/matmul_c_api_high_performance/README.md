# Matmul基础API高性能样例

## 概述

本样例使用Ascend C C API实现float16矩阵乘，并提供单缓冲基线和L1 Buffer/L0 Buffer Ping-Pong双缓冲两个场景。两个场景使用相同的矩阵规格、Cube分块、核数和计算接口，用于对比三级流水重叠带来的性能变化。

## 支持的产品及CANN软件版本

**表 1**  支持的产品及CANN软件版本

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

**表 2**  输入输出数据规格

| 数据 | 输入/输出 | 数据文件 | 数据类型 | 逻辑Shape | 文件及GM排布 | 数据说明 |
| --- | --- | --- | --- | --- | --- | --- |
| A | 输入 | `input/x1_gm.bin` | float16 | `[M, K] = [1024, 1024]` | ND，按`[M, K]`存储 | 在区间`[-0.1, 0.1)`内随机生成。 |
| B | 输入 | `input/x2_gm.bin` | float16 | `[K, N] = [1024, 1024]` | 转置后的ND，按`[N, K]`存储 | 在区间`[-0.1, 0.1)`内随机生成。 |
| C | 输出 | `output/output.bin` | float32 | `[M, N] = [1024, 1024]` | ND，按`[M, N]`存储 | Kernel计算结果 |
| golden | 真值 | `output/golden.bin` | float32 | `[M, N] = [1024, 1024]` | ND，按`[M, N]`存储 | 使用float32输入执行`np.matmul`生成。 |

矩阵沿M、N和K轴进行分块，分块及多核计算参数如下表所示。

**表 3**  矩阵分块及多核计算参数

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

**表 4**  数据搬运路径及数据排布

| 存储单元 | 数据 | 数据排布 | 搬运或计算接口 |
| --- | --- | --- | --- |
| GM | A、转置后的B | ND | - |
| L1 Buffer | A、B数据块 | Nz | `asc_set_gm2l1_nz_para`、`asc_copy_gm2l1_nd2nz` |
| L0A Buffer | A数据块 | Nz | `asc_copy_l12l0a` |
| L0B Buffer | B数据块 | Zn | `asc_copy_l12l0b` |
| L0C Buffer | C输出块 | Nz，float32 | `asc_mmad` |
| GM | C | ND，float32 | `asc_set_l0c2gm_nz2nd`、`asc_copy_l0c2gm` |

编译参数`SCENARIO_NUM`用于选择单缓冲基线或L1 Buffer/L0 Buffer双缓冲优化。两个场景的输入、矩阵规格、分块方式、核数和MMAD计算量完全相同，仅A/B缓冲区数量和流水同步方式不同。

**表 5**  场景配置

| `SCENARIO_NUM` | Kernel | 场景 | L1 Buffer | L0A Buffer/L0B Buffer | L0C Buffer | 流水关系 |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | `mmad_baseline` | 单缓冲基线 | A、B各一块，共32768字节 | A、B各一块，共32768字节 | 一块，共65536字节 | 每个K块完成搬入、加载、MMAD和全流水同步后再处理下一块。 |
| 1 | `mmad_double_buffer` | 双缓冲优化 | A、B各两块Ping/Pong，共65536字节 | A、B各两块Ping/Pong，共65536字节 | 一块，共65536字节 | MTE2、MTE1、M流水处理不同K块并形成跨轮重叠。 |

### 场景0：单缓冲基线

基线Kernel `mmad_baseline`在L1 Buffer、L0A Buffer和L0B Buffer中只为A/B各分配一块缓冲区，16轮K循环反复使用相同的缓冲区。各缓冲区的用途如下表所示。

**表 6**  单缓冲区配置

| 缓冲区 | 数组 | 元素数 | 字节数 | 用途 |
| --- | --- | --- | --- | --- |
| L1 Buffer A | `a_l1` | 8192个float16 | 16384 | 保存当前K轮从GM搬入并转换为Nz的A块。 |
| L1 Buffer B | `b_l1` | 8192个float16 | 16384 | 保存当前K轮从GM搬入并转换为Nz的B块。 |
| L0A Buffer | `a_l0` | 8192个float16 | 16384 | 保存当前K轮参与MMAD的A块。 |
| L0B Buffer | `b_l0` | 8192个float16 | 16384 | 保存当前K轮参与MMAD的B块。 |
| L0C Buffer | `c_l0` | 16384个float32 | 65536 | 累加16轮MMAD得到的C输出块。 |

每轮K循环按照以下顺序执行：

1. MTE2流水分别调用`copy_a_to_l1`和`copy_b_to_l1`，通过`asc_copy_gm2l1_nd2nz`将当前A/B块从GM搬入L1 Buffer并由ND转换为Nz；
2. `MTE2 -> MTE1`同步保证L1 Buffer数据就绪，MTE1流水随后调用`asc_copy_l12l0a`和`asc_copy_l12l0b`将A/B加载到L0A Buffer/L0B Buffer；
3. `MTE1 -> M`同步保证L0A Buffer/L0B Buffer数据就绪，M流水调用`asc_mmad`计算`[128, 64] x [64, 128]`。第0轮初始化L0C Buffer，后续轮次将结果累加到L0C Buffer；
4. 每轮末尾调用`asc_sync_pipe(PIPE_ALL)`，等待当前轮的MTE2、MTE1和M流水全部完成后，下一轮才能覆盖L1 Buffer和L0 Buffer中的相同缓冲区。

因此，当M流水计算第n个K块时，MTE1不能提前加载第n+1个K块，MTE2也不能提前搬入后续K块。三个阶段在每轮内依次推进，无法在不同K轮之间重叠。该场景作为L1 Buffer/L0 Buffer双缓冲优化的性能基线。

### 场景1：L1 Buffer/L0 Buffer双缓冲

优化Kernel `mmad_double_buffer`分别为L1 Buffer和L0 Buffer中的A/B数据分配Ping/Pong两组缓冲区。偶数K轮使用Ping和`EVENT_ID0`，奇数K轮使用Pong和`EVENT_ID1`，相邻K轮不会读写L1 Buffer或L0 Buffer中的同一组缓冲区。L0C Buffer不参与Ping-Pong切换，16轮MMAD始终累加到同一个`c_l0`输出块。

**表 7**  Ping-Pong缓冲区配置

| K轮次 | 缓冲区组 | Event ID | L1 Buffer | L0 Buffer |
| --- | --- | --- | --- | --- |
| 偶数轮 | Ping | `EVENT_ID0` | `a_l1_ping`、`b_l1_ping` | `a_l0_ping`、`b_l0_ping` |
| 奇数轮 | Pong | `EVENT_ID1` | `a_l1_pong`、`b_l1_pong` | `a_l0_pong`、`b_l0_pong` |

进入K循环前，代码通过反向Event通知MTE2和MTE1两组Ping/Pong缓冲区均可写。循环内使用以下依赖分别控制L1 Buffer和L0 Buffer中缓冲区的生产、消费与复用。

**表 8**  双缓冲同步关系

| 同步方向 | 保证的依赖 | 缓冲区复用条件 |
| --- | --- | --- |
| `MTE2 -> MTE1` | A/B搬入L1 Buffer完成后，MTE1才能读取当前组L1 Buffer数据。 | 防止MTE1读取L1 Buffer中尚未搬完的数据。 |
| `MTE1 -> MTE2` | MTE1读取L1 Buffer完成后，MTE2才能向L1 Buffer中的同一组缓冲区搬入后续K块。 | 防止MTE2提前覆盖L1 Buffer中仍在加载的数据。 |
| `MTE1 -> M` | A/B加载到L0A Buffer/L0B Buffer完成后，M流水才能执行当前轮MMAD。 | 防止MMAD读取L0 Buffer中尚未加载完成的数据。 |
| `M -> MTE1` | MMAD读取L0A Buffer/L0B Buffer完成后，MTE1才能向L0 Buffer中的同一组缓冲区加载后续K块。 | 防止MTE1提前覆盖L0 Buffer中仍在计算的数据。 |

以连续三个K轮为例：第0轮使用Ping，第1轮使用Pong，第2轮再次使用Ping。当M流水使用Ping计算第0轮时，MTE1可以将第1轮Pong数据从L1 Buffer加载到L0 Buffer，MTE2还可以将第2轮数据搬入L1 Buffer中已经释放的Ping缓冲区。进入稳定流水后，MTE2、MTE1和M流水可分别处理相邻的不同K块，从而隐藏部分GM到L1 Buffer搬运及L1 Buffer到L0 Buffer加载的等待时间。

16轮K循环结束后，代码等待两组L1 Buffer和L0 Buffer反向Event，确保所有缓冲区均已释放；随后使用`M -> FIX`的`EVENT_ID2`保证L0C Buffer累加完成，FIX流水调用`asc_copy_l0c2gm`将L0C Buffer中的Nz结果转换为ND并写回GM。

两个场景的核心区别如下表所示。

**表 9**  单缓冲与双缓冲对比

| 对比项 | 场景0：单缓冲基线 | 场景1：L1 Buffer/L0 Buffer双缓冲 |
| --- | --- | --- |
| L1 Buffer中的A/B缓冲区 | 每个矩阵一块，共32768字节 | 每个矩阵两块Ping/Pong，共65536字节。 |
| L0A Buffer/L0B Buffer | 每个矩阵一块，共32768字节 | 每个矩阵两块Ping/Pong，共65536字节。 |
| L0C Buffer | 一块，累加16轮MMAD | 一块，累加16轮MMAD。 |
| 相邻K轮使用的A/B缓冲区 | 相同，必须等待上一轮结束后复用。 | 不同，在Ping和Pong之间交替。 |
| 轮末同步 | 每轮执行`asc_sync_pipe(PIPE_ALL)` | 循环内使用两组Event管理局部依赖，K循环结束后统一等待。 |
| 跨轮流水重叠 | 不支持 | MTE2、MTE1、M流水可处理不同K块。 |
| 优化代价 | L1 Buffer和L0 Buffer占用较少。 | A/B缓冲区在L1 Buffer和L0 Buffer中的占用均增加一倍。 |

## 性能对比方法

两个场景除缓冲区数量和同步方式外，其余计算条件一致。分别编译`SCENARIO_NUM=0`和`SCENARIO_NUM=1`，使用性能分析工具采集Kernel的Task Duration，以单缓冲场景作为基线：

```text
性能提升比例 = 基线耗时 / 双缓冲耗时 - 1
```

同时可对比MTE2、MTE1和Cube流水的利用率与等待时间，确认收益是否来自搬运、加载和计算的重叠，而不是shape、分块或核数变化。

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

**表 10**  编译选项

| 编译选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构 |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式 |
| `SCENARIO_NUM` | `0`、`1`（默认） | 单缓冲基线、双缓冲优化 |

精度校验成功时输出：

```text
test pass!
```
