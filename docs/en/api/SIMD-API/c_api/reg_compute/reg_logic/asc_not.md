# asc_not

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.027903+00:00 pushedAt=2026-10-27T02:26:32.968Z nl=2 -->

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

The header file path is `"c_api/reg_compute/compute/reg_logic.h"`.

Performs a bitwise NOT operation on the source operand `src` based on `mask`, and returns the result through the function return value or writes it to the destination operand `dst`.

- Vector data register bitwise NOT: Performs a bitwise NOT on the vector data register according to the `dtype` bit width, and the result is a vector data register.

- Mask register bitwise NOT: Performs a bitwise NOT on the mask register, and the result is a mask register.

The calculation formula is as follows:

$$
dst_i = \sim src_i
$$

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

### Vector Data Register Bitwise NOT

```c
// Return the result through the function return value (placeholder form).
__simd_callee__ inline vector_<dtype> asc_not(vector_<dtype> src,
                                              vector_bool mask)

// Output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_not(vector_<dtype>& dst,
                                    vector_<dtype> src,
                                    vector_bool mask)
```

#### Data Types Supported by dtype

`dtype` values: `int8_t`, `uint8_t`, `int16_t`, `uint16_t`, `half`, `int32_t`, `uint32_t`, `float`.

#### Typical Prototype Example

```c
// Example: Perform bitwise NOT on a half vector data register.
__simd_callee__ inline vector_half asc_not(vector_half src,
                                           vector_bool mask)

__simd_callee__ inline void asc_not(vector_half& dst,
                                    vector_half src,
                                    vector_bool mask)
```

### Mask Register Bitwise NOT

```c
// Return the result through the function return value.
__simd_callee__ inline vector_bool asc_not(vector_bool src,
                                           vector_bool mask)

// Output the result through a reference parameter.
__simd_callee__ inline void asc_not(vector_bool& dst,
                                    vector_bool src,
                                    vector_bool mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| :----- | :------- | :------- |
| dst | Output | Destination operand (vector data register or mask register), used to store the NOT result, used only in the prototype without a return value.|
| src | Input | Source operand (vector data register or mask register).|
| mask | Input | Source operand mask (mask register).<br>&bull;When the source operand is a vector data register, elements with a corresponding bit of 1 participate in the computation, and elements with 0 do not. Elements not selected by mask are set to zero in the output.<br>&bull;When the source operand is a mask register, it indicates which bits are valid during the computation. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

- The prototype that returns the result through the function return value returns the bitwise NOT result, with a return type consistent with the data type of `src`.
- The prototype that outputs the result through a reference parameter has no return value.

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The number of elements participating in the computation is determined by the vector length (VL):
    - Number of elements in vector data register bitwise NOT = VL ÷ sizeof(dtype);
    - Number of bits in mask register bitwise NOT = VL.
- When a bit of `mask` is 0, the corresponding bit of the computation result is written as 0.

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

__simd_vf__ inline void compute(__ubuf__ int32_t* dst, __ubuf__ int32_t* src)
{
    vector_int32_t dst_reg;
    vector_int32_t src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);
    asc_not(dst_reg, src_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}

__global__ __vector__ void asc_not_kernel(__gm__ int32_t* dst, __gm__ int32_t* src)
{
    asc_init();
    __ubuf__ int32_t dst_local[ELEMENT_COUNT];
    __ubuf__ int32_t src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(int32_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    compute(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(int32_t));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<int32_t> src(ELEMENT_COUNT);
    std::vector<int32_t> output(ELEMENT_COUNT, 0);
    std::vector<int32_t> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        src[i] = static_cast<int32_t>(0x0f0f0000U + i);
        golden[i] = ~src[i];
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    int32_t* src_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), (ELEMENT_COUNT) * sizeof(int32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    int32_t* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), (ELEMENT_COUNT) * sizeof(int32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(int32_t), src.data(), src.size() * sizeof(int32_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_not_kernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int32_t), dst_device, output.size() * sizeof(int32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input", src);
    print_data("Output", output);
    print_data("Golden", golden);
    const bool passed = compare_data(output, golden);
    std::cout << (passed ? "[Success] asc_not passed." : "[Failed] asc_not failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
