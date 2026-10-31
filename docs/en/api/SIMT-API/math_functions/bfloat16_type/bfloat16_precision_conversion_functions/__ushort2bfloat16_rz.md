# \_\_ushort2bfloat16\_rz

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T13:21:33.027Z pushedAt=2026-10-20T03:02:40.489Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/bfloat16_type/bfloat16_precision_conversion_functions/__ushort2bfloat16_rz_res.md#id1 -->

## Description

The header file path is: `"simt_api/asc_bf16.h"`.

Follows the CAST\_TRUNC mode to convert `unsigned short int` type data into `bfloat16_t` type data, and returns the converted value.

## Prototype

```cpp
inline bfloat16_t __ushort2bfloat16_rz(const unsigned short int x)
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns bfloat16_t data converted from the input following the CAST\_TRUNC mode. The special values are as follows:

| Input | Return Value |
|---|---|
| 0 | 0 |
| 2049 | 2048 |
| 4097 | 4096 |
| 65505 | 65280 |
| 65535 (maximum value of unsigned short int) | 65280 |

## Constraints

None

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __global__ __launch_bounds__(1024) void kernel___ushort2bfloat16_rz(bfloat16_t* dst, unsigned short int* x, uint32_t input_total_length)
    {
        uint32_t idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= input_total_length) {
            return;
        }
        dst[idx] = __ushort2bfloat16_rz(x[idx]);
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel___ushort2bfloat16_rz(__gm__ bfloat16_t* dst, __gm__ unsigned short int* x, uint32_t input_total_length)
    {
        uint32_t idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= input_total_length) {
            return;
        }
        dst[idx] = __ushort2bfloat16_rz(x[idx]);
    }
    ```
