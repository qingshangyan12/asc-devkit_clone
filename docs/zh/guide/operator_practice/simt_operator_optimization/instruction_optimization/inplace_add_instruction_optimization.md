# 原地加法指令优化

【优先级】中

【描述】SIMT编程模式下，若GM地址上的数据只需执行逐元素累加，可以使用`asc_atomic_add`完成原地更新，减少线程单独读取目标旧值的一条指令发射。该方法适用于各线程访问互不重叠地址的场景，原子性本身不是功能需求。

本节使用原子操作的目的与[原子操作指令优化](./atomic_instruction_optimization.md)不同。后者针对多个线程更新同一地址、必须依靠原子性保证正确性的场景，优化方向是减少高开销的原子操作；本节针对各线程更新独立地址的场景，优化方向是使用原子加的指令形式减少循环体指令数。

两节的适用条件互斥，判断依据是各线程访问的目标地址是否重叠：

| 场景 | 地址是否重叠 | 原子性是否必需 | 优化方向 |
| :--- | :---: | :---: | :--- |
| 多线程更新同一地址 | 重叠 | 是 | 减少原子操作，见[原子操作指令优化](./atomic_instruction_optimization.md) |
| 各线程更新独立地址 | 不重叠 | 否 | 引入原子操作，见本节 |

若地址存在重叠，同一地址上的原子操作只能排队执行，引入原子操作会带来显著的性能损失，本节的优化不适用。

【样例介绍】本节以[原地加法指令优化样例](../../../../../../examples/03_simt_api/03_best_practices/03_instruction_optimizations/inplace_add_atomic/README.md)为载体。样例对三个形状为`[4194304]`的`int32_t`数组执行逐元素累加，包含三操作数乘加和两操作数加法两组对照。样例启动64个线程块、每个线程块2048个线程，各线程按固定步长遍历数组，目标地址互不重叠。

<!-- npu="950" id1 -->
下文性能数据均使用msOpProf工具在Ascend 950PR产品上采集，指标为Task Duration（Task整体耗时）。
<!-- end id1 -->

## 用原子加减少目标值读取

在`a[index] += b[index] * c[index]`中，普通原地加需要读取`a[index]`、`b[index]`和`c[index]`，完成计算后写回`a[index]`；使用原子加后只计算`b[index] * c[index]`，由`asc_atomic_add`完成对`a[index]`的累加。两种实现如下：

【反例】直接执行普通原地加：

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

上述实现需要单独发射读取`a[index]`的指令，循环体包含3次读取和1次写入。

【正例】使用`asc_atomic_add`完成累加：

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

上述实现不再单独读取`a[index]`，循环体减少一条目标值读取指令，变为2次读取和1次写入。两种实现的线程块数量、线程数、遍历方式和数据规模完全一致，性能数据如下：

| 场景 | 累加方式 | Task Duration（μs） | 耗时相对基线 |
| :---: | :--- | :---: | :---: |
| 0 | `a[index] += b[index] * c[index]` | 51.62 | 1× |
| 1 | `asc_atomic_add` | 47.51 | **0.92×** |

使用原子加后耗时下降8.0%。

对于两操作数加法，普通实现`a[index] += b[index]`需要读取`a[index]`和`b[index]`，而`asc_atomic_add(&a[index], b[index])`只需读取`b[index]`并在原子路径中完成目标值更新：

| 场景 | 累加方式 | Task Duration（μs） | 耗时相对基线 |
| :---: | :--- | :---: | :---: |
| 2 | `a[index] += b[index]` | 39.18 | 1× |
| 3 | `asc_atomic_add` | 36.52 | **0.93×** |

使用原子加后耗时下降6.8%。四个场景的实现均位于上述样例中，通过编译期选项`SCENARIO_NUM=0/1/2/3`选择。

## 性能分析

逐元素累加时，普通原地加需要将目标旧值读入寄存器，完成加法后再写回；原子加则将目标旧值的读取、计算和写回作为一个整体处理，减少目标值在寄存器与存储侧之间的一次往返。因此，优化收益来自循环体少发射一条目标值读取指令，而不是原子性本身或数据规模减少。

读取指令减少并不会带来等比例的耗时下降。每个Vector Core同时驻留多个Warp，一个Warp等待GM数据返回时，硬件调度器可以切换到其他就绪Warp发射指令，以隐藏访存延迟；因此被隐藏的等待时间不会随读取指令数等量减少。

## 适用条件

使用该优化前需逐项确认以下条件，任一条不满足则不应使用：

1. **各线程访问的目标地址互不重叠。** 若存在重叠，同一地址上的原子操作只能排队执行，性能将显著下降。
2. **不需要目标地址的旧值。** 旧值不参与后续计算，且不接收原子加的返回值。若后续逻辑需要旧值，或需要使用返回值，应采用普通读改写或参考[非必要不使用原子操作的返回值](./atomic_instruction_optimization.md)。
3. **累加操作可由原子接口表达。** 目标数据类型与运算需在[原子操作接口](../../../../api/SIMT-API/atomic_operations/atomic_operations_intro.md)的支持范围内。

【总结】当GM上的数据只需逐元素累加、旧值不参与后续计算、不使用原子加返回值，且各线程访问的地址互不重叠时，可使用原子加减少一条目标值读取指令的发射。判断顺序应为：先确认地址是否重叠；重叠时参考原子操作指令优化一节设法减少原子操作，不重叠时再考虑本节的引入原子加优化。
