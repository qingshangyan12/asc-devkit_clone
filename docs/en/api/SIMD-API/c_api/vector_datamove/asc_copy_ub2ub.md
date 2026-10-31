# asc_copy_ub2ub

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-27T01:34:55.234731+00:00 pushedAt=2026-10-27T02:26:34.757Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
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

The header file path is `"c_api/vector_datamove/vector_datamove.h"`.

Copies data from Unified Buffer (UB) to Unified Buffer (UB).

This API supports contiguous data copy and high-dimensional slicing data copy. Contiguous data copy copies the number of bytes specified by `size` contiguously within UB, as shown in Figure 1. High-dimensional slicing data copy copies the number of data blocks specified by `n_burst`, with the number of DataBlocks in each data block specified by `len_burst`. `src_gap` and `dst_gap` specify the gaps between adjacent data blocks in the source and destination operands, respectively, as shown in Figure 2.

**Figure 1** Contiguous data movement diagram
![](../../../figures/asc_copy_ub2ub_continuous.png "Contiguous data movement diagram")

**Figure 2** High-dimensional slicing data movement diagram
![](../../../figures/asc_copy_ub2ub_highdim_split.png "High-dimensional slicing data movement diagram")

This API takes effect only on AIV.

## Prototype

### Contiguous Data Movement

```c
__aicore__ inline void asc_copy_ub2ub(__ubuf__ void* dst,
                                      __ubuf__ void* src,
                                      uint32_t size)
```

### High-Dimensional Slicing Data Movement

```cpp
__aicore__ inline void asc_copy_ub2ub(__ubuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap)
```

## Parameters

### Contiguous Data Movement

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination UB. Must be 32-byte aligned. |
| src | Input | Start address of the source UB. Must be 32-byte aligned. |
| size | Input | Size of the data to be copied, in bytes. Value range: [32, 65535×32], and it must be a multiple of 32. |

### High-Dimensional Slicing Data Movement

**Table 2** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination UB. Must be 32-byte aligned. |
| src | Input | Start address of the source UB. Must be 32-byte aligned. |
| n_burst | Input | Number of contiguous data blocks to copy. Value range: [1, 65535]. |
| len_burst | Input | Length of each contiguous data block, in DataBlocks (32 bytes). Value range: [1, 65535]. |
| src_gap | Input | Gap between adjacent contiguous data blocks of the source operand (the gap between the tail of the preceding data block and the head of the following data block). The unit is DataBlock (32 bytes). Value range: [0, $2^{16}−1$]. |
| dst_gap | Input | Gap between adjacent contiguous data blocks of the destination operand (the gap between the tail of the preceding data block and the head of the following data block). The unit is DataBlock (32 bytes). Value range: [0, $2^{16}−1$]. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

### General Constraints

- Calling this API on a non-AIV returns directly.
- For the capacity and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- The effective DataBlocks copied for the source operand and destination operand must not overlap. In high-dimensional slicing data movement mode, if `src_gap` and `dst_gap` make the actual copied DataBlocks of the source and destination non-overlapping, the overall address spans covered by `src` and `dst` are allowed to overlap.
- If multiple instructions of this API need to be executed and their destination addresses overlap, you must insert synchronization instructions (`asc_sync_notify` and `asc_sync_wait`) to ensure that the instructions execute serially and prevent abnormal data from occurring.

### Contiguous Copy Mode Constraints

- `size` must be a multiple of 32; otherwise, the hardware uses integer division (`size/32`) to determine the number of complete blocks to copy, and the trailing bytes that do not form a complete 32-byte block are discarded.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

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

constexpr uint32_t INPUT_BYTES = 256;
constexpr uint32_t OUTPUT_BYTES = 256;

__global__ __vector__ void asc_copy_ub2ub_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t source[INPUT_BYTES], copied[INPUT_BYTES];
    asc_copy_gm2ub_align(source, input, INPUT_BYTES);
    asc_sync_mte2(0);
    // Copy INPUT_BYTES within UB on PIPE_V before the MTE3 consumer starts.
    asc_copy_ub2ub(copied, source, INPUT_BYTES);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, copied, INPUT_BYTES);
    asc_sync_mte3(0);
}

void print_data(const char* name, const std::vector<uint8_t>& data)
{
    std::cout << name << ":";
    const uint32_t count = data.size() < 32 ? data.size() : 32;
    for (uint32_t i = 0; i < count; ++i) std::cout << ' ' << +data[i];
    if (data.size() > count) std::cout << " ...";
    std::cout << std::endl;
}
} // namespace

int main()
{
    std::vector<uint8_t> input(INPUT_BYTES), output(OUTPUT_BYTES, 0), golden(OUTPUT_BYTES, 0);
    for (uint32_t i = 0; i < INPUT_BYTES; ++i) input[i] = static_cast<uint8_t>(i + 1);
    for (uint32_t i = 0; i < 256; ++i) golden[i] = input[i];
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(output_device, OUTPUT_BYTES, output.data(), OUTPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_copy_ub2ub_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input", input);
    print_data("Output", output);
    print_data("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_copy_ub2ub passed." : "[Failed] asc_copy_ub2ub failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
