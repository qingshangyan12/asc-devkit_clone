# NZ2DN

<!-- md-trans-meta sourceCommit=5df1d1c781b51fcb7826db7f2afab5f8bc453e93 translatedAt=2026-10-22T12:56:55.699Z pushedAt=2026-10-28T12:45:45.428Z nl=2 -->

## Description

During the data movement process of cube computation, the core function of Nz2DN is to convert an Nz-format cube into a DN-format cube.

The Nz2DN conversion process can be referenced in the following pseudocode:

```cpp
for (int h = 0; h < matrix_num; h++) {
    src_tmp3_addr = src + h * 16 * sizeof(src_element) * src_nz_matrix_stride;
    dst_tmp3_addr = dst + h * sizeof(dst_element) * dst_matrix_stride;
    for (int i = 0; i < ((n_size + 15) / 16); i++) {
        src_tmp2_addr = src_tmp3_addr + i * sizeof(src_element) * 16 * src_stride;
        dst_tmp2_addr = dst_tmp3_addr + i * sizeof(dst_element) * 16 * dst_stride;
        for (int j = 0; j < 16; j++) {
            if (i * 16 + j < n_size) {
                src_block_addr = src_tmp2_addr + j * sizeof(src_element);
                dst_block_addr = dst_tmp2_addr + j * sizeof(dst_element) * dst_stride;
                for (int k = 0; k < m_size; k++) {
                    src_ele_addr = src_block_addr + k * sizeof(src_element) * 16 * src_nz_fractal_stride;
                    dst_ele_addr = dst_block_addr + k * sizeof(dst_element);
                }
            }
        }
    }
}
```

The parameters used in the pseudocode are described as follows:

- src indicates the source operand's start address for data movement.
- dst indicates the purpose operand's start address for data movement.
- The start address of each Nz cube of the source operand is src\_tmp3\_addr.
- The start address of each DN cube of the destination operand is dst\_tmp3\_addr.
- The source operand's Nz cube is divided into groups of 16 columns, and the start address of each group is src\_tmp2\_addr.
- The destination operand's DN cube is divided into groups of 16 rows, and the start address of each group is dst\_tmp2\_addr.
- In each group of data derived from the Nz cube above, each column of elements is further grouped as a group of data, and the start address of each group of data is src\_block\_addr.
- In each group of data derived from the DN cube above, each row of elements is further grouped as a group of data, and the start address of each group of data is dst\_block\_addr.
- src\_ele\_addr is the address of each element in a column of the Nz cube, and dst\_ele\_addr is the address of each element in a row of the DN cube. The two are in a one-to-one correspondence, which completes the Nz2DN format conversion.

The following parameters are user-configurable parameters for Nz2DN. For their meanings and value ranges, see [asc_copy_l0c2gm](../asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md):

- matrix_num indicates the number of cubes to be converted by Nz2DN.
- m_size and n_size indicate the dimensions of the cube in the m direction and n direction, respectively.
- src_stride indicates the interval between the start addresses of adjacent Z layouts in the source Nz cube, and its unit is C0\_Size.
- dst_stride indicates the number of elements in each row of the destination DN cube, and its unit is element.
- src_nz_matrix_stride indicates the interval between the start addresses of different source Nz cubes, and its unit is C0\_Size.
- dst_matrix_stride indicates the interval between the start addresses of adjacent destination DN cubes, and its unit is element.
- src_nz_fractal_stride indicates the address offset between adjacent rows of the source cube Nz fractal, and its unit is C0\_Size.

## Constraints

- This feature is unavailable when Channel Split/Channel Merge (Nz2Nz) is enabled.
- This feature is unavailable when L0C Buffer-to-Unified Buffer (UB) transfer is in dual-destination mode.
- Destination operand addresses must not overlap.

## Exampless

As shown in the following figure, there are two Nz-format cubes in the L0C Buffer. Each cube has a dimension of 32 in the N direction and 48 in the M direction. After Nz2DN conversion, the data format is changed to the layout shown in the right part.

**Figure 1** Nz2DN diagram

![](../../../../figures/fixpipe_nz2dn_c_api.png)

For this scenario, the correspondence between the parameters in the figure and the C API configuration is as follows:

- matrix_num = 2, which indicates that the number of cubes to be converted by Nz2DN is 2.
- n_size = 32, which indicates that the size of the source Nz cube in the N direction is 32 elements.
- m_size = 48, which indicates that the size of the source Nz cube in the M direction is 48 elements.
- src_stride = 80, which indicates that the interval between the start addresses of adjacent Z layouts in the source Nz cube is 80 \* C0\_Size\(C0\_Size = 16\*sizeof\(T\), where T is the data type of the operand \) bytes.
- dst_stride = 80, which indicates that the number of elements in each row of the destination DN cube is 80.
- src_nz_matrix_stride = 240, indicating that the interval between the start addresses of different source Nz cubes is 240 \* C0\_Size.
- dst_matrix_stride = 48 \* 80, indicating that the interval between the start addresses of adjacent destination DN cubes is 48 \* 80 = 3840 elements, where 48 is the number of rows between the start addresses of destination cubes and 80 is dst_stride (the number of elements per row).
- src_nz_fractal_stride = 1: indicates that the address offset between adjacent rows of the source cube Nz fractal is 1 \* C0\_Size.
