# \_\_int2half\_ru\_sat

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-21T08:08:39.261Z pushedAt=2026-10-22T00:40:58.022Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/half_type/half_precision_conversion_functions/__int2half_ru_sat_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_fp16.h"`.

Converts int32 data into half data following the CAST\_CEIL mode, and returns the converted value.

## Prototype

```cpp
inline half __int2half_ru_sat(const int x)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns half data converted from the input int32 data following the CAST\_CEIL mode. Special values are as follows:

| x Value | Return Value |
| --- | --- |
| 0 | 0 |
| x>ASCRT\_MAX\_NORMAL\_FP16 | ASCRT\_MAX\_NORMAL\_FP16 |
| x<-ASCRT\_MAX\_NORMAL\_FP16 | -ASCRT\_MAX\_NORMAL\_FP16 |
| 2049 | 2050 |
| -2049 | -2048 |
| 4097 | 4100 |
| -4097 | -4096 |

## Constraints

Before using this API, set the CTRL[60] register to 0; otherwise, the saturation mode does not take effect. For the setting method, see [Controlling the Saturation Behavior](../../data_type_conversion/overview.md#section1194916101549).

In SIMT programming scenarios, the CTRL register cannot be set, so the saturation mode of this API does not take effect.

## Examples

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel__int2half_ru_sat(__gm__ half* dst, __gm__ int32_t* x, uint32_t total_length)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= total_length) {
            return;
        }
        dst[idx] = __int2half_ru_sat(x[idx]);
    }
    ```
