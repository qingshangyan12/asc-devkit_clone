# asc_storealign_squeeze_status_postupdate

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.109470+00:00 pushedAt=2026-10-27T02:26:33.289Z nl=2 -->

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

Writes the valid data length saved in the AR special register after the [asc_squeeze_and_storeunalign](../reg_permute_sel/asc_squeeze_and_storeunalign.md) operation to the Unified Buffer (UB), and automatically updates the destination address pointer by byte offset after the write. This API is suitable for continuously saving squeeze status in a hardware loop.

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

```cpp
__simd_callee__ inline void asc_storealign_squeeze_status_postupdate(__ubuf__ uint32_t*& dst,
                                                                     int32_t offset)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Input/output | Base address of the destination operand in UB, of type `__ubuf__ uint32_t*&`. When the API executes, the current AR value is written to this address; after execution, the pointer is incremented by `offset` bytes. |
| offset | Input | Destination address update amount, of type `int32_t`, in bytes. |

## Return Value

None

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The initial destination address and the destination address after each Post Update must be 4-byte aligned and must not exceed the actual available UB range.
- Before calling this API, use [asc_squeeze_and_storeunalign](../reg_permute_sel/asc_squeeze_and_storeunalign.md) to complete the squeeze operation. The value written is the current accumulated valid data byte count.
- If this API overlaps with other instructions in the UB address space, insert the synchronization instruction [asc_mem_bar](../reg_sync/asc_mem_bar.md) to serialize the multiple instructions.

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
#include "acl/acl.h"
#include "c_api/asc_simd.h"

namespace {
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr uint32_t STATUS_WORD_COUNT = 128;
constexpr int32_t POST_UPDATE_OFFSET = 32;
constexpr uint32_t SQUEEZE_STATUS_BYTES = ELEMENT_COUNT * sizeof(float);

__simd_vf__ inline void store_squeeze_status(
    __ubuf__ uint32_t* status, __ubuf__ float* packed, __ubuf__ float* input)
{
    vector_float src;
    vector_float squeezed;
    vector_store_unalign unalign_reg;
    vector_bool full_mask = asc_create_mask_b32(PAT_ALL);

    asc_loadalign(src, input); // Load the input data into the vector data register.
    asc_squeeze_and_storeunalign_init();
    asc_squeeze_and_storeunalign(packed, squeezed, unalign_reg, src, full_mask);
    asc_squeeze_and_storeunalign_finalize(packed, unalign_reg);

    // After each status write, status automatically increases by POST_UPDATE_OFFSET bytes.
    asc_storealign_squeeze_status_postupdate(status, POST_UPDATE_OFFSET);
    asc_storealign_squeeze_status_postupdate(status, POST_UPDATE_OFFSET);
}

__global__ __vector__ void store_squeeze_status_kernel(__gm__ uint32_t* output, __gm__ float* input)
{
    asc_init();
    __ubuf__ float input_local[ELEMENT_COUNT];
    __ubuf__ float packed_local[ELEMENT_COUNT];
    __ubuf__ uint32_t status_local[STATUS_WORD_COUNT];
    asc_copy_gm2ub_align(input_local, input, sizeof(input_local));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    store_squeeze_status(status_local, packed_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, status_local, sizeof(status_local));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> input(ELEMENT_COUNT);
    std::vector<uint32_t> output(STATUS_WORD_COUNT, 0U);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input[i] = static_cast<float>(i);
    }

    if (aclInit(nullptr) != ACL_SUCCESS || aclrtSetDevice(0) != ACL_SUCCESS) {
        return 1;
    }
    float* input_device = nullptr;
    uint32_t* output_device = nullptr;
    bool success =
        aclrtMalloc(reinterpret_cast<void**>(&input_device), input.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST) ==
            ACL_SUCCESS &&
        aclrtMalloc(
            reinterpret_cast<void**>(&output_device), output.size() * sizeof(uint32_t), ACL_MEM_MALLOC_HUGE_FIRST) ==
            ACL_SUCCESS;
    if (success) {
        success = aclrtMemcpy(
                      input_device, input.size() * sizeof(float), input.data(), input.size() * sizeof(float),
                      ACL_MEMCPY_HOST_TO_DEVICE) == ACL_SUCCESS;
    }
    if (success) {
        store_squeeze_status_kernel<<<1, 0>>>(output_device, input_device);
        success = aclrtSynchronizeDevice() == ACL_SUCCESS;
    }
    if (success) {
        success = aclrtMemcpy(
                      output.data(), output.size() * sizeof(uint32_t), output_device, output.size() * sizeof(uint32_t),
                      ACL_MEMCPY_DEVICE_TO_HOST) == ACL_SUCCESS;
    }
    if (input_device != nullptr) {
        aclrtFree(input_device);
    }
    if (output_device != nullptr) {
        aclrtFree(output_device);
    }
    aclrtResetDevice(0);
    aclFinalize();
    if (!success) {
        return 1;
    }
    success = success && output[0] == SQUEEZE_STATUS_BYTES && output[8] == SQUEEZE_STATUS_BYTES;
    std::cout << (success ? "PASS" : "FAIL") << '\n';
    return success ? 0 : 1;
}
```
