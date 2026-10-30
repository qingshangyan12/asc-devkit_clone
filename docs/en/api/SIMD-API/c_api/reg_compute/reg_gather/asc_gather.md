# asc_gather

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-27T01:34:54.905910+00:00 pushedAt=2026-10-27T02:26:32.905Z nl=2 -->

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

The header file path is `"c_api/reg_compute/compute/reg_gather.h"`.

Collects the source operand `src` into the destination operand `dst` element-wise based on the index position `index`.

**Register source gather mode**: Both the source operand and the destination operand are vector data registers. Based on the element-wise indices stored in the `index` vector register, elements at the corresponding positions are selected from the `src` vector data register and written to the corresponding positions of `dst`. No mask is used.

**Figure 1** Register source gather mode

![Register source gather mode](../../../../figures/asc_gather_vec.png)

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```c
// Output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_gather(vector_<dtype>& dst,
                                       vector_<dtype> src,
                                       vector_<index_dtype> index)

// Return the result through the function return value (placeholder form).
__simd_callee__ inline vector_<dtype> asc_gather(vector_<dtype> src,
                                                 vector_<index_dtype> index)
```

### Supported Data Types
**Table 1** Supported data types for register source gather mode

| dtype | index_dtype |
| :---------- | :-------------- |
| int8_t | uint8_t |
| uint8_t | uint8_t |
| hifloat8_t | uint8_t |
| fp8_e8m0_t | uint8_t |
| fp8_e5m2_t | uint8_t |
| fp8_e4m3fn_t | uint8_t |
| int16_t | uint16_t |
| uint16_t | uint16_t |
| half | uint16_t |
| bfloat16_t | uint16_t |
| int32_t | uint32_t |
| uint32_t | uint32_t |
| float | uint32_t |

### Typical Prototype Example

```c
// Example: dtype/index_dtype data types are int16_t/uint16_t, respectively.
// Output the result through a reference parameter.
__simd_callee__ inline void asc_gather(vector_int16_t& dst,
                                       vector_int16_t src,
                                       vector_uint16_t index)

// Return the result through the function return value.
__simd_callee__ inline vector_int16_t asc_gather(vector_int16_t src,
                                                 vector_uint16_t index)
```

## Parameters

**Table 2** Parameters

| Parameter | Input/Output | Description |
| :----- | :------- | :------- |
| dst | Output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). The dtype must be the same as dst. |
| index | Input | Data index (vector data register). The unit is the number of elements, dst[i] = src[index[i]]. |

For details about vector data registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

- The prototype that outputs the result through a reference parameter has no return value.
- The function prototype that outputs the result through the return value returns the gathered result. The return value type is the same as the type of the `dst` parameter in the corresponding reference output function prototype (with the reference removed).

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- Both the source and destination are vector data registers.

### Register Source Gather Mode Constraints

- `src` is of vector data register type, with a fixed bit width of [Vector Length (VL)](../../defs/type/data_type_definition.md) and a fixed number of stored elements. If an index value in `index` exceeds the maximum number of elements that can be stored in the current vector data register, it is handled as follows: let the maximum number of data elements that can be stored in the current vector data register be `VL_T`, and let the index value in `index` be `i`; the index value is updated to `i % VL_T`.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md).

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

constexpr uint32_t ELEMENT_COUNT = 128;

__simd_vf__ inline void gather_vf(__ubuf__ uint16_t* output, __ubuf__ uint16_t* input)
{
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    vector_uint16_t src;
    asc_loadalign(src, input);
    vector_uint16_t index;
    asc_loadalign(index, input);
    vector_uint16_t dst;
    asc_gather(dst, src, index);
    asc_storealign(output, dst, mask);
}

__global__ __vector__ void asc_gather_kernel(__gm__ uint16_t* output, __gm__ uint16_t* input)
{
    asc_init();
    __ubuf__ uint16_t output_local[ELEMENT_COUNT];
    __ubuf__ uint16_t input_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input_local, input, ELEMENT_COUNT * sizeof(uint16_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    gather_vf(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, ELEMENT_COUNT * sizeof(uint16_t));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint16_t> input(ELEMENT_COUNT);
    std::vector<uint16_t> output(ELEMENT_COUNT, 0xff);
    std::vector<uint16_t> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input[i] = static_cast<uint16_t>(ELEMENT_COUNT - 1 - i);
    }
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        golden[i] = input[input[i]];
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t* input_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), (ELEMENT_COUNT) * sizeof(uint16_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint16_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), (ELEMENT_COUNT) * sizeof(uint16_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, input.size() * sizeof(uint16_t), input.data(), input.size() * sizeof(uint16_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_gather_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint16_t), output_device, output.size() * sizeof(uint16_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input bytes", input);
    print_data("Output bytes", output);
    print_data("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_gather completed." : "[Failed] asc_gather output mismatch.")
              << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
