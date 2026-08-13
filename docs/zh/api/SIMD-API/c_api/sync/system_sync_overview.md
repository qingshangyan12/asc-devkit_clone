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

如图1所示，支持的四种同步控制模式的功能描述如下：

- 模式0：AI Core核间的同步控制。对于AIC全核场景，同步所有的AIC核，直到所有的AIC核都执行到CrossCoreSetFlag时，CrossCoreWaitFlag后续的指令才会执行；对于AIV全核场景，同步所有的AIV核，直到所有的AIV核都执行到CrossCoreSetFlag时，CrossCoreWaitFlag后续的指令才会执行。
- 模式1：AI Core内部，AIV核之间的同步控制。如果两个AIV核都运行了CrossCoreSetFlag，CrossCoreWaitFlag后续的指令才会执行。
- 模式2：AI Core内部，AIC与AIV之间的同步控制。在AIC核执行CrossCoreSetFlag之后，两个AIV上CrossCoreWaitFlag后续的指令才会继续执行；两个AIV都执行CrossCoreSetFlag后，AIC上CrossCoreWaitFlag后续的指令才能执行。
- 模式4：AI Core内部，AIC与单个AIV之间的同步控制。AIV0与AIV1可单独触发AIC等待。

**图1**  同步控制模式示意图<a name="fig37581010773"></a>  
![](../../../figures/3510_sync_control_mode_diagram.png "同步控制模式示意图")

AIC/AIV多核结构如下图2所示，AIC/AIV按group划分。一个group内细分block及subblock，block与subblock比例为1:N（N>=1），block表示多少个“主核”，subblock表示一个“主核”带多少个“从核”。

算子按计算特征可分为三类：Cube算子（矩阵计算）、Vector算子（矢量计算）和Mix算子（同时包含矩阵和矢量计算）。如表1所示，算子类型决定了其所需的核间同步方式和可选的group配置模式。其中Mix算子通过函数修饰符指定AIC与AIV的block/subblock关系；Cube算子和Vector算子为单类核执行，不涉及block/subblock划分。AIV为block且AIC为subblock的配置（即Vector核为主、Cube核为辅的模式）当前不支持。表2总结了核间同步接口及其支持的同步场景。

**图2**  block和subblock之间关系（灰色部分表示一个group，即1个block和N个subblock）<a name="zh-cn_topic_0000002542605351_fig52727114018"></a>
![](../../../../figures/block_subblock_relationship_3510.png "（NPU架构版本3510）block和subblock之间关系（灰色部分表示一个group，即1个block和N个subblock）")

**表1**  group配置<a name="zh-cn_topic_0000002542605351_table17238184964217"></a>

| 算子类型 | 函数修饰符 | Kernel类型 | block/subblock | 1 ：N |
| --- | --- | --- | --- | --- |
| Cube算子 | \_\_cube\_\_ | KERNEL_TYPE_AIC_ONLY | AIC为block，无subblock | 不涉及 |
| Vector算子 | \_\_vector\_\_ | KERNEL_TYPE_AIV_ONLY | AIV为block，无subblock | 不涉及 |
| Mix算子 | \_\_mix\_\_(1, 0) | KERNEL_TYPE_MIX_AIC_1_0 | AIC为block，无subblock | 不涉及 |
| Mix算子 | \_\_mix\_\_(0, 1) | KERNEL_TYPE_MIX_AIV_1_0 | AIV为block，无subblock | 不涉及 |
| Mix算子 | \_\_mix\_\_(1, 1) | KERNEL_TYPE_MIX_AIC_1_1 | AIC为block，AIV为subblock | 1：1 |
| Mix算子 | \_\_mix\_\_(1, 2) | KERNEL_TYPE_MIX_AIC_1_2 | AIC为block，AIV为subblock | 1：2 |


**表2**  核间同步接口

| 接口名称 | 同步范围 | 对应模式 | 功能简述 |
| --- | --- | --- | --- |
| [asc_sync_inter_arrive](asc_sync_inter_arrive.md)/[asc_sync_inter_wait](asc_sync_inter_wait.md) | 组间同步，即不同group之间所有block（所有AIC或所有AIV）之间的同步。 | 模式0（同步等待全部AIC或全部AIV执行结束） | `asc_sync_inter_arrive`为生产者，负责发送通知；`asc_sync_inter_wait`为消费者，负责阻塞等待通知到达。 |
| [asc_sync_subblock_arrive](asc_sync_subblock_arrive.md)/[asc_sync_subblock_wait](asc_sync_subblock_wait.md) | 组内subblock间的同步，即同一group内不同subblock（AIV）之间的同步。 | 模式1（单个AI Core内，全部AIV之间的同步） | `asc_sync_subblock_arrive`为生产者，负责发送通知；`asc_sync_subblock_wait`为消费者，负责阻塞等待通知到达。 |
| [asc_sync_block_arrive](asc_sync_block_arrive.md)/[asc_sync_block_wait](asc_sync_block_wait.md) |同一group内block（AIC）与所有subblock（AIV）之间的同步。 | 模式2（单个AI Core内，AIC与全部AIV之间的同步） | `asc_sync_block_arrive`为生产者，负责发送通知；`asc_sync_block_wait`为消费者，负责阻塞等待通知到达。 |
| [asc_sync_intra_arrive](asc_sync_intra_arrive.md)/[asc_sync_intra_wait](asc_sync_intra_wait.md) | 同一group内block（AIC）与单个subblock（AIV）之间的同步。 | 模式4（单个AI Core内，AIC核与单个AIV之间同步） | `asc_sync_intra_arrive`为生产者，负责发送通知；`asc_sync_intra_wait`为消费者，负责阻塞等待通知到达。 |
