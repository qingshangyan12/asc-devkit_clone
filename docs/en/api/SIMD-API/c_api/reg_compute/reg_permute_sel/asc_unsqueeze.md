# asc_unsqueeze

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-27T01:34:54.951942+00:00 pushedAt=2026-10-27T02:26:33.112Z nl=2 -->

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

The header file path is `"c_api/reg_compute/compute/reg_permute_sel.h"`.

Decompresses the data in dst based on mask, or returns the decompression result through the function return value. Decompression method: the 0th element of dst is set to 0, and the i-th element of dst equals the number of 1s in mask from the 0th to the (i-1)-th element. The most significant bit of mask is ignored and does not participate in the statistics.

The specific algorithm is shown in Figure 1. The first element of dst is 0. For subsequent elements, when the valid mask bit corresponding to dst[i-1] is 1, the value of dst[i] is dst[i-1] + 1; when the corresponding valid mask bit is 0, the value of dst[i] is dst[i-1].

**Figure 1** unsqueeze process

![unsqueeze process](../../../../figures/asc_unsqueeze.png)

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```c
// Return the result through the function return value.
__simd_callee__ inline vector_uint8_t asc_unsqueeze_u8(vector_bool mask)
__simd_callee__ inline vector_int8_t asc_unsqueeze_s8(vector_bool mask)
__simd_callee__ inline vector_uint16_t asc_unsqueeze_u16(vector_bool mask)
__simd_callee__ inline vector_int16_t asc_unsqueeze_s16(vector_bool mask)
__simd_callee__ inline vector_uint32_t asc_unsqueeze_u32(vector_bool mask)
__simd_callee__ inline vector_int32_t asc_unsqueeze_s32(vector_bool mask)

// Output the result through a reference parameter.
__simd_callee__ inline void asc_unsqueeze(vector_<dtype>& dst,
                                          vector_bool mask)
```

#### Data Types Supported by dtype
Data types supported by dtype: int8_t, uint8_t, int16_t, uint16_t, int32_t, and uint32_t. The return-value API distinguishes data types by function name suffix. The correspondence is as follows:

| Function Name Suffix | Data Type |
|---|---|
| u8 | uint8_t |
| s8 | int8_t |
| u16 | uint16_t |
| s16 | int16_t |
| u32 | uint32_t |
| s32 | int32_t |

#### Typical Prototype Example

```c
// Example: Decompress an int8_t vector data register and return the result through the function return value.
__simd_callee__ inline vector_int8_t asc_unsqueeze_s8(vector_bool mask)

// Example: Decompress an int8_t vector data register.
__simd_callee__ inline void asc_unsqueeze(vector_int8_t& dst,
                                          vector_bool mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Input/output | Source and destination vector data register. This parameter is included only in the no-return-value API. The API writes the count result during mask prefix counting. |
| mask | Input | Source mask register, used to provide decompression information for dst. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

For the return-value API, the vector data register that stores the decompression result is returned, and its data type is determined by the function name suffix (see the correspondence table in the prototype).

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- dst is a vector data register, and mask is a mask register.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510&& ./main
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

template <typename T>
bool compare_range_data(const std::vector<T>& actual, const std::vector<T>& expected,
    size_t begin, size_t count, double tolerance = 0.0)
{
    if (begin + count > actual.size() || begin + count > expected.size()) return false;
    for (size_t i = begin; i < begin + count; ++i) {
        if (actual[i] == expected[i]) continue;
        const double diff = static_cast<double>(actual[i]) - static_cast<double>(expected[i]);
        if (diff > tolerance || diff < -tolerance) return false;
    }
    return true;
}

constexpr uint32_t ELEMENT_COUNT = 64;
__simd_vf__ inline void unsqueeze(__ubuf__ uint32_t* dst)
{
    vector_uint32_t dst_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_unsqueeze(dst_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}
__global__ __vector__ void asc_unsqueeze_kernel(__gm__ uint32_t* dst)
{
    asc_init();
    __ubuf__ uint32_t dst_local[ELEMENT_COUNT];
    unsqueeze(dst_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync();
}
} // namespace
int main()
{
    std::vector<uint32_t> input = {ELEMENT_COUNT};
    std::vector<uint32_t> output(ELEMENT_COUNT, 0), golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) golden[i] = i;
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), (ELEMENT_COUNT) * sizeof(uint32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    asc_unsqueeze_kernel<<<1, 0>>>(dst_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint32_t), dst_device, output.size() * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input mask count", input);
    print_data("Output", output);
    print_data("Golden", golden);
    const bool passed = compare_data(output, golden);
    std::cout << (passed ? "[Success] asc_unsqueeze passed." : "[Failed] asc_unsqueeze failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
