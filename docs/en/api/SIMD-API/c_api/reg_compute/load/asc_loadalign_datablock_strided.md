# asc_loadalign_datablock_strided

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:33.777020+00:00 pushedAt=2026-10-27T02:26:31.976Z nl=2 -->

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

Loads 8 `DataBlock`s non-contiguously from the Unified Buffer (UB) at a 32-byte-aligned start address and returns the vector data register that stores the loaded result through the function return value. Each `DataBlock` has a data size of 32 bytes. You can configure the address stride between adjacent data blocks and the start read position of this load.

This API provides the same functionality as the non-contiguous aligned load mode of [asc_loadalign](asc_loadalign.md), except that this API returns the result through the function return value.

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

```c
__simd_callee__ inline vector_<dtype> asc_loadalign_datablock_strided(__ubuf__ <dtype>* src,
                                                                      uint16_t block_stride,
                                                                      uint16_t repeat_stride,
                                                                      vector_bool mask)
```

### Data Types Supported by dtype

The data types supported by dtype are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e2m1_t`, `fp4x2_e1m2_t`, `hifloat8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`. When dtype is `int4b_t`, the actual type of the return value is `vector_int4x2_t`.

### Typical Prototype Example

```c
// Example: uint8_t type.
__simd_callee__ inline vector_uint8_t asc_loadalign_datablock_strided(__ubuf__ uint8_t* src,
                                                                      uint16_t block_stride,
                                                                      uint16_t repeat_stride,
                                                                      vector_bool mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| src | Input | Source UB address. The actual read address must be aligned to 32 bytes. |
| block_stride | Input | Stride between the start addresses of adjacent `DataBlock` units in the source operand, in 32-byte units. |
| repeat_stride | Input | Offset of the start read address for this load relative to `src`, in 32-byte units. The actual start read address is `src` offset by `repeat_stride × 32` bytes. |
| mask | Input | Mask register that indicates which elements participate in the computation. This API uses `DataBlock` as the data movement unit.<br>&bull; When any element in a `DataBlock` is selected as valid by `mask`, all data in that `DataBlock` is loaded into the vector data register.<br>&bull; When all elements in a `DataBlock` are selected as invalid by `mask`, the data in that `DataBlock` is not loaded into the vector data register, and the elements at the corresponding positions are set to 0. No error is reported even if the UB access is out of bounds. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

Returns the vector data register that stores the non-contiguous aligned load result, with the data type consistent with `src`.

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The actual read address must be 32-byte aligned, and the read range of valid `DataBlock`s must be within the UB address space without exceeding the boundary; otherwise, an error is reported.
- When all elements in a `DataBlock` are set to invalid by `mask`, no error is reported even if the `DataBlock` is out of bounds.
- If the UB address ranges accessed by this instruction overlap with those accessed by other instructions, you must insert the synchronization instruction [asc_mem_bar](../reg_sync/asc_mem_bar.md) to ensure that the instructions execute serially.

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
constexpr uint32_t DATABLOCK_BYTES = 32;
constexpr uint32_t DATABLOCK_COUNT = 8;
constexpr uint16_t BLOCK_STRIDE = 2;
constexpr uint16_t REPEAT_STRIDE = 1;
constexpr uint32_t INPUT_BYTES = 16 * DATABLOCK_BYTES;
constexpr uint32_t OUTPUT_BYTES = DATABLOCK_COUNT * DATABLOCK_BYTES;

__simd_vf__ inline void asc_loadalign_datablock_strided_vf(
    __ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    vector_uint8_t dst = asc_loadalign_datablock_strided(input, BLOCK_STRIDE, REPEAT_STRIDE, mask);
    asc_storealign(output, dst, mask);
}

__global__ __vector__ void asc_loadalign_datablock_strided_kernel(
    __gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t input_local[INPUT_BYTES];
    __ubuf__ uint8_t output_local[OUTPUT_BYTES];
    asc_copy_gm2ub_align(input_local, input, INPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_vf_call<asc_loadalign_datablock_strided_vf>(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> input(INPUT_BYTES);
    std::vector<uint8_t> output(OUTPUT_BYTES, 0xff);
    std::vector<uint8_t> golden(OUTPUT_BYTES, 0);
    for (uint32_t i = 0; i < INPUT_BYTES; ++i) input[i] = static_cast<uint8_t>(i % 251 + 1);
    for (uint32_t block = 0; block < DATABLOCK_COUNT; ++block) {
        const uint32_t src_offset = (REPEAT_STRIDE + block * BLOCK_STRIDE) * DATABLOCK_BYTES;
        const uint32_t dst_offset = block * DATABLOCK_BYTES;
        for (uint32_t i = 0; i < DATABLOCK_BYTES; ++i) {
            golden[dst_offset + i] = input[src_offset + i];
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_loadalign_datablock_strided_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success]" : "[Failed]")
              << " asc_loadalign_datablock_strided example." << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
