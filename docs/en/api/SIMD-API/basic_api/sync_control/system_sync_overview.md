# System Synchronization Overview<a name="ZH-CN_TOPIC_0000002586323695"></a>

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-09-23T18:53:05.162Z pushedAt=2026-10-26T07:41:44.808Z nl=2 -->

In programming, synchronization refers to coordinating the access order and timing of multiple execution units (such as threads and processes) to shared resources to ensure program correctness. Without synchronization to strictly control the execution order, various problems such as inconsistent results and unpredictable behavior may occur.

AI Core synchronization is divided into three categories: intra-core synchronization, inter-core synchronization, and inter-task synchronization. The APIs corresponding to each category are as follows.

**Table 1**  Intra-core synchronization APIs

| API Name | Function Description | Synchronization Category |
| --- | --- | --- |
| [SetFlag/WaitFlag\(ISASI\)](intra_core_sync/SetFlag_WaitFlag_ISASI.md) | Multi-pipeline synchronization: synchronization instructions between different pipelines within the same core. SetFlag sends the notification, and WaitFlag blocks until SetFlag sends the notification. | Hardware Synchronization |
| [PipeBarrier](intra_core_sync/PipeBarrier_ISASI.md) | Single-pipeline synchronization: synchronization instructions between the same pipeline within the same core (PIPE_S single-pipeline synchronization is not supported). | Hardware Synchronization |
| [DataSyncBarrier](intra_core_sync/DataSyncBarrier_ISASI.md) | Blocks the execution of subsequent instructions until all previous memory access instructions (the memory locations to wait for can be controlled through parameters) finish executing. | Hardware Synchronization |
| [Lock/Unlock](intra_core_sync/Mutex_ISASI.md) | Used for synchronization between asynchronous pipeline instructions within a core. Its function is similar to the lock mechanism in a traditional CPU. It locks a specified pipeline and then releases the pipeline to complete the synchronization dependency between pipelines. | Hardware Synchronization |

**Table 2**  Inter-core synchronization APIs

| API Name | Function Description | Synchronization Category |
| --- | --- | --- |
| [CrossCoreSetFlag](inter_core_sync/CrossCoreSetFlag_ISASI.md)/[CrossCoreWaitFlag](inter_core_sync/CrossCoreWaitFlag_ISASI.md) |Synchronization control between multiple AICs or AIVs. CrossCoreSetFlag sends the notification, and CrossCoreWaitFlag blocks until CrossCoreSetFlag sends the notification.<br>Supports the following synchronization scenarios:<br>&bull;Synchronously wait for all AICs or all AIVs to finish executing.<br>&bull;Synchronization between all AIVs within a single AI Core.<br>&bull;Synchronization between the AIC and all AIVs within a single AI Core. | Hardware Synchronization |
| [IBSet](inter_core_sync/IBSet.md)/[IBWait](inter_core_sync/IBWait.md) | IBSet is called in the waited core to notify the waiting core by writing the value 1 to a certain address block in global memory; IBWait is called in the waiting core to continuously read the value of the corresponding address block in global memory until its value becomes 1. | Software Synchronization |
| [SyncAll](inter_core_sync/SyncAll.md) | Used for synchronization control between multiple cores. Supports the following synchronization scenarios:<br>&bull;Full-core synchronization of pure Vector operators. For details, see [Mode 0 of CrossCoreSetFlag](inter_core_sync/key_features.md).<br>&bull;Full-core synchronization of fused operators, which implements three inter-core synchronization capabilities at once (see [Key Feature Description of CrossCoreSetFlag](inter_core_sync/key_features.md)): first, complete the synchronization between the AIC and all AIVs within a single AI Core (AIC waits for AIVs); then complete the synchronization of all AICs between multiple AI Cores; finally, complete the synchronization between all AIVs and the AIC within a single AI Core (AIVs wait for the AIC). | Hardware Synchronization |
| [SyncAll](inter_core_sync/SyncAll.md) | Supports only multi-core synchronization of pure Vector operators (the number of cores participating in synchronization can be specified through an input parameter). Each AIV writes the signal value 1 to the corresponding location in global memory, and then each AIV continuously polls the signal values corresponding to all AIVs in global memory until the signal values corresponding to all AIV cores become 1. | Software Synchronization |
| [InitDetermineComputeWorkspace](inter_core_sync/InitDetermineComputeWorkspace.md)/[NotifyNextBlock](inter_core_sync/NotifyNextBlock.md)/[WaitPreBlock](inter_core_sync/WaitPreBlock.md) | &bull; InitDetermineComputeWorkspace, NotifyNextBlock, and WaitPreBlock must be used in combination. They ensure that multiple AIV cores execute strictly in ascending order of blockIdx, which is suitable for scenarios requiring deterministic computation.<br>&bull; InitDetermineComputeWorkspace initializes the value of the GM shared memory. Only after initialization is complete can NotifyNextBlock and WaitPreBlock be called.<br>&bull; NotifyNextBlock notifies other cores by writing to global memory that the current core has finished executing and that other cores can continue; WaitPreBlock determines whether the current core can continue by reading global memory. | Software Synchronization |

**Table 3**  Inter-task synchronization APIs

| API Name | Function Description | Synchronization Category |
| --- | --- | --- |
| [SetNextTaskStart](./inter_task_sync/SetNextTaskStart.md) | Called in the sub-core function (Kernel) of SuperKernel. The instructions after the call can run in parallel with subsequent other sub-core functions (Kernel), improving overall performance. | Hardware Synchronization |
| [WaitPreTaskEnd](./inter_task_sync/WaitPreTaskEnd.md) | Called in the sub-core function (Kernel) of SuperKernel. The instructions before the call can run in parallel with preceding other sub-core functions (Kernel), improving overall performance. | Hardware Synchronization |
