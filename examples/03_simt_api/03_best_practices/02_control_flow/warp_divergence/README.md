# SpMV Warp Divergence样例

## 概述

本样例以稀疏矩阵向量乘法（SpMV）为例，介绍Ascend C SIMT编程方式下Warp Divergence对性能的影响及优化思路。样例包含2个Case：

- **Case 1**：一个线程处理一行数据，由于不同行的非零元素数量不同，同一Warp内线程的循环次数不一致，产生严重的Warp Divergence。
- **Case 2**：一个Warp协作处理一行数据，减小Warp Divergence。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构

```text
├── warp_divergence
│   ├── CMakeLists.txt              // cmake编译文件
│   ├── spmv.asc                    // SIMT SpMV 样例实现
│   └── README.md
```

## 样例描述

- 样例功能

  使用CSR格式存储的稀疏矩阵与稠密向量相乘（SpMV），通过对比"一个线程处理一行数据"和"一个Warp协作处理一行数据"两种方式，展示Warp Divergence对性能的影响。

  稀疏矩阵是指大部分元素为零的矩阵。为了节省存储空间，稀疏矩阵通常采用CSR（Compressed Sparse Row）格式存储，仅保存非零元素及其位置信息。CSR格式使用3个数组：
  - **values**：所有非零元素的值，按行优先顺序排列
  - **col_ind**：每个非零元素的列索引，与values一一对应
  - **row_ptr**：行偏移数组，第i行的非零元素在values中的索引为`row_ptr[i] : row_ptr[i+1]`

  以一个 4×5 稀疏矩阵为例：

  ```
  原始矩阵 (4×5):                  CSR 格式:

       c0   c1   c2   c3   c4       
  r0 [ 1.0    0  2.0    0    0 ]    values  = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0]
  r1 [   0  3.0    0  4.0    0 ]    col_ind = [  0,   2,   1,   3,   3,   4,   0]
  r2 [   0    0    0  5.0  6.0 ]    row_ptr = [  0,   2,   4,   6,   7]
  r3 [ 7.0    0    0    0    0 ]

  通过 row_ptr 可定位每行的非零元素：

    row_ptr[0]=0, row_ptr[1]=2 → 第0行: values[0:2] = [1.0, 2.0], col_ind[0:2] = [0, 2]
    row_ptr[1]=2, row_ptr[2]=4 → 第1行: values[2:4] = [3.0, 4.0], col_ind[2:4] = [1, 3]
    row_ptr[2]=4, row_ptr[3]=6 → 第2行: values[4:6] = [5.0, 6.0], col_ind[4:6] = [3, 4]
    row_ptr[3]=6, row_ptr[4]=7 → 第3行: values[6:7] = [7.0],      col_ind[6:7] = [0]
  ```

  SpMV计算`y = A * x`，其中A为CSR格式稀疏矩阵，x为稠密向量，y为输出向量。

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">SpMV</td></tr>
  <tr><td rowspan="5" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">values</td><td align="center">[nnz]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">col_ind</td><td align="center">[nnz]</td><td align="center">uint32</td><td align="center">ND</td></tr>
  <tr><td align="center">row_ptr</td><td align="center">[8193]</td><td align="center">uint32</td><td align="center">ND</td></tr>
  <tr><td align="center">x</td><td align="center">[1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">y</td><td align="center">[8192]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">kernel_spmv_thread / kernel_spmv_warp</td></tr>
  </table>

> nnz 表示稀疏矩阵中非零元素的个数。样例使用的稀疏矩阵大小为8192×1024，每行非零元素个数为1~64。

## 样例实现

本样例包含2个独立的kernel，每个kernel对应特定的Case版本。

| Case   | 实现特点                                              | 使用的核函数             | 优化特性                |
|--------|---------------------------------------------------|--------------------|---------------------|
| Case 1 | 每个线程独立处理一行数据                   | kernel_spmv_thread | 基线版本，一个线程处理一行数据          |
| Case 2 | 一个Warp协作处理一行数据 | kernel_spmv_warp   | Warp协作处理一行数据，减小Warp Divergence |

两个Case使用相同的启动配置：`gridDim=8, blockDim=1024`，共8192个线程，除kernel内部实现外其余配置一致。

### 性能指标说明

|             字段名             | 字段含义                                             |
|:---------------------------:|:-------------------------------------------------|
|      Task Duration(μs)      | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。          |
|      aiv_total_cycles       | Task在 Vector Core上执行所消耗的CPU周期（Cycle）总数。          |
|   Read Main Memory          | 读主存储器数据的次数。                                       |

### Case 1: 一个线程处理一行数据（基线版本）

**实现方式**：每个线程根据全局ID确定处理的行号，独立遍历该行的所有非零元素并累加乘积。同一Warp内32个线程处理32行，各行非零元素个数不同，循环次数从1到64不等。

**关键代码**：

```cpp
float sum = 0.0f;
uint32_t row_start = row_ptr[row];
uint32_t row_end = row_ptr[row + 1];
for (uint32_t j = row_start; j < row_end; j++) {
    sum += values[j] * x[col_ind[j]];
}
y[row] = sum;
```

**性能数据**：

| Task Duration(μs) | aiv_total_cycles | Read Main Memory |
|:-----------------:|:----------------:|:----------------:|
|      81.811       |     127124       |      39808       |

**性能数据分析**：

- 在本实现中，不同行的非零元素数量不同（1~64个）。同一Warp内的32个线程各自处理不同行数据，各线程的循环次数不同，退出循环的时间不一致，产生严重的Warp Divergence。假设一个Warp中线程0处理非零元素个数为3的行，线程1处理非零元素个数为64的行。线程0在3次循环后进入空闲等待，而线程1需要64次循环。这种等待在每个Warp中都会发生，造成大量计算资源浪费。

**优化建议**：

> 💡 **使用Warp协作处理一行数据**
>
> 将"一个线程处理一行数据"改为"一个Warp协作处理一行数据"，使同一Warp内线程的循环次数基本相同，减小Warp Divergence。

### Case 2: 一个Warp协作处理一行数据

**实现方式**：一个Warp的32个线程协作处理同一行数据，各线程以`warpSize`为步长访问该行的非零元素，循环次数相同。使用`asc_reduce_add`对Warp内各线程的部分和进行规约，由线程0写出最终结果。

**关键代码**：

```cpp
float sum = 0.0f;
uint32_t row_start = row_ptr[row];
uint32_t row_end = row_ptr[row + 1];
for (uint32_t j = row_start + lane_id; j < row_end; j += warpSize) {
    sum += values[j] * x[col_ind[j]];
}
sum = asc_reduce_add(sum);
if (lane_id == 0) {
    y[row] = sum;
}
```

**性能数据**：

| Task Duration(μs) | aiv_total_cycles | Read Main Memory |
|:-----------------:|:----------------:|:----------------:|
|      31.772       |      49727       |      2278        |

**优化效果分析**：

- 同一Warp内的所有线程协作处理同一行数据，仅在最后一轮循环中部分线程可能不参与计算，不会产生较大的Warp Divergence，相比Case 1的81.811μs，Task Duration降至31.772μs。
- 与Case 1中同一Warp内各线程访问不同行的分散地址不同，Case 2中Warp内各线程访问同一行的连续元素，实现了访存合并，读主存储器次数从Case 1的39808次降至2278次。

## 性能对比总结

### Ascend 950PR性能数据

**综合优化效果**：
- 从Case 1基线版本到Case 2优化版本，Task Duration从81.811μs降低到31.772μs，耗时下降约61.1%

| Case   | Task Duration(μs) | Task Duration相对Case 1 | 优化点                        |
|--------|-------------------|-----------------------|----------------------------|
| Case 1 | 81.811            | **1x**                | 基线版本，一个线程处理一行数据          |
| Case 2 | 31.772            | **0.39x耗时**          | Warp协作处理一行数据，减小Warp Divergence，访存合并 |

## 调优建议

1. **减小Warp Divergence**：当同一Warp内线程因条件分支执行不同代码路径时，硬件串行执行各分支，导致部分线程空等。常见场景包括循环次数不一致、条件分支差异等。可通过Warp协作处理方式，使同一Warp内线程循环次数基本相同，减小Warp Divergence。

2. **利用访存合并**：确保Warp内各线程访问连续的内存地址，提升访存效率。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../docs/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  SCENARIO_NUM=1                       # 选择执行场景，可选1-2
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./demo                               # 执行样例
  ```

- 编译选项说明

  | 选项                        | 可选值        | 说明                                                |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM`            | `1`-`2`    | 样例类型，默认为1                                         |

  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy verification passed.
  ```