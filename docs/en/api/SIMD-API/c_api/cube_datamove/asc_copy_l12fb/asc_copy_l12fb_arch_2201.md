# asc_copy_l12fb

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T12:38:42.468Z pushedAt=2026-10-28T12:45:45.404Z nl=2 -->

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

Transfers data from the L1 Buffer to the Fixpipe Buffer. The Fixpipe Buffer is used to store quantization parameters.

## Prototype

- First-n-data transfer
    ```cpp
    __aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
    ```

- High-dimensional split transfer
    ```cpp
    __aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap_size, uint16_t dst_gap_size)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Destination operand start address. |
| src | Input | Source operand start address. |
| size | Input | Size of data to be transferred (in bytes). |
| n_burst | Input | Number of continuous data blocks to be transferred. Value range: [1, 4095]. |
| len_burst | Input | Length of each continuous data block to be transferred, unit is DataBlock (32 bytes). Value range: [1, 65535]. |
| src_gap_size | Input | Gap between adjacent continuous data blocks of the source operand (the gap between the end of the previous data block and the start of the next data block).<br>unit is DataBlock (32 bytes). |
| dst_gap_size | Input | Gap between adjacent continuous data blocks of the destination operand (the gap between the end of the previous data block and the start of the next data block).<br>unit is DataBlock (32 bytes). |

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../../general_description_and_constraints.md#storage-unit-description).
- For operand address overlap constraints, see [General Address Overlap Constraints](../../general_description_and_constraints.md#general-address-overlap-constraints).
- When the first-n-data transfer API is used, the transferred data size must be 32-byte aligned.

## Examples

```cpp
constexpr uint16_t n_burst = 1;
constexpr uint16_t len_burst = 1;
constexpr uint16_t src_gap_size = 0;
constexpr uint16_t dst_gap_size = 1;
__cbuf__ half src[256];
__fbuf__ half dst[256];
asc_copy_l12fb(dst, src, n_burst, len_burst, src_gap_size, dst_gap_size);
```
