# asc_half2int4x2

<!-- md-trans-meta sourceCommit=e430b2c74d537898700e8c0eb634eda72eb98460 translatedAt=2026-10-23T03:54:33.931719+00:00 pushedAt=2026-10-27T02:26:32.574Z nl=2 -->

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

Converts the `half` elements in `src` that participate in the computation to the `int4x2_t` type according to `mask`. The result can be output through a reference parameter or the function return value.

Each `half` element in `src` corresponds to one mask bit in `mask`. Two consecutive `half` elements are converted to two consecutive 4-bit signed integers, and the conversion results are packed into the same `int4x2_t` storage unit in input order, occupying 1 byte in total. When the mask bit in `mask` is 0, the corresponding 4-bit conversion result is set to 0.

Because the bit-width ratio of the source operand type to the destination operand type is 4:1, the `int4x2_t` storage units in `dst` are divided into four groups by index. The `dst_quarter_pos` parameter discretely selects index positions at intervals of 4 storage units in `dst` to write the conversion results, specifically indices 0, 4, 8, ..., indices 1, 5, 9, ..., indices 2, 6, 10, ..., or indices 3, 7, 11, ...; the other positions are set to zero.

For details about rounding modes and saturation/non-saturation modes, see [Rounding Modes and Saturation Modes](rounding_mode.md).

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```cpp
// Non-saturation mode, output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_half2int4x2_<round_mode>(
    vector_int4x2_t& dst,
    vector_half src,
    vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, <quarter_pos>> dst_quarter_pos)

// Non-saturation mode, return the result through the function return value (placeholder form).
__simd_callee__ inline vector_int4x2_t asc_half2int4x2_<round_mode>(
    vector_half src,
    vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, <quarter_pos>> dst_quarter_pos)

// Saturation mode, output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_half2int4x2_<round_mode>_sat(
    vector_int4x2_t& dst,
    vector_half src,
    vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, <quarter_pos>> dst_quarter_pos)

// Saturation mode, return the result through the function return value (placeholder form).
__simd_callee__ inline vector_int4x2_t asc_half2int4x2_<round_mode>_sat(
    vector_half src,
    vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, <quarter_pos>> dst_quarter_pos)
```

**Placeholder description:**

- `<round_mode>` supports `rd` (FLOOR), `rn` (RINT), `rna` (ROUND), `ru` (CEIL), and `rz` (TRUNC).
- `<quarter_pos>` supports `ASC_DISPERSE_FIRST_QUARTER`, `ASC_DISPERSE_SECOND_QUARTER`, `ASC_DISPERSE_THIRD_QUARTER`, and `ASC_DISPERSE_FOURTH_QUARTER`.

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| ------ | --------- | ---- |
| dst | Output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Mask register, which controls whether each element participates in the computation. When the bit corresponding to an element in `mask` is 1, the element participates in the computation; when it is 0, the element does not participate. |
| dst_quarter_pos | Input | Quarter position selection tag (compile-time tag dispatch, selecting the corresponding implementation through compile-time overloading), used to select the write position of the destination operand; other positions are cleared. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

- The prototype that outputs the result through a reference parameter has no return value.
- The function prototype that outputs the result through the function return value returns the computation result. The return value type is the same as the type of the `dst` parameter in the corresponding reference output function prototype (with the reference removed).

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The position selection tag parameter can only use the compile-time constants described above.
- When saturation mode or non-saturation mode is enabled, the ctrl register must be configured. For details, see [asc_set_ctrl](../../spr/asc_set_ctrl.md).
- When the mask bit in `mask` is 0, the corresponding element in `dst` is set to 0.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

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
constexpr uint32_t BUFFER_BYTES = 256;

__simd_vf__ inline void convert(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    vector_bool store_mask = asc_create_mask_b8(PAT_ALL);
    vector_int4x2_t dst;
    vector_half src;
    asc_loadalign(dst, reinterpret_cast<__ubuf__ int4b_t*>(output));
    asc_loadalign(src, reinterpret_cast<__ubuf__ half*>(input));
    asc_half2int4x2_rna(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ int4b_t*>(output), dst, store_mask);
}
} // namespace

__global__ __vector__ void asc_half2int4x2_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t output_local[BUFFER_BYTES];
    __ubuf__ uint8_t input_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(output_local, output, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    convert(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}

int main()
{
    constexpr uint8_t input_pattern[] = {0x00, 0x3c};
    constexpr uint8_t golden_pattern[] = {0x11, 0x00, 0x00, 0x00};
    std::vector<uint8_t> input(BUFFER_BYTES);
    std::vector<uint8_t> output(BUFFER_BYTES, 0);
    std::vector<uint8_t> golden(BUFFER_BYTES);
    for (size_t i = 0; i < BUFFER_BYTES; ++i) {
        input[i] = input_pattern[i % sizeof(input_pattern)];
        golden[i] = golden_pattern[i % sizeof(golden_pattern)];
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BUFFER_BYTES, input.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(output_device, BUFFER_BYTES, output.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_half2int4x2_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BUFFER_BYTES, output_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    bool passed = true;
    for (size_t i = 0; i < BUFFER_BYTES; ++i) passed = passed && (output[i] == golden[i]);
    std::cout << (passed ? "[Success] asc_half2int4x2_rna passed."
                         : "[Failed] asc_half2int4x2_rna failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
