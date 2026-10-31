# asc_copy_gm2l1_nd2nz

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T12:24:17.208Z pushedAt=2026-10-28T12:45:45.385Z nl=2 -->

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

Moves data from Global Memory (GM) to Level 1 cache (L1), supporting conversion from ND format to NZ format during data movement.

## Prototype

- Regular movement

    ```cpp
    __aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t nd_num,
        uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
        uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride)
    __aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t nd_num,
        uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
        uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride)
    __aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t nd_num,
        uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
        uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride)
    __aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ half* dst, __gm__ half* src, uint16_t nd_num,
        uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
        uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride)
    __aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ int16_t* dst, __gm__ int16_t* src, uint16_t nd_num,
        uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
        uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride)
    __aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ float* dst, __gm__ float* src, uint16_t nd_num,
        uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
        uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride)
    __aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t nd_num,
        uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
        uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride)
    __aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t nd_num,
        uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
        uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride)
    ```

## Parameters


**Table 1** Parameter description

| Parameter            | Input/Output | Description                                                                                                                                                                                                                                                          |
|:---------------------|:-------------|:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| dst                  | Output       | Start address of the destination operand (vector).                                                                                                                                                                                                                   |
| src                  | Input        | Start address of the source operand.                                                                                                                                                                                                                                  |
| nd_num               | Input        | Number of ND cubes to transfer. Value range: nd_num∈[0, 4095].                                                                                                                                                                                                     |
| n_value              | Input        | Number of rows in the ND cube. Value range: n_value∈[0, 16384].                                                                                                                                                                                                     |
| d_value              | Input        | Number of columns in the ND cube. Value range: d_value∈[0, 65535].                                                                                                                                                                                                  |
| src_nd_matrix_stride | Input        | Offset between the start addresses of adjacent ND cubes of the source operand. Value range: src_nd_matrix_stride∈[0, 65535], in elements.                                                                                                                           |
| src_d_value          | Input        | Offset between the start addresses of adjacent rows in the same ND cube of the source operand. Value range: src_d_value∈[1, 65535], in elements.                                                                                                                    |
| dst_nz_c0_stride     | Input        | After ND-to-NZ conversion, one row in the source operand is converted into multiple rows in the destination operand. dst_nz_c0_stride indicates the offset between adjacent start addresses of multiple rows from the same row of the source operand in the destination NZ cube. Value range: dst_nz_c0_stride∈[1, 16384], unit: C0_SIZE (32B). |
| dst_nz_n_stride      | Input        | Offset between the start addresses of adjacent rows of the Z-shaped cube in the destination NZ cube. Value range: dst_nz_n_stride∈[1, 16384], unit: C0_SIZE (32B).                                                                                                |
| dst_nz_matrix_stride | Input        | Offset between the start addresses of adjacent NZ cubes in the destination NZ cube. Value range: dst_nz_matrix_stride∈[1, 65535], in elements.                                                                                                                    |

## Return Value

None

## Pipeline Type

PIPE_MTE2

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../../general_description_and_constraints.md#storage-unit-description).
- If multiple asc_copy_gm2l1_nd2nz instructions need to be executed and their destination addresses overlap, insert synchronization instructions to serialize the multiple asc_copy_gm2l1_nd2nz instructions and prevent abnormal data.

## Examples

```cpp
// total_length indicates the total length of the data involved in the movement.
constexpr uint64_t total_length = 128;
__cbuf__ int8_t dst[total_length];
// Number of nd cubes to transfer.
constexpr uint16_t nd_num = 2;
// Number of rows in the nd cube.
constexpr uint16_t n_value = 4;
// Number of columns in the nd cube.
constexpr uint16_t d_value = 24;
// Start offset between adjacent nd cubes.
constexpr uint16_t src_nd_matrix_stride = 144;
// Number of elements in one row.
constexpr uint16_t src_d_value = 48;
// Offset between the start addresses of multiple rows after the format is converted from nd to nz.
constexpr uint16_t dst_nz_c0_stride = 11;
// Offset in dst between row x and row x+1 of an nd cube in src after conversion to nz format.
constexpr uint16_t dst_nz_n_stride = 2;
// Offset between the start of the x-th nd cube and the start of the (x+1)-th nd cube in dst.
constexpr uint16_t dst_nz_matrix_stride = 96;
// src indicates the start address of the source operand.
asc_copy_gm2l1_nd2nz(dst, src, nd_num, n_value, d_value, src_nd_matrix_stride, src_d_value, dst_nz_c0_stride, dst_nz_n_stride, dst_nz_matrix_stride);
```
