# asc_copy_gm2l1_arch_3510 (High-Dimensional Split Data Movement) (Deprecated)

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T00:32:08.355Z pushedAt=2026-10-28T12:55:18.801Z nl=2 -->

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
<!-- npu="910" id4 -->
- Atlas training products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference product AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference product Vector Core: Not supported
<!-- end id6 -->
<!-- npu="310b" id7 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id7 -->

## Description

**The old regular overload of `asc_copy_gm2l1` that uses `uint8_t pad_func_mode` and its `asc_copy_gm2l1_sync` API are deprecated. Use the [asc_copy_gm2l1](../cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_highdim_split_arch_3510.md) API with the `asc_channel_pad_mode pad_mode` parameter instead.**

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

Moves data from Global Memory to the L1 Buffer, keeping the format and content unchanged during data movement. High-dimensional split means that non-contiguous movement can be achieved by configuring movement parameters such as the number of data blocks, the length of a single data block, and the address offset. In addition, this API supports inserting or removing padding for 32-byte-granularity data in per-channel fill mode during the movement. The padding value must be set through [asc_set_gm2l1_pad](../cube_datamove/asc_set_gm2l1_pad.md).

This API takes effect only when executed on the AIC.

## Prototype

### high-dimensional split data movement

```c
__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t n_burst,
                                      uint32_t len_burst, uint8_t pad_func_mode, uint64_t src_stride,
                                      uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t n_burst,
                                           uint32_t len_burst, uint8_t pad_func_mode, uint64_t src_stride,
                                           uint32_t dst_stride)
```

## Parameters

### High-Dimensional Split Data Movement

**Table 1** Parameter description

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Destination operand, stored in the L1 Buffer. The destination address must be 32-byte aligned. |
| src | Input | Source operand, stored in GM. The start address must be 1-byte aligned. |
| n_burst | Input | Number of data blocks to move. Value range: [1, $2^{17}-1$]. |
| len_burst | Input | Length of each data block to move, in 32-byte units. Value range: [1, $2^{17}-1$]. |
| pad_func_mode | Input | Fill or removal data mode during the movement. Value range: [0, 8]. Fill mode inserts padding values into the destination data. The padding values must be set through [asc_set_gm2l1_padding](../cube_datamove/asc_set_gm2l1_padding.md). Removal mode removes part of the data from each 32 bytes being moved.<br>&nbsp;&nbsp;&bull; `0`: No padding.<br>&nbsp;&nbsp;&bull; `1`: For every 1 byte of data, insert 31 padding values, where each inserted padding value has a bit width of b8.<br>&nbsp;&nbsp;&bull; `2`: For every 2 bytes of data, insert 15 padding values, where each inserted padding value has a bit width of b16.<br>&nbsp;&nbsp;&bull; `3`: For every 4 bytes of data, insert 14 padding values, where each inserted padding value has a bit width of b16.<br>&nbsp;&nbsp;&bull; `4`: For every 8 bytes of data, insert 12 padding values, where each inserted padding value has a bit width of b16.<br>&nbsp;&nbsp;&bull; `5`: For every 16 bytes of data, insert 8 padding values, where each inserted padding value has a bit width of b16.<br>&nbsp;&nbsp;&bull; `6`: For every 32 bytes of data, remove the 28 most significant bits of data and keep only the 4 least significant bytes.<br>&nbsp;&nbsp;&bull; `7`: For every 32 bytes of data, remove the 24 most significant bits of data and keep only the 8 least significant bytes.<br>&nbsp;&nbsp;&bull; `8`: For every 32 bytes of data, remove the 16 most significant bits of data and keep only the 16 least significant bytes. |
| src_stride | Input | Interval between adjacent data blocks of the source operand (that is, the difference between the start address of the previous data block and the start address of the next data block). Value range: [0, $2^{36}-1$], in 32-byte units.<br>&nbsp;&nbsp;&bull; When n_burst = 1, `src_stride` is meaningless and can be set to 0. |
| dst_stride | Input | Interval between adjacent data blocks of the destination operation (that is, the difference between the start address of the previous data block and the start address of the next data block). Value range: [0, $2^{17}-1$], in 32-byte units.<br>&nbsp;&nbsp;&bull; When n_burst = 1, `dst_stride` is meaningless and can be set to 0. |

- For scenarios where `pad_func_mode` is set to fill data mode:
  - In fill mode, the length of each data block to move, `len_burst`, must be set to `1`. After filling, the length of each data block is 32 bytes in all fill modes.
  - In fill mode, moving one data block reads only 1/2/4/8/16 bytes of data from the source operand in GM, corresponding to the five fill modes in order.
  - Each data block movement reads continuously from the source operand in GM, and `src_stride` can be set to `0`.
- For scenarios where `pad_func_mode` is set to removal data mode:
  - For one data block movement of the source operand, the data read is `len_burst * 32` bytes, and the data actually written to the destination operation is `len_burst * 4/8/16` bytes, corresponding to the three removal modes in order.
  - Each data block movement writes continuously to the destination operation in the L1 Buffer, and `dst_stride` can be set to `0`.

## Return Value

None

## Pipeline Type

PIPE_MTE2

## Constraints

### General Constraints

- This API takes effect only on the AIC. Calls from non-AIC cores return directly.
- The `dst` start address must be 32-byte aligned (L1 Buffer alignment requirement); otherwise, the copy will be abnormal.
- The `src` start address must be 1-byte aligned (Global Memory alignment requirement); otherwise, the copy will be abnormal.
- If the destination address of this instruction overlaps with that of other instructions, insert synchronization instructions ([asc_sync_notify](../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../sync/intra_core_sync/asc_sync_wait.md)) to serialize the instructions and prevent abnormal data.
- L1 Buffer capacity limit: The total L1 Buffer capacity is 512 KB. The sum of the dst offset and the copy size must not exceed the boundary; otherwise, an exception is triggered.

### Constraints on High-Dimensional Split Data Movement

- The value range of `n_burst` is [1, $2^{17}-1$]. Values outside this range are truncated, causing the movement result to be unexpected.
- The value range of `len_burst` is [1, $2^{17}-1$]. Values outside this range are truncated, causing the movement result to be unexpected.
- The value range of `src_stride` is [0, $2^{36}-1$]. Values outside this range are truncated, causing the movement result to be unexpected.
- The value range of `dst_stride` is [0, $2^{17}-1$]. Values outside this range are truncated, causing the movement result to be unexpected.

s

After saving the code as `examples.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [`__NPU_ARCH__`](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

In the sample, the shapes of matrices A, B, and C are all [128,128], and the shape of the Bias matrix is [1,128]. The Bias is moved from GM to the L1 Buffer as two 256-byte blocks through the high-dimensional split movement API, and then moved to the BiasTable Buffer by calling [asc_copy_l12bt](../cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_3510.md). Finally, MMAD completes the `A × B + Bias` computation. In the sample, a single core occupies 64.5 KB of L1 Buffer, 32 KB of L0A Buffer, 32 KB of L0B Buffer, 64 KB of L0C Buffer, and 512 bytes of BiasTable Buffer. The sample reuses `EVENT_ID0` in sequence to complete the synchronization from MTE2 to MTE1, from MTE1 to M, and from M to FIX.

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
constexpr uint32_t A_ELEMENTS = M * K;
constexpr uint32_t B_ELEMENTS = N * K;
constexpr uint32_t C_ELEMENTS = M * N;
constexpr uint32_t BIAS_ELEMENTS = N;
constexpr uint32_t CUBE = 16;
constexpr uint16_t HALF_ONE = 0x3c00;
constexpr uint16_t HALF_NEG_ONE = 0xbc00;
constexpr uint32_t BIAS_BLOCK_BYTES = 256;
constexpr uint32_t BIAS_BLOCKS = 2;
constexpr uint32_t BIAS_BLOCK_LEN = BIAS_BLOCK_BYTES / 32;

__global__ __cube__ void AscCopyGm2l1HighdimBiasKernel(
    __gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ float* bias, __gm__ float* output)
{
    asc_init();
    __cbuf__ half a_l1[A_ELEMENTS], b_l1[B_ELEMENTS];
    __cbuf__ float bias_l1[BIAS_ELEMENTS];
    __ca__ half a_l0[A_ELEMENTS];
    __cb__ half b_l0[B_ELEMENTS];
    __cc__ float c_l0[C_ELEMENTS];

    asc_set_gm2l1_nz_para(1, 1, 128, 0);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), K * sizeof(half), 0, M, K, 0, false);
    asc_set_gm2l1_nz_para(1, 1, 128, 0);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), K * sizeof(half), 0, N, K, 0, false);

    // Split the 1 x 128 Bias data into two 256-byte blocks and move them from GM to the L1 Buffer. Alternatively, move a 512-byte block in one continuous operation.
    asc_copy_gm2l1(reinterpret_cast<__cbuf__ void*>(bias_l1), reinterpret_cast<__gm__ void*>(bias), BIAS_BLOCKS,
        BIAS_BLOCK_LEN, 0, BIAS_BLOCK_LEN, BIAS_BLOCK_LEN);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

    asc_copy_l12l0a(a_l0, a_l1, 0, 0, M / CUBE, K / CUBE, M / CUBE, M / CUBE);
    asc_copy_l12l0b(b_l0, b_l1, 0, 0, N / CUBE, K / CUBE, N / CUBE, N / CUBE);
    asc_copy_l12bt(0, bias_l1, BIAS_ELEMENTS * sizeof(float));
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);

    // Read the Bias from the BiasTable Buffer to initialize matrix C, and then perform the A x B + Bias computation.
    asc_mmad(c_l0, a_l0, b_l0, M, K, N, 0, false, true, false);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, N, M, N, M, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_ALL);
}

uint16_t ToHalfBits(int32_t value)
{
    return value > 0 ? HALF_ONE : (value < 0 ? HALF_NEG_ONE : 0);
}

void PrintRow(const char* label, const std::vector<float>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[i];
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<uint16_t> a(A_ELEMENTS), b(B_ELEMENTS);
    std::vector<float> bias(BIAS_ELEMENTS), output(C_ELEMENTS), golden(C_ELEMENTS);
    for (uint32_t row = 0; row < M; ++row) {
        for (uint32_t k = 0; k < K; ++k) {
            const int32_t value = static_cast<int32_t>((row + 2 * k) % 3) - 1;
            a[row * K + k] = ToHalfBits(value);
        }
    }
    for (uint32_t col = 0; col < N; ++col) {
        for (uint32_t k = 0; k < K; ++k) {
            const int32_t value = static_cast<int32_t>((2 * k + col) % 3) - 1;
            b[col * K + k] = ToHalfBits(value);
        }
        bias[col] = static_cast<float>(static_cast<int32_t>(col % 5) - 2);
    }
    for (uint32_t row = 0; row < M; ++row) {
        for (uint32_t col = 0; col < N; ++col) {
            int32_t sum = 0;
            for (uint32_t k = 0; k < K; ++k) {
                const int32_t a_value = static_cast<int32_t>((row + 2 * k) % 3) - 1;
                const int32_t b_value = static_cast<int32_t>((2 * k + col) % 3) - 1;
                sum += a_value * b_value;
            }
            golden[row * N + col] = static_cast<float>(sum) + bias[col];
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    float *bias_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&bias_device), bias.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(bias_device, bias.size() * sizeof(float), bias.data(), bias.size() * sizeof(float), ACL_MEMCPY_HOST_TO_DEVICE);
    AscCopyGm2l1HighdimBiasKernel<<<1, 0>>>(a_device, b_device, bias_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);

    PrintRow("Bias", bias);
    PrintRow("Output row 0", output);
    PrintRow("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_copy_gm2l1 high-dimensional bias copy passed."
                         : "[Failed] asc_copy_gm2l1 high-dimensional bias copy mismatch.") << std::endl;

    aclrtFree(a_device);
    aclrtFree(b_device);
    aclrtFree(bias_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
