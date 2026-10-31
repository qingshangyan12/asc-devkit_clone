# \_\_float2bfloat16\_rz\_sat

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T12:49:22.000Z pushedAt=2026-10-20T03:02:40.412Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/bfloat16_type/bfloat16_precision_conversion_functions/__float2bfloat16_rz_sat_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_bf16.h"`.

In saturation mode, converts a floating-point number to bfloat16 precision following the CAST\_TRUNC mode and returns the converted value.

## Prototype

```cpp
inline bfloat16_t __float2bfloat16_rz_sat(const float x)
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns bfloat16 data converted from the input following the CAST\_TRUNC mode in saturation mode. The special values are as follows:

| Input | non-saturation mode return value | saturation mode return value |
|---|---|---|
| ±0 | ±0 | ±0 |
| nan | nan | 0 |
| inf | inf | ASCRT_MAX_NORMAL_BF16 |
| -inf | -inf | -ASCRT_MAX_NORMAL_BF16 |
| >ASCRT_MAX_NORMAL_BF16 | ASCRT_MAX_NORMAL_BF16 | ASCRT_MAX_NORMAL_BF16 |
| <-ASCRT_MAX_NORMAL_BF16 | -ASCRT_MAX_NORMAL_BF16 | -ASCRT_MAX_NORMAL_BF16 |

## Constraints

Before using this API, set the CTRL\[60\] register to 0; otherwise, the saturation mode does not take effect. For the setting method, see [Controlling Saturation Behavior](../../data_type_conversion/overview.md#section1194916101549).

In SIMT programming scenarios, the saturation mode of this API does not take effect because the CTRL register cannot be set.

## Examples

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel__float2bfloat16_rz_sat(__gm__ bfloat16_t* dst, __gm__ float* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __float2bfloat16_rz_sat(x[idx]);
    }
    ```
