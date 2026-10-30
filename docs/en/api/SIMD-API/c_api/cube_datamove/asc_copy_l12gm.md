# asc_copy_l12gm

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T13:04:09.569Z pushedAt=2026-10-28T12:45:45.440Z nl=2 -->

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

Copies data from the L1 Buffer to Global Memory.

## Prototype

- Regular copy

    ```cpp
    __aicore__ inline void asc_copy_l12gm(__gm__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    ```

## Parameters


**Table 1** Parameter description

| Parameter | Input/Output | Description                                                              |
| :--- |:------|:----------------------------------------------------------------|
| dst | Output    | Start address of the destination operand.|
| src | Input    | Start address of the source operand.|
| n_burst | Input    | Number of consecutive data blocks to copy. Value range: [1, 4095]. |
| len_burst | Input    | Length of each consecutive data block to be copied, in units of 32 bytes. Value range: [1, 65535]. |
| src_gap | Input    | Gap between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration of the source operand. |
| dst_gap | Input    | Gap between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration of the destination operand, in units of 32 bytes. |

## Return Value

None

## Pipeline Type

PIPE_MTE3

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- If multiple asc_copy_l12gm instructions need to be executed and their destination addresses overlap, insert synchronization instructions to serialize the multiple asc_copy_l12gm instructions and prevent abnormal data.

## Examples

```cpp
// The number of consecutive data blocks to be copied is 1.
constexpr uint16_t n_burst = 1;
// The length of each consecutive data block to be copied is 32 bytes.
constexpr uint16_t len_burst = 1;
// The gap between the start addresses of the previous fractal and the next fractal of the source operand is 32 bytes.
constexpr uint16_t src_gap = 0;
// Between adjacent iterations, the interval between the start addresses of the previous fractal and the next fractal of the destination operand is 64 bytes.
constexpr uint16_t dst_gap = 1;
__cbuf__ half src[256];
// dst indicates the start address of the destination operand.
asc_copy_l12gm(dst, src, n_burst, len_burst, src_gap, dst_gap);
```
