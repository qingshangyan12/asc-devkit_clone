# asc_copy_l12l0b_sparse

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T13:10:29.608Z pushedAt=2026-10-28T12:45:45.446Z nl=2 -->

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

Copies a 512B dense weight cube stored in the L1 Buffer to the L0B Buffer, while reading a 128B index cube to sparsify the dense cube.

The index cube is arranged in reverse order within an int8_t address. For example, the index cube 1 2 0 1 0 2 1 0 is arranged in the address as 1 0 2 1 0 1 2 0, where 1 0 2 1 (corresponding to the first four elements 1 2 0 1 of the index matrix) is packed into one int8_t value, and 0 1 2 0 (corresponding to the last four elements 0 2 1 0 of the index matrix) is packed into another int8_t value.

## Prototype

- High-dimensional split copy
  ```cpp
  __aicore__ inline void asc_copy_l12l0b_sparse(__cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat)
  ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination operand in the L0B Buffer. |
| src | Input | Start address of the source operand in the L1 Buffer. |
| index | Input | Start address of the index cube in the L1 Buffer. |
| start_index | Input | Fractal cube ID, indicating that the copy starts from the specified fractal in the source operand (0 indicates the first fractal cube in the source operand). Value range: [0, 65535]. Unit: 512B. |
| repeat | Input | Iteration count. Each iteration processes 512B of data. Value range: [1, 255]. |

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).
- If repeat is 0, no copy is performed.
- start_index cannot be less than 0.
- Transposition is not supported.

## Examples

```cpp
__cb__ int8_t dst[256];
__cbuf__ int8_t src[256];
__cbuf__ int8_t index[64];
uint16_t start_index = 1;
uint8_t repeat = 8;
asc_copy_l12l0b_sparse(dst, src, index, start_index, repeat);
```
