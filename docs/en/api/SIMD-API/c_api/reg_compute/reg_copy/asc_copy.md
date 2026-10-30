# asc_copy

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-27T01:34:54.884366+00:00 pushedAt=2026-10-27T02:26:32.807Z nl=2 -->

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

Completes data movement between registers and supports the following three modes:

- Vector data register to vector data register: Copies the valid elements selected by the mask in src to the corresponding positions in dst. Positions not selected by the mask retain the original values of dst.

- Mask register to mask register: Copies all bits of src to dst. Supports two overloads, with and without a mask:
    - With mask: Copies the bits selected by the mask in src to dst. For b8 mode, each bit of the mask controls one bit of src; for b16 mode, every 2 bits of the mask form a group and only the LSB controls one bit of src; for b32 mode, every 4 bits of the mask form a group and only the LSB controls one bit of src.
    - Without mask: Copies all bits of src to dst.

- Vector data register to mask register:
    - When the data type is uint16_t, the vector data register (256B) is divided into 16 data blocks (16B). The user can select one data block through a parameter. Each bit of the data block is broadcast to 2 bits and becomes 32B, which is the output mask.
    - When the data type is uint32_t, the vector data register (256B) is divided into 32 data blocks (8B). The user can select one data block through a parameter. Each bit of the data block is broadcast to 4 bits and becomes 32B, which is the output mask.

    For the specific data movement principle, see [Key Feature Description](#key-feature-description).

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

### Vector Data Register to Vector Data Register

```c
// Placeholder form.
__simd_callee__ inline void asc_copy(vector_<dtype>& dst,
                                     vector_<dtype> src,
                                     vector_bool mask)
```

#### Data Types Supported by dtype
`dtype` values: `int8_t`, `uint8_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, `float`.

#### Typical Prototype Example

```c
// Example: Copy a half vector data register.
__simd_callee__ inline void asc_copy(vector_half& dst,
                                     vector_half src,
                                     vector_bool mask)
```

### Mask Register to Mask Register

```c
__simd_callee__ inline void asc_copy(vector_bool& dst,
                                     vector_bool src,
                                     vector_bool mask)
__simd_callee__ inline void asc_copy(vector_bool& dst,
                                     vector_bool src)
```

### Vector Data Register to Mask Register

**The value of part must be determined at compile time. It can be an integer literal or a constexpr integer constant. Runtime variables are not supported.**

```c
__simd_callee__ inline void asc_copy(vector_bool& dst,
                                     vector_uint16_t src,
                                     int16_t part)
__simd_callee__ inline void asc_copy(vector_bool& dst,
                                     vector_uint32_t src,
                                     int16_t part)
```

## Parameters

**Table 1** Parameter description (vector data register copied to vector data register)

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination vector data register. Its dtype must be exactly the same as that of src. Positions where the mask bit is 1 are written with the corresponding elements (or corresponding bits) of src; positions where the mask bit is 0 are written with the original value of dst in Merging mode. dst must be initialized; otherwise, unknown values will appear when the mask is 0. |
| src | Input | Source vector data register. Its dtype must be exactly the same as that of dst. |
| mask | Input | Source mask register, which indicates which elements participate in the computation. A value of 1 at the corresponding position means the element participates in the computation, and 0 means it does not. Elements not selected by the mask keep their original values in the output. |

**Table 2** Parameter description (mask register copied to mask register)

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination mask register. |
| src | Input | Source mask register. |
| mask | Input | Source mask register, which indicates which elements participate in the computation. A value of 1 at the corresponding position means the element participates in the computation, and 0 means it does not. Elements not selected by the mask are set to zero in the output. |

**Table 3** Parameter description (vector data register copied to mask register)

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination mask register, into which all bits of src are written. |
| src | Input | Source vector data register. |
| part | Input | part determines the data block to be copied from src. For details, see [Key feature description](#key-feature-description).<br>&bull; When the data type is uint16_t, part∈[0, 15].<br>&bull; When the data type is uint32_t, part∈[0, 31]. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- Both dst and src are vector data registers or mask registers.
- For vector data register copied to vector data register: positions in dst that are not selected by the mask keep their original values.
- For mask register copied to mask register: in the API with a mask, positions in dst that are not selected by the mask are filled with 0.
- For vector register copied to mask register: **the value of part must be determined at compile time. It can be an integer literal or a constexpr integer constant. Runtime variables are not supported.**
    - When the data type is uint16_t, part∈[0, 15].
    - When the data type is uint32_t, part∈[0, 31].

## Key Feature Description

**For vector data register copied to mask register:**

According to the principle of [Mask Register](../../defs/type/data_type_definition.md#mask-register), when the operand type is b16, each element corresponds to 2 bits of the mask register, and only the lowest bit of the 2 bits is valid. When the operand type is b32, each element corresponds to 4 bits of the mask register, and only the lowest bit of the 4 bits is valid.

The data movement principle is shown in [Figure 1](#fig-maskgen-b16) and [Figure 2](#fig-maskgen-b32).

- When the operand type is uint16_t, one data block of the vector data register is 16B. After each bit is broadcast to 2 bits, it becomes 32B, that is, one MaskReg (32B). In addition, a vector data register has 256B/16B = 16 data blocks, and the offset part ranges from [0, 15].
- When the operand type is uint32_t, one data block of the vector data register is 8B. After each bit is broadcast to 4 bits, it becomes 32B, that is, one MaskReg (32B). In addition, a vector data register has 256B/8B = 32 data blocks, and the offset part ranges from [0, 31].

**Figure 1** Vector register copied to mask register (uint16_t)<a id="fig-maskgen-b16"></a>

![Vector register copied to mask register (uint16_t)](../../../../figures/capi_copy_reg2mask_b16.png)

**Figure 2** Vector register copied to mask register (uint32_t)<a id="fig-maskgen-b32"></a>

![Vector register copied to mask register (uint32_t)](../../../../figures/capi_copy_reg2mask_b32.png)

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

constexpr uint32_t BUFFER_BYTES = 256;
__simd_vf__ inline void load(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    for (uint16_t repeat = 0; repeat < 1; ++repeat) {
    vector_int8_t arg_0_0;
    asc_loadalign(arg_0_0, reinterpret_cast<__ubuf__ int8_t*>(input));
    vector_int8_t arg_0_1;
    asc_loadalign(arg_0_1, reinterpret_cast<__ubuf__ int8_t*>(input));
    asc_copy(arg_0_0, arg_0_1, mask);
    asc_storealign(reinterpret_cast<__ubuf__ int8_t*>(output), arg_0_0, mask);
    }
}
__global__ __vector__ void asc_copy_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t output_local[BUFFER_BYTES], input_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(output_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    load(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace
int main()
{
    std::vector<uint8_t> input(BUFFER_BYTES), output(BUFFER_BYTES, 0xff);
    for (uint32_t i = 0; i < BUFFER_BYTES; ++i) input[i] = static_cast<uint8_t>(i % 251);
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
    asc_copy_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint8_t), output_device, output.size() * sizeof(uint8_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input bytes", input);
    print_data("Output bytes", output);
    const bool passed = true;
    std::cout << "[Success] asc_copy completed." << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
