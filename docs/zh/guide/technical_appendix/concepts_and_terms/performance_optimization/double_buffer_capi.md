# Double Buffer

Double Buffer是一种通过增加[Unified Buffer（UB）](../glossary.md#p17392122195118)缓冲区组数，使数据搬入、Vector计算和结果搬出能够跨数据块重叠执行的优化方法。本文以基于指针的C语言编程方式说明其工作方式和实现要点。

在矢量算子中，一个数据块通常需要经历以下三个阶段：

- 数据搬入：将输入数据从[Global Memory（GM）](../glossary.md#p1439018225112)搬入UB，由[Memory Transfer Engine 2（MTE2）](../glossary.md#p53901325511)对应的`PIPE_MTE2`流水执行。
- Vector计算：读取UB中的输入并将结果写入UB，由[`PIPE_V`](../../../../api/SIMD-API/c_api/sync/intra_core_sync_overview.md#硬件流水类型)流水执行。
- 数据搬出：将UB中的结果写回GM，由[Memory Transfer Engine 3（MTE3）](../glossary.md#p183901322511)对应的`PIPE_MTE3`流水执行。

`PIPE_MTE2`、`PIPE_V`和`PIPE_MTE3`表示算子的执行阶段。它们可以并行执行；但当多个阶段访问同一片UB时，必须通过同步保证访问顺序。

## 单缓冲区的数据依赖与流水等待

如[图1](#fig-data-vector)所示，单Buffer场景中，当前数据块的输入和输出都复用同一组UB缓冲区。当前块完成搬入、计算和搬出前，下一数据块不能覆盖这组缓冲区，因此三个阶段在不同数据块之间无法重叠。

**图1** 数据搬运与Vector计算过程<a name="fig-data-vector"></a>  
![](../../../figures/data_vec.png "数据搬运与Vector计算过程")

例如，若数据搬入、计算和搬出三个阶段耗时相近，Vector仅在计算阶段工作，其余时间需要等待输入搬入或结果搬出。单Buffer实现适合作为正确性和性能对比的基线，但不能隐藏这些等待时间。

## 双缓冲区的流水并行机制

Double Buffer为每个数据块准备两组独立的UB缓冲区：Ping和Pong。每组包含输入`x`、输入`y`和输出`z`三个互不重叠的子缓冲区。偶数数据块使用Ping，奇数数据块使用Pong。

图2展示了前三个时间点的操作。t0时，MTE2将块0的输入搬入Ping；t1时，MTE2将块1的输入搬入Pong，Vector同时读取Ping中存储的块0输入并将计算结果写入Ping；t2时，MTE2将块2的输入搬入Ping，Vector读取Pong中存储的块1输入并将计算结果写入Pong，MTE3同时从Ping搬出块0的结果。每个时间点内并行执行的操作访问不同的UB区域，因此不会发生读写冲突。

**图2** Ping/Pong Double Buffer的三个时间点<a name="fig-double-buffer-capi"></a>
![](./figures/double_buffer_capi.png "Ping/Pong Double Buffer的三个时间点")

图2中的时序可按如下方式理解：

| 时间点 | MTE2搬入 | Vector计算 | MTE3搬出 | 说明 |
| --- | --- | --- | --- | --- |
| t0 | 块0输入 -> Ping `x/y` | 无 | 无 | 流水预热。 |
| t1 | 块1输入 -> Pong `x/y` | 计算块0，读Ping `x/y`、写Ping `z` | 无 | 块0计算与块1搬入重叠。 |
| t2 | 块2输入 -> Ping `x/y` | 计算块1，读Pong `x/y`、写Pong `z` | 块0结果 <- Ping `z` | 三个阶段跨块重叠。 |

循环结束后还需要等待已提交的操作完成，这一阶段称为流水排空。首块只有搬入、末块只有搬出是正常现象，因此实际收益应以完整算子的端到端耗时衡量。

## 基于指针的C语言编程实现方法

采用基于指针的C语言编程方式时，Double Buffer不是单一开关，而是通过以下三项配合实现：

1. 在UB中分配Ping和Pong两组独立缓冲区。
2. 在循环中按数据块序号交替选择缓冲区组。
3. 使用同步原语约束同一组缓冲区的读、写和再次复用。

下面的代码省略了核间切分、对齐和尾块计算，只保留Double Buffer所在的内核位置及其关键逻辑。`BLOCK_LENGTH`表示单个数据块的最大元素个数，`current_length`表示当前块的实际元素个数。

```cpp
__global__ __vector__ void add_double_buffer(__gm__ half* x_gm, __gm__ half* y_gm, __gm__ half* z_gm,
    uint32_t total_elements)
{
    asc_init();

    // 一组包含x、y、z三个Buffer；两组分别为Ping和Pong。
    __ubuf__ half ub_buffer[BLOCK_LENGTH * 6];
    __ubuf__ half* x_ping = ub_buffer;
    __ubuf__ half* y_ping = x_ping + BLOCK_LENGTH;
    __ubuf__ half* z_ping = y_ping + BLOCK_LENGTH;
    __ubuf__ half* x_pong = z_ping + BLOCK_LENGTH;
    __ubuf__ half* y_pong = x_pong + BLOCK_LENGTH;
    __ubuf__ half* z_pong = y_pong + BLOCK_LENGTH;

    uint32_t block_count = (total_elements + BLOCK_LENGTH - 1) / BLOCK_LENGTH;
    for (uint32_t block_idx = 0; block_idx < block_count; ++block_idx) {
        uint32_t offset = block_idx * BLOCK_LENGTH;
        uint32_t current_length = (total_elements - offset) > BLOCK_LENGTH ?
            BLOCK_LENGTH : (total_elements - offset);

        // 偶数块使用Ping，奇数块使用Pong。
        uint8_t input_mutex_id = static_cast<uint8_t>(block_idx & 1);
        uint8_t output_mutex_id = static_cast<uint8_t>(input_mutex_id + 2);
        __ubuf__ half* x_local = (block_idx & 1) == 0 ? x_ping : x_pong;
        __ubuf__ half* y_local = (block_idx & 1) == 0 ? y_ping : y_pong;
        __ubuf__ half* z_local = (block_idx & 1) == 0 ? z_ping : z_pong;

        // MTE2：搬入当前块输入。输入mutex保护同组x/y的再次写入。
        asc_lock(PIPE_MTE2, input_mutex_id);
        asc_copy_gm2ub_align(x_local, x_gm + offset, current_length * sizeof(half));
        asc_copy_gm2ub_align(y_local, y_gm + offset, current_length * sizeof(half));
        asc_unlock(PIPE_MTE2, input_mutex_id);

        // Vector：等待当前x/y就绪，并等待同组z可写。
        asc_lock(PIPE_V, input_mutex_id);
        asc_lock(PIPE_V, output_mutex_id);
        asc_add(z_local, x_local, y_local, current_length);
        asc_unlock(PIPE_V, input_mutex_id);
        asc_unlock(PIPE_V, output_mutex_id);

        // MTE3：等待当前z计算完成后搬出。
        asc_lock(PIPE_MTE3, output_mutex_id);
        asc_copy_ub2gm_align(z_gm + offset, z_local, current_length * sizeof(half));
        asc_unlock(PIPE_MTE3, output_mutex_id);
    }

    // 排空最后一个或多个已提交、但尚未完成的异步操作。
    asc_sync_pipe(PIPE_ALL);
}
```

上述代码对应图2中的操作：`block_idx`为0、1、2时，分别选择Ping、Pong、Ping。循环按代码顺序提交操作，但不同执行阶段会在满足同步依赖后异步运行，因此相邻块可以形成重叠。

完整的可运行示例以及单Buffer基线实现请参见[融合向量计算Double Buffer性能样例](../../../../../../examples/02_simd_c_api/02_features/01_reg_vector_compute/00_add_double_buffer/README.md)。

## 同步和缓冲区复用

仅将UB空间扩展为两组并不能保证正确性。同步的作用是描述每个缓冲区在其生命周期中的可访问状态。以图2为例，需要满足以下关系：

| UB区域 | 生产者和消费者 | 需要保证的顺序 | 同步方式 |
| --- | --- | --- | --- |
| Ping/Pong `x/y` | MTE2写入 -> Vector读取 | 同组输入搬入完成后才能计算；下一次写同组输入前，前一次读取必须结束。 | 使用该组的输入`mutex_id`。 |
| Ping/Pong `z` | Vector写入 -> MTE3读取 | 计算完成后才能搬出结果。 | 使用该组的输出`mutex_id`。 |
| Ping/Pong `z` | MTE3读取 -> 下一次Vector写入 | 前一块结果搬出完成后，才能复用同组输出缓冲区。 | 下一次Vector获取同一输出`mutex_id`时等待。 |

t2中MTE2向Ping的`x/y`搬入块2，同时MTE3从Ping的`z`搬出块0，并不冲突：`x/y/z`是同一组内彼此独立的子缓冲区。真正不能省略的是两类复用等待：再次写入同组`x/y`前等待前一次Vector读取结束；再次写入同组`z`前等待前一次MTE3搬出结束。

本文代码使用[asc_lock](../../../../api/SIMD-API/c_api/sync/asc_lock.md)和[asc_unlock](../../../../api/SIMD-API/c_api/sync/asc_unlock.md)按Buffer生命周期管理跨阶段依赖。二者必须成对使用，且不能嵌套使用相同的`mutex_id`。`asc_lock/asc_unlock`当前仅支持Ascend 950PR/Ascend 950DT；其他产品应根据对应产品的同步接口支持情况，使用`asc_sync_notify/asc_sync_wait`等同步原语建立等价的数据依赖，不能直接照搬本节代码。

## 适用场景和限制

是否适合使用Double Buffer，取决于是否存在可重叠的跨块阶段和足够的本地存储空间，不取决于算子编程方式。不同编程方式的处理如下。

| 编程方式或场景 | 是否可使用 | 实现或判断方式 |
| --- | --- | --- |
| 基础API，使用`TPipe/TQue`管理资源 | 可以 | 基础API通过队列Buffer数量配置Double Buffer。该方式不使用本文的手动UB切分代码，可参考[Double Buffer场景](../../../operator_practice/simd_operator_impl/vector_programming/double_buffer_scenario.md)。 |
| 基于指针的C语言编程，手动申请`__ubuf__` | 可以 | 为每个待复用数据块准备Ping/Pong两组UB区域，并显式建立跨阶段同步。本文和配套样例采用此方式。 |
| 静态或动态形状编程 | 取决于资源和流水 | 编程形态不是限制条件；只要UB可容纳两组缓冲区、数据可分块，且存在可重叠的异步阶段，就可以采用。 |
| 单个数据块、数据量很小 | 通常无收益 | 没有足够的相邻块填充和排空流水，额外UB占用及同步开销可能超过收益。 |
| 无GM-UB搬运与Vector计算重叠机会 | 不适用 | 没有可隐藏的等待时间，复制Buffer不会带来流水收益。 |
| 高阶模板或封装API | 取决于接口 | 仅在对应接口明确提供Double Buffer配置时启用；不要将本文的同步代码直接嵌入其他编程模型。 |

开启Double Buffer前还应确认以下条件：

- UB容量能够容纳两组输入和输出缓冲区，以及其他运行时占用。
- 每个缓冲区的地址和每次数据搬运长度满足目标产品及搬运接口的对齐要求。
- 尾块按实际长度计算，不能按照满块长度访问GM或UB。
- 使用性能工具对比相同计算量、相同搬运量下的端到端耗时。多个阶段已经重叠后，各阶段耗时或占比不能直接相加。

Double Buffer能够减少等待时间，但不会减少计算量或GM搬运量。若搬运本身不是瓶颈、数据块数量不足，或额外UB空间导致分块变小，其性能收益可能有限，甚至不如单Buffer实现。
