# asc_copy_l12l0a_arch_3510 (2D Matrix Transfer)

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-22T12:40:12.543Z pushedAt=2026-10-28T12:45:45.407Z nl=2 -->

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

Transfers 2D-format data required for cube computation. The transfer granularity is a 512-byte data fractal, supporting both non-transposed and transposed cube transfer from L1 Buffer to L0A Buffer.

For different data types, the corresponding fractal cubes are as follows:

- For the b4 data type, each data fractal is a 16×64 cube in L0A Buffer.
- For the b8 data type, each data fractal is a 16×32 cube in L0A Buffer.
- For the b16 data type, each data fractal is a 16×16 cube.
- For the b32 data type, each data fractal is a 16×8 cube in L0A Buffer.

This API takes effect only when executed on the AIC.

## Prototype

### 2D Transfer Mode

```cpp
__aicore__ inline void asc_copy_l12l0a(__ca__ <dtype>* dst,
                                       __cbuf__ <dtype>* src,
                                       uint16_t m_start_position,
                                       uint16_t k_start_position,
                                       uint8_t m_step,
                                       uint8_t k_step,
                                       int16_t src_stride,
                                       uint16_t dst_stride)
```

### Data Types Supported by dtype

The data types supported by dtype are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e2m1_t`, `fp4x2_e1m2_t`, `hifloat8_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

### Prototype Example

```cpp
// Example: 2D transfer of the half type.
__aicore__ inline void asc_copy_l12l0a(__ca__ half* dst,
                                       __cbuf__ half* src,
                                       uint16_t m_start_position,
                                       uint16_t k_start_position,
                                       uint8_t m_step,
                                       uint8_t k_step,
                                       int16_t src_stride,
                                       uint16_t dst_stride)
```

### 2D Transpose Transfer Mode

```cpp
__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ <dtype>* dst,
                                                 __cbuf__ <dtype>* src,
                                                 uint16_t m_start_position,
                                                 uint16_t k_start_position,
                                                 uint8_t m_step,
                                                 uint8_t k_step,
                                                 int16_t src_stride,
                                                 uint16_t dst_stride)
```

### Data Types Supported by dtype

The data types supported by dtype are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e2m1_t`, `fp4x2_e1m2_t`, `hifloat8_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

### Prototype Example

```cpp
// Example: 2D transfer of the half type.
__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ half* dst,
                                       __cbuf__ half* src,
                                       uint16_t m_start_position,
                                       uint16_t k_start_position,
                                       uint8_t m_step,
                                       uint8_t k_step,
                                       int16_t src_stride,
                                       uint16_t dst_stride)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Meaning |
|---|---|---|
| dst | Output | Destination cube, stored in the L0A Buffer. The start address must be 512-byte aligned. |
| src | Input | Source cube, stored in the L1 Buffer. The start address must be 32-byte aligned. |
| m_start_position | Input | Start fractal position of the source cube in the row direction, in units of 16 elements. Value range: $[0, 2^{16}-1]$. |
| k_start_position | Input | Start fractal position of the source cube in the column direction, in units of 32 bytes. Value range: $[0, 2^{16}-1]$. |
| m_step | Input | Fractal step of the source cube in the row direction, that is, the number of fractal rows to transfer, in units of 16 elements. Value range: $[0, 255]$.<br>In the transpose scenario (when calling `asc_copy_l12l0a_transpose`), in addition to the value range, m_step must satisfy the following extra constraints:<br>&nbsp;&nbsp;&bull;When the data type is b4, m_step must be a multiple of 4;<br>&nbsp;&nbsp;&bull;When the data type is b8, m_step must be a multiple of 2;<br>&nbsp;&nbsp;&bull;When the data type is b16, m_step must be a multiple of 1;<br>&nbsp;&nbsp;&bull;When the data type is b32, m_step has no extra constraint.<br>**Note: m_step=0 means no transfer is performed, and this API is treated as a NOP (no operation).** |
| k_step | Input | Fractal step of the source cube in the column direction, that is, the number of fractal columns to transfer, in units of 32 bytes. Value range: $[0, 255]$.<br>In the transpose scenario (when calling `asc_copy_l12l0a_transpose`), in addition to the value range, k_step must satisfy the following extra constraints:<br>&nbsp;&nbsp;&bull;When the data type is b4, b8, or b16, k_step has no extra constraint;<br>&nbsp;&nbsp;&bull;When the data type is b32, k_step must be a multiple of 2.<br>**Note: k_step=0 means no transfer is performed, and this API is treated as a NOP (no operation).** |
| src_stride | Input | Interval between the start address of the previous fractal and the start address of the next fractal in the column direction of the source cube, in units of 512 bytes. Value range: $[-2^{15}, 2^{15}-1]$. |
| dst_stride | Input | Interval between the start address of the previous fractal and the start address of the next fractal in the column direction of the destination cube, in units of 512 bytes. Value range: $[1, 2^{16}-1]$. |

Taking the non-transpose scenario as an example, the parameter values and their explanations are shown in Figure 1:

- $m\_start\_position=2$, indicating that the start position of the source cube in the row direction is the 32nd element, that is, $2 \times 16=32$ elements.
- $k\_start\_position=2$, indicating that the start offset of the source cube in the column direction is $2 \times 32=64$ bytes.
- $m\_step=2$, indicating that the transfer length of the source cube in the row direction is $2 \times 16=32$ elements.
- $k\_step=3$, indicating that the transfer length of the source cube in the column direction is $3 \times 32=96$ bytes.
- $src\_stride=5$, indicating that the interval between the start addresses of adjacent fractals in the column direction of the source cube is $5 \times 512=2560$ bytes.
- $dst\_stride=3$, indicating that the interval between the start addresses of adjacent fractals in the destination cube column direction is $3 \times 512=1536$ bytes.

**Figure 1**  l12l0a_2d transfer parameter example (using the half data type, non-transpose as an example)

![l12l0a_2d transfer parameter example](../../../../figures/loaddata2dparamsv2_half_no_transpose.png)

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

### General Constraints

- This API returns directly when it is not called by the AIC.
- The dst start address must be 512-byte aligned (L0A Buffer alignment requirement); otherwise, the transfer will be abnormal.
- The src start address must be 32-byte aligned (L1 Buffer alignment requirement); otherwise, the transfer will be abnormal.
- src is located in the L1 Buffer, and dst is located in the L0A Buffer. They belong to different physical storage units, so there is no scenario where the source operand and destination operand addresses overlap.
- If the destination address of this instruction overlaps with that of other instructions, insert synchronization instructions ([asc_sync_notify](../../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../../sync/intra_core_sync/asc_sync_wait.md)) to serialize the instructions and prevent abnormal data.
- L0A Buffer capacity limit: The total L0A Buffer capacity is 64 KB. The sum of the dst offset and the transfer size must not exceed the boundary; otherwise, an exception is triggered.
- L1 Buffer capacity limit: The total L1 Buffer capacity is 512 KB. The sum of the src offset and the source cube occupied size must not exceed the boundary; otherwise, an exception is triggered.

### 2D Fractal Transfer Constraints

- When m_step or k_step is 0, no transfer is performed and this API is treated as a NOP (No Operation). Avoid passing 0 unintentionally, as it produces no valid data and increases instruction scheduling overhead.
- The API uses a complete 512-byte fractal as the minimum transfer granularity and does not support transferring only part of the elements within a fractal. Invalid elements in a fractal must be filled before the call according to the subsequent cube computation requirements.
- For the cube transpose scenario, different data types must satisfy different m_step and k_step constraints: for the b4 data type, m_step must be a multiple of 4; for the b8 data type, m_step must be a multiple of 2; for the b16 data type, m_step must be a multiple of 1; for the b32 data type, k_step must be a multiple of 2.
- For the cube transpose scenario, because of the m_step and k_step constraints, ensure that sufficient space is reserved in the L1 Buffer to avoid read out-of-bounds, which would cause undefined transfer behavior.
- For the b8 transpose scenario transfer, that is, calling the `asc_copy_l12l0a_transpose` API, assume the C cube size is (M, N). If $M > 32$ and $M \bmod 32 \le 16$:
  - In this scenario, if the `asc_copy_l12l0a_transpose` transfer API is called only once, because the minimum transfer unit in the b8 transpose scenario is two fractals, one invalid fractal is transferred in the M direction. During the subsequent Mmad computation, this fractal must participate in the computation; otherwise, the computation result is incorrect. Finally, when the cube is transferred out, only the valid region is transferred out. In this calling mode, the computed data size differs from the transferred-out data size, so the UnitFlag feature cannot be enabled when the cube is transferred out.
  - If the `asc_copy_l12l0a_transpose` transfer API is called multiple times in a for loop, set the `dst_stride` parameter appropriately to skip the invalid fractal in the M direction that is read additionally during the transpose when writing to the L0A Buffer. Then the Mmad computation does not need to include this fractal, and the computed data size is consistent with the transferred-out data size. In this scenario, the UnitFlag feature can be enabled when the cube is transferred out. However, note that using a for loop for multiple transfers can easily fill the MTE1 instruction queue, which may cause pipeline blocking in extreme pipelining scenarios. Choose whether to use a for loop for multiple transfers appropriately.

## Key Features

### Transfer Start Position Calculation in Non-transpose Scenario

Use m_step and k_step to transfer data blocks in multiple directions. Take the Nz fractal of $[M, K]$ as an example:

- m_start_position is 2, indicating that the transfer start position in the M direction is $16 \times 2=32$ elements away from the source operand start position along the M axis.
- k_start_position is 2, indicating that the transfer start position in the K direction is $32 \times 2=64$ bytes away from the source operand start position along the K axis.
- src_stride is 5, indicating that in the source operand, the interval between the start address of the previous fractal and that of the next fractal in the K direction is 5.
- dst_stride is 3, indicating that in the destination operand, the interval between the start address of the previous fractal and that of the next fractal in the K direction is 3.

The start address is calculated as follows:

$$
startAddr = srcAddr + (k_start_position \times \lvert src_stride \rvert + m_start_position) \times 512B
$$

Therefore, the final transfer start position is the thirteenth fractal cube (the first fractal cube starts from the source operand start address).

**Figure 2** Transfer start position diagram

![Transfer Start Position Diagram](../../../../figures/load2dv2_start_addr_demo.png "Transfer Start Position Diagram")

### Transpose Scenario Load In

For cube transpose operations on b4/b8/b16/b32, the following constraints on m_step and k_step must be met:

- For the b4 data type, m_step must be a multiple of 4.
- For the b8 data type, m_step must be a multiple of 2.
- For the b16 data type, m_step must be a multiple of 1.
- For the b32 data type, k_step must be a multiple of 2.

The transpose layouts of each data type are shown in Figure 3 to Figure 6.

**Figure 3** Transpose diagram for the b4 data type

![Transpose diagram for the b4 data type](../../../../figures/load2dv2_l12l0_trans_b4.png "Transpose diagram for the b4 data type")

**Figure 4** Transpose diagram for the b8 data type

![b8 data type transpose diagram](../../../../figures/load2dv2_l12l0_trans_b8.png "b8 data type transpose diagram")

**Figure 5** b16 data type transpose diagram

![b16 data type transpose diagram](../../../../figures/load2dv2_l12l0_trans_b16.png "b16 data type transpose diagram")

**Figure 6** b32 data type transpose diagram

![b32 data type transpose diagram](../../../../figures/load2dv2_l12l0_trans_b32.png "b32 data type transpose diagram")

## Examples

For the complete project sample in the repository, see [mmad sample](../../../../../../../examples/02_simd_c_api/03_c_api/03_matrix_compute/mmad).

After saving the code as `examples.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [`__NPU_ARCH__`](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

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
constexpr uint32_t CUBE_K = 32;

__global__ __cube__ void AscCopyL12l0aKernel(__gm__ int8_t* a, __gm__ int8_t* b,
    __gm__ int32_t* output)
{
    asc_init();
    __cbuf__ int8_t a_l1[M * K], b_l1[N * K];
    __ca__ int8_t a_l0[M * K];
    __cb__ int8_t b_l0[K * N];
    __cc__ int32_t c_l0[ELEMENTS];

    // Configure the Nz format and transfer the 128 x 128 cube from GM to the L1 Buffer.
    asc_set_gm2l1_nz_para(1, 1, 128, 0);
    asc_copy_gm2l1_nd2nz(a_l1, a, K, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, M, K, 0, false);
    asc_set_gm2l1_nz_para(1, 1, 128, 0);
    asc_copy_gm2l1_nd2nz(b_l1, b, K, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, N, K, 0, false);
    // Wait until both GM-to-L1 transfers finish before PIPE_MTE1 reads L1 Buffer.
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

    // Move all A and B fractals to L0A Buffer and L0B Buffer for the 128x128x128 cube multiplication.
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, M / CUBE_M, K / CUBE_K, M / CUBE_M, M / CUBE_M);
    asc_copy_l12l0b(b_l0, b_l1, 0, 0, N / CUBE_M, K / CUBE_K, N / CUBE_M, N / CUBE_M);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);

    // Multiply A and B after PIPE_MTE1 has produced both operands.
    asc_mmad(c_l0, a_l0, b_l0, M, K, N, asc_unit_flag_mode::DISABLE, true, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);

    // Convert the Nz result in L0C to ND format and transfer it back to GM.
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, N, M, N, M, asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM,
        asc_unit_flag_mode::DISABLE, QuantMode_t::NoQuant, asc_relu_pre_mode::NONE, false, true, false, false);
    asc_sync_pipe(PIPE_FIX);
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
    // Generate deterministic dense inputs and the corresponding host-side golden result.
    std::vector<int8_t> a(M * K), b(N * K);
    std::vector<int32_t> output(ELEMENTS), golden(ELEMENTS);
    for (uint32_t row = 0; row < M; ++row) {
        for (uint32_t k = 0; k < K; ++k) {
            a[row * K + k] = static_cast<int8_t>(static_cast<int32_t>((row + 2 * k) % 5) - 2);
        }
    }
    for (uint32_t col = 0; col < N; ++col) {
        for (uint32_t k = 0; k < K; ++k) {
            b[col * K + k] = static_cast<int8_t>(static_cast<int32_t>((3 * col + k) % 5) - 2);
        }
    }
    for (uint32_t row = 0; row < M; ++row) {
        for (uint32_t col = 0; col < N; ++col) {
            for (uint32_t k = 0; k < K; ++k) {
                golden[row * N + col] += a[row * K + k] * b[col * K + k];
            }
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    int8_t *a_device = nullptr, *b_device = nullptr;
    int32_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(int32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size(), a.data(), a.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size(), b.data(), b.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    AscCopyL12l0aKernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int32_t), output_device,
        output.size() * sizeof(int32_t), ACL_MEMCPY_DEVICE_TO_HOST);

    PrintRow("Output row 0", output);
    PrintRow("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_copy_l12l0a result is correct."
                         : "[Failed] asc_copy_l12l0a result mismatch.") << std::endl;

    aclrtFree(a_device);
    aclrtFree(b_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
