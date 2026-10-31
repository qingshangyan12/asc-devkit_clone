# asc_set_mmad_direction_m

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T03:19:00.100Z pushedAt=2026-10-28T12:21:43.308Z nl=2 -->

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

The header file path is `"c_api/cube_compute/cube_compute.h"`.

This API is used to set the cube multiply-accumulate (Mmad) computation to generate the cube computation result fractals column by column. This configuration only changes the generation order of the result fractals on the L0C Buffer, without changing the computation result, the number of multiply-accumulate operations, or the output format.

When UnitFlag is enabled, better performance can be achieved when the generation order of the result fractals on the L0C Buffer is consistent with the move-out order of the data move-out instruction. Therefore, this API can be used in the following scenarios:

- As the default configuration for all move-out scenarios when UnitFlag is disabled (in this case, different result generation directions usually have no substantial performance difference).
- When UnitFlag is enabled, no on-the-fly format conversion is performed during move-out, and the Channel Merge function is not triggered by enabling B8/B4 quantization.
- When UnitFlag is enabled, Nz2DN on-the-fly format conversion is enabled during move-out.

## Prototype

```c
__aicore__ inline void asc_set_mmad_direction_m()
```

## Parameters

None

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- This API must be called before the cube multiply-accumulate instructions ([asc_mmad](asc_mmad.md) and [asc_mmad_mx](asc_mmad_mx.md)) are executed, to ensure that the mode configuration takes effect during the cube multiply-accumulate computation.
- Once written, the direction configuration remains in effect. Subsequent cube multiply-accumulate instructions will continue to use the current direction configuration unless it is explicitly reconfigured. To switch to generating the cube computation result fractals row by row, call the [asc_set_mmad_direction_n](asc_set_mmad_direction_n.md) API again.

## Examples

After saving the code as `example.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

The following example code is supported only by Ascend 950PR&950DT products.

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t M = 16, K = 32, N = 16;

__global__ __cube__ void asc_set_mmad_direction_m_kernel(
    __gm__ int8_t* a, __gm__ int8_t* b, __gm__ int32_t* output)
{
    asc_init();
    __cbuf__ int8_t a_l1[M * K], b_l1[K * N];
    __ca__ int8_t a_l0[M * K];
    __cb__ int8_t b_l0[K * N];
    __cc__ int32_t c_l0[M * N];
    asc_set_gm2l1_nz_para(1, 1, 32, 0);
    asc_copy_gm2l1_nd2nz(
        a_l1, a, K, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, M, K, 0, false);
    asc_set_gm2l1_nz_para(1, 1, 32, 0);
    asc_copy_gm2l1_nd2nz(
        b_l1, b, N, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, K, N, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
    asc_copy_l12l0b_trans(b_l0, b_l1, static_cast<uint16_t>(0), static_cast<uint8_t>(1),
        static_cast<uint16_t>(2), static_cast<uint16_t>(1), static_cast<uint16_t>(0), static_cast<uint16_t>(0));
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_set_mmad_direction_m();
    asc_mmad(c_l0, a_l0, b_l0, M, K, N, asc_unit_flag_mode::DISABLE, true, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, N, M, N, M,
        asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, asc_unit_flag_mode::DISABLE, asc_quant_mode::NoQuant,
        asc_relu_pre_mode::NONE, false, true, false, false);
    asc_sync_pipe(PIPE_ALL);
}

void print_row(const char* label, const std::vector<int32_t>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[i];
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<int8_t> a(M * K), b(K * N);
    std::vector<int32_t> output(M * N), golden(M * N);
    for (uint32_t row = 0; row < M; ++row) for (uint32_t k = 0; k < K; ++k)
        a[row * K + k] = static_cast<int8_t>((row + k) % 5 - 2);
    for (uint32_t k = 0; k < K; ++k) for (uint32_t col = 0; col < N; ++col)
        b[k * N + col] = static_cast<int8_t>((k + 2 * col) % 7 - 3);
    for (uint32_t row = 0; row < M; ++row) for (uint32_t col = 0; col < N; ++col)
        for (uint32_t k = 0; k < K; ++k) golden[row * N + col] += a[row * K + k] * b[k * N + col];
    aclInit(nullptr);
    aclrtSetDevice(0);
    int8_t *a_device = nullptr, *b_device = nullptr;
    int32_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(int32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size(), a.data(), a.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size(), b.data(), b.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    asc_set_mmad_direction_m_kernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int32_t), output_device,
        output.size() * sizeof(int32_t), ACL_MEMCPY_DEVICE_TO_HOST);
    print_row("M-first output row 0", output);
    print_row("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_mmad_direction_m preserves MMAD values."
                         : "[Failed] asc_set_mmad_direction_m result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
<!-- end id8 -->
