# asc_loadalign_brc_elem2datablock_postupdate

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:33.772997+00:00 pushedAt=2026-10-27T02:26:31.965Z nl=2 -->

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

The header file path is `"c_api/reg_compute/load/loadalign.h"`.

Reads 8 consecutive elements from the Unified Buffer (UB) at a 16-byte-aligned (b16 type) or 32-byte-aligned (b32 type) start address and broadcasts each element to a corresponding `DataBlock` (32 bytes) in the destination vector data register. The data format and content remain unchanged during the load. This API uses the Post Update mode and automatically updates the source address pointer after the load is complete.

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

```c
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(vector_<dtype>& dst,
                                                                        __ubuf__ <dtype>*& src,
                                                                        int32_t offset)
```

### Data Types Supported by dtype

The data types supported by dtype are `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

### Typical Prototype Example

```c
// Example: half type.
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(vector_half& dst,
                                                                        __ubuf__ half*& src,
                                                                        int32_t offset)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Destination vector data register. The dtype must be the same as `src`, and VL bytes of data are loaded. |
| src | Input/output | Source UB address. The actual read address must meet the alignment requirement: the b16 type must be 16-byte aligned, and the b32 type must be 32-byte aligned. |
| offset | Input | Post Update stride, in elements. After the load from the current `src` address is complete, the API updates `src`; `src` increases by `offset × sizeof(dtype)` bytes. |

For details about vector data registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The current actual read address and the next actual read address after the Post Update must meet the corresponding alignment requirements: the b16 type must be 16-byte aligned, and the b32 type must be 32-byte aligned. The actual read range must be within the UB address space without exceeding the boundary; otherwise, an error is reported.
- The UB capacity is capped at 256 KB. The capacity available to users varies with compilation options and programming scenarios (by default, 6 KB of SIMD VF stack and 2 KB of Ascend C reserved space are set aside, leaving 248 KB available; in SIMD+SIMT mixed programming, an additional 32 KB to 128 KB is partitioned as Data Cache, further reducing the available capacity). The UB address after applying the offset must not exceed the actual available capacity; otherwise, an error is reported.
- If the UB address ranges accessed by this instruction overlap with those accessed by other instructions, you must insert the synchronization instruction [asc_mem_bar](../reg_sync/asc_mem_bar.md) to ensure that the instructions execute serially and prevent abnormal data from occurring.

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
__aicore__ constexpr uint32_t ceil_division(uint32_t numerator, uint32_t denominator)
{
    return (numerator + denominator - 1) / denominator;
}

constexpr uint32_t INPUT_ELEMENTS = 16;
constexpr uint32_t OUTPUT_ELEMENTS = 256;
constexpr uint32_t INPUT_BYTES = INPUT_ELEMENTS * sizeof(uint16_t);
constexpr uint32_t OUTPUT_BYTES = OUTPUT_ELEMENTS * sizeof(uint16_t);

__simd_vf__ inline void asc_loadalign_brc_elem2datablock_postupdate_vf(
    __ubuf__ uint16_t* output, __ubuf__ uint16_t* input, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    vector_uint16_t dst_reg;
    __ubuf__ uint16_t* src = input;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        asc_loadalign_brc_elem2datablock_postupdate(dst_reg, src, 8);
        asc_storealign(output + i * one_repeat_size, dst_reg, mask);
    }
}

__global__ __vector__ void asc_loadalign_brc_elem2datablock_postupdate_kernel(
    __gm__ uint16_t* output, __gm__ uint16_t* input)
{
    asc_init();
    __ubuf__ uint16_t input_local[INPUT_ELEMENTS];
    __ubuf__ uint16_t output_local[OUTPUT_ELEMENTS];
    asc_copy_gm2ub_align(input_local, input, INPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    uint16_t one_repeat_size = asc_get_vf_len() / sizeof(uint16_t);
    uint16_t repeat_time = ceil_division(OUTPUT_ELEMENTS, one_repeat_size);
    asc_vf_call<asc_loadalign_brc_elem2datablock_postupdate_vf>(output_local, input_local, one_repeat_size, repeat_time);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint16_t> input(INPUT_ELEMENTS);
    std::vector<uint16_t> output(OUTPUT_ELEMENTS, 0xffff);
    std::vector<uint16_t> golden(OUTPUT_ELEMENTS, 0);
    for (uint16_t i = 0; i < INPUT_ELEMENTS; ++i) input[i] = static_cast<uint16_t>(i + 1);
    for (uint16_t i = 0; i < OUTPUT_ELEMENTS; ++i) golden[i] = input[i / 16];

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t* input_device = nullptr;
    uint16_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_loadalign_brc_elem2datablock_postupdate_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success]" : "[Failed]") << " asc_loadalign_brc_elem2datablock_postupdate example." << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
