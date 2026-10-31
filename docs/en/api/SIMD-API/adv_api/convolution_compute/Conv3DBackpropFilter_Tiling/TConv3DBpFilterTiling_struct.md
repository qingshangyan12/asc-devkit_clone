# TConv3DBpFilterTiling Struct

<!-- md-trans-meta sourceCommit=ca3b72f6b4f376702407e982cf2cc174087341f5 translatedAt=2026-10-26T09:37:31.694Z pushedAt=2026-10-29T02:14:34.306Z nl=2 -->

The TConv3DBpFilterTiling struct contains the specification information of the Conv3dBackpropFilter operator and the related parameters of the tiling algorithm. It is passed to the Conv3dBackpropFilter kernel for data tiling, data movement, and computation. For the parameter description of the TConv3DBpFilterTiling struct, see [Table 1](#table1563162142915).

You can obtain the TConv3DBpFilterTiling struct by calling the [GetTiling](GetTiling.md) API. For details, see [Usage Guidelines](Conv3DBackpropFilter_Tiling_usage.md). Currently, custom configuration of the parameters in the TConv3DBpFilterTiling struct is not supported.

**Table 1**  TConv3DBpFilterTiling struct description

<a name="table1563162142915"></a>

| Parameter name | Description |
| --- | --- |
| batch | Batch of the input **GradOutput**, in elements. |
| cin | Channel of the input **Input**, in elements. |
| cout | Channel of the input **GradOutput**, in elements. |
| cin1G | Reserved parameter, not perceivable by you. |
| cout1G | Reserved parameter. No user awareness required. |
| dout | Depth of the input **GradOutput**, in elements. |
| ho | Height of the input **GradOutput**, in elements. |
| wo | Width of the input **GradOutput**, in elements. |
| di | Depth of the input **Input**, in elements. |
| hi | Height of the input **Input**, in elements. |
| wi | Width of the input **Input**, in elements. |
| dk | Depth of the output **Weight**, in elements. |
| hk | Height of the output **Weight**, in elements. |
| wk | Width of the output **Weight**, in elements. |
| group | Reserved parameter. No user awareness required. |
| strideD | Depth of the stride in convolution backward computation, in elements. |
| strideH | Height of the stride in convolution backward computation, in elements. |
| strideW | Width of the stride in convolution backward computation, in elements. |
| padFront | Front direction of the padding in the depth dimension in convolution backward computation, in elements. |
| padBack | Back direction of the padding in the depth dimension in convolution backward computation, in elements. |
| padUp | Up direction of the padding in the height dimension in convolution backward computation, in elements. |
| padDown | Down direction of the padding in the height dimension in convolution backward computation, in elements. |
| padLeft | Left direction of the padding in the width dimension in convolution backward computation, in elements. |
| padRight | Right direction of the padding in the width dimension in convolution backward computation, in elements. |
| dilationD | Depth of the dilation in convolution backward computation, in elements. |
| dilationH | Height of the dilation in convolution backward computation, in elements. |
| dilationW | Width of the dilation in convolution backward computation, in elements. |
| channelSize | Size of C0 under the current input data type. Currently, this parameter supports only the value 16. |
| al0Pbuffer | 1 indicates that **DoubleBuffer** is disabled, and 2 indicates that DoubleBuffer is enabled. |
| bl0Pbuffer | 1 indicates that **DoubleBuffer** is disabled, and 2 indicates that DoubleBuffer is enabled. |
| cl0Pbuffer | 1 indicates that **DoubleBuffer** is disabled, and 2 indicates that DoubleBuffer is enabled. |
| al1Pbuffer | 1 indicates that **DoubleBuffer** is disabled, and 2 indicates that DoubleBuffer is enabled. |
| bl1Pbuffer | 1 indicates that **DoubleBuffer** is disabled, and 2 indicates that DoubleBuffer is enabled. |
| baseM | Size in the M direction in L0, in elements. |
| baseK | Size in the K direction in L0, in elements. |
| baseN | Size in the N direction in L0, in elements. |
| m0 | Minimum fractal size in the M direction in L0. |
| k0 | Minimum fractal size in the K direction in L0. |
| n0 | Minimum fractal size in the N direction in L0. |
| stepM | Multiple of baseM in the M direction of the buffer where the matrix is cached in L1. |
| stepN | Multiple of baseN in the N direction of the buffer where the matrix is cached in L1. |
| stepKa | Multiple of baseK in the K direction of the buffer where the matrix is cached in L1. |
| stepKb | Multiple of baseK in the K direction of the buffer where the matrix is cached in L1. |
| iterateOrder | Reserved parameter. No user awareness required. |
| bl1Bound | Maximum data volume of the GradOutput matrix loaded into L1. |
| hf32Flag | Reserved parameter. No user awareness required. |
| singleCoreDK | Reserved parameter. No user awareness required. |
| singleCoreGroup | Reserved parameter. No user awareness required. |
| singleCoreCout | Size of the cout data volume calculated in the M direction on a single core, in elements. |
| singleCoreHo | Size of the ho data volume calculated in the K direction on a single core, in elements. |
| singleCoreBatch | Size of batch on a single core, in elements. |
| singleCoreCin | Size of the cin data volume calculated in the N direction on a single core, in elements. |
| totalL1Size | Size of L1, in elements. |
| singleCoreM | Size of M on a single core, in elements. |
| singleCoreN | Size of N on a single core, in elements. |
| singleCoreK | Size of K on a single core, in elements. |
