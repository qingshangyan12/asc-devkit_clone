# asc_float2bfloat16

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:33.904502+00:00 pushedAt=2026-10-27T02:26:32.481Z nl=2 -->

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

The header file path is `"c_api/reg_compute/reg_convert.h"`.

Converts each `float` type data in `src` to the `bfloat16_t` type according to `mask`, supports multiple rounding modes and saturation modes, and writes the result to `dst`.
Because the bit-width ratio of the source operand type to the destination operand type is 2:1, data of one `VL` size must be split into two parts when writing. The `dst_pos` parameter selects whether to write to even-indexed or odd-indexed positions in `dst`. The pseudocode is as follows:

```python
# The position check is only used to explain the API semantics. The actual position parameter is a compile-time constant.
# VL indicates the vector data register bit width, which is 256 bytes.
def asc_float2bfloat16_rn(dst, src, mask, dst_pos):
    offset = 0 if dst_pos == ASC_POSITION_EVEN else 1
    for i in range(VL // 4):
        dst[2 * i] = 0
        dst[2 * i + 1] = 0
        if mask[i]:
            dst[2 * i + offset] = bfloat16(src[i])  # float -> bfloat16, RINT rounding.
```

For details about rounding modes, see [Rounding Modes and Saturation Modes](rounding_mode.md).

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```c
// Output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_float2bfloat16_<round_mode><sat_mode>(vector_bfloat16_t& dst,
                                                                      vector_float src,
                                                                      vector_bool mask,
                                                                      std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)

__simd_callee__ inline void asc_float2bfloat16_<round_mode><sat_mode>(vector_bfloat16_t& dst,
                                                                      vector_float src,
                                                                      vector_bool mask,
                                                                      std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)

// Return the result through the function return value (placeholder form).
__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_<round_mode><sat_mode>(vector_float src,
                                                                      vector_bool mask,
                                                                      std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_<round_mode><sat_mode>(vector_float src,
                                                                      vector_bool mask,
                                                                      std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

- `<round_mode>` indicates the supported rounding modes: `rd` (`FLOOR`), `rn` (`RINT`), `rna` (`ROUND`), `ru` (`CEIL`), and `rz` (`TRUNC`).
- `<sat_mode>` indicates the saturation/non-saturation mode. When empty, it indicates non-saturation mode; when set to `_sat`, it indicates saturation mode.

### Typical Prototype Example

```c
// RINT rounding mode, non-saturation mode, data written to even-indexed positions.
__simd_callee__ inline void asc_float2bfloat16_rn(vector_bfloat16_t& dst,
                                                  vector_float src,
                                                  vector_bool mask,
                                                  std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| ------ | --------- | ------------------------------------------------------------------------------------------------------------------ |
| dst | Output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Mask register, which controls whether each element participates in the computation. When the bit corresponding to an element in `mask` is 1, the element participates in the computation; when it is 0, the element does not participate. |
| dst_pos | Input | Position selection tag (compile-time tag dispatch; the corresponding implementation is selected through compile-time overloading), of type `std::integral_constant<asc_position_mode, asc_position_mode::EVEN>` or `std::integral_constant<asc_position_mode, asc_position_mode::ODD>`. When set to `ASC_POSITION_EVEN`, the result is written to even-indexed positions of the destination operand and other positions are set to zero; when set to `ASC_POSITION_ODD`, the result is written to odd-indexed positions of the destination operand and other positions are set to zero. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

- The prototype that outputs the result through a reference parameter has no return value.
- The function prototype that outputs the result through the function return value returns the computation result. The return value type is the same as the type of the `dst` parameter in the corresponding reference output function prototype (with the reference removed).

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The position selection tag parameter can only use compile-time constants. The compiler dispatches to the corresponding overload at compile time based on this parameter.
- The position selection tag selects the write address of the destination operand. Other positions are set to zero.
- The data types of `src` and `dst` must match the function prototype.
- When the mask bit in `mask` is 0, the corresponding element in `dst` is set to 0.
- When the result is written to the odd-indexed positions of `dst`, the even-indexed positions are set to 0; when the result is written to the even-indexed positions, the odd-indexed positions are set to 0.
- When saturation or non-saturation mode is used, the `ctrl` register must be configured. For details, see [asc_set_ctrl](../../spr/asc_set_ctrl.md).

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
void print_data(const char* label, const std::vector<T>& values)
{
    std::cout << label << ":";
    const size_t count = values.size() < 8 ? values.size() : 8;
    for (size_t i = 0; i < count; ++i) std::cout << ' ' << +values[i];
    if (values.size() > count) std::cout << " ...";
    std::cout << std::endl;
}

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

constexpr uint32_t BUFFER_BYTES = 256;
__simd_vf__ inline void convert(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    vector_bfloat16_t dst_0;
    vector_float src_0;
    asc_loadalign(dst_0, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_0, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rd_sat(dst_0, src_0, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_0, mask);
    vector_bfloat16_t dst_1;
    vector_float src_1;
    asc_loadalign(dst_1, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_1, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rd(dst_1, src_1, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_1, mask);
    vector_bfloat16_t dst_2;
    vector_float src_2;
    asc_loadalign(dst_2, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_2, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rd_sat(dst_2, src_2, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_2, mask);
    vector_bfloat16_t dst_3;
    vector_float src_3;
    asc_loadalign(dst_3, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_3, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rn(dst_3, src_3, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_3, mask);
    vector_bfloat16_t dst_4;
    vector_float src_4;
    asc_loadalign(dst_4, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_4, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rn_sat(dst_4, src_4, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_4, mask);
    vector_bfloat16_t dst_5;
    vector_float src_5;
    asc_loadalign(dst_5, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_5, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rn(dst_5, src_5, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_5, mask);
    vector_bfloat16_t dst_6;
    vector_float src_6;
    asc_loadalign(dst_6, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_6, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rn_sat(dst_6, src_6, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_6, mask);
    vector_bfloat16_t dst_7;
    vector_float src_7;
    asc_loadalign(dst_7, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_7, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rna(dst_7, src_7, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_7, mask);
    vector_bfloat16_t dst_8;
    vector_float src_8;
    asc_loadalign(dst_8, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_8, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rna_sat(dst_8, src_8, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_8, mask);
    vector_bfloat16_t dst_9;
    vector_float src_9;
    asc_loadalign(dst_9, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_9, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rna(dst_9, src_9, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_9, mask);
    vector_bfloat16_t dst_10;
    vector_float src_10;
    asc_loadalign(dst_10, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_10, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rna_sat(dst_10, src_10, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_10, mask);
    vector_bfloat16_t dst_11;
    vector_float src_11;
    asc_loadalign(dst_11, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_11, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_ru(dst_11, src_11, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_11, mask);
    vector_bfloat16_t dst_12;
    vector_float src_12;
    asc_loadalign(dst_12, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_12, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_ru_sat(dst_12, src_12, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_12, mask);
    vector_bfloat16_t dst_13;
    vector_float src_13;
    asc_loadalign(dst_13, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_13, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_ru(dst_13, src_13, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_13, mask);
    vector_bfloat16_t dst_14;
    vector_float src_14;
    asc_loadalign(dst_14, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_14, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_ru_sat(dst_14, src_14, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_14, mask);
    vector_bfloat16_t dst_15;
    vector_float src_15;
    asc_loadalign(dst_15, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_15, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rz(dst_15, src_15, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_15, mask);
    vector_bfloat16_t dst_16;
    vector_float src_16;
    asc_loadalign(dst_16, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_16, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rz_sat(dst_16, src_16, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_16, mask);
    vector_bfloat16_t dst_17;
    vector_float src_17;
    asc_loadalign(dst_17, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_17, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rz(dst_17, src_17, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_17, mask);
    vector_bfloat16_t dst_18;
    vector_float src_18;
    asc_loadalign(dst_18, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_18, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rz_sat(dst_18, src_18, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_18, mask);
    vector_bfloat16_t dst_19;
    vector_float src_19;
    asc_loadalign(dst_19, reinterpret_cast<__ubuf__ bfloat16_t*>(output));
    asc_loadalign(src_19, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2bfloat16_rd(dst_19, src_19, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst_19, mask);
}
__global__ __vector__ void asc_float2bfloat16_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t output_local[BUFFER_BYTES];
    __ubuf__ uint8_t input_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(output_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    convert(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace
int main()
{
    constexpr uint8_t input_pattern[] = {0x00, 0x00, 0x80, 0x3f};
    constexpr uint8_t golden_pattern[] = {0x80, 0x3f, 0x00, 0x00};
    std::vector<uint8_t> input(BUFFER_BYTES);
    std::vector<uint8_t> output(BUFFER_BYTES, 0xff);
    std::vector<uint8_t> golden(BUFFER_BYTES);
    for (size_t i = 0; i < BUFFER_BYTES; ++i) {
        input[i] = input_pattern[i % sizeof(input_pattern)];
        golden[i] = golden_pattern[i % sizeof(golden_pattern)];
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), (BUFFER_BYTES) * sizeof(uint8_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), (BUFFER_BYTES) * sizeof(uint8_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, input.size() * sizeof(uint8_t), input.data(), input.size() * sizeof(uint8_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_float2bfloat16_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint8_t), output_device, output.size() * sizeof(uint8_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Conversion: vector_float -> vector_bfloat16_t" << std::endl;
    print_data("Input 1.0 (raw bytes)", input);
    print_data("Output (raw bytes)", output);
    print_data("Golden 1.0 (raw bytes)", golden);
    const bool passed = compare_data(output, golden);
    std::cout << (passed ? "[Success] asc_float2bfloat16_rd/asc_float2bfloat16_rd_sat/"
                           "asc_float2bfloat16_rn/asc_float2bfloat16_rn_sat/"
                           "asc_float2bfloat16_rna/asc_float2bfloat16_rna_sat/"
                           "asc_float2bfloat16_ru/asc_float2bfloat16_ru_sat/"
                           "asc_float2bfloat16_rz/asc_float2bfloat16_rz_sat/"
                           "ASC_POSITION_ODD/ASC_POSITION_EVEN passed."
                         : "[Failed] asc_float2bfloat16_rd/asc_float2bfloat16_rd_sat/"
                           "asc_float2bfloat16_rn/asc_float2bfloat16_rn_sat/"
                           "asc_float2bfloat16_rna/asc_float2bfloat16_rna_sat/"
                           "asc_float2bfloat16_ru/asc_float2bfloat16_ru_sat/"
                           "asc_float2bfloat16_rz/asc_float2bfloat16_rz_sat/"
                           "ASC_POSITION_ODD/ASC_POSITION_EVEN failed.")
              << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
