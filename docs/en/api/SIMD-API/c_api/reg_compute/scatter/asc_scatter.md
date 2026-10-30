# asc_scatter

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.078959+00:00 pushedAt=2026-10-27T02:26:33.189Z nl=2 -->

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

The header file path is `"c_api/reg_compute/scatter/ub_scatter.h"`.

Scatters elements from the source vector data register to the Unified Buffer (UB) according to the index values in the index register. `mask` indicates the elements that participate in the scatter. Source elements with a mask of 1 are written to the corresponding index positions in `dst`, and source elements with a mask of 0 are not written to the destination address, preserving the original data at the destination address. The data format and content remain unchanged during the transfer.

The scatter process is shown in the following figure.

**Figure 1** Scatter data

![](../../../../figures/reg_scatter.png)

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

```c
// Placeholder form.
__simd_callee__ inline void asc_scatter(__ubuf__ <dtype>* dst,
                                        vector_<dtype> src,
                                        vector_<index_dtype> index,
                                        vector_bool mask)
```

### Data Types Supported by dtype

**Table 1** Correspondence between dtype and index_dtype

| dtype | index_dtype |
|---|---|
| `int8_t` | `uint16_t` |
| `uint8_t` | `uint16_t` |
| `int16_t` | `uint16_t` |
| `uint16_t` | `uint16_t` |
| `half` | `uint16_t` |
| `bfloat16_t` | `uint16_t` |
| `int32_t` | `uint32_t` |
| `uint32_t` | `uint32_t` |
| `float` | `uint32_t` |

### Typical Prototype Example

```c
__simd_callee__ inline void asc_scatter(__ubuf__ int8_t* dst,
                                        vector_int8_t src,
                                        vector_uint16_t index,
                                        vector_bool mask)
```

## Parameters

**Table 2** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand in UB. The type is `__ubuf__ <dtype>*`, and the start address must be 32-byte aligned. |
| src | Input | Source operand (vector data register). The data type must be consistent with the data type pointed to by `dst`. |
| index | Input | Data index (vector data register), in elements. |
| mask | Input | Source operand mask (mask register), used to indicate which elements participate in the transfer. A value of 1 at the corresponding position indicates participation, and 0 indicates non-participation, preserving the original data at the destination address. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The source operand and index are vector data registers, and the destination operand is a UB address.
- The start address of `dst` must be 32-byte aligned.
- The UB capacity is capped at 256 KB. The capacity available to users varies with compilation options and programming scenarios (by default, 6 KB of SIMD VF stack and 2 KB of Ascend C reserved space are set aside, leaving 248 KB available; in SIMD and SIMT mixed programming, an additional 32 KB to 128 KB is partitioned as Data Cache, further reducing the available capacity). The actual write address after `index` indexing must not exceed the actual available capacity.
- If the UB address ranges accessed by this instruction overlap with those accessed by other instructions, you must insert the synchronization instruction [asc_mem_bar](../reg_sync/asc_mem_bar.md) to ensure that the instructions execute serially and prevent abnormal data from occurring.

### Scatter Constraints

- Source elements whose `mask` bit is 0 do not participate in the data movement, and the original data at their corresponding destination addresses remains unchanged.
- When `dst` is of the `int8_t` or `uint8_t` data type, only the even-position elements in the source operand are valid. That is, the elements at positions 0, 2, 4, ..., 252, 254 in `src` are scattered and stored to the destination operand.
- The valid index values in `index` must be unique. If duplicate valid index values exist, the system retains only the data corresponding to one of the index values and ignores the rest, and it cannot be determined which data is retained.
- The actual write address after indexing by `index` must fall within the valid UB address range.

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
constexpr uint32_t ELEMENT_COUNT = 128;
constexpr uint32_t REPEAT_TIMES = 3;
constexpr uint32_t OUTPUT_COUNT = REPEAT_TIMES * ELEMENT_COUNT;
constexpr uint16_t SENTINEL = 0xffff;
constexpr uint32_t INPUT_BYTES = ELEMENT_COUNT * sizeof(uint16_t);
constexpr uint32_t OUTPUT_BYTES = OUTPUT_COUNT * sizeof(uint16_t);

__simd_vf__ inline void scatter_vf(__ubuf__ uint16_t* output,
                                    __ubuf__ uint16_t* input,
                                    __ubuf__ uint16_t* indices)
{
    vector_uint16_t src;
    vector_uint16_t index;
    asc_loadalign(src, input);
    asc_loadalign(index, indices);
    vector_bool mask = asc_create_mask_b16(PAT_M4);
    for (uint32_t repeat = 0; repeat < REPEAT_TIMES; ++repeat) {
        asc_scatter(output + repeat * ELEMENT_COUNT, src, index, mask);
    }
}

__global__ __vector__ void asc_scatter_kernel(__gm__ uint16_t* output,
                                               __gm__ uint16_t* input,
                                               __gm__ uint16_t* indices)
{
    asc_init();
    __ubuf__ uint16_t output_local[OUTPUT_COUNT];
    __ubuf__ uint16_t input_local[ELEMENT_COUNT];
    __ubuf__ uint16_t index_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(output_local, output, OUTPUT_BYTES);
    asc_copy_gm2ub_align(input_local, input, INPUT_BYTES);
    asc_copy_gm2ub_align(index_local, indices, INPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    scatter_vf(output_local, input_local, index_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint16_t> input(ELEMENT_COUNT);
    std::vector<uint16_t> indices(ELEMENT_COUNT);
    std::vector<uint16_t> output(OUTPUT_COUNT, SENTINEL);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input[i] = static_cast<uint16_t>(i + 1);
        indices[i] = static_cast<uint16_t>((i * 3) % ELEMENT_COUNT);
    }

    if (aclInit(nullptr) != ACL_SUCCESS || aclrtSetDevice(0) != ACL_SUCCESS) return 1;
    uint16_t* input_device = nullptr;
    uint16_t* index_device = nullptr;
    uint16_t* output_device = nullptr;
    if (aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST) != ACL_SUCCESS ||
        aclrtMalloc(reinterpret_cast<void**>(&index_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST) != ACL_SUCCESS ||
        aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST) != ACL_SUCCESS) {
        return 1;
    }
    if (aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE) != ACL_SUCCESS ||
        aclrtMemcpy(index_device, INPUT_BYTES, indices.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE) != ACL_SUCCESS ||
        aclrtMemcpy(output_device, OUTPUT_BYTES, output.data(), OUTPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE) != ACL_SUCCESS) {
        return 1;
    }

    asc_scatter_kernel<<<1, 0>>>(output_device, input_device, index_device);
    if (aclrtSynchronizeDevice() != ACL_SUCCESS ||
        aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST) != ACL_SUCCESS) {
        return 1;
    }

    bool passed = true;
    for (uint32_t repeat = 0; repeat < REPEAT_TIMES && passed; ++repeat) {
        for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
            const uint16_t expected = (i % 4 == 0) ? input[i] : SENTINEL;
            const uint32_t position = repeat * ELEMENT_COUNT + indices[i];
            if (output[position] != expected) {
                std::cerr << "Mismatch at output[" << position << "]: expected " << expected
                          << ", actual " << output[position] << std::endl;
                passed = false;
                break;
            }
        }
    }

    aclrtFree(input_device);
    aclrtFree(index_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    std::cout << (passed ? "[Success] asc_scatter passed." : "[Failed] asc_scatter failed.") << std::endl;
    return passed ? 0 : 1;
}
```
