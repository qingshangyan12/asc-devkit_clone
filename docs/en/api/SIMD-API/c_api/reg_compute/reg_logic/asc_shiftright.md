# asc_shiftright

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.033898+00:00 pushedAt=2026-10-27T02:26:32.992Z nl=2 -->

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

Performs a right shift on the elements in the source operand vector data register according to the mask, with the number of right shift bits specified by the corresponding elements in the vector data register `shift`, and returns the result through the function return value or writes it to the destination operand. Elements at positions where the mask is 1 participate in the computation, and elements at positions where the mask is 0 are set to zero in the output result. The calculation formula is as follows:

$$
dst_i = src_i \gg shift_i
$$

Depending on the data type of the source operand, the right shift operation is classified into the following two cases:

- **Unsigned integer: A logical right shift is performed.** A logical right shift moves the entire binary number to the right by the specified number of bits, discards the least significant bit, and fills the most significant bit with 0. For example, after the binary number 1010101010101010 (`uint16_t` type) is logically shifted right by 1 bit, the result is 0101010101010101.
- **Signed integer: An arithmetic right shift is performed.** An arithmetic right shift moves the entire binary number to the right by the specified number of bits, discards the least significant bit, and fills the most significant bit with a copy of the sign bit. For example, after the binary number 1010101010101010 (`int16_t` type) is arithmetically shifted right by 1 bit, the result is 1101010101010101; after it is arithmetically shifted right by 3 bits, the result is 1111010101010101.

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```c
// Return the result through the function return value (placeholder form).
__simd_callee__ inline vector_<dtype0> asc_shiftright(vector_<dtype0> src,
                                                      vector_<dtype1> shift,
                                                      vector_bool mask)

// Output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_shiftright(vector_<dtype0>& dst,
                                           vector_<dtype0> src,
                                           vector_<dtype1> shift,
                                           vector_bool mask)
```

### Supported Data Types for dtype0 and dtype1

| `dtype0` | `dtype1` |
|---|---|
| `int8_t` | `int8_t` |
| `uint8_t` | `int8_t` |
| `int16_t` | `int16_t` |
| `uint16_t` | `int16_t` |
| `int32_t` | `int32_t` |
| `uint32_t` | `int32_t` |

### Typical Prototype Example

```c
// Return the result through the function return value.
__simd_callee__ inline vector_uint16_t asc_shiftright(vector_uint16_t src,
                                                      vector_int16_t shift,
                                                      vector_bool mask)

// Output the result through a reference parameter.
__simd_callee__ inline void asc_shiftright(vector_uint16_t& dst,
                                           vector_uint16_t src,
                                           vector_int16_t shift,
                                           vector_bool mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operand (vector data register). Only the void-returning API contains this parameter, and the data type must be the same as `src`. |
| src | Input | Source operand (vector data register). |
| shift | Input | Shift amount (vector data register). The data type must have the same bit width as `src` and be a signed integer type. |
| mask | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. A value of 1 at the corresponding position means the element participates in the computation, and 0 means it does not, with the corresponding element in the output result set to zero. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

For the return-value API, the vector data register that stores the right shift result is returned, and its data type is consistent with `src`.

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- Element positions with a mask bit of 0 do not participate in the right shift operation, and the corresponding positions in the output result are written with 0.

### Shift Constraints

- Elements in `shift` do not support being set to negative numbers; the behavior for negative numbers is undefined.
- For unsigned integers, when the shift amount is greater than the bit width of the `src` data type, the valid elements in the output result are set to zero.
- For signed integers, when the shift amount is greater than the bit width of the `src` data type, if an element in `src` is less than 0, the corresponding position in the output result is written with -1; if an element in `src` is greater than or equal to 0, the corresponding position in the output result is written with 0.

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
constexpr uint32_t ELEMENT_COUNT = 128;
constexpr uint32_t BUFFER_BYTES = ELEMENT_COUNT * sizeof(uint16_t);

__simd_vf__ inline void shiftright_vf(__ubuf__ uint16_t* output,
                                      __ubuf__ uint16_t* input,
                                      __ubuf__ int16_t* shift)
{
    vector_uint16_t src;
    vector_int16_t shift_reg;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    asc_loadalign(src, input);
    asc_loadalign(shift_reg, shift);
    vector_uint16_t dst = asc_shiftright(src, shift_reg, mask);
    asc_storealign(output, dst, mask);
}

__global__ __vector__ void asc_shiftright_kernel(__gm__ uint16_t* output,
                                                 __gm__ uint16_t* input,
                                                 __gm__ int16_t* shift)
{
    asc_init();
    __ubuf__ uint16_t output_local[ELEMENT_COUNT];
    __ubuf__ uint16_t input_local[ELEMENT_COUNT];
    __ubuf__ int16_t shift_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(shift_local, shift, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    shiftright_vf(output_local, input_local, shift_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint16_t> input(ELEMENT_COUNT);
    std::vector<int16_t> shift(ELEMENT_COUNT, 1);
    std::vector<uint16_t> output(ELEMENT_COUNT, 0);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input[i] = static_cast<uint16_t>(i);
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t* input_device = nullptr;
    int16_t* shift_device = nullptr;
    uint16_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&shift_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BUFFER_BYTES, input.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(shift_device, BUFFER_BYTES, shift.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_shiftright_kernel<<<1, 0>>>(output_device, input_device, shift_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BUFFER_BYTES, output_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    bool passed = true;
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        const uint16_t expected = static_cast<uint16_t>(input[i] >> shift[i]);
        if (output[i] != expected) {
            passed = false;
            break;
        }
    }
    std::cout << (passed ? "[Success] asc_shiftright completed."
                         : "[Failed] asc_shiftright output mismatch.")
              << std::endl;
    aclrtFree(input_device);
    aclrtFree(shift_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
