# asc_set_l0c2gm_channel_para (Deprecated)

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T04:33:44.135Z pushedAt=2026-10-28T12:55:49.482Z nl=2 -->

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

**The `asc_set_l0c2gm_channel_para` API is deprecated. Use the [asc_set_l0c_copy_channel_para](../cube_datamove/asc_set_l0c_copy_channel_para.md) API instead.**

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

This API configures the address offset between adjacent rows in the source Nz matrix when the L0C Buffer is moved out and Nz2DN format conversion is performed. After the configuration is written, it is used by the subsequent [asc_copy_l0c2gm](../cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md), [asc_copy_l0c2l1](../cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md), or [asc_copy_l0c2ub](../cube_datamove/asc_copy_l0c2ub.md) API that enables the Nz2DN function. When the Nz2DN function is not enabled, this configuration does not take effect.

This API is a matrix move-out related configuration API and takes effect only on the AIC.

## Prototype

```c
__aicore__ inline void asc_set_l0c2gm_channel_para(uint64_t config)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
|---|---|---|
| config | Input | Source operand configuration for Nz2DN format conversion. The API does not check or convert field values. Assemble the value as described in [Table 2](#table2). |

**Table 2** Description of the config field <a id="table2"></a>

| Bit Range | Description |
|---|---|
| 47:0 | Invalid bits. You do not need to care about them. Set them to 0. |
| 63:48 | Address offset between adjacent rows in the source matrix Nz fractal. The value range is [1, 65535], in units of 64 bytes (`16 × sizeof(dtype)`, where `dtype` is the data type on the L0C Buffer, supporting `int32_t` and `float`). |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- This API takes effect only on the AIC. Calling it on the AIV returns immediately.
- Call this API before the corresponding L0C Buffer move-out API, and the move-out API must enable the Nz2DN function. The configuration remains in effect until this API is called again to overwrite it.
- `config[63:48]` cannot be 0. Bits other than `config[63:48]` do not take effect and can be set to 0. The API writes `config` as a whole and does not check field values.
- When Nz2DN is enabled, if `config[63:48]` is not equal to 1, unitFlag cannot be enabled at the same time.

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
constexpr uint32_t M = 16;
constexpr uint32_t K = 16;
constexpr uint32_t N = 32;
constexpr uint32_t A_ELEMENTS = M * K;
constexpr uint32_t B_ELEMENTS = K * N;
constexpr uint32_t C_ELEMENTS = M * N;
constexpr uint16_t HALF_ONE = 0x3c00;
constexpr uint16_t HALF_NEG_ONE = 0xbc00;

__global__ __cube__ void asc_set_l0c2gm_channel_para_kernel(
    __gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ float* output)
{
    asc_init();
    __cbuf__ half a_l1[A_ELEMENTS];
    __cbuf__ half b_l1[B_ELEMENTS];
    __ca__ half a_l0[A_ELEMENTS];
    __cb__ half b_l0[B_ELEMENTS];
    __cc__ float c_l0[C_ELEMENTS];

    asc_set_gm2l1_nz_para(1, 1, 16, 0);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), K * sizeof(half), 0, M, K, 0, false);
    asc_set_gm2l1_nz_para(1, 1, 16, 0);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), N * sizeof(half), 0, K, N, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 2, 1, 2);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, M, K, N, 0, false, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);

    asc_set_l0c2gm_channel_para(1ULL << 48);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, N, M, M, M, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, false,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, true);
    asc_sync_pipe(PIPE_ALL);
}

void print_row(const char* label, const std::vector<float>& data, uint32_t offset)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[offset + i];
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<uint16_t> a(A_ELEMENTS, 0);
    std::vector<uint16_t> b(B_ELEMENTS, 0);
    std::vector<float> input_b(B_ELEMENTS, 0.0f);
    std::vector<float> output(C_ELEMENTS, 0.0f);
    std::vector<float> golden(C_ELEMENTS, 0.0f);
    for (uint32_t i = 0; i < M; ++i) {
        a[i * K + i] = HALF_ONE;
        b[i * N + i] = HALF_ONE;
        b[i * N + 16 + i] = HALF_NEG_ONE;
        input_b[i * N + i] = 1.0f;
        input_b[i * N + 16 + i] = -1.0f;
        golden[i * M + i] = 1.0f;
        golden[(16 + i) * M + i] = -1.0f;
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    asc_set_l0c2gm_channel_para_kernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_row("Input B row 0", input_b, 0);
    print_row("DN output row 0", output, 0);
    print_row("DN output row 16", output, 16 * M);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_l0c2gm_channel_para passed."
                         : "[Failed] asc_set_l0c2gm_channel_para failed.") << std::endl;
    aclrtFree(a_device);
    aclrtFree(b_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
<!-- end id8 -->
