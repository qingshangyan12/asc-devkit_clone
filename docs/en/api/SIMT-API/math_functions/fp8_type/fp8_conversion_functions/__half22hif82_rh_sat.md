# \_\_half22hif82\_rh\_sat

<!-- md-trans-meta sourceCommit=343a6e89d2704a866eb5b4d939aacaa8ebe1820b translatedAt=2026-10-27T09:55:49.829Z pushedAt=2026-10-27T11:36:23.271Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/fp8_type/fp8_conversion_functions/__half22hif82_rh_sat_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_fp8.h"`.

In saturation mode, converts the two components of the **half2** type data to **hifloat8\_t** precision following the **CAST\_HYBRID** mode, and returns the converted **hifloat8x2\_t** type data.

## Prototype

```cpp
inline hifloat8x2_t __half22hif82_rh_sat(const half2 x)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns the hifloat8x2\_t type data converted from the two input components in saturation mode using the CAST\_HYBRID mode. The special values are as follows:

| x component value | non-saturation mode return value | saturation mode return value |
|---|---|---|
| ±0 | 0 | 0 |
| nan | nan | 0 |
| inf | inf | 32768 |
| -inf | -inf | -32768 |
| ASCRT_MAX_NORMAL_FP16 | inf | 32768 |
| -ASCRT_MAX_NORMAL_FP16 | -inf | -32768 |

## Constraints

Before using this interface, set the CTRL\[60\] register to 0; otherwise, the saturation mode does not take effect. For the setting method, see [Controlling the Saturation Behavior](../../data_type_conversion/overview.md#section1194916101549).

This API is not supported in the SIMT programming scenario.

## Examples

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_fp8.h"

    // Use short vectors to improve data transfer efficiency.
    __simt_vf__ __launch_bounds__(1024) inline void simt_half22hif82_rh_sat(__gm__ half2* input, __gm__ hifloat8x2_t* output, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one half2 value, that is, two half values, so threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        output[idx] = __half22hif82_rh_sat(input[idx]);
    }
    __global__ __vector__ void cast_kernel(__gm__ half* input,  __gm__ uint8_t* output, uint32_t input_total_length)
    {
        asc_vf_call<simt_half22hif82_rh_sat>(dim3(1024), (__gm__ half2*)input, (__gm__ hifloat8x2_t*)output, input_total_length);
    }
    ```
