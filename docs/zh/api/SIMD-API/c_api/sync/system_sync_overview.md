# 系统同步能力概述<a name="ZH-CN_TOPIC_0000002586323695"></a>

在编程中，同步是指协调多个执行单元（如线程、进程等）对共享资源的访问顺序和时机来确保程序的正确性。如果没有同步来严格控制执行顺序，则会出现结果不一致、行为不可预测等多种问题。

AI Core的同步，总共分成2类：[核内同步](./intra_core_sync_overview.md)、核间同步。各类同步对应的接口如下。

**表1**  核内同步接口

| 接口名称 | 功能简述 |
| --- | --- |
| [asc_sync_notify](asc_sync_notify.md)/[asc_sync_wait](asc_sync_wait.md) | 多流水同步：同一核内不同流水之间的同步指令。`asc_sync_notify`负责发送通知，`asc_sync_wait`负责阻塞直到`asc_sync_notify`发送通知。 |
| [asc_sync_pipe](asc_sync_pipe.md) | 单流水同步：同一核内相同流水之间的同步指令（不支持PIPE_S单流水同步）。 |
| [asc_sync_data_barrier](asc_sync_data_barrier.md) | 阻塞后续的指令执行，直到所有之前的内存访问指令（需要等待的内存位置可通过参数控制）执行结束。 |
| [asc_lock](asc_lock.md)/[asc_unlock](asc_unlock.md) | 用于核内异步流水指令之间的同步处理，其功能类似于传统CPU中的锁机制。通过锁定指定流水再释放流水来完成流水间的同步依赖。 |

**表2**  核间同步接口

| 接口名称 | 同步范围 | 对应模式 | 功能简述 |
| --- | --- | --- | --- |
| [asc_sync_block_arrive](asc_sync_block_arrive.md)/[asc_sync_block_wait](asc_sync_block_wait.md) | 组内block和subblock间的同步，即同一group内block（AIC）与subblock（AIV）之间的同步。 | 模式2（单个AI Core内，AIC与全部AIV之间的同步） | `asc_sync_block_arrive`为生产者，负责发送通知；`asc_sync_block_wait`为消费者，负责阻塞等待通知到达。 |
| [asc_sync_inter_arrive](asc_sync_inter_arrive.md)/[asc_sync_inter_wait](asc_sync_inter_wait.md) | 组间同步，即不同group之间所有block（所有AIC或所有AIV）之间的同步。 | 模式0（同步等待全部AIC或全部AIV执行结束） | `asc_sync_inter_arrive`为生产者，负责发送通知；`asc_sync_inter_wait`为消费者，负责阻塞等待通知到达。 |
| [asc_sync_intra_arrive](asc_sync_intra_arrive.md)/[asc_sync_intra_wait](asc_sync_intra_wait.md) | block内同步，即同一block内block（AIC）与单个subblock（AIV）之间的同步。 | 模式4（单个AI Core内，AIC核与单个AIV之间同步） | `asc_sync_intra_arrive`为生产者，负责发送通知；`asc_sync_intra_wait`为消费者，负责阻塞等待通知到达。 |
| [asc_sync_subblock_arrive](asc_sync_subblock_arrive.md)/[asc_sync_subblock_wait](asc_sync_subblock_wait.md) | 组内subblock间的同步，即同一group内不同subblock（AIV）之间的同步。 | 模式1（单个AI Core内，全部AIV之间的同步） | `asc_sync_subblock_arrive`为生产者，负责发送通知；`asc_sync_subblock_wait`为消费者，负责阻塞等待通知到达。 |
