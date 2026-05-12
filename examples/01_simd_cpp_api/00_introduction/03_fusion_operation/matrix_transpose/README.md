# MatrixTranspose样例

## 概述

本样例以矩阵转置为例，介绍Ascend C SIMD与SIMT混合编程场景下的访存合并优化思路。样例包含2个kernel版本：先实现直接索引转置，再通过UB中转调整数据写回方式，使GM读写更接近连续访问，从而展示混合编程下矩阵转置的全局访存优化方法。

**优化路径**：

- Case 0：直接索引转置版本
- Case 1：UB中转 + 全局访存合并实现转置

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \> CANN 9.0.0

## 目录结构介绍

```text
├── matrix_transpose
│   ├── CMakeLists.txt         # cmake编译文件
│   ├── matrix_transpose.asc   # 矩阵转置样例实现（包含2个case）
│   └── README.md
```

## 样例描述

**样例功能**：

样例实现的功能是固定shape为1024×1024的矩阵转置

矩阵转置的计算公式为：

$$
output(x, y) = input(y, x)
$$

- input：输入矩阵，默认形状为[1024,1024]，数据类型为float
- output：输出矩阵，形状为[1024,1024]，数据类型为float

## 样例实现

### 计算映射与线程切分

对矩阵转置这类二维规则计算，采用tile方式组织数据可以将大矩阵分解为若干局部子块，使每个block只处理一个局部区域，便于索引计算和线程协作。本样例中的2个kernel都基于同一套tile切分方式组织计算，便于比较不同kernel在相同切分方式下的访存行为差异。

本样例采用SIMD与SIMT混合编程方式：核函数内部通过 `asc_vf_call` 调用 `__simt_vf__` 函数。混合编程场景下只支持一维线程块布局，因此二维矩阵tile不能直接映射到二维 `blockIdx.x/blockIdx.y`，tile内线程也不能使用二维 `threadIdx.x/threadIdx.y` 布局。本样例将二维tile坐标展平到一维 `blockIdx.x`，再将tile内32×32个元素展平到一维 `threadIdx.x`，在SIMT VF函数内部分别还原出二维tile坐标和tile内局部坐标。

本样例中子块tile大小取32×32，主要是因为SIMT执行模型中一个warp包含32个线程。同一warp的线程可处理tile中同一行的32个连续元素。这样在访问GM时，一个warp能够按行读取连续数据，有利于形成连续的全局访存模式。

**整体切分方式**：

- 将整个输入矩阵进行tile划分，每个tile大小固定是32×32，一个block对应处理输入矩阵中的一个大小为32×32的tile
- 由于混合编程下核函数只支持一维线程块布局，因此采用一维block切分，block数量为 `bn = grid_x * grid_y`，其中 `grid_x = matrix_width / TILE_DIM`，`grid_y = matrix_height / TILE_DIM`
- 每个一维 `blockIdx.x` 对应一个tile，并在SIMT VF函数内还原为二维tile坐标：

```cpp
int block_x = blockIdx.x % grid_width;
int block_y = blockIdx.x / grid_width;
```

**一维线程块与SIMT线程组织**：

- SIMT VF函数内部同样采用一维线程块布局，通过 `asc_vf_call(..., dim3(THREAD_COUNT), ...)` 启动
- `THREAD_COUNT = TILE_DIM * TILE_DIM = 1024`
- `threadIdx.x` 范围为0-1023，每个线程处理tile内1个元素
- tile内局部二维坐标由一维 `threadIdx.x` 还原：

```cpp
int tile_x = threadIdx.x % TILE_DIM;
int tile_y = threadIdx.x / TILE_DIM;
```

**以一个block为例的映射关系**：

下图给出了一个更直观的映射示意。左侧表示1024×1024的矩阵被划分为 32×32 个 tile。右侧表示其中一个block需要处理的一个tile，其大小为32×32。

<img src="./figure/blockMapping.png" width="60%">

以图中绿色元素为例，可以直接用kernel里的内置变量来推导它在GM中的input坐标。

- tile坐标是(1,2)，对应：
  - block_x = 1
  - block_y = 2
  - blockIdx.x = block_y × grid_width + block_x = 2 × 32 + 1 = 65
- 元素在tile内的局部坐标是(29,2)，对应：
  - threadIdx.x = tile_y × TILE_DIM + tile_x = 2 × 32 + 29 = 93

结合代码中的索引计算：

```cpp
int block_x = blockIdx.x % grid_width;
int block_y = blockIdx.x / grid_width;
int tile_x = threadIdx.x % TILE_DIM;
int tile_y = threadIdx.x / TILE_DIM;
int x_index = block_x * TILE_DIM + tile_x;
int y_index = block_y * TILE_DIM + tile_y;
int index = x_index + width * y_index;
```

代入本例中的变量值：

- block_x = 65 % 32 = 1
- block_y = 65 / 32 = 2
- tile_x = 93 % 32 = 29
- tile_y = 93 / 32 = 2
- x_index = block_x × TILE_DIM + tile_x = 1 × 32 + 29 = 61
- y_index = block_y × TILE_DIM + tile_y = 2 × 32 + 2 = 66

因此，该线程在GM中处理的输入元素坐标就是：

- input[y_index,x_index] = input[66,61]

如果继续代入一维地址公式，则有：

- index = x_index + width × y_index = 61 + 1024 × 66

为方便对比各个版本的性能差异，本样例的所有版本均采用完全一致的线程布局配置：

- 每个tile的大小TILE_DIM=32
- 外层kernel采用一维block数量 `bn = (matrix_width/32) × (matrix_height/32)`，每个block处理一个tile
- 二维tile坐标由一维 `blockIdx.x` 还原，不使用 `blockIdx.y`
- SIMT VF函数内部的线程组织为 `dim3(1024)`，tile内局部坐标由一维 `threadIdx.x` 还原，不使用 `threadIdx.y`

### 性能指标说明

**表1 MatrixTranspose性能指标字段说明表**

|       字段名       | 字段含义                                                                                      |
| :-----------------: | :-------------------------------------------------------------------------------------------- |
|  Task Duration(us)  | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。                    |
|    aiv_time(us)    | Task在AI Vector Core上的理论执行时间，单位为us。                                              |
|  aiv_total_cycles  | 该Task被分配到每个AI Vector Core计算单元上后，每个AI Vector Core计算单元上的执行的cycle总数。 |
|  aiv_vec_time(us)  | vec类型指令（向量类运算指令）耗时，单位us。                                                   |
|    aiv_vec_ratio    | vec类型指令（向量类运算指令）的cycle数在total cycle数中的占用比。                             |
| aiv_scalar_time(us) | 代表scalar类型指令（标量类运算指令）耗时。                                                    |
|  aiv_scalar_ratio  | 代表scalar类型指令（标量类运算指令）的cycle数在total cycle数中的占用比。                      |

除Task Duration外，本例中其余指标均展示的为所有block上性能指标的平均值。

### Case 0：直接索引转置版本

**实现方式**：对应样例中transpose_naive_kernel()函数

该版本直接按照转置公式计算输出坐标，是最直接的transpose实现版本。

**关键代码**：

```cpp
int index_in = x_index + width * y_index;
int index_out = y_index + height * x_index;

output[index_out] = input[index_in];
```

**实现过程拆解**：

1. 线程先按原坐标读取输入元素
2. 线程计算该元素转置后的输出位置
3. 线程把数据直接写到转置后的GM地址

下图展示了Case 0的数据流，其中标红展示了一个warp在读取GM和写入GM时处理的元素。对于同一个warp的线程会读取GM输入中tile的一行元素，写回到GM输出中tile的一列。在读取GM输入时，相邻线程访问的元素地址连续，为连续读，在写回到输出时，相邻线程却被拆散到输出矩阵的不同行上，为不连续写。因此，这一版的核心问题是转置后的写回地址不再连续，这通常会显著影响整体吞吐。

<img src="./figure/case0.png" width="60%">

**性能数据**：

| Task Duration(us) | aiv_time(us) | aiv_total_cycles | aiv_vec_time(us) | aiv_vec_ratio | aiv_scalar_time(us) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      70.304      |    4.257    |     7024.463     |      4.115      |     0.967     |        0.131        |      0.030      |

**性能分析**：

- Case 0的Task Duration为70.304us，作为直接索引转置版本，是后续优化版本的对比基线
- 该版本GM读取仍然是连续读，但GM写回是跨行、非连续写，同一个warp的写请求难以高效合并，因此整体耗时主要受限于写回访存模式

**性能优化建议**：

> ⚠️ **naive transpose通常受限于非连续写访存**
>
> 可以使用UB中数据中转，把不连续的访问转移到UB内，保证GM读写的连续行从而提高性能

---

### Case 1：UB中转 + 全局访存合并的转置版本

**实现方式**：参考transpose_coalesced_kernel()函数实现

该版本通过UB中转将GM的读写改为连续的访问模式，使得全局访存合并。

**关键代码**：

```cpp
int tile_x = threadIdx.x % TILE_DIM;
int tile_y = threadIdx.x / TILE_DIM;

tile[tile_y][tile_x] = input[index_in];
asc_syncthreads();

int block_x = blockIdx.x % grid_width;
int block_y = blockIdx.x / grid_width;

x_index = block_y * TILE_DIM + tile_x;
y_index = block_x * TILE_DIM + tile_y;
int index_out = x_index + y_index * height;

output[index_out] = tile[tile_x][tile_y];
```

**实现过程拆解**：

1. 每个线程从GM读取tile中的1个元素，一个warp会读取一个tile的一行元素
2. 按原坐标把元素写入UB中的tile，一个warp会将读取的一行写入到UB中的tile的一行
3. 同步，保证整个tile已经完整写入UB
4. 每个线程从UB中取数，一个warp会读取UB中tile的一列元素
5. 把取出的值写回GM中的转置位置，一个warp会将读取UB的一列元素写入到GM输出中tile的一行。

下图展示了Case 1的数据流，其中标红和标黄的元素展示了一个warp的线程在读取GM和写入GM时处理的元素。读取GM输入时，整个tile会按照GM的排布搬到UB，在写入GM输出时一个warp的线程会读取UB上的一列元素写回到其对应的转置后的位置。

<img src="./figure/case1.png" width="60%">

与Case 0不同的是，Case 0中线程是“直接把输入元素写到转置后的GM位置”，所以相邻线程会被打散到输出矩阵的不同行上，而Case 1中，线程先把数据放到UB里，把原来不连续的全局写访问转移到UB内的不连续读。因此，这一版的核心收益是：虽然增加了一次UB读写和一次同步，但换来了GM侧“读连续、写也连续”的访问模式，整体耗时通常会明显低于Case 0。

**性能数据**：

| Task Duration(us) | aiv_time(us) | aiv_total_cycles | aiv_vec_time(us) | aiv_vec_ratio | aiv_scalar_time(us) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      44.440      |    2.441    |     4027.577     |      2.295      |     0.941     |        0.134        |      0.054      |

**优化效果分析**：

- 相比Case 0的直接索引转置版本，Case 1的Task Duration从70.304us降低到44.440us，耗时下降约36.8%
- 按Task Duration计算，Case 1整体约为Case 0的1.58倍性能，说明通过UB中转改善GM写回连续性后，端到端耗时有明显下降
- Case 1仍然需要额外的UB读写和同步，因此优化后的Task Duration没有降到纯连续访存的理想水平；这部分耗时是UB中转换取GM访存合并带来的必要开销

**性能优化建议**：

> 💡 **先解决全局访存合并，再处理UB bank conflict**
>
> 在SIMT编程中，全局访存模式通常是第一层瓶颈，先让GM访问连续，再继续细化UB中的访问冲突。

---

## 编译运行

### 切换Case

在 cmake 编译时通过 `-DSCENARIO_NUM=N` 指定要运行的 case：

```bash
cmake -DSCENARIO_NUM=0 ..   # 运行 case 0（可替换为0-1）
```

各 case 说明：

- `0`：直接索引转置版本
- `1`：UB中转 + 全局访存合并实现转置

### 编译执行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

  - 默认路径，root用户安装CANN软件包

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```
  - 默认路径，非root用户安装CANN软件包

    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```
  - 指定路径install_path，安装CANN软件包

    ```bash
    source ${install_path}/cann/set_env.sh
    ```
- 样例执行

  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake -DSCENARIO_NUM=0 ..;    # 运行 case 0（可替换为0-1）
  make -j;                      # 编译工程
  ./demo                        # 执行样例
  ```
  执行结果如下，说明精度对比成功：

  ```text
  [Success] Case accuracy is verification passed.
  ```
