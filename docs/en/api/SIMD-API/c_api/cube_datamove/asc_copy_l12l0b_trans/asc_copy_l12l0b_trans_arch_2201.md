# asc_copy_l12l0b_trans

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-28T13:35:43.370Z pushedAt=2026-10-29T02:01:15.754Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Not supported
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

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

This API loads 2D data with transpose from the L1 Buffer to the L0B Buffer.

The following examples explain the API functionality and key parameters: in the figures below, an N shape or a Z shape represents a fractal.

- For the uint8_t/int8_t data type, each iteration processes 32 × 32 × 1B data and can handle 2 fractals (a fractal is 512B). In each iteration, 2 consecutive 16 × 32 fractals in the source operand are merged into a 32 × 32 block cube, transposed based on the block cube, and then split into 2 16 × 32 fractals after transposition. Different layouts are possible depending on parameters such as the destination operand fractal gap.

As shown below:

- A total of 3072B data is processed. Each iteration processes 32 × 32 × 1B data, and 3 iterations can complete the processing, with repeat_time = 3.
- src_stride = 1, which represents that between adjacent iterations, the interval between the start address of the previous block cube and the next block cube in the source operand is 1 (unit: 32 × 32 × 1B). The unit here is actually the size of the merged block cube.
- dst_gap = 1, which represents that between adjacent iterations, the interval from the end address of the first fractal in the previous iteration to the start address of the first fractal in the next iteration in the destination operand is 1 (unit: 512B).
- dst_frac_gap = 0, which represents that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal in the destination operand is 0 (unit: 512B).

![](../../../../figures/asc_copy_l12l0_trans_b8_example1.png)

As shown in the example below:

- The explanation of repeat_time and src_stride is the same as in the example above.
- dst_gap = 0, which means that between adjacent iterations, there is no gap between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration in the destination operand.
- dst_frac_gap = 2, which means that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal in the destination operand is 2 (unit: 512B).

![](../../../../figures/asc_copy_l12l0_trans_b8_example2.png)

- For the half/bfloat16_t data type, each iteration processes 16 × 16 × 2B data and can handle one fractal (a fractal is 512B). In each iteration, one 16 × 16 fractal in the source operand will be transposed.
  - Because each iteration processes 16 × 16 × 2B data, 3 iterations are needed to complete the operation, so repeat_time = 3;
  - src_stride = 1, which means that between adjacent iterations, the interval between the start address of the previous block cube and the start address of the next block cube in the source operand is 1 (unit: 16 × 16 × 2B);
  - dst_gap = 0, which means that between adjacent iterations, there is no gap between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration in the destination operand;
  - In this scenario, because the fractal is the block cube itself and each iteration processes one fractal, there is no interval between fractals within an iteration, so this parameter is ineffective.

![](../../../../figures/asc_copy_l12l0_trans_b16_example.png)

- For the float/int32_t/uint32_t data type, each iteration processes 16 × 16 × 4B data and can handle 2 fractals (a fractal is 512B). In each iteration, 2 consecutive 16 × 8 fractals of the source operand are merged into a 16 × 16 block cube, transposed based on the block cube, and then split into 2 16 × 8 fractals after transposition. Different layouts are possible depending on parameters such as the destination operand fractal gap.
As shown in the following example:
  - Because each iteration processes 16 × 16 × 4B data, 3 iterations can complete the operation, so repeat_time = 3;
  - src_stride = 1, which represents that between adjacent iterations, the interval between the start address of the previous block cube and the start address of the next block cube of the source operand is 1 (unit: 16 × 16 × 4B). Here the unit is actually the size of the merged block cube;
  - dst_gap = 1, which represents that between adjacent iterations, the interval from the end address of the first fractal of the previous iteration to the start address of the first fractal of the next iteration of the destination operand is 1 (unit: 512B);
  - dst_frac_gap = 0, which represents that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal of the destination operand is 0 (unit: 512B).

![](../../../../figures/asc_copy_l12l0_trans_b32_example1.png)

As shown in the following example:
- The explanation of repeat_time and src_stride is the same as in the example above.
- dst_gap = 0, indicating that between adjacent iterations, there is no gap between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration in the destination operand.
- dst_frac_gap = 2, indicating that within each iteration, the interval between the end address of the previous fractal and the start address of the next fractal in the destination operand is 2 (unit: 512B).

![](../../../../figures/asc_copy_l12l0_trans_b32_example2.png)

## Prototype

- High-dimensional split transfer
```cpp
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --------- | ----- | ---------------- |
| dst       | Output    | Destination operand. |
| src | Input    | Source operand. |
| index_id | Input | ID of the block cube, indicating the starting transfer position as the ordinal number of the block cube in the source operand (0 indicates the first block cube in the source operand). Value range: index_id∈[0, 65535]. |
| repeat | Input | Number of iteration repetitions. repeat∈[0, 255].<br> For the uint8_t/int8_t data type, each iteration processes 32 × 32 × 1B of data.<br> For the half/bfloat16_t data type, each iteration processes 16 × 16 × 2B of data.<br> For the float/int32_t/uint32_t data type, each iteration processes 16 × 16 × 4B of data. |
| src_stride | Input | Represents the interval between the start addresses of the previous block cube and the next block cube in the source operand between adjacent iterations, unit: (1024B). Value range: src_stride∈[0, 65535].<br> For the uint8_t/int8_t data type, the unit is 32 × 32 × 1B.<br> For the half/bfloat16_t data type, the unit is 16 × 16 × 2B.<br> For the float/int32_t/uint32_t data type, the unit is 16 × 16 × 4B.<br> For the int4b_t data type, each iteration processes 16 × 64 × 0.5B of data. |
| dst_gap | Input | Represents the interval between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration in the destination operand between adjacent iterations. Value range: dst_gap∈[0, 65535]. |
| enable_addr_decrement | Input | Controls the address update mode.<br>false: Increment. Each iteration adds src_stride to the previous address.<br>true: Decrement. Each iteration subtracts src_stride from the previous address. |
| dst_frac_gap | Input | Interval between the end address of the previous fractal and the start address of the next fractal after transposition in the destination operand within each iteration, in units of 512B. This parameter is valid only when the data type is float/int32_t/uint32_t/uint8_t/int8_t. |



## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../../general_description_and_constraints.md#storage-unit-description).
- repeat=0 indicates that no transfer operation is performed.
- You need to ensure that the transposed fractals in the destination operand do not overlap.

## Examples

```cpp
// Set the source operand and destination operand. total_length indicates the length of data involved in the computation.
constexpr uint64_t total_length = 512;
__cbuf__ int32_t src[total_length];
__cb__ int32_t dst[total_length];

// Set the configuration used during the transfer.
uint8_t n = 64;
uint8_t nBlockSize = 32;
uint8_t repeat = n / nBlockSize;
uint16_t index_id = 0;
uint16_t src_stride = 1;
uint16_t dst_gap = 1;
bool enable_addr_decrement = false;
uint16_t dst_frac_gap = 0;

// Data transfer process.
asc_copy_l12l0b_trans(dst, src, index_id, repeat, src_stride, dst_gap, enable_addr_decrement, dst_frac_gap);
```
