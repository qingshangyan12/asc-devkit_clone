# asc_squeeze_and_storeunalign

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.056273+00:00 pushedAt=2026-10-27T02:26:33.085Z nl=2 -->

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

Reg computation data movement API. Squeezes the elements selected by the mask `squeeze_mask` from the source vector data register `src1` into the destination vector data register `dst`, and stores these elements contiguously to the Unified Buffer (UB). The squeezed elements are arranged contiguously from the low bits of `dst` in their order in `src1`, and the remaining elements in `dst` are set to 0.

This API uses the AR special register to record the total number of bytes of valid data currently stored contiguously. Before calling this API for the first time, call [asc_squeeze_and_storeunalign_init](asc_squeeze_and_storeunalign_init.md) to clear the AR register. Before the API executes, the value in the AR register indicates the write offset relative to `dst_addr`, in bytes; after the API executes, the total number of bytes of the elements selected this time is accumulated into the AR register. By calling this API consecutively while keeping `dst_addr` and `src0` unchanged, multiple groups of filtered results can be written to the UB contiguously without manually updating the address. The value in the AR register can be obtained through the [asc_get_squeeze_status](../../spr/asc_get_squeeze_status.md) API.

The API writes main blocks that can completely form 32B to the UB, and temporarily stores tail blocks smaller than 32B in the unaligned register `src0`. After one or more consecutive store operations, call [asc_squeeze_and_storeunalign_finalize](asc_squeeze_and_storeunalign_finalize.md) to write the tail block to the UB.

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

```c
__simd_callee__ inline void asc_squeeze_and_storeunalign(__ubuf__ <dtype>* dst_addr,
                                                         vector_<dtype>& dst,
                                                         vector_store_unalign& src0,
                                                         vector_<dtype> src1,
                                                         vector_bool squeeze_mask)
```

### Data Types Supported by dtype

`int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e2m1_t`, `fp4x2_e1m2_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, `float`.

### Typical Prototype Example

```c
__simd_callee__ inline void asc_squeeze_and_storeunalign(__ubuf__ float* dst_addr,
                                                         vector_float& dst,
                                                         vector_store_unalign& src0,
                                                         vector_float src1,
                                                         vector_bool squeeze_mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst_addr | Output | Start address of the destination operand in the UB. The address does not need to be 32B-aligned, but must be aligned to `sizeof(dtype)` bytes. During consecutive stores, the actual write position is `dst_addr` plus the byte offset recorded in the AR register. The API does not modify the value of `dst_addr`. |
| dst | Output | Destination operand (vector data register), which returns the squeezed result by reference. The selected elements are arranged contiguously in the low bits, and the remaining elements are set to 0. |
| src0 | Input/output | Unaligned register with a length of 32B, used to temporarily store tail blocks smaller than 32B. The same register must be used for consecutive stores and finalization; the register does not need to be initialized before the first call. |
| src1 | Input | Source operand (vector data register). |
| squeeze_mask | Input | Source operand mask (mask register). When the corresponding position is 1, the element is selected; when it is 0, the element is not selected. |

For details about vector data registers, mask registers, and unaligned registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The UB capacity upper limit is 256 KB, and the user-available capacity varies with compilation options and programming scenarios. The sum of `dst_addr` and the accumulated byte count recorded in the AR register must not exceed the actual available capacity; otherwise, a write out-of-bounds exception is triggered.
- If this API overlaps with other instructions in the UB address space, insert the synchronization instruction [asc_mem_bar](../reg_sync/asc_mem_bar.md) to serialize the multiple instructions.

### Instruction Constraints

- Before each group of consecutive store operations starts, call [asc_squeeze_and_storeunalign_init](asc_squeeze_and_storeunalign_init.md) once. When calling this API consecutively, keep `dst_addr` and `src0` unchanged; after the last call, call [asc_squeeze_and_storeunalign_finalize](asc_squeeze_and_storeunalign_finalize.md) once to complete finalization.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENTS_PER_REGISTER = 64;
constexpr uint32_t REPEAT_COUNT = 2;
constexpr uint32_t SELECTED_PER_REGISTER = 22;
constexpr uint32_t INPUT_COUNT = ELEMENTS_PER_REGISTER * REPEAT_COUNT;
constexpr uint32_t OUTPUT_COUNT = SELECTED_PER_REGISTER * REPEAT_COUNT;

__simd_vf__ inline void pack_selected(
    __ubuf__ float* output, __ubuf__ float* register_output, __ubuf__ float* input)
{
    vector_float src;
    vector_float dst;
    vector_store_unalign unalign_reg;
    vector_bool squeeze_mask = asc_create_mask_b32(PAT_M3);
    vector_bool store_mask = asc_create_mask_b32(PAT_ALL);

    asc_squeeze_and_storeunalign_init();
    for (uint32_t repeat = 0; repeat < REPEAT_COUNT; ++repeat) {
        asc_loadalign(src, input + repeat * ELEMENTS_PER_REGISTER);
        asc_squeeze_and_storeunalign(output, dst, unalign_reg, src, squeeze_mask);
    }
    asc_squeeze_and_storeunalign_finalize(output, unalign_reg);
    asc_storealign(register_output, dst, store_mask);
}

__global__ __vector__ void squeeze_and_store_kernel(
    __gm__ float* output, __gm__ float* register_output, __gm__ float* input)
{
    asc_init();
    __ubuf__ float input_local[INPUT_COUNT];
    __ubuf__ float output_local[ELEMENTS_PER_REGISTER];
    __ubuf__ float register_local[ELEMENTS_PER_REGISTER];
    asc_copy_gm2ub_align(input_local, input, INPUT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    pack_selected(output_local, register_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, OUTPUT_COUNT * sizeof(float));
    asc_copy_ub2gm_align(register_output, register_local, ELEMENTS_PER_REGISTER * sizeof(float));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> input(INPUT_COUNT);
    std::vector<float> output(OUTPUT_COUNT, 0.0F);
    std::vector<float> register_output(ELEMENTS_PER_REGISTER, 0.0F);
    std::vector<float> golden;
    std::vector<float> register_golden(ELEMENTS_PER_REGISTER, 0.0F);
    for (uint32_t i = 0; i < INPUT_COUNT; ++i) input[i] = static_cast<float>(i) + 0.5F;
    for (uint32_t repeat = 0; repeat < REPEAT_COUNT; ++repeat) {
        for (uint32_t i = 0; i < ELEMENTS_PER_REGISTER; i += 3) {
            golden.push_back(input[repeat * ELEMENTS_PER_REGISTER + i]);
        }
    }
    std::copy(golden.end() - SELECTED_PER_REGISTER, golden.end(), register_golden.begin());

    aclInit(nullptr);
    aclrtSetDevice(0);
    float* input_device = nullptr;
    float* output_device = nullptr;
    float* register_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), input.size() * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&register_device), register_output.size() * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, input.size() * sizeof(float), input.data(), input.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    squeeze_and_store_kernel<<<1, 0>>>(output_device, register_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(register_output.data(), register_output.size() * sizeof(float), register_device,
        register_output.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden && register_output == register_golden;
    std::cout << (passed ? "[Success] squeeze and unaligned store completed."
                         : "[Failed] output mismatch.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtFree(register_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
