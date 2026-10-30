# asc_exp_sub

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.004381+00:00 pushedAt=2026-10-27T02:26:32.848Z nl=2 -->

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

Subtracts `src0` and `src1` element-wise according to `mask` and computes the natural exponential function with the difference as the exponent. The computation result is returned through the function return value or written to `dst`.

- `asc_exp_sub` is used for `float` type inputs. The calculation formula is as follows:

  $$
  result_i = e^{src0_i - src1_i}
  $$

- `asc_exp_sub_half2float` is used for `half` type inputs. This API supports reading elements at even or odd indices from the source operands, converting them to the `float` type, and then performing the computation. The calculation formula is as follows:

  $$
  \begin{cases}
  result_i = e^{float(src0_{2i}) - float(src1_{2i})} & \text{Extract even-indexed elements} \\
  result_i = e^{float(src0_{2i+1}) - float(src1_{2i+1})} & \text{Extract odd-indexed elements}
  \end{cases}
  $$

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

- Used to process source operands of the `float` data type.

  ```c
  // Return the result through the function return value.
  __simd_callee__ inline vector_float asc_exp_sub(vector_float src0,
                                                  vector_float src1,
                                                  vector_bool mask)

  // Output the result through a reference parameter.
  __simd_callee__ inline void asc_exp_sub(vector_float& dst,
                                          vector_float src0,
                                          vector_float src1,
                                          vector_bool mask)
  ```

- Processes the source operand of the `half` data type, supporting reading source data at even or odd indices.

  ```c
  // Return the result through the function return value (placeholder form).
  __simd_callee__ inline vector_float asc_exp_sub_half2float(vector_half src0,
                                                             vector_half src1,
                                                             vector_bool mask,
                                                             std::integral_constant<asc_position_mode, <src_pos>> src_pos)

  // Output the result through a reference parameter (placeholder form).
  __simd_callee__ inline void asc_exp_sub_half2float(vector_float& dst,
                                                     vector_half src0,
                                                     vector_half src1,
                                                     vector_bool mask,
                                                     std::integral_constant<asc_position_mode, <src_pos>> src_pos)
  ```

  **Placeholder description:**

  - `<src_pos>` supports `ASC_POSITION_EVEN` and `ASC_POSITION_ODD`.

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| ------ | --------- | ---- |
| dst | Output | Destination operand (vector data register). |
| src0 | Input | Source operand 0 (vector data register). |
| src1 | Input | Source operand 1 (vector data register). |
| mask | Input | Mask register that controls whether each element participates in the computation. When the bit in `mask` corresponding to an output element is 1, the element participates in the computation; when it is 0, the element does not participate. |
| src_pos | Input | Indicates the read position of the source operand. When the value is `ASC_POSITION_EVEN`, elements at even indices are read; when the value is `ASC_POSITION_ODD`, elements at odd indices are read. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

- The prototype that outputs the result through a reference parameter has no return value, and the computation result is written to `dst`.
- The prototype that outputs the result through the function return value returns the computation result, and the return value type is `vector_float`.

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- When the `mask` bit is 0, the corresponding element of the result is set to 0.
- Precision is improved during computation only when the bit width of the output data type is greater than that of the input.

## Examples

Save the following code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [__NPU_ARCH__](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 -lm && ./main
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
template <typename T>
void print_data(const char* label, const std::vector<T>& values)
{
    std::cout << label << ":";
    const size_t count = values.size() < 8 ? values.size() : 8;
    for (size_t i = 0; i < count; ++i) std::cout << ' ' << static_cast<float>(values[i]);
    if (values.size() > count) std::cout << " ...";
    std::cout << std::endl;
}

bool compare_data(const std::vector<float>& actual, const std::vector<float>& expected, double tolerance)
{
    if (actual.size() != expected.size()) return false;
    for (size_t i = 0; i < actual.size(); ++i) {
        const double diff = static_cast<double>(actual[i]) - static_cast<double>(expected[i]);
        if (diff > tolerance || diff < -tolerance) return false;
    }
    return true;
}

constexpr uint32_t FLOAT_COUNT = 64;
constexpr uint32_t HALF_COUNT = 128;

__simd_vf__ inline void compute(__ubuf__ float* float_dst, __ubuf__ float* half_even_dst,
    __ubuf__ float* half_odd_dst, __ubuf__ float* float_src0, __ubuf__ float* float_src1,
    __ubuf__ half* half_src0, __ubuf__ half* half_src1)
{
    vector_float float_src0_reg;
    vector_float float_src1_reg;
    vector_half half_src0_reg;
    vector_half half_src1_reg;
    asc_loadalign(float_src0_reg, float_src0);
    asc_loadalign(float_src1_reg, float_src1);
    asc_loadalign(half_src0_reg, half_src0);
    asc_loadalign(half_src1_reg, half_src1);

    uint32_t count = FLOAT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    vector_bool mask_b16 = asc_create_mask_b16(PAT_ALL);
    vector_float float_result = asc_exp_sub(float_src0_reg, float_src1_reg, mask);
    vector_float half_even_result;
    asc_exp_sub_half2float(half_even_result, half_src0_reg, half_src1_reg, mask_b16,
        ASC_POSITION_EVEN);
    vector_float half_odd_result = asc_exp_sub_half2float(half_src0_reg, half_src1_reg,
        mask_b16, ASC_POSITION_ODD);

    asc_storealign(float_dst, float_result, mask);
    asc_storealign(half_even_dst, half_even_result, mask);
    asc_storealign(half_odd_dst, half_odd_result, mask);
}

__global__ __vector__ void asc_exp_sub_kernel(__gm__ float* float_dst, __gm__ float* half_even_dst,
    __gm__ float* half_odd_dst, __gm__ float* float_src0, __gm__ float* float_src1,
    __gm__ half* half_src0, __gm__ half* half_src1)
{
    asc_init();
    __ubuf__ float float_dst_local[FLOAT_COUNT];
    __ubuf__ float half_even_dst_local[FLOAT_COUNT];
    __ubuf__ float half_odd_dst_local[FLOAT_COUNT];
    __ubuf__ float float_src0_local[FLOAT_COUNT];
    __ubuf__ float float_src1_local[FLOAT_COUNT];
    __ubuf__ half half_src0_local[HALF_COUNT];
    __ubuf__ half half_src1_local[HALF_COUNT];
    asc_copy_gm2ub_align(float_src0_local, float_src0, FLOAT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(float_src1_local, float_src1, FLOAT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(half_src0_local, half_src0, HALF_COUNT * sizeof(half));
    asc_copy_gm2ub_align(half_src1_local, half_src1, HALF_COUNT * sizeof(half));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    compute(float_dst_local, half_even_dst_local, half_odd_dst_local, float_src0_local,
        float_src1_local, half_src0_local, half_src1_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(float_dst, float_dst_local, FLOAT_COUNT * sizeof(float));
    asc_copy_ub2gm_align(half_even_dst, half_even_dst_local, FLOAT_COUNT * sizeof(float));
    asc_copy_ub2gm_align(half_odd_dst, half_odd_dst_local, FLOAT_COUNT * sizeof(float));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> float_src0(FLOAT_COUNT);
    std::vector<float> float_src1(FLOAT_COUNT);
    std::vector<half> half_src0(HALF_COUNT);
    std::vector<half> half_src1(HALF_COUNT);
    std::vector<float> float_output(FLOAT_COUNT);
    std::vector<float> half_even_output(FLOAT_COUNT);
    std::vector<float> half_odd_output(FLOAT_COUNT);
    std::vector<float> float_golden(FLOAT_COUNT);
    std::vector<float> half_even_golden(FLOAT_COUNT);
    std::vector<float> half_odd_golden(FLOAT_COUNT);
    for (uint32_t i = 0; i < FLOAT_COUNT; ++i) {
        float_src0[i] = static_cast<float>(i % 8) * 0.125f;
        float_src1[i] = static_cast<float>((i + 1) % 8) * 0.125f;
        float_golden[i] = std::exp(float_src0[i] - float_src1[i]);
    }
    for (uint32_t i = 0; i < HALF_COUNT; ++i) {
        half_src0[i] = static_cast<half>(static_cast<float>(i % 8) * 0.125f);
        half_src1[i] = static_cast<half>(static_cast<float>((i + 2) % 8) * 0.125f);
    }
    for (uint32_t i = 0; i < FLOAT_COUNT; ++i) {
        half_even_golden[i] = std::exp(static_cast<float>(half_src0[2 * i]) -
                                       static_cast<float>(half_src1[2 * i]));
        half_odd_golden[i] = std::exp(static_cast<float>(half_src0[2 * i + 1]) -
                                      static_cast<float>(half_src1[2 * i + 1]));
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    float* float_dst_device = nullptr;
    float* half_even_dst_device = nullptr;
    float* half_odd_dst_device = nullptr;
    float* float_src0_device = nullptr;
    float* float_src1_device = nullptr;
    half* half_src0_device = nullptr;
    half* half_src1_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&float_dst_device), FLOAT_COUNT * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&half_even_dst_device), FLOAT_COUNT * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&half_odd_dst_device), FLOAT_COUNT * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&float_src0_device), FLOAT_COUNT * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&float_src1_device), FLOAT_COUNT * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&half_src0_device), HALF_COUNT * sizeof(half), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&half_src1_device), HALF_COUNT * sizeof(half), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(float_src0_device, FLOAT_COUNT * sizeof(float), float_src0.data(), FLOAT_COUNT * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(float_src1_device, FLOAT_COUNT * sizeof(float), float_src1.data(), FLOAT_COUNT * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(half_src0_device, HALF_COUNT * sizeof(half), half_src0.data(), HALF_COUNT * sizeof(half),
        ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(half_src1_device, HALF_COUNT * sizeof(half), half_src1.data(), HALF_COUNT * sizeof(half),
        ACL_MEMCPY_HOST_TO_DEVICE);

    asc_exp_sub_kernel<<<1, 0>>>(float_dst_device, half_even_dst_device, half_odd_dst_device,
        float_src0_device, float_src1_device, half_src0_device, half_src1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(float_output.data(), FLOAT_COUNT * sizeof(float), float_dst_device, FLOAT_COUNT * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(half_even_output.data(), FLOAT_COUNT * sizeof(float), half_even_dst_device,
        FLOAT_COUNT * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(half_odd_output.data(), FLOAT_COUNT * sizeof(float), half_odd_dst_device,
        FLOAT_COUNT * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);

    print_data("Float src0", float_src0);
    print_data("Float src1", float_src1);
    print_data("Float output", float_output);
    print_data("Float golden", float_golden);
    print_data("Half src0", half_src0);
    print_data("Half src1", half_src1);
    print_data("Half even output", half_even_output);
    print_data("Half even golden", half_even_golden);
    print_data("Half odd output", half_odd_output);
    print_data("Half odd golden", half_odd_golden);
    const bool passed = compare_data(float_output, float_golden, 1e-4) &&
                        compare_data(half_even_output, half_even_golden, 1e-4) &&
                        compare_data(half_odd_output, half_odd_golden, 1e-4);
    std::cout << (passed ? "[Success] asc_exp_sub passed." : "[Failed] asc_exp_sub failed.") << std::endl;
    aclrtFree(float_dst_device);
    aclrtFree(half_even_dst_device);
    aclrtFree(half_odd_dst_device);
    aclrtFree(float_src0_device);
    aclrtFree(float_src1_device);
    aclrtFree(half_src0_device);
    aclrtFree(half_src1_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
