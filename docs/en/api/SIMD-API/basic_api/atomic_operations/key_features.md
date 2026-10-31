# Key Features<a name="ZH-CN_TOPIC_0000002555621184"></a>

<!-- md-trans-meta sourceCommit=69e8932e723e077228981554026ee19b14b10fa7 translatedAt=2026-09-23T10:29:09.964Z pushedAt=2026-10-26T07:41:43.957Z nl=2 -->

Deterministic computation refers to a computation process that always produces completely identical output results under the same input conditions, regardless of how the number of executions or the execution environment changes. Deterministic computation provides a guarantee for system stability and experimental verifiability.

## Deterministic Computation Overview<a name="ZH-CN_TOPIC_0000002555780816"></a>

To illustrate the problem of non-deterministic computation in atomic operation scenarios, we construct the following common deterministic computation scenario: first, initialize **GM** through a single set of floating-point data copy; then, start the atomic add operation; finally, accumulate multiple sets of floating-point data on **GM** through multiple data copies. The specific pseudocode is as follows:

```text
1. Copy data0 to GM;    // Copy data, overwriting the original random value in GM; the expected GM data is data0.
2. SetAtomicAdd();         // Enable atomic add; subsequent copies from Unified Buffer (UB)/L0C Buffer/L1 Buffer to GM all perform atomic add.
3. Copy data1 to GM;    // Data copy with the on-the-fly atomic operation; the expected GM data is data0 + data1.
4. Copy data2 to GM;    // Data copy with the on-the-fly atomic operation; the expected GM data is data0 + data1 + data2.
5. Copy data3 to GM;    // Data copy with the on-the-fly atomic operation; the expected GM data is data0 + data1 + data2 + data3.
```

As shown in Figure 1, the developer's expected result is that the order in which instructions are issued strictly corresponds to the actual instruction execution order. No matter how many times this code segment is executed, the final **GM** data is always data0 + data1 + data2 + data3, with completely consistent results, thereby achieving deterministic computation.

**Figure 1**  Deterministic computation scenario, GM data change process<a name="zh-cn_topic_0000002552941426_fig71821376818"></a>

![](../../../figures/deterministic_computation_gm_data_change_process.png "Deterministic computation scenario, GM data change process")

In practice, however, if developers do not intervene, the execution order of these instructions may change each time the program runs, ultimately causing the GM data to be inconsistent with the expected result. The following lists two possible instruction execution orders and their corresponding execution flows.

## Non-deterministic computation, result 1<a name="zh-cn_topic_0000002552941426_section027603463315"></a>

**Figure 2**  Non-deterministic computation scenario 1, GM data change process<a name="zh-cn_topic_0000002552941426_fig273814664819"></a>

![](../../../figures/non_deterministic_computation_1_gm_data_change_process.png "Non-deterministic computation scenario 1, GM data change process")

As shown in Figure 2, the instruction execution flow in this scenario is as follows:

1. In the initial state, the GM data is a random value;
2. Move data0 to GM, and the GM data is initialized to data0;
3. Execute SetAtomicAdd to enable atomic add for subsequent move instructions, and the GM data is data0;
4. The three move instructions with on-the-fly atomic operations are out of order, and the actual execution order is "move out data2 → move out data3 → move out data1". The final GM data is data0 + data2 + data3 + data1.

**Cause 1 of non-deterministic computation:**

The out-of-order execution of move instruction with on-the-fly atomic operations, combined with the fact that floating-point addition does not satisfy the associative law (i.e., (a+b)+c != a+(b+c)), causes the final GM data data0 + data2 + data3 + data1 to deviate from the expected data0 + data1 + data2 + data3.

The following three preconditions must be met for the out-of-order execution of move instructions with on-the-fly atomic operations to cause a deviation in the final result:

- The atomic operation type is atomic add (maximum and minimum operations satisfy the associative law).
- The atomic operation data type is floating-point (integer addition satisfies the associative law).
- The number of move instructions with on-the-fly atomic operations reaches 3 or more (floating-point addition satisfies the commutativity law but not the associative law).

## Non-deterministic Computation, Result 2<a name="zh-cn_topic_0000002552941426_section1547411390818"></a>

**Figure 3**  Non-deterministic computation scenario 2, GM data change process<a name="zh-cn_topic_0000002552941426_fig1829915169912"></a>

![](../../../figures/non_deterministic_computation_2_gm_data_change_process.png "Non-deterministic computation scenario 2, GM data change process")

As shown in Figure 3, the instruction execution flow in this scenario is as follows:

1. In the initial state, the GM data is: random value;
2. Execute SetAtomicAdd to enable atomic add for subsequent move instructions. The GM data is: random value;
3. Execute two move instructions with on-the-fly atomic operations in sequence, in the execution order of "move out data1 → move out data2". The GM data is: random value + data1 + data2;
4. Move data0 to GM. The accumulated result on GM is overwritten by data0. The GM data is: data0;
5. Finally, execute the move of data3. The final data on GM is: data0 + data3.

**Cause 2 of non-deterministic computation:**

Out-of-order execution between an ordinary move instruction issued before atomic add is enabled and a move instruction with atomic operations enabled causes the data on the GM on which the atomic operation has been completed to be incorrectly overwritten by data0, resulting in a non-deterministic computation result.

This kind of result deviation caused by out-of-order execution does not require any precondition. Developers no longer need to distinguish the atomic operation type or the atomic operation data type, nor do they need to consider whether the number of move instructions with on-the-fly atomic operations reaches three or more.

## Deterministic Computation Implementation Scheme<a name="ZH-CN_TOPIC_0000002586300751"></a>

Based on the two root causes of non-deterministic computation, the following describes the scheme for implementing deterministic computation from these two aspects. The core idea is to insert appropriate synchronization between instructions so that the relevant instructions execute in the expected deterministic order each time the program runs, ultimately ensuring that the program produces the same output each time it runs. Specifically, it includes the following two aspects:

- Insert synchronization between the move instruction before atomic add is enabled and the instruction that enables the atomic operation

    As shown in the following pseudocode, inserting synchronization between instructions 1 and 2 ensures that the initial value of GM meets expectations before the atomic operation starts.

- Synchronization between multiple move instructions after the atomic add operation is enabled

    Inserting synchronization between instructions 3 and 4, and between 4 and 5, ensures that the order of floating-point accumulation meets expectations.

    >[!CAUTION]Note
    >Developers do not need to insert synchronization between the instruction that enables the atomic operation and the subsequent move instructions.

```text
// The entire atomic add is executed within the same core, controlling the execution order of the five instructions as "1→2→3→4→5".
1. Move data0 to GM;    // Copy data, overwriting the original random value in GM, expecting the GM data to be data0.
intra-core synchronization
2. SetAtomicAdd();         // Enable atomic add. All subsequent moves from UB/L0C Buffer/L1 Buffer to GM perform atomic add.
// No synchronization is required between instructions 2 and 3.
3. Move data1 to GM;    // Data move after atomic add is enabled. The expected GM data is data0 + data1.
intra-core synchronization
4. Move data2 to GM;    // Data move after atomic add is enabled. The expected GM data is data0 + data1 + data2.
intra-core synchronization
5. Move data3 to GM;    // Data move after atomic add is enabled. The expected GM data is data0 + data1 + data2 + data3.
```

As shown in the following pseudocode, when the preceding instructions are executed on different cores, the intra-core synchronization above must be replaced with inter-core synchronization.

```text
// The entire atomic accumulation is executed on 4 different cores, controlling the execution order of the 4 cores as "core 0→core 1→core 2→core 3".
if (GetBlockIdx() == 0) {
   Move data0 to GM;
   inter-core synchronization
} else if (GetBlockIdx() == 1) {
   inter-core synchronization
   SetAtomicAdd();
   Move data1 to GM;
   inter-core synchronization
} else if (GetBlockIdx() == 2) {
   inter-core synchronization
   SetAtomicAdd();
   Move data2 to GM;
   inter-core synchronization
} else if (GetBlockIdx() == 3) {
   inter-core synchronization
   SetAtomicAdd();
   Move data3 to GM;
}
```

The following describes how to implement intra-core synchronization based on hardware synchronization instructions, and how to implement inter-core synchronization based on the software synchronization scheme.

## Intra-core Synchronization<a name="zh-cn_topic_0000002583421469_section6896184932119"></a>

The pipeline types of move instructions and instructions with atomic operations enabled are shown in the following table. When the preceding instructions are executed within the same core, developers can insert [single-pipeline synchronization](../sync_control/intra_core_sync/PipeBarrier_ISASI.md) or [multi-pipeline synchronization](../sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md) as needed. For details, see the scenarioNum=1 branch in the [sample](#zh-cn_topic_0000002583421469_example1).

**Table 1** Pipeline types of instructions related to deterministic computation for atomic operations

| Instruction Name | Pipeline Type |
| --- | --- |
| DataCopy | PIPE_MTE3 |
| Fixpipe | PIPE_FIX |
| SetAtomicAdd/SetAtomicMax/SetAtomicMin | PIPE_S |

## Inter-core Synchronization<a name="zh-cn_topic_0000002583421469_section331316523218"></a>

Because no hardware synchronization API is currently provided for controlling the execution order among different cores, inter-core synchronization in deterministic computation scenarios must be implemented through software simulation. For the three scenarios of pure Vector operators, pure Cube operators, and Mix operators (which contain both Vector and Cube computation), the software synchronization schemes adopted differ, as shown in the following table.

**Table 2**  Software implementation schemes for inter-core synchronization

| operator type | software synchronization scheme | description |
| --- | --- | --- |
| Pure Vector operator | Scheme 1: Through the combination of multiple pairs of [IBSet](../sync_control/inter_core_sync/IBSet.md) and [IBWait](../sync_control/inter_core_sync/IBWait.md) APIs, synchronization among multiple AIVs can be achieved. Refer to [the branch with scenarioNum=2 in the sample](#zh-cn_topic_0000002583421469_example2).| Scheme 1 supports specifying that only some AIVs participate in synchronization, and can control the execution order of each AIV.|
| Pure Vector operator | Scheme 2: Through the combined use of the three APIs [InitDetermineComputeWorkspace](../sync_control/inter_core_sync/InitDetermineComputeWorkspace.md), [NotifyNextBlock](../sync_control/inter_core_sync/NotifyNextBlock.md), and [WaitPreBlock](../sync_control/inter_core_sync/WaitPreBlock.md), ensure that all AIV cores execute in ascending blockIdx order.| Scheme 2 requires all AIVs to participate in synchronization, and the execution order is fixed to ascending blockIdx order.|
| Pure Cube operator | Inter-core synchronization is implemented through the semaphore in GM. First establish synchronization between a pair of AIC cores, and then extend it to synchronization among multiple AIC cores. Refer to [the branch with scenarioNum=3 in the sample](#zh-cn_topic_0000002583421469_example3).| When accessing GM through the Scalar unit, the data consistency issue among multiple cores must be considered.|
| Mix operator | Inter-core synchronization is implemented through the semaphore in GM. Synchronization is first established between a pair of cores, and then extended to synchronization among multiple cores.| When accessing GM through the Scalar unit, the data consistency issue among multiple cores must be considered. Here, "core" can be either AIV or AIC.|

The following figure shows how two cores perform inter-core synchronization through the semaphore in GM:

- After the previous core completes data copy or starts an atomic operation, it writes the value 1 to the semaphore in the inter-core shared GM through the Scalar unit, indicating that its task is complete. Intra-core synchronization also needs to be inserted in the previous core:
    - When there are multiple move instructions in the previous core, intra-core synchronization 1 needs to be inserted between them.
    - Before the Scalar unit writes data to GM, it must be ensured that all preceding move instructions have completed execution. Therefore, intra-core synchronization 2 also needs to be inserted between them.

- Before executing the copy task, the current core continuously reads the value of the semaphore through the Scalar unit. If the semaphore is not equal to 1, the current core enters a blocked waiting state. When the semaphore is detected to be equal to 1, the current core unblocks and starts executing its own data copy or atomic operation. To ensure that the current core does not execute the move instruction before the semaphore equals 1, intra-core synchronization 3 needs to be inserted before the move instruction.

**Figure 4**  Software synchronization scheme flowchart between a pair of cores<a name="zh-cn_topic_0000002583421469_fig1491417244811"></a>

![](../../../figures/software_synchronization_between_two_cores_flowchart.png "Software synchronization flowchart between two cores")

The Scalar unit accesses the semaphore on the GM in two ways:

- Access through the DCache

    Use the **GetValue** and **SetValue** member functions of **GlobalTensor** for the operation. In this case, developers need to manually call the [DataCacheCleanAndInvalid](../cache_control/DataCacheCleanAndInvalid.md) API to ensure data consistency across multiple cores.

- Access without the DCache

    Use [WriteGmBypassDCache](../scalar_compute/WriteGmBypassDCache_ISASI.md) and [ReadGmBypassDCache](../scalar_compute/ReadGmBypassDCache_ISASI.md). This method ensures data consistency across multiple cores without additional operations.

The performance difference between the two schemes is as follows: bypassing the DCache results in lower performance. However, if the amount of GM data read and written is small, the method that bypasses the DCache can be considered.

As shown in Figure 4, the inter-core synchronization scheme also needs to be used together with intra-core synchronization. The functions of the three intra-core synchronizations are described as follows:

- Intra-core synchronization 1 (optional): When multiple data move instructions exist within a core, this synchronization ensures that each move operation is executed strictly in order.
- Intra-core synchronization 2 (mandatory): Only after all tasks of the previous core are completed is the Scalar unit allowed to write 1 to the global memory semaphore.
- Intra-core synchronization 3 (required): Wait until the Scalar unit detects that the semaphore is updated to 1, and then the current core starts executing subsequent tasks.

## Example<a name="zh-cn_topic_0000002583421469_section027603463315"></a>

- **<a name="zh-cn_topic_0000002583421469_example1"></a>Intra-core synchronization deterministic computation (corresponding to scenarioNum=1): All instructions are executed in the same AIV**

    ```cpp
    // Initialize GM with src0Local before the atomic add.
    AscendC::DataCopy(dstGlobal, src0Local, DATA_LENGTH);
    // Ensure that GM initialization is complete before the atomic add.
    AscendC::PipeBarrier<PIPE_MTE3>();

    // Enable the atomic add.
    // The expected add execution order is: src2Local + src3Local + src1Local.
    AscendC::SetAtomicAdd<float>();
    AscendC::DataCopy(dstGlobal, src2Local, DATA_LENGTH);
    // Call PipeBarrier<PIPE_MTE3>() between the two move instructions to ensure the correct move order of src2Local and src3Local.
    AscendC::PipeBarrier<PIPE_MTE3>();
    AscendC::DataCopy(dstGlobal, src3Local, DATA_LENGTH);
    AscendC::PipeBarrier<PIPE_MTE3>();
    AscendC::DataCopy(dstGlobal, src1Local, DATA_LENGTH);

    AscendC::DisableDmaAtomic();
    // GM result: data0 + data2 + data3 + data1.
    ```

- **<a name="zh-cn_topic_0000002583421469_example2"></a>Pure Vector operator inter-core synchronization (corresponding to scenarioNum=2): Four moves are executed in four different AIVs**

    ```cpp
    // The expected execution order of the 4 cores is core 0 --> core 2 --> core 3 --> core 1.
    // Core 0 is responsible for initializing GM with src0Local before the atomic add; cores 2, 3, and 1 are responsible for performing the atomic add in order.

    if (blockIdx == 0) {
        AscendC::LocalTensor<float> src0Local = inQueueSrc0.AllocTensor<float>();
        AscendC::DataCopy(src0Local, src0Global, DATA_LENGTH);
        AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(0);

        // Initialize GM with src0Local before the atomic add.
        AscendC::DataCopy(dstGlobal, src0Local, DATA_LENGTH);

        // Core 0 notifies core 2 that the src0Local initialization of core 0 is complete.
        auto sync_buf = vecIn.AllocTensor<int32_t>();
        AscendC::IBSet(src4Global, sync_buf, 0, 0); // The last two parameters: int32_t blockIdx, int32_t eventID.
        inQueueSrc0.FreeTensor(src0Local);
        vecIn.FreeTensor(sync_buf);
        AscendC::printf("core index %d\n", blockIdx);
    } else if (blockIdx == 1) {
        AscendC::LocalTensor<float> src1Local = inQueueSrc1.AllocTensor<float>();
        // Core 1 waits for core 3 until the data of core 3 has been accumulated.
        auto sync_buf = vecIn.AllocTensor<int32_t>();
        AscendC::IBWait(src4Global, sync_buf, 3, 0); // The last two parameters: int32_t blockIdx, int32_t eventID.

        AscendC::DataCopy(src1Local, src1Global, DATA_LENGTH);
        AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(0);

        AscendC::DisableDmaAtomic();
        AscendC::SetAtomicAdd<float>();
        AscendC::DataCopy(dstGlobal, src1Local, DATA_LENGTH);
        AscendC::DisableDmaAtomic();
        vecIn.FreeTensor(sync_buf);
        inQueueSrc1.FreeTensor(src1Local);
        AscendC::printf("core index %d\n", blockIdx);
    } else if (blockIdx == 2) {
        AscendC::LocalTensor<float> src2Local = inQueueSrc2.AllocTensor<float>();
        // Core 2 waits for core 0 until core 0 has completed GM initialization.
        auto sync_buf = vecIn.AllocTensor<int32_t>();
        AscendC::IBWait(src4Global, sync_buf, 0, 0); // The last two parameters: int32_t blockIdx, int32_t eventID.

        AscendC::DataCopy(src2Local, src2Global, DATA_LENGTH);
        AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(0);

        AscendC::DisableDmaAtomic();
        AscendC::SetAtomicAdd<float>();
        AscendC::DataCopy(dstGlobal, src2Local, DATA_LENGTH);
        AscendC::DisableDmaAtomic();

        // Core 2 notifies core 3 that the data of core 2 has been accumulated.
        AscendC::IBSet(src4Global, sync_buf, 2, 0);
        vecIn.FreeTensor(sync_buf);
        inQueueSrc2.FreeTensor(src2Local);
        AscendC::printf("core index %d\n", blockIdx);
    } else if (blockIdx == 3) {
        // Core 3 waits for core 2 until the data of core 2 has been accumulated.
        auto sync_buf = vecIn.AllocTensor<int32_t>();
        AscendC::IBWait(src4Global, sync_buf, 2, 0); // The last two parameters: int32_t blockIdx, int32_t eventID.

        AscendC::LocalTensor<float> src3Local = inQueueSrc3.AllocTensor<float>();
        AscendC::DataCopy(src3Local, src3Global, DATA_LENGTH);
        //  AscendC::DumpTensor(src3Local,0,8);
        AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(0);

        AscendC::DisableDmaAtomic();
        AscendC::SetAtomicAdd<float>();
        AscendC::DataCopy(dstGlobal, src3Local, DATA_LENGTH);
        AscendC::DisableDmaAtomic();

        // Core 3 notifies core 1 that the data of core 3 has been accumulated.
        AscendC::IBSet(src4Global, sync_buf, 3, 0);
        vecIn.FreeTensor(sync_buf);
        inQueueSrc3.FreeTensor(src3Local);
        AscendC::printf("core index %d\n", blockIdx);
    }
    // GM result: data0 + data2 + data3 + data1.
    ```

- **<a name="zh-cn_topic_0000002583421469_example3"></a>Pure Cube inter-core synchronization (corresponding to scenarioNum=3): The four moves are executed in four different AICs.**

    ```cpp
    // The expected execution order of the four cores is core 0 --> core 2 --> core 3 --> core 1.
    // Core 0 initializes GM with src0Local before the atomic add; cores 2, 3, and 1 perform the atomic add in order.
    if (blockIdx == 0) {
        AscendC::DataCopy(src0Local, src0Global, DATA_LENGTH);
        AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(0);

        // Initialize GM with src0Local before the atomic add.
        AscendC::DataCopy(dstGlobal, src0Local, DATA_LENGTH);

        // Core 0 notifies core 2 that the initialization of src0Local on core 0 is complete.
        // Wait for the current core's task to complete before notifying the next core.
        AscendC::SetFlag<AscendC::HardEvent::MTE3_S>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_S>(0);
        AscendC::WriteGmBypassDCache<int32_t>(addr + blockIdx * 32, 1);
    } else if (blockIdx == 1) {
        int32_t preblockIdx = 3;

        // Core 1 waits for core 3 until the data on core 3 has been accumulated.
        while (true) {
            int32_t value = AscendC::ReadGmBypassDCache<int32_t>(addr + preblockIdx * 32);
            if (value == 1) {
                AscendC::WriteGmBypassDCache<int32_t>(addr + preblockIdx * 32, 0);
                break;
            }
        }
        // Wait for the notification from the previous core before starting the current core's task.
        AscendC::SetFlag<AscendC::HardEvent::S_MTE2>(0);
        AscendC::WaitFlag<AscendC::HardEvent::S_MTE2>(0);

        AscendC::DataCopy(src1Local, src1Global, DATA_LENGTH);
        AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(0);

        AscendC::DisableDmaAtomic();
        AscendC::SetAtomicAdd<float>();
        AscendC::DataCopy(dstGlobal, src1Local, DATA_LENGTH);
        AscendC::DisableDmaAtomic();
    } else if (blockIdx == 2) {
        int32_t preblockIdx = 0;

        // Core 2 waits for core 0 until core 0 has completed GM initialization.
        while (true) {
            int32_t value = AscendC::ReadGmBypassDCache<int32_t>(addr + preblockIdx * 32);
            if (value == 1) {
                AscendC::WriteGmBypassDCache<int32_t>(addr + preblockIdx * 32, 0);
                break;
            }
        }
        // Wait for the notification from the previous core before starting the current core's task.
        AscendC::SetFlag<AscendC::HardEvent::S_MTE2>(0);
        AscendC::WaitFlag<AscendC::HardEvent::S_MTE2>(0);

        AscendC::DataCopy(src2Local, src2Global, DATA_LENGTH);
        AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(0);

        AscendC::DisableDmaAtomic();
        AscendC::SetAtomicAdd<float>();
        AscendC::DataCopy(dstGlobal, src2Local, DATA_LENGTH);
        AscendC::DisableDmaAtomic();

        // Wait for the current core's task to complete before notifying the next core.
        AscendC::SetFlag<AscendC::HardEvent::MTE3_S>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_S>(0);
        AscendC::WriteGmBypassDCache<int32_t>(addr + blockIdx * 32, 1);
    } else if (blockIdx == 3) {
        int32_t preblockIdx = 2;

        // Core 3 waits for core 2 until the data on core 2 has been accumulated.
        while (true) {
            int32_t value = AscendC::ReadGmBypassDCache<int32_t>(addr + preblockIdx * 32);
            if (value == 1) {
                AscendC::WriteGmBypassDCache<int32_t>(addr + preblockIdx * 32, 0);
                break;
            }
        }

        // Wait for the notification from the previous core before starting the current core's task.
        AscendC::SetFlag<AscendC::HardEvent::S_MTE2>(0);
        AscendC::WaitFlag<AscendC::HardEvent::S_MTE2>(0);

        AscendC::DataCopy(src3Local, src3Global, DATA_LENGTH);
        AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(0);

        AscendC::DisableDmaAtomic();
        AscendC::SetAtomicAdd<float>();
        AscendC::DataCopy(dstGlobal, src3Local, DATA_LENGTH);
        AscendC::DisableDmaAtomic();

        // Wait for the current core's task to complete before notifying the next core.
        AscendC::SetFlag<AscendC::HardEvent::MTE3_S>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_S>(0);
        AscendC::WriteGmBypassDCache<int32_t>(addr + blockIdx * 32, 1);
    }
    // GM result: data0 + data2 + data3 + data1.
    ```

For the complete sample, see [Sample of deterministic computation using data copy with on-the-fly atomic operations](../../../../../../examples/01_simd_cpp_api/03_basic_api/06_atomic/set_atomic_deterministic_computation).
