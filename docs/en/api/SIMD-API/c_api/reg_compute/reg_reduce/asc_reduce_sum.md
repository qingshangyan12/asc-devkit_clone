# asc_reduce_sum

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.072441+00:00 pushedAt=2026-10-27T02:26:33.157Z nl=2 -->

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

Performs a reduction sum operation on the source operand `src` according to `mask`, summing all elements of `src` that participate in the computation to obtain the reduction result. The result is stored at position 0 of `dst`. Reference pseudocode:

```python
def asc_reduce_sum(dst, src, mask):
    values = [src[i] if mask[i] else 0 for i in range(len(src))]
    while len(values) > 1:
        values = [values[i] + values[i + 1]
                  for i in range(0, len(values), 2)]
    dst[0] = values[0]                  # Sum value.
    for i in range(1, len(dst)):
        dst[i] = 0                      # Set the remaining positions to 0.
```

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```c
// Return the result through the function return value (placeholder form).
__simd_callee__ inline vector_<dst_dtype> asc_reduce_sum(vector_<src_dtype> src,
                                                         vector_bool mask)

// Output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_reduce_sum(vector_<dst_dtype>& dst,
                                           vector_<src_dtype> src,
                                           vector_bool mask)
```

### Data Types Supported by src_dtype and dst_dtype

| `src_dtype` | `dst_dtype` |
| ----------- | ----------- |
| `int16_t` | `int32_t` |
| `uint16_t` | `uint32_t` |
| `half` | `half` |
| `int32_t` | `int32_t` |
| `uint32_t` | `uint32_t` |
| `float` | `float` |

### Typical Prototype Example

```c
// Example: Perform a reduction sum on a float vector data register.
__simd_callee__ inline void asc_reduce_sum(vector_float& dst,
                                           vector_float src,
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
- Returning the result through the function return value: returns the reduction result as a vector data register with the same data type as `dst_dtype`.

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The accumulation order within the instruction uses binary tree accumulation. Results are written consecutively to the destination operand, and the other elements in the destination operand are set to 0.
- When no element participates in the computation (`mask` is all 0), 0 is written to the corresponding position of the destination operand (specifically, +0 for floating-point numbers).
- For inputs of type `uint16_t`/`int16_t`, precision is promoted to `uint32_t`/`int32_t` for computation.

## Key Features

**asc_reduce_sum accumulation order**:

Computes the sum of valid elements in the source operand `src` using binary tree accumulation.

Take the summation of `half`-type data as an example. There are 128 numbers in `src`, which are added pairwise in a binary tree manner. The computation process is shown in the following figure:

1. data1 and data2 are added to obtain data01, data3 and data4 are added to obtain data02, ..., data125 and data126 are added to obtain data63, and data127 and data128 are added to obtain data64;
2. data01 and data02 are added to obtain data001, data03 and data04 are added to obtain data002, ..., and data63 and data64 are added to obtain data032;
3. By continuing this process, the destination operand is obtained as one `half`-type value, sum.

**Figure 1** reg_reduce_index

![reg_reduce_index](../../../../figures/reg_reduce_index.png "reg_reduce_index")

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [__NPU_ARCH__](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```c
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"
namespace {
template <typename T>
bool compare_data(const std::vector<T>& actual, const std::vector<T>& expected, double tolerance = 0.0)
{
    if (actual.size() != expected.size()) return false;
    for (size_t i = 0; i < actual.size(); ++i) {
        if (actual[i] == expected[i]) continue;
        const double diff = static_cast<double>(actual[i]) - static_cast<double>(expected[i]);
        if (diff > tolerance || diff < -tolerance) return false;
    }
    return true;
}

constexpr uint32_t ELEMENT_COUNT = 64;
__simd_vf__ inline void reduce(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src)
{
    vector_uint32_t dst_reg, src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);
    asc_reduce_sum(dst_reg, src_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}
__global__ __vector__ void asc_reduce_sum_kernel(__gm__ uint32_t* dst, __gm__ uint32_t* src)
{
    asc_init();
    __ubuf__ uint32_t dst_local[ELEMENT_COUNT], src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    reduce(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync();
}
} // namespace
int main()
{
    std::vector<uint32_t> src(ELEMENT_COUNT), output(ELEMENT_COUNT, 0), golden(ELEMENT_COUNT, 0);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) src[i] = i + 1;
    golden[0] = ELEMENT_COUNT * (ELEMENT_COUNT + 1) / 2;
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t* src_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), (ELEMENT_COUNT) * sizeof(uint32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint32_t* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), (ELEMENT_COUNT) * sizeof(uint32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(uint32_t), src.data(), src.size() * sizeof(uint32_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_reduce_sum_kernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint32_t), dst_device, output.size() * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    const bool passed = compare_data(output, golden);
    std::cout << (passed ? "[Success] asc_reduce_sum passed."
                         : "[Failed] asc_reduce_sum failed.")
              << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
