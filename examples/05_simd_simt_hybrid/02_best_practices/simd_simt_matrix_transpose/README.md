# MatrixTranspose样例

## 概述

本样例以矩阵转置为例，介绍Ascend C SIMD与SIMT混合编程方式下的访存优化思路。样例从连续复制基线和直接全局内存转置出发，逐步引入MTE搬运、UB中转、tile映射、UB padding和double buffer，展示在混合编程场景中如何用Vector侧MTE完成GM到UB的数据搬运，再通过SIMT VF完成UB内转置方向读取与GM写回。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```text
├── simd_simt_matrix_transpose
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── matrix_transpose.asc   // 矩阵转置样例实现
│   ├── figures                // README中的图片资源
│   └── README.md
```

## 样例描述

- 计算公式：

  $$
  output(x, y) = input(y, x)
  $$

  - input为输入矩阵，形状为[H,W]，数据类型为float。
  - output为输出矩阵，形状为[W,H]，数据类型为float。
- 样例规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">MatrixTranspose</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">copy_kernel / transpose_naive_kernel / transpose_ub_*_kernel</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过编译期宏`SCENARIO_NUM`选择执行路径。Host侧以三槽形式启动`__global__ __vector__`核函数；核函数内部使用MTE接口将GM数据搬运到UB，并通过`asc_vf_call`调用`__simt_vf__`函数完成SIMT线程级处理。

| Case | 核函数 | 线程块数量 | SIMT线程数 | 本步引入的变化 |
| --- | --- | --- | --- | --- |
| 0 | `copy_kernel` | `core` | 2048 | 连续复制基线，Vector侧通过MTE执行GM->UB->GM搬运 |
| 1 | `copy_kernel` | `core` | 1024 | 连续复制基线，线程数与后续1024线程转置版本对齐 |
| 2 | `transpose_naive_kernel` | `core` | 2048 | 直接SIMT全局内存转置，GM连续读、非连续写 |
| 3 | `transpose_ub_2tile_kernel` | `ceil(tiles / 2)` | 2048 | MTE搬入两个32x32 tile到UB，再由SIMT转置读UB并写回GM |
| 4 | `transpose_ub_2tile_kernel` | `core` | 2048 | 固定Thread Block数，每个Thread Block循环处理多个tile分组 |
| 5 | `transpose_ub_kernel` | `core` | 1024 | 每轮处理一个32x32 tile，降低线程数与索引复杂度 |
| 6 | `transpose_ub_pad_kernel` | `core` | 1024 | UB tile增加2列padding，缓解转置读阶段的bank冲突 |
| 7 | `transpose_ub_pad_kernel` | `core` | 1024 | 使用两份带padding的UB tile，为double buffer形式预留轮换buffer |

其中`core`为硬件vector core数，通过`aclrtGetDeviceInfo(ACL_DEV_ATTR_VECTOR_CORE_NUM)`在运行时查询获得。`tiles`为总tile数`(W/32) x (H/32)`，1024x1024时为1024。

### 线程块与tile映射

从Case 3开始，矩阵按32x32的tile处理。下图左侧表示1024x1024矩阵被划分为32x32个tile，右侧表示其中一个32x32的tile内的线程映射关系。tile内列索引记为`tx`，tile内行索引记为`ty`。

<img src="./figures/blockMapping.png" width="60%">

以图中绿色元素为例，可以根据tile坐标和tile内局部坐标推导它在GM中的input坐标。

- 图中绿色元素所在的tile坐标是`(2, 1)`。对于1024x1024矩阵，`tiles_x = 1024 / 32 = 32`，因此对应`tile_id = 2 * 32 + 1 = 65`。
- 图中绿色元素在tile内的局部坐标是`(2, 29)`，对应`ty = 2`、`tx = 29`。

代入坐标计算：

```cpp
uint32_t tile_y = tile_id / tiles_x;
uint32_t tile_x = tile_id - tile_y * tiles_x;
uint32_t x_index = tile_x * TILE_DIM + tx;
uint32_t y_index = tile_y * TILE_DIM + ty;
uint32_t input_offset = x_index + y_index * width;
```

### 混合编程数据流

Case 3及之后的转置主路径把GM到UB的数据搬运放在Vector侧执行：

```cpp
asc_set_gm2ub_loop_size(TILE_DIM, 1);
asc_set_gm2ub_loop1_stride(width * sizeof(float), ub_row_bytes);
asc_copy_gm2ub_align(dst, src, 1, TILE_ROW_BYTES, 0, 0, false, 0, 0, 0);
asc_set_gm2ub_loop_size(1, 1);
```

MTE搬运完成后，通过流水同步保证SIMT VF读取到完整tile：

```cpp
asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
asc_vf_call<simt_store_tile>(dim3(TILE_DIM, TILE_DIM, 1), output, tile, height, tiles_x, tile_id);
```

UB中转的核心思想如下图所示：GM读取阶段按输入矩阵行方向连续搬入UB；GM写回阶段交换输出tile坐标，使同一个warp写出输出矩阵的一行。原本非连续的GM写被转移为UB内转置方向读取。

<img src="./figures/case3.png" width="60%">

### Case说明

#### Case 0 / Case 1：MTE连续复制基线

Case 0和Case 1不执行转置，只用于对比连续GM搬运能力。Vector核函数将连续数据从GM搬到UB，再从UB写回GM；SIMT VF只参与对UB tile的简单触达，用于保持混合编程调用路径。

#### Case 2：直接全局内存转置

Case 2不使用UB中转。SIMT线程读取`input(row, col)`，直接写入`output(col, row)`。同一个warp内相邻线程的GM读取地址连续，但写回地址跨输出矩阵的不同行，难以形成连续写事务。

#### Case 3 / Case 4：MTE搬入UB后转置写回

Case 3按切分后的tile数量设置Thread Block数量，每个Thread Block处理两个tile。Case 4将Thread Block数量固定为硬件vector core数，并在核函数内部循环处理多个tile分组。两者都由Vector侧MTE完成GM到UB的tile搬入，再由SIMT VF按转置方向读取UB并连续写回GM。

#### Case 5：1024线程UB中转

Case 5每个Thread Block每轮处理一个32x32 tile，SIMT线程配置为`dim3(32, 32, 1)`。相比2048线程的双tile版本，该实现的索引逻辑更简单，也更接近一个tile对应一个Thread Block的自然映射。

#### Case 6：UB中转+padding

Case 6把UB中的tile布局从32x32调整为32x34。MTE搬入时通过`asc_set_gm2ub_loop1_stride`将UB目的行跨度设置为`34 * sizeof(float)`，使每行末尾保留2列padding，后续SIMT VF仍只读取有效的32列。

下面以Ascend 950PR/Ascend 950DT的UB划分规则为例说明padding的作用。

<img src="./figures/bank结构示意图.png">

UB内连续地址按32B粒度映射到bank0到bank15：

<img src="./figures/bank内存排布示意图.png">

Case 5中，一个warp转置读UB时会读取同一列的32个元素，容易集中访问相同bank group的subbank资源：

<img src="./figures/case5bank.png">

Case 6增加padding后，行跨度由32个float变为34个float，同一列元素会错开排布到不同subbank，降低转置读阶段的bank冲突风险：

<img src="./figures/case6bank.png">

#### Case 7：UB中转+padding+double buffer形式

Case 7使用两份32x34的UB tile按`cnt`轮换，为double buffer形式预留独立buffer。该版本的核心目的是展示混合编程场景下如何在Vector侧组织MTE搬入buffer，并把当前buffer交给SIMT VF处理。

## 编译运行

编译前请先加载CANN环境变量：

```bash
source ${install_path}/cann/set_env.sh
```

编译运行命令如下：

```bash
mkdir -p build
cd build
SCENARIO_NUM=7
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM} ..
make -j
./matrix_transpose 1024
```

`SCENARIO_NUM`取值范围为`0`到`7`，默认值为`7`。矩阵规模可通过运行参数指定，但需要是32的整数倍。

运行成功时，输出示例如下：

```text
Usage: ./matrix_transpose [matrix_size] (compile-time SCENARIO_NUM=7)
Running case 7: transpose_mte_ub_pad_db...
Threads/Thread Block: 1024
Matrix shape: 1024 x 1024
[Success] Case accuracy verification passed.
```

## 性能分析

可使用`msprof op`采集性能数据：

```bash
msprof op ./matrix_transpose 1024
```

本样例的混合编程版本与纯SIMT版本实现路径不同，性能数据需要重新采集后再填入文档。分析时建议优先关注`OpBasicInfo.csv`中的`Task Duration`，并结合`PipeUtilization.csv`中的MTE2、Vector流水占比，以及`ResourceConflictRatio.csv`中的UB bank冲突信息判断瓶颈。
