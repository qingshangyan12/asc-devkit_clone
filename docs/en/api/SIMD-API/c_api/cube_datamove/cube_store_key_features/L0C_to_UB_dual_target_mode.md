# L0C Buffer to UB Dual-Target Mode

<!-- md-trans-meta sourceCommit=5df1d1c781b51fcb7826db7f2afab5f8bc453e93 translatedAt=2026-10-22T12:56:38.427Z pushedAt=2026-10-28T12:45:45.429Z nl=2 -->

## Description

This feature applies only to the 3510. Within the same AI Core, there are one Cube Core and two Vector Cores. When dual-target mode control is enabled, the M×N cube in the L0C Buffer is split into two halves and written simultaneously to the Unified Buffer (UB) of each of the two Vector Cores, with the first half written to SUB BLOCK0 and the second half written to SUB BLOCK1.

- Dual-target mode (M dimension): The cube is split along the M dimension into two cubes of shape M / 2 \* N, which are written to the two UBs respectively.
- Dual-target mode (N dimension): The cube is split along the N dimension into two cubes of shape M \* N / 2, which are written to the two UBs respectively.

## Constraints

- The dual-target mode is supported only on the L0C Buffer-to-UB path, in the normal copy mode (Nz2Nz) or the Nz2ND copy scenario. It is not supported in in-line function scenarios.
- When splitting by the M dimension, M must be a multiple of 2.
- When splitting by the N dimension, N must be a multiple of 32.

## Exampless

The following figure shows how to enable the dual-target mode in normal copy mode. It is divided into splitting by the M dimension and splitting by the N dimension. When splitting by the M dimension, M must be a multiple of 2; when splitting by the N dimension, N must be a multiple of 32:

Splitting in the N direction:

- n_size = 32, indicating that the size of the copy cube in the source Nz cube in the N direction is 32 elements.
- m_size = 48, indicating that the size of the copy cube in the source Nz cube in the M direction is 48 elements.
- src_stride = 64, indicating the start address offset of adjacent Z layouts of the copy cube in the source Nz cube, that is, the interval between the start address of the first block Z layout cube and the start address of the second Z layout cube in the following figure is 64 \* C0\_Size.
- dst_stride = 64 \* C0, indicating the start address offset of adjacent Z layouts in the destination Nz cube. This stride is applied within each UB separately, not the distance between the address spaces of two UBs.

Splitting in the M direction:

- n_size = 32, indicating that the size of the copy cube in the source Nz cube in the N direction is 32 elements.
- m_size = 48, indicating that the complete source cube M is 48. After splitting, each UB receives 24 rows. Do not use the number of rows after splitting as the source m_size.
- src_stride = 64, indicating the start address offset between adjacent Z layouts in the source Nz cube, that is, the interval between the start address of the first Z layout cube and the start address of the second Z layout cube in the following figure is 64 \* C0\_Size.
- dst_stride = 40 \* C0, indicating the start address offset between adjacent Z layouts in the destination Nz cube, that is, the interval between the start address of the first Z layout and the start address of the second Z layout in UB0 (or UB1) in the following figure is 40 \* 16 elements.

**Figure 1** Schematic diagram of the Nz2Nz dual-target copy mode settings

![](../../../../figures/nz2nz_l0c2ub_dual_dst_c_api.png)

Enabling the dual-target mode in Nz2ND mode is shown in the following figure, which is divided into splitting by the M dimension and splitting by the N dimension. When splitting by the M dimension, M must be a multiple of 2; when splitting by the N dimension, N must be a multiple of 32:

- nd_num = 2, indicating that the number of source Nz cubes is 2. In the figure, the red-boxed area is cube 1, and the blue-boxed area is cube 2.
- n_size = 32, indicating that the size of the source Nz cube (the red-boxed area or the blue-boxed area in the figure) in the N direction is 32 elements.
- m_size = 48, indicating that the size of the source Nz cube in the M direction is 48 elements.
- src_stride = 64, indicating the start address offset between adjacent Z layouts in the source Nz cube, that is, the interval between the start address of the light-colored Z layout cube on the left and the start address of the dark-colored Z layout cube on the right in the red-boxed area of the following figure is 64 \* C0\_Size.
- dst_stride = 64, indicating that the number of elements in each row of the destination ND cube is 64.
- src_nd_stride = 240, indicating that the interval between the start addresses of different Nz cubes is 240 \* C0\_Size.
- dst_nd_stride = 4096, indicating that the offset between the start addresses of adjacent destination ND cubes is 4096 elements.

**Figure 2** Nz2ND dual-target copy-out mode configuration diagram

![](../../../../figures/nz2nd_l0c2ub_dual_dst_c_api.png)

In the figure, `nd_num`, `src_nd_stride`, and `dst_nd_stride` correspond to `matrix_num`, `src_nz_matrix_stride`, and `dst_matrix_stride` of [asc_set_l0c_copy_nz_para](../asc_set_l0c_copy_nz_para.md), respectively, and are set to 2, 240, and 4096 in this example. `asc_copy_l0c2ub` selects the split direction through `asc_dual_dst_mode::DUAL_DST_SPLIT_M` or `DUAL_DST_SPLIT_N`. In the Nz output example, C0 is 16, and `dst_stride` is counted by destination elements.
