# asc_storealign_pack_postupdate

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T04:27:26.842632+00:00 pushedAt=2026-10-27T02:26:33.248Z nl=2 -->

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

The header file path is `"c_api/reg_compute/store/storealign.h"`.

Compresses and stores data from a vector data register or mask register to the Unified Buffer (UB). After the write is complete, Post Update mode is enabled to automatically increment the destination address pointer, eliminating the need to manually maintain the destination address across multiple consecutive calls within a hardware loop. When the source operand is a vector data register, `mask` indicates the elements that participate in the store. When the corresponding `mask` bit is 1, the lower half bits of the valid element are written to the corresponding compressed destination position. When the corresponding `mask` bit is 0, the corresponding compressed destination position retains its original value. When the source operand is a mask register, every other bit is discarded, and the retained bits are contiguously compressed and stored to the destination address. Configuring a mask is not supported in the mask register store scenario. The destination operand is a UB address.

This API provides the following two function modes:

- **Vector data register store mode**: The unit of `offset` is elements. After the store is complete, the destination address is incremented by `offset × sizeof(dtype)` bytes.
- **Mask register store mode**: The unit of `offset` is bytes. After the store is complete, the destination address is incremented by `offset` bytes.

The following figure illustrates an example of **compressing data from b32 to b16 and storing it**:

**Figure 1** Compressed store data

![](../../../../figures/reg_store_pack.png)

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

### Vector Data Register Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ <dtype>*& dst,
                                                           vector_<dtype> src,
                                                           int32_t offset,
                                                           vector_bool mask)
```

#### Data Types Supported by dtype

The data types supported by `dtype` are `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, `float`, `int64_t`, and `uint64_t`. The corresponding vector register type is `vector_<dtype>`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint32_t*& dst,
                                                           vector_uint32_t src,
                                                           int32_t offset,
                                                           vector_bool mask)
```

### Mask Register Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ <dtype>*& dst,
                                                           vector_bool src,
                                                           int32_t offset)
```

#### Data Types Supported by dtype

The data types supported by `dtype` are `uint8_t`, `uint16_t`, and `uint32_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint8_t*& dst,
                                                           vector_bool src,
                                                           int32_t offset)
```


## Parameters

### Vector Data Register Store Mode

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Input/output | Start address of the destination operand, passed by pointer reference. The type is `__ubuf__ <dtype>*&`, and the start address must be 32-byte aligned. After the store is complete, this pointer is automatically updated by hardware. |
| src | Input | Source operand (vector data register). `dtype` must be the same as `dst`. |
| offset | Input | Address offset, of type `int32_t`, in elements. After the API is executed, the destination address is incremented by `offset × sizeof(dtype)` bytes. |
| mask | Input | Source operand mask (mask register), used to indicate the elements that participate in the data movement. A value of 1 at the corresponding position indicates participation, and 0 indicates non-participation. |

### Mask Register Store Mode

**Table 2** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Input/output | Start address of the destination operand, passed by pointer reference. The type is `__ubuf__ <dtype>*&`, and the start address must be 16-byte aligned. After the store is complete, this pointer is automatically updated by hardware. |
| src | Input | Source operand (mask register). |
| offset | Input | Address offset, of type `int32_t`, in bytes. After the API is executed, the underlying UB address of the `dst` pointer is updated by `offset` bytes. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The source operand is a vector data register or mask register, and the destination operand is a UB address. Pointers outside the UB address space cannot be passed as the destination address.
- The total UB capacity is 256 KB. The capacity available to users varies with compilation options and programming scenarios (by default, 6 KB of SIMD VF stack and 2 KB of Ascend C reserved space are set aside, leaving 248 KB available; in SIMD+SIMT mixed programming, an additional 32 KB to 128 KB is partitioned as Data Cache, further reducing the available capacity). The write range of the destination operand and the address after Post Update must not exceed the actual available capacity; otherwise, an error is reported.
- If the UB address ranges accessed by this instruction overlap with those accessed by other instructions, you must insert the synchronization instruction [asc_mem_bar](../reg_sync/asc_mem_bar.md) to ensure that the instructions execute serially and prevent abnormal data from occurring.

### Vector Data Register Packed Store Scenario

- The start address of `dst` must be 32-byte aligned.
- The content of an unassigned mask register is indeterminate, which may cause incorrect valid element positions.
- Source operand elements whose `mask` bit is 1 participate in the store, and their lower half bits are written to the corresponding compressed destination positions. Elements whose bit is 0 do not participate in the store, and the corresponding compressed destination positions retain their original values.
- The unit of `offset` is elements. The new `dst` address after Post Update must still be 32-byte aligned, and both the current write range and the updated address must fall within the available UB capacity.

### Mask Register Packed Store Scenario

- The start address of `dst` must be 16-byte aligned.
- The unit of `offset` is bytes. The new `dst` address after Post Update must still be 16-byte aligned, and both the current write range and the updated address must fall within the available UB capacity.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [__NPU_ARCH__](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

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
constexpr uint32_t MASK_BYTES = 32;
constexpr uint32_t PACKED_BYTES = 16;
constexpr uint32_t REPEAT_TIMES = 2;
constexpr uint32_t STEP_BYTES = 32;
constexpr uint32_t SECTION_BYTES = REPEAT_TIMES * STEP_BYTES;
constexpr uint32_t OUTPUT_BYTES = 3 * SECTION_BYTES;
constexpr uint8_t SENTINEL = 0xa5;

__simd_vf__ inline void pack_postupdate_vf(__ubuf__ uint8_t* output,
                                           __ubuf__ uint32_t* mask_input)
{
    vector_bool src;
    asc_loadalign(src, mask_input);

    __ubuf__ uint8_t* dst8 = output;
    __ubuf__ uint16_t* dst16 = reinterpret_cast<__ubuf__ uint16_t*>(output + SECTION_BYTES);
    __ubuf__ uint32_t* dst32 = reinterpret_cast<__ubuf__ uint32_t*>(output + 2 * SECTION_BYTES);
    for (uint32_t repeat = 0; repeat < REPEAT_TIMES; ++repeat) {
        asc_storealign_pack_postupdate(dst8, src, STEP_BYTES);
        asc_storealign_pack_postupdate(dst16, src, STEP_BYTES);
        asc_storealign_pack_postupdate(dst32, src, STEP_BYTES);
    }
}

__global__ __vector__ void asc_storealign_pack_postupdate_kernel(
    __gm__ uint8_t* output, __gm__ uint32_t* mask_input)
{
    asc_init();
    __ubuf__ uint8_t output_local[OUTPUT_BYTES];
    __ubuf__ uint32_t mask_local[MASK_BYTES / sizeof(uint32_t)];
    asc_copy_gm2ub(output_local, output, OUTPUT_BYTES);
    asc_copy_gm2ub(mask_local, mask_input, MASK_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    pack_postupdate_vf(output_local, mask_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm(output, output_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint32_t> mask_input(MASK_BYTES / sizeof(uint32_t), 0x55555555u);
    std::vector<uint8_t> output(OUTPUT_BYTES, SENTINEL);
    std::vector<uint8_t> expected(OUTPUT_BYTES, SENTINEL);
    for (uint32_t base = 0; base < OUTPUT_BYTES; base += SECTION_BYTES) {
        for (uint32_t repeat = 0; repeat < REPEAT_TIMES; ++repeat) {
            for (uint32_t i = 0; i < PACKED_BYTES; ++i) {
                expected[base + repeat * STEP_BYTES + i] = 0xff;
            }
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t* mask_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&mask_device), MASK_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(mask_device, MASK_BYTES, mask_input.data(), MASK_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(output_device, OUTPUT_BYTES, output.data(), OUTPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_storealign_pack_postupdate_kernel<<<1, 0>>>(output_device, mask_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == expected;
    std::cout << (passed ? "[Success] asc_storealign_pack_postupdate passed."
                         : "[Failed] asc_storealign_pack_postupdate output mismatch.")
              << std::endl;
    aclrtFree(mask_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
