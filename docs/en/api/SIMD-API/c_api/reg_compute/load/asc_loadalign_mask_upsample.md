# asc_loadalign_mask_upsample

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-27T01:34:54.706218+00:00 pushedAt=2026-10-27T02:26:32.023Z nl=2 -->

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

Reads VL/16 bytes of data from a 16-byte-aligned start address in the Unified Buffer (UB), duplicates each bit twice to obtain VL/8 bytes of data, and returns a mask register through the function return value.

This API has the same prototype functionality as the aligned load mode of [asc_loadalign_upsample](asc_loadalign_upsample.md) when the destination operand is a mask register, except that this API returns the result through the function return value.

**Figure 1** Upsampling load to a mask register

![](../../../../figures/capi_loadalign_mask_upsample.png)

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

```c
__simd_callee__ inline vector_bool asc_loadalign_mask_upsample(__ubuf__ uint32_t* src)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| src | Input | Source UB address. The actual read address must be 16-byte-aligned, and VL/16 bytes of data are read. |

## Return Value

Returns the mask register that stores the 2x upsampled load result, of type `vector_bool`, with an effective data length of VL/8.

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The actual read address of `src` must be 16-byte-aligned, and the actual read range must be within the UB address space without exceeding the boundary; otherwise, an error is reported.
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
constexpr uint32_t MASK_DATA_BYTES = 16;
constexpr uint32_t MASK_STORAGE_BYTES = 32;
constexpr uint32_t MASK_OUTPUT_BYTES = 32;

__simd_vf__ inline void asc_loadalign_mask_upsample_vf(
    __ubuf__ uint32_t* output, __ubuf__ uint32_t* mask_input)
{
    vector_bool mask = asc_loadalign_mask_upsample(mask_input);
    asc_storealign(output, mask);
}

__global__ __vector__ void asc_loadalign_mask_upsample_kernel(
    __gm__ uint32_t* output, __gm__ uint32_t* mask_input)
{
    asc_init();
    __ubuf__ uint32_t output_local[MASK_OUTPUT_BYTES / sizeof(uint32_t)];
    __ubuf__ uint32_t mask_local[MASK_STORAGE_BYTES / sizeof(uint32_t)];
    asc_copy_gm2ub_align(mask_local, mask_input, MASK_DATA_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_vf_call<asc_loadalign_mask_upsample_vf>(output_local, mask_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, MASK_OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint32_t> output(MASK_OUTPUT_BYTES / sizeof(uint32_t), 0);
    std::vector<uint32_t> golden(MASK_OUTPUT_BYTES / sizeof(uint32_t), 0x33333333u);
    std::vector<uint32_t> mask_input(MASK_STORAGE_BYTES / sizeof(uint32_t), 0);
    for (uint32_t i = 0; i < MASK_DATA_BYTES / sizeof(uint32_t); ++i) {
        mask_input[i] = 0x55555555u;
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t* output_device = nullptr;
    uint32_t* mask_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), MASK_OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&mask_device), MASK_STORAGE_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(mask_device, MASK_STORAGE_BYTES, mask_input.data(), MASK_STORAGE_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_loadalign_mask_upsample_kernel<<<1, 0>>>(output_device, mask_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), MASK_OUTPUT_BYTES, output_device, MASK_OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success]" : "[Failed]")
              << " asc_loadalign_mask_upsample example." << std::endl;
    aclrtFree(output_device);
    aclrtFree(mask_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
