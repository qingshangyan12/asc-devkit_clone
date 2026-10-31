# asc_icache_preload

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T02:57:55.042Z pushedAt=2026-10-28T12:21:43.277Z nl=2 -->

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
- Atlas inference product AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference product Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/cache_ctrl/cache_ctrl.h"`.

You can manually call this API to preload instructions from the Global Memory address where this API is called into the ICache (instruction cache), avoiding ICache misses during subsequent instruction fetches. The ICache caches instructions recently or frequently used by the Scalar unit. After prefetching through this API, the Scalar unit reads GM instructions from the ICache first.

The preload address is usually obtained through the [asc_get_program_counter](../utils/sys_var/asc_get_program_counter.md) API.

## Prototype

```c
__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
|---|---|---|
| addr | Input | Global Memory start address of the instructions to prefetch. |
| prefetch_len | Input | Length of the data to preload, in 2K Byte. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- The hardware aligns the `addr` prefetch address to 128B and takes the valid bits. Passing a non-128B-aligned address does not cause an error, but the actual prefetch start Cache Line is determined by the alignment result.
- The value range of `prefetch_len` is [0, 15] (AIC, corresponding to a 32KB ICache) or [0, 7] (AIV, corresponding to a 16KB ICache).
- When you call this API, the preload address must be obtained by calling the [asc_get_program_counter](../utils/sys_var/asc_get_program_counter.md) API to get the current program counter (PC) value. After obtaining it, you must clear the invalid high bits using the mask `0xFFFFFFFFFFFF` (all lower 48 bits set to 1) to obtain the valid 48-bit address of the current instruction on GM. This is the fixed procedure for obtaining the preload address. The sample code is as follows:

    ```cpp
    int64_t prefetch_length = 6;
    // Obtain the current program counter (PC) value through asc_get_program_counter.
    // The PC is a 64-bit return value with valid address bits 0 to 47. Clear the invalid high bits using the mask 0xFFFFFFFFFFFF (all lower 48 bits set to 1),
    // to obtain the valid 48-bit address of the current instruction on GM, which serves as the start address for preloading.
    int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
    asc_icache_preload(reinterpret_cast<void *>(pc), prefetch_length);
    ```

## Examples

After saving the code as `example.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
// This example only demonstrates the functionality of the asc_icache_preload API and does not represent an actual operator scenario.
// This example shows a scenario in a fused operator modified by `__mix__(1, 2)`, where the AIC completes cube multiplication and writes the float result to GM, and then two AIVs each read part of the result,
// multiply the result by a constant, and write it to the output GM. When ICache misses degrade AIV performance, the asc_icache_preload API is used while the AIV waits for the AIC
// to prefetch instructions, which reduces ICache misses in the AIV and thereby improves performance.
#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t MATRIX_DIM = 16;
constexpr uint32_t MATRIX_ELEMENTS = MATRIX_DIM * MATRIX_DIM;
constexpr uint32_t ELEMENTS_PER_AIV = MATRIX_ELEMENTS / 2;
constexpr uint32_t INPUT_BYTES = MATRIX_ELEMENTS * sizeof(half);
constexpr uint32_t FP32_RESULT_BYTES = MATRIX_ELEMENTS * sizeof(float);
constexpr uint32_t FP32_BYTES_PER_AIV = ELEMENTS_PER_AIV * sizeof(float);
constexpr uint32_t ELEMENTS_PER_VF = 64;
constexpr uint32_t AIV_REPEAT_TIMES = ELEMENTS_PER_AIV / ELEMENTS_PER_VF;
constexpr uint32_t DATA_BLOCK_BYTES = 32;
constexpr int64_t OUTPUT_READY_FLAG_ID = 0;
constexpr float OUTPUT_SCALE = 2.0F;

// Select an appropriate prefetch length (in units of 2KB) based on factors such as the total number of instructions in the AIV and the ICache capacity.
constexpr int64_t PREFETCH_LENGTH = 2;
constexpr int64_t PC_ADDRESS_MASK = 0xFFFFFFFFFFFF;

__simd_vf__ inline void ScaleResult(__ubuf__ float* output, __ubuf__ float* input)
{
    for (uint32_t i = 0; i < AIV_REPEAT_TIMES; ++i) {
        vector_float input_reg;
        vector_float output_reg;
        uint32_t count = ELEMENTS_PER_VF;
        vector_bool mask = asc_update_mask_b32(count);
        asc_loadalign(input_reg, input + i * ELEMENTS_PER_VF);
        asc_mul_scalar(output_reg, input_reg, OUTPUT_SCALE, mask);
        asc_storealign(output + i * ELEMENTS_PER_VF, output_reg, mask);
    }
}

__global__ __mix__(1, 2) void MatmulScaleKernel(
    __gm__ half* input, __gm__ float* matmul_result, __gm__ float* output, __gm__ int64_t* preload_status)
{
    asc_init();
    __cbuf__ half cube_a_l1[MATRIX_ELEMENTS];
    __cbuf__ half cube_b_l1[MATRIX_ELEMENTS];
    __ca__ half cube_a_l0[MATRIX_ELEMENTS];
    __cb__ half cube_b_l0[MATRIX_ELEMENTS];
    __cc__ float matmul_result_l0[MATRIX_ELEMENTS];

    if ASC_IS_AIC {
        // The AIC directly moves the two input matrices from GM to the L1 Buffer.
        asc_copy_gm2l1(cube_a_l1, input, 1, INPUT_BYTES / DATA_BLOCK_BYTES, asc_channel_pad_mode::NONE, 0, 0);
        asc_copy_gm2l1(cube_b_l1, input, 1, INPUT_BYTES / DATA_BLOCK_BYTES, asc_channel_pad_mode::NONE, 0, 0);
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_copy_l12l0a(cube_a_l0, cube_a_l1, 0, 0, 1, 1, 1, 1);
        asc_copy_l12l0b_transpose(cube_b_l0, cube_b_l1, 0, 0, 1, 1, 1, 1);
        asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
        asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
        asc_mmad(
            matmul_result_l0, cube_a_l0, cube_b_l0, MATRIX_DIM, MATRIX_DIM, MATRIX_DIM, asc_unit_flag_mode::DISABLE, false, false, true);
        asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
        asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
        asc_set_l0c_copy_nz_para(1, 0, 0);
        asc_copy_l0c2gm(matmul_result, matmul_result_l0, MATRIX_DIM, MATRIX_DIM, MATRIX_DIM, MATRIX_DIM,
            asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, asc_unit_flag_mode::DISABLE, QuantMode_t::NoQuant,
            asc_relu_pre_mode::NONE, false, true, false, false);
        asc_sync_block_arrive(PIPE_FIX, OUTPUT_READY_FLAG_ID);
    }

    if ASC_IS_AIV {
        // AIV is about to wait for AIC to write out the cube multiplication result. Prefetch the instructions required for the subsequent scaling computation before waiting.
        const int64_t pc = asc_get_program_counter() & PC_ADDRESS_MASK;
        asc_icache_preload(reinterpret_cast<void*>(pc), PREFETCH_LENGTH);
        // The prefetch status is for debugging only. A query result of 0 indicates completion, and 1 indicates that it is still in progress.
        asc_store_dev(preload_status + asc_get_sub_block_id(), asc_get_icache_preload_status());

        // Wait for AIC to finish writing out the cube multiplication result.
        asc_sync_block_wait(PIPE_MTE2, OUTPUT_READY_FLAG_ID);

        const uint32_t aiv_index = asc_get_sub_block_id();
        __ubuf__ float result_ub[ELEMENTS_PER_AIV];
        __ubuf__ float output_ub[ELEMENTS_PER_AIV];
        asc_copy_gm2ub_align(result_ub, matmul_result + aiv_index * ELEMENTS_PER_AIV, FP32_BYTES_PER_AIV);
        asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
        ScaleResult(output_ub, result_ub);
        asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
        asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
        asc_copy_ub2gm_align(output + aiv_index * ELEMENTS_PER_AIV, output_ub, FP32_BYTES_PER_AIV);
    }

    asc_sync_pipe(PIPE_ALL);
}

bool CheckResult(const std::vector<float>& matmul_result, const std::vector<float>& output)
{
    for (uint32_t i = 0; i < MATRIX_ELEMENTS; ++i) {
        if (matmul_result[i] != static_cast<float>(MATRIX_DIM) ||
            output[i] != static_cast<float>(MATRIX_DIM) * OUTPUT_SCALE) {
            return false;
        }
    }
    return true;
}
} // namespace

int main()
{
    std::vector<half> input(MATRIX_ELEMENTS, static_cast<half>(1.0F));
    std::vector<float> matmul_result(MATRIX_ELEMENTS, 0.0F);
    std::vector<float> output(MATRIX_ELEMENTS, 0.0F);
    half* input_device = nullptr;
    float* matmul_result_device = nullptr;
    float* output_device = nullptr;
    int64_t* preload_status_device = nullptr;
    std::vector<int64_t> preload_status(2, -1);

    aclInit(nullptr);
    aclrtSetDevice(0);
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&matmul_result_device), FP32_RESULT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), FP32_RESULT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&preload_status_device), preload_status.size() * sizeof(int64_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    MatmulScaleKernel<<<1, 0>>>(input_device, matmul_result_device, output_device, preload_status_device);
    const aclError ret = aclrtSynchronizeDevice();
    aclrtMemcpy(matmul_result.data(), FP32_RESULT_BYTES, matmul_result_device, FP32_RESULT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(output.data(), FP32_RESULT_BYTES, output_device, FP32_RESULT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(preload_status.data(), preload_status.size() * sizeof(int64_t), preload_status_device,
        preload_status.size() * sizeof(int64_t), ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = ret == ACL_SUCCESS && CheckResult(matmul_result, output);
    std::cout << "ICache preload status: " << preload_status[0] << ", " << preload_status[1] << std::endl;
    std::cout << (passed ? "[Success] asc_icache_preload passed." : "[Failed] asc_icache_preload failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(matmul_result_device);
    aclrtFree(output_device);
    aclrtFree(preload_status_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
