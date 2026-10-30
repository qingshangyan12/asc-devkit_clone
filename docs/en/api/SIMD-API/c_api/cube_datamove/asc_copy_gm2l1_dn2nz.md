# asc_copy_gm2l1_dn2nz

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-22T13:03:19.404Z pushedAt=2026-10-28T12:45:45.439Z nl=2 -->

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

Copies data from Global Memory to the L1 Buffer, converting the data from DN to Nz format during the copy so that it meets the fractal column continuity requirement of subsequent cube computation.

Before performing a DN2Nz format copy, call [asc_set_gm2l1_nz_para](asc_set_gm2l1_nz_para.md) to configure the destination Nz cube stride and the number of DN cubes to copy for the DN2Nz copy.

This API takes effect only when executed on the AIC.

As shown in [Figure 1](#dn2nz_equivalent_conversion), the DN-to-Nz format conversion is equivalent to the ND-to-Zn format conversion. You can use this instruction to perform the ND-to-Zn format conversion.

**Figure 1** DN2Nz and ND2Zn diagram<a id="dn2nz_equivalent_conversion"></a>

![](../../../figures/gm2l1_dn2nz_concept.png)

## Prototype

### Template Prototype (Placeholder Form)

```cpp
__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ <dtype>* dst,
                                            __gm__ <dtype>* src,
                                             uint64_t src_d_value,
                                             asc_load_l2_cache_mode l2_cache_mode,
                                            uint16_t n_value,
                                            uint32_t d_value,
                                             uint64_t src_nd_matrix_stride,
                                             bool enable_small_c0)
```

### Data Types Supported by dtype

The data types supported by dtype are `int8_t`, `uint8_t`, `hifloat8_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

## Examples

```cpp
// Example: Copy from DN to Nz format for the half type.
__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ half* dst,
                                            __gm__ half* src,
                                             uint64_t src_d_value,
                                             asc_load_l2_cache_mode l2_cache_mode,
                                            uint16_t n_value,
                                            uint32_t d_value,
                                             uint64_t src_nd_matrix_stride,
                                             bool enable_small_c0)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Destination operand, stored in the L1 Buffer. The destination address must be 32-byte aligned. |
| src | Input | Source operand, stored in GM. The start address must be 1-byte aligned. |
| src_d_value | Input | Offset between the start addresses of adjacent rows of the same DN cube in the source operand, in bytes. Value range: [0, $2^{40}-1$]. |
| l2_cache_mode | Input | Uses the [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md) enumeration value to configure the management policy of the input GM data in L2 Cache. Only `asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`, `asc_load_l2_cache_mode::NORMAL_LAST_VICTIM`, `asc_load_l2_cache_mode::NORMAL_PERSISTENT`, and `asc_load_l2_cache_mode::NOTALLOC_KEEP` are supported. |
| n_value | Input | Number of columns of the DN cube in the source cube, in elements. Value range: [1, 65535]. |
| d_value | Input | Number of rows of the DN cube in the source cube, in elements. Value range: [1, $2^{21}-1$]. |
| src_nd_matrix_stride | Input | Offset between the start addresses of adjacent DN cubes in the source cube, in bytes. Value range: [0, $2^{40}-1$].<br>&nbsp;&nbsp;&bull; When the number of DN cubes in the source cube is 1, this parameter is meaningless; set it to 0.<br>&nbsp;&nbsp;&bull; When the number of DN cubes in the source cube is greater than 1 and `src_nd_matrix_stride = 0`, the first DN cube of the source cube is copied repeatedly. |
| enable_small_c0 | Input | SmallC0 mode switch. It can be enabled only when $d\_value \le 4$; otherwise, the copy will be abnormal.<br>&nbsp;&nbsp;&bull; `false`: SmallC0 mode is disabled, and data is copied in the standard Nz format C0 (32-byte) layout.<br>&nbsp;&nbsp;&bull; `true`: SmallC0 mode is enabled. When the amount of data copied does not satisfy $4 \times \mathrm{sizeof}(\mathrm{dtype})$-byte alignment, the hardware automatically pads zeros in the destination cube to $4 \times \mathrm{sizeof}(\mathrm{dtype})$-byte alignment. |

## Return Value

None

## Pipeline Type

PIPE_MTE2

## Constraints

### General Constraints

- This API returns directly when it is not called by the AIC.
- The start address of `dst` must be 32-byte aligned (L1 Buffer alignment requirement); otherwise, the copy will be abnormal.
- The start address of `src` must be 1-byte aligned (Global Memory alignment requirement); otherwise, the copy will be abnormal.
- If the destination address of this instruction overlaps with that of other instructions, insert synchronization instructions ([asc_sync_notify](../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../sync/intra_core_sync/asc_sync_wait.md)) to serialize the instructions and prevent abnormal data.
- L1 Buffer capacity limit: The total L1 Buffer capacity is 512 KB. The sum of the `dst` offset and the copy size must not exceed the boundary; otherwise, an exception is triggered.

### DN2Nz Constraints

- Before calling this API, call [asc_set_gm2l1_nz_para](asc_set_gm2l1_nz_para.md) to configure the destination Nz cube stride and the number of DN cubes to be moved for DN2Nz data movement.
- The values of `src_d_value`, `n_value`, `d_value`, and `src_nd_matrix_stride` must be within their value ranges. Values outside the ranges are truncated, causing unexpected data movement results.
- When `enable_small_c0` is set to `true`, $d\_value \le 4$; otherwise, data movement is abnormal.

## Example

After saving the code as `examples.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [`__NPU_ARCH__`](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

The sample occupies 64 KB of L1 Buffer, 32 KB of L0A Buffer, 32 KB of L0B Buffer, and 64 KB of L0C Buffer on a single core. The sample reuses `EVENT_ID0` in sequence to complete the synchronization from MTE2 to MTE1, MTE1 to M, and M to FIX. For similar scenarios using basic APIs, see scenario 3 in the [data_copy_gm2l1 sample](../../../../../../examples/01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_gm2l1).

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t M = 128;
constexpr uint32_t K = 128;
constexpr uint32_t N = 128;
constexpr uint32_t ELEMENTS = M * N;
constexpr uint32_t CUBE_M = 16;
constexpr uint32_t CUBE_K = 16;
constexpr uint16_t HALF_ONE = 0x3c00;
constexpr uint16_t HALF_NEG_ONE = 0xbc00;

__global__ __cube__ void AscCopyGm2l1Dn2nzKernel(
    __gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ float* output)
{
    asc_init();
    __cbuf__ half a_l1[M * K], b_l1[N * K];
    __ca__ half a_l0[M * K];
    __cb__ half b_l0[K * N];
    __cc__ float c_l0[ELEMENTS];

    // Inputs A and B are stored in DN format as [K, M] and [N, K], respectively, and are converted to Nz format after being moved in.
    asc_set_gm2l1_nz_para(1, 1, 128, 0);
    asc_copy_gm2l1_dn2nz(reinterpret_cast<__cbuf__ half*>(a_l1), reinterpret_cast<__gm__ half*>(a),
        M * sizeof(half), asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, M, K, 0, false);
    asc_set_gm2l1_nz_para(1, 1, 128, 0);
    asc_copy_gm2l1_dn2nz(reinterpret_cast<__cbuf__ half*>(b_l1), reinterpret_cast<__gm__ half*>(b),
        K * sizeof(half), asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, K, N, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

    // Move the Nz cubes into L0A Buffer and L0B Buffer, respectively, for cube multiplication.
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, M / CUBE_M, K / CUBE_K,
        M / CUBE_M, M / CUBE_M);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, K / CUBE_M, N / CUBE_K,
        K / CUBE_M, N / CUBE_M);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);

    // Compute the 128 x 128 cube multiplication and move the result from L0C Buffer back to Global Memory.
    asc_mmad(c_l0, a_l0, b_l0, M, K, N, asc_unit_flag_mode::DISABLE, false, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, N, M, N, M, asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM,
        asc_unit_flag_mode::DISABLE, QuantMode_t::NoQuant, asc_relu_pre_mode::NONE, false, true, false, false);
    asc_sync_pipe(PIPE_FIX);
}

uint16_t ToHalfBits(int32_t value)
{
    return value > 0 ? HALF_ONE : (value < 0 ? HALF_NEG_ONE : 0);
}

template <typename T>
void PrintRow(const char* label, const std::vector<T>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) {
        std::cout << ' ' << +data[i];
    }
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    // Store A and B in DN format in GM, that is, store the transposed cubes of A and B, respectively.
    std::vector<uint16_t> a(K * M), b(N * K);
    std::vector<float> output(ELEMENTS), golden(ELEMENTS);
    for (uint32_t row = 0; row < M; ++row) {
        for (uint32_t k = 0; k < K; ++k) {
            const int32_t value = static_cast<int32_t>((row + 2 * k) % 3) - 1;
            a[k * M + row] = ToHalfBits(value);
        }
    }
    for (uint32_t k = 0; k < K; ++k) {
        for (uint32_t col = 0; col < N; ++col) {
            const int32_t value = static_cast<int32_t>((2 * k + col) % 3) - 1;
            b[col * K + k] = ToHalfBits(value);
        }
    }
    for (uint32_t row = 0; row < M; ++row) {
        for (uint32_t col = 0; col < N; ++col) {
            int32_t sum = 0;
            for (uint32_t k = 0; k < K; ++k) {
                const int32_t a_value = static_cast<int32_t>((row + 2 * k) % 3) - 1;
                const int32_t b_value = static_cast<int32_t>((2 * k + col) % 3) - 1;
                sum += a_value * b_value;
            }
            golden[row * N + col] = static_cast<float>(sum);
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    AscCopyGm2l1Dn2nzKernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device,
        output.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);

    PrintRow("Output row 0", output);
    PrintRow("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_copy_gm2l1_dn2nz preserved the MMAD operands/result."
                         : "[Failed] asc_copy_gm2l1_dn2nz result mismatch.") << std::endl;

    aclrtFree(a_device);
    aclrtFree(b_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
