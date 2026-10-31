# asc_cumulative_histogram

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.020134+00:00 pushedAt=2026-10-27T02:26:32.917Z nl=2 -->

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

Performs cumulative frequency statistics on the elements in the input vector data register to generate a cumulative histogram. The statistical result is accumulated on top of the existing data in the destination vector data register. A mask can be configured to indicate the elements that participate in the statistics. When a mask bit is 1, the corresponding element participates in the statistics; when it is 0, the element is not counted. The mask does not affect the write behavior of the destination vector data register.

Because the source Vector Data Register `src` has the data type `uint8_t` with a value range of [0, 255], while the destination Vector Data Register `dst` has the element type `uint16_t` and one Vector Length can store 128 `uint16_t` data elements, this API supports the following two modes, where n is the element index of `dst` and n ∈ {0, 1, ..., 127}:

- **Low mode (`BIN0`)**: `dst[n]` accumulates the number of elements in `src` whose values are not greater than `n`, with a cumulative threshold range of [0, 127].
- **High mode (`BIN1`)**: `dst[n]` accumulates the number of elements in `src` whose values are not greater than `128 + n`, with a cumulative threshold range of [128, 255].

As shown in the figure below:

![Cumulative statistics](../../../../figures/asc_cumulative_histogram.png)

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

### Low Mode

```c
__simd_callee__ inline void asc_cumulative_histogram_bin0(vector_uint16_t& dst,
                                                          vector_uint8_t src,
                                                          vector_bool mask)
```

### High Mode

```c
__simd_callee__ inline void asc_cumulative_histogram_bin1(vector_uint16_t& dst,
                                                          vector_uint8_t src,
                                                          vector_bool mask)
```

`src` supports the data type `uint8_t`, and `dst` supports the data type `uint16_t`.

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| :----- | :------- | :------- |
| dst | Input/output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register), used to indicate which elements participate in the computation. When the bit corresponding to an element in mask is 1, the element participates in the computation; when it is 0, the element does not participate in the computation. |

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

constexpr uint32_t SRC_COUNT = 256;
constexpr uint32_t DST_COUNT = 128;

__simd_vf__ inline void histogram(__ubuf__ uint16_t* low, __ubuf__ uint16_t* high, __ubuf__ uint8_t* src)
{
    vector_uint16_t low_reg, high_reg;
    vector_uint8_t src_reg;
    uint32_t src_count = SRC_COUNT;
    vector_bool src_mask = asc_update_mask_b8(src_count);
    uint32_t dst_count = DST_COUNT;
    vector_bool dst_mask = asc_update_mask_b16(dst_count);
    asc_duplicate_scalar(low_reg, static_cast<uint16_t>(0), dst_mask);
    asc_duplicate_scalar(high_reg, static_cast<uint16_t>(0), dst_mask);
    asc_loadalign(src_reg, src);
    asc_cumulative_histogram_bin0(low_reg, src_reg, src_mask);
    asc_cumulative_histogram_bin1(high_reg, src_reg, src_mask);
    asc_storealign(low, low_reg, dst_mask);
    asc_storealign(high, high_reg, dst_mask);
}

__global__ __vector__ void asc_cumulative_histogram_kernel(__gm__ uint16_t* low, __gm__ uint16_t* high, __gm__ uint8_t* src)
{
    asc_init();
    __ubuf__ uint16_t low_local[DST_COUNT], high_local[DST_COUNT];
    __ubuf__ uint8_t src_local[SRC_COUNT];
    asc_copy_gm2ub_align(src_local, src, SRC_COUNT);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    histogram(low_local, high_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(low, low_local, DST_COUNT * sizeof(uint16_t));
    asc_copy_ub2gm_align(high, high_local, DST_COUNT * sizeof(uint16_t));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> src(SRC_COUNT);
    std::vector<uint16_t> low(DST_COUNT, 0), high(DST_COUNT, 0);
    std::vector<uint16_t> golden_low(DST_COUNT), golden_high(DST_COUNT);
    for (uint32_t i = 0; i < SRC_COUNT; ++i) src[i] = static_cast<uint8_t>(i);
    for (uint32_t i = 0; i < DST_COUNT; ++i) {
        golden_low[i] = i + 1;
        golden_high[i] = DST_COUNT + i + 1;
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* src_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), (SRC_COUNT) * sizeof(uint8_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint16_t* low_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&low_device), (DST_COUNT) * sizeof(uint16_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint16_t* high_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&high_device), (DST_COUNT) * sizeof(uint16_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(uint8_t), src.data(), src.size() * sizeof(uint8_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_cumulative_histogram_kernel<<<1, 0>>>(low_device, high_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(low.data(), low.size() * sizeof(uint16_t), low_device, low.size() * sizeof(uint16_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(high.data(), high.size() * sizeof(uint16_t), high_device, high.size() * sizeof(uint16_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input", src);
    print_data("BIN0 output", low);
    print_data("BIN1 output", high);
    print_data("BIN0 Golden", golden_low);
    print_data("BIN1 Golden", golden_high);
    const bool passed = compare_data(low, golden_low) && compare_data(high, golden_high);
    std::cout << (passed ? "[Success] asc_cumulative_histogram_bin0/bin1 passed." : "[Failed] asc_cumulative_histogram_bin0/bin1 failed.") << std::endl;
    aclrtFree(high_device);
    aclrtFree(low_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
