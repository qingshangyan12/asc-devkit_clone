# MXFP8 Matmul C API 高性能样例

## 概述

本样例介绍如何基于 C API，通过 L1 Buffer/L0A Buffer/L0B Buffer 双缓冲、大包搬运、流水同步等优化手段，实现高性能的 MXFP8 Matmul 核函数。矩阵计算的基本流程参见[Cube矩阵计算编程](../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simd_programming/c_pointer_programming/cube_matrix_computation.md)。

## 本样例支持的产品及 CANN 软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR&950DT系列产品 | >= CANN 9.3.0 |

## 目录结构

```text
├── 03_matmul_mx
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── matmul_mx.asc           // C API 样例实现
│   ├── README_en.md            // 英文说明文档
│   └── README.md               // 中文说明文档
```

## MX Matmul 介绍

带有量化系数的矩阵乘加计算及其数据格式参见 [asc_mmad_mx](../../../../../docs/zh/api/SIMD-API/c_api/cube_compute/mmad_compute/asc_mmad_mx.md)。

普通 Matmul 只有两路矩阵输入：

$$
C = A * B
$$

MX Matmul 在此基础上额外引入两路 scale 输入。A、B 为 MXFP8 低比特数据，scaleA、scaleB 为量化系数。计算时，矩阵数据与对应 scale 共同参与运算：

$$
C = (\text{scaleA} \otimes A) \times (\text{scaleB} \otimes B)
$$

在MX Matmul中，K 方向每 32 个元素共享一个 scale，计算时沿 K 方向广播到对应的一组 MXFP8 数据上。

## 样例规格

本样例实现固定 shape 为 `8192 x 8192 x 8192` 的 MXFP8 Matmul，输出数据类型为 `half`。

### 输入输出

| 输入/输出 | 形状 | 类型 | 数据排布类型 |
| --- | --- | --- | --- |
| A | [8192, 8192] | fp8_e4m3fn_t | ND |
| B | [8192, 8192] | fp8_e4m3fn_t | ND |
| scale_a | [8192, 256] | fp8_e8m0_t | ND |
| scale_b | [8192, 256] | fp8_e8m0_t | ND |
| C | [8192, 8192] | half | ND |

A 和 B 在 GM 中分别按 `[M, K]`、`[N, K]` 行主序存储。scale_a 和 scale_b 分别按 `[M, K/32]`、`[N, K/32]` 行主序存储。L0C Buffer 使用 float 累加，最后转换为 half 输出，首轮累加初始化为零。

### 关键参数

| 代码参数 | 值 | 说明 |
| --- | --- | --- |
| matrix_m / matrix_n / matrix_k | 8192 / 8192 / 8192 | 完整矩阵形状 |
| block_m / block_n / block_k | 256 / 256 / 128 | 一次 Cube 计算的 M/N/K 范围 |
| core_m / core_n | 2048 / 1024 | 每个 block 的输出区域 |
| num_blocks | 32 | 固定启动的 Cube block 数 |
| k_rounds | 64 | 每个输出分块的 K 累加轮数 |
| data_step | 2 | 一个 A/B 大包包含的基础 K 块数 |
| data_k | 256 | 数据大包的逻辑 K 长度 |
| scale_step | 8 | 一个 scale 大包覆盖的基础 K 块数 |
| scale_chunk | 32 | 每行 scale 大包的字节数，对应逻辑 K=1024 |
| l0_buffer_bytes | 32768 | L0A Buffer/L0B Buffer 单个 Ping/Pong 缓冲区的字节数 |

> **约束说明**：本样例未提供任意 shape 或尾块处理。

## 样例实现

整个 kernel 的过程为：多核切分后，每个核负责一个 core_m * core_n 子矩阵，在 M/N 方向上循环，将 A/B 与 scale 搬入 L1 Buffer，再在K方向循环，加载到 L0A Buffer/L0B Buffer，完成 Cube 累加计算，最后把结果写回 GM。

| 代码函数 | 职责 |
| --- | --- |
| set_nz_config | 配置单矩阵 ND/DN→NZ 布局转换参数 |
| copy_data_to_l1 | A/B 矩阵数据大包搬运 GM→L1 Buffer |
| copy_scale_to_l1 | scale 数据大包搬运 GM→L1 Buffer |
| load_to_l0 | 矩阵数据与 scale 数据从 L1 Buffer 加载到 L0A Buffer/L0B Buffer |
| compute_block | 当前 K 块的 MX 矩阵乘累加 |
| copy_result_to_gm | 计算结果搬出写回 GM |
| process_output_block | 组织首个大包、K 循环、预取和输出 |
| matmul_mx | 分配片上存储，执行固定 32 核分工 |

### 数据流路径

```text
物理地址                          流水
  GM
  |  copy_data_to_l1             MTE2
  |  copy_scale_to_l1            MTE2
  v
  L1 Buffer
  |
  |  load_to_l0                  MTE1
  v
  L0A/L0B/L0A_MX/L0B_MX Buffer
  |
  |  compute_block                M
  v
  L0C Buffer
  |
  |  copy_result_to_gm           FIX
  v
  GM
```

#### 1. 多核切分：把大矩阵拆成 32 个子任务

M 方向分成 `8192/2048=4` 份，N 方向分成 `8192/1024=8` 份，形成 `4 x 8 = 32` 个子矩阵：

```cpp
uint32_t m_start = block_idx % (matrix_m / core_m) * core_m;
uint32_t n_start = block_idx / (matrix_m / core_m) * core_n;
```

每核先遍历 N、再遍历 M，输出区域包含 `8×4=32` 个 256×256 分块。每个分块沿 K 执行 64 轮乘累加。32 个 block 恰好覆盖完整输出。

#### 2. GM 到 L1 Buffer：矩阵和 scale 按大包搬运

`copy_data_to_l1` 对 A/B 分别执行 [`asc_copy_gm2l1_nd2nz`](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_load/asc_copy_gm2l1_nd2nz.md)。GM 行步长为 8192 字节，大包宽度为 256 个 FP8 元素，K 方向源偏移为 `k_idx*block_k` 字节；A/B 目的 C0 stride 分别为 block_m、block_n。

```cpp
set_nz_config(block_m);
asc_copy_gm2l1_nd2nz((__cbuf__ uint8_t*)a_l1,
    (__gm__ uint8_t*)(a + k_idx * block_k), matrix_k,
    asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, block_m, data_k, 0, false);
```

`copy_scale_to_l1` 将两个相邻 E8M0 字节视为一个 b16 元素，使用 [Dn2Nz](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_load/asc_copy_gm2l1_dn2nz.md) 保证 MX 所需的排列格式：

```cpp
set_nz_config(scale_chunk / 2);
asc_copy_gm2l1_dn2nz((__cbuf__ half*)scale_a_l1,
    (__gm__ half*)(a_scale + k_idx * block_k / 32), scale_k,
    asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, scale_chunk / 2, block_m, 0, false);
```

half 指针用于按 16 位数据搬运，不进行数值转换；GM 行步长参数仍为字节数 `scale_k=256`，不能因为指针类型变化再除以 2。

#### 3. L1 Buffer 到 L0A Buffer/L0B Buffer：加载 FP8 数据及 MX scale

量化系数加载的地址映射和参数单位参见 [asc_copy_l12l0a_mx](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_load/asc_copy_l12l0a_mx.md)。

`load_to_l0` 通过 `asc_copy_l12l0a/b` 加载矩阵数据，通过 `asc_copy_l12l0a_mx/b_mx` 加载对应 scale。

- FP8 数据分形的 K 长度为 32 元素，数据 `k_step=block_k/32=4`。
- MX scale 分形覆盖逻辑 K=64，scale `y_step=block_k/64=2`，不能照搬数据的步长。
- 数据在大包内偏移为 A 的 `data_offset*block_k*block_m` 或 B 的 `data_offset*block_k*block_n` 字节。
- scale 在大包内偏移为 `scale_offset_idx*(block_k/32)*16` 字节；源 stride 为 `scale_chunk/2=16`，目的 stride 为 `block_k/64=2`。
- MX 目的地址为对应 L0A Buffer/L0B Buffer 数据地址除以 16。

```cpp
asc_copy_l12l0a(a_dst, a_l1 + data_offset * block_k * block_m,
    0, 0, block_m / 16, block_k / 32, block_m / 16, block_m / 16);
asc_copy_l12l0b(b_dst, b_l1 + data_offset * block_k * block_n,
    0, 0, block_n / 16, block_k / 32, block_n / 16, block_n / 16);

uint32_t scale_offset = scale_offset_idx * (block_k / 32) * 16;
asc_copy_l12l0a_mx((uint64_t)a_dst / 16, scale_a_l1 + scale_offset,
    0, 0, block_m / 16, block_k / 64, scale_chunk / 2, block_k / 64);
asc_copy_l12l0b_mx((uint64_t)b_dst / 16, scale_b_l1 + scale_offset,
    0, 0, block_n / 16, block_k / 64, scale_chunk / 2, block_k / 64);
```

#### 4. L1 Buffer/L0A Buffer/L0B Buffer 双缓冲：让加载和计算交叠

样例在 L1 Buffer 和 L0A Buffer/L0B Buffer 都使用 Ping-Pong 双缓冲。这样做的核心目标是让搬运、加载、计算尽量交叠，减少等待。

本样例的主要缓冲区布局如下：

| 层级 | 缓冲区 | 内容 | 作用 |
|------|--------|------|------|
| L1 Buffer | `a_l1 Ping/Pong` | A 的大包数据 | 存放 GM->L1 Buffer 的 A 数据 |
| L1 Buffer | `b_l1 Ping/Pong` | B 的大包数据 | 存放 GM->L1 Buffer 的 B 数据 |
| L1 Buffer | `scalea_l1 Ping/Pong` | A 对应的 scale 数据 | 存放 A 侧 量化系数  |
| L1 Buffer | `scaleb_l1 Ping/Pong` | B 对应的 scale 数据 | 存放 B 侧 量化系数  |
| L0A Buffer | `a_l0 Ping/Pong` | 当前 K 分块的 A 数据 | Cube 计算左操作数 |
| L0B Buffer | `b_l0 Ping/Pong` | 当前 K 分块的 B 数据 | Cube 计算右操作数 |
| L0C Buffer | `c_l0` | float 累加结果 | 存放计算结果，供搬出指令写回 GM |

在 K 循环中，当前 K 分块 进入 Cube 计算时，下一批 A/B/scaleA/scaleB 可以提前发起搬运。Ping 和 Pong 交替使用，生产者和消费者通过事件同步确认缓冲区是否可写、数据是否可读。

#### 5. Cube 计算：按 K 分块 累加

`compute_block` 等待 L0A Buffer/L0B Buffer 数据就绪，执行一次 256×128×256 的 MX 矩阵乘累加。第一轮初始化 C，后续轮继续累加：

```cpp
asc_mmad_mx(c_l0, a_dst, b_dst, block_m, block_k, block_n,
    asc_unit_flag_mode::DISABLE, false, false, k_idx == 0);
```

每个输出分块共执行 64 次 MMAD；每轮计算后发出 M→MTE1 事件，允许后续复用该 L0A Buffer/L0B Buffer 缓冲区。

#### 6. Fixpipe 写回：从 float 转换到 half 输出

`copy_result_to_gm` 等待 Cube 完成，将 L0C Buffer 的 NZ 格式转换成行优先 ND，使用 [`F322F16`](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_store/asc_copy_l0c2gm.md) 输出 half：

```cpp
asc_set_l0c_copy_nz_para(1, 0, 0);
asc_copy_l0c2gm(dst, c_l0, block_n, block_m, matrix_n, block_m,
    asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, asc_unit_flag_mode::DISABLE,
    QuantMode_t::F322F16, asc_relu_pre_mode::NONE, false, true, false, false);
```

目的行步长为 8192 个 half 元素，源 NZ stride 为 block_m=256，输出地址为 `c+m*matrix_n+n`。

写回后发出 FIX→M 事件，将等待推迟到下一分块的第一条 MMAD 前，让其 L1 Buffer/L0A Buffer/L0B Buffer 搬运可先发起，同时防止在 FIX 读取结束前覆盖 L0C Buffer。

### 事件同步：把流水并行起来

使用 [asc_sync_notify / asc_sync_wait](../../../../../docs/zh/api/SIMD-API/c_api/sync/intra_core_sync/asc_sync_notify.md) 实现核内不同流水之间的同步控制。

| 方向 | event ID | 含义 |
| --- | --- | --- |
| MTE2→MTE1 | 0/1 | A/B 数据 ping/pong 就绪 |
| MTE1→MTE2 | 0/1 | A/B 数据 ping/pong 可覆盖 |
| MTE2→MTE1 | 2/3 | scale ping/pong 就绪 |
| MTE1→MTE2 | 2/3 | scale ping/pong 可覆盖 |
| MTE1→M | 0/1 | L0A Buffer/L0B Buffer ping/pong 数据及 scale 就绪 |
| M→MTE1 | 0/1 | Cube 已消费 L0A Buffer/L0B Buffer ping/pong |
| M→FIX | 0 | 完整输出分块 累加完成 |
| FIX→M | 0 | L0C Buffer 可供下一输出分块复用 |

事件按流水方向独立使用。入口预置 MTE1→MTE2 的 0～3、M→MTE1 的 0/1 和 FIX→M 的 0，使首次写入不等待未发生的消费者。出口消费所有剩余反向事件，并执行 `asc_sync_pipe(PIPE_ALL)`。

数据/scale 大包仅在首轮等待就绪、末轮释放；A/B 共用数据事件，两路 scale 共用 scale 事件。独立大包进度使 scale 无需每轮重复从 GM 搬入。

## 性能优化总结

- 固定 32 核、4×8 网格切分，每核计算完整 K，避免跨核归约。
- A/B 数据按两个基础 K 块搬运，scale 按八个基础 K 块搬运，减少 MTE2 指令数。
- 保持 256×256 输出分块 提高输入复用；基础 K=128 使 FP8 适配 L0A Buffer/L0B Buffer 双缓冲容量。
- L1 Buffer 数据、L1 Buffer scale、L0A Buffer/L0B Buffer 各自使用 ping/pong 和明确的同步事件，交叠搬运和计算。
- 推迟 L0C Buffer 复用等待，使下一输出块的输入预加载可与前一块写回交叠。

## 理论性能对比

以下为在具有 **32 个可用 Cube 核、主频 1.65 GHz** 的Ascend 950PR&950DT系列产品上采集的性能数据。

注意，在不同环境上的性能数据会有所差异，以下数据仅供参考，实际性能以实测为准。

| 指标 | 说明 |
| --- | --- |
| Task Duration(μs) | 核任务总耗时，端到端算子性能以此为准 |
| Block Num | 固定启动的 block 数 |
| aicore_time(μs) | profiler 报告的 AI Core 执行时间 |
| aic_mac_time(μs) / aic_mac_ratio | Cube 计算时间及占比 |
| aic_scalar_time(μs) / aic_scalar_ratio | 标量调度、地址计算等时间及占比 |
| aic_mte1_time(μs) / aic_mte1_ratio | L1 Buffer→L0A Buffer/L0B Buffer 搬运时间及占比 |
| aic_mte2_time(μs) / aic_mte2_ratio | GM→L1 Buffer 搬运时间及占比 |
| aic_fixpipe_time(μs) / aic_fixpipe_ratio | 输出写回时间及占比 |

实测数据：

| Case version | Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| C API MXFP8 Matmul | 1318.15 | 32 | 1317.12 | 1277.07 | 0.97 | 67.57 | 0.05 | 552.28 | 0.42 | 1185.64 | 0.9 | 32.26 | 0.02 |

注意，各流水可以重叠，不能把各时间相加作为任务耗时，Cube时间占比也不等于全部物理核的全局利用率。

### Cube 计算性能分析

FP8 每 cycle 每核处理 16×32×16 次乘加。

Cube 理论运算时间 $T_{cube}$ 为：

$$T_{cube}=\frac{M\times N\times K}{16\times32\times16\times1.65\times10^9\times32}
=\frac{8192^3}{8192\times1.65\times10^9\times32}\approx1271.001\ \mu s$$

该值为纯计算理论下界，不含流水填充、排空、写回等开销。

实测 Cube 时间为 `1277.07 μs`，相对理论值的误差为：

$$E_{cube}=\frac{1277.07-1271.001}{1271.001}\times100\%\approx0.48\%$$

### MTE2 带宽分析

每个 A 行块参与 `N/block_n=32` 个输出分块，每个 B 行块参与 `M/block_m=32` 个输出分块，scale 具有相同复用次数。FP8 每元素为 1 字节，因此 GM→L1 Buffer 总逻辑搬运量为：

$$D_{total}=\frac{N}{block_n}MK+\frac{M}{block_m}NK+
\frac{N}{block_n}M\cdot scale_k+\frac{M}{block_m}N\cdot scale_k$$

$$=2\ GiB+2\ GiB+64\ MiB+64\ MiB=4.125\ GiB=4,429,185,024\ B$$

不重复的输入数据为：

$$D_{input}=MK+NK+M\cdot scale_k+N\cdot scale_k=132\ MiB=138,412,032\ B$$

若每个输入字节仅首次从 HBM 读取、后续全部命中 L2，则后续访问量为 `D_total-D_input=4,290,772,992 B`。沿用原样例的带宽假设——HBM 1.6 TB/s、L2 5 TB/s——可作理想情况下的搬运时间估算：

$$T_{MTE2,model}=\frac{138,412,032}{1.6\times10^{12}}+
\frac{4,290,772,992}{5\times10^{12}}\approx944.662\ \mu s$$

容量采用二进制 MiB/GiB，带宽采用十进制 TB/s。带宽为模型假设，不能替代目标设备实测；HBM 和 L2 数据传输可以交叠，模型也未计入格式转换、指令和缓存冲突。4.125 GiB 是逻辑搬运量，不等于实际 HBM 流量。若按原样例假设的 128 MiB L2 容量，132 MiB 输入无法全部常驻；加上输出和其他流量，实际命中率需要由 profiler 验证。

实测 `aic_mte2_time` 为 `1185.64 μs`，相对模型值 `944.662 μs` 高约 `25.51%`。差异来自缓存命中情况、ND/DN→NZ 数据搬运、指令及同步开销；MTE2 与其他流水存在交叠，不能将该时间直接加到 Cube 时间上。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

### 配置环境变量

请根据当前环境上 CANN 开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。


```bash
source ${install_path}/cann/set_env.sh
```

> **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

### 样例执行

在本样例目录下执行如下命令。

```bash
mkdir -p build && cd build;  # 创建并进入 build 目录
cmake .. -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j;
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin
```

使用 NPU 仿真模式时，设置 `-DCMAKE_ASC_RUN_MODE=sim` 即可。

```bash
cmake .. -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j; # NPU 模式
cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j; # NPU 仿真模式
```

编译选项说明：

| 参数 | 可选值 | 说明 |
|------|--------|------|
| `CMAKE_ASC_RUN_MODE` | `npu` / `sim` | 运行模式：NPU 运行、NPU 仿真 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | 目标 SoC 架构 |

> **注意：** 切换 `CMAKE_ASC_RUN_MODE` / `CMAKE_ASC_ARCHITECTURES` 前需清理 CMake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

执行结果如下，说明精度对比成功。

```bash
test pass!
```

### 性能分析

### msOpProf工具介绍
msOpProf工具是单算子性能分析工具。包含msopprof和msopprof simulator两种使用方式。该工具协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

- 上板性能采集

    通过上板性能采集，可以直接测定算子昇腾AI处理器上的运行时间。该方式适合在板环境中快速定位算子性能问题。

    基于可执行文件demo通过msopprof执行算子调优：
    ```
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
cat ./OPPROF_*/PipeUtilization.csv
```