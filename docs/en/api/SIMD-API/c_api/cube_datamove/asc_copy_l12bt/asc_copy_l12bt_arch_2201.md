# asc_copy_l12bt

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T12:34:47.623Z pushedAt=2026-10-28T12:45:45.397Z nl=2 -->

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

Copies data from the L1 Buffer to the BiasTable Buffer. The BiasTable Buffer stores the bias used in cube computation.

## Prototype

- Copy the first n data blocks
    ```cpp
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint32_t size)
    ```

- Copy with high-dimensional segmentation
    ```cpp
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination operand. |
| src | Input | Start address of the source operand. |
| size | Input | Size of the data to copy (in bytes). |
| conv_control | Input | Whether to enable in-place conversion. |
| n_burst | Input | Number of consecutive data blocks to copy. Value range: [1, 4095]. |
| len_burst | Input | Length of each consecutive data block to copy, unit: DataBlock (32 bytes). Value range: [1, 65535]. |
| source_gap | Input | Gap between adjacent consecutive data blocks of the source operand (the gap between the end of the preceding data block and the start of the following data block).<br>Unit: DataBlock (32 bytes). |
| dst_gap | Input | Gap between adjacent consecutive data blocks of the destination operand (the gap between the end of the preceding data block and the start of the following data block).<br>Unit: DataBlock (32 bytes). |

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../../general_description_and_constraints.md#storage-unit-description).
- For the operand address overlap constraints, see [General Address Overlap Constraints](../../general_description_and_constraints.md#general-address-overlap-constraints).
- When the first-n-data-block copy API is used, the size of the data to copy must be 32-byte aligned.

## Examples

```cpp
constexpr uint16_t n_burst = 1;
constexpr uint16_t len_burst = 1;
constexpr uint16_t source_gap = 0;
constexpr uint16_t dst_gap = 1;
constexpr uint16_t conv_control = 0;
__cbuf__ half src[256];
__biasbuf__ half dst[256];
uint64_t dst_addr = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst)) / 16;
asc_copy_l12bt(dst_addr, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
```
