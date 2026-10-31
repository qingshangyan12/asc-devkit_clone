# Key Features<a id="inter_core_sync_key_features"></a>

<!-- md-trans-meta sourceCommit=c0c5cde6df534c03224f524ed2b2d081018dca09 translatedAt=2026-09-23T18:47:04.624Z pushedAt=2026-10-26T07:41:44.803Z nl=2 -->

This document mainly describes two key aspects of inter-core synchronization:

- The occupancy status of inter-core synchronization flagIds, which explains the flagId value range occupied internally by each API and helps developers avoid flagId conflicts when using **CrossCoreSetFlag**/**CrossCoreWaitFlag**.
- The implementation principles of each inter-core synchronization control mode, which, together with timing diagrams, introduce the working mechanisms of modes 0, 1, 2, and 4.

## Inter-core Synchronization flagId Occupancy Description<a id="inter_core_sync_flagid_usage"></a>

### Overview

When developers use the [CrossCoreSetFlag](CrossCoreSetFlag_ISASI.md) and [CrossCoreWaitFlag](CrossCoreWaitFlag_ISASI.md) APIs for inter-core synchronization, they need to pass in the inter-core synchronization flag **flagId**. Each flagId corresponds to a counter used for synchronization control.

The internal implementation of some APIs also occupies a portion of flagIds. When developers use these APIs together with CrossCoreSetFlag/CrossCoreWaitFlag, if flagId conflicts occur (that is, mutually independent synchronization operations reuse the same flagId), the corresponding counters are incorrectly shared and modified, causing abnormal inter-core synchronization behavior: blocking may fail to be released and the program may hang, or synchronization may complete early and blocking may be released early, resulting in incorrect computation results.

Therefore, when developers call CrossCoreSetFlag/CrossCoreWaitFlag together with these APIs that occupy flagIds, they should **avoid the flagIds already occupied internally by each API** and select flagIds that are not occupied.

Inter-core synchronization is divided into **modes 0, 1, and 2** and **mode 4**. The underlying hardware instructions of the two differ, and their flagIds are independent of each other. Therefore, for the occupancy status of flagIds of each API, refer to [flagId Occupancy of Modes 0, 1, and 2](#inter_core_sync_flagid_mode012) and [flagId Occupancy of Mode 4](#inter_core_sync_flagid_mode4) by **mode** and **NPU architecture version** respectively.

### flagId Occupancy of Modes 0, 1, and 2<a id="inter_core_sync_flagid_mode012"></a>

**Table 1**  flagId occupancy status of modes 0, 1, and 2

| API | Condition | Occupied flagId | Occupancy status of each compute core | NPU architecture version |
| :--- | :--- | :--- | :--- | :--- |
| [SyncAll](SyncAll.md) | `isAIVOnly=true` | 14 | AIV: 14 | 2201, 3510 |
| [SyncAll](SyncAll.md) | `isAIVOnly=false` | 11, 12, 13 | AIC: 11, 12, 13; AIV: 12, 13 | 2201, 3510 |
| [SetNextTaskStart](../inter_task_sync/SetNextTaskStart.md) | Called in a SuperKernel sub-core function (Kernel) | 11, 14 | AIC: 11; AIV: 14 | 2201, 3510 |
| [WaitPreTaskEnd](../inter_task_sync/WaitPreTaskEnd.md) | Called in a SuperKernel sub-core function (Kernel) | 11, 12, 13, 14 | AIC: 11, 12, 13; AIV: 12, 13, 14 | 2201, 3510 |
| [Matmul](../../../adv_api/cube_compute/Matmul_Kernel/Matmul_Kernel.md) | Define N Matmul objects | `[0, 2N-1]` | AIV/AIC allocated by object and subBlock | 2201 |
| [Matmul](../../../adv_api/cube_compute/Matmul_Kernel/Matmul_Kernel.md) | SuperKernel fusion scenario ([GetTaskRatio](../../tool_interface/system_resources_and_variables/GetTaskRatio.md) returns 2, that is, a 1:2 ratio) | 15 | AIV: 15 | 2201 |

### flagId Occupancy in Mode 4<a id="inter_core_sync_flagid_mode4"></a>

**Table 2**  flagId occupancy status in mode 4

| API | Restriction | Occupied flagId | Occupancy status per compute core | NPU architecture version |
| :--- | :--- | :--- | :--- | :--- |
| [SyncAll](SyncAll.md) | `isAIVOnly=false` (when the kernel function (Kernel) is modified with `__mix__(1, 2)`) | 12, 13, 28, 29 | AIC: 12, 13, 28, 29; AIV: 12, 13 | 3510 |
| [WaitPreTaskEnd](../inter_task_sync/WaitPreTaskEnd.md) | Called in the SuperKernel sub-core function (Kernel) | 12, 13, 28, 29 | AIC: 12, 13, 28, 29; AIV: 12, 13 | 3510 |
| [Matmul](../../../adv_api/cube_compute/Matmul_Kernel/Matmul_Kernel.md) | Defines N [Matmul objects](../../../adv_api/cube_compute/Matmul_Kernel/REGIST_MATMUL_OBJ.md) | `[0, N+17]` | AIC/AIV allocated by object, eventType, and subBlock | 3510 |
| [Matmul](../../../adv_api/cube_compute/Matmul_Kernel/Matmul_Kernel.md) | Scenario where both matrix A and matrix B enable [IBSHARE](../../../adv_api/cube_compute/Matmul_Kernel/Matmul_usage.md#p84551411817) | 15 | AIV: 15 | 3510 |
| [TSCM](../../resource_management/TSCM/TSCM.md) | L1 Buffer (TSCM) queue initiated from UB | 0~11 (allocated one by one downward from 11) | AIV | 3510 |
| [DataCopy (GM→L1)](../../cube_compute_ISASI/cube_compute_load/DataCopy_GMToL1_continuous.md) | None | 1, 17 | AIC: 1, 17; AIV: 1 | 3510 |

### Restriction Conditions of Each API

#### SyncAll

- **isAIVOnly=true**: All-core synchronization for pure Vector operators, where the flagId occupied by modes 0, 1, and 2 is 14 (AIV).
- **isAIVOnly=false**: All-core synchronization for Mix (including Cube and Vector computation) operators. The occupancy status varies with the NPU architecture and the kernel function (Kernel) type:
    - **2201 architecture**: The occupied flagIds for occupancy modes 0, 1, and 2 are 11, 12, and 13.
    - **3510 architecture**:
        - When the sub-core function (Kernel) is decorated with `__mix__(1, 1)`, the occupied flagIds of occupancy modes 0, 1, and 2 are 11, 12, and 13.
        - When the sub-core function (Kernel) is decorated with `__mix__(1, 2)`, the occupied flagId for occupancy modes 0, 1, and 2 is 11, and the occupied flagIds for occupancy mode 4 are 12, 13, 28, and 29.

    For the description of the `isAIVOnly` template parameter above, see [SyncAll](SyncAll.md); for the description of the `__mix__(1, 1)`/`__mix__(1, 2)` modifiers, see [Function Execution Space Qualifier](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#function-execution-space-qualifiers).

#### SetNextTaskStart / WaitPreTaskEnd

- Both are dedicated APIs for the **SuperKernel** scenario and take effect only when operators are fused in the [SuperKernel](../../../../../guide/programming_guide/advanced_programming/super_kernel/principles.md) manner and called in the sub-core function (Kernel).
- **SetNextTaskStart**: The flagIds occupied in occupancy modes 0, 1, and 2 are 11 (AIC) and 14 (AIV). See [SetNextTaskStart](../inter_task_sync/SetNextTaskStart.md).
- **WaitPreTaskEnd**: See [WaitPreTaskEnd](../inter_task_sync/WaitPreTaskEnd.md).
    - **2201 architecture**: The flagIds occupied in occupancy modes 0, 1, and 2 are 11, 12, 13, and 14.
    - **3510 architecture**: The flagIds occupied in occupancy modes 0, 1, and 2 are 11, 12, 13, and 14, and the flagIds occupied in mode 4 are 12, 13, 28, and 29.

#### Matmul

- The range of occupied flagId is related to the number of defined **Matmul** objects N (N is at most 4), and varies with the NPU architecture. See [REGIST_MATMUL_OBJ](../../../adv_api/cube_compute/Matmul_Kernel/REGIST_MATMUL_OBJ.md#constraints).
- **2201 architecture**: The occupied flagId for occupancy modes 0, 1, and 2 is [0, 2N-1] (when N=4, `[0,7]` is occupied). In the SuperKernel fusion scenario (1:2 ratio, that is, [GetTaskRatio](../../tool_interface/system_resources_and_variables/GetTaskRatio.md) returns 2), flagId 15 is also occupied.
- **3510 architecture**: The occupied flagId for occupancy mode 4 is [0, N+17] (when N=4, `[0,21]` is occupied). In the scenario where both matrix A and matrix B enable [IBSHARE](../../../adv_api/cube_compute/Matmul_Kernel/Matmul_usage.md#p84551411817), flagId 15 is also occupied.

#### TSCM

- Triggered only when the L1 Buffer (TSCM) queue initiated from the UB is used. See [TSCM](../../resource_management/TSCM/TSCM.md).
- Occupies the flagId of occupancy mode 4, ranging from **0 to 11**: the first TSCM queue occupies 11, the second occupies 10, and so on, allocated in descending order, down to 0.

#### DataCopy (GM→L1)

- Under the **3510 architecture**, the flagIds occupied by occupancy mode 4 are 1 and 17 (AIC: 1, 17; AIV: 1). See [DataCopy (GMToL1 continuous data transfer)](../../cube_compute_ISASI/cube_compute_load/DataCopy_GMToL1_continuous.md).

## Implementation Principles of Each Inter-Core Synchronization Control Mode<a id="sync_control_mode"></a>
As shown in Figure 1, this chapter introduces, with the aid of timing diagrams, the implementation principles of the four synchronization control modes supported when CrossCoreSetFlag and CrossCoreWaitFlag are used together.

- Mode 0: Synchronization control between AI Cores. For the AIC all-core scenario, all AIC cores are synchronized. Only when all AIC cores have executed CrossCoreSetFlag will the instructions in all subsequent pipelines of CrossCoreWaitFlag, or in the pipeline specified by the template parameter pipe (depending on the NPU architecture), be executed. For the AIV all-core scenario, all AIV cores are synchronized. Only when all AIV cores have executed CrossCoreSetFlag will the instructions in all subsequent pipelines of CrossCoreWaitFlag, or in the pipeline specified by the template parameter pipe (depending on the NPU architecture), be executed.
- Mode 1: Synchronization control between AIV cores within an AI Core. If both AIV cores have executed CrossCoreSetFlag, the instructions in all subsequent pipelines of CrossCoreWaitFlag, or in the pipeline specified by the template parameter pipe (depending on the NPU architecture), will be executed.
- Mode 2: Synchronization control between the AIC and the AIVs within an AI Core. After the AIC core executes CrossCoreSetFlag, the instructions in all subsequent pipelines of CrossCoreWaitFlag on the two AIVs, or in the pipeline specified by the template parameter pipe (depending on the NPU architecture), will continue to execute. After both AIVs execute CrossCoreSetFlag, the instructions in all subsequent pipelines of CrossCoreWaitFlag on the AIC, or in the pipeline specified by the template parameter pipe (depending on the NPU architecture), can be executed.
- Mode 4: Synchronization control between the AIC and a single AIV within an AI Core. After the AIC core executes CrossCoreSetFlag, the instructions in the subsequent pipeline specified by the template parameter pipe on the single AIV will continue to execute. After the single AIV executes CrossCoreSetFlag, the instructions in the subsequent pipeline specified by the template parameter pipe on the AIC can be executed. This mode is supported only on [NPU architecture version 3510](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md).

> [!NOTE]Note
> In modes 0, 1, and 2, the pipeline type blocked by `CrossCoreWaitFlag` varies with the NPU architecture:
> - On [NPU architecture version 2201](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md), the `pipe` parameter passed to `CrossCoreWaitFlag` is invalid. Regardless of its value, all subsequent instructions of all pipelines are blocked by default. It is recommended to omit the `pipe` parameter and use the default value.
> - On [NPU architecture version 3510](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md), the `pipe` parameter passed to `CrossCoreWaitFlag` takes effect, blocking the subsequent instructions of the pipeline specified by the template parameter `pipe`.

**Figure 1**  Diagram of synchronization control modes<a id="sync_control_mode_diagram"></a>    
![](../../../../figures/3510_sync_control_mode_diagram.png "Diagram of synchronization control modes")

The following synchronization features all use the following scenario configuration as an example: the kernel function is decorated with `__mix__(1, 2)`, meaning that each AI Core contains 1 AIC and 2 AIVs, and the number of logical cores is set to `numBlocks=2`, meaning that 2 AI Cores are started in total, resulting in 2 AICs and 4 AIVs. For ease of description, the 2 AICs are individually numbered AIC0 and AIC1; the 2 AIVs in AI Core0 are individually numbered AIV0-0 and AIV0-1, and the 2 AIVs in AI Core1 are individually numbered AIV1-0 and AIV1-1. **The initial value of the counter corresponding to `flagId` in each core is 0.**

<!-- npu="A3,910b" id1 -->

> [!NOTE]Note
> The following descriptions of modes 0, 1, and 2 are valid only for [NPU architecture version 2201](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md). In this case, the `pipe` parameter passed to `CrossCoreWaitFlag` is invalid, so the default value is used, which blocks all subsequent instructions of all pipelines by default.

## All-Core Synchronization of AIC or AIV Across Multiple AI Cores (Mode 0)<a id="multi_ai_core_aic_aiv_full_sync"></a>

All-core synchronization of AIC or AIV across multiple AI Cores requires the paired use of the [CrossCoreSetFlag](CrossCoreSetFlag_ISASI.md) and [CrossCoreWaitFlag](CrossCoreWaitFlag_ISASI.md) APIs in mode 0. This scenario can be divided into two categories:

- All-core synchronization of two AICs across two AI Cores.

    When both AICs have finished executing CrossCoreSetFlag, the counter of the corresponding flagId of each AIC increases by 1. If the counter of this flagId of the AIC is non-zero, the instructions in all pipelines blocked by CrossCoreWaitFlag will be executed only then, and the counter of this flagId decreases by 1.

    For each AIC, CrossCoreSetFlag and CrossCoreWaitFlag must be used in pairs.

- All-core synchronization of four AIVs across two AI Cores.

    When all four AIVs have finished executing CrossCoreSetFlag, the counter corresponding to each AIV core's flagId increases by 1. If the AIV's counter for this flagId is non-zero, the instructions in all pipelines blocked by CrossCoreWaitFlag will execute, and the counter for this flagId decreases by 1.

    For each AIV, CrossCoreSetFlag and CrossCoreWaitFlag must be used in pairs.

Taking Figure 2 as an example, the following code snippet demonstrates all-core synchronization of two AICs (AIC0 and AIC1) across two AI Cores:

```cpp
// When performing inter-core synchronization, the core function cannot be modified with __cube__ even if only AICs participate in the synchronization. For details, see the constraints of CrossCoreSetFlag.
if (ASCEND_IS_AIC) {
    // Each core should have paired CrossCoreSetFlag and CrossCoreWaitFlag calls similar to the following.
    // modeId must be set to 0, and flagId must be consistent; CrossCoreWaitFlag uses the default pipe value.
    AscendC::CrossCoreSetFlag<0, PIPE_M>(0);       // After the preceding PIPE_M pipeline tasks complete, notify the scheduling module.
    AscendC::CrossCoreWaitFlag<0>(0);              // After all AICs complete the preceding PIPE_M pipeline and the scheduling module updates the counter of flagId=0, unblock the subsequent instructions of all pipelines.
}
```

After AIC0 executes CrossCoreWaitFlag, the counter of flagId=0 on AIC0 is 0, and all subsequent instructions in all pipelines are blocked. It must wait until both AIC cores have executed CrossCoreSetFlag.

After AIC1 finishes executing CrossCoreSetFlag, the scheduling module detects that both AICs have executed CrossCoreSetFlag, so the counter value of flagId=0 on each AIC increases to 1. AIC0 and AIC1 detect that their respective flagId=0 counters have become 1, unblock the subsequent instructions of all pipelines, continue executing the subsequent instructions, and decrement the counter value by 1.

**Figure 2**  Mode 0: All-core synchronization of AICs across multiple AI Cores<a id="multi_ai_core_aic_full_sync"></a>    
![](../../../../figures/inter_core_aic_all_sync.png "All-core synchronization of AICs across multiple AI Cores")

## AIV All-Core Synchronization in a Single AI Core (Mode 1)<a id="single_ai_core_aiv_full_sync"></a>

This scenario requires the paired use of the [CrossCoreSetFlag](CrossCoreSetFlag_ISASI.md) and [CrossCoreWaitFlag](CrossCoreWaitFlag_ISASI.md) APIs in mode 1.

Two AIVs in one AI Core perform all-core synchronization. When both AIVs have finished executing CrossCoreSetFlag, the counter of the corresponding flagId of each AIV increases by 1. If the counter of that flagId of an AIV is non-zero, the instructions in all pipelines blocked by CrossCoreWaitFlag are allowed to proceed, and the counter of that flagId decreases by 1. For each AIV, CrossCoreSetFlag and CrossCoreWaitFlag must be used in pairs.

> [!CAUTION]
> In this mode, AIVs of different AI Cores do not affect each other's synchronization.

Taking Figure 3 as an example, the two AIVs (AIV0-0 and AIV0-1) in the 0th AI Core perform all-core synchronization. The code snippet is as follows:

```cpp
// When performing inter-core synchronization, even if only AIVs participate in the synchronization, the core function cannot be modified with __vector__. For details, see the constraints of CrossCoreSetFlag.
if (ASCEND_IS_AIV) {
    if (AscendC:: GetBlockIdx() <= 1) {
        // The two AIVs participating in the synchronization belong to AI Core 0.
        // Each AIV should have paired calls to CrossCoreSetFlag and CrossCoreWaitFlag similar to the following.
        // modeId must be set to 1, and flagId must be consistent. The pipe of CrossCoreWaitFlag uses the default value.
        AscendC::CrossCoreSetFlag<1, PIPE_MTE3>(0);       // After the preceding PIPE_MTE3 pipeline task is complete, notify the scheduling module.
        AscendC::CrossCoreWaitFlag<1>(0);                  // Wait until all AIVs in this AI Core complete the preceding PIPE_MTE3 pipeline and the scheduling module updates the counter of flagId=0, then unblock the subsequent instructions of all pipelines.
    }
}
```

After AIV0-0 executes **CrossCoreWaitFlag**, the counter of flagId=0 in AIV0-0 is 0, and all instructions in all subsequent pipelines are blocked. It must wait until both AIVs have executed **CrossCoreSetFlag**.

After AIV0-1 finishes executing **CrossCoreSetFlag**, the scheduling module detects that both AIVs have completed **CrossCoreSetFlag**, so it increments the counter of flagId=0 in AIV0-0 and AIV0-1 respectively to 1. AIV0-0 and AIV0-1 detect that their respective counters of flagId=0 have become 1, unblock the subsequent instructions of all pipelines, continue executing the subsequent instructions, and decrement the counter value by 1.

**Figure 3**  Mode 1: All-core synchronization of AIVs in a single AI Core<a id="single_ai_core_aiv_full_sync_diagram"></a>    
![](../../../../figures/single_core_aiv_all_sync.png "All-core synchronization of AIVs in a single AI Core")

## All-Core Synchronization Between AIC and AIV in a Single AI Core (Mode 2)<a id="single_ai_core_aic_aiv_full_sync"></a>

All-core synchronization between AIC and AIV in a single AI Core requires the paired use of the [CrossCoreSetFlag](CrossCoreSetFlag_ISASI.md) and [CrossCoreWaitFlag](CrossCoreWaitFlag_ISASI.md) APIs in mode 2. This scenario can be divided into two categories:

- The AIC in one AI Core executes CrossCoreSetFlag, and the corresponding two AIVs both execute CrossCoreWaitFlag.

    When the AIC finishes executing CrossCoreSetFlag, the counter of the corresponding flagId on the two AIVs increases by 1. If the counter of this flagId on an AIV is non-zero, the blocking of all subsequent instructions in the AIV pipeline is released, the instructions after CrossCoreWaitFlag continue to be issued, and the counter of this flagId decreases by 1.

    In total, one CrossCoreSetFlag call by the AIC corresponds to one CrossCoreWaitFlag call by each of the two AIVs, which counts as a paired use.

- Both AIVs in one AI Core execute CrossCoreSetFlag, and the corresponding AIC executes CrossCoreWaitFlag.

    When both AIVs have finished executing CrossCoreSetFlag, the counter of the corresponding flagId on the AIC increases by 1. If the counter of this flagId on the AIC is non-zero, the blocking of all subsequent instructions in the AIC pipeline is released, the instructions after CrossCoreWaitFlag continue to be issued, and the counter of this flagId decreases by 1.

    In total, one CrossCoreSetFlag call by each of the two AIVs corresponds to one CrossCoreWaitFlag call by the AIC, which counts as a paired use.

Taking Figure 4 as an example, it demonstrates the synchronization between AIC0 and AIV0-0 and AIV0-1 in the 0th AI Core (the AIVs initiate CrossCoreSetFlag). The code snippet is as follows:

```cpp
// Both AIC and AIV participate in inter-core synchronization, so the corresponding macros must be used to wrap the code segments separately. For details, see the constraint description of CrossCoreSetFlag.
if (ASCEND_IS_AIV) {
    if (AscendC:: GetBlockIdx() <= 1) {
        // The AIVs participating in synchronization belong to AI Core 0.
        // Each AIV should have a CrossCoreSetFlag call similar to the following.
        // modeId must be set to 2, and flagId must be consistent.
        AscendC::CrossCoreSetFlag<2, PIPE_MTE3>(0);       // Notify the scheduling module after the preceding PIPE_MTE3 pipeline task completes. Must be executed by all AIVs.
    }
}
if (ASCEND_IS_AIC) {
    if (AscendC:: GetBlockIdx() == 0) {
        // The AIC participating in synchronization belongs to AI Core 0.
        // The AIC should have a CrossCoreWaitFlag call similar to the following.
        // CrossCoreWaitFlag uses the default pipe value.
        AscendC::CrossCoreWaitFlag<2>(0);                 // After all AIVs in this AI Core complete the preceding PIPE_MTE3 pipeline and the scheduling module updates the counter of flagId=0, the AIC unblocks the subsequent instructions of all pipelines.
    }
}
```

After AIC0 executes CrossCoreWaitFlag, the counter of AIC0 flagId=0 is 0, and the instructions in all subsequent pipelines are blocked until both AIVs complete CrossCoreSetFlag.

When the CrossCoreSetFlag of AIV0-1 is executed after Vector instruction 1, all instructions preceding PIPE\_V are complete, and CrossCoreSetFlag finishes execution. However, AIV0-0 has not executed CrossCoreSetFlag at this point. Therefore, the counter of AIC0 flagId=0 remains 0, and the instructions in all pipelines of the AIC are still blocked.

When AIV0-0 finishes executing Vector instructions 1 and 2, all preceding PIPE_V instructions are complete, and CrossCoreSetFlag takes effect. At this point, the scheduling module detects that both AIVs have completed CrossCoreSetFlag, so it increments the counter value of AIC0 flagId=0 to 1. When AIC0 detects that the counter corresponding to flagId=0 has become 1, the AIC0 core unblocks subsequent instructions in all pipelines, continues to execute the subsequent instruction Matrix instruction 2, and decrements the counter value by 1.

**Figure 4**  Mode 2: All-core synchronization between AIC and AIV in a single AI Core (AIV performs CrossCoreSetFlag)<a id="single_ai_core_aic_aiv_full_sync_diagram"></a>    
![](../../../../figures/single_core_aic_aiv_sync_aiv_setflag.png "All-core synchronization between AIC and AIV in a single AI Core (AIV performs CrossCoreSetFlag)")
<!-- end id1 -->

<!-- npu="950" id2 -->
> [!NOTE] Note
> Mode 4 is supported only on [NPU architecture version 3510](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md). In this case, the `pipe` parameter passed to `CrossCoreWaitFlag` takes effect, blocking the subsequent instructions of **the specified pipeline configured by the template parameter `pipe`**.

## Synchronization Between AIC and a Single AIV in a Single AI Core (Mode 4)<a id="single_ai_core_aic_single_aiv_sync"></a>

Synchronization between AIC and a single AIV in a single AI Core requires the paired use of the mode-4 [CrossCoreSetFlag](CrossCoreSetFlag_ISASI.md) and [CrossCoreWaitFlag](CrossCoreWaitFlag_ISASI.md) APIs. This scenario can be divided into two categories:

- The AIC in one AI Core executes CrossCoreSetFlag, and the corresponding AIV executes CrossCoreWaitFlag.

    When the AIC finishes executing CrossCoreSetFlag, the counter of the corresponding flagId on the AIV increases by 1. If the counter of this flagId on the AIV is non-zero, the blocking of the pipeline specified by the template parameter pipe on the AIV is released, the instructions after CrossCoreWaitFlag continue to be issued, and the counter of this flagId decreases by 1.

    In total, one CrossCoreSetFlag call by the AIC paired with one CrossCoreWaitFlag completion by a single AIV counts as a matched use.

- One AIV in one AI Core executes CrossCoreSetFlag, and the corresponding AIC executes CrossCoreWaitFlag.

    When the AIV finishes executing CrossCoreSetFlag, the counter of the corresponding flagId on the AIC increases by 1. If the counter of this flagId on the AIC is non-zero, the blocking of the pipeline specified by the template parameter pipe on the AIC is released, the instructions after CrossCoreWaitFlag continue to be issued, and the counter of this flagId decreases by 1.

    In total, one CrossCoreSetFlag call by a single AIV and one CrossCoreWaitFlag call by the AIC count as a matched pair.

Taking Figure 5 as an example, the synchronization between AIV0-1 and AIC0 in the 0th AI Core is demonstrated (AIV0-1 initiates CrossCoreSetFlag). The code snippet is as follows:

```cpp
// Both AIC and AIV participate in inter-core synchronization, so the corresponding macros must be used to wrap the code segments separately. For details, see the constraint description of CrossCoreSetFlag.
if (ASCEND_IS_AIV) {
    if ( AscendC:: GetBlockIdx() == 1) {
        // modeId must be configured to 4. For flagId, 0-15 on AIV0-1 correspond to 16-31 on AIC0.
        AscendC::CrossCoreSetFlag<4, PIPE_MTE3>(0);   // After the preceding PIPE_MTE3 pipeline tasks on AIV0-1 are complete, notify the scheduling module.
    }
}
if (ASCEND_IS_AIC) {
    if ( AscendC:: GetBlockIdx() == 0) {
        // modeId must be configured to 4, and flagId must be consistent. The pipe parameter of CrossCoreWaitFlag takes effect, and here the specified pipeline (PIPE_FIX) configured by the template parameter pipe is blocked.
        AscendC::CrossCoreWaitFlag<4, PIPE_FIX>(16);       // Block the subsequent instructions of the specified pipeline (PIPE_FIX here) configured by the template parameter pipe until all AIV0-1 in this AI Core complete the preceding PIPE_MTE3 pipeline, and after the scheduling module updates the counter of flagId=16, AIC0 is unblocked.
    }
}
```

After AIC0 executes CrossCoreWaitFlag, the counter of AIC0 flagId=16 is 0 at this point. All subsequent instructions of the specified pipeline (PIPE_FIX here) configured by the template parameter pipe are blocked, and it needs to wait until one AIV completes CrossCoreSetFlag.

- AIV0-0 does not need to execute CrossCoreSetFlag, nor does it need to participate in synchronization.
- After all PIPE_MTE3 instructions of AIV0-1 are executed, CrossCoreSetFlag takes effect. At this point, the scheduling module detects that one AIV has completed CrossCoreSetFlag, so it increases the counter value of AIC0 flagId=16 to 1. When AIC0 detects that the counter of the corresponding flagId=16 becomes 1, AIC0 unblocks the subsequent instructions of the specified pipeline configured by the template parameter pipe, continues to execute the subsequent PIPE_FIX instructions, and decrements the counter value by 1.

**Figure 5**  Mode 4: Synchronization between AIC and a single AIV in a single AI Core (AIV performs CrossCoreSetFlag)<a id="single_ai_core_aic_single_aiv_sync_diagram"></a>    
![](../../../../figures/single_ai_core_aic_single_aiv_sync.png "Synchronization between AIC and a single AIV in a single AI Core (AIV performs CrossCoreSetFlag)")
<!-- end id2 -->
