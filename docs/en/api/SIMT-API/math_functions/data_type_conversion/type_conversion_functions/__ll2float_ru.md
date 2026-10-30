# \_\_ll2float\_ru

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-20T03:43:09.046Z pushedAt=2026-10-20T06:17:01.347Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/data_type_conversion/type_conversion_functions/__ll2float_ru_res.md#id1 -->

## Description

The header file path is `"simt_api/device_functions.h"`.

Converts **int64** data to a floating-point number following the CAST\_CEIL mode, and returns the converted value.

## Prototype

```cpp
inline float __ll2float_ru(const long long int x)
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns the floating-point number obtained by converting the input following the CAST\_CEIL mode. Special values are as follows:

| Input | Return Value |
|---|---|
| 9223372036854775807(INT64_MAX) | 9.22337204e+18 |
| -9223372036854775808(INT64_MIN) | -9.22337204e+18 |
| 16777217(2^24+1 ) | 16777218 |
| -16777217(-2^24-1) | -ASCRT_TWO_TO_24_F |

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/device_functions.h"

    __global__ __launch_bounds__(1024) void kernel__ll2float_ru(float* dst, int64_t* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __ll2float_ru(x[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/device_functions.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel__ll2float_ru(__gm__ float* dst, __gm__ int64_t* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __ll2float_ru(x[idx]);
    }
    ```
