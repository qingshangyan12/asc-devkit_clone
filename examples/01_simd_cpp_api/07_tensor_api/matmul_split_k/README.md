# 基于Tensor API实现的Split-K Matmul样例

## 概述

本样例使用Tensor API完成矩阵乘法的分块搬运和乘加计算，并沿K轴将计算拆分为4个独立分片。每个Cube block计算一个K分片的部分结果，再通过C API提供的DMA原子加法将部分结果累加到同一个GM输出矩阵中。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | > CANN 9.1.0 |

> **说明：** 本样例使用Tensor API，仅支持 `dav-3510` 架构（对应 Ascend 950PR/Ascend 950DT）。

## 目录结构介绍

```text
├── matmul_split_k
│   ├── scripts
│   │   ├── gen_data.py         // 生成float16输入和float32真值
│   │   └── verify_result.py    // 对比输出与真值
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 主机侧二进制文件读写函数
│   ├── matmul_split_k.asc      // Ascend C样例实现与调用代码
│   └── README.md               // 样例说明文档
```

运行数据脚本或样例后，会在当前目录生成 `input/` 和 `output/` 子目录，其中分别保存输入数据、输出数据及golden结果。

## 样例描述

### 样例功能

矩阵乘法计算公式为：

$$C_{M,N}=A_{M,K}\times B_{K,N}$$

K轴被均匀划分为4个分片。第 `splitIdx` 个block计算：

$$C^{(splitIdx)}=A[:, splitIdx\times256:(splitIdx+1)\times256]\times B[splitIdx\times256:(splitIdx+1)\times256, :]$$

最终输出为4个分片结果的逐元素累加：

$$C=\sum_{i=0}^{3}C^{(i)}$$

### 样例规格

| 项目 | 规格 |
|------|------|
| A shape / 类型 / 格式 | `[16, 1024]` / `float16` / ND |
| B shape / 类型 / 格式 | `[1024, 16]` / `float16` / ND |
| C shape / 类型 / 格式 | `[16, 16]` / `float32` / ND |
| K分片数 `SPLIT_K` | `4` |
| 每个分片的K大小 `BASE_K` | `256` |
| Kernel block数量 | `4` |
| Kernel名称 | `matmul_split_k_kernel` |

### 实现流程

| 步骤 | 接口 | 功能 |
|------|------|------|
| 1 | `make_tensor`、`make_frame_layout` | 创建GM、L1、L0A、L0B和L0C张量 |
| 2 | `slice` | 根据 `block_idx` 计算K分片起始位置，并取得A、B的当前分片 |
| 3 | `copy(copy_gm_to_l1)` | 将当前A、B分片从GM搬运到L1，并完成ND到NZ布局转换 |
| 4 | `copy(copy_l1_to_l0a/copy_l1_to_l0b)` | 将L1数据搬运到L0A、L0B，准备Cube计算 |
| 5 | `mmad` | 计算当前K分片的矩阵乘结果，结果保存在L0C中 |
| 6 | `asc_set_atomic_add_float` + `copy(copy_l0c_to_gm)` | 配置float类型DMA原子加法，将L0C结果累加写入GM中的C |
| 7 | 主机侧校验 | 将C拷回主机，与float32 NumPy golden结果比较 |

输出矩阵在Kernel启动前由主机侧清零，以保证4个block的原子累加从零开始。`copy_l0c_to_gm` 完成后关闭DMA原子加法配置，避免影响后续操作。

## 使用的主要接口

- Tensor API：`make_tensor`、`make_mem_ptr`、`make_frame_layout`、`slice`、`make_copy`、`copy`、`make_mmad`、`mmad`。
- C API：`asc_set_atomic_add_float` 和 `asc_disable_dma_atomic`，用于配置和关闭L0C到GM的float原子累加。
- 主机侧运行时接口：使用ACL完成设备初始化、内存分配、数据拷贝、Kernel启动和结果回收。

## 编译运行

在本样例根目录下执行以下命令。

### 配置CANN环境

```bash
source ${install_path}/cann/set_env.sh
```

`${install_path}` 为CANN安装目录。例如：

```bash
source /home/developer/Ascend/cann-9.2.0/set_env.sh
```

### 编译、运行和校验

```bash
mkdir -p build
cd build
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
cd ..
python3 scripts/gen_data.py
./build/demo
python3 scripts/verify_result.py output/output.bin output/golden.bin
```

`CMAKE_ASC_RUN_MODE=sim` 表示使用3510仿真模式；`CMAKE_ASC_ARCHITECTURES=dav-3510` 表示目标架构为 Ascend 950PR/Ascend 950DT。默认运行模式为 `npu`，在实际设备上运行时可省略 `CMAKE_ASC_RUN_MODE=sim`。

切换运行模式前请清理 `build` 目录中的CMake缓存后重新配置。

### 预期结果

校验脚本比较输出和golden的每个float32元素。全部元素通过误差阈值时输出：

```text
test pass! max absolute error: ...
```

