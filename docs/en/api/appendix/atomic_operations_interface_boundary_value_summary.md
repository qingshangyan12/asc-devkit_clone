# Summary of Atomic Operation Boundary Values<a name="ZH-CN_TOPIC_0000002608310699"></a>

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-29T10:32:36.114Z pushedAt=2026-10-29T10:53:06.276Z nl=2 -->

## Overview<a name="section3674181114910"></a>

This section summarizes the two types of output results of atomic operation APIs under boundary value input. The following content applies to the following models:

<!-- npu="A3" id2 -->
- Atlas A3 products
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products
<!-- end id3 -->

Under boundary value input, atomic operation APIs produce two types of output results:

- INF/NAN mode (non-saturation mode)

    For the three scenarios where the computation result is inf, -inf, or nan, the result is output as is.

- Saturation mode

    This mode is set through the [SetSaturationFlag](../SIMD-API/basic_api/special_register_access/SetSaturationFlag_ISASI.md) API. In saturation mode, inf and -inf are saturated to MAX and MIN for output, and nan is saturated to 0 for output.

## Specific Calculation Results of Atomic Operation APIs Under Boundary Value Input<a name="section8282511541"></a>

Note: In the following tables, MAX/MIN correspond to the maximum and minimum values of the operand's data type.

**Table 1**  Calculation results of SetAtomicAdd under special value/boundary value input

<a name="table15675934145714"></a>

| data type of src/dst | element value of src0 | element value of src1 | element value of dst<br>(INF/NAN mode) | element value of dst<br>(saturation mode) |
| --- | --- | --- | --- | --- |
| half | -65504.0 (MIN) | -65504.0 (MIN)/-inf | -inf | **-65504.0** |
| half | -65504.0 (MIN) | 65504.0 (MAX) | 0.0 | 0.0 |
| half | -65504.0 (MIN) | 0.0/-0.0 | -65504.0 | -65504.0 |
| half | -65504.0 (MIN) | nan | nan | **0.0** |
| half | -65504.0 (MIN) | inf | inf | **65504.0** |
| half | 65504.0 (MAX) | 65504.0 (MAX)/inf | inf | **65504.0** |
| half | 65504.0 (MAX) | 0.0/-0.0 | 65504.0 | 65504.0 |
| half | 65504.0 (MAX) | nan | nan | **0.0** |
| half | 65504.0 (MAX) | -inf | -inf | **-65504.0** |
| half | 0.0 | 0.0/-0.0 | 0.0 | 0.0 |
| half | 0.0 | nan | nan | **0.0** |
| half | 0.0 | inf | inf | **65504.0** |
| half | 0.0 | -inf | -inf | **-65504.0** |
| half | -0.0 | -0.0 | -0.0 | -0.0 |
| half | -0.0 | nan | nan | **0.0** |
| half | -0.0 | inf | inf | **65504.0** |
| half | -0.0 | -inf | -inf | **-65504.0** |
| half | nan | nan/inf/-inf | nan | **0.0** |
| half | inf | inf | inf | **65504.0** |
| half | inf | -inf | nan | **0.0** |
| half | -inf | -inf | -inf | **-65504.0** |

Note: The results in the element value of dst (INF/NAN mode) column are consistent with the IEEE 754 theoretical results.

**Table 2**  Calculation results of SetAtomicMax under special value/boundary value input

<a name="table57228341577"></a>

| data type of src/dst | element value of src0 | element value of src1 | element value of dst<br>(INF/NAN mode) | element value of dst<br>(saturation mode) |
| --- | --- | --- | --- | --- |
| half | -65504.0 (MIN) | -65504.0 (MIN)/-inf | -65504.0 | -65504.0 |
| half | -65504.0 (MIN) | 65504.0 (MAX) | 65504.0 | 65504.0 |
| half | -65504.0 (MIN) | 0.0 | 0.0 | 0.0 |
| half | -65504.0 (MIN) | -0.0 | -0.0 | -0.0 |
| half | -65504.0 (MIN) | nan | nan | **0.0** |
| half | -65504.0 (MIN) | inf | inf | **65504.0** |
| half | 65504.0 (MAX) | 65504.0 (MAX)/0.0/-0.0/-inf | 65504.0 | 65504.0 |
| half | 65504.0 (MAX) | nan | nan | **0.0** |
| half | 65504.0 (MAX) | inf | inf | **65504.0** |
| half | 0.0 | 0.0/-0.0/-inf | 0.0 | 0.0 |
| half | 0.0 | nan | nan | **0.0** |
| half | 0.0 | inf | inf | **65504.0** |
| half | -0.0 | -0.0/-inf | -0.0 | -0.0 |
| half | -0.0 | nan | nan | **0.0** |
| half | -0.0 | inf | inf | **65504.0** |
| half | nan | nan/inf/-inf | nan | **0.0** |
| half | inf | inf/-inf | inf | **65504.0** |
| half | -inf | -inf | -inf | **-65504.0** |

Note: The results in the element value of dst (INF/NAN mode) column are consistent with the IEEE 754 theoretical results.

**Table 3** Calculation results of SetAtomicMin for special value/boundary value inputs

<a name="table673793455714"></a>

| data type of src/dst | element value of src0 | element value of src1 | element value of dst<br>(register non-saturation mode) | element value of dst<br>(register saturation mode) |
| --- | --- | --- | --- | --- |
| half | -65504.0 (MIN) | -65504.0 (MIN)/65504.0 (MAX)/0.0/-0.0/inf | -65504.0 | -65504.0 |
| half | -65504.0 (MIN) | nan | nan | **0.0** |
| half | -65504.0 (MIN) | -inf | -inf | **-65504.0** |
| half | 65504.0 (MAX) | 65504.0 (MAX)/inf | 65504.0 | 65504.0 |
| half | 65504.0 (MAX) | 0.0 | 0.0 | 0.0 |
| half | 65504.0 (MAX) | -0.0 | -0.0 | -0.0 |
| half | 65504.0 (MAX) | nan | nan | **0.0** |
| half | 65504.0 (MAX) | -inf | -inf | **-65504.0** |
| half | 0.0 | 0.0/inf | 0.0 | 0.0 |
| half | 0.0 | -0.0 | -0.0 | -0.0 |
| half | 0.0 | nan | nan | **0.0** |
| half | 0.0 | -inf | -inf | **-65504.0** |
| half | -0.0 | -0.0/inf | -0.0 | -0.0 |
| half | -0.0 | nan | nan | 0.0 |
| half | -0.0 | -inf | -inf | **-65504.0** |
| half | nan | nan/inf/-inf | nan | **0.0** |
| half | inf | inf | inf | **65504.0** |
| half | inf | -inf | -inf | **-65504.0** |
| half | -inf | -inf | -inf | **-65504.0** |

Note: The results in the element value of dst (INF/NAN mode) column are consistent with the IEEE 754 theoretical results.
