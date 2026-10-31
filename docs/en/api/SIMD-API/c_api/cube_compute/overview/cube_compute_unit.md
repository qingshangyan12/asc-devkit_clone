# Cube Computation Unit

<!-- md-trans-meta sourceCommit=5df1d1c781b51fcb7826db7f2afab5f8bc453e93 translatedAt=2026-10-21T03:10:33.499Z pushedAt=2026-10-28T12:21:43.295Z nl=2 -->

The Cube computation unit is dedicated to executing cube operations. The dedicated buffers it directly accesses are as follows: the L0A Buffer stores the left cube, the L0B Buffer stores the right cube, and the L0C Buffer stores the accumulation initialization value and the cube computation result.

<!-- npu="910b,A3,950" id1 -->
The highlighted part in the following figure shows the Cube computation unit and the dedicated buffers it directly accesses.
<!-- end id1 -->

<!-- npu="910b,A3" id2 -->
**Figure 1** [NPU architecture version 2201](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md) Cube computation unit architecture  
![](../../../../figures/architecture_of_cube_compute_unit_a2a3.png "Cube computation unit architecture - A2A3")
<!-- end id2 -->

<!-- npu="950" id3 -->
**Figure 2** [NPU architecture version 3510](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md) Cube computation unit architecture  
![](../../../../figures/npu_3510_hw_arch_cube.png "Cube computation unit architecture - 950")
<!-- end id3 -->

The storage units associated with cube computation are as follows:

| Buffer Type | Description |
| ----------- | ------ |
| L0A Buffer | Internal physical storage unit in the AI Core, typically used to store the left cube for cube computation. |
| L0B Buffer | Internal physical storage unit in the AI Core, typically used to store the right cube for cube computation. |
| L0C Buffer | Internal physical storage unit in the AI Core, typically used to store the cube computation result and the cube accumulation initialization value. |
| L1 Buffer | Internal physical storage unit in the AI Core with a relatively large capacity, typically used to cache the input data for cube computation. The input for cube computation generally needs to be moved from GM to the L1 Buffer, and then moved to the L0A Buffer and L0B Buffer respectively. |
| Fixpipe Buffer | Internal physical storage unit in the AI Core, typically used to store data such as quantization parameters required during Fixpipe data movement. |
| BiasTable Buffer | Bias storage, an internal physical storage unit in the AI Core, typically used to store the bias data required for cube computation. |
