# nanf

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-20T09:35:52.505Z pushedAt=2026-10-20T10:26:42.254Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/float_math_functions/nanf_res.md#id1 -->

## Description

The header file path is `"simt_api/math_functions.h"`.

Generates a float-type quiet NaN value (with the most significant bit of the mantissa set to 1) based on the input string, and writes the lower 22 bits of the parsed value of `tagp` into the lower 22 bits of the NaN value.

![](../../../figures/nanf.png)

## Prototype

```cpp
inline float nanf(const char* tagp)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| tagp | Input | First address of the source operand string. Decimal, octal, and hexadecimal formats are supported. If the string cannot be parsed in any supported base, it is treated as 0. |

## Return Value

Returns a float-type quiet NaN value. The lower 22 bits of the return value are the lower 22 bits of the parsed value of `tagp`. If the `tagp` string cannot be parsed in any supported base, the lower 22 bits are 0. If the parsed value of `tagp` overflows, the lower 22 bits are all ones.

## Constraints

None

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __global__ __launch_bounds__(1024) void kernel_nanf(float* dst)
    {
        dst[0] = nanf("0x1A");
        dst[1] = nanf("123");
        dst[2] = nanf("abc");
        dst[3] = nanf("0");
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel_nanf(__gm__ float* dst)
    {
        dst[0] = nanf("0x1A");
        dst[1] = nanf("123");
        dst[2] = nanf("abc");
        dst[3] = nanf("0");
    }
    ```

The input and output examples are as follows:

```text
tagp: "0x1A", "123", "abc", "0"
result: nan(0x7fc0001a) nan(0x7fc0007b) nan(0x7fc00000) nan(0x7fc00000)
```
