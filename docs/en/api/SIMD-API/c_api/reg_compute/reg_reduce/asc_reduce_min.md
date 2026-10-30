# asc_reduce_min

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.069441+00:00 pushedAt=2026-10-27T02:26:33.145Z nl=2 -->

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

The header file path is `"c_api/reg_compute/compute/reg_reduce.h"`.

Performs a reduction minimum operation on the source operand `src` according to `mask` to obtain the reduction result. The result is stored in element 0 of `dst`, the original bit pattern of the index of the minimum value in `src` is stored in element 1 of `dst`, and the other elements in `dst` are set to 0. If there are multiple minimum values, the smallest index is retained. The calculation formula is as follows:

$$
\begin{aligned}
dst_0 &= \min\{src_i \mid mask_i = 1\} \\
dst_1 &= \operatorname{argmin}\{src_i \mid mask_i = 1\}
\end{aligned}
$$

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```c
// Return the result through the function return value (placeholder form).
__simd_callee__ inline vector_<dtype> asc_reduce_min(vector_<dtype> src,
                                                     vector_bool mask)

// Output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_reduce_min(vector_<dtype>& dst,
                                           vector_<dtype> src,
                                           vector_bool mask)
```

### Data Types Supported by dtype

`dtype` values are `int16_t`, `uint16_t`, `half`, `int32_t`, `uint32_t`, and `float`.

### Typical Prototype Example

```c
// Example: Perform a reduction minimum operation on a half vector data register.
__simd_callee__ inline void asc_reduce_min(vector_half& dst,
                                           vector_half src,
                                           vector_bool mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| ------ | --------- | ---------------------------------------- |
| dst | Output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Mask register, used to control whether each element participates in the reduction. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

- Outputting the result through a reference parameter: the reduction result is written to `dst`.
- Returning the result through the function return value: the reduction result is returned, and its type is a vector data register consistent with the data type of `src`.

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- Elements not selected by `mask` are treated as the maximum value of the corresponding data type, and the maximum value of a floating-point type is `+inf`. If none of the elements in `src` are selected by `mask`, this maximum value is written to element 0 of `dst`, and the remaining elements are set to 0.
- The comparison follows $min(-0, +0) = -0$.
- If the input data contains nan, nan is written to element 0 of `dst`, and the index of the first nan is written to element 1 of `dst`.

## Key Features

**The reduction produces two results**: value and index. The index value requires a forced type conversion:

The index of `dst` is stored according to the data type of `dst`. For example, when `dst` is of the half type, the index is stored as the half type. Therefore, to read the index, you need to use the reinterpret\_cast method to convert it to an integer type. If the data type is half, use reinterpret\_cast\<uint16_t*\>; if the data type is float, use reinterpret\_cast\<uint32_t*\>. For how to extract the value and index separately, see [Examples](#examples).

## Examples

Save the following code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [__NPU_ARCH__](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
// This example uses the half type to demonstrate how to extract the "value + index" from the reduction result separately.
// In the reduction result, element 0 of dst is the minimum value, and element 1 is the original bit pattern of the minimum value index.
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"
namespace {
constexpr uint32_t ELEMENT_COUNT = 64;
__simd_vf__ inline void reduce(__ubuf__ half* dst, __ubuf__ half* src)
{
    vector_half dst_reg, src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b16(count);
    asc_loadalign(src_reg, src);
    asc_reduce_min(dst_reg, src_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}
__global__ __vector__ void asc_reduce_min_kernel(__gm__ half* dst, __gm__ half* src)
{
    asc_init();
    __ubuf__ half dst_local[ELEMENT_COUNT], src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(half));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    reduce(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(half));
    asc_sync();
}
} // namespace
int main()
{
    std::vector<half> src(ELEMENT_COUNT), output(ELEMENT_COUNT, half(0));
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        src[i] = half(static_cast<float>(i) + 1.0f);  // 1, 2, ..., 64
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    half* src_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), (ELEMENT_COUNT) * sizeof(half),
        ACL_MEM_MALLOC_HUGE_FIRST);
    half* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), (ELEMENT_COUNT) * sizeof(half),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(half), src.data(), src.size() * sizeof(half),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_reduce_min_kernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(half), dst_device, output.size() * sizeof(half),
        ACL_MEMCPY_DEVICE_TO_HOST);
    half min_value = output[0];                                      // Extract the minimum value.
    uint16_t min_index = *reinterpret_cast<uint16_t*>(&output[1]);   // Extract the minimum value index: the index is stored as the half type and must be forcibly converted to an integer type.
    const bool passed = (static_cast<float>(min_value) == 1.0f) && (min_index == 0);
    std::cout << (passed ? "[Success] asc_reduce_min passed." : "[Failed] asc_reduce_min failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
