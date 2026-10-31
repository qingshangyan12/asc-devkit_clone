# System Synchronization Overview

<!-- md-trans-meta sourceCommit=5e55c38a4e567465079c4ca7387d07ab9e6347c5 translatedAt=2026-10-23T03:54:34.185146+00:00 pushedAt=2026-10-27T02:26:33.681Z nl=2 -->

In programming, synchronization refers to coordinating the access order and timing of shared resources among multiple execution units (such as threads and processes) to ensure program correctness. Without synchronization to strictly control the execution order, various problems such as inconsistent results and unpredictable behavior may occur.

AI Core synchronization is divided into 2 categories: [intra-core synchronization](intra_core_sync/intra_core_sync_overview.md) and [inter-core synchronization](inter_core_sync/inter_core_sync_overview.md). The APIs corresponding to each category are as follows.

**Table 1** Intra-core synchronization APIs

| API Name | Function Description |
| --- | --- |
| [asc_sync_notify](intra_core_sync/asc_sync_notify.md)/[asc_sync_wait](intra_core_sync/asc_sync_wait.md) | Multi-pipeline synchronization: synchronization instructions between different pipelines within the same core. `asc_sync_notify` sends a notification, and `asc_sync_wait` blocks until `asc_sync_notify` sends the notification. |
| [asc_sync_pipe](intra_core_sync/asc_sync_pipe.md) | Single-pipeline synchronization: synchronization instructions between the same pipeline within the same core (`PIPE_S` single-pipeline synchronization is not supported). |
| [asc_sync_data_barrier](intra_core_sync/asc_sync_data_barrier.md) | Blocks the execution of subsequent instructions until all preceding memory access instructions (the memory locations to wait for can be controlled by parameters) have completed. |
| [asc_lock](intra_core_sync/asc_lock.md)/[asc_unlock](intra_core_sync/asc_unlock.md) | Multi-pipeline synchronization: used for synchronization between asynchronous pipeline instructions within a core. Its function is similar to the lock mechanism in traditional CPUs. It locks a specified pipeline and then releases it to complete synchronization dependencies between pipelines. |
| [asc_sync_vec](intra_core_sync/asc_sync_vec.md) | Easy-to-use intra-core synchronization API: performs synchronization operations on `PIPE_V`, ensuring that subsequent instructions of other pipelines can start executing only after all preceding instructions in `PIPE_V` have completed. It can only be called in AIV. |
| [asc_sync_mte2](intra_core_sync/asc_sync_mte2.md) | Easy-to-use intra-core synchronization API: performs synchronization operations on `PIPE_MTE2`, ensuring that subsequent instructions of other pipelines can start executing only after all preceding instructions in `PIPE_MTE2` have completed. It can only be called in AIV. |
| [asc_sync_mte3](intra_core_sync/asc_sync_mte3.md) | Easy-to-use intra-core synchronization API: performs synchronization operations on `PIPE_MTE3`, ensuring that subsequent instructions of other pipelines can start executing only after all preceding instructions in `PIPE_MTE3` have completed. It can only be called in AIV. |
| [asc_sync](intra_core_sync/asc_sync.md) | All-pipeline synchronization: A synchronization instruction between all pipelines in the same core, functionally equivalent to `asc_sync_pipe(PIPE_ALL)`. Blocks subsequent instructions of all hardware pipelines after the call point until all preceding instructions of all hardware pipelines before the call point are complete. |

Figure 1 shows four synchronization control modes, whose functions are described below. The figure is based on a scenario where the kernel function is qualified with `__mix__(1, 2)` and the logical core count is `numBlocks=4`.

- Mode 0: Synchronization control between AI Core cores. For the all-AIC core scenario, all AIC cores are synchronized, and instructions after `asc_sync_inter_wait` execute only after all AIC cores have executed `asc_sync_inter_arrive`. For the all-AIV core scenario, all AIV cores are synchronized, and instructions after `asc_sync_inter_wait` execute only after all AIV cores have executed `asc_sync_inter_arrive`.
- Mode 1: Synchronization control between AIV cores within an AI Core. Instructions after `asc_sync_subblock_wait` execute only after both AIV cores have executed `asc_sync_subblock_arrive`.
- Mode 2: Synchronization control between AIC and AIV within an AI Core. Only after the AIC core executes `asc_sync_block_arrive` can instructions after `asc_sync_block_wait` on the two AIV cores continue to execute. Only after both AIV cores execute `asc_sync_block_arrive` can instructions after `asc_sync_block_wait` on the AIC core execute.
- Mode 4: Synchronization control between AIC and a single AIV within an AI Core. Only after a single AIV core executes `asc_sync_intra_arrive` can instructions after `asc_sync_intra_wait` on the AIC core continue to execute. Only after the AIC core executes `asc_sync_intra_arrive` can instructions after `asc_sync_intra_wait` on the single AIV core execute. AIV0 and AIV1 can independently trigger the AIC wait.

**Figure 1**  Schematic diagram of the four inter-core synchronization control modes<a id="fig_sync_control_mode"></a>    
![](../../../figures/3510_sync_control_mode_diagram.png)

**Table 2**  Inter-core synchronization APIs

| API Name | Synchronization Scope | Corresponding Mode | Function Description |
| --- | --- | --- | --- |
| [asc_sync_inter_arrive](inter_core_sync/asc_sync_inter_arrive.md)/[asc_sync_inter_wait](inter_core_sync/asc_sync_inter_wait.md) | Inter-group synchronization, that is, synchronization among all blocks (all AICs or all AIVs) in different groups. | Mode 0 (wait for all AICs or all AIVs to finish execution) | `asc_sync_inter_arrive` acts as the producer and sends notifications; `asc_sync_inter_wait` acts as the consumer and blocks until the notification arrives. |
| [asc_sync_subblock_arrive](inter_core_sync/asc_sync_subblock_arrive.md)/[asc_sync_subblock_wait](inter_core_sync/asc_sync_subblock_wait.md) | Synchronization among subblocks within a group, that is, synchronization among different subblocks (AIVs) in the same group. | Mode 1 (synchronization among all AIVs within a single AI Core) | `asc_sync_subblock_arrive` acts as the producer and sends notifications; `asc_sync_subblock_wait` acts as the consumer and blocks until the notification arrives. |
| [asc_sync_block_arrive](inter_core_sync/asc_sync_block_arrive.md)/[asc_sync_block_wait](inter_core_sync/asc_sync_block_wait.md) | Synchronization between a block (AIC) and all subblocks (AIVs) in the same group. | Mode 2 (synchronization between the AIC and all AIVs within a single AI Core) | `asc_sync_block_arrive` acts as the producer and sends notifications; `asc_sync_block_wait` acts as the consumer and blocks until the notification arrives. |
| [asc_sync_intra_arrive](inter_core_sync/asc_sync_intra_arrive.md)/[asc_sync_intra_wait](inter_core_sync/asc_sync_intra_wait.md) | Synchronization between a block (AIC) and a single subblock (AIV) in the same group. | Mode 4 (synchronization between the AIC core and a single AIV within a single AI Core) | `asc_sync_intra_arrive` acts as the producer and sends notifications; `asc_sync_intra_wait` acts as the consumer and blocks until the notification arrives. |
