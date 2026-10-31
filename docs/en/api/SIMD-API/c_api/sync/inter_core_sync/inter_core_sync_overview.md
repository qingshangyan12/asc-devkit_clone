# Inter-Core Synchronization Overview

<!-- md-trans-meta sourceCommit=5e55c38a4e567465079c4ca7387d07ab9e6347c5 translatedAt=2026-10-23T03:54:34.165056+00:00 pushedAt=2026-10-27T02:26:33.584Z nl=2 -->

Inter-core synchronization is typically used when a core can obtain the data required for its computation only after all other related cores have completed their computations.  

In Figure 1, AIC depends on the `asc_reduce` computation result of AIV. Because the overall vector is large, it must be split into multiple parts, with each AIV completing part of the computation. Each AIV writes its partial computation result to GM through atomic accumulation. AIC must read the final result after all AIVs complete the accumulation. Therefore, inter-core synchronization is required to ensure the timing correctness when AIC reads the `asc_reduce` result.

**Figure 1** Example of an inter-core synchronization service scenario (AIC-to-AIV ratio is 1:2)    
![](../../../../figures/inter_core_sync_scenario_example.png)

The AIC/AIV multi-core structure is shown in Figure 2. AICs and AIVs are divided into groups. A group contains 1 block and N subblocks (N≥1), where block indicates the primary core and each subblock indicates a secondary core.

Operators can be classified into three types by computation characteristics: Cube operators (Cube computation), Vector operators (vector computation), and Mix operators (including both Cube computation and vector computation). As shown in Table 1, the operator type determines the required inter-core synchronization mode and the available group configuration mode. For Mix operators, the block/subblock relationship between AIC and AIV is specified by the function execution space qualifier. Cube operators and Vector operators are executed by a single type of core and do not involve block/subblock division. The configuration in which AIV is the block and AIC is the subblock (that is, Vector Core as the primary core and Cube Core as the secondary core) is currently not supported. Table 2 summarizes the inter-core synchronization APIs and their supported synchronization scenarios.

**Figure 2** Relationship between block and subblock (the gray part indicates a group, that is, 1 block and N subblocks)<a id="fig_block_subblock_relationship"></a>    
![](../../../../figures/block_subblock_relationship_3510.png)

**Table 1** Group configuration

| Operator Type | Function Execution Space Qualifier | Kernel Type | block/subblock | 1:N |
| --- | --- | --- | --- | --- |
| Cube operator | `__cube__` | `KERNEL_TYPE_AIC_ONLY` | AIC is a block, with no subblock. | Not involved. |
| Vector operator | `__vector__` | `KERNEL_TYPE_AIV_ONLY` | AIV is a block, with no subblock. | Not involved. |
| Mix operator | `__mix__(1, 0)` | `KERNEL_TYPE_MIX_AIC_1_0` | AIC is a block, with no subblock. | Not involved. |
| Mix operator | `__mix__(0, 1)` | `KERNEL_TYPE_MIX_AIV_1_0` | AIV is a block, with no subblock. | Not involved. |
| Mix operator | `__mix__(1, 1)` | `KERNEL_TYPE_MIX_AIC_1_1` | AIC is a block, and AIV is a subblock. | 1: 1 |
| Mix operator | `__mix__(1, 2)` | `KERNEL_TYPE_MIX_AIC_1_2` | AIC is a block, and AIV is a subblock. | 1: 2 |

**Table 2** Supported inter-core synchronization scenarios

| API Name | Synchronization Scope | Corresponding Mode | Function Description |
| --- | --- | --- | --- |
| [asc_sync_inter_arrive](asc_sync_inter_arrive.md)/[asc_sync_inter_wait](asc_sync_inter_wait.md) | Inter-group synchronization, that is, synchronization among all blocks (all AICs or all AIVs) in different groups. | Mode 0 (wait for all AICs or all AIVs to finish execution) | `asc_sync_inter_arrive` acts as the producer and sends notifications; `asc_sync_inter_wait` acts as the consumer and blocks until the notification arrives. |
| [asc_sync_subblock_arrive](asc_sync_subblock_arrive.md)/[asc_sync_subblock_wait](asc_sync_subblock_wait.md) | Synchronization among subblocks within a group, that is, synchronization among different subblocks (AIVs) in the same group. | Mode 1 (synchronization among all AIVs within a single AI Core) | `asc_sync_subblock_arrive` acts as the producer and sends notifications; `asc_sync_subblock_wait` acts as the consumer and blocks until the notification arrives. |
| [asc_sync_block_arrive](asc_sync_block_arrive.md)/[asc_sync_block_wait](asc_sync_block_wait.md) | Synchronization between a block (AIC) and all subblocks (AIVs) in the same group. | Mode 2 (synchronization between the AIC and all AIVs within a single AI Core) | `asc_sync_block_arrive` acts as the producer and sends notifications; `asc_sync_block_wait` acts as the consumer and blocks until the notification arrives. |
| [asc_sync_intra_arrive](asc_sync_intra_arrive.md)/[asc_sync_intra_wait](asc_sync_intra_wait.md) | Synchronization between a block (AIC) and a single subblock (AIV) in the same group. | Mode 4 (synchronization between the AIC core and a single AIV within a single AI Core) | `asc_sync_intra_arrive` acts as the producer and sends notifications; `asc_sync_intra_wait` acts as the consumer and blocks until the notification arrives. |
