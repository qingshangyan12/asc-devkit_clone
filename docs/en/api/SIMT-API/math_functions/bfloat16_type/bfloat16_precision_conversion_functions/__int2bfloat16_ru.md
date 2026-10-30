# \_\_int2bfloat16\_ru

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T13:00:35.895Z pushedAt=2026-10-20T03:02:40.437Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/bfloat16_type/bfloat16_precision_conversion_functions/__int2bfloat16_ru_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_bf16.h"`.

In the CAST\_CEIL mode, converts int32 type data to bfloat16 type data and returns the converted value.

## Prototype

```cpp
inline bfloat16_t __int2bfloat16_ru(const int x)
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns bfloat16 data converted from the input in the CAST\_CEIL mode. The special values are as follows:

| x Value | Return Value |
|---|---|
| 0 | 0 |
| 257 | 258 |
| -257 | -256 |
| 513 | 516 |
| -513 | -512 |
| 2147483647 (INT32_MAX) | 2.14748e+09 |
| -2147483648 (INT32_MIN) | -2.14748e+09 |

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __global__ __launch_bounds__(1024) void kernel__int2bfloat16_ru(bfloat16_t* dst, int32_t* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __int2bfloat16_ru(x[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel__int2bfloat16_ru(__gm__ bfloat16_t* dst, __gm__ int32_t* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __int2bfloat16_ru(x[idx]);
    }
    ```
