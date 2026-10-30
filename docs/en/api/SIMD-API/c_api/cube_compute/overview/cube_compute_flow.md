# Dense Cube Computation Flow

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T03:10:05.852Z pushedAt=2026-10-28T12:21:43.297Z nl=2 -->

The C API provides data load, cube computation, and result store APIs for the cube computation programming model. These APIs carry the data movement and computation capabilities of each path in the Cube core, as shown in the following figure:

**Figure 1** Dense cube basic computation flowchart  
![](../../../../figures/matrix_computation_workflow_c_api.png "Dense cube basic computation flowchart")

1. Load the pre-arranged cubes A and B through [asc_copy_gm2l1](../../cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1.md). For ND inputs, use [asc_copy_gm2l1_nd2nz](../../cube_datamove/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz.md) to complete the fractal conversion. Bias and the in-line quantization coefficient are also loaded into the L1 buffer first.

2. Load A and B into the L0A buffer and L0B buffer through [asc_copy_l12l0a](../../cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a.md) and [asc_copy_l12l0b](../../cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b.md), respectively. Select the transpose variant based on the input layout. Bias and the quantization coefficient are loaded into the corresponding buffers through [asc_copy_l12bt](../../cube_datamove/asc_copy_l12bt/asc_copy_l12bt.md) and [asc_copy_l12fb](../../cube_datamove/asc_copy_l12fb/asc_copy_l12fb.md), respectively.

3. Perform computation through [asc_mmad](../asc_mmad.md) and output the result to the L0C buffer. The parameter order is `m, k, n`. For the first computation from zero, set `c_matrix_init_val=true`. For subsequent K-block accumulation, set `c_matrix_source=false` and `c_matrix_init_val=false`. For the parameters of Bias initialization, see [asc_mmad](../asc_mmad.md).

4. Store the result through [asc_copy_l0c2gm](../../cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm.md), configuring the in-line quantization, activation, and output format. The result can also be moved to the L1 buffer. See [Overall Store Description](../../cube_datamove/cube_compute_store_overall_description.md).

<!-- npu="950" id1 -->
# Mx Cube Computation Flow

Mx cube computation is supported only on Ascend 950PR&950DT products.

MX cube multiplication (microscaling cube multiply-accumulate) is cube multiplication with quantization coefficients. Both the left cube and the right cube have corresponding quantization coefficient cubes: the left quantization coefficient cube scaleA and the right quantization coefficient cube scaleB. In the MX cube multiplication scenario, the left quantization coefficient cube is multiplied by the left cube, the right quantization coefficient cube is multiplied by the right cube, and cube multiplication is performed on the two products. The C API supports MX cube computation through asc_mmad_mx and independent data/coefficient load APIs, as shown in the following figure:

**Figure 2** Mx basic computation flowchart  
![](../../../../figures/mx_matrix_computation_workflow_c_api.png "Mx basic cube computation flowchart")

1. Load A, B, and the [scaleA/scaleB coefficient cubes](../cube_compute_fractal_intro/aux_cube_fractal_format_details.md) from GM to the L1 Buffer. The A/B data fractal differs from the coefficient fractal, so configure their layouts, sizes, and strides separately.

2. Use [asc_copy_l12l0a](../../cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_2d_arch_3510.md) and [asc_copy_l12l0b](../../cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_2d_arch_3510.md) to load A/B; additionally call [asc_copy_l12l0a_mx](../../cube_datamove/asc_copy_l12l0a_mx.md) and [asc_copy_l12l0b_mx](../../cube_datamove/asc_copy_l12l0b_mx.md) to load scaleA/scaleB.

3. After the data, coefficients, and optional Bias are ready, compute through [asc_mmad_mx](../asc_mmad_mx.md) and output the result to the L0C Buffer.

4. Use [asc_copy_l0c2gm](../../cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md) to process and store the result.
<!-- end id1 -->

<!-- npu="910b,A3" id2 -->
# 4-choose-2 Sparse Cube Computation Flow

A sparse cube is a special type of cube that contains many zero elements. 4-choose-2 structured sparse cube computation requires that in a group of four consecutive weights or activation values (usually a row or column in a tensor), at most two values are non-zero, and the remaining two are forced to zero. For Atlas A2 products and Atlas A3 products, Ascend C provides corresponding sparse APIs for the 4-choose-2 sparse cube computation programming model, as shown in the following figure:

**Figure 3** 4-choose-2 sparse cube basic computation flowchart  
![](../../../../figures/sparse_matrix_computation_workflow_c_api.png "4-choose-2 sparse cube basic computation flowchart")

1. First, divide the original cube B into groups of four consecutive elements at a fixed granularity, and generate the corresponding compressed cube B and [index cube](../cube_compute_fractal_intro/aux_cube_fractal_format_details.md#4-select-2-sparse-index-cube) offline as inputs according to the 4-choose-2 structured sparse compression rule.

2. Use the asc_copy_gm2l1 API to load the GM data of the original cube A, the compressed cube B, and the index cube into the L1 Buffer (if quantization is present, it is also loaded into the L1 Buffer through asc_copy_gm2l1).

3. Use the asc_copy_l12l0a API to load the original cube A into the L0A Buffer, and use the [asc_copy_l12l0b_sparse API](../../cube_datamove/asc_copy_l12l0b_sparse.md) to load the compressed cube B and the index cube into the L0B Buffer and the built-in IDX Buffer respectively for computation (if quantization is present, use asc_copy_l12fb to load the quantization coefficient data from the L1 Buffer into the Fixpipe Buffer).

4. Use the asc_mmad_sparse API to perform 4-choose-2 sparse cube computation on the data in the L0A Buffer, L0B Buffer, and built-in IDX Buffer, and output the result to the L0C Buffer. For details, see [asc_mmad_sparse computation](../asc_mmad_sparse.md).

5. Use the asc_copy_l0c2gm API to process the data in the L0C Buffer and store it to GM. The asc_copy_l0c2gm API can use the Fixpipe Buffer data to perform operations such as quantization and ReLU. For details, see [asc_copy_l0c2gm (L0C Buffer to GM data transfer)](../../cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm.md).
<!-- end id2 -->

# Conv2D Forward Propagation Flow

The essence of Conv2D forward propagation is to convert the convolution operation into cube multiplication: first, img2col (Image to Column) expands the FeatureMap into a left cube A according to the convolution kernel sliding window, uses the Weight as the right cube B, and then performs cube multiply-accumulate to obtain the output. The difference from the [dense cube computation flow](#dense-cube-computation-flow) is that after cube A is loaded directly from GM, [asc_copy_l12l0a's 3D reload](../../cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a.md) performs the img2col spatial expansion online during the transfer from the L1 Buffer to the L0A Buffer, based on the convolution kernel size, stride, dilation, and padding, as shown in the following figure:

**Figure 4** Conv2D forward propagation flowchart  
![](../../../../figures/conv2d_forward_c_api.png "Conv2D forward propagation flowchart")

1. Load the FeatureMap and Weight from GM into the L1 Buffer. The FeatureMap is prepared in the [NC1HWC0 format](../cube_compute_fractal_intro/key_fractal_format_details.md#nc1hwc0-format); the Weight is prepared according to the layout required for the subsequent L0B Buffer transfer.

2. Use the 3D reload of `asc_copy_l12l0a` to perform img2col, and load the Weight through `asc_copy_l12l0b`. Before the call, configure the FeatureMap attributes, repeat stride, boundary, and padding. For details, see [Load Overview](../../cube_datamove/cube_compute_load_overall_description.md).

3. Use [asc_mmad](../asc_mmad.md) to complete the cube multiply-add, with the result stored in the L0C Buffer.

4. Use the [Nz2ND](../../cube_datamove/cube_store_key_features/NZ2ND.md) capability of `asc_copy_l0c2gm` to output the convolution result as NHWC. For dimension mapping, see [Convolution Format Conversion](../../cube_datamove/cube_store_key_features/convolution_format_conversion.md).<!-- npu="950" id3 -->In particular, for Ascend 950PR&950DT products, you can also use [Nz2DN](../../cube_datamove/cube_store_key_features/NZ2DN.md) to output the convolution result in NCHW format.<!-- end id3 -->
