# SIMT协作组简介

协作组（cooperative_groups）是Ascend C SIMT编程模型的扩展，用于组织协作线程组。协作组使开发者能够控制线程协作的粒度，实现更丰富、更高效的并行处理逻辑。

Ascend C SIMT编程模型中仅提供`asc_syncthreads()`接口来实现线程块内所有线程的同步。当开发者希望以更细粒度的方式同步线程，从而实现更高的性能和更灵活的设计时，往往需要开发者自行通过内存栅栏接口实现相应的同步逻辑。这些代码编写成本高、调优困难，并且难以及时适配芯片版本的迭代。协作组提供安全可靠的机制，协助开发者实现灵活、高效的线程协作。

## 协作组编程模型

协作组编程模型描述了SIMT线程的同步与协作模式。它提供了自定义线程组的方法以及操作线程组的接口，为实现多种协同并行模式提供了支持。

协作组中最重要的概念是使用一个对象表示一组线程，然后通过对象方法控制组内线程。对象可以明确表达程序员的意图，将协作组对象传递到某个函数可以清晰地表达“哪些线程必须共同调用该函数”。

全量的协作组API列表可在[SIMT协作组API](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMT-API/协作组/协作组.md)中获取。使用协作组API时需要包含以下头文件，并使用协作组命名空间。

```c++
#include <simt_api/cooperative_groups.h>

using namespace cooperative_groups;
// 更推荐使用别名，避免污染全局命名空间
namespace cg = cooperative_groups;
```

## 协作组类型

协作组分为隐式组和显式组。隐式组与核函数中的代码无关，它代表的是核函数的启动配置（创建了多少个线程块，每个线程块中有多少个线程来执行核函数）。隐式组为划分出更细粒度的协作组提供了起点。显式组是在核函数中由开发者手动划分出的协作组。

当前提供以下协作组类型：

- 隐式组：
    - [thread_block](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMT-API/协作组/thread_block/thread_block简介.md)：对线程块的抽象，提供线程块级别的线程管理和同步接口。
- 显式组：
    - [thread_block_tile](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMT-API/协作组/thread_block_tile/thread_block_tile简介.md)：由父组划分得到的固定大小线程子组，支持组内同步、shfl类和vote类线程操作。
    - [coalesced_group](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMT-API/协作组/coalesced_group/coalesced_group简介.md)：当前Warp内处于同一执行路径的活跃线程集合，常用于处理分支发散后的线程协作。

**图 1** 线程块中的协作组层次结构图

![线程块中的协作组层次结构](../../../../figures/cooperative_groups.png)

### thread_block

`thread_block`表示当前线程所在的线程块，通过`this_thread_block()`获取。它是进一步划分子协作组的常用起点。

```c++
thread_block block = this_thread_block();
```

`thread_block`提供线程块级别的信息查询和同步能力，例如：

- `thread_rank()`：获取当前线程在线程块内的线性rank。
- `thread_index()`：获取当前线程在线程块内的三维索引。
- `group_index()`：获取当前线程块在Grid中的三维索引。
- `sync()`：同步线程块内线程。

### thread_block_tile

`thread_block_tile`表示由父组划分得到的固定大小线程子组。子组大小由模板参数指定，适合在编译期确定每个协作单元大小的场景。

```c++
thread_block block = this_thread_block();
auto tile4 = tiled_partition<4>(block);
```

`thread_block_tile`除支持`sync()`、`thread_rank()`、`num_threads()`等基础接口外，还支持shfl类和vote类接口，例如`shfl()`、`shfl_up()`、`shfl_down()`、`shfl_xor()`、`any()`、`all()`和`ballot()`。

> [!CAUTION]注意 
> SIMT架构不支持独立线程调度。一个Warp内的多个协作组之间应避免存在相互等待或数据依赖，否则可能出现卡死。

### coalesced_group

在SIMT架构的硬件层面上，处理器以32个线程为一组（一个Warp）来执行线程。当线程遇到条件分支时会出现发散，Warp会串行地执行每个分支，在执行某个分支时会屏蔽不在该指令路径上的线程。在路径上保持活跃的线程被称为合并线程（coalesced）。`coalesced_group`表示当前Warp内处于活跃状态并参与当前执行路径的线程集合，通过`coalesced_threads()`接口创建。它常用于条件分支中，仅让到达当前分支的活跃线程组成协作组。

```c++
if (threadIdx.x % 2 == 0) {
    coalesced_group active = coalesced_threads();
    unsigned int rank = active.thread_rank();
    ...
}
```

通过`coalesced_threads()`创建的`coalesced_group`对象表示的是在当前时刻处于活跃状态的线程集合。其组内包含哪些线程由`coalesced_threads()`接口调用的位置决定，并且不随后续代码执行变化，也不保证这些线程在整个执行过程中持续保持活跃。

## 组划分

协作组支持将父组划分为更小的子组。当前提供以下划分接口：

- [tiled_partition](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMT-API/协作组/tiled_partition.md)：将父组划分为指定大小的子组。
- [binary_partition](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMT-API/协作组/binary_partition.md)：根据布尔标签将父组划分为两个`coalesced_group`子组。

### tiled_partition

`tiled_partition`用于按固定大小划分父组。模板版本在编译期指定子组大小，并返回`thread_block_tile`对象。

```c++
using namespace cooperative_groups;

__global__ void simt_kernel(...)
{
    thread_block block = this_thread_block();
    auto tile8 = tiled_partition<8>(block);

    unsigned int lane = tile8.thread_rank();
    ...
}
```

模板版本的划分大小必须满足`tiled_partition`接口约束，详细要求请参考[tiled_partition](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMT-API/协作组/tiled_partition.md)。

### binary_partition

`binary_partition`用于按照布尔条件将`coalesced_group`或`thread_block_tile`划分为两个子组。条件值相同的线程会进入同一子组。

```c++
using namespace cooperative_groups;

__global__ void simt_kernel(int *inputArr)
{
    thread_block block = this_thread_block();
    auto tile32 = tiled_partition<32>(block);

    int elem = inputArr[block.thread_rank()];
    coalesced_group subgroup = binary_partition(tile32, (elem & 1));

    unsigned int rank = subgroup.thread_rank();
    ...
}
```

## 注意事项

### 协作组对象的创建和传递

创建协作组对象是一项集体操作，所有组内的线程都必须参与。如果在并非所有线程都可到达的条件分支中创建，可能导致死锁或数据损坏。因此建议尽早创建协作组对象，并在整个核函数中使用该对象。

协作组对象没有默认构造函数，必须在声明时进行初始化，因此不建议对协作组对象进行拷贝构造。当将协作组对象传入函数时，建议通过引用方式传递。

### 协作组接口的使用

协作组中的同步和通信接口一般要求组内相关线程共同参与调用。常见接口包括：

- `sync()`：同步组内线程。
- `shfl()`、`shfl_up()`、`shfl_down()`、`shfl_xor()`：在组内线程之间交换寄存器数据。
- `any()`、`all()`：对组内线程的输入结果进行判断。
- `ballot()`：返回组内活跃线程输入结果对应的bitmap。

调用这些接口时，需要保证参与操作的线程集合与组句柄描述的线程集合一致。若只有部分组内线程到达同步点，或者不同线程以不一致的控制流调用同一个集体操作，可能导致结果不正确或程序卡死。

## 算子开发中的使用方法

在算子开发中使用协作组时，建议先根据计算模式确定“一个协作单元处理多少数据”，再选择对应的协作组类型和线程映射方式。

### 规划线程粒度

常见线程规划方式如下：

- 每个线程处理一个或多个独立元素：适用于逐元素计算、简单变换、按元素判断等场景。通常使用`thread_block`的`thread_rank()`计算全局线性下标。
- 每个子组处理一个数据片段：适用于一个输出元素需要多个线程共同完成的场景，例如小规模归约、窗口计算、Gather后处理等。通常使用`tiled_partition`将`thread_block`划分为`thread_block_tile`，再由子组内线程协作完成一个片段。
- 当前活跃线程共同处理分支内数据：适用于分支发散后仍需要在当前路径上做协作的场景。通常在分支内使用`coalesced_threads()`获取`coalesced_group`。

选择线程块大小时，应让线程块内线程数覆盖一个或多个完整的协作单元。例如每8个线程协作处理一个输出元素时，可以让一个`thread_block`包含多个8线程子组，使每个子组独立处理一个输出元素或一个数据片段。

### 切分数据

使用`thread_block`处理线性数据时，可按线程块和线程rank计算全局下标：

```c++
using namespace cooperative_groups;

__global__ void simt_kernel(int *input, int *output, unsigned int total)
{
    thread_block block = this_thread_block();
    unsigned int idx = block.group_index().x * block.num_threads() + block.thread_rank();

    if (idx < total) {
        output[idx] = input[idx] + 1;
    }
}
```

当一个数据片段需要多个线程协作时，可先划分子组，再用`meta_group_rank()`确定当前子组处理的数据片段，用`thread_rank()`确定当前线程在片段内处理的位置：

```c++
using namespace cooperative_groups;

__global__ void simt_kernel(int *input, int *output, unsigned int total)
{
    constexpr unsigned int TILE_SIZE = 8;
    thread_block block = this_thread_block();
    auto tile = tiled_partition<TILE_SIZE>(block);

    unsigned int tiles_per_block = block.num_threads() / TILE_SIZE;
    unsigned int tile_id = block.group_index().x * tiles_per_block + tile.meta_group_rank();
    unsigned int elem = tile_id * TILE_SIZE + tile.thread_rank();

    if (elem < total) {
        output[elem] = input[elem];
    }
}
```

上述映射中，`block.group_index()`用于定位线程块负责的数据范围，`tile.meta_group_rank()`用于定位线程块内的子组，`tile.thread_rank()`用于定位子组内线程负责的数据位置。对于长度不能整除线程数或子组大小的输入，需要在访问Global Memory或Unified Buffer前使用边界判断，避免越界读写。

### 选择协作组类型

- 只需要线程块级同步或线性下标时，优先使用`thread_block`。
- 每个输出由固定数量线程共同完成时，优先使用模板形式的`thread_block_tile`，便于编译期确定子组大小。
- 子组大小需要运行时确定，或需要处理不能整除的尾部数据时，可使用非模板形式的`tiled_partition`，并通过返回组的`size()`或`num_threads()`判断当前组内实际线程数。
- 分支内只对当前活跃线程做`shfl`、`any`、`all`、`ballot`等操作时，使用`coalesced_group`。

如果子组内线程需要先写入共享数据，再由其他线程读取，应在读写之间使用对应组的`sync()`。如果数据交换只发生在组内寄存器之间，优先使用`shfl`类接口，减少通过内存中转的数据访问。

## 协作组简单示例

以一个简单的归约算子为例，展示协作组的使用方式。

```c++
using namespace cooperative_groups;
__aicore__ inline int reduce_sum(thread_group& g, int* workspace, int val)
{
    uint32_t rank = g.thread_rank();

    for(uint32_t i = g.size() / 2; i > 0; i /= 2) {
        workspace[rank] = val;

        g.sync();

        if (rank < i) {
            val += workspace[rank + i];
        }

        g.sync();
    }

    // ...
}

__global__ void sum_kernel(...)
{
    __ubuf__ int workspace[1024];

    thread_block block = this_thread_block();
    unsigned int idx = block.group_index().x * block.num_threads() + block.thread_rank();
    int val = input[idx];
    output = reduce_sum(block, workspace, val);

    // ...
}
```
