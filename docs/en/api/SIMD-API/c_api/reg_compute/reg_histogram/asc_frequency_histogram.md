# asc_frequency_histogram

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-27T01:34:54.909915+00:00 pushedAt=2026-10-27T02:26:32.922Z nl=2 -->

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

The header file path is `"c_api/reg_compute/compute/reg_histogram.h"`.

Performs frequency statistics on the elements in the input Vector Data Register to generate a histogram. The statistical results are accumulated on top of the original data in the destination Vector Data Register. A mask can be configured to indicate the elements that participate in the statistics: elements with a mask of 1 participate in the statistics, and elements with a mask of 0 do not. The mask does not affect the write behavior of the destination Vector Data Register.

Because the source Vector Data Register `src` has the data type `uint8_t` (value range 0 to 255), while each element of the destination Vector Data Register `dst` is `uint16_t` and one Vector Length can store 128 `uint16_t` data elements, this API supports two modes:

- **Low mode (`BIN0`)**: Counts the occurrences of values in the range [0, 127] in `src` and accumulates them into `dst`. `dst[0]` represents the occurrence count of value 0, and `dst[127]` represents the occurrence count of value 127.
- **High mode (`BIN1`)**: Counts the occurrences of values in the range [128, 255] in `src` and accumulates them into `dst`. `dst[0]` represents the occurrence count of value 128, and `dst[127]` represents the occurrence count of value 255.

As shown in the figure below:

![Frequency statistics](../../../../figures/asc_frequency_histogram.png)

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

### Low Mode

```cpp
__simd_callee__ inline void asc_frequency_histogram_bin0(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
```

### High Mode

```cpp
__simd_callee__ inline void asc_frequency_histogram_bin1(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Input/output | Destination operand (vector data register).|
| src | Input | Source operand (vector data register).|
| mask | Input | Source operand mask (mask register), used to indicate which elements participate in the computation. A value of 1 at the corresponding position indicates participation, and 0 indicates non-participation. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).

### Computation Constraints

- `mask` is used to filter the source operands and does not affect the original data or write behavior of positions in `dst` that are not accumulated. When a mask bit is 0, the value at the corresponding position of the source operand `src` is ignored, and the value at the corresponding position of `dst` is the statistical result obtained after ignoring `src` at that position.
- The statistical results in `dst` are accumulated on top of the original data. `dst` must be initialized before the first statistics operation. When this API is called multiple times, the later statistical results are accumulated into the previous statistical results.
- The data type of `dst` is `uint16_t`, and the maximum value of a single statistical result is 65535. The user must ensure that the accumulated statistical results do not exceed this range; otherwise, overflow occurs.

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
constexpr uint32_t INPUT_COUNT = 256;
constexpr uint32_t BIN_COUNT = 256;
constexpr uint32_t INPUT_BYTES = INPUT_COUNT * sizeof(uint8_t);
constexpr uint32_t OUTPUT_BYTES = BIN_COUNT * sizeof(uint16_t);

__simd_vf__ inline void histogram_vf(__ubuf__ uint16_t* output, __ubuf__ uint8_t* input)
{
    vector_uint8_t src;
    vector_uint16_t dst0;
    vector_uint16_t dst1;
    vector_bool src_mask = asc_create_mask_b8(PAT_ALL);
    vector_bool dst_mask = asc_create_mask_b16(PAT_ALL);

    asc_duplicate_scalar(dst0, static_cast<uint16_t>(0));
    asc_duplicate_scalar(dst1, static_cast<uint16_t>(0));
    asc_loadalign(src, input);
    asc_frequency_histogram_bin0(dst0, src, src_mask);
    asc_frequency_histogram_bin1(dst1, src, src_mask);
    asc_storealign(output, dst0, dst_mask);
    asc_storealign(output + 128, dst1, dst_mask);
}

__global__ __vector__ void asc_frequency_histogram_kernel(__gm__ uint16_t* output,
                                                           __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t input_local[INPUT_COUNT];
    __ubuf__ uint16_t output_local[BIN_COUNT];
    asc_copy_gm2ub_align(input_local, input, INPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    histogram_vf(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> input(INPUT_COUNT);
    std::vector<uint16_t> output(BIN_COUNT, 0);
    std::vector<uint16_t> expected(BIN_COUNT, 0);
    for (uint32_t i = 0; i < INPUT_COUNT; ++i) {
        input[i] = static_cast<uint8_t>((i * 37U + 11U) & 0xffU);
        ++expected[input[i]];
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    uint16_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_frequency_histogram_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    bool passed = true;
    for (uint32_t i = 0; i < BIN_COUNT; ++i) {
        if (output[i] != expected[i]) {
            passed = false;
            break;
        }
    }
    std::cout << (passed ? "[Success] asc_frequency_histogram completed."
                         : "[Failed] asc_frequency_histogram output mismatch.")
              << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
