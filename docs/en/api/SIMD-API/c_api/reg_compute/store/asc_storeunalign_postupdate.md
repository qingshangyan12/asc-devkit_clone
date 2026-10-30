# asc_storeunalign_postupdate

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.118491+00:00 pushedAt=2026-10-27T02:26:33.314Z nl=2 -->

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

The header file path is `"c_api/reg_compute/store/storeunalign.h"`.

Contiguously stores data from a vector data register or mask register to a Unified Buffer (UB) address that is not aligned to 32 bytes. Enables Post Update mode to automatically increment the destination address pointer or address register after storing, eliminating the need to manually maintain the destination address during repeated calls within a hardware loop.

For vector data registers, this API provides the following two modes:

- **Immediate offset store mode:** Uses `count` to specify the number of elements to store and updates the `dst` pointer by `count` elements. Must be used with the post-processing API [asc_storeunalign_post_postupdate](asc_storeunalign_post_postupdate.md).
- **Address register offset store mode:** Stores a fixed VL (256 bytes) length. Uses `addr_reg` to specify the iterative offset of the destination address. After the instruction stores the data, the offset in the address register is incremented by VL (256 bytes). Must be used with the post-processing API [asc_storeunalign_post](asc_storeunalign_post.md).

The vector data register store principle is as follows:

Let the start address of the destination operand be `dst_start`, the end address be `dst_end`, and the number of tail block elements be `unalign_count = (dst_end - dst_end / 32 * 32) / sizeof(dtype)`. The stored data is divided into the following two parts:

- The 32-byte-aligned main block part `UB[dst_start, dst_end - unalign_count * sizeof(dtype)]`.
- The tail block part `UB[dst_end - unalign_count * sizeof(dtype), dst_end]` that is not aligned to 32 bytes.

For mask registers, this API provides one mode:

- **Mask register store mode:** When the data type at the UB address is `b16`, the hardware instruction extracts the least significant bit (LSB) from every 2 bits of data and packs the 32B data in the mask register into 16B for writing to UB. After the store is complete, the UB address is updated by a 16B offset. When the data type at the UB address is `b32`, the hardware instruction extracts the least significant bit (LSB) from every 4 bits of data and packs the 32B data in the mask register into 8B for writing to UB. After the store is complete, the UB address is updated by an 8B offset. Must be used with the post-processing API [asc_storeunalign_post](asc_storeunalign_post.md).

This API can concatenate the previous **contiguous tail block** with the main block of the current store and simultaneously update the current tail block data into the unaligned register. An uninitialized unaligned register can be used for the first store. If the tail block of the previous store and the main block of the current store are not contiguous, the post-processing API must be called for each tail block processing. If they are contiguous, the post-processing API needs to be called only for the last tail block processing.

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

### Immediate Offset Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ <dtype>*& dst,
                                                        vector_store_unalign& unalign_reg,
                                                        vector_<dtype> src,
                                                        uint32_t count)
```

#### Data Types Supported by dtype

`dtype` supports the following data types: `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e2m1_t`, `fp4x2_e1m2_t`, `hifloat8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, `float`, and `int64_t`. The corresponding vector data register type is `vector_<dtype>`, where `int4b_t` corresponds to `vector_int4x2_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ uint8_t*& dst,
                                                        vector_store_unalign& unalign_reg,
                                                        vector_uint8_t src,
                                                        uint32_t count)
```

### Address Register Offset Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ <dtype>* dst,
                                                        vector_store_unalign& unalign_reg,
                                                        vector_<dtype> src,
                                                        addr_reg& count)
```

#### Data Types Supported by dtype

`dtype` supports the following data types: `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e2m1_t`, `fp4x2_e1m2_t`, `hifloat8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, `float`, and `int64_t`. The corresponding vector data register type is `vector_<dtype>`, where `int4b_t` corresponds to `vector_int4x2_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ uint8_t* dst,
                                                        vector_store_unalign& unalign_reg,
                                                        vector_uint8_t src,
                                                        addr_reg& count)
```

### Mask Register Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ <dtype>*& dst,
                                                        vector_store_unalign& unalign_reg,
                                                        vector_bool src)
```

#### Data Types Supported by dtype

`dtype` supports the following data types: `uint16_t` and `uint32_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ uint16_t*& dst,
                                                        vector_store_unalign& unalign_reg,
                                                        vector_bool src)
```

## Parameters

### Immediate Offset Store Mode

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Input/output | Start address of the destination operand in UB, of type `__ubuf__ <dtype>*&`. |
| unalign_reg | Input/output | Unaligned register, of type `vector_store_unalign&`, with a length of 32 bytes. |
| src | Input | Source operand (vector data register), of type `vector_<dtype>`. `dtype` must be consistent with the data type pointed to by `dst`. |
| count | Input | Number of elements to store this time, of type `uint32_t`. |

### Address Register Offset Store Mode

**Table 2** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand in UB, of type `__ubuf__ <dtype>*`. |
| unalign_reg | Input/output | Unaligned register, of type `vector_store_unalign&`, with a length of 32 bytes. |
| src | Input | Source operand (vector data register), of type `vector_<dtype>`. `dtype` must be consistent with the data type pointed to by `dst`. |
| count | Input/output | Iterative offset of the destination address, of type `addr_reg&`, pre-constructed by the matching address register construction API. This parameter is used to update the destination address and does not control the amount of data stored in a single operation. After the instruction is executed, the offset in the address register is incremented by one VL (256 bytes). |

### Mask Register Store Mode

**Table 3** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Input/output | Start address of the destination operand in UB, of type `__ubuf__ <dtype>*&`. For the `b16` type, the destination address must be 16-byte aligned; for the `b32` type, the destination address must be 8-byte aligned. After storing, `dst` is automatically updated to the end address of this store operation. |
| unalign_reg | Input/output | Unaligned register, of type `vector_store_unalign&`, with a length of 32 bytes. |
| src | Input | Source operand (mask register), of type `vector_bool`. |

For details about vector data registers, mask registers, unaligned registers, and address registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The UB capacity is capped at 256 KB. The capacity available to users varies with compilation options and programming scenarios (by default, 6 KB of SIMD VF stack and 2 KB of Ascend C reserved space are set aside, leaving 248 KB available; in SIMD+SIMT mixed programming, an additional 32 KB to 128 KB is partitioned as Data Cache, further reducing the available capacity). After the destination operand address is updated, it must not exceed the actual available capacity; otherwise, an error is reported.
- If the UB address ranges accessed by this instruction overlap with those accessed by other instructions, you must insert the synchronization instruction [asc_mem_bar](../reg_sync/asc_mem_bar.md) to ensure that the instructions execute serially and prevent abnormal data from occurring.

### Instruction Constraints

- The same `unalign_reg` must be reused for consecutive calls.
- The mask register contiguous store mode must be finalized using the immediate offset store mode of [asc_storeunalign_post](asc_storeunalign_post.md), reusing the same `unalign_reg`.
- The immediate offset store mode must be finalized using [asc_storeunalign_post_postupdate](asc_storeunalign_post_postupdate.md), reusing the same `unalign_reg`.
- For the address register offset store mode, `addr_reg` must be pre-constructed through [asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md) and assigned for use inside the inner loop body. It must be finalized using [asc_storeunalign_post](asc_storeunalign_post.md), reusing the same `unalign_reg` and address register.
- In immediate offset store mode, the maximum value of `count` must not exceed the number of elements that a single vector data register can hold.
- In vector data register store scenarios, the destination address does not need to be 32-byte aligned, but it must be aligned to `sizeof(dtype)` bytes.
- In mask register store scenarios, the destination address of the `b16` type must be 16-byte aligned, and the destination address of the `b32` type must be 8-byte aligned.

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
constexpr uint32_t ELEMENT_COUNT = 256;
constexpr uint32_t START_OFFSET = 8;
constexpr uint32_t OUTPUT_BYTES = START_OFFSET + ELEMENT_COUNT;

__simd_vf__ inline void storeunalign_postupdate_vf(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_uint8_t src;
    vector_store_unalign unalign_reg;
    __ubuf__ uint8_t* dst = output + START_OFFSET;
    asc_loadalign(src, input);
    asc_storeunalign_postupdate(dst, unalign_reg, src, ELEMENT_COUNT);
    asc_storeunalign_post_postupdate(dst, unalign_reg, 0);
}

__global__ __vector__ void storeunalign_postupdate_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t output_local[288];
    __ubuf__ uint8_t input_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input_local, input, ELEMENT_COUNT);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    storeunalign_postupdate_vf(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> input(ELEMENT_COUNT);
    std::vector<uint8_t> output(OUTPUT_BYTES, 0);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) input[i] = static_cast<uint8_t>(i);
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), ELEMENT_COUNT, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, ELEMENT_COUNT, input.data(), ELEMENT_COUNT, ACL_MEMCPY_HOST_TO_DEVICE);
    storeunalign_postupdate_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    bool passed = true;
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) passed = passed && output[START_OFFSET + i] == input[i];
    std::cout << (passed ? "[Success] asc_storeunalign_postupdate completed."
                         : "[Failed] asc_storeunalign_postupdate output mismatch.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
