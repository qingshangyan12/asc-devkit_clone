# 基于Tensor API实现的自定义Op样例

## 概述

本样例基于Tensor API实现固定Shape的QKV投影，重点展示如何定义一个自定义Copy Operation，并将它接入Tensor API的Atom调用流程。

输入是Batch模式的三维ND张量。自定义Operation在GM到L1的搬运过程中，将输入的Batch维和Sequence维展平，同时完成ND到NZ的格式转换：

```text
input_flat = reshape(input, [B * S, H])
output = input_flat * weight + bias
```

其中`weight[H, 3H]`和`bias[3H]`在所有Batch之间共享。Q、K、V三个投影结果在输出的最后一维中连续排列，本样例只负责投影计算，不再拆分Q、K、V。

本样例同时演示以下内容：

- 自定义`copy_slice_gm_to_l1` Operation及`copy_traits`接入方式。
- `asc_set_gm2l1_nz_para`和`asc_copy_gm2l1_nd2nz`的3510 C API调用。
- `single_core_m/n/k`与`base_m/n/k`两级静态Tiling。
- M/N维多核切分，以及核内M/N/K三重循环。
- L1、L0A、L0B、BiasTable、L0C之间的Tensor API数据通路。
- 使用`asc_lock`和`asc_unlock`对各流水阶段进行同步。

## 本样例支持的产品及CANN软件版本

| 产品 | NPU架构 | CANN软件版本 |
|------|---------|--------------|
| Ascend 950PR/Ascend 950DT | `dav-3510` | >= CANN 9.2.0 |

## 目录结构介绍

```text
├── scripts
│   ├── gen_data.py             // 生成输入数据和CPU侧真值
│   └── verify_result.py        // 比对样例输出和真值
├── CMakeLists.txt                  // 编译工程文件
├── custom_op_tensor_api.asc       // 自定义Operation、Kernel和Host调用
├── data_utils.h                   // 二进制数据读写工具
├── README_en.md                   // 英文说明文档
└── README.md                      // 中文说明文档
```

## 样例描述

### 样例规格

| Tensor | 逻辑Shape | GM Format | 数据类型 | 说明 |
|--------|------------|-----------|----------|------|
| `input` | `[B, S, H] = [16, 64, 256]` | ND | `half` | Batch输入 |
| `weight` | `[H, 3H] = [256, 768]` | ND | `half` | 共享投影权重 |
| `bias` | `[3H] = [768]` | ND | `half` | 共享偏置 |
| `output` | `[B*S, 3H] = [1024, 768]` | ND | `half` | 投影结果 |

输出在逻辑上也可以解释为`[B, S, 3H] = [16, 64, 768]`。展平只改变逻辑索引，不需要额外的数据搬运。

矩阵乘法参数为：

```text
M = B * S = 1024
K = H     = 256
N = 3 * H = 768
```

Host侧脚本使用固定随机种子生成`half`输入，CPU侧使用`float32`累加计算golden，最后转换为`float16`保存。

本样例使用固定Shape，所有分块均可整除，因此Kernel中不包含尾块分支。当前参数满足：

```text
(B * S) % single_core_m = 0
N % single_core_n       = 0
single_core_m % base_m  = 0
single_core_n % base_n  = 0
K % single_core_k       = 0
single_core_k % base_k  = 0
base_m % S              = 0
```

### Tiling和多核映射

#### 两级Tiling参数

| 参数 | 数值 | 含义 |
|------|------|------|
| `single_core_m` | 256 | 一个Cube核负责的M方向范围 |
| `single_core_n` | 384 | 一个Cube核负责的N方向范围 |
| `single_core_k` | 256 | 一个Cube核覆盖的完整K范围 |
| `base_m` | 128 | 核内一次`mmad`的M tile大小 |
| `base_n` | 192 | 核内一次`mmad`的N tile大小 |
| `base_k` | 64 | 核内一次`mmad`的K tile大小 |

`single_core_m`与`base_m`、`single_core_n`与`base_n`均不相等，单个核内的循环次数为：

```text
m_loop = single_core_m / base_m = 2
n_loop = single_core_n / base_n = 2
k_loop = single_core_k / base_k = 4
```

每个M/N tile执行4次K方向`mmad`。第一个K tile使用`init_with_zero=true`并加载Bias，后续K tile使用累加模式。

K维不在多核之间拆分：每个核都覆盖完整的`K=256`，再以`base_k=64`进行4次核内累加。每核共计算`2 * 2 * 4 = 16`个`mmad` tile，8个核共计算128个`mmad` tile。

#### Cube核映射

Kernel使用一维`block_idx`映射二维的single-core M/N区域：

```cpp
m_iter_index = block_idx % (M / single_core_m);
n_iter_index = block_idx / (M / single_core_m);
single_core_m_start = m_iter_index * single_core_m;
single_core_n_start = n_iter_index * single_core_n;
```

当前配置下：

```text
M / single_core_m = 4
N / single_core_n = 2
num_blocks = 4 * 2 = 8
```

| `block_idx` | M范围 | N范围 | Batch范围 |
|-------------|--------|--------|------------|
| `0..3` | 每核连续256行 | `[0, 384)` | 每核4个Batch |
| `4..7` | 每核连续256行 | `[384, 768)` | 每核4个Batch |

例如`block_idx=5`时，该核负责：

```text
M: [256, 512)
N: [384, 768)
Batch: [4, 8)
```

核内继续将该区域切成`2 x 2`个`[base_m, base_n]`输出tile。由于`base_m=128=2*S`，每个M tile对应两个完整Batch，匹配自定义Operation的Batch搬运语义。

### Tensor构造

#### GM Tensor

| Tensor | Layout | 用途 |
|--------|--------|------|
| `input_gm_tensor` | `nd_ext_layout_ptn(batch, sequence_length, hidden_size)` | `[16, 64, 256]` Batch ND |
| `weight_gm_tensor` | `nd_ext_layout_ptn(hidden_size, projection_size)` | `[256, 768]` ND权重 |
| `bias_gm_tensor` | `nd_ext_layout_ptn(1, projection_size)` | `[1, 768]` ND Bias |
| `output_gm_tensor` | `nd_ext_layout_ptn(flattened_m, projection_size)` | `[1024, 768]` ND输出 |

通过`slice`截取当前M/N/K区域，GM Tensor的原始stride会被保留：输入行stride为`H=256`个元素，Batch stride为`S*H=16384`个元素。

#### Local Tensor和Buffer

| Tensor | 存储位置 | Layout | Shape |
|--------|----------|--------|-------|
| `input_l1_tensor` | L1 | NZ | `[128, 64]` |
| `weight_l1_tensor` | L1 | NZ | `[64, 192]` |
| `bias_l1_tensor` | L1 | ND | `[1, 192]` |
| `input_l0_tensor` | L0A | NZ | `[128, 64]` |
| `weight_l0_tensor` | L0B | ZN | `[64, 192]` |
| `bias_l0_tensor` | BiasTable | ND | `[1, 192]` |
| `output_l0_tensor` | L0C | NZ | `[128, 192]` |

本地Buffer按`base_m/base_n/base_k`分配，避免按整个single-core区域申请更大的L1、L0空间。

### 自定义Copy Operation

#### Operation语义

`copy_slice_gm_to_l1`处理以下固定格式转换：

```text
[B_tile, S, K_tile] ND -> [B_tile * S, K_tile] NZ
```

当前每次搬运：

```text
[2, 64, 64] ND -> [128, 64] NZ
```

调用方式与标准Tensor API Atom一致：

```cpp
const auto custom_copy_atom = make_copy(copy_slice_gm_to_l1{}, gm_to_l1_trait_default{});
copy(custom_copy_atom, input_l1_tensor, input_gm_slice);
```

Operation根据源、目的Layout推导C API参数，并在3510分支中依次调用：

```cpp
asc_set_gm2l1_nz_para(...);
asc_copy_gm2l1_nd2nz(...);
```

源Tensor的Cache Mode会转换为C API要求的`asc_load_l2_cache_mode`后传入。

#### ND2NZ参数

对于当前`[2, 64, 64]`输入Slice和`half`类型：

| C API参数 | 当前值 | 推导方式与含义 |
|------------|--------|------------------|
| `matrix_num` | 2 | `base_m / S`，本次搬运的ND矩阵数量 |
| `n_value` | 64 | `S`，每个ND矩阵的行数 |
| `d_value` | 64 | `base_k`，每个ND矩阵的列数 |
| `src_d_value` | 512 Byte | `H * sizeof(half)`，相邻ND行的GM地址间隔 |
| `src_nd_matrix_stride` | 32768 Byte | `S * H * sizeof(half)`，相邻Batch矩阵的GM地址间隔 |
| `dst_nz_n_stride` | 1 | 目的NZ相邻行起始地址的间隔，单位为32 Byte |
| `dst_nz_c0_stride` | 128 | `base_m`，目的NZ相邻Z分形起始地址的间隔，单位为32 Byte |
| `dst_nz_matrix_stride` | 64 | `S`，目的NZ中相邻Batch矩阵起始地址的间隔，单位为32 Byte |
| `enable_small_c0` | `false` | `d_value`大于4，不使用SmallC0 |

`dst_nz_matrix_stride`保证第二个Batch接续写入`[128, 64]` NZ目标，不覆盖第一个Batch。

### Kernel伪代码

Kernel的核心计算结构如下，模板参数对应源码中的固定Tiling配置：

```cpp
for (m_block_index = 0; m_block_index < m_loop; ++m_block_index) {
    for (n_block_index = 0; n_block_index < n_loop; ++n_block_index) {
        for (k_tile_index = 0; k_tile_index < k_loop; ++k_tile_index) {
            // GM -> L1：输入执行自定义 ND2NZ，权重执行标准 ND2NZ
            // L1 -> L0A/L0B/BiasTable
            // Mmad(base_m, base_n, base_k)
        }
        // L0C -> GM：写回当前 [base_m, base_n] 输出 tile
    }
}
```

其中，输入的`batch_start`由当前M tile的起始行计算得到：

```text
batch_start = (single_core_m_start + m_block_index * base_m) / S
```

因为`base_m=128`且`S=64`，每次自定义Copy都正好覆盖2个连续Batch。K循环只改变输入和权重的列/行起点，不改变输出tile的位置。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  mkdir -p build && cd build;                                                     # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                            # 编译工程，默认NPU模式
  python3 ../scripts/gen_data.py                                                  # 生成测试输入数据
  ./demo                                                                          # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin         # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim`参数即可。

  示例如下：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;   # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理cmake缓存，可在build目录下执行`rm CMakeCache.txt`后重新cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU 运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` |`dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
