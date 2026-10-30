# asc_mmad_sparse

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-28T13:32:57.201Z pushedAt=2026-10-29T02:01:15.743Z nl=2 -->

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

The header file path is `"c_api/cube_compute/cube_compute.h"`.

Performs a cube multiply-accumulate operation, where the input left cube A is a sparse cube and the right cube B is a dense cube. For cube A, densification is completed during the asc_mmad_sparse computation. For cube B, densification is completed by you during input data preparation before computation (densify it according to the dense algorithm described below). Dense cube B must be loaded through [asc_copy_l12l0b_sparse](../cube_datamove/asc_copy_l12l0b_sparse.md), together with the index cube. The index cube is generated during the densification of cube B and is then used for the densification of cube A.

## Prototype

```cpp
__aicore__ inline void asc_mmad_sparse(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool c_matrix_source, bool c_matrix_init_val)
```


## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| --------- | ----- | ---------------- |
| c       | Output    | 	Destination operand, result cube C. The start address must be 1024-byte aligned.            |
| a       | Input    |    Source operand, left cube A. The start address must be 512-byte aligned.|
| b       | Input    | 	Source operand, right cube B. The start address must be 512-byte aligned.      |
| m       | Input    | 	Height of the left cube. Value range: m∈[0,4095].      |
| k       | Input    | 	Width of the left cube and height of the right cube. Value range: k∈[0,4095].      |
| n       | Input    | 	Width of the right cube. Value range: n∈[0,4095].      |
| unit_flag_mode | Input | Controls fine-grained parallelism between the cube multiply-accumulate instruction and the cube move-out instruction. When UnitFlag is enabled, the hardware moves out the computation result each time it finishes computing a fractal. Value description:<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`: UnitFlag is not enabled.<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`: UnitFlag is enabled, and the hardware does not change the unit flag after executing the instruction.<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`: UnitFlag is enabled, and the hardware changes the unit flag after executing the instruction. |
| c_matrix_source       | Input    | 	Configures whether the initial value of cube C comes from the BiasTable Buffer (the hardware buffer that stores bias). Value description:  <br>&bull; true: Comes from the BiasTable Buffer. <br>&bull; false: Comes from the L0C Buffer.    |
| c_matrix_init_val       | Input    | 	Configures whether the initial value of cube C is 0. Value description:  <br>&bull; true: The initial value of cube C is 0. <br>&bull; false: The initial value of cube C is configured through the c_matrix_source parameter.     |


## Return Value

None

## Pipeline Type

PIPE_M

## Constraints

- For the original sparse cube B, at most 2 non-zero elements are allowed among every 4 elements. If there are 3 or more non-zero elements, only the first 2 non-zero elements are used.
- If any of m, k, or n is 0, the instruction is not executed.

## Dense Algorithm

Assume that at least two of every four elements in the original sparse cube B are zero. The densified cube B is a dense cube with two zeros filtered out from every four elements. During the densification of cube B, an index cube is generated as follows: for every four elements in sparse cube B, two 2-bit indexes are generated in the index cube and encoded according to the following rules. The indexes must be within the range {0, 1, 2}.
<br>&bull;The first index indicates the relative position of the first non-zero element among the first three elements.
<br>&bull;The second index indicates the relative position of the second non-zero element among the last three elements.
<br>See the following table for details. Here, "-" indicates that the algorithm does not care about the value at that position because it is filtered out.
**Table 2** Dense algorithm

<table border="1" style="border-collapse: collapse; width: 100%; text-align: center;">
  <!-- Row 1: Not merged -->
  <tr>
    <td style="padding: 8px; border: 1px solid #333;">Example</td>
    <td style="padding: 8px; border: 1px solid #333;">ele0</td>
    <td style="padding: 8px; border: 1px solid #333;">ele1</td>
    <td style="padding: 8px; border: 1px solid #333;">ele2</td>
    <td style="padding: 8px; border: 1px solid #333;">ele3</td>
    <td style="padding: 8px; border: 1px solid #333;">Index_a[i]</td>
    <td style="padding: 8px; border: 1px solid #333;">Index_b[i]</td>
  </tr>

  <!-- Rows 2-7: First column merged (6 rows in total) -->
  <tr>
    <td rowspan="6" style="padding: 8px; border: 1px solid #333; vertical-align: middle;">Two non-zero elements</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">X</td>
    <td style="padding: 8px; border: 1px solid #333;">Y</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b10</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b10</td>
  </tr>
  <tr>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">X</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">Y</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b01</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b10</td>
  </tr>
  <tr>
    <td style="padding: 8px; border: 1px solid #333;">X</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">Y</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b00</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b10</td>
  </tr>
  <tr>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">X</td>
    <td style="padding: 8px; border: 1px solid #333;">Y</td>
    <td style="padding: 8px; border: 1px solid #333;">-</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b01</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b01</td>
  </tr>
  <tr>
    <td style="padding: 8px; border: 1px solid #333;">X</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">Y</td>
    <td style="padding: 8px; border: 1px solid #333;">-</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b00</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b01</td>
  </tr>
  <tr>
    <td style="padding: 8px; border: 1px solid #333;">X</td>
    <td style="padding: 8px; border: 1px solid #333;">Y</td>
    <td style="padding: 8px; border: 1px solid #333;">-</td>
    <td style="padding: 8px; border: 1px solid #333;">-</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b00</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b00</td>
  </tr>

  <!-- Rows 8-11: First column merged (4 rows in total) -->
  <tr>
    <td rowspan="4" style="padding: 8px; border: 1px solid #333; vertical-align: middle;">One non-zero element</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">X</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b00</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b10</td>
  </tr>
  <tr>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">X</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b10</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b00</td>
  </tr>
  <tr>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">X</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b01</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b00</td>
  </tr>
  <tr>
    <td style="padding: 8px; border: 1px solid #333;">X</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b00</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b00</td>
  </tr>

  <!-- Row 12: Not merged -->
  <tr>
    <td style="padding: 8px; border: 1px solid #333;">All zero</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">0</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b00</td>
    <td style="padding: 8px; border: 1px solid #333;">2'b00</td>
  </tr>
</table>
This index cube is used for the densification of cube A. Based on the index cube, two elements are selected from every four elements in MatrixA to participate in the computation, as shown in the following figure:

![Dense algorithm illustration](../../../figures/asc_mmad_sparse_dense_algorithm.png)

## Examples

```cpp
constexpr uint32_t total_length = 128;
__ca__ int8_t a[total_length];
__cb__ int8_t b[total_length];
__cc__ int32_t c[total_length];
uint16_t m = 2;
uint16_t k = 2;
uint16_t n = 1;
asc_unit_flag_mode unit_flag_mode = asc_unit_flag_mode::ENABLE_KEEP;
bool c_matrix_source = true;
bool c_matrix_init_val = true;
asc_mmad_sparse(c, a, b, m, k, n, unit_flag_mode, c_matrix_source, c_matrix_init_val);
```
