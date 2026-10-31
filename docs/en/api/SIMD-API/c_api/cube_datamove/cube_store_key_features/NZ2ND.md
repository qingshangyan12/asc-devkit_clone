# NZ2ND

<!-- md-trans-meta sourceCommit=5df1d1c781b51fcb7826db7f2afab5f8bc453e93 translatedAt=2026-10-22T12:57:50.776Z pushedAt=2026-10-28T12:45:45.433Z nl=2 -->

## Description

When a cube is moved out, the Nz2ND function converts the cube from the Nz format to the ND format.

The Nz2ND conversion process can be referenced from the following pseudocode (all addresses are counted in bytes, and src_matrix_stride_64b is a unified 64B stride notation used to uniformly describe the source cube interval):

```cpp
for (i = 0; i < matrix_num; i++) {
    // fractal_size = 16 * sizeof(L0C_element)
    src_temp_nd_addr = src + src_matrix_stride_64b * fractal_size * i;
    dst_temp_nd_addr = dst + dst_matrix_stride * sizeof(dst_element) * i;
    for (j = 0; j < m_size; j++) {
        src_temp_n_addr = src_temp_nd_addr + j * 16 * sizeof(L0C_element);
        dst_temp_n_addr = dst_temp_nd_addr + j * dst_stride * sizeof(dst_element);
        for (k = 0; k < ((n_size + 15) / 16); k++) {
            src_block_addr = src_temp_n_addr + k * src_stride * 16 * sizeof(L0C_element);
            dst_block_addr = dst_temp_n_addr + k * 16 * sizeof(dst_element);
        }
    }
}
```

The parameters used in the pseudocode are described as follows:

- src indicates the start address of the source operand move.
- dst indicates the start address of the destination operation move.
- The start address of each Nz cube in the source operand is src\_temp\_nd\_addr.
- The start address of each ND cube in the destination operation is dst\_temp\_nd\_addr.
- The start address of each row in each Nz cube in the source operand is src\_temp\_n\_addr.
- The start address of each row in each ND cube in the destination operation is dst\_temp\_n\_addr.
- In each row, 16 data elements form one data block. The start address of each data block in the source operand is src\_block\_addr, and the start address of each data block in the destination operand is dst\_block\_addr.

The following parameters are user-configurable parameters for Nz2ND. For their meanings and value ranges, see [C API copy-out parameters](../asc_copy_l0c2gm/asc_copy_l0c2gm.md):

- m_size and n_size indicate the cube dimensions in the m direction and n direction, respectively.
- src_stride indicates the interval between different Z fractal columns in the Nz cube, in C0\_Size.
- dst_stride indicates the interval between different rows in the destination ND cube, in elements.
- matrix_num indicates the number of cubes to be converted by Nz2ND.
- src_matrix_stride_64b indicates the interval between different Nz cubes, in 64B, that is, 16×sizeof(L0C_element).
- dst_matrix_stride indicates the interval between different destination ND cubes, in elements.

## Constraints

- This feature is unavailable when Channel Split/Channel Merge (Nz2Nz) is enabled.
- Destination operand addresses must not overlap.

## Exampless

As shown in the following figure, there are two cubes in Nz format in the L0C Buffer. Each cube has a dimension of 32 in the N direction and 48 in the M direction. After Nz2ND conversion, the data format is changed to the layout shown in the right part.

**Figure 1** Nz2ND diagram

![](../../../../figures/fixpipe_nz2nd_c_api.png)

For this scenario, the correspondence between the parameters in the figure and the C API configuration is as follows:

- matrix_num = 2, indicating that the number of cubes to be converted by Nz2ND is 2.
- n_size = 32, indicating that the size of the source Nz cube in the N direction is 32 elements.
- m_size = 48, indicating that the size of the source Nz cube in the M direction is 48 elements.
- src_stride = 64, indicating that the interval between the start addresses of adjacent Z layouts in the source Nz cube is 64 \* C0\_Size\(C0\_Size = 16\*sizeof\(T\), where T is the data type of the operand \) bytes.
- dst_stride = 64, indicating that the number of elements in each row of the destination ND cube is 64.
- src_matrix_stride_64b = 256, indicates different Nz cube start addresses are spaced 256 \* C0\_Size bytes apart.
- dst_matrix_stride = 4096, indicates the destination adjacent ND cube start addresses are offset by 4096 elements.
