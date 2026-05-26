# SIMT协作组简介

协作组（cooperative_groups）是Ascend C SIMT编程模型提供的线程协作抽象，用于把核函数中的线程组织成可显式操作的线程组。通过协作组，开发者可以在不同粒度上表达线程间的同步、数据交换、条件判断和分组关系，避免在代码中隐式依赖固定的线程编号或Warp布局。

协作组的核心思想是：将一组需要共同执行某个操作的线程表示为一个组句柄，并通过该句柄访问组内线程数量、当前线程在组内的rank、同步接口以及组内通信接口。这样，函数可以直接接收协作组对象作为参数，明确说明调用该函数的线程协作范围。

## 使用场景

协作组适用于以下场景：

- 在线程块内进行同步，并使用组内rank计算数据访问位置。
- 将线程块划分为固定大小的子组，使多个子组分别处理不同数据片段。
- 在Warp发散后获取当前实际活跃的线程集合，并仅在这些线程之间执行协作操作。
- 使用`shfl`、`any`、`all`、`ballot`等接口完成组内寄存器数据交换和谓词归约。

## 头文件与命名空间

使用协作组API时需要包含以下头文件：

```c++
#include <simt_api/cooperative_groups.h>
```

所有协作组API均位于`cooperative_groups`命名空间下，调用时可以显式添加命名空间前缀，也可以在核函数中使用`using namespace cooperative_groups;`。

```c++
using namespace cooperative_groups;
```

## 协作组句柄

协作组对象是一个组句柄。句柄描述当前线程所属的协作组，并提供查询和集体操作接口。常用信息包括：

- 当前线程在组内的rank，例如`thread_rank()`。
- 组内线程数量，例如`num_threads()`或`size()`。
- 当前线程块、线程索引和线程块维度等信息，例如`group_index()`、`thread_index()`、`dim_threads()`。
- 组内同步和数据交换接口，例如`sync()`、`shfl()`、`ballot()`。

协作组句柄通常在核函数入口处或进入某个明确的协作区域时创建，然后传递给需要使用该协作范围的函数。函数通过接收组句柄，可以明确表达“哪些线程必须共同调用该函数”。

```c++
using namespace cooperative_groups;

__global__ void simt_kernel(...)
{
    thread_block block = this_thread_block();

    unsigned int rank = block.thread_rank();
    unsigned int count = block.num_threads();

    // block内所有线程在此处同步。
    block.sync();
    ...
}
```

## 协作组类型

当前提供以下协作组类型：

- [thread_block](../../../../api/SIMT-API/协作组/thread_block/thread_block简介.md)：对当前线程块的抽象，提供线程块级别的线程管理和同步接口。
- [thread_block_tile](../../../../api/SIMT-API/协作组/thread_block_tile/thread_block_tile简介.md)：由父组划分得到的固定大小线程子组，支持组内同步、数据交换和谓词操作。
- [coalesced_group](../../../../api/SIMT-API/协作组/coalesced_group/coalesced_group简介.md)：当前Warp内处于同一执行路径的活跃线程集合，常用于处理分支发散后的线程协作。

### thread_block

`thread_block`表示当前线程所在的线程块，可通过`this_thread_block()`获取。它是进一步划分子组的常用起点。

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

`thread_block_tile`除支持`sync()`、`thread_rank()`、`num_threads()`等基础接口外，还支持组内数据交换和谓词操作，例如`shfl()`、`shfl_up()`、`shfl_down()`、`shfl_xor()`、`any()`、`all()`和`ballot()`。

> [!CAUTION]注意 
> SIMT架构不支持独立线程调度。一个Warp内的多个协作组之间应避免存在相互等待或数据依赖，否则可能出现卡死。

### coalesced_group

`coalesced_group`表示当前Warp内处于活跃状态并参与当前执行路径的线程集合，可通过`coalesced_threads()`获取。它常用于条件分支中，仅让到达当前分支的活跃线程组成协作组。

```c++
if (threadIdx.x % 2 == 0) {
    coalesced_group active = coalesced_threads();
    unsigned int rank = active.thread_rank();
    ...
}
```

当Warp发生分支发散时，硬件会分路径执行分支中的指令。`coalesced_group`用于描述当前路径上的活跃线程，因此其组大小和组内rank可能随调用位置变化。

## 组划分

协作组支持将父组划分为更小的子组。当前提供以下划分接口：

- [tiled_partition](../../../../api/SIMT-API/协作组/tiled_partition.md)：将父组划分为指定大小的子组。
- [binary_partition](../../../../api/SIMT-API/协作组/binary_partition.md)：根据布尔标签将父组划分为两个`coalesced_group`子组。

> [!CAUTION]注意 
> 对协作组进行划分是集体操作，父组中的所有线程都必须参与。如果父组是在并非所有线程都能到达的条件分支中创建的，或者只有部分父组线程调用划分接口，可能导致死锁或数据损坏。

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

模板版本的划分大小必须满足`tiled_partition`接口约束，详细要求请参考[tiled_partition](../../../../api/SIMT-API/协作组/tiled_partition.md)。

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

## 同步与集体操作

协作组中的同步和通信接口一般要求组内相关线程共同参与调用。常见接口包括：

- `sync()`：同步组内线程。
- `shfl()`、`shfl_up()`、`shfl_down()`、`shfl_xor()`：在组内线程之间交换寄存器数据。
- `any()`、`all()`：对组内线程的谓词结果进行判断。
- `ballot()`：返回组内活跃线程谓词结果对应的位图。

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

## 使用注意事项

- `thread_block`表示核函数启动配置中的线程块，是隐式存在的线程组。建议在核函数入口处、进入条件分支前调用`this_thread_block()`创建句柄，并在后续代码中复用该句柄。
- `thread_block`句柄的创建应由线程块内线程一致执行。不要只让部分线程创建并使用同一个线程块级协作组。
- 对协作组进行划分时，应确保父组中的所有线程都执行同一次划分操作。不要在只有部分父组线程可达的分支中对该父组调用`tiled_partition`或`binary_partition`。
- `coalesced_group`用于描述当前执行路径上的活跃线程集合，可以在条件分支内调用`coalesced_threads()`获取当前分支内的活跃线程组。
- 使用`thread_block_tile`或`coalesced_group`时，应关注SIMT架构不支持独立线程调度的约束，避免同一Warp内不同协作组之间形成数据依赖或等待关系。
- 协作组对象不提供默认构造函数，应在声明时完成初始化。
- 推荐将需要协作的函数设计为接收协作组参数，使调用方明确传入协作范围。

## 示例：以子组为单位处理数据

以下示例将线程块划分为大小为4的子组，每个子组内通过`shfl()`读取组内rank为0的线程提供的值。

```c++
using namespace cooperative_groups;

__global__ void simt_kernel(int *input, int *output)
{
    thread_block block = this_thread_block();
    auto tile4 = tiled_partition<4>(block);

    int value = input[block.thread_rank()];
    int base = tile4.shfl(value, 0);

    output[block.thread_rank()] = base + tile4.thread_rank();
}
```

该示例中：

- `block`描述整个线程块。
- `tile4`描述从`block`划分出的4线程子组。
- `tile4.thread_rank()`返回当前线程在4线程子组内的rank。
- `tile4.shfl(value, 0)`读取同一子组内rank为0的线程的`value`。
