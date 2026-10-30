# 核内同步能力概述

## 为什么需要核内同步

AI Core内部的执行单元（如MTE2搬运单元、Vector计算单元等）以异步并行的方式运行，在读写同一存储资源时可能存在数据依赖关系。为确保数据一致性及计算正确性，需通过同步控制协调操作时序。

<!-- npu="950" id2 -->
针对[NPU架构3510](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)，硬件架构图如下，高亮部分展示了并行执行的计算单元和搬运单元。

**图1**  NPU架构3510架构图    
![](../../../../figures/npu_3510_hw_arch_sync.png)

<!-- end id2 -->

<!-- npu="A3,910b" id1 -->

针对[NPU架构2201](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)，硬件架构图如下，高亮部分展示了并行执行的计算单元和搬运单元。

**图2**  NPU架构2201架构图    
![](../../../../figures/atlas_a2_a3_architecture.png)

<!-- end id1 -->

下图示例描述了一个常见的Vector计算数据流：

1. 先通过DMA执行单元将数据从GM搬入到Local Memory；
2. 进行计算；
3. 然后再通过DMA执行单元将计算结果从Local Memory搬出到GM。

**图3**  Vector计算数据流示意图    
![](../../../../figures/vector_compute_data_flow.png)

四个执行单元Scalar、Vector、DMA（MTE2）、DMA（MTE3）并行执行，若访问同一片Local Memory，需要同步机制来控制它们的访问时序：保证先搬入Local Memory后再计算，计算完成后再搬出。

**图4**  核内并行流水执行时序示意图    
![](../../../../figures/intra_core_parallel_pipeline_execution_timing.png)

## 硬件流水类型

AI Core内部并行的指令流水类型和解释如下所示：

> [!NOTE]说明
> 不同的硬件架构，每一种硬件流水类型包含的具体流水会有所差异，详细介绍请参考[硬件实现](../../../../../guide/programming_guide/advanced_programming/hardware_implementation/hardware_implementation.md)章节。

**表1** 指令流水类型和相关说明

| 流水类型 | 含义 |
| --- | --- |
| PIPE_S | 标量流水，使用标量访存语句或标量计算接口访问GM、片上存储地址时为此流水 |
| PIPE_V | 矢量计算流水及部分硬件架构下的L0C Buffer->UB数据搬运流水 |
| PIPE_M | 矩阵计算流水 |
| PIPE_MTE1 | L1 Buffer->L0A Buffer、L1 Buffer->L0B Buffer数据搬运流水 |
| PIPE_MTE2 | GM->L1 Buffer、GM->UB等数据搬运流水 |
| PIPE_MTE3 | UB->GM等数据搬运流水 |
| PIPE_FIX | L0C Buffer->GM、L0C Buffer->L1 Buffer等数据搬运流水 |

此外，`PIPE_ALL`表示全部流水，用于需要等待所有流水前序指令完成的场景。

同步接口中`pipe_t`枚举类型的取值如下：<a id="硬件流水类型"></a>

```c
typedef enum {
    PIPE_S = 0,
    PIPE_V = 1,
    PIPE_M = 2,
    PIPE_MTE1 = 3,
    PIPE_MTE2 = 4,
    PIPE_MTE3 = 5,
    PIPE_ALL = 6,
    PIPE_FIX = 10
} pipe_t;
```

## 核内同步分类

对上述核内并行流水的同步控制可按表2选择。

**表2** 核内同步接口分类说明

| 同步类型 | 接口 | 适用场景 | 差异点 |
| --- | --- | --- | --- |
| 多流水同步 | [asc_sync_notify](asc_sync_notify.md)/[asc_sync_wait](asc_sync_wait.md) | 同一核内具有数据依赖的不同类型流水指令之间的同步。 | 需要开发者显式指定一对源流水和目的流水。`asc_sync_notify`在源流水前序指令的所有读写操作完成后设置标志位；`asc_sync_wait`阻塞目的流水，直至检测到该标志位，解除阻塞并清除标志位。 |
| 多流水同步 | [asc_lock](asc_lock.md)/[asc_unlock](asc_unlock.md) | 同一核内不同流水之间的同步控制。 | 通过`mutex_id`锁定和释放指定流水。`asc_lock`根据`mutex_id`获取Mutex，若Mutex已被锁定，将阻塞指定流水上后续指令的执行；`asc_unlock`在指定流水的前序指令执行完成后释放对应Mutex。与`asc_sync_notify`/`asc_sync_wait`相比，该组合内聚性更强、与其他流水解耦，可简化反向同步逻辑。 |
| 多流水同步 | [asc_sync_mte2](asc_sync_mte2.md)、[asc_sync_mte3](asc_sync_mte3.md)、[asc_sync_vec](asc_sync_vec.md) | 需要同时阻塞多条目的流水，直到指定源流水的前序指令完成。 | **易用性接口**，内部封装了多对`asc_sync_notify`与`asc_sync_wait`。<br>`asc_sync_mte2`等价于分别执行`PIPE_MTE2`->`PIPE_MTE3`和`PIPE_MTE2`->`PIPE_V`两组同步。<br>`asc_sync_mte3`等价于分别执行`PIPE_MTE3`->`PIPE_MTE2`和`PIPE_MTE3`->`PIPE_V`两组同步。<br>`asc_sync_vec`等价于分别执行`PIPE_V`->`PIPE_MTE2`、`PIPE_V`->`PIPE_MTE3`和`PIPE_V`->`PIPE_S`三组同步。 |
| 单流水同步 | [asc_sync_pipe](asc_sync_pipe.md) | 同一核内具有数据依赖的相同类型流水指令之间的同步。 | 同一流水中虽然指令是顺序执行，但后一条指令开始执行时前一条指令不一定执行结束。`asc_sync_pipe`保证前序指令中所有数据读写全部完成后，后序指令才开始执行。该接口不支持`PIPE_S`单流水同步。 |
| 全流水同步 | [asc_sync](asc_sync.md) | 需要阻塞调用点后所有硬件流水的后序指令，直到调用点之前所有硬件流水的前序指令全部完成。 | 功能与`asc_sync_pipe(PIPE_ALL)`等价，适用于无法只约束单条流水的场景。 |
| 内存访问屏障 | [asc_sync_data_barrier](asc_sync_data_barrier.md) | 需要等待此前已发出但尚未完成的内存访问指令全部执行完成。 | 通过`arg`参数指定屏障作用的内存范围，确保屏障前后的内存访问指令按预期顺序完成。 |

<!-- npu="A3,910b" id4 -->
以[NPU架构2201](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)为例，该硬件架构下所有合法的核内同步组合如[表3](#aic_intra_core_sync_combinations)和[表4](#aiv_intra_core_sync_combinations)所示。其中，“不涉及”表示硬件层面不存在此种同步组合，“暂无应用场景”表示存在此种同步组合，但在实际开发场景中暂不需要使用。

<a id="aic_intra_core_sync_combinations"></a>

**表3** AIC中所有合法的核内同步组合

| 源流水/目的流水 | PIPE_S | PIPE_M | PIPE_MTE1 | PIPE_MTE2 | PIPE_MTE3 | PIPE_FIX |
| --- | --- | --- | --- | --- | --- | --- |
| PIPE_S | 不涉及 | 不涉及 | 不涉及 | 不涉及 | 不涉及 | 不涉及 |
| PIPE_M | 不涉及 | `asc_sync_pipe(PIPE_M)` | `asc_sync_notify(PIPE_M, PIPE_MTE1, EVENT_ID0)`<br>`asc_sync_wait(PIPE_M, PIPE_MTE1, EVENT_ID0)` | `asc_sync_notify(PIPE_M, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_M, PIPE_MTE2, EVENT_ID0)` | 不涉及 | `asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0)`<br>`asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0)` |
| PIPE_MTE1 | 不涉及 | `asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0)` | `asc_sync_pipe(PIPE_MTE1)` | `asc_sync_notify(PIPE_MTE1, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE1, PIPE_MTE2, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE1, PIPE_MTE3, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE1, PIPE_MTE3, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE1, PIPE_FIX, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE1, PIPE_FIX, EVENT_ID0)` |
| PIPE_MTE2 | 不涉及 | `asc_sync_notify(PIPE_MTE2, PIPE_M, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_M, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0)` | `asc_sync_pipe(PIPE_MTE2)` | `asc_sync_notify(PIPE_MTE2, PIPE_MTE3, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_MTE3, EVENT_ID0)` | 暂无应用场景 |
| PIPE_MTE3 | 不涉及 | 不涉及 | `asc_sync_notify(PIPE_MTE3, PIPE_MTE1, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE3, PIPE_MTE1, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE3, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE3, PIPE_MTE2, EVENT_ID0)` | `asc_sync_pipe(PIPE_MTE3)` | 暂无应用场景 |
| PIPE_FIX | 不涉及 | `asc_sync_notify(PIPE_FIX, PIPE_M, EVENT_ID0)`<br>`asc_sync_wait(PIPE_FIX, PIPE_M, EVENT_ID0)` | `asc_sync_notify(PIPE_FIX, PIPE_MTE1, EVENT_ID0)`<br>`asc_sync_wait(PIPE_FIX, PIPE_MTE1, EVENT_ID0)` | 暂无应用场景 | 暂无应用场景 | `asc_sync_pipe(PIPE_FIX)` |

<a id="aiv_intra_core_sync_combinations"></a>

**表4** AIV中所有合法的核内同步组合

| 源流水/目的流水 | PIPE_S | PIPE_V | PIPE_MTE2 | PIPE_MTE3 |
| --- | --- | --- | --- | --- |
| PIPE_S | 不涉及 | `asc_sync_notify(PIPE_S, PIPE_V, EVENT_ID0)`<br>`asc_sync_wait(PIPE_S, PIPE_V, EVENT_ID0)` | `asc_sync_notify(PIPE_S, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_S, PIPE_MTE2, EVENT_ID0)` | `asc_sync_notify(PIPE_S, PIPE_MTE3, EVENT_ID0)`<br>`asc_sync_wait(PIPE_S, PIPE_MTE3, EVENT_ID0)` |
| PIPE_V | `asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0)`<br>`asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0)` | `asc_sync_pipe(PIPE_V)` | `asc_sync_notify(PIPE_V, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_V, PIPE_MTE2, EVENT_ID0)` | `asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0)`<br>`asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0)` |
| PIPE_MTE2 | `asc_sync_notify(PIPE_MTE2, PIPE_S, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_S, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0)` | `asc_sync_pipe(PIPE_MTE2)` | `asc_sync_notify(PIPE_MTE2, PIPE_MTE3, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_MTE3, EVENT_ID0)` |
| PIPE_MTE3 | `asc_sync_notify(PIPE_MTE3, PIPE_S, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE3, PIPE_S, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE3, PIPE_V, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE3, PIPE_V, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE3, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE3, PIPE_MTE2, EVENT_ID0)` | `asc_sync_pipe(PIPE_MTE3)` |

<!-- end id4 -->

## 什么时候需要开发者手动插入同步

C API编程方式下所有的同步均需开发者手动管理。
