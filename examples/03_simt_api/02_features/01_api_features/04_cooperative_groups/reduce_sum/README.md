# 协作组多粒度归约样例

## 概述

本样例通过归约求和算子展现[协作组API](../../../../../../docs/zh/api/SIMT-API/cooperative_groups/cooperative_groups.md)的基本用法，重点呈现了协作组提供的代码抽象能力。
## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.2.0


## 目录结构介绍

```text
├── reduce_sum
│   ├── figures                             // README中的图片资源
│   ├── CMakeLists.txt                      // 编译工程文件
│   ├── reduce_sum.asc                      // 样例实现文件，含三个kernel与host侧调用
│   ├── README.md
│   └── README_en.md
```


## 样例描述

### 样例功能

本样例给出了3种归约求和算子的写法，分别展现协作组从`grid`、`thread block`、`Warp`粒度控制线程协作的方式。并且将规约求和操作提炼为一个通用函数`reduce_sum`，通过传入不同的协作组对象实现不同粒度的规约操作，展现协作组的代码抽象能力。
### 样例规格

三个用例采用同一执行配置：grid为4个线程块，每个block为1024个线程（默认线程配置），共4096个线程，动态ubuf为0，核函数调用形式为`<<<dim3(4), dim3(1024), DYN_UBUF_SIZE, stream>>>`。


<table>
<tr><td rowspan="1" align="center">样例类型（OpType）</td><td colspan="4" align="center">Cooperative Groups</td></tr>
<tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">input</td><td align="center">[4096]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[1]</td><td align="center">int32</td><td align="center">ND</td></tr>
</table>


### 样例实现

**图 1**  折半归约的过程

![](./figures/halving_reduction.png)

此处以16线程为例：每轮活跃线程将自身持有的值与相距`i`个线程处的值相加，随后活跃线程数减半，间距`i`同步折半。

```cpp
template <typename PtrT>
__aicore__ inline int32_t reduce_sum(const thread_group& g, PtrT x, int32_t val)
{
    const int32_t lane = static_cast<int32_t>(g.thread_rank());
    for (int32_t i = static_cast<int32_t>(g.size()) / 2; i > 0; i /= 2) {
        x[lane] = val;              // ① 写入本线程所属格位
        g.sync();                   // ② 等待全组写入完成
        if (lane < i) {
            val += x[lane + i];     // ③ 读取相距i个线程处的格位
        }
        g.sync();                   // ④ 等待全组读取完成
    }
    return lane == 0 ? val : -1;
}
```

参与归约的线程数为`N = g.size()`，参与计算的相邻线程之间的距离`i`初始值为`N/2`，之后逐轮折半。由于`val`保存在各线程私有的寄存器中，其他线程无法访问，所以提供参数`x`指向暂存区，这部分内存用于存储计算的中间值。

> [!NOTE]说明
> 用例调用`reduce_sum`接口时需要使用`global_idx < num_elements ? input[global_idx] : 0`防止输入越界。

#### 性能指标说明


| 指标 | 说明 |
| --- | --- |
| Task Duration（μs） | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间 |

#### Case0：跨线程块归约

本样例grid内全部线程合并为一组参与归约，调用一次`reduce_sum`得出总和。归约共需12轮折半，每轮都通过grid级屏障进行同步：前两轮的相加跨越block边界，此后归约收敛到block 0内部，但传入`reduce_sum`的组对象仍然是`grid_group`，所以每轮同步仍需要grid内4096个线程全部到达，最后由rank 0的线程直接写出结果。

**图 2**  Case0跨线程块归约

![](./figures/reduce_sum_case0.png)

```cpp
__global__ void reduce_grid_group(
    const int32_t* input, int32_t* grid_workspace, int32_t* total_grid, int32_t num_elements)
{
    grid_group grid = this_grid();
    const int32_t global_idx = (int32_t)grid.thread_rank();
    volatile int32_t* ws = grid_workspace;
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    const int32_t val = reduce_sum(grid, ws, in_val);
    if (grid.thread_rank() == 0) {
        *total_grid = val;
    }
}

```
参与归约的线程为grid内的全部4096个线程，需要跨block归约，而GM是整个grid中所有block共用的内存，所以暂存区`grid_workspace`必须分配在GM上，且`ws`必须加`volatile`限定，保证每轮读取都落到GM上而不是各核私有的Data Cache上，确保轮次间的数据一致性。

**性能数据**：

| Task Duration（μs） |
| :-----------------: |
|        16.101       |

#### Case1：线程块内归约

本样例参与归约的线程为一个block内的1024个线程，4个block各自独立归约。归约共需10轮折半，每轮都通过块级屏障进行同步；各block由rank 0通过`asc_atomic_add`将本块的和累加至同一输出，从而汇总出总和。

**图 3**  Case1线程块内归约

![](./figures/reduce_sum_case1.png)

```cpp
__global__ void reduce_thread_block(const int32_t* input, int32_t* total_block, int32_t num_elements)
{
    __ubuf__ int32_t ub_workspace[THREADS_PER_BLOCK];
    thread_block block = this_thread_block();
    const int32_t global_idx = (int32_t)(blockIdx.x * blockDim.x + block.thread_rank());
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    const int32_t val = reduce_sum(block, ub_workspace, in_val);
    if (block.thread_rank() == 0) {
        asc_atomic_add(total_block, val);
    }
}
```
参与归约的线程在block内部，此时暂存区位于UB上，其大小由`THREADS_PER_BLOCK`在kernel内静态定义，不需要通过参数传入。

**性能数据**：

| Task Duration（μs） |
| :-----------------: |
|        3.514        |

#### Case2：warp内归约

本样例参与归约的线程为一个warp内的32个线程，归约分两级完成：第一级由block内的32个warp各自独立归约，每组32个线程做5轮折半，每轮都通过warp级屏障进行同步，由各warp的rank 0把部分和写入`ub_partial`；两级之间用一次块级屏障，确保32个部分和全部写入完成；第二级由warp 0读取这32个部分和，再做5轮折半，同样只在warp内同步，最后由其rank 0通过`asc_atomic_add`把本块的和累加到同一个输出上，4个block汇总出总和。
**图 4**  Case2warp内归约
![](./figures/reduce_sum_case2.png)

```cpp
__global__ void reduce_block_tile(const int32_t* input, int32_t* total_tile, int32_t num_elements)
{
    __ubuf__ int32_t ub_workspace[THREADS_PER_BLOCK];
    __ubuf__ int32_t ub_partial[WARPS_PER_BLOCK];
    thread_block block = this_thread_block();
    const int32_t global_idx = (int32_t)(blockIdx.x * blockDim.x + block.thread_rank());
    auto tile = tiled_partition<TILE_SIZE>(block);
    const int32_t offset = (int32_t)tile.meta_group_rank() * TILE_SIZE;
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    int32_t val = reduce_sum(tile, ub_workspace + offset, in_val);

    if (tile.thread_rank() == 0) {
        ub_partial[tile.meta_group_rank()] = val;
    }
    block.sync();

    if (tile.meta_group_rank() == 0) {
        val = reduce_sum(tile, ub_workspace, ub_partial[tile.thread_rank()]);

        if (tile.thread_rank() == 0) {
            asc_atomic_add(total_tile, val);
        }
    }
}
```
`TILE_SIZE`取32即一个warp的宽度，使`tiled_partition<TILE_SIZE>(block)`划分出的每个子组恰好落在一个warp内，此时组内`sync`只需等齐32个线程，开销小于要等齐1024个线程的块级屏障。取值大于32会使子组跨越warp，组内`sync`退化为块级屏障，该优势不再存在。

**性能数据**：

| Task Duration（μs） |
| :-----------------: |
|        2.397        |

## 性能对比

三个用例的性能数据如下表：

| 用例 | 参与归约的线程数 | 折半轮数 | 同步次数与作用域 | Task Duration（μs） |
| :--: | :----: | :--: | :--: | :-----------------: |
| 跨线程块归约 | 4096 | 12 | 24次grid级 | 16.101 |
| 线程块内归约 | 1024 | 10 | 20次块级 | 3.514 |
| warp内归约 | 32 | 5 + 5 | 20次warp内 + 1次块级 | 2.397 |

三个用例的输入规模与访存量相同，性能差异主要来自同步的范围：一次同步要等齐的线程越多，先到达的线程就要在屏障上闲置越久。

- warp内归约比线程块内归约快约31.8%。两者的折半轮数相同，暂存区都在UB上，差别只在同步的范围：warp内归约的20次同步每次只需等齐32个线程，两级之间只用一次块级屏障；线程块内归约的20次同步每次都要等齐块内1024个线程。
- 跨线程块归约的耗时约为线程块内归约的4.6倍，原因有两点：一是每次同步都要等齐grid内的4096个线程，等待的范围最大；二是暂存区只能放在GM上，指针还要加`volatile`，每轮读写都直接访问GM，而GM的访问延迟远高于UB。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。运行时以参数指定用例号，单次执行仅运行指定的那一个用例。

  ```bash
  mkdir -p build && cd build;                             # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # 编译工程
  ./cooperative_groups 0                                  # 执行跨线程块归约用例
  ./cooperative_groups 1                                  # 执行线程块内归约用例
  ./cooperative_groups 2                                  # 执行warp内归约用例
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT） |


  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy verification passed.
  ```

## 性能分析

使用`msOpProf`工具获取详细性能数据：

```bash
msopprof ./cooperative_groups 0   # 分析跨线程块归约用例的性能
msopprof ./cooperative_groups 1   # 分析线程块内归约用例的性能
msopprof ./cooperative_groups 2   # 分析warp内归约用例的性能
```

命令完成后，会在默认目录下生成以"OPPROF_{timestamp}_XXX"命名的文件夹，性能数据文件夹结构示例如下：

```text
├──dump                       # 原始的性能数据，用户无需关注
├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
├──L2Cache.csv                # L2 Cache命中率
├──Memory.csv                 # UB，L1和主存储器读写带宽速率
├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
├──OpBasicInfo.csv            # 算子基础信息
├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
├──ResourceConflictRatio.csv  # UB上的bank group、bank conflict和资源冲突率在所有指令中的占比
└──visualize_data.bin         # MindStudio Insight呈现文件
```
