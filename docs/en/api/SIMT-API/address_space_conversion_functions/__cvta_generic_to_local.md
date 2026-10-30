# \_\_cvta\_generic\_to\_local

<!-- md-trans-meta sourceCommit=343a6e89d2704a866eb5b4d939aacaa8ebe1820b translatedAt=2026-10-27T09:30:54.919Z pushedAt=2026-10-27T11:36:23.217Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/address_space_conversion_functions/__cvta_generic_to_local_res.md#id1 -->

## Description

The header file path is `"simt_api/device_functions.h"`.

Converts the input pointer to the value of the stack space address it points to and returns it.

## Prototype

```cpp
size_t __cvta_generic_to_local(const void* ptr)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| ptr | Input | Source operand. |

## Return Value

The address value of the stack space pointed to by the input pointer.
This interface does not verify whether the input address is a valid address that can be safely accessed. The return value is a valid stack space address only when `ptr` actually points to the stack space. Special scenarios are described as follows:

| Input Scenario | Return Value |
| --- | --- |
| `ptr` is a Global Memory pointer | Undefined behavior; the return value is not a valid stack space address. |
| `ptr` is `nullptr` | Returns `0x00000000fff00000`. |
| `ptr` is a UB pointer | A compilation error is reported. |

## Constraints

This interface is not supported in SIMD and SIMT hybrid programming scenarios.

## Examples

SIMT programming scenario:

```cpp
#include "simt_api/device_functions.h"

__global__ __launch_bounds__(1024) void kernel__cvta_generic_to_local(uint32_t* src)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    uint32_t local_data[2] = {src[idx], src[idx]};
    size_t addr_val = __cvta_generic_to_local(local_data);
    if (threadIdx.x < 4 && blockIdx.x == 4) {
        printf("threadId(%u) local_data stack address value is: 0x%lx\n", threadIdx.x, addr_val);
    }
}
```
