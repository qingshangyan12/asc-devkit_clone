# Real-Time Quantization and Real-Time ReLU Scenario Combinations

<!-- md-trans-meta sourceCommit=5df1d1c781b51fcb7826db7f2afab5f8bc453e93 translatedAt=2026-10-22T12:50:43.574Z pushedAt=2026-10-28T12:45:45.420Z nl=2 -->

Matrix move-out supports multiple combinations of real-time quantization and real-time ReLU. For the commonly used not-activated/Normal ReLU combinations, see the following table. For Scalar/Vector extended combinations, see the following sections.

<!-- npu="950,A3,910b" id1 -->
The combinations of real-time quantization and real-time ReLU are as follows:

<!-- npu="A3,910b" id2 -->
**Table 1** Combinations of real-time quantization and real-time ReLU ([NPU architecture version 2201](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))

| quant_pre / relu_pre | 0 (not activated) | 1 (Normal ReLU) |
| ------------------ | --------- | ------------ |
| &bull;REQ8<br>&bull;DEQF16<br>&bull;QF322B8_PRE | M1=QUANT_PRE\[31:0\]<br>M2=QUANT_PRE\[31:0\]<br>M1 and M2 all are quantization parameters | M1=QUANT_PRE\[31:0\]<br>M2=0<br>M1 as quantization parameter, and M2 as NORMAL coefficient |
| &bull;VREQ8<br>&bull;VDEQF16<br>&bull;VQF322B8_PRE | M1=Quant_PRE_ADDR\[i\]\[31:0\]<br>M2=Quant_PRE_ADDR\[i\]\[31:0\]<br>M1 and M2 all are quantization parameters, and i as the column index of the original cube | M1=Quant_PRE_ADDR\[i\]\[31:0\]<br>M2=0<br>M1 as quantization parameter, M2 as NORMAL coefficient, and i as the column index of the original cube |

<!-- end id2 -->

<!-- npu="950" id3 -->
**Table 2** Combinations of real-time quantization and real-time ReLU ([NPU architecture version 3510](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))

| quant_pre_mode / relu_pre_mode | NONE | NORMAL |
| --- | --- | --- |
| &bull;REQ8<br>&bull;DEQF16<br>&bull;QF322B8_PRE<br>&bull;QF322FP8_PRE<br>&bull;QF322HIF8_PRE<br>&bull;QF322HIF8_PRE_HYBRID<br>&bull;QS322BF16_PRE<br>&bull;QF322F16_PRE<br>&bull;QF322BF16_PRE<br>&bull;QF322F32_PRE | M1=QUANT_PRE\[31:13\]<br>M2=QUANT_PRE\[31:13\]<br>M1 and M2 all are quantization parameters | M1=QUANT_PRE\[31:13\]<br>M2=0<br>M1 as quantization parameter, and M2 as NORMAL coefficient |
| &bull;VREQ8<br>&bull;VDEQF16<br>&bull;VQF322B8_PRE<br>&bull;VQF322FP8_PRE<br>&bull;VQF322HIF8_PRE<br>&bull;VQF322HIF8_PRE_HYBRID<br>&bull;VQS322BF16_PRE<br>&bull;VQF322F16_PRE<br>&bull;VQF322BF16_PRE<br>&bull;VQF322F32_PRE | M1=Quant_PRE_ADDR\[i\]\[31:13\]<br>M2=Quant_PRE_ADDR\[i\]\[31:13\]<br>M1 and M2 all are quantization parameters, and i as the column index of the original cube | M1=Quant_PRE_ADDR\[i\]\[31:13\]<br>M2=0<br>M1 as quantization parameter, M2 as NORMAL coefficient, and i as the column index of the original cube |

<!-- end id3 -->

Note: M1 is the real-time coefficient used when the original data is non-negative, and M2 is the real-time coefficient used when the original data is negative.
<!-- end id1 -->
