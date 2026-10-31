# asc_extract_mask

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:33.998365+00:00 pushedAt=2026-10-27T02:26:32.813Z nl=2 -->

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

The header file path is `"c_api/reg_compute/reg_copy.h"`.

Extracts a mask register from a vector data register and returns the result through the function return value.

- When the data type is uint16_t, the vector data register (256B) is divided into 16 data blocks (16B). The user can select a data block through a parameter. After each bit of the data block is broadcast to 2 bits, it becomes 32B, which is the output mask.
- When the data type is uint32_t, the vector data register (256B) is divided into 32 data blocks (8B). You can select a data block through a parameter. Each bit of the data block is broadcast to 4 bits to become 32B, which is the output mask.

For the detailed data movement principle, see [Key Feature Description of asc_copy](asc_copy.md#key-feature-description).

This API has the same function as the vector data register to mask register copy mode of [asc_copy](asc_copy.md), except that this API returns the result through the function return value.

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

**The value of part must be determined at compile time. It can be an integer literal or a constexpr integer constant. Runtime variables are not supported.**

```c
__simd_callee__ inline vector_bool asc_extract_mask(vector_uint16_t src,
                                                     int16_t part)
__simd_callee__ inline vector_bool asc_extract_mask(vector_uint32_t src,
                                                     int16_t part)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| src | Input | Source vector data register. |
| part | Input | part determines the data block to be moved from src. For details, see [Key feature description of asc_copy](asc_copy.md#key-feature-description).<br>&bull; When the data type is uint16_t, part ∈ [0, 15].<br>&bull; When the data type is uint32_t, part ∈ [0, 31]. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

Returns the mask register that stores the extraction result, of type `vector_bool`.

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- src is a vector data register.
- **The value of part must be determined at compile time. It can be an integer literal or a constexpr integer constant. Runtime variables are not supported.**
    - When the data type is uint16_t, part∈[0, 15].
    - When the data type is uint32_t, part∈[0, 31].

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

constexpr uint32_t ELEMENT_COUNT = 128;
__simd_vf__ inline void extract_mask(__ubuf__ uint32_t* output, __ubuf__ uint16_t* input)
{
    vector_uint16_t src_reg;
    asc_loadalign(src_reg, input);
    vector_bool mask = asc_extract_mask(src_reg, 0);
    asc_storealign(output, mask);
}
__global__ __vector__ void asc_extract_mask_kernel(__gm__ uint32_t* output, __gm__ uint16_t* input)
{
    asc_init();
    __ubuf__ uint32_t output_local[ELEMENT_COUNT];
    __ubuf__ uint16_t input_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input_local, input, ELEMENT_COUNT * sizeof(uint16_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    extract_mask(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync();
}
} // namespace
int main()
{
    std::vector<uint16_t> input(ELEMENT_COUNT);
    std::vector<uint32_t> output(ELEMENT_COUNT, 0);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) input[i] = static_cast<uint16_t>(i);
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t* input_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), (ELEMENT_COUNT) * sizeof(uint16_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint32_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), (ELEMENT_COUNT) * sizeof(uint32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, input.size() * sizeof(uint16_t), input.data(), input.size() * sizeof(uint16_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_extract_mask_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint32_t), output_device, output.size() * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input", input);
    print_data("Output", output);
    const bool passed = true;
    std::cout << "[Success] asc_extract_mask completed." << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
