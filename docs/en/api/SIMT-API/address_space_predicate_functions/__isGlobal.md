# \_\_isGlobal

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T08:42:57.527Z pushedAt=2026-10-16T10:10:03.745Z -->

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
- Atlas 200I/500 A2 inference product: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products AI Core: Not supported
- Atlas inference products Vector Core: Not supported
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas training products: Not supported
<!-- end id6 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/address_space_predicate_functions/__isGlobal_res.md#id1 -->

## Description

The header file path is: `"simt_api/device_functions.h"`.

Determines whether the input pointer points to an address in the Global Memory address space.

## Prototype

```cpp
unsigned int __isGlobal(const void* ptr)
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| ptr | Input | Source operand. |

## Return Value

If the input pointer points to an address in the Global Memory address space, the function returns 1; otherwise, it returns 0.  
This API classifies the input pointer based on its address space information and does not verify whether the pointer is a valid address that can be safely accessed. A return value of 1 from `__isGlobal()` only indicates that the pointer is classified as a Global Memory address, and does not necessarily mean that the address can be safely accessed. Special scenarios are described as follows:  

| Input Scenario | Return Value |
| --- | --- |
| `ptr` is a valid UB pointer | 0 |
| `ptr` is a valid stack pointer | 0 |
| `ptr` is `nullptr` | 0 |
| `ptr` is `(void*)0x1` | 0 |
| `ptr` is an all-ones address | 1 |
| `ptr` is returned by `__cvta_global_to_generic(0)` or `__cvta_global_to_generic(1)` | 0 |
| `ptr` is returned by `__cvta_global_to_generic(all-ones)` | 1 |

Therefore, `__isGlobal(ptr) == 1` alone cannot be used to determine whether `ptr` is a Global Memory address that can be safely accessed.

## Constraints

This API is not supported in mixed SIMD and SIMT programming scenarios.

## Examples

SIMT programming scenario:

```cpp
#include "simt_api/device_functions.h"

__global__ __launch_bounds__(1024) void kernel_isGlobal(uint32_t* dst, uint32_t* src)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    dst[idx] = __isGlobal(src + idx);
}
```
