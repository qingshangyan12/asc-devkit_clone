# MxFP4 Matmul 基础 API 高性能样例

## 概述

本样例以 MxFP4 矩阵乘法为例，介绍基于 Ascend C 基础 API 和静态 Tensor 编程范式的高性能实现方法。样例使用手工地址管理和手工同步，通过多核切分、 L1/L0 Ping-Pong 双缓冲、A/B 与 scale 数据异步大包搬运等方式完成 MxFP4 Matmul 计算。


## 支持的产品

- Ascend 950PR / Ascend 950DT

## 目录结构介绍

```
├── matmul_mxfp4_basic_api_high_performance
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── mmad_mx.asc             // Ascend C 基础 API 样例实现
```

## 样例描述

### 样例功能

样例实现固定 shape 为 `8192 x 8192 x 8192` 的 MxFP4 矩阵乘法，输入包括 MxFP4 矩阵 A/B 以及对应的量化缩放系数 scaleA/scaleB，输出为 bfloat16 矩阵 C。

### MxMatmul 简介

MxMatmul 是带量化缩放系数的矩阵乘法。与普通 Matmul 相比，MxMatmul 除 A/B 两路矩阵输入外，还额外引入 scaleA/scaleB 两路 scale 输入。

计算公式如下：

$$
C = (A \otimes \text{scaleA}) \times (B \otimes \text{scaleB})
$$

其中 $\otimes$ 表示广播乘，K 方向每 32 个元素共享一个 scale。

### 数据规格

| 输入/输出 | 逻辑形状 | 数据类型 | 数据排布类型 | 说明 |
|------|------|----------|------|------|
| A | `[M, K]` | `fp4x2_e1m2_t` | ND|左矩阵，脚本按 fp4x2 打包写入 |
| scaleA | `[M, scaleK]` | `fp8_e8m0_t` | ND|A 矩阵的缩放因子，K 方向每 32 个元素共享一个 scale |
| B | `[K, N]` | `fp4x2_e1m2_t` | DN|右矩阵，脚本预转置后按 `[N, K]` 打包写入 |
| scaleB | `[scaleK, N]` | `fp8_e8m0_t` | DN|B 矩阵的缩放因子，K 方向每 32 个元素共享一个 scale |
| C | `[M, N]` | `bfloat16_t` | ND|输出矩阵 |

其中：

- `M=N=K=8192`
- `scaleK = align_even(ceil(K / 32))`
- A/B 以 `fp4x2_e1m2_t` 形式存储，每个字节打包 2 个 fp4 元素

## 样例实现

### 实现要点

- 使用基础 API 静态 Tensor 编程方式实现 MxFP4 Matmul，kernel 中显式管理 GM、L1、L0A、L0B、L0C 的地址与数据流。
- L1 和 L0 均采用 Ping-Pong 双缓冲，降低 GM 到 L1、L1 到 L0、Cube 计算和 Fixpipe 写回之间的等待。
- A/B 数据使用 `fp4x2_e1m2_t` 打包搬运，scaleA/scaleB 使用 `fp8_e8m0_t` 搬运。
- 固定使用 32 核，M 方向切 4 份，N 方向切 8 份，每个核计算一个 M/N 子块。

### 关键参数

| 参数 | 值 | 说明 |
|------|----|------|
| `M=N=K` | `8192` | 固定矩阵规模 |
| `baseM=baseK=baseN` | `256` | Cube 基本块大小 |
| `singleCoreM` | `2048` | 单核 M 方向计算范围 |
| `singleCoreN` | `1024` | 单核 N 方向计算范围 |
| `singleCoreK` | `8192` | 单核 K 方向计算范围 |
| `stepKa=stepKb` | `2` | A/B 在 K 方向的大包搬运步长 |
| `scaleFactorKa=scaleFactorKb` | `4` | scaleA/scaleB 相对 A/B 的 K 方向搬运比例 |
| `Block Num` | `32` | 使用核数 |

## 性能对比总结

### Ascend 950PR 芯片性能数据

| Case version | Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 基础API MXMatmul | 682.152 | 32 | 681.17 | 639.276 | 0.938 | 58.661 | 0.086 | 291.996 | 0.429 | 596.412 | 0.876 | 33.942 | 0.05 |


## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

### 配置环境变量

请根据当前环境上 CANN 开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

- 默认路径，root 用户安装 CANN 软件包
  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

- 默认路径，非 root 用户安装 CANN 软件包
  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

- 指定路径 install_path，安装 CANN 软件包
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

### 样例执行

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

## 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./demo
```

当前目录下会生成 PROF_ 前缀的文件夹，`mindstudio_profiler_output` 目录保存 Host 和各个 Device 的性能数据汇总，性能数据分析推荐查看该目录下文件：

```bash
PROF_xxxx_XXXXXX
├── device_{id}
├── host
├── mindstudio_profiler_log
└── mindstudio_profiler_output
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```

查看具体的性能分析结果：

```bash
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```
