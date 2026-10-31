# asc_mem_bar

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-27T01:34:54.964454+00:00 pushedAt=2026-10-27T02:26:33.177Z nl=2 -->

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

The header file path is `"c_api/reg_compute/reg_sync.h"`.

Synchronization instruction between different pipelines in Reg vector computation. This synchronization instruction specifies the source pipeline and destination pipeline. As shown in the following figure, the destination pipeline waits for all instructions on the source pipeline to complete before execution.
For read/write scenarios, if the read and write instructions use the same register, register ordering is automatically enforced and the instructions are executed in code order, so no synchronization instruction is required. If they use different registers, a synchronization instruction must be inserted to ensure the execution order of the read and write instructions. The same applies to write/write scenarios.

![Pipeline wait diagram within the SIMD_VF function](../../../../figures/simd_vf_pipeline_wait_diagram.png)

This API takes effect only on AIV.

## Prototype

```cpp
__simd_callee__ inline void asc_mem_bar(MEM_TYPE mem_type)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| :----- | :------- | :------- |
| mem_type | Input | Type of the synchronization pipeline. The type is `MEM_TYPE`. For the value range, see Table 2 `mem_type` value description. |

**Table 2** Supported mem_type value range of this API (for the meanings of source/destination pipelines, see Table 3 Reg computation pipeline description)

| Value | Source Pipeline | Destination Pipeline |
| ------ | -------- | -------- |
| `VV_ALL` | `VEC_ALL` | `VEC_ALL` |
| `VST_VLD` | `VEC_STORE` | `VEC_LOAD` |
| `VLD_VST` | `VEC_LOAD` | `VEC_STORE` |
| `VST_VST` | `VEC_STORE` | `VEC_STORE` |
| `VS_ALL` | `VEC_ALL` | `SCALAR_ALL` |
| `VST_LD` | `VEC_STORE` | `SCALAR_LOAD` |
| `VLD_ST` | `VEC_LOAD` | `SCALAR_STORE` |
| `VST_ST` | `VEC_STORE` | `SCALAR_STORE` |
| `SV_ALL` | `SCALAR_ALL` | `VEC_ALL` |
| `ST_VLD` | `SCALAR_STORE` | `VEC_LOAD` |
| `LD_VST` | `SCALAR_LOAD` | `VEC_STORE` |
| `ST_VST` | `SCALAR_STORE` | `VEC_STORE` |

**Table 3** Reg computation pipeline description

| Pipeline | Meaning |
| ------ | -------- |
| `VEC_STORE` | Vector write-to-UB pipeline within the `SIMD_VF` function.<br>Corresponds to register-to-UB transfer instructions, for example, [asc_storealign](../store/asc_storealign.md). |
| `VEC_LOAD` | Vector read-from-UB pipeline within the `SIMD_VF` function.<br>Corresponds to UB-to-register transfer instructions, for example, [asc_loadalign](../load/asc_loadalign.md). |
| `SCALAR_STORE` | Scalar write-to-UB pipeline within the `SIMD_VF` function. |
| `SCALAR_LOAD` | Scalar read-from-UB pipeline within the `SIMD_VF` function. |
| `VEC_ALL` | All vector read/write UB pipelines within the `SIMD_VF` function. |
| `SCALAR_ALL` | All scalar read/write UB pipelines within the `SIMD_VF` function. |

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).

### Instruction Constraints

- In read/write dependency scenarios, if the read instruction and write instruction use the same register, register ordering is triggered, and the instructions execute in code order without inserting additional synchronization instructions.
- Redundant synchronization instructions degrade performance. Avoid multiple synchronization instruction calls by hoisting them out of loops or splitting loops.
- Synchronization is required only when Unified Buffer (UB) data has dependencies. Whether a dependency exists depends on whether the memory read and written by the instructions overlaps. The memory read/write patterns of some transfer instructions are as follows:
    - Instructions such as [asc_gather](../ub_gather/asc_gather.md)/[asc_scatter](../scatter/asc_scatter.md)/[asc_gather_datablock](../ub_gather/asc_gather_datablock.md) depend on the `index` address offset.
    - A single [asc_loadalign_brc_elem](../load/asc_loadalign_brc_elem.md) instruction reads 32B of data and then broadcasts the first element.

## Examples

In the following example, the previous loop writes the accumulation result to UB through `asc_storealign`, and the next loop reads the result from the same UB address space through `asc_loadalign`. Because the two instructions use different vector data registers, `asc_mem_bar(VST_VLD)` must be called before the next loop reads UB to wait for the previous loop's UB write to complete.

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t VECTOR_ELEMENT_COUNT = 64;
constexpr uint32_t BLOCK_COUNT = 4;
constexpr uint32_t INPUT_COUNT = VECTOR_ELEMENT_COUNT * BLOCK_COUNT;
constexpr uint32_t INPUT_BYTES = INPUT_COUNT * sizeof(float);
constexpr uint32_t OUTPUT_BYTES = VECTOR_ELEMENT_COUNT * sizeof(float);

__simd_vf__ inline void accumulate_vf(__ubuf__ float* data)
{
    vector_float src0;
    vector_float src1;
    vector_float dst;
    vector_bool mask = asc_create_mask_b32(PAT_ALL);

    for (uint32_t i = 1; i < BLOCK_COUNT; ++i) {
        asc_mem_bar(VST_VLD);
        asc_loadalign(src0, data);
        asc_loadalign(src1, data + i * VECTOR_ELEMENT_COUNT);
        asc_add(dst, src0, src1, mask);
        asc_storealign(data, dst, mask);
    }
}

__global__ __vector__ void asc_mem_bar_kernel(__gm__ float* output, __gm__ float* input)
{
    asc_init();
    __ubuf__ float data_local[INPUT_COUNT];
    asc_copy_gm2ub_align(data_local, input, INPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    accumulate_vf(data_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, data_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> input(INPUT_COUNT);
    std::vector<float> output(VECTOR_ELEMENT_COUNT, 0.0f);
    std::vector<float> expected(VECTOR_ELEMENT_COUNT, 0.0f);
    for (uint32_t block = 0; block < BLOCK_COUNT; ++block) {
        for (uint32_t i = 0; i < VECTOR_ELEMENT_COUNT; ++i) {
            input[block * VECTOR_ELEMENT_COUNT + i] = static_cast<float>(block * 10U + i);
            expected[i] += input[block * VECTOR_ELEMENT_COUNT + i];
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    float* input_device = nullptr;
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_mem_bar_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    bool passed = true;
    for (uint32_t i = 0; i < VECTOR_ELEMENT_COUNT; ++i) {
        if (std::fabs(output[i] - expected[i]) > 1e-6f) {
            passed = false;
            break;
        }
    }
    std::cout << (passed ? "[Success] asc_mem_bar completed."
                         : "[Failed] asc_mem_bar output mismatch.")
              << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
