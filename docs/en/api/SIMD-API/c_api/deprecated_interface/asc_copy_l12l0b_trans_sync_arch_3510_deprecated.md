# asc_copy_l12l0b_trans_sync_arch_3510 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:15:45.463Z pushedAt=2026-10-28T12:55:18.855Z nl=2 -->

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

The header file path is: `"c_api/composite/cube_datamove_composite.h"`.

**The `asc_copy_l12l0b_trans_sync` API is deprecated. Use the [corresponding asynchronous API](../cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md) and the synchronization API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

This API loads 2D-format data with transpose from the L1 Buffer to the L0B Buffer.

The following example explains the API functionality and key parameters: in the figures below, an N shape or a Z shape represents a fractal.

- For the uint8_t/int8_t data type, each iteration processes 32 × 32 × 1B of data and can handle 2 fractals (a fractal is 512B). In each iteration, 2 consecutive 16 × 32 fractals in the source operand are merged into one 32 × 32 block matrix, the matrix transpose is performed on the block-based matrix, and the transposed fractal is split into 2 16 × 32 fractals, which can have different layouts according to the destination operand fractal interval and other parameters.

As shown in the figure below:

- A total of 3072B of data is processed. Each iteration processes 32 × 32 × 1B of data, so 3 iterations can complete the operation, with repeat_time = 3;
- src_stride = 1, which indicates adjacent iterations, the interval between the start address of the previous block matrix and the next block matrix in the source operand is 1 (unit: 32 × 32 × 1B). The unit here is actually the matrix size after concatenation;
- dst_gap = 1, which indicates adjacent iterations, the interval between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration in the destination operand is 1 (unit: 512B);
- dst_frac_gap = 0, indicating that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal of the destination operand is 0 (unit: 512B).
![](../../../figures/asc_copy_l12l0_trans_b8_example1.png)

As shown in the example below:

- The explanation of repeat_time and src_stride is the same as in the example in the figure above.
- dst_gap = 0, indicating that between adjacent iterations, the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration of the destination operand have no interval.
- dst_frac_gap = 2, indicating that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal of the destination operand is 2 (unit: 512B).
![](../../../figures/asc_copy_l12l0_trans_b8_example2.png)

- For the half/bfloat16_t data type, each iteration processes 16 × 16 × 2B data and can process one fractal (a fractal is 512B). In each iteration, one 16 × 16 fractal in the source operand will be transposed.
  - Because each iteration processes 16 × 16 × 2B data, three iterations are required to complete, so repeat_time = 3.
  - src_stride = 1, indicating that between adjacent iterations, the interval between the start address of the previous block matrix and the start address of the next block matrix of the source operand is 1 (unit: 16 × 16 × 2B).
  - dst_gap = 0, indicating that between adjacent iterations, the destination operand's first fractal end address of the previous iteration has no interval to the first fractal start address of the next iteration;
  - In this scenario, because the fractal is the block matrix, each iteration processes one fractal, and there is no interval between fractals within an iteration, so this parameter is invalid.
![](../../../figures/asc_copy_l12l0_trans_b16_example.png)

- For the float/int32_t/uint32_t data types, each iteration processes 16 × 16 × 4B data and can process 2 fractals (a fractal is 512B). In each iteration, 2 consecutive 16 × 8 fractals of the source operand are merged into one 16 × 16 block matrix, transposed based on the block matrix, and then split into 2 16 × 8 fractals after transposition. The layout can vary according to parameters such as the destination operand fractal interval.
As shown in the figure below:
  - Because each iteration processes 16 × 16 × 4B data, 3 iterations are required to complete, so repeat_time = 3;
  - src_stride = 1, indicating that between adjacent iterations, the interval between the start address of the previous block matrix and the next block matrix of the source operand is 1 (unit: 16 × 16 × 4B). The unit here is actually the size of the concatenated block matrix;
  - dst_gap = 1, indicating that between adjacent iterations, the interval between the destination operand's first fractal end address of the previous iteration and the first fractal start address of the next iteration is 1 (unit: 512B);
  - dst_frac_gap = 0, indicating that within each iteration, the interval between the destination operand's previous fractal end address and the next fractal start address is 0 (unit: 512B).
![](../../../figures/asc_copy_l12l0_trans_b32_example1.png)

As shown in the following figure:
- The explanation of repeat_time and src_stride is the same as in the example in the figure above.
- dst_gap = 0, indicating that between adjacent iterations, the destination operand's first fractal end address of the previous iteration and the first fractal start address of the next iteration have no interval.
- dst_frac_gap = 2, indicating that within each iteration, the interval between the destination operand's previous fractal end address and the next fractal start address is 2 (unit: 512B).
![](../../../figures/asc_copy_l12l0_trans_b32_example2.png)

- For the int4b\_t data type, each iteration processes 64\*64\*0.5B data and can process 4 fractals (one fractal is 512B). In each iteration, 4 consecutive 16\*64 fractals in the source operand will be merged into 1 64\*64 block matrix. The matrix transpose is performed based on the block matrix, and after transposition it is split into 4 16\*64 fractals, which can have different arrangements according to the destination operand fractal interval and other parameters.

    The int4b\_t data type requires two numbers to be combined into one int8\_t or uint8\_t number. The combination rules are as follows:

    ![](../../../figures/int4b_example.png)

  As shown in the following figure:
  - A total of 6144B of data needs to be processed. Each iteration processes 64\*64\*0.5B data, and 3 iterations can complete the processing, so repeat_time = 3;
  - src_stride = 1, indicating that between adjacent iterations, the interval between the start address of the previous block matrix and the start address of the next block matrix of the source operand is 1 (unit: 64\*64\*0.5B). The unit here is actually the size of the concatenated block matrix.
  - dst_gap = 1, indicating that between adjacent iterations, the interval between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration of the destination operand is 1 (unit: 512B).
  - dst_frac_gap = 0, indicating that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal of the destination operand is 0 (unit: 512B).

  ![](../../../figures/asc_copy_l12l0_trans_b4_example1.png)

  As shown in the figure below:

  - The explanation of repeat_time and src_stride is the same as in the example in the figure above.
  - dst_gap = 0, indicating that between adjacent iterations, the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration of the destination operand have no interval.
  - dst_frac_gap = 2, indicating that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal of the destination operand is 2 (unit: 512B).

  ![](../../../figures/asc_copy_l12l0_trans_b4_example2.png)

## Prototype

```c
// Placeholder form.
__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ <dtype>* dst,
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

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
|:-----------------| :--- |:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| dst | Output | Destination L0B buffer address. |
| src | Input | Source L1 buffer address. |
| index_id | Input | Matrix ID, indicating the fractal index in the source operand from which the transfer starts. Value range: [0, 65535]. |
| repeat | Input | Number of iterations.<br> For data types with a bit width of 4, each iteration processes 4 fractals, and each fractal is 16\*64\*0.5B data.<br> For data types with a bit width of 8, each iteration processes 2 fractals, and each fractal is 16\*32\*1B data.<br> For data types with a bit width of 16, each iteration processes 1 fractal, and each fractal is 16\*16\*2B data. For data types with a bit width of 32, each iteration processes 4 fractals, and each fractal is 16\*8\*4B data. Value range: [1, 255]. |
| src_stride | Input | Interval between the start addresses of the previous and next fractals of the source operand in adjacent iterations. Unit: one fractal of 512B. Value range: [0, 65535]. |
| dst_gap | Input | Interval between the end address of the previous fractal and the start address of the next fractal of the destination operand in adjacent iterations. Unit: one fractal of 512B. Value range: [0, 65535]. |
| dst_frac_gap | Input | Interval between the end address of the previous fractal and the start address of the next fractal of the destination operand before transpose within each iteration. Unit: 512B. Valid only when the data bit width is 4, 8, or 32. |
| src_frac_gap | Input | Interval between the end address of the previous fractal and the start address of the next fractal of the source operand before transpose within each iteration. Unit: 512B. Valid only when the data bit width is 4, 8, or 32. |

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- repeat=0 indicates that no transfer is performed.
- You must ensure that the transposed fractals of the destination operand do not overlap.
- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).

## Examples

```cpp
// Set the source operand and destination operand. total_length indicates the data length involved in the computation.
constexpr uint64_t total_length = 512;
__cbuf__ int32_t src[total_length];
__cb__ int32_t dst[total_length];

// Set the configuration during the transfer.
uint8_t n = 64;
uint8_t nBlockSize = 32;
uint8_t repeat = n / nBlockSize;
uint16_t index_id = 0;
uint16_t src_stride = 1;
uint16_t dst_gap = 1;
uint16_t dst_frac_gap = 15;
uint16_t src_frac_gap = 8;

asc_copy_l12l0b_trans_sync(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
```
