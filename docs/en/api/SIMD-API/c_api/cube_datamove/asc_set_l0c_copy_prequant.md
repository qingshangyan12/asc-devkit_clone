# asc_set_l0c_copy_prequant

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-22T13:28:20.634Z pushedAt=2026-10-28T12:45:45.480Z nl=2 -->

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

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

This API configures the scaling factor, offset, and sign of the quantization result required for quantization computation when in-line scalar quantization is performed during L0C buffer eviction. After configuration, the settings are used by the subsequently called [asc_copy_l0c2gm](./asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md), [asc_copy_l0c2l1](./asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md), or [asc_copy_l0c2ub](./asc_copy_l0c2ub.md) API, and must be used together with the `quant_pre_mode` parameter of the eviction API.

The scalar quantization enumeration values supported by the `quant_pre_mode` quantization mode parameter are as follows:

- DEQF16: Quantizes int32_t to half, scalar quantization.
- REQ4: Quantizes int32_t to int4b_t, scalar quantization.
- REQ8: Quantizes int32_t to int8_t/uint8_t, scalar quantization.
- QS322BF16_PRE: Quantizes int32_t to bfloat16_t, scalar quantization.
- QF322F16_PRE: Quantizes float to half, scalar quantization.
- QF322BF16_PRE: Quantizes float to bfloat16_t, scalar quantization.
- QF322S4_PRE: Quantizes float to int4b_t, scalar quantization.
- QF322B8_PRE: float quantized to int8_t/uint8_t, scalar quantization.
- QF322FP8_PRE: float quantized to fp8_e4m3fn_t, scalar quantization.
- QF322HIF8_PRE: float quantized to hifloat8_t (Half to Away Round), scalar quantization.
- QF322HIF8_PRE_HYBRID: float quantized to hifloat8_t (Hybrid Round), scalar quantization.
- QF322F32_PRE: float quantized to float, scalar quantization. The precision can reach two thousandths but cannot reach two ten-thousandths.

This API is a cube copy-out related configuration API and takes effect only on the AIC.

## Prototype

```cpp
__aicore__ inline void asc_set_l0c_copy_prequant(float scale, uint16_t offset, bool is_signed);

__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config);
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| scale | Input | Scaling factor used by the in-band scalar quantization. The value cannot be inf or nan. |
| offset | Input | Offset required for the in-band quantization computation, with a value range of [0, 511]. Set this parameter to 0 when no offset is used. It takes effect in REQ8/QF322B8_PRE/REQ4/QF322S4_PRE modes. |
| is_signed | Input | Whether the quantization result is signed data. When the value is `true`, the quantization result is int8_t; when the value is `false`, the quantization result is uint8_t. It takes effect in REQ8/QF322B8_PRE modes. |
| config | Input | Scalar quantization parameter assembled from `scale`, `offset`, and `is_signed`. See Table 2 for the meaning of each bit of config. |

**Table 2** Meaning of each bit of config

| Bit Width | Variable Name | Description |
| --- | --- | --- |
| 0~12 | - | Invalid bit. |
| 13~31 | scale | The data type is treated as float, and the hardware performs computation in the (1, 8, 10) format. Bit 31 is the sign bit, bits 23 to 30 are the exponent bits, and bits 13 to 22 are the mantissa bits. The value cannot be inf or nan. |
| 32~36 | - | Invalid bit. |
| 37~45 | offset | 9-bit integer data. The result of multiplying the source data by the quantization factor is added to Offset. Set this parameter to 0 when no offset is used. It takes effect in REQ8/QF322B8_PRE/REQ4/QF322S4_PRE modes. |
| 46 | is_signed | When the value is 1, the quantization result is int8_t; when the value is 0, the quantization result is uint8_t. It takes effect in REQ8/QF322B8_PRE modes. |
| 47~63 | - | Invalid bit. |

Python sample code for the REQ8 quantization algorithm is provided. See [\(V\)REQ8 quantization algorithm](../../../../../../examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/fixpipe_l0c2gm/scripts/gen_data_s322s8.py).

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- The quantization parameters cannot be inf, nan, or denormal numbers.
- This API returns directly when it is not called by the AIC.
- This API must be called before the corresponding L0C Buffer copy-out API is executed, and the corresponding Scalar quantization mode must be configured in the copy-out API.

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
constexpr uint32_t DIM = 16;
constexpr uint32_t ELEMENTS = DIM * DIM;
constexpr uint16_t HALF_ONE = 0x3c00;
constexpr float SCALE = 2.0f;
constexpr uint16_t OFFSET = 3;

__global__ __cube__ void asc_set_l0c_copy_prequant_kernel(
    __gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ int8_t* output)
{
    asc_init();
    __cbuf__ half a_l1[ELEMENTS], b_l1[ELEMENTS];
    __ca__ half a_l0[ELEMENTS];
    __cb__ half b_l0[ELEMENTS];
    __cc__ float c_l0[ELEMENTS];

    asc_set_gm2l1_nz_para(1, 1, 16, 0);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), DIM * sizeof(half), asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, DIM, DIM, 0, false);
    asc_set_gm2l1_nz_para(1, 1, 16, 0);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), DIM * sizeof(half), asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, DIM, DIM, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 1, 1, 1);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, DIM, DIM, DIM, asc_unit_flag_mode::DISABLE, false, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);

    asc_set_l0c_copy_prequant(SCALE, OFFSET, true);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, DIM, DIM, DIM, DIM, asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM,
        asc_unit_flag_mode::DISABLE, QuantMode_t::QF322B8_PRE, asc_relu_pre_mode::NONE, false, true, false, false);
    asc_sync_pipe(PIPE_ALL);
}

void print_row(const char* label, const std::vector<int8_t>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << static_cast<int32_t>(data[i]);
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<uint16_t> a(ELEMENTS), b(ELEMENTS);
    std::vector<int8_t> output(ELEMENTS), golden(ELEMENTS, static_cast<int8_t>(OFFSET));
    for (uint32_t i = 0; i < DIM; ++i) {
        a[i * DIM + i] = HALF_ONE;
        b[i * DIM + i] = HALF_ONE;
        golden[i * DIM + i] = static_cast<int8_t>(SCALE + OFFSET);
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    int8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(int8_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    asc_set_l0c_copy_prequant_kernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int8_t), output_device, output.size() * sizeof(int8_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_row("Quantized output row 0", output);
    print_row("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_l0c_copy_prequant scalar quantization passed."
                         : "[Failed] asc_set_l0c_copy_prequant result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
<!-- end id8 -->
