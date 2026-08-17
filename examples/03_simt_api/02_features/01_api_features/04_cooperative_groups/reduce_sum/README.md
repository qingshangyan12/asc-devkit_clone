# 协作组多粒度规约样例

## 概述

本样例基于Ascend C SIMT编程模型，演示cooperative_groups协作组库如何将线程组织为不同层级并执行高效的并行规约。

样例设计一个通用规约接口`reduce_sum`，形参取`thread_group`基类；围绕该接口构造三个用例，分别以跨块（`grid_group`）、整块（`thread_block`）、子组（`thread_block_tile`）为规约域。三处调用的书写形式一致，粒度差异由传入的组对象承载。

编程模型为纯SIMT形态。`grid_group`不支持SIMD与SIMT混合编程场景，本样例不涉及该场景。

## 支持的产品

Ascend 950PR、Ascend 950DT。

## 支持的CANN软件版本

>= 9.2.0


## 目录结构介绍

```text
├── reduce_sum
│   ├── figures                             // README中的图片资源
│   ├── CMakeLists.txt                      // 编译工程文件
│   ├── reduce_sum.asc                      // 样例实现文件，含三个kernel与host侧调用
│   ├── README.md                           
│   └── README_en.md                        
```

`CMakeLists.txt`为编译工程文件；`reduce_sum.asc`中包含通用规约接口`reduce_sum`、三个用例的SIMT kernel实现，以及host侧的参数解析、用例分发与结果校验。

## 样例描述

### 样例功能

输入为一个`int32`数组，`input[i] = i + 1`，共4096个元素，ND格式；输出为1个`int32`，即规约总和。按等差数列求和得解析解`4096 × 4097 / 2 = 8390656`，校验期望值由该公式在代码中算出。

三个用例的规约域划分不同，但覆盖同一批数据且互不重叠，三个总和均为该解析解。三者的调用链与各自的规约层级如下：

**图 1**  reduce_sum的调用链与规约层级

![](./figures/reduce_sum_dispatch.png)

三个用例各自具备独立kernel、独立launch与独立显存，单次执行仅运行其中一个。host侧的用例函数按暂存区所在的存储层级归并为两个：跨块规约的暂存区在全局内存，整块与子组两个场景的暂存区均在共享内存，因为case1和case2实现过程基本一致，所以将这两个case放到一个函数中。

### 样例规格

三个用例采用同一执行配置：grid为4个线程块，每块1024线程即32个warp，共4096线程，动态ubuf为0，启动形态为`<<<dim3(4), dim3(1024), DYN_UBUF_SIZE, stream>>>`。`THREADS_PER_BLOCK`取1024，为默认线程配置下每块的线程数；`TILE_SIZE`取32以对齐warp宽度，使子组恰好落在单个warp内。


<table>
<tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">input</td><td align="center">[4096]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td rowspan="3" align="center">样例输出</td><td align="center">total_grid（CASE=0）</td><td align="center">[1]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td align="center">total_block（CASE=1）</td><td align="center">[1]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td align="center">total_tile（CASE=2）</td><td align="center">[1]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">reduce_grid_group / reduce_thread_block / reduce_block_tile</td></tr>
<tr><td rowspan="1" align="center">gridDim</td><td colspan="4" align="center">(4, 1, 1)</td></tr>
<tr><td rowspan="1" align="center">blockDim</td><td colspan="4" align="center">(1024, 1, 1)</td></tr>
</table>


### 样例实现

三个用例共用同一份规约实现，仅在传入的组对象、暂存区与初值上不同：

```cpp
reduce_sum(grid,  ws,                    input[global_idx]);          // case0
reduce_sum(block, ub_workspace,          input[global_idx]);          // case1
reduce_sum(tile,  ub_workspace + offset, input[global_idx]);          // case2第一级
reduce_sum(tile,  ub_workspace,          ub_partial[thread_rank()]);  // case2第二级
```

接口的形参取`thread_group`基类，函数体只使用`thread_rank()`、`size()`、`sync()`三个基类接口。`g.sync()`落到warp内同步、块级屏障还是grid级屏障，由基类依组的实际类型分派决定，接口自身不感知这一分派过程：

```cpp
template <typename PtrT>
__aicore__ inline int32_t reduce_sum(const thread_group& g, PtrT x, int32_t val)
{
    const int32_t lane = static_cast<int32_t>(g.thread_rank());
    for (int32_t i = static_cast<int32_t>(g.size()) / 2; i > 0; i /= 2) {
        x[lane] = val;              // ① 写入本线程所属格位
        g.sync();                   // ② 等待全组写入完成
        if (lane < i) {
            val += x[lane + i];     // ③ 前半段读取后半段伙伴的格位
        }
        g.sync();                   // ④ 等待全组读取完成
    }
    return lane == 0 ? val : -1;
}
```

记规约域大小为`N = g.size()`，跨距`i`自`N / 2`起逐轮折半，共`log2(N)`轮。每轮的四步操作与最终的返回值为三个用例所共用：全组`N`个线程各执行`x[lane] = val`，将当前持有的值写入暂存区中与自身rank对应的格位；`g.sync()`等待全组写入完成；`lane < i`的线程执行`val += x[lane + i]`，读取跨距为`i`的伙伴格位并在寄存器内累加，本轮活跃值由`i × 2`个减至`i`个；`g.sync()`等待全组读取完成，以免下一轮的写入覆盖尚未读走的格位。循环在`i`减至0时结束，此时lane 0持有全组之和，其余线程返回−1，交由调用方按各自的汇总方式处置。

每轮相加的两个值各自代表一批元素，两批之间没有重叠，相加得到的新值就代表这两批元素之和。因此每轮结束时，剩下的活跃值合起来仍然覆盖全部元素，不重不漏，只是持有值的线程数减半。这一点与规约域大小`N`无关，三种粒度算出的总和因此相同。

**图 2**  折半规约的实现过程：跨距逐轮折半，活跃线程数随之减半

![](./figures/halving_reduction.png)

形参`x`所指的暂存区承担组内线程交换中间值的职能。`val`位于各线程私有的寄存器，跨线程的合并经这块参与线程均可寻址的内存完成。暂存区容量按`g.size()`格开满，格号取组内rank，执行结束后其中留下的是最后一轮的中间态，不回读至host侧。所在的存储层级由规约域的跨度决定：跨越block时为全局内存，不超出block时为共享内存。

模板参数`PtrT`承接暂存区指针的地址空间限定符，取全局内存裸指针或`__ubuf__ int32_t*`，粒度的分派仍由组对象`g`完成。

三个kernel的参数表末位均带`uint32_t num_elements`，取输入处各有一句`global_idx < num_elements ? input[global_idx] : 0`。线程数由执行配置固定为4096，下标超出元素个数的线程取加法单位元0参与求和，与其余线程一同写入格位并走完各轮折半与同步。

#### 1：跨块规约场景

规约域为grid内全部4096个线程，一次`reduce_sum`调用得出总和。

```cpp
__global__ void reduce_grid_group(
    const int32_t* input, int32_t* grid_workspace, int32_t* total_grid, uint32_t num_elements)
{
    grid_group grid = this_grid();
    const uint32_t global_idx = static_cast<uint32_t>(grid.thread_rank());
    volatile int32_t* ws = grid_workspace;
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    const int32_t val = reduce_sum(grid, ws, in_val);
    if (grid.thread_rank() == 0) {
        *total_grid = val;
    }
}
```

`grid_group`的`thread_rank()`即全局序号0~4095，直接用作`input`的下标。

规约域跨越block，暂存区须在整个grid范围内可见，四层存储中只有全局内存满足该要求。全局内存由host侧分配并初始化，device侧只能通过kernel参数获取其指针，因此暂存区以形参`int32_t* grid_workspace`的形式传入。三个kernel中只有本场景带有workspace形参。

规约域为4096，共12轮。前两轮的读取跨越block边界：`i = 2048`时块0与块1的lane 0~2047读取块2、块3写入的格位，`i = 1024`时块0的lane 0~1023读取块1写入的格位。第三轮起跨距降至512以内，加法收敛至块0内部。

暂存区指针在传入`reduce_sum`前先转为`volatile int32_t*`。Data Cache为各核私有，`grid.sync()`只对齐各核的执行进度，不使已有的缓存副本失效，跨block的读取须经`volatile`绕过缓存才能取到伙伴block当轮写出的值。

后十轮的加法虽已收敛至块0，组对象仍为`grid_group`，`g.sync()`每次仍要求全grid的4096个线程到齐，块1至块3的3072个线程需跟随完成每轮的写入与两次同步。规约域取grid粒度时，同步的作用域始终覆盖全部4096个线程。

12轮结束后全grid仅rank 0持有总和，由`*total_grid = val`直接写出。写者唯一，不需要原子加，`total_grid`也不需要预先清零。

#### 2：整块规约场景

规约域为一个线程块的1024个线程，四个block各自规约出部分和，再经原子加汇总。

```cpp
__global__ void reduce_thread_block(const int32_t* input, int32_t* total_block, uint32_t num_elements)
{
    __ubuf__ static int32_t ub_workspace[THREADS_PER_BLOCK];

    thread_block block = this_thread_block();
    const uint32_t global_idx = static_cast<uint32_t>(blockIdx.x) * static_cast<uint32_t>(blockDim.x)
                              + static_cast<uint32_t>(block.thread_rank());
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    const int32_t val = reduce_sum(block, ub_workspace, in_val);

    if (block.thread_rank() == 0) {
        asc_atomic_add(total_block, val);
    }
}
```

规约域不再跨越block，暂存区只需在块内可见，共享内存即可满足。共享内存的片上偏移由编译器在编译期排定，因此暂存区可直接在kernel内以`__ubuf__ static`声明，无需从参数表传入，函数签名比跨块场景少一个形参。静态声明要求长度为编译期常量，此处取`constexpr int32_t THREADS_PER_BLOCK`。

`block.thread_rank()`返回的是块内序号0~1023，不能直接用作输入下标，须先按`blockIdx.x * blockDim.x + block.thread_rank()`换算为全局下标。rank的语义由全局收窄至块内，这是本场景与跨块场景在实现上的主要差异。四个block各自从0开始编号，每块都有自己的rank 0，因此四块各产出一个部分和。

规约域为1024，共10轮。前5轮`i`自512递减至32，活跃线程横跨多个warp，由块级屏障完成同步；`i = 32`之后活跃线程收缩至lane 0~31，恰为一个warp，但组对象仍为`thread_block`，同步仍分派至块级屏障。子组规约场景要优化的正是这最后5轮，将同步替换为warp内同步。

暂存区的读写不出核，`x[lane] = val`与`val += x[lane + i]`均作用于本核的Unified Buffer。参与线程共享同一块片上内存，`asc_syncthreads()`之后写入的值即对块内全部线程可见，读取时无需`volatile`。

四个block的部分和与汇总：

| block | 覆盖元素 | 部分和 |
|---|---|---|
| 0 | 1~1024 | 524800 |
| 1 | 1025~2048 | 1573376 |
| 2 | 2049~3072 | 2621952 |
| 3 | 3073~4096 | 3670528 |

通式为`524800 + 1048576k`（`k`为block索引，取值0~3）。四个部分和经`asc_atomic_add`累加，结果与block的完成顺序无关。总和由这四次累加构成，`total_block`在launch前由`aclrtMemset`清零。

#### 3：子组规约场景

规约分两级完成，`reduce_sum`在同一个kernel内被调用两次，两次的规约域均为32。

```cpp
__global__ void reduce_block_tile(const int32_t* input, int32_t* total_tile, uint32_t num_elements)
{
    __ubuf__ static int32_t ub_workspace[THREADS_PER_BLOCK];   // 第一级，32个子组分段共用
    __ubuf__ static int32_t ub_partial[WARPS_PER_BLOCK];       // 两级之间传递部分和

    thread_block block = this_thread_block();
    const uint32_t global_idx = static_cast<uint32_t>(blockIdx.x) * static_cast<uint32_t>(blockDim.x)
                              + static_cast<uint32_t>(block.thread_rank());

    auto tile = tiled_partition<TILE_SIZE>(block);
    const int32_t offset = static_cast<int32_t>(tile.meta_group_rank()) * TILE_SIZE;

    // 第一级：32路并行，规约域32
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    int32_t val = reduce_sum(tile, ub_workspace + offset, in_val);
    if (tile.thread_rank() == 0) {
        ub_partial[tile.meta_group_rank()] = val;
    }

    block.sync();

    // 第二级：仅warp 0执行，规约域仍为32，输入换为部分和
    if (tile.meta_group_rank() == 0) {
        val = reduce_sum(tile, ub_workspace, ub_partial[tile.thread_rank()]);
        if (tile.thread_rank() == 0) {
            asc_atomic_add(total_tile, val);
        }
    }
}
```

`tiled_partition<32>(block)`以线程块为父组，将其切分为`1024 / 32 = 32`个子组，全grid共128个。子组宽度与warp宽度相同，因此`sync()`分派至warp内同步路径。返回类型为`thread_block_tile<TILE_SIZE, thread_block>`，由子组宽度与父组类型两个模板参数确定，接收处用`auto`承接即可，无需写出完整类型。

切分之后有两个rank需要区分：`tile.thread_rank()`为warp内序号0~31，`reduce_sum`内部的`lane`取该值；`tile.meta_group_rank()`为该子组在父组中的编号0~31，即warp号。`offset = tile.meta_group_rank() * TILE_SIZE`按warp号计算偏移，使32个warp在同一块`ub_workspace`上各占一段独立的32格，互不干扰。

第一级的规约域为32，折半5轮，128个warp全部并行执行。第w个warp负责全局下标`32w`至`32w+31`，即元素值`32w+1`至`32w+32`，部分和为`32 × 32w + 32 × 33 / 2 = 1024w + 528`：

| warp | 覆盖元素 | 部分和 |
|---|---|---|
| 0 | 1~32 | 528 |
| 1 | 33~64 | 1552 |
| … | … | … |
| 127 | 4065~4096 | 130576 |

第一级完成后，各段的rank 0将部分和写入`ub_partial`，下标取warp号，32个子组各写一格。

两级之间以`block.sync()`衔接，同步粒度为块级。原因在于`ub_partial`的32个写者分属32个不同的warp，读者却是warp 0的32个lane，写与读跨越了warp边界，须由块级屏障保证这32格在第二级读取前全部落地。

第二级由`if (tile.meta_group_rank() == 0)`限定在warp 0执行，其余31个warp跳过。warp 0的32个lane各读取一个部分和作为初值，再规约一次即得本块总和。此时`reduce_sum`内部的`g.sync()`为warp内同步，作用域为进入该分支的这一个warp，跳过的31个warp不参与该同步。第二级的暂存区取`ub_workspace`起始的32格，即tile 0在第一级使用过的那一段；该段的部分和已转存至`ub_partial`，被覆盖不影响结果，因此无需另开一块空间。

两次调用传入同一个`tile`组对象，规约域均为32，均为5轮折半，折半过程完全相同，差异仅在三个实参的取值：

| | 输入 | 暂存区 | 执行范围 |
|---|---|---|---|
| 第一级 | `input[global_idx]` | `ub_workspace + offset` | 全部32个warp |
| 第二级 | `ub_partial[thread_rank()]` | `ub_workspace` | 仅warp 0 |

同一实现体在同一个kernel内实例化两次，函数体无需为此改动。两级合计10次warp内同步加1次块级同步，第一级的并行度为128路，代价是额外占用`ub_partial[32]`共128B的片上空间。

同一block内32个warp的部分和相加即得该块的部分和，数值与整块规约场景一致，四块经原子加后总和为8390656。

#### 4：host侧实现

`main`依次完成七步：解析参数，构造输入与期望值，经`aclInit`、`aclrtSetDevice`、`aclrtCreateStream`初始化运行环境，申请输入显存并拷入，按用例分发至两个用例函数之一取回总和，释放显存与去初始化，最后调用`verify_result`校验并以其返回值作为进程退出码。

```cpp
// case0：暂存区在全局内存
int32_t run_global_workspace_case(int32_t* input_device, int32_t num_elements, aclrtStream stream);
// case1与case2：暂存区在共享内存，由kernel形参选定发射哪一个
int32_t run_shared_workspace_case(KernelType kernel, int32_t* input_device, int32_t num_elements, aclrtStream stream);
```

两个用例函数互不调用，各自完成本场景所需显存的分配、kernel发射、流同步、总和回读与显存释放，返回该总和。结果的比对不在用例函数内进行，集中在`verify_result`处。

划分依据为暂存区所在的存储层级，该层级决定host侧是否需要额外分配显存。跨块规约的暂存区在全局内存，须在host侧另开一块`TOTAL_THREADS`个`int32`的显存并列入kernel参数表；其总和由grid内rank 0唯一写出，不经原子加，因而不需要预先清零。整块与子组两个场景的暂存区以`__ubuf__ static`声明于kernel内，host侧无对应的分配动作，两者的显存需求同为一个`int32`的总和；汇总均经`asc_atomic_add`完成，故总和在launch前由`aclrtMemset`清零。

后两个场景的host侧流程由此完全一致，差异仅在发射哪一个kernel，`run_shared_workspace_case`以形参`KernelType kernel`承接该差异：

```cpp
if (kernel == KernelType::THREAD_BLOCK) {
    reduce_thread_block<<<BLOCKS_PER_GRID, THREADS_PER_BLOCK, DYN_UBUF_SIZE, stream>>>(
        input_device, total_device, num_elements);
} else {
    reduce_block_tile<<<BLOCKS_PER_GRID, THREADS_PER_BLOCK, DYN_UBUF_SIZE, stream>>>(
        input_device, total_device, num_elements);
}
```

显存分配、清零、流同步、总和回读与释放五步在该分支之外，两个场景共用一份。单次执行仍只发射一个kernel，用例的选定由`main`中的分发完成。

期望值由元素个数在代码中算出，与解析解公式一致。`num_elements`取`TOTAL_THREADS`即`BLOCKS_PER_GRID * THREADS_PER_BLOCK`合4096，golden为8390656：

```cpp
const int32_t golden = static_cast<int32_t>(num_elements * (num_elements + 1) / 2);  // 4096 → 8390656
```

用例函数返回后，`main`按释放输入显存、销毁stream、复位device、去初始化的顺序收尾，最后返回校验结果：

```cpp
CHECK_ACL(aclrtFree(input_device));
CHECK_ACL(aclrtDestroyStream(stream));
CHECK_ACL(aclrtResetDevice(device_id));
CHECK_ACL(aclFinalize());

return verify_result(output, golden);
```

全部ACL调用经`CHECK_ACL`包裹。返回码非`ACL_SUCCESS`时，宏打印所在文件与行号、错误码、`aclGetRecentErrMsg()`取得的运行时信息与出错调用的字面文本，随即以`EXIT_FAILURE`终止进程。`aclGetRecentErrMsg()`在无错误详情时可能返回`nullptr`，直接传给`%s`是未定义行为，故做空指针兜底：

```cpp
#define CHECK_ACL(expr)                                                                       \
    do {                                                                                      \
        aclError ret = (expr);                                                                \
        if (ret != ACL_SUCCESS) {                                                             \
            const char* msg = aclGetRecentErrMsg();                                           \
            fprintf(                                                                          \
                stderr, "Ascend Error: %s:%d code=%d %s \"%s\"\n", __FILE__, __LINE__,        \
                static_cast<int>(ret), (msg != nullptr) ? msg : "no detail", #expr);          \
            exit(EXIT_FAILURE);                                                               \
        }                                                                                     \
    } while (0)
```

出错即终止，不再进入后续的launch与回读，避免在运行环境已然异常的前提下比对结果。规约结果的正确性由`verify_result`比对总和与golden得出，其返回值即进程退出码。

## 编译运行

在样例根目录下执行以下步骤完成编译与运行。

配置环境变量，其中`${install_path}`为CANN软件的安装目录，若安装时未指定则默认为`/usr/local/Ascend`：

```bash
source ${install_path}/cann/set_env.sh
```

`CMakeLists.txt`中不写入CANN路径，工具链、头文件与运行时库均由环境变量给出，`find_package(ASC)`据此定位所用版本，编译与运行两个阶段都取自这套环境变量。机器上装有多个CANN版本时，可核对以下两项确认当前生效的是哪一份：

```bash
which bisheng            # 应指向 ${install_path}/cann-9.2.0/bin/bisheng
echo $ASCEND_HOME_PATH   # 应为 ${install_path}/cann-9.2.0
```

CMake将编译器路径写入`build`目录的缓存，切换CANN版本后删除该目录重新配置即可。

编译并运行：

```bash
mkdir build && cd build;
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
./cooperative_groups 0
```

编译选项仅`CMAKE_ASC_ARCHITECTURES`一项，取值为`dav-3510`，对应Ascend 950PR/950DT的NPU架构。传给编译器的选项为`--npu-arch=${CMAKE_ASC_ARCHITECTURES} --enable-simt`。

运行时以参数指定用例号，取0、1、2之一，单次执行运行指定的那一个用例。参数缺失或非法时打印用法并以退出码1返回：

```bash
./cooperative_groups 0    # 跨块规约场景
./cooperative_groups 1    # 整块规约场景
./cooperative_groups 2    # 子组规约场景
```

以整块规约场景为例，校验通过时打印如下，退出码为0：

```text
Case 1: whole-block reduction (thread_block), reduce domain = 1024
num_elements = 4096, threads = 4096
grid = 4 blocks x 1024 threads, reduce domain = 1024 per block, workspace in shared memory
Output: 8390656
Golden: 8390656
[Success] Case accuracy verification passed.
```

## 性能分析

使用`msopprof`工具采集单算子性能数据，待测命令的参数直接跟在可执行文件后：

```bash
msopprof ./cooperative_groups 1   # 分析整块规约case的性能
msopprof ./cooperative_groups 2   # 分析子组规约case的性能
```

工具在终端直接打印`Op Name`、`Task Duration(us)`、`Block Dim`等基础信息与调优建议，完整结果落在当前目录的`OPPROF_*`目录下。各跑3次实测`Task Duration(us)`：

| 用例 | 规约域 | 同步 | Task Duration(us) |
|---|---|---|---|
| 整块规约 | 1024 | 20次块级 | 7.01 / 7.12 / 7.22 |
| 子组规约 | 32 | 10次warp内 + 1次块级 | 6.11 / 6.29 / 6.58 |

子组规约比整块规约快约10%~13%，两者三次运行的区间不重叠。

需要进一步定位瓶颈时，可通过`--aic-metrics`细分采集维度，例如`msopprof --aic-metrics=PipeUtilization,Memory ./cooperative_groups 2`。无NPU硬件时可改用仿真模式：

```bash
msopprof simulator --soc-version=<soc_version> ./cooperative_groups 2
```

两个场景的输入规模、访存量与暂存区所在层级相同，区别在同步的作用域：子组规约的第一级5轮走warp内同步，仅两级之间用一次块级屏障；整块规约的10轮全部走块级屏障。同步作用域越小，单次屏障需等齐的线程数越少。

