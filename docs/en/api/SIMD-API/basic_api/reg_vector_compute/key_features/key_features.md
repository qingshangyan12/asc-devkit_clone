# Key Features

<!-- md-trans-meta sourceCommit=ca3b72f6b4f376702407e982cf2cc174087341f5 translatedAt=2026-09-23T16:18:23.753Z pushedAt=2026-10-26T07:41:44.528Z nl=2 -->

## Hardware Loop (HardwareLoop)

In the Reg vector computation architecture, the Vector Function (VF) is the core carrier for implementing high-performance vector computation. A VF function can contain up to four levels of nested loops, and each loop level can also contain multiple serial loops. VF loops have limited support for control structures, supporting only **for** loops and conditional statements, and do not support other control structures such as **switch**, **do-while**, and **while-do**. When the loops in a VF function meet the Hardware Loop coding specifications, they are optimized by the compiler into Hardware Loops, which improves overall performance. Otherwise, the loop logic is composed of iteration variables and conditional statements to form a Software Loop, and VF loop optimization cannot be enabled.

### Hardware Loop Coding Specifications
- Hardware Loop supports a maximum of four nesting levels and supports serial Hardware Loops.
- Iteration variable type<br>The iteration variables of all loops in a VF must be of the **uint16_t** type.
- Start value and step<br>The loop start value begins from 0.<br>The step of each iteration must increment by 1.
- Jump instructions are not allowed inside a loop, such as conditional jumps like **if/else** and the ternary operator **?:**.<br>**if/else** inside a loop in a VF hinders Hardware Loop generation. Although the compiler performs **if/else** elimination optimization as much as possible, completeness is not guaranteed.
- Once execution begins, the loop count/boundary must not be modified.
- To use the count of an outer loop as the loop boundary, assign the outer loop counter to a scalar and then use it as the inner loop boundary.

## Dual Instruction Issue Optimization
For the single/dual issue status of Reg vector computation API instructions, see [Reg Vector Computation API Instruction Single/Dual Issue Summary](../../../../appendix/reg_vector_compute_interface_instruction_single_dual_issue_summary.md).

For detailed performance optimization of dual instruction issue, see [Operator Practice Reference - Dual Instruction Issue Optimization](../../../../../guide/operator_practice/simd_operator_optimization/vector_compute/vf_optimization/dual_issue_optimization.md).

### Dual Instruction Issue

Dual instruction issue means that the processor can issue two instructions to the execution units for processing in the same clock cycle.

This mechanism improves the instruction processing efficiency of the processor per unit time without changing the program logic, and is one of the important foundations for implementing instruction-level parallelism.

### Reasonably Split VF Loops

A longer VF loop is not necessarily better, and it is not advisable to place all operations in a single for loop. Intermediate results should be moved out to the Unified Buffer (UB) as appropriate to reduce data dependencies. When there are too many instructions, an Icache miss may be triggered, and dual instruction issue cannot be performed even if there is no dependency between loops. When a loop contains synchronization, the loop can be split to hoist the synchronization out and reduce the number of synchronization operations.

### Manually Controlled Loop Splitting

If a loop contains too many instructions with heavy dependencies, the instructions cannot execute concurrently, meaning the dual instruction issue feature cannot be enabled. You can unroll the loop to improve dual instruction issue capability, approaching the hardware's out-of-order execution behavior, and reduce the waiting caused by register resources not being ready.

## VF Fusion
VF fusion merges multiple VF functions in the code into a single VF function, effectively improving performance. The VF fusion feature is a compiler optimization feature, enabled through the compilation option `--cce-simd-vf-fusion=true`. Automatic VF fusion uses the Loop Fuse algorithm to convert VFs into Loop form, then fuses VFs that are control-flow-equivalent, and finally restores the VFs. The compiler first performs a legality check before fusion to determine whether two VFs are equivalent, whether the Main-side intermediate code can be executed within the VF, and whether positive gains can be produced after fusion. If the VF fusion conditions are met, the compiler automatically performs VF fusion optimization. To ensure that the execution logic and semantics of the fused VF are consistent with those before fusion, synchronization instructions are conservatively inserted between the original two VFs. The compiler also attempts to hoist and merge instructions in the fused VF to optimize the VF code. The fusion strategy is to fuse as much as possible. Users can increase the opportunities for VF fusion by coding according to the patterns that satisfy the fusion legality check. Users can also refer to the fusion principles to perform fusion optimization manually.

For details on VF fusion performance optimization, see [Operator Practice Reference - VF Fusion Optimization](../../../../../guide/operator_practice/simd_operator_optimization/vector_compute/vf_optimization/vf_fusion_optimization.md).

## Data Movement Optimization
The Reg vector computation API provides various data movement instructions as shown in the following table. Select appropriate data movement APIs and leverage their capabilities for optimization. For details on performance optimization in continuous non-aligned scenarios, see [Operator Practice Reference - Continuous Non-aligned Scenario Optimization](../../../../../guide/operator_practice/simd_operator_optimization/vector_compute/vf_optimization/continuous_unaligned_optimization.md).

| Scenario | Description |
| :-- | :-- | 
| Consecutive aligned load (LoadAlign) | Loads data from the UB with a 32B-aligned start address consecutively into RegTensor. |
| Non-consecutive aligned load (LoadAlign) | Loads data from the UB with a 32B-aligned start address non-consecutively into RegTensor.|
| Consecutive non-aligned load (LoadUnAlign) | Loads data from the UB with a non-32B-aligned start address consecutively into RegTensor.|
| MaskReg load (LoadAlign) | Loads data from the UB with a 32B-aligned start address consecutively into MaskReg.|
| MaskReg load (MaskGenWithRegTensor) | Loads data from RegTensor into MaskReg.|
| Scattered load (Gather) | Collects source operand elements into the destination operand RegTensor according to the index values.|
| Scattered load (GatherB) | Collects source operand elements into the destination operand RegTensor by DataBlock (32B) according to the index values.|

### Enabling Distribution Mode for Data Movement

- Consecutive aligned load (LoadAlign)

  As shown in [half-to-int32_t type conversion process](#fig1), the half-type elements with a data volume of VL/2 at the UB address xAddr are loaded into xReg through LoadAlign (distribution mode DIST_UNPACK_B16), Cast is called to convert half to int32_t and write the result to yReg, and finally the data is stored to the UB address yAddr through StoreAlign (distribution mode DIST_NORM).

  **Figure 1** half-to-int32_t type conversion process<a id="fig1"></a>

  ![](../../../../figures/half_to_int32_conversion.png "half-to-int32_t type conversion process")

  ```cpp
  // Bit width conversion from small to large, using half->int32_t as an example. 
  static constexpr AscendC::Reg::CastTrait castTrait = {
      AscendC::Reg::RegLayout::ZERO,
      AscendC::Reg::SatMode::UNKNOWN,
      AscendC::Reg::MaskMergeMode::ZEROING,
      AscendC::RoundMode::CAST_FLOOR
  };
  AscendC::Reg::LoadAlign<half, AscendC::Reg::PostLiteral::POST_MODE_UPDATE, AscendC::Reg::LoadDist::DIST_UNPACK_B16>(xReg, xAddr, oneRepeatSize); 
  AscendC::Reg::Cast<int32_t, half, castTrait>(yReg, xReg, mask);
  AscendC::Reg::StoreAlign<int32_t, AscendC::Reg::PostLiteral::POST_MODE_UPDATE>(yAddr, yReg, oneRepeatSize, mask);  
  ```

- Consecutive aligned store (StoreAlign)

  As shown in [float-to-int16_t type conversion process](#fig2), the float-type elements with a data volume of VL at UB address xAddr are loaded into xReg through LoadAlign (distribution mode DIST_NORM), Cast is called to convert float to int16_t and write the result to yReg, and finally the data is stored to UB address yAddr through StoreAlign (distribution mode DIST_PACK_B32).

  **Figure 2** float-to-int16_t type conversion process<a id="fig2"></a>

  ![](../../../../figures/float_to_int16_conversion.png "float-to-int16_t type conversion process")

  ```cpp
  // Bit width conversion from larger to smaller, taking float->int16_t as an example. 
  AscendC::Reg::LoadAlign<float, AscendC::Reg::PostLiteral::POST_MODE_UPDATE>(xReg, xAddr, oneRepeatSize);  
  AscendC::Reg::Cast<int16_t, float, castTrait>(yReg, xReg, mask); 
  AscendC::Reg::StoreAlign<int16_t, AscendC::Reg::PostLiteral::POST_MODE_UPDATE, AscendC::Reg::StoreDist::DIST_PACK_B32>(yAddr, yReg, oneRepeatSize, mask); 
  ```
