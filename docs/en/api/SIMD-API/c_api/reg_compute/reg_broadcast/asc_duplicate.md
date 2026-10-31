# asc_duplicate

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:33.854569+00:00 pushedAt=2026-10-27T02:26:32.263Z nl=2 -->

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

Broadcasts the lowest element of source operand src (that is, the element with subscript 0) to the positions in destination operand dst selected by mask. mask only selects the element positions in dst and does not affect the element read from src; the positions in dst not selected by mask are set to zero.

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```c
// Return the result through the function return value (placeholder form).
__simd_callee__ inline vector_<dtype> asc_duplicate(vector_<dtype> src,
                                                    vector_bool mask)

// Output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_duplicate(vector_<dtype>& dst,
                                          vector_<dtype> src,
                                          vector_bool mask)
```

### Data Types Supported by dtype

`dtype` values are: `int8_t`, `uint8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

### Typical Prototype Example

```c
// Return the result through the function return value.
__simd_callee__ inline vector_int8_t asc_duplicate(vector_int8_t src,
                                                   vector_bool mask)
// Output the result through a reference parameter.
__simd_callee__ inline void asc_duplicate(vector_int8_t& dst,
                                          vector_int8_t src,
                                          vector_bool mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operand (vector data register), which stores the broadcast result. The data type must be consistent with src. Only the no-return-value API contains this parameter. |
| src | Input | Source operand (vector data register), whose lowest element is the data to be broadcast. The data type must be consistent with dst. |
| mask | Input | Effective indication of the source operand element operation (mask register). The elements selected by mask are filled in dst with the lowest element of src, and the unselected elements are set to zero in dst. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

For the return-value API, the vector data register that stores the broadcast result is returned, and its data type is consistent with src.

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The hardware preserves the order of data dependencies involving the same register, so no additional synchronization instructions are required. If this API and preceding or subsequent Reg data movement APIs use different registers to access the same UB address and have read-after-write or write-after-write dependencies, call [asc_mem_bar](../reg_sync/asc_mem_bar.md) for synchronization.
- mask only selects the positions in dst where the broadcast value is written and does not select elements in src. Regardless of whether the lowest bit of mask is valid, the data to be broadcast is always the lowest element of src.

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

__simd_vf__ inline void duplicate(__ubuf__ float* dst, __ubuf__ float* src)
{
    vector_float src_reg;
    uint32_t active_count = ACTIVE_COUNT;
    uint32_t full_count = ELEMENT_COUNT;
    vector_bool active_mask = asc_update_mask_b32(active_count);
    vector_bool full_mask = asc_update_mask_b32(full_count);

    asc_loadalign(src_reg, src);
    vector_float dst_reg = asc_duplicate(src_reg, active_mask);
    asc_storealign(dst, dst_reg, full_mask);
}

__global__ __vector__ void asc_duplicate_kernel(__gm__ float* dst, __gm__ float* src)
{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    __ubuf__ float src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, sizeof(src_local));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    duplicate(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, sizeof(dst_local));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> src(ELEMENT_COUNT);
    std::vector<float> output(ELEMENT_COUNT);
    std::vector<float> golden(ELEMENT_COUNT, 0.0f);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) src[i] = static_cast<float>(i + 1);
    for (uint32_t i = 0; i < ACTIVE_COUNT; ++i) golden[i] = src.front();

    aclInit(nullptr);
    aclrtSetDevice(0);
    float *src_device = nullptr, *dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), src.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(float), src.data(), src.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_duplicate_kernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), dst_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_duplicate passed."
                        : "[Failed] asc_duplicate failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
<!-- end id8 -->
