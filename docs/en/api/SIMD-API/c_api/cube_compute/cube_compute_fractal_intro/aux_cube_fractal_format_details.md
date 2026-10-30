# Auxiliary Cube Fractal Format in Detail

<!-- npu="950" id1 -->
<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T03:02:11.894Z pushedAt=2026-10-28T12:21:43.285Z nl=2 -->

## MX Coefficient Cube

For Ascend 950PR&950DT products:

Floating-point numbers are widely used in scientific computing, image processing, neural networks, and other fields. Taking AI training as an example, existing floating-point formats either have an insufficient value range or low precision, which affects the convergence speed and performance of models. Meeting both the value range and precision requirements simultaneously would lead to excessive memory usage, thereby increasing the cost of data storage and transmission. To address this, the industry has proposed a new floating-point format: the Microscaling (MX) format. Floating-point numbers in the MX format support AI training and inference at lower bit widths while occupying less memory. Data formats conforming to the MX standard can achieve robust AI training and inference model accuracy when using 8-bit or lower bit widths.

The MX format is a block data format. Multiple data elements can form a block (or a group), and data is organized in units of blocks. Data in the MX format consists of three parts:

- A shared scale factor X with a bit width of w bits;
- Private elements P<sub>i</sub> with a bit width of d bits;
- A block size k, which indicates how many low-bit data elements form a block;

All k elements P<sub>i</sub> have the same bit width and data type and share a single scale factor X. Each block containing k elements can be encoded using (w+k\*d) bits. The data type of the elements and the scale factor can be selected independently.

The following figure shows the data structure of floating-point numbers in the MX format, where S, E, and M represent the sign, exponent, and mantissa field values of a floating-point number, respectively. The shared scale factor X is a scaling factor applied to the entire data block, and it determines the dynamic range of all elements in the block. By introducing a shared scale factor, data in the MX format can flexibly represent data of different ranges while maintaining a low bit width. The block size k refers to the number of low-bit data elements that form a data block (or group). The private element P<sub>i</sub> refers to each low-bit data element in the data block. After being adjusted by the scale factor X, these elements jointly represent a high-precision floating-point number or integer.

**Figure 1** MX format composition diagram  
![](../../../../figures/mx_format.png "MX format composition diagram")

MX cube multiplication is cube multiplication with quantization coefficients X, meaning that both the left cube and the right cube have corresponding quantization coefficient matrices: the left quantization coefficient cube scaleA and the right quantization coefficient cube scaleB. The calculation formula is: C = \(scaleA ⊗ A\) \* \(scaleB ⊗ B\) + Bias, where "⊗" denotes broadcast multiplication. When the left/right cube is multiplied with the left/right quantization coefficient cube, every 32 elements along the K dimension share one quantization factor. As shown in the following figure, A is the left cube with shape \[M, K\]; scaleA is the left quantization coefficient cube with shape \[M, K/32\]; B is the right cube with shape \[K, N\]; scaleB is the right quantization coefficient cube with shape \[K/32, N\].

**Figure 2** MX cube multiplication diagram  
![](../../../../figures/mx_matrix_computation_demo.png "MX cube multiplication diagram")

- **Introduction to the MX scaleA coefficient cube fractal**

    - **Physical location:** Global Memory/L1 Buffer/Unified Buffer (UB): stores the left cube quantization coefficient data generated offline/online.

    - **Design principle:** In MX cube multiplication, cube A and the scaleA cube are mapped one-to-one in physical addresses. The asc_copy_l12l0a_mx/asc_copy_l12l0b_mx (coefficient copy) API requires that the scaleA cube in the L1 Buffer satisfy the row-read requirement. Therefore, in the L1 Buffer, the scaleA cube uses a small-z large-Z layout (Zz) stored by rows.

    - **Format conversion process:** Assume the original A cube has size (M, K). The corresponding scaleA cube then has size (M, K/32). The scaleA cube is divided into M1 \* K1 fractals, arranged in row-major order with a Z-shaped pattern. Each fractal contains M0 \* K0 elements, arranged in row-major order with a Z-shaped pattern, which is why this data format is called the Zz format. Here, \(M0, K0\) represents the shape of one fractal, which must be consistent with the physical memory layout of the L0A_MX Buffer, and the data type must be fp8_e8m0_t with a shape of 16x2.

        The conversion process is expressed by the following formula:

        ```text
        (…, B, M, K/32)->pad->(…, B, M1 * M0, K1 * K0)->reshape->(…, B, M1, M0, K1, K0)->transpose->(…, B, M1, K1, M0, K0)
        ```

    - **Fractal size constraint:** The data type is fp8\_e8m0\_t, and the fractal size is 16x2 (M0, K0).

    As shown in the following figure, if the scaleA cube on GM uses the ND layout, it can be transferred to the L1 Buffer using the ND2Nz.b16/DN2Nz.b16 instructions to ensure that the fractal in the L1 Buffer uses the Zz layout, satisfying the requirements of the asc_copy_l12l0a_mx/asc_copy_l12l0b_mx (coefficient copy) API.

    **Figure 3** Layout format of the scaleA cube at different locations  
    ![](../../../../figures/mx_scaleA_format.png "Layout format of the scaleA cube at different locations")

- **Introduction to the MX scaleB Coefficient Cube Fractal**

    - **Physical location:** Global Memory/L1 Buffer/UB: Stores the offline/online generated right cube quantization coefficient data.

    - **Design principle:** In MX cube multiplication, the B cube and the scaleB cube are mapped one-to-one in physical addresses. The asc_copy_l12l0a_mx/asc_copy_l12l0b_mx (coefficient copy) APIs require that the scaleB cube on the L1 Buffer satisfy the column-read requirement. Therefore, on the L1 Buffer, the scaleB cube uses a column-major layout with a large N and small n (Nn).

    - **Format conversion process:** Assume that the original B cube has a size of (K, N). The corresponding scaleB cube has a size of (K/32, N). The scaleB cube is divided into K1 \* N1 fractals, arranged in column-major order, with a shape resembling the letter N. Each fractal contains K0 \* N0 elements, arranged in column-major order, with a shape resembling the letter n. Therefore, this data format is called the Nn format. Here, (K0, N0) represents the shape of a fractal. The fractal must be consistent with the physical memory layout of the L0B_MX Buffer, and the data type must be fp8_e8m0_t, with a shape of 2x16.

        The conversion process is expressed by the following formula:

        ```text
        (…, B, K/32, N)->pad->(…, B, K1 * K0, N1 * N0)->reshape->(…, B, K1, K0, N1, N0)->transpose->(…, B, N1, K1, N0, K0)
        ```

    - **Fractal size constraint:** The data type is fp8\_e8m0\_t, and the fractal size is 2x16 (K0, N0).

    As shown in the following figure, if the scaleB cube on the Global Memory (GM) uses the ND layout, it can be copied to the L1 Buffer using the ND2Nz.b16/DN2Nz.b16 instructions to ensure that the fractal on the L1 Buffer uses the Nn layout, satisfying the requirements of the asc_copy_l12l0a_mx/asc_copy_l12l0b_mx (coefficient copy) APIs.

    **Figure 4** Layout format of the scaleB cube at different locations  
    ![](../../../../figures/mx_scaleB_format.png "Layout format of the scaleB cube at different locations")

For the coefficient copy APIs, see [asc_copy_l12l0a_mx](../../cube_datamove/asc_copy_l12l0a_mx.md) and [asc_copy_l12l0b_mx](../../cube_datamove/asc_copy_l12l0b_mx.md).
<!-- end id1 -->

<!-- npu="A3,910b" id2 -->
## 4-Select-2 Sparse Index Cube

For Atlas A2 products and Atlas A3 products:

- Definition: The index cube generated during the densification of the original B cube. Assuming that every 4 elements of the original B cube contain at least 2 zero elements, the densified B cube is a dense cube with the 2 zeros filtered out.
- Scenario: It typically resides in Global Memory (GM). Before performing sparse cube operations, it usually needs to be generated offline and converted to the large-Z small-n format (Zn) to fit the Cube unit.
- Generation process: As shown in the following figure, the index cube is generated during the densification process. Assuming that every 4 elements of the original B cube contain at least 2 zero elements, the densified B cube is a dense cube with the 2 zeros filtered out. For every 4 elements in the original cube B, two 2-bit indexes are generated in the index cube and encoded according to the following rules. The indexes must be within the range of \{0, 1, 2\}.

    **Figure 5** Index generation diagram  
    ![](../../../../figures/index_generation.png "Index generation diagram")
    - The first index indicates the relative position of the first non-zero element among the first 3 elements.
    - The second index indicates the relative position of the second non-zero element among the last 3 elements.
    - One uint8 element in the index cube is composed of four uint2\_t values. For example, in the figure above, the generated index data is 1 2 0 1, and the layout of the index data in the Global Memory (GM) address from high to low bits is 1 0 2 1, where 1 0 2 1 (corresponding to the first 4 bits 1 2 0 1 of the index cube) is one uint8\_t.

**Figure 6** Densified B cube diagram  
![](../../../../figures/density_B_matrix_demo.png "Densified B cube diagram")

Here, "densification" refers to 4-to-2 compression: for every four original elements, two storage slots are retained and the position index is recorded. The densified B cube mentioned below is the compressed B cube. For specific rules, see the following table, where "-" indicates that the value at that position is not of concern because it is filtered out during the subsequent Sparse asc_mmad computation.

<table><thead>
<tr><th>Example</th><th>B cube element 0</th><th>B cube element 1</th><th>B cube element 2</th><th>B cube element 3</th><th>Index data 0</th><th>Index data 1</th>
</tr></thead><tbody>
<tr><td rowspan="6">Two of the four elements are non-zero</td>
<td>0</td><td>0</td><td>X</td><td>Y</td><td>2'b10</td><td>2'b10</td></tr>
<tr><td>0</td><td>X</td><td>0</td><td>Y</td><td>2'b01</td><td>2'b10</td></tr>
<tr><td>X</td><td>0</td><td>0</td><td>Y</td><td>2'b00</td><td>2'b10</td></tr>
<tr><td>0</td><td>X</td><td>Y</td><td>-</td><td>2'b01</td><td>2'b01</td></tr>
<tr><td>X</td><td>0</td><td>Y</td><td>-</td><td>2'b00</td><td>2'b01</td></tr>
<tr><td>X</td><td>Y</td><td>-</td><td>-</td><td>2'b00</td><td>2'b00</td></tr>
<tr><td rowspan="4">One nonzero element among the four elements</td>
<td>0</td><td>0</td><td>0</td><td>X</td><td>2'b00</td><td>2'b10</td></tr>
<tr><td>0</td><td>0</td><td>X</td><td>0</td><td>2'b10</td><td>2'b00</td></tr>
<tr><td>0</td><td>X</td><td>0</td><td>0</td><td>2'b01</td><td>2'b00</td></tr>
<tr><td>X</td><td>0</td><td>0</td><td>0</td><td>2'b00</td><td>2'b00</td></tr>
<tr><td>No non-zero elements among the four elements</td>
<td>0</td><td>0</td><td>0</td><td>0</td><td>2'b00</td><td>2'b00</td>
</tr></tbody></table>

- **Physical location:** GM/L1 Buffer: Stores the index cube data generated offline.

- **Design principle:** Column read requirement: In sparse cube multiplication, the B cube and the Index cube are mapped one-to-one by physical address. The asc_copy_l12l0b_sparse instruction requires that the B cube and the Index cube in the L1 Buffer satisfy the column read requirement. Therefore, the Index cube in the GM/L1 Buffer uses a large-Z small-n layout stored by column (Zn).

- **Format conversion process:** Assume the original B cube size is (K, N). After 4-to-2 densification, the B cube and the Index cube have a size of (K/2, N). The Index cube is divided into K1 \* N1 fractals, arranged in row-major order with a Z-shaped pattern. Each fractal contains N0 \* K0 elements, arranged in column major order with an N-shaped pattern, which is why this data format is called the Zn format. Here, \(N0, K0\) represents the shape of a fractal, which must be consistent with the B cube fractal. The B cube data type is int8\_t, and its small fractal shape is 16\*32. Each uint8\_t data in the Index cube fractal is formed by concatenating four uint2\_t elements, whose addresses correspond one-to-one with the B cube elements. Therefore, the fractal shape is also 16\*32, but the size is 128 bytes.

    The conversion process is expressed by the following formula:

    ```text
    (…, B, K/2, N)->pad->(…, B, K1 * K0, N1 * N0)->reshape->(…, B, K1, K0, N1, N0)->transpose->(…, B, K1, N1, N0, K0)
    ```

- **Fractal size constraint:** When stored as the uint8 data type, the fractal size is 16x8 (N0, K0).
<!-- end id2 -->
