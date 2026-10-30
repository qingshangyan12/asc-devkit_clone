# asc_mull

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-27T01:34:54.755365+00:00 pushedAt=2026-10-27T02:26:32.227Z nl=2 -->

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

The header file path is `"c_api/reg_compute/compute/reg_arith.h"`.

Multiplies the valid elements in two source vector data registers based on the mask. Elements with a data type bit width of 32 bits are multiplied to obtain a 64-bit product, which is split into the low 32 bits and high 32 bits and written to two destination vector data registers respectively. Elements with a mask value of 1 at the corresponding position participate in the computation, and elements with a mask value of 0 are set to zero in the destination vector data registers. The calculation formula is as follows:

$$
dst0_i = (src0_i \times src1_i) \& ((1 \ll bit) - 1)
$$

$$
dst1_i = (src0_i \times src1_i) \gg bit
$$

Here, `bit` indicates the data type bit width of `src0` and `src1`.

**Figure 1** asc_mull computation diagram

![](../../../../figures/asc_mull.png)

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```c
// Placeholder form.
__simd_callee__ inline void asc_mull(vector_<dtype>& dst0,
                                     vector_<dtype>& dst1,
                                     vector_<dtype> src0,
                                     vector_<dtype> src1,
                                     vector_bool mask)
```

### Data Types Supported by dtype

`dtype` values: `int32_t`, `uint32_t`.

### Typical Prototype Example

```c
__simd_callee__ inline void asc_mull(vector_uint32_t& dst0,
                                     vector_uint32_t& dst1,
                                     vector_uint32_t src0,
                                     vector_uint32_t src1,
                                     vector_bool mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst0 | Output | Destination operand (vector data register). The data type must be the same as `src0`. |
| dst1 | Output | Destination operand (vector data register). The data type must be the same as `src0`. |
| src0 | Input | Source operand (vector data register). |
| src1 | Input | Source operand (vector data register). The data type must be the same as `src0`. |
| mask | Input | Source operand mask (mask register), used to indicate the elements that participate in the computation. A value of 1 at the corresponding position indicates participation, and 0 indicates non-participation, with the corresponding elements of `dst0` and `dst1` set to zero. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- Element positions with a mask bit of 0 do not participate in the multiplication operation, and the corresponding positions of `dst0` and `dst1` are written with 0.

### Computation Constraints

- `dst0` and `dst1` must be different vector data registers; otherwise, undefined behavior occurs.
- The source operands and destination operands can be the same vector data registers, for example, `asc_mull(src0, src1, src0, src1, mask);`.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr uint32_t BUFFER_BYTES = ELEMENT_COUNT * sizeof(uint32_t);

__simd_vf__ inline void mull_vf(__ubuf__ uint32_t* output_low,
                                __ubuf__ uint32_t* output_high,
                                __ubuf__ uint32_t* input0,
                                __ubuf__ uint32_t* input1)
{
    vector_uint32_t src0;
    vector_uint32_t src1;
    vector_uint32_t dst0;
    vector_uint32_t dst1;
    vector_bool mask = asc_create_mask_b32(PAT_ALL);
    asc_loadalign(src0, input0);
    asc_loadalign(src1, input1);
    asc_mull(dst0, dst1, src0, src1, mask);
    asc_storealign(output_low, dst0, mask);
    asc_storealign(output_high, dst1, mask);
}

__global__ __vector__ void asc_mull_kernel(__gm__ uint32_t* output_low,
                                           __gm__ uint32_t* output_high,
                                           __gm__ uint32_t* input0,
                                           __gm__ uint32_t* input1)
{
    asc_init();
    __ubuf__ uint32_t output_low_local[ELEMENT_COUNT];
    __ubuf__ uint32_t output_high_local[ELEMENT_COUNT];
    __ubuf__ uint32_t input0_local[ELEMENT_COUNT];
    __ubuf__ uint32_t input1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input0_local, input0, BUFFER_BYTES);
    asc_copy_gm2ub_align(input1_local, input1, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    mull_vf(output_low_local, output_high_local, input0_local, input1_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output_low, output_low_local, BUFFER_BYTES);
    asc_copy_ub2gm_align(output_high, output_high_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint32_t> input0(ELEMENT_COUNT);
    std::vector<uint32_t> input1(ELEMENT_COUNT);
    std::vector<uint32_t> output_low(ELEMENT_COUNT, 0);
    std::vector<uint32_t> output_high(ELEMENT_COUNT, 0);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input0[i] = i + 1;
        input1[i] = 0x10000000U + i;
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t* input0_device = nullptr;
    uint32_t* input1_device = nullptr;
    uint32_t* output_low_device = nullptr;
    uint32_t* output_high_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input0_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&input1_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_low_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_high_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input0_device, BUFFER_BYTES, input0.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(input1_device, BUFFER_BYTES, input1.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_mull_kernel<<<1, 0>>>(output_low_device, output_high_device, input0_device, input1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output_low.data(), BUFFER_BYTES, output_low_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(output_high.data(), BUFFER_BYTES, output_high_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    bool passed = true;
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        const uint64_t expected = static_cast<uint64_t>(input0[i]) * input1[i];
        if (output_low[i] != static_cast<uint32_t>(expected) ||
            output_high[i] != static_cast<uint32_t>(expected >> 32)) {
            passed = false;
            break;
        }
    }
    std::cout << (passed ? "[Success] asc_mull completed."
                         : "[Failed] asc_mull output mismatch.")
              << std::endl;
    aclrtFree(input0_device);
    aclrtFree(input1_device);
    aclrtFree(output_low_device);
    aclrtFree(output_high_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
