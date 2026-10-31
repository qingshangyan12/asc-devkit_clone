# \_\_float22hif82\_rna\_sat

<!-- md-trans-meta sourceCommit=343a6e89d2704a866eb5b4d939aacaa8ebe1820b translatedAt=2026-10-27T09:53:10.546Z pushedAt=2026-10-27T11:36:23.268Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products AI Core: Not supported
- Atlas inference products Vector Core: Not supported
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas training products: Not supported
<!-- end id6 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/fp8_type/fp8_conversion_functions/__float22hif82_rna_sat_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_fp8.h"`.

In saturation mode, converts the two components of **float2** type data to **hifloat8\_t** precision following the CAST\_ROUND mode, and returns the converted **hifloat8x2\_t** type data.

## Prototype

```cpp
inline hifloat8x2_t __float22hif82_rna_sat(const float2 x)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns the CAST\_ROUND mode-converted hifloat8x2\_t type data from the two input components in saturation mode. Special values are as follows:

| x value | non-saturation mode return value | saturation mode return value |
|---|---|---|
| ±0 | 0 | 0 |
| nan | nan | 0 |
| inf | inf | 32768 |
| -inf | -inf | -32768 |
| Positive value beyond the hif8 range | inf | 32768 |
| Negative value beyond the hif8 range | -inf | -32768 |

## Constraints

Before using this API, set the CTRL\[60\] register to 0; otherwise, the saturation mode does not take effect. For the setting method, see [Controlling the Saturation Behavior](../../data_type_conversion/overview.md#section1194916101549).

This API is not supported in the SIMT programming scenario.

## Examples

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_fp8.h"

    // Use short vectors to improve data movement efficiency.
    __simt_vf__ __launch_bounds__(1024) inline void simt_float22hif82_rna_sat(__gm__ float2* input, __gm__ hifloat8x2_t* output, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one float2 value, that is, two float values. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        output[idx] = __float22hif82_rna_sat(input[idx]);
    }
    __global__ __vector__ void cast_kernel(__gm__ float* input, __gm__ uint8_t* output, uint32_t input_total_length)
    {
        asc_vf_call<simt_float22hif82_rna_sat>(dim3(1024), (__gm__ float2*)input, (__gm__ hifloat8x2_t*)output, input_total_length);
    }
    ```
