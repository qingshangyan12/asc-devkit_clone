# min

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T07:12:14.132Z pushedAt=2026-10-22T07:20:10.064Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/integer_math_functions/min_res.md#id1 -->

## Description

The header file path is `"simt_api/math_functions.h"`.

Obtains the minimum of two input values. When one input is a signed integer and the other is an unsigned integer, the signed integer is first converted to the corresponding unsigned type before the comparison.

## Prototype

```cpp
long long min(long long x, long long y)
```

```cpp
long min(long x, long y)
```

```cpp
int min(int x, int y)
```

```cpp
short min(short x, short y)
```

```cpp
char min(char x, char y)
```

```cpp
unsigned long long min(unsigned long long x, unsigned long long y)
```

```cpp
unsigned long min(unsigned long x, unsigned long y)
```

```cpp
unsigned int min(unsigned int x, unsigned int y)
```

```cpp
unsigned short min(unsigned short x, unsigned short y)
```

```cpp
unsigned char min(unsigned char x, unsigned char y)
```

```cpp
unsigned int min(unsigned int x, int y)
```

```cpp
unsigned int min(int x, unsigned int y)
```

```cpp
unsigned long int min(long int x, unsigned long int y)
```

```cpp
unsigned long int min(unsigned long int x, long int y)
```

```cpp
unsigned long long int min(long long int x, unsigned long long int y)
```

```cpp
unsigned long long int min(unsigned long long int x, long long int y)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |
| y | Input | Source operand. |

## Return Value

Returns the minimum value of the input data.

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __global__ __launch_bounds__(1024) void KernelMin(long long* dst, long long* x, long long* y, uint32_t input_total_length)
    {
        uint32_t idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= input_total_length) {
            return;
        }
        dst[idx] = min(x[idx], y[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __simt_vf__ __launch_bounds__(1024) inline void KernelMin(__gm__ long long* dst, __gm__ long long* x, __gm__ long long* y, uint32_t input_total_length)
    {
        uint32_t idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= input_total_length) {
            return;
        }
        dst[idx] = min(x[idx], y[idx]);
    }
    ```
