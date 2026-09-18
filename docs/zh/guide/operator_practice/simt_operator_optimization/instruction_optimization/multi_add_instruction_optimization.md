# 乘加指令优化

【优先级】中

【描述】SIMT编程模式下，若某个GM地址上的数据执行乘加操作，可以使用[原子操作接口](../../../../api/SIMT-API/atomic_operations/atomic_operations_intro.md)减少一条目标值读取指令的发射。即利用`asc_atomic_add`，避免线程单独发射读取`a[i]`的指令。

需要特别说明的是，本节使用原子操作的目的与[原子操作指令优化](./atomic_instruction_optimization.md)一节不同。后者针对的是“多线程更新同一地址、原子性为功能必需”的场景，优化方向是设法减少高开销的原子操作；本节针对的是“各线程更新互不重叠的地址、不存在同地址竞争”的场景，原子性并非功能需求，而是利用原子加的指令形式减少循环体中的一条读取指令。

两节的适用条件互斥，判断依据是各线程访问的目标地址是否重叠：

| 场景 | 地址是否重叠 | 原子性是否必需 | 优化方向 |
| :--- | :---: | :---: | :--- |
| 多线程更新同一地址 | 重叠 | 是 | 减少原子操作，见[原子操作指令优化](./atomic_instruction_optimization.md) |
| 各线程更新独立地址 | 不重叠 | 否 | 引入原子操作，见本节 |

若地址存在重叠，同一地址上的原子操作只能排队依次执行，引入原子操作将带来显著的性能损失，本节的优化不适用。

【样例介绍】本节以[原地加法指令优化样例](../../../../../../examples/03_simt_api/03_best_practices/03_instruction_optimizations/inplace_add_atomic/README.md)为载体说明该优化方法。样例对三个形状为[4194304]的int32_t数组执行向量乘加`a[i] += b[i] * c[i]`，启动64个线程块、每个线程块2048个线程。样例中各线程按固定步长的方式遍历，访问的地址互不重叠，符合本节的适用条件。

<!-- npu="950" id1 -->
下文性能数据均使用msOpProf工具在Ascend 950PR产品上采集。
<!-- end id1 -->

## 用原子加进行指令优化

在`a[i] += b[i] * c[i]`计算中，线程需要读取`a[i]`，`b[i]`和`c[i]`，完成乘加后写回`a[i]`。

使用原子加后，线程只计算`b[i] * c[i]`并发射`asc_atomic_add`对应的原子加指令，不再单独发射读取`a[i]`的指令。以`a[i] += b[i] * c[i]`为例，两种实现的循环体读写指令如下：

| 实现方式 | 循环体读写指令 | 说明 |
| :--- | :---: | :--- |
| a[i] += b[i] * c[i] | 3读、1写 | 需要发射读取a[i]的指令 |
| asc_atomic_add | 2读、1写 | 不单独发射读取a[i]的指令 |

【反例】直接执行`a[i] += b[i] * c[i]`。

```cpp
__global__ __launch_bounds__(THREADS_PER_BLOCK) void vector_muladd_plain(
    int32_t* a, const int32_t* b, const int32_t* c, uint32_t element_count)
{
    uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;
    const uint32_t stride = gridDim.x * blockDim.x;
    for (; index < element_count; index += stride) {
        a[index] += b[index] * c[index];
    }
}
```

上述实现中，`a[index] += ...`需要读出`a[index]`的当前值、与`b[index] * c[index]`的乘积相加、再写回`a[index]`。循环体包含`a`、`b`、`c`三次读取和一次写入。

【正例】用`asc_atomic_add`完成累加，减少一条目标值读取指令的发射。

```cpp
__global__ __launch_bounds__(THREADS_PER_BLOCK) void vector_muladd_atomic(
    int32_t* a, const int32_t* b, const int32_t* c, uint32_t element_count)
{
    uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;
    const uint32_t stride = gridDim.x * blockDim.x;
    for (; index < element_count; index += stride) {
        asc_atomic_add(&a[index], b[index] * c[index]);
    }
}
```

上述实现中，只计算`b[index] * c[index]`，并通过`asc_atomic_add`完成对`a[index]`的累加，不再单独读取`a[index]`。因此循环体少发射一条目标值读取指令。

两种实现的线程块数量、线程数、遍历方式与数据规模完全一致，仅循环体的累加写法不同，性能数据如下：

| 实现方式 | Task Duration（μs） |
| :--- | :---: |
| 核内读改写 | 51.62 |
| 原子加 | 47.51 |

Task Duration从51.62μs降至47.51μs，降幅8.0%。

## 说明

上述数据中，循环体读取指令减少一条，但耗时仅下降8.0%，原因在于读GM的耗时大头是数据往返延迟，而不是指令发射本身。

**多线程对访存延迟的掩盖。** 每个Vector Core配置2048个线程，即64个Warp（每个Warp含32个线程）。当一个Warp等待GM数据返回时，硬件调度器会切换到其他就绪的Warp发射指令，以隐藏访存延迟。因此，少发一条读取指令主要减少发射开销；被隐藏的访存等待时间并未等量减少，耗时不会与读取指令数的减少成比例下降。

## 适用条件

使用该优化前需逐项确认以下条件，任一条不满足则不应使用：

1. **各线程访问的目标地址互不重叠。** 若存在重叠，同一地址上的原子操作只能排队依次执行，性能将显著下降。
2. **不需要目标地址的旧值。** 旧值不参与后续计算，且不接收原子加的返回值。若后续逻辑需要旧值，或接收原子加返回值，则不能使用本节优化方法，相关说明详见[非必要不使用原子操作的返回值](./atomic_instruction_optimization.md)。
3. **累加操作可由原子接口表达。** 目标数据类型与运算需在[原子操作接口](../../../../api/SIMT-API/atomic_operations/atomic_operations_intro.md)的支持范围内。

【总结】当GM上的数据只需累加、旧值不参与后续计算、不使用原子加返回值，且各线程访问的地址互不重叠时，可使用原子加减少一条读取目标地址的指令发射。该优化的关键是循环体指令条数减少。

需要强调的是，本节与[原子操作指令优化](./atomic_instruction_optimization.md)的适用条件互斥。判断顺序应为：先确认各线程访问的地址是否重叠——重叠则参考原子操作指令优化一节设法减少原子操作，不重叠才考虑本节引入原子操作。
