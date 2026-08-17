# sync概述

Tensor API的数据搬运和矩阵计算接口不会自动处理不同流水线、不同核或不同子核之间的全部依赖关系。在使用Tensor API组织多级流水、复用Local Memory或进行多核协同时，可配合使用C API提供的同步控制接口。使用相关接口时，需要额外引入`c_api/sync.h`，具体约束和产品支持情况以对应C API资料为准。

同步控制相关C API如下表所示。

| 接口 | 用途 |
| :--- | :--- |
| [asc_sync](../c_api/sync/asc_sync.md) | 等待所有流水线操作完成。 |
| [asc_sync_pipe](../c_api/sync/asc_sync_pipe.md) | 等待指定流水线的前序数据读写完成，约束同一流水线内的执行顺序。 |
| [asc_sync_mte2](../c_api/sync/asc_sync_mte2.md) | 等待PIPE_MTE2流水线的前序操作完成。 |
| [asc_sync_mte3](../c_api/sync/asc_sync_mte3.md) | 等待PIPE_MTE3流水线的前序操作完成。 |
| [asc_sync_notify](../c_api/sync/asc_sync_notify.md) | 设置流水线同步标志，与`asc_sync_wait`配合使用。 |
| [asc_sync_wait](../c_api/sync/asc_sync_wait.md) | 等待流水线同步标志，阻塞当前流水线直到条件满足。 |
| [asc_lock](../c_api/sync/asc_lock.md) | 获取Mutex并按需阻塞指定流水线。 |
| [asc_unlock](../c_api/sync/asc_unlock.md) | 在前序指令完成后释放指定Mutex。 |
| [asc_sync_data_barrier](../c_api/sync/asc_sync_data_barrier.md) | 等待指定内存范围的前序访问完成，处理Scalar内存依赖。 |
| [asc_sync_intra_arrive](../c_api/sync/asc_sync_intra_arrive.md) | 在分离模式下发送同一Block内AIC与指定AIV之间的同步事件。 |
| [asc_sync_intra_wait](../c_api/sync/asc_sync_intra_wait.md) | 在分离模式下等待同一Block内AIC与指定AIV之间的同步事件。 |
| [asc_sync_inter_arrive](../c_api/sync/asc_sync_inter_arrive.md) | 在分离模式下发送不同Group之间的同步事件。 |
| [asc_sync_inter_wait](../c_api/sync/asc_sync_inter_wait.md) | 在分离模式下等待不同Group之间的同步事件。 |
| [asc_sync_block_arrive](../c_api/sync/asc_sync_block_arrive.md) | 在分离模式下发送同一Group内Block与SubBlock之间的同步事件。 |
| [asc_sync_block_wait](../c_api/sync/asc_sync_block_wait.md) | 在分离模式下等待同一Group内Block与SubBlock之间的同步事件。 |
| [asc_sync_subblock_arrive](../c_api/sync/asc_sync_subblock_arrive.md) | 在分离模式下发送同一Group内不同SubBlock之间的同步事件。 |
| [asc_sync_subblock_wait](../c_api/sync/asc_sync_subblock_wait.md) | 在分离模式下等待同一Group内不同SubBlock之间的同步事件。 |
