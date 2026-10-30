# \_\_half\_as\_ushort

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-21T07:11:11.914Z pushedAt=2026-10-21T07:23:34.159Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/half_type/half_precision_conversion_functions/__half_as_ushort_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_fp16.h"`.

Reinterprets the bits of half type data as unsigned short int type data.

## Prototype

```cpp
inline unsigned short int __half_as_ushort(const half x)
```

## Parameters

**Table 1**  Parameters

| parameter name | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

The unsigned short int data obtained by reinterpreting the bits of the input half type data. Special values are as follows:

| x Value | Return Value |
| --- | --- |
| 0 | 0 |
| nan | 32256 |
| inf | 31744 |
| -inf | 64512 |

## Constraints

None

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    __global__ __launch_bounds__(1024) void kernel___half_as_ushort(unsigned short int* dst, half* x, uint32_t input_total_length)
    {
        uint32_t idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= input_total_length) {
            return;
        }
        dst[idx] = __half_as_ushort(x[idx]);
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel___half_as_ushort(__gm__ unsigned short int* dst, __gm__ half* x, uint32_t input_total_length)
    {
        uint32_t idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= input_total_length) {
            return;
        }
        dst[idx] = __half_as_ushort(x[idx]);
    }
    ```
