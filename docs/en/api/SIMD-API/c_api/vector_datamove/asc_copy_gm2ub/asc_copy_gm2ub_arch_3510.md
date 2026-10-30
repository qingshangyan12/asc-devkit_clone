# asc_copy_gm2ub

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-27T01:34:55.214715+00:00 pushedAt=2026-10-27T02:26:34.675Z nl=2 -->

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

The header file path is `"c_api/vector_datamove/vector_datamove.h"`.

Copies data from Global Memory (GM) to Unified Buffer (UB).

This API supports contiguous data movement and high-dimensional slicing data movement. Contiguous data movement copies the number of bytes specified by `size` from source GM to destination UB, as shown in Figure 1. High-dimensional slicing data movement copies data blocks according to the count specified by `burst_count`, with each data block copied according to the number of bytes specified by `burst_len`, and uses `src_stride` and `dst_stride` to specify the intervals between the start addresses of adjacent data blocks in the source and destination operands, as shown in Figure 2.

**Figure 1** Contiguous data movement diagram

![](../../../../figures/asc_copy_gm2ub_continuous.png "Contiguous data movement diagram")

**Figure 2** High-dimensional slicing data movement diagram

![](../../../../figures/asc_copy_gm2ub_highdim_split.png "High-dimensional slicing data movement diagram")

This API takes effect only on AIV.

## Prototype

### Contiguous Data Movement

```c
__aicore__ inline void asc_copy_gm2ub(__ubuf__ void* dst,
                                      __gm__ void* src,
                                      uint32_t size)
```

### High-Dimensional Slicing Data Movement

```c
__aicore__ inline void asc_copy_gm2ub(__ubuf__ void* dst,
                                      __gm__ void* src,
                                      uint16_t burst_count,
                                      uint16_t burst_len,
                                      uint16_t src_stride,
                                      uint16_t dst_stride)
```

## Parameters

### Contiguous Data Movement

**Table 1** Parameters

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination UB. Must be 32-byte aligned. |
| src | Input | Start address of the source GM. Must be 1-byte aligned. |
| size | Input | Size of data to be copied, in bytes. Value range: [1, $2^{21}−1$]. |

### High-Dimensional Slicing Data Movement

**Table 2** Parameters

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination UB. Must be 32-byte aligned. |
| src | Input | Start address of the source GM. Must be 1-byte aligned. |
| burst_count | Input | Number of contiguous data blocks to be copied. Value range: [1, $2^{16}−1$]. |
| burst_len | Input | Length of each contiguous data block to be copied, in bytes. Value range: [1, $2^{16}−1$]. |
| src_stride | Input | Distance between adjacent contiguous data blocks in the source operand (interval between the head of the preceding data block and the head of the following data block), in bytes. Value range: [0, $2^{16}−1$].<br>When only 1 data block is copied, that is, `burst_count` is set to 1, this parameter can be set to 0. |
| dst_stride | Input | Distance between adjacent contiguous data blocks in the destination operand (interval between the head of the preceding data block and the head of the following data block), in bytes. Value range: [0, $2^{16}−1$].<br>When only 1 data block is copied, that is, `burst_count` is set to 1, this parameter can be set to 0. |

## Return Value

None

## Pipeline Type

PIPE_MTE2

## Constraints

### General Constraints

- Calling this API on a non-AIV returns directly.
- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../../general_description_and_constraints.md#storage-unit-description).
- If the UB address ranges accessed by this instruction overlap with those accessed by other instructions, you must insert synchronization instructions [asc_sync_notify](../../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../../sync/intra_core_sync/asc_sync_wait.md) to ensure that the instructions execute serially and prevent abnormal data from occurring.

### Contiguous Data Movement Constraints

- `size` must satisfy dtype byte alignment: when dtype is b16, it must be a multiple of 2; when dtype is b32, it must be a multiple of 4.

### High-Dimensional Slicing Data Movement Constraints

- `burst_len` must satisfy dtype byte alignment: when dtype is b16, it must be a multiple of 2; when dtype is b32, it must be a multiple of 4.
- When `dst_stride` is not equal to `burst_len`, `dst_stride` requires 32-byte alignment.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

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

__global__ __vector__ void asc_copy_gm2ub_arch_3510_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t local[INPUT_BYTES];
    // Copy INPUT_BYTES from GM to UB, then wait only for PIPE_MTE2.
    asc_copy_gm2ub(local, input, INPUT_BYTES);
    asc_sync_mte2(0);
    asc_copy_ub2gm_align(output, local, INPUT_BYTES);
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
    asc_copy_gm2ub_arch_3510_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input", input);
    print_data("Output", output);
    print_data("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_copy_gm2ub passed." : "[Failed] asc_copy_gm2ub failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
