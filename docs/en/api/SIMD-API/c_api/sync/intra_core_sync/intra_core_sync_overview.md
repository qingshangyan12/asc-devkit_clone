# Intra-Core Synchronization Overview

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:54:34.183154+00:00 pushedAt=2026-10-27T02:26:33.670Z nl=2 -->

## Why Intra-Core Synchronization Is Needed

Execution units inside the AI Core (such as the MTE2 DMA unit and the Vector computation unit) run asynchronously in parallel and may have data dependencies when reading and writing the same storage resource. To ensure data consistency and computation correctness, synchronization control is required to coordinate operation timing.

<!-- npu="950" id2 -->
For [NPU architecture 3510](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch), the hardware architecture diagram is as follows. The highlighted part shows the compute unit and DMA unit that execute in parallel.

**Figure 1** NPU architecture 3510 diagram    
![](../../../../figures/npu_3510_hw_arch_sync.png)

<!-- end id2 -->

<!-- npu="A3,910b" id1 -->

For the [NPU architecture 2201](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch), the hardware architecture diagram is as follows. The highlighted part shows the compute unit and DMA unit that execute in parallel.

**Figure 2** NPU architecture 2201 diagram    
![](../../../../figures/atlas_a2_a3_architecture.png)

<!-- end id1 -->

The following example describes a common Vector computation data flow:

1. First, the DMA execution unit moves data from GM to Local Memory.
2. Perform computation.
3. Then, the DMA execution unit moves the computation result from Local Memory to GM.

**Figure 3** Vector computation data flow diagram    
![](../../../../figures/vector_compute_data_flow.png)

The four execution units Scalar, Vector, DMA (MTE2), and DMA (MTE3) execute in parallel. If they access the same Local Memory, a synchronization mechanism is required to control their access timing: ensure that data is moved into Local Memory before computation, and moved out after computation is complete.

**Figure 4** Intra-core parallel pipeline execution timing diagram    
![](../../../../figures/intra_core_parallel_pipeline_execution_timing.png)

## Hardware Pipeline Type

The parallel instruction pipeline types inside the AI Core and their descriptions are as follows:

> [!NOTE]
> For different hardware architectures, the specific pipelines included in each hardware pipeline type may differ. For details, see [Hardware Implementation](../../../../../guide/programming_guide/advanced_programming/hardware_implementation/hardware_implementation.md).

**Table 1** Instruction pipeline types and related descriptions

| Pipeline Type | Meaning |
| --- | --- |
| PIPE_S | Scalar pipeline, used when scalar memory access statements or scalar computation APIs access GM or on-chip memory addresses. |
| PIPE_V | Vector computation pipeline and, on some hardware architectures, the L0C Buffer->UB data movement pipeline. |
| PIPE_M | Cube computation pipeline |
| PIPE_MTE1 | L1 Buffer->L0A Buffer and L1 Buffer->L0B Buffer data movement pipelines. |
| PIPE_MTE2 | GM->L1 Buffer, GM->UB, and other data movement pipelines. |
| PIPE_MTE3 | UB->GM and other data movement pipelines. |
| PIPE_FIX | L0C Buffer->GM, L0C Buffer->L1 Buffer, and other data movement pipelines. |

In addition, `PIPE_ALL` indicates all pipelines and is used in scenarios where it is necessary to wait for all preceding instructions of all pipelines.

The values of the `pipe_t` enum type in the synchronization APIs are as follows: <a id="Hardware Pipeline Type"></a>

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


## Intra-Core Synchronization Categories

Synchronization control for the preceding intra-core parallel pipelines:

- Multi-pipeline synchronization: synchronization between instructions of different pipeline types that have data dependencies within the same core.

    - Use the [asc_sync_notify](asc_sync_notify.md)/[asc_sync_wait](asc_sync_wait.md) APIs to control synchronization between different pipelines. In the `asc_sync_notify`/`asc_sync_wait` instructions, you can specify the execution order of a pair of instruction pipelines (source pipeline and destination pipeline), which implements a "lock" mechanism between the two instruction pipelines. The principle is as follows:
        - `asc_sync_notify`: After all read and write operations of the preceding instructions in the source pipeline are complete, the current instruction starts to execute and sets the corresponding flag bit in the hardware to 1.
        - `asc_sync_wait`: When the destination pipeline reaches this instruction, if the corresponding flag bit in the hardware is 0, subsequent instructions in the destination pipeline remain blocked; if the corresponding flag bit in the hardware is 1, the flag bit is set to 0 and subsequent instructions in the destination pipeline begin execution.
     <!-- npu="950" id3 -->
     - For Ascend 950PR&950DT products, the [asc_lock](asc_lock.md)/[asc_unlock](asc_unlock.md) APIs are added to control synchronization between different pipelines. Use `asc_lock` to lock a specified pipeline (blocking subsequent instructions), and then use `asc_unlock` to release the pipeline, completing the synchronization dependency between pipelines.
        - `asc_lock`: Acquires the Mutex based on `mutex_id`. If the Mutex is already locked, it blocks the subsequent instruction queue of the specified pipeline until the Mutex corresponding to `mutex_id` in the preceding instructions is released by `asc_unlock`.
        - `asc_unlock`: After the preceding instructions of the current pipeline exit, it releases the corresponding Mutex based on `mutex_id`.
     - The combination of `asc_lock` and `asc_unlock` is more cohesive than `asc_sync_notify` and `asc_sync_wait` and is decoupled from other pipelines, simplifying reverse synchronization logic. For details, see [Description](asc_lock.md#description).
     <!-- end id3 -->
     - Use easy-to-use APIs such as [asc_sync_mte2](asc_sync_mte2.md), [asc_sync_mte3](asc_sync_mte3.md), and [asc_sync_vec](asc_sync_vec.md) for multi-pipeline synchronization. These APIs internally integrate multiple pairs of `asc_sync_notify` and `asc_sync_wait` to block subsequent instructions of multiple destination pipelines simultaneously until the preceding instructions of the specified source pipeline complete, thereby reducing the amount of manually written synchronization code. The synchronization relationships encapsulated in each API are as follows:
        - `asc_sync_mte2`: After the preceding instructions of `PIPE_MTE2` are complete, it releases the subsequent instructions of both destination pipelines `PIPE_MTE3` and `PIPE_V` at the same time. This is equivalent to executing a pair of `asc_sync_notify` and `asc_sync_wait` for `PIPE_MTE2`->`PIPE_MTE3` and `PIPE_MTE2`->`PIPE_V`, respectively.
        - `asc_sync_mte3`: After the preceding instructions of `PIPE_MTE3` are complete, it releases the subsequent instructions of both destination pipelines `PIPE_MTE2` and `PIPE_V` at the same time. This is equivalent to executing a pair of `asc_sync_notify` and `asc_sync_wait` for `PIPE_MTE3`->`PIPE_MTE2` and `PIPE_MTE3`->`PIPE_V`, respectively.
        - `asc_sync_vec`: After the preceding instructions of `PIPE_V` are complete, it releases the subsequent instructions of the three destination pipelines `PIPE_MTE2`, `PIPE_MTE3`, and `PIPE_S` at the same time. This is equivalent to executing a pair of `asc_sync_notify` and `asc_sync_wait` for `PIPE_V`->`PIPE_MTE2`, `PIPE_V`->`PIPE_MTE3`, and `PIPE_V`->`PIPE_S`, respectively.
- Single-pipeline synchronization: synchronization between instructions of the same pipeline type that have data dependencies within the same core.
    - Use the [asc_sync_pipe](asc_sync_pipe.md) API for synchronization control within the same pipeline. Although instructions in the same pipeline execute sequentially, this does not mean that the preceding instruction has completed when the next instruction starts. The `asc_sync_pipe` instruction ensures that all data reads and writes in the preceding instructions are complete before subsequent instructions start executing. Note that this API does not support synchronization of the `PIPE_S` single pipeline.
    - To synchronize all pipelines, use the [asc_sync](asc_sync.md) API, which is functionally equivalent to `asc_sync_pipe(PIPE_ALL)`. It blocks subsequent instructions of all hardware pipelines after the call point until all preceding instructions of all hardware pipelines before the call point have completed.
- Use the [asc_sync_data_barrier](asc_sync_data_barrier.md) API to block the execution of subsequent instructions until all previously issued but not yet completed memory access instructions have completed. Developers use the `arg` parameter to specify the memory range affected by the barrier, ensuring that memory access instructions before and after the barrier complete in the expected order.

<!-- npu="A3,910b" id4 -->
Taking [NPU architecture 2201](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch) as an example, all legal intra-core synchronization combinations under this hardware architecture are shown in [Table 2](#aic_intra_core_sync_combinations) and [Table 3](#aiv_intra_core_sync_combinations). Here, "Not involved" means that this synchronization combination does not exist at the hardware level, and "No application scenario yet" means that this synchronization combination exists but is not currently needed in actual development scenarios.

<a id="aic_intra_core_sync_combinations"></a>

**Table 2** All legal intra-core synchronization combinations in AIC

| Source Pipeline/Destination Pipeline | PIPE_S | PIPE_M | PIPE_MTE1 | PIPE_MTE2 | PIPE_MTE3 | PIPE_FIX |
| --- | --- | --- | --- | --- | --- | --- |
| PIPE_S | Not involved. | Not involved. | Not involved. | Not involved. | Not involved. | Not involved. |
| PIPE_M | Not involved. | `asc_sync_pipe(PIPE_M)` | `asc_sync_notify(PIPE_M, PIPE_MTE1, EVENT_ID0)`<br>`asc_sync_wait(PIPE_M, PIPE_MTE1, EVENT_ID0)` | `asc_sync_notify(PIPE_M, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_M, PIPE_MTE2, EVENT_ID0)` | Not involved. | `asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0)`<br>`asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0)` |
| PIPE_MTE1 | Not involved. | `asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0)` | `asc_sync_pipe(PIPE_MTE1)` | `asc_sync_notify(PIPE_MTE1, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE1, PIPE_MTE2, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE1, PIPE_MTE3, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE1, PIPE_MTE3, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE1, PIPE_FIX, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE1, PIPE_FIX, EVENT_ID0)` |
| PIPE_MTE2 | Not involved. | `asc_sync_notify(PIPE_MTE2, PIPE_M, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_M, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0)` | `asc_sync_pipe(PIPE_MTE2)` | `asc_sync_notify(PIPE_MTE2, PIPE_MTE3, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_MTE3, EVENT_ID0)` | No application scenario yet |
| PIPE_MTE3 | Not involved. | Not involved. | `asc_sync_notify(PIPE_MTE3, PIPE_MTE1, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE3, PIPE_MTE1, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE3, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE3, PIPE_MTE2, EVENT_ID0)` | `asc_sync_pipe(PIPE_MTE3)` | No application scenario yet |
| PIPE_FIX | Not involved. | `asc_sync_notify(PIPE_FIX, PIPE_M, EVENT_ID0)`<br>`asc_sync_wait(PIPE_FIX, PIPE_M, EVENT_ID0)` | `asc_sync_notify(PIPE_FIX, PIPE_MTE1, EVENT_ID0)`<br>`asc_sync_wait(PIPE_FIX, PIPE_MTE1, EVENT_ID0)` | No application scenario yet | No application scenario yet | `asc_sync_pipe(PIPE_FIX)` |

<a id="aiv_intra_core_sync_combinations"></a>

**Table 3** All legal intra-core synchronization combinations in AIV

| Source Pipeline/Destination Pipeline | PIPE_S | PIPE_V | PIPE_MTE2 | PIPE_MTE3 |
| --- | --- | --- | --- | --- |
| PIPE_S | Not involved. | `asc_sync_notify(PIPE_S, PIPE_V, EVENT_ID0)`<br>`asc_sync_wait(PIPE_S, PIPE_V, EVENT_ID0)` | `asc_sync_notify(PIPE_S, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_S, PIPE_MTE2, EVENT_ID0)` | `asc_sync_notify(PIPE_S, PIPE_MTE3, EVENT_ID0)`<br>`asc_sync_wait(PIPE_S, PIPE_MTE3, EVENT_ID0)` |
| PIPE_V | `asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0)`<br>`asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0)` | `asc_sync_pipe(PIPE_V)` | `asc_sync_notify(PIPE_V, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_V, PIPE_MTE2, EVENT_ID0)` | `asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0)`<br>`asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0)` |
| PIPE_MTE2 | `asc_sync_notify(PIPE_MTE2, PIPE_S, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_S, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0)` | `asc_sync_pipe(PIPE_MTE2)` | `asc_sync_notify(PIPE_MTE2, PIPE_MTE3, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE2, PIPE_MTE3, EVENT_ID0)` |
| PIPE_MTE3 | `asc_sync_notify(PIPE_MTE3, PIPE_S, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE3, PIPE_S, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE3, PIPE_V, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE3, PIPE_V, EVENT_ID0)` | `asc_sync_notify(PIPE_MTE3, PIPE_MTE2, EVENT_ID0)`<br>`asc_sync_wait(PIPE_MTE3, PIPE_MTE2, EVENT_ID0)` | `asc_sync_pipe(PIPE_MTE3)` |

<!-- end id4 -->

## When Developers Need to Manually Insert Synchronization

In C API programming, all synchronization must be managed manually by developers.
