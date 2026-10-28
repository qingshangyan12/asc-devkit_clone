# 系统同步能力概述

在Ascend C编程中，同步用于约束AI Core内外不同执行单元的执行先后关系，确保后续指令在依赖的数据、状态或同步信号就绪后再继续执行。由于数据搬运、计算、不同核函数（Kernel）或不同AI Core之间可以并行，如果缺少必要的同步，后续执行单元可能读取到尚未写入完成的数据，或在前序任务未完成时提前执行，导致计算结果错误或执行行为不符合预期。

AI Core的同步，总共分成2类：[核内同步](intra_core_sync/intra_core_sync_overview.md)、[核间同步](inter_core_sync/inter_core_sync_overview.md)。

以下内容展示两类同步的使用场景。

- 核内同步用于约束同一核内不同硬件流水或同一硬件流水内指令的执行时序。

    例如在AIV中，MTE2单元通过`PIPE_MTE2`将数据搬入片上存储后，Vector单元才可以通过`PIPE_V`读取该数据进行计算，此时需要插入`PIPE_MTE2`到`PIPE_V`的同步，使`PIPE_V`等待`PIPE_MTE2`的前序搬入完成。
- 核间同步用于约束不同核之间的执行时序，参与同步的对象可以是多个AI Core，也可以是同一AI Core内的AIC和AIV。
    <!-- npu="950" id1 -->
    如图1所示，针对[NPU架构3510](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)，同一个AI Core内AIV通过`PIPE_MTE3`将数据从UB搬运到L1 Buffer后，AIC侧`PIPE_MTE1`才能将L1 Buffer中的数据搬入L0A Buffer和L0B Buffer并继续执行矩阵计算，此时需要让AIC等待AIV的数据搬运完成通知。

    核内同步和核间同步的区别在于：核内同步解决同一核内部硬件流水之间的数据依赖，关注`PIPE_MTE2`、`PIPE_V`等流水的先后关系；核间同步解决不同核或不同逻辑核之间的进度依赖，关注AIC、AIV或多个AI Core之间的先后关系。

    **图1**  [NPU架构3510](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)架构图
    ![](../../../figures/npu_3510_hw_arch_sync.png)
    <!-- end id1 -->

各类同步对应的接口如下。

**表1** 核内同步接口

| 接口名称 | 功能简述 |
| --- | --- |
| [asc_sync_notify](intra_core_sync/asc_sync_notify.md)/[asc_sync_wait](intra_core_sync/asc_sync_wait.md) | 多流水同步：同一核内不同流水之间的同步指令。`asc_sync_notify`负责发送通知，`asc_sync_wait`负责阻塞直到`asc_sync_notify`发送通知。 |
| [asc_sync_pipe](intra_core_sync/asc_sync_pipe.md) | 单流水同步：同一核内相同流水之间的同步指令（不支持`PIPE_S`单流水同步）。 |
| [asc_sync_data_barrier](intra_core_sync/asc_sync_data_barrier.md) | 阻塞后续的指令执行，直到所有之前的内存访问指令（需要等待的内存位置可通过参数控制）执行结束。 |
| [asc_lock](intra_core_sync/asc_lock.md)/[asc_unlock](intra_core_sync/asc_unlock.md) | 多流水同步：用于核内异步流水指令之间的同步处理，其功能类似于传统CPU中的锁机制。通过锁定指定流水再释放流水来完成流水间的同步依赖。 |
| [asc_sync_vec](intra_core_sync/asc_sync_vec.md) | 核内同步易用性接口：针对`PIPE_V`执行同步操作，保证`PIPE_V`中前序指令全部完成后，其他流水的后续指令才能开始执行。只能在AIV中调用。 |
| [asc_sync_mte2](intra_core_sync/asc_sync_mte2.md) | 核内同步易用性接口：针对`PIPE_MTE2`执行同步操作，保证`PIPE_MTE2`中前序指令全部完成后，其他流水的后续指令才能开始执行。只能在AIV中调用。 |
| [asc_sync_mte3](intra_core_sync/asc_sync_mte3.md) | 核内同步易用性接口：针对`PIPE_MTE3`执行同步操作，保证`PIPE_MTE3`中前序指令全部完成后，其他流水的后续指令才能开始执行。只能在AIV中调用。 |
| [asc_sync](intra_core_sync/asc_sync.md) | 全部流水同步：同一核内所有流水之间的同步指令，功能与`asc_sync_pipe(PIPE_ALL)`等价。阻塞调用点后所有硬件流水的后序指令，直到调用点之前所有硬件流水的前序指令全部完成。 |

图2展示了四种同步控制模式，各模式的功能描述如下。该图基于核函数（Kernel）使用`__mix__(1, 2)`修饰、逻辑核数`numBlocks=4`的场景配置。

- 模式0：AI Core核间的同步控制。对于AIC全核场景，同步所有的AIC核，直到所有的AIC核都执行到`asc_sync_inter_arrive`时，`asc_sync_inter_wait`后续的指令才会执行；对于AIV全核场景，同步所有的AIV核，直到所有的AIV核都执行到`asc_sync_inter_arrive`时，`asc_sync_inter_wait`后续的指令才会执行。
- 模式1：AI Core内部，AIV核之间的同步控制。如果两个AIV核都运行了`asc_sync_subblock_arrive`，`asc_sync_subblock_wait`后续的指令才会执行。
- 模式2：AI Core内部，AIC与AIV之间的同步控制。在AIC核执行`asc_sync_block_arrive`之后，两个AIV上`asc_sync_block_wait`后续的指令才会继续执行；两个AIV都执行`asc_sync_block_arrive`后，AIC上`asc_sync_block_wait`后续的指令才能执行。
- 模式4：AI Core内部，AIC与单个AIV之间的同步控制。在单个AIV核执行`asc_sync_intra_arrive`之后，AIC上`asc_sync_intra_wait`后续的指令才会继续执行；AIC执行`asc_sync_intra_arrive`后，单个AIV上`asc_sync_intra_wait`后续的指令才能执行。AIV0与AIV1可单独触发AIC等待。

**图2**  四种核间同步控制模式示意图<a id="fig_sync_control_mode"></a>
![](../../../figures/3510_sync_control_mode_diagram.png)

**表2** 核间同步接口

| 接口名称 | 同步范围 | 对应模式 | 功能简述 |
| --- | --- | --- | --- |
| [asc_sync_inter_arrive](inter_core_sync/asc_sync_inter_arrive.md)/[asc_sync_inter_wait](inter_core_sync/asc_sync_inter_wait.md) | 组间同步，即不同group之间所有block（所有AIC或所有AIV）之间的同步。 | 模式0（同步等待全部AIC或全部AIV执行结束） | `asc_sync_inter_arrive`为生产者，负责发送通知；`asc_sync_inter_wait`为消费者，负责阻塞等待通知到达。 |
| [asc_sync_subblock_arrive](inter_core_sync/asc_sync_subblock_arrive.md)/[asc_sync_subblock_wait](inter_core_sync/asc_sync_subblock_wait.md) | 组内subblock间的同步，即同一group内不同subblock（AIV）之间的同步。 | 模式1（单个AI Core内，全部AIV之间的同步） | `asc_sync_subblock_arrive`为生产者，负责发送通知；`asc_sync_subblock_wait`为消费者，负责阻塞等待通知到达。 |
| [asc_sync_block_arrive](inter_core_sync/asc_sync_block_arrive.md)/[asc_sync_block_wait](inter_core_sync/asc_sync_block_wait.md) | 同一group内block（AIC）与所有subblock（AIV）之间的同步。 | 模式2（单个AI Core内，AIC与全部AIV之间的同步） | `asc_sync_block_arrive`为生产者，负责发送通知；`asc_sync_block_wait`为消费者，负责阻塞等待通知到达。 |
| [asc_sync_intra_arrive](inter_core_sync/asc_sync_intra_arrive.md)/[asc_sync_intra_wait](inter_core_sync/asc_sync_intra_wait.md) | 同一group内block（AIC）与单个subblock（AIV）之间的同步。 | 模式4（单个AI Core内，AIC核与单个AIV之间同步） | `asc_sync_intra_arrive`为生产者，负责发送通知；`asc_sync_intra_wait`为消费者，负责阻塞等待通知到达。 |
