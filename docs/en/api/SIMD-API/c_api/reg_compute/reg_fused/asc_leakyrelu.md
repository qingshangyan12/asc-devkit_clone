# asc_leakyrelu

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.007892+00:00 pushedAt=2026-10-27T02:26:32.860Z nl=2 -->

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

The header file path is `"c_api/reg_compute/compute/reg_fused.h"`.

Performs a Leaky ReLU operation on the elements in the source vector data register according to the mask, and returns the result through the function return value or writes it to the destination vector data register. Elements greater than 0 in the source operand are directly written to the output result, and elements less than or equal to 0 are multiplied by a scalar immediate value and then written to the output result. Elements whose mask bit is 1 participate in the computation, and elements whose mask bit is 0 are set to zero in the output result. The calculation formula is as follows:

$$
dst_i = \begin{cases} src_i & src_i > 0 \\ src_i \times \alpha & src_i \leq 0 \end{cases}
$$

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```c
// Return the result through the function return value (placeholder form).
__simd_callee__ inline vector_<dtype> asc_leakyrelu(vector_<dtype> src,
                                                    <dtype> alpha,
                                                    vector_bool mask)

// Output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_leakyrelu(vector_<dtype>& dst,
                                          vector_<dtype> src,
                                          <dtype> alpha,
                                          vector_bool mask)
```

### Data Types Supported by dtype

`dtype` values: `half`, `float`.

### Typical Prototype Example

```c
// Return the result through the function return value.
__simd_callee__ inline vector_half asc_leakyrelu(vector_half src,
                                                 half alpha,
                                                 vector_bool mask)

// Output the result through a reference parameter.
__simd_callee__ inline void asc_leakyrelu(vector_half& dst,
                                          vector_half src,
                                          half alpha,
                                          vector_bool mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Destination operand (vector data register). Only the void-returning API contains this parameter, and the data type must be the same as `src`. |
| src | Input | Source operand (vector data register). |
| alpha | Input | Negative half-axis slope (scalar). The data type must be the same as `src`. |
| mask | Input | Source operand mask (mask register), which indicates which elements participate in the computation. A value of 1 at the corresponding position means the element participates in the computation, and 0 means it does not. Elements not selected by `mask` are set to zero in the output. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

For the return-value API, the vector data register that stores the Leaky ReLU computation result is returned, and its data type is consistent with `src`.

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- Elements whose mask bit is 0 do not participate in the operation, and 0 is written to the corresponding positions in the output result.

### Computation Constraints

- When the data in the source vector data register is negative zero (-0), it is treated as a negative number, and the multiplication complies with IEEE 754 floating-point multiplication rules.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr uint32_t BUFFER_BYTES = ELEMENT_COUNT * sizeof(float);
constexpr float ALPHA = 0.1f;

__simd_vf__ inline void leakyrelu_vf(__ubuf__ float* output, __ubuf__ float* input)
{
    vector_float src;
    vector_bool mask = asc_create_mask_b32(PAT_ALL);
    asc_loadalign(src, input);
    vector_float dst = asc_leakyrelu(src, ALPHA, mask);
    asc_storealign(output, dst, mask);
}

__global__ __vector__ void asc_leakyrelu_kernel(__gm__ float* output, __gm__ float* input)
{
    asc_init();
    __ubuf__ float output_local[ELEMENT_COUNT];
    __ubuf__ float input_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    leakyrelu_vf(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> input(ELEMENT_COUNT);
    std::vector<float> output(ELEMENT_COUNT, 0.0f);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input[i] = static_cast<float>(static_cast<int32_t>(i) - 64);
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    float* input_device = nullptr;
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BUFFER_BYTES, input.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_leakyrelu_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BUFFER_BYTES, output_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    bool passed = true;
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        const float expected = input[i] > 0.0f ? input[i] : input[i] * ALPHA;
        if (std::fabs(output[i] - expected) > 1e-6f) {
            passed = false;
            break;
        }
    }
    std::cout << (passed ? "[Success] asc_leakyrelu completed."
                         : "[Failed] asc_leakyrelu output mismatch.")
              << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
