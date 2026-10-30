# asc_set_hf32_round_mode

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T03:17:25.183Z pushedAt=2026-10-28T12:21:43.305Z nl=2 -->

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

This API sets the rounding mode for HF32. Before using this API, call [asc_enable_hf32](asc_enable_hf32.md) to enable HF32 mode.

## Prototype

```c
__aicore__ inline void asc_set_hf32_round_mode(asc_hf32_round_mode hf32_round_mode)
```

## Parameters

| Parameter | Input/Output | Description |
|:-------|:----------|:------|
| hf32_round_mode | Input | Input parameter for controlling the HF32 rounding mode. It is of the [asc_hf32_round_mode](../defs/enum/asc_hf32_round_mode.md) enumeration type and supports the following two enumeration values:<br>&nbsp;&nbsp;&bull; NEAREST_AWAY: FP32 is rounded to HF32 by rounding to the nearest value, with ties rounding away from zero.<br>&nbsp;&nbsp;&bull; NEAREST_EVEN: FP32 is rounded to HF32 by rounding to the nearest value, with ties rounding to even. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- After HF32 mode is enabled, if this API is not called, the rounding mode represented by `NEAREST_EVEN` is used by default.
- This API must be called before the cube multiply-accumulate instruction ([asc_mmad](asc_mmad.md)) is executed, to ensure that the mode configuration takes effect during cube multiply-accumulate computation.
- The rounding mode configured by this API takes effect only while HF32 mode is enabled. If HF32 mode is not enabled, the rounding mode configuration has no actual effect.
- Once configured, the rounding mode remains in effect. After HF32 mode is disabled and then re-enabled, the last rounding mode configuration is still used. To switch the rounding mode, call this API again.

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
constexpr uint32_t DIM = 16, ELEMENTS = DIM * DIM;

__global__ __cube__ void asc_set_hf32_round_mode_kernel(
    __gm__ float* a, __gm__ float* b, __gm__ float* output, asc_hf32_round_mode round_mode)
{
    asc_init();
    __cbuf__ float a_l1[ELEMENTS], b_l1[ELEMENTS];
    __ca__ float a_l0[ELEMENTS];
    __cb__ float b_l0[ELEMENTS];
    __cc__ float c_l0[ELEMENTS];
    asc_set_gm2l1_nz_para(1, 1, 16, 0);
    asc_copy_gm2l1_nd2nz(a_l1, a, DIM * sizeof(float), asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM,
        DIM, DIM, 0, false);
    asc_set_gm2l1_nz_para(1, 1, 16, 0);
    asc_copy_gm2l1_nd2nz(b_l1, b, DIM * sizeof(float), asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM,
        DIM, DIM, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 2, 1, 1);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 2, 1, 1);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_enable_hf32();
    asc_set_hf32_round_mode(round_mode);
    asc_mmad(c_l0, a_l0, b_l0, DIM, DIM, DIM, asc_unit_flag_mode::DISABLE, true, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, DIM, DIM, DIM, DIM,
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
    constexpr float tie = 1.0f + 0.5f / 1024.0f;
    std::vector<float> a(ELEMENTS), b(ELEMENTS), nearest_away(ELEMENTS), nearest_even(ELEMENTS);
    std::vector<float> away_golden(ELEMENTS), even_golden(ELEMENTS);
    for (uint32_t i = 0; i < DIM; ++i) {
        a[i * DIM + i] = tie;
        b[i * DIM + i] = 1.0f;
        away_golden[i * DIM + i] = 1.0f + 1.0f / 1024.0f;
        even_golden[i * DIM + i] = 1.0f;
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    float *a_device = nullptr, *b_device = nullptr, *away_device = nullptr, *even_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), ELEMENTS * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), ELEMENTS * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&away_device), ELEMENTS * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&even_device), ELEMENTS * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, ELEMENTS * sizeof(float), a.data(), ELEMENTS * sizeof(float), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, ELEMENTS * sizeof(float), b.data(), ELEMENTS * sizeof(float), ACL_MEMCPY_HOST_TO_DEVICE);
    asc_set_hf32_round_mode_kernel<<<1, 0>>>(
        a_device, b_device, away_device, asc_hf32_round_mode::NEAREST_AWAY);
    aclrtSynchronizeDevice();
    asc_set_hf32_round_mode_kernel<<<1, 0>>>(
        a_device, b_device, even_device, asc_hf32_round_mode::NEAREST_EVEN);
    aclrtSynchronizeDevice();
    aclrtMemcpy(nearest_away.data(), ELEMENTS * sizeof(float), away_device, ELEMENTS * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(nearest_even.data(), ELEMENTS * sizeof(float), even_device, ELEMENTS * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    print_row("Input tie row 0", a);
    print_row("NEAREST_AWAY row 0", nearest_away);
    print_row("NEAREST_EVEN row 0", nearest_even);
    const bool passed = nearest_away == away_golden && nearest_even == even_golden;
    std::cout << (passed ? "[Success] asc_set_hf32_round_mode selects tie rounding."
                         : "[Failed] asc_set_hf32_round_mode result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(away_device); aclrtFree(even_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
<!-- end id8 -->
