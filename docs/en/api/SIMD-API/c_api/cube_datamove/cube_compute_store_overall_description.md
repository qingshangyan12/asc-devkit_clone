# Matrix Move-out Overview

<!-- md-trans-meta sourceCommit=5df1d1c781b51fcb7826db7f2afab5f8bc453e93 translatedAt=2026-10-22T13:38:03.457Z pushedAt=2026-10-28T12:45:45.494Z nl=2 -->

The move-out of cube computation is a core category of APIs in the Ascend C programming framework for data movement, primarily implementing data transfer from the L0C Buffer to Global Memory and the L1 Buffer. This API family provides multiple data movement modes to meet the data movement requirements in cube computation, and is typically used in conjunction with the asc_mmad API.

The move-out APIs of cube computation support multiple inline data movement scenarios, including **inline quantization, inline ReLU, inline format conversion (Nz2ND, Nz2DN), inline channel split, and inline channel merge**. By configuring API parameters and auxiliary registers, you can precisely control key information such as the data movement path, start position, movement length, and inline movement capabilities, fully leveraging the hardware data movement capability to maximize the overall execution performance of the operator.

<!-- npu="950,A3,910b" id1 -->
The overall flow of cube computation move-out is shown in the following figure:
<!-- end id1 -->

<!-- npu="A3,910b" id2 -->
**Figure 1** Overall flow of cube computation move-out ([NPU architecture version 2201](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))

![](../../../figures/fixpipe_execution_flow_a2a3.png)
<!-- end id2 -->

<!-- npu="950" id3 -->
**Figure 2** Overall flow of cube computation move-out ([NPU architecture version 3510](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))

![](../../../figures/fixpipe_execution_flow_a5.png)
<!-- end id3 -->

The memory layout of the Fixpipe Buffer corresponding to different quantization modes is as follows:

**Figure 3** Fixpipe Buffer memory layout

![](../../../figures/fixpipe_buffer.png)
