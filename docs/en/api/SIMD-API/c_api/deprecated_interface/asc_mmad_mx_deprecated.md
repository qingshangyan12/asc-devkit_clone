# asc_mmad_mx (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:26:52.545Z pushedAt=2026-10-28T12:55:35.729Z nl=2 -->

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

The header file path is `"c_api/cube_compute/cube_compute.h"`.

**The `asc_mmad_mx` API and the `asc_mmad_mx_sync` API whose input parameter `unit_flag` is of the `uint8_t` type are deprecated. Use the [asc_mmad_mx](../cube_compute/asc_mmad_mx.md) API whose input parameter `unit_flag_mode` is of the `asc_unit_flag_mode` type instead. To implement the synchronization function of the original `asc_mmad_mx_sync` API, use the new `asc_mmad_mx` API together with `asc_sync()`. For details, see [asc_sync](../sync/intra_core_sync/asc_sync.md).**

This API performs matrix multiply-accumulate computation with quantization coefficients (MmadMx). Both the left matrix A and the right matrix B have corresponding quantization coefficient matrices: the left quantization coefficient matrix ScaleA and the right quantization coefficient matrix ScaleB. In the MmadMx scenario, the left quantization coefficient matrix is multiplied by the left matrix, the right quantization coefficient matrix is multiplied by the right matrix, and then matrix multiplication is performed on the two products. The calculation formula is as follows:

$$
C_{M \times N} = \left(ScaleA_{M \times (K/32)} \otimes A_{M \times K}\right) \times \left(ScaleB_{(K/32) \times N} \otimes B_{K \times N}\right) + C_{M \times N}
$$

Here, $\otimes$ denotes broadcast multiplication. Every 32 consecutive elements of matrices A and B along the K dimension share one quantization coefficient from the corresponding quantization coefficient matrices ScaleA and ScaleB. By configuring the parameters of this API, the C matrix can be initialized to an all-zero matrix, a matrix in the L0C Buffer, or a Bias matrix.

This API does not take the Scale matrix addresses as separate inputs. Instead, it reads the corresponding Scale matrices from the L0A_MX Buffer and L0B_MX Buffer based on the start addresses of matrices A and B and a fixed address mapping relationship. The information about each matrix is described in the following table:

**Table 1** Matrix information

| Cube | Storage Location | Shape (Rows × Columns) | Data Format | Fractal Size (Rows × Columns) |
| --- | --- | --- | --- | --- |
| A | L0A Buffer | M×K | Nz | 16×K0 |
| ScaleA | L0A_MX Buffer | M×(K / 32) | Zz | 16×2 |
| B | L0B Buffer | K×N | Zn | K0×16 |
| ScaleB | L0B_MX Buffer | (K / 32)×N | Nn | 2×16 |
| C | L0C Buffer | M×N | Nz | 16×16 |
| Bias (used to initialize the C matrix) | BiasTable Buffer | 1×N. When used, M rows are replicated through broadcasting to initialize the C matrix. | ND | - |

When the matrix data type is `fp8_e5m2_t` or `fp8_e4m3fn_t`, `K0 = 32`. When the matrix data type is `fp4x2_e2m1_t` or `fp4x2_e1m2_t`, `K0 = 64`.

This API is a matrix computation API and takes effect only on the AIC.

## Prototype

**Placeholder form:**

```c
__aicore__ inline void asc_mmad_mx(__cc__ float* c_matrix,
                                   __ca__ <a_dtype>* a_matrix,
                                   __cb__ <b_dtype>* b_matrix,
                                   uint16_t left_height,
                                   uint16_t n_dim,
                                   uint16_t right_width,
                                   uint8_t unit_flag,
                                   bool disable_gemv,
                                   bool c_matrix_source,
                                   bool c_matrix_init_val)

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c_matrix,
                                        __ca__ <a_dtype>* a_matrix,
                                        __cb__ <b_dtype>* b_matrix,
                                        uint16_t left_height,
                                        uint16_t n_dim,
                                        uint16_t right_width,
                                        uint8_t unit_flag,
                                        bool disable_gemv,
                                        bool c_matrix_source,
                                        bool c_matrix_init_val)
```

**Data types supported by dtype:**

The value combinations of `a_dtype` and `b_dtype` are listed in the following table:

**Table 2** Supported data type combinations<a id="asc_mmad_mx_data_type"></a>

| a_dtype | b_dtype | ScaleA/ScaleB data type | Result matrix C data type | Bias data type |
| --- | --- | --- | --- | --- |
| `fp4x2_e1m2_t` | `fp4x2_e1m2_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp4x2_e1m2_t` | `fp4x2_e2m1_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp4x2_e2m1_t` | `fp4x2_e1m2_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp4x2_e2m1_t` | `fp4x2_e2m1_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e4m3fn_t` | `fp8_e4m3fn_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e4m3fn_t` | `fp8_e5m2_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e5m2_t` | `fp8_e4m3fn_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e5m2_t` | `fp8_e5m2_t` | `fp8_e8m0_t` | `float` | `float` |

**Typical example:**

```c
__aicore__ inline void asc_mmad_mx(__cc__ float* c_matrix,
                                   __ca__ fp4x2_e1m2_t* a_matrix,
                                   __cb__ fp4x2_e2m1_t* b_matrix,
                                   uint16_t left_height,
                                   uint16_t n_dim,
                                   uint16_t right_width,
                                   uint8_t unit_flag,
                                   bool disable_gemv,
                                   bool c_matrix_source,
                                   bool c_matrix_init_val)
```

## Parameters

**Table 3** Parameters<a id="asc_mmad_mx_param_table"></a>

| Parameter | Input/Output | Description |
| --- | --- | --- |
| c_matrix | Output | Destination operand, the start address of result matrix C in the L0C Buffer. The data type is `float`, and 1024-byte alignment is required. |
| a_matrix | Input | Source operand, the start address of left matrix A in the L0A Buffer. The data type is determined by the API overload. For details, see [Table 2](#asc_mmad_mx_data_type).<br>When the matrix data type is `fp4x2_e2m1_t` or `fp4x2_e1m2_t`, 512-byte alignment is required. When the matrix data type is `fp8_e5m2_t` or `fp8_e4m3fn_t`, 1024-byte alignment is required.<br>The start address of the ScaleA matrix in the L0A_MX Buffer is the start address of `a_matrix` divided by 16. |
| b_matrix | Input | Source operand, the start address of right matrix B in the L0B Buffer. The data type is determined by the API overload. For details, see [Table 2](#asc_mmad_mx_data_type).<br>When the matrix data type is `fp4x2_e2m1_t` or `fp4x2_e1m2_t`, 512-byte alignment is required. When the matrix data type is `fp8_e5m2_t` or `fp8_e4m3fn_t`, 1024-byte alignment is required.<br>The start address of the ScaleB matrix in the L0B_MX Buffer is the start address of `b_matrix` divided by 16. |
| left_height | Input | M dimension size of left matrix A and result matrix C, in elements. The value range is [0, 4095]. |
| n_dim | Input | K dimension size of left matrix A and right matrix B, in elements. The value range is [0, 4095], and it must be a multiple of 64. |
| right_width | Input | N dimension size of right matrix B and result matrix C, in elements. The value range is [0, 4095]. |
| unit_flag | Input | Controls fine-grained parallelism between the matrix multiply-accumulate instruction and the matrix move-out instruction. When UnitFlag is enabled, the hardware moves out the computation result each time it finishes computing a fractal. The values are described as follows:<br>&nbsp;&nbsp;&bull; 0: UnitFlag is disabled.<br>&nbsp;&nbsp;&bull; 2: UnitFlag is enabled, and the hardware does not change the unit flag after executing the instruction.<br>&nbsp;&nbsp;&bull; 3: UnitFlag is enabled, and the hardware changes the unit flag after executing the instruction.<br>The matrix multiply-accumulate instruction and the corresponding matrix move-out instruction must both enable or both disable UnitFlag. When UnitFlag is enabled, no synchronization instruction needs to be inserted between the instructions. |
| disable_gemv | Input | When M is 1, configures whether to disable GEMV mode.<br>&nbsp;&nbsp;&bull; false: GEMV mode is enabled.<br>&nbsp;&nbsp;&bull; true: GEMV mode is disabled.<br>When M is not 1, this parameter does not take effect. |
| c_matrix_source | Input | When `c_matrix_init_val` is false, configures the source of the initial value of matrix C.<br>&nbsp;&nbsp;&bull; false: The initial value of matrix C comes from the L0C Buffer.<br>&nbsp;&nbsp;&bull; true: The initial value of matrix C comes from the BiasTable Buffer. |
| c_matrix_init_val | Input | Configures whether to set the initial value of matrix C to 0.<br>&nbsp;&nbsp;&bull; true: The initial value of matrix C is set to 0, and `c_matrix_source` does not take effect.<br>&nbsp;&nbsp;&bull; false: No zeroing operation is performed, and the initial value of matrix C is configured by `c_matrix_source`. |

## Return Value

None

## Pipeline Type

PIPE_M

## Constraints

- This API takes effect only on the AIC. Calling it on the AIV returns immediately.
- If any of `left_height`, `n_dim`, or `right_width` is 0, the API is treated as a NOP (no operation).
- Before calling this API, use APIs such as [asc_copy_l12l0a_mx](../cube_datamove/asc_copy_l12l0a_mx.md) and [asc_copy_l12l0b_mx](../cube_datamove/asc_copy_l12l0b_mx.md) to move the ScaleA and ScaleB matrices into the L0A_MX Buffer and L0B_MX Buffer, respectively. The data type of the ScaleA and ScaleB matrices must be `fp8_e8m0_t`, and each Scale element in the K direction corresponds to 32 input matrix elements.

- Memory usage constraints:

  - The L0C Buffer is 256 KB, the L0A Buffer and L0B Buffer are each 64 KB, the L0A_MX Buffer and L0B_MX Buffer are each 4 KB, and the BiasTable Buffer is 4 KB. The start address and occupied space of a matrix must not exceed the range of the corresponding buffer.
  - The start address of each matrix must meet the alignment requirements in [Table 3](#asc_mmad_mx_param_table). For other address constraints on operands, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
  - The start address of the ScaleA matrix in the L0A_MX Buffer is the start address of `a_matrix` divided by 16, and the start address of the ScaleB matrix in the L0B_MX Buffer is the start address of `b_matrix` divided by 16. When moving in the Scale matrices, use destination addresses that match matrix A and matrix B.
  - `n_dim` must be a multiple of 64. If the original K is not a multiple of 64, pad K upward to a multiple of 64 and set the matrix A and matrix B data in the padded region to 0. The `n_dim` passed to the API, the matrix storage space, and the K dimension size of the Scale matrices must all be calculated based on the padded K.
  - The M and N dimensions of the matrix storage space must each be padded upward to a multiple of 16, while `left_height` and `right_width` still pass the valid M and N values of the matrix. The padded portion is invalid data and does not participate in the computation of the valid region of the result matrix.
  - The Scale matrices must cover all data required for this matrix multiply-accumulate operation. The M dimension of ScaleA must be padded upward to a multiple of 16, and its K dimension size is K / 32. The K dimension size of ScaleB is K / 32, and its N dimension must be padded upward to a multiple of 16. The corresponding start addresses and occupied space must not exceed the range of the MX Buffer.
  - When M is 1 and `disable_gemv` is false, GEMV mode is enabled by default. In this case, matrix A is read from the L0A Buffer in ND format, so matrix A must be laid out in ND format, and its start address must still meet the alignment requirements in [Table 3](#asc_mmad_mx_param_table).

- Synchronization constraints:

  For the scenario where the input matrix is computed in blocks along the K axis and the results are accumulated into the same L0C Buffer, when `(left_height / 16) * (right_width / 16) < 10`, call [asc_sync_pipe](../sync/intra_core_sync/asc_sync_pipe.md) between two adjacent matrix multiply-accumulate instructions and set the `pipe` parameter to `PIPE_M`.

- UnitFlag constraints:

  - When UnitFlag is enabled, both the matrix multiply-accumulate instruction and the corresponding matrix move-out instruction must enable UnitFlag. To keep the same L0C Buffer memory space continuously occupied by multiple matrix multiply-accumulate instructions or multiple matrix move-out instructions, set the unitFlag value of the first n-1 instructions to 2 to maintain the occupied state of the operated memory space, and set the last instruction to 3 to release the occupied state.
  - When UnitFlag is enabled, the matrix computation direction must be consistent with the matrix move-out read order. When the matrix move-out instruction enables the Nz2ND in-place format conversion, or when no in-place format conversion is performed but B8/B4 quantization is enabled and the Channel Merge function is triggered, call [asc_set_mmad_direction_n](../cube_compute/asc_set_mmad_direction_n.md); in other scenarios, call [asc_set_mmad_direction_m](../cube_compute/asc_set_mmad_direction_m.md).
  - When UnitFlag is enabled, it is recommended that the computation data volume of the matrix multiply-accumulate be consistent with the data volume of the matrix move-out. Inconsistency between the two may cause execution exceptions. To clear the residual state generated by UnitFlag, call [asc_set_l0c2gm_config (deprecated)](asc_set_l0c2gm_config_deprecated.md) and set `enable_unit_flag` to true to set the unit flag of all memory blocks in the L0C Buffer to 0 and disable UnitFlag.

- Special value/boundary value constraints:

  When the floating-point input or output contains inf/nan, you can configure the CTRL\[48\] bit of the CTRL register through the [asc_set_ctrl](../spr/asc_set_ctrl.md) API to control the computation mode:

  - When set to 0, saturation mode is used, where inf output is saturated to ±MAX and nan output is saturated to 0.
  - When set to 1, non-saturation mode is used, where inf/nan keeps the original output.

  Note that NaN inputs should be avoided, as they may cause execution errors.
