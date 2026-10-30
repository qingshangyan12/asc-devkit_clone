# asc_copy_l12l0b_trans

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-28T13:35:58.791Z pushedAt=2026-10-29T02:01:15.755Z nl=2 -->

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

Copies data from the L1 Buffer to the L0B Buffer, performing a 2D fractal cube transpose in units of 512-byte data fractals during the copy. This API organizes the copy based on parameters such as fractal index, repeat count, source stride, destination gap, and inter-fractal gap. It applies to copy scenarios where 2D-format data required for ordinary cube computation is moved between the L1 Buffer and the L0B Buffer with transposition.

The following examples explain the API functionality and key parameters: in the figures below, an N shape or a Z shape represents a fractal.

- For the uint8_t/int8_t data type, each iteration processes 32 × 32 × 1B of data and can handle 2 fractals (a fractal is 512B). In each iteration, 2 consecutive 16 × 32 fractals in the source operand are merged into 1 32 × 32 block cube, transposed based on the block cube, and then split into 2 16 × 32 fractals after transposition. The transposed fractals can have different layouts according to the destination operand fractal interval and other parameters.

As shown in the figure below:

- A total of 3072B of data is processed. Each iteration processes 32 × 32 × 1B of data, requiring 3 iterations to complete, with repeat_time = 3.
- src_stride = 1, representing the interval between the start address of the previous block cube and the start address of the next block cube in the source operand across adjacent iterations, which is 1 (unit: 32 × 32 × 1B). The unit here is actually the size of the block cube after concatenation.
- dst_gap = 1, representing the interval between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration in the destination operand across adjacent iterations, which is 1 (unit: 512B).
- dst_frac_gap = 0, representing the interval between the end address of the previous fractal and the start address of the next fractal of the destination operand within each iteration, which is 0 (unit: 512B).
![](../../../../figures/asc_copy_l12l0_trans_b8_example1.png)

As shown in the figure below:

- The explanation of repeat_time and src_stride is the same as in the example in the figure above.
- dst_gap = 0, indicating that between adjacent iterations, there is no interval between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration in the destination operand.
- dst_frac_gap = 2, indicating that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal in the destination operand is 2 (unit: 512B).
![](../../../../figures/asc_copy_l12l0_trans_b8_example2.png)

- For the half/bfloat16_t data type, each iteration processes 16 × 16 × 2B of data and can process 1 fractal (a fractal is 512B). In each iteration, one 16 × 16 fractal in the source operand will be transposed.
  - Because each iteration processes 16 × 16 × 2B of data, 3 iterations are required to complete, so repeat_time = 3;
  - src_stride = 1, indicating that between adjacent iterations, the interval between the start address of the previous block cube and the next block cube in the source operand is 1 (unit: 16 × 16 × 2B);
  - dst_gap = 0, indicating that between adjacent iterations, there is no interval between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration in the destination operand;
  - In this scenario, because the fractal is the block cube itself, each iteration processes one fractal, so there is no interval between fractals within an iteration, and this parameter setting is ineffective.
![](../../../../figures/asc_copy_l12l0_trans_b16_example.png)

- For the float/int32_t/uint32_t data types, each iteration processes 16 × 16 × 4B of data and can handle 2 fractals (a fractal is 512B). In each iteration, 2 consecutive 16 × 8 fractals of the source operand will be merged into a 16 × 16 block cube, a block-based cube transpose is performed, and the transposed fractal is then split into 2 16 × 8 fractals, which can have different layouts according to the destination operand fractal interval and other parameters.
As shown in the figure below:
  - Because each iteration processes 16 × 16 × 4B of data, 3 iterations can complete the operation, so repeat_time = 3;
  - src_stride = 1, representing that between adjacent iterations, the interval between the start address of the previous block cube and the next block cube of the source operand is 1 (unit: 16 × 16 × 4B). The unit here is actually the cube size after concatenation;
  - dst_gap = 1, representing that between adjacent iterations, the interval from the end address of the first fractal of the previous operand in the destination operand to the start address of the first fractal of the next iteration is 1 (unit: 512B);
  - dst_frac_gap = 0, representing that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal of the destination operand is 0 (unit: 512B).
![](../../../../figures/asc_copy_l12l0_trans_b32_example1.png)

As shown in the figure below:
- The explanation of repeat_time and src_stride is the same as the example in the figure above.
- dst_gap = 0, indicating that between adjacent iterations, there is no interval between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration in the destination operand.
- dst_frac_gap = 2, indicating that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal in the destination operand is 2 (unit: 512B).
![](../../../../figures/asc_copy_l12l0_trans_b32_example2.png)

- For the int4b\_t data type, each iteration processes 64\*64\*0.5B data and can process 4 fractals (a fractal is 512B). In each iteration, 4 consecutive 16\*64 fractals in the source operand will be merged into 1 64\*64 block cube, and a cube transpose is performed based on the block cube. After the transpose, the result is split into 4 16\*64 fractals, which can have different arrangements according to the destination operand fractal interval and other parameters.

    The int4b\_t data type requires two numbers to be combined into one int8\_t or uint8\_t number. The combination rules are as follows:

    ![](../../../../figures/int4b_example.png)

  As shown in the figure below:
  - A total of 6144B of data needs to be processed. Each iteration processes 64\*64\*0.5B data, so 3 iterations can complete the processing, with repeat_time = 3.
  - src_stride = 1, indicating that between adjacent iterations, the interval between the start address of the previous block cube and the start address of the next block cube in the source operand is 1 (unit: 64\*64\*0.5B). The unit here is actually the size of the block cube after concatenation.
  - dst_gap = 1, indicating that between adjacent iterations, the interval from the end address of the first fractal of the previous iteration to the start address of the first fractal of the next iteration in the destination operand is 1 (unit: 512B).
  - dst_frac_gap = 0, indicating that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal of the destination operand is 0 (unit: 512B).
  
  ![](../../../../figures/asc_copy_l12l0_trans_b4_example1.png)

  As shown in the figure below:

  - The explanation of repeat_time and src_stride is the same as the example in the figure above.
  - dst_gap = 0, indicating that between adjacent iterations, there is no interval between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration of the destination operand.
  - dst_frac_gap = 2, indicating that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal of the destination operand is 2 (unit: 512B).

  ![](../../../../figures/asc_copy_l12l0_trans_b4_example2.png)

This API takes effect only when executed on the AIC.

## Prototype

```c
// Placeholder form
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ <dtype>* dst,
                                             __cbuf__ <dtype>* src,
                                             uint16_t index_id,
                                             uint8_t repeat,
                                             uint16_t src_stride,
                                             uint16_t dst_gap,
                                             uint16_t dst_frac_gap,
                                             uint16_t src_frac_gap)
```

### Data Types Supported by dtype

The data types supported by dtype are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e2m1_t`, `fp4x2_e1m2_t`, `hifloat8_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

### Prototype Example

```c
// Example: transposed copy of the half type.
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ half* dst,
                                             __cbuf__ half* src,
                                             uint16_t index_id,
                                             uint8_t repeat,
                                             uint16_t src_stride,
                                             uint16_t dst_gap,
                                             uint16_t dst_frac_gap,
                                             uint16_t src_frac_gap)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Destination operand, stored in the L0B Buffer. The destination address must be 512-byte aligned. |
| src | Input | Source operand, stored in the L1 Buffer. The source address must be 32-byte aligned. |
| index_id | Input | Starting fractal index on the source side, that is, the sequence number of the starting fractal cube in the source cube (counting from 0). Value range: [0, $2^{16}-1$]. |
| repeat | Input | Number of iterative copies. Value range: [1, 255].<br> For the `b4` data type, each iteration processes 4 fractals, and each fractal is 16\*64\*0.5B data;<br> For the `b8` data type, each iteration processes 2 fractals, and each fractal is 16\*32\*1B data;<br>For the `b16` data type, each iteration processes 1 fractal, and each fractal is 16\*16\*2B data; for the `b32` data type, each iteration processes 4 fractals, and each fractal is 16\*8\*4B data. |
| src_stride | Input | Interval between the start addresses of the previous fractal and the next fractal of the source operand between adjacent repeats, in units of 512 bytes. Value range: [0, $2^{16}-1$]. |
| dst_gap | Input | Interval between the end address of the previous fractal and the start address of the next fractal of the destination operand between adjacent repeats, in units of 512 bytes. The effective stride of the instruction is the passed value plus 1 (that is, passing 0 represents a stride of 1 fractal). Value range: [0, $2^{16}-1$]. |
| dst_frac_gap | Input | Interval between the end address of the previous fractal and the start address of the next fractal of the destination operand within the same repeat, in units of 512 bytes. The effective stride of the instruction is the passed value plus 1, and this parameter takes effect only for the b4/b8/b32 data types. Value range: [0, $2^{16}-1$]. |
| src_frac_gap | Input | Interval between the end address of the previous fractal and the start address of the next fractal of the source operand within the same repeat, in units of 512 bytes. The effective stride of the instruction is the passed value plus 1, and this parameter takes effect only for the b4/b8/b32 data types. Value range: [0, $2^{16}-1$]. |

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

- This API returns directly when it is not called by the AIC.
- The src start address must be 32-byte aligned (L1 Buffer alignment requirement); otherwise, an address alignment exception is triggered.
- The dst start address must be 512-byte aligned (L0B Buffer alignment requirement); otherwise, an address alignment exception is triggered.
- If the destination address of this instruction overlaps with that of other instructions, insert synchronization instructions ([asc_sync_notify](../../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../../sync/intra_core_sync/asc_sync_wait.md)) to serialize the instructions and prevent abnormal data.
- L1 Buffer capacity limit: The total L1 Buffer capacity is 512 KB. The sum of the src offset and the copy size must not exceed the boundary; otherwise, an address overflow exception is triggered.
- L0B Buffer capacity limit: The total L0B Buffer capacity is 64 KB. The sum of the dst offset and the copy size must not exceed the boundary; otherwise, an address overflow exception is triggered.
- When `repeat` is 0, no copy is performed, and this API is treated as a NOP (no operation).
- You must ensure that the transposed fractals of the destination operand do not overlap; otherwise, the copy result may not meet expectations.

## Examples

After saving the code as `examples.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [`__NPU_ARCH__`](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cstdint>
#include <cstdio>
#include "acl/acl.h"
#include "c_api/asc_simd.h"

constexpr uint32_t M = 128, N = 128, K = 128;
constexpr uint32_t BYTES = K * N * sizeof(half);

__global__ __cube__ void CopyL12l0bTransKernel(__gm__ half* input)
{
    asc_init();
    __cbuf__ half l1[K * N];
    __cb__ half l0b[K * N];
    asc_copy_gm2l1(l1, input, 1, BYTES / 32, asc_channel_pad_mode::NONE, 0, 0);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0b_trans(l0b, l1, static_cast<uint16_t>(0), static_cast<uint8_t>(N / 16),
        static_cast<uint16_t>(K / 16), static_cast<uint16_t>(0), static_cast<uint16_t>(0),
        static_cast<uint16_t>(0));
    asc_sync_pipe(PIPE_MTE1);
}

int main()
{
    aclInit(nullptr); aclrtSetDevice(0);
    half* input = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    CopyL12l0bTransKernel<<<1, 0>>>(input);
    const aclError ret = aclrtSynchronizeDevice();
    std::printf("M=%u N=%u K=%u asc_copy_l12l0b_trans=%s\n", M, N, K,
        ret == ACL_SUCCESS ? "PASS" : "FAIL");
    aclrtFree(input); aclrtResetDevice(0); aclFinalize();
    return ret == ACL_SUCCESS ? 0 : 1;
}
```
