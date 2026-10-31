# asc_mul_scalar_float2half_rn

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.010892+00:00 pushedAt=2026-10-27T02:26:32.871Z nl=2 -->

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

Multiplies source operand `src` (float type) element-wise by scalar `value` according to `mask`, and then converts the result to the half type according to the ROUND rounding mode to obtain the computation result. The calculation formula is as follows:

$$
dst_i = round\_to\_half(src_i \times value)
$$

Because the bit width ratio of the source operand (float, 32 bits) to the destination operand (half, 16 bits) is 2:1, the converted data of one `vector_float` fills one `vector_half`. The `dst_pos` parameter specifies whether the result is written to even or odd indices in `dst`, and unwritten positions are set to 0.

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```c
// Return the result through the function return value, with data written to even indices.
__simd_callee__ inline vector_half asc_mul_scalar_float2half_rn(vector_float src,
                                                                float value,
                                                                vector_bool mask,
                                                                std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)

// Return the result through the function return value, with data written to odd indices.
__simd_callee__ inline vector_half asc_mul_scalar_float2half_rn(vector_float src,
                                                                float value,
                                                                vector_bool mask,
                                                                std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)

// Output the result through a reference parameter, with data written to even indices.
__simd_callee__ inline void asc_mul_scalar_float2half_rn(vector_half& dst,
                                                         vector_float src,
                                                         float value,
                                                         vector_bool mask,
                                                         std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)

// Output the result through a reference parameter, with data written to odd indices.
__simd_callee__ inline void asc_mul_scalar_float2half_rn(vector_half& dst,
                                                         vector_float src,
                                                         float value,
                                                         vector_bool mask,
                                                         std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| -------- | --------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| dst | Output | Destination operand (vector data register, half type). |
| src | Input | Source operand (vector data register, float type). |
| value | Input | Source operand (scalar, float type). |
| mask | Input | Mask register, which controls whether each element participates in the computation. When the bit corresponding to an element in `mask` is 1, the element participates in the computation; when it is 0, the element does not participate. |
| dst_pos | Input | Data write position mode, with values of `ASC_POSITION_EVEN` (even positions) or `ASC_POSITION_ODD` (odd positions). When even positions are selected, odd positions are set to 0; when odd positions are selected, even positions are set to 0. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

- Outputting the result through a reference parameter: the computation result is written to `dst`.
- Returning the result through the function return value: the computation result is returned, with the type `vector_half`.

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- When the `mask` bit is 0, the corresponding element of the result is set to 0.
- When `dst_pos` is `ASC_POSITION_EVEN`, the result is written to the even index positions of `dst`, and odd index positions are set to 0; when `dst_pos` is `ASC_POSITION_ODD`, the result is written to the odd index positions of `dst`, and even index positions are set to 0.

## Examples

Save the following code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [__NPU_ARCH__](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

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
constexpr float SCALAR_VALUE = 2.0f;

__simd_vf__ inline void compute(__ubuf__ half* dst, __ubuf__ float* src)
{
    vector_half dst_reg;
    vector_float src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    vector_bool mask_b16_all = asc_create_mask_b16(PAT_ALL);
    asc_loadalign(src_reg, src);
    asc_mul_scalar_float2half_rn(dst_reg, src_reg, SCALAR_VALUE, mask, ASC_POSITION_EVEN);
    asc_storealign(dst, dst_reg, mask_b16_all);
}

__global__ __vector__ void asc_mul_scalar_float2half_rn_kernel(__gm__ half* dst, __gm__ float* src)
{
    asc_init();
    __ubuf__ half dst_local[ELEMENT_COUNT * 2];
    __ubuf__ float src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    compute(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> src(ELEMENT_COUNT);
    std::vector<half> output(ELEMENT_COUNT * 2, half(0));
    std::vector<half> golden(ELEMENT_COUNT * 2, half(0));
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        src[i] = (static_cast<int>(i) - 16) * 0.25f;
    }
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        golden[2 * i] = static_cast<half>(src[i] * SCALAR_VALUE);
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    float* src_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), (ELEMENT_COUNT) * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    half* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), (ELEMENT_COUNT * 2) * sizeof(half),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(float), src.data(), src.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_mul_scalar_float2half_rn_kernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(half), dst_device, output.size() * sizeof(half),
        ACL_MEMCPY_DEVICE_TO_HOST);
    const bool passed = compare_data(output, golden, 1e-3);
    std::cout << (passed ? "[Success] asc_mul_scalar_float2half_rn passed." : "[Failed] asc_mul_scalar_float2half_rn failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
