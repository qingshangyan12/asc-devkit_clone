# asc_duplicate_scalar

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:33.861080+00:00 pushedAt=2026-10-27T02:26:32.290Z nl=2 -->

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
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/reg_compute/compute/reg_broadcast.h"`.

Broadcasts the scalar `value` to the destination operand. Two API forms are supported: outputting the result through a reference parameter and returning the result through the function return value. Each form supports the following two overloads:

- With `mask`: Writes `value` to the positions in `dst` selected by `mask`, and sets the positions not selected by `mask` to 0.
- Without `mask`: Writes `value` to all element positions of `dst`.

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

### With mask

```cpp
// Return the result through the function return value (placeholder form).
__simd_callee__ inline vector_<dtype> asc_duplicate_scalar(<dtype> value,
                                                           vector_bool mask)

// Output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_duplicate_scalar(vector_<dtype>& dst,
                                                 <dtype> value,
                                                 vector_bool mask)
```

**Placeholder description:**

- `<dtype>` values are: `int8_t`, `uint8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

#### Typical Prototype Example

```cpp
// Return the result through the function return value.
__simd_callee__ inline vector_int8_t asc_duplicate_scalar(int8_t value,
                                                          vector_bool mask)
// Output the result through a reference parameter.
__simd_callee__ inline void asc_duplicate_scalar(vector_int8_t& dst,
                                                 int8_t value,
                                                 vector_bool mask)
```

### Without mask

```cpp
// Return the result through the function return value (placeholder form).
__simd_callee__ inline vector_<dtype> asc_duplicate_scalar(<dtype> value)

// Output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_duplicate_scalar(vector_<dtype>& dst,
                                                 <dtype> value)
```

**Placeholder description:**

- `<dtype>` values are: `int8_t`, `uint8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

#### Typical Prototype Example

```cpp
// Return the result through the function return value.
__simd_callee__ inline vector_int8_t asc_duplicate_scalar(int8_t value)

// Output the result through a reference parameter.
__simd_callee__ inline void asc_duplicate_scalar(vector_int8_t& dst,
                                                 int8_t value)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operand (vector data register), which stores the broadcast result. Only the API that outputs the result through a reference parameter includes this parameter. The element data type of `dst` is used to determine the function overload. |
| value | Input | Source operand (scalar), which is the data to be broadcast. For the API with a return value, the data type of `value` is used to determine the function overload and return type. It is recommended to pass a variable with an explicit type or use an explicit type conversion. |
| mask | Input | Effective indication of element operations on the destination operand (mask register). Only the overload with `mask` includes this parameter. Elements selected by `mask` are filled with `value` in `dst`, and unselected elements are set to 0 in `dst`. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

- The function prototype that outputs the result through a reference parameter has no return value, and the broadcast result is written to `dst`.
- The function prototype that returns the result through the function return value returns the broadcast result, with a return type of `vector_<dtype>` corresponding to `dtype`.

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The hardware preserves the order of data dependencies involving the same register, so no additional synchronization instructions are required. If this API and preceding or subsequent Reg data movement APIs use different registers to access the same UB address and have read-after-write or write-after-write dependencies, call [asc_mem_bar](../reg_sync/asc_mem_bar.md) for synchronization.
- When calling the API with a return value, use a variable with an explicit type or an explicit type conversion, such as `static_cast<uint8_t>(1)`, to match the correct function prototype.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

The following example code is supported only on Ascend 950PR&950DT products.

```cpp
#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr uint32_t ACTIVE_COUNT = 4;
constexpr float FILL_VALUE = 3.5f;

__simd_vf__ inline void duplicate_scalar(__ubuf__ float* dst)
{
    uint32_t active_count = ACTIVE_COUNT;
    uint32_t full_count = ELEMENT_COUNT;
    vector_bool active_mask = asc_update_mask_b32(active_count);
    vector_bool full_mask = asc_update_mask_b32(full_count);
    vector_float dst_reg;
    asc_duplicate_scalar(dst_reg, FILL_VALUE, active_mask);
    asc_storealign(dst, dst_reg, full_mask);
}

__global__ __vector__ void asc_duplicate_scalar_kernel(__gm__ float* dst)
{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    duplicate_scalar(dst_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, sizeof(dst_local));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> output(ELEMENT_COUNT);
    std::vector<float> golden(ELEMENT_COUNT, 0.0f);
    for (uint32_t i = 0; i < ACTIVE_COUNT; ++i) golden[i] = FILL_VALUE;

    aclInit(nullptr);
    aclrtSetDevice(0);
    float* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    asc_duplicate_scalar_kernel<<<1, 0>>>(dst_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), dst_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_duplicate_scalar passed."
                        : "[Failed] asc_duplicate_scalar failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
<!-- end id8 -->
