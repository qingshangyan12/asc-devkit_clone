# asc_mmad_sparse (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:27:24.517Z pushedAt=2026-10-28T12:55:35.735Z nl=2 -->

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

**The `asc_mmad_sparse` API whose `unit_flag` parameter is of the `uint8_t` type and the `asc_mmad_sparse_sync` API are deprecated. Use the [asc_mmad_sparse](../cube_compute/asc_mmad_sparse.md) API whose `unit_flag_mode` parameter is of the `asc_unit_flag_mode` type instead. To implement the synchronization function of the original `asc_mmad_sparse_sync` API, use the new `asc_mmad_sparse` API together with `asc_sync()`. For details, see [asc_sync](../sync/intra_core_sync/asc_sync.md).**

This API performs a sparse matrix multiply-accumulate operation. The input left matrix A is a sparse matrix, and the right matrix B is a dense matrix. Matrix A is densified during computation, and matrix B must be densified during input data preparation.

This API is a matrix computation API and takes effect only on the AIC.

## Prototype

```c
__aicore__ inline void asc_mmad_sparse(__cc__ int32_t* c,
                                       __ca__ int8_t* a,
                                       __cb__ int8_t* b,
                                       uint16_t m,
                                       uint16_t k,
                                       uint16_t n,
                                       uint8_t unit_flag,
                                       bool c_matrix_source,
                                       bool c_matrix_init_val)

__aicore__ inline void asc_mmad_sparse_sync(__cc__ int32_t* c,
                                            __ca__ int8_t* a,
                                            __cb__ int8_t* b,
                                            uint16_t m,
                                            uint16_t k,
                                            uint16_t n,
                                            uint8_t unit_flag,
                                            bool c_matrix_source,
                                            bool c_matrix_init_val)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| c | Output | Start address of the result matrix C in the L0C buffer, which must be 1024-byte aligned. |
| a | Input | Start address of the left matrix A in the L0A buffer, which must be 512-byte aligned. |
| b | Input | Start address of the right matrix B in the L0B buffer, which must be 512-byte aligned. |
| m | Input | M dimension size of the left matrix A and the result matrix C. Value range: [0, 4095]. |
| k | Input | K dimension size of the left matrix A and the right matrix B. Value range: [0, 4095]. |
| n | Input | N dimension size of the right matrix B and the result matrix C. Value range: [0, 4095]. |
| unit_flag | Input | UnitFlag control parameter. 0: disabled. 2: enabled, and the unit flag is retained after execution. 3: enabled, and the unit flag is updated after execution. |
| c_matrix_source | Input | When `c_matrix_init_val` is false, configures whether the initial value of matrix C comes from the BiasTable buffer. |
| c_matrix_init_val | Input | Configures whether to set the initial value of matrix C to 0. |

## Return Value

None

## Pipeline Type

PIPE_M

## Constraints

- This API takes effect only on the AIC. Calling it on the AIV returns immediately.
- The original sparse matrix B contains at most 2 non-zero elements in every 4 elements. If there are 3 or more non-zero elements, only the first 2 non-zero elements are used.
- If any of `m`, `k`, and `n` is 0, the API is treated as a NOP (no operation).
- For sparse matrix data preparation, the dense algorithm, and other constraints, see [asc_mmad_sparse](../cube_compute/asc_mmad_sparse.md).
