# asc_enable_hif8

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T03:14:51.488Z pushedAt=2026-10-28T12:21:43.301Z nl=2 -->

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

This API enables the HiF8 mode for cube multiply-accumulate (Mmad) computation. After HiF8 mode is enabled, when both the left cube A and right cube B of a cube multiply-accumulate instruction use `fp8_e4m3fn_t` as the input data type, the data in the L0A Buffer and L0B Buffer is converted to the `hifloat8_t` type before participating in the cube multiplication.

## Prototype

```c
__aicore__ inline void asc_enable_hif8()
```

## Parameters

None

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- This API must be called before the cube multiply-accumulate instructions ([asc_mmad](./asc_mmad.md) and [asc_mmad_mx](./asc_mmad_mx.md)) are executed to ensure that the mode configuration takes effect before the cube multiply-accumulate result is generated.
- Once enabled, HiF8 mode remains in effect and is not automatically disabled. Subsequent cube multiply-accumulate instructions continue to use the current mode unless explicitly reconfigured. To switch back to FP8 mode, call the [asc_disable_hif8](./asc_disable_hif8.md) API again.
- This API takes effect only when the cube multiply-accumulate input data type is `fp8_e4m3fn_t`×`fp8_e4m3fn_t`. Other FP8 data type combinations (`fp8_e4m3fn_t`×`fp8_e5m2_t`, `fp8_e5m2_t`×`fp8_e4m3fn_t`, and `fp8_e5m2_t`×`fp8_e5m2_t`) do not support FP8 and HiF8 mode selection, and calling this API has no actual effect.

<!-- npu="950" id8 -->
## Examples

After saving the code as `example.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

The following example code is supported only on Ascend 950PR&950DT products.

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t M = 16;
constexpr uint32_t K = 32;
constexpr uint32_t N = 16;

__global__ __cube__ void asc_enable_hif8_kernel(
    __gm__ uint8_t* a, __gm__ uint8_t* b, __gm__ float* hif8_output)
{
    asc_init();
    __cbuf__ fp8_e4m3fn_t a_l1[M * K], b_l1[K * N];
    __ca__ fp8_e4m3fn_t a_l0[M * K];
    __cb__ fp8_e4m3fn_t b_l0[K * N];
    __cc__ float hif8_l0[M * N];
    asc_set_gm2l1_nz_para(1, 1, 16, 0);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ fp8_e4m3fn_t*>(a), K,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, M, K, 0, false);
    asc_set_gm2l1_nz_para(1, 1, 16, 0);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ fp8_e4m3fn_t*>(b), K,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, N, K, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
    asc_copy_l12l0b(b_l0, b_l1, 0, 0, 1, 1, 1, 1);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_enable_hif8();
    asc_mmad(hif8_l0, a_l0, b_l0, M, K, N, asc_unit_flag_mode::DISABLE, true, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(hif8_output, hif8_l0, N, M, N, M,
        asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, asc_unit_flag_mode::DISABLE, asc_quant_mode::NoQuant,
        asc_relu_pre_mode::NONE, false, true, false, false);
    asc_sync_pipe(PIPE_ALL);
}

void print_row(const char* label, const std::vector<float>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[i];
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<uint8_t> a(M * K), b(N * K);
    std::vector<float> hif8_output(M * N), golden(M * N);
    for (uint32_t i = 0; i < M; ++i) a[i * K + i] = 0x08;
    for (uint32_t i = 0; i < N; ++i) b[i * K + i] = 0x08;
    for (uint32_t i = 0; i < M; ++i) golden[i * N + i] = 1.0f;
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t *a_device = nullptr, *b_device = nullptr;
    float* hif8_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&hif8_device), hif8_output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size(), a.data(), a.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size(), b.data(), b.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    asc_enable_hif8_kernel<<<1, 0>>>(a_device, b_device, hif8_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(hif8_output.data(), hif8_output.size() * sizeof(float), hif8_device,
        hif8_output.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    print_row("HiF8 mode output row 0", hif8_output);
    print_row("Golden row 0", golden);
    std::cout << "HiF8 raw 1.0: 0x08" << std::endl;
    const bool passed = hif8_output == golden;
    std::cout << (passed ? "[Success] asc_enable_hif8 changes 8bit MMAD interpretation."
                         : "[Failed] asc_enable_hif8 result mismatch.") << std::endl;
    aclrtFree(a_device);
    aclrtFree(b_device);
    aclrtFree(hif8_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
<!-- end id8 -->
