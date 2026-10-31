# asc_duplicate_scalar_merge

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:33.862942+00:00 pushedAt=2026-10-27T02:26:32.296Z nl=2 -->

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

Broadcasts the scalar `value` to the positions in the destination operand `dst` selected by `mask`, while the elements in `dst` not selected by `mask` retain their original values.

This API uses the same broadcast data source as [asc_duplicate_scalar](asc_duplicate_scalar.md), but differs in that it uses merge mode to preserve the elements in `dst` that do not participate in the computation.

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```cpp
// Placeholder form.
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_<dtype>& dst,
                                                       <dtype> value,
                                                       vector_bool mask)
```

**Placeholder description:**

- `<dtype>` values are: `int8_t`, `uint8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

### Typical Prototype Example

```cpp
__simd_callee__ inline void asc_duplicate_scalar_merge(vector_float& dst,
                                                       float value,
                                                       vector_bool mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Input/output | Destination operand (vector data register). Elements selected by `mask` are written with `value`, and unselected elements retain the values from before the API call. The element data type must be the same as that of `value`. |
| value | Input | Source operand (scalar), which is the data to be broadcast. Its data type must be the same as that of the elements in `dst`. |
| mask | Input | Effective indication of element operations on the destination operand (mask register). When the corresponding bit is 1, the element in `dst` is written with `value`; when it is 0, the element in `dst` retains its original value. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- Initialize `dst` before calling the API; otherwise, the values of elements not selected by `mask` are undefined.
- The hardware preserves the order of data dependencies involving the same register, so no additional synchronization instructions are required. If this API and preceding or subsequent Reg data movement APIs use different registers to access the same UB address and have read-after-write or write-after-write dependencies, call [asc_mem_bar](../reg_sync/asc_mem_bar.md) for synchronization.

## Examples

Save the following code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [__NPU_ARCH__](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

The following example broadcasts the scalar `1.0f` to the first 4 elements of `dst`, while the remaining elements not selected by `mask` retain their original values.

```cpp
#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr uint32_t ACTIVE_COUNT = 4;
constexpr float INITIAL_VALUE = -1.0f;
constexpr float FILL_VALUE = 1.0f;

__simd_vf__ inline void duplicate_scalar_merge(__ubuf__ float* dst)
{
    vector_float dst_reg;
    uint32_t active_count = ACTIVE_COUNT;
    uint32_t full_count = ELEMENT_COUNT;
    vector_bool active_mask = asc_update_mask_b32(active_count);
    vector_bool full_mask = asc_update_mask_b32(full_count);

    asc_loadalign(dst_reg, dst);
    asc_duplicate_scalar_merge(dst_reg, FILL_VALUE, active_mask);
    asc_storealign(dst, dst_reg, full_mask);
}

__global__ __vector__ void asc_duplicate_scalar_merge_kernel(__gm__ float* dst)
{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(dst_local, dst, sizeof(dst_local));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    duplicate_scalar_merge(dst_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, sizeof(dst_local));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> output(ELEMENT_COUNT, INITIAL_VALUE);
    std::vector<float> golden(ELEMENT_COUNT, INITIAL_VALUE);
    for (uint32_t i = 0; i < ACTIVE_COUNT; ++i) golden[i] = FILL_VALUE;

    aclInit(nullptr);
    aclrtSetDevice(0);
    float* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(dst_device, output.size() * sizeof(float), output.data(), output.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_duplicate_scalar_merge_kernel<<<1, 0>>>(dst_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), dst_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_duplicate_scalar_merge passed."
                        : "[Failed] asc_duplicate_scalar_merge failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
